.SUFFIXES:

ifndef SRCDIR
  SRCDIR := $(CURDIR)
endif

-include $(SRCDIR)/setup.mk
include $(SRCDIR)/package.mk

ifndef BUILDTOOLS
  BUILDTOOLS := $(wildcard $(SRCDIR)/build)
  ifeq ($(BUILDTOOLS),)
    $(error Cannot find build tools)
  endif
endif

include $(BUILDTOOLS)/debug.mk

ifndef FLAVORS
   FLAVORS := default
endif

ifndef BUILDDIR
  BUILDDIR := /tmp/$(USER)/$(PACKAGE)
endif

ifndef VERBOSE
QUIET:=@
endif


include $(BUILDTOOLS)/bootstrap.mk
