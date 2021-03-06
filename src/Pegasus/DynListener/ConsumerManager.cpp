//%2005////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000, 2001, 2002 BMC Software; Hewlett-Packard Development
// Company, L.P.; IBM Corp.; The Open Group; Tivoli Systems.
// Copyright (c) 2003 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation, The Open Group.
// Copyright (c) 2004 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation; VERITAS Software Corporation; The Open Group.
// Copyright (c) 2005 Hewlett-Packard Development Company, L.P.; IBM Corp.;
// EMC Corporation; VERITAS Software Corporation; The Open Group.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// THE ABOVE COPYRIGHT NOTICE AND THIS PERMISSION NOTICE SHALL BE INCLUDED IN
// ALL COPIES OR SUBSTANTIAL PORTIONS OF THE SOFTWARE. THE SOFTWARE IS PROVIDED
// "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//==============================================================================
//
// Author: Heather Sterling (hsterl@us.ibm.com)
//
// Modified By: 
//
//%/////////////////////////////////////////////////////////////////////////////

#include <Pegasus/Common/Config.h>
//#include <cstdlib>
//#include <dlfcn.h>
#include <Pegasus/Common/System.h>
#include <Pegasus/Common/FileSystem.h>
#include <Pegasus/Common/Tracer.h>
#include <Pegasus/Common/Logger.h>
#include <Pegasus/Common/XmlReader.h>
#include <Pegasus/Common/XmlParser.h>
#include <Pegasus/Common/XmlWriter.h>
#include <Pegasus/Common/IPC.h>

#include "ConsumerManager.h"

PEGASUS_NAMESPACE_BEGIN
PEGASUS_USING_STD;

//ATTN: Can we just use a properties file instead??  If we only have one property, we may want to just parse it ourselves.
// We may need to add more properties, however.  Potential per consumer properties: unloadOk, idleTimout, retryCount, etc
static struct OptionRow optionsTable[] =
//optionname defaultvalue rqd  type domain domainsize clname hlpmsg
{
{"location", "", false, Option::STRING, 0, 0, "location", "library name for the consumer"},
};

const Uint32 NUM_OPTIONS = sizeof(optionsTable) / sizeof(optionsTable[0]);

//retry settings
static const Uint32 DEFAULT_MAX_RETRY_COUNT = 5;
static const Uint32 DEFAULT_RETRY_LAPSE = 300000;  //ms = 5 minutes



ConsumerManager::ConsumerManager(const String& consumerDir, const String& consumerConfigDir, Boolean enableConsumerUnload, Uint32 idleTimeout) : 
_consumerDir(consumerDir),
_consumerConfigDir(consumerConfigDir),
_enableConsumerUnload(enableConsumerUnload),
_idleTimeout(idleTimeout),
_forceShutdown(true)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::ConsumerManager");

    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Consumer library directory: " + consumerDir);
    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Consumer configuration directory: " + consumerConfigDir);

    Tracer::trace(__FILE__,__LINE__,TRC_LISTENER,Tracer::LEVEL4,
                  "Consumer unload enabled %d: idle timeout %d",
                  enableConsumerUnload,
                  idleTimeout);


    //ATTN: Bugzilla 3765 - Uncomment when OptionManager has a reset capability
    //_optionMgr.registerOptions(optionsTable, NUM_OPTIONS);

    struct timeval deallocateWait = {15, 0};
    _thread_pool = new ThreadPool(0, "ConsumerManager", 0, 0, deallocateWait);

    _init();

    PEG_METHOD_EXIT();
}

ConsumerManager::~ConsumerManager()
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::~ConsumerManager");

    unloadAllConsumers();

    if (_thread_pool != NULL)
    {
    delete _thread_pool;
    }

    ConsumerTable::Iterator i = _consumers.start();
    for (; i!=0; i++)
    {
        DynamicConsumer* consumer = i.value();
        delete consumer;
    }

    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Deleted all consumers");

    ModuleTable::Iterator j = _modules.start();
    for (;j!=0;j++)
    {
        ConsumerModule* module = j.value();
        delete module;
    }

    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Deleted all modules");

    PEG_METHOD_EXIT();
}

