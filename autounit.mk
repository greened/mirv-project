ifndef AUTOUNIT_MK
AUTOUNIT_MK := 1

include $(BUILDTOOLS)/autoobj.mk
include $(BUILDTOOLS)/autoexe.mk
include $(BUILDTOOLS)/configure/grep.mk
include $(BUILDTOOLS)/configure/cut.mk
include $(BUILDTOOLS)/configure/cmp.mk

define make_unittest_impl

$$(call make_objects,$(1),$(2),$(4),$(5),$(6))

$$(foreach obj,$$($(1)_OBJS),$$(call make_simple_exe,$(1),$$(obj),$(3),$(7),$(8)))

%.out: %.exe
	$(QUIET)$$(<) > $$(@)

%.cf: %.c
	$(QUIET)$(GREP) "STDOUT" $(<) | $(CUT) -d' ' -f2- > $$(@)

%.result: %.out, %.cf
	$(QUIET)if $(CMP) $$(*).out $$(*).cf; then \
	          $(QUIET)echo "PASS: $$(*).exe" | $(TEE) $$(@); \
	        else \
	          $(QUIET)echo "FAIL: $$(*).exe" | $(TEE) $$(@); \
	        fi

$(1)_UNITTESTS += $$(patsubst %.o,%.result,$$($(1)_OBJS))

endef

# $1: A unique identifier for the set of tests
# $2: The source directory containing the tests
# $3: Libraries needed to build the tests
# $4: Include directories
# $5: The compiler
# $6: Compiler flags
# $7: The linker
# $8: Linker flags
make_unittest = $(eval $(call make_unittest_impl,$(1),$(2),$(3),$(4),$(5),$(6),$(7),$(8)))

endif
