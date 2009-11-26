ifndef LN_MK
LN_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(LN),)
  __sink := $(call mc_tool,LN,ln)
else
  ifeq ($(origin LN),default)
    override LN :=
    __sink := $(call mc_tool,LN,ln)
  endif
endif

endif