void ConsumerManager::_init()
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::_init");

    //check if there are any outstanding indications
    Array<String> files;
    Uint32 pos;
    String consumerName;

    if (FileSystem::getDirectoryContents(_consumerConfigDir, files))
    {
        for (Uint32 i = 0; i < files.size(); i++)
        {
            pos = files[i].find(".dat");
            if (pos != PEG_NOT_FOUND)
            {
                consumerName = files[i].subString(0, pos);

                try
                {
                    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Attempting to load indication for!" + consumerName + "!");
                    getConsumer(consumerName);

                } catch (...)
                {
                    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Cannot load consumer from file " + files[i]);
                }
            }
        }
    }

    PEG_METHOD_EXIT();
}

String ConsumerManager::getConsumerDir()
{
    return _consumerDir;
}

String ConsumerManager::getConsumerConfigDir()
{
    return _consumerConfigDir;
}

Boolean ConsumerManager::getEnableConsumerUnload()
{
    return _enableConsumerUnload;
}

Uint32 ConsumerManager::getIdleTimeout()
{
    return _idleTimeout;
}

/** Retrieves the library name associated with the consumer name.  By default, the library name
  * is the same as the consumer name.  However, you may specify a different library name in a consumer
  * configuration file.  This file must be named "MyConsumer.txt" and contain the following:
  *     location="libraryName"
  *
  * The config file is optional and is generally only needed in cases where there are strict requirements
  * on library naming.
  *
  * It is the responsibility of the caller to catch any exceptions thrown by this method.
  */
String ConsumerManager::_getConsumerLibraryName(const String & consumerName)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::getConsumerLibraryName");

    //default library name is consumer name
    String libraryName = consumerName;

    //check whether an alternative library name was specified in an optional consumer config file
    String configFile = FileSystem::getAbsolutePath((const char*)_consumerConfigDir.getCString(), String(consumerName + ".conf"));
    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Looking for config file " + configFile);

    if (FileSystem::exists(configFile) && FileSystem::canRead(configFile))
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Found config file for consumer " + consumerName);

        try
        {
            //Bugzilla 3765 - Change this to use a member var when OptionManager has a reset option
            OptionManager _optionMgr;
            _optionMgr.registerOptions(optionsTable, NUM_OPTIONS); //comment this line out later
            _optionMgr.mergeFile(configFile);
            _optionMgr.checkRequiredOptions();

            if (!_optionMgr.lookupValue("location", libraryName) || (libraryName == String::EMPTY))
            {
                PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Warning: Using default library name since none was specified in " + configFile); 
                libraryName = consumerName;
            }

        } catch (Exception & ex)
        {
            throw Exception(MessageLoaderParms("DynListener.ConsumerManager.INVALID_CONFIG_FILE",
                                               "Error reading $0: $1.",
                                               configFile,
                                               ex.getMessage()));
        }
    } else
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "No config file exists for " + consumerName);
    }

    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "The library name for " + consumerName + " is " + libraryName);

    PEG_METHOD_EXIT();
    return libraryName;
}

/** Returns the DynamicConsumer for the consumerName.  If it already exists, we return the one in the cache.  If it
 *  DNE, we create it and initialize it, and add it to the table.
 * @throws Exception if we cannot successfully create and initialize the consumer
 */ 
