function(cntp_init target cxx-standard)
    set_target_properties(${target} PROPERTIES
            AUTOMOC ON
            AUTORCC ON
            AUTOUIC ON
            CXX_STANDARD ${cxx-standard}
            CXX_STANDARD_REQUIRED ON)

    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
    add_compile_definitions(SYSTEM_LIBRARY_DIRECTORY="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
endfunction()