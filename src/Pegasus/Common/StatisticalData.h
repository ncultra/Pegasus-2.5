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
// Author: Arthur Pichlkostner
//             (checked in: Markus Mueller sedgewick_de@yahoo.de)
//
// Modified By:
//
//%/////////////////////////////////////////////////////////////////////////////

#ifndef STATISTICAL_DATA_H
#define STATISTICAL_DATA_H

#include <Pegasus/Common/Config.h>
#include <iostream>
#include <cstring>
#include <Pegasus/Common/InternalException.h>
#include <Pegasus/Common/IPC.h>
#include <Pegasus/Common/String.h>
#include <Pegasus/Common/CIMProperty.h>
#include <Pegasus/Common/CIMInstance.h>
#include <Pegasus/Common/CIMDateTime.h>
#include <Pegasus/Common/Linkage.h>

PEGASUS_NAMESPACE_BEGIN

#ifndef PEGASUS_DISABLE_PERFINST

#define STAT_GETSTARTTIME \
TimeValue startTime = TimeValue::getCurrentTime();


#define STAT_PMS_PROVIDEREND \
response->endProvider();            \
response->setStartServerTime(request->getStartServerTime()); \
response->setStartProviderTime(startTime);


#define STAT_SERVERSTART \
request->setStartServerTime(startTime);


#define STAT_SERVEREND \
response->endServer();\

#define STAT_BYTESSENT \
Uint16 statType = (response->getType() >= CIM_GET_CLASS_RESPONSE_MESSAGE)? \
  response->getType() - CIM_GET_CLASS_RESPONSE_MESSAGE:response->getType() - 1;\
StatisticalData::current()->addToValue(message.size(), statType, StatisticalData::PEGASUS_STATDATA_BYTES_SENT);



#define STAT_SERVEREND_ERROR \
response->endServer();


#define STAT_PROVIDERSTART \
request->startProvider();


#define STAT_PROVIDEREND   \
request->endProvider();

#define STAT_COPYDISPATCHER \
response->setStartServerTime(request->getStartServerTime());



// copy request timing info into response
#define STAT_COPYDISPATCHER_REP \
response->setStartServerTime(request->getStartServerTime());   \
response->setStartProviderTime(request->getStartProviderTime());   \
response->setEndProviderTime(request->getEndProviderTime());



/*the request size value must be stored (requSize) and passed to the StatisticalData object at the
 end of processing other wise it will be the ONLY vlaue that is passed to the client which reports 
 the current state of the object, not the pevious (one command ago) state */

#define STAT_BYTESREAD \
Uint16 statType = (request->getType() >= CIM_GET_CLASS_RESPONSE_MESSAGE)? \
    request->getType() - CIM_GET_CLASS_RESPONSE_MESSAGE: request->getType()-1;\
StatisticalData::current()->requSize = contentLength; \
StatisticalData::current()->addToValue(contentLength, statType, StatisticalData::PEGASUS_STATDATA_BYTES_READ);





//serverResponseTime should be an Uint64 but the << operator wont accept that
#define STAT_SERVERTIME \
if (StatisticalData::current()->copyGSD)\
	 out << "WBEMServerResponseTime: " << (Uint32) serverResponseTime << "\r\n";
#else
#define STAT_GETSTARTTIME
#define STAT_PMS_PROVIDEREND
#define STAT_SERVERSTART
#define STAT_SERVEREND
#define STAT_SERVEREND_ERROR
#define STAT_PROVIDERSTART
#define STAT_PROVIDEREND
#define STAT_PROVIDEREND_REP
#define STAT_COPYDISPATCHER
#define STAT_COPYDISPATCHER_REP
#define STAT_BYTESREAD
#define STAT_SERVERTIME
#define STAT_BYTESSENT
#endif

class PEGASUS_COMMON_LINKAGE StatisticalData
{
   public:
      enum StatRequestType{
         GET_CLASS,
         GET_INSTANCE,
	 EXPORT_INDICATION,
         DELETE_CLASS,
         DELETE_INSTANCE,
         CREATE_CLASS,
         CREATE_INSTANCE,
         MODIFY_CLASS,
         MODIFY_INSTANCE,
         ENUMERATE_CLASSES,
         ENUMERATE_CLASS_NAMES,
         ENUMERATE_INSTANCES,
         ENUMERATE_INSTANCE_NAMES,
         EXEC_QUERY,
         ASSOCIATORS,
         ASSOCIATOR_NAMES,
         REFERENCES,
         REFERENCE_NAMES,
         GET_PROPERTY,
         SET_PROPERTY,
         GET_QUALIFIER,
         SET_QUALIFIER,
         DELETE_QUALIFIER,
         ENUMERATE_QUALIFIERS,
         INVOKE_METHOD,
         NUMBER_OF_TYPES
      };

      enum StatDataType{
         PEGASUS_STATDATA_SERVER,
         PEGASUS_STATDATA_PROVIDER,
         PEGASUS_STATDATA_BYTES_SENT,
         PEGASUS_STATDATA_BYTES_READ
      };

      static const Uint32 length;
      static StatisticalData* current();

      StatisticalData();

      timeval timestamp;

      Sint64 numCalls[NUMBER_OF_TYPES];
      Sint64 cimomTime[NUMBER_OF_TYPES];
      Sint64 providerTime[NUMBER_OF_TYPES];
      Sint64 responseSize[NUMBER_OF_TYPES];
      Sint64 requestSize[NUMBER_OF_TYPES];
	Sint64 requSize;	//tempory storage for requestSize vlaue
	Boolean copyGSD;
//	Uint64 totalServTime;
      static StatisticalData* cur;
      void addToValue(Sint64 value, Uint16 type, Uint32 t);
      static String requestName[];
     void setCopyGSD(Boolean flag);

   protected:
      Mutex _mutex;
};


PEGASUS_NAMESPACE_END
#endif

