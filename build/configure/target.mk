ifndef TARGET_MK
TARGET_MK := 1

include $(BUILDTOOLS)/configure/build_host.mk

ifndef TARGET
  TARGET := $(BUILD_HOST)
endif

$(info $(CONFIGURE) Configuring for target $(TARGET))

endif
