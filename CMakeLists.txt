# ==========================================
# Project
# ==========================================
cmake_minimum_required(VERSION 3.20)

project(
    TTS.Core
    VERSION 0.1.0
    LANGUAGES CXX
)

# ==========================================
# Target
# ==========================================
add_library(TTS.Core INTERFACE)
add_library(TTS::Core ALIAS TTS.Core)

set_target_properties(
    TTS.Core PROPERTIES
    EXPORT_NAME Core
)

# ==========================================
# Compiler Requirements
# ==========================================
target_compile_features(
    TTS.Core
    INTERFACE
        cxx_std_20
)

# ==========================================
# Include Directories
# ==========================================
target_include_directories(
    TTS.Core
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
    TARGETS TTS.Core
    EXPORT TTSCoreTargets
)

install(
    EXPORT TTSCoreTargets
    FILE TTSCoreTargets.cmake
    NAMESPACE TTS::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TTS
)


# ==========================================
# Package Configuration
# ==========================================
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/TTSConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/TTSConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TTS
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/TTSConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/TTSConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/TTSConfigVersion.cmake
    DESTINATION 
        ${CMAKE_INSTALL_LIBDIR}/cmake/TTS
)


# ==========================================
# Documentation and License
# ==========================================
install(
    FILES
        LICENSE.txt
        README.md
    DESTINATION 
        ${CMAKE_INSTALL_DATADIR}/TTS.Core
)