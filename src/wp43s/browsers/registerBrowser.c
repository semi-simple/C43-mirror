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

/********************************************//**
 * \file registerBrowser.c The register browser application
 ***********************************************/

#include "browsers/registerBrowser.h"

#include "charString.h"
#include "debug.h"
#include "display.h"
#include "flags.h"
#include "fonts.h"
#include "items.h"
#include "registers.h"
#include "screen.h"
#include <string.h>

#include "wp43s.h"



#ifndef TESTSUITE_BUILD
  /********************************************//**
   * \brief The register browser
   *
   * \param[in] unusedButMandatoryParameter uint16_t
   * \return void
   ***********************************************/
  void registerBrowser(uint16_t unusedButMandatoryParameter) {
    int16_t registerNameWidth;

    if(calcMode != CM_REGISTER_BROWSER) {
      if(calcMode == CM_AIM) {
        hideCursor();
        cursorEnabled = false;
      }

      previousCalcMode = calcMode;
      calcMode = CM_REGISTER_BROWSER;
      clearSystemFlag(FLAG_ALPHA);
      return;
    }

    if(currentRegisterBrowserScreen == 9999) { // Init
      currentRegisterBrowserScreen = REGISTER_X;
      rbrMode = RBR_GLOBAL;
      showContent = true;
      rbr1stDigit = true;
    }

    if(rbrMode == RBR_GLOBAL) { // Global registers
      for(int16_t row=0; row<10; row++) {
        calcRegister_t regist = (currentRegisterBrowserScreen + row) % FIRST_LOCAL_REGISTER;
        switch(regist) {
          case REGISTER_X: strcpy(tmpString, "X:"); break;
          case REGISTER_Y: strcpy(tmpString, "Y:"); break;
          case REGISTER_Z: strcpy(tmpString, "Z:"); break;
          case REGISTER_T: strcpy(tmpString, "T:"); break;
          case REGISTER_A: strcpy(tmpString, "A:"); break;
          case REGISTER_B: strcpy(tmpString, "B:"); break;
          case REGISTER_C: strcpy(tmpString, "C:"); break;
          case REGISTER_D: strcpy(tmpString, "D:"); break;
          case REGISTER_L: strcpy(tmpString, "L:"); break;
          case REGISTER_I: strcpy(tmpString, "I:"); break;
          case REGISTER_J: strcpy(tmpString, "J:"); break;
          case REGISTER_K: strcpy(tmpString, "K:"); break;
          default: sprintf(tmpString, "R%02d:", regist);
        }

        // register name or number
        registerNameWidth = showString(tmpString, &standardFont, 1, 219 - 22 * row, vmNormal, false, true);

        if(   (regist <  REGISTER_X && regist % 5 == 4)
           || (regist >= REGISTER_X && regist % 4 == 3)) {
          lcd_fill_rect(0, 218 - 22 * row, SCREEN_WIDTH, 1, LCD_EMPTY_VALUE);
        }

        if(getRegisterDataType(regist) == dtReal34) {
          if(showContent) {
            real34ToDisplayString(REGISTER_REAL34_DATA(regist), getRegisterAngularMode(regist), tmpString, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth, 34, false, STD_SPACE_4_PER_EM);
          }
          else {
            sprintf(tmpString, "%d bytes", (int16_t)TO_BYTES(REAL34_SIZE));
          }
        }
        else if(getRegisterDataType(regist) == dtComplex34) {
          if(showContent) {
            complex34ToDisplayString(REGISTER_COMPLEX34_DATA(regist), tmpString, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth, 34, false, STD_SPACE_4_PER_EM);
          }
          else {
            sprintf(tmpString, "%d bytes", (int16_t)TO_BYTES(COMPLEX34_SIZE));
          }
        }
        else if(getRegisterDataType(regist) == dtLongInteger) {
          if(showContent) {
            if(getRegisterLongIntegerSign(regist) == LI_NEGATIVE) {
              longIntegerRegisterToDisplayString(regist, tmpString, TMP_STR_LENGTH, SCREEN_WIDTH - 1 - registerNameWidth, 50, STD_SPACE_4_PER_EM, false);   //JM added last parameter: Allow LARGELI
            }
            else {
              longIntegerRegisterToDisplayString(regist, tmpString, TMP_STR_LENGTH, SCREEN_WIDTH - 9 - registerNameWidth, 50, STD_SPACE_4_PER_EM, false);   //JM added last parameter: Allow LARGELI
            }
          }
          else {
            sprintf(tmpString, "%" PRIu32 " bits " STD_CORRESPONDS_TO " 4+%" PRIu32 " bytes", (uint32_t)TO_BYTES(getRegisterMaxDataLength(regist)) * 8, (uint32_t)TO_BYTES(getRegisterMaxDataLength(regist)));
          }
        }
        else if(getRegisterDataType(regist) == dtShortInteger) {
          if(showContent) {
            shortIntegerToDisplayString(regist, tmpString, false);
          }
          else {
            strcpy(tmpString, "64 bits " STD_CORRESPONDS_TO " 8 bytes");
          }
        }
        else if(getRegisterDataType(regist) == dtString) {
          if(showContent) {
            strcpy(tmpString, STD_LEFT_SINGLE_QUOTE);
            strncat(tmpString, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
            strcat(tmpString, STD_RIGHT_SINGLE_QUOTE);
            if(stringWidth(tmpString, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
              tmpString[stringLastGlyph(tmpString)] = 0;
              while(stringWidth(tmpString, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
                tmpString[stringLastGlyph(tmpString)] = 0;
              }
             strcat(tmpString + stringByteLength(tmpString), STD_ELLIPSIS);
            }
          }
          else {
            sprintf(tmpString, "%" PRIu32 " character%s " STD_CORRESPONDS_TO " 4+%" PRIu32 " bytes", (uint32_t)stringGlyphLength(REGISTER_STRING_DATA(regist)), stringGlyphLength(REGISTER_STRING_DATA(regist))==1 ? "" : "s", (uint32_t)TO_BYTES(getRegisterMaxDataLength(regist)));
          }
        }
        else if(getRegisterDataType(regist) == dtTime) {
          if(showContent) {
            timeToDisplayString(regist, tmpString, true);
          }
          else {
            sprintf(tmpString, "%d bytes", (int16_t)TO_BYTES(REAL34_SIZE));
          }
        }
        else if(getRegisterDataType(regist) == dtDate) {
          if(showContent) {
            dateToDisplayString(regist, tmpString);
          }
          else {
            sprintf(tmpString, "%d bytes", (int16_t)TO_BYTES(REAL34_SIZE));
          }
        }
        else if(getRegisterDataType(regist) == dtReal34Matrix) {
          if(showContent) {
            real34MatrixToDisplayString(regist, tmpString);
          }
          else {
            dataBlock_t* dblock = REGISTER_REAL34_MATRIX_DBLOCK(regist);
            sprintf(tmpString, "%" PRIu16 " element%s " STD_CORRESPONDS_TO " 4+%" PRIu32 " bytes", dblock->matrixRows * dblock->matrixColumns, (dblock->matrixRows * dblock->matrixColumns)==1 ? "" : "s", (uint32_t)TO_BYTES(dblock->matrixRows * dblock->matrixColumns * REAL34_SIZE));
          }
        }
        else if(getRegisterDataType(regist) == dtConfig) {
          if(showContent) {
            strcpy(tmpString, "Configuration data");
          }
          else {
            sprintf(tmpString, "%d bytes", (int16_t)TO_BYTES(CONFIG_SIZE));
          }
        }
        else {
          sprintf(tmpString, "Data type %s: to be coded", getDataTypeName(getRegisterDataType(regist), false, true));
        }

        showString(tmpString, &standardFont, SCREEN_WIDTH - stringWidth(tmpString, &standardFont, false, true) - 1, 219-22*row, vmNormal, false, true);
      }
    }

    else if(rbrMode == RBR_LOCAL) { // Local registers
      if(currentNumberOfLocalRegisters != 0) { // Local registers are allocated
        for(int16_t row=0; row<10; row++) {
          calcRegister_t regist = currentRegisterBrowserScreen + row;
          if(regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) {
            sprintf(tmpString, "R.%02d:", regist);

            // register number
            registerNameWidth = showString(tmpString, &standardFont, 1, 219 - 22 * row, vmNormal, true, true);

            if(   (regist <  REGISTER_X && regist % 5 == 4)
               || (regist >= REGISTER_X && regist % 4 == 3)) {
              lcd_fill_rect(0, 218 - 22 * row, SCREEN_WIDTH, 1, LCD_EMPTY_VALUE);
            }

            if(getRegisterDataType(regist) == dtReal34) {
              if(showContent) {
                real34ToDisplayString(REGISTER_REAL34_DATA(regist), getRegisterAngularMode(regist), tmpString, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth, 34, false, STD_SPACE_4_PER_EM);
              }
              else {
                sprintf(tmpString, "%d bytes", (int16_t)TO_BYTES(REAL34_SIZE));
              }
            }
            else if(getRegisterDataType(regist) == dtComplex34) {
              if(showContent) {
                complex34ToDisplayString(REGISTER_COMPLEX34_DATA(regist), tmpString, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth, 34, false, STD_SPACE_4_PER_EM);
              }
              else {
                sprintf(tmpString, "4+%d bytes", (int16_t)TO_BYTES(COMPLEX34_SIZE));
              }
            }
            else if(getRegisterDataType(regist) == dtLongInteger) {
              if(showContent) {
                longIntegerRegisterToDisplayString(regist, tmpString, TMP_STR_LENGTH, SCREEN_WIDTH - 1 - registerNameWidth, 50, STD_SPACE_4_PER_EM, false);   //JM added last parameter: Allow LARGELI
              }
              else {
                sprintf(tmpString, "%" PRIu32 " bits " STD_CORRESPONDS_TO " 4+4+%" PRIu32 " bytes", (uint32_t)TO_BYTES(getRegisterMaxDataLength(regist)) * 8, (uint32_t)TO_BYTES(getRegisterMaxDataLength(regist)));
              }
            }
            else if(getRegisterDataType(regist) == dtShortInteger) {
              if(showContent) {
                shortIntegerToDisplayString(regist, tmpString, false);
              }
              else {
                strcpy(tmpString, "64 bits " STD_CORRESPONDS_TO " 4+8 bytes");
              }
            }
            else if(getRegisterDataType(regist) == dtString) {
              if(showContent) {
                strcpy(tmpString, STD_LEFT_SINGLE_QUOTE);
                strncat(tmpString, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
                strcat(tmpString, STD_RIGHT_SINGLE_QUOTE);
                if(stringWidth(tmpString, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
                  tmpString[stringLastGlyph(tmpString)] = 0;
                  while(stringWidth(tmpString, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
                    tmpString[stringLastGlyph(tmpString)] = 0;
                  }
                 strcat(tmpString + stringByteLength(tmpString), STD_ELLIPSIS);
                }
              }
              else {
                sprintf(tmpString, "%" PRIu32 " character%s " STD_CORRESPONDS_TO " 4+4+%" PRIu32 " bytes", (uint32_t)stringGlyphLength(REGISTER_STRING_DATA(regist)), stringGlyphLength(REGISTER_STRING_DATA(regist))==1 ? "" : "s", (uint32_t)TO_BYTES(getRegisterMaxDataLength(regist)));
              }
            }
            else if(getRegisterDataType(regist) == dtTime) {
              if(showContent) {
                timeToDisplayString(regist, tmpString, true);
              }
              else {
                sprintf(tmpString, "%d bytes", (int16_t)TO_BYTES(REAL34_SIZE));
              }
            }
            else if(getRegisterDataType(regist) == dtDate) {
              if(showContent) {
                dateToDisplayString(regist, tmpString);
              }
              else {
                sprintf(tmpString, "%d bytes", (int16_t)TO_BYTES(REAL34_SIZE));
              }
            }
            else if(getRegisterDataType(regist) == dtReal34Matrix) {
              if(showContent) {
                real34MatrixToDisplayString(regist, tmpString);
              }
              else {
                dataBlock_t* dblock = REGISTER_REAL34_MATRIX_DBLOCK(regist);
                sprintf(tmpString, "%" PRIu16 " element%s " STD_CORRESPONDS_TO " 4+%" PRIu32 " bytes", dblock->matrixRows * dblock->matrixColumns, (dblock->matrixRows * dblock->matrixColumns)==1 ? "" : "s", (uint32_t)TO_BYTES(dblock->matrixRows * dblock->matrixColumns * REAL34_SIZE));
              }
            }
            else if(getRegisterDataType(regist) == dtConfig) {
              if(showContent) {
                strcpy(tmpString, "Configuration data");
              }
              else {
                sprintf(tmpString, "%d bytes", (int16_t)TO_BYTES(CONFIG_SIZE));
              }
            }
            else {
              sprintf(tmpString, "Data type %s: to be coded", getDataTypeName(getRegisterDataType(regist), false, true));
            }

            showString(tmpString, &standardFont, SCREEN_WIDTH - stringWidth(tmpString, &standardFont, false, true), 219 - 22 * row, vmNormal, false, true);
          }
        }
      }
      else { // no local register allocated
        rbrMode = RBR_GLOBAL;
        registerBrowser(NOPARAM);
      }
    }
  }
#endif // TESTSUITE_BUILD
