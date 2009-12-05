ifndef TESTSUITE_UNIT_FILTER_SNAPSHOT_PRINT_TARGET_MK
TESTSUITE_UNIT_FILTER_SNAPSHOT_PRINT_TARGET_MK := 1

libmirvfilter_print_test_SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))
libmirvfilter_print_test_LIBS = $(libmirvfilter_print_LIBRARIES)
libmirvfilter_print_test_INCLUDES = $(FINAL_BUILDDIR)/include

include $(SRCDIR)/src/filter/snapshot/print/target.mk

include $(BUILDTOOLS)/autounit.mk

$(call make_unittest,libmirvfilter_print_test,$(libmirvfilter_print_test_SRCDIR),$(libmirvfilter_print_test_LIBS),$(libmirvfilter_print_test_INCLUDES),,,,,$(libmirvfilter_print_test_LIBS) $(libmirvfilter_print_test_INCLUDES))

$(call debug, [print test] libmirvfilter_print_test_UNITTESTS = $(libmirvfilter_print_test_UNITTESTS))

TARGETS += $(libmirvfilter_print_test_UNITTESTS)

endif
