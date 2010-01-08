ifndef EXECUTE_MK
EXECUTE_MK := 1

include $(BUILDTOOLS)/configure/try.mk
include $(BUILDTOOLS)/configure/define.mk

define mc_execute_impl

$$(call debug,call mc_execute $(1):$(2):$(3))

$(1)_execute_target = $(FINAL_BUILDDIR)/configure/$(1)_execute_result.mk

$$(call debug,[mc_execute] $$($(1)_execute_target): $(3))

include $(3)

$$($(1)_execute_target): $(3)
	$$(if $(3),$$(eval include $(3)))
	$$(call debug,CXX = $$(CXX))
	$(QUIET)$$(call mc_define_create,$(1),$$(shell $$($(2))),$$@,$(3))

CONFIGURE_INCLUDES += $$($(1)_execute_target)

endef

# $1: Variable to set with command output
# $2: Command to execute
# $3: Dependency
mc_execute = $(eval $(call mc_execute_impl,$(1),$(2),$(3)))

# $1: Variable to set with execute result
# $2: Command to execute
# $3: True action
# $4: False action
# $5: Dependency
mc_try_execute = $(call mc_try,$(1),$(call bind4,$(1)_try_execute_bind,mc_execute,$(1),$(2),$(5)),$(3),$(4),$(FINAL_BUILDDIR)/configure/$(1)_execute_result.mk)
endif
