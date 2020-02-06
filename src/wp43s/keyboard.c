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

#ifndef TESTSUITE_BUILD
/********************************************//**
 * \brief Displays the f or g shift state in the
 * upper left corner of the T register line
 *
 * \param void
 * \return void
 ***********************************************/
void showShiftState(void) {
  if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FONT_BROWSER) {
    if(shiftStateChanged) {
      if(shiftF) {
        showGlyph(STD_SUP_f, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // f is pixel 4+8+3 wide
      }
      else if(shiftG) {
        showGlyph(STD_SUP_g, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // g is pixel 4+10+1 wide
      }
      else {
        refreshRegisterLine(REGISTER_T);

        if(TAM_REGISTER_LINE == REGISTER_T && (calcMode == CM_TAM || calcMode == CM_ASM)) {
          showString(tamBuffer, &standardFont, 25, Y_POSITION_OF_TAM_LINE + 6, vmNormal, true, true);
        }
      }

      shiftStateChanged = false;
    }
  }
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
  if(shiftF) {
    shiftF = false;
    shiftStateChanged = true;
  }

  if(shiftG) {
    shiftG = false;
    shiftStateChanged = true;
  }

  showShiftState();
}



/********************************************//**
 * \brief Executes one function from a softmenu
 *
 * \param[in] fn int16_t    Function key from 1 to 6
 * \param[in] itemShift int16_t Shift status
 *                          *  0 = not shifted
 *                          *  6 = f shifted
 *                          * 12 = g shifted
 * \return void
 ***********************************************/
void executeFunction(int16_t fn, int16_t itemShift) {
  int16_t row, func;
  const softmenu_t *sm;

  if(softmenuStackPointer > 0) {
    sm = &softmenu[softmenuStack[softmenuStackPointer - 1].softmenu];
    row = min(3, (sm->numItems + modulo(softmenuStack[softmenuStackPointer - 1].firstItem - sm->numItems, 6))/6 - softmenuStack[softmenuStackPointer - 1].firstItem/6) - 1;

    if(itemShift/6 <= row && softmenuStack[softmenuStackPointer - 1].firstItem + itemShift + (fn - 1) < sm->numItems) {
      func = (sm->softkeyItem)[softmenuStack[softmenuStackPointer - 1].firstItem + itemShift + (fn - 1)];

      if(func == CHR_PROD_SIGN) {
        func = (productSign == PS_CROSS ? CHR_DOT : CHR_CROSS);
      }

      if(func < 0) { // softmenu
        showSoftmenu(NULL, func, true);
      }
      else if((calcMode == CM_NORMAL || calcMode == CM_NIM) && (CHR_0<=func && func<=CHR_F)) {
        addItemToNimBuffer(func);
      }
      else if(calcMode == CM_TAM) {
        addItemToBuffer(func);
      }
      else if(func > 0) { // function
        if(calcMode == CM_NIM && func != KEY_CC) {
          closeNim();
          if(calcMode != CM_NIM) {
            if(indexOfItems[func % 10000].func == fnConstant) {
              STACK_LIFT_ENABLE;
            }
          }
        }

        if(lastErrorCode == 0) {
          resetTemporaryInformation();
          runFunction(func % 10000);
        }
      }
    }
  }
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
  int16_t fn = *((char *)data) - '0';

  if(calcMode != CM_CONFIRMATION) {
    allowScreenUpdate = true;

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
      refreshStack();
    }

    if(softmenuStackPointer > 0) {
      if(calcMode == CM_ASM) {
        calcModeNormal();
      }

      if(shiftF) {
        resetShiftState();
        executeFunction(fn,  6);
      }
      else if(shiftG) {
        resetShiftState();
        executeFunction(fn, 12);
      }
      else {
        executeFunction(fn, 0);
      }
    }
    else {
      resetShiftState();
    }
  }
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
  btnPressed(w, data);
  btnReleased(w, data);
}



