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
//             (carolann_graves@hp.com)
//
//%/////////////////////////////////////////////////////////////////////////////

#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/Constants.h>
#include <Pegasus/Common/System.h>
#include <Pegasus/Client/CIMClient.h>

PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;

const CIMNamespaceName NAMESPACE = CIMNamespaceName ("root/PG_InterOp");
const CIMNamespaceName NAMESPACE1 = CIMNamespaceName ("root/SampleProvider");
const CIMNamespaceName NAMESPACE2 = CIMNamespaceName ("root/cimv2");
const CIMNamespaceName NAMESPACE3 = CIMNamespaceName ("test/TestProvider");
const CIMNamespaceName SOURCENAMESPACE = 
    CIMNamespaceName ("root/SampleProvider");

void _createModuleInstance 
    (CIMClient & client, 
     const String & name,
     const String & location)
{
    CIMInstance moduleInstance (PEGASUS_CLASSNAME_PROVIDERMODULE);
    moduleInstance.addProperty (CIMProperty (CIMName ("Name"), name));
    moduleInstance.addProperty (CIMProperty (CIMName ("Vendor"), 
        String ("Hewlett-Packard Company")));
    moduleInstance.addProperty (CIMProperty (CIMName ("Version"),
        String ("2.0")));
    moduleInstance.addProperty (CIMProperty (CIMName ("InterfaceType"),
        String ("C++Default")));
    moduleInstance.addProperty (CIMProperty (CIMName ("InterfaceVersion"), 
        String ("2.2.0")));
    moduleInstance.addProperty (CIMProperty (CIMName ("Location"), location));

    CIMObjectPath path = client.createInstance (NAMESPACE, moduleInstance);
}

void _createProviderInstance 
    (CIMClient & client, 
     const String & name,
     const String & providerModuleName)
{
    CIMInstance providerInstance (PEGASUS_CLASSNAME_PROVIDER);
    providerInstance.addProperty (CIMProperty (CIMName ("Name"), name));
    providerInstance.addProperty (CIMProperty (CIMName ("ProviderModuleName"), 
        providerModuleName));

    CIMObjectPath path = client.createInstance (NAMESPACE, providerInstance);
}

void _createCapabilityInstance 
    (CIMClient & client, 
     const String & providerModuleName,
     const String & providerName,
     const String & capabilityID,
     const String & className,
     const CIMPropertyList & supportedProperties)
{
    Array <String> namespaces;
    Array <Uint16> providerType;
    namespaces.append (SOURCENAMESPACE.getString ());
    providerType.append (4);
    CIMInstance capabilityInstance (PEGASUS_CLASSNAME_PROVIDERCAPABILITIES);
    capabilityInstance.addProperty (CIMProperty (CIMName ("ProviderModuleName"),
        providerModuleName));
    capabilityInstance.addProperty (CIMProperty (CIMName ("ProviderName"),
        providerName));
    capabilityInstance.addProperty (CIMProperty (CIMName ("CapabilityID"),
        capabilityID));
    capabilityInstance.addProperty (CIMProperty (CIMName ("ClassName"), 
        className));
    capabilityInstance.addProperty (CIMProperty (CIMName ("Namespaces"), 
        namespaces));
    capabilityInstance.addProperty (CIMProperty (CIMName ("ProviderType"),
        CIMValue (providerType)));
    if (!supportedProperties.isNull ())
    {
        Array <String> propertyNameStrings;
        for (Uint32 i = 0; i < supportedProperties.size (); i++)
        {
            propertyNameStrings.append (supportedProperties [i].getString ());
        }
        capabilityInstance.addProperty (CIMProperty 
            (CIMName ("supportedProperties"), CIMValue (propertyNameStrings)));
    }

    CIMObjectPath path = client.createInstance (NAMESPACE, capabilityInstance);
}

