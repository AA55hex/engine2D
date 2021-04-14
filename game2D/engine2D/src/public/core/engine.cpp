#include <core/engine.h>
#include <core/engine_impl.h>
#include <core/igame.h>
#include <glad/glad.h>
#include <render/renderer.h>
#include <sound/sound_buffer.h>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <string>

namespace core
{
std::unique_ptr<engine::engine_impl> engine::impl{nullptr};

bool engine::init(const window_properties& window_prop,
                  const audio_properties& audio_prop)
{
  const int init_result = SDL_Init(
      SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS |
      SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER);
  if (init_result != 0)
    {
      const char* err_message = SDL_GetError();
      std::cerr << "engine:: inicialize error: failed call SDL_Init: "
                << err_message << std::endl;
      initialized = false;
      return true;
    }

  if (initialized)
    {
      std::cerr << "engine:: init error: already inicialized" << std::endl;
      return false;
    }

  impl = std::make_unique<engine_impl>(window_prop, audio_prop);
  if (!impl->window.init())
    {
      impl.release();
      std::cerr << "engine:: init error: window init failure" << std::endl;
      return false;
    }
  if (!impl->audio.init())
    {
      impl.release();
      std::cerr << "engine:: init error: can't audio init failure" << std::endl;
      return false;
    }
  initialized = true;
  std::clog << "engine::init: completed" << std::endl;
  return true;
}

void engine::dispose()
{
  if (!initialized)
    {
      std::cerr << "engine:: dispose error: engine is not inicialized"
                << std::endl;
      return;
    }
  impl.release();
  SDL_Quit();
  initialized = false;
  std::clog << "engine::dispose: engine has been disposed" << std::endl;
}

void engine::launch(igame& game)
{
  if (!initialized)
    {
      std::cerr << "engine:: launch failure: engine is not initialized"
                << std::endl;
      return;
    }
  if (!game.is_inicialized() && !game.inicialize())
    {
      std::cerr << "engine:: launch failure: can't init game object"
                << std::endl;
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

      impl->window.swap_buffers();
    }
}

window& engine::get_window() { return impl->window; }
audio& engine::get_audio() { return impl->audio; }

}  // namespace core
