#pragma once
#include <SDL.h>
#include <memory>
namespace sound
{
/*!
 * \brief Состояния звуковой дорожки
 */
enum struct playback_state
{
  on_play,   ///< играет
  on_pause,  ///< остановлен
  on_delete  ///< удален из списка проигрывания
};

/*!
 * \brief Звук из wav-формата
 * Представляет собой класс, содержащий данные о звуке.
 * Не отвечает за проигрывание звуков, но сожет создавать звуковые дорожки для
 * проигрывания.
 */
class wav_sound
{
 public:
  /*!
   * \brief Проигрыватель звука
   * Структура, отвечающая за проигрывание wav_sound-звука.
   */
  struct playback
  {
    /*!
     * \brief Создания звуковой дорожки
     * \param sound звук для проигрывания
     * \param pos   позиция начала проигрывания
     * \param loop  зациленность дорожки
     * \param state состояние воспроизведения
     */
    playback(wav_sound& sound, uint32_t pos = 0, bool loop = false,
             playback_state state = playback_state ::on_play);

    /*!
     * \brief Устанавлевает состояние дорожки в начало проигрыша и состояние
     * воспроизведения
     */
    void replay();
    wav_sound& sound;      ///< звук для проигрывания
    uint32_t position{0};  ///< позиция начала проигрывания
    playback_state state{playback_state ::on_play};  ///< зациленность дорожки
    bool is_looped{false};  ///< состояние воспроизведения
  };

  /*!
   * \brief Создание звука
   * \param data        указатель на бинарное представление звука
   * \param duration    длительность звука
   */
  wav_sound(uint8_t* data, uint32_t duration);

  /*!
   * \brief Создание и запуск звуковой дорожки
   * \param pos     место начала проигрывания
   * \param loop    зацикливание
   * \param state   начальное состояние
   * \return    звуковая дорожка, загруженная в звуковой буфер
   */
  std::shared_ptr<playback> play(
      uint32_t pos = 0, bool loop = false,
      playback_state state = playback_state ::on_play);

  /*!
   * \brief Возврат указателя на бинарное представление звука
   * \return указатель на бинарное представление звука
   */
  uint8_t* get_data() const { return data; }

  /*!
   * \brief Возврат продолжительности звука
   * \return продолжительность звука
   */
  uint32_t get_duration() const { return duration; }

  ~wav_sound();

  wav_sound(wav_sound&&);
  wav_sound& operator=(wav_sound&&);

  wav_sound() = delete;
  wav_sound(wav_sound&) = delete;
  wav_sound& operator=(wav_sound&) = delete;

 private:
  uint8_t* data{nullptr};
  uint32_t duration{0};
};
}  // namespace sound
