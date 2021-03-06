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
// Author: Lyle Wilkinson, Hewlett-Packard Company <lyle_wilkinson@hp.com>
//
// Modified By:
//
//%/////////////////////////////////////////////////////////////////////////////

#include "IPInfo.h"

PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;

static const CIMName          CLASS_NAME = CIMName ("CIM_IPProtocolEndpoint");
static const CIMNamespaceName NAMESPACE  = CIMNamespaceName ("root/cimv2");
#define HeaderFormat "%-16s %-10s %-10s %-16s %-16s"


////////////////////////////////////////////////////////////////////////////////
//  Constructor for IP Protocol Endpoint Info
////////////////////////////////////////////////////////////////////////////////
IPPEpInfo::IPPEpInfo(CIMClient &client, Boolean enableDebug,
               ostream& outPrintWriter, ostream& errPrintWriter)
{

    _enableDebug = enableDebug;

    try
    {
        Boolean deepInheritance = true;
	Boolean localOnly = true;
	Boolean includeQualifiers = false;
	Boolean includeClassOrigin = false;
      
      	Array<CIMInstance> cimInstances = 
	    		client.enumerateInstances(NAMESPACE, CLASS_NAME,
			deepInheritance, localOnly, includeQualifiers,
			includeClassOrigin);
 
      	Uint32 numberInstances = cimInstances.size();

      	if (_enableDebug)
	{
		outPrintWriter << numberInstances << " instances of " <<
	             CLASS_NAME.getString() << endl;
	}

	if (numberInstances > 0)
	{
            _gatherProperties(cimInstances[0]);
	    _outputHeader(outPrintWriter);

	    for (Uint32 i = 0; i < numberInstances; i++)
	    {
	        _gatherProperties(cimInstances[i]);
	        _outputInstance(outPrintWriter);

	     }   // end for looping through instances
	}
        else
	{
	     outPrintWriter << "No instances of class " << CLASS_NAME.getString() << endl;
	}

    }  // end try 
   
    catch(Exception& e)
    {
        errPrintWriter << "Error getting instances of class " <<
             CLASS_NAME.getString() << endl;
    }
}

////////////////////////////////////////////////////////////////////////////////
//  Destructor for IP Protocol Endpoint Info
////////////////////////////////////////////////////////////////////////////////
IPPEpInfo::~IPPEpInfo(void)
{
}

////////////////////////////////////////////////////////////////////////////////
//  Gather Properities for IP Protocol Endpoint Info
////////////////////////////////////////////////////////////////////////////////
void IPPEpInfo::_gatherProperties(CIMInstance &inst)
{
    _ipSCCN             = String::EMPTY;
    _ipSN               = String::EMPTY;
    _ipCCN              = String::EMPTY;
    _ipName             = String::EMPTY;

    _ipCaption          = String::EMPTY;
    _ipDescription      = String::EMPTY;
    _ipNameFormat       = String::EMPTY;
    _ipProtocolType     = 0;  // Unknown
    _ipAddress          = String::EMPTY;
    _ipSubnetMask       = String::EMPTY;
    _ipAddrType         = 0;  // Unknown
    _ipIPVersionSupport = 0;  // Unknown

    for (Uint32 j=0; j < inst.getPropertyCount(); j++)
    {
	CIMName propertyName = inst.getProperty(j).getName();

        // Properties that are also keys
      	if (propertyName.equal("SystemCreationClassName"))
	    inst.getProperty(j).getValue().get(_ipSCCN);

      	else if (propertyName.equal("SystemName"))
	    inst.getProperty(j).getValue().get(_ipSN);

      	else if (propertyName.equal("CreationClassName"))
	    inst.getProperty(j).getValue().get(_ipCCN);

      	else if (propertyName.equal("Name"))
	    inst.getProperty(j).getValue().get(_ipName);

        // Other properties
      	else if (propertyName.equal("Caption"))
	    inst.getProperty(j).getValue().get(_ipCaption); 

      	else if (propertyName.equal("Description"))
	    inst.getProperty(j).getValue().get(_ipDescription); 

      	else if (propertyName.equal("NameFormat"))
	    inst.getProperty(j).getValue().get(_ipNameFormat); 

      	else if (propertyName.equal("ProtocolType"))
	    inst.getProperty(j).getValue().get(_ipProtocolType); 

      	else if (propertyName.equal("Address"))
	    inst.getProperty(j).getValue().get(_ipAddress); 

      	else if (propertyName.equal("SubnetMask"))
	    inst.getProperty(j).getValue().get(_ipSubnetMask); 

      	else if (propertyName.equal("AddressType"))
	    inst.getProperty(j).getValue().get(_ipAddrType); 

      	else if (propertyName.equal("IPVersionSupport"))
	    inst.getProperty(j).getValue().get(_ipIPVersionSupport); 

   } // end for loop through properties

}


////////////////////////////////////////////////////////////////////////////////
//  Header Section for IP Protocol Endpoint Info
////////////////////////////////////////////////////////////////////////////////
void IPPEpInfo::_outputHeader(ostream &outPrintWriter)
{

    outPrintWriter << endl << ">>>> IP Protocol Endpoint Information <<<<" <<
         endl << endl;

    if (_ipSCCN.size() > 0)
        outPrintWriter << "System Creation Class Name  : " << _ipSCCN << endl;

    if (_ipSN.size() > 0)
        outPrintWriter << "System Name                 : " << _ipSN << endl;

    if (_ipCCN.size() > 0)
        outPrintWriter << "Creation Class Name         : " << _ipCCN << endl;

    if (_ipNameFormat.size() > 0)
        outPrintWriter << "Name Format                 : " << _ipNameFormat << endl;

    outPrintWriter << "IP Version Support          : ";
    if (_ipIPVersionSupport == 1)
	outPrintWriter << "IPv4 Only" << endl;
    else if (_ipIPVersionSupport == 2)
	outPrintWriter << "IPv6 Only" << endl;
    else
	outPrintWriter << "Unknown" << endl;

    char header[81];

    sprintf(header, HeaderFormat, "Endpoint", "AddrType", "Protocol",
				  "Address", "SubnetMask");

    outPrintWriter << endl << header << endl;
    
}

////////////////////////////////////////////////////////////////////////////////
//  Output an instance of an IP Protocol Endpoint Info
////////////////////////////////////////////////////////////////////////////////
void IPPEpInfo::_outputInstance(ostream &outPrintWriter)
{
    String _ipAT,  // Address Type
	   _ipPT;  // Protocol Type


    if (_ipAddrType == 1)
	_ipAT = "IPv4";
    else if (_ipAddrType == 2)
	_ipAT = "IPv6";
    else
	_ipAT = "Unk";

    if (_ipProtocolType == 0)
	_ipPT = "Unknown";
    else if (_ipProtocolType == 2)
	_ipPT = "IPv4";
    else if (_ipProtocolType == 3)
	_ipPT = "IPv6";
    else
	_ipPT = "Unk";

    char row[81];

    sprintf(row, HeaderFormat, (const char *)_ipName.getCString(),
			       (const char *)_ipAT.getCString(),
			       (const char *)_ipPT.getCString(),
			       (const char *)_ipAddress.getCString(),
			       (const char *)_ipSubnetMask.getCString()
			       );
    outPrintWriter << row << endl;
    
}
