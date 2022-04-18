/*!
 * @file DFPlayerMini.cpp
 *
 * @mainpage DFPlayerMini
 *
 * @section intro_sec Introduction
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
 * @section author Author
 *
 * Originally written by Power_Broker as a hobby.
 * Modified by Walwan for general use.
 *
 */

#include "DFPlayerMini.hpp"

using namespace DFPLAYERMINI;

/**************************************************************************/
/*!
        @brief  Class constructor
        @param  feedback
                Boolean to control whether the packet requires the module
                to give feedback.
*/
/**************************************************************************/
DFPlayerMini::DFPlayerMini(bool feedback) {
  if (feedback)
    _sendStack.feedback = PACKET::FEEDBACK::YES;
  else
    _sendStack.feedback = PACKET::FEEDBACK::NO;
}

/**************************************************************************/
/*!
        @brief  Play the next song in chronological order.
*/
/**************************************************************************/
void DFPlayerMini::playNext() {
  _sendStack.command = CONTROLCMD::PLAY_NEXT;
  _sendStack.paramMSB = 0;
  _sendStack.paramLSB = 0;

  setChecksum();
}

/**************************************************************************/
/*!
        @brief  Play the previous song in chronological order.
*/
/**************************************************************************/
void DFPlayerMini::playPrevious() {
  _sendStack.command = CONTROLCMD::PLAY_PREV;
  _sendStack.paramMSB = 0;
  _sendStack.paramLSB = 0;

  setChecksum();
}

/**************************************************************************/
/*!
        @brief  Play a specific track in the root folder.
        @param    trackNum
                          The track number to play ranging from 0 to 2999.
                          If the input trackNum is greater than the limit,
                          will use the maximum value
*/
/**************************************************************************/
void DFPlayerMini::playTrack(uint16_t trackNum) {
  _sendStack.command = CONTROLCMD::PLAY_TRACK;

  trackNum =
      trackNum <= LIMIT::MAX_ROOT_TRACK ? trackNum : LIMIT::MAX_ROOT_TRACK;
  _sendStack.paramMSB = static_cast<uint8_t>(trackNum >> 8);
  _sendStack.paramLSB = static_cast<uint8_t>(trackNum & 0xFF);

  setChecksum();
}

/**************************************************************************/
/*!
        @brief  Play a specific track in a specific folder.
        @param    folderNum
                          The folder number to find a track.
        @param    trackNum
                          The track number to play.
*/
/**************************************************************************/
void DFPlayerMini::playFolderTrack(uint8_t folderNum, uint8_t trackNum) {
  _sendStack.command = CONTROLCMD::PLAY_FOLDER_TRACK;

  folderNum = folderNum >= LIMIT::MIN_FOLDER ? folderNum : LIMIT::MIN_FOLDER;
  folderNum = folderNum <= LIMIT::MAX_FOLDER ? folderNum : LIMIT::MAX_FOLDER;
  _sendStack.paramMSB = folderNum;

  trackNum =
      trackNum >= LIMIT::MIN_FOLDER_TRACK ? trackNum : LIMIT::MIN_FOLDER_TRACK;
  // trackNum = trackNum <= LIMIT::MAX_FOLDER_TRACK ? trackNum :
  // LIMIT::MAX_FOLDER_TRACK;
  _sendStack.paramLSB = trackNum;

  setChecksum();
}

/**************************************************************************/
/*!
        @brief  Stop the current playback
*/
/**************************************************************************/
void DFPlayerMini::pause() {
  _sendStack.command = CONTROLCMD::PAUSE;
  _sendStack.paramMSB = 0;
  _sendStack.paramLSB = 0;

  setChecksum();
}

/**************************************************************************/
/*!
        @brief  Play a specific track in the folder named "MP3".
        @param    trackNum
                          The track number to play.
*/
/**************************************************************************/
// void DFPlayerMini::playFromMP3Folder(uint16_t trackNum) {
//   _sendStack.command = USE_MP3_FOLDER;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = (trackNum >> 8) & 0xFF;
//   _sendStack.paramLSB = trackNum & 0xFF;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Interrupt the current track with a new track.
        @param    trackNum
                          The track number to play.
