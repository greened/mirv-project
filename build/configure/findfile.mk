ifndef FINDFILE_MK
FINDFILE_MK := 1

include $(BUILDTOOLS)/configure/fold.mk
include $(BUILDTOOLS)/configure/define.mk

findfile = $(strip $(foreach dir,$(subst :, ,$(PATH)),$(wildcard $(dir)/$(1)) ))

define mc_find_op_impl

override $(1) := $$(call findfile,$(1))

$(FINAL_BUILDDIR)/configure/$(1).mk:
	+$(QUIET)[ -d $$(@D) ] || mkdir -p $$(@D)
	$(QUIET)$$(call mc_define,$(1),$$($(1)),$$@,append)

CONFIGURE_INCLUDES += $(FINAL_BUILDDIR)/configure/$(1).mk

endef

mc_find_op = $(eval $(call mc_find_op_impl,$(1))) $(FINAL_BUILDDIR)/configure/$(1).mk

mc_find = $(foreach file,$(1),$(call mc_find_op,$(file)))

endif
