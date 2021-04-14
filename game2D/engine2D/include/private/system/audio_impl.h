#pragma once
#include <SDL.h>
#include <core/audio.h>
namespace core
{
class audio_impl final : public audio
{
 public:
  audio_impl(const audio_properties& properties);
  bool init();
  ~audio_impl() override;

  audio_impl(audio_impl&&);
  audio_impl& operator=(audio_impl&&);
  bool is_initialized() { return data.initialized; }

  void lock() { SDL_LockAudioDevice(data.audio_device); }
  void unlock() { SDL_UnlockAudioDevice(data.audio_device); }

 private:
  struct audio_data
  {
    SDL_AudioDeviceID audio_device{};
    SDL_AudioSpec specification{};
    bool initialized{false};
    ~audio_data();
  };
  audio_data data{};
  audio_properties properties{};

  static void audio_callback(void* userdata, Uint8* stream, int len);
};

}  // namespace core
