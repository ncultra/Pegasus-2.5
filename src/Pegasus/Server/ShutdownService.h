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
// Author: Jenny Yu, Hewlett-Packard Company (jenny_yu@hp.com)
//
// Modified By:
//     Amit K Arora, IBM (amita@in.ibm.com) for PEP#101
//
//%////////////////////////////////////////////////////////////////////////////

#ifndef Pegasus_ShutdownService_h
#define Pegasus_ShutdownService_h

#include <cctype>
#include <Pegasus/Common/Config.h>
#include <Pegasus/Config/ConfigManager.h>
#include <Pegasus/Common/String.h>
#include <Pegasus/Common/ArrayInternal.h>
#include <Pegasus/Common/System.h>
#include <Pegasus/Common/ModuleController.h>
#include <Pegasus/Common/AutoPtr.h>
#include <Pegasus/Server/CIMServer.h>
#include <Pegasus/Server/Linkage.h>

PEGASUS_NAMESPACE_BEGIN

/**
///////////////////////////////////////////////////////////////////////////////
//
// This class defines the ShutdownService necessary to process the shutdown
// request to gracefully shutdown the CIMServer.
//
// In order to shutdown the CIMServer gracefully, the CIMServer state
// information must be maintained.
//
// The state of the CIMServer is kept in the CIMServerState object.  After
// CIMServer starts up, the CIMServer state is set to RUNNING.  When a
// shutdown request is received by the ShutdownService, the CIMServer state
// will be set to TERMINATING.
//
// Whenever a request comes through a client connection, before the request
// is being routed off to the appropriate message queue, the CIMServer state
// is checked.  If the state is set to TERMINATING, this means CIMServer is
// in the process of being shutdown, an error response will be sent back
// to the client to indicate that the request cannot be processed due to
// CIMServer shutting down.
//
// In order to determine if the CIMServer is servicing any CIM requests at
// the time of a shutdown, CIMServer keeps track of the number of CIM requests
// that are outstanding.  This request count is kept in the HTTPConnection
// object.  The request count is incremented everytime a request comes through
// a client connection, and is decremented everytime a response is sent back
// to the client.
//
// Before the ShutdownService shuts down the CIMServer, the request count is
// checked to determine if there are any outstanding CIM requests being
// processed.  If there are no requests outstanding, the CIMServer will be
// shutdown.  If there are requests outstanding, the ShutdownService will
// wait periodically until the requests are all processed or until the
// timeout expires before shutting down CIMServer.
//
///////////////////////////////////////////////////////////////////////////////
*/

class PEGASUS_SERVER_LINKAGE ShutdownService
{
public:

    /**
    Terminate the ShutdownService
    */
    static void destroy( void );
    /**
    Construct the singleton instance of the ShutdownService and return a
    pointer to that instance.
    */
    static ShutdownService* getInstance(CIMServer* cimserver);

    /**
    Shutdown CIMOM.
    @param requestPending Boolean indicating whether the shutdown was
           initiated through a synchronous CIM request (true) or not (false).
    */
    void shutdown(Boolean force, Uint32 timeout, Boolean requestPending);

    void shutdownCimomServices();
    Boolean waitUntilNoMoreRequests(Boolean requestPending);

protected:

    static pegasus_internal_identity        _id;
    static ModuleController *               _controller;
    static ModuleController::client_handle *_client_handle;

private:

    static ShutdownService*         _instance;
    static CIMServer*               _cimserver;
    static Uint32                   _shutdownTimeout;

    //
    // This is meant to be a singleton, so the constructor and the
    // destructor are made private.
    //

    /** Constructor. */
    ShutdownService(CIMServer* cimserver);

    /** Destructor. */
    ~ShutdownService();

    void _shutdownCIMServer();

    void _resumeCIMServer();


    void _sendShutdownRequestToService(const char * serviceName);

    void _shutdownProviders();

    void _initTimeoutValues(Uint32 timeoutParmValue);

};

PEGASUS_NAMESPACE_END

#endif /* Pegasus_ShutdownService_h */