DynamicConsumer* ConsumerManager::getConsumer(const String& consumerName)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::getConsumer");

    DynamicConsumer* consumer = 0;
    CIMIndicationConsumerProvider* consumerRef = 0;
    Boolean cached = false;
    Boolean entryExists = false;

    AutoMutex lock(_consumerTableMutex);

    if (_consumers.lookup(consumerName, consumer))
    {
        //why isn't this working??
        entryExists = true;

        if (consumer && consumer->isLoaded())
        {
            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL3, "Consumer exists in the cache and is already loaded: " + consumerName);
            cached = true;
        }
    } else
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL3, "Consumer not found in cache, creating " + consumerName);
        consumer = new DynamicConsumer(consumerName);
        //ATTN: The above is a memory leak if _initConsumer throws an exception
        //need to delete it in that case
    }

    if (!cached)
    {
        _initConsumer(consumerName, consumer);

        if (!entryExists)
        {
            _consumers.insert(consumerName, consumer);
        }
    }

    consumer->updateIdleTimer();

    PEG_METHOD_EXIT();
    return consumer;
}

/** Initializes a DynamicConsumer.
 * Caller assumes responsibility for mutexing the operation as well as ensuring the consumer does not already exist.
 * @throws Exception if the consumer cannot be initialized
 */
void ConsumerManager::_initConsumer(const String& consumerName, DynamicConsumer* consumer)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::_initConsumer");

    CIMIndicationConsumerProvider* base = 0;
    ConsumerModule* module = 0;

    //lookup provider module in cache (if it exists, it returns the cached module, otherwise it creates and returns a new one)
    String libraryName = _getConsumerLibraryName(consumerName);
    module = _lookupModule(libraryName);

    //build library path
    String libraryPath = FileSystem::getAbsolutePath((const char*)_consumerDir.getCString(), FileSystem::buildLibraryFileName(libraryName));
    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Loading library: " + libraryPath);

    //load module
    try
    {
        base = module->load(consumerName, libraryPath);
        consumer->set(module, base);

    } catch (Exception& ex)
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Error loading consumer module: " + ex.getMessage());

        throw Exception(MessageLoaderParms("DynListener.ConsumerManager.CANNOT_LOAD_MODULE",
                                           "Cannot load module ($0:$1): Unknown exception.",
                                           consumerName,
                                           libraryName));
    } catch (...)
    {
        throw Exception(MessageLoaderParms("DynListener.ConsumerManager.CANNOT_LOAD_MODULE",
                                           "Cannot load module ($0:$1): Unknown exception.",
                                           consumerName,
                                           libraryName));
    }

    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Successfully loaded consumer module " + libraryName);

    //initialize consumer
    try
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Initializing Consumer " +  consumerName);

        consumer->initialize();

        //ATTN: need to change this
        Semaphore* semaphore = new Semaphore(0);  //blocking

        consumer->setShutdownSemaphore(semaphore);

        //start the worker thread
        if (_thread_pool->allocate_and_awaken(consumer,
                                          _worker_routine,
                                          semaphore) != PEGASUS_THREAD_OK)
    {
        Logger::put(Logger::STANDARD_LOG, System::CIMSERVER, Logger::TRACE,
        "Not enough threads for consumer.");
 
        Tracer::trace(TRC_LISTENER, Tracer::LEVEL2,
        "Could not allocate thread for consumer.");

       consumer->setShutdownSemaphore(0);
       delete semaphore;
           throw Exception(MessageLoaderParms("DynListener.ConsumerManager.CANNOT_ALLOCATE_THREAD",
                        "Not enough threads for consumer worker routine."));
        }

        //wait until the listening thread has started.  Otherwise, there is a miniscule chance that the first event will be enqueued
        //before the consumer is waiting for it and the first indication after loading the consumer will be lost
        consumer->waitForEventThread();

        //load any outstanding requests
        Array<CIMInstance> outstandingIndications = _deserializeOutstandingIndications(consumerName);
        if (outstandingIndications.size())
        {
            //the consumer will signal itself in _loadOustandingIndications
            consumer->_loadOutstandingIndications(outstandingIndications);
        }

        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Successfully initialized consumer " + consumerName);

    } catch (...)
    {
        module->unloadModule();
        consumer->reset();
        throw Exception(MessageLoaderParms("DynListener.ConsumerManager.CANNOT_INITIALIZE_CONSUMER",
                                           "Cannot initialize consumer ($0).",
                                           consumerName));        
    }

    PEG_METHOD_EXIT();    
}


