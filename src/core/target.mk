ifdef CORE_NO_SOURCES

libmirv_core_SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))
libmirv_core_SUBDIRS := builder filter ir mem util
libmirv_core_SRCDIRS := $(foreach dir,$(libmirv_core_SUBDIRS),$(addprefix $(libmirv_core_SRCDIR),$(dir)))

$(call debug,[core] SRCDIRS = $(libmirv_core_SRCDIRS))

include $(BUILDTOOLS)/autolib.mk

$(call make_library,libmirv_core,$(libmirv_core_SRCDIRS))

TARGETS += $(libmirv_core_LIBRARIES)

$(call debug,[core] TARGETS = $(TARGETS))

endif
