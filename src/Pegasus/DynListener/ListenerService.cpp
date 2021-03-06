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
// Modified By: Aruran, IBM (ashanmug@in.ibm.com) for Bug# 3654
//
//%/////////////////////////////////////////////////////////////////////////////

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/System.h>
#include <Pegasus/Common/IPC.h>

#include "ListenerService.h"

PEGASUS_NAMESPACE_BEGIN
PEGASUS_USING_STD;

static const int SHUTDOWN_TIMEOUT = 10; //seconds

ListenerService::ListenerService(ConsumerManager* consumerManager) : 
_consumerManager(consumerManager),
_dispatcher(0),
_portNumber(0),
_useSSL(false),
_sslContext(0),
_initialized(0),
_running(0),
_dieNow(0),
_shutdownSem(0),
_monitor(0),
_acceptor(0),
_responseEncoder(0),
_requestDecoder(0),
_listening_thread(0),
_polling_thread(0)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ListenerService::ListenerService");


    PEG_METHOD_EXIT();
}

ListenerService::~ListenerService()
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ListenerService::ListenerService");

    if (_running)
    {
        shutdownListener();
    }

    if (_initialized)
    {
        //cleanup everything we initialized

        if (_sslContext != NULL)
            delete _sslContext;

        if (_dispatcher != NULL)
            delete _dispatcher;

        if (_responseEncoder != NULL)
            delete _responseEncoder;

        if (_requestDecoder != NULL)
            delete _requestDecoder;

        if (_shutdownSem != NULL)
            delete _shutdownSem;
        
        //do not delete _consumerManager
        //it is deleted by CIMListener
    }

    PEG_METHOD_EXIT();
}

ListenerService::ListenerService(const ListenerService& x)
{
}

Boolean ListenerService::initializeListener(Uint32 portNumber, Boolean useSSL, SSLContext* sslContext)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ListenerService::initializeListener");

    if (_initialized)
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Warning: The listener is already initialized.");
        return true;
    }

    _portNumber = portNumber;
    _useSSL = useSSL;
    _sslContext = sslContext;

    if (_useSSL && (_sslContext == NULL))
    {
        throw Exception(MessageLoaderParms("DynListener.ListenerService.INVALID_SSL_CONFIGURATION",
                                           "Invalid SSL configuration: No SSLContext was specified."));
    }

    if (!_useSSL && _sslContext)
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Warning: An SSLContext was specified for a non-SSL configuration.");
    }

    _dispatcher = new DynamicListenerIndicationDispatcher(_consumerManager);

    _responseEncoder = new CIMExportResponseEncoder();

    _requestDecoder = new CIMExportRequestDecoder(_dispatcher,
                                                  _responseEncoder->getQueueId());

    _shutdownSem = new Semaphore(0);

    _initialized = true;

    PEG_METHOD_EXIT();
    return true;
}

Boolean ListenerService::runListener()
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ListenerService::runListener");

    if (!_initialized)
    {
        throw Exception(MessageLoaderParms("DynListener.ListenerService.NOT_INITIALIZED",
                                           "Error: You must initialize the listener prior to running it."));
    }

    if (_running)
    {
        throw Exception(MessageLoaderParms("DynListener.ListenerService.ALREADY_RUNNING",
                                           "Error: The listener is already running."));
    }
    
    _monitor = new Monitor();

    _acceptor = new HTTPAcceptor(
                                _monitor, 
                                _requestDecoder, 
                                false, 
                                _portNumber, 
                                _sslContext,
                                false);

    //create listening thread
    _listening_thread = new Thread(_listener_routine, this, 0);

    //bind listener socket
    _acceptor->bind();

    //start listening thread
    ThreadStatus rtn = PEGASUS_THREAD_OK;
    while ( (rtn = _listening_thread->run()) != PEGASUS_THREAD_OK)
    {
	if (rtn == PEGASUS_THREAD_INSUFFICIENT_RESOURCES)
        	pegasus_yield();
	else {
		// We need to set _running to true so that we can shutdown the 
		// rest of the classes
		delete _listening_thread; _listening_thread = 0;
    		_running = true;
		shutdownListener();		
        	throw Exception(MessageLoaderParms("DynListener.ListenerService.CANNOT_ALLOCATE_THREAD",
                                           "Error: Cannot allocate thread."));
	}
	
    }
    
    if (_consumerManager->getEnableConsumerUnload())
    {
        //create polling thread
        _polling_thread = new Thread(_polling_routine , this, 0);
    
        //start polling thread
        while ( (rtn=_polling_thread->run()) != PEGASUS_THREAD_OK)
        {
	   if (rtn == PEGASUS_THREAD_INSUFFICIENT_RESOURCES)
            pegasus_yield();
	   else
	   {
		/* We should delete them the objects, but there is a question
                   of how to turn of the _listening_thread? */
		delete _polling_thread; _polling_thread = 0;
    		_running = true;
		shutdownListener();		
        	throw Exception(MessageLoaderParms("DynListener.ListenerService.CANNOT_ALLOCATE_THREAD",
                                           "Error: Cannot allocate thread."));
	    }
        }
    }

    _running = true;

    PEG_METHOD_EXIT();
    return true;
}

