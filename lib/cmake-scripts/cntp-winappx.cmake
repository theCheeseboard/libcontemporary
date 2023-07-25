include_guard()

function(cntp_winappx target appxmanifest)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        get_target_property(APPX_DISPLAY_NAME ${target} READABLE_NAME)
        get_target_property(APPX_EXECUTABLE_NAME ${target} OUTPUT_NAME)

        cntp_repoversion(REPOVERSION)


        string(REGEX REPLACE "\\." ";" APPX_SDK_VERSION_PARTS ${CMAKE_SYSTEM_VERSION})

        # Default versions to 0 if they can't be found - the list will always have at least 4 elements
        list(APPEND APPX_SDK_VERSION_PARTS 0 0 0 0)
        list(POP_FRONT APPX_SDK_VERSION_PARTS APPX_SDK_VERSION_MAJOR APPX_SDK_VERSION_MINOR APPX_SDK_VERSION_REVISION APPX_SDK_VERSION_BUILD)

        set(${outvar}_MAJOR ${${outvar}_MAJOR} PARENT_SCOPE)
        set(${outvar}_MINOR ${${outvar}_MINOR} PARENT_SCOPE)
        set(${outvar}_REVISION ${${outvar}_REVISION} PARENT_SCOPE)
        set(${outvar}_BUILD ${GIT_REVLIST_OUTPUT} PARENT_SCOPE)

        set(APPX_EXECUTABLE_NAME ${APPX_EXECUTABLE_NAME}.exe)
        set(APPX_ARCH x64) # TODO: Update
        set(APPX_VERSION ${REPOVERSION_MAJOR}.${REPOVERSION_MINOR}.${REPOVERSION_REVISION}.0)
        set(APPX_SDK_VERSION ${APPX_SDK_VERSION_MAJOR}.${APPX_SDK_VERSION_MINOR}.${APPX_SDK_VERSION_REVISION}.${APPX_SDK_VERSION_BUILD})
        configure_file(${appxmanifest} appxmanifest.xml)
    endif()
endfunction()
