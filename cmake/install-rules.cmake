if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/mirv-project-${PROJECT_VERSION}"
      CACHE PATH ""
  )
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package mirv-project)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT mirv-project_Development
)

install(
    TARGETS mirv-project_mirv-project
    EXPORT mirv-projectTargets
    RUNTIME #
    COMPONENT mirv-project_Runtime
    LIBRARY #
    COMPONENT mirv-project_Runtime
    NAMELINK_COMPONENT mirv-project_Development
    ARCHIVE #
    COMPONENT mirv-project_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    mirv-project_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(mirv-project_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${mirv-project_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT mirv-project_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${mirv-project_INSTALL_CMAKEDIR}"
    COMPONENT mirv-project_Development
)

install(
    EXPORT mirv-projectTargets
    NAMESPACE mirv-project::
    DESTINATION "${mirv-project_INSTALL_CMAKEDIR}"
    COMPONENT mirv-project_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
