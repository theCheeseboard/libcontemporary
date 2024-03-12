include_guard()

function(cntp_translate target)
    set(oneValueArgs OUTPUT)
    cmake_parse_arguments(TRANSLATE "" "${oneValueArgs}" "" ${ARGN})

    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/translations)

    file(GLOB TRANSLATION_FILES ${CMAKE_CURRENT_SOURCE_DIR}/translations/*.ts)
    list(APPEND TRANSLATION_FILES ${CMAKE_CURRENT_SOURCE_DIR}/translations/en_US.ts)

    get_target_property(SOURCE_FILES ${target} TRANSLATION_SOURCES)
    if("${SOURCE_FILES}" STREQUAL "SOURCE_FILES-NOTFOUND")
        get_target_property(SOURCE_FILES ${target} SOURCES)
        if(SOURCE_FILES STREQUAL "SOURCE_FILES-NOTFOUND")
            set(SOURCE_FILES "")
        endif()

        get_target_property(QML_FILES ${target} QT_QML_MODULE_QML_FILES)
        if(NOT QML_FILES STREQUAL "QML_FILES-NOTFOUND")
            foreach(QML_FILE ${QML_FILES})
                list(APPEND SOURCE_FILES ${QML_FILE})
            endforeach()
        endif()
    endif()

    get_target_property(TARGET_MODULES ${target} CNTP_MODULE_TARGETS)
    if(NOT TARGET_MODULES STREQUAL "TARGET_MODULES-NOTFOUND")
        foreach(MODULE ${TARGET_MODULES})
            get_target_property(MODULE_QML_FILES ${MODULE} QT_QML_MODULE_QML_FILES)
            if(NOT TARGET_MODULES STREQUAL "MODULE_QML_FILES-NOTFOUND")
                foreach(QML_FILE ${MODULE_QML_FILES})
                    list(APPEND SOURCE_FILES ${QML_FILE})
                endforeach()
            endif()
        endforeach()
    endif()

    set(QT_DEFAULT_MAJOR_VERSION ${QT_VERSION_MAJOR})
    qt_add_translations(${target}
        TS_FILES ${TRANSLATION_FILES}
        QM_FILES_OUTPUT_VARIABLE QM_FILES
        LUPDATE_TARGET ${target}_lupdate
        LRELEASE_TARGET ${target}_lrelease
        IMMEDIATE_CALL
        SOURCES ${SOURCE_FILES}
    )
    add_dependencies(${target}_lrelease ${target}_lupdate)

    if("${TRANSLATE_OUTPUT}" STREQUAL "")
        include(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cntp-sharedir.cmake)
        cntp_get_target_share_dir(SHARE_DIR ${target})

        IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            cntp_get_target_share_subdir_name(SHARE_DIR_CHILD ${target})
            foreach(file IN LISTS QM_FILES)
                get_target_property(INSTALL_TARGET ${target} CNTP_PARENT_TARGET)
                if(INSTALL_TARGET STREQUAL "INSTALL_TARGET-NOTFOUND")
                    set(INSTALL_TARGET ${target})
                endif()

                target_sources(${INSTALL_TARGET} PRIVATE ${file})
                set_source_files_properties(${file} PROPERTIES
                        MACOSX_PACKAGE_LOCATION Resources/translations/${SHARE_DIR_CHILD})
            endforeach()
        ENDIF()

        IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
            cntp_get_target_share_subdir_name(SHARE_DIR_CHILD ${target})
            install(FILES ${QM_FILES}
                    DESTINATION translations/${SHARE_DIR_CHILD})
        ENDIF()

        IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
            install(FILES ${QM_FILES}
                    DESTINATION ${SHARE_DIR}/translations)
        ENDIF()
    ELSE()
        set(${TRANSLATE_OUTPUT} ${QM_FILES} PARENT_SCOPE)
    ENDIF()
endfunction()