*/
/**************************************************************************/
// void DFPlayerMini::playAdvertisement(uint16_t trackNum) {
//   _sendStack.command = INSERT_ADVERT;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = (trackNum >> 8) & 0xFF;
//   _sendStack.paramLSB = trackNum & 0xFF;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Stop the interrupting track.
*/
/**************************************************************************/
// void DFPlayerMini::stopAdvertisement() {
//   _sendStack.command = STOP_ADVERT;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = 0;
//   _sendStack.paramLSB = 0;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Increment the volume by 1 out of 30.
*/
/**************************************************************************/
void DFPlayerMini::incVolume() {
  _sendStack.command = CONTROLCMD::INC_VOL;
  _sendStack.paramMSB = 0;
  _sendStack.paramLSB = 0;

  setChecksum();
}

/**************************************************************************/
/*!
        @brief  Decrement the volume by 1 out of 30.
*/
/**************************************************************************/
void DFPlayerMini::decVolume() {
  _sendStack.command = CONTROLCMD::DEC_VOL;
  _sendStack.paramMSB = 0;
  _sendStack.paramLSB = 0;

  setChecksum();
}

/**************************************************************************/
/*!
        @brief  Set the volume to a specific value out of 30.
        @param    volume
                          The volume level (0 - 30).
*/
/**************************************************************************/
void DFPlayerMini::setVolume(uint8_t volume) {
  _sendStack.command = CONTROLCMD::SET_VOL;
  _sendStack.paramMSB = 0;

  volume = volume <= LIMIT::MAX_VOLUME ? volume : LIMIT::MAX_VOLUME;
  _sendStack.paramLSB = volume;

  setChecksum();
}

/**************************************************************************/
/*!
        @brief  Set the EQ mode.
        @param    setting
                          The desired EQ ID.
*/
/**************************************************************************/
// void DFPlayerMini::EQSelect(uint8_t setting) {
//   if (setting <= 5) {
//     _sendStack.command = EQ;
//     _sendStack.feedback = NO_FEEDBACK;
//     _sendStack.paramMSB = 0;
//     _sendStack.paramLSB = setting;
//
//     setChecksum();
//   }
// }

/**************************************************************************/
/*!
        @brief  Loop a specific track.
        @param    trackNum
                          The track number to play.
*/
/**************************************************************************/
// void DFPlayerMini::loop(uint16_t trackNum) {
//   _sendStack.command = PLAYBACK_MODE;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = (trackNum >> 8) & 0xFF;
//   _sendStack.paramLSB = trackNum & 0xFF;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Specify the playback source.
        @param    source
                          The playback source ID.
*/
/**************************************************************************/
void DFPlayerMini::playbackSource(uint8_t source) {
  _sendStack.command = CONTROLCMD::SET_PLAYBACK_SRC;
  _sendStack.paramMSB = 0;

  switch (source) {
  case PLAYBACK_SRC::U:
  case PLAYBACK_SRC::TF:
  case PLAYBACK_SRC::AUX:
  case PLAYBACK_SRC::SLEEP:
    //  case PLAYBACK_SRC::FLASH:
    break;
  default:
    source = PLAYBACK_SRC::TF;
  }
  _sendStack.paramLSB = source;

  setChecksum();
}

/**************************************************************************/
/*!
        @brief  Put the MP3 player in standby mode (this is NOT sleep mode).
*/
/**************************************************************************/
void DFPlayerMini::standbyMode() {
  _sendStack.command = CONTROLCMD::MODE_STANDBY;
  _sendStack.paramMSB = 0;
  _sendStack.paramLSB = 0;

  setChecksum();
}

/**************************************************************************/
/*!
        @brief  Pull the MP3 player out of standby mode.
*/
/**************************************************************************/
void DFPlayerMini::normalMode() {
  _sendStack.command = CONTROLCMD::MODE_NORMAL;
  _sendStack.paramMSB = 0;
  _sendStack.paramLSB = 0;

  setChecksum();
}

