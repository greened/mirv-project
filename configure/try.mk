ifndef TRY_MK
TRY_MK := 1

define mc_try_impl

#$$(call debug,mc_try_impl("$(1)" "$$(2)" "$$(3)" "$$(4)"))

#$(1)_TRY_TEST = $$(2)
#$(1)_TRY_TRUE_ACTION = $$(3)
#$(1)_TRY_FALSE_ACTION = $$(4)

#$$(call debug,$(1)_TRY_TEST = $$($(1)_TRY_TEST))
#$$(call debug,$(1)_TRY_TRUE_ACTION = $$($(1)_TRY_TRUE_ACTION))
#$$(call debug,$(1)_TRY_FALSE_ACTION = $$($(1)_TRY_FALSE_ACTION))

# Generate the rules for the test
$(1)_try_target := $$($(2))

#$$(call debug,$(1)_TRY_TRUE_ACTION = $$($(1)_TRY_TRUE_ACTION))

#$$(call debug,$(FINAL_BUILDDIR)/configure/$(1).mk: $$($(1)_try_target))

# Set the variable value
$(5): $$($(1)_try_target)
	$$(if $$($(1)),$$($(3)),$$($(4)))

CONFIGURE_INCLUDES += $(5)
endef

# $1: Variable to test
# $2: Test to run
# $3: True action
# $4: False action
# $5: Include file to create (actions must create it)
mc_try = $(eval $(call mc_try_impl,$(1),$(2),$(3),$(4),$(5))) $(5)

endif
