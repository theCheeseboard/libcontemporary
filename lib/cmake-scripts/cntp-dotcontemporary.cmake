function(cntp_dotcontemporary_path targetName outputVar)
    get_target_property(CURRENT_DIR ${targetName} SOURCE_DIR)
    while(NOT EXISTS ${CURRENT_DIR}/.contemporary.json)
        IF(${CURRENT_DIR} STREQUAL "/")
            message(FATAL_ERROR "No .contemporary.json file found in project")
        ENDIF()
        cmake_path(GET CURRENT_DIR PARENT_PATH CURRENT_DIR)
    endwhile()

    set(${outputVar} ${CURRENT_DIR}/.contemporary.json PARENT_SCOPE)
endfunction()

function(cntp_dotcontemporary targetName outputVar)
    cntp_dotcontemporary_path(${targetName} DOTCONTEMPORARY_FILE)
    file(READ ${DOTCONTEMPORARY_FILE} DOTCONTEMPORARY)

    string(JSON JSON_OUTPUT ERROR_VARIABLE JSON_ERROR GET ${DOTCONTEMPORARY} ${ARGN})
    if(NOT ${ERROR_VARIABLE} STREQUAL "NOTFOUND")
        message(SEND_ERROR "Could not read .contemporary.json file")
        message(FATAL_ERROR ${ERROR_VARIABLE})
    endif()

    set(${outputVar} ${JSON_OUTPUT} PARENT_SCOPE)
endfunction()

function(cntp_dotcontemporary_desktopId targetName outputVar)
    set(options RESOLVE_BLUEPRINT)
    cmake_parse_arguments(DESKTOP_ID "${options}" "" "" ${ARGN})

    cntp_dotcontemporary(${targetName} DESKTOPID "desktop-id")

    if(${DESKTOP_ID_RESOLVE_BLUEPRINT})
        set(DESKTOPID "${DESKTOPID}_blueprint")
    endif()

    set(${outputVar} ${DESKTOPID} PARENT_SCOPE)
endfunction()