/**************************************************************************/
/*!
        @brief  Reset all settings to factory default.
*/
/**************************************************************************/
void DFPlayerMini::reset() {
  _sendStack.command = CONTROLCMD::MODE_RESET;
  _sendStack.paramMSB = 0;
  _sendStack.paramLSB = 0;

  setChecksum();
}

/**************************************************************************/
/*!
        @brief  Resume playing current track.
*/
/**************************************************************************/
// void DFPlayerMini::resume() {
//   _sendStack.command = PLAYBACK;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = 0;
//   _sendStack.paramLSB = 1;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Pause playing current track.
*/
/**************************************************************************/
// void DFPlayerMini::pause() {
//   _sendStack.command = PAUSE;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = 0;
//   _sendStack.paramLSB = 1;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Play a specific track from a specific folder.
        @param    folderNum
                          The folder number.
        @param    trackNum
                          The track number to play.
*/
/**************************************************************************/
// void DFPlayerMini::playFolder(uint8_t folderNum, uint8_t trackNum) {
//   _sendStack.command = SPEC_FOLDER;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = folderNum;
//   _sendStack.paramLSB = trackNum;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Play a specific track from a specific folder, where the track
                        names are numbered 4 digit (e.g. 1234-mysong.mp3) and
   can be up to 3000. Only 15 folders ("01" to "15") are supported in this mode.
        @param    folderNum
                          The folder number.
        @param    trackNum
                          The track number to play.
*/
/**************************************************************************/
// void DFPlayerMini::playLargeFolder(uint8_t folderNum, uint16_t trackNum) {
//   const uint16_t arg = (((uint16_t)folderNum) << 12) | (trackNum & 0xfff);
//
//   _sendStack.command = SPEC_TRACK_3000;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = arg >> 8;
//   _sendStack.paramLSB = arg & 0xff;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Specify volume gain.
        @param    gain
                          The specified volume gain.
*/
/**************************************************************************/
// void DFPlayerMini::volumeAdjustSet(uint8_t gain) {
//   if (gain <= 31) {
//     _sendStack.command = VOL_ADJ;
//     _sendStack.feedback = NO_FEEDBACK;
//     _sendStack.paramMSB = 0;
//     _sendStack.paramLSB = VOL_ADJUST + gain;
//
//     setChecksum();
//   }
// }

/**************************************************************************/
/*!
        @brief  Play all tracks.
*/
/**************************************************************************/
// void DFPlayerMini::startRepeatPlay() {
//   _sendStack.command = REPEAT_PLAY;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = 0;
//   _sendStack.paramLSB = START_REPEAT;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Stop repeat play.
*/
/**************************************************************************/
// void DFPlayerMini::stopRepeatPlay() {
//   _sendStack.command = REPEAT_PLAY;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = 0;
//   _sendStack.paramLSB = STOP_REPEAT;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Play all tracks in a given folder.
        @param    folderNum
                          The folder number.
*/
/**************************************************************************/
// void DFPlayerMini::repeatFolder(uint16_t folder) {
//   _sendStack.command = REPEAT_FOLDER;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = (folder >> 8) & 0xFF;
//   _sendStack.paramLSB = folder & 0xFF;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Play all tracks in a random order.
*/
/**************************************************************************/
// void DFPlayerMini::randomAll() {
//   _sendStack.command = RANDOM_ALL;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = 0;
//   _sendStack.paramLSB = 0;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Repeat the current track.
*/
/**************************************************************************/
// void DFPlayerMini::startRepeat() {
//   _sendStack.command = REPEAT_CURRENT;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = 0;
//   _sendStack.paramLSB = 0;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Stop repeat play of the current track.
*/
/**************************************************************************/
// void DFPlayerMini::stopRepeat() {
//   _sendStack.command = REPEAT_CURRENT;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = 0;
//   _sendStack.paramLSB = 1;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Turn on DAC.
*/
/**************************************************************************/
// void DFPlayerMini::startDAC() {
//   _sendStack.command = SET_DAC;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = 0;
//   _sendStack.paramLSB = 0;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Turn off DAC.
*/
/**************************************************************************/
// void DFPlayerMini::stopDAC() {
//   _sendStack.command = SET_DAC;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = 0;
//   _sendStack.paramLSB = 1;
//
//   setChecksum();
// }

