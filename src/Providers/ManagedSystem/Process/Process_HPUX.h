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
// Author: Christopher Neufeld <neufeld@linuxcare.com>
//         David Kennedy       <dkennedy@linuxcare.com>
//
// Modified By:
//         David Kennedy       <dkennedy@linuxcare.com>
//         Christopher Neufeld <neufeld@linuxcare.com>
//         Al Stone, Hewlett-Packard Company <ahs3@fc.hp.com>
//         Jim Metcalfe, Hewlett-Packard Company
//         Carlos Bonilla, Hewlett-Packard Company
//         Mike Glantz, Hewlett-Packard Company <michael_glantz@hp.com>
//
//%////////////////////////////////////////////////////////////////////////////

#ifndef PG_PROCESS_HPUX_H
#define PG_PROCESS_HPUX_H

/* ==========================================================================
   Includes.
   ========================================================================== */

#include <Pegasus/Provider/CIMInstanceProvider.h>
#include <sys/dirent.h>   // readdir
#include <time.h>         // localtime_r
#include <sys/param.h>    // pstat
#include <sys/pstat.h>    // pstat
#include <devnm.h>        // devnm
#include <sys/stat.h>     // stat
#include <sys/utsname.h>  // uname
#include <unistd.h>       // gethostname
#include <sys/socket.h>   // gethostbyname
#include <netinet/in.h>   // gethostbyname
#include <netdb.h>        // gethostbyname

PEGASUS_USING_STD;
PEGASUS_USING_PEGASUS;


/* ==========================================================================
   Type Definitions.
   ========================================================================== */

class Process
{
public:

  Process();
  ~Process();
  
  Boolean getCaption(String&) const;
  
  Boolean getDescription(String&) const;

  Boolean getInstallDate(CIMDateTime&) const;

  Boolean getStatus(String&) const;

  Boolean getName(String&) const;

  Boolean getPriority(Uint32&) const;

  Boolean getExecutionState(Uint16&) const;

  Boolean getOtherExecutionDescription(String&) const;

  Boolean getCreationDate(CIMDateTime&) const;

  Boolean getTerminationDate(CIMDateTime&) const;

  Boolean getKernelModeTime(Uint64&) const;

  Boolean getUserModeTime(Uint64&) const;

  Boolean getWorkingSetSize(Uint64&) const;

  Boolean getRealUserID(Uint64&) const;

  Boolean getProcessGroupID(Uint64&) const;

  Boolean getProcessSessionID(Uint64&) const;

  Boolean getProcessTTY(String&) const;

  Boolean getModulePath(String&) const;

  Boolean getParameters(Array<String>&) const;

  Boolean getProcessNiceValue(Uint32&) const;
  
  Boolean getProcessWaitingForEvent(String&) const;

  Boolean getCPUTime(Uint32&) const;

  Boolean getRealText(Uint64&) const;

  Boolean getRealData(Uint64&) const;

  Boolean getRealStack(Uint64&) const;

  Boolean getVirtualText(Uint64&) const;

  Boolean getVirtualData(Uint64&) const;

  Boolean getVirtualStack(Uint64&) const;

  Boolean getVirtualMemoryMappedFileSize(Uint64&) const;

  Boolean getVirtualSharedMemory(Uint64&) const;

  Boolean getCpuTimeDeadChildren(Uint64&) const;

  Boolean getSystemTimeDeadChildren(Uint64&) const;

  Boolean getParentProcessID(String&) const;

  Boolean getRealSpace(Uint64&) const;
  
  // Loads the internal process status structure with
  // the status data for the indexed process and, if
  // necessary (on HP-UX) updates pIndex to skip unused
  // entries so that a simple increment will allow next
  // call to access next entry
  Boolean loadProcessInfo(int &pIndex);
  
  // Finds the requested process and loads its info into
  // the internal process status structure
  Boolean findProcess(const String& handle);
  
  String getHandle(void) const;
  
  String getCurrentTime(void) const;

  String getOSName(void) const;
  
  String getCSName(void) const;
  
private:
  struct pst_status pInfo;
};


#endif  /* #ifndef PG_PROCESS_HPUX_H */
