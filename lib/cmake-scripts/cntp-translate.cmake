function(cntp_translate target)
    include(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cntp-sharedir.cmake)
    cntp_get_target_share_dir(SHARE_DIR ${target})

    file(GLOB TRANSLATION_FILES ${CMAKE_CURRENT_SOURCE_DIR}/translations/*.ts)
    #list(PREPEND TRANSLATION_FILES ${CMAKE_CURRENT_SOURCE_DIR}/translations/en_US.ts)

    qt_add_translations(${target}
        TS_FILES ${TRANSLATION_FILES}
        QM_FILES_OUTPUT_VARIABLE QM_FILES
    )

   install(FILES ${QM_FILES} DESTINATION "translations")
endfunction()