function(cntp_target_name targetName targetReadableName)
    IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        IF(BLUEPRINT)
            set(TARGET_FINAL_NAME "${targetReadableName} Blueprint")
        ELSE()
            set(TARGET_FINAL_NAME "${targetReadableName}")
        ENDIF()
    ELSE()
        string(TOLOWER ${targetReadableName} TARGET_LOWER_NAME)
        IF(BLUEPRINT)
            set(TARGET_FINAL_NAME "${TARGET_LOWER_NAME}-blueprint")
        ELSE()
            set(TARGET_FINAL_NAME "${TARGET_LOWER_NAME}")
        ENDIF()
    ENDIF()


    set_target_properties(${targetName} PROPERTIES
            OUTPUT_NAME "${TARGET_FINAL_NAME}")
    target_compile_definitions(${targetName} PRIVATE
            T_APPMETA_READABLE_NAME="${TARGET_FINAL_NAME}")
endfunction()
