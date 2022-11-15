project(libartbase LANGUAGES C CXX ASM)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake"
            TLS_VERIFY ON)
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES tinyxml2/9.0.0
                               libarchive/3.6.1
                      OPTIONS tinyxml2:shared=False
                      GENERATORS cmake_find_package
                                 virtualenv
)

conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
                    PROFILE_HOST ../armv8.profile
                    PROFILE_BUILD ../build.profile
                    BUILD missing)

# Set conan environment variables generated using the virtualenv generator
# This is necessary since for some reason Conan does not correctly set the variables found (e.g. the Android NDK compiler path)
# A particular attention should be paid to the PATH variable since it is not good to completely overwrite it
if(EXISTS "${CMAKE_BINARY_DIR}/environment.sh.env")
    FILE(READ "${CMAKE_BINARY_DIR}/environment.sh.env" _contents)
    STRING(REGEX REPLACE "\n" ";" _contents "${_contents}")
    foreach(_line ${_contents})
        string(REGEX MATCH "([^=]+)=\"(.*)\"" _match ${_line})
        if(NOT "${_match}" STREQUAL "" AND NOT "${CMAKE_MATCH_1}" STREQUAL "PATH")
            #message("in ${CMAKE_MATCH_1} = ${CMAKE_MATCH_2}")
            set(ENV{${CMAKE_MATCH_1}} "${CMAKE_MATCH_2}")
        endif()
    endforeach()
    unset(_contents)
    unset(_line)
    unset(_match)
else()
    message(FATAL_ERROR "Android NDK environment variables not found!")
endif()

set(CMAKE_C_COMPILER   $ENV{CC})
set(CMAKE_CXX_COMPILER $ENV{CXX})
set(CMAKE_CXX_COMPILER_AR $ENV{AR})
set(CMAKE_CXX_COMPILER_RANLIB $ENV{RANLIB})
set(CMAKE_AR           $ENV{AR})
set(CMAKE_AS           $ENV{AS})
set(CMAKE_RANLIB       $ENV{RANLIB})
set(CMAKE_STRIP        $ENV{STRIP})
set(CMAKE_ADDR2LINE    $ENV{ADDR2LINE})
set(CMAKE_NM           $ENV{NM})
set(CMAKE_OBJCOPY      $ENV{OBJCOPY})
set(CMAKE_OBJDUMP      $ENV{OBJDUMP})
set(CMAKE_READELF      $ENV{READELF})
set(CMAKE_LD           $ENV{LD})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_BINARY_DIR})

find_package(tinyxml2 MODULE REQUIRED)

FetchContent_Declare(libziparchive-ndk
                     GIT_REPOSITORY https://github.com/MaDeG/libziparchive-ndk.git
                     GIT_TAG master
                     GIT_SUBMODULES ""
                     SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(libziparchive-ndk)
FetchContent_GetProperties(libziparchive-ndk)

set(LIBARTBASE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../)

set(ANDROID_SOURCES
    ${LIBARTBASE_ROOT}/android-base/errno_restorer.h
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

set(LIBARTBASE_SOURCES
    ${LIBARTBASE_ROOT}/arch/instruction_set.cc
    ${LIBARTBASE_ROOT}/base/allocator.cc
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
    ${LIBARTBASE_ROOT}/base/unix_file/random_access_file_utils.cc
    ${LIBARTBASE_ROOT}/base/utils.cc
    ${LIBARTBASE_ROOT}/base/zip_archive.cc
)

add_library(LIBARTBASE STATIC ${ANDROID_SOURCES} ${LIBARTBASE_SOURCES})
target_include_directories(LIBARTBASE PUBLIC ${LIBARTBASE_ROOT} ${tinyxml2_INCLUDE_DIRS})
target_include_directories(LIBARTBASE PRIVATE ${libziparchive-ndk_SOURCE_DIR}/include)
target_link_libraries(LIBARTBASE TinyXML)
add_compile_definitions(ART_STACK_OVERFLOW_GAP_arm=8192
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

set_property(TARGET LIBARTBASE PROPERTY CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(LIBARTBASE PRIVATE $<BUILD_INTERFACE:-Wno-unknown-attributes>)
endif()
