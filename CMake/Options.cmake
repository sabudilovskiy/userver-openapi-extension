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
        value 2ab37ab25adf2e9acc1ec31807dd632b02e9adfb
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
        variable SETUP_ENV
        value ON
)

CreateProjectVariable(
        variable COMPILE_OPTIONS_VISIBLE
        value PUBLIC
)
