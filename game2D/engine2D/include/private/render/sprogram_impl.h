#pragma once
#include <glad/glad.h>
#include <render/shader_program.h>
namespace render
{
struct shader_program::sprogram_impl
{
  bool create_shader(const std::string& source, GLenum s_type,
                     GLuint& shader_id);
  bool compile_status{false};
  GLuint id{0};
};
}  // namespace render
