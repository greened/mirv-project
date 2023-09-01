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
