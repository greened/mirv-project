ifndef TESTSUITE_UNIT_CORE_FILTER_TARGET_MK
TESTSUITE_UNIT_CORE_FILTER_TARGET_MK := 1

libmirvcore_filter_test_SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(SRCDIR)/src/Core/target.mk
include $(SRCDIR)/src/Filter/Snapshot/Print/target.mk

include $(BUILDTOOLS)/autounit.mk

libmirvcore_filter_test_LIBS = $(filter %.a,$(libmirvfilter_print_LIBRARIES) $(libmirv_core_LIBRARIES))
libmirvcore_filter_test_INCLUDES = $(FINAL_BUILDDIR)/include

$(call make_unittest,libmirvcore_filter_test,$(libmirvcore_filter_test_SRCDIR),$(libmirvcore_filter_test_LIBS),$(libmirvcore_filter_test_INCLUDES),,,,-static,$(libmirvcore_filter_test_LIBS) $(libmirvcore_filter_test_INCLUDES))

$(call debug, [print test] libmirvcore_filter_test_UNITTESTS = $(libmirvcore_filter_test_UNITTESTS))

TARGETS   += $(libmirvcore_filter_test_UNITTESTS)
UNITTESTS += $(libmirvcore_filter_test_UNITTESTS)

endif
