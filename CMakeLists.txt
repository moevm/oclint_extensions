CMAKE_MINIMUM_REQUIRED(VERSION 2.13.4)
PROJECT(OCLINT_EXTENSIONS)

SET(LLVM_ROOT ${CMAKE_SOURCE_DIR}/oclint/build/llvm-install)

SET(CMAKE_DISABLE_SOURCE_CHANGES ON)
SET(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_MACOSX_RPATH ON)
SET(CMAKE_BUILD_TYPE None)

IF(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    SET(CMAKE_CXX_FLAGS "-fcolor-diagnostics")
ENDIF()
SET(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_LINKER_FLAGS} -fno-rtti -fPIC ${CMAKE_CXX_FLAGS}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_LINKER_FLAGS} -fno-rtti")

IF(OCLINT_BUILD_TYPE STREQUAL "Release")
    SET(CMAKE_CXX_FLAGS "-O3 -DNDEBUG ${CMAKE_CXX_FLAGS}")
    SET(CMAKE_SHARED_LINKER_FLAGS "-s ${CMAKE_SHARED_LINKER_FLAGS}")
ELSE()
    SET(CMAKE_CXX_FLAGS "-O0 -g ${CMAKE_CXX_FLAGS}")
    SET(CMAKE_SHARED_LINKER_FLAGS "-g ${CMAKE_SHARED_LINKER_FLAGS}")
ENDIF()

SET(OCLINT_VERSION_RELEASE "22.02")

IF(NOT EXISTS ${LLVM_ROOT}/include/llvm)
    MESSAGE(FATAL_ERROR "LLVM_ROOT (${LLVM_ROOT}) is not a valid LLVM install. Could not find ${LLVM_ROOT}/include/llvm")
ENDIF()
MESSAGE("LLVM_ROOT: ${LLVM_ROOT}")
IF(EXISTS ${LLVM_ROOT}/lib/cmake/llvm)
    SET(LLVM_DIR ${LLVM_ROOT}/lib/cmake/llvm)
ELSE()
    SET(LLVM_DIR ${LLVM_ROOT}/share/llvm/cmake)
ENDIF()
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LLVM_DIR}")
INCLUDE(LLVMConfig)

INCLUDE_DIRECTORIES( ${LLVM_INCLUDE_DIRS} )
LINK_DIRECTORIES( ${LLVM_LIBRARY_DIRS} )
ADD_DEFINITIONS( ${LLVM_DEFINITIONS} )

STRING(REGEX MATCH "[0-9]+\\.[0-9]+(\\.[0-9]+)?" LLVM_VERSION_RELEASE ${LLVM_PACKAGE_VERSION})

MESSAGE(STATUS "Found LLVM LLVM_PACKAGE_VERSION: ${LLVM_PACKAGE_VERSION} - LLVM_VERSION_RELEASE: ${LLVM_VERSION_RELEASE}")
MESSAGE(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
LLVM_MAP_COMPONENTS_TO_LIBNAMES(REQ_LLVM_LIBRARIES asmparser bitreader instrumentation mcparser option support frontendopenmp)

SET(CLANG_LIBRARIES
    clangToolingCore
    clangTooling
    clangFrontend
    clangDriver
    clangSerialization
    clangParse
    clangSema
    clangAnalysis
    clangEdit
    clangASTMatchers
    clangAST
    clangLex
    clangBasic)

IF(TEST_BUILD)
    ENABLE_TESTING()
    IF(NOT APPLE)
        ADD_DEFINITIONS(
            --coverage
            )
    ENDIF()

    INCLUDE_DIRECTORIES(
        ${GOOGLETEST_SRC}/googlemock/include
        ${GOOGLETEST_SRC}/googletest/include
        )
    LINK_DIRECTORIES(
        ${GOOGLETEST_BUILD}
        ${GOOGLETEST_BUILD}/lib
        )
    SET(GTEST_LIBS gmock gtest)

    # Find CUDA
    FIND_PROGRAM(NVIDIA_NVCC_BIN "nvcc")
    IF (NVIDIA_NVCC_BIN)
        MESSAGE(STATUS "Enable tests for CUDA rules.")
        SET(TEST_CUDA TRUE)
    ELSE()
        SET(TEST_CUDA FALSE)
    ENDIF()

    # Setup the path for profile_rt library
    STRING(TOLOWER ${CMAKE_SYSTEM_NAME} COMPILER_RT_SYSTEM_NAME)
    LINK_DIRECTORIES(${LLVM_LIBRARY_DIRS}/clang/${LLVM_VERSION_RELEASE}/lib/${COMPILER_RT_SYSTEM_NAME})
    IF(APPLE)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    ELSEIF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64")
        SET(PROFILE_RT_LIBS clang_rt.profile-aarch64 --coverage)
    ELSE()
        SET(PROFILE_RT_LIBS clang_rt.profile-x86_64 --coverage)
    ENDIF()
ENDIF()

IF(DOC_GEN_BUILD)
    SET(CMAKE_CXX_FLAGS "-DDOCGEN ${CMAKE_CXX_FLAGS}")
ENDIF()




INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/oclint/oclint-core/include
    ${CMAKE_SOURCE_DIR}/oclint/oclint-metrics/include
    ${CMAKE_SOURCE_DIR}/oclint/oclint-rules/include
    ${CMAKE_SOURCE_DIR}/oclint/oclint-rules
    )
LINK_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/oclint/build/oclint-core/lib
    ${CMAKE_SOURCE_DIR}/oclint/build/oclint-metrics/lib
    ${CMAKE_SOURCE_DIR}/oclint/build/oclint-rules/lib/helper
    ${CMAKE_SOURCE_DIR}/oclint/build/oclint-rules/lib/util
    ${CMAKE_SOURCE_DIR}/oclint/build/oclint-rules/lib
    )

MACRO(build_dynamic_rule name)
    ADD_LIBRARY(${name}Rule SHARED ${CMAKE_SOURCE_DIR}/rules/${name}Rule.cpp)

    #SET_TARGET_PROPERTIES(${name}Rule
    #    PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
    #               RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
    #    )

    TARGET_LINK_LIBRARIES(${name}Rule OCLintAbstractRule)

    TARGET_LINK_LIBRARIES(${name}Rule
        clangASTMatchers
    )

    TARGET_LINK_LIBRARIES(${name}Rule
        OCLintMetric
        OCLintHelper
        OCLintUtil
        OCLintCore
    )
    install(TARGETS ${name}Rule DESTINATION lib/oclint/rules)
ENDMACRO(build_dynamic_rule)

BUILD_DYNAMIC_RULE(TooLongIfSequence)
BUILD_DYNAMIC_RULE(MallocSizeof)
