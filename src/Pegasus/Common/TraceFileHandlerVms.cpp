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
// Author: Sushma Fernandes, Hewlett-Packard Company (sushma_fernandes@hp.com)
//
// Modified By:
//
//%/////////////////////////////////////////////////////////////////////////////

#include <fcntl.h>
#include <Pegasus/Common/Logger.h>
#include <Pegasus/Common/System.h>
#include <Pegasus/Common/TraceFileHandler.h>

PEGASUS_USING_STD;

PEGASUS_NAMESPACE_BEGIN

/////////////////////////////////////////////////////////////////////////////// 
//  Writes message to file. Locks the file before writing to it
//  Implementation of this function is platform specific
///////////////////////////////////////////////////////////////////////////////
void TraceFileHandler::handleMessage (
				       const char *message,
				       const char *fmt,
				       va_list argList)
{
  Sint32 retCode;
  Sint32 fileDesc;

  // Do not add Trace calls in the Critical section
  // ---- BEGIN CRITICAL SECTION

  // Check if the file has been deleted, if so re-open the file and continue
  if (!System::exists (_fileName))
  {
    if (_fileHandle == 0)
    {
      _fileHandle = fopen (_fileName, "a+", "shr=get,put,upd");
//          _fileHandle = fopen(_fileName,"a","shr=get");
    }
    else
    {
      _fileHandle = freopen (_fileName, "a+", _fileHandle, "shr=get,put,upd");
//          _fileHandle = freopen(_fileName,"a",_fileHandle,"shr=get");
    }
    if (!_fileHandle)
    {
      // Unable to re-open file, log a message

      Logger:: put_l (Logger:: DEBUG_LOG, System:: CIMSERVER, Logger::WARNING,
	     "Common.TraceFileHandlerVms.FAILED_TO_OPEN_FILE",
	     "Failed to open File $0", _fileName);
      return;
    }

    //
    // Set permissions on the trace file to 0400
    //

    if (!FileSystem::changeFilePermissions (String (_fileName), (S_IRUSR | S_IWUSR)))
    {
      Logger:: put_l (Logger:: DEBUG_LOG, "Tracer", Logger::WARNING,
	     "Common.TraceFileHandlerVms.FAILED_TO_SET_FILE_PERMISSIONS",
	     "Failed to set permissions on file $0", _fileName);
      return;
    }
  }
  // Seek to the end of File

  retCode = fseek (_fileHandle, 0, SEEK_END);

  // Write the message to the file

  retCode = fprintf (_fileHandle, "%s", message);
  retCode = vfprintf (_fileHandle, fmt, argList);
  retCode = fprintf (_fileHandle, "\n");
  retCode = fflush (_fileHandle);
  fileDesc = fileno (_fileHandle);
  retCode = fsync (fileDesc);
  _wroteToLog = false;
//        retCode = fclose(_fileHandle);
  //        _fileHandle = 0;

  // ---- END CRITICAL SECTION

  return;
}

PEGASUS_NAMESPACE_END
