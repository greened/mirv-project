# From http://make.paulandlesley.org/autodep.html

ifndef CXXPATTERNS_MK
CXXPATTERNS_MK := 1

include $(BUILDTOOLS)/configure/cxx.mk
include $(BUILDTOOLS)/cxxautodep.mk

%.o: %.cc
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXDEPS)
	$(if $(QUIET),$(info [CXX] $<))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.cpp
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXDEPS)
	$(if $(QUIET),$(info [CXX] $<))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.C
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXDEPS)
	$(if $(QUIET),$(info [CXX] $<))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@

%-pic.o: %.cc
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXDEPS)
	$(if $(QUIET),$(info [CXX] $<))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CXXPICFLAGS) $(CPPFLAGS) $< -o $@

%-pic.o: %.cpp	
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXDEPS)
	$(if $(QUIET),$(info [CXX] $<))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CXXPICFLAGS) $(CPPFLAGS) $< -o $@

%-pic.o: %.C
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXDEPS)
	$(if $(QUIET),$(info [CXX] $<))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CXXPICFLAGS) $(CPPFLAGS) $< -o $@

endif
