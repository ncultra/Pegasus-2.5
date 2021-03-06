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


#include <Pegasus/Client/CIMClient.h>
#include <Pegasus/Common/CIMClass.h>
#include <Pegasus/Common/CIMName.h>
#include <iostream>
#include <stdlib.h>

PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;


Uint16 _validateGetClass(CIMClient& cli, CIMNamespaceName& nameSpace, CIMName& className)
{
   try
   {
      //cout << "we are in the function _validateGetClass" << endl;
      CIMClass perfClass;
      perfClass = cli.getClass(nameSpace,
                               className,
                               false,
                               false,
                               false);
   }

   catch (Exception& e)
   {
      cerr << "Exception getClass : " << e.getMessage() << endl;
      cerr << "Exception occured in the Pegasus/ControlProviders/Statictic/test file" << endl;
      return(5);
   }
   return(0);
}



Uint16 _validateGetInst(CIMClient& cli, CIMNamespaceName& nameSpace, CIMInstance& instName)
{
   //cout << "we are in the function _validateGetInst" << endl;

   CIMObjectPath objPath = instName.getPath();
   CIMInstance inst;
   try
   {
      inst = cli.getInstance(nameSpace,
                             objPath,
                             false,
                             false,
                             false);
   }
   catch (Exception& e)
   {
      cerr << "Exception getInstances : " << e.getMessage() << endl;
      cerr << "Exception occured in the Pegasus/ControlProviders/Statictic/test file" << endl;
      return(4);
   }

   return(0);
}

Uint16 _validateEumInstNames(CIMClient& cli, CIMNamespaceName& nameSpace, CIMName&
                             className)
{
   //cout <<"we are in the _validateEumInstNames func" << endl;

   try
   {
      cli.enumerateInstanceNames(nameSpace, className);
   }
   catch (Exception& e)
   {
      cerr << "Exception enumerateInstances : " << e.getMessage() << endl;
      cerr << "Exception occured in the Pegasus/ControlProviders/Statictic/test file" << endl;
      return(3);
   }

   return(0);
}


			

Uint16 _validateInstOperations(CIMClient& cli, CIMNamespaceName& nameSpace, CIMName& className)
{
   //cout << "we are in the function _validateEumInst" <<endl;

   Array<CIMInstance> instances;
   try
   {
      instances = cli.enumerateInstances(nameSpace,
                                         className,
                                         false,
                                         false,
                                         false,
                                         false);
   }
   catch (Exception& e)
   {
      cerr << "Exception enumerateInstances : " << e.getMessage() << endl;
      cerr << "Exception occured in the Pegasus/ControlProviders/Statictic/test file" << endl;
      return(2);
   }
   

   Uint16 ret = _validateGetInst(cli, nameSpace, instances[3]);
   if (ret != 0)
      return(ret);

   ret = _validateEumInstNames(cli, nameSpace, className);
   if (ret != 0)
      return(ret);
   
   return(0);
}


Uint16 _validateClientRequest(CIMClient& cli)
{

   String nS = "/root/cimv2";
   CIMNamespaceName nameSpace;
   nameSpace = nS;

   String cN = "CIM_CIMOMStatisticalData";
   CIMName className;
   className = cN;

   Uint16 ret = _validateGetClass(cli, nameSpace, className);
   if (ret != 0)
      return(ret);

   ret = _validateInstOperations(cli, nameSpace, className);
   if (ret != 0)
      return(ret);

   return(0);
}

int main(int argc, char** argv)
{
//    verbose = getenv("PEGASUS_TEST_VERBOSE");

   Uint16 re_num;

   CIMClient client;
   String host = "localhost";
   Uint32 portNumber = 5988;
   String userName = String::EMPTY;
   String password = String::EMPTY;

   try
   {
      client.connect (host, portNumber,
                      userName, password);
   }
   catch (Exception& e)
   {
      cerr << "Error: " << e.getMessage() <<  endl;
	  cerr << "Exception occured in the Pegasus/ControlProviders/Statictic/test file" << endl;
      return(1);
   }
   re_num = _validateClientRequest(client);

   if (re_num != 0)
   {
      exit(re_num);
   }
/* ATTN:WCW there needs to be some validation on headerfield of response
   re_num = _validateUpadatedFields(client);

   if (re_num != 0)
     {exit(re_num);}
*/
   cout << argv[0] << " +++++ passed all tests" << endl;
   return 0;
}

