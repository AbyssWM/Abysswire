cmake_minimum_required(VERSION 3.19)

file(READ "${CMAKE_SOURCE_DIR}/VERSION" VER_RAW)
string(STRIP ${VER_RAW} ABYSSWIRE_VERSION)

add_compile_definitions(ABYSSWIRE_VERSION="${ABYSSWIRE_VERSION}")

project(
  abysswire
  VERSION ${ABYSSWIRE_VERSION}
  DESCRIPTION "A fast and consistent wire protocol for IPC")

include(CTest)
include(CheckIncludeFile)
include(GNUInstallDirs)

add_subdirectory(scanner)

set(PREFIX ${CMAKE_INSTALL_PREFIX})
set(INCLUDE ${CMAKE_INSTALL_FULL_INCLUDEDIR})
set(LIBDIR ${CMAKE_INSTALL_FULL_LIBDIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(
  deps
  REQUIRED
  IMPORTED_TARGET
  abyssutils>=0.9.0
  libffi
)

configure_file(abysswire.pc.in abysswire.pc @ONLY)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(
  -Wall
  -Wextra
  -Wno-unused-parameter
  -Wno-unused-value
  -Wno-missing-field-initializers
  -Wpedantic)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES DEBUG)
  message(STATUS "Configuring abysswire in Debug")
  add_compile_definitions(ABYSSWIRE_DEBUG)
  set(BUILD_TESTING ON)
else()
  add_compile_options(-O3)
  message(STATUS "Configuring abysswire in Release")
  set(BUILD_TESTING OFF)
endif()

add_compile_definitions(HT_HIDDEN=public)

file(GLOB_RECURSE SRCFILES CONFIGURE_DEPENDS "src/*.cpp" "include/*.hpp")
file(GLOB_RECURSE PUBLIC_HEADERS CONFIGURE_DEPENDS "include/*.hpp")

execute_process(COMMAND ./scripts/generateShaderIncludes.sh
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_library(abysswire SHARED ${SRCFILES})
target_include_directories(
  abysswire
  PUBLIC "./include"
  PRIVATE "./src" "${CMAKE_BINARY_DIR}")
set_target_properties(abysswire PROPERTIES VERSION ${ABYSSWIRE_VERSION} SOVERSION
                                                                      3)
target_link_libraries(abysswire PkgConfig::deps)

check_include_file("sys/timerfd.h" HAS_TIMERFD)
pkg_check_modules(epoll IMPORTED_TARGET epoll-shim)
if(NOT HAS_TIMERFD AND epoll_FOUND)
  target_link_libraries(abysswire PkgConfig::epoll)
endif()

# helper func
function(protocol proto_dir proto_name is_client out_base out_var)
  if(is_client)
    set(mode "--client")
    set(suffix "client")
  else()
    set(mode "")
    set(suffix "server")
  endif()

  set(outdir "${CMAKE_SOURCE_DIR}/tests/generated")
  set(xml_in "${proto_dir}/${proto_name}.xml")

  set(tmp_cpp "${outdir}/${proto_name}-${suffix}.cpp")
  set(tmp_hpp "${outdir}/${proto_name}-${suffix}.hpp")

  set(out_cpp "${outdir}/${out_base}-${suffix}.cpp")
  set(out_hpp "${outdir}/${out_base}-${suffix}.hpp")

  add_custom_command(
    OUTPUT "${out_cpp}" "${out_hpp}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${outdir}"
    COMMAND abysswire-scanner ${mode} "${xml_in}" "${outdir}"
    DEPENDS abysswire-scanner "${xml_in}"
    VERBATIM
    COMMENT
      "Generating ${suffix} protocol from ${proto_name}.xml -> ${out_base}-${suffix}.{cpp,hpp}"
  )

  set(${out_var}
      "${out_cpp};${out_hpp}"
      PARENT_SCOPE)
endfunction()

# tests
if(BUILD_TESTING)
  message(STATUS "building tests is enabled")

  enable_testing()

  # Server + Client tests
  add_custom_target(tests)

  message(STATUS "generating protocols")
  protocol("${CMAKE_SOURCE_DIR}/tests" "protocol-v1" TRUE "test_protocol_v1"
           CLIENT_GEN)
  protocol("${CMAKE_SOURCE_DIR}/tests" "protocol-v1" FALSE "test_protocol_v1"
           SERVER_GEN)
  add_executable(client "${CMAKE_SOURCE_DIR}/tests/Client.cpp"
                        "tests/generated/test_protocol_v1-client.cpp")
  target_link_libraries(client PRIVATE PkgConfig::deps abysswire)
  add_dependencies(tests client)

  add_executable(server "${CMAKE_SOURCE_DIR}/tests/Server.cpp"
                        "tests/generated/test_protocol_v1-server.cpp")
  target_link_libraries(server PRIVATE PkgConfig::deps abysswire)
  add_dependencies(tests server)

  add_executable(fork "${CMAKE_SOURCE_DIR}/tests/Fork.cpp"
                        "tests/generated/test_protocol_v1-client.cpp"
                        "tests/generated/test_protocol_v1-server.cpp")
  target_link_libraries(fork PRIVATE PkgConfig::deps abysswire)
  add_dependencies(tests fork)

  # GTests
  find_package(GTest CONFIG REQUIRED)
  include(GoogleTest)
  file(GLOB_RECURSE TESTFILES CONFIGURE_DEPENDS "tests/unit/*.cpp")
  add_executable(abysswire_tests ${TESTFILES})

  target_compile_options(abysswire_tests PRIVATE --coverage -fsanitize=address)
  target_link_options(abysswire_tests PRIVATE --coverage)
  
  target_include_directories(
    abysswire_tests
    PUBLIC "./include"
    PRIVATE "./src" "./src/include" "./protocols" "${CMAKE_BINARY_DIR}")
  target_link_libraries(abysswire_tests PRIVATE asan abysswire GTest::gtest_main
                                                       PkgConfig::deps)
  gtest_discover_tests(abysswire_tests
    PROPERTIES ENVIRONMENT "ASAN_OPTIONS=detect_leaks=0"
  )

  # Add coverage to abysswire for test builds
  target_compile_options(abysswire PRIVATE --coverage)
  target_link_options(abysswire PRIVATE --coverage)
else()
  message(STATUS "building tests is disabled")
endif()

# Installation
install(TARGETS abysswire)
install(DIRECTORY "include/abysswire" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/abysswire.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
