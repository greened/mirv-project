ifndef UNIQ_MK
UNIQ_MK := 1

include $(BUILDTOOLS)/configure/tool.mk

ifeq ($(UNIQ),)
  $(call mc_tool,UNIQ,uniq)
else
  ifeq ($(origin UNIQ),default)
    override UNIQ :=
    $(call mc_tool,UNIQ,uniq)
  endif
endif

endif
