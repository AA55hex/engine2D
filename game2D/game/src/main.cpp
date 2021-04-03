#include <glm/vec2.hpp>
#include <iostream>
#include "core/engine.h"
#include "core/igame.h"
#include "glm/vec2.hpp"
#include "input/input_event.h"
#include "input/input_manager.h"
#include "render/index_buffer.h"
#include "render/renderer.h"
#include "render/shader_program.h"
#include "render/sprite2D.h"
#include "render/sprite_animator.h"
#include "render/vertex_array.h"
#include "render/vertex_buffer.h"
#include "resources/resource_manager.h"
#include "sound/sound_buffer.h"

class game : public core::igame
{
 public:
  game() {}
  bool inicialize() override
  {
    int i{0};
    i++;
    auto snd{mgr.load_wav("bg_sound", "sound/highlands.wav")};
    mgr.load_wav("tank", "sound/tank.wav");
    snd_buffer.use();
    snd->play(0, true);

    auto prg{mgr.load_shader_program("test_prg", "shaders/test_shader.vert",
                                     "shaders/test_shader.frag")};
    auto tank_tex{mgr.load_texture_atlas2D(
        "texture", "textures/tanks.png",
        std::vector<std::string>{"test1", "test2", "test3"}, 16, 16)};
    auto tank_map_tex{mgr.load_texture2D("tank_map", "textures/tank_map.png")};

    auto s_tank{mgr.load_sprite("sprite", "texture", "test_prg", "test1")};
    auto s_map{mgr.load_sprite("tank_map", "tank_map", "test_prg")};

    animator = render::sprite_animator{s_tank};

    animator.add_frame(tank_tex->get_subtexture("test1"), 400);
    animator.add_frame(tank_tex->get_subtexture("test2"), 400);

    tank_settings.size = glm::vec2{2.f / 16, 2.f / 16};
    init = true;
    return init;
  }

  void read_input(double) override
  {
    using namespace input;
    input_event event{};
    while (input_manager::read_input(&event))
      {
        switch (event.type)
          {
            case event_type::quit:
              playing = false;
              break;
            case event_type::keyboard:
              {
                const float movespeed{0.005f};
                if (event.state == event_state::key_down)
                  {
                    switch (event.key)
                      {
                        case keyboard_key::w:
                          tank_move.x = movespeed;
                          break;
                        case keyboard_key::a:
                          tank_move.y = movespeed;
                          break;
                        case keyboard_key::s:
                          tank_move.z = movespeed;
                          break;
                        case keyboard_key::d:
                          tank_move.w = movespeed;
                          break;
                        default:
                          break;
                      }
                  }
                else
                  {
                    switch (event.key)
                      {
                        case keyboard_key::w:
                          tank_move.x = 0;
                          break;
                        case keyboard_key::a:
                          tank_move.y = 0;
                          break;
                        case keyboard_key::s:
                          tank_move.z = 0;
                          break;
                        case keyboard_key::d:
                          tank_move.w = 0;
                          break;
                        case keyboard_key::space:
                          mgr.get_wav("tank")->play();
                          break;
                        default:
                          break;
                      }
                  }
                break;
              }
            default:
              break;
          }
      }
  }
  void update_data(double duration) override
  {
    glm::vec2 movement{tank_move.w - tank_move.y, tank_move.x - tank_move.z};
    if (movement.x != 0.f || movement.y != 0.f)
      {
        float t_hipo{static_cast<float>(
            std::sqrt(movement.x * movement.x + movement.y * movement.y))};

        const float cos{movement.y / t_hipo};
        const float pi_rel{180.f / static_cast<float>(M_PI)};
        const float acos{std::acos(cos)};

        tank_settings.rotation = acos * pi_rel;
        if (movement.x > 0) tank_settings.rotation = -tank_settings.rotation;
        tank_settings.position += movement;
      }

    animator.update(duration);
  }
  void render_output() override
  {
    animator.render(tank_settings);
    mgr.get_sprite("tank_map")->render(map_settings);
  }

  bool is_playing() override { return playing; }
  bool is_inicialized() override { return init; }
  glm::vec4 tank_move{0};  // w-a-s-d
  ~game() override {}

 private:
  render::sprite_animator animator{nullptr};
  render::render_settings tank_settings{};

  const render::render_settings map_settings{glm::vec2{-1, -1},
                                             glm::vec2{2, 2}};

  resources::resource_manager mgr{"res/"};
  bool playing{true};
  bool init{false};

  sound::sound_buffer snd_buffer{};
  const glm::vec2 normal{0, 0.1f};
};

int main()
{
  using namespace core;
  engine::inicialize(640, 480, "test", true);
  // auto sound{mgr.load_wav("highlands", "sound/highlands.wav")};

  render::renderer::clear_color(0.f, 0.f, 0.f, 0.0f);
  game my_game{};
  engine::play(my_game);

  engine::dispose();
  return 0;
}
