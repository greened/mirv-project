ifndef SRC_FILTER_SNAPSHOT_PRINT_TARGET_MK
SRC_FILTER_SNAPSHOT_PRINT_TARGET_MK := 1

libmirvfilter_print_SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))

$(call debug,[print] SRCDIR = $(libmirvfilter_print_SRCDIR))

include $(BUILDTOOLS)/autolib.mk

$(call make_library,libmirvfilter_print,$(libmirvfilter_print_SRCDIR))

TARGETS += $(libmirvfilter_print_LIBRARIES)

endif
