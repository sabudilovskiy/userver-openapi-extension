CreateGlobalVariable(
        variable USERVER_FEATURE_POSTGRESQL
        value ON
)
CreateGlobalVariable(
        variable USERVER_FEATURE_PATCH_LIBPQ
        value OFF
)

CreateProjectVariable(
        variable USERVER_VERSION
        value 690596421c5278d06a6ce430c60c65586ec031f8
)

#size of constexpr strings
CreateProjectVariable(
        variable FIXED_STRING_SIZE
        value 64
)

#declare macros or not
CreateProjectVariable(
        variable CREATE_MACROS
        value 1
)

#Should aggregate types be considered reflective
CreateProjectVariable(
        variable DEFAULT_REFLECTIVABLE
        value 1
)

CreateProjectVariable(
        variable BUILD_UNIT_TESTS
        value 1
)

CreateProjectVariable(
        variable BUILD_TEST_SERVICES
        value 1
)

CreateProjectVariable(
        variable COMPILE_OPTIONS_VISIBLE
        value PUBLIC
)
