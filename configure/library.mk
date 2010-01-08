ifndef LIBRARY_MK
LIBRARY_MK := 1

include $(BUILDTOOLS)/configure/executable.mk

# $1: A unique name
# $2: The name of the variable holding the compiler location
# $3: The source code to compile
# $4: The source code file extension
# $5: The library to link
# $6: Extra compiler flags
# $7: Extra linker flags
# $8: Compile true action
# $9: Compile false action
# $10: Link true action
# $11: Link false action
# $12: Dependence
mc_try_library = $(call debug,[mc_try_library] mc_try_library("$(1)" "$(2)" "$(3)" "$(4)" "$(5)" "$(6)" "$(7)" "$(8)" "$(9)" "$(10)" "$(11)" "$(12)"))$(call mc_try_executable,$(1)_library,$(2),$(3),$(4),$(6),$(7) $(foreach lib,$($(5)),$(patsubst %.a,%,$(patsubst lib%,-l%,$(lastword $(subst /, ,$(lib)))))),$(8),$(9),$(10),$(11),$(12))

# $1: A unique name
# $2: The source code to compile
# $3: The library to link to
# $4: Extra compiler flags
# $5: Extra linker flags
# $6: Compile true action
# $7: Compile false action
# $8: Link true action
# $9: Link false action
# $10: Dependence
mc_try_library_c = $(call mc_try_library,$(1),CC,$(2),c,$(3),$(4),$(5),$(6),$(7),$(8),$(9),$(10))

# $1: A unique name
# $2: The source code to compile
# $3: The library to link to
# $4: Extra compiler flags
# $5: Extra linker flags
# $6: Compile true action
# $7: Compile false action
# $8: Link true action
# $9: Link false action
# $10: Dependence
mc_try_library_cxx = $(call mc_try_library,$(1),CXX,$(2),cc,$(3),$(4),$(5),$(6),$(7),$(8),$(9),$(10))

endif
