ifndef COMPILE_MK
COMPILE_MK := 1

include $(BUILDTOOLS)/configure/try.mk
include $(BUILDTOOLS)/configure/define.mk
include $(BUILDTOOLS)/configure/bind.mk
include $(BUILDTOOLS)/configure/ls.mk

nullstring :=
space := $(nullstring) # end of the line

define newline
$(nullstring)

endef

define mc_compile_impl

$$(call debug,[mc_compile_impl] mc_compile_impl("$(1)" "$(2)" "$(3)" "$(4)" "$(5)" "$(6)"))

$(1)_OSRCFILE := $(FINAL_BUILDDIR)/configure/$(1)_compile_test.$(4)
$(1)_OFILE := $$(patsubst %.$(4),%.o,$$($(1)_OSRCFILE))

$(1)_compile_source := $$(subst $$(newline), ,$$(subst $$(space),_,$$($(3))))

$$(call debug,[mc_compile_impl] $(1)_compile_source = $$($(1)_compile_source))
$$(call debug,[mc_compile_impl] $(1)_OSRCFILE = $$($(1)_OSRCFILE))
$$(call debug,[mc_compile_impl] $(1)_OFILE    = $$($(1)_OFILE))

.PRECIOUS: $$($(1)_OSRCFILE)
$$($(1)_OSRCFILE):
	$(QUIET)cat /dev/null > $$@
	$(QUIET)$$(foreach line,$$($(1)_compile_source),echo "$$(subst _, ,$$(line))" >> $$@; )

$$(call debug,[mc_compile_impl] $$($(1)_OFILE): $$($(1)_OSRCFILE) $(6))

include $(6)

.PRECIOUS: $$($(1)_OFILE)
.SECONDEXPANSION:
$$($(1)_OFILE): $$($(1)_OSRCFILE) $(6)
	$$(if $(6),$$(eval include $(6)))
	-$(QUIET)$$($(2)) $(5) -c $$< -o $$@

$(1)_compile_target = $(FINAL_BUILDDIR)/configure/$(1)_compile_result.mk

$$(call debug,[mc_compile_impl] $$($(1)_compile_target): $$($(1)_OFILE))

$$($(1)_compile_target): $$($(1)_OFILE)
	$(QUIET)if [ -e $$(<) ]; then \
	          $$(call mc_define_append,$(1)_compile,yes,$$@); \
	        else \
	          $$(call mc_define_append,$(1)_compile,,$$@); \
	        fi

CONFIGURE_INCLUDES += $$($(1)_compile_target)

endef

# $1: A unique name
# $2: The name of the variable holding the compiler location
# $3: The source code to compile
# $4: The source code file extension
# $5: Extra compiler flags
# $6: Dependency
# Set $(1)_compile to yes or empty
mc_compile = $(eval $(call mc_compile_impl,$(1),$(2),$(3),$(4),$(5),$(6)))

# $1: A unique name
# $2: The source code to compile
# $3: Extra compiler flags
# $4: Dependence
# Set CC_compile to yes or empty
mc_compile_c = $(call mc_compile,$(1),CC,$(2),c,$(3),$(4))

# $1: A unique name
# $2: The source code to compile
# $3: Extra compiler flags
# $4: Dependence
# Set CXX_compile to yes or empty
mc_compile_cxx = $(call mc_compile,$(1),CXX,$(2),cc,$(3),$(4))

# $1: A unique name
# $2: The name of the variable holding the compiler location
# $3: The source code to compile
# $4: The source code file extension
# $5: Extra compiler flags
# $6: True action
# $7: False action
# $8: Dependence
mc_try_compile = $(call debug,[mc_try_compile] mc_try_compile("$(1)" "$(2)" "$(3)" "$(4)" "$(5)" "$(6)" "$(7)" "$(8)"))$(call mc_try,$(1)_compile,$(call bind6,$(1)_compile_bind,mc_compile,$(1),$(2),$(3),$(4),$(5),$(8)),$(6),$(7),$(FINAL_BUILDDIR)/configure/$(1)_compile_result.mk)

# $1: A unique tag
# $2: The source code to compile
# $3: Extra compiler flags
# $4: True action
# $5: False action
# $6: Dependence
mc_try_compile_c = $(call mc_try_compile,$(1),CC,$(2),c,$(3),$(4),$(5),$(6))

# $1: A unique name
# $2: The source code to compile
# $3: Extra compiler flags
# $4: True action
# $5: False action
# $6: Dependence
mc_try_compile_cxx = $(call mc_try_compile,$(1),CXX,$(2),cc,$(3),$(4),$(5),$(6))

endif
