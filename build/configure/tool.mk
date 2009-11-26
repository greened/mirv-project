ifndef TOOL_MK
TOOL_MK := 1

include $(BUILDTOOLS)/configure/findfile.mk
include $(BUILDTOOLS)/configure/target.mk
include $(BUILDTOOLS)/configure/build_host.mk
include $(BUILDTOOLS)/configure/define.mk

define mc_tool_impl

# Generate the search rules
$(1)_MC_FIND_INCLUDE := $$(strip $$(call mc_find,$(2)))

$(FINAL_BUILDDIR)/configure/$(1).mk: $$($(1)_MC_FIND_INCLUDE)
	+$(QUIET)[ -d $$(@D) ] || mkdir -p $$(@D)
	$(QUIET)$$(call mc_define,$(1),$$(strip $$(firstword $(foreach var,$(2),$$($(var))))),$$@)
	$(QUIET)echo '$$$$(if $$$$($(1)),,$$$$(error $$$$(if $(3),$(3),Could not find definition for $(1))))' >> $$@

CONFIGURE_INCLUDES += $(FINAL_BUILDDIR)/configure/$(1).mk

endef

# $1: The name of the variable to set
# $2: The list of tools to search for
# $3: The message to output on error
mc_tool = $(eval $(call mc_tool_impl,$(1),$(2),$(3)))

# $1: The name of the variable to set
# $2: The list of tools to search for
# $3: The message to output on error
mc_target_tool = $(call mc_tool,$(1),$(if $(call ne,$(TARGET),$(BUILD_HOST)),$(subst %,$(TARGET)-% %,$(2)),$(2)),$(3))

endif