/**************************************************************************/
/*!
        @brief  Put the MP3 player into sleep mode.
*/
/**************************************************************************/
// void DFPlayerMini::sleep() { playbackSource(SLEEP); }

/**************************************************************************/
/*!
        @brief  Pull the MP3 player out of sleep mode.
*/
/**************************************************************************/
// void DFPlayerMini::wakeUp() { playbackSource(TF); }

/**************************************************************************/
/*!
        @brief  Determine if a track is currently playing.
        @return True if a track is currently playing, false if not, -1 if error.
*/
/**************************************************************************/
// bool DFPlayerMini::isPlaying() {
//   int16_t result = query(GET_STATUS_);
//
//   if (result != -1)
//     return (result & 1);
//
//   return 0;
// }

/**************************************************************************/
/*!
        @brief  Determine the current volume setting.
        @return Volume level (0-30), -1 if error.
*/
/**************************************************************************/
// int16_t DFPlayerMini::currentVolume() { return query(GET_VOL); }

/**************************************************************************/
/*!
        @brief  Determine the current EQ setting.
        @return EQ setting, -1 if error.
*/
/**************************************************************************/
// int16_t DFPlayerMini::currentEQ() { return query(GET_EQ); }

/**************************************************************************/
/*!
        @brief  Determine the current mode.
        @return Mode, -1 if error.
*/
/**************************************************************************/
// int16_t DFPlayerMini::currentMode() { return query(GET_MODE); }

/**************************************************************************/
/*!
        @brief  Determine the current firmware version.
        @return Firmware version, -1 if error.
*/
/**************************************************************************/
// int16_t DFPlayerMini::currentVersion() { return query(GET_VERSION); }

/**************************************************************************/
/*!
        @brief  Determine the number of tracks accessible via USB.
        @return Number of tracks accessible via USB, -1 if error.
*/
/**************************************************************************/
// int16_t DFPlayerMini::numUsbTracks() { return query(GET_TF_FILES); }

/**************************************************************************/
/*!
        @brief  Determine the number of tracks accessible via SD card.
        @return Number of tracks accessible via SD card, -1 if error.
*/
/**************************************************************************/
// int16_t DFPlayerMini::numSdTracks() { return query(GET_U_FILES); }

/**************************************************************************/
/*!
        @brief  Determine the number of tracks accessible via flash.
        @return Number of tracks accessible via flash, -1 if error.
*/
/**************************************************************************/
// int16_t DFPlayerMini::numFlashTracks() { return query(GET_FLASH_FILES); }

/**************************************************************************/
/*!
        @brief  Determine the current track played via USB.
        @return Current track played via USB, -1 if error.
*/
/**************************************************************************/
// int16_t DFPlayerMini::currentUsbTrack() { return query(GET_TF_TRACK); }

/**************************************************************************/
/*!
        @brief  Determine the current track played via SD card.
        @return Current track played via SD card, -1 if error.
*/
/**************************************************************************/
// int16_t DFPlayerMini::currentSdTrack() { return query(GET_U_TRACK); }

/**************************************************************************/
/*!
        @brief  Determine the current track played via flash.
        @return Current track played via flash, -1 if error.
*/
/**************************************************************************/
// int16_t DFPlayerMini::currentFlashTrack() { return query(GET_FLASH_TRACK); }

/**************************************************************************/
/*!
        @brief  Determine the number of tracks in the specified folder.
        @param    folder
                          The folder number.
        @return Number of tracks in the specified folder, -1 if error.
*/
/**************************************************************************/
// int16_t DFPlayerMini::numTracksInFolder(uint8_t folder) {
//   return query(GET_FOLDER_FILES, (folder >> 8) & 0xFF, folder & 0xFF);
// }

