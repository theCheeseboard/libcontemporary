include_guard()

include(FetchContent)

function(cntp_fakeit target version)
    FetchContent_Declare(
      fakeit-${version}
      GIT_REPOSITORY https://github.com/eranpeer/FakeIt.git
      GIT_TAG        ${version}
    )
    FetchContent_MakeAvailable(fakeit-${version})
    target_link_libraries(${target} PRIVATE FakeIt::FakeIt-qtest)
endfunction()
