#pragma once
#include <glm/vec2.hpp>
#include "input_event.h"

namespace input
{
/*!
 * \brief Менеджер событий ввода
 * Центральный класс, отвечающий за организацю передачи данных о событиях ввода
 * исполняемому процессу.
 * Полностью статический.
 * \note Для функционирования класса проиницализируйте движок с помощью класса
 * core::engine
 * \sa core::engine::window_init(int width, int height, std::string title, bool
 * gl_debug)
 */
class input_manager
{
  input_manager() = delete;

 public:
  /*!
   * \brief Чтение следущего события клавиатуры
   * \param[out] event  полученное событие
   * \return true - событие было записано в переменную event, в ином случае -
   * false
   */
  static bool read_input(input_event* event);
  /*!
   * \brief Возвращает положение мыши (x;y) на экране в диапазоне [-1;1]
   * \note Центр оси координат находится по центру экрана. Крайние точки по оси
   * абсцисс и ординат находятся в диапазоне [-1;1]
   * \return Положение мыши относительно окна
   */
  static glm::vec2 get_mouse_state();
};
}  // namespace input
