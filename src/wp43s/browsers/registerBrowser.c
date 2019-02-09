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

#include "wp43s.h"


/********************************************//**
 * \brief The register browser
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void registerBrowser(uint16_t unusedParamButMandatory) {
 int16_t registerNameWidth;

 if(currentRegisterBrowserScreen == 9999) { // Init
    currentRegisterBrowserScreen = REGISTER_X;
    rbrMode = RBR_GLOBAL;
    showContent = true;
    calcMode = CM_REGISTER_BROWSER;
  }

  if(currentRegisterBrowserScreen < 9999) {
    if(rbrMode == RBR_GLOBAL) { // Global registers
      clearScreen(false, true, true);

      calcRegister_t regist;
      for(int16_t row=0; row<10; row++) {
        regist = (currentRegisterBrowserScreen + row) % FIRST_LOCAL_REGISTER;
        if     (regist == REGISTER_X) strcpy(tmpStr3000, "X:");
        else if(regist == REGISTER_Y) strcpy(tmpStr3000, "Y:");
        else if(regist == REGISTER_Z) strcpy(tmpStr3000, "Z:");
        else if(regist == REGISTER_T) strcpy(tmpStr3000, "T:");
        else if(regist == REGISTER_A) strcpy(tmpStr3000, "A:");
        else if(regist == REGISTER_B) strcpy(tmpStr3000, "B:");
        else if(regist == REGISTER_C) strcpy(tmpStr3000, "C:");
        else if(regist == REGISTER_D) strcpy(tmpStr3000, "D:");
        else if(regist == REGISTER_L) strcpy(tmpStr3000, "L:");
        else if(regist == REGISTER_I) strcpy(tmpStr3000, "I:");
        else if(regist == REGISTER_J) strcpy(tmpStr3000, "J:");
        else if(regist == REGISTER_K) strcpy(tmpStr3000, "K:");
        else                          sprintf(tmpStr3000, "R%02d:", regist);

        // register name or number
        registerNameWidth = showString(tmpStr3000, &standardFont, 1, 219-22*row, vmNormal, false, true);

        if(   (regist <  REGISTER_X && regist % 5 == 4)
           || (regist >= REGISTER_X && regist % 4 == 3)) {
          for(int16_t x=0; x<SCREEN_WIDTH; x++) {
            setPixel(x, 218-22*row);
          }
        }

        if(getRegisterDataType(regist) == dtReal16) {
          if(showContent) {
            real16ToDisplayString(REGISTER_REAL16_DATA(regist), false, tmpStr3000);
          }
          else {
            strcpy(tmpStr3000, "8 bytes");
          }
        }
        else if(getRegisterDataType(regist) == dtReal34) {
          if(showContent) {
            real34ToDisplayString(REGISTER_REAL34_DATA(regist), tmpStr3000);
          }
          else {
            strcpy(tmpStr3000, "16 bytes");
          }
        }
        else if(getRegisterDataType(regist) == dtComplex16) {
          if(showContent) {
            complex16ToDisplayString(REGISTER_COMPLEX16_DATA(regist), tmpStr3000);
          }
          else {
            strcpy(tmpStr3000, "16 bytes");
          }
        }
        else if(getRegisterDataType(regist) == dtComplex34) {
          if(showContent) {
            complex34ToDisplayString(REGISTER_COMPLEX34_DATA(regist), tmpStr3000);
          }
          else {
            strcpy(tmpStr3000, "32 bytes");
          }
        }
        else if(getRegisterDataType(regist) == dtBigInteger) {
          if(showContent) {
            bigIntegerToDisplayString(regist, tmpStr3000);
          }
          else {
            sprintf(tmpStr3000, "%" FMT16U " bits := 2+%" FMT16U " bytes", *(REGISTER_DATA_MAX_LEN(regist)) * 8, *(REGISTER_DATA_MAX_LEN(regist)));
          }
        }
        else if(getRegisterDataType(regist) == dtSmallInteger) {
          if(showContent) {
            const font_t *font;

            font = &standardFont;
            smallIntegerToDisplayString(regist, tmpStr3000, &font);
          }
          else {
            strcpy(tmpStr3000, "64 bits := 8 bytes");
          }
        }
        else if(getRegisterDataType(regist) == dtString) {
          if(showContent) {
            strcpy(tmpStr3000, "'");
            strncat(tmpStr3000, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
            strcat(tmpStr3000, "'");
            if(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
              tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
              while(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
                tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
              }
             strcat(tmpStr3000 + stringByteLength(tmpStr3000), STD_ELLIPSIS);
            }
          }
          else {
            sprintf(tmpStr3000, "%" FMT32S " character%s := 2+%" FMT16U " bytes", stringGlyphLength(REGISTER_STRING_DATA(regist)), stringGlyphLength(REGISTER_STRING_DATA(regist))==1 ? "" : "s", *(REGISTER_DATA_MAX_LEN(regist)));
          }
        }
        else {
          sprintf(tmpStr3000, "Data type %s: to be coded", getDataTypeName(getRegisterDataType(regist), false, false));
        }

        showString(tmpStr3000, &standardFont, SCREEN_WIDTH - stringWidth(tmpStr3000, &standardFont, false, true) - 1, 219-22*row, vmNormal, false, true);
      }
    }

    else if(rbrMode == RBR_LOCAL) { // Local registers
      if(numberOfLocalRegisters != 0) { // Local registers are allocated
        clearScreen(false, true, true);

        calcRegister_t regist;
        for(int16_t row=0; row<10; row++) {
          regist = currentRegisterBrowserScreen + row;
          if(regist - FIRST_LOCAL_REGISTER < numberOfLocalRegisters) {
            sprintf(tmpStr3000, "R.%02d:", regist);

            // register number
            registerNameWidth = showString(tmpStr3000, &standardFont, 1, 219-22*row, vmNormal, false, false);

            if(   (regist <  REGISTER_X && regist % 5 == 4)
               || (regist >= REGISTER_X && regist % 4 == 3)) {
              for(int16_t x=0; x<SCREEN_WIDTH; x++) {
                setPixel(x, 218-22*row);
              }
            }

            if(getRegisterDataType(regist) == dtReal16) {
              if(showContent) {
                real16ToDisplayString(REGISTER_REAL16_DATA(regist), false, tmpStr3000);
              }
              else {
                strcpy(tmpStr3000, "4+8 bytes");
              }
            }
            else if(getRegisterDataType(regist) == dtReal34) {
              if(showContent) {
                real34ToDisplayString(REGISTER_REAL34_DATA(regist), tmpStr3000);
              }
              else {
                strcpy(tmpStr3000, "4+16 bytes");
              }
            }
            else if(getRegisterDataType(regist) == dtComplex16) {
              if(showContent) {
                complex16ToDisplayString(REGISTER_COMPLEX16_DATA(regist), tmpStr3000);
              }
              else {
                strcpy(tmpStr3000, "4+16 bytes");
              }
            }
            else if(getRegisterDataType(regist) == dtComplex34) {
              if(showContent) {
                complex34ToDisplayString(REGISTER_COMPLEX34_DATA(regist), tmpStr3000);
              }
              else {
                strcpy(tmpStr3000, "4+32 bytes");
              }
            }
            else if(getRegisterDataType(regist) == dtBigInteger) {
              if(showContent) {
                bigIntegerToDisplayString(regist, tmpStr3000);
              }
              else {
                sprintf(tmpStr3000, "%" FMT16U " bits := 4+2+%" FMT16U " bytes", *(REGISTER_DATA_MAX_LEN(regist)) * 8, *(REGISTER_DATA_MAX_LEN(regist)));
              }
            }
            else if(getRegisterDataType(regist) == dtSmallInteger) {
              if(showContent) {
                const font_t *font;

                font = &standardFont;
                smallIntegerToDisplayString(regist, tmpStr3000, &font);
              }
              else {
                strcpy(tmpStr3000, "64 bits := 4+8 bytes");
              }
            }
            else if(getRegisterDataType(regist) == dtString) {
              if(showContent) {
                strcpy(tmpStr3000, "'");
                strncat(tmpStr3000, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
                strcat(tmpStr3000, "'");
                if(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
                  tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
                  while(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
                    tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
                  }
                 strcat(tmpStr3000 + stringByteLength(tmpStr3000), STD_ELLIPSIS);
                }
              }
              else {
                sprintf(tmpStr3000, "%" FMT32S " character%s := 4+2+%" FMT16U " bytes", stringGlyphLength(REGISTER_STRING_DATA(regist)), stringGlyphLength(REGISTER_STRING_DATA(regist))==1 ? "" : "s", *(REGISTER_DATA_MAX_LEN(regist)));
              }
            }
            else {
              sprintf(tmpStr3000, "Data type %s: to be coded", getDataTypeName(getRegisterDataType(regist), false, false));
            }

            showString(tmpStr3000, &standardFont, SCREEN_WIDTH - stringWidth(tmpStr3000, &standardFont, false, true), 219-22*row, vmNormal, false, true);
          }
        }
      }
      else { // no local register allocated
        rbrMode = RBR_GLOBAL;
      }
    }
  }
}
