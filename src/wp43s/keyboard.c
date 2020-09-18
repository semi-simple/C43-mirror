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
int16_t determineFunctionKeyItem(const char *data) {
  int16_t row, item = ITM_NOP;
  const softmenu_t *sm;
  int16_t itemShift, fn = *(data) - '0';

  if(shiftF) {
    itemShift = 6;
  }
  else if(shiftG) {
    itemShift = 12;
  }
  else {
    itemShift = 0;
  }

  if(softmenuStackPointer > 0) {
    sm = &softmenu[softmenuStack[softmenuStackPointer - 1].softmenu];
    row = min(3, (sm->numItems + modulo(softmenuStack[softmenuStackPointer - 1].firstItem - sm->numItems, 6))/6 - softmenuStack[softmenuStackPointer - 1].firstItem/6) - 1;

    if(itemShift/6 <= row && softmenuStack[softmenuStackPointer - 1].firstItem + itemShift + (fn - 1) < sm->numItems) {
      item = (sm->softkeyItem)[softmenuStack[softmenuStackPointer - 1].firstItem + itemShift + (fn - 1)];

      if(item > 0) {
        item %= 10000;
      }

      if(item == CHR_PROD_SIGN) {
        item = (getSystemFlag(FLAG_MULTx) ? CHR_DOT : CHR_CROSS);
      }
    }
  }

  return item;
}



/********************************************//**
 * \brief Simulate a function key click.
 *
 * \param w GtkWidget* The button to pass to btnFnPressed and btnFnReleased
 * \param data gpointer String containing the key ID
 * \return void
 ***********************************************/
#ifdef PC_BUILD
void btnFnClicked(GtkWidget *w, gpointer data) {
  GdkEvent mouseButton;
  mouseButton.button.button = 1;

  btnFnPressed(w, &mouseButton, data);
  btnFnReleased(w, &mouseButton, data);
}
#endif



/********************************************//**
 * \brief A calc function key was pressed
 *
 * \param w GtkWidget*
 * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
 * \return void
 ***********************************************/
#ifdef PC_BUILD
void btnFnPressed(GtkWidget *notUsed, GdkEvent *event, gpointer data) {
  if(event->button.button == 2) { // Middle click
    shiftF = true;
    shiftG = false;
  }
  if(event->button.button == 3) { // Right click
    shiftF = false;
    shiftG = true;
  }
#endif
#ifdef DMCP_BUILD
void btnFnPressed(void *data) {
#endif
  if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FONT_BROWSER) {
    int16_t item = determineFunctionKeyItem((char *)data);

    shiftF = false;
    shiftG = false;
    if(item != ITM_NOP && item != ITM_NULL) {
      if(lastErrorCode != 0) {
        lastErrorCode = 0;
      }

      #if(FN_KEY_TIMEOUT_TO_NOP == 1)
        showFunctionName(item, 1000); // 1000ms = 1s
      #else
        showFunctionNameItem = item;
      #endif
    }
    else {
      showFunctionNameItem = ITM_NOP;
    }
  }
}



/********************************************//**
 * \brief A calc function key was released
 *
 * \param w GtkWidget*
 * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
 * \return void
 ***********************************************/
