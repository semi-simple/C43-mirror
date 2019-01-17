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
 * \file keyboard.c Keyboard management
 ***********************************************/

#include "wp43s.h"

/********************************************//**
 * \brief Displays the f or g shift state in the
 * upper left corner of the T register line
 *
 * \param void
 * \return void
 ***********************************************/
void showShiftState(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("showShiftState");
  #endif

  if(shiftF) {
    showGlyph(NUM_SUP_f, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // f 4+8+3 is pixel wide
  }
  else if(shiftG) {
    showGlyph(NUM_SUP_g, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // g 4+10+1 is pixel wide
  }
  else {
    refreshRegisterLine(REGISTER_T);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("showShiftState");
  #endif
}


/********************************************//**
 * \brief Displays the function of the
 * currently pressed button in the
 * upper left corner of the T register line
 *
 * \param void
 * \return void
 ***********************************************/
void showFunctionName(int16_t item) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("showFunctionName");
  #endif

  showString(indexOfItems[item].itemName, &standardFont, 1, 134 - 37*(REGISTER_T-100), vmNormal, false, false);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("showFunctionName");
  #endif
}



/********************************************//**
 * \brief Resets shift keys status and clears the
 * corresponding area on the screen
 *
 * \param void
 * \return void
 *
 ***********************************************/
void resetShiftState(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("resetShiftState");
  #endif

  shiftF = false;
  shiftG = false;
  showShiftState();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("resetShiftState");
  #endif
}



/********************************************//**
 * \brief Executes one function from a softmenu
 *
 * \param[in] fn int16_t    Function key from 1 to 6
 * \param[in] shift int16_t Shift status
 *                          * 0 = not shifted
 *                          * 1 = f shifted
 *                          * 2 = g shifted
 * \return void
 ***********************************************/
void executeFunction(int16_t fn, int16_t shift) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("executeFunction");
  #endif

  int16_t    row, func;
  const softmenu_t *sm;

  if(softmenuStackPointer > 0) {
    sm = &softmenu[softmenuStack[softmenuStackPointer - 1].softmenu];
    row = min(3, sm->numRows - softmenuStack[softmenuStackPointer - 1].row) - 1;

    if(shift <= row) {
      func = (sm->softkeyRow)[6*(softmenuStack[softmenuStackPointer - 1].row + shift) + (fn-1)];
      if(func < 0) { // softmenu
        showSoftmenu(NULL, func, true);
      }
      else if((calcMode == CM_NORMAL || calcMode == CM_NIM) && (CHR_0<=func && func<=CHR_F)) {
        addItemToNimBuffer(func);
      }
      else if(func > 0) { // function
        if(calcMode == CM_NIM) {
          closeNim();
        }

        if(lastErrorCode == 0) {
          temporaryInformation = TI_NO_INFO;
          runFunction(func);
        }
      }
    }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("executeFunction");
  #endif
}


/********************************************//**
 * \brief One of the function keys was clicked
 *
 * \param w GtkWidget* The clicked button
 * \param data gpointer String containing the key ID
 * \return void
 ***********************************************/
#ifdef PC_BUILD
void btnFnClicked(GtkWidget *w, gpointer data) {
#endif
#ifdef DMCP_BUILD
void btnFnClicked(void *w, void *data) {
#endif
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("btnFnClicked");
  #endif

  int16_t fn = *((char *)data) - '0';

  if(calcMode != CM_CONFIRMATION) {
    allowScreenUpdate = true;

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
      refreshStack();
    }

    if(softmenuStackPointer > 0) {
      if(shiftF) {
        resetShiftState();
        executeFunction(fn, 1);
      }
      else if(shiftG) {
        resetShiftState();
        executeFunction(fn, 2);
      }
      else {
        executeFunction(fn, 0);
      }
    }
    else {
      resetShiftState();
    }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("btnFnClicked");
  #endif
}


uint16_t determineItem(const calcKey_t *key) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("determineItem");
  #endif

  uint16_t result;
  if(calcMode == CM_NORMAL || calcMode == CM_NIM || calcMode == CM_FONT_BROWSER || calcMode == CM_FLAG_BROWSER || calcMode == CM_REGISTER_BROWSER || calcMode == CM_BUG_ON_SCREEN || calcMode == CM_CONFIRMATION) {
    result = shiftF ? key->fShifted :
             shiftG ? key->gShifted :
                      key->primary;
  }
  else if(calcMode == CM_AIM) {
    result = shiftF ? key->fShiftedAim :
             shiftG ? key->gShiftedAim :
                      key->primaryAim;

  }
  else if(calcMode == CM_TAM) {
    result = key->primaryTam; // No shifted function in TAM
  }
  else {
    displayBugScreen("In function determineItem: item was not determined!");
    result=0;
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("determineItem");
  #endif
  return result;
}


/********************************************//**
 * \brief Simulate a button click.
 *
 * \param w GtkWidget* The button to pass to btnPressed and btnReleased
 * \param data gpointer String containing the key ID
 * \return void
 ***********************************************/
#ifdef PC_BUILD
void btnClicked(GtkWidget *w, gpointer data) {
#endif
#ifdef DMCP_BUILD
void btnClicked(void *w, void *data) {
#endif
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("btnClicked");
  #endif

  btnPressed(w, data);
  btnReleased(w, data);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("btnClicked");
  #endif
}


/********************************************//**
 * \brief A calc button was pressed
 *
 * \param w GtkWidget*
 * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
 * \return void
 ***********************************************/
#ifdef PC_BUILD
void btnPressed(GtkWidget *notUsed, gpointer data) {
#endif
#ifdef DMCP_BUILD
void btnPressed(void *notUsed, void *data) {
#endif
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("btnPressed");
  #endif

  const calcKey_t *key;

  keyHoldFunction = 0;

  key = userModeEnabled ? (kbd_usr + (*((char *)data) - '0')*10 + *(((char *)data)+1) - '0') : (kbd_std + (*((char *)data) - '0')*10 + *(((char *)data)+1) - '0');

  allowScreenUpdate = true;


  // Shift f pressed
  if(key->primary == KEY_f && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_NIM)) {
    if(temporaryInformation != TI_NO_INFO) {
      temporaryInformation = TI_NO_INFO;
      refreshRegisterLine(REGISTER_X);
      refreshRegisterLine(REGISTER_Y);
    }

    if(shiftF) {
      shiftF = false;
      if(lastErrorCode != 0) {
        lastErrorCode = 0;
        refreshStack();
      }
    }
    else {
      shiftF = true;
    }

    shiftG = false;

    showShiftState();
  }

  // Shift g pressed
  else if(key->primary == KEY_g && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_NIM)) {
    if(temporaryInformation != TI_NO_INFO) {
      temporaryInformation = TI_NO_INFO;
      refreshRegisterLine(REGISTER_X);
      refreshRegisterLine(REGISTER_Y);
    }

    if(shiftG) {
      shiftG = false;
      if(lastErrorCode != 0) {
        lastErrorCode = 0;
        refreshStack();
      }
    }
    else {
      shiftG = true;
    }

    shiftF = false;

    showShiftState();
  }

  else {
    int16_t item = determineItem(key);
    resetShiftState();

    if(lastErrorCode != 0 && item != KEY_EXIT && item != KEY_BACKSPACE) {
      lastErrorCode = 0;
      refreshStack();
    }

    if(temporaryInformation != TI_NO_INFO && temporaryInformation != TI_ARE_YOU_SURE) {
      temporaryInformation = TI_NO_INFO;
      refreshRegisterLine(REGISTER_X);
      refreshRegisterLine(REGISTER_Y);
    }

    if(item == ITM_ENTER) {
      if(calcMode == CM_NIM) {
        addItemToNimBuffer(ITM_ENTER);
      }

      else if(calcMode == CM_AIM) {
        uint16_t mem;

        calcModeNormal();
        showAlphaMode();
        popSoftmenu();

        mem = stringByteLength(aimBuffer) + 3u; // +3 = 2 bytes for the data length and 1 for the terminating \0
        if(mem%2 == 1) {
          mem++;
        }

        liftStack(dtString, mem);

        *(uint16_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = mem - 2;
        memcpy(POINTER_TO_REGISTER_STRING(REGISTER_X), aimBuffer, stringByteLength(aimBuffer) + 1u);

        #if (STACK_LIFT_DEBUG == 1)
          stackLiftEnable();
        #else
          stackLiftEnabled = true;
        #endif

        liftStack(dtString, mem);

        *(uint16_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = mem - 2;
        memcpy(POINTER_TO_REGISTER_STRING(REGISTER_X), aimBuffer, stringByteLength(aimBuffer) + 1u);
        aimBuffer[0] = 0;

        #if (STACK_LIFT_DEBUG == 1)
          stackLiftDisable();
        #else
          stackLiftEnabled = false;
        #endif

        refreshStack();
      }

      else if(calcMode == CM_TAM) {
        addItemToBuffer(ITM_ENTER);
      }

      else if(calcMode == CM_NORMAL) {
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
      }

      else if(calcMode == CM_FONT_BROWSER) {
      }

      else if(calcMode == CM_FLAG_BROWSER) {
      }

      else if(calcMode == CM_CONFIRMATION) {
        calcMode = previousCalcMode;
        confirmedFunction(CONFIRMED);
        refreshStack();
      }

      else {
        displayBugScreen("In function btnPressed: unexpected case while processing key ENTER!");
      }
    }

    else if(item == KEY_EXIT) {
      if(calcMode == CM_NORMAL) {
        if(lastErrorCode != 0) {
          lastErrorCode = 0;
          refreshStack();
        }
        else if(softmenuStackPointer > 0) {
          popSoftmenu();
        }
      }

      else if(calcMode == CM_AIM) {
        if(softmenuStack[softmenuStackPointer-1].softmenu == MY_ALPHA_MENU) {
          uint16_t mem;

          calcModeNormal();
          showAlphaMode();
          popSoftmenu();

          mem = stringByteLength(aimBuffer) + 3u; // +3 = 2 bytes for the data length and 1 for the terminating \0
          if(mem%2 == 1) {
            mem++;
          }

          liftStack(dtString, mem);

          *(uint16_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = mem - 2;
          memcpy(POINTER_TO_REGISTER_STRING(REGISTER_X), aimBuffer, stringByteLength(aimBuffer) + 1u);

          #if (STACK_LIFT_DEBUG == 1)
            stackLiftEnable();
          #else
            stackLiftEnabled = true;
          #endif

          refreshStack();
        }
        else {
         popSoftmenu();
        }
      }

      else if(calcMode == CM_TAM) {
        calcModeNormal();
        refreshStack();
      }

      else if(calcMode == CM_NIM) {
        addItemToNimBuffer(KEY_EXIT);
      }

      else if(calcMode == CM_FLAG_BROWSER || calcMode == CM_FONT_BROWSER || calcMode == CM_REGISTER_BROWSER) {
        calcMode = previousCalcMode;
        clearScreen(false, true, true);
        refreshStack();
        showSoftmenuCurrentPart();
        currentFlgScr = 0;
        currentFntScr = 0;
        currentRegisterBrowserScreen = 9999;
        oldTime[0] = 0;
        showDateTime();
      }

      else if(calcMode == CM_BUG_ON_SCREEN) {
        //displayDateTime = true;
        calcMode = previousCalcMode;
        clearScreen(false, true, true);
        refreshStack();
        showSoftmenuCurrentPart();
      }

      else if(calcMode == CM_CONFIRMATION) {
        calcMode = previousCalcMode;
        temporaryInformation = TI_NO_INFO;
        refreshStack();
      }

      else {
       displayBugScreen("In function btnPressed: unexpected case while processing key EXIT!");
      }
    }

    else if(item == KEY_CC) {
      if(calcMode == CM_NORMAL) {
        uint32_t dataTypeX = getRegisterDataType(REGISTER_X);
        uint32_t dataTypeY = getRegisterDataType(REGISTER_Y);

        if(   (dataTypeX == dtReal16 || dataTypeX == dtReal34 || dataTypeX == dtBigInteger)
           && (dataTypeY == dtReal16 || dataTypeY == dtReal34 || dataTypeY == dtBigInteger)) {
          copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
          fnSetFlag(FLAG_CPXRES);

          if(dataTypeX == dtBigInteger) {
            if(dataTypeY == dtReal16 || dataTypeY == dtBigInteger) {
              convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
              dataTypeX = dtReal16;
            }
            else { // dataTypeY == dtReal34
              convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
              dataTypeX = dtReal34;
            }
          }

          if(dataTypeY == dtBigInteger) {
            if(dataTypeX == dtReal16) {
              convertBigIntegerRegisterToReal16Register(REGISTER_Y, REGISTER_Y);
              dataTypeY = dtReal16;
            }
            else { // dataTypeX == dtReal34
              convertBigIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
              dataTypeY = dtReal34;
            }
          }

          if(dataTypeX == dtReal16 && dataTypeY == dtReal34) {
            real34_t temp;

            real16ToReal34(POINTER_TO_REGISTER_DATA(REGISTER_X), &temp);
            reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, 0);
            real34Copy(&temp, POINTER_TO_REGISTER_DATA(REGISTER_X));
            dataTypeX = dtReal34;
          }

          if(dataTypeY == dtReal16 && dataTypeX == dtReal34) {
            real34_t temp;

            real16ToReal34(POINTER_TO_REGISTER_DATA(REGISTER_Y), &temp);
            reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, 0);
            real34Copy(&temp, POINTER_TO_REGISTER_DATA(REGISTER_Y));
          }

          if(dataTypeX == dtReal16) {
            complex16_t temp;

            real16Copy(POINTER_TO_REGISTER_DATA(REGISTER_X), &temp);
            real16Copy(POINTER_TO_REGISTER_DATA(REGISTER_Y), COMPLEX16_IMAGINARY_PART_POINTER(&temp));
            reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, 0);

            if(complexMode == CM_POLAR) {
              convertAngle16FromTo(COMPLEX16_IMAGINARY_PART_POINTER(&temp), angularMode, AM_RADIAN);
              real16PolarToRectangular(REAL16_POINTER(&temp), COMPLEX16_IMAGINARY_PART_POINTER(&temp), REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
            }
            else {
              complex16Copy(&temp, POINTER_TO_REGISTER_DATA(REGISTER_X));
            }
          }
          else { //dataTypeX == dtReal34
            complex34_t temp;

            real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_X), &temp);
            real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_Y), COMPLEX34_IMAGINARY_PART_POINTER(&temp));
            reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, 0);

            if(complexMode == CM_POLAR) {
              convertAngle34FromTo(COMPLEX34_IMAGINARY_PART_POINTER(&temp), angularMode, AM_RADIAN);
              real34PolarToRectangular(REAL34_POINTER(&temp), COMPLEX34_IMAGINARY_PART_POINTER(&temp), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
            }
            else {
              complex34Copy(&temp, POINTER_TO_REGISTER_DATA(REGISTER_X));
            }
          }

          fnDropY(NOPARAM);
          refreshStack();
        }

        else if(dataTypeX == dtComplex16) {
          copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
          reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, 0);

          #if (STACK_LIFT_DEBUG == 1)
            stackLiftEnable();
          #else
            stackLiftEnabled = true;
          #endif
          if(complexMode == CM_RECTANGULAR) {
            real16Copy(COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_L)), POINTER_TO_REGISTER_DATA(REGISTER_X));
            liftStack(dtReal16, REAL16_SIZE);
            real16Copy(POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_X));
          }
          else { // CM_POLAR mode
            liftStack(dtReal16, REAL16_SIZE);
            real16RectangularToPolar(REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_L)), COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_L)), REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_Y)));
            temporaryInformation = TI_RADIUS_THETA;
          }

          refreshStack();
        }

        else if(dataTypeX == dtComplex34) {
          copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
          reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, 0);

          #if (STACK_LIFT_DEBUG == 1)
            stackLiftEnable();
          #else
            stackLiftEnabled = true;
          #endif
          if(complexMode == CM_RECTANGULAR) {
            real34Copy(COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_L)), POINTER_TO_REGISTER_DATA(REGISTER_X));
            liftStack(dtReal34, REAL34_SIZE);
            real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_X));
          }
          else { // CM_POLAR mode
            liftStack(dtReal34, REAL34_SIZE);
            real34RectangularToPolar(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_L)), COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_L)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_Y)));
            temporaryInformation = TI_RADIUS_THETA;
          }

          refreshStack();
        }

        else {
          displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "You cannot use CC with %s in X and %s in Y!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false), getDataTypeName(getRegisterDataType(REGISTER_Y), true, false));
            showInfoDialog("In function btnPressed:", errorMessage, NULL, NULL);
          #endif
        }
      }

      else if(calcMode == CM_NIM) {
        addItemToNimBuffer(KEY_CC);
      }

      else {
        sprintf(errorMessage, "In function btnPressed: %" FMT8U " is an unexpected value for calcMode while processing CC function (complex closing, composing, cutting, & converting)!", calcMode);
        displayBugScreen(errorMessage);
      }
    }

    else if(item == KEY_BACKSPACE) {
      if(calcMode == CM_NORMAL) {
        if(lastErrorCode != 0) {
          lastErrorCode = 0;
          refreshStack();
        }
        else {
          fnClX(NOPARAM);

          #if (STACK_LIFT_DEBUG == 1)
            stackLiftDisable();
          #else
            stackLiftEnabled = false;
          #endif
        }
      }

      else if(calcMode == CM_AIM) {
        uint16_t lg, x, y, newXCursor;

        if(stringByteLength(aimBuffer) > 0) {
          lg = stringLastGlyph(aimBuffer);
          aimBuffer[lg] = 0;
          newXCursor = showString(aimBuffer, &standardFont, 1, Y_POSITION_OF_AIM_LINE+6, vmNormal, true, true);
          for(x=newXCursor; x<xCursor+6; x++) {
            for(y=Y_POSITION_OF_AIM_LINE+6; y<Y_POSITION_OF_AIM_LINE+26; y++) {
              clearPixel(x, y);
            }
          }
          xCursor = newXCursor;
        }
      }

      else if(calcMode == CM_NIM) {
        addItemToNimBuffer(KEY_BACKSPACE);
      }

      else if(calcMode == CM_TAM) {
        tamTransitionSystem(TT_BACKSPACE);
      }

      else if(calcMode == CM_REGISTER_BROWSER || calcMode == CM_FONT_BROWSER || calcMode == CM_FLAG_BROWSER) {
        calcMode = previousCalcMode;
        clearScreen(false, true, true);
        refreshStack();
        showSoftmenuCurrentPart();
        currentFlgScr = 0;
        currentFntScr = 0;
        currentRegisterBrowserScreen = 9999;
        oldTime[0] = 0;
        showDateTime();
      }

      else if(calcMode == CM_BUG_ON_SCREEN) {
        //displayDateTime = true;
        calcMode = previousCalcMode;
        clearScreen(false, true, true);
        refreshStack();
        showSoftmenuCurrentPart();
      }

      else if(calcMode == CM_CONFIRMATION) {
        calcMode = previousCalcMode;
        temporaryInformation = TI_NO_INFO;
        refreshStack();
      }

      else {
        displayBugScreen("In function btnPressed: unexpected case while processing key BACKSPACE!");
      }
    }

    else if(item == KEY_UP) {
      if(calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_NIM) {
        if(softmenuStackPointer > 0  && softmenuStack[softmenuStackPointer - 1].softmenu != MY_ALPHA_MENU) {
          if((softmenuStack[softmenuStackPointer - 1].row + 3) < softmenu[softmenuStack[softmenuStackPointer-1].softmenu].numRows) {
            softmenuStack[softmenuStackPointer - 1].row += 3;
            showSoftmenuCurrentPart();
          }
          else {
            softmenuStack[softmenuStackPointer - 1].row = 0;
            showSoftmenuCurrentPart();
          }
        }
        else if(calcMode == CM_AIM) {
          alphaCase = AC_UPPER;
          showAlphaMode();
        }
      }

      else if(calcMode == CM_TAM) {
        addItemToBuffer(ITM_Max);
      }

      else if(calcMode == CM_REGISTER_BROWSER) {
        if(rbrMode == RBR_GLOBAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen + 1, FIRST_LOCAL_REGISTER);
          registerBrowser(NOPARAM);
        }
        else if(rbrMode == RBR_LOCAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_LOCAL_REGISTER + 1, numberOfLocalRegisters) + FIRST_LOCAL_REGISTER;
          registerBrowser(NOPARAM);
        }
        else if(rbrMode == RBR_NAMED) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1000 + 1, numberOfNamedRegisters) + 1000;
          registerBrowser(NOPARAM);
        }
        else {
          sprintf(errorMessage, "In function btnPressed: unexpected case while processing key UP! %" FMT8U " is an unexpected value for rbrMode.", rbrMode);
          displayBugScreen(errorMessage);
        }
      }

      else if(calcMode == CM_FLAG_BROWSER) {
        currentFlgScr = 3 - currentFlgScr;
        flagBrowser(NOPARAM);
      }

      else if(calcMode == CM_FONT_BROWSER) {
        if(currentFntScr >= 2) {
          currentFntScr--;
          fontBrowser(NOPARAM);
        }
      }

      else {
       displayBugScreen("In function btnPressed: unexpected case while processing key UP!");
      }
    }

    else if(item == KEY_DOWN) {
      if(calcMode == CM_NORMAL || calcMode == CM_AIM) {
        if(softmenuStackPointer > 0 && softmenuStack[softmenuStackPointer - 1].softmenu != MY_ALPHA_MENU) {
          if((softmenuStack[softmenuStackPointer - 1].row - 3) >= 0) {
            softmenuStack[softmenuStackPointer - 1].row -= 3;
            showSoftmenuCurrentPart();
          }
          else {
            softmenuStack[softmenuStackPointer - 1].row = (softmenu[softmenuStack[softmenuStackPointer-1].softmenu].numRows - 1) / 3 * 3;
            showSoftmenuCurrentPart();
          }
        }
        else if(calcMode == CM_AIM) {
          alphaCase = AC_LOWER;
          showAlphaMode();
        }
      }

      else if(calcMode == CM_TAM) {
        addItemToBuffer(ITM_Min);
      }

      else if(calcMode == CM_REGISTER_BROWSER) {
        if(rbrMode == RBR_GLOBAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1, FIRST_LOCAL_REGISTER);
          registerBrowser(NOPARAM);
        }
        else if(rbrMode == RBR_LOCAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_LOCAL_REGISTER - 1, numberOfLocalRegisters) + FIRST_LOCAL_REGISTER;
          registerBrowser(NOPARAM);
        }
        else if(rbrMode == RBR_NAMED) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1000 - 1, numberOfNamedRegisters) + 1000;
          registerBrowser(NOPARAM);
        }
        else {
          sprintf(errorMessage, "In function btnPressed: unexpected case while processing key DOWN! %" FMT8U " is an unexpected value for rbrMode.", rbrMode);
          displayBugScreen(errorMessage);
        }
      }

      else if(calcMode == CM_FLAG_BROWSER) {
        currentFlgScr = 3 - currentFlgScr;
        flagBrowser(NOPARAM);
      }

      else if(calcMode == CM_FONT_BROWSER) {
        if(currentFntScr < numScreensNumericFont+numScreensStandardFont) {
          currentFntScr++;
          fontBrowser(NOPARAM);
        }
      }

      else {
        displayBugScreen("In function btnPressed: unexpected case while processing key DOWN!");
      }
    }

    else if(item == KEY_dotD) {
      if(calcMode == CM_NIM) {
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function btnPressed:", "the data type date is to be coded!", NULL, NULL);
        #endif
      }

      else if(displayRealAsFraction) {
        displayRealAsFraction = false;
        refreshStack();
      }

      else if(calcMode == CM_NORMAL) {
        if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
          convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
          refreshRegisterLine(REGISTER_X);
        }

        else if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
          convertSmallIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
          refreshRegisterLine(REGISTER_X);
        }

        else if(getRegisterDataType(REGISTER_X) == dtReal34) {
          convertRegister34To16(REGISTER_X);
          refreshRegisterLine(REGISTER_X);
        }

        else if(getRegisterDataType(REGISTER_X) != dtReal16) {
          displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "data type %s cannot be converted to a real16!", getRegisterDataTypeName(REGISTER_X, false, false));
            showInfoDialog("In function btnPressed:", errorMessage, NULL, NULL);
          #endif
        }
      }

      else {
        displayBugScreen("In function btnPressed: unexpected case while processing .d function!");
      }
    }

    else if(calcMode == CM_NORMAL) {
      if(item < 0) {
        showSoftmenu(NULL, item, false);
      }
      else if(item == ITM_EXPONENT || item==CHR_PERIOD || (CHR_0<=item && item<=CHR_9)) {
        addItemToNimBuffer(item);
      }
      else {
        showFunctionName(item);
        keyHoldFunction=item;
      }
    }

    else if(calcMode == CM_AIM) {
      if(item < 0) {
        showSoftmenu(NULL, item, false);
      }

      else if(alphaCase==AC_LOWER && (CHR_A<=item && item<=CHR_Z)) {
        addItemToBuffer(item + 26);
      }

      else if(alphaCase==AC_LOWER && (CHR_ALPHA<=item && item<=CHR_OMEGA)) {
        addItemToBuffer(item + 36);
      }

      else if(item == CHR_DOWN_ARROW) {
        nextChar = NC_SUBSCRIPT;
      }

      else if(item == CHR_UP_ARROW) {
        nextChar = NC_SUPERSCRIPT;
      }

      else {
        showFunctionName(item);
        keyHoldFunction=item;
      }
    }

    else if(calcMode == CM_TAM) {
      addItemToBuffer(item);
    }

    else if(calcMode == CM_NIM) {
      if(item < 0) {
        showSoftmenu(NULL, item, false);
      }
      else {
        addItemToNimBuffer(item);
      }
    }

    else if(calcMode == CM_REGISTER_BROWSER) {
      if(item == CHR_PERIOD) {
        if(rbrMode == RBR_GLOBAL) {
          rbrMode = RBR_LOCAL;
          currentRegisterBrowserScreen = FIRST_LOCAL_REGISTER;
        }
        else if(rbrMode == RBR_LOCAL) {
          rbrMode = RBR_GLOBAL;
          currentRegisterBrowserScreen = REGISTER_X;
        }
        else if(rbrMode == RBR_NAMED) {
          rbrMode = RBR_LOCAL;
          currentRegisterBrowserScreen = FIRST_LOCAL_REGISTER;
        }
        registerBrowser(NOPARAM);
      }
      if(item == ITM_RS) {
        if(rbrMode == RBR_GLOBAL) {
          rbrMode = RBR_NAMED;
          currentRegisterBrowserScreen = 1000;
        }
        else if(rbrMode == RBR_LOCAL) {
          rbrMode = RBR_NAMED;
          currentRegisterBrowserScreen = 1000;
        }
        else if(rbrMode == RBR_NAMED) {
          rbrMode = RBR_GLOBAL;
          currentRegisterBrowserScreen = REGISTER_X;
        }
        registerBrowser(NOPARAM);
      }

      else if(item == ITM_RCL) {
        if(rbrMode == RBR_GLOBAL || rbrMode == RBR_LOCAL) {
          calcMode = previousCalcMode;
          currentFlgScr = 0;
          currentFntScr = 0;
          oldTime[0] = 0;
          showDateTime();
          clearScreen(false, true, true);
          fnRecall(currentRegisterBrowserScreen);
          #if (STACK_LIFT_DEBUG == 1)
            stackLiftEnable();
          #else
            stackLiftEnabled = true;
          #endif
          refreshStack();
          currentRegisterBrowserScreen = 9999;
        }
        else if(rbrMode == RBR_NAMED) {
        }
      }
      else if(item == ITM_STO) {
        showContent = !showContent;
        registerBrowser(NOPARAM);
      }
    }

    else if(calcMode == CM_FLAG_BROWSER) {
    }

    else if(calcMode == CM_FONT_BROWSER) {
    }

    else if(calcMode == CM_ERROR_MESSAGE) {
    }

    else if(calcMode == CM_BUG_ON_SCREEN) {
    }

    else if(calcMode == CM_CONFIRMATION) {
      if(item == CHR_3 || item == ITM_XEQ) { // Yes or XEQ
        calcMode = previousCalcMode;
        temporaryInformation = TI_NO_INFO;
        confirmedFunction(CONFIRMED);
        refreshStack();
      }

      else if(item == CHR_7) { // No
        calcMode = previousCalcMode;
        temporaryInformation = TI_NO_INFO;
        refreshStack();
      }
    }

    else {
      sprintf(errorMessage, "In function btnPressed: %" FMT8U " is an unexpected value while processing calcMode!", calcMode);
      displayBugScreen(errorMessage);
    }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("btnPressed");
  #endif
}


/********************************************//**
 * \brief A calc button was released
 *
 * \param w GtkWidget*
 * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
 * \return void
 ***********************************************/
#ifdef PC_BUILD
void btnReleased(GtkWidget *notUsed, gpointer data) {
#endif
#ifdef DMCP_BUILD
void btnReleased(void *notUsed, void *data) {
#endif
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("btnReleased");
  #endif

  if(keyHoldFunction) {
    refreshStack();
    runFunction(keyHoldFunction);
    keyHoldFunction=0;
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("btnReleased");
  #endif
}