void _createHandlerInstance 
    (CIMClient & client, 
     const String & name,
     const String & destination,
     const CIMNamespaceName & handlerNS)
{
    CIMInstance handlerInstance (PEGASUS_CLASSNAME_INDHANDLER_CIMXML);
    handlerInstance.addProperty (CIMProperty (CIMName 
        ("SystemCreationClassName"), System::getSystemCreationClassName ()));
    handlerInstance.addProperty (CIMProperty (CIMName ("SystemName"),
        System::getFullyQualifiedHostName ()));
    handlerInstance.addProperty (CIMProperty (CIMName ("CreationClassName"),
        PEGASUS_CLASSNAME_INDHANDLER_CIMXML.getString ()));
    handlerInstance.addProperty (CIMProperty (CIMName ("Name"), name));
    handlerInstance.addProperty (CIMProperty (CIMName ("Destination"),
        destination));

    CIMObjectPath path = client.createInstance (handlerNS, handlerInstance);
}

void _createFilterInstance 
    (CIMClient & client, 
     const String & name,
     const String & query,
     const String & qlang,
     const CIMNamespaceName & filterNS)
{
    CIMInstance filterInstance (PEGASUS_CLASSNAME_INDFILTER);
    filterInstance.addProperty (CIMProperty (CIMName 
        ("SystemCreationClassName"), System::getSystemCreationClassName ()));
    filterInstance.addProperty (CIMProperty (CIMName ("SystemName"),
        System::getFullyQualifiedHostName ()));
    filterInstance.addProperty (CIMProperty (CIMName ("CreationClassName"),
        PEGASUS_CLASSNAME_INDFILTER.getString ()));
    filterInstance.addProperty (CIMProperty (CIMName ("Name"), name));
    filterInstance.addProperty (CIMProperty (CIMName ("Query"), query));
    filterInstance.addProperty (CIMProperty (CIMName ("QueryLanguage"),
        String (qlang)));
    filterInstance.addProperty (CIMProperty (CIMName ("SourceNamespace"),
        SOURCENAMESPACE.getString ()));

    CIMObjectPath path = client.createInstance (filterNS, filterInstance);
}

void _createSubscriptionInstance 
    (CIMClient & client,
     const CIMObjectPath & filterPath,
     const CIMObjectPath & handlerPath,
     const CIMNamespaceName & subscriptionNS)
{
    CIMInstance subscriptionInstance (PEGASUS_CLASSNAME_INDSUBSCRIPTION);
    subscriptionInstance.addProperty (CIMProperty (CIMName ("Filter"),
        filterPath, 0, PEGASUS_CLASSNAME_INDFILTER));
    subscriptionInstance.addProperty (CIMProperty (CIMName ("Handler"),
        handlerPath, 0, PEGASUS_CLASSNAME_INDHANDLER_CIMXML));
    subscriptionInstance.addProperty (CIMProperty
        (CIMName ("SubscriptionState"), CIMValue ((Uint16) 2)));

    CIMObjectPath path = client.createInstance (subscriptionNS, 
        subscriptionInstance);
}

void _modifyCapabilityInstance 
    (CIMClient & client, 
     const String & providerModuleName,
     const String & providerName,
     const String & capabilityID,
     const CIMPropertyList & supportedProperties)
{
    CIMInstance modifiedInstance (PEGASUS_CLASSNAME_PROVIDERCAPABILITIES);
    modifiedInstance.addProperty (CIMProperty (CIMName ("ProviderModuleName"),
        providerModuleName));
    modifiedInstance.addProperty (CIMProperty (CIMName ("ProviderName"),
        providerName));
    modifiedInstance.addProperty (CIMProperty (CIMName ("CapabilityID"),
        capabilityID));

    Array<CIMKeyBinding> keyBindings;
    keyBindings.append (CIMKeyBinding ("ProviderModuleName",
        providerModuleName, CIMKeyBinding::STRING));
    keyBindings.append (CIMKeyBinding ("ProviderName",
        providerName, CIMKeyBinding::STRING));
    keyBindings.append (CIMKeyBinding ("CapabilityID",
        capabilityID, CIMKeyBinding::STRING));
    CIMObjectPath path ("", CIMNamespaceName (),
        CIMName ("PG_ProviderCapabilities"), keyBindings);
    modifiedInstance.setPath (path);
    if (!supportedProperties.isNull ())
    {
        Array <String> propertyNameStrings;
        for (Uint32 i = 0; i < supportedProperties.size (); i++)
        {
            propertyNameStrings.append (supportedProperties [i].getString ());
        }
        modifiedInstance.addProperty (CIMProperty 
            (CIMName ("supportedProperties"), CIMValue (propertyNameStrings)));
    }
    Array <CIMName> propertyNames;
    propertyNames.append (CIMName ("SupportedProperties"));
    client.modifyInstance (NAMESPACE, modifiedInstance, false, 
        CIMPropertyList (propertyNames));
}

