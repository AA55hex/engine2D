#include "render/sprite_animator.h"
#include "render/sprite2D.h"

namespace render
{
sprite_animator::sprite_animator(std::shared_ptr<sprite2D> sprite_)
    : sprite{sprite_}
{
}

void sprite_animator::update(double duration)
{
  if (frames.size() > 0)
    {
      current_duration += duration;
      double max_duration{frames[current_frame_index].duration};
      //  while (current_duration >= max_duration)
      //    {
      //      current_duration -= max_duration;
      //      current_frame_index = (current_frame_index + 1) % frames.size();
      //      double max_duration = frames[current_frame_index].duration;
      //    }

      if (current_duration >= max_duration)
        {
          current_duration -= max_duration;
          current_frame_index++;
          current_frame_index %= frames.size();
        }
    }
}
void sprite_animator::render(const render_settings& settings)
{
  if (frames.size() > 0)
    sprite->render(settings, frames[current_frame_index].discriptor);
  else
    sprite->render(settings);
}

void sprite_animator::add_frame(const frame_descriptor& discriptor,
                                double duration)
{
  frames.push_back({discriptor, duration});
  summary_duration += duration;
}
void sprite_animator::clear_frames()
{
  frames.clear();
  summary_duration = 0;
  current_frame_index = 0;
}

const sprite_animator::frame* sprite_animator::current_frame()
{
  return frames.size() > 0 ? &frames[current_frame_index] : nullptr;
}

}  // namespace render
