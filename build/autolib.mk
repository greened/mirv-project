ifndef AUTOLIB_MK
AUTOLIB_MK := 1

include $(BUILDTOOLS)/autoobj.mk
include $(BUILDTOOLS)/configure/ar.mk
include $(BUILDTOOLS)/configure/mkdir.mk
include $(BUILDTOOLS)/configure/ranlib.mk
include $(BUILDTOOLS)/configure/shlib.mk

define make_library_impl

$$(call make_objects,$(1),$(2))
$$(foreach srcdir,$(3),$$(call make_objects,$(1)_$$(subst /,_,$$(srcdir)),$$(srcdir)))
$$(foreach srcdir,$(3),$$(eval $(1)_SRCS += $$($(1)_$$(subst /,_,$$(srcdir))_SRCS)))
$$(foreach srcdir,$(3),$$(eval $(1)_OBJS += $$($(1)_$$(subst /,_,$$(srcdir))_OBJS)))

$(1)_LIBRARIES := $$($(1)_OBJDIR)/$(1).a $$($(1)_OBJDIR)/$(1).so

$$($(1)_OBJDIR)/$(1).a: $$($(1)_OBJS)
	+$$(QUIET)[ -d $$(@D) ] || $$(MKDIR) -p $$(@D)
	$$(if $$(QUIET),$$(info [AR] $$@))
	$$(QUIET)$$(AR) r $$@ $$($(1)_OBJS)
	$$(if $$(QUIET),$$(info [RANLIB] $$@))
	$$(QUIET)$$(RANLIB) $$@

$$(call debug,[lib] $$($(1)_OBJDIR)/$(1).a: $$($(1)_OBJS))

$(1)_SOBJS := $$(patsubst %.o,%-pic.o,$$($(1)_OBJS))

$$($(1)_OBJDIR)/$(1).so: $$($(1)_SOBJS)
	$$(if $(QUIET),$$(info [SHLIB] $$@))
	+$(QUIET)[ -d $$(@D) ] || $(MKDIR) -p $$(@D)
	$(QUIET)$$(call make_shlib,$$@,$$($(1)_SOBJS),$$($(1)_SRCS))

$$(call debug,[lib] $$($(1)_OBJDIR)/$(1).so: $$($(1)_SRCS))
$$(call debug,[lib] $$($(1)_OBJDIR)/$(1).so: $$($(1)_SOBJS))

$$(call debug,[lib] $(1)_LIBRARIES = $$($(1)_LIBRARIES))

endef

# $1: A unique prefix.
# $2: Source dir for the library build.
# $3: Additional source directories.
# $4: Dependency.
make_library = $(eval $(call make_library_impl,$(1),$(2),$(3),$(4)))

endif
