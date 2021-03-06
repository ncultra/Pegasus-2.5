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
// Author: Nag Boranna, Hewlett-Packard Company (nagaraja_boranna@hp.com)
//
// Modified By:
//
//%/////////////////////////////////////////////////////////////////////////////

#include <Pegasus/Common/Config.h>
#include <cassert>
#include <iostream>
#include <Pegasus/Common/System.h>
#include <Pegasus/Common/Base64.h>
#include <Pegasus/Common/AuthenticationInfo.h>
#include <Pegasus/Config/ConfigManager.h>
#include <Pegasus/Security/Authentication/LocalAuthenticationHandler.h>

//
// Enable debug messages
//
//#define DEBUG 1


PEGASUS_USING_PEGASUS;

PEGASUS_USING_STD;

Boolean verbose = false;

String authType = "Local";

String testUser = System::getEffectiveUserName();

String challenge = String::EMPTY;

String filePath = String::EMPTY;

AuthenticationInfo* authInfo = 0;

void testAuthHeader()
{
    LocalAuthenticationHandler  localAuthHandler;

    String respHeader = 
        localAuthHandler.getAuthResponseHeader(authType, testUser, authInfo);

#ifdef DEBUG
    if (verbose) cout << "respHeader= " << respHeader << endl;
#endif
    
    challenge = authInfo->getAuthChallenge();

    PEGASUS_ASSERT(respHeader.size() != 0);

    Uint32 startQuote = respHeader.find(0, '"');
    assert(startQuote != PEG_NOT_FOUND);

    Uint32 endQuote = respHeader.find(startQuote + 1, '"');
    assert(startQuote != PEG_NOT_FOUND);

    filePath = respHeader.subString(startQuote + 1, (endQuote - startQuote - 1));

    PEGASUS_ASSERT(filePath.size() != 0);
}

//
// Test with invalid userPass
//
void testAuthenticationFailure_1()
{
    String authHeader = String::EMPTY;
    Boolean authenticated;

    LocalAuthenticationHandler  localAuthHandler;

    //
    // Test with invalid auth header
    //
    authHeader = testUser;
    authHeader.append(filePath);
    authHeader.append(challenge);

    authenticated = localAuthHandler.authenticate(authHeader, authInfo);

    if (verbose) cout << "authHeader: " << authHeader << endl;

    if (authenticated)
        if (verbose) cout << "User " + testUser + " authenticated successfully." << endl;
    else
        if (verbose) cout << "User " + testUser + " authentication failed.." << endl;

    PEGASUS_ASSERT(!authenticated);
}

//
// Test with invalid system user
//
void testAuthenticationFailure_2()
{
    String authHeader = String::EMPTY;
    Boolean authenticated;

    LocalAuthenticationHandler  localAuthHandler;

    //
    // Test with invalid auth header
    //
    authHeader = testUser;
    authHeader.append(filePath);

    authenticated = localAuthHandler.authenticate(authHeader, authInfo);

    if (verbose) cout << "authHeader: " << authHeader << endl;

    if (authenticated)
        if (verbose) cout << "User " + testUser + " authenticated successfully." << endl;
    else
        if (verbose) cout << "User " + testUser + " authentication failed.." << endl;

    PEGASUS_ASSERT(!authenticated);
}

//
// Test with invalid password 
//
void testAuthenticationFailure_3()
{
    String authHeader = String::EMPTY;
    Boolean authenticated;

    LocalAuthenticationHandler  localAuthHandler;

    authHeader = testUser;
    authHeader.append(":");
    authHeader.append(filePath);
    authHeader.append(":");

    authenticated = localAuthHandler.authenticate(authHeader, authInfo);

    if (verbose) cout << "authHeader: " << authHeader << endl;

    if (authenticated)
        if (verbose) cout << "User " + testUser + " authenticated successfully." << endl;
    else
        if (verbose) cout << "User " + testUser + " authentication failed.." << endl;

    PEGASUS_ASSERT(!authenticated);
}

//
// Test with invalid CIM user or invalid password
//
void testAuthenticationFailure_4()
{
    String authHeader = String::EMPTY;
    Boolean authenticated;

    LocalAuthenticationHandler  localAuthHandler;

    authHeader = testUser;
    authHeader.append(":");
    authHeader.append(filePath);
    authHeader.append(":");
    authHeader.append("asd442394asd");

    authenticated = localAuthHandler.authenticate(authHeader, authInfo);

    if (verbose) cout << "authHeader: " << authHeader << endl;

    if (authenticated)
        if (verbose) cout << "User " + testUser + " authenticated successfully." << endl;
    else
        if (verbose) cout << "User " + testUser + " authentication failed.." << endl;

    PEGASUS_ASSERT(!authenticated);
}

//
// Test with valid user name and password 
//
void testAuthenticationSuccess()
{
    String authHeader = String::EMPTY;

    LocalAuthenticationHandler  localAuthHandler;

    authHeader = testUser;
    authHeader.append(":");
    authHeader.append(filePath);
    authHeader.append(":");
    authHeader.append(challenge);

    Boolean authenticated =
        localAuthHandler.authenticate(authHeader, authInfo);

    if (verbose) cout << "authHeader: " << authHeader << endl;

    if (authenticated)
        if (verbose) cout << "User " + testUser + " authenticated successfully." << endl;
    else
        if (verbose) cout << "User " + testUser + " authentication failed.." << endl;

    PEGASUS_ASSERT(authenticated);
}

////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    verbose = (getenv ("PEGASUS_TEST_VERBOSE")) ? true : false;
    if (verbose) cout << argv[0] << ": started" << endl;

#if defined(PEGASUS_OS_TYPE_UNIX)

    try
    {
#ifdef DEBUG
        Tracer::setTraceFile("/tmp/trace");
        Tracer::setTraceComponents("all");
	verbose = true;
#endif

        ConfigManager* configManager = ConfigManager::getInstance();

        const char* path = getenv("PEGASUS_HOME");
        String pegHome = path;

        if(pegHome.size())
            ConfigManager::setPegasusHome(pegHome);

        if (verbose) cout << "Peg Home : " << ConfigManager::getPegasusHome() << endl;
        authInfo = new AuthenticationInfo(true);

        if (verbose) cout << "Doing testAuthHeader()...." << endl;
        testAuthHeader();

        if (verbose) cout << "Doing testAuthenticationFailure_1()...." << endl;
        testAuthenticationFailure_1();

        if (verbose) cout << "Doing testAuthenticationFailure_2()...." << endl;
        testAuthenticationFailure_2();

        if (verbose) cout << "Doing testAuthenticationFailure_3()...." << endl;
        testAuthenticationFailure_3();

        if (verbose) cout << "Doing testAuthenticationFailure_4()...." << endl;
        testAuthenticationFailure_4();

        if (verbose) cout << "Doing testAuthenticationSuccess()...." << endl;
        testAuthenticationSuccess();

    }
    catch(Exception& e)
    {
      cout << argv[0] << "Exception: " << e.getMessage() << endl;
        PEGASUS_ASSERT(0);
    }

    delete authInfo;
#endif

    cout << argv[0] << " +++++ passed all tests" << endl;

    return 0;
}
