include_guard()

function(cntp_winappx target appxmanifest)
    get_target_property(APPX_DISPLAY_NAME ${target} READABLE_NAME)
    get_target_property(APPX_EXECUTABLE_NAME ${target} OUTPUT_NAME)

    cntp_repoversion(REPOVERSION)

    set(APPX_EXECUTABLE_NAME ${APPX_EXECUTABLE_NAME}.exe)
    set(APPX_ARCH x64) # TODO: Update
    set(APPX_VERSION ${REPOVERSION_MAJOR}.${REPOVERSION_MINOR}.${REPOVERSION_REVISION}.${REPOVERSION_BUILD})
    configure_file(${appxmanifest} appxmanifest.xml)
endfunction()
