cmake_minimum_required(VERSION 3.14)
project(project)
set(SOURCE_FILES test.cpp)
set(PROJECT_NAME project)

# Add path for custom modules
set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules
        )

set(STBIN TRUE)
find_package(gmp)
if (GMP_FOUND)
    message(STATUS "Found libgmp")
    include_directories(${GMP_INCLUDE_DIR})
    set(ARITH "gmp" CACHE STRING "")
else()
    set(ARITH "easy" CACHE STRING "")
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_options(${PROJECT_NAME}  PUBLIC -Wl,-no_pie)

set(CMAKE_CXX_STANDARD 11)

set(TIMER "CYCLE" CACHE STRING "")
set(CHECK "off" CACHE STRING "")
set(VERBS "off" CACHE STRING "")
set(ALLOC "AUTO" CACHE STRING "")
set(SHLIB "OFF" CACHE STRING "")
set(MULTI "PTHREAD" CACHE STRING "")
set(DOCUM "off" CACHE STRING "")

set(FP_PRIME 381 CACHE INTEGER "")

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

include(FetchContent)

FetchContent_Declare(
        relic
        GIT_REPOSITORY https://github.com/relic-toolkit/relic.git
        GIT_TAG        origin/master
)
FetchContent_MakeAvailable(relic)

target_include_directories(${PROJECT_NAME}  PUBLIC  ${relic_BINARY_DIR}/include)
target_include_directories(${PROJECT_NAME}  PUBLIC  ${relic_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME}  PUBLIC  ${GMP_INCLUDES})
target_include_directories(${PROJECT_NAME}  PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../contrib/catch)
target_include_directories(${PROJECT_NAME}  PUBLIC  ${INCLUDE_DIRECTORIES})
target_link_directories(${PROJECT_NAME}  PUBLIC ${relic_SOURCE_DIR}/lib)

target_include_directories(${PROJECT_NAME}  PUBLIC bls/src)
target_link_directories(${PROJECT_NAME}  PUBLIC bls/build/src)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} blstmp relic_s pthread sodium gmp)
