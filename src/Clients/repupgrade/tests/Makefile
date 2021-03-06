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
ROOT = ../../../..

OLD_REPOSITORY_PATH = ./
NEW_REPOSITORY_PATH = ./newRepository
RESULTDIR           = ./newRepository/test\#repupgrade
TESTNAMESPACE       = test/repupgrade
RESULTFILE 	    =  $(TMP_DIR)/result
MASTERRESULTFILE    = $(ROOT)/src/$(DIR)/result.master

DIR = Clients/repupgrade/tests

include $(ROOT)/mak/config.mak

LIBRARIES = \
    pegcommon \
    pegrepository 

LOCAL_DEFINES = -DPEGASUS_INTERNALONLY

PROGRAM = upgradetest

SOURCES = UpgradeTest.cpp

include $(ROOT)/mak/program.mak
include $(ROOT)/mak/test.mak

tests:
ifndef PEGASUS_USE_RELEASE_DIRS
	@$(RMREPOSITORY) $(OLD_REPOSITORY_PATH)repository/test#repupgrade
	@$(RMREPOSITORY) ./newRepository
	@ cimmofl "-R$(OLD_REPOSITORY_PATH)" "-M$(REPOSITORY_MODE)" "-I$(CIM_SCHEMA_DIR)" "-n$(TESTNAMESPACE)" $(ALLOW_EXPERIMENTAL) $(CIM_SCHEMA_DIR)/Core$(CIM_SCHEMA_VER)_Qualifiers.mof
	@ cimmofl "-R$(OLD_REPOSITORY_PATH)" "-M$(REPOSITORY_MODE)" "-n$(TESTNAMESPACE)" $(ROOT)/src/Providers/TestProviders/Load/TST_Family.mof
	@ $(MKDIRHIER) $(NEW_REPOSITORY_PATH)
	@repupgrade -o $(OLD_REPOSITORY_PATH)repository -n $(NEW_REPOSITORY_PATH)
	$(PROGRAM) $(OLD_REPOSITORY_PATH)repository $(NEW_REPOSITORY_PATH)
	@$(RM) $(RESULTFILE)
	@$(MAKE) -i -s tests_ignoreerror
	@$(STRIPCRS)
	@$(COMPARERESULTS)
	@$(ECHO) +++ Test passed +++
endif

tests_ignoreerror:
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Display usage - error" >> $(RESULTFILE)
	@repupgrade >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Display Usage - error" >> $(RESULTFILE)
	@repupgrade -o  >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Display Usage - error" >> $(RESULTFILE)
	@repupgrade -n  >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Display help message" >> $(RESULTFILE)
	@repupgrade -h  >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Display help message" >> $(RESULTFILE)
	@repupgrade --help  >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Non-existent old repository - error case" >> $(RESULTFILE)
	@repupgrade -o non-existent-dir  >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Non-existent new repository - error case" >> $(RESULTFILE)
	@repupgrade -n non-existent-dir  >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Combining options - error case" >> $(RESULTFILE)
	@repupgrade -h -o >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Combining options - error case" >> $(RESULTFILE)
	@repupgrade -h -r >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Combining options - error case" >> $(RESULTFILE)
	@repupgrade -h -n >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Combining options - error case" >> $(RESULTFILE)
	@repupgrade -o -h >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Combining options - error case" >> $(RESULTFILE)
	@repupgrade -n -h >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Combining options - error case" >> $(RESULTFILE)
	@repupgrade -o -o >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Combining options - error case" >> $(RESULTFILE)
	@repupgrade -n -n >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Combining options - error case" >> $(RESULTFILE)
	@repupgrade -h -h >> $(RESULTFILE) $(REDIRECTERROR)
	@$(ECHO) "  " >> $(RESULTFILE)
	@$(ECHO) "Combining options - error case" >> $(RESULTFILE)
	@repupgrade -v -v >> $(RESULTFILE) $(REDIRECTERROR)

poststarttests:

