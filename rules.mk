include $(BUILDTOOLS)/configure/ln.mk
include $(BUILDTOOLS)/configure/touch.mk

IFLAGS = -I$(SRCDIR)/src/lib -I$(FINAL_BUILDDIR)/include

debug_CFLAGS := -g
debug_CXXFLAGS := -g
debug_CPPFLAGS = -DDEBUG $(IFLAGS)

opt_CFLAGS := -O2
opt_CXXFLAGS := -O2
opt_CPPFLAGS = -O2 $(IFLAGS)

INCLUDE_LINK = $(FINAL_BUILDDIR)/include/mirv

all: $(TARGETS)

$(TARGETS): $(INCLUDE_LINK)
$(info $(TARGETS): $(INCLUDE_LINK))

$(ALL_OBJS): $(INCLUDE_LINK)
$(info $(ALL_OBJS): $(INCLUDE_LINK))

$(INCLUDE_LINK):
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(LN) -sf $(SRCDIR)/src/core $@

include $(BUILDTOOLS)/cxxpatterns.mk
