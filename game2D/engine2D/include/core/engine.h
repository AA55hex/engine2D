#pragma once
#include <SDL.h>
#include <string>
namespace sound
{
class sound_buffer;
void audio_callback(void* userdata, Uint8* stream, int len);
}  // namespace sound

namespace render
{
class renderer;
}

namespace input
{
class input_manager;
}

namespace core
{
class igame;
/*!
 * \brief Центральный класс игрового движка
 * Класс предоставляет интерфейс взаимодействия с интерфейсом igame.
 * На данный момент является полностью статическим и поддерживает одновременную
 * работу только с одним объектом игры.
 * \note Перед обращением к каким-либо структурам библиотеки, загрузите все
 * ресурсы с помощью данного класса.
 * \sa engine::inicialize(int width, int height, std::string title, bool
 * gl_debug = false)
 * \sa igame
 */
class engine
{
 public:
  engine() = delete;
  /*!
   * \brief Инициализация ресурсов игрового движка
   * Инициализация необходима для работы всех остальных классов.
   * При отсутствии инициализации большая часть структур библиотеки будует
   * возвращать исключание.
   * При работе возвращает log-и событий.
   * \param width   ширина окна
   * \param height  высота окна
   * \param title   заголовок окна
   * \param gl_debug    true - использовать debug-контекст окна
   * \return true - успешная инициализация, false - инициализация
   * прервана
   * \note При неудачной инициализации обратите внимание на полученные
   * log-и
   */
  static bool inicialize(int width, int height, std::string title,
                         bool gl_debug = false);
  /*!
   * \brief Освобождение ресурсов движка
   * \note Если не была проведена инициализация, в log-и выведется
   * соответствующее сообщение
   */
  static void dispose();
  /*!
   * \brief Состояние инициализации движка
   * \return true - ресурсы загружены
   */
  static bool is_inicialized() { return inicialized; }
  /*!
   * \brief Запуск цикла игры
   * Сначала происходит разовая поптыка инициализации игры, если она не была
   * произведена.
   * Цикл состоит из следующих этапов этапов:
   * - четние событий ввода
   * - изменение данных игрового мира
   * - подготовка видео-буфера к заполнению
   * - обработка звука и изображения для нового кадра
   * - переключение видео-буфера.
   * \param game    объект игрового мира
   * \note Всегда используйте данный метод как основной игровой цикл. В ином
   * случае у вас не будет доступа к переключению виде-буфера
   * \note Цикл останавливается, когда функция igame::is_playing() возвращает
   * false;
   * \sa igame::is_playing()
   */
  static void play(igame& game);

 private:
  friend class sound::sound_buffer;
  friend void sound::audio_callback(void*, Uint8* stream, int len);

  friend class input::input_manager;

  inline static bool inicialized{false};
  static bool window_init(int width, int height, std::string title,
                          bool gl_debug);
  static bool audio_init();

  struct audio_data
  {
    SDL_AudioDeviceID audio_device;
    SDL_AudioSpec audio_device_spec;
  };

  struct window_data
  {
    SDL_Window* window;
    SDL_GLContext gl_context;
    int height;
    int width;
  };

  inline static audio_data audio{};
  inline static window_data window{};
};
}  // namespace core
