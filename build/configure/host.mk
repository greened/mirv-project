ifndef HOST_MK
HOST_MK := 1

include $(BUILDTOOLS)/configure/build_host.mk

ifndef HOST
  HOST := $(BUILD_HOST)
endif

$(info $(CONFIGURE) Configuring for host $(HOST))

endif
