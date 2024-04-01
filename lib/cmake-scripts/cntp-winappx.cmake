include_guard()

function(cntp_translate_arch outvar arch)
    if(${arch} STREQUAL "AMD64")
        set(${outvar} "x64" PARENT_SCOPE)
    elseif(${arch} STREQUAL "arm64")
        set(${outvar} "arm64" PARENT_SCOPE)
    else()
        set(${outvar} ${arch} PARENT_SCOPE)
    endif()
endfunction()

function(cntp_winappx target appxmanifest)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        get_target_property(APPX_DISPLAY_NAME ${target} READABLE_NAME)
        get_target_property(APPX_EXECUTABLE_NAME ${target} OUTPUT_NAME)

        cntp_repoversion(REPOVERSION)

        if(";${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION};$ENV{UCRTVersion};$ENV{WindowsSDKVersion};" MATCHES [=[;(10\.[0-9.]+)[;\]]=])
            get_filename_component(WINDOWS_KITS_BASE_DIR "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]" ABSOLUTE)
            set(WINDOWS_SDK_VERSION ${CMAKE_MATCH_1})
        endif()

        cntp_translate_arch(CNTP_TARGET_TRANSLATED_ARCH ${CNTP_TARGET_ARCH})

        string(REGEX REPLACE "\\." ";" APPX_SDK_VERSION_PARTS ${WINDOWS_SDK_VERSION})

        # Default versions to 0 if they can't be found - the list will always have at least 4 elements
        list(APPEND APPX_SDK_VERSION_PARTS 0 0 0 0)
        list(POP_FRONT APPX_SDK_VERSION_PARTS APPX_SDK_VERSION_MAJOR APPX_SDK_VERSION_MINOR APPX_SDK_VERSION_REVISION APPX_SDK_VERSION_BUILD)

        set(${outvar}_MAJOR ${${outvar}_MAJOR} PARENT_SCOPE)
        set(${outvar}_MINOR ${${outvar}_MINOR} PARENT_SCOPE)
        set(${outvar}_REVISION ${${outvar}_REVISION} PARENT_SCOPE)
        set(${outvar}_BUILD ${GIT_REVLIST_OUTPUT} PARENT_SCOPE)

        set(APPX_EXECUTABLE_NAME ${APPX_EXECUTABLE_NAME}.exe)
        set(APPX_ARCH ${CNTP_TARGET_TRANSLATED_ARCH})
        set(APPX_VERSION ${REPOVERSION_MAJOR}.${REPOVERSION_MINOR}.${REPOVERSION_REVISION}.0)
        set(APPX_SDK_VERSION ${APPX_SDK_VERSION_MAJOR}.${APPX_SDK_VERSION_MINOR}.${APPX_SDK_VERSION_REVISION}.${APPX_SDK_VERSION_BUILD})
        configure_file(${appxmanifest} appxmanifest.xml)
    endif()
endfunction()
