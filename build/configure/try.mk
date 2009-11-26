ifndef TRY_MK
TRY_MK := 1

define mc_try_impl

$$(call debug,[mc_try] mc_try_impl("$(1)" [$(1)] "$(2)" [$$(2)] "$(3)" [$$(3)] "$(4)" [$$(4)] "$(5)" [$$(5)] "$(6)" [$$(6)]))

$(1)_TRY_TEST = $$(2)
$(1)_TRY_TRUE_ACTION = $$(3)
$(1)_TRY_FALSE_ACTION = $$(4)

$$(call debug,[mc_try] $(1)_TRY_TEST = $$($(1)_TRY_TEST))
$$(call debug,[mc_try] $(1)_TRY_TRUE_ACTION = $$($(1)_TRY_TRUE_ACTION))
$$(call debug,[mc_try] $(1)_TRY_FALSE_ACTION = $$($(1)_TRY_FALSE_ACTION))

# Generate the rules for the test
$$($(2))

$$(call debug,[mc_try] $(1)_TRY_TRUE_ACTION = $$($(1)_TRY_TRUE_ACTION))

$$(call debug,[mc_try] $(5): $(6))

# Set the variable value
$(5): $(6)
	$$(if $(6),$$(eval include $(6)))
	$$(call debug,[mc_try] $$(if $$($(1)),$$($(3)),$$($(4))))
	$$(call mc_create_start,$$@,$(6))
	$(QUIET)echo "$$$$(if $$$$($(1)),$$$$($(3)),$$$$($(4)))" >> $$@
	$$(call mc_create_end,$$@)

CONFIGURE_INCLUDES += $(5)
endef

# $1: Variable to test
# $2: Test to run
# $3: True action
# $4: False action
# $5: Include file to create (actions must create it)
# $6: Dependency
mc_try = $(eval $(call mc_try_impl,$(1),$(2),$(3),$(4),$(5),$(6)))

endif
