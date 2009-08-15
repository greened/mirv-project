findfile = $(strip $(foreach dir,$(subst :, ,$(PATH)),$(wildcard $(dir)/$(1)) ))


gcc = $(call findfile,gcc)

define var_rule_impl

$(1)_rule:
	$$(eval $(1) := $$($2))echo $(1) := $$($(2)) > $$@

endef

var_rule = $(eval $(call var_rule_impl,$(1),$(2)))$(1)_rule

define check_rule_impl

$(1)_check_rule: $(2)
	$$(info $(1) = $$($(1)))
	touch $$@
endef

check_rule = $(eval $(call check_rule_impl,$(1),$(2)))$(1)_check_rule

var_rule_tag := $(call var_rule,CC,gcc)

check_rule_tag := $(call check_rule,CC,$(var_rule_tag))


include $(var_rule_tag) $(check_rule_tag)

.PHONY: all
all:
	$(info all CC = $(CC))
