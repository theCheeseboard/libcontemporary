include_guard()

function(cntp_target_name targetName targetReadableName)
    IF(BLUEPRINT)
        set(TARGET_READABLE_NAME "${targetReadableName} Blueprint")
    ELSE()
        set(TARGET_READABLE_NAME "${targetReadableName}")
    ENDIF()

    IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(TARGET_FINAL_NAME ${TARGET_READABLE_NAME})
    ELSE()
        string(TOLOWER ${targetReadableName} TARGET_LOWER_NAME)
        string(REPLACE " " "-" TARGET_LOWER_SKEWER_NAME ${TARGET_LOWER_NAME})
        IF(BLUEPRINT)
            set(TARGET_FINAL_NAME "${TARGET_LOWER_SKEWER_NAME}-blueprint")
        ELSE()
            set(TARGET_FINAL_NAME "${TARGET_LOWER_SKEWER_NAME}")
        ENDIF()
    ENDIF()

    if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
        # Changing the target name when building for Android confuses Qt Creator
        set_target_properties(${targetName} PROPERTIES
                OUTPUT_NAME "${TARGET_FINAL_NAME}")
    endif()
    target_compile_definitions(${targetName} PRIVATE
            T_APPMETA_READABLE_NAME="${TARGET_READABLE_NAME}")

    if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
        # Also update the AndroidManifest.xml
        get_target_property(ANDROID_PACKAGE_SOURCE_DIR ${targetName} QT_ANDROID_PACKAGE_SOURCE_DIR)
        file(READ ${ANDROID_PACKAGE_SOURCE_DIR}/AndroidManifest.xml ANDROID_MANIFEST)
        string(REPLACE "-- %%INSERT_APP_NAME%% --" "${TARGET_READABLE_NAME}" ANDROID_MANIFEST "${ANDROID_MANIFEST}")
        file(WRITE ${ANDROID_PACKAGE_SOURCE_DIR}/AndroidManifest.xml "${ANDROID_MANIFEST}")
    endif()
endfunction()
