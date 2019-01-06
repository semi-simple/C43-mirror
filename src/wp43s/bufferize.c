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
 * \file bufferize.c
 ***********************************************/

#include "wp43s.h"


void fnAim(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnAim");
  #endif

  resetShiftState();
  calcModeAIM(NOPARAM); // Alpha Input Mode

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnAim");
  #endif
}


/********************************************//**
 * \brief Adds an item to the alpha buffer
 *
 * \param[in] item uint16_t
 * \return void
 ***********************************************/
void addItemToBuffer(uint16_t item) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addItemToBuffer");
  #endif

  if(item == NOPARAM) {
    displayBugScreen("In function addItemToBuffer: item should not be NOPARAM=7654!");
  }
  else {
    if(calcMode == CM_AIM) {
      if(nextChar == NC_SUBSCRIPT) {
        if(item == CHR_alpha                    ) item  = CHR_SUB_alpha;
        if(item == CHR_delta                    ) item  = CHR_SUB_delta;
        if(item == CHR_mu                       ) item  = CHR_SUB_mu;
        if(item == CHR_SUN                      ) item  = CHR_SUB_SUN;
        if(item == CHR_h                        ) item  = CHR_SUB_h;
        if(item == CHR_t                        ) item  = CHR_SUB_t;
        if(item == CHR_INFINITY                 ) item  = CHR_SUB_INFINITY;
        if(item == CHR_s                        ) item  = CHR_SUB_s;
        if(item == CHR_PLUS                     ) item  = CHR_SUB_PLUS;
        if(item == CHR_MINUS                    ) item  = CHR_SUB_MINUS;
        if(item >= CHR_0    && item <= CHR_9    ) item += CHR_SUB_0    - CHR_0;
        if(item >= CHR_a    && item <= CHR_e    ) item += CHR_SUB_a    - CHR_a;
        if(item >= CHR_i    && item <= CHR_q    ) item += CHR_SUB_i    - CHR_i;
        if(item >= CHR_u    && item <= CHR_z    ) item += CHR_SUB_u    - CHR_u;
        if(item >= CHR_A    && item <= CHR_Z    ) item += CHR_SUB_A    - CHR_A;
      }
      else if(nextChar == NC_SUPERSCRIPT) {
        if(item == CHR_a                        ) item  = CHR_SUP_a;
        if(item == CHR_x                        ) item  = CHR_SUP_x;
        if(item == CHR_INFINITY                 ) item  = CHR_SUP_INFINITY;
        if(item == CHR_PLUS                     ) item  = CHR_SUP_PLUS;
        if(item == CHR_MINUS                    ) item  = CHR_SUP_MINUS;
        if(item >= CHR_0    && item <= CHR_9    ) item += CHR_SUP_0    - CHR_0;
        if(item >= CHR_f    && item <= CHR_h    ) item += CHR_SUP_f    - CHR_f;
        if(item == CHR_r                        ) item  = CHR_SUP_r;
        if(item == CHR_T                        ) item  = CHR_SUP_T;
      }

      if(stringByteLength(aimBuffer) + stringByteLength(indexOfItems[item].itemPrinted) >= AIM_BUFFER_LENGTH) {
        sprintf(errorMessage, "In function addItemToBuffer: the AIM input buffer is full! %d bytes for now", AIM_BUFFER_LENGTH);
        displayBugScreen(errorMessage);
      }
      else {
        strcat(aimBuffer, indexOfItems[item].itemPrinted);
        if(stringWidth(aimBuffer, &standardFont, true, true) >= SCREEN_WIDTH-9) {
          btnClicked(NULL, "16"); // back space
          #ifdef PC_BUILD
            showInfoDialog("In function addItemToBuffer:", "the aimBuffer string is too wide!", NULL, NULL);
          #endif
        }
      else
         xCursor = showString(aimBuffer, &standardFont, 1, Y_POSITION_OF_AIM_LINE+6, vmNormal, true, true);
      }

      nextChar = NC_NORMAL;
    }

    else if(calcMode == CM_TAM) {
      if(tamMode==TM_STO_RCL && (item==ITM_Max || item==ITM_Min || item==ITM_ADD || item==ITM_SUB || item==ITM_MULT || item==ITM_DIV || item==ITM_Config || item==ITM_Stack)) { // Operation
        tamOperation = item;
        tamTransitionSystem(TT_OPERATION);
      }
      else if(REGISTER_X <= indexOfItems[item].param && indexOfItems[item].param <= REGISTER_K) { // Lettered register
        tamLetteredRegister = indexOfItems[item].param;
        tamTransitionSystem(TT_LETTER);
      }
      //else if(namedVariable)
      // tamTransitionSystem(TT_VARIABLE);
      else if(CHR_0 <= item && item <= CHR_9) { // Digits from 0 to 9
        tamDigit = item - CHR_0;
        tamTransitionSystem(TT_DIGIT);
      }
      else if(item == ITM_ENTER) {
        tamTransitionSystem(TT_ENTER);
      }
      else if(item == CHR_PERIOD) { // --> .
        tamTransitionSystem(TT_DOT);
      }
      else if(item == ITM_INDIRECTION) { // --> Indirection
        tamTransitionSystem(TT_INDIRECT);
      }
      else if(item == KEY_BACKSPACE) {
        tamTransitionSystem(TT_BACKSPACE);
      }
      else {
        funcOK = false;

        #if (LOG_FUNCTIONS == 1)
          leavingFunction("addItemToBuffer");
        #endif

        return;
      }
    }

    else if(calcMode == CM_NIM) {
      addItemToNimBuffer(item);
    }

    else {
      funcOK = false;

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("addItemToBuffer");
      #endif

      return;
    }

    funcOK = true;
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addItemToBuffer");
  #endif
}



