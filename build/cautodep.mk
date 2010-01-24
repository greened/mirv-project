# From http://make.paulandlesley.org/autodep.html

ifndef AUTODEP_MK
AUTODEP_MK := 1

include $(BUILDTOOLS)/configure/cp.mk
include $(BUILDTOOLS)/configure/mv.mk
include $(BUILDTOOLS)/configure/sed.mk
include $(BUILDTOOLS)/configure/sort.mk
include $(BUILDTOOLS)/configure/uniq.mk
include $(BUILDTOOLS)/configure/cc.mk
include $(BUILDTOOLS)/configure/cpp.mk

CMAKEDEPEND = $(if $(CONFIG_HAVE_GCC),$(CC) -MM $(CPPFLAGS) -o $*.d $<,$(CPP) $(CPPFLAGS) $< | $(SED) -n 's%^\# *[0-9][0-9]* *"\([^"]*\)".*%$*.o: \1%p' | $(SORT) | $(UNIQ) > $*.d

#ifdef HAVE_GCC
#  CMAKEDEPEND = $(CC) -MM $(CPPFLAGS) -o $*.d $<
#else
#  CMAKEDEPEND = $(CPP) $(CPPFLAGS) $< \
#                      | $(SED) -n 's/^\# *[0-9][0-9]* *"\([^"]*\)".*/$*.o: \1/p' \
#                      | $(SORT) | $(UNIQ) > $*.d
#endif

PROCESS_CDEPS = $(CP) -f $*.d $*.P && \
                    $(SED) -i -e 's%$(@F)%$@%' $*.P && \
                    $(SED) -e 's%#.*%%' -e 's%^[^:]*: *%%' -e 's% *\\$$%%' \
                           -e '\%^$$% d' -e 's%$$% :%' < $*.d >> $*.P && \
                $(MV) -f $*.P $*.d
endif
