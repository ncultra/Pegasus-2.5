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
// Modified By:
//
//%/////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include "WQLOperand.h"

PEGASUS_NAMESPACE_BEGIN

WQLOperand::WQLOperand() : _type(NULL_VALUE) 
{ 

}

WQLOperand::WQLOperand(const WQLOperand& x)
{
    assign(x);
}

WQLOperand::~WQLOperand()
{
    clear();
}

WQLOperand& WQLOperand::operator=(const WQLOperand& x)
{
    clear();
    assign(x);
    return *this;
}

void WQLOperand::clear()
{
    if (_type == PROPERTY_NAME)
	((String*)_propertyName)->~String();
    else if (_type == STRING_VALUE)
	((String*)_stringValue)->~String();

    _type = NULL_VALUE;
}

void WQLOperand::assign(const WQLOperand& x)
{
    switch (_type = x._type)
    {
	case PROPERTY_NAME:
	    new(_propertyName) String(*((String*)x._propertyName));
	    break;

	case STRING_VALUE:
	    new(_stringValue) String(*((String*)x._stringValue));
	    break;

	case INTEGER_VALUE:
	    _integerValue = x._integerValue;
	    break;

	case DOUBLE_VALUE:
	    _doubleValue = x._doubleValue;
	    break;

	case BOOLEAN_VALUE:
	    _booleanValue = x._booleanValue;
	    break;

	case NULL_VALUE:
	    _integerValue = 0;
	    break;
    }
}

String WQLOperand::toString() const
{
    String result;

    switch (_type)
    {
	case PROPERTY_NAME:
	{
	    result = "PROPERTY_NAME: ";
	    result.append(*((String*)_propertyName));
	    break;
	}

	case STRING_VALUE:
	{
	    result = "STRING_VALUE: ";
	    result.append(*((String*)_stringValue));
	    break;
	}

	case INTEGER_VALUE:
	{
	    result = "INTEGER_VALUE: ";
	    char buffer[32];
	    sprintf(buffer, "%" PEGASUS_64BIT_CONVERSION_WIDTH "d", 
                    _integerValue);
	    result.append(buffer);
	    break;
	}

	case DOUBLE_VALUE:
	{
	    result = "DOUBLE_VALUE: ";
	    char buffer[32];
	    sprintf(buffer, "%f", _doubleValue);
	    result.append(buffer);
	    break;
	} 

	case BOOLEAN_VALUE:
	{
	    result = "BOOLEAN_VALUE: ";

	    if (_booleanValue)
		result.append("TRUE");
	    else
		result.append("FALSE");

	    break;
	}

	default:
	    result = "NULL_VALUE";
	    break;
    }

    return result;
}

PEGASUS_NAMESPACE_END