void _deleteSubscriptionInstance 
    (CIMClient & client, 
     const String & filterName,
     const String & handlerName,
     const CIMNamespaceName & filterNS,
     const CIMNamespaceName & handlerNS,
     const CIMNamespaceName & subscriptionNS)
{
    Array<CIMKeyBinding> filterKeyBindings;
    filterKeyBindings.append (CIMKeyBinding ("SystemCreationClassName",
        System::getSystemCreationClassName (), CIMKeyBinding::STRING));
    filterKeyBindings.append (CIMKeyBinding ("SystemName",
        System::getFullyQualifiedHostName (), CIMKeyBinding::STRING));
    filterKeyBindings.append (CIMKeyBinding ("CreationClassName",
        PEGASUS_CLASSNAME_INDFILTER.getString (), CIMKeyBinding::STRING));
    filterKeyBindings.append (CIMKeyBinding ("Name", filterName,
        CIMKeyBinding::STRING));
    CIMObjectPath filterPath ("", filterNS,
        PEGASUS_CLASSNAME_INDFILTER, filterKeyBindings);

    Array<CIMKeyBinding> handlerKeyBindings;
    handlerKeyBindings.append (CIMKeyBinding ("SystemCreationClassName",
        System::getSystemCreationClassName (), CIMKeyBinding::STRING));
    handlerKeyBindings.append (CIMKeyBinding ("SystemName",
        System::getFullyQualifiedHostName (), CIMKeyBinding::STRING));
    handlerKeyBindings.append (CIMKeyBinding ("CreationClassName",
        PEGASUS_CLASSNAME_INDHANDLER_CIMXML.getString (),
        CIMKeyBinding::STRING));
    handlerKeyBindings.append (CIMKeyBinding ("Name", handlerName,
        CIMKeyBinding::STRING));
    CIMObjectPath handlerPath ("", handlerNS,
        PEGASUS_CLASSNAME_INDHANDLER_CIMXML, handlerKeyBindings);

    Array<CIMKeyBinding> subscriptionKeyBindings;
    subscriptionKeyBindings.append (CIMKeyBinding ("Filter",
        filterPath.toString (), CIMKeyBinding::REFERENCE));
    subscriptionKeyBindings.append (CIMKeyBinding ("Handler",
        handlerPath.toString (), CIMKeyBinding::REFERENCE));
    CIMObjectPath subscriptionPath ("", CIMNamespaceName (),
        PEGASUS_CLASSNAME_INDSUBSCRIPTION, subscriptionKeyBindings);
    client.deleteInstance (subscriptionNS, subscriptionPath);
}

void _deleteSubscriptionInstance 
    (CIMClient & client, 
     const String & filterName,
     const String & handlerName)
{
    _deleteSubscriptionInstance (client, filterName, handlerName,
        CIMNamespaceName (), CIMNamespaceName (), NAMESPACE);
}

