#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <map>
#include <string>
#include "render/frame_structures.h"
namespace render
{
/*!
 * \brief Цеветовые модели
 */
enum class color_format
{
  red = GL_RED,
  green = GL_GREEN,
  blue = GL_BLUE,
  alpha = GL_ALPHA,
  rgb = GL_RGB,
  rgba = GL_RGBA
};

/*!
 * \brief Класс-текстура
 * Представляет собой абстракцию загруженой gl-текстуры.
 * \note Координаты текстур нормализованы по диапазону [0;1].
 *       Начало координатной плоскости (0;0) находится в левом нижнем углу окна.
 */
class texture2D
{
 public:
  /*!
   * \brief Инициализация текстуры
   * \param width       ширина текстуры
   * \param height      высота текстуры
   * \param data        указатель на данные текстуры
   * \param format      цветовая модель
   * \param filter      режим фильтрации текстуры при рендере
   * \param wrapMode    поведение текстуры при отрисовке
   */
  texture2D(int width, int height, const void* data, color_format format,
            const GLenum filter = GL_LINEAR,
            const GLenum wrapMode = GL_CLAMP_TO_EDGE);

  /*!
   * \brief Сделать текстуру активной
   * Привязывает текстуру к активному текстурному буферу.
   * Одновременно один текстурный буфер может содержать одну текстуру
   */
  void bind() const;

  /*!
   * \brief Возврат ширины текстуры
   * \return Ширина текстуры
   */
  int get_width() const;

  /*!
   * \brief Возврат высоты текстуры
   * \return Высота текстуры
   */
  int get_height() const;

  ~texture2D();

  texture2D(texture2D&&);
  texture2D& operator=(texture2D&&);

  /*!
   * \brief Добавление подтекстуры из текстуры.
   * Добавляет координаты подтекстуры в хранилище подтекстур объекта.
   * \note Координаты текстур нормализованы по диапазону [0;1].
   *       Начало координатной плоскости (0;0) находится в левом нижнем углу
   * окна.
   * \param key         имя подтекстуры
   * \param subtexture  описатель области подтекстуры
   * \return Созданная подтекстура
   */
  const frame_descriptor& add_subtexture(std::string key,
                                         const frame_descriptor& subtexture);

  /*!
   * \brief Добавление подтекстуры из текстуры.
   *  \note Координаты текстур нормализованы по диапазону [0;1].
   *       Начало координатной плоскости (0;0) находится в левом нижнем углу
   * окна.
   * \param key         имя подтекстуры
   * \param left_bottom левый нижний угол подтекстуры
   * \param right_top   правый верхний угол подтекстуры
   * \return Созданная подтекстура
   */
  const frame_descriptor& add_subtexture(std::string key, glm::vec2 left_bottom,
                                         glm::vec2 right_top);

  /*!
   * \brief Возврат подтекстуры
   * Ищет подтекстуру объекта по её имени.
   * \param key имя подтекстуры
   * \return Подтекстура объекта.
   * Если подтекстура не найдена, возвращается вся текстура
   */
  const frame_descriptor& get_subtexture(std::string key) const;

  texture2D(texture2D&) = delete;
  texture2D& operator=(texture2D&) = delete;

  /*!
   * \brief Активировать буфер текстуры
   * Активирует текстурный буфер по его номеру.
   * Текстурный буфер используется для привязки текстуры к нему.
   * \note Одновременно может быть активнен только один текстурный буфер.
   * \param offset  индекс текстурного буфера. Максимальный диапазон [0;31].
   */
  static void active_texture(unsigned int offset);

 private:
  void restore(int width, int height, const void* data, color_format format);
  GLuint id{0};
  int height{0};
  int width{0};

  std::map<std::string, frame_descriptor> subtexture_map{};
  const frame_descriptor default_texture{};
};

}  // namespace render
