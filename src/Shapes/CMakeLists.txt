project(Shapes)

add_library(${PROJECT_NAME} SHARED
    AARectangle.cpp
    Circle.cpp
    Lines2D.cpp
    Shape.cpp
    Star.cpp
    Triangle.cpp
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