/** Returns the ConsumerModule with the given library name.  If it already exists, we return the one in the cache.  If it
 *  DNE, we create it and add it to the table.
 * @throws Exception if we cannot successfully create and initialize the consumer
 */ 
ConsumerModule* ConsumerManager::_lookupModule(const String & libraryName) 
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::_lookupModule");

    AutoMutex lock(_moduleTableMutex);

    ConsumerModule* module = 0;

    //see if consumer module is cached
    if (_modules.lookup(libraryName, module))
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4,
                         "Found Consumer Module" + libraryName + " in Consumer Manager Cache");

    } else
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4,
                         "Creating Consumer Provider Module " + libraryName);

        module = new ConsumerModule(); 
        _modules.insert(libraryName, module);
    }

    PEG_METHOD_EXIT();
    return(module);
}

/** Returns true if there are active consumers
 */ 
Boolean ConsumerManager::hasActiveConsumers()
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::hasActiveConsumers");

    AutoMutex lock(_consumerTableMutex);
    DynamicConsumer* consumer = 0;

    try
    {
        for (ConsumerTable::Iterator i = _consumers.start(); i != 0; i++)
        {
            consumer = i.value();

            if (consumer && consumer->isLoaded() && (consumer->getPendingIndications() > 0))
            {
                PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Found active consumer: " + consumer->_name);
                PEG_METHOD_EXIT();
                return true;
            }
        }
    } catch (...)
    {
        // Unexpected exception; do not assume that no providers are loaded
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Unexpected Exception in hasActiveConsumers.");
        PEG_METHOD_EXIT();
        return true;
    }

    PEG_METHOD_EXIT();
    return false;
}

/** Returns true if there are loaded consumers
 */ 
Boolean ConsumerManager::hasLoadedConsumers()
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::hasLoadedConsumers");

    AutoMutex lock(_consumerTableMutex);
    DynamicConsumer* consumer = 0;

    try
    {
        for (ConsumerTable::Iterator i = _consumers.start(); i != 0; i++)
        {
            consumer = i.value();

            if (consumer && consumer->isLoaded())
            {
                PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Found loaded consumer: " + consumer->_name);
                PEG_METHOD_EXIT();
                return true;
            }
        }
    } catch (...)
    {
        // Unexpected exception; do not assume that no providers are loaded
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Unexpected Exception in hasLoadedConsumers.");
        PEG_METHOD_EXIT();
        return true;
    }

    PEG_METHOD_EXIT();
    return false;
}


/** Shutting down a consumer consists of four major steps:
 * 1) Send the shutdown signal.  This causes the worker routine to break out of the loop and exit.
 * 2) Wait for the worker thread to end.  This may take a while if it's processing an indication.  This
 *    is optional in a shutdown scenario.  If the listener is shutdown with a -f force, the listener
 *    will not wait for the consumer to finish before shutting down.  Note that a normal shutdown only allows
 *    the current consumer indication to finish.  All other queued indications are serialized to a log and 
 *    are sent when the consumer is reoaded.
 * 3) Terminate the consumer provider interface.
 * 4) Decrement the module refcount (the module will automatically unload when it's refcount == 0)
 * 
 * In a scenario where more multiple consumers are loaded, the shutdown signal should be sent to all
 * of the consumers so the threads can finish simultaneously.
 * 
 * ATTN: Should the normal shutdown wait for everything in the queue to be processed?  Just new indications
 * to be processed?  I am not inclined to this solution since it could take a LOT of time.  By serializing 
 * and deserialing indications between shutdown and startup, I feel like we do not need to process ALL
 * queued indications on shutdown.  
 */ 