void _deleteHandlerInstance 
    (CIMClient & client, 
     const String & name,
     const CIMNamespaceName & handlerNS)
{
    Array<CIMKeyBinding> keyBindings;
    keyBindings.append (CIMKeyBinding ("SystemCreationClassName",
        System::getSystemCreationClassName (), CIMKeyBinding::STRING));
    keyBindings.append (CIMKeyBinding ("SystemName",
        System::getFullyQualifiedHostName (), CIMKeyBinding::STRING));
    keyBindings.append (CIMKeyBinding ("CreationClassName",
        PEGASUS_CLASSNAME_INDHANDLER_CIMXML.getString (),
        CIMKeyBinding::STRING));
    keyBindings.append (CIMKeyBinding ("Name", name,
        CIMKeyBinding::STRING));
    CIMObjectPath path ("", CIMNamespaceName (),
        PEGASUS_CLASSNAME_INDHANDLER_CIMXML, keyBindings);
    client.deleteInstance (handlerNS, path);
}

void _deleteFilterInstance 
    (CIMClient & client, 
     const String & name,
     const CIMNamespaceName & filterNS)
{
    Array<CIMKeyBinding> keyBindings;
    keyBindings.append (CIMKeyBinding ("SystemCreationClassName",
        System::getSystemCreationClassName (), CIMKeyBinding::STRING));
    keyBindings.append (CIMKeyBinding ("SystemName",
        System::getFullyQualifiedHostName (), CIMKeyBinding::STRING));
    keyBindings.append (CIMKeyBinding ("CreationClassName",
        PEGASUS_CLASSNAME_INDFILTER.getString (), CIMKeyBinding::STRING));
    keyBindings.append (CIMKeyBinding ("Name", name, 
        CIMKeyBinding::STRING));
    CIMObjectPath path ("", CIMNamespaceName (),
        PEGASUS_CLASSNAME_INDFILTER, keyBindings);
    client.deleteInstance (filterNS, path);
}

void _deleteCapabilityInstance 
    (CIMClient & client, 
     const String & providerModuleName,
     const String & providerName,
     const String & capabilityID)
{
    Array<CIMKeyBinding> keyBindings;
    keyBindings.append (CIMKeyBinding ("ProviderModuleName",
        providerModuleName, CIMKeyBinding::STRING));
    keyBindings.append (CIMKeyBinding ("ProviderName",
        providerName, CIMKeyBinding::STRING));
    keyBindings.append (CIMKeyBinding ("CapabilityID",
        capabilityID, CIMKeyBinding::STRING));
    CIMObjectPath path ("", CIMNamespaceName (),
        CIMName ("PG_ProviderCapabilities"), keyBindings);
    client.deleteInstance (NAMESPACE, path);
}

void _deleteProviderInstance 
    (CIMClient & client, 
     const String & name,
     const String & providerModuleName)
{
    Array<CIMKeyBinding> keyBindings;
    keyBindings.append (CIMKeyBinding ("Name",
        name, CIMKeyBinding::STRING));
    keyBindings.append (CIMKeyBinding ("ProviderModuleName",
        providerModuleName, CIMKeyBinding::STRING));
    CIMObjectPath path ("", CIMNamespaceName (),
        CIMName ("PG_Provider"), keyBindings);
    client.deleteInstance (NAMESPACE, path);
}

void _deleteModuleInstance 
    (CIMClient & client, 
     const String & name)
{
    Array<CIMKeyBinding> keyBindings;
    keyBindings.append (CIMKeyBinding ("Name",
        name, CIMKeyBinding::STRING));
    CIMObjectPath path ("", CIMNamespaceName (),
        CIMName ("PG_ProviderModule"), keyBindings);
    client.deleteInstance (NAMESPACE, path);
}