PEGASUS_THREAD_RETURN PEGASUS_THREAD_CDECL ListenerService::_listener_routine(void *param)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ListenerService::_listener_routine");

    Thread *myself = reinterpret_cast<Thread *>(param);
    ListenerService* listenerService = reinterpret_cast<ListenerService*>(myself->get_parm());

    static int modulator = 0;  //ATTN: What is this for???

    while (true)
    {
          static int modulator = 0;

          if(!(listenerService->_dieNow))
          {
              //PEGASUS_STD(cout) << "Running monitor\n";
              if(!(listenerService->_monitor->run(30000)))
              {   
      modulator++;
      try 
      {
         //MessageQueueService::_check_idle_flag = 1;
         //MessageQueueService::_polling_sem.signal();
         MessageQueueService::get_thread_pool()->cleanupIdleThreads();
      }
      catch(...)
      {
      }
    }
          } else
          {
				PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "ListenerService::Got shutdown signal.");
              break;
          }
    }

    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "ListenerService::Stopping _listener_routine");

    PEG_METHOD_EXIT();

    return 0;  //success
}


PEGASUS_THREAD_RETURN PEGASUS_THREAD_CDECL ListenerService::_polling_routine(void *param)
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ListenerService::_polling_routine");

    Thread *myself = reinterpret_cast<Thread *>(param);
    ListenerService* listenerService = reinterpret_cast<ListenerService*>(myself->get_parm());

    while (true)
    {
        try
        {
            //do a timed wait so we do can process a shutdown signal immediately
            listenerService->_shutdownSem->time_wait(listenerService->_consumerManager->getIdleTimeout());

            if (listenerService->_dieNow)
            {
                //shutdown
                break;
            }

        } catch (TimeOut& te)
        {
            //time to check for idle consumers
            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL3, "Unloading idle consumers");
            listenerService->_consumerManager->unloadIdleConsumers();
        }
    }

    PEG_METHOD_EXIT();
    return 0; //success
}

Boolean ListenerService::shutdownListener()
{
    PEG_METHOD_ENTER(TRC_LISTENER, "ListenerService::shutdownListener");

    if (!_running)
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Warning: The listener is not currently running.");

        return true;
    }

    Boolean gracefulShutdown = true;

    //stop the monitor from accepting connections
    _monitor->stopListeningForConnections(true);

    _acceptor->closeConnectionSocket();

    //allow client threads to complete, wait 10 sec max
    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "ListenerService::Waiting for outstanding requests...");
    Uint32 reqCount;
    Uint32 countDown = SHUTDOWN_TIMEOUT;
    for (; countDown > 0; countDown--)
    {
        reqCount = _acceptor->getOutstandingRequestCount();

        if (reqCount > 0)
        {
            pegasus_sleep(1000);
        } else
        {
            break;
        }
    }

    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "ListenerService::Finished waiting for outstanding requests.");

    if (reqCount > 0)
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "ListenerService::Did not successfully process all incoming requests to the acceptor.");
        gracefulShutdown = false;
    }

    //stop the monitor thread and idle thread
    _dieNow = true;

    //ATTN: This is ghetto and needs to be fixed
    //signal twice, once for polling thread, once for listener thread
    //have to signal before both join calls so we do not hit a deadlock
    _shutdownSem->signal();
    _shutdownSem->signal();

    _monitor->tickle();

    //stop listener thread
    try
    {
        _listening_thread->join();
        delete _listening_thread;
        _listening_thread = 0;

    } catch (...)
    {
        PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Did not successfully stop monitor thread");
        gracefulShutdown = false;
    }

    //stop polling thread
    if (_polling_thread) //may not be started if polling is disabled
    {
        try
        {
            _polling_thread->join();
            delete _polling_thread;
            _polling_thread = 0;

        } catch (...)
        {
            PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Did not successfully stop polling thread");
            gracefulShutdown = false;
        }
    }

    //delete acceptor
    delete _acceptor;
    _acceptor = 0;

    //delete monitor
    delete _monitor;
    _monitor = 0;

    PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL2, "Listener stopped.");

    //reset status values
    _running = false;
    _dieNow = false;

	if (gracefulShutdown)
	{
		PEG_TRACE_STRING(TRC_LISTENER, Tracer::LEVEL4, "Listener shutdown gracefully");
	} 

    PEG_METHOD_EXIT();
    return(gracefulShutdown);
}

Boolean ListenerService::isAlive() const throw()
{
    return _running;
}

//ATTN: Methods for old CIMListener interface...do we need them now?
Uint32 ListenerService::getPortNumber() const
{
    return _portNumber;
}


PEGASUS_NAMESPACE_END
