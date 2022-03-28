function(cntp_define_build_dirs)
    execute_process(
            COMMAND qmake6 -query QT_INSTALL_PLUGINS
            RESULT_VARIABLE PLUGIN_INSTALLATION_DIR_RESULT
            OUTPUT_VARIABLE PLUGIN_INSTALLATION_DIR
    )

    if (PLUGIN_INSTALLATION_DIR_RESULT EQUAL 0)
        string(STRIP "${PLUGIN_INSTALLATION_DIR}" PLUGIN_INSTALLATION_DIR)
        file(TO_CMAKE_PATH "${PLUGIN_INSTALLATION_DIR}" PLUGIN_INSTALLATION_DIR)
        set(CNTP_INSTALL_PLUGINS ${PLUGIN_INSTALLATION_DIR} PARENT_SCOPE)
    else()
        message(FATAL_ERROR "Couldn't call qtpaths. Ensure Qt 6 is installed correctly and qtpaths is located in your PATH.")
    endif()
endfunction()