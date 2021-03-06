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
// Author: Sean Keenan, Hewlwtt-Packard Company <sean.keenan@hp.com>
//
// Modified By:
//
//==============================================================================
//%////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// INCLUDES
//------------------------------------------------------------------------------

//Pegasus includes
#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/Exception.h>
#include "NTPProviderSecurity.h"    

// The following includes are necessary to gethostbyaddr and gethostname
// functions 
#include <ctype.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//used by gethostname function
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>

#include "NTPServiceProvider.h"

#define MAXHOSTNAMELEN 256

//------------------------------------------------------------------------------
PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;

// File configurations
static const String NTP_FILE_CONFIG("/etc/ntp.conf");
static const String NTP_NAME("xntpd");


//==============================================================================
//
// Class [NTPService] methods
//
//==============================================================================

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
NTPService::NTPService(void)
{
    // Retrieve NTP informations
    if(!getNTPInfo())
        throw CIMObjectNotFoundException("NTPService "
            "can't create PG_NTPService instance");
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
NTPService::~NTPService(void)
{
}

//------------------------------------------------------------------------------
// FUNCTION: getUtilGetHostName 
//
// REMARKS: 
//
// PARAMETERS:  [OUT] systemName -> string that will contain the host name 
//
// RETURN: TRUE if successful, FALSE otherwise
//------------------------------------------------------------------------------
static Boolean getUtilGetHostName(String& systemName)
{
     char    hostName[MAXHOSTNAMELEN];
     struct  hostent *he;

     if (gethostname(hostName, MAXHOSTNAMELEN) != 0)
     {
         return false;
     }

     // Now get the official hostname.  If this call fails then return
     // the value from gethostname().

     if (he=gethostbyname(hostName))
      {
        strcpy(hostName, he->h_name);
     }

     systemName.assign(hostName);

     return true;
}

//------------------------------------------------------------------------------
// FUNCTION: getSystemName 
//
// REMARKS: 
//
// PARAMETERS:  [OUT] systemName -> string that will contain the system name 
//
// RETURN: TRUE if successful , FALSE otherwise
//------------------------------------------------------------------------------
Boolean NTPService::getSystemName(String& systemName)
{
    return getUtilGetHostName(systemName);
}

//------------------------------------------------------------------------------
// FUNCTION: getHostName
//
// REMARKS: Resolves name servers
//
// PARAMETERS:  [IN] serverAddress -> string containing the IP address
//                [OUT] hostName -> string that will contain the name server
//
// RETURN: TRUE if valid host IP, FALSE otherwise
//------------------------------------------------------------------------------
Boolean NTPService::getHostName(String serverAddress, String & hostName) 
{
    Boolean ok = false;
    int ps, value = 0;
    String strValue;
    struct hostent *host;
    struct in_addr ia;
    
    hostName.clear();
// 
// unsigned long S_addr;
// 
// 
    if((ia.s_addr = inet_addr(serverAddress.getCString())) != 0) 
    {
        host = gethostbyaddr((const char *)&ia, 
                              sizeof(struct in_addr),
                               AF_INET);
        if(host != NULL) 
        {
            hostName.assign(host->h_name);
            ok = true;
        }
   }
   return ok;
}    

//------------------------------------------------------------------------------
// FUNCTION: getHostAddress
//
// REMARKS: Resolves address servers
//
// PARAMETERS:  [IN] serverName     -> string containing the name server
//                [OUT] serverAddress -> string that will contain the IP server
//
// RETURN: TRUE if valid host name, FALSE otherwise
//------------------------------------------------------------------------------
Boolean NTPService::getHostAddress(String serverName, String & serverAddress) 
{
    Boolean ok = false;
    int ps, value = 0;
    String strValue;
    struct hostent *host;
    struct in_addr ia;
    
    serverAddress.clear();
    host = gethostbyname(serverName.getCString());
    if(host != NULL) 
    {
        ia = *(struct in_addr *)(host->h_addr);
        serverAddress.assign(inet_ntoa(ia));
        ok = true;
    }
    return ok;
}    

//------------------------------------------------------------------------------
// FUNCTION: isHostAddress
//
// REMARKS: Verify if host is address
//
// PARAMETERS:  [IN] host -> string containing the host name or address
//
// RETURN: TRUE if host address, FALSE otherwise
//------------------------------------------------------------------------------
Boolean NTPService::isHostAddress(String host) {
    int ps;
    String strValue;
    Boolean ok = false;
    
    ps = host.find(".");
    if(ps > 0) {
        strValue.assign(host.subString(0, ps - 1));
        ok = (atoi(strValue.getCString()) > 0);
    }
    return ok;
}

//------------------------------------------------------------------------------
// FUNCTION: getLocalHostName
//
// REMARKS: Retrieves the local host name
//
// PARAMETERS:  [OUT] hostName -> string that will contain the local host name
//
// RETURN: TRUE if local hostname is valid, FALSE otherwise
//------------------------------------------------------------------------------
Boolean NTPService::getLocalHostName(String & hostName) 
{
    char host[30];
    if(gethostname(host, sizeof(host)))
        return false;

       hostName.assign(host);
    return true;
}

//------------------------------------------------------------------------------
// FUNCTION: AccessOk
//
// REMARKS: Status of context user
//
// PARAMETERS:    [IN]  context  -> pointer to Operation Context
//
// RETURN: TRUE, if user have privileges, otherwise FALSE
//------------------------------------------------------------------------------
Boolean NTPService::AccessOk(const OperationContext & context)
{
    NTPProviderSecurity sec(context);
    Boolean ok = sec.checkAccess(NTP_FILE_CONFIG,
                                  SEC_OPT_READ);
    return ok;
}

//------------------------------------------------------------------------------
// FUNCTION: getNTPInfo
//
// REMARKS: Retrieves the NTP information from the "/etc/ntp.conf" file, 
//            and sets private variables to hold the data read.
//
// RETURN: 
//------------------------------------------------------------------------------
Boolean NTPService::getNTPInfo() 
{
    FILE *fp;
    int i, ps = 0;
    long lstPos = 0;
    char buffer[5000];
    Boolean ok = false,
            okRet = false;
    String strKey, 
           strHost, 
           strBuffer;

    // Open NTP configuration file
    if((fp = fopen(NTP_FILE_CONFIG.getCString(), "r")) == NULL)
        return ok;

    // Clear attributes 
    ntpName.clear();
    ntpServerAddress.clear();

    memset(buffer, 0, sizeof(buffer));
    while(fgets(buffer, sizeof(buffer), fp) != NULL) 
    {
        buffer[strlen(buffer)-1] = 0;
        strBuffer.assign(buffer);

        ps = strBuffer.find(NTP_ROLE_CLIENT);
        
        okRet = true;
        if(ps == 0) 
        {
            okRet = true;
            fseek(fp, lstPos, SEEK_SET);
            fscanf(fp, "%s", buffer);
            strBuffer.assign(buffer);
            ps = strBuffer.find(NTP_ROLE_CLIENT);
            if(ps < 0) {
                lstPos = ftell(fp);
                continue;
            }

            fscanf(fp, "%s", buffer);
            strHost.assign(buffer);

            ok = false;    
            // Verify if name server exists in array
            for(i=0; i < ntpServerAddress.size(); i++) 
            {
                if(String::equalNoCase(ntpServerAddress[i], strHost)) 
                {
                    ok = true;
                    break;
                }
            }
            if(!ok) 
            {
                ntpServerAddress.append(strHost);
                if(ntpName.size() == 0) 
                    // Set ntpName variable with name server, if strHost variable
                    // is an IP address.
                    getHostName(strHost, ntpName);
            }
        }
        lstPos = ftell(fp);
    }
    fclose(fp);        
    return okRet;
}

//------------------------------------------------------------------------------
// FUNCTION: getNTPName
//
// REMARKS: returns the Name property
//
// PARAMETERS: [OUT] strValue -> string that will receive the NTP_Name property 
//                                 value
//
// RETURN: TRUE (hard-coded property value)
//------------------------------------------------------------------------------
Boolean NTPService::getNTPName(String & strValue) 
{
    strValue.assign(NTP_NAME);
    return true;
}

//------------------------------------------------------------------------------
// FUNCTION: getCaption
//
// REMARKS: returns the Caption property
//
// PARAMETERS: [OUT] strValue -> string that will receive the Caption property 
//                                 value
//
// RETURN: TRUE (hard-coded property value)
//------------------------------------------------------------------------------
Boolean NTPService::getCaption(String & strValue) 
{
    strValue.assign(NTP_CAPTION);
    return true;
}

//------------------------------------------------------------------------------
// FUNCTION: getDescription
//
// REMARKS: returns the Description property
//
// PARAMETERS: [OUT] strValue -> string that will receive the Description property 
//                                 value
//
// RETURN: TRUE (hard-coded property value)
//------------------------------------------------------------------------------
Boolean NTPService::getDescription(String & strValue) 
{
    strValue.assign(NTP_DESCRIPTION);
    return true;
}

//------------------------------------------------------------------------------
// FUNCTION: getServerAddress
//
// REMARKS: returns the ServerAddress array property, if is a NTP client
//
// PARAMETERS: [OUT] strValue -> string that will receive the ServerAddress 
//                                 property value
//
// RETURN: TRUE if 'ServerAddress' was successfully obtained, FALSE otherwise.
//------------------------------------------------------------------------------
Boolean NTPService::getServerAddress(Array<String> & strValue) 
{
    strValue.clear();
    for(int i=0; i < ntpServerAddress.size(); i++) 
        strValue.append(ntpServerAddress[i]);
    return true;
}