uint16_t determineItem(const calcKey_t *key) {
  uint16_t result;
  if(calcMode == CM_NORMAL || calcMode == CM_NIM || calcMode == CM_FONT_BROWSER || calcMode == CM_FLAG_BROWSER || calcMode == CM_REGISTER_BROWSER || calcMode == CM_BUG_ON_SCREEN || calcMode == CM_CONFIRMATION) {
    result = shiftF ? key->fShifted :
             shiftG ? key->gShifted :
                      key->primary;
  }
  else if(calcMode == CM_AIM || calcMode == CM_ASM) {
    result = shiftF ? key->fShiftedAim :
             shiftG ? key->gShiftedAim :
                      key->primaryAim;

  }
  else if(calcMode == CM_TAM) {
    result = key->primaryTam; // No shifted function in TAM
  }
  else {
    displayBugScreen("In function determineItem: item was not determined!");
    result = 0;
  }
  return result;
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
  const calcKey_t *key;
  int16_t itemShift;

  key = userModeEnabled ? (kbd_usr + (*((char *)data) - '0')*10 + *(((char *)data)+1) - '0') : (kbd_std + (*((char *)data) - '0')*10 + *(((char *)data)+1) - '0');

  allowScreenUpdate = true;


  // Shift f pressed and shift g not active
  if(key->primary == KEY_f && !shiftG && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_NIM || calcMode == CM_ASM)) {
    resetTemporaryInformation();

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
      refreshStack();
    }

    shiftF = !shiftF;
    shiftStateChanged = true;

    showShiftState();
  }

  // Shift g pressed and shift f not active
  else if(key->primary == KEY_g && !shiftF && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_NIM || calcMode == CM_ASM)) {
    resetTemporaryInformation();

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
      refreshStack();
    }

    shiftG = !shiftG;
    shiftStateChanged = true;

    showShiftState();
  }

  else {
    int16_t item = determineItem(key);

    if(item == CHR_PROD_SIGN) {
      item = (productSign == PS_DOT ? CHR_DOT : CHR_CROSS);
    }

    resetShiftState();

    if(lastErrorCode != 0 && item != KEY_EXIT && item != KEY_BACKSPACE) {
      lastErrorCode = 0;
      refreshStack();
    }

    resetTemporaryInformation();

    if(item == ITM_ENTER) {
      fnEnter(NOPARAM);
    }

    else if(item == KEY_EXIT) {

      if(calcMode == CM_BUG_ON_SCREEN) {
        calcMode = previousCalcMode;
        clearScreen(false, true, true);
        refreshStack();
        showSoftmenuCurrentPart();
      }

      else if(calcMode == CM_NORMAL) {
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
          calcModeNormal();
          popSoftmenu();

          if(aimBuffer[0] == 0) {
            restoreStack();
          }
          else {
            int16_t len = stringByteLength(aimBuffer);

            reallocateRegister(REGISTER_X, dtString, len, AM_NONE);

            memcpy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, len + 1);
            aimBuffer[0] = 0;

            STACK_LIFT_ENABLE;
          }

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

      else if(calcMode == CM_ASM) {
        calcModeNormal();
      }

      else if(calcMode == CM_NIM) {
        addItemToNimBuffer(KEY_EXIT);
      }

      else if(calcMode == CM_FLAG_BROWSER || calcMode == CM_FONT_BROWSER || calcMode == CM_REGISTER_BROWSER) {
        rbr1stDigit = true;
        calcMode = previousCalcMode;
        clearScreen(false, true, true);
        refreshStack();
        showSoftmenuCurrentPart();
        oldTime[0] = 0;
        showDateTime();
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

        if(   (dataTypeX == dtReal34 || dataTypeX == dtLongInteger)
           && (dataTypeY == dtReal34 || dataTypeY == dtLongInteger)) {
          showFunctionName(ITM_REtoCX, 10);
        }
        else if(dataTypeX == dtComplex34) {
          showFunctionName(ITM_CXtoRE, 10);
        }
        else {
          displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "You cannot use CC with %s in X and %s in Y!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false), getDataTypeName(getRegisterDataType(REGISTER_Y), true, false));
            showInfoDialog("In function btnPressed:", errorMessage, NULL, NULL);
          #endif
        }
      }

      else if(calcMode == CM_NIM) {
        addItemToNimBuffer(KEY_CC);
      }

      else if(calcMode == CM_FLAG_BROWSER || calcMode == CM_FONT_BROWSER || calcMode == CM_REGISTER_BROWSER) {
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
          showFunctionName(ITM_CLX, 10);
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

      else if(calcMode == CM_ASM) {
        addItemToBuffer(KEY_BACKSPACE);
      }

      else if(calcMode == CM_REGISTER_BROWSER || calcMode == CM_FONT_BROWSER || calcMode == CM_FLAG_BROWSER) {
        calcMode = previousCalcMode;
        clearScreen(false, true, true);
        refreshStack();
        showSoftmenuCurrentPart();
        oldTime[0] = 0;
        showDateTime();
      }

      else if(calcMode == CM_BUG_ON_SCREEN) {
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
      if(calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_NIM || calcMode == CM_ASM) {
        resetAlphaSelectionBuffer();
        if(softmenuStackPointer > 0  && softmenuStack[softmenuStackPointer - 1].softmenu != MY_ALPHA_MENU) {
          int16_t sm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
          if((sm == -MNU_alpha_omega || sm == -MNU_ALPHAintl) && alphaCase == AC_LOWER) {
            alphaCase = AC_UPPER;
            showAlphaMode();
            softmenuStack[softmenuStackPointer - 1].softmenu--; // Switch to the upper case menu
            showSoftmenuCurrentPart();
          }
          else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH) && alphaCase == AC_LOWER) {
            alphaCase = AC_UPPER;
            showAlphaMode();
          }
          else {
            itemShift = alphaSelectionMenu == ASM_NONE ? 18 : 6;

            if((softmenuStack[softmenuStackPointer - 1].firstItem + itemShift) < softmenu[softmenuStack[softmenuStackPointer-1].softmenu].numItems) {
              softmenuStack[softmenuStackPointer - 1].firstItem += itemShift;
              showSoftmenuCurrentPart();
            }
            else {
              softmenuStack[softmenuStackPointer - 1].firstItem = 0;
              showSoftmenuCurrentPart();
            }

            setCatalogLastPos();
          }
        }
        else {
          if(alphaCase != AC_UPPER) {
            alphaCase = AC_UPPER;
            showAlphaMode();
          }
        }
      }

      else if(calcMode == CM_TAM) {
        addItemToBuffer(ITM_Max);
      }

      else if(calcMode == CM_REGISTER_BROWSER) {
        rbr1stDigit = true;
        if(rbrMode == RBR_GLOBAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen + 1, FIRST_LOCAL_REGISTER);
          registerBrowser(NOPARAM);
        }
        else if(rbrMode == RBR_LOCAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_LOCAL_REGISTER + 1, numberOfLocalRegisters) + FIRST_LOCAL_REGISTER;
          registerBrowser(NOPARAM);
        }
        else if(rbrMode == RBR_NAMED) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1000 + 1, numberOfNamedVariables) + 1000;
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
        }
        fontBrowser(NOPARAM);
      }

      else {
       displayBugScreen("In function btnPressed: unexpected case while processing key UP!");
      }
    }

    else if(item == KEY_DOWN) {
      if(calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_NIM || calcMode == CM_ASM) {
        resetAlphaSelectionBuffer();
        if(softmenuStackPointer > 0  && softmenuStack[softmenuStackPointer - 1].softmenu != MY_ALPHA_MENU) {
          int16_t sm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
          if((sm == -MNU_ALPHA_OMEGA || sm == -MNU_ALPHAINTL) && alphaCase == AC_UPPER) {
            alphaCase = AC_LOWER;
            showAlphaMode();
            softmenuStack[softmenuStackPointer - 1].softmenu++; // Switch to the lower case menu
            showSoftmenuCurrentPart();
          }
          else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH) && alphaCase == AC_UPPER) {
            alphaCase = AC_LOWER;
            showAlphaMode();
          }
          else {
            itemShift = alphaSelectionMenu == ASM_NONE ? 18 : 6;

            if((softmenuStack[softmenuStackPointer - 1].firstItem - itemShift) >= 0) {
              softmenuStack[softmenuStackPointer - 1].firstItem -= itemShift;
            }
            else if((softmenuStack[softmenuStackPointer - 1].firstItem - itemShift) >= -5) {
              softmenuStack[softmenuStackPointer - 1].firstItem = 0;
            }
            else {
              softmenuStack[softmenuStackPointer - 1].firstItem = ((softmenu[softmenuStack[softmenuStackPointer-1].softmenu].numItems - 1)/6) / (itemShift/6) * itemShift;
            }
            showSoftmenuCurrentPart();

            setCatalogLastPos();
          }
        }
        else {
          if(alphaCase != AC_LOWER) {
            alphaCase = AC_LOWER;
            showAlphaMode();
          }
        }
      }

      else if(calcMode == CM_TAM) {
        addItemToBuffer(ITM_Min);
      }

      else if(calcMode == CM_REGISTER_BROWSER) {
        rbr1stDigit = true;
        if(rbrMode == RBR_GLOBAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1, FIRST_LOCAL_REGISTER);
          registerBrowser(NOPARAM);
        }
        else if(rbrMode == RBR_LOCAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_LOCAL_REGISTER - 1, numberOfLocalRegisters) + FIRST_LOCAL_REGISTER;
          registerBrowser(NOPARAM);
        }
        else if(rbrMode == RBR_NAMED) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1000 - 1, numberOfNamedVariables) + 1000;
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
        }
        fontBrowser(NOPARAM);
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

      else if(fractionType != FT_NONE) {
        fractionType = FT_NONE;
        refreshStack();
      }

      else if(calcMode == CM_NORMAL) {
        fnToReal(NOPARAM);
      }

      else if(calcMode == CM_FLAG_BROWSER || calcMode == CM_FONT_BROWSER || calcMode == CM_REGISTER_BROWSER) {
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
        showFunctionName(item, 10);
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
        showFunctionName(item, 10);
      }
    }

    else if(calcMode == CM_TAM) {
      addItemToBuffer(item);
    }

    else if(calcMode == CM_ASM) {
      if(item < 0) {
        //showSoftmenu(NULL, item, false);
      }

      else if(alphaCase==AC_LOWER && (CHR_A<=item && item<=CHR_Z)) {
        addItemToBuffer(item + 26);
      }

      else if(alphaCase==AC_LOWER && (CHR_ALPHA<=item && item<=CHR_OMEGA)) {
        addItemToBuffer(item + 36);
      }

      else if(item == CHR_DOWN_ARROW || item == CHR_UP_ARROW) {
        addItemToBuffer(item );
      }

      else {
        showFunctionName(item, 10);
      }
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
        rbr1stDigit = true;
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
      else if(item == ITM_RS) {
        rbr1stDigit = true;
        showContent = !showContent;
        registerBrowser(NOPARAM);
      }
      else if(item == ITM_RS) {
        rbr1stDigit = true;
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
        rbr1stDigit = true;
        if(rbrMode == RBR_GLOBAL || rbrMode == RBR_LOCAL) {
          calcMode = previousCalcMode;
          oldTime[0] = 0;
          showDateTime();
          clearScreen(false, true, true);
          fnRecall(currentRegisterBrowserScreen);
          STACK_LIFT_ENABLE;
          refreshStack();
        }
        else if(rbrMode == RBR_NAMED) {
        }
      }
      else if(CHR_0 <= item && item <= CHR_9 && (rbrMode == RBR_GLOBAL || rbrMode == RBR_LOCAL)) {
        if(rbr1stDigit) {
          rbr1stDigit = false;
          rbrRegister = item - CHR_0;
        }
        else {
          rbr1stDigit = true;
          rbrRegister = rbrRegister*10 + item - CHR_0;

          if(rbrMode == RBR_GLOBAL) {
            currentRegisterBrowserScreen = rbrRegister;
            registerBrowser(NOPARAM);
          }
          else {
            rbrRegister = (rbrRegister >= numberOfLocalRegisters ? 0 : rbrRegister);
            currentRegisterBrowserScreen = FIRST_LOCAL_REGISTER + rbrRegister;
            registerBrowser(NOPARAM);
          }
        }
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
  if(showFunctionNameItem != 0) {
    int16_t item = showFunctionNameItem;
    hideFunctionName();
    runFunction(item);
  }
}



void fnComplexCCCC(uint16_t unusedParamButMandatory) {
  // Emulating a CC key stroke

  //if(!shiftF) {
  //  shiftF = true;
  //  shiftStateChanged = true;
  //}

  btnClicked(NULL, "09"); // Index 9 of kbd_std array (beginning with "{34,   KEY_CC,") in file assign.c
}
#endif // TESTSUITE_BUILD
