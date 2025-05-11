project(App)

add_library(${PROJECT_NAME} SHARED
    App.cpp
    Button.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIRS}

    ${CMAKE_SOURCE_DIR}/inc
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL2

        Graphics
        Input
        Scenes
        Shapes
        Utils
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_BINARY_DIR}/dist/bin
)
