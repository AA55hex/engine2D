cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(engine2D_lib)

find_package(SDL2 REQUIRED
    NAMES SDL2 sdl2)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_subdirectory(external/glad)
add_subdirectory(external/glm)

set(PUBLIC_INCLUDES
    include/public/core/engine.h
    src/public/core/engine.cpp

    include/public/core/igame.h

    include/public/core/window.h
    include/public/core/audio.h


    include/public/render/shader_program.h
    src/public/render/shader_program.cpp

    include/public/render/texture2D.h
    src/public/render/texture2D.cpp

    include/public/render/sprite2D.h
    src/public/render/sprite2D.cpp

    include/public/render/sprite_animator.h
    src/public/render/sprite_animator.cpp

    include/public/render/frame_structures.h


    include/public/sound/wav_sound.h
    src/public/sound/wav_sound.cpp

    include/public/sound/sound_buffer.h
    src/public/sound/sound_buffer.cpp


    include/public/input/input_manager.h
    src/public/input/input_manager.cpp

    include/public/input/input_event.h
    src/public/input/input_event.cpp


    include/public/resources/resource_manager.h
    src/public/resources/resource_manager.cpp

    include/public/resources/stb_image.h)

set(PRIVATE_INCLUDES
    include/private/system/audio_impl.h
    src/private/system/audio.cpp

    include/private/system/window_impl.h
    src/private/system/window.cpp

    include/private/system/r_bind.h


    include/private/core/engine_impl.h



    include/private/render/vertex_buffer.h
    src/private/render/vertex_buffer.cpp

    include/private/render/index_buffer.h
    src/private/render/index_buffer.cpp

    include/private/render/vertex_array.h
    src/private/render/vertex_array.cpp

    include/private/render/renderer.h
    src/private/render/renderer.cpp

    include/private/render/vertex_buffer_descriptor.h
    src/private/render/vertex_buffer_descriptor.cpp

    include/private/render/sprite2D_impl.h

    include/private/render/sprogram_impl.h
    )

add_library(engine2D STATIC
    ${PUBLIC_INCLUDES}
    ${PRIVATE_INCLUDES}
   )
target_include_directories(engine2D PRIVATE
    include/private/)

target_include_directories(engine2D PUBLIC
    include/public/)



    
target_compile_features(engine2D PRIVATE cxx_std_17)

target_link_libraries(engine2D PUBLIC glm)

target_link_libraries(engine2D PRIVATE glad)

target_link_libraries(engine2D PRIVATE OpenGL::OpenGL)

target_link_libraries(engine2D PRIVATE SDL2::SDL2)


