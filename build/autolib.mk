ifndef AUTOLIB_MK
AUTOLIB_MK := 1

include $(BUILDTOOLS)/autoobj.mk
include $(BUILDTOOLS)/configure/ar.mk
include $(BUILDTOOLS)/configure/ranlib.mk
include $(BUILDTOOLS)/configure/shlib.mk

define make_library_impl

$$(call make_objects,$(1),$(2))

$(1)_LIBRARIES := $$($(1)_OBJDIR)/$(1).a $$($(1)_OBJDIR)/$(1).so

$$($(1)_OBJDIR)/$(1).a: $$($(1)_OBJS)
	$$(if $(QUIET),$$(info [AR] $$@))
	$(QUIET)$$(AR) r $$@ $$^
	+$(QUIET)[ -d $(@D) ] || $(MKDIR) -p $(@D)
	$$(if $(QUIET),$$(info [RANLIB] $$@))
	$(QUIET)$$(RANLIB) $$@

$$(call debug,$$($(1)_OBJDIR)/$(1).a: $$($(1)_OBJS))

$(1)_SOBJS := $$(patsubst %.o,%-pic.o,$$($(1)_OBJS))

$$($(1)_OBJDIR)/$(1).so: $$($(1)_SOBJS)
	$$(if $(QUIET),$$(info [SHLIB] $$@))
	+$(QUIET)[ -d $(@D) ] || $(MKDIR) -p $(@D)
	$$(call make_shlib,$$@,$$^,$$($(1)_SRCS))

$$(call debug,$$($(1)_OBJDIR)/$(1).so: $$($(1)_SOBJS))

$$(call debug,$(1)_LIBRARIES = $$($(1)_LIBRARIES))

endef

make_library = $(eval $(call make_library_impl,$(1),$(2)))

endif
