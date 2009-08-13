ifndef BUILD_HOST_MK
BUILD_HOST_MK := 1

ifndef BUILD_HOST
  BUILD_HOST := $(shell $(BUILDTOOLS)/configure/config.guess)
endif

$(info $(CONFIGURE) Configuring for build host $(BUILD_HOST))

endif
