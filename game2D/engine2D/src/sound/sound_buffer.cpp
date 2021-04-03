#include "sound/sound_buffer.h"
#include <SDL.h>
#include <core/engine.h>
#include <algorithm>
#include <cstring>
namespace sound
{
sound_buffer::sound_buffer(int silence_) : silence{silence_} {}

void sound_buffer::use() { current_buffer = this; }
void sound_buffer::detach() { current_buffer = nullptr; }

void sound_buffer::push_playback(playback_ptr playback)
{
  SDL_LockAudioDevice(core::engine::audio.audio_device);

  playback_list.push_back(playback);

  SDL_UnlockAudioDevice(core::engine::audio.audio_device);
}

void sound_buffer::clear()
{
  SDL_LockAudioDevice(core::engine::audio.audio_device);

  playback_list.clear();

  SDL_UnlockAudioDevice(core::engine::audio.audio_device);
}

sound_buffer::~sound_buffer()
{
  if (current_buffer == this)
    {
      detach();
    }
}

}  // namespace sound
