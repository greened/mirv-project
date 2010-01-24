# From http://make.paulandlesley.org/autodep.html

ifndef AUTODEP_MK
AUTODEP_MK := 1

include $(BUILDTOOLS)/configure/cp.mk
include $(BUILDTOOLS)/configure/mv.mk
include $(BUILDTOOLS)/configure/sed.mk
include $(BUILDTOOLS)/configure/sort.mk
include $(BUILDTOOLS)/configure/uniq.mk
include $(BUILDTOOLS)/configure/cxx.mk
#include $(BUILDTOOLS)/configure/cc.mk
#include $(BUILDTOOLS)/configure/cpp.mk
include $(BUILDTOOLS)/configure/cxxpp.mk

ifdef HAVE_GCC
  CMAKEDEPEND = $(CC) -MM $(CPPFLAGS) -o $*.d $<
  CXXMAKEDEPEND = $(CXX) -MM $(CPPFLAGS) -o $*.dd $<
else
  CMAKEDEPEND = $(CPP) $(CPPFLAGS) $< \
                      | $(SED) -n 's%^\# *[0-9][0-9]* *"\([^"]*\)".*%$*.o: \1%p' \
                      | $(SORT) | $(UNIQ) > $*.d
  CXXMAKEDEPEND = $(CPP) $(CPPFLAGS) $< \
                        | $(SED) -n 's%^\# *[0-9][0-9]* *"\([^"]*\)".*%$*.o: \1%p' \
                        | $(SORT) | $(UNIQ) > $*.dd
endif

PROCESS_CDEPS := $(CP) $*.d $*.P && cat $*.d && \
                    $(SED) -e 's%#.*%%' -e 's%^[^:]*: *%%' -e 's% *\\$$%%' \
                           -e '%^$$% d' -e 's%$$% :%' < $*.d >> $*.P && \
                 $(MV) -f $*.P $*.d

PROCESS_CXXDEPS := $(CP) $*.dd $*.PP && cat $*.dd && \
                      $(SED) -e 's%#.*%%' -e 's%^[^:]*: *%%' -e 's% *\\$$%%' \
                             -e '%^$$% d' -e 's%$$% :%' < $*.dd >> $*.PP && \
                   $(MV) -f $*.PP $*.dd

endif
