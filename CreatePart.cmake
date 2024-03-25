macro(CreateTrivialPart)
    set(oneValueArgs name path)
    set(multiValueArgs "")
    set(options "")
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set (source_regex "${CMAKE_CURRENT_SOURCE_DIR}${ARG_path}/*.cpp")
    set (header_path "${CMAKE_CURRENT_SOURCE_DIR}${ARG_path}")
    FILE(GLOB_RECURSE sources CONFIGURE_DEPENDS ${source_regex})
    set(cur_module ${PROJECT_NAME}_${ARG_name})
    set(${ARG_name}_module ${cur_module})
    IF (sources)
        add_library(${cur_module} ${sources})
        target_include_directories(${cur_module} PUBLIC "${header_path}")
    else ()
        add_library(${cur_module} INTERFACE)
        target_include_directories(${cur_module} INTERFACE "${header_path}")
    ENDIF ()

    unset(cur_module)
    unset(headers_path)
    unset(source_regex)
    unset(oneValueArgs)
    unset(multiValueArgs)
    unset(options)
endmacro()

macro(CreateTrivialExe)
    set(oneValueArgs name path)
    set(multiValueArgs "")
    set(options "")
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set (source_regex "${CMAKE_CURRENT_SOURCE_DIR}${ARG_path}/*.cpp")
    set (header_path "${CMAKE_CURRENT_SOURCE_DIR}${ARG_path}")
    FILE(GLOB_RECURSE sources CONFIGURE_DEPENDS ${source_regex})
    set(cur_exe ${PROJECT_NAME}_${ARG_name})
    set(${ARG_name}_exe ${cur_exe})
    add_executable(${cur_exe} ${sources})
    target_include_directories(${cur_exe} PRIVATE ${header_path})
    unset(cur_exe)
    unset(headers_path)
    unset(source_regex)
    unset(oneValueArgs)
    unset(multiValueArgs)
    unset(options)
endmacro()
