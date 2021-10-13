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
#include "flags.h"
#include "fonts.h"
#include "gui.h"
#include "memory.h"
#include "screen.h"
#include "wp43s.h"



void fnEqEdit(uint16_t unusedButMandatoryParameter) {
  const char *equationString = TO_PCMEMPTR(allFormulae[currentFormula].pointerToFormulaData);
  xcopy(aimBuffer, equationString, stringByteLength(equationString) + 1);
  calcMode = CM_EIM;
  alphaCase = AC_UPPER;
  setSystemFlag(FLAG_ALPHA);
  yCursor = 0;
  xCursor = stringGlyphLength(equationString);
  #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
    calcModeAimGui();
  #endif // PC_BUILD && (SCREEN_800X480 == 0)
}

void fnEqDelete(uint16_t unusedButMandatoryParameter) {
  deleteEquation(currentFormula);
}

void fnEqCursorLeft(uint16_t unusedButMandatoryParameter) {
  if(xCursor > 0) --xCursor;
}

void fnEqCursorRight(uint16_t unusedButMandatoryParameter) {
  if(xCursor < (uint32_t)stringGlyphLength(aimBuffer)) ++xCursor;
}



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

void deleteEquation(uint16_t equationId) {
  if(equationId < numberOfFormulae) {
    if(allFormulae[equationId].sizeInBlocks > 0)
      wp43sFree(TO_PCMEMPTR(allFormulae[equationId].pointerToFormulaData), allFormulae[equationId].sizeInBlocks);
    for(uint16_t i = equationId + 1; i < numberOfFormulae; ++i)
      allFormulae[i - 1] = allFormulae[i];
    wp43sFree(allFormulae + (--numberOfFormulae), TO_BLOCKS(sizeof(registerHeader_t)));
    if(numberOfFormulae == 0)
      allFormulae = NULL;
    if(numberOfFormulae > 0 && currentFormula >= numberOfFormulae)
      currentFormula = numberOfFormulae - 1;
  }
}



bool_t showEquation(uint16_t equationId, uint16_t startAt, uint16_t cursorAt) {
  if(equationId < numberOfFormulae || equationId == EQUATION_AIM_BUFFER) {
    char *bufPtr = tmpString;
    const char *strPtr = equationId == EQUATION_AIM_BUFFER ? aimBuffer : (char *)TO_PCMEMPTR(allFormulae[equationId].pointerToFormulaData);
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
    if(startAt == cursorAt) {
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
          glyphWidth = stringWidth(STD_ELLIPSIS, &standardFont, true, true);
          while(1) {
            if(*bufPtr == STD_CURSOR[0] && *(bufPtr + 1) == STD_CURSOR[1]) cursorShown = false;
            strWidth -= stringWidth(bufPtr, &standardFont, true, true);
            *bufPtr = 0;
            if((strWidth + glyphWidth) <= (SCREEN_WIDTH - 2)) break;
            doubleBytednessHistory >>= 1;
            bufPtr -= (doubleBytednessHistory & 0x00000001) ? 2 : 1;
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

    if(cursorShown || cursorAt == EQUATION_NO_CURSOR)
      showString(tmpString, &standardFont, 1, SCREEN_HEIGHT - SOFTMENU_HEIGHT * 3 + 2 , vmNormal, true, true);
    return cursorShown;
  }
  else return false;
}