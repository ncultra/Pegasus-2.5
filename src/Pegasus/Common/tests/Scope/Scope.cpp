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
// Author: Carol Ann Krug Graves, Hewlett-Packard Company 
//         (carolann_graves@hp.com)
//
// Modified By: 
//
//%/////////////////////////////////////////////////////////////////////////////

//
//  This test module tests the functions associated with the CIMScope class.
//

#include <cassert>
#include <Pegasus/Common/CIMScope.h>
#include <Pegasus/Common/Exception.h>

PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;

static char * verbose;

void test01 ()
{
    //
    //  Test default constructor
    //
    CIMScope s0;

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s0: " << s0.toString () << PEGASUS_STD (endl);
    }
    assert (s0.toString () == String::EMPTY);

    CIMScope s1 = CIMScope ();

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s1: " << s1.toString () << PEGASUS_STD (endl);
    }
    assert (s1.toString () == String::EMPTY);

    //
    //  Test Uint32 constructor
    //
    CIMScope s2 = CIMScope (CIMScope::NONE);

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s2: " << s2.toString () << PEGASUS_STD (endl);
    }
    assert (s2.toString () == String::EMPTY);

    CIMScope s3 = CIMScope (CIMScope::PROPERTY);

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s3: " << s3.toString () << PEGASUS_STD (endl);
    }
    assert (s3.toString () == "PROPERTY");

    CIMScope s4 = CIMScope (CIMScope::ANY);

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s4: " << s4.toString () << PEGASUS_STD (endl);
    }
    assert (s4.toString () == 
        "CLASS ASSOCIATION INDICATION PROPERTY REFERENCE METHOD PARAMETER");

    CIMScope s5 = CIMScope (CIMScope::CLASS + CIMScope::ASSOCIATION);

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s5: " << s5.toString () << PEGASUS_STD (endl);
    }
    assert (s5.toString () == "CLASS ASSOCIATION");

    //
    //  Test copy constructor
    //
    CIMScope s6 = CIMScope (s1);

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s6: " << s6.toString () << PEGASUS_STD (endl);
    }
    assert (s6.toString () == String::EMPTY);

    CIMScope s7 = CIMScope (s2);

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s7: " << s7.toString () << PEGASUS_STD (endl);
    }
    assert (s7.toString () == String::EMPTY);

    CIMScope s8 = CIMScope (s3);

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s8: " << s8.toString () << PEGASUS_STD (endl);
    }
    assert (s8.toString () == "PROPERTY");

    CIMScope s9 = CIMScope (s4);

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s9: " << s9.toString () << PEGASUS_STD (endl);
    }
    assert (s9.toString () == 
        "CLASS ASSOCIATION INDICATION PROPERTY REFERENCE METHOD PARAMETER");

    CIMScope s10 = CIMScope (s5);

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s10: " << s10.toString () << PEGASUS_STD (endl);
    }
    assert (s10.toString () == "CLASS ASSOCIATION");

    //
    //  Test assignment operator
    //
    s0 = s4;

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s0: " << s0.toString () << PEGASUS_STD (endl);
    }
    assert (s0.toString () == 
        "CLASS ASSOCIATION INDICATION PROPERTY REFERENCE METHOD PARAMETER");

    s0 = s3;

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s0: " << s0.toString () << PEGASUS_STD (endl);
    }
    assert (s0.toString () == "PROPERTY");

    //
    //  Test equal (CIMScope) 
    //
    assert (s1.equal (s2));
    assert (s3.equal (s0));
    assert (!(s1.equal (s5)));
    assert (!(s9.equal (s10)));

    //
    //  Test addScope (Uint32) 
    //
    s0.addScope (CIMScope::INDICATION);

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s0: " << s0.toString () << PEGASUS_STD (endl);
    }
    assert (s0.toString () == "INDICATION PROPERTY");

    s7.addScope (CIMScope::METHOD + CIMScope::PARAMETER + CIMScope::REFERENCE);

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s7: " << s7.toString () << PEGASUS_STD (endl);
    }
    assert (s7.toString () == "REFERENCE METHOD PARAMETER");

    s4.addScope (CIMScope::METHOD);

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s4: " << s4.toString () << PEGASUS_STD (endl);
    }
    assert (s4.toString () == 
        "CLASS ASSOCIATION INDICATION PROPERTY REFERENCE METHOD PARAMETER");

    s10.addScope (CIMScope::NONE);

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s10: " << s10.toString () << PEGASUS_STD (endl);
    }
    assert (s10.toString () == "CLASS ASSOCIATION");

    s2.addScope (CIMScope::ANY);

    if (verbose)
    {
	PEGASUS_STD (cout) << "\n----------------------\n";
        PEGASUS_STD (cout) << "s2: " << s2.toString () << PEGASUS_STD (endl);
    }
    assert (s2.toString () == 
        "CLASS ASSOCIATION INDICATION PROPERTY REFERENCE METHOD PARAMETER");

    //
    //  Test hasScope (Uint32) 
    //
    assert (s0.hasScope (CIMScope::INDICATION));
    assert (s0.hasScope (CIMScope::INDICATION + CIMScope::PROPERTY));
    assert (!(s0.hasScope (CIMScope::ASSOCIATION)));
    assert (!(s0.hasScope (CIMScope::ASSOCIATION + CIMScope::CLASS)));
    assert (!(s0.hasScope (CIMScope::ANY)));

    assert (s7.hasScope (CIMScope::PARAMETER));
    assert (s7.hasScope (CIMScope::METHOD + CIMScope::PARAMETER));
    assert (!(s7.hasScope (CIMScope::PROPERTY)));
    assert (!(s7.hasScope (CIMScope::PROPERTY + CIMScope::CLASS)));
    assert (!(s7.hasScope (CIMScope::ANY)));

    assert (s4.hasScope (CIMScope::ANY));
    assert (s4.hasScope (CIMScope::CLASS));
    assert (s4.hasScope (CIMScope::CLASS + CIMScope::PARAMETER));

    //
    //  Test hasScope (CIMScope) 
    //
    assert (s0.hasScope (s8));
    assert (s4.hasScope (s9));
    assert (!(s0.hasScope (s7)));
    assert (!(s0.hasScope (s4)));
    assert (!(s7.hasScope (s5)));

    //
    //  Test toString ()
    //
    String ss1 = s1.toString ();
    assert (ss1 == String::EMPTY);

    String ss3 = s3.toString ();
    assert (ss3 == "PROPERTY");

    String ss7 = s7.toString ();
    assert (ss7 == "REFERENCE METHOD PARAMETER");

    String ss4 = s4.toString ();
    assert (ss4 == 
        "CLASS ASSOCIATION INDICATION PROPERTY REFERENCE METHOD PARAMETER");
}

int main (int argc, char** argv)
{
    verbose = getenv ("PEGASUS_TEST_VERBOSE");

    try
    {
        test01 ();

        PEGASUS_STD (cout) << argv [0] << " +++++ passed all tests" 
                           << PEGASUS_STD (endl);
    }
    catch (Exception & e)
    {
        PEGASUS_STD (cerr) << argv [0] << " Exception " << e.getMessage () 
                           << PEGASUS_STD (endl);
        exit (1);
    }
    return 0;
}
