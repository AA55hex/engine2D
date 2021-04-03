#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "render/shader_program.h"
#include "render/sprite2D.h"
#include "render/texture2D.h"
#include "sound/wav_sound.h"
namespace resources
{
/*!
 * \brief Менеджер ресурсов
 * Представляет собой абстракцию менеджера по загрузке и хранению подгружаемых
 * ресурсов, таких, как текстуры, спрайты, звуки и т.д.
 * Изначально содержит интерфейс по загрузке и хранению всех примитивов
 * библиотеки.
 * При необходимости, можно создать дочерний класс и дополнить его
 * интерфейсом хранения и загрузки игровых объектов.
 * \note Выводит логи в процессе создания/загрузки/возврата ресурса.
 * \note Каождый объект имеет свой список ресурсов
 */
class resource_manager
{
 public:
  /*!
   * \brief Инициализация менеджера ресурсов
   * \param dir_path    путь к директории ресурсов
   */
  resource_manager(const std::string& dir_path);

  /*!
   * \brief Изменение директории ресурсов
   * \param dir_path    путь к директории ресурсов
   */
  void reset_path(const std::string& dir_path);

  resource_manager(resource_manager&&);
  resource_manager& operator=(resource_manager&&);

  /*!
   * \brief Загрузка шейдерной программы, используя GLSL-шейдеры вершинного и
   * фрагментного типа из файла.
   * \param prg_name    имя, идентифицирующее объект
   * \param vert_path   относительный путь к вершинному шейдеру
   * \param frag_path   относительный путь к фрагментному шейдеру
   * \return Загруженная шейдерная программа
   * \note Программа создается даже в случае неудачной компиляции
   */
  std::shared_ptr<render::shader_program> load_shader_program(
      const std::string& prg_name, const std::string& vert_path,
      const std::string& frag_path);

  /*!
   * \brief Возврат шейдерной программы по имени
   * Возвращает шейдерную программу из списка загруженных программ
   * \param имя, идентифицирующее объект
   * \return Искомый объект, nullptr - объект не найден
   */
  std::shared_ptr<render::shader_program> get_shader_program(
      const std::string& prg_name);

  /*!
   * \brief Загрузка 2D-текстуры из файла
   * \param texture_name    мя, идентифицирующее объект
   * \param filepath        относительный путь к текстуре
   * \return Загруженная тексткура, nullptr - объект не загружен
   */
  std::shared_ptr<render::texture2D> load_texture2D(
      const std::string& texture_name, const std::string& filepath);

  /*!
   * \brief Возврат загруженной текстуры по имени
   * \param texture_name    имя, идентифицирующее объект
   * \return Искомый объект, nullptr - объект не найден
   */
  std::shared_ptr<render::texture2D> get_texture2D(
      const std::string& texture_name);

  /*!
   * \brief Загрузка текстурного атласа из файла
   * Загружает текстуру, представляющую собой текстурный атлас, создавая
   * подтекстуры равного размера с указанными именами
   * \note Текстуры и текстурные атласы хранятся совместно
   * \param texture_name        имя, идентифицирующее объект
   * \param filepath            пкть к текстуре
   * \param subtexture_names    Имена для ижентификации подтекстур
   * \param frame_width         Ширина подтекстуры
   * \param frame_height        Высота подтекстуры
   * \return Загруденный текстурный атлас, nullptr - объект не загружен
   */
  std::shared_ptr<render::texture2D> load_texture_atlas2D(
      const std::string& texture_name, const std::string& filepath,
      std::vector<std::string> subtexture_names, unsigned int frame_width,
      unsigned int frame_height);

  /*!
   * \brief Загрузка спрайта из загруженных ресурсов
   * Создает спрайт на основе уже загруженной текстуры и программы
   * \param sprite_name имя, идентифицирующее объект
   * \param texture     имя загруденной текстуры
   * \param program     имя загруженной шейдерной программы
   * \param subtexture  имя подтекстуры для инициализации спрайта
   * \return Созданный спрайт, nullptr - объект не загружен
   */
  std::shared_ptr<render::sprite2D> load_sprite(
      const std::string& sprite_name, const std::string& texture,
      const std::string& program, const std::string& subtexture = "");

  /*!
   * \brief Возврат загруденного спрайта по имени
   * \param sprite_name имя, идентифицирующее объект
   * \return Искомый объект, nullptr - объект не найден
   */
  std::shared_ptr<render::sprite2D> get_sprite(const std::string& sprite_name);

  /*!
   * \brief Загрузка wav-звука из файла
   * \param wav_name    имя, идентифицирующее объект
   * \param filepath    относительный путь к wav-файлу
   * \return Загруденный звук, nullptr - объект не загружен
   */
  std::shared_ptr<sound::wav_sound> load_wav(const std::string& wav_name,
                                             const std::string& filepath);

  /*!
   * \brief Возврат загруженного звука
   * \param wav_name    имя, идентифицирующее объект
   * \return Искомый объект, nullptr - объект не найден
   */
  std::shared_ptr<sound::wav_sound> get_wav(const std::string& wav_name);

  resource_manager(resource_manager&) = delete;
  resource_manager& operator=(resource_manager&) = delete;

 private:
  void path_configure(const std::string& dir_path);
  std::string get_file_data(const std::string& additional);

  std::map<std::string, std::shared_ptr<render::shader_program>>
      shader_program_map;

  std::map<std::string, std::shared_ptr<render::texture2D>> texture_map;

  std::map<std::string, std::shared_ptr<sound::wav_sound>> wav_map;

  std::map<std::string, std::shared_ptr<render::sprite2D>> sprite_map;

  std::string path;
};
}  // namespace resources
