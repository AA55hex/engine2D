#pragma once
#include <glad/glad.h>
#include <string>
namespace render
{
/*!
 * \brief Буфер индексов вершин шейдера
 * Данный класс является оболочкой gl-структуры, отвечающей за хранение номеров
 * вершин для отрисовки в определенном порядке
 * \note Класс работает только с массивами данных типа unsigned int
 */
class index_buffer
{
 public:
  /*!
   * \brief Инициализация пустым буфером
   */
  index_buffer();
  /*!
   * \brief Инициализация
   * \param count   количество элементов в массиве
   * \param data    указатель на элементы массива
   */
  index_buffer(const unsigned int count, const unsigned int* data);

  /*!
   * \brief Активация буфера
   * Привязывает буфер к контексту окна, делая его активным.
   * Все данные об индексах вершин берутся только из активного буфера.
   * Одновременно активным может быть только один буфер, при активации одного
   * буфера, остальные становятся неактивными.
   */
  void bind() const;
  /*!
   * \brief Деактивация буфера
   * Отвязывает буфер от контекста окна, устанавливая буфер по умолчанию
   */
  void detach() const;

  /*!
   * \brief Перезагрузка буфера
   * Сбрасывает массив данных в буфере и создает новый, заполняя его новыми
   * данными.
   * \param count    количество элементов в массиве
   * \param data    указатель на элементы массива
   * \note Данные data могут быть удалены после использования
   * \note Буфер автоматически выполняет команду render::index_buffer::bind()
   * \sa render::index_buffer::bind()
   */
  void restore(const unsigned int count, const unsigned int* data);

  /*!
   * \brief Изменение буфера
   * Перезаписывает данные буфера, начиная с первого элемента массива новыми
   * данными.
   * \param count    количество элементов в массиве
   * \param data    указатель на элементы массива
   * \note Данные data могут быть удалены после использования
   * \note Буфер автоматически выполняет команду render::index_buffer::bind()
   * \sa render::index_buffer::bind()
   */
  void update(const unsigned int count, const unsigned int* data);

  ~index_buffer();

  index_buffer(index_buffer&&);
  index_buffer& operator=(index_buffer&&);

  index_buffer(index_buffer&) = delete;
  index_buffer operator=(index_buffer&) = delete;

  /*!
   * \brief Возвращает количество элементов в буфере
   * \return Количество элементов в буфере
   */
  unsigned int get_count() const { return count; }

 private:
  GLuint id{0};
  unsigned int count{0};

  static const GLenum buffer_type{GL_ELEMENT_ARRAY_BUFFER};
  static const GLuint element_size{sizeof(GLuint)};
};
}  // namespace render
