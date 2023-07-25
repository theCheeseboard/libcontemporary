include_guard()

function(cntp_repoversion outvar)
    set(${outvar}_MAJOR 0 PARENT_SCOPE)
    set(${outvar}_MINOR 0 PARENT_SCOPE)
    set(${outvar}_REVISION 0 PARENT_SCOPE)
    set(${outvar}_BUILD 0 PARENT_SCOPE)

    find_program(CNTP_GIT_COMMAND git)
    if(CNTP_GIT_COMMAND)
        execute_process(COMMAND ${CNTP_GIT_COMMAND} describe --abbrev=0 --tags
            OUTPUT_VARIABLE GIT_DESCRIBE_OUTPUT
            RESULT_VARIABLE GIT_DESCRIBE_RESULT
            OUTPUT_STRIP_TRAILING_WHITESPACE
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
        if(CNTP_GIT_RESULT EQUAL 0)
            execute_process(COMMAND ${CNTP_GIT_COMMAND} rev-list ${GIT_DESCRIBE_OUTPUT}.. --count
                OUTPUT_VARIABLE GIT_REVLIST_OUTPUT
                RESULT_VARIABLE GIT_DESCRIBE_RESULT
                OUTPUT_STRIP_TRAILING_WHITESPACE
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
            if(CNTP_GIT_RESULT EQUAL 0)
                if(GIT_DESCRIBE_OUTPUT MATCHES "v*")
                    string(SUBSTRING ${GIT_DESCRIBE_OUTPUT} 1 -1 GIT_VERSION_STRIPPED)
                    string(REGEX REPLACE "\\." ";" GIT_VERSION_PARTS ${GIT_VERSION_STRIPPED})

                    # Default versions to 0 if they can't be found - the list will always have at least 3 elements
                    list(APPEND GIT_VERSION_PARTS 0 0 0)
                    list(POP_FRONT GIT_VERSION_PARTS ${outvar}_MAJOR ${outvar}_MINOR ${outvar}_REVISION)

                    set(${outvar}_MAJOR ${${outvar}_MAJOR} PARENT_SCOPE)
                    set(${outvar}_MINOR ${${outvar}_MINOR} PARENT_SCOPE)
                    set(${outvar}_REVISION ${${outvar}_REVISION} PARENT_SCOPE)
                    set(${outvar}_BUILD ${GIT_REVLIST_OUTPUT} PARENT_SCOPE)
                else()
                    message("Cannot determine repo version from Git; last tag does not start with v")
                endif()
            else()
                message("Cannot determine repo version from Git; maybe not a Git repository?")
            endif()
        else()
            message("Cannot determine repo version from Git; maybe not a Git repository?")
        endif()
    else()
        message("Cannot determine repo version because Git is not installed")
    endif()
endfunction()
