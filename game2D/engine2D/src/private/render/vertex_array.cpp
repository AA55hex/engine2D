#include "render/vertex_array.h"
#include "render/vertex_buffer.h"
#include "render/vertex_buffer_descriptor.h"
namespace render
{
vertex_array::vertex_array()
{
  glGenVertexArrays(1, &id);
  glBindVertexArray(id);
}

void vertex_array::bind() const { glBindVertexArray(id); }
void vertex_array::detach() const { glBindVertexArray(0); }

void vertex_array::bind_vertex_buffer(
    const vertex_buffer& buffer, const vertex_buffer_descriptor& descriptor)
{
  glBindVertexArray(id);
  buffer.bind();

  unsigned int descriptor_size{
      static_cast<unsigned int>(descriptor.get_descriptors().size())};

  auto& descriptors{descriptor.get_descriptors()};

  GLbyte* offset{nullptr};

  for (unsigned int pointer{0}; pointer < descriptor_size; pointer++)
    {
      va_arrays_count++;
      auto& element{descriptors[pointer]};
      offset += element.size;

      glEnableVertexAttribArray(va_arrays_count);
      glVertexAttribPointer(va_arrays_count, element.size, GL_FLOAT,
                            element.normalized, descriptor.get_stride(),
                            offset);
    }
}

vertex_array::vertex_array(vertex_array&& vao)
{
  id = vao.id;
  vao.id = 0;
}
vertex_array& vertex_array::operator=(vertex_array&& vao)
{
  glDeleteVertexArrays(1, &id);

  id = vao.id;
  vao.id = 0;

  return *this;
}

vertex_array::~vertex_array() { glDeleteVertexArrays(1, &id); }
}  // namespace render
