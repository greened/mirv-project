ifndef MV_MK
MV_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(MV),)
  __sink := $(call mc_tool,MV,mv)
else
  ifeq ($(origin MV),default)
    override MV :=
    __sink := $(call mc_tool,MV,mv)
  endif
endif

endif