/** Unloads all consumers.
 */ 
void ConsumerManager::unloadAllConsumers()
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::unloadAllConsumers");

    AutoMutex lock(_consumerTableMutex);

    if (!_consumers.size())
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "There are no consumers to unload.");
        PEG_METHOD_EXIT();
        return;
    }

    if (!_forceShutdown)
    {
        //wait until all the consumers have finished processing the events in their queue
        //ATTN: Should this have a timeout even though it's a force??
        while (hasActiveConsumers())
        {
            pegasus_sleep(500);
        }
    }

    Array<DynamicConsumer*> loadedConsumers;

    ConsumerTable::Iterator i = _consumers.start();
    DynamicConsumer* consumer = 0;

    for (; i!=0; i++)
    {
        consumer = i.value();
        if (consumer && consumer->isLoaded())
        {
            loadedConsumers.append(consumer);
        }
    }

    if (loadedConsumers.size())
    {
        try
        {
            _unloadConsumers(loadedConsumers);

        } catch (Exception& ex)
        {
            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Error unloading consumers.");
        }
    } else
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "There are no consumers to unload.");
    }

    PEG_METHOD_EXIT();
}

/** Unloads idle consumers.
 */ 
void ConsumerManager::unloadIdleConsumers()
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::unloadIdleConsumers");

    AutoMutex lock(_consumerTableMutex);

    if (!_consumers.size())
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "There are no consumers to unload.");
        PEG_METHOD_EXIT();
        return;
    }

    Array<DynamicConsumer*> loadedConsumers;

    ConsumerTable::Iterator i = _consumers.start();
    DynamicConsumer* consumer = 0;

    for (; i!=0; i++)
    {
        consumer = i.value();
        if (consumer && consumer->isLoaded() && consumer->isIdle())
        {
            loadedConsumers.append(consumer);
        }
    }

    if (loadedConsumers.size())
    {
        try
        {
            _unloadConsumers(loadedConsumers);

        } catch (Exception& ex)
        {
            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Error unloading consumers.");
        }
    } else
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "There are no consumers to unload.");
    }

    PEG_METHOD_EXIT();
}

/** Unloads a single consumer.
 */ 
void ConsumerManager::unloadConsumer(const String& consumerName)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::unloadConsumer");

    AutoMutex lock(_consumerTableMutex);

    DynamicConsumer* consumer = 0;

    //check whether the consumer exists
    if (!_consumers.lookup(consumerName, consumer))
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL3, "Error: cannot unload consumer, unknown consumer " + consumerName);
        return;
    }

    //check whether the consumer is loaded
    if (consumer && consumer->isLoaded())  //ATTN: forceShutdown?
    {
        //unload the consumer
        Array<DynamicConsumer*> loadedConsumers;
        loadedConsumers.append(consumer);

        try
        {
            _unloadConsumers(loadedConsumers);

        } catch (Exception& ex)
        {
            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Error unloading consumers.");
        }

    } else
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL3, "Error: cannot unload consumer " + consumerName);
    }

    PEG_METHOD_EXIT();
}

/** Unloads the consumers in the given array.
 *  The consumerTable mutex MUST be locked prior to entering this method.
 */ 
