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
// Author: David Dillard, VERITAS Software Corp. (david.dillard@veritas.com)
//
//%=============================================================================

#include <iostream>
#include "Files.h"
#include "CopyCmd.h"
#ifdef _WIN32_WINNT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

int SleepCmd(const vector<string>& args)
{

    if (args.size() < 2)
    {
        cerr << args[0] << ": wrong number of arguments" << endl;
        return(1);
    }

    unsigned seconds;
    char magnitude, dummy;
    unsigned count = sscanf(args[1].c_str(), "%u%c%c", &seconds, &magnitude, &dummy);

    if (count == 1)
    {
        magnitude = 's';
    }

    if ((count != 1) && (count != 2))
    {
        cerr << args[0] << ": invalid number of seconds" << endl;
        return(1);
    }

    switch (magnitude)
    {
        case 's':
            break;

        case 'm':
            seconds *= 60;
            break;

        case 'h':
            seconds *= 3600;
            break;

        default:
            cerr << args[0] << ": invalid time magnitude \'" << magnitude << "\'" << endl;
            return(2);
    }

#ifdef _WIN32_WINNT
    Sleep(seconds * 1000);
#else
    while (seconds > 0)
    {
        seconds = sleep(seconds);
    }
#endif

    return(0);
}
