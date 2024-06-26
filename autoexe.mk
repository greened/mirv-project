ifndef AUTOEXE_MK
AUTOEXE_MK := 1

define make_exe_impl

$$(call debug,[exe] '$(1)' '$(2)' '$(3)' '$(4)' '$(5)' '$(6)')
$$(call debug,[exe] LIBS = $(3))
$$(call debug,[exe] LIBS subst = $(foreach lib,$(3),$(subst /, ,$(lib))))
$$(call debug,[exe] LIBS last = $(foreach lib,$(3),$(lastword $(subst /, ,$(3)))))
$$(call debug,[exe] LIBS patsubst = $(foreach lib,$(3),$(patsubst lib%.a,-l%,$(lastword $(subst /, ,$(lib))))))
$$(call debug,[exe] LIBPATH = $(foreach lib,$(3),-L$(subst /$(lastword $(subst /, ,$(lib))),,$(lib))))
$$(call debug,[exe] LDFLAGS = $(foreach lib,$(3),$(patsubst lib%.a,-l%,$(lastword $(subst /, ,$(lib))))) $(foreach lib,$(3),-L$(subst /$(lastword $(subst /, ,$(lib))),,$(lib))) $(5))

$(1): LDFLAGS += $(foreach lib,$(3),-L$(subst /$(lastword $(subst /, ,$(lib))),,$(lib))) $(foreach lib,$(3),$(patsubst lib%.a,-l%,$(lastword $(subst /, ,$(lib))))) $(5)

$$(call debug,[exe] $(1): LDFLAGS += $(foreach lib,$(3),$(patsubst lib%.a,-l%,$(lastword $(subst /, ,$(lib))))) $(foreach lib,$(3),-L$(subst /$(lastword $(subst /, ,$(lib))),,$(lib))) $(5))

$$(call debug,[exe] $(1): $(2) $(3) $(6))

ifneq ($(4),)
$(1): LXX = $$($(4))
$(1): LCC = $$($(4))
endif

$(1): $(2) $(3) $(6)
	@$(if $(QUIET),echo [LDX] $$@)
	$(QUIET)$$(LXX) -o $$(@) $$(filter %.o,$$(^)) $$(LDFLAGS)

endef

# $1: The executable name
# $2: The objects making up the executable
# $3: Libraries needed to create the executable
# $4: The linker
# $5: Additional linker flags
# $6: Dependency
make_exe = $(eval $(call make_exe_impl,$(1),$(2),$(3),$(4),$(5),$(6)))

# $1: The object making up the executable
# $2: Libraries needed to create the executable
# $3: The linker
# $4: Additional linker flags
# $5: Dependency
make_simple_exe = $(call make_exe,$(patsubst %.o,%.exe,$(1)),$(1),$(2),$(3),$(4),$(5))

endif
