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
// Author: Paulo F. Borges (pfborges@wowmail.com)
//
// Modified By: 
//         Sean Keenan, Hewlett-Packard Company <sean.keenan@hp.com>
//
//%/////////////////////////////////////////////////////////////////////////////

#ifndef Pegasus_Security_h
#define Pegasus_Security_h

//------------------------------------------------------------------------------
// INCLUDES
//------------------------------------------------------------------------------
//Pegasus includes
#include <Pegasus/Common/Config.h>
#include <Pegasus/Provider/CIMInstanceProvider.h>
#include <Pegasus/Provider/CIMMethodProvider.h>
#include <Pegasus/Provider/ProviderException.h>    
#include <Pegasus/Common/Constants.h>
#include <Pegasus/Common/CIMMessage.h>
#include <Pegasus/Common/OperationContext.h>
#include <Pegasus/Common/System.h>

// Security includes
#if !defined (PEGASUS_OS_VMS)
#include <sys/getaccess.h>
#endif
#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
    
//------------------------------------------------------------------------------ 
PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;

// Options operation to verify access
static const String SEC_OPT_READ("r");
static const String SEC_OPT_WRITE("w");
static const String SEC_OPT_READ_WRITE("rw");
static const String SEC_OPT_EXECUTE("x");
static const String SEC_OPT_ALL("rwx");

// Parameter option to Operation context
static const Uint32 CONTEXT_ID = 1;

//------------------------------------------------------------------------------
// Class [NTPProviderSecurity] Definition
//------------------------------------------------------------------------------
class NTPProviderSecurity
{
    
public:
    NTPProviderSecurity(const OperationContext & context);
    virtual ~NTPProviderSecurity(void);

public:
    //
    // Public Functions - Interface
    //

	// This function retrieves the permissions of file by username
	Boolean checkAccess(const String filename,
    		            const String chkoper);

private:
    //
    // Class attributes 
    //
    String secUsername;
};

#endif
