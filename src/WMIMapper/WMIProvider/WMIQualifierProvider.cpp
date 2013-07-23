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
// Modified By: Jair F. T. Santos (t.dos.santos.francisco@hp.com)
//
// WMIQualifierProvider.cpp: implementation of the WMIQualifierProvider class.
//
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// WMIQualifierProvider::
//
// ///////////////////////////////////////////////////////////////////////////

#include "Stdafx.h"

#include <Pegasus/Common/CIMQualifier.h>
#include <Pegasus/Common/CIMQualifierDecl.h>

#include "WMICollector.h"
#include "WMIBaseProvider.h"
#include "WMIClassProvider.h"
#include "WMIQualifierProvider.h"

PEGASUS_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WMIQualifierProvider::WMIQualifierProvider()
{

	_collector = NULL;
	m_bInitialized = false;
}

WMIQualifierProvider::~WMIQualifierProvider()
{
	cleanup();

}

/////////////////////////////////////////////////////////////////////////////
// Due to the difference between DMTF and WMI approaches related to        //
// qualifiers, the qualifier handling methods will not be implemented      //
// they will only return success from some cases (SetQualifier and         //
// DeleteQualifier methods) and will throw an NOT_SUPPORTED exception      //
// for the other methods (GetQualifier and EnumerateQualifiers) (by Jair)  //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// WMIQualifierProvider::getQualifier
//
// ///////////////////////////////////////////////////////////////////////////
CIMQualifierDecl WMIQualifierProvider::getQualifier(
        const String& nameSpace,
        const String& userName,
        const String& password,
        const String& qualifierName)
{
	throw CIMException(CIM_ERR_NOT_SUPPORTED);
}

/////////////////////////////////////////////////////////////////////////////
// WMIQualifierProvider::setQualifier
//
// ///////////////////////////////////////////////////////////////////////////
void WMIQualifierProvider::setQualifier(
        const String& nameSpace,
        const String& userName,
        const String& password,
        const CIMQualifierDecl& qualifierDecl)
{
	return;
}

/////////////////////////////////////////////////////////////////////////////
// WMIQualifierProvider::deleteQualifier
//
// ///////////////////////////////////////////////////////////////////////////
void WMIQualifierProvider::deleteQualifier(
        const String& nameSpace,
        const String& userName,
        const String& password,
        const String& qualifierName)
{
	return;
}

/////////////////////////////////////////////////////////////////////////////
// WMIQualifierProvider::enumerateQualifiers
//
// ///////////////////////////////////////////////////////////////////////////
Array<CIMQualifierDecl> WMIQualifierProvider::enumerateQualifiers(
        const String& nameSpace,
		const String& userName,
        const String& password)
{
	throw CIMException(CIM_ERR_NOT_SUPPORTED);
}

PEGASUS_NAMESPACE_END
