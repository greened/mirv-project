ifndef bind_mk
bind_mk := 1

define bind1_impl

$(1)_bind1 = $$(call $(2),$(3))

endef

bind1 = $(eval $(call bind1_impl,$(1),$(2),$(3)))$(1)_bind1

define bind2_impl

$(1)_bind2 = $$(call $(2),$(3),$(4))

endef

bind2 = $(eval $(call bind2_impl,$(1),$(2),$(3),$(4)))$(1)_bind2

define bind3_impl

$(1)_bind3 = $$(call $(2),$(3),$(4),$(5))

endef

bind3 = $(eval $(call bind3_impl,$(1),$(2),$(3),$(4),$(5)))$(1)_bind3

endif
