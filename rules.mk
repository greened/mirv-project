include $(BUILDTOOLS)/configure/cp.mk
include $(BUILDTOOLS)/configure/ln.mk
include $(BUILDTOOLS)/configure/touch.mk

INCLUDE_LINK = $(FINAL_BUILDDIR)/include/mirv

all: $(TARGETS)

$(TARGETS): $(INCLUDE_LINK)
#$(info $(TARGETS): $(INCLUDE_LINK))

$(ALL_OBJS): $(INCLUDE_LINK)
#$(info $(ALL_OBJS): $(INCLUDE_LINK))

$(INCLUDE_LINK):
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(LN) -sf $(SRCDIR)/src $@

include $(BUILDTOOLS)/cxxpatterns.mk

.PHONY: update_tests
update_tests: $(patsubst %.display,%.update,$(UNITTESTS))

.PHONY: replace_tests
replace_tests:
	$(foreach test,$(UNITTESTS),$(CP) $(FINAL_BUILDDIR)/$(patsubst %.display,%.update,$(test)) $(SRCDIR)/$(patsubst %.display,%.cpp,$(test)) ; )