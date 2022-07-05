function(cntp_init target cxx-standard)
    set_target_properties(${target} PROPERTIES
            AUTOMOC ON
            AUTORCC ON
            AUTOUIC ON
            CXX_STANDARD ${cxx-standard}
            CXX_STANDARD_REQUIRED ON)
            
    find_package(Qt6 REQUIRED COMPONENTS LinguistTools)

    cntp_dotcontemporary_desktopId(${target} DESKTOPID RESOLVE_BLUEPRINT)

    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
    add_compile_definitions(SYSTEM_LIBRARY_DIRECTORY="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
    add_compile_definitions(SYSTEM_PREFIX_DIRECTORY="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_PREFIX}")
    add_compile_definitions(SYSTEM_DATA_DIRECTORY="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}")

    if(NOT ${DESKTOPID} STREQUAL "${DESKTOPID}-NOTFOUND")
        add_compile_definitions(T_APPMETA_DESKTOP_ID="${DESKTOPID}")
    endif()

    cntp_enable_coroutines(${target})
endfunction()

function(cntp_init_plugin parent target cxx-standard share-subdir)
    cntp_init(${target} ${cxx-standard})
    cntp_install_plugin(${parent} ${target})

    cntp_get_target_share_dir_name(SHARE_DIR ${parent})

    set_target_properties(${target} PROPERTIES
            CNTP_DATA_SUBDIR ${SHARE_DIR}/${share-subdir}
            CNTP_DATA_SUBDIR_WITHOUT_PARENT ${share-subdir}
            CNTP_PARENT_TARGET ${parent})
    add_dependencies(${parent} ${target})
endfunction()
