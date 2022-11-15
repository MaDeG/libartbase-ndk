cmake_minimum_required(VERSION 3.10)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    include("./CMakeListsConan.txt")
endif()

project(libartbase LANGUAGES CXX)

include(FetchContent)

find_package(tinyxml2 MODULE REQUIRED)

FetchContent_Declare(libziparchive-ndk
                     GIT_REPOSITORY git@github.com:MaDeG/libziparchive-ndk.git
                     GIT_TAG integration
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
