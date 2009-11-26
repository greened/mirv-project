ifndef LS_MK
LS_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(LS),)
  __sink := $(call mc_tool,LS,ls)
else
  ifeq ($(origin LS),default)
    override LS :=
    __sink := $(call mc_tool,LS,ls)
  endif
endif

endif
