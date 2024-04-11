cmake_minimum_required(VERSION 3.20)

project(openapi-extension-userver)
set(CMAKE_CXX_STANDARD 20)

include(CTest)
include(CMake/CreatePart.cmake)
include(CMake/InitializeCppMacro.cmake)
include(CMake/CreateVariable.cmake)

CreateVariable(
        variable USERVER_VERSION
        value 72408adfccf2b68da3ad58b7a1cf29c8e51676c3
)

if (NOT DEFINED USERVER_FEATURE_POSTGRESQL)
    set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)
endif ()
if (NOT DEFINED USERVER_FEATURE_PATCH_LIBPQ)
    set(USERVER_FEATURE_PATCH_LIBPQ OFF CACHE BOOL "" FORCE)
endif ()

include(FetchContent)
FetchContent_Declare(
        userver
        GIT_REPOSITORY https://github.com/userver-framework/userver
        GIT_TAG ${${PROJECT_NAME}_USERVER_VERSION}
)
FetchContent_MakeAvailable(userver)

enable_testing()

#size of constexpr strings
CreateVariable(
        variable FIXED_STRING_SIZE
        value 64
)

#declare macros or not
CreateVariable(
        variable CREATE_MACROS
        value 1
)

#Should aggregate types be considered reflective
CreateVariable(
        variable DEFAULT_REFLECTIVABLE
        value 1
)

CreateVariable(
        variable BUILD_UNIT_TESTS
        value 1
)

CreateVariable(
        variable BUILD_TEST_SERVICES
        value 1
)

CreateVariable(
        variable COMPILE_OPTIONS_VISIBLE
        value PUBLIC
)

userver_setup_environment()

CreateTrivialPart(
        name "library"
        path "/library"
)

target_compile_options(${library_module} ${${PROJECT_NAME}_COMPILE_OPTIONS_VISIBLE} "-Wno-missing-field-initializers")
target_link_libraries(${library_module} PUBLIC "userver-core" "userver-postgresql")

InitializeCppMacro(
        target ${library_module}
        name_macro UOPENAPI_FIXED_STRING_SIZE
        variable FIXED_STRING_SIZE
)

InitializeCppMacro(
        target ${library_module}
        name_macro UOPENAPI_CREATE_MACROS
        variable CREATE_MACROS
)

InitializeCppMacro(
        target ${library_module}
        name_macro UOPENAPI_DEFAULT_REFLECTIVABLE
        variable DEFAULT_REFLECTIVABLE
)

if (${PROJECT_NAME}_BUILD_UNIT_TESTS)
    message(STATUS "UOPENAPI BUILD UNIT TESTS : enabled")
    CreateTrivialExe(
            name "unit_tests"
            path "/unit_tests"
    )
    target_link_libraries(${unit_tests_exe} PUBLIC ${library_module})
    target_link_libraries(${unit_tests_exe} PUBLIC userver-utest)

    add_google_tests(${unit_tests_exe})
    unset (unit_tests_exe)
else ()
    message(STATUS "UOPENAPI BUILD UNIT TESTS : disabled")
endif ()

if (${PROJECT_NAME}_BUILD_TEST_SERVICES)
    message(STATUS "UOPENAPI BUILD TEST SERVICES : enabled")
    add_subdirectory(examples)
    unset (test_service_exe)
else ()
    message(STATUS "UOPENAPI BUILD TEST SERVICES : disabled")
endif ()

unset (library_module)


