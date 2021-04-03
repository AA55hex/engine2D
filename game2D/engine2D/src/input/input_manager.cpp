#include <SDL.h>
#include <input/input_manager.h>
#include <algorithm>
#include <array>
#include "core/engine.h"
namespace input
{
static bool translate_key(SDL_KeyboardEvent event, keyboard_key& result);
static bool translate_key(SDL_MouseButtonEvent event, mouse_button& result);

bool input_manager::read_input(input_event* event)
{
  SDL_Event src_event;
  while (SDL_PollEvent(&src_event))
    {
      switch (src_event.type)
        {
          case SDL_QUIT:
            {
              event->type = event_type::quit;
              event->state = event_state::none;
              event->key = keyboard_key::none;
              event->mouse = mouse_button::none;
              return true;
            }
          case SDL_KEYDOWN:
          case SDL_KEYUP:
            {
              if (src_event.key.repeat == 0)
                {
                  event->type = event_type::keyboard;
                  event->state = src_event.type == SDL_KEYDOWN
                                     ? event_state::key_down
                                     : event_state::key_up;
                  translate_key(src_event.key, event->key);
                  event->mouse = mouse_button::none;
                  return true;
                }
              break;
            }
          case SDL_MOUSEBUTTONDOWN:
          case SDL_MOUSEBUTTONUP:
            {
              event->type = event_type::mouse;
              event->state = src_event.type == SDL_MOUSEBUTTONDOWN
                                 ? event_state::key_down
                                 : event_state::key_up;
              event->key = keyboard_key::none;
              translate_key(src_event.button, event->mouse);
              return true;
            }
          case SDL_MOUSEMOTION:
            {
              event->type = event_type::mouse;
              event->state = event_state::motion;
              event->key = keyboard_key::none;
              event->mouse = mouse_button::none;
              return true;
            }
          default:
            break;
        }
    }

  return false;
}

glm::vec2 input_manager::get_mouse_state()
{
  int x, y;
  const float window_width{static_cast<float>(core::engine::window.width)};
  const float window_height{static_cast<float>(core::engine::window.height)};

  SDL_GetMouseState(&x, &y);

  const float x_norm{(x * 2) / window_width - 1.f};
  const float y_norm{(y * 2) / window_height - 1.f};

  glm::vec2 mouse_state{x_norm, y_norm};
  return mouse_state;
}

struct bind
{
  int engine_key;
  int sdl_key;
};

const std::array<bind, 7> keyboard_relation{
    {{static_cast<int>(keyboard_key::q), SDL_SCANCODE_Q},
     {static_cast<int>(keyboard_key::w), SDL_SCANCODE_W},
     {static_cast<int>(keyboard_key::e), SDL_SCANCODE_E},
     {static_cast<int>(keyboard_key::a), SDL_SCANCODE_A},
     {static_cast<int>(keyboard_key::s), SDL_SCANCODE_S},
     {static_cast<int>(keyboard_key::d), SDL_SCANCODE_D},
     {static_cast<int>(keyboard_key::space), SDL_SCANCODE_SPACE}}};

static bool translate_key(SDL_KeyboardEvent event, keyboard_key& result)
{
  auto it = std::find_if(
      keyboard_relation.begin(), keyboard_relation.end(),
      [&](const bind& buff) { return buff.sdl_key == event.keysym.scancode; });
  if (it == keyboard_relation.end())
    {
      result = keyboard_key::none;
      return false;
    }
  result = keyboard_key(it->engine_key);
  return true;
}

const std::array<bind, 6> mouse_relation{
    {{static_cast<int>(mouse_button::button_left), SDL_SCANCODE_Q},
     {static_cast<int>(mouse_button::button_middle), SDL_SCANCODE_W},
     {static_cast<int>(mouse_button::button_right), SDL_SCANCODE_E},
     {static_cast<int>(mouse_button::button_x1), SDL_SCANCODE_A},
     {static_cast<int>(mouse_button::button_x2), SDL_SCANCODE_S}}};

static bool translate_key(SDL_MouseButtonEvent event, mouse_button& result)
{
  auto it = std::find_if(
      mouse_relation.begin(), mouse_relation.end(),
      [&](const bind& buff) { return buff.sdl_key == event.button; });
  if (it == mouse_relation.end())
    {
      result = mouse_button::none;
      return false;
    }
  result = mouse_button(it->engine_key);
  return true;
}

}  // namespace input
