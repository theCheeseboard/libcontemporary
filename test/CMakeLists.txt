enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

function(cntp_test)
    set(options)
    set(oneValueArgs NAME)
    set(multiValueArgs FILES)
    cmake_parse_arguments(TEST_ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(target libcontemporary_tests_${TEST_ARGS_NAME})

    add_executable(${target}
        ${TEST_ARGS_FILES}
    )
    add_test(NAME ${TEST_ARGS_NAME} COMMAND ${TEST_ARGS_NAME})
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 20
        AUTOMOC ON
        AUTOMOC_MOC_OPTIONS "--output-json"
    )

    target_link_libraries(${target} PRIVATE Qt${QT_VERSION_MAJOR}::Test libcontemporary)
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../lib/)
endfunction()

add_subdirectory(trange)
add_subdirectory(di)
