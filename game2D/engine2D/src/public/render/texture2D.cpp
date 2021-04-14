#include "render/texture2D.h"
#include <glad/glad.h>
#include <hash_set>
#include <iostream>
namespace render
{
GLenum get_gl_color(color_format format)
{
  switch (format)
    {
      case color_format::red:
        return GL_RED;
      case color_format::green:
        return GL_GREEN;
      case color_format::blue:
        return GL_BLUE;
      case color_format::alpha:
        return GL_ALPHA;
      case color_format::rgb:
        return GL_RGB;
      case color_format::rgba:
        return GL_RGBA;
    }
  return GL_NONE;
}

GLint get_gl_filter(texture_filter filter)
{
  switch (filter)
    {
      case texture_filter::linear:
        return GL_NEAREST;
      case texture_filter::nearest:
        return GL_LINEAR;
    }
  return GL_NONE;
}

GLint get_gl_wrap_mode(wrap_mode mode)
{
  switch (mode)
    {
      case wrap_mode::clamp_to_edge:
        return GL_CLAMP_TO_EDGE;
      case wrap_mode::mirrored_repeat:
        return GL_MIRRORED_REPEAT;
      case wrap_mode::repeat:
        return GL_REPEAT;
    }
  return GL_NONE;
}

texture2D::texture2D(int width_, int height_, const void* data,
                     const color_format format, const texture_filter filter,
                     const wrap_mode w_mode)
    : height{height_}, width{width_}
{
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  GLenum gl_color{get_gl_color(format)};

  glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(gl_color), width, height, 0,
               gl_color, GL_UNSIGNED_BYTE, data);

  GLint gl_wrap_mode{get_gl_wrap_mode(w_mode)};
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrap_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrap_mode);

  GLint gl_filter{get_gl_filter(filter)};
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter);

  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void texture2D::bind() const { glBindTexture(GL_TEXTURE_2D, id); }

int texture2D::get_width() const { return width; }
int texture2D::get_height() const { return height; }

texture2D::~texture2D() { glDeleteTextures(1, &id); }

texture2D::texture2D(texture2D&& texture)
{
  id = texture.id;
  width = texture.width;
  height = texture.height;

  texture.id = 0;
}

texture2D& texture2D::operator=(texture2D&& texture)
{
  glDeleteTextures(1, &id);

  id = texture.id;
  width = texture.width;
  height = texture.height;

  texture.id = 0;

  return *this;
}

void texture2D::active_texture(unsigned int offset)
{
  GLenum buffer{GL_TEXTURE0 + offset};
  glActiveTexture(buffer);
}

const frame_descriptor& texture2D::add_subtexture(
    std::string key, const frame_descriptor& subtexture)
{
  subtexture_map[key] = subtexture;
  return subtexture_map[key];
}
const frame_descriptor& texture2D::add_subtexture(std::string key,
                                                  glm::vec2 left_bottom_uv,
                                                  glm::vec2 right_top_uv)
{
  subtexture_map[key] = frame_descriptor{left_bottom_uv, right_top_uv};
  return subtexture_map[key];
}
const frame_descriptor& texture2D::get_subtexture(std::string key) const
{
  auto it = subtexture_map.find(key);

  if (it == subtexture_map.end()) return default_texture;

  return it->second;
}

}  // namespace render
