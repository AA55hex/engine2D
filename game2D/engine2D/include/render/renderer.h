#include <glad/glad.h>

namespace render
{
class vertex_array;
class index_buffer;
class shader_program;

/*!
 * \brief Рендер-менеджер
 * Статический класс, предоставляющий интерфейс рендера изображения
 * \note Перед использование необходима инициализация ресурсов в классе
 * core::engine
 */
class renderer
{
 public:
  // static void swap_buffers();
  /*!
   * \brief Отрисовка в back-буфер
   * \param vao     используемый массив атрибутов
   * \param vebo    используемый массив индексов
   * \param program используемая шейдерная программа
   * \param mode    режим отрисовки
   */
  static void draw(const vertex_array& vao, const index_buffer& vebo,
                   const shader_program& program, GLenum mode = GL_TRIANGLES);
  /*!
   * \brief Задать диапазон для отрисовки
   * Задает диапазон, в границах которого будет расчитываться отрисовка
   * Границы отрисовки указываются не нормализованными значениями.
   * \note Начало оси координат находится в левом верхнем углу.
   * \param x       положение точки по оси абсцисс
   * \param y       положение точки по оси ординат
   * \param width   ширина области
   * \param height  высота области
   */
  static void set_viewport(int x, int y, int width, int height);
  /*!
   * \brief Установить цвет очистки экрана
   * Устанавливает, каким цветом будет заполняться буфер экрана при вызове
   * renderer::clear()
   * \param r   red-канал
   * \param g   green-канал
   * \param b   blue-канал
   * \param a   alpha-канал
   */
  static void clear_color(float r, float g, float b, float a);
  /*!
   * \brief Очистка back-буфера
   * Очищает буфер, который будет заполняться, цветом, указанным в
   * renderer::clear_color(float r, float g, float b, float a)
   */
  static void clear();

  renderer() = delete;
};

}  // namespace render
