cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
project(openapi-extension-userver)

include(CTest)
include(CreatePart.cmake)
include(InitializeCppMacro.cmake)
enable_testing()

# Adding userver dependency
add_subdirectory(userver)
userver_setup_environment()

CreateTrivialPart(
        name "library"
        path "/library"
)

target_compile_options(${library_module} PRIVATE "-Wno-missing-field-initializers")

target_link_libraries(${library_module} PUBLIC userver-core)

CreateTrivialExe(
    name "unit_tests"
    path "/checks/unit"
)

#set(${CURRENT_PROJECT}_FIXED_STRING_SIZE 128)

InitializeCppMacro(
        target ${library_module}
        name_macro UOPENAPI_FIXED_STRING_SIZE
        variable FIXED_STRING_SIZE
        default 64
)

InitializeCppMacro(
        target ${library_module}
        name_macro UOPENAPI_CREATE_MACROS
        variable CREATE_MACROS
        default 1
)

InitializeCppMacro(
        target ${library_module}
        name_macro UOPENAPI_DEFAULT_REFLECTIVABLE
        variable DEFAULT_REFLECTIVABLE
        default true
)


target_link_libraries(${unit_tests_exe} PUBLIC ${library_module})
target_link_libraries(${unit_tests_exe} PUBLIC userver-utest)
target_compile_options(${unit_tests_exe} PRIVATE "-Wno-missing-field-initializers")

add_google_tests(${unit_tests_exe})

add_subdirectory(checks/service)

unset (library_module)
unset (unit_tests_exe)
unset (test_service_exe)

