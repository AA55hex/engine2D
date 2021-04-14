#include "sound/sound_buffer.h"
#include <SDL.h>
#include <core/engine.h>
#include <core/engine_impl.h>
#include <algorithm>
#include <cstring>
namespace sound
{
sound_buffer::sound_buffer(int silence_) : silence{silence_} {}

void sound_buffer::use() { current_buffer = this; }
void sound_buffer::detach() { current_buffer = nullptr; }

void sound_buffer::push_playback(playback_ptr playback)
{
  core::engine::impl->audio.lock();

  playback_list.push_back(playback);

  core::engine::impl->audio.unlock();
}

void sound_buffer::clear()
{
  core::engine::impl->audio.lock();

  playback_list.clear();

  core::engine::impl->audio.unlock();
}

sound_buffer::~sound_buffer()
{
  if (current_buffer == this)
    {
      detach();
    }
}

}  // namespace sound
