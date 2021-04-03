cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(game2D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(game2D game/src/main.cpp
    res/shaders/test_shader.vert
    res/shaders/test_shader.frag)

add_subdirectory(external/glad)
add_subdirectory(external/glm)

add_subdirectory(engine2D)


target_link_libraries(game2D PRIVATE engine2D)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
