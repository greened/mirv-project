ifndef AR_MK
AR_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(AR),)
  __sink := $(call mc_target_tool,AR,ar)
else
  ifeq ($(origin AR),default)
    override AR :=
    __sink := $(call debug,Before call mc_target_tools) $(call mc_target_tool,AR,ar) $(call debug,After call mc_target_tools)
  endif
endif

endif
