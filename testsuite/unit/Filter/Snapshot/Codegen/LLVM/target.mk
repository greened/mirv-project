ifndef TESTSUITE_UNIT_FILTER_SNAPSHOT_CODEGEN_LLVM_TARGET_MK
TESTSUITE_UNIT_FILTER_SNAPSHOT_CODEGEN_LLVM_TARGET_MK := 1

libmirvfilter_codegen_llvm_test_SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(SRCDIR)/src/Core/target.mk
include $(SRCDIR)/src/Filter/Snapshot/Codegen/LLVM/target.mk

include $(BUILDTOOLS)/autounit.mk

libmirvfilter_codegen_llvm_test_LIBS = $(filter %.a,$(libmirvfilter_codegen_llvm_LIBRARIES) $(libmirv_core_LIBRARIES))
libmirvfilter_codegen_llvm_test_INCLUDES = $(FINAL_BUILDDIR)/include
libmirvfilter_codegen_llvm_test_LIBDIRS = $(FINAL_BUILDDIR)/src/Core $(FINAL_BUILDDIR)/src/Filter/Snapshot/Codegen/LLVM
libmirvfilter_codegen_llvm_test_LINK_FLAGS = $(addprefix,-L,$(libmirvfilter_codegen_llvm_test_LIBDIRS)) $(addprefix,-Xlinker -rpath -Xlinker ,$(libmirvfilter_codegen_llvm_test_LIBDIRS))

# FIXME: Do not hardcode path to LLVM libraries.
libmirvfilter_codegen_llvm_LIBS = $(shell llvm-config-2.8 --libs core)

$(call make_unittest,libmirvfilter_codegen_llvm_test,$(libmirvfilter_codegen_llvm_test_SRCDIR),$(libmirvfilter_codegen_llvm_test_LIBS),$(libmirvfilter_codegen_llvm_test_INCLUDES),,,, $(libmirvfilter_codegen_llvm_test_LINK_FLAGS) $(libmirvfilter_codegen_llvm_LIBS) $(LLVM_LINK_FLAGS),$(libmirvfilter_codegen_llvm_test_LIBS) $(libmirvfilter_codegen_llvm_test_INCLUDES))

$(call debug, [llvm codegen test] libmirvfilter_codegen_llvm_test_UNITTESTS = $(libmirvfilter_codegen_llvm_test_UNITTESTS))

$(libmirvfilter_codegen_llvm_test_UNITTESTS): \
  CPPFLAGS += $(LLVM_COMPILE_FLAGS)
$(libmirvfilter_codegen_llvm_test_UNITTESTS): \
  opt_CPPFLAGS += $(LLVM_COMPILE_FLAGS)
$(libmirvfilter_codegen_llvm_test_UNITTESTS): \
  debug_CPPFLAGS += $(LLVM_COMPILE_FLAGS)
$(libmirvfilter_codegen_llvm_test_UNITTESTS): \
  DFLAGS += $(LLVM_COMPILE_FLAGS)

TARGETS   += $(libmirvfilter_codegen_llvm_test_UNITTESTS)
UNITTESTS += $(libmirvfilter_codegen_llvm_test_UNITTESTS)

endif
