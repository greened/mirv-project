ifndef AUTOIN_MK
AUTOIN_MK := 1

include $(BUILDTOOLS)/configure/sed.mk

define make_in_impl
$$(call debug,[in] "$(1)" "$(2)" "$(3)" "$(4)" "$(5)")

$(1)_INS := $$(subst //,/,$$(subst $(SRCDIR)/,,$$(foreach dir,$(2),$$(wildcard $$(dir)/*.$(4).in))))

$$(call debug,[in] $(1)_INS = $$($(1)_INS))

$(1)_SED_EXPRESSIONS := $$(addprefix -e 's%,$$(addsuffix %g',$$(subst :,%,$(3))))

$$(call debug,[in] $(1)_SED_EXPRESSIONS = $$($(1)_SED_EXPRESSIONS))

%.$(4): %.$(4).in $(5)
	+$$(QUIET)[ -d $$(@D) ] || mkdir -p $$(@D)	
	$$(if $$(QUIET),$$(info [SED] $$@))
	$$(QUIET)$$(SED) $$($(1)_SED_EXPRESSIONS) $$< > $$@

endef

# $1: A unique tag
# $2: A list of directories to search
# $3: A list of replacement pairs, text:replacement
# $4: Suffix for output file (<file>.suffix.in -> <file>.suffix)
# $5: Dependency
make_in = $(eval $(call make_in_impl,$(1),$(2),$(3),$(4),$(5)))

endif
