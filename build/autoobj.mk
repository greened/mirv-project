ifndef AUTOOBJ_MK
AUTOOBJ_MK := 1

include $(BUILDTOOLS)/autosrc.mk

define make_objects_impl

$$(call debug,[obj] '$(1)' '$(2)' '$(3)' '$(4)' '$(5)')

$$(call make_source,$(1),$(2))
$(1)_OBJS := $$(patsubst %.c,%.o,$$(patsubst %.cc,%.o,$$(patsubst %.C,%.o,$$(patsubst %.cpp,%.o,$$($(1)_SRCS)))))

$$(call debug,[obj] $(1)_OBJS = $$($(1)_OBJS))

$(1)_CPPFLAGS = $(patsubst %,-I%,$(3)) $(5)
$$($(1)_OBJS): CPPFLAGS += $$($(1)_CPPFLAGS)

ifneq ($(4),)
$$($(1)_OBJS): CXX = $(4)
$$($(1)_OBJS): CC  = $(4)
endif

$(1)_OBJDIR := $(subst $(SRCDIR)/,,$(2))
$$(call debug,[obj] $(1)_OBJDIR = $$($(1)_OBJDIR))

ALL_OBJS += $$($(1)_OBJS)

endef

# $1: A unique identifier
# $2: The directory containing the sources
# $3: Include directories
# $4: The compiler
# $5: Compiler flags
make_objects = $(eval $(call make_objects_impl,$(1),$(2),$(3),$(4),$(5)))

endif
