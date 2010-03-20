ifndef mirv_doc
mirv_doc := 1

doc_SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(BUILDTOOLS)/autodox.mk

$(call make_doxyconfig,libmirv_core,$(doc_SRCDIR))

TARGETS += $(libmirv_core_DOXY_STAMPS)

$(call debug,[doc] TARGETS = $(TARGETS))

endif
