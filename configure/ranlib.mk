ifndef RANLIB_MK
RANLIB_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(RANLIB),)
  __sink := $(call mc_target_tool,RANLIB,ranlib)
else
  ifeq ($(origin RANLIB),default)
    override RANLIB :=
    __sink := $(call mc_target_tool,RANLIB,ranlib)
  endif
endif

endif
