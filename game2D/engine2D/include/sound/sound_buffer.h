#pragma once
#include <memory>
#include <vector>
#include "wav_sound.h"
namespace sound
{
/*!
 * \brief Буфер пригрываемых звуков
 * Представлят собой буфер, хранящий массив проигрываемых звуковых дорожек.
 * Могут использоваться для сохранения состояний нескольких звуковых дорожек
 * одновременно.
 * Активным может быть только один буфер, в то время как остальные
 * замораживают свой звуковой поток.
 */
class sound_buffer
{
 public:
  using playback_ptr = std::shared_ptr<wav_sound::playback>;

  /*!
   * \brief Инициализация звукового буфера с заданием тишины
   * \param silence амплитуда тишины
   * \note Тишина будет проигрываться каждый раз, когда буфер окажется пустым
   */
  sound_buffer(int silence = 0);

  /*!
   * \brief Активация звукового буфера
   * Делает буфер активным, проигрывая его массив звуков
   */
  void use();

  /*!
   * \brief Деактивация звукового буфера
   */
  void detach();

  /*!
   * \brief Добавляет проигрываемую дорожку в конец списка
   * \param playback    проигрываемая дорожка
   * \note Если playback не является зацикленым, то по окончанию звука от
   * перейдет в состояние on_delete и удалится из проигрывателя
   */
  void push_playback(playback_ptr playback);

  /*!
   * \brief Очищает звуковой поток
   */
  void clear();
  ~sound_buffer();

  /*!
   * \brief Возврат активного звукового буфера
   * \return активный звуковой буфер
   */
  static sound_buffer* current() { return current_buffer; }

 private:
  int silence{0};
  std::vector<playback_ptr> playback_list{};

  friend void audio_callback(void* userdata, Uint8* stream, int len);

  inline static sound_buffer* current_buffer{};
};
}  // namespace sound
