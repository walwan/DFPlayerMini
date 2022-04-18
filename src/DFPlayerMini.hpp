/*!
 * @file DFPlayerMini.h
 *
 * This is the documentation for the MP3-TF-16P module, aka. DFPlayer Mini MP3
 * Player, for general use. It is designed specifically to work with the <a
 * href="https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299"> DFPlayerMini
 * breakout</a>
 *
 * These MP3 players use UART to communicate, 2 pins (TX + RX) are required
 * to interface with the breakout. An external power supply and output amp
 * improves the MP3 player's functionality. Also, see <a
 * href="https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299"> the wiki</a> for
 * more detailed wiring instructions.
 *
 * Originally written by Power_Broker as a hobby.
 * Modified by walwan for general use.
 *
 */

#ifndef __DFPLAYERMINI_H__
#define __DFPLAYERMINI_H__

#include <stdint.h>

/**************************************************************************/
/*!
        @brief  Namespace for dfplayermini
*//**************************************************************************/
namespace DFPLAYERMINI {

/** Packet Values */
namespace PACKET {
constexpr uint8_t SIZE =
    10; // total number of bytes in a packet (same for cmds and queries)

constexpr uint8_t START = 0x7E;   // start byte
constexpr uint8_t VERSION = 0xFF; // version
constexpr uint8_t LEN =
    0x6; // number of bytes in a packet (except for checksum data and EB)
enum FEEDBACK : uint8_t { YES = 1, NO = 0 }; // feedback required bit
constexpr uint8_t END = 0xEF;                // end byte
} // namespace PACKET

/** Limits */
namespace LIMIT {
constexpr uint16_t MIN_ROOT_TRACK = 0;    // min track number in the root folder
constexpr uint16_t MAX_ROOT_TRACK = 2999; // max track number in the root folder

constexpr uint8_t MIN_FOLDER = 1;  // max folder number
constexpr uint8_t MAX_FOLDER = 99; // min folder number

constexpr uint16_t MIN_FOLDER_TRACK =
    1; // min track number in the current folder
constexpr uint16_t MAX_FOLDER_TRACK =
    255; // max track number in the current folder

constexpr uint8_t MIN_VOLUME = 0;  // minimum system volume
constexpr uint8_t MAX_VOLUME = 30; // maximum system volume
} // namespace LIMIT

/** Control Command Values */
namespace CONTROLCMD {
/** Specify playback track */
constexpr uint8_t PLAY_NEXT = 0x01; // play next track in the current folder
constexpr uint8_t PLAY_PREV = 0x02; // play previous track in the current folder
constexpr uint8_t PLAY_TRACK =
    0x03; // play specific track 0-2999 (in the root folder)
constexpr uint8_t PLAY_FOLDER_TRACK =
    0x0F; // play specific track 0-255 in a specific folder 0-99

/** Play control */
constexpr uint8_t PLAY = 0x0D;  // start playback
constexpr uint8_t PAUSE = 0x0E; // pause playback

/** Adjust playback and sound parameters */
constexpr uint8_t INC_VOL = 0x04; // increase volume by 1
constexpr uint8_t DEC_VOL = 0x05; // decrease volume by 1
constexpr uint8_t SET_VOL = 0x06; // set volume

constexpr uint8_t SET_EQ = 0x07; // set EQ

constexpr uint8_t SET_PLAYBACK_MODE = 0x08; // set playback mode
constexpr uint8_t SET_REPEAT_PLAY = 0x11;   // switch repeat play

constexpr uint8_t SET_PLAYBACK_SRC = 0x09; // set playback source

constexpr uint8_t SET_AUDIO_AMP = 0x10; // set audio amplification

/** Set module working mode */
constexpr uint8_t MODE_STANDBY = 0x0A; // enter standby mode
constexpr uint8_t MODE_NORMAL = 0x0B;  // enter normal working mode
constexpr uint8_t MODE_RESET = 0x0C;   // reset module

} // namespace CONTROLCMD

/** Query Command Values */
// TODO: comment
namespace QUERYCMD {
constexpr uint8_t SEND_INIT = 0x3F;
constexpr uint8_t RETRANSMIT = 0x40;
constexpr uint8_t REPLY = 0x41;
constexpr uint8_t GET_STATUS_ = 0x42;
constexpr uint8_t GET_VOL = 0x43;
constexpr uint8_t GET_EQ = 0x44;
constexpr uint8_t GET_MODE = 0x45;
constexpr uint8_t GET_VERSION = 0x46;
constexpr uint8_t GET_TF_FILES = 0x47;
constexpr uint8_t GET_U_FILES = 0x48;
constexpr uint8_t GET_FLASH_FILES = 0x49;
constexpr uint8_t KEEP_ON = 0x4A;
constexpr uint8_t GET_TF_TRACK = 0x4B;
constexpr uint8_t GET_U_TRACK = 0x4C;
constexpr uint8_t GET_FLASH_TRACK = 0x4D;
constexpr uint8_t GET_FOLDER_FILES = 0x4E;
constexpr uint8_t GET_FOLDERS = 0x4F;
} // namespace QUERYCMD

/** EQ Values */
namespace EQ {
constexpr uint8_t NORMAL = 0;
constexpr uint8_t POP = 1;
constexpr uint8_t ROCK = 2;
constexpr uint8_t JAZZ = 3;
constexpr uint8_t CLASSIC = 4;
constexpr uint8_t BASE = 5;
} // namespace EQ

/** Mode Values */
namespace PLAYBACK_MODE {
constexpr uint8_t REPEAT = 0;
constexpr uint8_t FOLDER_REPEAT = 1;
constexpr uint8_t SINGLE_REPEAT = 2;
constexpr uint8_t RANDOM = 3;
}; // namespace PLAYBACK_MODE

/** Playback Source Values */
namespace PLAYBACK_SRC {
constexpr uint8_t U = 1;
constexpr uint8_t TF = 2;
constexpr uint8_t AUX = 3;
constexpr uint8_t SLEEP = 4;
// constexpr uint8_t FLASH = 5;
} // namespace PLAYBACK_SRC

/** Base Volume Adjust Value */
// static constexpr uint8_t VOL_ADJUST = 0x10;

/** Repeat Play Values */
namespace REPEAT_PLAY {
constexpr uint8_t STOP = 0;
constexpr uint8_t START = 1;
} // namespace REPEAT_PLAY

/** Struct to store entire serial datapacket used for MP3 config/control
 */
struct stack_t {
  uint8_t start_byte;
  uint8_t version;
  uint8_t length;
  uint8_t command;
  uint8_t feedback;
  uint8_t paramMSB;
  uint8_t paramLSB;
  uint8_t checksumMSB;
  uint8_t checksumLSB;
  uint8_t end_byte;
};

/**************************************************************************/
/*!
        @brief  Class for interacting with DFPlayerMini MP3 player
*/
/**************************************************************************/
class DFPlayerMini {