void addItemToNimBuffer(int16_t item) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addItemToNimBuffer");
  #endif

  int16_t index;
  bool_t done;

  if(calcMode == CM_NORMAL) {
    if(item == ITM_EXPONENT) {
      calcModeNIM(NOPARAM);
      nimBuffer[0] = '+';
      nimBuffer[1] = '1';
      nimBuffer[2] = 0;
      nimNumberPart = NP_REAL_FLOAT_PART;
    }

    else if(item==CHR_PERIOD) {
      calcModeNIM(NOPARAM);
      nimBuffer[0] = '+';
      nimBuffer[1] = '0';
      nimBuffer[2] = 0;
      nimNumberPart = NP_INT_10;
    }

    else if(CHR_0<=item && item<=CHR_F) {
      calcModeNIM(NOPARAM);
      nimBuffer[0] = '+';
      nimBuffer[1] = 0;
      nimNumberPart = NP_EMPTY;
    }

    else {
      sprintf(errorMessage, "In function addItemToNimBuffer: %d is an unexpected item value when initializing NIM!", item);
      displayBugScreen(errorMessage);

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("addItemToNimBuffer");
      #endif

      return;
    }

    //debugNIM();
  }

  done = false;
  if(CHR_0 <= item && item <= CHR_9) {
    done = true;

    strcat(nimBuffer, indexOfItems[item].itemPrinted);
    if(nimNumberPart == NP_EMPTY) {
      nimNumberPart = NP_INT_10;
      //debugNIM();
    }
  }

  else if(CHR_A <= item && item <= CHR_F) {
    done = true;

    if(nimNumberPart == NP_EMPTY || nimNumberPart == NP_INT_10 || nimNumberPart == NP_INT_16) {
      strcat(nimBuffer, indexOfItems[item].itemPrinted);
      hexDigits++;

      nimNumberPart = NP_INT_16;
      //debugNIM();
    }
  }

  else if(item == CHR_PERIOD) {
    done = true;

    if(nimBuffer[strlen(nimBuffer)-1] == 'i') {
      strcat(nimBuffer, "0");
    }

    if(nimNumberPart == NP_INT_10) {
      strcat(nimBuffer, ".");

      nimNumberPart = NP_REAL_FLOAT_PART;
      //debugNIM();
    }

    else if(nimNumberPart == NP_REAL_FLOAT_PART) {
      uint16_t i;

      if(nimBuffer[strlen(nimBuffer) - 1] == '.') {
        strcat(nimBuffer, "0");
      }

      for(i=0; i<strlen(nimBuffer); i++) {
        if(nimBuffer[i] == '.') {
          nimBuffer[i] = ' ';
          break;
        }
      }
      strcat(nimBuffer, "/");

      nimNumberPart = NP_FRACTION_DENOMINATOR;
      //debugNIM();
    }

    else if(nimNumberPart == NP_COMPLEX_INT_PART) {
      strcat(nimBuffer, ".");

      nimNumberPart = NP_COMPLEX_FLOAT_PART;
      //debugNIM();
    }
  }

  else if(item == ITM_EXPONENT) {
    done = true;

    if(nimBuffer[strlen(nimBuffer)-1] == 'i') {
      strcat(nimBuffer, "1");
    }

    if(nimNumberPart == NP_INT_10 || nimNumberPart == NP_REAL_FLOAT_PART) {
      strcat(nimBuffer, "e+");
      exponentSignLocation = strlen(nimBuffer) - 1;

      nimNumberPart = NP_REAL_EXPONENT;
      //debugNIM();
    }

    else if(nimNumberPart == NP_COMPLEX_INT_PART || nimNumberPart == NP_COMPLEX_FLOAT_PART) {
      strcat(nimBuffer, "e+");
      imaginaryExponentSignLocation = strlen(nimBuffer) - 1;

      nimNumberPart = NP_COMPLEX_EXPONENT;
      //debugNIM();
    }
  }

  else if(item == ITM_toINT) { // #
    done = true;

    if(nimNumberPart == NP_INT_10 || nimNumberPart == NP_INT_16) {
      strcat(nimBuffer, "#");

      nimNumberPart = NP_INT_BASE;
      //debugNIM();
    }
  }

  else if(item == ITM_CHS) { // +/-
    done = true;

    if(nimNumberPart == NP_INT_10 || nimNumberPart == NP_INT_16 || nimNumberPart == NP_INT_BASE || nimNumberPart == NP_REAL_FLOAT_PART || nimNumberPart == NP_FRACTION_DENOMINATOR) {
      if(nimBuffer[0] == '+') {
        nimBuffer[0] = '-';
      }
      else {
        nimBuffer[0] = '+';
      }
    }

    else if(nimNumberPart == NP_REAL_EXPONENT) {
      if(nimBuffer[exponentSignLocation] == '+') {
        nimBuffer[exponentSignLocation] = '-';
      }
      else {
        nimBuffer[exponentSignLocation] = '+';
      }
    }

    else if(nimNumberPart == NP_COMPLEX_INT_PART || nimNumberPart == NP_COMPLEX_FLOAT_PART) {
      if(nimBuffer[imaginaryMantissaSignLocation] == '+') {
        nimBuffer[imaginaryMantissaSignLocation] = '-';
      }
      else {
        nimBuffer[imaginaryMantissaSignLocation] = '+';
      }
    }

    else if(nimNumberPart == NP_COMPLEX_EXPONENT) {
      if(nimBuffer[imaginaryExponentSignLocation] == '+') {
        nimBuffer[imaginaryExponentSignLocation] = '-';
      }
      else {
        nimBuffer[imaginaryExponentSignLocation] = '+';
      }
    }
  }

  else if(item == KEY_CC) {
    int16_t lastChar = strlen(nimBuffer) - 1;

    done = true;

    if(nimNumberPart == NP_REAL_EXPONENT && (nimBuffer[lastChar] == '+' || nimBuffer[lastChar] == '-') && nimBuffer[lastChar - 1] == 'e') {
      nimBuffer[lastChar - 1] = 0;
    }
    else if(nimNumberPart == NP_REAL_EXPONENT && nimBuffer[lastChar] == 'e') {
      nimBuffer[lastChar] = 0;
    }

    if(nimNumberPart == NP_INT_10 || nimNumberPart == NP_REAL_FLOAT_PART || nimNumberPart == NP_REAL_EXPONENT) {
      imaginaryMantissaSignLocation = strlen(nimBuffer);
      strcat(nimBuffer, "+i");

      nimNumberPart = NP_COMPLEX_INT_PART;
      //debugNIM();
    }
  }

  else if(item == ITM_pi) {
    done = true;

    if(nimNumberPart == NP_COMPLEX_INT_PART && nimBuffer[strlen(nimBuffer) - 1] == 'i') {
      strcat(nimBuffer, "3.141592653589793");
    }
  }

  else if(item == KEY_BACKSPACE) {
    int16_t lastChar = strlen(nimBuffer) - 1;

    done = true;

    if(nimNumberPart == NP_INT_10) {
    }

    else if(nimNumberPart == NP_INT_16) {
      if(nimBuffer[lastChar] >= 'A') {
        hexDigits--;
      }

      if(hexDigits == 0) {
        nimNumberPart = NP_INT_10;
        //debugNIM();
      }
    }

    else if(nimNumberPart == NP_INT_BASE) {
      if(nimBuffer[lastChar] == '#') {
        if(hexDigits > 0) {
          nimNumberPart = NP_INT_16;
        }
        else {
          nimNumberPart = NP_INT_10;
        }
        //debugNIM();
      }
    }

    else if(nimNumberPart == NP_REAL_FLOAT_PART) {
      if(nimBuffer[lastChar] == '.') {
        nimNumberPart = NP_INT_10;
        //debugNIM();
      }
    }

    else if(nimNumberPart == NP_REAL_EXPONENT) {
      if(nimBuffer[lastChar] == '+') {
        nimBuffer[lastChar--] = 0;
      }

      if(nimBuffer[lastChar] == 'e') {
        nimNumberPart = NP_INT_10;
        for(int16_t i=0; i<lastChar; i++) {
          if(nimBuffer[i] == '.') {
            nimNumberPart = NP_REAL_FLOAT_PART;
            break;
          }
        }
        //debugNIM();
      }
    }

    else if(nimNumberPart == NP_FRACTION_DENOMINATOR) {
      if(nimBuffer[lastChar] == '/') {
        nimNumberPart = NP_REAL_FLOAT_PART;
        for(int16_t i=0; i<lastChar; i++) {
          if(nimBuffer[i] == ' ') {
            nimBuffer[i] = '.';
            break;
          }
        }
        //debugNIM();
      }
    }

    else if(nimNumberPart == NP_COMPLEX_INT_PART) {
      if(nimBuffer[lastChar] == 'i') {
        nimNumberPart = NP_INT_10;
        for(int16_t i=0; i<lastChar; i++) {
          if(nimBuffer[i] == 'e') {
            nimNumberPart = NP_REAL_EXPONENT;
            break;
          }
          if(nimBuffer[i] == '.') {
            nimNumberPart = NP_REAL_FLOAT_PART;
          }
        }
        //debugNIM();
        lastChar--;
      }
    }

    else if(nimNumberPart == NP_COMPLEX_FLOAT_PART) {
      if(nimBuffer[lastChar] == '.') {
        nimNumberPart = NP_COMPLEX_INT_PART;
        //debugNIM();
      }
    }

    else if(nimNumberPart == NP_COMPLEX_EXPONENT) {
      if(nimBuffer[lastChar] == '+') {
        nimBuffer[lastChar--] = 0;
      }

      if(nimBuffer[lastChar] == 'e') {
        nimNumberPart = NP_COMPLEX_INT_PART;
        for(int16_t i=imaginaryMantissaSignLocation+2; i<lastChar; i++) {
          if(nimBuffer[i] == '.') {
            nimNumberPart = NP_COMPLEX_FLOAT_PART;
            break;
          }
        }
        //debugNIM();
      }
    }

    nimBuffer[lastChar--] = 0;

    if(lastChar == -1 || (lastChar == 0 && nimBuffer[0] == '+')) {
      calcModeNormal();
      restoreStack();
    }
  }

  else if(item == KEY_EXIT) {
    done = true;

    closeNim();
    if(calcMode != CM_NIM && lastErrorCode == 0) {
      #if (STACK_LIFT_DEBUG == 1)
        stackLiftEnable();
      #else
        stackLiftEnabled = true;
      #endif
    }
  }

  else if(item == ITM_ENTER) {
    done = true;

    closeNim();
    if(calcMode != CM_NIM && lastErrorCode == 0) {
      if(nimNumberPart == NP_COMPLEX_INT_PART || nimNumberPart == NP_COMPLEX_FLOAT_PART || nimNumberPart == NP_COMPLEX_EXPONENT) {
        fnSetFlag(FLAG_CPXRES);
      }

      #if (STACK_LIFT_DEBUG == 1)
        stackLiftEnable();
      #else
        stackLiftEnabled = true;
      #endif

      liftStack(getRegisterDataType(REGISTER_X), getRegisterDataSize(REGISTER_X));
      copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);

      refreshStack();

      #if (STACK_LIFT_DEBUG == 1)
        stackLiftDisable();
      #else
        stackLiftEnabled = false;
      #endif

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("addItemToNimBuffer");
      #endif

      return;
    }
  }

  else if(item == ITM_LN && nimNumberPart == NP_INT_BASE && nimBuffer[strlen(nimBuffer) - 1] == '#') { // D for decimal base
    done = true;

    strcat(nimBuffer, "10");
  }

  else if(item == ITM_RCL && nimNumberPart == NP_INT_BASE && nimBuffer[strlen(nimBuffer) - 1] == '#') { // H for hexadecimal base
    done = true;

    strcat(nimBuffer, "16");
  }

  if(done) {
    //printf("nimBuffer = <%s>\n", nimBuffer);
    //Convert nimBuffer to display string
    if(nimNumberPart == NP_INT_10 || nimNumberPart == NP_INT_16 || nimNumberPart == NP_INT_BASE || nimNumberPart == NP_REAL_FLOAT_PART) {
      if(nimBuffer[0] == '-') {
        strcpy(nimBufferDisplay, nimBuffer);
      }
      else {
        strcpy(nimBufferDisplay, nimBuffer + 1);
      }
    }

    else if(nimNumberPart == NP_REAL_EXPONENT) {
      if(nimBuffer[0] == '-') {
        strncpy(nimBufferDisplay, nimBuffer, exponentSignLocation - 1);
        nimBufferDisplay[exponentSignLocation - 1] = 0;
      }
      else {
        strncpy(nimBufferDisplay, nimBuffer + 1, exponentSignLocation - 2);
        nimBufferDisplay[exponentSignLocation - 2] = 0;
      }

      exponentToDisplayString(atoi(nimBuffer + exponentSignLocation), nimBufferDisplay + stringByteLength(nimBufferDisplay));
      if(nimBuffer[exponentSignLocation + 1] == 0) {
        nimBufferDisplay[stringByteLength(nimBufferDisplay) - 2] = 0;
        if(nimBuffer[exponentSignLocation] == '-') {
          strcat(nimBufferDisplay, NUM_SUP_MINUS);
        }
      }
    }

    else if(nimNumberPart == NP_FRACTION_DENOMINATOR) {
      if(nimBuffer[0] == '-') {
        strcpy(nimBufferDisplay, nimBuffer);
      }
      else {
        strcpy(nimBufferDisplay, nimBuffer + 1);
      }

      for(index=0; nimBufferDisplay[index]!=' '; index++); // The ending semi colon is OK here
      strcpy(nimBufferDisplay + index, NUM_SPACE_4_PER_EM);
      supNumberToDisplayString(atoi(nimBuffer + index + 1), nimBufferDisplay + index + 2);

      strcat(nimBufferDisplay, "/");

      for(; nimBuffer[index]!='/'; index++); // The ending semi colon is OK here
      if(nimBuffer[++index] != 0) {
        subNumberToDisplayString(atoi(nimBuffer + index), nimBufferDisplay + stringByteLength(nimBufferDisplay));
      }
    }

    else if(nimNumberPart == NP_COMPLEX_INT_PART || nimNumberPart == NP_COMPLEX_FLOAT_PART || nimNumberPart == NP_COMPLEX_EXPONENT) {
      // Real part
      for(index=1; index<imaginaryMantissaSignLocation && nimBuffer[index] != 'e'; index++); // The ending semi colon is OK here

      if(index < imaginaryMantissaSignLocation) { // There is an exposant in the real part
        if(nimBuffer[0] == '-') {
          strncpy(nimBufferDisplay, nimBuffer, exponentSignLocation - 1);
          nimBufferDisplay[exponentSignLocation - 1] = 0;
        }
        else {
          strncpy(nimBufferDisplay, nimBuffer + 1, exponentSignLocation - 2);
          nimBufferDisplay[exponentSignLocation - 2] = 0;
        }

        exponentToDisplayString(atoi(nimBuffer + exponentSignLocation), nimBufferDisplay + stringByteLength(nimBufferDisplay));
      }
      else { // No exponent
        if(nimBuffer[0] == '-') {
          strncpy(nimBufferDisplay, nimBuffer, index);
          nimBufferDisplay[index] = 0;
        }
        else {
          strncpy(nimBufferDisplay, nimBuffer + 1, index - 1);
          nimBufferDisplay[index - 1] = 0;
        }
      }

      // Complex "separator"
      if(complexMode == CM_RECTANGULAR) {
        strcat(nimBufferDisplay, NUM_SPACE_4_PER_EM);
        if(nimBuffer[imaginaryMantissaSignLocation] == '-') {
          strcat(nimBufferDisplay, "-");
        }
        else {
          strcat(nimBufferDisplay, "+");
        }
        strcat(nimBufferDisplay, COMPLEX_UNIT);
        strcat(nimBufferDisplay, PRODUCT_SIGN);
      }
      else {
        strcat(nimBufferDisplay, NUM_SPACE_4_PER_EM NUM_MEASURED_ANGLE NUM_SPACE_4_PER_EM);
        if(nimBuffer[imaginaryMantissaSignLocation] == '-') {
          strcat(nimBufferDisplay, "-");
        }
      }

      // Imaginary part
      for(index=imaginaryMantissaSignLocation+1; index<(int16_t)strlen(nimBuffer) && nimBuffer[index] != 'e'; index++); // The ending semi colon is OK here

      if(index < (int16_t)strlen(nimBuffer)) { // There is an exposant in the real part
        nimBuffer[index] = 0;
        strcat(nimBufferDisplay, nimBuffer + imaginaryMantissaSignLocation + 2);
        nimBuffer[index] = 'e';

        exponentToDisplayString(atoi(nimBuffer + imaginaryExponentSignLocation), nimBufferDisplay + stringByteLength(nimBufferDisplay));

        if(nimBuffer[imaginaryExponentSignLocation + 1] == 0) {
          nimBufferDisplay[stringByteLength(nimBufferDisplay) - 2] = 0;
          if(nimBuffer[imaginaryExponentSignLocation] == '-') {
            strcat(nimBufferDisplay, NUM_SUP_MINUS);
          }
        }
      }
      else {
        strcat(nimBufferDisplay, nimBuffer + imaginaryMantissaSignLocation + 2);
      }
    }

    else {
      sprintf(errorMessage, "In function addItemToNimBuffer: %d is an unexpected nimNumberPart value when converting buffer to display!", nimNumberPart);
      displayBugScreen(errorMessage);
    }

    if(radixMark == RM_COMMA) {
      for(index=stringByteLength(nimBufferDisplay) - 1; index>0; index--) {
        if(nimBufferDisplay[index] == '.') {
          nimBufferDisplay[index] = ',';
        }
      }
    }

    refreshRegisterLine(NIM_REGISTER_LINE);
  }

  else {
    closeNim();
    if(calcMode != CM_NIM && lastErrorCode == 0) {
      runFunction(item);
      refreshStack();
    }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addItemToNimBuffer");
  #endif
}



