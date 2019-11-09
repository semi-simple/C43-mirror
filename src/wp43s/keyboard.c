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
    if(shiftF) {
      showGlyph(NUM_SUP_f, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // f 4+8+3 is pixel wide
//#ifndef TESTSUITE_BUILD
      showSoftmenuCurrentPart();                                                //JM - Redraw boxes etc after shift is shown
      if(softmenuStackPointer > 0) {                                            //JM - Display dot in the f - line
        JM_DOT( -1, 201 );                                                      //JM - Display dot in the f - line
        JM_DOT( 392, 201 );                                                     //JM - Display dot in the f - line
      }                                                                         //JM - Display dot in the f - line
//#endif
    }
    else if(shiftG) {
      showGlyph(NUM_SUP_g, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // g 4+10+1 is pixel wide
//#ifndef TESTSUITE_BUILD
      showSoftmenuCurrentPart();                                                //JM - Redraw boxes etc after shift is shown
      if(softmenuStackPointer > 0) {                                            //JM - Display dot in the g - line
        JM_DOT( -1, 175 );                                                      //JM - Display dot in the g - line
        JM_DOT( 392, 175 );                                                     //JM - Display dot in the g - line
        JM_DOT( -1, 182 );                                                      //JM - Display dot in the g - line
        JM_DOT( 392, 182 );                                                     //JM - Display dot in the g - line
      }                                                                         //JM - Display dot in the g - line
//#endif
    }
    else {
      refreshRegisterLine(REGISTER_T);
//#ifndef TESTSUITE_BUILD
      showSoftmenuCurrentPart();                                                //JM - Redraw boxes etc after shift was shown
//#endif
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
  if(shiftF || shiftG)        //vv dr
  {
  shiftF = false;
  shiftG = false;
  showShiftState();
  }                           //^^
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
    row = min(3, sm->numItems/6 - softmenuStack[softmenuStackPointer - 1].firstItem/6) - 1;

    if(itemShift/6 <= row) {
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
        if(calcMode == CM_NIM && func != KEY_CC && func != KEY_CC1 ) {     //JM CPX Added CC1
          closeNim();
          if(calcMode != CM_NIM) {
            if(indexOfItems[func % 10000].func == fnConstant) {
              stackLiftEnable();
            }
          }
        }

        if(lastErrorCode == 0) {
          temporaryInformation = TI_NO_INFO;
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
    result = 0;
  }
  return result;
}



/********************************************//**
 * \brief convert a string containing the key number to int16_t
 *
 * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
 * \return int16_t
 ***********************************************/
//#ifdef DMCP_BUILD
//int16_t stringToKeyNumber(void *data) {
//  return (*((char *)data) - '0')*10 + *(((char *)data)+1) - '0';
//}
//#else
//int16_t stringToKeyNumber(gpointer data) {
//  return (*((char *)data) - '0')*10 + *(((char *)data)+1) - '0';
//}
//#endif
#define stringToKeyNumber(data)         ((*((char *)data) - '0')*10 + *(((char *)data)+1) - '0')



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

  if(testEnabled) { fnSwStart(1); }     //dr

  key = userModeEnabled && ((calcMode == CM_NORMAL) || (calcMode == CM_NIM)) ? (kbd_usr + stringToKeyNumber(data)) : (kbd_std + stringToKeyNumber(data));
  //JM Added (calcMode == CM_NORMAL) to prevent user substitution in AIM and TAM

  allowScreenUpdate = true;



#ifdef JM_MULTISHIFT ////MULTISHIFT AND CLRDROP                                //JM TIMER - checks on any key pressed.

  JM_auto_drop_enabled=true;                          //JM TIMER CLRDROP
  if(key->primary == KEY_BACKSPACE) {
    #ifdef DMCP_BUILD                                 //JM TIMER DMCP CLRDROP
    now = sys_current_ms();                           //JM TIMER DMCP SHIFTCANCEL
    if(now > now_MEM + JM_CLRDROP_TIMER) {            //JM TIMER DMCP CLRDROP Trying out timer shift allow double backspace to DROP
      JM_auto_drop_enabled=false;                     //JM TIMER DMCP CLRDROP
    }                                                 //JM TIMER DMCP CLRDROP
    #endif                                            //JM TIMER DMCP CLRDROP
    #ifdef PC_BUILD                                   //JM TIMER EMULATOR CLRDROP
    now = g_get_monotonic_time();                     //JM usec
    if(now > now_MEM + JM_CLRDROP_TIMER*1000) {       //JM TIMER EMULATOR CLRDROP Trying out timer shift allow double backspace to DROP
      JM_auto_drop_enabled=false;                     //JM TIMER EMULATOR CLRDROP
    }                                                 //JM TIMER EMULATOR CLRDROP
    #endif                                            //JM TIMER EMULATOR CLRDROP

    now_MEM = now;                                    //JM TIMER -- any last key pressed

    if(JM_auto_drop_activated) {                      //JM TIMER CLRDROP ensure multiple drops don't not occur
      JM_auto_drop_enabled = false;                   //JM TIMER CLRDROP
      JM_auto_drop_activated = false;                 //JM TIMER CLRDROP
    }
  }

  // JM Shift f pressed  //JM shifts change f/g to a single function key toggle to match DM42 keyboard
  // JM Inserted new section and removed old f and g key processing sections
  if(key->primary == KEY_fg && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_NIM)) {   //JM shifts
    if(temporaryInformation != TI_NO_INFO) {                                                                                  //JM shifts
      temporaryInformation = TI_NO_INFO;                                                                                      //JM shifts
      refreshRegisterLine(REGISTER_X);                                                                                        //JM shifts
      refreshRegisterLine(REGISTER_Y);                                                                                        //JM shifts
    }                                                                                                                         //JM shifts
                                                                                                                              //JM shifts
    if(lastErrorCode != 0) {                                                                                                  //JM shifts
      lastErrorCode = 0;                                                                                                      //JM shifts
      refreshStack();                                                                                                         //JM shifts
    }                                                                                                                         //JM shifts

    if(ShiftTimoutMode || Home3TimerMode) {
      if(Home3TimerMode) {
        JM_SHIFT_HOME_TIMER2 = JM_SHIFT_HOME_TIMER1;
        JM_SHIFT_HOME_TIMER1 = JM_SHIFT_TIMER_LOOP  - JM_SHIFT_RESET;
        //printf("T2 %d T1 %d SR %d SUB %d \n", JM_SHIFT_HOME_TIMER2, JM_SHIFT_HOME_TIMER1, JM_SHIFT_RESET, JM_SHIFT_HOME_TIMER1 + JM_SHIFT_HOME_TIMER2);
        if(JM_SHIFT_HOME_TIMER1 + JM_SHIFT_HOME_TIMER2 <= JM_3_SHIFT_CUTOFF) {  //increased limit from 500 to 600 ms
          JM_SHIFT_HOME_TIMER1 = JM_SHIFT_TIMER_LOOP; //max
          shiftF = false;  // Set it up, for flags to be cleared below.
          shiftG = true;
          if(HOME3) {
            if((softmenuStackPointer > 0) && (softmenuStackPointer_MEM == softmenuStackPointer)) {                            //JM shifts
              popSoftmenu();                                                                                                  //JM shifts
            }
            else {
              if (calcMode == CM_AIM) {                                                                                       //JM shifts
                showSoftmenu(NULL, -MNU_ALPHA, true);                                                                         //JM shifts //JM ALPHA-HOME  ALPHA AIM OR NIM
              }
              else {                                                                                                          //JM SHIFTS
                showSoftmenu(NULL, -MNU_HOME, true);                                                                          //JM shifts  //JM ALPHA-HOME
              }                                                                                                               //JM shifts
                                                                                                                              //JM shifts
              softmenuStackPointer_MEM = softmenuStackPointer;                                                                //JM shifts
            }
          }                                                                                                                   //JM shifts
        }
      }
      JM_SHIFT_RESET =  JM_SHIFT_TIMER_LOOP;
    }

    if(!shiftF && !shiftG) {                                                                                                  //JM shifts
      shiftF = true;                                                                                                          //JM shifts
      shiftG = false;
    }                                                                                                                         //JM shifts
    else if(shiftF && !shiftG) {                                                                                              //JM shifts
      shiftF = false;                                                                                                         //JM shifts
      shiftG = true;                                                                                                          //JM shifts
    }
    else if(!shiftF && shiftG) {                                                                                              //JM shifts
      shiftF = false;                                                                                                         //JM shifts
      shiftG = false;                                                                                                         //JM shifts
    }
    else if(shiftF && shiftG) {                                                                                               //JM shifts  should never be possible. included for completeness
      shiftF = false;                                                                                                         //JM shifts
      shiftG = false;                                                                                                         //JM shifts
    }                                                                                                                         //JM shifts
                                                                                                                              //JM shifts
    showShiftState();                                                                                                         //JM shifts
  }                                                                                                                           //JM shifts
  else
                                                                                                                              //JM shifts
#endif

  // Shift f pressed
  if(key->primary == KEY_f && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_NIM)) {
    if(temporaryInformation != TI_NO_INFO) {
      temporaryInformation = TI_NO_INFO;
      refreshRegisterLine(REGISTER_X);
      refreshRegisterLine(REGISTER_Y);
    }

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
      refreshStack();
    }

    if(ShiftTimoutMode) {                       //vv JM
      JM_SHIFT_RESET =  JM_SHIFT_TIMER_OFF;     //      JM_SHIFT_TIMER_LOOP
    }                                           //^^

    shiftF = !shiftF;
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

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
      refreshStack();
    }

    if(ShiftTimoutMode) {                       //vv JM
      JM_SHIFT_RESET =  JM_SHIFT_TIMER_OFF;     //      JM_SHIFT_TIMER_LOOP
    }                                           //^^

    shiftG = !shiftG;
    shiftF = false;

    showShiftState();
  }

  //JM ASSIGN - GET FUNCTION NUMBER --------------------------------------------------------------------------------
  else if(JM_ASN_MODE == 32766) {            //JM Check if JM ASSIGN IS IN PROGRESS AND CAPTURE THE FUNCTION AND KEY TO BE ASSIGNED
    //printf("%d\n", determineItem(key));    //JM GET FUNCTION NUMBER: If seek is pressed, a function can be chosen and pressed.
    JM_ASN_MODE = determineItem(key);        //JM The result is the function number, item number, asnd is placed in
    fnKEYSELECT();                           //JM Place in auto trigger register, ready for next keypress
    key = (kbd_std + 32);                    //JM EXIT key to exit when done and cancel shifts
    shiftG = false;
    shiftF = false;
  }

  //JM ASSIGN - GET KEY & ASSIGN MEMORY FUNCTION JM_ASN_MODE
                                             //JM JM_ASN_MODE contains the command to be put on a key. 0 if not active
  else if(JM_ASN_MODE != 0) {                //JM GET KEY
    int16_t tempkey = stringToKeyNumber(data);
    fnASSIGN(JM_ASN_MODE, tempkey);          //JM CHECKS FOR INVALID KEYS IN HERE
    JM_ASN_MODE = 0;                         //JM Catchall - cancel the mode once it had the opportunity to be handled. Whether handled or not.
    key = (kbd_std + 32);                    //JM EXIT key to exit when done and cancel shifts
    shiftG = false;
    shiftF = false;
  }
  //JM    ^^^^^^^^^^^^^^^^^^^^^^^^^^^ --------------------------------------------------------------------------------

  else {
    int16_t item = determineItem(key);

  //JM NORMKEY _ CHANGE NORMAL MODE KEY SIGMA+ TO SOMETHING ELSE
    if((calcMode == CM_NORMAL) && (!userModeEnabled && ( stringToKeyNumber(data) == 0) )) {
      //printf("%d", stringToKeyNumber(data));
      item = Norm_Key_00_VAR;
    }
  //JM    ^^^^^^^^^^^^^^^^^^^^^^^^^^^ --------------------------------------------------------------------------------

    if(item == CHR_PROD_SIGN) {
      item = (productSign == PS_DOT ? CHR_DOT : CHR_CROSS);
    }

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
        calcModeNormal();
        showAlphaMode();
        popSoftmenu();

        if(aimBuffer[0] == 0) {
          restoreStack();
        }
        else {
          int16_t mem = stringByteLength(aimBuffer);

          reallocateRegister(REGISTER_X, dtString, mem, AM_NONE);
          memcpy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, mem + 1);

          STACK_LIFT_ENABLE;
          liftStack();
          STACK_LIFT_DISABLE;

          copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          aimBuffer[0] = 0;
        }

        refreshStack();
      }

      else if(calcMode == CM_TAM) {
        addItemToBuffer(ITM_ENTER);
      }

      else if(calcMode == CM_NORMAL) {
        STACK_LIFT_ENABLE;

        liftStack();
        copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);

        refreshStack();

        if(eRPN == false) {                                                     //JM eRPN modification. Ensure DUP always set SL
        STACK_LIFT_DISABLE;
        }                                                                       //JM eRPN modification
      }

      else if(calcMode == CM_FONT_BROWSER) {
      }

      else if(calcMode == CM_FLAG_BROWSER) {
      }

      else if(calcMode == CM_REGISTER_BROWSER) {
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
        if(/*(*/ softmenuStackPointer == 1 /*) && (softmenuStack[softmenuStackPointer-1].softmenu == MY_ALPHA_MENU)*/) {      //JM ALPHA-HOME make sure we are at the bottom of the stack
          calcModeNormal();
          showAlphaMode();
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

      else if(calcMode == CM_NIM) {
        addItemToNimBuffer(KEY_EXIT);
      }

      else if(calcMode == CM_FLAG_BROWSER || calcMode == CM_FONT_BROWSER || calcMode == CM_REGISTER_BROWSER) {
        rbr1stDigit = true;
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

    else if((item == KEY_CC) || (item == KEY_COMPLEX) || (item == KEY_CC1)) {             //JM CPX Add COMPLEX CPX*
      if((calcMode == CM_NORMAL) && ((item == KEY_CC1))) {                                //JM CPX empty function for CC1
        }
      else if((calcMode == CM_NORMAL) && ((item == KEY_CC) || (item == KEY_COMPLEX))) {   //JM CPX
        uint32_t dataTypeX = getRegisterDataType(REGISTER_X);
        uint32_t dataTypeY = getRegisterDataType(REGISTER_Y);
        bool_t xIsAReal;

        if(   (dataTypeX == dtReal16 || dataTypeX == dtReal34 || dataTypeX == dtLongInteger)
           && (dataTypeY == dtReal16 || dataTypeY == dtReal34 || dataTypeY == dtLongInteger)) {
          copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
          fnSetFlag(FLAG_CPXRES);

          xIsAReal = true;
          if(complexMode == CM_POLAR) {
            if(dataTypeX == dtReal16 && getRegisterAngularMode(REGISTER_X) != AM_NONE) {
              convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), AM_RADIAN);
              setRegisterAngularMode(REGISTER_X, AM_NONE);
              dataTypeX = dtReal16;
              xIsAReal = false;
            }
            else if(dataTypeX == dtReal34 && getRegisterAngularMode(REGISTER_X) != AM_NONE) {
              convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), AM_RADIAN);
              setRegisterAngularMode(REGISTER_X, AM_NONE);
              dataTypeX = dtReal34;
              xIsAReal = false;
            }
          }

          if(dataTypeX == dtLongInteger) {
            if(dataTypeY == dtReal16 || dataTypeY == dtLongInteger) {
              convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
              dataTypeX = dtReal16;
            }
            else { // dataTypeY == dtReal34
              convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
              dataTypeX = dtReal34;
            }
          }

          if(dataTypeY == dtLongInteger) {
            if(dataTypeX == dtReal16) {
              convertLongIntegerRegisterToReal16Register(REGISTER_Y, REGISTER_Y);
              dataTypeY = dtReal16;
            }
            else { // dataTypeX == dtReal34
              convertLongIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
              dataTypeY = dtReal34;
            }
          }

          if(dataTypeX == dtReal16 && dataTypeY == dtReal34) {
            real34_t temp;

            real16ToReal34(REGISTER_REAL16_DATA(REGISTER_X), &temp);
            reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
            real34Copy(&temp, REGISTER_REAL34_DATA(REGISTER_X));
            dataTypeX = dtReal34;
          }

          if(dataTypeY == dtReal16 && dataTypeX == dtReal34) {
            real34_t temp;

            real16ToReal34(REGISTER_REAL16_DATA(REGISTER_Y), &temp);
            reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, AM_NONE);
            real34Copy(&temp, REGISTER_REAL34_DATA(REGISTER_Y));
          }

          if(dataTypeX == dtReal16) {
            complex16_t temp;

            real16Copy(REGISTER_REAL16_DATA(REGISTER_Y), &temp);
            real16Copy(REGISTER_REAL16_DATA(REGISTER_X), VARIABLE_IMAG16_DATA(&temp));
            reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);

            if(complexMode == CM_POLAR) {
              if(real16CompareEqual(VARIABLE_REAL16_DATA(&temp), const16_0)) {
                real16Zero(VARIABLE_IMAG16_DATA(&temp));
              }
              else {
                real16_t magnitude16, theta16;

                real16Copy(VARIABLE_REAL16_DATA(&temp), &magnitude16);
                real16Copy(VARIABLE_IMAG16_DATA(&temp), &theta16);
                if(xIsAReal) {
                  convertAngle16FromTo(&theta16, currentAngularMode, AM_RADIAN);
                }
                if(real16CompareLessThan(&magnitude16, const16_0)) {
                  real16SetPositiveSign(&magnitude16);
                  real16Add(&theta16, const_pi, &theta16);
                  real16DivideRemainder(&theta16, const_2pi, &theta16);
                }
                real16PolarToRectangular(&magnitude16, &theta16, REGISTER_REAL16_DATA(REGISTER_X), REGISTER_IMAG16_DATA(REGISTER_X)); // theta16 in radian
              }
            }
            else {
              complex16Copy(&temp, REGISTER_COMPLEX16_DATA(REGISTER_X));
            }
          }
          else { //dataTypeX == dtReal34
            complex34_t temp;

            real34Copy(REGISTER_REAL34_DATA(REGISTER_Y), &temp);
            real34Copy(REGISTER_REAL34_DATA(REGISTER_X), VARIABLE_IMAG34_DATA(&temp));
            reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

            if(complexMode == CM_POLAR) {
              if(real34CompareEqual(VARIABLE_REAL34_DATA(&temp), const34_0)) {
                real34Zero(VARIABLE_IMAG34_DATA(&temp));
              }
              else {
                real34_t magnitude34, theta34;

                real34Copy(VARIABLE_REAL34_DATA(&temp), &magnitude34);
                real34Copy(VARIABLE_IMAG34_DATA(&temp), &theta34);
                if(xIsAReal) {
                  convertAngle34FromTo(&theta34, currentAngularMode, AM_RADIAN);
                }
                if(real34CompareLessThan(&magnitude34, const34_0)) {
                  real34SetPositiveSign(&magnitude34);
                  real34Add(&theta34, const_pi, &theta34);
                  real34DivideRemainder(&theta34, const34_2pi, &theta34);
                }
                real34PolarToRectangular(&magnitude34, &theta34, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // theta34 in radian
              }
            }
            else {
              complex34Copy(&temp, REGISTER_COMPLEX34_DATA(REGISTER_X));
            }
          }

          fnDropY(NOPARAM);
          STACK_LIFT_ENABLE;
          refreshStack();
        }

        else if(dataTypeX == dtComplex16) {
          copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
          reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

          STACK_LIFT_ENABLE;
          if(complexMode == CM_RECTANGULAR) {
            real16Copy(REGISTER_REAL16_DATA(REGISTER_L), REGISTER_REAL16_DATA(REGISTER_X));
            liftStack();
            real16Copy(REGISTER_IMAG16_DATA(REGISTER_L), REGISTER_REAL16_DATA(REGISTER_X));
            temporaryInformation = TI_RE_IM;
          }
          else { // CM_POLAR mode
            liftStack();
            real16RectangularToPolar(REGISTER_REAL16_DATA(REGISTER_L), REGISTER_IMAG16_DATA(REGISTER_L), REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X)); // X in radians
            convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_RADIAN, currentAngularMode);
            setRegisterAngularMode(REGISTER_X, currentAngularMode);
            temporaryInformation = TI_RADIUS_THETA;
          }

          refreshStack();
        }

        else if(dataTypeX == dtComplex34) {
          copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
          reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

          STACK_LIFT_ENABLE;
          if(complexMode == CM_RECTANGULAR) {
            real34Copy(REGISTER_REAL34_DATA(REGISTER_L), REGISTER_REAL34_DATA(REGISTER_X));
            liftStack();
            reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
            real34Copy(REGISTER_IMAG34_DATA(REGISTER_L), REGISTER_REAL34_DATA(REGISTER_X));
            temporaryInformation = TI_RE_IM;
          }
          else { // CM_POLAR mode
            liftStack();
            reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
            real34RectangularToPolar(REGISTER_REAL34_DATA(REGISTER_L), REGISTER_IMAG34_DATA(REGISTER_L), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X)); // X in radians
            convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, currentAngularMode);
            setRegisterAngularMode(REGISTER_X, currentAngularMode);
            temporaryInformation = TI_RADIUS_THETA;
          }

          refreshStack();
        }

        else {
          displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "You cannot use Complex Construct with %s in X and %s in Y!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false), getDataTypeName(getRegisterDataType(REGISTER_Y), true, false)); //JM changed text referring to CC
            showInfoDialog("In function btnPressed:", errorMessage, NULL, NULL);
          #endif
        }
      }

      else if(calcMode == CM_NIM) {
        addItemToNimBuffer(item);       //JM Original CPX CC. Change KEY_CC to item to be able to add KEY_CC1. See JM CPX*
      }

      else if(calcMode == CM_FLAG_BROWSER || calcMode == CM_FONT_BROWSER || calcMode == CM_REGISTER_BROWSER) {
      }

      else {
        sprintf(errorMessage, "In function btnPressed: %" FMT8U " is an unexpected value for calcMode while processing Complex Construct function (complex closing, composing, cutting, & converting)!", calcMode); //JM Changed reference to CC
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

          STACK_LIFT_DISABLE;

          if(JM_auto_drop_enabled) {         //JM TIMER CLRDROP ON DOUBLE BACKSPACE
            fnDrop(NOPARAM);                 //JM TIMER CLRDROP ON DOUBLE BACKSPACE
            JM_auto_drop_activated = true;   //JM TIMER CLRDROP ON DOUBLE BACKSPACE
            STACK_LIFT_ENABLE;               //JM TIMER CLRDROP ON DOUBLE BACKSPACE
          }                                  //JM TIMER CLRDROP ON DOUBLE BACKSPACE
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

    else if((calcMode == CM_AIM) && (item == CHR_case) && (alphaCase == AC_LOWER)) {      //JM CASE JM CAPS
      alphaCase = AC_UPPER;                                                     //JM CASE JM CAPS
      showAlphaMode();                                                          //JM CASE JM CAPS
#ifdef PC_BUILD     //dr - new AIM
      calcModeAimGui();
#endif
      }                                                                         //JM CASE JM CAPS

    else if(item == KEY_UP) {
      if(calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_NIM) {
        if(softmenuStackPointer > 0  && softmenuStack[softmenuStackPointer - 1].softmenu != MY_ALPHA_MENU) {
          itemShift = alphaSelectionMenu == ASM_NONE ? 18 : 6;

          if((softmenuStack[softmenuStackPointer - 1].firstItem + itemShift) < softmenu[softmenuStack[softmenuStackPointer-1].softmenu].numItems) {
            softmenuStack[softmenuStackPointer - 1].firstItem += itemShift;
            showSoftmenuCurrentPart();
          }
          else {
            softmenuStack[softmenuStackPointer - 1].firstItem = 0;
            showSoftmenuCurrentPart();
          }

               if(alphaSelectionMenu == ASM_CNST) lastCnstMenuPos = softmenuStack[softmenuStackPointer - 1].firstItem;
          else if(alphaSelectionMenu == ASM_FCNS) lastFcnsMenuPos = softmenuStack[softmenuStackPointer - 1].firstItem;
        }
        else if(calcMode == CM_AIM) {
          alphaCase = AC_UPPER;
          showAlphaMode();
#ifdef PC_BUILD     //dr - new AIM
          calcModeAimGui();
#endif
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

    else if((calcMode == CM_AIM) && (item == CHR_case)  && (alphaCase == AC_UPPER)) {     //JM CASE JM CAPS
      alphaCase = AC_LOWER;                                                     //JM CASE JM CAPS
      showAlphaMode();                                                          //JM CASE JM CAPS
#ifdef PC_BUILD     //dr - new AIM
       calcModeAimGui();
#endif
     }                                                                          //JM CASE JM CAPS

    else if(item == KEY_DOWN) {
      if(calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_NIM) {
        if(softmenuStackPointer > 0 && softmenuStack[softmenuStackPointer - 1].softmenu != MY_ALPHA_MENU) {
          itemShift = alphaSelectionMenu == ASM_NONE ? 18 : 6;

          if((softmenuStack[softmenuStackPointer - 1].firstItem - itemShift) >= 0) {
            softmenuStack[softmenuStackPointer - 1].firstItem -= itemShift;
            showSoftmenuCurrentPart();
          }
          else {
            softmenuStack[softmenuStackPointer - 1].firstItem = (softmenu[softmenuStack[softmenuStackPointer-1].softmenu].numItems/6 - 1) / (itemShift/6) * itemShift; // doesn't work if numItems is not a multiple of 6
            showSoftmenuCurrentPart();
          }

               if(alphaSelectionMenu == ASM_CNST) lastCnstMenuPos = softmenuStack[softmenuStackPointer - 1].firstItem;
          else if(alphaSelectionMenu == ASM_FCNS) lastFcnsMenuPos = softmenuStack[softmenuStackPointer - 1].firstItem;
        }
        else if(calcMode == CM_AIM) {
          alphaCase = AC_LOWER;
          showAlphaMode();
#ifdef PC_BUILD     //dr - new AIM
          calcModeAimGui();
#endif
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

      else if(displayRealAsFraction) {
        displayRealAsFraction = false;
        refreshStack();
      }

      else if(calcMode == CM_NORMAL) {
        switch(getRegisterDataType(REGISTER_X)) {
          case dtLongInteger :
            convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
            refreshRegisterLine(REGISTER_X);
            break;

          case dtReal16:
            if(getRegisterAngularMode(REGISTER_X) != AM_NONE) {
              if(getRegisterAngularMode(REGISTER_X) == AM_DMS) {
                convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
              }
              setRegisterAngularMode(REGISTER_X, AM_NONE);
              refreshRegisterLine(REGISTER_X);
            }
            break;

          case dtShortInteger :
            convertShortIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
            refreshRegisterLine(REGISTER_X);
            break;

          case dtReal34:
            if(getRegisterAngularMode(REGISTER_X) == AM_NONE) {
              convertRegister34To16(REGISTER_X);
              refreshRegisterLine(REGISTER_X);
            }
            else {
              if(getRegisterAngularMode(REGISTER_X) == AM_DMS) {
                convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
              }
              setRegisterAngularMode(REGISTER_X, AM_NONE);
              refreshRegisterLine(REGISTER_X);
            }
            break;

          default :
            displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              sprintf(errorMessage, "data type %s cannot be converted to a real16!", getRegisterDataTypeName(REGISTER_X, false, false));
              showInfoDialog("In function btnPressed:", errorMessage, NULL, NULL);
            #endif
        }
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

      else if(alphaCase==AC_LOWER && ( (CHR_ALPHA<=item && item<=CHR_OMEGA) || (CHR_QOPPA<=item && item<=CHR_SAMPI) ))  {  //JM GREEK
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
          currentFlgScr = 0;
          currentFntScr = 0;
          oldTime[0] = 0;
          showDateTime();
          clearScreen(false, true, true);
          fnRecall(currentRegisterBrowserScreen);
          STACK_LIFT_ENABLE;
          refreshStack();
          currentRegisterBrowserScreen = 9999;
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
    /*if(item == CHR_3 || item == ITM_XEQ) { // Yes or XEQ*/                    // JM YN
      if(item == ITEM_CONF_Y || item == ITM_XEQ) { // Yes or XEQ                // JM YN For Layout DM42, changed "Y" on CHR_3 to ITM_SUB as the alpha character move due to operator swap
        calcMode = previousCalcMode;
        temporaryInformation = TI_NO_INFO;
        confirmedFunction(CONFIRMED);
        refreshStack();
      }

    /*else if(item == CHR_7) { // No*/                                          // JM YN
      else if(item == ITEM_CONF_N ) { // No                                     // JM YN
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

  if(testEnabled) { fnSwStop(1); }      //dr
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
  shiftF = true;

  #ifdef PC_BUILD
    btnClicked(NULL, "02");
  #endif

  #ifdef DMCP_BUILD
    btnClicked(NULL, "02");
  #endif
}
#endif // TESTSUITE_BUILD
