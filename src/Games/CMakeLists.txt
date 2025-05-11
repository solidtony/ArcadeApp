project(Games)

add_library(${PROJECT_NAME} SHARED
    Actor.cpp
    Ball.cpp
    Block.cpp
    BreakOut.cpp
    BreakoutGameLevel.cpp
    Excluder.cpp
    Ghost.cpp
    GhostAI.cpp
    LevelBoundary.cpp
    Pacman.cpp
    PacmanGame.cpp
    PacmanGameUtils.cpp
    PacmanLevel.cpp
    Paddle.cpp
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
