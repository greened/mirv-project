ifndef AUTOEXE_MK
AUTOEXE_MK := 1

define make_exe_impl

$(1): LDFLAGS += $(foreach lib,$(3),$(patsubst lib%.a,-l%,$(firstword $(patsubst /, ,$$(lib))))) $(5)

$$(call debug,[exe] $(1): LDFLAGS += $(foreach lib,$(3),$(patsubst lib%.a,-l%,$(firstword $(patsubst /, ,$$(lib))))) $(5)) 

$$(call debug,[exe] $(1): $(2) $(3))

$(1): $(2) $(3)
	$$($(4)) -o $$(@) $$(filter %.o,$$(^)) $$(LDFLAGS)

endef

# $1: The executable name
# $2: The objects making up the executable
# $3: Libraries needed to create the executable
# $4: The linker
# $5: Additional linker flags
make_exe = $(eval $(call make_exe_impl,$(1),$(2),$(3),$(4),$(5)))

# $1: The object making up the executable
# $2: Libraries needed to create the executable
# $3: The linker
# $4: Additional linker flags
make_simple_exe = $(call make_exe,$(patsubst %.o,%.exe,$(1)),$(1),$(2),$(3),$(4))

endif
