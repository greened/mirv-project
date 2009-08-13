ifndef FOLD_MK
FOLD_MK := 1

fold = $(call debug,fold("$(1)" "$(2)")) $(if $(1),$(call fold,$(wordlist 2,$(words $(1)),$(1)),$(2),$(call $(2),$(firstword $(1)),$(3))))

reverse_fold = $(if $(1),$(call $(2),$(firstword $(1),$(call reverse_fold,$(wordlist 2,$(words $(1)),$(1)),$(2),$(3)))))

endif