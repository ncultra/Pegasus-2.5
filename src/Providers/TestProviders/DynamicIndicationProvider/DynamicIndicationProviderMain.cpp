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
// Author: Jake Kitchener (jakekit@us.ibm.com)
//
// Modified By: Heather Sterling (hsterl@us.ibm.com)
//
//%/////////////////////////////////////////////////////////////////////////////

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/String.h>
#include <Pegasus/Provider/CIMIndicationProvider.h>

#include "DynamicIndicationProvider.h"

/** This test provider allows you to generate indications on demand using an invokeMethod call.
 * The method, sendIndication(), takes the indication classname, severity, and description as arguments. 
 * This is a lot easier than hard-coding indication providers to do what you want for testing/debugging purposes.
 * 
 * To register the provider, use the following commands (this will vary based on configuration):
 * cp *.mof pegasus/mof
 * cd pegasus/mof
 * ../bin/cimmof -nroot/pg_interop DynamicIndicationProviderRegistration.mof
 * ../bin/cimmof -nroot/cimv2 DynamicIndicationProviderSchema.mof
 * 
 * You must create a subscription against one of the provider's indication classes in order for it to be enabled
 * for indications.
 * 
 * To exercise the provider, you can script commands using CLI:
 *  >./CLI im PG_DynamicIndication sendIndication severity=0 classname=MyTestIndication description=test
 */ 

PEGASUS_NAMESPACE_BEGIN

extern "C"
PEGASUS_EXPORT CIMProvider* PegasusCreateProvider(const String& providerName)
{
    if (String::equalNoCase(providerName, "DynamicIndicationProvider"))
    {
         return(new DynamicIndicationProvider());
    }
    return 0;
}

PEGASUS_NAMESPACE_END
