include_guard()

function(cntp_get_target_share_dir_name destination target)
    get_target_property(SHARE_DIR ${target} CNTP_DATA_SUBDIR)

    if(SHARE_DIR STREQUAL "SHARE_DIR-NOTFOUND")
        message(FATAL_ERROR "Please define CNTP_DATA_SUBDIR on the target ${target}")
    endif()

    set(${destination} ${SHARE_DIR} PARENT_SCOPE)
endfunction()

function(cntp_get_target_share_dir destination target)
    include(GNUInstallDirs)
    cntp_get_target_share_dir_name(SHARE_DIR ${target})

    set(${destination} ${CMAKE_INSTALL_DATADIR}/${SHARE_DIR} PARENT_SCOPE)
endfunction()