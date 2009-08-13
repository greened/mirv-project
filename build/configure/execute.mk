ifndef EXECUTE_MK
EXECUTE_MK := 1

include $(BUILDTOOLS)/configure/try.mk
include $(BUILDTOOLS)/configure/define.mk

define mc_execute_impl

$(FINAL_BUILDDIR)/configure/$(1)_execute.mk: $(3)
	$$(call mc_define,$(1),$$($(2)),$$@)

CONFIGURE_INCLUDES += $(FINAL_BUILDDIR)/configure/$(1)_execute.mk

endef

# $1: Variable to set with command output
# $2: Command to execute
# $3: Dependency
mc_execute = $(eval $(call mc_execute_impl,$(1),$(call bind1,$(1)_execute_bind,shell,$$($(2))),$(3))) $(FINAL_BUILDDIR)/configure/$(1)_execute.mk

# $1: Variable to set with execute result
# $2: Command to execute
# $3: True action
# $4: False action
# $5: Include file to create (actions must create it)
# $6: Dependency
mc_try_execute = $(call mc_try,$(1),$(call bind3,$(1)_try_execute_bind,mc_execute,$(1),$(2),$(6)),$(3),$(4),$(5))
endif