/**************************************************************************/
/*!
        @brief  Determine the number of folders available.
        @return Number of folders available, -1 if error.
*/
/**************************************************************************/
// int16_t DFPlayerMini::numFolders() { return query(GET_FOLDERS); }

/**************************************************************************/
/*!
        @brief  Set the timout value for MP3 player query responses.
        @param    threshold
                          Number of ms allowed for the MP3 player to respond
   (timeout) to a query.
*/
/**************************************************************************/
// void DFPlayerMini::setTimeout(unsigned long threshold) {
//   _threshold = threshold;
//   timoutTimer.begin(_threshold);
// }

/**************************************************************************/
/*!
        @brief  return the checksum of a given config/command
                packet.
        @param    _stack
                          reference to a struct containing the config/command
   packet to calculate the checksum over.
*/
/**************************************************************************/
uint16_t DFPlayerMini::calChecksum(stack_t &_stack) {
  return (~(_stack.version + _stack.length + _stack.command + _stack.feedback +
            _stack.paramMSB + _stack.paramLSB)) +
         1;
}

/**************************************************************************/
/*!
        @brief  return the checksum of a given config/command
                packet.
        @param    _stack
                          reference to a struct containing the config/command
   packet to calculate the checksum over.
*/
/**************************************************************************/
void DFPlayerMini::setChecksum() {
  uint16_t checksum = calChecksum(_sendStack);
  _sendStack.checksumMSB = static_cast<uint8_t>(checksum >> 8);
  _sendStack.checksumLSB = static_cast<uint8_t>(checksum);
}

/**************************************************************************/
/*!
        @brief  Send a config/command packet to the MP3 player.
*/
/**************************************************************************/
// void DFPlayerMini::sendData() {
//   _serial->write(_sendStack.start_byte);
//   _serial->write(_sendStack.version);
//   _serial->write(_sendStack.length);
//   _serial->write(_sendStack.command);
//   _serial->write(_sendStack.feedback);
//   _serial->write(_sendStack.paramMSB);
//   _serial->write(_sendStack.paramLSB);
//   _serial->write(_sendStack.checksumMSB);
//   _serial->write(_sendStack.checksumLSB);
//   _serial->write(_sendStack.end_byte);
//
//   if (_debug) {
//     Serial.print(F("Sent "));
//     printStack(_sendStack);
//     Serial.println();
//   }
// }

/**************************************************************************/
/*!
        @brief  Clear out the serial input buffer connected to the MP3 player.
*/
/**************************************************************************/
// void DFPlayerMini::flush() {
//   while (_serial->available())
//     _serial->read();
// }

/**************************************************************************/
/*!
        @brief  Query the MP3 player for specific information.
        @param    cmd
                          The command/query ID.
        @param    msb
                          The payload/parameter MSB.
        @param    lsb
                          The payload/parameter LSB.
        @return Query response, -1 if error.
*/
/**************************************************************************/
// int16_t DFPlayerMini::query(uint8_t cmd, uint8_t msb, uint8_t lsb) {
//   flush();
//
//   _sendStack.command = cmd;
//   _sendStack.feedback = NO_FEEDBACK;
//   _sendStack.paramMSB = msb;
//   _sendStack.paramLSB = lsb;
//
//   setChecksum();
//   timoutTimer.start();
//
//   if (parseFeedback())
//     if (recStack.command != 0x40)
//       return (recStack.paramMSB << 8) | recStack.paramLSB;
//
//   return -1;
// }

