cmake_minimum_required(VERSION 3.15)

if(WIN32)
    project(Tetris CXX)
else()
    project(tetris CXX)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build-Type: Debug/Release" FORCE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --tags --match "[0-9]*.[0-9]*.[0-9]*" --abbrev=7 --dirty
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_DESCRIBE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
string(REGEX MATCH "^([0-9]+)\.([0-9]+)\.([0-9]+)" _ver_match "${GIT_DESCRIBE}")
if(_ver_match)
    set(PROJECT_VERSION_MAJOR "${CMAKE_MATCH_1}")
    set(PROJECT_VERSION_MINOR "${CMAKE_MATCH_2}")
    set(PROJECT_VERSION_PATCH "${CMAKE_MATCH_3}")
    set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
    if(NOT GIT_DESCRIBE STREQUAL PROJECT_VERSION)
        string(REPLACE "${PROJECT_VERSION}-" "" GIT_SUFFIX "${GIT_DESCRIBE}")
        set(PROJECT_VERSION "${PROJECT_VERSION}-${GIT_SUFFIX}")
    endif()
else()
    set(PROJECT_VERSION "0.0.0-unknown")
endif()
set(PROJECT_LEGAL_COPYRIGHT "Copyright Fanuware (c) 2025")
message (STATUS "Project version: ${PROJECT_VERSION}")

if(NOT DEFINED QT_VERSION_MAJOR)
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
endif()
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

include(ui/CMakeLists.txt)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


add_executable(${PROJECT_NAME} res/application.qrc
    $<$<CXX_COMPILER_ID:MSVC>:
        res/application.rc
    >
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

add_subdirectory(include)
add_subdirectory(src)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    $<$<CONFIG:Debug>:
        DEBUG
    >
    $<$<CONFIG:Release>:
        NDEBUG;
        QT_NO_DEBUG;
        QT_NO_DEBUG_OUTPUT
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:DEBUG>:
            _DEBUG
        >
        _WINDOWS;
        _UNICODE
    >
    UNICODE;
    QT_DLL;
    QT_NO_PRINTER
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    $<$<CONFIG:Debug>:
        $<$<CXX_COMPILER_ID:GNU>:
            -O0;
            -g;
            -feliminate-unused-debug-types
        >
        $<$<CXX_COMPILER_ID:MSVC>:
            /MDd;
            /Od;
            /Zi;
            /W3
        >
    >
    $<$<CONFIG:Release>:
        $<$<CXX_COMPILER_ID:GNU>:
            -O2
        >
        $<$<CXX_COMPILER_ID:MSVC>:
            /MD;
            /O2
        >
    >
)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets)

if(MSVC)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::QWindowsIntegrationPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Core> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Gui> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Widgets> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

if (UNIX)
    set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VENDOR "Fanuware")
    set(CPACK_PACKAGE_CONTACT "fanuware@gmail.com")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tetris")
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
    if(EXISTS "${CMAKE_SOURCE_DIR}/LICENSE")
        set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
    endif()
    if(EXISTS "${CMAKE_SOURCE_DIR}/README.md")
        set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
    endif()
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-Qt${QT_VERSION_MAJOR}")
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Fanuware")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "qtbase5-dev | qt6-base-dev")
    include(CPack)
endif()