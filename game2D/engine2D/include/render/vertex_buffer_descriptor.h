#include <glad/glad.h>
#include <vector>
namespace render
{
/*!
 * \brief Описатель вершины
 * Класс, хранящий метаданные вершины какого-либо вершинного буфера.
 * Используется для взаимодействия render::vertex_array и render::vertex_buffer
 * Согласно описателю, в vao-буфере будут зарезервированы входные атрибуты для
 * шейдерной программы
 * \note Последовательность описателей атрибутов имеет значение
 * \sa render::vertex_buffer_descriptor::element_descriptor
 * \sa render::vertex_array
 */
class vertex_buffer_descriptor
{
 public:
  /*!
   * \brief Метаданные входного атрибута вершины
   * Воспомогательная внутренняя структура, хранящая метаданные отдельного
   * атрибута вершины. Исходя из данных, происходит надстройка входящего
   * атрибута шейдерной программы
   */
  struct element_descriptor
  {
    int count;  ///< количество элементов заданного типа
    GLenum type;      ///< соответствующий gl-тип данных
    bool normalized;  ///< необходимость в нормализации данных при рендере
    int size;  ///< общий размер, занимаемый данным атрибутом в байтах
  };

  /*!
   * \brief Инициализация описателя вершины
   */
  vertex_buffer_descriptor() {}

  /*!
   * \return Размер одного описанного элемента буфера в виде суммы размеров всех
   * описателей атрибутов элемента
   * \sa render::vertex_buffer_descriptor::element_descriptor
   */
  int get_stride() const { return stride; }

  /*!
   * \brief Добавить описатель одного следующего атрибута элемента в конец
   * списка
   * \note Порядок добавления описателей атрибутов имеет значение. Атрибуты
   * должны описываться последовательно в порядке их визического хранения в
   * бефере вершин
   * \param count
   * \param normalized
   */
  void add_element_descriptor_float(int count, bool normalized);

  /*!
   * \return Списко описателей атрибутов вершины
   */
  const std::vector<element_descriptor>& get_descriptors() const
  {
    return descriptors;
  }

 private:
  int stride{};
  std::vector<element_descriptor> descriptors{};
};
}  // namespace render
