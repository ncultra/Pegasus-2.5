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
// Author: Uday Birajdar (uday@spsoftindia.com) 
//        
//
// Modified By: Swapnil Joshi (joshi.swapnil@spsoftindia.com)
//              Girish Dudhe (dudhe.girish@spsoftindia.com)
//		Mukesh Bafna (bafna.mukesh@spsoftindia.com)
//
//%////////////////////////////////////////////////////////////////////////////

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/System.h>
#include <Pegasus/Common/Logger.h>
#include "OperatingSystemProvider.h"

#include <sys/utsname.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

PEGASUS_USING_STD;

OperatingSystem::OperatingSystem(void)
{
}

OperatingSystem::~OperatingSystem(void)
{
}

static Boolean _getOSName(String& osName)
{
    struct utsname  unameInfo;

    // Call uname and check for any errors.
    if (uname(&unameInfo) < 0)
    {
       return false;
    }

    osName.assign(unameInfo.sysname);

    return true;
}

static Boolean getUtilGetHostName(String& csName)
{
     char    hostName[MAXHOSTNAMELEN];
     struct  hostent *he;

     if (gethostname(hostName, MAXHOSTNAMELEN) != 0)
     {
         return false;
     }

     // Now get the official hostname.  If this call fails then return
     // the value from gethostname().

     if (he=gethostbyname(hostName))
     {
        strcpy(hostName, he->h_name);
     }

     csName.assign(hostName);

     return true;
}

Boolean OperatingSystem::getCSName(String& csName)
{
return getUtilGetHostName(csName);
}

Boolean OperatingSystem::getName(String& osName)
{
	return _getOSName(osName);
}

Boolean OperatingSystem::getCaption(String& caption)
{
      caption.assign("The current Operating System");

     return true;
}

Boolean OperatingSystem::getDescription(String& description)
{
      description.assign("This instance reflects the Operating System"
           " on which the CIMOM is executing (as distinguished from instances"
           " of other installed operating systems that could be run).");

   return true;
}

Boolean OperatingSystem::getInstallDate(CIMDateTime& installDate)
{

    return false;
}

Boolean OperatingSystem::getStatus(String& status)
{

   status.assign("Unknown");

   return true;
}

Boolean OperatingSystem::getVersion(String& osVersion)
{
    struct utsname  unameInfo;

    // Call uname and check for any errors.

    if (uname(&unameInfo) < 0)
    {
       return false;
    }

    osVersion.assign(unameInfo.release);

    return true;
}

Boolean OperatingSystem::getOSType(Uint16& osType)
{
    osType = MACOS;

    return true;

}

Boolean OperatingSystem::getOtherTypeDescription(String& otherTypeDescription)
{
    struct utsname  unameInfo;
    char version[sizeof(unameInfo.release) + sizeof(unameInfo.version)];

    // Call uname and check for any errors.

    if (uname(&unameInfo) < 0)
    {
       return false;
    }

    sprintf(version, "%s %s", unameInfo.version, unameInfo.release);
    otherTypeDescription.assign(version);

    return true;
}

static CIMDateTime time_t_to_CIMDateTime(time_t *time_to_represent)
{
   const int CIM_DATE_TIME_ASCII_LEN = 256;
   const CIMDateTime NULLTIME;

   CIMDateTime dt;
   char date_ascii_rep[CIM_DATE_TIME_ASCII_LEN];
   char utc_offset[20];
   struct tm broken_time;
   struct timeval   tv;
   struct timezone  tz;

   dt = NULLTIME;
   localtime_r(time_to_represent, &broken_time);
   gettimeofday(&tv,&tz);
   if (strftime(date_ascii_rep, CIM_DATE_TIME_ASCII_LEN,
                "%Y%m%d%H%M%S.000000", &broken_time))
   {
      snprintf(utc_offset, 20, "%+03ld", -tz.tz_minuteswest);
      strncat(date_ascii_rep, utc_offset, CIM_DATE_TIME_ASCII_LEN);
      dt = String(date_ascii_rep);
   }

   return dt;
}

Boolean OperatingSystem::getLastBootUpTime(CIMDateTime& lastBootUpTime)
{
    return false;
}

Boolean OperatingSystem::getLocalDateTime(CIMDateTime& localDateTime)
{
   time_t now;

   now = time(NULL);
   localDateTime = time_t_to_CIMDateTime(&now);
   return true;
}

Boolean OperatingSystem::getCurrentTimeZone(Sint16& currentTimeZone)
{
    struct timeval   tv;
    struct timezone  tz;

    // Get the time from the system.
    gettimeofday(&tv,&tz);
    currentTimeZone = -tz.tz_minuteswest;
    return true;
}

Boolean OperatingSystem::getNumberOfLicensedUsers(Uint32& numberOfLicensedUsers)
{
    return false;
}

Boolean OperatingSystem::getNumberOfUsers(Uint32& numberOfUsers)
{
/*
    struct utmpx * utmpp;

    numberOfUsers = 0;

    while ((utmpp = getutxent()) != NULL)
    {
        if (utmpp->ut_type == USER_PROCESS)
        {
            numberOfUsers++;
        }
    }

    endutxent();
*/
	numberOfUsers =0;
    return true;
}

Boolean OperatingSystem::getNumberOfProcesses(Uint32& numberOfProcesses)
{
      return false;
}

Boolean OperatingSystem::getMaxNumberOfProcesses(Uint32& mMaxProcesses)
{
    return false; 
}

Boolean OperatingSystem::getTotalSwapSpaceSize(Uint64& mTotalSwapSpaceSize)
{
    return false;
}

Boolean OperatingSystem::getTotalVirtualMemorySize(Uint64& total)
{
    return false;
}

Boolean OperatingSystem::getFreeVirtualMemory(Uint64& freeVirtualMemory)
{
    return false;
}

Boolean OperatingSystem::getFreePhysicalMemory(Uint64& total)
{
    return false;
}

Boolean OperatingSystem::getTotalVisibleMemorySize(Uint64& memory)
{
    return false;
}

Boolean OperatingSystem::getSizeStoredInPagingFiles(Uint64& total)
{
    return false;
}

Boolean OperatingSystem::getFreeSpaceInPagingFiles(Uint64& freeSpaceInPagingFiles)
{
    return false;
}

Boolean OperatingSystem::getMaxProcessMemorySize(Uint64& maxProcessMemorySize)
{
    return false;
}

Boolean OperatingSystem::getDistributed(Boolean& distributed)
{
    distributed = false;
    return true;
}

Boolean OperatingSystem::getMaxProcsPerUser(Uint32& maxProcsPerUser)
{
    return false;
}

Boolean OperatingSystem::getSystemUpTime(Uint64& mUpTime)
{
    return false;
}

Boolean OperatingSystem::getOperatingSystemCapability(String& scapability)
{
    char capability[80];
    void *ptr;
    int ptr_bits;

    ptr_bits = 8*sizeof(ptr);

    sprintf (capability, "%d bit", ptr_bits);

    scapability.assign(capability);
    return true;

}

Uint32 OperatingSystem::_reboot()
{
   return false;
}

Uint32 OperatingSystem::_shutdown()
{
   return false;
}
