#//%2005////////////////////////////////////////////////////////////////////////
#//
#// Copyright (c) 2000, 2001, 2002 BMC Software; Hewlett-Packard Development
#// Company, L.P.; IBM Corp.; The Open Group; Tivoli Systems.
#// Copyright (c) 2003 BMC Software; Hewlett-Packard Development Company, L.P.;
#// IBM Corp.; EMC Corporation, The Open Group.
#// Copyright (c) 2004 BMC Software; Hewlett-Packard Development Company, L.P.;
#// IBM Corp.; EMC Corporation; VERITAS Software Corporation; The Open Group.
#// Copyright (c) 2005 Hewlett-Packard Development Company, L.P.; IBM Corp.;
#// EMC Corporation; VERITAS Software Corporation; The Open Group.
#//
#// Permission is hereby granted, free of charge, to any person obtaining a copy
#// of this software and associated documentation files (the "Software"), to
#// deal in the Software without restriction, including without limitation the
#// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
#// sell copies of the Software, and to permit persons to whom the Software is
#// furnished to do so, subject to the following conditions:
#// 
#// THE ABOVE COPYRIGHT NOTICE AND THIS PERMISSION NOTICE SHALL BE INCLUDED IN
#// ALL COPIES OR SUBSTANTIAL PORTIONS OF THE SOFTWARE. THE SOFTWARE IS PROVIDED
#// "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
#// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
#// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
#// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
#// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
#// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#//
#//==============================================================================
# Recurse the directories defined in DIRS
# This makefile simply recurses all of the toplevel provider directories

ROOT = ../../..

include $(ROOT)/mak/config.mak

#Directories to recurse
DIRS = \
	AlertIndicationProvider \
	ClientProvider \
	DefaultInstanceProvider \
	DynamicIndicationProvider \
	FamilyProvider \
	FamilyProvider/tests \
	IndicationStressTestProvider \
	IndicationStressTestProvider/testclient \
	InstanceProvider \
	InstanceProvider/testclient \
	MethodTestProvider \
	MethodTestProvider/testclient \
	MCCA_TestAssocProvider \
	PG_TestPropertyTypes \
	ProcessIndicationProvider \
	UserContextTestProvider \
	UserContextTestProvider/testclient

ifdef PEGASUS_ENABLE_CMPI_PROVIDER_MANAGER
DIRS += \
	CMPI/TestIndications \
	CMPI/TestIndications/tests \
	CMPI/TestMethod \
	CMPI/TestMethod/tests \
	CMPI/TestFail_1 \
	CMPI/TestFail_1/tests \
	CMPI/TestFail_2 \
	CMPI/TestFail_2/tests\
	CMPI/TestFail_3 \
	CMPI/TestFail_3/tests \
	CMPI/TestFail_4 \
	CMPI/TestFail_4/tests \
	CMPI/TestProviderThreaded \
	CMPI/TestProviderThreaded/tests



ifdef PEGASUS_ENABLE_EXECQUERY
DIRS += \
	CMPI/TestInstance \
	CMPI/TestInstance/tests
endif
endif
include $(ROOT)/mak/recurse.mak
