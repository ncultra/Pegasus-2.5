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
// Author: Barbara Packard (barbara_packard@hp.com)
//
// Modified By:
//
//%/////////////////////////////////////////////////////////////////////////////
// WMIBaseProvider.h: interface for the WMIBaseProvider class.
//
//////////////////////////////////////////////////////////////////////


#ifndef Pegasus_WMIBaseProvider_h
#define Pegasus_WMIBaseProvider_h

#include "config.h"
#include <Pegasus/Provider/CIMInstanceProvider.h>

PEGASUS_NAMESPACE_BEGIN

class WMICollector;

class PEGASUS_WMIPROVIDER_LINKAGE WMIBaseProvider 
{
public:
	WMIBaseProvider();
	virtual ~WMIBaseProvider();

	void initialize(bool bLocal = FALSE);
	virtual void terminate(void);

	CIMInstance getCIMInstance(const String& nameSpace,
		const String& userName,
		const String& password,
		const CIMObjectPath & instanceName, 
		const CIMPropertyList & propertyList = CIMPropertyList( ));

	CIMClass getCIMClass(const String& nameSpace, 
		const String& userName,
		const String& password,
		const String& className,
		const CIMPropertyList &propertyList = CIMPropertyList());

	Array<CIMObject> execCIMQuery(
		const String& nameSpace,
		const String& userName,
		const String& password,
        const String& queryLanguage,
        const String& query,
		const CIMPropertyList& propertyList = CIMPropertyList(),
		Boolean includeQualifiers = false,
		Boolean includeClassOrigin = false) ;

protected:
	void setup(
		const String & nameSpace,
		const String & userName,
		const String & password);
	void initCollector(bool bLocal = FALSE);
	void cleanup();

	String getQueryString(
		const CIMObjectPath& objectName, 
		const String& sQueryCommand,							   
		const String& assocClass = String::EMPTY, 
		const String& resultClass = String::EMPTY, 
		const String& role = String::EMPTY,
		const String& resultRole = String::EMPTY);

	String getObjectName(
		const CIMObjectPath& objectName); 

	WMICollector		*_collector;

	bool	m_bInitialized;

	String				m_sNamespace;
	String				m_sUserName;
	String				m_sPassword;
};

PEGASUS_NAMESPACE_END

#endif
