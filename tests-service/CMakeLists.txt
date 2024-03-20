include(../CreatePart.cmake)

CreateTrivialExe(
        name "test_service"
        path "/service"
)

target_link_libraries(${test_service_exe} PUBLIC ${library_module})

include(UserverTestsuite)

userver_testsuite_add_simple()

include(GNUInstallDirs)
file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${test_service_exe} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${test_service_exe})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${test_service_exe} COMPONENT ${test_service_exe})