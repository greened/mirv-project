# From http://make.paulandlesley.org/autodep.html

ifndef CPATTERNS_MK
CPATTERNS_MK := 1

include $(BUILDTOOLS)/configure/c.mk
include $(BUILDTOOLS)/cautodep.mk

%.o: %.c	
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CMAKEDEPEND)
	$(QUIET)$(PROCESS_CDEPS)
	$(if $(QUIET),$(info [CC] $@))
	$(QUIET)$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

%-pic.o: %.c
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CMAKEDEPEND)
	$(QUIET)$(PROCESS_CDEPS)
	$(if $(QUIET),$(info [CC] $@))
	$(QUIET)$(CC) $(CFLAGS) $(CPICFLAGS) $(CPPFLAGS) -c $< -o $@

endif