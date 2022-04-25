cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)

project(libcontemporary VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(lib)
add_subdirectory(cntp-trdesktop)
add_subdirectory(cntp-appicontool)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_subdirectory(cntp-macdeploy)
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_subdirectory(cntp-windeploy)
ENDIF()
