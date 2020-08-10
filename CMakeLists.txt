cmake_minimum_required(VERSION 3.10)
project(project)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES test.cpp)

set(sodium_USE_STATIC_LIBS TRUE)
find_package(sodium)
if (SODIUM_FOUND)
    message(STATUS "Found libsodium")
    message(STATUS "Sodium include dir = ${sodium_INCLUDE_DIR}")
    set(BLSALLOC_SODIUM "1" CACHE STRING "")
    set(SODIUM_STATIC "1" CACHE STRING "")
    include_directories(${sodium_INCLUDE_DIR})
endif()

set(STBIN TRUE)
find_package(gmp)
if (GMP_FOUND)
    message(STATUS "Found libgmp")
    include_directories(${GMP_INCLUDE_DIR})
    set(ARITH "gmp" CACHE STRING "")
else()
    set(ARITH "easy" CACHE STRING "")
endif()

set(TIMER "CYCLE" CACHE STRING "")
set(CHECK "off" CACHE STRING "")
set(VERBS "off" CACHE STRING "")
set(ALLOC "AUTO" CACHE STRING "")
set(SHLIB "OFF" CACHE STRING "")
set(MULTI "PTHREAD" CACHE STRING "")
set(DOCUM "off" CACHE STRING "")

if(EMSCRIPTEN)
    # emscripten needs arch set to be none since it can't compile assembly
    set(ARCH "" CACHE STRING "")
    # emscripten is a 32 bit compiler
    set(WSIZE 32 CACHE INTEGER "")
else()
    set(WSIZE 64 CACHE INTEGER "")
endif()

IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(DSEED "UDEV" CACHE STRING "")
    set(FP_QNRES "off" CACHE STRING "")
ELSEIF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(SEED "WCGR" CACHE STRING "")
    set(FP_QNRES "on" CACHE STRING "")
ELSE()
    set(DSEED "UDEV" CACHE STRING "")
    set(FP_QNRES "on" CACHE STRING "")
ENDIF()
set(STBIN "OFF" CACHE STRING "")

set(FP_METHD "INTEG;INTEG;INTEG;MONTY;LOWER;SLIDE" CACHE STRING "")
set(COMP "-O3 -funroll-loops -fomit-frame-pointer" CACHE STRING "")
set(FP_PMERS "off" CACHE STRING "")
set(FPX_METHD "INTEG;INTEG;LAZYR" CACHE STRING "")
set(EP_PLAIN "off" CACHE STRING "")
set(EP_SUPER "off" CACHE STRING "")
# Disable relic tests and benchmarks
set(TESTS 0 CACHE INTEGER "")
set(BENCH 0 CACHE INTEGER "")
set(QUIET 1 CACHE INTEGER "")

set(PP_EXT "LAZYR" CACHE STRING "")
set(PP_METHD "LAZYR;OATEP" CACHE STRING "")

# CMake 3.14+
include(FetchContent)

FetchContent_Declare(
        relic
        GIT_REPOSITORY https://github.com/relic-toolkit/relic.git
        GIT_TAG        origin/master
)
FetchContent_MakeAvailable(relic)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
source_group("SrcHeaders" FILES ${HEADERS})

include_directories(
        ${INCLUDE_DIRECTORIES}
        ${relic_SOURCE_DIR}/include
        ${relic_BINARY_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../contrib/catch
        ${GMP_INCLUDES}
)

INCLUDE_DIRECTORIES(bls/src)
LINK_DIRECTORIES(bls/build/src)

set(OPREFIX object_)
find_library(GMP_NAME NAMES libgmp.a gmp)
find_library(SODIUM_NAME NAMES libsodium.a sodium)
find_library(BLS_NAME NAMES libbls.a bls)
find_library(BLS_TEMP_NAME NAMES libblstmp.a blstmp)

if (GMP_FOUND)
    list(APPEND LIBRARIES_TO_COMBINE COMMAND mkdir ${OPREFIX}gmp || true && cd ${OPREFIX}gmp &&  ${CMAKE_AR} -x ${GMP_NAME})
endif()
if (SODIUM_FOUND)
    message("SODIUM_FOUND in src/CMakeLists.txt")
    list(APPEND LIBRARIES_TO_COMBINE COMMAND mkdir ${OPREFIX}sodium || true && cd ${OPREFIX}sodium &&  ${CMAKE_AR} -x ${SODIUM_NAME})
endif()

add_executable(project ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(project blstmp relic_s sodium)