#pragma once
#include <render/index_buffer.h>
#include <render/sprite2D.h>
#include <render/vertex_array.h>
#include <render/vertex_buffer.h>
namespace render
{
struct sprite2D::sprite2D_impl
{
  vertex_array vba;
  vertex_buffer sprite_vbo;
  vertex_buffer texture_vbo;
  index_buffer vebo;
};
}  // namespace render
