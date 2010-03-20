ifndef AUTODOX_MK
AUTODOX_MK := 1

include $(BUILDTOOLS)/autoin.mk
include $(BUILDTOOLS)/configure/doxygen.mk
include $(BUILDTOOLS)/configure/touch.mk

define doxygen_rule

$(FINAL_BUILDDIR)/stamp-$(1)-doxygen: $(2)	
	+$$(QUIET)[ -d $$(@D) ] || mkdir -p $$(@D)		
	$$(if $$(QUIET),$$(info [DOX] $$@))
	$$(QUIET)$$(DOXYGEN) $(2)
	$$(QUIET)$$(TOUCH) $$@

$(1)_DOXY_STAMPS += $(FINAL_BUILDDIR)/stamp-$(1)-doxygen

$$(call debug,[dox] $(1)_DOXY_STAMPS = $$($(1)_DOXY_STAMPS))

endef

define make_doxyconfig_impl

# Create the Doxygen config file
$$(call make_in,$(1),$(2),@BUILDDIR@:$$(FINAL_BUILDDIR) @SRCDIR@:$$(SRCDIR),dox)

$(1)_DOXY_CONFIGS := $$(patsubst %.dox.in,%.dox,$$($(1)_INS))

$$(call debug,[dox] $(1)_DOXY_CONFIGS = $$($(1)_DOXY_CONFIGS))

$$(foreach config,$$($(1)_DOXY_CONFIGS),$$(eval $$(call doxygen_rule,$(1),$$(config))))

endef

# $1: A unique identifier
# $2: The directory containing the Doxygen input config file
# $3: Dependency
make_doxyconfig = $(eval $(call make_doxyconfig_impl,$(1),$(2),$(3)))

endif
