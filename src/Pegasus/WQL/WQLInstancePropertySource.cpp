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
// Author: Adrian Schuur (schuur@de.ibm.com)
//
// Modified By:
//
//%/////////////////////////////////////////////////////////////////////////////

#include "WQLInstancePropertySource.h"
#include <Pegasus/Common/CIMName.h>
#include <Pegasus/Common/CIMInstance.h>

PEGASUS_NAMESPACE_BEGIN

Boolean WQLInstancePropertySource::getValue(
	const CIMName& propertyName, 
	WQLOperand& value) const
{
   unsigned int pos=ci.findProperty(propertyName);
   if (pos==PEG_NOT_FOUND) return false;
    
   CIMValue val=ci.getProperty(pos).getValue();
   CIMType type=val.getType();
   
   if (val.isNull()) {
      value=WQLOperand();
      return true;
   }
   if (val.isArray()) return false;
   
   switch (type) {
   case CIMTYPE_UINT8:
      Uint8 propertyValueUint8;
      val.get(propertyValueUint8);
      value=WQLOperand(propertyValueUint8,WQL_INTEGER_VALUE_TAG);
      break;

   case CIMTYPE_UINT16:
      Uint16 propertyValueUint16;
      val.get(propertyValueUint16);
      value=WQLOperand(propertyValueUint16, WQL_INTEGER_VALUE_TAG);
      break;

   case CIMTYPE_UINT32:
      Uint32 propertyValueUint32;
      val.get(propertyValueUint32);
      value=WQLOperand(propertyValueUint32, WQL_INTEGER_VALUE_TAG);
      break;

   case CIMTYPE_UINT64:
      Uint64 propertyValueUint64;
      val.get(propertyValueUint64);
      value=WQLOperand(propertyValueUint64, WQL_INTEGER_VALUE_TAG);
      break;

   case CIMTYPE_SINT8:
      Sint8 propertyValueSint8;
      val.get(propertyValueSint8);
      value=WQLOperand(propertyValueSint8, WQL_INTEGER_VALUE_TAG);
      break;

   case CIMTYPE_SINT16:
      Sint16 propertyValueSint16;
      val.get(propertyValueSint16);
      value=WQLOperand(propertyValueSint16, WQL_INTEGER_VALUE_TAG);
      break;

   case CIMTYPE_SINT32:
      Sint32 propertyValueSint32;
      val.get(propertyValueSint32);
      value=WQLOperand(propertyValueSint32, WQL_INTEGER_VALUE_TAG);
      break;

   case CIMTYPE_SINT64:
      Sint64 propertyValueSint64;
       val.get(propertyValueSint64);
       value=WQLOperand(propertyValueSint64, WQL_INTEGER_VALUE_TAG);
       break;

   case CIMTYPE_REAL32:
      Real32 propertyValueReal32;
      val.get(propertyValueReal32);
      value=WQLOperand(propertyValueReal32, WQL_DOUBLE_VALUE_TAG);
      break;

   case CIMTYPE_REAL64:
      Real64 propertyValueReal64;
      val.get(propertyValueReal64);
      value=WQLOperand(propertyValueReal64, WQL_DOUBLE_VALUE_TAG);
      break;

   case CIMTYPE_BOOLEAN :
      Boolean booleanValue;
      val.get(booleanValue);
      value=WQLOperand(booleanValue, WQL_BOOLEAN_VALUE_TAG);
      break;

   case CIMTYPE_CHAR16:
   case CIMTYPE_DATETIME :
   case CIMTYPE_STRING : {
         String strValue;
         val.get(strValue);
         value=WQLOperand(strValue,WQL_STRING_VALUE_TAG);
         break;
      }
   default: return false;   
   }
   return true;
 }	

PEGASUS_NAMESPACE_END
