target_sources(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/about.h
    ${CMAKE_CURRENT_LIST_DIR}/applicationdefs.h
    ${CMAKE_CURRENT_LIST_DIR}/mainwindow.h
    ${CMAKE_CURRENT_LIST_DIR}/options.h
    ${CMAKE_CURRENT_LIST_DIR}/playfield.h
    ${CMAKE_CURRENT_LIST_DIR}/tetrisshape.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
