cmake_minimum_required(VERSION 3.19)

project(
  abysswire-scanner
  DESCRIPTION "A protocol code generator for abysswire"
  VERSION ${ABYSSWIRE_VERSION})

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(PREFIX ${CMAKE_INSTALL_PREFIX})

set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

# configure
set(CMAKE_CXX_STANDARD 23)
add_compile_options(-Wall -Wextra -Wno-unused-parameter -Wno-unused-value
                    -Wno-missing-field-initializers -Wno-narrowing)

add_compile_definitions(SCANNER_VERSION="${ABYSSWIRE_VERSION}")

configure_file(abysswire-scanner.pc.in abysswire-scanner.pc @ONLY)

# dependencies
message(STATUS "Checking deps...")

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET pugixml)

find_library(librt rt)
if("${librt}" MATCHES "librt-NOTFOUND")
  unset(LIBRT)
else()
  set(LIBRT rt)
endif()

add_executable(abysswire-scanner main.cpp)
target_link_libraries(abysswire-scanner PRIVATE ${LIBRT} Threads::Threads
                                               PkgConfig::deps)

target_include_directories(abysswire-scanner
                           PUBLIC "${CMAKE_SOURCE_DIR}/include")

configure_package_config_file(
  abysswire-scanner-config.cmake.in abysswire-scanner-config.cmake
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/abysswire-scanner"
  PATH_VARS CMAKE_INSTALL_BINDIR)
write_basic_package_version_file(
  "abysswire-scanner-config-version.cmake"
  VERSION "${VERSION}" ARCH_INDEPENDENT
  COMPATIBILITY AnyNewerVersion)

# Installation
install(TARGETS abysswire-scanner)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/abysswire-scanner.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/abysswire-scanner-config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/abysswire-scanner-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/abysswire-scanner)