/**************************************************************************/
/*!
        @brief  Parse MP3 player query responses.
        @return True if success, false if error.
*/
/**************************************************************************/
// bool DFPlayerMini::parseFeedback() {
//   while (true) {
//     if (_serial->available()) {
//       uint8_t recChar = _serial->read();
//
//       if (_debug) {
//         Serial.print(F("Rec: "));
//         Serial.println(recChar, HEX);
//         Serial.print(F("State: "));
//       }
//
//       switch (state) {
//       case find_start_byte: {
//         if (_debug)
//           Serial.println(F("find_start_byte"));
//
//         if (recChar == SB) {
//           recStack.start_byte = recChar;
//           state = find_ver_byte;
//         }
//         break;
//       }
//       case find_ver_byte: {
//         if (_debug)
//           Serial.println(F("find_ver_byte"));
//
//         if (recChar != VER) {
//           if (_debug)
//             Serial.println(F("ver error"));
//
//           state = find_start_byte;
//           return false;
//         }
//
//         recStack.version = recChar;
//         state = find_len_byte;
//         break;
//       }
//       case find_len_byte: {
//         if (_debug)
//           Serial.println(F("find_len_byte"));
//
//         if (recChar != LEN) {
//           if (_debug)
//             Serial.println(F("len error"));
//
//           state = find_start_byte;
//           return false;
//         }
//
//         recStack.length = recChar;
//         state = find_command_byte;
//         break;
//       }
//       case find_command_byte: {
//         if (_debug)
//           Serial.println(F("find_command_byte"));
//
//         recStack.command = recChar;
//         state = find_feedback_byte;
//         break;
//       }
//       case find_feedback_byte: {
//         if (_debug)
//           Serial.println(F("find_feedback_byte"));
//
//         recStack.feedback = recChar;
//         state = find_param_MSB;
//         break;
//       }
//       case find_param_MSB: {
//         if (_debug)
//           Serial.println(F("find_param_MSB"));
//
//         recStack.paramMSB = recChar;
//         state = find_param_LSB;
//         break;
//       }
//       case find_param_LSB: {
//         if (_debug)
//           Serial.println(F("find_param_LSB"));
//
//         recStack.paramLSB = recChar;
//         state = find_checksum_MSB;
//         break;
//       }
//       case find_checksum_MSB: {
//         if (_debug)
//           Serial.println(F("find_checksum_MSB"));
//
//         recStack.checksumMSB = recChar;
//         state = find_checksum_LSB;
//         break;
//       }
//       case find_checksum_LSB: {
//         if (_debug)
//           Serial.println(F("find_checksum_LSB"));
//
//         recStack.checksumLSB = recChar;
//
//         int recChecksum = (recStack.checksumMSB << 8) | recStack.checksumLSB;
//         setChecksum();
//         int calcChecksum = (recStack.checksumMSB << 8) |
//         recStack.checksumLSB;
//
//         if (recChecksum != calcChecksum) {
//           if (_debug) {
//             Serial.println(F("checksum error"));
//             Serial.print(F("recChecksum: 0x"));
//             Serial.println(recChecksum, HEX);
//             Serial.print(F("calcChecksum: 0x"));
//             Serial.println(calcChecksum, HEX);
//             Serial.println();
//           }
//
//           state = find_start_byte;
//           return false;
//         } else
//           state = find_end_byte;
//         break;
//       }
//       case find_end_byte: {
//         if (_debug)
//           Serial.println(F("find_end_byte"));
//
//         if (recChar != EB) {
//           if (_debug)
//             Serial.println(F("eb error"));
//
//           state = find_start_byte;
//           return false;
//         }
//
//         recStack.end_byte = recChar;
//         state = find_start_byte;
//         return true;
//         break;
//       }
//       default:
//         break;
//       }
//     }
//
//     if (timoutTimer.fire()) {
//       if (_debug)
//         Serial.println(F("timeout error"));
//
//       state = find_start_byte;
//       return false;
//     }
//   }
// }

