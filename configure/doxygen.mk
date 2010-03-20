ifndef DOXYGEN_MK
DOXYGEN_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(DOXYGEN),)
  __sink := $(call mc_tool,DOXYGEN,doxygen)
else
  ifeq ($(origin DOXYGEN),default)
    override DOXYGEN :=
    __sink := $(call mc_tool,DOXYGEN,doxygen)
  endif
endif

endif
