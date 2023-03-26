include_guard()

function(cntp_dotcontemporary_path targetName outputVar)
    set(options REQUIRED)
    cmake_parse_arguments(DOTCONTEMPORARY_PATH "${options}" "" "" ${ARGN})

    get_target_property(CURRENT_DIR ${targetName} SOURCE_DIR)
    while(NOT EXISTS ${CURRENT_DIR}/.contemporary.json)
        set(OLD_CURRENT_DIR ${CURRENT_DIR})
        cmake_path(GET CURRENT_DIR PARENT_PATH CURRENT_DIR)
        IF(${CURRENT_DIR} STREQUAL ${OLD_CURRENT_DIR})
            if(DOTCONTEMPORARY_PATH_REQUIRED)
                message(FATAL_ERROR "No .contemporary.json file found in project")
            else()
                set(${outputVar} "${outputVar}-NOTFOUND" PARENT_SCOPE)
                return()
            endif()
        ENDIF()
    endwhile()

    set(${outputVar} ${CURRENT_DIR}/.contemporary.json PARENT_SCOPE)
endfunction()

function(cntp_dotcontemporary targetName outputVar)
    set(options REQUIRED)
    cmake_parse_arguments(DESKTOP_ID "${options}" "" "" ${ARGN})

    cntp_dotcontemporary_path(${targetName} DOTCONTEMPORARY_FILE)
    if(${DOTCONTEMPORARY_FILE} STREQUAL "DOTCONTEMPORARY_FILE-NOTFOUND")
        if(DESKTOP_ID_REQUIRED)
            message(FATAL_ERROR "No .contemporary.json file found in project")
        else()
            set(${outputVar} "${outputVar}-NOTFOUND" PARENT_SCOPE)
            return()
        endif()
    endif()

    file(READ ${DOTCONTEMPORARY_FILE} DOTCONTEMPORARY)

    string(JSON JSON_OUTPUT ERROR_VARIABLE JSON_ERROR GET ${DOTCONTEMPORARY} ${ARGN})
    if(NOT ${ERROR_VARIABLE} STREQUAL "NOTFOUND")
        if(DESKTOP_ID_REQUIRED)
            message(SEND_ERROR "Could not read .contemporary.json file")
            message(FATAL_ERROR ${ERROR_VARIABLE})
        else()
            set(${outputVar} "${outputVar}-NOTFOUND" PARENT_SCOPE)
            return()
        endif()
    endif()

    set(${outputVar} ${JSON_OUTPUT} PARENT_SCOPE)
endfunction()

function(cntp_dotcontemporary_desktopId targetName outputVar)
    set(options RESOLVE_BLUEPRINT REQUIRED)
    cmake_parse_arguments(DESKTOP_ID "${options}" "" "" ${ARGN})

    cntp_dotcontemporary(${targetName} DESKTOPID "desktop-id")
    if(${DESKTOPID} STREQUAL "DESKTOPID-NOTFOUND")
        if(DESKTOP_ID_REQUIRED)
            message(SEND_ERROR "Could not read .contemporary.json file")
        else()
            set(${outputVar} "${outputVar}-NOTFOUND" PARENT_SCOPE)
            return()
        endif()
    endif()

    if(${DESKTOP_ID_RESOLVE_BLUEPRINT} AND BLUEPRINT)
        set(DESKTOPID "${DESKTOPID}_blueprint")
    endif()

    set(${outputVar} ${DESKTOPID} PARENT_SCOPE)
endfunction()
