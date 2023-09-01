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

include(cmake/prelude.cmake)

project(
    mirv-project
    VERSION 0.1.0
    DESCRIPTION "MIRV compiler project"
    HOMEPAGE_URL "https://obbligato.org/mirv-project"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    mirv-project_mirv-project
    source/mirv-project.cpp
)
add_library(mirv-project::mirv-project ALIAS mirv-project_mirv-project)

include(GenerateExportHeader)
generate_export_header(
    mirv-project_mirv-project
    BASE_NAME mirv-project
    EXPORT_FILE_NAME export/mirv-project/mirv-project_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(mirv-project_mirv-project PUBLIC MIRV_PROJECT_STATIC_DEFINE)
endif()

set_target_properties(
    mirv-project_mirv-project PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME mirv-project
    OUTPUT_NAME mirv-project
)

target_include_directories(
    mirv-project_mirv-project ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    mirv-project_mirv-project SYSTEM
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(mirv-project_mirv-project PUBLIC cxx_std_20)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${mirv-project_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT mirv-project_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of mirv-project"
  )
endif()

include(cmake/dev-mode.cmake)
