#pragma once
#include <render/frame_structures.h>
#include <memory>
#include <string>
namespace render
{
class texture2D;
class shader_program;

class index_buffer;
class vertex_array;
class vertex_buffer;
/*!
 * \brief Спрайт
 * Представляет собой абстракцию спрайта для отрисовки.
 * \note Отрисовка спрайта требует определенных имен uniform-полей в шейдере:
 * - s_model    - mat4, модельная матрица
 * - s_texture  - sampler2D, указатель на текстуру
 * - s_layer    - int, слой отрисовки
 * \sa sprite2D::render(const render_settings& settings)
 */
class sprite2D
{
 public:
  /*!
   * \brief Инициализация спрайта
   * \param texture     текстура спрайта
   * \param subtexture  подтекстура спрайта
   * \param program     шейдерная программа для отрисовки спрайта
   * \note Если texture не содержит подтекстуры с именем subtexture, то
   * используется вся текстура
   * \note После инициализации описатель области примет координаты указанной
   * области
   */
  sprite2D(std::shared_ptr<texture2D> texture, std::string subtexture,
           std::shared_ptr<shader_program> program);

  /*!
   * \brief Отрисовка в back-буфер
   * Отрисовывает спрайт в буфер, согласно настройкам.
   * \note в качестве frame_descriptor'a используется последний применявшийся
   * дескриптор
   * \note Отрисовка спрайта требует определенных имен uniform-полей в шейдере:
   * - s_model    - mat4, модельная матрица
   * - s_texture  - sampler2D, указатель на текстуру
   * - s_layer    - int, слой отрисовки
   * \param settings    свойства отрисовки
   */
  void render(const render_settings& settings);

  /*!
   * \brief Отрисовка в back-буфер
   * Отрисовывает спрайт в буфер, согласно настройкам.
   * \note Отрисовка спрайта требует определенных имен uniform-полей в шейдере:
   * - s_model    - mat4, модельная матрица
   * - s_texture  - sampler2D, указатель на текстуру
   * - s_layer    - int, слой отрисовки
   * \param settings        свойства отрисовки
   * \param f_discriptor    описатель диапазона отрисовки в текстуре
   * f_discriptor указывает какую именно часть текстуры следует отрисовывать
   * \note координаты текстуры в f_discriptor находятся в диапазоне [0;1] от
   * левого нижнего угла
   */
  void render(const render_settings& settings,
              const frame_descriptor& f_discriptor);

  ~sprite2D();

  sprite2D(sprite2D&&);
  sprite2D& operator=(sprite2D&&);

  sprite2D(sprite2D&) = delete;
  sprite2D& operator=(sprite2D&) = delete;

  /*!
   * \brief Активный дескриптор
   * Возвращет последний использовавшийся описатель области.
   * \note После инициализации описатель принимает координаты указанной
   * подтекстуры или текстуры
   * \return Активный описатель области
   */
  const frame_descriptor& get_current_frame() { return frame; }

 private:
  frame_descriptor frame;

  std::shared_ptr<shader_program> program;
  std::shared_ptr<texture2D> texture;

  struct sprite2D_impl;
  std::unique_ptr<sprite2D_impl> impl{};
};
}  // namespace render
