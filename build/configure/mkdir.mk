ifndef MKDIR_MK
MKDIR_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(MKDIR),)
  __sink := $(call mc_tool,MKDIR,mkdir)
else
  ifeq ($(origin MKDIR),default)
    override MKDIR :=
    __sink := $(call mc_tool,MKDIR,mkdir)
  endif
endif

endif
