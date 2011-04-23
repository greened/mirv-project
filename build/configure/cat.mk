ifndef CAT_MK
CAT_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(CAT),)
  __sink := $(call mc_target_tool,CAT,cat)
else
  ifeq ($(origin CAT),default)
    override CAT :=
    __sink := $(call debug,Before call mc_target_tools) $(call mc_target_tool,CAT,cat) $(call debug,After call mc_target_tools)
  endif
endif

endif
