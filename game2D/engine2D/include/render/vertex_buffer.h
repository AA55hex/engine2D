#pragma once
#include <glad/glad.h>
namespace render
{
/*!
 * \brief Буфер вершин
 * Оболочка gl-буфера вершин.
 * Представляет собой массив произвольных данных, реализация которых описывается
 * вручную.
 * Данный класс должен использоваться как хранилище входных данных
 * рендера и использоваться совместно с vertex_array.
 * \note Все взаимодействия с буфером вершин следует проводить опосредованно, с
 * помощью vertex_array.
 * \sa render::vertex_array
 * \note Все динамические данные, поступающие в класс
 * могут быть удалены после инициализации, так как они полностью копируются в
 * память видеокарты.
 */
class vertex_buffer
{
 public:
  /*!
   * \brief Инициализация бустого буфера вершин
   */
  vertex_buffer();

  /*!
   * \brief Инициализация с заполнением
   * \param size    размер данных в байтах
   * \param data    указатель на загружаемые данные
   * \note Данные data могут быть удалены после инициализации
   */
  vertex_buffer(const unsigned int size, const void* data);

  /*!
   * \brief Активация буфера вершин
   * Привязывает буфер вершин к контексту, делая его активным.
   * Одновременно активен только один буфер вершин.
   * \note Не используйте данную функцию непосредственно, вместо этого
   * используйте render::vertex_array::bind_vertex_buffer(const vertex_buffer&
   * buffer, const vertex_buffer_descriptor& descriptor)
   * \sa render::vertex_array::bind_vertex_buffer(const vertex_buffer& buffer,
   * const vertex_buffer_descriptor& descriptor)
   */
  void bind() const;

  /*!
   * \brief Деактивация буфера
   * Деактивирует буфер вершин, привязывая стандартный буфер
   */
  void detach() const;

  /*!
   * \brief Перезагрузка буфера
   * Сбрасывает массив данных в буфере и создает новый, заполняя его новыми
   * данными.
   * \param size    размер данных в байтах
   * \param data    указатель на загружаемые данные
   * \note Данные data могут быть удалены после использования
   * \note Буфер автоматически выполняет команду render::vertex_buffer::bind()
   * \sa render::vertex_buffer::bind()
   */
  void restore(const unsigned int size, const void* data);

  /*!
   * \brief Изменение буфера
   * Перезаписывает данные буфера, начиная с первого элемента массива новыми
   * данными.
   * \param size    размер данных в байтах
   * \param data    указатель на загружаемые данные
   * \note Данные data могут быть удалены после использования
   * \note Буфер автоматически выполняет команду render::vertex_buffer::bind()
   * \sa render::vertex_buffer::bind()
   */
  void update(const unsigned int size, const void* data);

  ~vertex_buffer();

  vertex_buffer(vertex_buffer&&);
  vertex_buffer& operator=(vertex_buffer&&);

  vertex_buffer(vertex_buffer&) = delete;
  vertex_buffer operator=(vertex_buffer&) = delete;

 private:
  GLuint id{0};
  unsigned int size{0};
  static const GLenum buffer_type{GL_ARRAY_BUFFER};
};
}  // namespace render
