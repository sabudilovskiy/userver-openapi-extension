cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
project(openapi-extension-userver)

include(CTest)
include(CMake/CreatePart.cmake)
include(CMake/InitializeCppMacro.cmake)
include(CMake/CreateVariable.cmake)
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


add_subdirectory(userver)
userver_setup_environment()

CreateTrivialPart(
        name "library"
        path "/library"
)

target_compile_options(${library_module} PRIVATE "-Wno-missing-field-initializers")
target_link_libraries(${library_module} PUBLIC userver-core)

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
            path "/checks/unit"
    )
    target_link_libraries(${unit_tests_exe} PUBLIC ${library_module})
    target_link_libraries(${unit_tests_exe} PUBLIC userver-utest)
    target_compile_options(${unit_tests_exe} PRIVATE "-Wno-missing-field-initializers")

    add_google_tests(${unit_tests_exe})
    unset (unit_tests_exe)
else ()
    message(STATUS "UOPENAPI BUILD UNIT TESTS : disabled")
endif ()

if (${PROJECT_NAME}_BUILD_TEST_SERVICES)
    message(STATUS "UOPENAPI BUILD TEST SERVICES : enabled")
    add_subdirectory(checks/service)
    unset (test_service_exe)
else ()
    message(STATUS "UOPENAPI BUILD TEST SERVICES : disabled")
endif ()

unset (library_module)


