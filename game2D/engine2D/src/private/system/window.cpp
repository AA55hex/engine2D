#include <glad/glad.h>
#include <render/renderer.h>
#include <system/window_impl.h>
#include <cassert>
#include <iostream>
namespace core
{
window_impl::window_impl(const window_properties& properties_)
    : properties{properties_}
{
}

bool window_impl::init()
{
  if (properties.debug_enabled)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

  data.window = SDL_CreateWindow(
      properties.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      properties.width, properties.height, SDL_WINDOW_OPENGL);

  if (!data.window)
    {
      const char* err_log{SDL_GetError()};
      std::cerr << "window inicializer:: Failed to create window: " << err_log;
      return false;
    }

  data.gl_context = SDL_GL_CreateContext(data.window);
  if (!data.gl_context)
    {
      const char* err_log{SDL_GetError()};
      std::cerr << "window inicializer:: Failed to create window context: "
                << err_log;
      SDL_DestroyWindow(data.window);
      return false;
    }

  SDL_GL_MakeCurrent(data.window, data.gl_context);

  /// TODO - move in another class
  if (!gladLoadGLES2Loader(SDL_GL_GetProcAddress))
    {
      std::cerr << "window inicializer:: Failed to load Glad" << std::endl;
      SDL_DestroyWindow(data.window);
      SDL_GL_DeleteContext(data.gl_context);
      return false;
    }

  glEnable(GL_DEPTH_TEST);
  render::renderer::clear_color(0.f, 0.f, 0.f, 0.f);
  /// TODO
  std::clog << "window inicializer::inicialization completed" << std::endl;
  data.initialized = true;
  return true;
}

const window_properties& window_impl::get_properties() { return properties; }

window_impl::~window_impl() {}

window_impl::window_impl(window_impl&& window)
{
  properties = window.properties;
  data = window.data;

  window.data.initialized = false;
}

void window_impl::swap_buffers()
{
  assert(data.initialized);
  SDL_GL_SwapWindow(data.window);
}

window_impl& window_impl::operator=(window_impl&& window)
{
  if (data.initialized)
    {
      SDL_GL_DeleteContext(data.gl_context);
      SDL_DestroyWindow(data.window);
    }

  properties = window.properties;
  data = window.data;

  window.data.initialized = false;

  return *this;
}

window_impl::window_data::~window_data()
{
  if (initialized)
    {
      SDL_GL_DeleteContext(gl_context);
      SDL_DestroyWindow(window);
    }
}

}  // namespace core
