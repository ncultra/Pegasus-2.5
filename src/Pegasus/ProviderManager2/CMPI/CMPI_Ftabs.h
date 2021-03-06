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
// Author:      Adrian Schuur, schuur@de.ibm.com
//
// Modified By:
//
//%/////////////////////////////////////////////////////////////////////////////


#ifndef _CMPI_Ftabs_H_
#define _CMPI_Ftabs_H_

#include <iostream>
#include <stdlib.h>

#include <Pegasus/Provider/CMPI/cmpidt.h>
#include <Pegasus/Provider/CMPI/cmpift.h>

#include <Pegasus/Common/CIMType.h>

PEGASUS_NAMESPACE_BEGIN

extern CMPIInstanceFT *CMPI_Instance_Ftab;
extern CMPIInstanceFT *CMPI_InstanceOnStack_Ftab;

extern CMPIObjectPathFT *CMPI_ObjectPath_Ftab;
extern CMPIObjectPathFT *CMPI_ObjectPathOnStack_Ftab;

extern CMPIArgsFT *CMPI_Args_Ftab;
extern CMPIArgsFT *CMPI_ArgsOnStack_Ftab;

extern CMPIContextFT *CMPI_Context_Ftab;
extern CMPIContextFT *CMPI_ContextOnStack_Ftab;

extern CMPIResultFT *CMPI_ResultRefOnStack_Ftab;
extern CMPIResultFT *CMPI_ResultInstOnStack_Ftab;
extern CMPIResultFT *CMPI_ResultData_Ftab;
extern CMPIResultFT *CMPI_ResultMethOnStack_Ftab;
extern CMPIResultFT *CMPI_ResultResponseOnStack_Ftab;
extern CMPIResultFT *CMPI_ResultExecQueryOnStack_Ftab;

extern CMPIDateTimeFT *CMPI_DateTime_Ftab;
extern CMPIArrayFT *CMPI_Array_Ftab;
extern CMPIStringFT *CMPI_String_Ftab;

extern CMPISelectExpFT *CMPI_SelectExp_Ftab;
extern CMPISelectCondFT *CMPI_SelectCond_Ftab;
extern CMPISubCondFT *CMPI_SubCond_Ftab;
extern CMPIPredicateFT *CMPI_Predicate_Ftab;

extern CMPIBrokerFT *CMPI_Broker_Ftab;
extern CMPIBrokerEncFT *CMPI_BrokerEnc_Ftab;
extern CMPIBrokerExtFT *CMPI_BrokerExt_Ftab;

extern CMPIEnumerationFT *CMPI_ObjEnumeration_Ftab;
extern CMPIEnumerationFT *CMPI_InstEnumeration_Ftab;
extern CMPIEnumerationFT *CMPI_OpEnumeration_Ftab;

PEGASUS_NAMESPACE_END

#endif
