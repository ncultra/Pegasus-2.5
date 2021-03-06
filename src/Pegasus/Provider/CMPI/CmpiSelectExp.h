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
// Modified By: Heidi Neuman, heidineu@de.ibm.com
//              Angel Nunez Mencias, anunez@de.ibm.com
//
//%/////////////////////////////////////////////////////////////////////////////

#ifndef _CmpiSelectExp_h_
#define _CmpiSelectExp_h_

#include "cmpidt.h"
#include "cmpift.h"

//#include "CmpiImpl.h"

#include "CmpiObject.h"
#include "CmpiStatus.h"
#include "CmpiObjectPath.h"
#include "CmpiBroker.h"
#include "CmpiData.h"
#include "Linkage.h"

class CmpiObjectPath;

/** This class represents the SelectExp of a CIM class. It is used manipulate
    SelectExps and their parts.
*/

class PEGASUS_CMPI_PROVIDER_LINKAGE CmpiSelectExp : public CmpiObject {
   friend class CmpiBroker;
   friend class CmpiResult;
   friend class CmpiIndicationMI;
protected:

   /** Protected constructor used by MIDrivers to encapsulate CMPISelectExp.
   */
   CmpiSelectExp(const CMPISelectExp* enc);

   /** Gets the encapsulated CMPISelectExp.
   */
   CMPISelectExp *getEnc() const;

private:

   /** Constructor - Should not be called
   */
   CmpiSelectExp();
public:
};

#endif