CIMObjectPath _buildFilterOrHandlerPath
    (const CIMName & className,
     const String & name,
     const CIMNamespaceName & namespaceName = CIMNamespaceName ())
{
    CIMObjectPath path;

    Array <CIMKeyBinding> keyBindings;
    keyBindings.append (CIMKeyBinding ("SystemCreationClassName",
        System::getSystemCreationClassName (), CIMKeyBinding::STRING));
    keyBindings.append (CIMKeyBinding ("SystemName",
        System::getFullyQualifiedHostName (), CIMKeyBinding::STRING));
    keyBindings.append (CIMKeyBinding ("CreationClassName",
        className.getString(), CIMKeyBinding::STRING));
    keyBindings.append (CIMKeyBinding ("Name", name, CIMKeyBinding::STRING));
    path.setClassName (className);
    path.setKeyBindings (keyBindings);
    path.setNameSpace (namespaceName);

    return path;
}

void _usage ()
{
    PEGASUS_STD (cerr) 
        << "Usage: TestDisableEnable "
        << "{setup | create | delete | create2 | addProvider | modifyProvider1 "
        << "| modifyProvider2 | deleteProvider | delete2 | cleanup} {WQL | CIM:CQL}" 
        << PEGASUS_STD (endl);
}

void _setup (CIMClient & client, String& qlang)
{
    try
    {
        _createModuleInstance (client, 
            String ("ProcessIndicationProviderModule"), 
            String ("ProcessIndicationProvider"));
        _createProviderInstance (client, 
            String ("ProcessIndicationProvider"), 
            String ("ProcessIndicationProviderModule"));
        Array <CIMName> propertyNames;
        propertyNames.append (CIMName ("IndicationTime"));
        propertyNames.append (CIMName ("IndicationIdentifier"));
        CIMPropertyList supportedProperties (propertyNames);
        _createCapabilityInstance (client, 
            String ("ProcessIndicationProviderModule"), 
            String ("ProcessIndicationProvider"),
            String ("DECapability01"),
            String ("CIM_ProcessIndication"),
            supportedProperties);
        _createFilterInstance (client, String ("DEFilter01"),
            String ("SELECT IndicationTime FROM cim_processindication"),
            qlang, NAMESPACE);
        _createFilterInstance (client, String ("DEFilter02"),
            String ("SELECT IndicationTime, IndicationIdentifier "
                    "FROM CIM_ProcessIndication"),
            qlang, NAMESPACE1);
        _createHandlerInstance (client, String ("DEHandler01"), 
            String ("localhost/CIMListener/test1"), NAMESPACE);
        _createHandlerInstance (client, String ("DEHandler02"), 
            String ("localhost/CIMListener/test1"), NAMESPACE2);
    }
    catch (Exception & e)
    {
        PEGASUS_STD (cerr) << "setup failed: " << e.getMessage ()
                           << PEGASUS_STD (endl);
        exit (-1);
    }

    PEGASUS_STD (cout) << "+++++ setup completed successfully"
                       << PEGASUS_STD (endl);
}

void _create (CIMClient & client)
{
    try
    {
        _createSubscriptionInstance (client, 
            _buildFilterOrHandlerPath (PEGASUS_CLASSNAME_INDFILTER, 
                String ("DEFilter01")),
            _buildFilterOrHandlerPath (PEGASUS_CLASSNAME_INDHANDLER_CIMXML,
                String ("DEHandler01")), NAMESPACE);
    }
    catch (Exception & e)
    {
        PEGASUS_STD (cerr) << "create failed: " << e.getMessage ()
                           << PEGASUS_STD (endl);
        exit (-1);
    }

    PEGASUS_STD (cout) << "+++++ create completed successfully"
                       << PEGASUS_STD (endl);
}

void _create2 (CIMClient & client, String& qlang)
{
    try
    {
        _createSubscriptionInstance (client, 
            _buildFilterOrHandlerPath (PEGASUS_CLASSNAME_INDFILTER, 
                String ("DEFilter02"), NAMESPACE1),
            _buildFilterOrHandlerPath (PEGASUS_CLASSNAME_INDHANDLER_CIMXML,
                String ("DEHandler02"), NAMESPACE2), NAMESPACE3);
    }
    catch (Exception & e)
    {
        PEGASUS_STD (cerr) << "create2 failed: " << e.getMessage ()
                           << PEGASUS_STD (endl);
        exit (-1);
    }

    PEGASUS_STD (cout) << "+++++ create2 completed successfully"
                       << PEGASUS_STD (endl);
}

