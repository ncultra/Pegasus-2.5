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
// Author: Mike Brasher (mbrasher@bmc.com)
//
// Modified By: Carol Ann Krug Graves, Hewlett-Packard Company
//                (carolann_graves@hp.com)
//              Roger Kumpf, Hewlett-Packard Company (roger_kumpf@hp.com)
//
//%/////////////////////////////////////////////////////////////////////////////

#include "CIMStatusCode.h"

PEGASUS_NAMESPACE_BEGIN

static const char* _cimMessages[] =
{
    "CIM_ERR_SUCCESS: successful",

    "CIM_ERR_FAILED: A general error occurred that is not covered by a more specific "
    "error code",

    "CIM_ERR_ACCESS_DENIED: Access to a CIM resource was not available to the client",

    "CIM_ERR_INVALID_NAMESPACE: The target namespace does not exist",

    "CIM_ERR_INVALID_PARAMETER: One or more parameter values passed to the method "
    "were invalid",

    "CIM_ERR_INVALID_CLASS: The specified class does not exist",

    "CIM_ERR_NOT_FOUND: The requested object could not be found",

    "CIM_ERR_NOT_SUPPORTED: The requested operation is not supported",

    "CIM_ERR_CLASS_HAS_CHILDREN: Operation cannot be carried out on this class since "
    "it has subclasses",

    "CIM_ERR_CLASS_HAS_INSTANCES: Operation cannot be carried out on this class since "
    "it has instances",

    "CIM_ERR_INVALID_SUPERCLASS: Operation cannot be carried out since the specified "
    "superclass does not exist",

    "CIM_ERR_ALREADY_EXISTS: Operation cannot be carried out because an object already "
    "exists",

    "CIM_ERR_NO_SUCH_PROPERTY: The specified property does not exist",

    "CIM_ERR_TYPE_MISMATCH: The value supplied is incompatible with the type",

    "CIM_ERR_QUERY_LANGUAGE_NOT_SUPPORTED: The query language is not recognized or "
    "supported",

    "CIM_ERR_INVALID_QUERY: The query is not valid for the specified query language",

    "CIM_ERR_METHOD_NOT_AVAILABLE: The extrinsic method could not be executed",

    "CIM_ERR_METHOD_NOT_FOUND: The specified extrinsic method does not exist"
};

// l10n - keys for cimstatus messages
static const char* _cimMessageKeys[] =
{
    "Common.CIMStatusCode.CIM_ERR_SUCCESS",

    "Common.CIMStatusCode.CIM_ERR_FAILED",

    "Common.CIMStatusCode.CIM_ERR_ACCESS_DENIED",

    "Common.CIMStatusCode.CIM_ERR_INVALID_NAMESPACE",

    "Common.CIMStatusCode.CIM_ERR_INVALID_PARAMETER",

    "Common.CIMStatusCode.CIM_ERR_INVALID_CLASS",

    "Common.CIMStatusCode.CIM_ERR_NOT_FOUND",

    "Common.CIMStatusCode.CIM_ERR_NOT_SUPPORTED",

    "Common.CIMStatusCode.CIM_ERR_CLASS_HAS_CHILDREN",

    "Common.CIMStatusCode.CIM_ERR_CLASS_HAS_INSTANCES",

    "Common.CIMStatusCode.CIM_ERR_INVALID_SUPERCLASS",

    "Common.CIMStatusCode.CIM_ERR_ALREADY_EXISTS",

    "Common.CIMStatusCode.CIM_ERR_NO_SUCH_PROPERTY",

    "Common.CIMStatusCode.CIM_ERR_TYPE_MISMATCH",

    "Common.CIMStatusCode.CIM_ERR_QUERY_LANGUAGE_NOT_SUPPORTED",

    "Common.CIMStatusCode.CIM_ERR_INVALID_QUERY",

    "Common.CIMStatusCode.CIM_ERR_METHOD_NOT_AVAILABLE",

    "Common.CIMStatusCode.CIM_ERR_METHOD_NOT_FOUND"
};

// l10n TODO - the first func should go away when all Pegasus is globalized

const char* cimStatusCodeToString(CIMStatusCode code)
{
    if (Uint32(code) < (sizeof(_cimMessageKeys)/sizeof(_cimMessageKeys[0])))
	return _cimMessages[Uint32(code)];

    return "Unrecognized CIM status code";
}


String cimStatusCodeToString(CIMStatusCode code,
			     const ContentLanguages& contentLanguages)
{
    MessageLoaderParms parms;

    if (Uint32(code) < (sizeof(_cimMessageKeys)/sizeof(_cimMessageKeys[0])))
    {
        parms = MessageLoaderParms(
            _cimMessageKeys[Uint32(code)], _cimMessages[Uint32(code)]);
    }
    else
    {
        parms = MessageLoaderParms(
            "Common.CIMStatusCode.UNRECOGNIZED_STATUS_CODE",
            "Unrecognized CIM status code \"$0\"", Uint32(code));
    }

    if (contentLanguages.size() > 0)
    {
        //build AcceptLanguages from contentLanguages, use in getMessage
        parms.acceptlanguages =
            AcceptLanguages(contentLanguages.getLanguageElement(0).getTag());
    }

    return MessageLoader::getMessage(parms);
}

// l10n 
ContentLanguages cimStatusCodeToString_Thread(String & message, CIMStatusCode code)
{
    MessageLoaderParms parms;

    if (Uint32(code) < (sizeof(_cimMessageKeys)/sizeof(_cimMessageKeys[0])))
    {
        parms = MessageLoaderParms(
            _cimMessageKeys[Uint32(code)], _cimMessages[Uint32(code)]);
    }
    else
    {
        parms = MessageLoaderParms(
            "Common.CIMStatusCode.UNRECOGNIZED_STATUS_CODE",
            "Unrecognized CIM status code \"$0\"", Uint32(code));
    }

    //parms.useThreadLocale = true;
    message = MessageLoader::getMessage(parms);
    return parms.contentlanguages;
}

PEGASUS_NAMESPACE_END
