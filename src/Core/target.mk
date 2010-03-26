ifndef mirv_src_core
mirv_src_core := 1

libmirv_core_SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))
libmirv_core_SUBDIRS := Builder Filter IR Memory Utility
libmirv_core_SRCDIRS := $(foreach dir,$(libmirv_core_SUBDIRS),$(addprefix $(libmirv_core_SRCDIR),$(dir)))

include $(BUILDTOOLS)/boost.mk

$(call debug,[core] SRCDIRS = $(libmirv_core_SRCDIRS))

include $(BUILDTOOLS)/autolib.mk

$(call make_library,libmirv_core,$(libmirv_core_SRCDIR),$(libmirv_core_SRCDIRS))

TARGETS += $(libmirv_core_LIBRARIES)

$(call debug,[core] TARGETS = $(TARGETS))

endif
