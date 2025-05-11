project(Scenes)

add_library(${PROJECT_NAME} SHARED
    ArcadeScene.cpp
    ButtonOptionsScene.cpp
    GameScene.cpp
    NotImplementedScene.cpp
    PacmanStartScene.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/inc
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Games
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_BINARY_DIR}/dist/bin
)
