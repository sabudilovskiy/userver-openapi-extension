macro(CreateProjectVariable)
    set(oneValueArgs variable value)
    set(multiValueArgs "")
    set(options "")
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    IF(NOT DEFINED ${CMAKE_PROJECT_NAME}_${ARG_variable})
        IF (DEFINED ARG_value)
            SET(${CMAKE_PROJECT_NAME}_${ARG_variable} ${ARG_value})
        endif ()
    endif ()

    unset(oneValueArgs)
    unset(multiValueArgs)
    unset(options)
endmacro()

macro(CreateGlobalVariable)
    set(oneValueArgs variable value)
    set(multiValueArgs "")
    set(options "")
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    IF(NOT DEFINED ${ARG_variable})
        IF (DEFINED ARG_value)
            SET(${ARG_variable} ${ARG_value})
        endif ()
    endif ()

    unset(oneValueArgs)
    unset(multiValueArgs)
    unset(options)
endmacro()

