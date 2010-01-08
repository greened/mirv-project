ifndef EXECUTABLE_MK
EXECUTABLE_MK := 1

include $(BUILDTOOLS)/configure/compile.mk
include $(BUILDTOOLS)/configure/link.mk

# $1: A unique name
# $2: The name of the variable holding the compiler location
# $3: Source code to compile and link
# $4: The source code file extension
# $5: Extra compiler flags
# $6: Extra linker flags
# $7: Compile true action
# $8: Compile false action
# $9: Link true action
# $10: Link false action
# $11: Dependence
mc_try_executable = $(call debug,[mc_try_executable] mc_try_executable("$(1)" "$(2)" "$(3)" "$(4)" "$(5)" "$(6)" "$(7)" "$(8)" "$(9)" "$(10)" "$(11)"))$(call mc_try_compile,$(1)_executable,$(2),$(3),$(4),$(5),$(7),$(8),$(11))$(call mc_try_link,$(1)_executable,$(2),$(FINAL_BUILDDIR)/configure/$(1)_executable_compile_test.o,$(6),$(9),$(10),$(FINAL_BUILDDIR)/configure/$(1)_executable_compile_result.mk)

# $1: A unique name
# $2: Source code to compile and link
# $3: Extra compiler flags
# $4: Extra linker flags
# $5: Compile true action
# $6: Compile false action
# $7: Link true action
# $8: Link false action
# $9: Dependence
mc_try_executable_c = $(call mc_try_executable,$(1),CC,$(2),c,$(3),$(4),$(5),$(6),$(7),$(8),$(9))

# $1: A unque name
# $2: Source code to compile and link
# $3: Extra compiler flags
# $4: Extra linker flags
# $5: Compile true action
# $6: Compile false action
# $7: Link true action
# $8: Link false action
# $9: Dependence
mc_try_executable_cxx = $(call mc_try_executable,$(1),CXX,$(2),cc,$(3),$(4),$(5),$(6),$(7),$(8),$(9))

endif
