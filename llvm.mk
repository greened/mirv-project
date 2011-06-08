ifndef LLVM_MK
LLVM_MK := 1

$(call debug,[llvm] llvm.mk)

include $(BUILDTOOLS)/configure/library.mk

# See if the LLVM libraries are available.

define LLVM_SOURCE
#include <llvm/Constants.h>
#include <llvm/LLVMContext.h>
#include <llvm/Type.h>

int main(void)
{
  llvm::LLVMContext Context;
  llvm::Value *Const = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 4);
  return 0;
}
endef

override LLVM_LIBRARY = libLLVMCore.a libLLVMSupport.a libLLVMSystem.a libpthread.a

ifndef LLVM_PATH
LLVM_PATH=/usr/lib/llvm-2.8
endif

ifndef LLVM_LIBRARY_PATH
LLVM_LIBRARY_PATH = $(LLVM_PATH)/lib
endif

ifndef LLVM_INCLUDE_PATH
LLVM_INCLUDE_PATH = $(LLVM_PATH)/include
endif

override LLVM_LINK_FLAGS = -L$(LLVM_LIBRARY_PATH) -rdynamic -Xlinker -rpath -Xlinker $(LLVM_LIBRARY_PATH) -lpthread -ldl -lm
override LLVM_COMPILE_FLAGS = -I$(LLVM_INCLUDE_PATH) -fpermissive -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS
override LLVM_COMPILE_ERROR = $(call mc_error,Cannot find LLVM libraries)
override LLVM_LINK_ERROR = $(call mc_error,Cannot link to LLVM libraries)
override LLVM_COMPILE_TRUE = $(call mc_define_append_nomsg,CONFIG_LLVM_COMPILE,yes,$@); $(call mc_info_append,C++ compiler can find LLVM headers,$@)
override LLVM_LINK_TRUE = $(call mc_define_append_nomsg,CONFIG_LLVM_LINK,yes,$@); $(call mc_info_append,C++ linker can link to LLVM libraries,$@)
override LLVM_DEP_MK = $(FINAL_BUILDDIR)/configure/CXX.mk

$(call mc_try_library_cxx,LLVM,LLVM_SOURCE,LLVM_LIBRARY,$(LLVM_COMPILE_FLAGS),$(LLVM_LINK_FLAGS),LLVM_COMPILE_TRUE,LLVM_COMPILE_ERROR,LLVM_LINK_TRUE,LLVM_LINK_ERROR,$(LLVM_DEP_MK))

endif
