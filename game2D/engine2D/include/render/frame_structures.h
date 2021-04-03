#pragma once
#include <glm/vec2.hpp>
namespace render
{
/*!
 * \brief Описатель области текстуры или текстурного атласа
 * \note Координаты текстур задаются диапазоном [0;1].
 * Начало системы координат находится в левом нижнем углу окна.
 */
struct frame_descriptor
{
  glm::vec2 left_bottom_uv{0.f};  ///< координата левого нижнего угла области
  glm::vec2 right_top_uv{1.f};  ///< координата правого верхнего угла области
};

/*!
 * \brief Найстройка отрисовки
 * Воспомогательная структура для отрсовки спрайтов
 * \note Координаты и размер задаются диапазоном [-1;1]
 */
struct render_settings
{
  glm::vec2 position{
      0};  ///< положение нижнего левого угла отрисовываемого объекта
  glm::vec2 size{1};  ///< размер объекта по ширине и высоте
  int layer{0};       ///< слой отрисовки (макс. 50)
  float rotation{0};  ///< угол поворота объекта
};

}  // namespace render
