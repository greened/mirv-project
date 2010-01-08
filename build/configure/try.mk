ifndef TRY_MK
TRY_MK := 1

define mc_try_impl

$$(call debug,[mc_try] mc_try_impl("$(1)" [$(1)] "$(2)" [$$(2)] "$(3)" [$$(3)] "$(4)" [$$(4)] "$(5)" [$$(5)]))

$(1)_TRY_TEST = $(2)
$(1)_TRY_TRUE_ACTION = $(3)
$(1)_TRY_FALSE_ACTION = $(4)

$$(call debug,[mc_try] $(1)_TRY_TEST = $$($(1)_TRY_TEST))
$$(call debug,[mc_try] $(1)_TRY_TRUE_ACTION = $$($(1)_TRY_TRUE_ACTION))
$$(call debug,[mc_try] $(1)_TRY_FALSE_ACTION = $$($(1)_TRY_FALSE_ACTION))

# Generate the rules for the test
$$($(2))

$(1)_try_target = $(1)_try_result.mk

$$(call debug,[mc_try] $$($(1)_try_target): $(5))

include $(5)

# Set the variable value
$$($(1)_try_target): $(5)
	$$(if $(5),$$(eval include $(5)))
	$$(call debug,[mc_try] $$(if $$($(1)),$$($(3)),$$($(4))))
	$(QUIET)$$(call mc_create_start,$$@,$(5))
	$(QUIET)$$(if $$($(1)),$$($(3)),$$($(4)))
	$(QUIET)$$(call mc_create_end,$$@)

CONFIGURE_INCLUDES += $$($(1)_try_target)
endef

# $1: Variable to test
# $2: Test to run
# $3: True action
# $4: False action
# $5: Dependency
mc_try = $(eval $(call mc_try_impl,$(1),$(2),$(3),$(4),$(5)))

endif
