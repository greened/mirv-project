# Copyright 2023 David A. Greene

# This file is part of the MIRV project.

# MIRV is free software: you can redistribute it and/or modify it under the
# terms of the GNU Affero General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.

# MIRV is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
# details.

# You should have received a copy of the GNU General Public License along with
# Foobar. If not, see <https://www.gnu.org/licenses/>.

ifndef TESTSUITE_UNIT_CORE_BUILDER_TARGET_MK
TESTSUITE_UNIT_CORE_BUILDER_TARGET_MK := 1

libmirvcore_builder_test_SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(SRCDIR)/src/Core/target.mk
include $(SRCDIR)/src/Filter/Snapshot/Print/target.mk

include $(BUILDTOOLS)/autounit.mk

libmirvcore_builder_test_LIBS = $(filter %.a,$(libmirvfilter_print_LIBRARIES) $(libmirv_core_LIBRARIES))
libmirvcore_builder_test_INCLUDES = $(FINAL_BUILDDIR)/include

$(call make_unittest,libmirvcore_builder_test,$(libmirvcore_builder_test_SRCDIR),$(libmirvcore_builder_test_LIBS),$(libmirvcore_builder_test_INCLUDES),,,,-static,$(libmirvcore_builder_test_LIBS) $(libmirvcore_builder_test_INCLUDES))

$(call debug, [print test] libmirvcore_builder_test_UNITTESTS = $(libmirvcore_builder_test_UNITTESTS))

TARGETS   += $(libmirvcore_builder_test_UNITTESTS)
UNITTESTS += $(libmirvcore_builder_test_UNITTESTS)

endif
