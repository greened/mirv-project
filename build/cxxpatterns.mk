# From http://make.paulandlesley.org/autodep.html

ifndef CXXPATTERNS_MK
CXXPATTERNS_MK := 1

include $(BUILDTOOLS)/configure/cxx.mk
include $(BUILDTOOLS)/cxxautodep.mk

%.o: %.cc
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXDEPS)
	$(if $(QUIET),$(info [CXX] $@))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -c -o $@

%.o: %.cpp
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXDEPS)
	$(if $(QUIET),$(info [CXX] $@))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -c -o $@

%.o: %.C
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXDEPS)
	$(if $(QUIET),$(info [CXX] $@))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -c -o $@

%-pic.o: %.cc
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXSOMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXSODEPS)
	$(if $(QUIET),$(info [CXX] $@))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CXXPICFLAGS) $(CPPFLAGS) $< -c -o $@

%.dso: %.cc
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXSOMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXSODEPS)
	$(if $(QUIET),$(info [CXX] $@))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CXXPICFLAGS) $(CPPFLAGS) $< -c -o $@

%-pic.o: %.cpp	
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXSOMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXSODEPS)
	$(if $(QUIET),$(info [CXX] $@))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CXXPICFLAGS) $(CPPFLAGS) $< -c -o $@

%.dso: %.cpp	
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXSOMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXSODEPS)
	$(if $(QUIET),$(info [CXX] $@))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CXXPICFLAGS) $(CPPFLAGS) $< -c -o $@

%-pic.o: %.C
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXSOMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXSODEPS)
	$(if $(QUIET),$(info [CXX] $@))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CXXPICFLAGS) $(CPPFLAGS) $< -c -o $@

%.dso: %.C
	+$(QUIET)[ -d $(@D) ] || mkdir -p $(@D)
	$(QUIET)$(CXXSOMAKEDEPEND)
	$(QUIET)$(PROCESS_CXXSODEPS)
	$(if $(QUIET),$(info [CXX] $@))
	$(QUIET)$(CXX) $(CXXFLAGS) $(CXXPICFLAGS) $(CPPFLAGS) $< -c -o $@

endif
