include_guard()

option(FORCE_BLUEPRINT "Force a Blueprint build (FORCE_STABLE overrides)" OFF)
option(FORCE_STABLE "Force a Stable build (overrides FORCE_BLUEPRINT)" OFF)

if(FORCE_STABLE)
    message("Forcing a Stable build")
elseif(FORCE_BLUEPRINT)
    message("Forcing a Blueprint build")
    set(BLUEPRINT ON)
else()
    find_program(CNTP_GIT_COMMAND git)
    if(CNTP_GIT_COMMAND)
        execute_process(COMMAND ${CNTP_GIT_COMMAND} tag --points-at HEAD
            OUTPUT_VARIABLE CNTP_GIT_OUTPUT
            RESULT_VARIABLE CNTP_GIT_RESULT
            OUTPUT_STRIP_TRAILING_WHITESPACE
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
        if(CNTP_GIT_RESULT EQUAL 0)
            if("${CNTP_GIT_OUTPUT}" STREQUAL "")
                set(BLUEPRINT ON)
            endif()
        else()
            message("Cannot determine Blueprint status from Git; maybe not a Git repository?")
        endif()
    else()
        message("Cannot determine Blueprint status because Git is not installed")
    endif()
endif()

if(BLUEPRINT)
    message("Building as Blueprint")
    add_compile_definitions(T_BLUEPRINT_BUILD)
else()
    message("Building as Stable")
endif()