CreateTrivialExe(
        name "unit_tests"
        path ""
)
target_link_libraries(${unit_tests_exe} PUBLIC ${library_module})
target_link_libraries(${unit_tests_exe} PUBLIC userver-utest)

add_google_tests(${unit_tests_exe})
unset (unit_tests_exe)
