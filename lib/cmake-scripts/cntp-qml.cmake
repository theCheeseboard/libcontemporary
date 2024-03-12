include_guard()

macro(cntp_add_qml_module moduleName parentTarget)
    qt_add_library(${parentTarget}-qmlmodule-${moduleName} STATIC)
    target_link_libraries(${parentTarget}-qmlmodule-${moduleName}
        PRIVATE libcontemporary Qt::Quick Qt::QuickControls2
    )
    qt_add_qml_module(${parentTarget}-qmlmodule-${moduleName} ${ARGN})

    target_link_libraries(${parentTarget} PRIVATE ${parentTarget}-qmlmodule-${moduleName}plugin)

    set_property(TARGET ${parentTarget} APPEND
        PROPERTY CNTP_MODULE_TARGETS ${parentTarget}-qmlmodule-${moduleName}
    )
endmacro()
