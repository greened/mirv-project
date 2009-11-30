ifndef CREATE_MK
CREATE_MK := 1

# $1: File to create
# $2: Dependence
mc_create_start = echo "ifndef $(subst .,_,$(subst /,_,$(1)))" > $(1) && echo "$(subst .,_,$(subst /,_,$(1))) := 1" >> $(1) && echo >> $(1) && echo "include $(BUILDTOOLS)/configure/configure.mk" >> $(1) $(if $(2),&& echo "include $(2)" >> $(1) && echo >> $(1))

# $1: File to create
mc_create_end = echo >> $(1) && echo "endif  \# $(subst .,_,$(subst /,_,$(1)))" >> $(1)

endif
