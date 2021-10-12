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
 * \file equation.c
 ***********************************************/

#include "solver/equation.h"

#include "charString.h"
#include "defines.h"
#include "fonts.h"
#include "memory.h"
#include "screen.h"
#include "wp43s.h"

void setEquation(uint16_t equationId, const char *equationString) {
  uint32_t newSizeInBlocks = TO_BLOCKS(stringByteLength(equationString) + 1);
  if(allFormulae[equationId].sizeInBlocks == 0) {
    allFormulae[equationId].pointerToFormulaData = TO_WP43SMEMPTR(wp43sAllocate(newSizeInBlocks));
  }
  else {
    allFormulae[equationId].pointerToFormulaData = TO_WP43SMEMPTR(wp43sReallocate(TO_PCMEMPTR(allFormulae[equationId].pointerToFormulaData), allFormulae[equationId].sizeInBlocks, newSizeInBlocks));
  }
  allFormulae[equationId].sizeInBlocks = newSizeInBlocks;
  xcopy(TO_PCMEMPTR(allFormulae[equationId].pointerToFormulaData), equationString, stringByteLength(equationString) + 1);
}

bool_t showEquation(uint16_t equationId, uint16_t startAt, uint16_t cursorAt) {
  if(equationId < numberOfFormulae) {
    char *bufPtr = tmpString;
    char *strPtr = (char *)TO_PCMEMPTR(allFormulae[equationId].pointerToFormulaData);
    uint16_t strLength = 0;
    int16_t strWidth = 0;
    int16_t glyphWidth = 0;
    uint32_t doubleBytednessHistory = 0;
    bool_t labelFound = false;
    bool_t cursorShown = false;

    if(startAt > 0) {
      *bufPtr       = STD_ELLIPSIS[0];
      *(bufPtr + 1) = STD_ELLIPSIS[1];
      *(bufPtr + 2) = 0;
      strWidth += stringWidth(bufPtr, &standardFont, true, true);
      bufPtr += 2;
    }
    else if(startAt > 0) {
      *bufPtr       = STD_CURSOR[0];
      *(bufPtr + 1) = STD_CURSOR[1];
      *(bufPtr + 2) = 0;
      strWidth += stringWidth(bufPtr, &standardFont, true, true);
      bufPtr += 2;
      cursorShown = true;
    }

    while((*strPtr) != 0) {
      if((++strLength) > startAt) {
        doubleBytednessHistory <<= 1;
        *bufPtr = *strPtr;
        if((*strPtr) == ':' && !labelFound && strLength <= 8) {
          *(bufPtr + 1) = STD_SPACE_4_PER_EM[0];
          *(bufPtr + 2) = STD_SPACE_4_PER_EM[1];
          *(bufPtr + 3) = 0;
          doubleBytednessHistory <<= 1;
          doubleBytednessHistory |= 1;
          bufPtr += 1;
        }
        else if((*strPtr) == '=' || (*strPtr) == '+' || (*strPtr) == '-' || (*strPtr) == '/') {
          *bufPtr       = STD_SPACE_4_PER_EM[0];
          *(bufPtr + 1) = STD_SPACE_4_PER_EM[1];
          *(bufPtr + 2) = *strPtr;
          *(bufPtr + 3) = 0;
          strWidth += stringWidth(bufPtr, &standardFont, true, true);
          *(bufPtr + 3) = STD_SPACE_4_PER_EM[0];
          *(bufPtr + 4) = STD_SPACE_4_PER_EM[1];
          *(bufPtr + 5) = 0;
          doubleBytednessHistory <<= 2;
          doubleBytednessHistory |= 5;
          bufPtr += 3;
        }
        else if(((*strPtr) == STD_CROSS[0] && (*(strPtr + 1)) == STD_CROSS[1]) || ((*strPtr) == STD_DOT[0] && (*(strPtr + 1)) == STD_DOT[1])) {
          *bufPtr       = STD_SPACE_4_PER_EM[0];
          *(bufPtr + 1) = STD_SPACE_4_PER_EM[1];
          *(bufPtr + 2) = *strPtr;
          *(bufPtr + 3) = *(strPtr + 1);
          *(bufPtr + 4) = 0;
          strWidth += stringWidth(bufPtr, &standardFont, true, true);
          *(bufPtr + 4) = STD_SPACE_4_PER_EM[0];
          *(bufPtr + 5) = STD_SPACE_4_PER_EM[1];
          *(bufPtr + 6) = 0;
          doubleBytednessHistory <<= 2;
          doubleBytednessHistory |= 7;
          bufPtr += 4;
        }
        else if((*strPtr) & 0x80) {
          *(bufPtr + 1) = *(strPtr + 1);
          *(bufPtr + 2) = 0;
          doubleBytednessHistory |= 1;
        }
        else {
          *(bufPtr + 1) = 0;
        }

        glyphWidth = stringWidth(bufPtr, &standardFont, true, true);
        strWidth += glyphWidth;
        if(strLength == cursorAt) {
          bufPtr += (doubleBytednessHistory & 0x00000001) ? 2 : 1;
          *bufPtr       = STD_CURSOR[0];
          *(bufPtr + 1) = STD_CURSOR[1];
          *(bufPtr + 2) = 0;
          glyphWidth = stringWidth(bufPtr, &standardFont, true, true);
          strWidth += glyphWidth;
          doubleBytednessHistory <<= 1;
          doubleBytednessHistory |= 1;
          cursorShown = true;
        }
        if(strWidth > (SCREEN_WIDTH - 2)) {
          *bufPtr = 0;
          strWidth -= glyphWidth;
          glyphWidth = stringWidth(STD_ELLIPSIS, &standardFont, true, true);
          while((strWidth + glyphWidth) > (SCREEN_WIDTH - 2)) {
            doubleBytednessHistory >>= 1;
            bufPtr -= (doubleBytednessHistory & 0x00000001) ? 2 : 1;
            if(*bufPtr == STD_CURSOR[0] && *(bufPtr + 1) == STD_CURSOR[1]) cursorShown = false;
            strWidth -= stringWidth(bufPtr, &standardFont, true, true);
            *bufPtr = 0;
          }
          *bufPtr       = STD_ELLIPSIS[0];
          *(bufPtr + 1) = STD_ELLIPSIS[1];
          *(bufPtr + 2) = 0;
          break;
        }
        bufPtr += (doubleBytednessHistory & 0x00000001) ? 2 : 1;
      }
      strPtr += ((*strPtr) & 0x80) ? 2 : 1;
    }

    showString(tmpString, &standardFont, 1, SCREEN_HEIGHT - SOFTMENU_HEIGHT * 3 + 2 , vmNormal, true, true);
    return cursorShown;
  }
  else return false;
}