#ifdef PC_BUILD
void btnFnReleased(GtkWidget *notUsed, GdkEvent *event, gpointer data) {
#endif
#ifdef DMCP_BUILD
void btnFnReleased(void *data) {
#endif
  if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FONT_BROWSER) {
    if(showFunctionNameItem != 0) {
      int16_t item = showFunctionNameItem;
      #if(FN_KEY_TIMEOUT_TO_NOP == 1)
        hideFunctionName();
      #endif

      if(calcMode != CM_CONFIRMATION) {
        if(lastErrorCode != 0) {
          lastErrorCode = 0;
        }

        if(softmenuStackPointer > 0) {
          if(calcMode == CM_ASM) {
            calcModeNormal();
          }
          else if(calcMode == CM_ASM_OVER_TAM) {
            reallyRunFunction(getOperation(), indexOfItems[item].param); // TODO: check why the param is taken from item and not from getOperation
            calcModeNormal();
            refreshScreen();
            return;
          }
          else if(calcMode == CM_ASM_OVER_AIM) {
            calcMode = CM_AIM;
            addItemToBuffer(item);
            calcMode = CM_ASM_OVER_AIM;
            refreshScreen();
            return;
          }

          if(item < 0) { // softmenu
            if(item != -MNU_SYSFL || calcMode != CM_TAM || transitionSystemState == 0) {
              showSoftmenu(NULL, item, true);
            }
          }
          else if((calcMode == CM_NORMAL || calcMode == CM_NIM) && (CHR_0<=item && item<=CHR_F)) {
            addItemToNimBuffer(item);
          }
          else if(calcMode == CM_TAM) {
            addItemToBuffer(item);
          }
          else if(item > 0) { // function
            if(calcMode == CM_NIM && item != KEY_CC) {
              closeNim();
              if(calcMode != CM_NIM) {
                if(indexOfItems[item].func == fnConstant) {
                  setSystemFlag(FLAG_ASLIFT);
                }
              }
            }

            if(lastErrorCode == 0) {
              temporaryInformation = TI_NO_INFO;
              runFunction(item);
            }
          }
        }
      }
    }

    refreshScreen();
  }
}



int16_t determineItem(const char *data) {
  int16_t result;
  const calcKey_t *key;

  key = getSystemFlag(FLAG_USER) ? (kbd_usr + (*data - '0')*10 + *(data+1) - '0') : (kbd_std + (*data - '0')*10 + *(data+1) - '0');

  // Shift f pressed and shift g not active
  if(key->primary == KEY_f && !shiftG && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_NIM || calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_AIM)) {
    temporaryInformation = TI_NO_INFO;

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
    }

    shiftF = !shiftF;
    return ITM_NOP;
  }

  // Shift g pressed and shift f not active
  else if(key->primary == KEY_g && !shiftF && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_NIM || calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_AIM)) {
    temporaryInformation = TI_NO_INFO;

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
    }

    shiftG = !shiftG;
    return ITM_NOP;
  }

  if(calcMode == CM_NORMAL || calcMode == CM_NIM || calcMode == CM_FONT_BROWSER || calcMode == CM_FLAG_BROWSER || calcMode == CM_REGISTER_BROWSER || calcMode == CM_BUG_ON_SCREEN || calcMode == CM_CONFIRMATION) {
    result = shiftF ? key->fShifted :
             shiftG ? key->gShifted :
                      key->primary;
  }
  else if(calcMode == CM_AIM || calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_AIM) {
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

  if(result == CHR_PROD_SIGN) {
    result = (getSystemFlag(FLAG_MULTx) ? CHR_CROSS : CHR_DOT);
  }

  shiftF = false;
  shiftG = false;

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
  GdkEvent mouseButton;
  mouseButton.button.button = 1;
  mouseButton.type = 0;

  btnPressed(w, &mouseButton, data);
  btnReleased(w, &mouseButton, data);
}
#endif
#ifdef DMCP_BUILD
void btnClicked(void *unused, void *data) {
  btnPressed(data);
  btnReleased(data);
}
#endif



/********************************************//**
 * \brief A calc button was pressed
 *
 * \param w GtkWidget*
 * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
 * \return void
 ***********************************************/
