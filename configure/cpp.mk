ifndef CPP_MK
CPP_MK := 1

$(call debug,cpp.mk)

include $(BUILDTOOLS)/configure/cc.mk
include $(BUILDTOOLS)/configure/target.mk
include $(BUILDTOOLS)/configure/findfile.mk
include $(BUILDTOOLS)/configure/preprocess.mk
include $(BUILDTOOLS)/configure/try.mk
include $(BUILDTOOLS)/configure/define.mk

CPP_SOURCE := int main(void) { return(0); }
CPP_TRUE = $(call mc_define,CPP,$(CC) -E,$@)
CPP_FALSE = $(call mc_error,Cannot run $(CC) -E)

ifeq ($(origin CPP),default)
  override CPP :=
endif
__sink := $(call mc_try_preprocess_c,CPP_SOURCE,CPP_TRUE,CPP_FALSE,$(FINAL_BUILDDIR)/configure/CPP.mk)

endif
