cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(engine2D_lib)

find_package(SDL2 REQUIRED
    NAMES SDL2 sdl2)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)



add_library(engine2D STATIC
    include/core/engine.h
    src/core/engine.cpp

    include/core/igame.h


    include/render/shader_program.h
    src/render/shader_program.cpp

    include/render/vertex_buffer.h
    src/render/vertex_buffer.cpp

    include/render/index_buffer.h
    src/render/index_buffer.cpp

    include/render/vertex_array.h
    src/render/vertex_array.cpp

    include/render/renderer.h
    src/render/renderer.cpp

    include/render/texture2D.h
    src/render/texture2D.cpp

    include/render/sprite2D.h
    src/render/sprite2D.cpp

    include/render/sprite_animator.h
    src/render/sprite_animator.cpp

    include/render/frame_structures.h

    include/render/vertex_buffer_descriptor.h
    src/render/vertex_buffer_descriptor.cpp


    include/sound/wav_sound.h
    src/sound/wav_sound.cpp

    include/sound/sound_buffer.h
    src/sound/sound_buffer.cpp


    include/input/input_manager.h
    src/input/input_manager.cpp

    include/input/input_event.h
    src/input/input_event.cpp


    include/resources/resource_manager.h
    src/resources/resource_manager.cpp

    include/resources/stb_image.h


    pimpl/include/render/engine_impl.h)

target_include_directories(engine2D PUBLIC
    include/)

target_include_directories(engine2D PRIVATE
    pimpl/include/)
    
target_compile_features(engine2D PRIVATE cxx_std_17)

target_link_libraries(engine2D PUBLIC glad)

target_link_libraries(engine2D PUBLIC glm)

target_link_libraries(engine2D PUBLIC OpenGL::OpenGL)

target_link_libraries(engine2D PUBLIC SDL2::SDL2)


