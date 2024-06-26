ifndef SRC_FILTER_SNAPSHOT_CODEGEN_LLVM_TARGET_MK
SRC_FILTER_SNAPSHOT_CODEGEN_LLVM_TARGET_MK := 1

libmirvfilter_codegen_llvm_SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))

$(call debug,[llvm] SRCDIR = $(libmirvfilter_codegen_llvm_SRCDIR))

include $(BUILDTOOLS)/llvm.mk
include $(BUILDTOOLS)/autolib.mk

$(call make_library,libmirvfilter_codegen_llvm,$(libmirvfilter_codegen_llvm_SRCDIR))

$(libmirvfilter_codegen_llvm_LIBRARIES): \
  CPPFLAGS += $(LLVM_COMPILE_FLAGS)

TARGETS += $(libmirvfilter_codegen_llvm_LIBRARIES)

endif
