ifndef CMP_MK
CMP_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(CMP),)
  __sink := $(call mc_target_tool,CMP,cmp)
else
  ifeq ($(origin CMP),default)
    override CMP :=
    __sink := $(call debug,Before call mc_target_tools) $(call mc_target_tool,CMP,cmp) $(call debug,After call mc_target_tools)
  endif
endif

endif
