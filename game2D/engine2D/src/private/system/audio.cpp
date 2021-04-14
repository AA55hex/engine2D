#include <sound/sound_buffer.h>
#include <sound/wav_sound.h>
#include <system/audio_impl.h>
#include <algorithm>
#include <cstring>
#include <iostream>
namespace core
{
audio_impl::audio_impl(const audio_properties& properties_)
    : properties{properties_}
{
}
bool audio_impl::init()
{
  // TODO: move to engine class
  if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
      std::cerr << "error: can't init audio module: " << SDL_GetError();
      return false;
    }

  const char* device_name = nullptr;    // device name or nullptr
  const int32_t is_capture_device = 0;  // 0 - play device, 1 - microphone
  SDL_AudioSpec disired{.freq = properties.freq,
                        .format = AUDIO_S16LSB,
                        .channels = properties.channels,
                        .silence = properties.silence,
                        .samples = properties.samples,
                        .padding = properties.padding,
                        .size = properties.size,
                        .callback = audio_callback,
                        .userdata = &data};

  SDL_AudioSpec returned{};

  const int32_t allow_changes = 0;

  data.audio_device = SDL_OpenAudioDevice(device_name, is_capture_device,
                                          &disired, &returned, allow_changes);
  data.specification = returned;
  if (data.audio_device == 0)
    {
      std::cerr << "audio inicializer:: failed to open audio device: "
                << SDL_GetError() << std::endl;
      return false;
    }

  if (disired.format != returned.format ||
      disired.channels != returned.channels || disired.freq != returned.freq)
    {
      std::clog << "audio inicializer:: disired_properties != created audio "
                   "device settings!"
                << std::endl;
    }
  SDL_PauseAudioDevice(data.audio_device, SDL_FALSE);

  std::clog << "audio inicializer::inicialization completed" << std::endl;
  data.initialized = true;
  return true;
}
audio_impl::~audio_impl() {}

audio_impl::audio_impl(audio_impl&& audio)
{
  properties = audio.properties;
  data = audio.data;

  audio.data.initialized = false;
}
audio_impl& audio_impl::operator=(audio_impl&& audio)
{
  if (data.initialized) SDL_CloseAudioDevice(data.audio_device);
  properties = audio.properties;
  data = audio.data;

  audio.data.initialized = false;
  return *this;
}

audio_impl::audio_data::~audio_data()
{
  if (initialized) SDL_CloseAudioDevice(audio_device);
}

void audio_impl::audio_callback(void* userdata, Uint8* stream, int len)
{
  if (!sound::sound_buffer::current())
    {
      std::memset(stream, 0, static_cast<size_t>(len));
      return;
    }
  audio_data& data = *reinterpret_cast<audio_data*>(userdata);
  sound::sound_buffer& buffer{*sound::sound_buffer::current()};
  for (size_t it{0}; it < buffer.playback_list.size(); it++)
    {
      if (buffer.playback_list[it]->state == sound::playback_state::on_delete)
        {
          buffer.playback_list.erase(buffer.playback_list.begin() +
                                     static_cast<int>(it));
          it--;
        }
    }
  std::remove_if(buffer.playback_list.begin(), buffer.playback_list.end(),
                 [](sound::sound_buffer::playback_ptr& p) {
                   return p->state == sound::playback_state::on_delete;
                 });
  std::memset(stream, buffer.silence, static_cast<size_t>(len));
  for (sound::sound_buffer::playback_ptr& it : buffer.playback_list)
    {
      switch (it->state)
        {
          case sound::playback_state::on_play:
            {
              const uint32_t delta = it->sound.get_duration() - it->position;
              const uint8_t* pos_ptr = &it->sound.get_data()[it->position];
              const uint32_t stream_length{static_cast<uint32_t>(len)};
              if (delta > 0)
                {
                  uint32_t length{delta > stream_length ? stream_length
                                                        : delta};

                  SDL_MixAudioFormat(stream, pos_ptr, data.specification.format,
                                     length, SDL_MIX_MAXVOLUME);

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
                      it->state = sound::playback_state::on_delete;
                    }
                }
            }
            break;
          case sound::playback_state::on_pause:
            break;
          default:
            break;
        }
    }
}
}  // namespace core
