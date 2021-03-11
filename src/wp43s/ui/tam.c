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
 * \file tam.c
 ***********************************************/

#include "wp43s.h"

#ifndef TESTSUITE_BUILD
  // TAM transition system
  #define TT_OPERATION                               0 // +, -, *, /, min, max
  #define TT_LETTER                                  1
  #define TT_VARIABLE                                2
  #define TT_DIGIT                                   3
  #define TT_ENTER                                   4
  #define TT_DOT                                     5 // For local flags and registers
  #define TT_INDIRECT                                6 // For indirect addressing
  #define TT_BACKSPACE                               7
  #define TT_BASE02                                  8
  #define TT_BASE10                                  9
  #define TT_BASE16                                 10
  #define TT_INT                                    11
  #define TT_FP                                     12
  #define TT_NOTHING                                13

  static int16_t getOperation(void) {
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


  static void updateTamBuffer() {
    char regists[5];
    char *tbPtr = tamBuffer;
    if(tam.mode == 0) {
      return;
    }
    tbPtr = stpcpy(tbPtr, indexOfItems[getOperation()].itemCatalogName);
    tbPtr = stpcpy(tbPtr, " ");
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
        int16_t max = (tam.dot ? ((tam.mode == TM_FLAGR || tam.mode == TM_FLAGW) ? NUMBER_OF_LOCAL_FLAGS : currentNumberOfLocalRegisters) : tam.max);
        uint8_t maxDigits = (max < 10 ? 1 : (max < 100 ? 2 : (max < 1000 ? 3 : (max < 10000 ? 4 : 5))));
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
    tbPtr[0] = 0;
  }

  static void reallyTamTransitionSystem(uint16_t tamEvent, int16_t operation, int16_t digit, int16_t letteredRegister) {
    int16_t min, max;
    bool_t forceTry = false;
    bool_t valueParameter = (tam.function == ITM_GTOP || tam.function == ITM_BESTF || tam.function == ITM_CNST);

    // Shuffle is handled completely differently to everything else
    if(tam.mode == TM_SHUFFLE) {
      // Shuffle keeps the source register number for each destination register (X, Y, Z, T) in two bits
      // consecutively, with the 'valid' bit eight above that number
      // E.g. 0000010100001110 would mean that two registers have been entered: T, Z in that order
      if(tamEvent == TT_LETTER) {
        for(int i=0; i<4; i++) {
          if(!((tam.value >> (2*i + 8)) & 1)) {
            uint16_t mask = 3 << (2*i);
            tam.value |= 1 << (2*i + 8);
            tam.value = (tam.value & ~mask) | (((letteredRegister-REGISTER_X) << (2*i)) & mask);
            if(i == 3) {
              reallyRunFunction(getOperation(), tam.value);
              tamLeaveMode();
            }
            break;
          }
        }
      }
      else if(tamEvent == TT_BACKSPACE) {
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
      }
      return;
    }

    min = (tam.dot ? 0 : tam.min);
    max = (tam.dot ? ((tam.mode == TM_FLAGR || tam.mode == TM_FLAGW) ? NUMBER_OF_LOCAL_FLAGS : currentNumberOfLocalRegisters) : tam.max);
    switch(tamEvent) {
      case TT_OPERATION:
        if(!tam.alpha && !tam.digitsSoFar && !tam.indirect) {
          if(tam.function == ITM_GTOP) {
            if(operation == ITM_Max) { // UP
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
              return;
            }

            if(operation == ITM_Min) { // DOWN
              if(currentProgramNumber == numberOfPrograms - 1) { // We are in the last program in memory
                return;
              }

              tam.value = programList[currentProgramNumber].step;
              reallyRunFunction(ITM_GTOP, tam.value);
              tamLeaveMode();
              return;
            }
          }
          else if(tam.mode == TM_STORCL && tam.currentOperation != ITM_Config && tam.currentOperation != ITM_Stack) {
            if(operation == tam.currentOperation) {
              tam.currentOperation = tam.function;
            } else {
              tam.currentOperation = operation;
              if(operation == ITM_dddEL || operation == ITM_dddIJ) {
                reallyRunFunction(getOperation(), NOPARAM);
                tamLeaveMode();
                return;
              }
            }
          }
        }
        return;

      case TT_LETTER:
        if(!tam.digitsSoFar && !tam.alpha && tam.function != ITM_BESTF && tam.function != ITM_CNST && tam.mode != TM_VALUE && tam.mode != TM_VALUE_CHB) {
          tam.value = letteredRegister;
          forceTry = true;
        }
        break;

      case TT_VARIABLE:
        if(!tam.digitsSoFar && !tam.alpha && !tam.dot && !valueParameter && tam.mode != TM_VALUE && tam.mode != TM_VALUE_CHB) {
          tam.alpha = true;
          setSystemFlag(FLAG_ALPHA);
          aimBuffer[0] = 0;
          calcModeAim(NOPARAM);
        }
        return;

      case TT_DIGIT:
        if(!tam.alpha && (tam.value*10 + digit) <= max) {
          tam.value = tam.value*10 + digit;
          tam.digitsSoFar++;
        }
        break;

      case TT_INT :
        fnIp(NOPARAM);
        tamLeaveMode();
        return;

      case TT_FP :
        fnFp(NOPARAM);
        tamLeaveMode();
        return;

      case TT_BASE02:
        tam.value = 2;
        forceTry = true;
        break;

      case TT_BASE10:
        tam.value = 10;
        forceTry = true;
        break;

      case TT_BASE16:
        tam.value = 16;
        forceTry = true;
        break;

      case TT_DOT:
        if(tam.function == ITM_GTOP) {
          tam.value = tam.max;
        }
        else if(!tam.alpha && !tam.digitsSoFar && !tam.dot && !valueParameter) {
          if(tam.function == ITM_GTO) {
            tam.function = ITM_GTOP;
            tam.min = 1;
            tam.max = programList[currentProgramNumber].step - programList[currentProgramNumber - 1].step;
          }
          else if(tam.indirect && currentLocalRegisters != NULL) {
            tam.dot = true;
          }
          else if(tam.mode != TM_VALUE && tam.mode != TM_VALUE_CHB) {
            if(((tam.mode == TM_FLAGR || tam.mode == TM_FLAGW) && currentLocalFlags != NULL) || ((tam.mode != TM_FLAGR && tam.mode != TM_FLAGW) && currentLocalRegisters != NULL)) {
              tam.dot = true;
            }
          }
        }
        return;

      case TT_INDIRECT:
        if(!tam.alpha && !tam.digitsSoFar && !tam.dot && !valueParameter) {
          tam.indirect = true;
        }
        return;

      case TT_ENTER:
        forceTry = true;
        break;

      case TT_BACKSPACE:
        if(tam.alpha) {
          // backspaces within AIM are handled by addItemToBuffer, so this is if the aimBuffer is already empty
          tam.alpha = false;
          clearSystemFlag(FLAG_ALPHA);
          #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
            calcModeTamGui();
          #endif // PC_BUILD && (SCREEN_800X480 == 0)
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
        }
        else if(tam.currentOperation != tam.function) {
          tam.currentOperation = tam.function;
        }
        else {
          tamLeaveMode();
        }
        return;
    }

    // All operations that may try and evaluate the function shouldn't return but let execution fall through to here

    if(!tam.alpha) {
      // Check whether it is possible to add any more digits: if not, execute the function
      if(min <= tam.value && tam.value <= max && (forceTry || tam.value*10 > max)) {
        int16_t value = tam.value;
        bool_t run = true;
        if(tam.dot) {
          value += FIRST_LOCAL_REGISTER;
        }
        if(tam.indirect) {
          value = indirectAddressing(value, min, max);
          run = (lastErrorCode == 0);
        }
        if(tam.function == ITM_GTOP) {
          if(forceTry && tam.digitsSoFar < 3) {
            fnGoto(value);
          }
          else {
            reallyRunFunction(getOperation(), value + programList[currentProgramNumber - 1].step - 1);
          }
        }
        else if(run) {
          reallyRunFunction(getOperation(), value);
        }
        tamLeaveMode();
      }
    }
    else {
      bool_t tryAllocate = (tam.function == ITM_STO && !tam.indirect);
      int16_t value;
      if(tryAllocate) {
        value = findOrAllocateNamedVariable(aimBuffer);
      }
      else {
        value = findNamedVariable(aimBuffer);
        if(value == INVALID_VARIABLE) {
          temporaryInformation = TI_UNDEF_SOURCE_VAR;
        }
      }
      aimBuffer[0] = 0;
      if(tam.indirect && value != INVALID_VARIABLE) {
        value = indirectAddressing(value, min, max);
        if(lastErrorCode != 0) {
          value = INVALID_VARIABLE;
        }
      }
      if(value != INVALID_VARIABLE) {
        reallyRunFunction(getOperation(), value);
      }
      tamLeaveMode();
    }
  }

  /********************************************//**
   * \brief Sets the calc mode to TAM buffer mode
   *
   * \return void
   ***********************************************/
  void tamEnterMode(void) {
    if(calcMode == CM_NIM) {
      closeNim();
    }

    tam.alpha = false;
    tam.currentOperation = tam.function;
    tam.digitsSoFar = 0;
    tam.dot = false;
    tam.indirect = false;
    tam.value = 0;

    switch(tam.mode) {
      case TM_VALUE:
      case TM_VALUE_CHB:
      case TM_REGISTER:
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

      default:
        sprintf(errorMessage, "In function calcModeTam: %" PRIu16 " is an unexpected value for tam.mode!", tam.mode);
        displayBugScreen(errorMessage);
        return;
    }

    numberOfTamMenusToPop = 1;

    updateTamBuffer();

    clearSystemFlag(FLAG_ALPHA);

    #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
      calcModeTamGui();
    #endif // PC_BUILD && (SCREEN_800X480 == 0)
  }



  /********************************************//**
   * \brief Leaves the TAM buffer
   *
   * \return void
   ***********************************************/
  void tamLeaveMode(void) {
    tam.alpha = false;
    tam.mode = 0;
    catalog = CATALOG_NONE;
    clearSystemFlag(FLAG_ALPHA);

    while(numberOfTamMenusToPop--) {
      popSoftmenu();
    }

    #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
      if(calcMode == CM_NORMAL || calcMode == CM_PEM) {
        calcModeNormalGui();
      }
      else if(calcMode == CM_AIM) {
        calcModeAimGui();
      }
    #endif // PC_BUILD && (SCREEN_800X480 == 0)
  }

  static uint16_t keyToEvent(uint16_t key, int16_t *operation, int16_t *digit, int16_t *letteredRegister) {
    uint16_t event = TT_NOTHING;
    if(item == ITM_ENTER) {
      event = TT_ENTER;
    }
    else if(tam.alpha) {
      // Text added above, just transition to variable to get the text
      if(stringGlyphLength(aimBuffer) > 6) {
        event = TT_ENTER;
      }
      else {
        event = TT_VARIABLE;
      }
    }
    else if(item==ITM_Max || item==ITM_Min || item==ITM_ADD || item==ITM_SUB || item==ITM_MULT || item==ITM_DIV || item==ITM_Config || item==ITM_Stack || item==ITM_dddEL || item==ITM_dddIJ) { // Operation
      *operation = item;
      event = TT_OPERATION;
    }
    else if(tam.function == ITM_toINT && item == ITM_REG_I) {
      event = TT_INT;
    }
    else if(tam.function == ITM_toINT && item == ITM_alpha) {
      event = TT_FP;
    }
    else if(tam.function == ITM_toINT && item == ITM_REG_D) {
      event = TT_BASE10;
    }
    else if(tam.function == ITM_toINT && item == ITM_REG_B) {
      event = TT_BASE02;
    }
    else if(tam.function == ITM_toINT && item == ITM_HEX) {
      event = TT_BASE16;
    }
    else if(REGISTER_X <= indexOfItems[item].param && indexOfItems[item].param <= REGISTER_K) { // Lettered register
      *letteredRegister = indexOfItems[item].param;
      event = TT_LETTER;
    }
    else if(ITM_0 <= item && item <= ITM_9) { // Digits from 0 to 9
      *digit = item - ITM_0;
      event = TT_DIGIT;
    }
    else if(item == ITM_PERIOD) { // .
      event = TT_DOT;
    }
    else if(item == ITM_INDIRECTION) { // Indirection
      event = TT_INDIRECT;
    }
    else if(item == ITM_BACKSPACE) {
      event = TT_BACKSPACE;
    }
    else if(item == ITM_alpha) {
      event = TT_VARIABLE;
    }
    else if(item == ITM_0P || item == ITM_1P) {
      reallocateRegister(TEMP_REGISTER_1, dtReal34, REAL34_SIZE, AM_NONE);
      real34Copy(item == ITM_1P ? const34_1 : const34_0, REGISTER_REAL34_DATA(TEMP_REGISTER_1));
      *letteredRegister = TEMP_REGISTER_1;
      event = TT_LETTER;
    }
    return event;
  }

  void tamProcessInput(uint16_t item) {
    int16_t operation, digit, letteredRegister;
    uint16_t event = keyToEvent(item, &operation, &digit, &letteredRegister);
    reallyTamTransitionSystem(event, operation, digit, letteredRegister);
    updateTamBuffer();
  }
#endif // TESTSUITE_BUILD
