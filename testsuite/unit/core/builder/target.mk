ifndef TESTSUITE_UNIT_CORE_BUILDER_TARGET_MK
TESTSUITE_UNIT_CORE_BUILDER_TARGET_MK := 1

libmirvcore_builder_test_SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(SRCDIR)/src/core/target.mk
include $(SRCDIR)/src/filter/snapshot/print/target.mk

include $(BUILDTOOLS)/autounit.mk

libmirvcore_builder_test_LIBS = $(filter %.a,$(libmirvfilter_print_LIBRARIES) $(libmirv_core_LIBRARIES))
libmirvcore_builder_test_INCLUDES = $(FINAL_BUILDDIR)/include

$(call make_unittest,libmirvcore_builder_test,$(libmirvcore_builder_test_SRCDIR),$(libmirvcore_builder_test_LIBS),$(libmirvcore_builder_test_INCLUDES),,,,-static,$(libmirvcore_builder_test_LIBS) $(libmirvcore_builder_test_INCLUDES))

$(call debug, [print test] libmirvcore_builder_test_UNITTESTS = $(libmirvcore_builder_test_UNITTESTS))

TARGETS += $(libmirvcore_builder_test_UNITTESTS)

endif
