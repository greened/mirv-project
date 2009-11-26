ifndef LINK_MK
LINK_MK := 1

include $(BUILDTOOLS)/configure/try.mk
include $(BUILDTOOLS)/configure/define.mk
include $(BUILDTOOLS)/configure/bind.mk
include $(BUILDTOOLS)/configure/ls.mk

define mc_link_impl

$$(call debug,[mc_link_impl] mc_link_impl("$(1)" "$(2)" "$(3)" "$(4)" "$(5)"))

$(1)_LDOBJFILE := $(2)
$(1)_LDFILE := $$(patsubst %.o,%.exe,$$($(1)_LDOBJFILE))

$$(call debug,[mc_link_impl] $(1)_LDOBJFILE = $$($(1)_LDOBJFILE))
$$(call debug,[mc_link_impl] $(1)_LDFILE    = $$($(1)_LDFILE))

$$(call debug,[mc_link_impl] $$($(1)_LDFILE): $$($(1)_LDOBJFILE) $(5))

$$(if $(5),include $(5))

.PRECIOUS: $$($(1)_LDFILE)
.SECONDEXPANSION:
$$($(1)_LDFILE): $$($(1)_LDOBJFILE) $(5)
	$$(if $(5),$$(eval include $(5)))
	-$(QUIET)$$($(1)) $$< $(3) -o $$@

$$(call debug,[mc_link_impl] $(4): $$($(1)_LDFILE))

$(4): $$($(1)_LDFILE)
	$(QUIET)$$(if $$(wildcard $$(<),$$(call mc_define,$(1)_compile,yes,$$@),$$(call mc_define,$(1)_compile,,$$@))

endef

CONFIGURE_INCLUDES += $(4)

# $1: The name of the variable holding the linker location
# $2: The object file to link
# $3: Additional linker flags (libraries, etc.)
# $4: Output file to create
# $5: Dependency
# Set $(1)_link to yes or empty
mc_link = $(eval $(call mc_link_impl,$(1),$(2),$(3),$(4),$(5)))

# $1: The object file to link
# $2: Output file to create
# $3: Dependence
# Set CC_link to yes or empty
mc_link_c = $(call mc_link,CC,$(1),,$(2),$(3))

# $1: The object file to link
# $2: Output file to create
# $3: Dependence
# Set CXX_link to yes or empty
mc_link_cxx = $(call mc_link,CXX,$(1),,$(2),$(3))

# $1: The name of the variable holding the linker location
# $2: The object file to link
# $3: True action
# $4: False action
# $5: Include file to create (actions must create it)
# $6: Dependence
mc_try_link = $(call mc_try,$(1)_link,$(call bind5,$(1)_link_bind,mc_link,$(1),$(2),,$(patsubst %.mk,%_link.mk,$(5)),$(6)),$(3),$(4),$(patsubst %.mk,%_link.mk,$(5)))

# $1: The object file to link
# $2: True action
# $3: False action
# $4: Include file to create (actions must create it)
# $5: Dependence
mc_try_link_c = $(call mc_try,CC_link,$(call bind3,CC_link_bind,mc_link_c,$(1),$(patsubst %.mk,%_link.mk,$(4)),$(5)),$(2),$(3),$(4),$(patsubst %.mk,%_link.mk,$(4)))

# $1: The object file to link
# $2: True action
# $3: False action
# $4: Include file to create (actions must create it)
# $5: Dependence
mc_try_link_cxx = $(call mc_try,CXX_link,$(call bind3,CXX_link_bind,mc_link_cxx,$(1),$(patsubst %.mk,%_link.mk,$(4)),$(5)),$(2),$(3),$(4),$(patsubst %.mk,%_link.mk,$(4)))

endif
