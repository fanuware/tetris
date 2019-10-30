cmake_minimum_required(VERSION 3.1.0)

project(tetris)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# qt5 library
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/tetrisshape.cpp
    src/playfield.cpp
    src/about.cpp
    src/options.cpp
)

set(HEADERS
    include/tetrisshape.h
)

set(UI
    ui/mainwindow.ui
    ui/options.ui
    ui/about.ui
)

set(QOBJECTS
    include/mainwindow.h
    include/playfield.h
    include/options.h
    include/about.h
)

qt5_wrap_cpp(MOC ${QOBJECTS})
qt5_wrap_ui(UI_HEADER ${UI})
qt5_add_resources(QRC_CPP res/resource.qrc)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(tetris ${SOURCES} ${HEADERS} ${QOBJECTS} ${MOC} ${UI_HEADER} ${QRC_CPP})

target_compile_definitions(tetris PRIVATE QT)
target_link_libraries(tetris Qt5::Core Qt5::Gui Qt5::Widgets)

target_include_directories(tetris PRIVATE
    include
)
