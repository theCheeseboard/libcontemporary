function(cntp_app_icon targetName)
    set(singleValueArgs BASEICON BASEICON_MAC)
    cmake_parse_arguments(APP_ICON "" "${singleValueArgs}" "" ${ARGN})

    IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        find_path(CNTPAPPICONTOOL_PATH cntp-appicontool.exe
            PATHS ${CMAKE_INSTALL_FULL_BINDIR}
            HINTS ${CMAKE_CURRENT_LIST_DIR}/../../../../bin)
    ELSE()
        find_path(CNTPAPPICONTOOL_PATH cntp-appicontool
            PATHS ${CMAKE_INSTALL_FULL_BINDIR})
    ENDIF()


    IF(${CNTPAPPICONTOOL_PATH} STREQUAL "CNTPAPPICONTOOL_PATH-NOTFOUND")
        message(FATAL_ERROR "cntp-appicontool executable not available, app icon cannot be created")
    ELSE()

        get_filename_component(BASEICON_PATH "${APP_ICON_BASEICON}" REALPATH)
        get_filename_component(BASEICON_PATH_MAC "${APP_ICON_BASEICON_MAC}" REALPATH)

        cntp_dotcontemporary_path(${targetName} DOTCONTEMPORARY_FILE REQUIRED)
        cntp_dotcontemporary(${targetName} COLOR1 "theme-colors" 0)
        cntp_dotcontemporary(${targetName} COLOR2 "theme-colors" 1)
        cntp_dotcontemporary_desktopId(${targetName} DESKTOPID RESOLVE_BLUEPRINT)

        list(APPEND ICONTOOL_ARGS
            -i ${BASEICON_PATH}
            -m ${BASEICON_PATH_MAC}
            -c "'${COLOR1}:${COLOR2}'"
            -s ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOPID}.svg
            -r ${CMAKE_CURRENT_BINARY_DIR}/appicon.qrc)

        list(APPEND ICONTOOL_OUTPUTS
            ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOPID}.svg
            ${CMAKE_CURRENT_BINARY_DIR}/appicon.qrc)

        if(BLUEPRINT)
            list(APPEND ICONTOOL_ARGS -b)
        endif()

        IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

        ENDIF()

        IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            list(APPEND ICONTOOL_ARGS
                -n ${CMAKE_CURRENT_BINARY_DIR}/native.icns)

            list(APPEND ICONTOOL_OUTPUTS
                ${CMAKE_CURRENT_BINARY_DIR}/native.icns)
        ENDIF()

        IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

        ENDIF()

        add_custom_command(OUTPUT ${ICONTOOL_OUTPUTS}
            COMMAND ${CNTPAPPICONTOOL_PATH}/cntp-appicontool ARGS ${ICONTOOL_ARGS}
            DEPENDS ${DOTCONTEMPORARY_FILE} ${BASEICON_PATH} ${BASEICON_PATH_MAC})

        target_sources(${targetName} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/appicon.qrc)

        IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set_target_properties(${targetName} PROPERTIES
                    MACOSX_BUNDLE_ICON_FILE "native.icns")
            target_sources(${targetName} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/native.icns)
            set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/native.icns PROPERTIES
                    MACOSX_PACKAGE_LOCATION Resources)
        ENDIF()

        IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
            install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOPID}.svg
                DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps/)
        ENDIF()
    ENDIF()
endfunction()