  stack_t _sendStack = {PACKET::START,
                        PACKET::VERSION,
                        PACKET::LEN,
                        0,
                        PACKET::FEEDBACK::YES,
                        0,
                        0,
                        0,
                        0,
                        PACKET::END};
  stack_t _recvStack;

  static uint16_t calChecksum(stack_t &_stack);
  void setChecksum();

public:
  // bool _debug;

  DFPlayerMini(bool feedback = true);

  void playNext();
  void playPrevious();
  void playTrack(uint16_t trackNum);
  void playFolderTrack(uint8_t folderNum, uint8_t trackNum);
  void play();
  void pause();

  // void playFromMP3Folder(uint16_t trackNum);
  // void playAdvertisement(uint16_t trackNum);
  // void stopAdvertisement();
  void incVolume();
  void decVolume();
  void setVolume(uint8_t volume);
  // void EQSelect(uint8_t setting);
  // void loop();
  void playbackSource(uint8_t source);
  void standbyMode();
  void normalMode();
  void reset();
  // void resume();
  // void pause();
  // void playFolder(uint8_t folderNum, uint8_t trackNum);
  // void playLargeFolder(uint8_t folderNum, uint16_t trackNum);
  // void volumeAdjustSet(uint8_t gain);
  // void startRepeatPlay();
  // void stopRepeatPlay();
  // void repeatFolder(uint16_t folder);
  // void randomAll();
  // void startRepeat();
  // void stopRepeat();
  // void startDAC();
  // void stopDAC();
  // void sleep();
  // void wakeUp();

  // bool isPlaying();
  // int16_t currentVolume();
  // int16_t currentEQ();
  // int16_t currentMode();
  // int16_t currentVersion();
  // int16_t numUsbTracks();
  // int16_t numSdTracks();
  // int16_t numFlashTracks();
  // int16_t currentUsbTrack();
  // int16_t currentSdTrack();
  // int16_t currentFlashTrack();
  // int16_t numTracksInFolder(uint8_t folder);
  // int16_t numFolders();

  // void setTimeout(unsigned long threshold);
  // void sendData();
  // void flush();
  // int16_t query(uint8_t cmd, uint8_t msb = 0, uint8_t lsb = 0);
  // bool parseFeedback();

  // void printStack(stack _stack);
  void getStack(uint8_t *_stack) const;
  void getStack(stack_t &_stack) const;
  void setStack(uint8_t *_stack);
  //  void printError();
};

} // namespace DFPLAYERMINI

#endif
