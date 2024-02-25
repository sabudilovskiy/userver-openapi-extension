cmake_minimum_required(VERSION 3.27)
include(CreatePart.cmake)
include(InitializeCppMacro.cmake)
set(CMAKE_CXX_STANDARD 20)
project(openapi-extension-userver)

# Adding userver dependency
add_subdirectory(userver)
userver_setup_environment()

CreateTrivialPart(
        name "library"
        path "/library"
)

target_link_libraries(${library_module} PUBLIC userver-core)

CreateTrivialExe(
    name "unit_tests"
    path "/tests-unit"
)

set(${CURRENT_PROJECT}_FIXED_STRING_SIZE 128)

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

add_google_tests(${unit_tests_exe})

unset (library_module)
unset (unit_tests_exe)

