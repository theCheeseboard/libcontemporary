function(cntp_target_install_appstream_metainfo_file target translationfile)
    IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        find_path(CNTPTRAPPSTREAM_PATH cntp-trappstream)
        IF(${CNTPTRAPPSTREAM_PATH} STREQUAL "CNTPTRAPPSTREAM_PATH-NOTFOUND")
            message(FATAL_ERROR "cntp-trappstream executable not available, appstream metainfo file translations cannot be created")
        ELSE()
            message("-- Finding appstream metainfo files to install for (target ${target})")
            get_filename_component(translationfile_noext ${translationfile} NAME_WLE)

            install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${translationfile_noext}.xml
                    DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo/)
        ENDIF()
    ENDIF()
endfunction()

function(cntp_target_translate_appstream_metainfo_file target translationfile)
    find_path(CNTPTRAPPSTREAM_PATH cntp-trappstream)
    IF(${CNTPTRAPPSTREAM_PATH} STREQUAL "CNTPTRAPPSTREAM_PATH-NOTFOUND")
        message(FATAL_ERROR "cntp-trappstream executable not available, appstream metainfo file translations cannot be created")
    ELSE()
        message("-- Finding appstream metainfo files to generate translations for (target ${target})")
        get_filename_component(translationfile_noext ${translationfile} NAME_WLE)

        add_custom_target(${target}_CNTPAPPSTREAMTRANSLATIONS_${translationfile_noext}
                COMMAND ${CNTPTRAPPSTREAM_PATH}/cntp-trappstream --json-generate --json-directory ${CMAKE_CURRENT_SOURCE_DIR}/translations/appstream/${translationfile_noext}/ --metainfo-template ${translationfile}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMENT "Generating appstream metainfo translation files"
                VERBATIM)
        add_dependencies(${target} ${target}_CNTPAPPSTREAMTRANSLATIONS_${translationfile_noext})

        add_custom_target(${target}_CNTPAPPSTREAMTRANSLATIONMERGE_${translationfile_noext}
                COMMAND ${CNTPTRAPPSTREAM_PATH}/cntp-trappstream --metainfo-generate --json-directory ${CMAKE_CURRENT_SOURCE_DIR}/translations/appstream/${translationfile_noext}/ --metainfo-template ${translationfile} --metainfo-output ${CMAKE_CURRENT_BINARY_DIR}/${translationfile_noext}.xml
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMENT "Generating appstream metainfo file from translations"
                VERBATIM)
        add_dependencies(${target} ${target}_CNTPAPPSTREAMTRANSLATIONMERGE_${translationfile_noext})
    ENDIF()
endfunction()

function(cntp_target_appstream_metainfo_file target)
    set(multiValueArgs METAINFO_FILE BLUEPRINT_METAINFO_FILE)
    cmake_parse_arguments(METAINFO_FILE "" "" "${multiValueArgs}" ${ARGN})

    foreach(METAINFO_FILE ${METAINFO_FILE_BLUEPRINT_METAINFO_FILE} ${METAINFO_FILE_METAINFO_FILE})
        cntp_target_translate_appstream_metainfo_file(${target} ${METAINFO_FILE})
    endforeach()

    if(BLUEPRINT AND DEFINED METAINFO_FILE_BLUEPRINT_METAINFO_FILE)
        foreach(METAINFO_FILE ${METAINFO_FILE_BLUEPRINT_METAINFO_FILE})
            cntp_target_install_appstream_metainfo_file(${target} ${METAINFO_FILE})
        endforeach()
    else()
        foreach(METAINFO_FILE ${METAINFO_FILE_METAINFO_FILE})
            cntp_target_install_appstream_metainfo_file(${target} ${METAINFO_FILE})
        endforeach()
    endif()
endfunction()
