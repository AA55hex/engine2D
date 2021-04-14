#pragma once
#include <core/engine.h>
#include <system/audio_impl.h>
#include <system/window_impl.h>
namespace core
{
struct engine::engine_impl
{
  engine_impl(const window_properties& window_prop,
              const audio_properties& audio_prop)
      : window{window_prop}, audio{audio_prop}
  {
  }
  window_impl window;
  audio_impl audio;

  ~engine_impl() {}
};
}  // namespace core
