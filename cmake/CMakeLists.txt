if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

cmake_minimum_required(VERSION 3.20)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

project(libartbase LANGUAGES CXX)

include(ExternalProject)
include(FetchContent)

FetchContent_Declare(libziparchive-ndk
                     GIT_REPOSITORY https://github.com/MaDeG/libziparchive-ndk.git
                     GIT_TAG master
                     GIT_SUBMODULES ""
                     SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(libziparchive-ndk)
FetchContent_GetProperties(libziparchive-ndk)

set(tinyxml2_BUILD_TESTING OFF)
FetchContent_Declare(libtinyxml2
                     GIT_REPOSITORY https://github.com/MaDeG/libtinyxml2-ndk.git
                     GIT_TAG master
                     GIT_SUBMODULES ""
                     SOURCE_SUBDIR .)
FetchContent_MakeAvailable(libtinyxml2)
FetchContent_GetProperties(libtinyxml2)

FetchContent_Declare(libartpalette-ndk
                     GIT_REPOSITORY https://github.com/MaDeG/libartpalette-ndk.git
                     GIT_TAG master
                     GIT_SUBMODULES ""
                     SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(libartpalette-ndk)
FetchContent_GetProperties(libartpalette-ndk)

FetchContent_Declare(libbase-ndk
                     GIT_REPOSITORY https://github.com/MaDeG/libbase-ndk.git
                     GIT_TAG master
                     GIT_SUBMODULES ""
                     SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(libbase-ndk)
FetchContent_GetProperties(libbase-ndk)

FetchContent_Declare(liblog-ndk
                     GIT_REPOSITORY https://github.com/MaDeG/liblog-ndk.git
                     GIT_TAG master
                     GIT_SUBMODULES ""
                     SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(liblog-ndk)
FetchContent_GetProperties(liblog-ndk)

# Used as header only library
FetchContent_Declare(libcap-official
                     GIT_REPOSITORY https://android.googlesource.com/platform/external/libcap.git
                     GIT_TAG fbfa21662ea4478f4b663a96407f51b8babe4883
                     GIT_SUBMODULES ""
                     SOURCE_SUBDIR .)
FetchContent_MakeAvailable(libcap-official)
FetchContent_GetProperties(libcap-official)

set(LIBARTBASE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../)

set(ANDROID_SOURCES ${LIBARTBASE_ROOT}/android-base/errno_restorer.h
                    ${LIBARTBASE_ROOT}/android-base/file.cpp
                    ${LIBARTBASE_ROOT}/android-base/file.h
                    ${LIBARTBASE_ROOT}/android-base/log_main.h
                    ${LIBARTBASE_ROOT}/android-base/logging.h
                    ${LIBARTBASE_ROOT}/android-base/macros.h
                    ${LIBARTBASE_ROOT}/android-base/off64_t.h
                    ${LIBARTBASE_ROOT}/android-base/parseint.h
                    ${LIBARTBASE_ROOT}/android-base/stringprintf.cpp
                    ${LIBARTBASE_ROOT}/android-base/stringprintf.h
                    ${LIBARTBASE_ROOT}/android-base/strings.cpp
                    ${LIBARTBASE_ROOT}/android-base/strings.h
                    ${LIBARTBASE_ROOT}/android-base/threads.h
                    ${LIBARTBASE_ROOT}/android-base/thread_annotations.h
                    ${LIBARTBASE_ROOT}/android-base/unique_fd.h
                    ${LIBARTBASE_ROOT}/android-base/utf8.h
)

set(LIBARTBASE_SOURCES ${LIBARTBASE_ROOT}/arch/instruction_set.cc
                       ${LIBARTBASE_ROOT}/arch/instruction_set.operator_out.cc
                       ${LIBARTBASE_ROOT}/base/allocator.cc
                       ${LIBARTBASE_ROOT}/base/allocator.operator_out.cc
                       ${LIBARTBASE_ROOT}/base/arena_allocator.cc
                       ${LIBARTBASE_ROOT}/base/arena_bit_vector.cc
                       ${LIBARTBASE_ROOT}/base/bit_vector.cc
                       ${LIBARTBASE_ROOT}/base/compiler_filter.cc
                       ${LIBARTBASE_ROOT}/base/enums.cc
                       ${LIBARTBASE_ROOT}/base/file_magic.cc
                       ${LIBARTBASE_ROOT}/base/file_utils.cc
                       ${LIBARTBASE_ROOT}/base/flags.cc
                       ${LIBARTBASE_ROOT}/base/hex_dump.cc
                       ${LIBARTBASE_ROOT}/base/logging.cc
                       ${LIBARTBASE_ROOT}/base/malloc_arena_pool.cc
                       ${LIBARTBASE_ROOT}/base/membarrier.cc
                       ${LIBARTBASE_ROOT}/base/memfd.cc
                       ${LIBARTBASE_ROOT}/base/memory_region.cc
                       ${LIBARTBASE_ROOT}/base/mem_map.cc
                       ${LIBARTBASE_ROOT}/base/metrics/metrics_common.cc
                       ${LIBARTBASE_ROOT}/base/os_linux.cc
                       ${LIBARTBASE_ROOT}/base/runtime_debug.cc
                       ${LIBARTBASE_ROOT}/base/safe_copy.cc
                       ${LIBARTBASE_ROOT}/base/scoped_arena_allocator.cc
                       ${LIBARTBASE_ROOT}/base/scoped_flock.cc
                       ${LIBARTBASE_ROOT}/base/socket_peer_is_trusted.cc
                       ${LIBARTBASE_ROOT}/base/time_utils.cc
                       ${LIBARTBASE_ROOT}/base/unix_file/fd_file.cc
                       ${LIBARTBASE_ROOT}/base/unix_file/fd_file.operator_out.cc
                       ${LIBARTBASE_ROOT}/base/unix_file/random_access_file_utils.cc
                       ${LIBARTBASE_ROOT}/base/utils.cc
                       ${LIBARTBASE_ROOT}/base/zip_archive.cc
                       ${LIBARTBASE_ROOT}/base/globals_unix.cc
                       ${LIBARTBASE_ROOT}/base/zip_archive.cc
                       ${LIBARTBASE_ROOT}/base/mem_map_unix.cc
)

add_library(artbase STATIC ${ANDROID_SOURCES} ${LIBARTBASE_SOURCES})
target_include_directories(artbase PUBLIC ${LIBARTBASE_ROOT})
target_include_directories(artbase PRIVATE ${libziparchive-ndk_SOURCE_DIR}/include
                                           ${tinyxml2_SOURCE_DIR}
                                           ${libartpalette-ndk_SOURCE_DIR}/include
                                           ${libbase-ndk_SOURCE_DIR}/include
                                           ${liblog-ndk_SOURCE_DIR}/include
                                           ${libcap-official_SOURCE_DIR}/libcap/include
)
target_link_libraries(artbase log z tinyxml2 ziparchive artpalette base log-ndk)
add_compile_definitions(artbase ART_STACK_OVERFLOW_GAP_arm=8192
                                ART_STACK_OVERFLOW_GAP_arm64=8192
                                ART_STACK_OVERFLOW_GAP_riscv64=8192
                                ART_STACK_OVERFLOW_GAP_x86=8192
                                ART_STACK_OVERFLOW_GAP_x86_64=8192
                                ART_NDEBUG_OPT_FLAG=-O3
                                ART_DEFAULT_GC_TYPE=CMC
                                ART_DEFAULT_COMPACT_DEX_LEVEL=fast
                                ART_FRAME_SIZE_LIMIT=7400
                                ART_BASE_ADDRESS=32768
)

set_property(TARGET artbase PROPERTY CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(artbase PRIVATE $<BUILD_INTERFACE:-Wno-unknown-attributes>)
endif()
