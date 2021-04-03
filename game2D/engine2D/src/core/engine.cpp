#include <core/engine.h>
#include <core/igame.h>
#include <glad/glad.h>
#include <render/renderer.h>
#include <sound/sound_buffer.h>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include "input/input_manager.h"
namespace core
{
bool engine::inicialize(int width, int height, std::string title, bool gl_debug)
{
  if (inicialized)
    {
      std::cerr << "engine:: inicialize error: engine already inicialized"
                << std::endl;
      return false;
    }
  const int init_result = SDL_Init(
      SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS |
      SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER);

  if (init_result != 0)
    {
      const char* err_message = SDL_GetError();
      std::cerr << "engine:: inicialize error: failed call SDL_Init: "
                << err_message << std::endl;
      inicialized = false;
      return inicialized;
    }
  inicialized = window_init(width, height, title, gl_debug) && audio_init();
  return inicialized;
}

bool engine::window_init(int width, int height, std::string title,
                         bool gl_debug)
{
  window.width = width;
  window.height = height;

  if (gl_debug)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

  window.window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, width, height,
                                   SDL_WINDOW_OPENGL);

  if (window.window == nullptr)
    {
      const char* err_log{SDL_GetError()};
      std::cerr << "engine inicializer::Failed to create window: " << err_log;
      return false;
    }

  window.gl_context = SDL_GL_CreateContext(window.window);
  SDL_GL_MakeCurrent(window.window, window.gl_context);

  if (!gladLoadGLES2Loader(SDL_GL_GetProcAddress))
    {
      std::cerr << "engine inicializer: Failed to load Glad";
      return false;
    }

  glEnable(GL_DEPTH_TEST);
  return true;
}
bool engine::audio_init()
{
  if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
      std::cerr << "error: can't init audio: " << SDL_GetError();
      return false;
    }

  const char* device_name = nullptr;    // device name or nullptr
  const int32_t is_capture_device = 0;  // 0 - play device, 1 - microphone
  SDL_AudioSpec disired{.freq = 48000,
                        .format = AUDIO_S16LSB,
                        .channels = 2,  // stereo
                        .silence = 0,
                        .samples = 4096,  // must be power of 2
                        .padding = 0,
                        .size = 0,
                        .callback = sound::audio_callback,
                        .userdata = nullptr};

  SDL_AudioSpec returned{};

  const int32_t allow_changes = 0;

  audio.audio_device = SDL_OpenAudioDevice(device_name, is_capture_device,
                                           &disired, &returned, allow_changes);
  audio.audio_device_spec = returned;
  if (audio.audio_device == 0)
    {
      std::cerr << "error: failed to open audio device: " << SDL_GetError()
                << std::endl;
      return EXIT_FAILURE;
    }

  if (disired.format != returned.format ||
      disired.channels != returned.channels || disired.freq != returned.freq)
    {
      std::cerr << "error: disired != returned audio device settings!";
      return EXIT_FAILURE;
    }
  SDL_PauseAudioDevice(audio.audio_device, SDL_FALSE);
  return true;
}

void engine::dispose()
{
  if (!inicialized)
    {
      std::cerr << "engine:: inicialize error: engine is not inicialized"
                << std::endl;
      return;
    }
  SDL_CloseAudioDevice(audio.audio_device);

  SDL_GL_DeleteContext(window.gl_context);
  SDL_DestroyWindow(window.window);
  SDL_Quit();
  inicialized = false;
}

void engine::play(igame& game)
{
  if (!game.is_inicialized() && !game.inicialize())
    {
      return;
    }
  auto last_time = std::chrono::high_resolution_clock::now();
  while (game.is_playing())
    {
      auto current_time = std::chrono::high_resolution_clock::now();
      double duration =
          std::chrono::duration<double, std::milli>(current_time - last_time)
              .count();
      last_time = current_time;

      game.read_input(duration);
      game.update_data(duration);

      render::renderer::clear();

      game.render_output();

      SDL_GL_SwapWindow(window.window);
    }
}

}  // namespace core

namespace sound
{
void audio_callback(void*, Uint8* stream, int len)
{
  if (!sound::sound_buffer::current())
    {
      std::memset(stream, 0, static_cast<size_t>(len));
      return;
    }
  sound::sound_buffer& buffer{*sound::sound_buffer::current()};
  for (size_t it{0}; it < buffer.playback_list.size(); it++)
    {
      if (buffer.playback_list[it]->state == playback_state::on_delete)
        {
          buffer.playback_list.erase(buffer.playback_list.begin() + it);
          it--;
        }
    }
  std::remove_if(buffer.playback_list.begin(), buffer.playback_list.end(),
                 [](sound_buffer::playback_ptr& p) {
                   return p->state == playback_state::on_delete;
                 });
  std::memset(stream, buffer.silence, static_cast<size_t>(len));
  for (sound_buffer::playback_ptr& it : buffer.playback_list)
    {
      switch (it->state)
        {
          case playback_state::on_play:
            {
              const uint32_t delta = it->sound.get_duration() - it->position;
              const uint8_t* pos_ptr = &it->sound.get_data()[it->position];
              const uint32_t stream_length{static_cast<uint32_t>(len)};
              if (delta > 0)
                {
                  uint32_t length{delta > stream_length ? stream_length
                                                        : delta};

                  SDL_MixAudioFormat(
                      stream, pos_ptr,
                      core::engine::audio.audio_device_spec.format, length,
                      SDL_MIX_MAXVOLUME);

                  it->position += length;
                }
              else
                {
                  if (it->is_looped)
                    {
                      it->position = 0;
                    }
                  else
                    {
                      it->state = playback_state::on_delete;
                    }
                }
            }
            break;
          case playback_state::on_pause:
            break;
          default:
            break;
        }
    }
}
}  // namespace sound
