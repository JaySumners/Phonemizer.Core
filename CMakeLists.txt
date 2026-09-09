# ==========================================
# Project
# ==========================================
cmake_minimum_required(VERSION 3.20)

project(
    Phonemizer.Core
    VERSION 0.1.0
    LANGUAGES CXX
)

# ==========================================
# Target
# ==========================================
add_library(Phonemizer.Core INTERFACE)
add_library(Phonemizer::Core ALIAS Phonemizer.Core)

set_target_properties(
    Phonemizer.Core PROPERTIES
    EXPORT_NAME Core
)

# ==========================================
# Compiler Requirements
# ==========================================
target_compile_features(
    Phonemizer.Core
    INTERFACE
        cxx_std_20
)

# ==========================================
# Include Directories
# ==========================================
target_include_directories(
    Phonemizer.Core
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# ==========================================
# Installation
# ==========================================
install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# ==========================================
# CMake Package Export
# ==========================================
install(
    TARGETS Phonemizer.Core
    EXPORT PhonemizerCoreTargets
)

install(
    EXPORT PhonemizerCoreTargets
    FILE PhonemizerCoreTargets.cmake
    NAMESPACE Phonemizer::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Phonemizer
)


# ==========================================
# Package Configuration
# ==========================================
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/PhonemizerConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/PhonemizerConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Phonemizer
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/PhonemizerConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/PhonemizerConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/PhonemizerConfigVersion.cmake
    DESTINATION 
        ${CMAKE_INSTALL_LIBDIR}/cmake/Phonemizer
)


# ==========================================
# Documentation and License
# ==========================================
install(
    FILES
        LICENSE.txt
        README.md
    DESTINATION 
        ${CMAKE_INSTALL_DATADIR}/Phonemizer.Core
)