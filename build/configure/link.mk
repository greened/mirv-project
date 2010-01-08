ifndef LINK_MK
LINK_MK := 1

include $(BUILDTOOLS)/configure/try.mk
include $(BUILDTOOLS)/configure/define.mk
include $(BUILDTOOLS)/configure/bind.mk
include $(BUILDTOOLS)/configure/ls.mk

define mc_link_impl

$$(call debug,[mc_link_impl] mc_link_impl("$(1)" "$(2)" "$(3)" "$(4)" "$(5)"))

$(1)_LDOBJFILE := $(3)
$(1)_LDFILE := $$(patsubst %.o,%.exe,$$($(1)_LDOBJFILE))

$$(call debug,[mc_link_impl] $(1)_LDOBJFILE = $$($(1)_LDOBJFILE))
$$(call debug,[mc_link_impl] $(1)_LDFILE    = $$($(1)_LDFILE))

$$(call debug,[mc_link_impl] $$($(1)_LDFILE): $$($(1)_LDOBJFILE) $(5))

include $(5)

.PRECIOUS: $$($(1)_LDFILE)
.SECONDEXPANSION:
$$($(1)_LDFILE): $$($(1)_LDOBJFILE) $(5)
	$$(if $(5),$$(eval include $(5)))
	-$(QUIET)$$($(2)) $(4) $$< -o $$@

$(1)_link_target = $(FINAL_BUILDDIR)/configure/$(1)_link_result.mk

$$(call debug,[mc_link_impl] $$($(1)_link_target): $$($(1)_LDFILE))

$$($(1)_link_target): $$($(1)_LDFILE)
	$(QUIET)if [ -x $$(<) ]; then \
	          $$(call mc_define_append,$(1)_link,yes,$$@); \
	        else \
	          $$(call mc_define_append,$(1)_link,,$$@); \
	        fi

CONFIGURE_INCLUDES += $$($(1)_link_target)

endef

# $1: A unique name
# $2: The name of the variable holding the linker location
# $3: The object file to link
# $4: Additional linker flags (libraries, etc.)
# $5: Dependency
# Set $(1)_link to yes or empty
mc_link = $(eval $(call mc_link_impl,$(1),$(2),$(3),$(4),$(5)))

# $1: A unique name
# $2: The object file to link
# $3: Additional linker flags (libraries, etc.)
# $4: Dependence
# Set CC_link to yes or empty
mc_link_c = $(call mc_link,$(1),CC,$(2),$(3),$(4))

# $1: A unique name
# $2: The object file to link
# $3: Additional linker flags (libraries, etc.)
# $4: Dependence
# Set CXX_link to yes or empty
mc_link_cxx = $(call mc_link,$(1),CXX,$(2),$(3),$(4))

# $1: A unique name
# $2: The name of the variable holding the linker location
# $3: The object file to link
# $4: Additional linker flags (libraries, etc.)
# $5: True action
# $6: False action
# $7: Dependence
mc_try_link = $(call mc_try,$(1)_link,$(call bind5,$(1)_link_bind,mc_link,$(1),$(2),$(3),$(4),$(7)),$(5),$(6),$(FINAL_BUILDDIR)/configure/$(1)_link_result.mk)

# $1: A unique name
# $2: The object file to link
# $3: Additional linker flags (libraries, etc.)
# $4: True action
# $5: False action
# $6: Dependence
mc_try_link_c = $(call mc_try_link,$(1),CC,$(2),$(3),$(4),$(5),$(6))

# $1: A unique name
# $2: The object file to link
# $3: Additional linker flags (libraries, etc.)
# $4: True action
# $5: False action
# $6: Dependence
mc_try_link_cxx = $(call mc_try_link,$(1),CXX,$(2),$(3),$(4),$(5),$(6))

endif
