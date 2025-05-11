project(Input)

add_library(${PROJECT_NAME} SHARED
    GameController.cpp
    InputController.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIRS}

    ${CMAKE_SOURCE_DIR}/inc
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL2
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_BINARY_DIR}/dist/bin
)
