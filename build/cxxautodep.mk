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

CXXMAKEDEPEND = $(if $(CONFIG_HAVE_GXX),$(CXX) -MM $(CPPFLAGS) -o $*.dd $<,$(if $(CONFIG_HAVE_CLANGXX),$(CXX) -MM $(CPPFLAGS) -o $*.dd $<, $(CPP) $(CPPFLAGS) $< | $(SED) -n 's%^\# *[0-9][0-9]* *"\([^"]*\)".*%$*.o: \1%p' | $(SORT) | $(UNIQ) > $*.dd))

CXXSOMAKEDEPEND = $(if $(CONFIG_HAVE_GXX),$(CXX) -MM $(CPPFLAGS) -o $*.ddo $<,$(if $(CONFIG_HAVE_CLANGXX),$(CXX) -MM $(CPPFLAGS) -o $*.ddo $<,$(CPP) $(CPPFLAGS) $< | $(SED) -n 's%^\# *[0-9][0-9]* *"\([^"]*\)".*%$*.o: \1%p' | $(SORT) | $(UNIQ) > $*.ddo))

#ifdef CONFIG_HAVE_GXX
#  CXXMAKEDEPEND = $(CXX) -MM $(CPPFLAGS) -o $*.dd $<
#else
#  CXXMAKEDEPEND = $(CPP) $(CPPFLAGS) $< \
                        | $(SED) -n 's/^\# *[0-9][0-9]* *"\([^"]*\)".*/$*.o: \1/p' \
                        | $(SORT) | $(UNIQ) > $*.dd
#endif

PROCESS_CXXDEPS = $(CP) -f $*.dd $*.PP && \
                      $(SED) -i -e 's%$(subst .,\.,$(@F))%$@%' $*.PP && \
                      $(SED) -e 's%\#.*%%' -e 's%^[^:]*: *%%' -e 's% *\\$$%%' \
                             -e 's%^[ ]*%%' -e '/^$$/ d' \
                             -e 's%$$%:%' < $*.dd >> $*.PP && \
                  $(MV) -f $*.PP $*.dd

PROCESS_CXXSODEPS = $(CP) -f $*.ddo $*.PPo && \
                      $(SED) -i -e 's%$(subst .,\.,$(@F))%$@%' $*.PPo && \
                      $(SED) -e 's%\#.*%%' -e 's%^[^:]*: *%%' -e 's% *\\$$%%' \
                             -e 's%^[ ]*%%' -e '/^$$/ d' \
                             -e 's%$$%:%' < $*.ddo >> $*.PPo && \
                  $(MV) -f $*.PPo $*.ddo

endif
