#pragma once
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>
namespace render
{
class texture2D;

/*!
 * \brief Шейдерная программа
 * Класс представляет собой абстракцию вокруг gl-программы отрисовки.
 * \note Взаимодействует только с шейдерами, написанными на языке GLSL 300 es
 */
class shader_program
{
 public:
  /*!
   * \brief Инициализация шейдерной программы
   * Взаимодействует только с шейдерами, написанными на языке GLSL 300 es
   * \param vert_src    строка с source-кодом вершинного шейдера
   * \param frag_src    строка с source-кодом фрагментного шейдера
   * \note процесс инициализации лоигруется с указанием ошибок инициализации
   */
  shader_program(const std::string& vert_src, const std::string& frag_src);

  /*!
   * \brief Состояние компиляции программы
   * Указывает, удачно ли скомпилировалась программа
   * \return true - удачная компиляция, false - компиляция прервана
   */
  bool is_compiled() const;

  /*!
   * \brief Активация шейдерной программы
   * Делает программу активной для контекста окна.
   * \note Одновременно может быть активна только одна программа, остальные
   * деактивируются
   */
  void use() const;

  shader_program& operator=(shader_program&&);
  shader_program(shader_program&&);

  ~shader_program();

  /*!
   * \brief Установка uniform-поля
   * \param u_name  имя uniform-поля в шейдере
   * \param value   значение для заполнения
   * \note для указания значения для типа sampler2D необходима вписать индекс
   * текстурного буфера, к кторому привязана текстура.
   * \return
   * \sa render::texture2D
   */
  bool set_uniform(std::string u_name, int value);

  /*!
   * \brief Установка uniform-поля
   * \param u_name  имя uniform-поля в шейдере
   * \param value   значение для заполнения
   * \return
   */
  bool set_uniform(std::string u_name, const glm::mat4x4& value);

  shader_program(shader_program&) = delete;
  shader_program& operator=(shader_program&) = delete;

 private:
  bool create_shader(const std::string& source, GLenum s_type,
                     GLuint& shader_id);
  bool compile_status{false};
  GLuint id{0};
};
}  // namespace render
