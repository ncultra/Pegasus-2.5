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
// Author: Chip Vincent (cvincent@us.ibm.com)
//
// Modified By: Roger Kumpf, Hewlett-Packard Company (roger_kumpf@hp.com)
//              Nitin Upasani, Hewlett-Packard Company (Nitin_Upasani@hp.com)
//              Mike Day, IBM (mdday@us.ibm.com)
//              Yi Zhou, Hewlett-Packard Company (yi_zhou@hp.com)
//              Heather Sterling, IBM (hsterl@us.ibm.com)
//
//%/////////////////////////////////////////////////////////////////////////////

#ifndef Dynamic_Consumer_Facade_h
#define Dynamic_Consumer_Facade_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/IPC.h>
#include <Pegasus/Provider/CIMIndicationConsumerProvider.h>
#include <Pegasus/DynListener/Linkage.h>

PEGASUS_NAMESPACE_BEGIN

// The ProviderModule class wraps a provider pointer extracted from a provider
// module to ensure it is complete and well behaved. So, regardless of the
// method supported by a "real" provider, it can be placed inside a reliable
// facade with a known interface.

class PEGASUS_DYNLISTENER_LINKAGE DynamicConsumerFacade : public CIMIndicationConsumerProvider
{
public:
    DynamicConsumerFacade(CIMIndicationConsumerProvider* consumer);
    virtual ~DynamicConsumerFacade(void);

    virtual void initialize(CIMOMHandle & cimom);

    virtual void terminate(void);

    virtual void consumeIndication(
        const OperationContext & context,
        const String & url,
        const CIMInstance& indicationInstance);

protected:
    CIMIndicationConsumerProvider* _consumer;
    AtomicInt _current_operations;

};

PEGASUS_NAMESPACE_END

#endif

