ifndef EXECUTE_MK
EXECUTE_MK := 1

include $(BUILDTOOLS)/configure/try.mk
include $(BUILDTOOLS)/configure/define.mk

define mc_execute_impl

$$(call debug,call mc_execute $(1):$(2):$(3):$(4))

$$(call debug,[mc_execute] $(3): $(4))

include $(4)

$(3): $(4)
	$$(if $(4),$$(eval include $(4)))
	$$(call debug,CXX = $$(CXX))
	$(QUIET)$$(call mc_define,$(1),$$(shell $$($(2))),$$@,$(3))

CONFIGURE_INCLUDES += $(3)

endef

# $1: Variable to set with command output
# $2: Command to execute
# $3: File to create
# $4: Dependency
mc_execute = $(eval $(call mc_execute_impl,$(1),$(2),$(3),$(4)))

# $1: Variable to set with execute result
# $2: Command to execute
# $3: True action
# $4: False action
# $5: Include file to create (actions must create it)
# $6: Dependency
mc_try_execute = $(call mc_try,$(1),$(call bind4,$(1)_try_execute_bind,mc_execute,$(1),$(2),$(patsubst %.mk,%_execute.mk,$(5)),$(6)),$(3),$(4),$(5),$(patsubst %.mk,%_execute.mk,$(5)))

endif
