ifndef SRC_FILTER_SNAPSHOT_CODEGEN_LLVM_JIT_TARGET_MK
SRC_FILTER_SNAPSHOT_CODEGEN_LLVM_JIT_TARGET_MK := 1

libmirvfilter_codegen_llvmjit_SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))

$(call debug,[llvm-jit] SRCDIR = $(libmirvfilter_codegen_llvmjit_SRCDIR))

include $(BUILDTOOLS)/llvm.mk
include $(BUILDTOOLS)/autolib.mk

$(call make_library,libmirvfilter_codegen_llvmjit,$(libmirvfilter_codegen_llvmjit_SRCDIR))

$(libmirvfilter_codegen_llvmjit_LIBRARIES): \
  CPPFLAGS += -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS

TARGETS += $(libmirvfilter_codegen_llvmjit_LIBRARIES)

endif
