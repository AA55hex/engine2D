#include "resources/resource_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "resources/stb_image.h"
namespace resources
{
resource_manager::resource_manager(const std::string& dir_path)
{
  path_configure(dir_path);
}

void resource_manager::reset_path(const std::string& dir_path)
{
  path_configure(dir_path);
}

void resource_manager::path_configure(const std::string& dir_path)
{
  size_t last_pos{dir_path.find_last_of("/\\")};
  path = dir_path;
  if (path.length() != 0 && path.length() - 1 != last_pos)
    {
      path += "/";
    }
}

std::shared_ptr<render::shader_program> resource_manager::load_shader_program(
    const std::string& prg_name, const std::string& vert_path,
    const std::string& frag_path)
{
  std::string vert_src{get_file_data(vert_path)};
  if (vert_src.empty())
    {
      std::cerr << "Failed to load vertex shader" << std::endl;
    }
  std::string frag_src{get_file_data(frag_path)};
  if (frag_src.empty())
    {
      std::cerr << "Failed to load fragment shader" << std::endl;
    }
  shader_program_map[prg_name] =
      std::make_shared<render::shader_program>(vert_src, frag_src);

  std::shared_ptr<render::shader_program> result{shader_program_map[prg_name]};

  if (!result->is_compiled())
    {
      std::cerr << "Failed to load shader program <" << prg_name << ">"
                << std::endl
                << "> Vertex shader: " << vert_path << std::endl
                << "> Fragment shader: " << frag_path << std::endl;
      return nullptr;
    }

  return result;
}
std::shared_ptr<render::shader_program> resource_manager::get_shader_program(
    const std::string& prg_name)
{
  auto it{shader_program_map.find(prg_name)};
  if (it == shader_program_map.end())
    {
      std::cerr << "resource_manager: shader program <" << prg_name
                << "> not found" << std::endl;
      return nullptr;
    }
  return it->second;
}

std::string resource_manager::get_file_data(const std::string& additional)
{
  std::string filepath{path + additional};
  std::ifstream file{filepath, std::ios::binary};
  if (!file.is_open())
    {
      std::cerr << "Resource manager: wrong filepath " << filepath << std::endl;
      return "";
    }
  std::stringstream buff{};
  buff << file.rdbuf();
  return buff.str();
}

std::shared_ptr<render::texture2D> resource_manager::load_texture2D(
    const std::string& texture_name, const std::string& filepath)
{
  int width, height, channels;
  std::string full_path = path + filepath;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data =
      stbi_load(full_path.c_str(), &width, &height, &channels, 0);
  if (data == nullptr)
    {
      std::cerr << "Resource manager: can't to load texture " << texture_name
                << std::endl
                << "Filepath: " << filepath << std::endl;
      return nullptr;
    }
  render::color_format format;
  switch (channels)
    {
      case 3:
        format = render::color_format::rgb;
        break;
      case 4:
        format = render::color_format::rgba;
        break;
      default:
        format = render::color_format::rgba;
        break;
    }
  texture_map[texture_name] =
      std::make_shared<render::texture2D>(width, height, data, format);
  stbi_image_free(data);
  return texture_map[texture_name];
}
std::shared_ptr<render::texture2D> resource_manager::get_texture2D(
    const std::string& texture_name)
{
  auto it = texture_map.find(texture_name);
  if (it == texture_map.end())
    {
      std::cerr << "Resource_manager: texture <" << texture_name
                << "> not found" << std::endl;
      return nullptr;
    }
  return it->second;
}

std::shared_ptr<sound::wav_sound> resource_manager::load_wav(
    const std::string& wav_name, const std::string& filepath)
{
  using namespace std;

  std::string full_path = path + filepath;

  SDL_AudioSpec audio_spec_from_file{};
  uint8_t* sample_buffer_from_file = nullptr;
  uint32_t sample_buffer_len_from_file = 0;

  clog << "loading sample buffer from file: " << filepath << endl;

  if (SDL_LoadWAV(full_path.c_str(), &audio_spec_from_file,
                  &sample_buffer_from_file,
                  &sample_buffer_len_from_file) == nullptr)
    {
      cerr << "Resource_manager:: audio load runtime error: can't parse and "
              "load audio samples from file\n";
      return nullptr;
    }

  clog << "audio buffer from file size: " << sample_buffer_len_from_file
       << " B (" << sample_buffer_len_from_file / double(1024 * 1024) << ") Mb"
       << endl;

  wav_map[wav_name] = std::make_shared<sound::wav_sound>(
      sample_buffer_from_file, sample_buffer_len_from_file);

  return wav_map[wav_name];
}
std::shared_ptr<sound::wav_sound> resource_manager::get_wav(
    const std::string& wav_name)
{
  auto it = wav_map.find(wav_name);
  if (it == wav_map.end())
    {
      std::cerr << "Resource_manager: wav_sound <" << wav_name << "> not found"
                << std::endl;
      return nullptr;
    }
  return it->second;
}

std::shared_ptr<render::texture2D> resource_manager::load_texture_atlas2D(
    const std::string& texture_name, const std::string& filepath,
    std::vector<std::string> subtexture_names, unsigned int frame_width,
    unsigned int frame_height)
{
  auto texture{load_texture2D(texture_name, filepath)};
  if (!texture)
    {
      return nullptr;
    }
  render::frame_descriptor buff{};

  const float offset_fix{0.1f};

  const unsigned int tex_width{static_cast<unsigned int>(texture->get_width())};
  const unsigned int tex_height{
      static_cast<unsigned int>(texture->get_height())};

  unsigned int current_offset_x{0};
  unsigned int current_offset_y{tex_height};

  for (const std::string& sub_name : subtexture_names)
    {
      buff.left_bottom_uv = glm::vec2{
          (static_cast<float>(current_offset_x) + offset_fix) /
              static_cast<float>(tex_width),
          (static_cast<float>(current_offset_y - frame_height) + offset_fix) /
              static_cast<float>(tex_height)};
      buff.right_top_uv = glm::vec2{
          (static_cast<float>(current_offset_x + frame_width) - offset_fix) /
              static_cast<float>(tex_width),
          (static_cast<float>(current_offset_y) - offset_fix) /
              static_cast<float>(tex_height)};

      current_offset_x += frame_width;
      if (current_offset_x >= tex_width)
        {
          current_offset_x = 0;
          current_offset_y -= frame_height;
        }

      texture->add_subtexture(sub_name, buff);
    }
  return texture;
}

std::shared_ptr<render::sprite2D> resource_manager::load_sprite(
    const std::string& sprite_name, const std::string& texture_name,
    const std::string& program_name, const std::string& subtexture_name)
{
  auto texture{get_texture2D(texture_name)};
  auto program{get_shader_program(program_name)};
  if (!texture || !program)
    {
      std::cerr << "Resource_manager: failed to load sprite <" << sprite_name
                << ">" << std::endl;
      return nullptr;
    }
  sprite_map[sprite_name] =
      std::make_shared<render::sprite2D>(texture, subtexture_name, program);
  return sprite_map[sprite_name];
}

std::shared_ptr<render::sprite2D> resource_manager::get_sprite(
    const std::string& sprite_name)
{
  auto it = sprite_map.find(sprite_name);
  if (it == sprite_map.end())
    {
      std::cerr << "Resource_manager: sprite <" << sprite_name << "> not found"
                << std::endl;
      return nullptr;
    }
  return it->second;
}
}  // namespace resources
