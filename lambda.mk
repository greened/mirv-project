print = $(info $(1))

define doit 

$(1)_DOIT_TEST = $(2)
$(1)_DOIT_ACTION = $(3)

$$(info $(1)_DOIT_TEST = $$($(1)_DOIT_TEST))
$$(info $(1)_DOIT_ACTION = $$($(1)_DOIT_ACTION))

$(if $$($(1)_DOIT_TEST),$$(eval $$($(1)_DOIT_ACTION)),$$(info You lose))

endef

$(call doit,HELLO,,$$(call print,Hello))

all:
