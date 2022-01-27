configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/applicationdefs.cpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/applicationdefs.cpp)

target_sources(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/applicationdefs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/about.cpp
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/options.cpp
    ${CMAKE_CURRENT_LIST_DIR}/playfield.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tetrisshape.cpp
)
