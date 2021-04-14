#pragma once
#include <SDL.h>
#include "core/window.h"
namespace system_resources
{
struct window_data;
}

namespace core
{
class window_impl final : public window
{
 public:
  window_impl(const window_properties& properties);
  bool init();
  bool is_initialized() { return data.initialized; }

  const window_properties& get_properties() override;

  ~window_impl() override;

  void swap_buffers();

  window_impl(window_impl&&);
  window_impl& operator=(window_impl&&);

 private:
  struct window_data
  {
   public:
    window_data() {}
    SDL_Window* window{nullptr};
    SDL_GLContext gl_context{nullptr};
    bool initialized{false};

    ~window_data();
  };

  window_properties properties;
  window_data data{};
};
}  // namespace core
