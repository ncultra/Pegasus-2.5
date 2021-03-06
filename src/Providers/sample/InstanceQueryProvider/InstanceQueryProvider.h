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
//              Adrian Schuur (schuur@de.ibm.com))
//
//%/////////////////////////////////////////////////////////////////////////////

#ifndef Pegasus_InstanceProvider_h
#define Pegasus_InstanceProvider_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Provider/CIMInstanceQueryProvider.h>

class InstanceQueryProvider :
	public PEGASUS_NAMESPACE(CIMInstanceQueryProvider)
{
public:
	InstanceQueryProvider(void);
	virtual ~InstanceQueryProvider(void);

	// CIMProvider interface
	virtual void initialize(PEGASUS_NAMESPACE(CIMOMHandle) & cimom);
	virtual void terminate(void);

	// CIMInstanceProvider interface
	virtual void getInstance(
		const PEGASUS_NAMESPACE(OperationContext) & context,
		const PEGASUS_NAMESPACE(CIMObjectPath) & ref,
		const PEGASUS_NAMESPACE(Boolean) includeQualifiers,
		const PEGASUS_NAMESPACE(Boolean) includeClassOrigin,
		const PEGASUS_NAMESPACE(CIMPropertyList) & propertyList,
		PEGASUS_NAMESPACE(InstanceResponseHandler) & handler);

	virtual void enumerateInstances(
		const PEGASUS_NAMESPACE(OperationContext) & context,
		const PEGASUS_NAMESPACE(CIMObjectPath) & ref,
		const PEGASUS_NAMESPACE(Boolean) includeQualifiers,
		const PEGASUS_NAMESPACE(Boolean) includeClassOrigin,
		const PEGASUS_NAMESPACE(CIMPropertyList) & propertyList,
		PEGASUS_NAMESPACE(InstanceResponseHandler) & handler);

	virtual void enumerateInstanceNames(
		const PEGASUS_NAMESPACE(OperationContext) & context,
		const PEGASUS_NAMESPACE(CIMObjectPath) & ref,
		PEGASUS_NAMESPACE(ObjectPathResponseHandler) & handler);

	virtual void modifyInstance(
		const PEGASUS_NAMESPACE(OperationContext) & context,
		const PEGASUS_NAMESPACE(CIMObjectPath) & ref,
		const PEGASUS_NAMESPACE(CIMInstance) & obj,
		const PEGASUS_NAMESPACE(Boolean) includeQualifiers,
		const PEGASUS_NAMESPACE(CIMPropertyList) & propertyList,
		PEGASUS_NAMESPACE(ResponseHandler) & handler);

	virtual void createInstance(
		const PEGASUS_NAMESPACE(OperationContext) & context,
		const PEGASUS_NAMESPACE(CIMObjectPath) & ref,
		const PEGASUS_NAMESPACE(CIMInstance) & obj,
		PEGASUS_NAMESPACE(ObjectPathResponseHandler) & handler);

	virtual void deleteInstance(
		const PEGASUS_NAMESPACE(OperationContext) & context,
		const PEGASUS_NAMESPACE(CIMObjectPath) & ref,
		PEGASUS_NAMESPACE(ResponseHandler) & handler);

    virtual void execQuery(
       const PEGASUS_NAMESPACE(OperationContext) & context,
       const PEGASUS_NAMESPACE(CIMObjectPath) & objectPath,
       const PEGASUS_NAMESPACE(QueryExpression) & query,
       PEGASUS_NAMESPACE(InstanceResponseHandler) & handler);



protected:
	PEGASUS_NAMESPACE(Array)<PEGASUS_NAMESPACE(CIMObjectPath)> _instanceNames;
	PEGASUS_NAMESPACE(Array)<PEGASUS_NAMESPACE(CIMInstance)> _instances;

};

#endif
