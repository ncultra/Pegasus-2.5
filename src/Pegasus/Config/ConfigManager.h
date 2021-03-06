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
// Author: Nag Boranna (nagaraja_boranna@hp.com)
//
// Modified By: Sushma Fernandes, Hewlett-Packard Company
//                 (sushma_fernandes@hp.com)
//              Roger Kumpf, Hewlett-Packard Company (roger_kumpf@hp.com)
//              Aruran, IBM (ashanmug@in.ibm.com) for Bug# 3612
//              Aruran, IBM (ashanmug@in.ibm.com) for Bug# 3610
//              David Dillard, VERITAS Software Corp.
//                  (david.dillard@veritas.com)
//
//%////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
// This file defines the classes necessary to manage configuration properties
// specified on the commandline and configuration files for Pegasus.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef Pegasus_ConfigManager_h
#define Pegasus_ConfigManager_h

#include <cctype>
#include <stdlib.h>
#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/String.h>
#include <Pegasus/Common/ArrayInternal.h>
#include <Pegasus/Common/InternalException.h>
#include <Pegasus/Common/AutoPtr.h>
#include <Pegasus/Common/HashTable.h>
#include <Pegasus/Config/ConfigPropertyOwner.h>
#include <Pegasus/Config/ConfigFileHandler.h>

#include <Pegasus/Config/TracePropertyOwner.h>
#include <Pegasus/Config/LogPropertyOwner.h>
#include <Pegasus/Config/DefaultPropertyOwner.h>
#include <Pegasus/Config/SecurityPropertyOwner.h>
#include <Pegasus/Config/RepositoryPropertyOwner.h>
#include <Pegasus/Config/ShutdownPropertyOwner.h>
#include <Pegasus/Config/FileSystemPropertyOwner.h>
#include <Pegasus/Config/ProviderDirPropertyOwner.h>
#include <Pegasus/Config/NormalizationPropertyOwner.h>

PEGASUS_NAMESPACE_BEGIN

/**
  This class reads configuration properties from the config file, maps the
  properties to owners, and implements access methods.
*/

class PEGASUS_CONFIG_LINKAGE ConfigManager
{

private:

    /**
        Refers to the singleton ConfigManager instance.  If no ConfigManager
        instance has been constructed, this value is null.
     */
    static ConfigManager* _instance;


    /** Constructor. */
    ConfigManager();


    /**
    Initialize config property with the value specified as a
    command line option.

    @param configOption    name and value of the command line option.

    @exception InvalidPropertyValue  if property value is not valid.
    @exception UnrecognizedConfigProperty  if property is not defined.
    */
    Boolean _initPropertyWithCommandLineOption(
        const String& configOption);


    /**
    load config properties from the file

    @exception CannotRenameFile  if failed to rename the config file.
    @exception CannotOpenFile if failed to set permissions on the config file.
    @exception ConfigFileSyntaxError  if there are synatx error
                            while parsing the config files.
    */
    void _loadConfigProperties();


    /**
    Initialize config property owners and add them to the property owner table
    */
    void _initPropertyTable();

    /**
    HashTable used to identify owners.
    */
    typedef HashTable<String,
        ConfigPropertyOwner*,EqualFunc<String>,HashFunc<String> > OwnerTable;

    /**
    HashTable used to identify fixed values.
    */
    typedef HashTable<String,
        const char*,EqualFunc<String>,HashFunc<String> > FixedValueTable;

    /*
    friend declaration needed by some compilers to allow OwnerTable and
    FixedValueTable to be accessible from PropertyTable.
    */
    struct PropertyTable;
    friend struct ConfigManager::PropertyTable;

    /**
    Structure used to identify properties.
    */
    struct PropertyTable
    {
        OwnerTable ownerTable;
        FixedValueTable fixedValueTable;
    };

    /**
    HashTable to store the config property names and
    property owners
    */
    AutoPtr<PropertyTable> _propertyTable; //PEP101

    /**
    Handler to access the config files.
    */
    AutoPtr<ConfigFileHandler>    _configFileHandler; //PEP101

    /**
    Pegasus home variable
    */
    static String	  _pegasusHome;

public:

    /**
    Default location of Pegasus home.
    */
    static const String PEGASUS_HOME_DEFAULT;


    /**
    Property Owners

    When a new property owner is created be sure to add static
    variable pointers for each of the new property owner.
    */
    static TracePropertyOwner*      traceOwner;

    static LogPropertyOwner*        logOwner;

    static DefaultPropertyOwner*    defaultOwner;

    static SecurityPropertyOwner*   securityOwner;

    static RepositoryPropertyOwner* repositoryOwner;

    static ShutdownPropertyOwner*   shutdownOwner;

    static FileSystemPropertyOwner*   fileSystemOwner;

    static ProviderDirPropertyOwner*	providerDirOwner;

    static NormalizationPropertyOwner * normalizationOwner;

    /**
        Boolean indicating whether configuration data should be read from
        and persisted to configuration files.  If true, this ConfigManager
        instance actively manages the configuration data.  All operations
        are functional and updates are written to the configuration files.  If
        false, the ConfigManager does not read to or write from configuration
        files.  In addition, property values are not validated.  When this
        value is false, the behavior of methods that specifically implicate
        configuration files is not defined.  The default value is false.
     */
    Boolean useConfigFiles;

    /**
        Get a reference to the singleton ConfigManager instance.  If no
        ConfigManager instance exists, construct one.
    */
    static ConfigManager* getInstance();


