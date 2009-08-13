ifndef LINK_MK
LINK_MK := 1

include $(BUILDTOOLS)/configure/try.mk
include $(BUILDTOOLS)/configure/define.mk
include $(BUILDTOOLS)/configure/bind.mk

define mc_link_text

#$$(call debug,mc_link_impl("$(1)" "$(2)" "$(3)"))

$(1)_LDOBJFILE := $(2)
$(1)_LDFILE := $$(patsubst %.o,%.exe,$$($(1)_LDOBJFILE))

#$$(call debug,$(1)_LDOBJFILE = $$($(1)_LDOBJFILE))
#$$(call debug,$(1)_LDFILE    = $$($(1)_LDFILE))

#$$(call debug,$$($(1)_LDFILE): $$($(1)_LDOBJFILE) $(FINAL_BUILDDIR)/configure/$$(firstword $(1)).mk)

.PRECIOUS: $$($(1)_LDFILE)
.SECONDEXPANSION:
$$($(1)_LDFILE): $$($(1)_LDOBJFILE) $(4)
	-$(QUIET)$$($(1)) $$< $(3) -o $$@

#$$(call debug,$(FINAL_BUILDDIR)/configure/$(1)_link.mk: $$($(1)_LDFILE))
$(FINAL_BUILDDIR)/configure/$(1)_link_target: $$($(1)_LDFILE)
	$(QUIET)$$(if $$(wildcard $$($(1)_LDFILE)),$$(call mc_define,$(1)_link,yes,$$@),$$(call mc_define,$(1)_link,,$$@))

endef

define mc_link_impl

$$(eval $$(call mc_link_text,$(1),$(2),$(3),$(4)))

.SECONDEXPANSION:
$(FINAL_BUILDDIR)/configure/$(1)_link.mk: $(FINAL_BUILDDIR)/configure/$(1)_link_target
	$(QUIET)echo 'include $(FINAL_BUILDDIR)/configure/$(1)_link_target' > $$@

CONFIGURE_INCLUDES += $(FINAL_BUILDDIR)/configure/$(1)_link.mk
CONFIGURE_TARGETS += $(FINAL_BUILDDIR)/configure/$(1)_link_target

endef

# $1: The name of the variable holding the linker location
# $2: The object file to link
# $3: Additional linker flags (libraries, etc.)
# $4: Dependency
# Set $(1)_link to yes or empty
mc_link = $(eval $(call mc_link_impl,$(1),$(2),$(3),$(4))) $(FINAL_BUILDDIR)/configure/$(1)_link.mk 


# $1: The object file to link
# Set CC_link to yes or empty
mc_link_c = $(call mc_link,CC,$(1),,$(FINAL_BUILDDIR)/configure/CC.mk)

# $1: The object file to link
# Set CXX_link to yes or empty
mc_link_cxx = $(call mc_link,CXX,$(1),,$(FINAL_BUILDDIR)/configure/CXX.mk)

# $1: The name of the variable holding the linker location
# $2: The object file to link
# $3: True action
# $4: False action
# $5: Include file to create (actions must create it)
mc_try_link = $(call mc_try,$(1)_link,$(call bind2,$(1)_link_bind,mc_link,$(1),$(2)),$(3),$(4),$(5))

# $1: The object file to link
# $2: True action
# $3: False action
# $4: Include file to create (actions must create it)
mc_try_link_c = $(call mc_try,CC_link,$(call bind1,CC_link_bind,mc_link_c,$(1)),$(2),$(3),$(4))

# $1: The object file to link
# $2: True action
# $3: False action
# $4: Include file to create (actions must create it)
mc_try_link_cxx = $(call mc_try,CXX_link,$(call bind1,CXX_link_bind,mc_link_cxx,$(1)),$(2),$(3),$(4))

endif
