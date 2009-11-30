ifndef COMPILE_MK
COMPILE_MK := 1

include $(BUILDTOOLS)/configure/try.mk
include $(BUILDTOOLS)/configure/define.mk
include $(BUILDTOOLS)/configure/bind.mk
include $(BUILDTOOLS)/configure/ls.mk

define mc_compile_impl

$$(call debug,[mc_compile_impl] mc_compile_impl("$(1)" "$(2)" "$(3)" "$(4)" "$(5)"))

$(1)_OSRCFILE := $(FINAL_BUILDDIR)/configure/$(1)_compile_test.$(3)
$(1)_OFILE := $$(patsubst %.$(3),%.o,$$($(1)_OSRCFILE))

$(1)_compile_source := $$($(2))

$$(call debug,[mc_compile_impl] $(1)_OSRCFILE = $$($(1)_OSRCFILE))
$$(call debug,[mc_compile_impl] $(1)_OFILE    = $$($(1)_OFILE))

.PRECIOUS: $$($(1)_OSRCFILE)
$$($(1)_OSRCFILE):
	$(QUIET)echo "$$($(1)_compile_source)" > $$@

$$(call debug,[mc_compile_impl] $$($(1)_OFILE): $$($(1)_OSRCFILE) $(5))

include $(5)

.PRECIOUS: $$($(1)_OFILE)
.SECONDEXPANSION:
$$($(1)_OFILE): $$($(1)_OSRCFILE) $(5)
	$$(if $(5),$$(eval include $(5)))
	-$(QUIET)$$($(1)) -c $$< -o $$@

$$(call debug,[mc_compile_impl] $(4): $$($(1)_OFILE))

#	$(QUIET)$$(if $$(wildcard $$(<)),$$(call mc_define,$(1)_compile,yes,$$@),$$(call mc_define,$(1)_compile,,$$@))

$(4): $$($(1)_OFILE)
	$(QUIET)if [ -e $$(<) ]; then \
	          $$(call mc_define_append,$(1)_compile,yes,$$@); \
	        else \
	          $$(call mc_define_append,$(1)_compile,,$$@); \
	        fi

CONFIGURE_INCLUDES += $(4)

endef

# $1: The name of the variable holding the compiler location
# $2: The source code to compile
# $3: The source code file extension
# $4: Output file to create
# $5: Dependency
# Set $(1)_compile to yes or empty
mc_compile = $(eval $(call mc_compile_impl,$(1),$(2),$(3),$(4),$(5)))

# $1: The source code to compile
# $2: Output file to create
# $3: Dependence
# Set CC_compile to yes or empty
mc_compile_c = $(call mc_compile,CC,$(1),c,$(2),$(3))

# $1: The source code to compile
# $2: Output file to create
# $3: Dependence
# Set CXX_compile to yes or empty
mc_compile_cxx = $(call mc_compile,CXX,$(1),cc,$(2),$(3))

# $1: The name of the variable holding the compiler location
# $2: The source code to compile
# $3: True action
# $4: False action
# $5: The source code file extension
# $6: Include file to create (actions must create it)
# $7: Dependence
mc_try_compile = $(call mc_try,$(1)_compile,$(call bind5,$(1)_compile_bind,mc_compile,$(1),$(2),$(5),$(patsubst %.mk,%_compile.mk,$(6)),$(7)),$(3),$(4),$(6),$(patsubst %.mk,%_compile.mk,$(6)))

# $1: The source code to compile
# $2: True action
# $3: False action
# $4: Include file to create (actions must create it)
# $5: Dependence
mc_try_compile_c = $(call mc_try,CC_compile,$(call bind3,CC_compile_bind,mc_compile_c,$(1),$(patsubst %.mk,%_compile.mk,$(4)),$(5)),$(2),$(3),$(4),$(patsubst %.mk,%_compile.mk,$(4)))

# $1: The source code to compile
# $2: True action
# $3: False action
# $4: Include file to create (actions must create it)
# $5: Dependence
mc_try_compile_cxx = $(call mc_try,CXX_compile,$(call bind3,CXX_compile_bind,mc_compile_cxx,$(1),$(patsubst %.mk,%_compile.mk,$(4)),$(5)),$(2),$(3),$(4),$(patsubst %.mk,%_compile.mk,$(4)))

endif
