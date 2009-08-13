ifndef DEFINE_MK
DEFINE_MK := 1

include $(BUILDTOOLS)/configure/configure.mk

# $1: Variable name to define
# $2: Value to define
# $3: Target file
# $4: Append if set
mc_define = $(call mc_info,$(1) = $(2))$(eval override $(1) := $(2))$(if $(4),echo "override $(1) := $(2)" >> $(3),echo "override $(1) := $(2)" > $(3))

endif