/**************************************************************************/
/*!
        @brief  Print the entire contents of the specified config/command
                packet for debugging purposes.
        @param    _stack
                          Struct containing the config/command packet
                          to print.
*/
/**************************************************************************/
// void DFPlayerMini::printStack(stack _stack) {
//   Serial.println(F("Stack:"));
//   Serial.print(_stack.start_byte, HEX);
//   Serial.print(' ');
//   Serial.print(_stack.version, HEX);
//   Serial.print(' ');
//   Serial.print(_stack.length, HEX);
//   Serial.print(' ');
//   Serial.print(_stack.command, HEX);
//   Serial.print(' ');
//   Serial.print(_stack.feedback, HEX);
//   Serial.print(' ');
//   Serial.print(_stack.paramMSB, HEX);
//   Serial.print(' ');
//   Serial.print(_stack.paramLSB, HEX);
//   Serial.print(' ');
//   Serial.print(_stack.checksumMSB, HEX);
//   Serial.print(' ');
//   Serial.print(_stack.checksumLSB, HEX);
//   Serial.print(' ');
//   Serial.println(_stack.end_byte, HEX);
// }

/**************************************************************************/
/*!
        @brief    Save the current packet to send into the array _stack points
                  to.
        @param    _stack
                  uint8_t array to save the packet.
*/
/**************************************************************************/
void DFPlayerMini::getStack(uint8_t *_stack) const {
  _stack[0] = _sendStack.start_byte;
  _stack[1] = _sendStack.version;
  _stack[2] = _sendStack.length;
  _stack[3] = _sendStack.command;
  _stack[4] = _sendStack.feedback;
  _stack[5] = _sendStack.paramMSB;
  _stack[6] = _sendStack.paramLSB;
  _stack[7] = _sendStack.checksumMSB;
  _stack[8] = _sendStack.checksumLSB;
  _stack[9] = _sendStack.end_byte;
}

/**************************************************************************/
/*!
        @brief    Save the current packet to send into the stack_t struct.
        @param    _stack
                  reference to a stack_t struct to save the packet.
*/
/**************************************************************************/
void DFPlayerMini::getStack(stack_t &_stack) const { _stack = _sendStack; }

/**************************************************************************/
/*!
        @brief    Save the current received packet from the array _stack points
                  to.
        @param    _stack
                  uint8_t array to get the packet.
*/
/**************************************************************************/
void DFPlayerMini::setStack(uint8_t *_stack) {
  _recvStack.start_byte = _stack[0];
  _recvStack.version = _stack[1];
  _recvStack.length = _stack[2];
  _recvStack.command = _stack[3];
  _recvStack.feedback = _stack[4];
  _recvStack.paramMSB = _stack[5];
  _recvStack.paramLSB = _stack[6];
  _recvStack.checksumMSB = _stack[7];
  _recvStack.checksumLSB = _stack[8];
  _recvStack.end_byte = _stack[9];
}

/**************************************************************************/
/*!
        @brief  Print the error description if an error has been received
*/
/**************************************************************************/
// void DFPlayerMini::printError() {
//   if (recStack.command == 0x40) {
//     switch (recStack.paramLSB) {
//     case 0x1: {
//       Serial.println(F("Module busy (this info is returned when the "
//                        "initialization is not done)"));
//       break;
//     }
//     case 0x2: {
//       Serial.println(F("Currently sleep mode(supports only specified device
//       in "
//                        "sleep mode)"));
//       break;
//     }
//     case 0x3: {
//       Serial.println(F("Serial receiving error(a frame has not been received
//       "
//                        "completely yet)"));
//       break;
//     }
//     case 0x4: {
//       Serial.println(F("Checksum incorrect"));
//       break;
//     }
//     case 0x5: {
//       Serial.println(F("Specified track is out of current track scope"));
//       break;
//     }
//     case 0x6: {
//       Serial.println(F("Specified track is not found"));
//       break;
//     }
//     case 0x7: {
//       Serial.println(F("Insertion error(an inserting operation only can be "
//                        "done when a track is being played)"));
//       break;
//     }
//     case 0x8: {
//       Serial.println(
//           F("SD card reading failed(SD card pulled out or damaged)"));
//       break;
//     }
//     case 0xA: {
//       Serial.println(F("Entered into sleep mode"));
//       break;
//     }
//     default: {
//       Serial.print(F("Unknown error: "));
//       Serial.println(recStack.paramLSB);
//       break;
//     }
//     }
//   } else
//     Serial.println(F("No error"));
// }
