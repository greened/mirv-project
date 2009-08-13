# From http://make.paulandlesley.org/autodep.html

ifndef AUTODEP_MK
AUTODEP_MK := 1

include $(BUILDTOOLS)/configure/cp.mk
include $(BUILDTOOLS)/configure/mv.mk
include $(BUILDTOOLS)/configure/sed.mk
include $(BUILDTOOLS)/configure/sort.mk
include $(BUILDTOOLS)/configure/uniq.mk
include $(BUILDTOOLS)/configure/cxx.mk
include $(BUILDTOOLS)/configure/cxxpp.mk

CXXMAKEDEPEND = $(if $(CONFIG_HAVE_GXX),$(CXX) -MM $(CPPFLAGS) -o $*.dd $<,$(CPP) $(CPPFLAGS) $< | $(SED) -n 's/^\# *[0-9][0-9]* *"\([^"]*\)".*/$*.o: \1/p' | $(SORT) | $(UNIQ) > $*.dd)

#ifdef CONFIG_HAVE_GXX
#  CXXMAKEDEPEND = $(CXX) -MM $(CPPFLAGS) -o $*.dd $<
#else
#  CXXMAKEDEPEND = $(CPP) $(CPPFLAGS) $< \
                        | $(SED) -n 's/^\# *[0-9][0-9]* *"\([^"]*\)".*/$*.o: \1/p' \
                        | $(SORT) | $(UNIQ) > $*.dd
#endif

PROCESS_CXXDEPS = $(CP) $*.dd $*.PP; \
                      $(SED) -e 's/\#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
                             -e '/^$$/ d' -e 's/$$/ :/' < $*.dd >> $*.PP; \
                  $(MV) -f $*.PP $*.dd

endif
