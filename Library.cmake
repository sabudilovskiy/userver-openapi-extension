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