void ConsumerManager::_unloadConsumers(Array<DynamicConsumer*> consumersToUnload)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::_unloadConsumers");

    //tell consumers to shutdown
    for (Uint32 i = 0; i < consumersToUnload.size(); i++)
    {
        consumersToUnload[i]->sendShutdownSignal();
    }

    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Sent shutdown signal to all consumers.");

    //wait for all the consumer worker threads to complete
    //since we can only shutdown after they are all complete, it does not matter if the first, fifth, or last
    //consumer takes the longest; the wait time is equal to the time it takes for the busiest consumer to stop
    //processing its requests.
    for (Uint32 i = 0; i < consumersToUnload.size(); i++)
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL3, "Unloading consumer " + consumersToUnload[i]->getName());

        //wait for the consumer worker thread to end
        try
        {
            Semaphore* _shutdownSemaphore = consumersToUnload[i]->getShutdownSemaphore();
            if (_shutdownSemaphore)
            {
                _shutdownSemaphore->time_wait(10000); 
            }

        } catch (TimeOut &)
        {
            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Timed out while attempting to stop consumer thread.");
        }

        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Terminating consumer.");

        try
        {
            //terminate consumer provider interface
            consumersToUnload[i]->terminate();

            //unload consumer provider module
            PEGASUS_ASSERT(consumersToUnload[i]->_module != 0);
            consumersToUnload[i]->_module->unloadModule();

            //serialize outstanding indications
            _serializeOutstandingIndications(consumersToUnload[i]->getName(), consumersToUnload[i]->_retrieveOutstandingIndications());

            //reset the consumer
            consumersToUnload[i]->reset();

            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL3, "Consumer library successfully unloaded.");

        } catch (Exception& e)
        {
            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Error unloading consumer: " + e.getMessage()); 
            //ATTN: throw exception? log warning?
        }
    }

    PEG_METHOD_EXIT();
}

/** Serializes oustanding indications to a <MyConsumer>.dat file
 */
void ConsumerManager::_serializeOutstandingIndications(const String& consumerName, Array<CIMInstance> indications)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::_serializeOutstandingIndications");

    if (!indications.size())
    {
        PEG_METHOD_EXIT();
        return;
    }

    String fileName = FileSystem::getAbsolutePath((const char*)_consumerConfigDir.getCString(), String(consumerName + ".dat"));
    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Consumer dat file: " + fileName);

    Array<char> buffer;

    // Open the log file and serialize remaining 
    FILE* fileHandle = 0;
    fileHandle = fopen((const char*)fileName.getCString(), "w"); 

    if (!fileHandle)
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Unable to open log file for " + consumerName);

    } else
    {
        Tracer::trace(__FILE__,__LINE__,TRC_LISTENER,Tracer::LEVEL3,
                      "Serializing %d outstanding requests for %s",
                      indications.size(),
                      (const char*)consumerName.getCString());

        //we have to put the array of instances under a valid root element or the parser complains 
        XmlWriter::append(buffer, "<IRETURNVALUE>\n");

        for (Uint32 i = 0; i < indications.size(); i++)
        {
            XmlWriter::appendValueNamedInstanceElement(buffer, indications[i]);
        }

        XmlWriter::append(buffer, "</IRETURNVALUE>\0");

        fputs((const char*)buffer.getData(), fileHandle);

        fclose(fileHandle);
    }

    PEG_METHOD_EXIT();
}

/** Reads outstanding indications from a <MyConsumer>.dat file
 */ 
Array<CIMInstance> ConsumerManager::_deserializeOutstandingIndications(const String& consumerName)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::_deserializeOutstandingIndications");

    String fileName = FileSystem::getAbsolutePath((const char*)_consumerConfigDir.getCString(), String(consumerName + ".dat"));
    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Consumer dat file: " + fileName);

    Array<CIMInstance> cimInstances;

    // Open the log file and serialize remaining indications
    if (FileSystem::exists(fileName)  && FileSystem::canRead(fileName))
    {
        Array<char> text;
        CIMInstance cimInstance;
        XmlEntry entry;

        try
        {
            FileSystem::loadFileToMemory(text, fileName);  //ATTN: Is this safe to use; what about CRLFs?
            text.append('\0');

            //parse the file
            XmlParser parser((char*)text.getData());
            XmlReader::expectStartTag(parser, entry, "IRETURNVALUE");

            while (XmlReader::getNamedInstanceElement(parser, cimInstance))
            {
                cimInstances.append(cimInstance);
            }

            XmlReader::expectEndTag(parser, "IRETURNVALUE");

            Tracer::trace(__FILE__,__LINE__,TRC_LISTENER,Tracer::LEVEL3,
                          "Consumer %s has %d outstanding indications",
                          (const char*)consumerName.getCString(),
                          cimInstances.size());

            //delete the file 
            FileSystem::removeFile(fileName);

        } catch (Exception& ex)
        {
            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL3, "Error parsing dat file: " + ex.getMessage() + " " + consumerName);

        } catch (...)
        {
            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Error parsing dat file: Unknown Exception " + consumerName);
        }
    }

    PEG_METHOD_EXIT();
    return cimInstances;
}



