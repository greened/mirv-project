ifndef SORT_MK
SORT_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(SORT),)
  __sink := $(call mc_tool,SORT,sort)
else
  ifeq ($(origin SORT),default)
    override SORT :=
    __sink := $(call mc_tool,SORT,sort)
  endif
endif

endif