int16_t getStoRclOperation(void) {
  if(tamFunction == ITM_STO) {
    if     (tamCurrentOperation == ITM_ADD   ) return ITM_STOPLUS;
    else if(tamCurrentOperation == ITM_SUB   ) return ITM_STOMINUS;
    else if(tamCurrentOperation == ITM_MULT  ) return ITM_STOMULT;
    else if(tamCurrentOperation == ITM_DIV   ) return ITM_STODIV;
    else if(tamCurrentOperation == ITM_Max   ) return ITM_STOMAX;
    else if(tamCurrentOperation == ITM_Min   ) return ITM_STOMIN;
    else if(tamCurrentOperation == ITM_Config) return ITM_STOCFG;
    else if(tamCurrentOperation == ITM_Stack ) return ITM_STOS;
    else {
      sprintf(errorMessage, "In function getStoRclOperation: status=0 tamTransition=TT_OPERATION tamCurrentOperation=ITM_STO. %d is an unexpected value for tamCurrentOperation!", tamCurrentOperation);
      displayBugScreen(errorMessage);
      return ITM_NOP;
    }
  }
  else if(tamFunction == ITM_RCL) {
    if     (tamCurrentOperation == ITM_ADD   ) return ITM_RCLPLUS;
    else if(tamCurrentOperation == ITM_SUB   ) return ITM_RCLMINUS;
    else if(tamCurrentOperation == ITM_MULT  ) return ITM_RCLMULT;
    else if(tamCurrentOperation == ITM_DIV   ) return ITM_RCLDIV;
    else if(tamCurrentOperation == ITM_Max   ) return ITM_RCLMAX;
    else if(tamCurrentOperation == ITM_Min   ) return ITM_RCLMIN;
    else if(tamCurrentOperation == ITM_Config) return ITM_STOCFG;
    else if(tamCurrentOperation == ITM_Stack ) return ITM_STOS;
    else {
      sprintf(errorMessage, "In function getStoRclOperation: status=0 tamTransition=TT_OPERATION tamCurrentOperation=ITM_RCL. %d is an unexpected value for tamCurrentOperation!", tamCurrentOperation);
      displayBugScreen(errorMessage);
      return ITM_NOP;
    }
  }
  else {
    sprintf(errorMessage, "In function getStoRclOperation: status=0 tamTransition=TT_OPERATION. %d is an unexpected value for tamFunction!", tamFunction);
    displayBugScreen(errorMessage);
    return ITM_NOP;
  }
}



