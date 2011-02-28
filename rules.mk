include $(BUILDTOOLS)/configure/ln.mk
include $(BUILDTOOLS)/configure/touch.mk

IFLAGS = -I$(SRCDIR)/src/Library -I$(FINAL_BUILDDIR)/include
DFLAGS = -DBOOST_MPL_LIMIT_VECTOR_SIZE=50 -DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
# -DBOOST_SP_NO_SP_CONVERTIBLE=1

#debug_CFLAGS := -g -Wall -Werror
#debug_CXXFLAGS := -g -Wall -Werror
debug_CFLAGS := -g -Wall
debug_CXXFLAGS := -g -Wall -std=c++0x
debug_CPPFLAGS = -DDEBUG $(IFLAGS) $(DFLAGS) -std=c++0x

#opt_CFLAGS := -O2 -Wall -Werror
#opt_CXXFLAGS := -O2 -Wall -Werror
opt_CFLAGS := -O2 -Wall
opt_CXXFLAGS := -O2 -Wall -std=c++0x
opt_CPPFLAGS = -O2 $(IFLAGS) $(DFLAGS) -std=c++0x

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
