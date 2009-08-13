ifndef CP_MK
CP_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(CP),)
  __sink := $(call mc_tool,CP,cp)
else
  ifeq ($(origin CP),default)
    override CP :=
    __sink := $(call mc_tool,CP,cp)
  endif
endif

endif
