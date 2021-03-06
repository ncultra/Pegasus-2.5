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
// Author: Konrad Rzeszutek <konradr@us.ibm.com>
//
//%/////////////////////////////////////////////////////////////////////////////

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/Constants.h>
#include <Pegasus/Common/CIMType.h>
#include <Pegasus/Common/CIMName.h>
#include <Pegasus/Common/CIMInstance.h>
#include <Pegasus/Common/CIMProperty.h>
#include <Pegasus/Common/CIMValue.h>
#include <Pegasus/Common/System.h>
#include <Pegasus/Common/InternalException.h>
#include <Pegasus/Client/CIMClient.h>

PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;


const CIMNamespaceName PROVIDERNAMESPACE =
CIMNamespaceName ("test/TestProvider");
const char *queries[] = {
  "select n32,s,ElementName,n64,n16,f,d from TestCMPI_Instance where n32=42",   // 0
  "SELECT * FROM TestCMPI_Instance",    // 1
  "SELECT not_present_property FROM TestCMPI_Instance", // 2 - won't get anything back
  "SELECT * FROM TestCMPI_Instance where s=s",  // 3
  "SELECT * FROM TestCMPI_Instance where s=s AND n64=64",       //4
  "SELECT s FROM TestCMPI_Instance where n64=40",       // 5
#if 0
  "SELECT * FROM TestCMPI_Instance where TestCMPI_Instance ISA TestCMPI_Parent",        // 6
  "SELECT * FROM TestCMPI_Instance where TestCMPI_Instance ISA TestCMPI_Indication",    //7 - wont'get anything back
#endif
  "SELECT * FROM TestCMPI_Instance  WHERE (s IS NULL) OR (n16=16)",     // 8
};

#define QUERIES  6
Boolean verbose;


void _checkStringProperty
  (CIMInstance & instance,
   const String & name, const String & value, Boolean null = false)
{
  Uint32 pos = instance.findProperty (name);
  PEGASUS_ASSERT (pos != PEG_NOT_FOUND);

  CIMProperty theProperty = instance.getProperty (pos);
  CIMValue theValue = theProperty.getValue ();

  PEGASUS_ASSERT (theValue.getType () == CIMTYPE_STRING);
  PEGASUS_ASSERT (!theValue.isArray ());
  if (null)
    {
      PEGASUS_ASSERT (theValue.isNull ());
    }
  else
    {
      PEGASUS_ASSERT (!theValue.isNull ());
      String result;
      theValue.get (result);

      if (verbose)
        {
          if (result != value)
            {
              cerr << "Property value comparison failed.  ";
              cerr << "Expected " << value << "; ";
              cerr << "Actual property value was " << result << "." << endl;
            }
        }

      PEGASUS_ASSERT (result == value);
    }
}

void _checkUint16Property
  (CIMInstance & instance, const String & name, Uint16 value)
{
  Uint32 pos = instance.findProperty (name);
  PEGASUS_ASSERT (pos != PEG_NOT_FOUND);

  CIMProperty theProperty = instance.getProperty (pos);
  CIMValue theValue = theProperty.getValue ();

  PEGASUS_ASSERT (theValue.getType () == CIMTYPE_UINT16);
  PEGASUS_ASSERT (!theValue.isArray ());
  PEGASUS_ASSERT (!theValue.isNull ());
  Uint16 result;
  theValue.get (result);

  if (verbose)
    {
      if (result != value)
        {
          cerr << "Property value comparison failed.  ";
          cerr << "Expected " << value << "; ";
          cerr << "Actual property value was " << result << "." << endl;
        }
    }

  PEGASUS_ASSERT (result == value);
}

void _checkUint32Property
  (CIMInstance & instance, const String & name, Uint32 value)
{
  Uint32 pos = instance.findProperty (name);
  PEGASUS_ASSERT (pos != PEG_NOT_FOUND);

  CIMProperty theProperty = instance.getProperty (pos);
  CIMValue theValue = theProperty.getValue ();

  PEGASUS_ASSERT (theValue.getType () == CIMTYPE_UINT32);
  PEGASUS_ASSERT (!theValue.isArray ());
  PEGASUS_ASSERT (!theValue.isNull ());
  Uint32 result;
  theValue.get (result);

  if (verbose)
    {
      if (result != value)
        {
          cerr << "Property value comparison failed.  ";
          cerr << "Expected " << value << "; ";
          cerr << "Actual property value was " << result << "." << endl;
        }
    }

  PEGASUS_ASSERT (result == value);
}

