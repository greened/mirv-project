ifndef TESTSUITE_UNIT_FILTER_SNAPSHOT_CODEGEN_LLVM_TARGET_MK
TESTSUITE_UNIT_FILTER_SNAPSHOT_CODEGEN_LLVM_TARGET_MK := 1

libmirvfilter_codegen_llvm_test_SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(SRCDIR)/src/Core/target.mk
include $(SRCDIR)/src/Filter/Snapshot/Codegen/LLVM/target.mk

include $(BUILDTOOLS)/autounit.mk

libmirvfilter_codegen_llvm_test_LIBS = $(filter %.a,$(libmirvfilter_codegen_llvm_LIBRARIES) $(libmirv_core_LIBRARIES))
libmirvfilter_codegen_llvm_test_INCLUDES = $(FINAL_BUILDDIR)/include

# FIXME: Do not hardcode path to LLVM libraries.
$(call make_unittest,libmirvfilter_codegen_llvm_test,$(libmirvfilter_codegen_llvm_test_SRCDIR),$(libmirvfilter_codegen_llvm_test_LIBS),$(libmirvfilter_codegen_llvm_test_INCLUDES),,,,-static -L/usr/lib/llvm/lib -lLLVMCore -lLLVMSupport -lLLVMSystem -lpthread -ldl -lm,$(libmirvfilter_codegen_llvm_test_LIBS) $(libmirvfilter_codegen_llvm_test_INCLUDES))

$(call debug, [llvm codegen test] libmirvfilter_codegen_llvm_test_UNITTESTS = $(libmirvfilter_codegen_llvm_test_UNITTESTS))

$(libmirvfilter_codegen_llvm_test_UNITTESTS): \
  CPPFLAGS += -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS

TARGETS += $(libmirvfilter_codegen_llvm_test_UNITTESTS)

endif
