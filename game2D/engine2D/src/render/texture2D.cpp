#include "render/texture2D.h"
#include <iostream>
namespace render
{
texture2D::texture2D(int width_, int height_, const void* data,
                     color_format format, const GLenum filter,
                     const GLenum wrapMode)
    : height{height_}, width{width_}
{
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0,
               static_cast<GLenum>(format), GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

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
