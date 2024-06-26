ifndef bind_mk
bind_mk := 1

define bind1_impl

$(1)_bind1 = $$(call $(2),$(3))
$$(call debug,[bind1] $(1)_bind1 = call $(2) $(3))

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

define bind4_impl

$(1)_bind4 = $$(call $(2),$(3),$(4),$(5),$(6))

endef

bind4 = $(eval $(call bind4_impl,$(1),$(2),$(3),$(4),$(5),$(6)))$(1)_bind4

define bind5_impl

$(1)_bind5 = $$(call $(2),$(3),$(4),$(5),$(6),$(7))

endef

bind5 = $(eval $(call bind5_impl,$(1),$(2),$(3),$(4),$(5),$(6),$(7)))$(1)_bind5

define bind6_impl

$(1)_bind6 = $$(call $(2),$(3),$(4),$(5),$(6),$(7),$(8))

endef

bind6 = $(eval $(call bind6_impl,$(1),$(2),$(3),$(4),$(5),$(6),$(7),$(8)))$(1)_bind6

endif
