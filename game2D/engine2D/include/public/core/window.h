#pragma once
#include <string>

namespace core
{
struct window_properties
{
  int width{0};
  int height{0};
  std::string title{0};
  bool debug_enabled{false};
};

class window
{
 public:
  virtual const window_properties& get_properties() = 0;

  window(window&) = delete;
  window& operator=(window&) = delete;

 protected:
  window() = default;
  virtual ~window() = default;
};
}  // namespace core
