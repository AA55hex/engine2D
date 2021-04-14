#include <render/sprite2D.h>
#include <render/sprite2D_impl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include "glad/glad.h"
#include "render/renderer.h"
#include "render/shader_program.h"
#include "render/texture2D.h"
#include "render/vertex_buffer_descriptor.h"
namespace render
{
sprite2D::sprite2D(std::shared_ptr<texture2D> texture_, std::string subtexture_,
                   std::shared_ptr<shader_program> program_)
    : program{program_}, texture{texture_}
{
  impl = std::make_unique<sprite2D_impl>();
  impl->vba = vertex_array{};
  impl->vba.bind();
  // spite impl->vba description
  float sprite_pos[]{0, 0, 0, 1, 1, 0, 1, 1};
  impl->sprite_vbo = vertex_buffer{sizeof(sprite_pos), sprite_pos};

  vertex_buffer_descriptor sprite_descriptor{};
  sprite_descriptor.add_element_descriptor_float(2, false);

  impl->vba.bind_vertex_buffer(impl->sprite_vbo, sprite_descriptor);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, nullptr);

  // texture impl->vba description
  frame_descriptor subtexture{texture_->get_subtexture(subtexture_)};
  frame = subtexture;
  float tex_pos[]{frame.left_bottom_uv.x, frame.left_bottom_uv.y,
                  frame.left_bottom_uv.x, frame.right_top_uv.y,
                  frame.right_top_uv.x,   frame.left_bottom_uv.y,
                  frame.right_top_uv.x,   frame.right_top_uv.y};
  impl->texture_vbo = vertex_buffer{sizeof(tex_pos), tex_pos};

  vertex_buffer_descriptor texture_descriptor{};
  texture_descriptor.add_element_descriptor_float(2, false);

  impl->vba.bind_vertex_buffer(impl->texture_vbo, texture_descriptor);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 2, nullptr);

  // index buffer description
  unsigned int indices[]{1, 0, 2, 3};
  impl->vebo = index_buffer{4, indices};
  impl->vebo.bind();

  impl->vba.detach();
  impl->vebo.detach();
}

void sprite2D::render(const render_settings& settings)
{
  glm::mat4x4 model{1};

  model = glm::translate(
      model, glm::vec3{settings.position.x + settings.size.x * 0.5f,
                       settings.position.y + settings.size.y * 0.5f, 0});
  model =
      glm::rotate(model, glm::radians(settings.rotation), glm::vec3{0, 0, 1});
  model = glm::translate(
      model, glm::vec3{settings.size.x * -0.5f, settings.size.y * -0.5f, 0});
  model = glm::scale(model, glm::vec3{settings.size, 1});

  impl->vba.bind();

  program->use();
  program->set_uniform("s_model", model);

  texture2D::active_texture(0);
  texture->bind();
  program->set_uniform("s_texture", 0);

  program->set_uniform("s_layer", settings.layer);

  renderer::draw(impl->vba, impl->vebo, *program, GL_TRIANGLE_FAN);
  impl->vba.detach();
  impl->vebo.detach();
}

void sprite2D::render(const render_settings& settings,
                      const frame_descriptor& f_discriptor)
{
  frame = f_discriptor;
  float tex_pos[]{frame.left_bottom_uv.x, frame.left_bottom_uv.y,
                  frame.left_bottom_uv.x, frame.right_top_uv.y,
                  frame.right_top_uv.x,   frame.left_bottom_uv.y,
                  frame.right_top_uv.x,   frame.right_top_uv.y};
  impl->texture_vbo.update(sizeof(tex_pos), tex_pos);

  glm::mat4x4 model{1};
  model = glm::translate(
      model, glm::vec3{settings.position.x + settings.size.x * 0.5f,
                       settings.position.y + settings.size.y * 0.5f, 0});
  model =
      glm::rotate(model, glm::radians(settings.rotation), glm::vec3{0, 0, 1});
  model = glm::translate(
      model, glm::vec3{settings.size.x * -0.5f, settings.size.y * -0.5f, 0});
  model = glm::scale(model, glm::vec3{settings.size, 1});

  impl->vba.bind();

  program->use();
  program->set_uniform("s_model", model);

  texture2D::active_texture(0);
  texture->bind();

  program->set_uniform("s_layer", settings.layer);

  renderer::draw(impl->vba, impl->vebo, *program, GL_TRIANGLE_FAN);
  impl->vba.detach();
  impl->vebo.detach();
}

sprite2D::~sprite2D() {}
}  // namespace render
