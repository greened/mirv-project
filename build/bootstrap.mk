# From http://make.paulandlesley.org/multi-arch.html

include $(BUILDTOOLS)/configure/configure.mk

ORIGPACKAGE += $(PACKAGE)

# We don't have a build directory yet, so hard-code find and hope for
# the best.
TARGET_INCLUDES := $(filter-out $(BUILDTOOLS)/configure/target.mk,$(shell find $(SRCDIR) -name "target.mk" -print))
PACKAGE_INCLUDES := $(filter-out $(SRCDIR)/package.mk,$(shell find $(SRCDIR) -name "package.mk" -print))

$(call debug,TARGET_INCLUDES  = $(TARGET_INCLUDES))
$(call debug,PACKAGE_INCLUDES = $(PACKAGE_INCLUDES))

override PACKAGE := $(lastword $(ORIGPACKAGE))
override ORIGPACKAGE := $(filer-out $(PACKAGE),$(ORIGPACKAGE))

FINAL_BUILDDIRS := $(strip $(foreach flavor,$(FLAVORS),$(BUILDDIR)/$(BUILD_HOST)/$(flavor) ))

ifeq ($(strip $(filter $(CURDIR),$(FINAL_BUILDDIRS))),)
# Not in any build directory
include $(BUILDTOOLS)/builddir.mk
else
# Run the build
VPATH := $(SRCDIR)

$(call debug,VPATH = $(VPATH))

# This is needed because the TARGET_INCLUDES could introduce rules and
# we don't want those to become the default.
.PHONY: default
default: all

-include $(TARGET_INCLUDES) ""
-include $(PACKAGE_INCLUDES) ""

$(call debug,TARGETS = $(TARGETS))

# This must come before we include configure makefiles because this
# may add more things to be configured
include $(SRCDIR)/rules.mk

CFLAGS = $($(FLAVOR)_CFLAGS)
CXXFLAGS = $($(FLAVOR)_CXXFLAGS)
CPPFLAGS = $($(FLAVOR)_CPPFLAGS)

$(call debug,CFLAGS = $(CFLAGS))
$(call debug,CXXFLAGS = $(CXXFLAGS))
$(call debug,CPPFLAGS = $(CPPFLAGS))

$(call debug,CONFIGURE_INCLUDES = $(CONFIGURE_INCLUDES))
include $(CONFIGURE_INCLUDES)

# TODO: Move this into another variable of conditional includes
ifeq ($(CONFIG_HAVE_GXX),yes)
  include $(BUILDTOOLS)/configure/gxx.mk
endif


endif
