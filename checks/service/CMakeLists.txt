project(check_service)
set(CMAKE_CXX_STANDARD 20)

include(../../CreatePart.cmake)

add_executable(${PROJECT_NAME} main/main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ${library_module})

include(UserverTestsuite)

userver_testsuite_add_simple(
        SERVICE_TARGET ${PROJECT_NAME}
)

unset(check_service_exe)