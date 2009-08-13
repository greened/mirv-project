ifndef SED_MK
SED_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(SED),)
  __sink := $(call mc_tool,SED,sed)
else
  ifeq ($(origin SED),default)
    override SED :=
    __sink := $(call mc_tool,SED,sed)
  endif
endif

endif
