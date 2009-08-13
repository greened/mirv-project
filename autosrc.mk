ifndef AUTOSRC_MK
AUTOSRC_MK := 1

define make_source_impl

$$(call debug,$(1) SRCDIRS = $(2))

$(1)_CSRCS := $$(subst //,/,$$(subst $(SRCDIR)/,,$$(foreach dir,$(2),$$(wildcard $$(dir)/*.c))))

$$(call debug,$(1)_CSRCS = $$($(1)_CSRCS))

$(1)_CXXSRCS := $$(subst //,/,$$(subst $(SRCDIR)/,,$$(foreach dir,$(2),$$(wildcard $$(dir)/*.cc) $$(wildcard $$(dir)/*.C) $$(wildcard $$(dir)/*.cpp))))

$$(call debug,$(1)_CXXSRCS = $$($(1)_CXXSRCS))

$(1)_SRCS := $$($(1)_CSRCS) $$($(1)_CXXSRCS)

$(1)_DEPS :=  $$(patsubst %.c,%.d,$$(patsubst %.cc,%.dd,$$(patsubst %.C,%.dd,$$(patsubst %.cpp,%.dd,$$($(1)_SRCS)))))

$$(call debug,$(1)_SRCS = $$($(1)_SRCS))
$$(call debug,$(1)_DEPS = $$($(1)_DEPS))

-include $$($(1)_DEPS)

endef

make_source = $(eval $(call make_source_impl,$(1),$(2)))

endif
