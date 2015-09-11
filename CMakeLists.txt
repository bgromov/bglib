cmake_minimum_required (VERSION 2.8.3)

project (bglib)

set (BGLIB_VERSION_MAJOR "1")
set (BGLIB_VERSION_MINOR "3")
set (BGLIB_VERSION_PATCH "2")
set (BGLIB_VERSION_BUILD "122")

set (BGLIB_VERSION "${BGLIB_VERSION_MAJOR}.${BGLIB_VERSION_MINOR}.${BGLIB_VERSION_PATCH}")
set (BGLIB_VERSION_FULL "${BGLIB_VERSION}-${BGLIB_VERSION_BUILD}")

include_directories (
    include
    include/bglib
)

add_definitions (
    -DPLATFORM_POSIX
    -std=c++11
)

add_library (${PROJECT_NAME} SHARED
    src/cmd_def.cpp
    src/uart.cpp
)

set_target_properties (${PROJECT_NAME} PROPERTIES
    VERSION ${BGLIB_VERSION}
    SOVERSION ${BGLIB_VERSION}
)

add_executable (scan_example
    examples/scan_example/main.cpp
)

target_link_libraries (scan_example
    ${PROJECT_NAME}
)

### Install
install (TARGETS ${PROJECT_NAME}
    DESTINATION lib
)

configure_file (
    ${PROJECT_SOURCE_DIR}/lib${PROJECT_NAME}.pc.in
    ${PROJECT_BINARY_DIR}/lib${PROJECT_NAME}.pc
    @ONLY
)

install (FILES ${PROJECT_BINARY_DIR}/lib${PROJECT_NAME}.pc
    DESTINATION lib/pkgconfig
)

install (DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include
)

install (DIRECTORY ${PROJECT_SOURCE_DIR}/examples/
    DESTINATION share/${PROJECT_NAME}/examples
)

### Debian package
include (InstallRequiredSystemLibraries)
set (CPACK_GENERATOR "DEB")
# Set package version
set (CPACK_PACKAGE_VERSION_MAJOR "${BGLIB_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${BGLIB_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${BGLIB_VERSION_PATCH}")
set (CPACK_PACKAGE_VERSION_BUILD "${BGLIB_VERSION_BUILD}")
set (CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-${CPACK_PACKAGE_VERSION_BUILD}")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "BGLib - BlueGiga BLE library")
# Debian package specifics
set (CPACK_DEBIAN_PACKAGE_MAINTAINER "Boris Gromov (boris@idsia.ch)")
set (CPACK_DEBIAN_PACKAGE_PROVIDES "libbglib")

set (CPACK_SET_DESTDIR true)

include (CPack)
