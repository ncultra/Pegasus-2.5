ROOT = $(PEGASUS_ROOT)

PEGASUS_ZOS_PROGRAM_OBJECT = yes

DIR = InterfaceArchive/$(VERSION)/tests/Clients/EnumInstances

include ../../../version.mak
include $(ROOT)/mak/config.mak
include ../libraries.mak

EXTRA_INCLUDES = 

PROGRAM = EnumInstances
SOURCES = EnumInstances.cpp

include $(ROOT)/mak/program.mak

ifdef PEGASUS_TEST_BACKWARD_COMPATIBILITY
  INCLUDES = -I$(ROOT)/InterfaceArchive/$(VERSION)/include
endif 

tests:

poststarttests:
	@$(PROGRAM)
