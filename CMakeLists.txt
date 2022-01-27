cmake_minimum_required(VERSION 3.5.0)

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

set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "1")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(PROJECT_LEGAL_COPYRIGHT "Copyright Fanuware (c) 2022")

find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)

include(ui/CMakeLists.txt)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME} res/application.qrc
    $<$<CXX_COMPILER_ID:MSVC>:
        res/application.rc
    >
)

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

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)

if(MSVC)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()
