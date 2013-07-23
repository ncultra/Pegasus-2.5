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
// Author: Nitin Upasani, Hewlett-Packard Company (Nitin_Upasani@hp.com)
//
// Modified By: Yi Zhou, Hewlett-Packard Company (yi_zhou@hp.com)
//
//%/////////////////////////////////////////////////////////////////////////////

#include <Pegasus/Common/Config.h>
#include <Pegasus/Provider/CIMIndicationConsumerProvider.h>

#include "RT_IndicationConsumer.h"

PEGASUS_USING_PEGASUS;

PEGASUS_USING_STD;


RT_IndicationConsumer::RT_IndicationConsumer()
{
}

RT_IndicationConsumer::~RT_IndicationConsumer()
{
}

void RT_IndicationConsumer::initialize(CIMOMHandle& handle)
{
}

void RT_IndicationConsumer::terminate()
{
}

void RT_IndicationConsumer::consumeIndication(
   const OperationContext & context,
   const String& url,
   const CIMInstance& indicationInstance)
{
    String indicationID;
    String indicationTime;
    String methodName;

    //
    // get IndicationID
    //
    try
    {

        indicationID = indicationInstance.getProperty(
                            indicationInstance.findProperty(
                            PROPERTY_NAME_INDICATIONID)).getValue().toString();
     }
     catch (...)
     {
        indicationID = UNKNOWN;
     }

     //
     // get IndicationTime 
     //
     try
     {
        indicationTime = indicationInstance.getProperty(
                        indicationInstance.findProperty(
                        PROPERTY_NAME_INDICATIONTIME)).getValue().toString();
     }
     catch (...)
     {
        indicationTime = UNKNOWN;
     }

     //
     // get MethodName
     //
     try
     {
        indicationInstance.getProperty(indicationInstance.findProperty(
                        PROPERTY_NAME_METHODNAME)).getValue().get(methodName);
     }
     catch (...)
     {
        methodName = UNKNOWN;
     }

    cout << "IndicationID = " << indicationID << ", IndicationTime = " <<
        indicationTime << ", MethodName = " << methodName << endl;
}
