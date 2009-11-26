ifndef EXECUTABLE_MK
EXECUTABLE_MK := 1

include $(BUILDTOOLS)/configure/compile.mk
include $(BUILDTOOLS)/configure/link.mk

# $1: The name of the variable holding the compiler location
# $2: Source code to compile and link
# $3: Compile true action
# $4: Compile false action
# $5: Link true action
# $6: Link false action
# $7: The source code file extension
# $8: Compile include file to create (actions must create it) 
# $9: Link include file to create (actions must create it)
# $10: Dependence
mc_try_executable = $(call mc_try_compile,$(1),$(2),$(3),$(4),$(7),$(8),$(10))$(call mc_try_link,$(1),$(1)_compile_test.o,$(5),$(6),$(9),$(10))

# $1: Source code to compile and link
# $2: Compile true action
# $3: Compile false action
# $4: Link true action
# $5: Link false action
# $6: Compile include file to create (actions must create it) 
# $7: Link include file to create (actions must create it)
# $8: Dependence
mc_try_executable_c = $(call mc_try_compile_c,$(1),$(2),$(3),$(6),$(8))$(call mc_try_link_c,$(FINAL_BUILDDIR)/configure/CC_compile_test.o,$(4),$(5),$(7),$(8))

# $1: Source code to compile and link
# $2: Compile true action
# $3: Compile false action
# $4: Link true action
# $5: Link false action
# $6: Compile include file to create (actions must create it)
# $7: Link include file to create (actions must create it)
# $8: Dependence
mc_try_executable_cxx = $(call mc_try_compile_cxx,$(1),$(2),$(3),$(6),$(8))$(call mc_try_link_cxx,$(FINAL_BUILDDIR)/configure/CXX_compile_test.o,$(4),$(5),$(7),$(8))

endif
