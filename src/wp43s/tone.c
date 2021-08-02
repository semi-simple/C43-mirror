/* This file is part of 43S.
 *
 * 43S is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 43S is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 43S.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tone.h"

#include "screen.h"

#include <stdio.h>
#if defined(TESTSUITE_BUILD)
#elif defined(PC_BUILD) && defined(__MINGW64__)
  #define NOMINMAX
  #include <windows.h>
#elif defined(PC_BUILD) && defined(__APPLE__)
#elif defined(PC_BUILD)
#endif

#include "wp43s.h"

#ifdef DMCP_BUILD
  TO_QSPI uint32_t frequency[10] = {164814, 220000, 246942, 277183, 293665, 329628, 369995, 415305, 440000, 554365};
#endif // DMCP_BUILD

static void playTone(uint16_t toneNum) {
#if defined(TESTSUITE_BUILD)
#elif defined(PC_BUILD) && defined(__MINGW64__)
  char filename[32];
  if(toneNum < 10) {
    sprintf(filename, "res\\tone\\tone%" PRIu16 ".wav", toneNum);
    PlaySoundA(filename, NULL, SND_FILENAME | SND_SYNC | SND_NODEFAULT);
  }
#elif defined(PC_BUILD) && defined(__APPLE__)
#elif defined(PC_BUILD)
#elif defined(DMCP_BUILD)
  if(toneNum < 10) {
    start_buzzer_freq(frequency[toneNum]);
    sys_delay(250);
    stop_buzzer();
  }
#endif
}

void fnTone(uint16_t toneNum) {
  #ifdef DMCP_BUILD
    lcd_refresh();
  #else // !DMCP_BUILD
    refreshLcd(NULL);
  #endif // DMCP_BUILD

  playTone(toneNum);
}

void fnBeep(uint16_t unusedButMandatoryParameter) {
  #ifdef DMCP_BUILD
    lcd_refresh();
  #else // !DMCP_BUILD
    refreshLcd(NULL);
  #endif // DMCP_BUILD

  playTone(8);
  playTone(5);
  playTone(9);
  playTone(8);
}

