CNTP_INSTALL_PREFIX = $$PREFIX
equals(CNTP_INSTALL_PREFIX, "") {
    CNTP_INSTALL_PREFIX = $$[CNTP_INSTALL_PREFIX]
}
equals(CNTP_INSTALL_PREFIX, "") {
    CNTP_INSTALL_PREFIX = $$[QT_INSTALL_PREFIX]
    CONFIG += CNTP_INSTALL_PREFIX_IS_DEFAULT
}

equals(CNTP_INSTALL_LIB, "") {
    CNTP_INSTALL_PREFIX_IS_DEFAULT {
        CNTP_INSTALL_LIB = $$[QT_INSTALL_LIBS]
    } else {
        CNTP_INSTALL_LIB = $$CNTP_INSTALL_PREFIX/lib
    }
}

equals(CNTP_INSTALL_HEADERS, "") {
    CNTP_INSTALL_PREFIX_IS_DEFAULT {
        CNTP_INSTALL_HEADERS = $$[QT_INSTALL_HEADERS]
    } else {
        CNTP_INSTALL_HEADERS = $$CNTP_INSTALL_PREFIX/include
    }
}

equals(CNTP_INSTALL_MODULES, "") {
    CNTP_INSTALL_PREFIX_IS_DEFAULT {
        CNTP_INSTALL_MODULES = $$[QMAKE_MKSPECS]/modules
    } else {
        CNTP_INSTALL_MODULES = $$CNTP_INSTALL_PREFIX/qt/mkspecs/modules
    }
}

equals(CNTP_INSTALL_BIN, "") {
    CNTP_INSTALL_BIN = $$CNTP_INSTALL_PREFIX/bin
}

equals(CNTP_INSTALL_SETTINGS, "") {
    CNTP_INSTALL_SETTINGS = /etc
}

equals(CNTP_INSTALL_QT_PLUGINS, "") {
    CNTP_INSTALL_QT_PLUGINS = $$[QT_INSTALL_PLUGINS]
}

DEFINES += SYSTEM_LIBRARY_DIRECTORY=\\\"$$CNTP_INSTALL_LIB\\\"
DEFINES += SYSTEM_PREFIX_DIRECTORY=\\\"$$CNTP_INSTALL_PREFIX\\\"
