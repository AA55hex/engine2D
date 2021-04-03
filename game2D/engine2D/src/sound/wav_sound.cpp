#include <sound/sound_buffer.h>
#include <sound/wav_sound.h>
namespace sound
{
wav_sound::wav_sound(uint8_t* data, uint32_t dur) : data{data}, duration{dur} {}

std::shared_ptr<wav_sound::playback> wav_sound::play(uint32_t pos, bool loop,
                                                     playback_state state)
{
  auto result{std::make_shared<wav_sound::playback>(*this, pos, loop, state)};
  if (sound_buffer::current()) sound_buffer::current()->push_playback(result);
  return result;
}

wav_sound::~wav_sound() { SDL_FreeWAV(data); }

wav_sound::wav_sound(wav_sound&& wav)
{
  data = wav.data;
  duration = wav.duration;

  wav.data = nullptr;
}
wav_sound& wav_sound::operator=(wav_sound&& wav)
{
  SDL_FreeWAV(data);

  data = wav.data;
  duration = wav.duration;

  wav.data = nullptr;
  return *this;
}

wav_sound::playback::playback(wav_sound& sound_, uint32_t pos, bool loop,
                              playback_state state_)
    : sound{sound_}, position{pos}, state{state_}, is_looped{loop}
{
}

void wav_sound::playback::replay()
{
  position = 0;
  state = playback_state ::on_play;
}

}  // namespace sound
