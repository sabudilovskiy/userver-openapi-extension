cmake_minimum_required(VERSION 3.20)

project(openapi-extension-userver)
set(CMAKE_CXX_STANDARD 20)

include(CTest)
include(CMake/CreatePart.cmake)
include(CMake/CreateVariable.cmake)
include(CMake/InitializeCppMacro.cmake)
include(CMake/Options.cmake)

include(FetchContent)
FetchContent_Declare(
        userver
        GIT_REPOSITORY https://github.com/userver-framework/userver
        GIT_TAG ${${PROJECT_NAME}_USERVER_VERSION}
)
FetchContent_MakeAvailable(userver)

enable_testing()

if (${PROJECT_NAME}_SETUP_ENV)
    message(STATUS "UOPENAPI set env: ${${PROJECT_NAME}_SETUP_ENV}")
    userver_setup_environment()
endif()

include(Library.cmake)

if (${PROJECT_NAME}_BUILD_UNIT_TESTS)
    message(STATUS "UOPENAPI BUILD UNIT TESTS : enabled")
    add_subdirectory(unit_tests)
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


