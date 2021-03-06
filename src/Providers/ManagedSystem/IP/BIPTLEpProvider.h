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
// Author: Christopher Neufeld <neufeld@linuxcare.com>
//         David Kennedy       <dkennedy@linuxcare.com>
//
// Modified By:
//         David Kennedy       <dkennedy@linuxcare.com>
//         Christopher Neufeld <neufeld@linuxcare.com>
//         Al Stone, Hewlett-Packard Company <ahs3@fc.hp.com>
//         Jim Metcalfe, Hewlett-Packard Company
//         Carlos Bonilla, Hewlett-Packard Company
//         Mike Glantz, Hewlett-Packard Company <michael_glantz@hp.com>
//         Lyle Wilkinson, Hewlett-Packard Company <lyle_wilkinson@hp.com>
//              Carol Ann Krug Graves, Hewlett-Packard Company
//                (carolann_graves@hp.com)
//
//%////////////////////////////////////////////////////////////////////////////

#ifndef PG_BIPTLEP_PROVIDER_H
#define PG_BIPTLEP_PROVIDER_H

/* ==========================================================================
   Includes.
   ========================================================================== */

#include <Pegasus/Provider/CIMInstanceProvider.h>
#include "IPPlatform.h"

PEGASUS_USING_STD;
PEGASUS_USING_PEGASUS;


class BIPTLEpProvider : public CIMInstanceProvider
{

public:

  BIPTLEpProvider();

  ~BIPTLEpProvider();

  void createInstance(const OperationContext       &context,
                    const CIMObjectPath           &instanceName,
                    const CIMInstance            &instanceObject,
                    ObjectPathResponseHandler &handler);

  void deleteInstance(const OperationContext       &context,
                    const CIMObjectPath           &instanceReference,
                    ResponseHandler &handler);

  void enumerateInstances(
	const OperationContext & context,
	const CIMObjectPath & classReference,
        const Boolean includeQualifiers,
        const Boolean includeClassOrigin,
	const CIMPropertyList & propertyList,
	InstanceResponseHandler & handler);

  void enumerateInstanceNames(const OperationContext        &context,
                            const CIMObjectPath            &ref,
                            ObjectPathResponseHandler &handler);

  void getInstance(const OperationContext       &context,
                 const CIMObjectPath           &instanceName,
                 const Boolean includeQualifiers,
                 const Boolean includeClassOrigin,
                 const CIMPropertyList        &propertyList,
                 InstanceResponseHandler &handler);

  void modifyInstance(const OperationContext       &context,
                    const CIMObjectPath           &instanceName,
                    const CIMInstance            &instanceObject,
                    const Boolean includeQualifiers,
		    const CIMPropertyList        &propertyList,
                    ResponseHandler &handler);

  void initialize(CIMOMHandle&);

  void terminate(void);

private:

  // private member to store handle passed by initialize()
  CIMOMHandle _cimomHandle;

  Array<CIMKeyBinding> _constructKeyBindings(const CIMNamespaceName &nameSpace,
					     const IPInterface &ipif);

  Array<CIMKeyBinding> _constructReference(const CIMName& className,
					   const String& sysName,
					   const String& instName);

  // Used to add properties to an instance
  CIMInstance _constructInstance(const CIMName &clnam,
                                 const CIMNamespaceName &nameSpace,
                                 const IPInterface &ipInt);

  // Used to check if a reference path to a Protocol Endpoint is good
  Boolean _goodPERefKeys(const CIMObjectPath &instName,
			 String &rccn,
			 String &rname);

  // checks the class passed by the cimom and throws
  // an exception if it's not supported by this provider
  void _checkClass(CIMName&);

};


#endif  /* #ifndef PG_BIPTLEP_PROVIDER_H */
