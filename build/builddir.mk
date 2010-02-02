# From http://make.paulandlesley.org/multi-arch.html

# Jump to the build directory

.SUFFIXES:

include $(BUILDTOOLS)/configure/build_host.mk

PATHSEP_SUBST := __pathsep__

ifndef VERBOSE
MAKE_SILENT_DIRECTORY := --no-print-directory
endif

ifdef MDEBUG
BUILD_MAKEFLAGS += MDEBUG=$(MDEBUG)
endif

MAKETARGET = $(MAKE) $(MAKE_SILENT_DIRECTORY) -C $(subst $(PATHSEP_SUBST),/,$(1)) -f $(BUILDTOOLS)/top.mk \
                     FLAVOR=$(notdir $(subst $(PATHSEP_SUBST),/,$(1))) SRCDIR=$(CURDIR) BUILDTOOLS=$(BUILDTOOLS) FINAL_BUILDDIR=$(subst $(PATHSEP_SUBST),/,$(1)) $(BUILD_MAKEFLAGS) $(MAKECMDGOALS)

define make_builddir_target_impl
$$(call debug,[build] Make $(1))

.PHONY: $(1)
$(1):
	+$(QUIET)[ -d $(subst $(PATHSEP_SUBST),/,$(1)) ] || mkdir -p $(subst $(PATHSEP_SUBST),/,$(1))
	+$(QUIET)[ -d $(subst $(PATHSEP_SUBST),/,$(1))/configure ] || mkdir -p $(subst $(PATHSEP_SUBST),/,$(1))/configure
	+$(QUIET)$(MAKETARGET)
endef

make_builddir_target = $(eval $(call make_builddir_target_impl,$(1)))

# We don't have a build directory so we can't test for tools yet.  So
# hardwire mkdir and rm and hope they work.

BUILDDIR_TARGETS := $(foreach builddir,$(FINAL_BUILDDIRS),$(subst /,$(PATHSEP_SUBST),$(builddir)))

$(call debug,BUILDDIR_TARGETS = $(BUILDDIR_TARGETS))

$(foreach builddir_target,$(BUILDDIR_TARGETS),$(call make_builddir_target,$(builddir_target)))

Makefile : ;
%.mk :: ;

% :: $(BUILDDIR_TARGETS) ; :

BUILDCLEAN_TARGETS := $(foreach builddir,$(FINAL_BUILDDIRS),$(subst /,$(PATHSEP_SUBST),$(builddir)_clean))

.PHONY: clean
clean: $(BUILDCLEAN_TARGETS)

.PHONY: %_clean
%_clean:
	rm -rf $(subst $(PATHSEP_SUBST),/,$*)