void _checkUint64Property
  (CIMInstance & instance, const String & name, Uint64 value)
{
  Uint32 pos = instance.findProperty (name);
  PEGASUS_ASSERT (pos != PEG_NOT_FOUND);

  CIMProperty theProperty = instance.getProperty (pos);
  CIMValue theValue = theProperty.getValue ();

  PEGASUS_ASSERT (theValue.getType () == CIMTYPE_UINT64);
  PEGASUS_ASSERT (!theValue.isArray ());
  PEGASUS_ASSERT (!theValue.isNull ());
  Uint64 result;
  theValue.get (result);

  if (verbose)
    {
      if (result != value)
        {
          char buffer[32];      // Should need 21 chars max
          sprintf (buffer, "%" PEGASUS_64BIT_CONVERSION_WIDTH "u", value);
          cerr << "Property value comparison failed.  ";
          cerr << "Expected " << buffer << "; ";
          sprintf (buffer, "%" PEGASUS_64BIT_CONVERSION_WIDTH "u", result);
          cerr << "Actual property value was " << buffer << "." << endl;
        }
    }

  PEGASUS_ASSERT (result == value);
}



void
_usage ()
{
  cerr << "Usage: TestCMPIIndicationSubscription " << "{test}" << endl;
}





void
_test (CIMClient & client)
{
  try
  {
    String wql ("WQL");
    String cql ("CIM:CQL");

    for (Uint32 i = 0; i < QUERIES; i++)
      {

        if (verbose)
          cerr << "Querying " << queries[i] << endl;

        Array < CIMObject > objects = client.execQuery (PROVIDERNAMESPACE,
                                                        wql, queries[i]);

        if (objects.size () == 0)
          {
            // Only the third (second when starting from zero) and eight(7) won't
            // return instances.
            PEGASUS_ASSERT (i == 2 || i == 5 || i == 7);
            if (verbose)
              cerr << "No instance returned.. That is good" << endl;
          }
        for (Uint32 i = 0; i < objects.size (); i++)
          {

            if (objects[i].isInstance ())
              {

                CIMInstance inst (objects[i]);

                if (inst.findProperty ("ElementName") != PEG_NOT_FOUND)
                  _checkStringProperty (inst, "ElementName",
                                        "TestCMPI_Instance");

                if (inst.findProperty ("s") != PEG_NOT_FOUND)
                  _checkStringProperty (inst, "s", "s");

                if (inst.findProperty ("n32") != PEG_NOT_FOUND)
                  _checkUint32Property (inst, "n32", 42);

                if (inst.findProperty ("n64") != PEG_NOT_FOUND)
                  _checkUint64Property (inst, "n64", 64);

                if (inst.findProperty ("n16") != PEG_NOT_FOUND)
                  _checkUint16Property (inst, "n16", 16);

              }
          }
      }
  }

  catch (Exception & e)
  {
    cerr << "test failed: " << e.getMessage () << endl;
    exit (-1);
  }

  cout << "+++++ test completed successfully" << endl;
}


int
main (int argc, char **argv)
{
  verbose = (getenv ("PEGASUS_TEST_VERBOSE")) ? true : false;
  CIMClient client;
  try
  {
    client.connectLocal ();
  }
  catch (Exception & e)
  {
    cerr << e.getMessage () << endl;
    return -1;
  }

  if (argc != 2)
    {
      _usage ();
      return 1;
    }

  else
    {
      const char *opt = argv[1];

      if (String::equalNoCase (opt, "test"))
        {
          _test (client);
        }
      else
        {
          cerr << "Invalid option: " << opt << endl;
          _usage ();
          return -1;
        }
    }

  return 0;
}
