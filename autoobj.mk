ifndef AUTOOBJ_MK
AUTOOBJ_MK := 1

include $(BUILDTOOLS)/autosrc.mk

define make_objects_impl

$$(call make_source,$(1),$(2))
$(1)_OBJS := $$(patsubst %.c,%.o,$$(patsubst %.cc,%.o,$$(patsubst %.C,%.o,$$(patsubst %.cpp,%.o,$$($(1)_SRCS)))))

$(1)_OBJDIR := $(subst $(SRCDIR)/,,$(2))
$$(call debug $(1)_OBJDIR = $$($(1)_OBJDIR))

ALL_OBJS += $$($(1)_OBJS)

endef

make_objects = $(eval $(call make_objects_impl,$(1),$(2)))

endif
