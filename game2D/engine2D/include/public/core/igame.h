#pragma once
namespace core
{
/*!
 * \brief igame - интерфейс взаимодействия с классом engine.
 * \details Данный интерфейс содержит весь необходимый функционал для
 * взаимодействия с классом engine.
 * \note Используйте реализацию данного класса в качестве класса-игры.
 */
class igame
{
 public:
  /*!
   * \brief Инициализация необходимых ресурсов
   * \return true - успешнаяя инициализация, false - инициализация прервана
   */
  virtual bool inicialize() = 0;
  /*!
   * \brief Действия по обработке ванных ввода с клавиатуры и мыши
   * \param duration    время, прошедшее с последнего вызова
   */
  virtual void read_input(double duration) = 0;
  /*!
   * \brief Действия по обновлению данных игровых объектов
   * \param duration   время, прошедшее с последнего вызова
   */
  virtual void update_data(double duration) = 0;
  /*!
   * \brief Действия по обработке изображения и звука.
   * Перед вызовом данной функции из класса engine, происходит подготовка
   * следущего игрового кадра. После вызова данной функции из класса engine,
   * происходит обновление кадра.
   * \note Не производите запросы по обработке
   * изображения вне диапазона данной функции.
   */
  virtual void render_output() = 0;

  /*!
   * \brief Указывает на состояние игры
   * \return состояние игры. true - игровой процесс продолжается, false -
   * окончен
   */
  virtual bool is_playing() = 0;
  /*!
   * \brief Указывает на состояние инициализации ресурсов игры
   * \return true - ресурсы загружены, false - ресурсы не загружены
   * \note Возврат false так же может говорить о не успешной инициализации игры
   */
  virtual bool is_inicialized() = 0;

  virtual ~igame() = default;
};
}  // namespace core