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
// Author: Tony Fiorentino (fiorentino_tony@emc.com)
//
// Modified By: Keith Petley (keithp@veritas.com)
//
//%/////////////////////////////////////////////////////////////////////////////

#include "CIMServerDescription.h"

PEGASUS_NAMESPACE_BEGIN

CIMServerDescription::CIMServerDescription()
{
}

CIMServerDescription::CIMServerDescription(const String & url)
    : _serviceLocationTcp(url), _port(PEG_NOT_FOUND)
{
}

CIMServerDescription::~CIMServerDescription()
{
}

String CIMServerDescription::getUrl()
{
  return _serviceLocationTcp;
}

String CIMServerDescription::getValue(
    const String & attributeName,
    const String & defaultValue) const
{
    Array<String> vals;
    if (getValues(attributeName, vals) == false || vals.size() == 0)
    {
        return defaultValue;
    }
    else
    {
        return vals[0];
    }
}

Boolean CIMServerDescription::getValues(
    const String& attributeName,
    Array<String>& attributeValue) const
{
    // find the attribute entry
    for (Uint32 idx=0; idx<_attributes.size(); idx++)
    {
        if (String::equalNoCase(_attributes[idx].getTag(), attributeName))
        {
            attributeValue = _attributes[idx].getValues();
            return true;
        }
    }

    return false;
}

Array<Attribute> CIMServerDescription::getAttributes() const
{
    return _attributes;
}

void CIMServerDescription::setAttributes(const Array<Attribute>& attributes)
{
    _attributes = attributes;
}

#define PEGASUS_ARRAY_T CIMServerDescription
#include <Pegasus/Common/ArrayImpl.h>
#undef PEGASUS_ARRAY_T

PEGASUS_NAMESPACE_END
