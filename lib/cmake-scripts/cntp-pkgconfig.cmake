function(cntp_find_pkgconfig targetName packageName)
    set(options REQUIRED)
    set(multiValueArgs WIN_FALLBACK_DLL WIN_FALLBACK_IMPLIB WIN_FALLBACK_INCLUDE)
    cmake_parse_arguments(FIND_PKGCONFIG "${options}" "" "${multiValueArgs}" ${ARGN})

    message("${FIND_PKGCONFIG_PACKAGES}")

    IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        add_library(PkgConfig::${targetName} SHARED IMPORTED)
        set_target_properties(PkgConfig::${targetName} PROPERTIES
                IMPORTED_LOCATION ${FIND_PKGCONFIG_WIN_FALLBACK_DLL}
                IMPORTED_IMPLIB ${FIND_PKGCONFIG_WIN_FALLBACK_IMPLIB})
        target_include_directories(PkgConfig::${targetName} INTERFACE
                ${FIND_PKGCONFIG_WIN_FALLBACK_INCLUDE})
    ELSE()
        IF(FIND_PKGCONFIG_REQUIRED)
            pkg_check_modules(${targetName} IMPORTED_TARGET REQUIRED ${packageName})
        ELSE()
            pkg_check_modules(${targetName} IMPORTED_TARGET ${packageName})
        ENDIF()
    ENDIF()
endfunction()