    /**
    Terminate the ConfigManager.
    */
    static void destroy( void );
    /**
    Initialize the current value of a config property.

    @param  propertyName  The name of the property to initialize (e.g.,
                          "httpPort").
    @param  propertyValue The initial value of the property.
    @return true if the property found and initialized, else false.

    @exception UnrecognizedConfigProperty  if property is not defined.
    @exception InvalidPropertyValue  if property value is not valid.
    */
    Boolean initCurrentValue(
        const String& name,
        const String& value);

    /**
    Update current value of a property.

    @param  propertyName  The name of the property to update (eg. "httpPort").
    @param  propertyValue The new value of the property.  If the value is
                          null, the property should be reset to its default
                          value.
    @param  unset         Specifies whether the property should be updated
                          or unset.
    @return true if the property found and updated, else false.

    @exception NonDynamicConfigProperty  if property is not dynamic.
    @exception UnrecognizedConfigProperty  if property is not defined.
    @exception InvalidPropertyValue  if property value is not valid.
    */
    Boolean updateCurrentValue(
        const String& name,
        const String& value,
        Boolean unset);

    /**
    Update planned value of a property.

    @param  propertyName  The name of the property to update (eg. "httpPort").
    @param  propertyValue The new value of the property.  If the value is
                          null, the property should be reset to its default
                          value.
    @param  unset         Specifies whether the property should be updated
                          or unset.
    @return Boolean       True if the property found and updated.

    @exception NonDynamicConfigProperty  if property is not dynamic.
    @exception UnrecognizedConfigProperty  if property is not defined.
    @exception InvalidPropertyValue  if property value is not valid.
    */
    Boolean updatePlannedValue(
        const String& name,
        const String& value,
        Boolean unset);


    /**
    Validate the value of a property.

    @param  name    The name of the property.
    @param  value   The value of the property to be validated.
    @return true if the value of the property is valid, else false.

    @exception UnrecognizedConfigProperty  if property is not defined.
    */
    Boolean validatePropertyValue(const String& name, const String& value);

    /**
    Get default value of the specified property.

    @param  name    The name of the property.
    @return string containing the default value of the specified property.

    @exception UnrecognizedConfigProperty  if property is not defined.
    */
    String getDefaultValue(const String& name) const;


    /**
    Get current value of the specified property.

    @param  name    The name of the property.
    @return string containing the current value of the specified property.

    @exception UnrecognizedConfigProperty  if property is not defined.
    */
    String getCurrentValue(const String& name) const;


    /**
    Get planned value of the specified property.

    @param  name    The name of the property.
    @return string containing the current value of the specified property.

    @exception UnrecognizedConfigProperty  if property is not defined.
    */
    String getPlannedValue(const String& name) const;


    /**
    Get all the attributes of the specified property.

    @param name          The name of the property.
    @param propertyInfo  List containing the property info.

    @exception UnrecognizedConfigProperty  if property is not defined.
    */
    void getPropertyInfo(const String& name, Array<String>& propertyInfo) const;


    /**
    Get a list of all the property names.

    @param propertyNames  List containing all the property names.
    @param includeHiddenProperties  Boolean indicating whether hidden
                                    properties should be included in the
                                    returned list.
    */
    void getAllPropertyNames(
        Array<String>& propertyNames,
        Boolean includeHiddenProperties) const;

    /**
    Merges the config properties from the specified configuration files.

    @param currentFile   Name of file that contains current config properties.
    @param plannedFile   Name of file that contains planned config properties.

    @exception NoSuchFile  if the specified config file does not exist.
    @exception FileNotReadable  if the specified config file is not readable.
    @exception CannotRenameFile  if failed to rename the config file.
    @exception CannotOpenFile if failed to set permissions on the config file.
    @exception ConfigFileSyntaxError  if there is synatx error
                            while parsing the config files.
    @exception InvalidPropertyValue if validation fails for a config property
        in either file.
    @exception UnrecognizedConfigProperty if a config property specified in
        either file is not defined.
    */
    void mergeConfigFiles(const String& currentFile, const String& plannedFile);

    /**
    Merge the config properties from the default planned config file
    with the properties in the default current config file.

    @exception NoSuchFile  if the default config file does not exist.
    @exception FileNotReadable  if the default config file is not readable.
    @exception CannotRenameFile  if failed to rename the config file.
    @exception CannotOpenFile if failed to set permissions on the config file.
    @exception ConfigFileSyntaxError  if there are synatx error
                            while parsing the config files.
    @exception InvalidPropertyValue if validation fails for a config property
        in either file.
    @exception UnrecognizedConfigProperty if a config property specified in
        either file is not defined.
    */
    void mergeConfigFiles();


    /**
    Merge option values from the command line.

    @param argc number of argument on the command line.
    @param argv list of command line arguments.

    @exception  InvalidPropertyValue if validation fails.
    @exception  UnrecognizedConfigProperty  if property is not defined.
    */
    void mergeCommandLine(int& argc, char**& argv);


    /**
    Get Pegasus Home
    */
    static String getPegasusHome();

    /**
    Set Pegasus Home
    */
    static void setPegasusHome(const String& home);

    /**
    Get Homed Path
    This function checks if the argument passed is an absolute path.
    If true then it returns the same value. Else, it prepends
    the value of pegasusHome to the value.
    */
    static String getHomedPath(const String& value);

};

PEGASUS_NAMESPACE_END

#endif /* Pegasus_ConfigManager_h */

