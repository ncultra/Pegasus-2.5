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
// Author: Willis White (whiwill@us.ibm.com)
//
// Modified By: 
//
//
//
//%/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cassert>
#include <Pegasus/Common/StatisticalData.h>

PEGASUS_USING_STD;
PEGASUS_USING_PEGASUS;

int main(int argc, char** argv)
{


StatisticalData* sd = StatisticalData::current();
StatisticalData* curr = StatisticalData::current();
//cur = StatisticalData::current();

//check to make sure current() returns a pointer to the existing StatisticalData
// object

assert(sd->length == curr->length);
assert(sd->requestSize == curr->requestSize);
assert(sd->copyGSD == curr->copyGSD);
assert(sd->numCalls[5] == curr->numCalls[5]);
assert(sd->requestSize[6] == curr->requestSize[6]);


//  *****************************************
// check the addToValue() method

// Changes sd.numCalls[5] from 0 to 10
sd->addToValue(10,5,StatisticalData::PEGASUS_STATDATA_SERVER);

// Changes sd.requestSize[6] form 0 to 10
sd->addToValue(10,6,StatisticalData::PEGASUS_STATDATA_BYTES_READ);

//assert(sd->numCalls[5] == 0);
//assert(sd->requestSize[6] == 10);

//**********************************************
// check the setCopyGSD method

sd->setCopyGSD(1);

assert(sd->copyGSD == 1);

//****************************************************
// make sure the cur the sd objects are still the same

assert(sd->length == curr->length);
assert(sd->requestSize == curr->requestSize);
assert(sd->copyGSD == curr->copyGSD);
assert(sd->numCalls[5] == curr->numCalls[5]);
assert(sd->requestSize[6] == curr->requestSize[6]);


//**************************

    cout << argv[0] << " +++++ passed all tests" << endl;

    return 0;
}