#ifdef PC_BUILD
void btnPressed(GtkWidget *notUsed, GdkEvent *event, gpointer data) {
  if(event->type == GDK_DOUBLE_BUTTON_PRESS || event->type == GDK_TRIPLE_BUTTON_PRESS) { // return unprocessed for double or triple click
    return;
  }
  if(event->button.button == 2) { // Middle click
    shiftF = true;
    shiftG = false;
  }
  if(event->button.button == 3) { // Right click
    shiftF = false;
    shiftG = true;
  }
#endif
#ifdef DMCP_BUILD
void btnPressed(void *data) {
#endif
  int16_t item = determineItem((char *)data);

  showFunctionNameItem = 0;
  if(item != ITM_NOP && item != ITM_NULL) {
    processKeyAction(item);
    if(!keyActionProcessed) {
      showFunctionName(item, 1000); // 1000ms = 1s
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
void btnReleased(GtkWidget *notUsed, GdkEvent *event, gpointer data) {
#endif
#ifdef DMCP_BUILD
void btnReleased(void *data) {
#endif
  if(showFunctionNameItem != 0) {
    int16_t item = showFunctionNameItem;
    hideFunctionName();
    if(item < 0) {
      showSoftmenu(NULL, item, calcMode == CM_AIM ? true : false);
    }
    else {
      runFunction(item);
    }
  }

  refreshScreen();
}



/********************************************//**
 * \brief A calc button was pressed
 *
 * \param w GtkWidget*
 * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
 * \return void
 ***********************************************/
void processKeyAction(int16_t item) {
  keyActionProcessed = false;

  if(lastErrorCode != 0 && item != KEY_EXIT && item != KEY_BACKSPACE) {
    lastErrorCode = 0;
  }

  temporaryInformation = TI_NO_INFO;

  switch(item) {
    case KEY_BACKSPACE:
      fnKeyBackspace(NOPARAM);
      keyActionProcessed = true;
      break;

    case KEY_UP:
      fnKeyUp(NOPARAM);
      keyActionProcessed = true;
      break;

    case KEY_DOWN:
      fnKeyDown(NOPARAM);
      keyActionProcessed = true;
      break;

    case KEY_EXIT:
      fnKeyExit(NOPARAM);
      keyActionProcessed = true;
      break;

    case KEY_CC:
    case ITM_ENTER:
    case KEY_dotD:
      if(calcMode == CM_REGISTER_BROWSER || calcMode == CM_FLAG_BROWSER || calcMode == CM_FONT_BROWSER) {
        keyActionProcessed = true;
      }
      break;

    default:
      switch(calcMode) {
        case CM_NORMAL:
          if(item == ITM_EXPONENT || item==CHR_PERIOD || (CHR_0<=item && item<=CHR_9)) {
            addItemToNimBuffer(item);
            keyActionProcessed = true;
          }
          // Following commands do not timeout to NOP
          else if(item == KEY_UNDO || item == KEY_BST || item == KEY_SST || item == ITM_PR || item == ITM_AIM) {
            runFunction(item);
            keyActionProcessed = true;
          }
          break;

        case CM_AIM:
          if(alphaCase == AC_LOWER && (CHR_A <= item && item <= CHR_Z)) {
            addItemToBuffer(item + 26);
            keyActionProcessed = true;
          }

          else if(alphaCase == AC_LOWER && (CHR_ALPHA <= item && item <= CHR_OMEGA)) {
            addItemToBuffer(item + 36);
            keyActionProcessed = true;
          }

          else if(item == CHR_DOWN_ARROW) {
            nextChar = NC_SUBSCRIPT;
            keyActionProcessed = true;
          }

          else if(item == CHR_UP_ARROW) {
            nextChar = NC_SUPERSCRIPT;
            keyActionProcessed = true;
          }
          break;

        case CM_TAM:
          addItemToBuffer(item);
          keyActionProcessed = true;
          break;

        case CM_ASM:
        case CM_ASM_OVER_TAM:
        case CM_ASM_OVER_AIM:
          if(alphaCase==AC_LOWER && (CHR_A<=item && item<=CHR_Z)) {
            addItemToBuffer(item + 26);
            keyActionProcessed = true;
          }

          else if(alphaCase==AC_LOWER && (CHR_ALPHA<=item && item<=CHR_OMEGA)) {
            addItemToBuffer(item + 36);
            keyActionProcessed = true;
          }

          else if(item == CHR_DOWN_ARROW || item == CHR_UP_ARROW) {
            addItemToBuffer(item);
            keyActionProcessed = true;
          }
          break;

        case CM_NIM:
          keyActionProcessed = true;
          addItemToNimBuffer(item);
          break;

        case CM_REGISTER_BROWSER:
          if(item == CHR_PERIOD) {
            rbr1stDigit = true;
            if(rbrMode == RBR_GLOBAL) {
              if(allLocalRegisterPointer->numberOfLocalRegisters > 0) {
                rbrMode = RBR_LOCAL;
                currentRegisterBrowserScreen = FIRST_LOCAL_REGISTER;
              }
              else if(allNamedVariablePointer->numberOfNamedVariables > 0) {
                rbrMode = RBR_NAMED;
                currentRegisterBrowserScreen = FIRST_NAMED_VARIABLE;
              }
            }
            else if(rbrMode == RBR_LOCAL) {
              if(allNamedVariablePointer->numberOfNamedVariables > 0) {
                rbrMode = RBR_NAMED;
                currentRegisterBrowserScreen = FIRST_NAMED_VARIABLE;
              }
              else {
                rbrMode = RBR_GLOBAL;
                currentRegisterBrowserScreen = REGISTER_X;
              }
            }
            else if(rbrMode == RBR_NAMED) {
              rbrMode = RBR_GLOBAL;
              currentRegisterBrowserScreen = REGISTER_X;
            }
          }
          else if(item == ITM_RS) {
            rbr1stDigit = true;
            showContent = !showContent;
          }
          else if(item == ITM_RCL) {
            rbr1stDigit = true;
            if(rbrMode == RBR_GLOBAL || rbrMode == RBR_LOCAL) {
              calcMode = previousCalcMode;
              fnRecall(currentRegisterBrowserScreen);
              setSystemFlag(FLAG_ASLIFT);
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
              }
              else {
                rbrRegister = (rbrRegister >= allLocalRegisterPointer->numberOfLocalRegisters ? 0 : rbrRegister);
                currentRegisterBrowserScreen = FIRST_LOCAL_REGISTER + rbrRegister;
              }
            }
          }

          keyActionProcessed = true;
          break;

        case CM_FLAG_BROWSER:
        case CM_FONT_BROWSER:
        case CM_ERROR_MESSAGE:
        case CM_BUG_ON_SCREEN:
          keyActionProcessed = true;
          break;

        case CM_CONFIRMATION:
          if(item == CHR_3 || item == ITM_XEQ) { // Yes or XEQ
            calcMode = previousCalcMode;
            temporaryInformation = TI_NO_INFO;
            confirmedFunction(CONFIRMED);
          }

          else if(item == CHR_7) { // No
            calcMode = previousCalcMode;
            temporaryInformation = TI_NO_INFO;
          }

          keyActionProcessed = true;
          break;

        default:
          sprintf(errorMessage, "In function btnPressed: %" PRIu8 " is an unexpected value while processing calcMode!", calcMode);
          displayBugScreen(errorMessage);
      }
  }
}
#endif // TESTSUITE_BUILD



/********************************************//**
 * \brief Processing ENTER key
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnKeyEnter(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
  switch(calcMode) {
    case CM_NORMAL:
      setSystemFlag(FLAG_ASLIFT);

      liftStack();
      copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);

      clearSystemFlag(FLAG_ASLIFT);
      break;

    case CM_AIM:
    case CM_ASM_OVER_AIM:
      calcModeNormal();
      while(softmenuStackPointer > softmenuStackPointerBeforeAIM) {
        popSoftmenu();
      }

      if(aimBuffer[0] == 0) {
        undo();
      }
      else {
        int16_t len = stringByteLength(aimBuffer) + 1;

        reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), AM_NONE);
        xcopy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, len);

        setSystemFlag(FLAG_ASLIFT);
        liftStack();
        clearSystemFlag(FLAG_ASLIFT);

        copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
        aimBuffer[0] = 0;
      }
      break;

    case CM_NIM:
      closeNim();

      setSystemFlag(FLAG_ASLIFT);
      liftStack();
      clearSystemFlag(FLAG_ASLIFT);

      copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
      break;

    case CM_TAM:
      tamTransitionSystem(TT_ENTER);
      break;

    case CM_ASM:
    case CM_ASM_OVER_TAM:
    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FONT_BROWSER:
    case CM_ERROR_MESSAGE:
    case CM_BUG_ON_SCREEN:
      break;

    case CM_CONFIRMATION:
      calcMode = previousCalcMode;
      confirmedFunction(CONFIRMED);
      break;

    default:
      sprintf(errorMessage, "In function fnKeyEnter: unexpected calcMode value (%" PRIu8 ") while processing key ENTER!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}



/********************************************//**
 * \brief Processing EXIT key
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnKeyExit(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
  switch(calcMode) {
    case CM_NORMAL:
      if(lastErrorCode != 0) {
        lastErrorCode = 0;
      }
      else if(softmenuStackPointer > 0) {
        popSoftmenu();
      }
      break;

    case CM_AIM:
      if(softmenuStack[softmenuStackPointer-1].softmenu == MY_ALPHA_MENU) {
        calcModeNormal();
        popSoftmenu();

        if(aimBuffer[0] == 0) {
          undo();
        }
        else {
          int16_t len = stringByteLength(aimBuffer) + 1;

          reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), AM_NONE);

          xcopy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, len);
          aimBuffer[0] = 0;

          setSystemFlag(FLAG_ASLIFT);
        }
      }
      else {
       popSoftmenu();
       if(softmenuStackPointer == 0) {
         softmenuStackPointerBeforeAIM = 0;
         showSoftmenu(NULL, -MNU_MyAlpha, false);
       }
      }
      break;

    case CM_NIM:
      addItemToNimBuffer(KEY_EXIT);
      break;

    case CM_TAM:
    case CM_ASM:
      calcModeNormal();
      break;

    case CM_ASM_OVER_TAM:
      transitionSystemState = 0;
      calcModeTam();
      sprintf(tamBuffer, "%s __", indexOfItems[getOperation()].itemCatalogName);
      tamTransitionSystem(TT_NOTHING);
      break;

    case CM_ASM_OVER_AIM:
      calcModeAim(NOPARAM);
      break;

    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FONT_BROWSER:
      rbr1stDigit = true;
      calcMode = previousCalcMode;
      break;

    case CM_BUG_ON_SCREEN:
      calcMode = previousCalcMode;
      break;

    case CM_CONFIRMATION:
      calcMode = previousCalcMode;
      temporaryInformation = TI_NO_INFO;
      break;

    default:
      sprintf(errorMessage, "In function fnKeyExit: unexpected calcMode value (%" PRIu8 ") while processing key EXIT!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}



/********************************************//**
 * \brief Processing CC key
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnKeyCC(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
  uint32_t dataTypeX;
  uint32_t dataTypeY;

  switch(calcMode) {
    case CM_NORMAL:
      dataTypeX = getRegisterDataType(REGISTER_X);
      dataTypeY = getRegisterDataType(REGISTER_Y);

      if(   (dataTypeX == dtReal34 || dataTypeX == dtLongInteger)
         && (dataTypeY == dtReal34 || dataTypeY == dtLongInteger)) {
        runFunction(ITM_REtoCX);
      }
      else if(dataTypeX == dtComplex34) {
        runFunction(ITM_CXtoRE);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "You cannot use CC with %s in X and %s in Y!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false), getDataTypeName(getRegisterDataType(REGISTER_Y), true, false));
          moreInfoOnError("In function btnPressed:", errorMessage, NULL, NULL);
        #endif
      }
      break;

    case CM_NIM:
      addItemToNimBuffer(KEY_CC);
      break;

    case CM_ASM:
    case CM_ASM_OVER_TAM:
    case CM_ASM_OVER_AIM:
      break;

    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FONT_BROWSER:
      break;

    default:
      sprintf(errorMessage, "In function fnKeyCC: unexpected calcMode value (%" PRIu8 ") while processing key CC!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}



/********************************************//**
 * \brief Processing BACKSPACE key
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnKeyBackspace(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
  uint16_t lg, x, y, newXCursor;

  switch(calcMode) {
    case CM_NORMAL:
      if(lastErrorCode != 0) {
        lastErrorCode = 0;
      }
      else {
        runFunction(ITM_CLX);
      }
      break;

    case CM_AIM:
      if(stringByteLength(aimBuffer) > 0) {
        lg = stringLastGlyph(aimBuffer);
        aimBuffer[lg] = 0;
      }
      break;

    case CM_ASM_OVER_AIM:
      if(stringByteLength(aimBuffer) > 0) {
        lg = stringLastGlyph(aimBuffer);
        aimBuffer[lg] = 0;
        newXCursor = showString(aimBuffer, &standardFont, 1, Y_POSITION_OF_AIM_LINE + 6, vmNormal, true, true);
        for(x=newXCursor; x<xCursor+6; x++) {
          for(y=Y_POSITION_OF_AIM_LINE+6; y<Y_POSITION_OF_AIM_LINE+26; y++) {
            clearPixel(x, y);
          }
        }
        xCursor = newXCursor;
      }
      break;

    case CM_NIM:
      addItemToNimBuffer(KEY_BACKSPACE);
      break;

    case CM_TAM:
      tamTransitionSystem(TT_BACKSPACE);
      break;

    case CM_ASM:
      addItemToBuffer(KEY_BACKSPACE);
      break;

    case CM_ASM_OVER_TAM:
      break;

    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FONT_BROWSER:
      calcMode = previousCalcMode;
      break;

    case CM_BUG_ON_SCREEN:
      calcMode = previousCalcMode;
      break;

    case CM_CONFIRMATION:
      calcMode = previousCalcMode;
      temporaryInformation = TI_NO_INFO;
      break;

    default:
      sprintf(errorMessage, "In function fnKeyBackspace: unexpected calcMode value (%" PRIu8 ") while processing key BACKSPACE!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}



/********************************************//**
 * \brief Processing UP key
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnKeyUp(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
  int16_t itemShift;

  switch(calcMode) {
    case CM_NORMAL:
    case CM_AIM:
    case CM_NIM:
    case CM_ASM:
    case CM_ASM_OVER_TAM:
    case CM_ASM_OVER_AIM:
      resetAlphaSelectionBuffer();
      if(softmenuStackPointer > 0  && softmenuStack[softmenuStackPointer - 1].softmenu != MY_ALPHA_MENU) {
        int16_t sm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
        if((sm == -MNU_alpha_omega || sm == -MNU_ALPHAintl) && alphaCase == AC_LOWER) {
          alphaCase = AC_UPPER;
          softmenuStack[softmenuStackPointer - 1].softmenu--; // Switch to the upper case menu
        }
        else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH) && alphaCase == AC_LOWER) {
          alphaCase = AC_UPPER;
        }
        else {
          itemShift = alphaSelectionMenu == ASM_NONE ? 18 : 6;

          if((softmenuStack[softmenuStackPointer - 1].firstItem + itemShift) < softmenu[softmenuStack[softmenuStackPointer-1].softmenu].numItems) {
            softmenuStack[softmenuStackPointer - 1].firstItem += itemShift;
          }
          else {
            softmenuStack[softmenuStackPointer - 1].firstItem = 0;
          }

          setCatalogLastPos();
        }
      }
      else {
        if(alphaCase != AC_UPPER) {
          alphaCase = AC_UPPER;
        }
      }
      break;

    case CM_TAM:
      addItemToBuffer(ITM_Max);
      break;

    case CM_REGISTER_BROWSER:
      rbr1stDigit = true;
      if(rbrMode == RBR_GLOBAL) {
        currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen + 1, FIRST_LOCAL_REGISTER);
      }
      else if(rbrMode == RBR_LOCAL) {
        currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_LOCAL_REGISTER + 1, allLocalRegisterPointer->numberOfLocalRegisters) + FIRST_LOCAL_REGISTER;
      }
      else if(rbrMode == RBR_NAMED) {
        currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_NAMED_VARIABLE + 1, allNamedVariablePointer->numberOfNamedVariables) + FIRST_NAMED_VARIABLE;
      }
      else {
        sprintf(errorMessage, "In function btnPressed: unexpected case while processing key UP! %" PRIu8 " is an unexpected value for rbrMode.", rbrMode);
        displayBugScreen(errorMessage);
      }
      break;

    case CM_FLAG_BROWSER:
      currentFlgScr = 3 - currentFlgScr;
     break;

    case CM_FONT_BROWSER:
      if(currentFntScr >= 2) {
        currentFntScr--;
      }
      break;

    default:
      sprintf(errorMessage, "In function fnKeyUp: unexpected calcMode value (%" PRIu8 ") while processing key UP!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}



/********************************************//**
 * \brief Processing DOWN key
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnKeyDown(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
  int16_t itemShift;

  switch(calcMode) {
    case CM_NORMAL:
    case CM_AIM:
    case CM_NIM:
    case CM_ASM:
    case CM_ASM_OVER_TAM:
    case CM_ASM_OVER_AIM:
      resetAlphaSelectionBuffer();
      if(softmenuStackPointer > 0  && softmenuStack[softmenuStackPointer - 1].softmenu != MY_ALPHA_MENU) {
        int16_t sm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
        if((sm == -MNU_ALPHA_OMEGA || sm == -MNU_ALPHAINTL) && alphaCase == AC_UPPER) {
          alphaCase = AC_LOWER;
          softmenuStack[softmenuStackPointer - 1].softmenu++; // Switch to the lower case menu
        }
        else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH) && alphaCase == AC_UPPER) {
          alphaCase = AC_LOWER;
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

          setCatalogLastPos();
        }
      }
      else {
        if(alphaCase != AC_LOWER) {
          alphaCase = AC_LOWER;
        }
      }
      break;

    case CM_TAM:
      addItemToBuffer(ITM_Min);
      break;


    case CM_REGISTER_BROWSER:
      rbr1stDigit = true;
      if(rbrMode == RBR_GLOBAL) {
        currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1, FIRST_LOCAL_REGISTER);
      }
      else if(rbrMode == RBR_LOCAL) {
        currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_LOCAL_REGISTER - 1, allLocalRegisterPointer->numberOfLocalRegisters) + FIRST_LOCAL_REGISTER;
      }
      else if(rbrMode == RBR_NAMED) {
        currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1000 - 1, allNamedVariablePointer->numberOfNamedVariables) + 1000;
      }
      else {
        sprintf(errorMessage, "In function btnPressed: unexpected case while processing key DOWN! %" PRIu8 " is an unexpected value for rbrMode.", rbrMode);
        displayBugScreen(errorMessage);
      }
      break;

    case CM_FLAG_BROWSER:
      currentFlgScr = 3 - currentFlgScr;
      break;

    case CM_FONT_BROWSER:
      if(currentFntScr < numScreensNumericFont+numScreensStandardFont) {
        currentFntScr++;
      }
      break;

    default:
      sprintf(errorMessage, "In function fnKeyDown: unexpected calcMode value (%" PRIu8 ") while processing key DOWN!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}



/********************************************//**
 * \brief Processing .d key
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnKeyDotD(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
  switch(calcMode) {
    case CM_NORMAL:
      if(getSystemFlag(FLAG_FRACT)) {
        clearSystemFlag(FLAG_FRACT);
      }
      else {
        runFunction(ITM_toREAL);
      }
      break;

    case CM_NIM:
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function btnPressed:", "the data type date is to be coded!", NULL, NULL);
      #endif
      break;

    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FONT_BROWSER:
      break;

    default:
      sprintf(errorMessage, "In function fnKeyDotD: unexpected calcMode value (%" PRIu8 ") while processing key .d!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}
