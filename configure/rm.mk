ifndef RM_MK
RM_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(RM),)
  __sink := $(call mc_tool,RM,rm))
else
  ifeq ($(origin RM),default)
    override RM :=
    __sink := $(call mc_tool,RM,rm))
  endif
endif

endif
