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
// Author: Mike Brasher (mbrasher@bmc.com)
//
// Modified By:
//
//%/////////////////////////////////////////////////////////////////////////////

#ifndef Pegasus_WQLParserState_h
#define Pegasus_WQLParserState_h

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/ArrayInternal.h>
#include <Pegasus/WQL/WQLSelectStatement.h>

PEGASUS_NAMESPACE_BEGIN

/** This is an internal structure used for holding parser state information
    during parsing (while inside the WQLParser::parse() method). Instances
    of this class are created and destroyed by WQLParser::parse().
*/
struct WQLParserState
{
    //
    // Text containing SELECT statement (must be null-terminated).
    //

    const char* text;

    //
    // Size in bytes (including null terminator) of the text.
    //

    Uint32 textSize;

    //
    // Offset to current parser position in the above text.
    //

    Uint32 offset;

    //
    // The output object being initialized during parsing. This object is
    // set to point to the statement argument passed to the WQLParser::parse()
    // method.
    //

    WQLSelectStatement* statement;

    //
    // Set to true when there is an error (set by WQL_error()).
    //

    Boolean error;

    //
    // Error message if any (set by WQL_error()).
    //

    String errorMessage;

    //
    // A list of strings created by lexer. These are deleted by the
    // WQLParser when cleaning up in the parse() method. It is not possible
    // to reliably clean them up anywhere else due to potential parse errors
    // which prevent YACC actions (which might attempt to free them) from
    // being reached. These strings are outstanding in the sense that they
    // need to be cleaned up (not in the sense that they are excellent examples
    // of strings).
    //

    Array<char*> outstandingStrings;
};

PEGASUS_NAMESPACE_END

#endif /* Pegasus_WQLParserState_h */
