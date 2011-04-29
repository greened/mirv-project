ifndef AUTOUNIT_MK
AUTOUNIT_MK := 1

include $(BUILDTOOLS)/autoobj.mk
include $(BUILDTOOLS)/autoexe.mk
include $(BUILDTOOLS)/configure/grep.mk
include $(BUILDTOOLS)/configure/cut.mk
include $(BUILDTOOLS)/configure/cmp.mk
include $(BUILDTOOLS)/configure/tee.mk
include $(BUILDTOOLS)/configure/cat.mk

define make_unittest_impl

$$(call debug,[unit] '$(1)' '$(2)' '$(3)' '$(4)' '$(5)' '$(6)' '$(7)' '$(8)' '$(9)')

$$(call make_objects,$(1),$(2),$(4),$(5),$(6))

$$(call debug,[unit] $(1)_OBJS = $$($(1)_OBJS))

$$(foreach obj,$$($(1)_OBJS),$$(call make_simple_exe,$$(obj),$(3),$(7),$(8),$(9)))

$(1)_UNITTESTS     += $$(patsubst %.o,%.display,$$($(1)_OBJS))
$(1)_UNITTEST_DEPS += $$(patsubst %.o,%.dd,$$($(1)_OBJS))

-include $$($(1)_UNITTEST_DEPS)

endef

# $1: A unique identifier for the set of tests
# $2: The source directory containing the tests
# $3: Libraries needed to build the tests
# $4: Include directories
# $5: The compiler
# $6: Compiler flags
# $7: The linker
# $8: Linker flags
# $9: Dependency
make_unittest = $(eval $(call make_unittest_impl,$(1),$(2),$(3),$(4),$(5),$(6),$(7),$(8),$(9)))

.PRECIOUS: %.out %.cf %.result %.src %.mch %.hdr %.new

%.out: %.exe
	-$(QUIET)$(<) > $(@)

%.cf: %.cpp
	-$(QUIET)$(GREP) "STDOUT" $(<) | $(CUT) -d' ' -f3- > $(@)

%.result: %.out %.cf
	$(QUIET)if $(CMP) -s $(*).out $(*).cf; then \
	          echo "PASS: $(*).exe" > $(@); \
	        else \
	          echo "FAIL: $(*).exe" > $(@); \
	        fi

.PHONY: %.display
%.display: %.result
	$(QUIET)$(CAT) $<

%.hdr:
	$(QUIET)$(GREP) "^//" $(SRCDIR)/$(*).cpp | $(GREP) -v "STDOUT" > $(@)

%.src:
	$(QUIET)$(GREP) -v "^//" $(SRCDIR)/$(*).cpp > $(@)

%.mch: %.out
	$(QUIET)$(CAT) $(<) | sed -e "s%^%// STDOUT: %" > $(@)

%.update: %.hdr %.mch %.src
	$(QUIET)$(CAT) $(^) > $(@)

endif
