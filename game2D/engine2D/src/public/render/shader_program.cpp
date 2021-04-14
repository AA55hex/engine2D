#include <glad/glad.h>
#include <render/shader_program.h>
#include <render/sprogram_impl.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
namespace render
{
shader_program::shader_program(const std::string& vert_src,
                               const std::string& frag_src)
{
  impl = std::make_unique<sprogram_impl>();
  GLuint v_shader;
  if (!impl->create_shader(vert_src, GL_VERTEX_SHADER, v_shader))
    {
      std::cerr << "Shader program: vertex shader compile time error";
    }
  GLuint f_shader;
  if (!impl->create_shader(frag_src, GL_FRAGMENT_SHADER, f_shader))
    {
      std::cerr << "Shader program: fragment shader compile time error"
                << std::endl;
    }

  impl->id = glCreateProgram();

  glAttachShader(impl->id, v_shader);
  glAttachShader(impl->id, f_shader);

  glLinkProgram(impl->id);

  GLint linked;
  glGetProgramiv(impl->id, GL_LINK_STATUS, &linked);
  if (!linked)
    {
      GLint log_length{0};
      glGetProgramiv(impl->id, GL_INFO_LOG_LENGTH, &log_length);
      if (log_length != 0)
        {
          char* info_log{new char[static_cast<unsigned long>(log_length)]};
          std::cerr << "Shader program linker error:" << std::endl
                    << info_log << std::endl;
        }
    }
  else
    {
      impl->compile_status = true;
    }

  glDeleteShader(v_shader);
  glDeleteShader(f_shader);
}

bool shader_program::is_compiled() const { return impl->compile_status; }
void shader_program::use() const { glUseProgram(impl->id); }

shader_program& shader_program::operator=(shader_program&& prg)
{
  glDeleteShader(impl->id);

  impl->id = prg.impl->id;
  impl->compile_status = prg.impl->compile_status;

  prg.impl->id = 0;
  prg.impl->compile_status = false;
  return *this;
}

shader_program::shader_program(shader_program&& prg)
{
  impl->id = prg.impl->id;
  impl->compile_status = prg.impl->compile_status;

  prg.impl->id = 0;
  prg.impl->compile_status = false;
}

shader_program::~shader_program() { glDeleteShader(impl->id); }

bool shader_program::sprogram_impl::create_shader(const std::string& source,
                                                  GLenum s_type,
                                                  GLuint& shader_id)
{
  shader_id = glCreateShader(s_type);
  const char* src{source.c_str()};
  glShaderSource(shader_id, 1, &src, nullptr);
  glCompileShader(shader_id);

  GLint compiled;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);

  if (!compiled)
    {
      GLint log_length{0};
      glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

      if (log_length != 0)
        {
          char* info_log{new char[static_cast<unsigned long>(log_length)]};
          glGetShaderInfoLog(shader_id, log_length, nullptr, info_log);
          std::cerr << "Shader compile time error :" << std::endl
                    << info_log << std::endl;
        }
      return false;
    }
  return true;
}

bool shader_program::set_uniform(std::string u_name, int value)
{
  GLint u_id{glGetUniformLocation(impl->id, u_name.c_str())};

  if (u_id == -1)
    {
      std::cerr << "Can't find uniform: " << u_name << std::endl;
      return false;
    }

  glUniform1i(u_id, value);

  return true;
}

bool shader_program::set_uniform(std::string u_name, const glm::mat4x4& value)
{
  GLint u_id{glGetUniformLocation(impl->id, u_name.c_str())};

  if (u_id == -1)
    {
      std::cerr << "Can't find mat4x4-uniform: " << u_name << std::endl;
      return false;
    }

  glUniformMatrix4fv(u_id, 1, GL_FALSE, glm::value_ptr(value));

  return true;
}

}  // namespace render
