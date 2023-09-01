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

cmake_minimum_required(VERSION 3.14)

project(mirv-projectTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(mirv-project REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(mirv-project_test source/mirv-project_test.cpp)
target_link_libraries(mirv-project_test PRIVATE mirv-project::mirv-project)
target_compile_features(mirv-project_test PRIVATE cxx_std_20)

add_test(NAME mirv-project_test COMMAND mirv-project_test)

# ---- End-of-file commands ----

add_folders(Test)
