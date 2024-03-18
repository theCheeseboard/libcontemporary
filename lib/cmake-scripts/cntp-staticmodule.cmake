include_guard()

function(cntp_add_static_module)
    set(options NO_LINK)
    set(oneValueArgs MODULE_NAME PARENT_TARGET TARGET_NAME)
    set(multiValueArgs SOURCES)

    cmake_parse_arguments(ADD_STATIC_MODULE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT DEFINED ADD_STATIC_MODULE_MODULE_NAME)
        message(FATAL_ERROR "MODULE_NAME not passed to invocation of cntp_add_static_module")
    endif()
    if(NOT DEFINED ADD_STATIC_MODULE_PARENT_TARGET)
        message(FATAL_ERROR "PARENT_TARGET not passed to invocation of cntp_add_static_module")
    endif()
    if(NOT DEFINED ADD_STATIC_MODULE_TARGET_NAME)
        set(ADD_STATIC_MODULE_TARGET_NAME ${ADD_STATIC_MODULE_PARENT_TARGET}-module-${ADD_STATIC_MODULE_MODULE_NAME})
    endif()

    qt_add_library(${ADD_STATIC_MODULE_TARGET_NAME} STATIC)

    if(DEFINED ADD_STATIC_MODULE_SOURCES)
        target_sources(${ADD_STATIC_MODULE_TARGET_NAME} PRIVATE ${ADD_STATIC_MODULE_SOURCES})
    endif()

    set_property(TARGET ${ADD_STATIC_MODULE_PARENT_TARGET} APPEND
        PROPERTY CNTP_MODULE_TARGETS ${ADD_STATIC_MODULE_TARGET_NAME}
    )

    if(NOT ADD_STATIC_MODULE_NO_LINK)
        target_link_libraries(${ADD_STATIC_MODULE_PARENT_TARGET} PRIVATE ${ADD_STATIC_MODULE_TARGET_NAME})
    endif()
endfunction()
