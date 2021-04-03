#pragma once
#include <render/frame_structures.h>
#include <memory>
#include <vector>
namespace render
{
class sprite2D;

/*!
 * \brief Аниматор спрайта
 * \note Аниматор спрайта работает только с текстурными атласами, но не с
 * массивами текстур
 */
class sprite_animator
{
 public:
  /*!
   * \brief Фрейм анимации
   * Воспомогательная структура, согласно которой отрисовывается спрайт
   */
  struct frame
  {
    const frame_descriptor discriptor;  ///< описатель области кадра
    double duration{0};  ///< длительность кадра в миллисекундах
  };

  /*!
   * \brief Инициализация аниматора
   * \param sprite  спрайт, используемый для анимации
   */
  sprite_animator(std::shared_ptr<sprite2D> sprite);

  /*!
   * \brief Обновление  состояния анимации
   * \param duration    прошедшее время в миллисекундах
   * \note За один вызов функция может перейти только на 1 кадр, не смотря на
   * значение duration.
   */
  void update(double duration);
  /*!
   * \brief Отрисовка активного фрейма
   * \param settings    параметры отрисовки
   * \note Если аниматор не содержит кадров, функция обратится к функции
   * sprite2D::render(const render_settings& settings)
   * \sa render::sprite2D::render(const render_settings& settings)
   */
  void render(const render_settings& settings);

  /*!
   * \brief Добавление фрейма в конец списка кадров
   * \param discriptor  описатель области фрейма
   * \param duration    продолжительность кадра, мс
   * \note Допустимо 0 значение duration.
   * Отрицательное значение вызовет зацикливание на кадре.
   */
  void add_frame(const frame_descriptor& discriptor, double duration);

  /*!
   * \brief Удаление списка кадров
   * Очищает весь список кадров
   */
  void clear_frames();

  /*!
   * \brief Возврат активного кадра
   * \return Активный кадр анимации
   */
  const frame* current_frame();

  /*!
   * \brief Возарвт количества кадров в аниматоре
   * \return Количество кадров в аниматоре
   */
  size_t get_frames_count() { return frames.size(); }

  /*!
   * \brief Возврат общей продолжительности анимации
   * \return Сумма продолжительностей всех кадров
   */
  double get_duration() { return summary_duration; }

 private:
  std::shared_ptr<sprite2D> sprite;
  std::vector<frame> frames{};
  size_t current_frame_index{0};
  double current_duration{0};
  double summary_duration{0};
};
}  // namespace render
