ifndef SHLIB_MK
SHLIB_MK := 1

define make_shlib_impl

include $(BUILDTOOLS)/configure/cc.mk
include $(BUILDTOOLS)/configure/cxx.mk

ifneq ($$(strip $$(filter %.cc,$(3)) $$(filter %.cxx,$(3)) $$(filter %.C,$(3))),)

  ifdef CONFIG_HAVE_GXX
    $$(LXX) -rdynamic -shared -o $(1) $(2)
  else
    $$(error Unknown C++ compiler $(CC))
  endif

else
  ifneq ($$(strip $$(filter %.c,$(3))),)
    ifdef CONFIG_HAVE_GCC
      $$(LD) -rdynamic -shared -o $(1) $(2)
    else
      $$(error Unknown C compiler $(CC))
    endif
  else
    $$(error Unknown source file type in $(3))
  endif
endif

endef

make_shlib = $(if $(strip $(filter %.cc,$(3)) $(filter %.cxx,$(3)) $(filter %.C,$(3))),$(if $(CONFIG_HAVE_GXX),$(LXX) -rdynamic -shared -o $(1) $(2),$(error Unknown C++ compiler $(CC))),$(if $(strip $(filter %.c,$(3))),$(if $(CONFIG_HAVE_GCC),$(LD) -rdynamic -shared -o $(1) $(2),$(error Unknown C compiler $(CC))),$(error Unknown source file type in $(3))))

# $(eval $(call make_shlib_impl,$(1),$(2),$(3)))

endif
