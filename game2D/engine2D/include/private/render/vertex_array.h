#pragma once
#include <glad/glad.h>
#include "index_buffer.h"
#include "vertex_buffer.h"

namespace render
{
class vertex_buffer;
class vertex_buffer_descriptor;

/*!
 * \brief Массив атрибутов вершин (vao)
 * Данный класс представляет собой оболочку вокруг соответствующей gl-структуры.
 * Каждый vao-объект сохраняет информацию об активных атрибутах и вершинах, из
 * которых будут браться данные для атрибутов.
 * Разные vao имеют разные привязанные буферы и вершины и не зависят друг от
 * друга
 * \note Не работайте с вершинными буфермаи и буферами индексов без
 * использования vertex_array-объекта. Не используйте стандартный vao-объект для
 * своих целей.
 * \note При взаимподействии с render::vertex_buffer всегда будьте уверены, что
 * vertex_buffer будет существовать до удаления vao-объекта
 */
class vertex_array
{
 public:
  /*!
   * \brief Инициализация vao
   * \note После инициализации созданный vao станет активным
   */
  vertex_array();

  /*!
   * \brief Активация vao
   * Делакет vao активным.
   */
  void bind() const;

  /*!
   * \brief Деактивация vao
   * Делает активной стандартный vao
   */
  void detach() const;

  /*!
   * \brief Привязка вершинного буфера к vao
   * Привязывает буфер вершин к своему контексту, описывая передаваемые
   * атрибуты буфера с помощью декстриптора
   * \param buffer      привязываемый вершинный буфер
   * \param descriptor  описатель вершинного буфера
   * После привязки буфера вершин, vao-буфер сохранит правила загрузки данных из
   * буфера в атрибуты
   * \note При взаимподействии с render::vertex_buffer всегда будьте уверены,
   * что vertex_buffer будет существовать до удаления vao-объекта.
   * Лучше всего хранить их в виде полей класса.
   */
  void bind_vertex_buffer(const vertex_buffer& buffer,
                          const vertex_buffer_descriptor& descriptor);

  /*!
   * \brief Возвращает количество используемых атрибутов vao
   * \return Количество используемых атрибутов vao
   */
  unsigned int get_attributes_count() const { return va_arrays_count; }

  vertex_array(vertex_array&&);
  vertex_array& operator=(vertex_array&&);

  ~vertex_array();

  vertex_array(vertex_array&) = delete;
  vertex_array& operator=(vertex_array&) = delete;

 private:
  GLuint id{0};
  unsigned int va_arrays_count{0};
};
}  // namespace render
