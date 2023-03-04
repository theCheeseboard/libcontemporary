include_guard()

function(cntp_find_tool var name)
    # ../../../../bin/
    set(BIN_HINT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
    cmake_path(GET BIN_HINT PARENT_PATH BIN_HINT)
    cmake_path(GET BIN_HINT PARENT_PATH BIN_HINT)
    cmake_path(GET BIN_HINT PARENT_PATH BIN_HINT)
    cmake_path(GET BIN_HINT PARENT_PATH BIN_HINT)

    find_program(${var} ${name} REQUIRED
        HINTS ${BIN_HINT}/bin
    )
    set(${var} ${${var}} PARENT_SCOPE)
endfunction()
