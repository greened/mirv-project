ifndef CONFIGURE_MK
CONFIGURE_MK := 1

CONFIGURE := [configure]
CONFIGURE_COMMA := ,

mc_error = $(error $(CONFIGURE) $(1))
mc_info  = $(info $(CONFIGURE) $(1))
mc_info_append  = echo "\$$(call mc_info,$(1))" >> $(2)


include $(BUILDTOOLS)/configure/host.mk
include $(BUILDTOOLS)/configure/build_host.mk
include $(BUILDTOOLS)/configure/target.mk

endif

