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

#include "ui/tam.h"

#include "bufferize.h"
#include "charString.h"
#include "constantPointers.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "gui.h"
#include "items.h"
#include "mathematics/fractionalPart.h"
#include "mathematics/integerPart.h"
#include "matrix.h"
#include "programming/lblGtoXeq.h"
#include "programming/manage.h"
#include "registers.h"
#include "softmenus.h"
#include <string.h>

#include "wp43s.h"

#ifndef TESTSUITE_BUILD
  int16_t tamOperation(void) {
    switch(tam.function) {
      case ITM_STO :
        switch(tam.currentOperation) {
          case ITM_ADD    : return ITM_STOADD;
          case ITM_SUB    : return ITM_STOSUB;
          case ITM_MULT   : return ITM_STOMULT;
          case ITM_DIV    : return ITM_STODIV;
          case ITM_Max    : return ITM_STOMAX;
          case ITM_Min    : return ITM_STOMIN;
          case ITM_Config : return ITM_STOCFG;
          case ITM_Stack  : return ITM_STOS;
          case ITM_dddEL  : return ITM_STOEL;
          case ITM_dddIJ  : return ITM_STOIJ;
          default :         return ITM_STO;
        }

      case ITM_RCL :
        switch(tam.currentOperation) {
          case ITM_ADD    : return ITM_RCLADD;
          case ITM_SUB    : return ITM_RCLSUB;
          case ITM_MULT   : return ITM_RCLMULT;
          case ITM_DIV    : return ITM_RCLDIV;
          case ITM_Max    : return ITM_RCLMAX;
          case ITM_Min    : return ITM_RCLMIN;
          case ITM_Config : return ITM_RCLCFG;
          case ITM_Stack  : return ITM_RCLS;
          case ITM_dddEL  : return ITM_RCLEL;
          case ITM_dddIJ  : return ITM_RCLIJ;
          default :         return ITM_RCL;
        }

      default :             return tam.function;
    }
  }



  static uint8_t _tamMaxDigits(int16_t max) {
    if(tam.function == ITM_GTOP) {
      return (max < 1000 ? 3 : (max < 10000 ? 4 : 5));
    }
    else {
      return (max < 10 ? 1 : (max < 100 ? 2 : (max < 1000 ? 3 : (max < 10000 ? 4 : 5))));
    }
  }


  static void _tamUpdateBuffer() {
    char regists[5];
    char *tbPtr = tamBuffer;
    if(tam.mode == 0) {
      return;
    }

    if(tam.mode == TM_KEY) {
      tbPtr = stpcpy(tbPtr, "KEY ");
      if(tam.keyInputFinished) {
        if(tam.keyIndirect) {
          tbPtr = stpcpy(tbPtr, STD_RIGHT_ARROW);
        }
        if(tam.keyDot) {
          tbPtr = stpcpy(tbPtr, ".");
        }
        if(tam.keyAlpha) {
          tbPtr = stpcpy(tbPtr, STD_LEFT_SINGLE_QUOTE);
          tbPtr = stpcpy(tbPtr, aimBuffer + AIM_BUFFER_LENGTH / 2);
          tbPtr = stpcpy(tbPtr, STD_RIGHT_SINGLE_QUOTE);
        }
        else {
          int16_t v = tam.key;
          for(int i = 1; i >= 0; i--) {
            tbPtr[i] = '0' + (v % 10);
            v /= 10;
          }
          tbPtr += 2;
        }
        if(tam.function == ITM_KEYX) {
          tbPtr = stpcpy(tbPtr, " XEQ ");
        }
        else {
          tbPtr = stpcpy(tbPtr, " GTO ");
        }
      }
    }
    else {
      tbPtr = stpcpy(tbPtr, indexOfItems[tamOperation()].itemCatalogName);
      tbPtr = stpcpy(tbPtr, " ");
    }

    if(tam.mode == TM_SHUFFLE) {
      // Shuffle keeps the source register number for each destination register (X, Y, Z, T) in two bits
      // consecutively, with the 'valid' bit eight above that number
      // E.g. 0000010100001110 would mean that two registers have been entered: T, Z in that order
      regists[4] = 0;
      for(int i=0;i<4;i++) {
        if((tam.value >> (i*2 + 8)) & 1) {
          uint8_t regNum = (tam.value >> (i*2)) & 3;
          regists[i] = (regNum == 3 ? 't' : 'x' + regNum);
        }
        else {
          regists[i] = '_';
        }
      }
      tbPtr = stpcpy(tbPtr, regists);
    }
    else {
      if(tam.indirect) {
        tbPtr = stpcpy(tbPtr, STD_RIGHT_ARROW);
      }
      if(tam.dot) {
        tbPtr = stpcpy(tbPtr, ".");
      }
      if(tam.alpha) {
        tbPtr = stpcpy(tbPtr, STD_LEFT_SINGLE_QUOTE);
        if(aimBuffer[0] == 0) {
          tbPtr = stpcpy(tbPtr, "_");
        }
        else {
          tbPtr = stpcpy(tbPtr, aimBuffer);
          tbPtr = stpcpy(tbPtr, STD_RIGHT_SINGLE_QUOTE);
        }
      }
      else {
        int16_t max = (tam.indirect ? (tam.dot ? currentNumberOfLocalRegisters : 99)
          : (tam.dot ? ((tam.mode == TM_FLAGR || tam.mode == TM_FLAGW) ? NUMBER_OF_LOCAL_FLAGS : currentNumberOfLocalRegisters) : tam.max));
        uint8_t maxDigits = _tamMaxDigits(max);
        uint8_t underscores = maxDigits - tam.digitsSoFar;
        int16_t v = tam.value;
        for(int i = tam.digitsSoFar - 1; i >= 0; i--) {
          tbPtr[i] = '0' + (v % 10);
          v /= 10;
        }
        tbPtr += tam.digitsSoFar;
        for(int i = 0; i < underscores; i++) {
          tbPtr[0] = '_';
          tbPtr++;
        }
      }
    }

    if(tam.mode == TM_KEY && !tam.keyInputFinished) {
      if(tam.function == ITM_KEYX) {
        tbPtr = stpcpy(tbPtr, " XEQ __");
      }
      else {
        tbPtr = stpcpy(tbPtr, " GTO __");
      }
    }
    tbPtr[0] = 0;
  }



  static void _tamHandleShuffle(uint16_t item) {
    // Shuffle keeps the source register number for each destination register (X, Y, Z, T) in two bits
    // consecutively, with the 'valid' bit eight above that number
    // E.g. 0000010100001110 would mean that two registers have been entered: T, Z in that order
    switch(item) {
      case ITM_REG_X:
      case ITM_REG_Y:
      case ITM_REG_Z:
      case ITM_REG_T:
        for(int i=0; i<4; i++) {
          if(!((tam.value >> (2*i + 8)) & 1)) {
            uint16_t mask = 3 << (2*i);
            tam.value |= 1 << (2*i + 8);
            tam.value = (tam.value & ~mask) | (((item-ITM_REG_X) << (2*i)) & mask);
            if(i == 3) {
              reallyRunFunction(tamOperation(), tam.value);
              tamLeaveMode();
            }
            break;
          }
        }
        break;
      case ITM_BACKSPACE:
        // We won't have all four registers at this point as otherwise TAM would already be closed
        for(int i=3; i>=0; i--) {
          if((tam.value >> (2*i + 8)) & 1) {
            tam.value &= ~(1 << (2*i + 8));
            break;
          }
          else if(i == 0) {
            tamLeaveMode();
            break;
          }
        }
        break;
    }
  }



  static void _tamProcessInput(uint16_t item) {
    int16_t min, max, min2, max2;
    bool_t forceTry = false, tryOoR = false;
    bool_t valueParameter = (tam.function == ITM_GTOP || tam.function == ITM_BESTF || tam.function == ITM_CNST);
    char *forcedVar = NULL;

    // Shuffle is handled completely differently to everything else
    if(tam.mode == TM_SHUFFLE) {
      _tamHandleShuffle(item);
      return;
    }

    min = (tam.dot ? 0 : tam.min);
    max = (tam.dot ? ((tam.mode == TM_FLAGR || tam.mode == TM_FLAGW) ? NUMBER_OF_LOCAL_FLAGS : currentNumberOfLocalRegisters) : tam.max);
    min2 = (tam.indirect ? 0 : min);
    max2 = (tam.indirect ? (tam.dot ? currentNumberOfLocalRegisters : 99) : max);
    if(item == ITM_ENTER || (tam.alpha && stringGlyphLength(aimBuffer) > 6)) {
      forceTry = true;
    }
    else if(item == ITM_BACKSPACE) {
      if(tam.alpha) {
        if(stringByteLength(aimBuffer) != 0) {
          // Delete the last character
          int16_t lg = stringLastGlyph(aimBuffer);
          aimBuffer[lg] = 0;
        }
        else if(tam.mode == TM_NEWMENU) {
          tamLeaveMode();
          runFunction(ITM_ASSIGN);
        }
        else if(tam.mode == TM_KEY && tam.keyInputFinished) {
          tam.value            = tam.key / 10;
          tam.alpha            = tam.keyAlpha;
          tam.dot              = tam.keyDot;
          tam.indirect         = tam.keyIndirect;
          tam.keyInputFinished = false;
          xcopy(aimBuffer, aimBuffer + AIM_BUFFER_LENGTH / 2, 16);
          aimBuffer[0]    = 0;
          tam.key         = 0;
          tam.keyAlpha    = false;
          tam.keyDot      = false;
          tam.keyIndirect = false;
          tam.max         = 21;
          tam.min         = 1;
          tam.digitsSoFar = 1;
          popSoftmenu();
          showSoftmenu(-MNU_TAM);
          --numberOfTamMenusToPop;
          if(!tam.alpha) {
            clearSystemFlag(FLAG_ALPHA);
            #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
              calcModeTamGui();
            #endif // PC_BUILD && (SCREEN_800X480 == 0)
          }
        }
        else {
          // backspaces within AIM are handled by addItemToBuffer, so this is if the aimBuffer is already empty
          tam.alpha = false;
          clearSystemFlag(FLAG_ALPHA);
          #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
            calcModeTamGui();
          #endif // PC_BUILD && (SCREEN_800X480 == 0)
        }
      }
      else if(tam.digitsSoFar > 0) {
        if(--tam.digitsSoFar != 0) {
          tam.value /= 10;
        }
        else {
          tam.value = 0;
        }
      }
      else if(tam.function == ITM_GTOP) {
        tam.function = ITM_GTO;
        tam.min = indexOfItems[ITM_GTO].tamMinMax >> TAM_MAX_BITS;
        tam.max = indexOfItems[ITM_GTO].tamMinMax & TAM_MAX_MASK;
      }
      else if(tam.dot) {
        tam.dot = false;
      }
      else if(tam.indirect) {
        tam.indirect = false;
        if(tam.mode == TM_FLAGR || tam.mode == TM_FLAGW) {
          popSoftmenu();
          showSoftmenu(-MNU_TAMFLAG);
          --numberOfTamMenusToPop;
        }
        else if(tam.mode == TM_LABEL || (tam.mode == TM_KEY && tam.keyInputFinished)) {
          popSoftmenu();
          showSoftmenu(-MNU_TAMLABEL);
          --numberOfTamMenusToPop;
        }
      }
      else if(tam.currentOperation != tam.function) {
        tam.currentOperation = tam.function;
      }
      else if(tam.mode == TM_KEY && tam.keyInputFinished) {
        tam.value            = tam.key / 10;
        tam.alpha            = tam.keyAlpha;
        tam.dot              = tam.keyDot;
        tam.indirect         = tam.keyIndirect;
        tam.keyInputFinished = false;
        xcopy(aimBuffer, aimBuffer + AIM_BUFFER_LENGTH / 2, 16);
        aimBuffer[0]    = 0;
        tam.key         = 0;
        tam.keyAlpha    = false;
        tam.keyDot      = false;
        tam.keyIndirect = false;
        tam.max         = 21;
        tam.min         = 1;
        tam.digitsSoFar = 1;
        popSoftmenu();
        showSoftmenu(-MNU_TAM);
        --numberOfTamMenusToPop;
        if(tam.alpha) {
          setSystemFlag(FLAG_ALPHA);
          calcModeAim(NOPARAM);
        }
        #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
          calcModeTamGui();
        #endif // PC_BUILD && (SCREEN_800X480 == 0)
      }
      else {
        tamLeaveMode();
      }
      return;
    }
    else if(item == MNU_DYNAMIC) {
      forcedVar = dynmenuGetLabel(dynamicMenuItem);
      if(forcedVar[0] == 0) {
        forcedVar = NULL;
      }
      forceTry = true;
    }
    else if(tam.alpha) {
      // Do nothing if it wasn't enter or backspace as the text input is handled elsewhere
      return;
    }
    else if(item == ITM_alpha) {
      bool_t allowAlphaMode = false;
      allowAlphaMode = allowAlphaMode || (!tam.digitsSoFar && !tam.dot && !valueParameter && (tam.mode == TM_STORCL || tam.mode == TM_M_DIM || tam.mode == TM_REGISTER || tam.mode == TM_CMP || tam.function == ITM_MVAR));
      allowAlphaMode = allowAlphaMode || (!tam.digitsSoFar && !tam.dot && tam.indirect);
      allowAlphaMode = allowAlphaMode || (!tam.digitsSoFar && !tam.dot && tam.mode == TM_LABEL);
      allowAlphaMode = allowAlphaMode || (!tam.digitsSoFar && !tam.dot && tam.keyInputFinished && tam.mode == TM_KEY);
      allowAlphaMode = allowAlphaMode || (!tam.digitsSoFar && (tam.function == ITM_LBL || tam.function == ITM_GTOP));
      if(allowAlphaMode) {
        tam.alpha = true;
        setSystemFlag(FLAG_ALPHA);
        aimBuffer[0] = 0;
        calcModeAim(NOPARAM);
      }
      return;
    }
    else if(item==ITM_Max || item==ITM_Min || item==ITM_ADD || item==ITM_SUB || item==ITM_MULT || item==ITM_DIV || item==ITM_Config || item==ITM_Stack || item==ITM_dddEL || item==ITM_dddIJ) { // Operation
      if(!tam.digitsSoFar && !tam.indirect) {
        if(tam.function == ITM_GTOP) {
          if(item == ITM_Max) { // UP
            if(currentLocalStepNumber == 1) { // We are on 1st step of current program
              if(currentProgramNumber == 1) { // It's the 1st program in memory
                return;
              }
              else { // It isn't the 1st program in memory
                tam.value = programList[currentProgramNumber - 2].step;
              }
            }
            else { // We aren't on 1st step of current program
              tam.value = programList[currentProgramNumber - 1].step;
            }
            reallyRunFunction(ITM_GTOP, tam.value);
            tamLeaveMode();
            hourGlassIconEnabled = false;
            return;
          }

          if(item == ITM_Min) { // DOWN
            if(currentProgramNumber == numberOfPrograms - 1) { // We are in the last program in memory
              return;
            }

            tam.value = programList[currentProgramNumber].step;
            reallyRunFunction(ITM_GTOP, tam.value);
            tamLeaveMode();
            hourGlassIconEnabled = false;
            return;
          }
        }
        else if(tam.mode == TM_STORCL && tam.currentOperation != ITM_Config && tam.currentOperation != ITM_Stack) {
          if(item == tam.currentOperation) {
            tam.currentOperation = tam.function;
          } else {
            tam.currentOperation = item;
            if(item == ITM_dddEL || item == ITM_dddIJ) {
              reallyRunFunction(tamOperation(), NOPARAM);
              if(tam.mode) tamLeaveMode();
              hourGlassIconEnabled = false;
              return;
            }
          }
        }
      }
      return;
    }
    else if(tam.function == ITM_toINT && item == ITM_REG_I) {
      if(calcMode == CM_PEM) {
        addStepInProgram(ITM_IP);
      }
      else {
        fnIp(NOPARAM);
      }
      tamLeaveMode();
      return;
    }
    else if(tam.function == ITM_toINT && item == ITM_alpha) {
      if(calcMode == CM_PEM) {
        addStepInProgram(ITM_FP);
      }
      else {
        fnFp(NOPARAM);
      }
      tamLeaveMode();
      return;
    }
    else if(tam.function == ITM_toINT && (item == ITM_REG_D || item == ITM_ENTER)) {   //JM
      tam.value = 10;
      forceTry = true;
    }
    else if(tam.function == ITM_toINT && item == ITM_REG_B) {
      tam.value = 2;
      forceTry = true;
    }
    else if(tam.function == ITM_toINT && item == ITM_HEX) {
      tam.value = 16;
      forceTry = true;
    }
    else if(tam.mode == TM_LABEL && !tam.indirect && item == ITM_E) {
      tam.value = 100 - 'A' + 'E';
      forceTry = true;
      tryOoR = true;
    }
    else if(REGISTER_X <= indexOfItems[item].param && indexOfItems[item].param <= REGISTER_K) {
      if(!tam.digitsSoFar && tam.function != ITM_BESTF && tam.function != ITM_CNST && (tam.indirect || (tam.mode != TM_VALUE && tam.mode != TM_VALUE_CHB))) {
        if(tam.mode == TM_LABEL && !tam.indirect) {
          switch(indexOfItems[item].param) {
            case REGISTER_A: tam.value = 100 - 'A' + 'A'; forceTry = true; tryOoR = true; break;
            case REGISTER_B: tam.value = 100 - 'A' + 'B'; forceTry = true; tryOoR = true; break;
            case REGISTER_C: tam.value = 100 - 'A' + 'C'; forceTry = true; tryOoR = true; break;
            case REGISTER_D: tam.value = 100 - 'A' + 'D'; forceTry = true; tryOoR = true; break;
            case REGISTER_X: tam.alpha = true; aimBuffer[0] = 'X'; aimBuffer[1] = 0; forceTry = true; break;
            case REGISTER_Y: tam.alpha = true; aimBuffer[0] = 'Y'; aimBuffer[1] = 0; forceTry = true; break;
            case REGISTER_Z: tam.alpha = true; aimBuffer[0] = 'Z'; aimBuffer[1] = 0; forceTry = true; break;
            case REGISTER_T: tam.alpha = true; aimBuffer[0] = 'T'; aimBuffer[1] = 0; forceTry = true; break;
            case REGISTER_L: tam.alpha = true; aimBuffer[0] = 'L'; aimBuffer[1] = 0; forceTry = true; break;
            case REGISTER_I: tam.alpha = true; aimBuffer[0] = 'I'; aimBuffer[1] = 0; forceTry = true; break;
            case REGISTER_J: tam.alpha = true; aimBuffer[0] = 'J'; aimBuffer[1] = 0; forceTry = true; break;
            case REGISTER_K: tam.alpha = true; aimBuffer[0] = 'K'; aimBuffer[1] = 0; forceTry = true; break;
          }
        }
        else {
          tam.value = indexOfItems[item].param;
          forceTry = true;
          // Register letters access registers not accessible via number codes, so we shouldn't look at the tam.max value
          // when determining if this is valid
          tryOoR = true;
        }
      }
    }
    else if(item == ITM_0P || item == ITM_1P) {
      reallocateRegister(TEMP_REGISTER_1, dtReal34, REAL34_SIZE, amNone);
      real34Copy(item == ITM_1P ? const34_1 : const34_0, REGISTER_REAL34_DATA(TEMP_REGISTER_1));
      if(!tam.digitsSoFar && tam.function != ITM_BESTF && tam.function != ITM_CNST && tam.mode != TM_VALUE && tam.mode != TM_VALUE_CHB) {
        tam.value = TEMP_REGISTER_1;
        forceTry = true;
        // Register letters access registers not accessible via number codes, so we shouldn't look at the tam.max value
        // when determining if this is valid
        tryOoR = true;
      }
    }
    else if(ITM_0 <= item && item <= ITM_9) {
      int16_t digit = item - ITM_0;
      uint8_t maxDigits = _tamMaxDigits(max2);
      // If the number is below our minimum, prevent further entry of digits
      if(!tam.alpha && (tam.value*10 + digit) <= max2 && tam.digitsSoFar < maxDigits) {
        tam.value = tam.value*10 + digit;
        tam.digitsSoFar++;
        if(tam.digitsSoFar == maxDigits) {
          forceTry = true;
        }
      }
    }
    else if(item == ITM_PERIOD) {
      if(tam.function == ITM_GTOP) {
        tam.value = programList[numberOfPrograms - 1].step;
        reallyRunFunction(ITM_GTOP, tam.value);
        if((*currentStep != 0xff) || (*(currentStep + 1) != 0xff)) {
          currentStep = firstFreeProgramByte;
          insertStepInProgram(ITM_END);
          tam.value = programList[numberOfPrograms - 1].step;
          reallyRunFunction(ITM_GTOP, tam.value);
        }
        tamLeaveMode();
        hourGlassIconEnabled = false;
        return;
      }
      else if(!tam.alpha && !tam.digitsSoFar && !tam.dot && !valueParameter) {
        if(tam.function == ITM_GTO) {
          tam.function = ITM_GTOP;
          tam.min = 1;
          tam.max = getNumberOfSteps();
        }
        else if(tam.indirect && currentNumberOfLocalRegisters) {
          tam.dot = true;
        }
        else if(tam.mode != TM_VALUE && tam.mode != TM_VALUE_CHB) {
          if(((tam.mode == TM_FLAGR || tam.mode == TM_FLAGW) && currentLocalFlags != NULL) || ((tam.mode != TM_FLAGR && tam.mode != TM_FLAGW) && currentNumberOfLocalRegisters)) {
            tam.dot = true;
          }
        }
      }
      return;
    }
    else if(item == ITM_INDIRECTION) {
      if(!tam.alpha && !tam.digitsSoFar && !tam.dot && !valueParameter) {
        if(!tam.indirect && (tam.mode == TM_FLAGR || tam.mode == TM_FLAGW || tam.mode == TM_LABEL)) {
          popSoftmenu();
          showSoftmenu(-MNU_TAM);
          --numberOfTamMenusToPop;
        }
        tam.indirect = true;
      }
      return;
    }
    else if(indexOfItems[item].func == fnGetSystemFlag && (tam.mode == TM_FLAGR || tam.mode == TM_FLAGW)) {
      // A function key has been pressed that corresponds to a system flag
      tam.value = indexOfItems[item].param;
      tryOoR = true;
      forceTry = true;
    }
    else {
      // Do nothing
      return;
    }

    // All operations that may try and evaluate the function shouldn't return but let execution fall through to here

    if(tam.mode == TM_KEY && !tam.keyInputFinished) {
      if(tam.alpha || forcedVar || ((tryOoR || (min2 <= tam.value && tam.value <= max2)) && (forceTry || tam.value*10 > max2))) {
        tam.key              = tam.value;
        tam.keyAlpha         = tam.alpha;
        tam.keyDot           = tam.dot;
        tam.keyIndirect      = tam.indirect;
        tam.keyInputFinished = true;
        xcopy(aimBuffer + AIM_BUFFER_LENGTH / 2, aimBuffer, 16);
        aimBuffer[0]    = 0;
        tam.value       = 0;
        tam.alpha       = false;
        tam.dot         = false;
        tam.indirect    = false;
        tam.max         = 99;
        tam.min         = 0;
        tam.digitsSoFar = 0;
        popSoftmenu();
        showSoftmenu(-MNU_TAMLABEL);
        --numberOfTamMenusToPop;
        clearSystemFlag(FLAG_ALPHA);
        #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
          calcModeTamGui();
        #endif // PC_BUILD && (SCREEN_800X480 == 0)
      }
      else if(tam.digitsSoFar == 2 && tam.value == 0) {
        tam.digitsSoFar = 1;
      }
    }
    else if(!tam.alpha && !forcedVar) {
      // Check whether it is possible to add any more digits: if not, execute the function
      if((tryOoR || (min2 <= tam.value && tam.value <= max2)) && (forceTry || tam.value*10 > max2)) {
        int16_t value = tam.value;
        bool_t run = true;
        if(tam.dot) {
          value += FIRST_LOCAL_REGISTER;
        }
        if(tam.indirect && calcMode != CM_PEM) {
          value = indirectAddressing(value, (tam.mode == TM_STORCL || tam.mode == TM_M_DIM), min, max);
          run = (lastErrorCode == 0);
        }
        if(tam.function == ITM_GTOP) {
          if(forceTry && tam.digitsSoFar < 3) {
            fnGoto(value);
          }
          else {
            reallyRunFunction(tamOperation(), value + programList[currentProgramNumber - 1].step - 1);
          }
        }
        else if(run) {
          switch(calcMode) {
            case CM_MIM:
              mimRunFunction(tamOperation(), value);
              break;
            case CM_PEM:
              addStepInProgram(tamOperation());
              break;
            default:
              reallyRunFunction(tamOperation(), value);
          }
        }
        if(tamOperation() == ITM_M_GOTO_ROW) {
          tamLeaveMode();
          tamEnterMode(ITM_M_GOTO_COLUMN);
        }
        else {
          if(tam.mode) tamLeaveMode();
        }
      }
    }
    else {
      char *buffer = (forcedVar ? forcedVar : aimBuffer);
      bool_t tryAllocate = ((tam.function == ITM_STO || tam.function == ITM_M_DIM || tam.function == ITM_MVAR || tam.function == ITM_INPUT) && !tam.indirect);
      int16_t value;
      if(tam.mode == TM_NEWMENU) {
        value = 1;
      }
      else if(tam.mode == TM_LABEL || tam.mode == TM_SOLVE || (tam.mode == TM_KEY && tam.keyInputFinished)) {
        value = findNamedLabel(buffer);
        if(value == INVALID_VARIABLE && tam.function != ITM_LBL && tam.function != ITM_LBLQ) {
          displayCalcErrorMessage(ERROR_LABEL_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "string '%s' is not a named label", buffer);
            moreInfoOnError("In function _tamProcessInput:", errorMessage, NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        }
      }
      else if(tryAllocate) {
        value = findOrAllocateNamedVariable(buffer);
      }
      else {
        value = findNamedVariable(buffer);
        if(value == INVALID_VARIABLE) {
          displayCalcErrorMessage(ERROR_UNDEF_SOURCE_VAR, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "string '%s' is not a named variable", buffer);
            moreInfoOnError("In function _tamProcessInput:", errorMessage, NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        }
      }
      if(calcMode == CM_PEM) {
        addStepInProgram(tamOperation());
      }
      if(tam.mode != TM_NEWMENU) aimBuffer[0] = 0;
      if(tam.indirect && value != INVALID_VARIABLE && calcMode != CM_PEM) {
        value = indirectAddressing(value, (tam.mode == TM_STORCL || tam.mode == TM_M_DIM), min, max);
        if(lastErrorCode != 0) {
          value = INVALID_VARIABLE;
        }
      }
      if(value != INVALID_VARIABLE || tamOperation() == ITM_LBLQ) {
        if(calcMode == CM_MIM) {
          mimRunFunction(tamOperation(), value);
        }
        else if(tam.function == ITM_GTOP) {
          reallyRunFunction(ITM_GTOP, labelList[value - FIRST_LABEL].step);
        }
        else if(calcMode == CM_PEM) {
          // already done
        }
        else {
          reallyRunFunction(tamOperation(), value);
        }
      }
      if(tamOperation() == ITM_M_GOTO_ROW) {
        tamLeaveMode();
        tamEnterMode(ITM_M_GOTO_COLUMN);
      }
      else {
        if(tam.mode) tamLeaveMode();
      }
    }
  }



  void tamEnterMode(int16_t func) {
    tam.mode = func == ITM_ASSIGN ? TM_NEWMENU : indexOfItems[func].param;
    tam.function = func;
    tam.min = indexOfItems[func].tamMinMax >> TAM_MAX_BITS;
    tam.max = indexOfItems[func].tamMinMax & TAM_MAX_MASK;

    if(tam.max == 16383) { // Only case featuring more than TAM_MAX_BITS bits is GTO.
      tam.max = 32766;
    }

    if(func == ITM_CNST) {
      tam.max = NUMBER_OF_CONSTANTS_39 + NUMBER_OF_CONSTANTS_51 + NUMBER_OF_CONSTANTS_1071 + NUMBER_OF_CONSTANTS_34 - 1;
    }

    if(calcMode == CM_NIM) {
      closeNim();
    }
    else if(calcMode == CM_PEM && aimBuffer[0] != 0) {
      if(getSystemFlag(FLAG_ALPHA)) pemCloseAlphaInput();
      else                          pemCloseNumberInput();
      aimBuffer[0] = 0;
    }

    tam.alpha = (func == ITM_ASSIGN);
    tam.currentOperation = tam.function;
    tam.digitsSoFar = 0;
    tam.dot = false;
    tam.indirect = false;
    tam.value = 0;

    tam.key = 0;
    tam.keyAlpha = false;
    tam.keyDot = false;
    tam.keyIndirect = false;
    tam.keyInputFinished = false;

    switch(tam.mode) {
      case TM_VALUE:
      case TM_VALUE_CHB:
      case TM_REGISTER:
      case TM_M_DIM:
      case TM_KEY:
        showSoftmenu(-MNU_TAM);
        break;

      case TM_CMP:
        showSoftmenu(-MNU_TAMCMP);
        break;

      case TM_FLAGR:
      case TM_FLAGW:
        showSoftmenu(-MNU_TAMFLAG);
        break;

      case TM_STORCL:
        showSoftmenu(-MNU_TAMSTORCL);
        break;

      case TM_SHUFFLE:
        showSoftmenu(-MNU_TAMSHUFFLE);
        break;

      case TM_LABEL:
        showSoftmenu(-MNU_TAMLABEL);
        break;

      case TM_SOLVE:
        if(func == ITM_SOLVE && calcMode == CM_PEM)
          showSoftmenu(-MNU_TAM);
        else
          showSoftmenu(-MNU_TAMLABEL);
        break;

      case TM_NEWMENU:
        break;

      default:
        sprintf(errorMessage, "In function calcModeTam: %" PRIu16 " is an unexpected value for tam.mode!", tam.mode);
        displayBugScreen(errorMessage);
        return;
    }

    numberOfTamMenusToPop = func == ITM_ASSIGN ? 0 : 1;

    _tamUpdateBuffer();

    clearSystemFlag(FLAG_ALPHA);

    #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
      if(tam.mode == TM_NEWMENU) {
        setSystemFlag(FLAG_ALPHA);
        aimBuffer[0] = 0;
        calcModeAim(NOPARAM);
      }
      else {
        calcModeTamGui();
      }
    #endif // PC_BUILD && (SCREEN_800X480 == 0)
  }



  void tamLeaveMode(void) {
    tam.alpha = false;
    tam.mode = 0;
    catalog = CATALOG_NONE;
    clearSystemFlag(FLAG_ALPHA);

    while(numberOfTamMenusToPop--) {
      popSoftmenu();
    }

    #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
      if(calcMode == CM_NORMAL || calcMode == CM_PEM || calcMode == CM_MIM || calcMode == CM_TIMER) {
        calcModeNormalGui();
      }
      else if(calcMode == CM_AIM) {
        calcModeAimGui();
      }
    #endif // PC_BUILD && (SCREEN_800X480 == 0)

    if(calcMode == CM_PEM) {
      hourGlassIconEnabled = false;
    }
  }



  void tamProcessInput(uint16_t item) {
    _tamProcessInput(item);
    _tamUpdateBuffer();
  }
#endif // TESTSUITE_BUILD