void _delete (CIMClient & client)
{
    try
    {
        _deleteSubscriptionInstance (client, String ("DEFilter01"),
            String ("DEHandler01"));
    }
    catch (Exception & e)
    {
        PEGASUS_STD (cerr) << "delete failed: " << e.getMessage ()
                           << PEGASUS_STD (endl);
        exit (-1);
    }

    PEGASUS_STD (cout) << "+++++ delete completed successfully"
                       << PEGASUS_STD (endl);
}

void _delete2 (CIMClient & client)
{
    try
    {
        _deleteSubscriptionInstance (client, String ("DEFilter02"),
            String ("DEHandler02"), NAMESPACE1, NAMESPACE2, NAMESPACE3);
    }
    catch (Exception & e)
    {
        PEGASUS_STD (cerr) << "delete2 failed: " << e.getMessage ()
                           << PEGASUS_STD (endl);
        exit (-1);
    }

    PEGASUS_STD (cout) << "+++++ delete2 completed successfully"
                       << PEGASUS_STD (endl);
}

void _cleanup (CIMClient & client)
{
    try
    {
        _deleteHandlerInstance (client, String ("DEHandler01"), NAMESPACE);
        _deleteHandlerInstance (client, String ("DEHandler02"), NAMESPACE2);
        _deleteFilterInstance (client, String ("DEFilter01"), NAMESPACE);
        _deleteFilterInstance (client, String ("DEFilter02"), NAMESPACE1);
        _deleteCapabilityInstance (client, 
            String ("ProcessIndicationProviderModule"), 
            String ("ProcessIndicationProvider"),
            String ("DECapability01"));
        _deleteProviderInstance (client, 
            String ("ProcessIndicationProvider"), 
            String ("ProcessIndicationProviderModule"));
        _deleteModuleInstance (client, 
            String ("ProcessIndicationProviderModule"));
    }
    catch (Exception & e)
    {
        PEGASUS_STD (cerr) << "cleanup failed: " << e.getMessage ()
                           << PEGASUS_STD (endl);
        exit (-1);
    }

    PEGASUS_STD (cout) << "+++++ cleanup completed successfully"
                       << PEGASUS_STD (endl);
}

void _addProvider (CIMClient & client)
{
    try
    {
        _createProviderInstance (client, 
            String ("ProcessIndicationProvider2"), 
            String ("ProcessIndicationProviderModule"));
        Array <CIMName> propertyNames;
        propertyNames.append (CIMName ("IndicationTime"));
        CIMPropertyList supportedProperties (propertyNames);
        _createCapabilityInstance (client, 
            String ("ProcessIndicationProviderModule"), 
            String ("ProcessIndicationProvider2"),
            String ("DECapability02"),
            String ("CIM_ProcessIndication"),
            supportedProperties);
    }
    catch (Exception & e)
    {
        PEGASUS_STD (cerr) << "addProvider failed: " << e.getMessage ()
                           << PEGASUS_STD (endl);
        exit (-1);
    }

    PEGASUS_STD (cout) << "+++++ addProvider completed successfully"
                       << PEGASUS_STD (endl);
}

void _modifyProvider1 (CIMClient & client)
{
    try
    {
        Array <CIMName> propertyNames;
        propertyNames.append (CIMName ("IndicationTime"));
        propertyNames.append (CIMName ("IndicationIdentifier"));
        CIMPropertyList supportedProperties (propertyNames);
        _modifyCapabilityInstance (client, 
            String ("ProcessIndicationProviderModule"), 
            String ("ProcessIndicationProvider2"),
            String ("DECapability02"),
            supportedProperties);
    }
    catch (Exception & e)
    {
        PEGASUS_STD (cerr) << "modifyProvider1 failed: " << e.getMessage ()
                           << PEGASUS_STD (endl);
        exit (-1);
    }

    PEGASUS_STD (cout) << "+++++ modifyProvider1 completed successfully"
                       << PEGASUS_STD (endl);
}

