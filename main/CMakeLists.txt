project(${CMAKE_PROJECT_NAME})

add_executable(${PROJECT_NAME}
    main.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/inc
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        App
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_BINARY_DIR}/dist/bin
)
