project(Utils)

add_library(${PROJECT_NAME} SHARED
    FileCommandLoader.cpp
    Utils.cpp
    Vec2D.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/inc
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_BINARY_DIR}/dist/bin
)