/** 
 * This is the main worker thread of the consumer.  By having only one thread per consumer, we eliminate a ton
 * of synchronization issues and make it easy to prevent the consumer from performing two mutually exclusive
 * operations at once.  This also prevents one bad consumer from taking the entire listener down.  That being said,
 * it is up to the programmer to write smart consumers, and to ensure that their actions don't deadlock the worker thread. 
 * 
 * If a consumer receives a lot of traffic, or it's consumeIndication() method takes a considerable amount of time to
 * complete, it may make sense to make the consumer multi-threaded.  The individual consumer can immediately spawn off
 * new threads to handle indications, and return immediately to catch the next indication.  In this way, a consumer
 * can attain extremely high performance. 
 * 
 * There are three different events that can signal us:
 * 1) A new indication (signalled by DynamicListenerIndicationDispatcher)
 * 2) A shutdown signal (signalled from ConsumerManager, due to a listener shutdown or an idle consumer state)
 * 3) A retry signal (signalled from this routine itself)
 * 
 * The idea is that all new indications are put on the front of the queue and processed first.  All of the retry
 * indications are put on the back of the queue and are only processed AFTER all new indications are sent.
 * Before processing each indication, we check to see whether or not the shutdown signal was given.  If so,
 * we immediately break out of the loop, and another compenent serializes the remaining indications to a file.
 * 
 * An indication gets retried if the consumer throws a CIM_ERR_FAILED exception.
 * 
 * This function makes sure it waits until the default retry lapse has passed to avoid issues with the following scenario:
 * 20 new indications come in, 10 of them are successful, 10 are not.
 * We were signalled 20 times, so we will pass the time_wait 20 times.  Perceivably, the process time on each indication
 * could be minimal.  We could potentially proceed to process the retries after a very small time interval since
 * we would never hit the wait for the retry timeout.  
 * 
 * ATTN: Outstanding issue with this strategy -- 20 new indications come in, 19 of them come in before the first one
 * is processed.  Because new indications are first in, first out, the 19 indications will be processed in reverse order.
 * Is this a problem? Short answer - NO.  They could arrive in reverse order anyways depending on the network stack.
 * 
 */ 
