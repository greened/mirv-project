ifndef COMPILE_MK
COMPILE_MK := 1

include $(BUILDTOOLS)/configure/try.mk
include $(BUILDTOOLS)/configure/define.mk
include $(BUILDTOOLS)/configure/bind.mk

define mc_compile_text

#$$(call debug,mc_compile_impl("$(1)" "$(2)" "$(3)"))

$(1)_OSRCFILE := $(FINAL_BUILDDIR)/configure/$(1)_compile_test.$(3)
$(1)_OFILE := $$(patsubst %.$(3),%.o,$$($(1)_OSRCFILE))

$(1)_compile_source := $$($(2))

#$$(call debug,$(1)_OSRCFILE = $$($(1)_OSRCFILE))
#$$(call debug,$(1)_OFILE    = $$($(1)_OFILE))

.PRECIOUS: $$($(1)_OSRCFILE)
$$($(1)_OSRCFILE):
	$(QUIET)echo "$$($(1)_compile_source)" > $$@

#$$(call debug,$$($(1)_OFILE): $$($(1)_OSRCFILE) $(FINAL_BUILDDIR)/configure/$$(firstword $(1)).mk)

.PRECIOUS: $$($(1)_OFILE)
.SECONDEXPANSION:
$$($(1)_OFILE): $$($(1)_OSRCFILE) $(4)
	-$(QUIET)$$($(1)) -c $$< -o $$@

#$$(call debug,$(FINAL_BUILDDIR)/configure/$(1)_compile.mk: $$($(1)_OFILE))

$(FINAL_BUILDDIR)/configure/$(1)_compile_target: $$($(1)_OFILE)
	$(QUIET)$$(if $$(wildcard $$($(1)_OFILE)),$$(call mc_define,$(1)_compile,yes,$$@),$$(call mc_define,$(1)_compile,,$$@))

endef

define mc_compile_impl

$$(eval $$(call mc_compile_text,$(1),$(2),$(3),$(4)))

.SECONDEXPANSION:
$(FINAL_BUILDDIR)/configure/$(1)_compile.mk: $(FINAL_BUILDDIR)/configure/$(1)_compile_target
	$(QUIET)echo 'include $(FINAL_BUILDDIR)/configure/$(1)_compile_target' > $$@

CONFIGURE_INCLUDES += $(FINAL_BUILDDIR)/configure/$(1)_compile.mk
CONFIGURE_TARGETS += $(FINAL_BUILDDIR)/configure/$(1)_compile_target

endef

# $1: The name of the variable holding the compiler location
# $2: The source code to compile
# $3: The source code file extension
# $4: Dependency
# Set $(1)_compile to yes or empty
mc_compile = $(eval $(call mc_compile_impl,$(1),$(2),$(3),$(4))) $(FINAL_BUILDDIR)/configure/$(1)_compile.mk 


# $1: The source code to compile
# Set CC_compile to yes or empty
mc_compile_c = $(call mc_compile,CC,$(1),c,$(FINAL_BUILDDIR)/configure/CC.mk)

# $1: The source code to compile
# Set CXX_compile to yes or empty
mc_compile_cxx = $(call mc_compile,CXX,$(1),cc,$(FINAL_BUILDDIR)/configure/CXX.mk)

# $1: The name of the variable holding the compiler location
# $2: The source code to compile
# $3: True action
# $4: False action
# $5: The source code file extension
# $6: Include file to create (actions must create it)
mc_try_compile = $(call mc_try,$(1)_compile,$(call bind3,$(1)_compile_bind,mc_compile,$(1),$(2),$(5)),$(3),$(4),$(6))

# $1: The source code to compile
# $2: True action
# $3: False action
# $4: Include file to create (actions must create it)
mc_try_compile_c = $(call mc_try,CC_compile,$(call bind1,CC_compile_bind,mc_compile_c,$(1)),$(2),$(3),$(4))

# $1: The source code to compile
# $2: True action
# $3: False action
# $4: Include file to create (actions must create it)
mc_try_compile_cxx = $(call mc_try,CXX_compile,$(call bind1,CXX_compile_bind,mc_compile_cxx,$(1)),$(2),$(3),$(4))

endif