void tamTransitionSystem(uint16_t tamTransition) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("tamTransitionSystem");
  #endif

  uint16_t status = transitionSystemStatus;

  // Entry point
  if(status == 0) { // OP __
    if(tamTransition == TT_OPERATION) {
      strcpy(transitionSystemOperation, tamOperation==ITM_Max ? STD_UP_ARROW : (tamOperation==ITM_Min ? STD_DOWN_ARROW : (tamOperation==ITM_ADD ? "+" : (tamOperation==ITM_SUB ? "-" : (tamOperation==ITM_MULT ? STD_CROSS : (tamOperation==ITM_DIV ? "/" : (tamOperation==ITM_Config ? "CFG" : "S")))))));
      sprintf(tamBuffer, "%s%s __", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      tamCurrentOperation = tamOperation;
      transitionSystemStatus = 1;
    }
    else if(tamTransition == TT_LETTER) {
      indexOfItems[tamFunction].func(tamLetteredRegister);
      calcModeNormal();
    }
    else if(tamTransition == TT_VARIABLE) {
    }
    else if(tamTransition == TT_DIGIT) {
      tamNumber = tamDigit;
      sprintf(tamBuffer, "%s %d_", indexOfItems[tamFunction].itemPrinted, tamNumber);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 2;
    }
    else if(tamTransition == TT_DOT) {
      sprintf(tamBuffer, "%s .__", indexOfItems[tamFunction].itemPrinted);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 3;
    }
    else if(tamTransition == TT_INDIRECT) {
      sprintf(tamBuffer, "%s " STD_RIGHT_ARROW "__", indexOfItems[tamFunction].itemPrinted);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 5;
    }
    else if(tamTransition == TT_BACKSPACE) {
      calcModeNormal();
      //refreshStack();
      refreshRegisterLine(TAM_REGISTER_LINE);
    }
  }


  // RCL+, RCL-, RCL×, RCL/, RCL^, RCLv, STO+, STO-, STO×, STO/, STO^ or RCLv
  else if(status == 1) {// OPo __
    strcpy(transitionSystemOperation, tamOperation==ITM_Max ? STD_UP_ARROW : (tamOperation==ITM_Min ? STD_DOWN_ARROW : (tamOperation==ITM_ADD ? "+" : (tamOperation==ITM_SUB ? "-" : (tamOperation==ITM_MULT ? STD_CROSS : (tamOperation==ITM_DIV ? "/" : (tamOperation==ITM_Config ? "CFG" : "S")))))));

    if((tamTransition==TT_OPERATION && tamOperation==tamCurrentOperation) || tamTransition==TT_BACKSPACE) {
      sprintf(tamBuffer, "%s __   ", indexOfItems[tamFunction].itemPrinted);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 0;
    }
    else if(tamTransition == TT_OPERATION) {
      tamCurrentOperation = tamOperation;
      strcpy(transitionSystemOperation, tamOperation==ITM_Max ? STD_UP_ARROW : (tamOperation==ITM_Min ? STD_DOWN_ARROW : (tamOperation==ITM_ADD ? "+" : (tamOperation==ITM_SUB ? "-" : (tamOperation==ITM_MULT ? STD_CROSS : (tamOperation==ITM_DIV ? "/" : (tamOperation==ITM_Config ? "CFG" : "S")))))));
      sprintf(tamBuffer, "%s%s __", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);
    }
    else if(tamTransition == TT_LETTER) {
      indexOfItems[getStoRclOperation()].func(tamLetteredRegister);
      calcModeNormal();
    }
    else if(tamTransition == TT_VARIABLE) {
    }
    else if(tamTransition == TT_DIGIT) {
      tamNumber = tamDigit;
      sprintf(tamBuffer, "%s%s %d_", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation, tamNumber);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 9;
    }
    else if(tamTransition == TT_DOT) {
      sprintf(tamBuffer, "%s%s .__", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 10;
    }
    else if(tamTransition == TT_INDIRECT) {
      sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW "__", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 12;
    }
  }


  else if(status == 2) { // OP d_
    if(tamTransition == TT_DIGIT) {
      if(tamNumberMin <= (tamNumber*10 + tamDigit) && (tamNumber*10 + tamDigit) <= tamNumberMax) {
        indexOfItems[tamFunction].func(tamNumber*10 + tamDigit);
        calcModeNormal();
      }
    }
    else if(tamTransition == TT_ENTER) {
      if(tamNumberMin <= tamNumber && tamNumber <= tamNumberMax) {
        indexOfItems[tamFunction].func(tamNumber);
        calcModeNormal();
      }
    }
    else if(tamTransition == TT_BACKSPACE) {
      sprintf(tamBuffer, "%s __", indexOfItems[tamFunction].itemPrinted);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);
      transitionSystemStatus = 0;
    }
  }


  else if(status == 3) { // OP .__
    if(tamTransition == TT_DIGIT) {
      if(tamDigit < numberOfLocalRegisters) {
        tamNumber = tamDigit;
        sprintf(tamBuffer, "%s .%d_", indexOfItems[tamFunction].itemPrinted, tamNumber);
        showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

        transitionSystemStatus = 4;
      }
    }
    else if(tamTransition == TT_BACKSPACE) {
      sprintf(tamBuffer, "%s __ ", indexOfItems[tamFunction].itemPrinted);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 0;
    }
  }


  else if(status == 4) { // OP .d_
    if(tamTransition == TT_DIGIT) {
      if(tamNumber*10 + tamDigit < numberOfLocalRegisters) {
        indexOfItems[tamFunction].func(tamNumber*10 + tamDigit + FIRST_LOCAL_REGISTER);
        calcModeNormal();
      }
    }
    else if(tamTransition == TT_ENTER) {
      if(tamNumber < numberOfLocalRegisters) {
        indexOfItems[tamFunction].func(tamNumber + FIRST_LOCAL_REGISTER);
        calcModeNormal();
      }
    }
    else if(tamTransition == TT_BACKSPACE) {
      sprintf(tamBuffer, "%s .__", indexOfItems[tamFunction].itemPrinted);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);
      transitionSystemStatus = 3;
    }
  }


  else if(status == 5) { // OP -->__
    if(tamTransition == TT_LETTER) {
      calcRegister_t value = indirectAddressing(tamLetteredRegister);

      if(value != 9999) {
        indexOfItems[tamFunction].func(value);
        calcModeNormal();
      }
    }
    else if(tamTransition == TT_VARIABLE) {
    }
    else if(tamTransition == TT_DIGIT) {
      tamNumber = tamDigit;
      sprintf(tamBuffer, "%s " STD_RIGHT_ARROW "%d_", indexOfItems[tamFunction].itemPrinted, tamNumber);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 6;
    }
    else if(tamTransition == TT_DOT) {
      sprintf(tamBuffer, "%s " STD_RIGHT_ARROW ".__", indexOfItems[tamFunction].itemPrinted);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 7;
    }
    else if(tamTransition == TT_BACKSPACE) {
      sprintf(tamBuffer, "%s __ ", indexOfItems[tamFunction].itemPrinted);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 0;
    }
  }


  else if(status == 6) { // OP -->d_
    if(tamTransition == TT_DIGIT) {
      if(tamNumber*10 + tamDigit <= 99) {
        calcRegister_t value = indirectAddressing(tamNumber*10 + tamDigit);

        if(value != 9999) {
          indexOfItems[tamFunction].func(value);
          calcModeNormal();
        }
      }
    }
    else if(tamTransition == TT_ENTER) {
      calcRegister_t value = indirectAddressing(tamNumber);

      if(value != 9999) {
        indexOfItems[tamFunction].func(value);
        calcModeNormal();
      }
    }
    else if(tamTransition == TT_BACKSPACE) {
      sprintf(tamBuffer, "%s " STD_RIGHT_ARROW "__", indexOfItems[tamFunction].itemPrinted);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);
      transitionSystemStatus = 5;
    }
  }


  else if(status == 7) { // OP -->.__
    if(tamTransition == TT_DIGIT) {
      if(tamDigit < numberOfLocalRegisters) {
        tamNumber = tamDigit;
        sprintf(tamBuffer, "%s " STD_RIGHT_ARROW ".%d_", indexOfItems[tamFunction].itemPrinted, tamNumber);
        showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

        transitionSystemStatus = 8;
      }
    }
    else if(tamTransition == TT_BACKSPACE) {
      sprintf(tamBuffer, "%s " STD_RIGHT_ARROW "__ ", indexOfItems[tamFunction].itemPrinted);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 5;
    }
  }


  else if(status == 8) { // OP -->.d_
    if(tamTransition == TT_DIGIT) {
      if(tamNumber*10 + tamDigit < numberOfLocalRegisters) {
        calcRegister_t value = indirectAddressing(tamNumber*10 + tamDigit + FIRST_LOCAL_REGISTER);

        if(value != 9999) {
          indexOfItems[tamFunction].func(value);
          calcModeNormal();
        }
      }
    }
    else if(tamTransition == TT_ENTER) {
      if(tamNumber < numberOfLocalRegisters) {
        indexOfItems[tamFunction].func(tamNumber + FIRST_LOCAL_REGISTER);
        calcModeNormal();
      }
    }
    else if(tamTransition == TT_BACKSPACE) {
      sprintf(tamBuffer, "%s " STD_RIGHT_ARROW ".__", indexOfItems[tamFunction].itemPrinted);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);
      transitionSystemStatus = 7;
    }
  }


  else if(status == 9) { //OPo d_
    if(tamTransition == TT_DIGIT) {
      if(tamNumberMin <= (tamNumber*10 + tamDigit) && (tamNumber*10 + tamDigit) <= tamNumberMax) {
        indexOfItems[getStoRclOperation()].func(tamNumber*10 + tamDigit);
        calcModeNormal();
      }
    }
    else if(tamTransition == TT_ENTER) {
      if(tamNumberMin <= tamNumber && tamNumber <= tamNumberMax) {
        indexOfItems[getStoRclOperation()].func(tamNumber);
        calcModeNormal();
      }
    }
    else if(tamTransition == TT_BACKSPACE) {
      sprintf(tamBuffer, "%s%s __", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);
      transitionSystemStatus = 1;
    }
  }


  else if(status == 10) { // OPo .__
    if(tamTransition == TT_DIGIT) {
      if(tamDigit < numberOfLocalRegisters) {
        tamNumber = tamDigit;
        sprintf(tamBuffer, "%s%s .%d_", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation, tamNumber);
        showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

        transitionSystemStatus = 11;
      }
    }
    else if(tamTransition == TT_BACKSPACE) {
      sprintf(tamBuffer, "%s%s __ ", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 1;
    }
  }


  else if(status == 11) { // OPo .d_
    if(tamTransition == TT_DIGIT) {
      if(tamNumber*10 + tamDigit < numberOfLocalRegisters) {
        indexOfItems[getStoRclOperation()].func(tamNumber*10 + tamDigit + FIRST_LOCAL_REGISTER);
        calcModeNormal();
      }
    }
    else if(tamTransition == TT_ENTER) {
      if(tamNumber < numberOfLocalRegisters) {
        indexOfItems[getStoRclOperation()].func(tamNumber + FIRST_LOCAL_REGISTER);
        calcModeNormal();
      }
    }
    else if(tamTransition == TT_BACKSPACE) {
      sprintf(tamBuffer, "%s%s .__", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);
      transitionSystemStatus = 10;
    }
  }


  else if(status == 12) { // OPo -->__
    if(tamTransition == TT_LETTER) {
      calcRegister_t value = indirectAddressing(tamLetteredRegister);

      if(value != 9999) {
        indexOfItems[getStoRclOperation()].func(value);
        calcModeNormal();
      }
    }
    else if(tamTransition == TT_VARIABLE) {
    }
    else if(tamTransition == TT_DIGIT) {
      tamNumber = tamDigit;
      sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW "%d_", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation, tamNumber);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 13;
    }
    else if(tamTransition == TT_DOT) {
      sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW ".__", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 14;
    }
    else if(tamTransition == TT_BACKSPACE) {
      sprintf(tamBuffer, "%s%s __ ", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 1;
    }
  }


  else if(status == 13) { // OPo -->d_
    if(tamTransition == TT_DIGIT) {
      if(tamNumber*10 + tamDigit <= 99) {
        calcRegister_t value = indirectAddressing(tamNumber*10 + tamDigit);

        if(value != 9999) {
          indexOfItems[getStoRclOperation()].func(value);
          calcModeNormal();
        }
      }
    }
    else if(tamTransition == TT_ENTER) {
      calcRegister_t value = indirectAddressing(tamNumber);

      if(value != 9999) {
        indexOfItems[getStoRclOperation()].func(value);
        calcModeNormal();
      }
    }
    else if(tamTransition == TT_BACKSPACE) {
      sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW "__", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);
      transitionSystemStatus = 12;
    }
  }


  else if(status == 14) { // OPo -->.__
    if(tamTransition == TT_DIGIT) {
      if(tamDigit < numberOfLocalRegisters) {
        tamNumber = tamDigit;
        sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW ".%d_", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation, tamNumber);
        showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

        transitionSystemStatus = 15;
      }
    }
    else if(tamTransition == TT_BACKSPACE) {
      sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW "__ ", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

      transitionSystemStatus = 12;
    }
  }


  else if(status == 15) { // OPo -->.d_
    if(tamTransition == TT_DIGIT) {
      if(tamNumber*10 + tamDigit < numberOfLocalRegisters) {
        calcRegister_t value = indirectAddressing(tamNumber*10 + tamDigit + FIRST_LOCAL_REGISTER);

        if(value != 9999) {
          indexOfItems[getStoRclOperation()].func(value);
          calcModeNormal();
        }
      }
    }
    else if(tamTransition == TT_ENTER) {
      if(tamNumber < numberOfLocalRegisters) {
        indexOfItems[getStoRclOperation()].func(tamNumber + FIRST_LOCAL_REGISTER);
        calcModeNormal();
      }
    }
    else if(tamTransition == TT_BACKSPACE) {
      sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW ".__", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
      showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);
      transitionSystemStatus = 14;
    }
  }


  else { // This should never happen
    sprintf(errorMessage, "In function tamTransitionSystem: unknown state %" FMT16U " of the TAM transition system! This should never happen!", status);
    displayBugScreen(errorMessage);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("tamTransitionSystem");
  #endif
}



