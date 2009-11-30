ifndef PREPROCESS_MK
PREPROCESS_MK := 1

include $(BUILDTOOLS)/configure/try.mk
include $(BUILDTOOLS)/configure/define.mk
include $(BUILDTOOLS)/configure/bind.mk

define mc_preprocess_text

$(1)_PPSRCFILE := $(FINAL_BUILDDIR)/configure/$(1)_preprocess_test.$(3)
$(1)_PPFILE := $$(patsubst %.$(3),%.i,$$($(1)_PPSRCFILE))

$(1)_preprocess_source := $$($(2))

.PRECIOUS: $$($(1)_PPSRCFILE)
$$($(1)_PPSRCFILE):
	$(QUIET)echo "$$($(1)_preprocess_source)" > $$@

.PRECIOUS: $$($(1)_PPFILE)
.SECONDEXPANSION:
$$($(1)_PPFILE): $$($(1)_PPSRCFILE) $(4)
	$$(info $(1) = $$($(1)))
	-$(QUIET)$$($(1)) -E $$< -o $$@

$(FINAL_BUILDDIR)/configure/$(1)_preprocess_target: $$($(1)_PPFILE)
	echo wildcard $$< = $$(wildcard $$<)
	$(QUIET)$$(if $$(wildcard $$($(1)_PPFILE)),$$(call mc_define_append,$(1)_preprocess,yes,$$@),$$(call mc_define,$(1)_preprocess,,$$@))

endef

define mc_preprocess_impl

$$(eval $$(call mc_preprocess_text,$(1),$(2),$(3),$(4)))

.SECONDEXPANSION:
$(FINAL_BUILDDIR)/configure/$(1)_preprocess.mk: $(FINAL_BUILDDIR)/configure/$(1)_preprocess_target
	$(QUIET)echo 'include $(FINAL_BUILDDIR)/configure/$(1)_preprocess_target' > $$@

CONFIGURE_INCLUDES += $(FINAL_BUILDDIR)/configure/$(1)_preprocess.mk
CONFIGURE_TARGETS += $(FINAL_BUILDDIR)/configure/$(1)_preprocess_target

endef

# $1: The name of the variable holding the compiler location
# $2: The source code to preprocess
# $3: The source code file extension
# $4: Dependency
# Set $(1)_preprocess to yes or empty
mc_preprocess = $(FINAL_BUILDDIR)/configure/$(1)_preprocess.mk $(eval $(call mc_preprocess_impl,$(1),$(2),$(3),$(4)))


# $1: The source code to preprocess
# Set CC_preprocess to yes or empty
mc_preprocess_c = $(call mc_preprocess,CC,$(1),c,$(FINAL_BUILDDIR)/configure/CC.mk)

# $1: The source code to preprocess
# Set CXX_preprocess to yes or empty
mc_preprocess_cxx = $(call mc_preprocess,CXX,$(1),cc,$(FINAL_BUILDDIR)/configure/CXX.mk)

# $1: The name of the variable holding the compiler location
# $2: The source code to preprocess
# $3: True action
# $4: False action
# $5: Include file to create (actions must create it)
mc_try_preprocess = $(call mc_try,$(1)_preprocess,$(call bind2,$(1)_preprocess_bind,mc_preprocess,$(1),$(2)),$(3),$(4),$(5))

# $1: The source code to preprocess
# $2: True action
# $3: False action
# $4: Include file to create (actions must create it)
mc_try_preprocess_c = $(call mc_try,CC_preprocess,$(call bind1,CC_preprocess_bind,mc_preprocess_c,$(1)),$(2),$(3),$(4))

# $1: The source code to preprocess
# $2: True action
# $3: False action
# $4: Include file to create (actions must create it)
mc_try_preprocess_cxx = $(call mc_try,CXX_preprocess,$(call bind1,CXX_preprocess_bind,mc_preprocess_cxx,$(1)),$(2),$(3),$(4))

endif
