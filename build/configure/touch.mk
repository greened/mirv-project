ifndef TOUCH_MK
TOUCH_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(TOUCH),)
  __sink := $(call mc_tool,TOUCH,touch)
else
  ifeq ($(origin TOUCH),default)
    override TOUCH :=
    __sink := $(call mc_tool,TOUCH,touch)
  endif
endif

endif