void closeNim(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("closeNim");
  #endif

  int16_t lastChar = strlen(nimBuffer) - 1;

  if(nimNumberPart != NP_INT_16) { // We need a # and a base
    if(nimNumberPart != NP_INT_BASE || nimBuffer[lastChar] != '#') { // We need a base
      if((nimNumberPart == NP_COMPLEX_EXPONENT || nimNumberPart == NP_REAL_EXPONENT) && (nimBuffer[lastChar] == '+' || nimBuffer[lastChar] == '-') && nimBuffer[lastChar - 1] == 'e') {
        nimBuffer[--lastChar] = 0;
        lastChar--;
      }
      else if(nimNumberPart == NP_REAL_EXPONENT && nimBuffer[lastChar] == 'e') {
        nimBuffer[lastChar--] = 0;
      }

      if(nimNumberPart == NP_COMPLEX_INT_PART && nimBuffer[lastChar] == 'i') {
        nimBuffer[++lastChar] = '1';
        nimBuffer[lastChar + 1] = 0;
      }

      if((nimBuffer[0] != '-' || nimBuffer[1] != 0) && (nimBuffer[lastChar] != '-')) { // The buffer is not just the minus sign AND The last char of the buffer is not the minus sign
        calcModeNormal();

        if(nimNumberPart == NP_INT_10) {
          bigInteger_t tmp;

          stringToBigInteger(nimBuffer + (nimBuffer[0] == '+' ? 1 : 0), 10, &tmp);
          convertBigIntegerToBigIntegerRegister(&tmp, REGISTER_X);
        }
        else if(nimNumberPart == NP_INT_BASE) {
          bigInteger_t minVal, value, maxVal;
          int16_t posHash, i, lg;
          int32_t base;

          lg = strlen(nimBuffer);
          posHash = 0;
          for(i=1; i<lg; i++) {
            if(nimBuffer[i] == '#') {
              posHash = i;
              break;
            }
          }

          for(i=posHash+1; i<lg; i++) {
            if(nimBuffer[i]<'0' || nimBuffer[i]>'9') {
              // This should never happen
              displayCalcErrorMessage(9, REGISTER_T, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                showInfoDialog("In function closeNIM:", "there is a non numeric character in the base of the integer!", NULL, NULL);
              #endif

              #if (LOG_FUNCTIONS == 1)
                leavingFunction("closeNim");
              #endif

              return;
            }
          }

          base = atoi(nimBuffer + posHash + 1);
          if(base < 2 || base > 16) {
            displayCalcErrorMessage(9, REGISTER_T, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
             showInfoDialog("In function closeNIM:", "the base of the integer must be from 2 to 16!", NULL, NULL);
            #endif

            #if (LOG_FUNCTIONS == 1)
              leavingFunction("closeNim");
            #endif

            return;
          }

          for(i=nimBuffer[0] == '-' ? 1 : 0; i<posHash; i++) {
            if((nimBuffer[i] > '9' ? nimBuffer[i] - 'A' + 10 : nimBuffer[i] - '0') >= base) {
              displayCalcErrorMessage(9, REGISTER_T, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                sprintf(errorMessage, "digit %c is not allowed in base %d!", nimBuffer[i], base);
                showInfoDialog("In function closeNIM:", errorMessage, NULL, NULL);
              #endif

              restoreStack();

              #if (LOG_FUNCTIONS == 1)
                leavingFunction("closeNim");
              #endif

              return;
            }
          }

          stringToBigInteger(nimBuffer + (nimBuffer[0] == '+' ? 1 : 0), base, &value);

          // maxVal = 2^smallIntegerWordSize
          if(smallIntegerWordSize >= 1 && smallIntegerWordSize <= 64) {
            bigInteger2Exp(smallIntegerWordSize, &maxVal);
          }
          else {
            sprintf(errorMessage, "In function closeNIM: %d is an unexpected value for smallIntegerWordSize!", smallIntegerWordSize);
            displayBugScreen(errorMessage);

            #if (LOG_FUNCTIONS == 1)
              leavingFunction("closeNim");
            #endif

            return;
          }

          // minVal = -maxVal/2
          bigIntegerSetZero(&minVal); // Mandatory! Else segmentation fault at next instruction
          bigIntegerDivide2(&maxVal, &minVal); // minVal = maxVal / 2
          bigIntegerSetNegativeSign(&minVal); // minVal = -minVal

          if(smallIntegerMode != SIM_UNSIGN) {
            bigIntegerDivide2(&maxVal, &maxVal); // maxVal /= 2
          }

          bigIntegerSubtractUInt(&maxVal, 1, &maxVal); // maxVal--

          if(smallIntegerMode == SIM_UNSIGN) {
            bigIntegerSetZero(&minVal); // minVal = 0
          }

          if(smallIntegerMode == SIM_1COMPL || smallIntegerMode == SIM_SIGNMT) {
            bigIntegerAddUInt(&minVal, 1, &minVal); // minVal++
          }

          if(bigIntegerCompare(&value, &minVal) == BIG_INTEGER_LESS_THAN || bigIntegerCompare(&value, &maxVal) == BIG_INTEGER_GREATER_THAN) {
            displayCalcErrorMessage(1, REGISTER_T, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              char strMin[22], strMax[22];
              bigIntegerToString(&minVal, strMin, 10);
              bigIntegerToString(&maxVal, strMax, 10);
              sprintf(errorMessage, "For word size of %d bit%s and integer mode %s,", smallIntegerWordSize, smallIntegerWordSize>1 ? "s" : "", getSmallIntegerModeName(smallIntegerMode));
              sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "the entered number must be from %s to %s!", strMin, strMax);
              showInfoDialog("In function closeNIM:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
            #endif

            #if (LOG_FUNCTIONS == 1)
              leavingFunction("closeNim");
            #endif

            return;
          }

          reallocateRegister(REGISTER_X, dtSmallInteger, SMALL_INTEGER_SIZE, base);

          char strValue[22];
          bigIntegerToString(&value, strValue, 10);

          uint64_t val;
          if(value.sign) {
            val = atoll(strValue + 1); // value is negative: discard the minus sign
          }
          else {
            val = atoll(strValue); // value is positive
          }

          if(smallIntegerMode == SIM_UNSIGN) {
            *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = val;
          }
          else if(smallIntegerMode == SIM_2COMPL) {
            if(value.sign) {
              val = (~val + 1) & smallIntegerMask;
            }

            *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = val;
          }
          else if(smallIntegerMode == SIM_1COMPL) {
            if(value.sign) {
              val = ~val & smallIntegerMask;
            }

            *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = val;
          }
          else if(smallIntegerMode == SIM_SIGNMT) {
            if(value.sign) {
              val += smallIntegerMask;
            }

            *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = val;
          }
          else {
            sprintf(errorMessage, "In function closeNIM: %d is an unexpected value for smallIntegerMode!", smallIntegerMode);
            displayBugScreen(errorMessage);
            *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = 0;
          }
        }
        else if(nimNumberPart == NP_REAL_FLOAT_PART || nimNumberPart == NP_REAL_EXPONENT) {
          reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, 0);
          stringToReal16(nimBuffer, POINTER_TO_REGISTER_DATA(REGISTER_X));
          if(real16IsInfinite(POINTER_TO_REGISTER_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
            displayCalcErrorMessage(1, REGISTER_T, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              showInfoDialog("In function closeNIM:", "the absolute value of a real must be less than 10^385!", "Unless D flag (Danger) is set.", NULL);
            #endif
          }
        }
        else if(nimNumberPart == NP_FRACTION_DENOMINATOR) {
          int16_t i, posSpace, posSlash, lg;
          int32_t integer, numer, denom;
          real16_t temp;

          lg = strlen(nimBuffer);

          posSpace = 0;
          for(i=2; i<lg; i++) {
            if(nimBuffer[i] == ' ') {
              posSpace = i;
              break;
            }
          }

          for(i=1; i<posSpace; i++) {
            if(nimBuffer[i]<'0' || nimBuffer[i]>'9') { // This should never happen
              displayCalcErrorMessage(28, REGISTER_T, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                showInfoDialog("In function parseNimString:", "there is a non numeric character in the integer part of the fraction!", NULL, NULL);
              #endif

              #if (LOG_FUNCTIONS == 1)
                leavingFunction("closeNim");
              #endif

              return;
            }
          }

          posSlash = 0;
          for(i=posSpace+2; i<lg; i++) {
            if(nimBuffer[i] == '/') {
              posSlash = i;
              break;
            }
          }

          for(i=posSpace+1; i<posSlash; i++) {
            if(nimBuffer[i]<'0' || nimBuffer[i]>'9') { // This should never happen
             displayCalcErrorMessage(28, REGISTER_T, NIM_REGISTER_LINE);
             #if (EXTRA_INFO_ON_CALC_ERROR == 1)
               showInfoDialog("In function parseNimString:", "there is a non numeric character in the numerator part of the fraction!", NULL, NULL);
             #endif

             #if (LOG_FUNCTIONS == 1)
               leavingFunction("closeNim");
             #endif

             return;
            }
          }

          for(i=posSlash+1; i<lg; i++) {
            if(nimBuffer[i]<'0' || nimBuffer[i]>'9') {
              displayCalcErrorMessage(28, REGISTER_T, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                showInfoDialog("In function parseNimString:", "there is a non numeric character in the denominator part of the fraction!", NULL, NULL);
              #endif

              #if (LOG_FUNCTIONS == 1)
                leavingFunction("closeNim");
              #endif

              return;
            }
          }

          nimBuffer[posSpace] = 0;
          nimBuffer[posSlash] = 0;
          integer = atoi(nimBuffer + 1);
          numer   = atoi(nimBuffer + posSpace + 1);
          denom   = atoi(nimBuffer + posSlash + 1);

          if(denom == 0 && !getFlag(FLAG_DANGER)) {
            displayCalcErrorMessage(1, REGISTER_T, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              showInfoDialog("In function parseNimString:", "the denominator of the fraction should not be 0!", "Unless D flag (Danger) is set.", NULL);
            #endif

            #if (LOG_FUNCTIONS == 1)
              leavingFunction("closeNim");
            #endif

            return;
          }

          reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, 0);
          int32ToReal16(numer, POINTER_TO_REGISTER_DATA(REGISTER_X));
          int32ToReal16(denom, &temp);
          real16Divide(POINTER_TO_REGISTER_DATA(REGISTER_X), &temp, POINTER_TO_REGISTER_DATA(REGISTER_X));
          int32ToReal16(integer, &temp);
          real16Add(POINTER_TO_REGISTER_DATA(REGISTER_X), &temp, POINTER_TO_REGISTER_DATA(REGISTER_X));
          if(nimBuffer[0] == '-')
           real16SetNegativeSign(POINTER_TO_REGISTER_DATA(REGISTER_X));

          displayRealAsFraction = true;
          if(integer != 0) {
            fractionType = FT_PROPER;
          }
          else {
            fractionType = FT_IMPROPER;
          }
        }
        else if(nimNumberPart == NP_COMPLEX_INT_PART || nimNumberPart == NP_COMPLEX_FLOAT_PART || nimNumberPart == NP_COMPLEX_EXPONENT) {
          int16_t imaginarySign;

          reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, 0);

          if(nimBuffer[imaginaryMantissaSignLocation] == '+') {
            imaginarySign = 1;
          }
          else {
            imaginarySign = -1;
          }
          nimBuffer[imaginaryMantissaSignLocation] = 0;

          stringToReal16(nimBuffer, POINTER_TO_REGISTER_DATA(REGISTER_X));
          if(real16IsInfinite(POINTER_TO_REGISTER_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
            displayCalcErrorMessage(1, REGISTER_T, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              showInfoDialog("In function closeNIM:", "the absolute value of the real part of a complex must be less than 10^385!", "Unless D flag (Danger) is set.", NULL);
            #endif
          }

          stringToReal16(nimBuffer + imaginaryMantissaSignLocation + 2, POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL16_SIZE);
          if(imaginarySign == -1) {
            real16SetNegativeSign(POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL16_SIZE);
          }
          if(real16IsInfinite(POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL16_SIZE) && !getFlag(FLAG_DANGER)) {
            displayCalcErrorMessage(1, REGISTER_T, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              showInfoDialog("In function closeNIM:", "the absolute value of the imaginary part of a complex must be less than 10^385!", "Unless D flag (Danger) is set.", NULL);
            #endif
          }

          if(complexMode == CM_POLAR) {
            if(real16CompareEqual(REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), const16_0)) {
              real16Zero(COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
            }
            else {
              real16_t magnitude16, theta16;

              real16Copy(POINTER_TO_REGISTER_DATA(REGISTER_X), &magnitude16);
              real16Copy(COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), &theta16);
              if(real16CompareLessThan(&magnitude16, const16_0)) {
                real16SetPositiveSign(&magnitude16);
                real16Add(&theta16, const16_pi, &theta16);
                real16Remainder(&theta16, const16_2pi, &theta16);
              }
              convertAngle16FromTo(&theta16, angularMode, AM_RADIAN);
              real16PolarToRectangular(&magnitude16, &theta16, REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
            }
          }
        }
        else {
          sprintf(errorMessage, "In function closeNIM: %d is an unexpected nimNumberPart value!", nimNumberPart);
          displayBugScreen(errorMessage);
        }
      }
    }
  }

  refreshRegisterLine(NIM_REGISTER_LINE);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("closeNim");
  #endif
}
