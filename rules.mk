include $(BUILDTOOLS)/configure/ln.mk
include $(BUILDTOOLS)/configure/touch.mk

IFLAGS = -I$(SRCDIR)/src/lib -I$(FINAL_BUILDDIR)/include
DFLAGS = -DBOOST_MPL_LIMIT_VECTOR_SIZE=40 -DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS

debug_CFLAGS := -g -Wall -Werror
debug_CXXFLAGS := -g -Wall -Werror
debug_CPPFLAGS = -DDEBUG $(IFLAGS) $(DFLAGS)

opt_CFLAGS := -O2 -Wall -Werror
opt_CXXFLAGS := -O2 -Wall -Werror
opt_CPPFLAGS = -O2 $(IFLAGS) $(DFLAGS)

INCLUDE_LINK = $(FINAL_BUILDDIR)/include/mirv

all: $(TARGETS)

$(TARGETS): $(INCLUDE_LINK)
$(info $(TARGETS): $(INCLUDE_LINK))

$(ALL_OBJS): $(INCLUDE_LINK)
$(info $(ALL_OBJS): $(INCLUDE_LINK))

$(INCLUDE_LINK):
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(LN) -sf $(SRCDIR)/src $@

include $(BUILDTOOLS)/cxxpatterns.mk