void _modifyProvider2 (CIMClient & client)
{
    try
    {
        Array <CIMName> propertyNames;
        propertyNames.append (CIMName ("IndicationTime"));
        CIMPropertyList supportedProperties (propertyNames);
        _modifyCapabilityInstance (client, 
            String ("ProcessIndicationProviderModule"), 
            String ("ProcessIndicationProvider2"),
            String ("DECapability02"),
            supportedProperties);
    }
    catch (Exception & e)
    {
        PEGASUS_STD (cerr) << "modifyProvider2 failed: " << e.getMessage ()
                           << PEGASUS_STD (endl);
        exit (-1);
    }

    PEGASUS_STD (cout) << "+++++ modifyProvider2 completed successfully"
                       << PEGASUS_STD (endl);
}

void _deleteProvider (CIMClient & client)
{
    try
    {
        _deleteCapabilityInstance (client, 
            String ("ProcessIndicationProviderModule"), 
            String ("ProcessIndicationProvider2"),
            String ("DECapability02"));
        _deleteProviderInstance (client, 
            String ("ProcessIndicationProvider2"), 
            String ("ProcessIndicationProviderModule"));
    }
    catch (Exception & e)
    {
        PEGASUS_STD (cerr) << "deleteProvider failed: " << e.getMessage ()
                           << PEGASUS_STD (endl);
        exit (-1);
    }

    PEGASUS_STD (cout) << "+++++ deleteProvider completed successfully"
                       << PEGASUS_STD (endl);
}

int _test(CIMClient& client, const char* opt, String& qlang)
{
  if (String::equalNoCase (opt, "setup"))
  {  
    _setup (client, qlang);
  }
  else if (String::equalNoCase (opt, "create"))
  {
    _create (client);
  }
  else if (String::equalNoCase (opt, "create2"))
  {
    _create2 (client, qlang);
  }
  else if (String::equalNoCase (opt, "addProvider"))
  {
    _addProvider (client);
  }
  else if (String::equalNoCase (opt, "modifyProvider1"))
  {  
    _modifyProvider1 (client);
  }
  else if (String::equalNoCase (opt, "modifyProvider2"))
  {
    _modifyProvider2 (client);
  }
  else if (String::equalNoCase (opt, "deleteProvider"))
  {
    _deleteProvider (client);
  }
  else if (String::equalNoCase (opt, "delete"))
  {
    _delete (client);
  }
  else if (String::equalNoCase (opt, "delete2"))
  {
    _delete2 (client);
  }
  else if (String::equalNoCase (opt, "cleanup"))
  {
    _cleanup (client);
  }
  else
  {
    PEGASUS_STD (cerr) << "Invalid option: " << opt 
                       << PEGASUS_STD (endl);
    _usage ();
    return -1;
  }
  
  return 0;
}

int main (int argc, char** argv)
{
    CIMClient client;
    try
    {
        client.connectLocal ();
    }
    catch (Exception & e)
    {
        PEGASUS_STD (cerr) << e.getMessage () << PEGASUS_STD (endl);
        return -1;
    }

    if (argc != 3)
    {
        _usage ();
        return 1;
    }

    else
    {
        const char * opt = argv [1];
        const char * optLang = argv [2];
        String qlang(optLang);

#ifdef PEGASUS_DISABLE_CQL
        if (qlang == "CIM:CQL")
        {
          PEGASUS_STD (cout) << "+++++ cql test disabled" << PEGASUS_STD (endl);
          return 0;
        }
        else
        {
          return _test(client, opt, qlang);
        }
#else
        return _test(client, opt, qlang);
#endif
    }

    PEGASUS_UNREACHABLE( return 0; )
}
