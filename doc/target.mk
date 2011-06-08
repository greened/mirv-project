ifndef mirv_doc
mirv_doc := 1

doc_SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(BUILDTOOLS)/autodox.mk

$(call debug,[doc] SRCDIR = $(SRCDIR))

doc_SRCS := $(shell find $(SRCDIR)/src -regextype posix-extended -regex ".*\.cpp|.*\.c" -print)

$(call debug,[doc] SRCS = $(doc_SRCS))

$(call make_doxyconfig,mirv,$(doc_SRCDIR),$(doc_SRCS))

# Disable this for now due to doxygen bugs processing C++.
#TARGETS += $(mirv_DOXY_STAMPS)

$(call debug,[doc] TARGETS = $(TARGETS))

endif
