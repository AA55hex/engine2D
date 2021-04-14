#pragma once
#include <string>
namespace core
{
struct audio_properties
{
  int freq{48000};
  std::uint8_t channels{2};
  std::uint8_t silence{0};
  std::uint16_t samples{4096};  // must be power of 2
  std::uint16_t padding{0};
  std::uint32_t size{0};
};

class audio
{
 public:
  audio(audio&) = delete;
  audio& operator=(audio&) = delete;

 protected:
  audio() = default;
  virtual ~audio() = default;
};
}  // namespace core
