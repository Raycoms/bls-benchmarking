cmake_minimum_required(VERSION 3.10)
project(project)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES test.cpp)

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