PEGASUS_THREAD_RETURN PEGASUS_THREAD_CDECL ConsumerManager::_worker_routine(void *param)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ConsumerManager::_worker_routine");

    DynamicConsumer* myself = static_cast<DynamicConsumer*>(param);
    String name = myself->getName();
    DQueue<IndicationDispatchEvent> tmpEventQueue(true);

    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "_worker_routine::entering loop for " + name);

    PEGASUS_STD(cout) << "Worker thread started for consumer : " << name << endl;

    myself->_listeningSemaphore->signal();

    while (true)
    {
        try
        {
            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "_worker_routine::waiting " + name);

            //wait to be signalled
            myself->_check_queue->time_wait(DEFAULT_RETRY_LAPSE);

            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "_worker_routine::signalled " + name);

            //check whether we received the shutdown signal
            if (myself->_dieNow)
            {
                PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "_worker_routine::shutdown received " + name);
                break;
            }

            //signal must have been due to an incoming event
            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "_worker_routine::indication received " + name);

            //create a temporary queue to store failed indications
            tmpEventQueue.empty_list();

            //continue processing events until the queue is empty
            //make sure to check for the shutdown signal before every iteration

            while (myself->_eventqueue.size())
            {
                //check for shutdown signal
                //this only breaks us out of the queue loop, but we will immediately get through the next wait from
                //the shutdown signal itself, at which time we break out of the main loop
                if (myself->_dieNow)
                {
                    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Received signal to shutdown, jumping out of queue loop " + name);
                    break;
                }

                //pop next indication off the queue
                IndicationDispatchEvent* event = 0;
                event = myself->_eventqueue.remove_first();  //what exceptions/errors can this throw?

                if (!event)
                {
                    //this should never happen
                    continue;
                }

                //check retry status. do not retry until the retry time has elapsed
                if (event->getRetries() > 0)
                {
                    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Last attempt time from event is " + event->getLastAttemptTime().toString());
                    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Current time is " + CIMDateTime::getCurrentDateTime().toString());
                    Sint64 differenceInMicroseconds = CIMDateTime::getDifference(event->getLastAttemptTime(),
                                                                                 CIMDateTime::getCurrentDateTime());

                    if (differenceInMicroseconds < (DEFAULT_RETRY_LAPSE * 1000))
                    {
                        //do not retry; just add to the retry queue
                        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "_worker_routine::cannot retry event until retry time has elapsed");
                        tmpEventQueue.insert_last(event);
                        continue;
                    }
                }

                PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "_worker_routine::consumeIndication " + name);

                try
                {
                    myself->consumeIndication(event->getContext(),
                                              event->getURL(),
                                              event->getIndicationInstance());

                    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "_worker_routine::processed indication successfully. " + name);

                    delete event;
                    continue;

                } catch (CIMException & ce)
                {
                    //check for failure
                    if (ce.getCode() == CIM_ERR_FAILED)
                    {
                        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "_worker_routine::consumeIndication() temporary failure: " + ce.getMessage() + " " + name);

                        //update event parameters
                        event->increaseRetries();

                        //determine if we have hit the max retry count
                        if (event->getRetries() >= DEFAULT_MAX_RETRY_COUNT)
                        {
                            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2,
                                             "Error: the maximum retry count has been exceeded.  Removing the event from the queue.");

                            Logger::put(
                                       Logger::ERROR_LOG,
                                       "ConsumerManager",
                                       Logger::SEVERE,
                                       "The following indication did not get processed successfully: $0", 
                                       event->getIndicationInstance().getPath().toString());

                            delete event;
                            continue;

                        } else
                        {
                            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "_worker_routine::placing failed indication back in queue");
                            tmpEventQueue.insert_last(event);
                        }

                    } else
                    {
                        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Error: consumeIndication() permanent failure: " + ce.getMessage());
                        delete event;
                        continue;
                    }

                } catch (Exception & ex)
                {
                    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Error: consumeIndication() permanent failure: " + ex.getMessage());
                    delete event;
                    continue;

                } catch (...)
                {
                    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Error: consumeIndication() failed: Unknown exception.");
                    delete event;
                    continue;
                } //end try

            } //while eventqueue

            //copy the failed indications back to the main queue
            //since we are always adding the failed indications to the back, it should not interfere with the
            //dispatcher adding events to the front

            //there is no = operator for DQueue so we must do it manually
            IndicationDispatchEvent* tmpEvent = 0;
            while (tmpEventQueue.size())
            {
                tmpEvent = tmpEventQueue.remove_first();
                myself->_eventqueue.insert_last(tmpEvent);
            }

        } catch (TimeOut& te)
        {
            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "_worker_routine::Time to retry any outstanding indications.");

            //signal the queue in the same way we would if we received a new indication
            //this allows the thread to fall into the queue processing code
            myself->_check_queue->signal();

        } //time_wait


    } //shutdown

    PEG_METHOD_EXIT();
    return 0;
}


PEGASUS_NAMESPACE_END



