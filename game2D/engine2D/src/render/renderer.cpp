#include "render/renderer.h"
#include "core/engine.h"
#include "render/index_buffer.h"
#include "render/shader_program.h"
#include "render/vertex_array.h"
namespace render
{
void renderer::draw(const vertex_array& vao, const index_buffer& vebo,
                    const shader_program& program, GLenum mode)
{
  program.use();
  vao.bind();
  vebo.bind();

  glDrawElements(mode, static_cast<int>(vebo.get_count()), GL_UNSIGNED_INT,
                 nullptr);
}
void renderer::set_viewport(int x, int y, int width, int height)
{
  glViewport(x, y, width, height);
}
void renderer::clear_color(float r, float g, float b, float a)
{
  glClearColor(r, g, b, a);
}
void renderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

// void renderer::swap_buffers() { SDL_GL_SwapWindow(engine::window::window); }
}  // namespace render
