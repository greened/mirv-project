ifndef SHLIB_MK
SHLIB_MK := 1

define make_shlib_impl

include $(BUILDTOOLS)/configure/cc.mk
include $(BUILDTOOLS)/configure/cxx.mk

ifneq ($(strip $(filter %.cc,$(3)) $(filter %.cxx,$(3)) $(filter %.C,$(3))),)

  ifdef HAVE_GXX
    $$(LXX) -rdynamic -shared -o $(1).so $(2)
  else
$(error Unknown C++ compiler $(CC))
  endif

else
  ifneq ($(strip $(filter %.c,$(3))),)
    ifdef HAVE_GCC
      $$(LD) -rdynamic -shared -o $(1).a $(2)
    else
$(error Unknown C compiler $(CC))
    endif
  else
$(error Unknown source file type in $(3))
  endif
endif

endef

make_shlib = $(eval $(call make_shlib,$(1),$(2),$(3)))

endif
