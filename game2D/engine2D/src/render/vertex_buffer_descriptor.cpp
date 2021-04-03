#include "render/vertex_buffer_descriptor.h"

namespace render
{
void vertex_buffer_descriptor::add_element_descriptor_float(int count,
                                                            bool normalized)
{
  const int size{count * static_cast<int>(sizeof(GLfloat))};
  descriptors.push_back({count, GL_FLOAT, normalized, size});
  stride += size;
}

}  // namespace render
