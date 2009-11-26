ifndef DEFINE_MK
DEFINE_MK := 1

include $(BUILDTOOLS)/configure/configure.mk
include $(BUILDTOOLS)/configure/create.mk

define mc_define_impl

$(3)_define.mk: $(4)
	$$(call debug,[mc_define] $(1) = $(2))
	$(QUIET)$$(if $(4),echo "include $(4)" > $$(@); echo "override $(1) := $(2)" >> $$(@),echo "override $(1) := $(2)" > $$(@))

CONFIGURE_INCLUDES += $(3)_define.mk

endef

# $1: Variable name to define
# $2: Value to define
# $3: Target file
# $4: Dependence
#mc_define = $(eval $(call mc_define_impl,$(1),$(2),$(3),$(4)))
mc_define = $(call mc_create_start,$(3),$(4)); echo "\$$(call mc_info,$(1) := $(2))" >> $(3); $(if $(4),echo "include $(4)" >> $(3);) echo "override $(1) := $(2)" >> $(3); $(call mc_create_end,$(3))

# $1: Variable name to define
# $2: Value to define
# $3: Target file
#mc_define_immediate = $(call mc_define,$(1),$(2),$(3))$(eval $(1) = $(2))

endif
