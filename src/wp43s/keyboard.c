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

bool_t DOT_G_painted, DOT_F_painted;                                      //JM OPTIMISE the dot placement by removing the same pixels placed. No need for the full redraw of the softmenu.

void DOT_F() {
  JM_DOT( -1, 201 );                                                      //JM - Display dot in the f - line
  JM_DOT( 392, 201 );                                                     //JM - Display dot in the f - line
  DOT_F_painted = !DOT_F_painted;
}

void DOT_G() {
  JM_DOT( -1, 175-2 );                                                      //JM - Display dot in the g - line
  JM_DOT( 392, 175-2 );                                                     //JM - Display dot in the g - line
  JM_DOT( -1, 182+3 );                                                      //JM - Display dot in the g - line
  JM_DOT( 392, 182+3 );                                                     //JM - Display dot in the g - line
  DOT_G_painted = !DOT_G_painted;
}

void DOT_F_clear() {
  if(DOT_F_painted) {
    DOT_F();
  }
}

void DOT_G_clear() {
  if(DOT_G_painted) {
    DOT_G();
  }
}

void showShiftState(void) {
  if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FONT_BROWSER) {
//  if(shiftStateChanged) {                                                     //dr
      if(shiftF) {
        showGlyph(STD_SUP_f, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // f is pixel 4+8+3 wide
        //showSoftmenuCurrentPart();                                                //JM - Redraw boxes etc after shift is shown
        if(softmenuStackPointer > 0) {                                            //JM - Display dot in the f - line
          DOT_G_clear();
          DOT_F();
          if(!FN_timeouts_in_progress) {
            if(!ULFL) {
              underline(1);
              ULFL = !ULFL;
            }
            if(ULGL) {
              underline(2);
              ULGL = !ULGL;
            }
          }
        }                                                                         //JM - Display dot in the f - line
      }
      else if(shiftG) {
        showGlyph(STD_SUP_g, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // g is pixel 4+10+1 wide
      //showSoftmenuCurrentPart();                                                //JM - Redraw boxes etc after shift is shown
        if(softmenuStackPointer > 0) {                                            //JM - Display dot in the g - line
          DOT_F_clear(); //cancel dots
          DOT_G();
          if(!FN_timeouts_in_progress) {
            if(ULFL) {
              underline(1);
              ULFL = !ULFL;
            }
            if(!ULGL) {
              underline(2);
              ULGL = !ULGL;
            }
          }
        }                                                                         //JM - Display dot in the g - line
      }
      else {
        //DOT_G_clear(); //cancel dots
        refreshRegisterLine(REGISTER_T);
        showSoftmenuCurrentPart();            //JM TO REMOVE STILL !!             //JM - Redraw boxes etc after shift is shown
        DOT_G_painted = false;
        DOT_F_painted = false;

        if(TAM_REGISTER_LINE == REGISTER_T && (calcMode == CM_TAM || calcMode == CM_ASM)) {
          showString(tamBuffer, &standardFont, 25, Y_POSITION_OF_TAM_LINE + 6, vmNormal, true, true);
        }
      }

//    shiftStateChanged = false;                                                //vv dr
//  }                                                                           //^^
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
  if(shiftF || shiftG) {                                                        //vv dr
    shiftF = false;
    shiftG = false;
    showShiftState();
  }                                                                             //^^
}

//JM Changing the individual softmenu cell, based on the content of the assigned keyboard array
bool_t func_lookup(int16_t fn, int16_t itemShift, int16_t *funk) {
  int16_t ix_fn, ix_sm,ix, ix0;  //JMXXX
  bool_t tmp;
  tmp = false;

  ix = itemShift + (fn - 1);
  ix0 = softmenuStack[softmenuStackPointer - 1].firstItem;
  ix_sm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
  
  if(ix_sm == -MNU_HOME) {
    if(menu_A_HOME[ix0+ix]!=-1) {
      ix_fn = -9999; 
  
      //The problem was at this point menu_A_HOME[ix0+ix] could be -1.
      //Which means kbd_std or kbd_usr[-1] is looked up.
  
      if(menu_A_HOME[ix0+ix] < 100) {ix_fn = !userModeEnabled ? (kbd_std[menu_A_HOME[ix0+ix]    ].primary ) : (kbd_usr[menu_A_HOME[ix0+ix]    ].primary );}  else           
      if(menu_A_HOME[ix0+ix] < 200) {ix_fn = !userModeEnabled ? (kbd_std[menu_A_HOME[ix0+ix]-100].fShifted) : (kbd_usr[menu_A_HOME[ix0+ix]-100].fShifted);}  else
      if(menu_A_HOME[ix0+ix] < 300) {ix_fn = !userModeEnabled ? (kbd_std[menu_A_HOME[ix0+ix]-200].gShifted) : (kbd_usr[menu_A_HOME[ix0+ix]-200].gShifted);}  else
      if(menu_A_HOME[ix0+ix]>= 300) {ix_fn = -9999;}
      //printf("--> MNU_HOME:%d (current menu %d): first item ix0:%d + ix:%d\n",MNU_HOME, ix_sm,ix0,ix);
      //printf("    menu_A_HOME looked up key:%d menu_HOME original softkey function: %d\n", menu_A_HOME[ix0+ix], menu_HOME[ix0+ix]);
      //printf("    Function on key: %d. ", ix_fn);
      //printf(   "  Use this function: %d ", (userModeEnabled && (menu_A_HOME[ix0+ix]!=-1)) );
      //printf(   "  %s\n", indexOfItems[ix_fn].itemSoftmenuName );
  
      //Handle normal mode key change in dynamic HOME menu
      if(ix_fn != -9999 && ix == 0 && !userModeEnabled && menu_A_HOME[ix0+ix] == 0 && (calcMode == CM_NORMAL || calcMode == CM_NIM) && (Norm_Key_00_VAR != kbd_std[0].primary)){
        ix_fn = Norm_Key_00_VAR;
        tmp = true;
      }
  /*
      //Change direct entry functions from key allocation to special allocation
      if (ix_fn == ITM_toINT)  {    //  793   fnChangeBase, TM_VALUE_CHB, STD_RIGHT_ARROW "INT",                         "#"
          ix_fn = KEY_HASH;         //  1737  fnBASE_Hash,  NOPARAM, "##" STD_RIGHT_ARROW "INT", 
      } else
      if (ix_fn == KEY_dotD) {      //  1527 fnJM, 255, "", ".d",
         ix_fn = ITM_DOTDEMU;       //  1935 fnKeyDotD, NOPARAM, "Dot.d"
      }
      //printf("    Function on key: %d. Use this function: %d %s\n", ix_fn, (userModeEnabled && (menu_A_HOME[ix0+ix]!=-1)), indexOfItems[ix_fn].itemSoftmenuName );
*/  
      *funk = ix_fn;
      return ix_fn != -9999 && ((userModeEnabled) || (!userModeEnabled && tmp));
    }
    return false;
  }
  return false;
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
  int16_t ix_fn;  //JMXXX
  const softmenu_t *sm;
  //printf("Exec %d=\n",fn); //JMEXEC

  if(softmenuStackPointer > 0) {
    sm = &softmenu[softmenuStack[softmenuStackPointer - 1].softmenu];
    row = min(3, (sm->numItems + modulo(softmenuStack[softmenuStackPointer - 1].firstItem - sm->numItems, 6))/6 - softmenuStack[softmenuStackPointer - 1].firstItem/6) - 1;

    if(itemShift/6 <= row && softmenuStack[softmenuStackPointer - 1].firstItem + itemShift + (fn - 1) < sm->numItems) {
      func = (sm->softkeyItem)[softmenuStack[softmenuStackPointer - 1].firstItem + itemShift + (fn - 1)];

      ix_fn = 0;                                /*JMEXEC XXX vv*/
      if(func_lookup(fn,itemShift,&ix_fn)) {
        //printf("---%d\n",ix_fn);
        func = ix_fn;
      }                                         /*JMEXEC XXX ^^*/

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
        if(calcMode == CM_NIM && func != KEY_CC ) {
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
  else if(lastErrorCode == 0) {                  //JM FN KEYS, when no softmenu is present

    switch(fn) {
      //JM FN KEYS DIRECTLY ACCESSIBLE IF NO MENUS ARE UP
      case 1: {resetTemporaryInformation(); func = ( ITM_SIGMAMINUS ) ;} break;  //ITM_pi
      case 2: {resetTemporaryInformation(); func = ( !userModeEnabled ? (kbd_std[1].fShifted) : (kbd_usr[1].fShifted) ) ;} break;
      case 3: {resetTemporaryInformation(); func = ( !userModeEnabled ? (kbd_std[2].fShifted) : (kbd_usr[2].fShifted) ) ;} break;
      case 4: {resetTemporaryInformation(); func = ( !userModeEnabled ? (kbd_std[3].fShifted) : (kbd_usr[3].fShifted) ) ;} break;
      case 5: {resetTemporaryInformation(); func = ( !userModeEnabled ? (kbd_std[4].fShifted) : (kbd_usr[4].fShifted) ) ;} break;
      case 6: {resetTemporaryInformation(); func = ( ITM_XFACT ) ;} break;  //ITM_XTHROOT
      default:{func = 0;} break;
    }

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
  }   // JM FN KEYS ^^
}



//******************* JM LONGPRESS & JM DOUBLE CLICK START *******************************

int16_t nameFunction(int16_t fn, int16_t itemShift) {                       //JM LONGPRESS vv
  int16_t row, func;
  int16_t ix_fn;  //JMXXX
  func = 0;
  const softmenu_t *sm;

  if(softmenuStackPointer > 0) {
    sm = &softmenu[softmenuStack[softmenuStackPointer - 1].softmenu];
    row = min(3, (sm->numItems + modulo(softmenuStack[softmenuStackPointer - 1].firstItem - sm->numItems, 6))/6 - softmenuStack[softmenuStackPointer - 1].firstItem/6) - 1;

    if(itemShift/6 <= row && softmenuStack[softmenuStackPointer - 1].firstItem + itemShift + (fn - 1) < sm->numItems) {
      func = (sm->softkeyItem)[softmenuStack[softmenuStackPointer - 1].firstItem + itemShift + (fn - 1)];
/*XXX*/
      ix_fn = 0;
      if(func_lookup(fn,itemShift,&ix_fn)) {
        //printf("---%d\n",ix_fn);
        func = ix_fn;
      }
/*XXX*/

      if(func == CHR_PROD_SIGN) {
        func = (productSign == PS_CROSS ? CHR_DOT : CHR_CROSS);
      }

      if(func < 0) {
        func = - func;
      }
    }
  }

  return func % 10000;
}


//CONCEPT - actual timing was changed:

/* Switching profile, to jump to g[FN]:
                   set g
____-----_______--------->
    P    R      P

   |>              btnFnPressed:  mark time and ignore if FN_double_click is not set
        |>         btnFnReleased: if t<t0 then set FN_double_click AND prevent normal MAIN keypress
               |>  btnFnPressed:  if t<t1 AND if FN_double_click then jump start to g[FN]


Standard procedure LONGPRESS:

   | 800 | 800 | 800 |    ms     Monitored while LONGPRESSED
___+-----|-----|-----|--> NOP    Press and time out to NOP
___+---x_|_____|_____|___        Press and release within Fx time. Fx selected upon release
___+-----|--x__|_____|___        Press and hold, release within f(Fx) time. f(Fx) selected upon release
___+-----|-----|--___|___        Press and hold and hold, release within g(Fx) time. g(Fx) selected upon release
         |     |     |

At x, at time 0, wait 75 ms to see if a press happens, i.e. double click.
- After 75 ms, execute function.
- If before 75 ms, key is pressed, jump to the beginning of g(Fx) state.

     STATE #1 #2 #3 #4
___________---___---_____       STATES indicated

----------x1 x2 x3 x4           X's indicate transitions X


Detail for x (transition press/release):

STATE1  | STATE2  | STATE3
        |t=0      |t=75 ms
--------E______   |             Showing release and immediate execution at "E" (previous system, no LONGPRESS no DOUBLE click)
--------x_________E_____        Showing release at x and delayed execution "E" (if double click is to be detected)
         #########              ## shows DEAD TIME where 43C does not react to the key release that already happened
                  ^             ^ shows the point where the x command is executed if no double press recognised.

--------x_______G-|-----        Starting with key already pressed, release at x, re-press shown at 60 ms, double click registered, g-shift activated, COMMAND displayed, timing started to NOP if not released.

--------x_________E             Starting with key already pressed, release at x, no repress within 75 ms execute Fx.
--------x_________|_C---        Starting with key already pressed, release at x, re-press shown at 85 ms, timing cycle C re-started on Fx.


Double click release:
Timing is reset to start 800 ms from G again
--------x_______G---E__        Starting with key already pressed, release at x, re-press shown at 60 ms, double click registered, g-shift activated, COMMAND displayed, timing started, key released

//Summary
  //Process starts in btnFnReleased, with a FN key released.
  //  do timecheck, i.e. zero the timer
  //At the next press, i.e. btnFnPressed, check for a 5 ms < re-press < 75 ms gap, which, indicates a double press. (5 ms = debounce check).
  //Set a flag here, to let the refresh cyclic check for 75 ms and, if exceeded, execute the function

*/



//**************JM DOUBLE CLICK SUPPORT vv **********************************
void FN_cancel() {
  FN_key_pressed = 0;
  FN_timeouts_in_progress = false;
  fnTimerStop(TO_FN_LONG);                                  //dr
}



//*************----------*************------- FN KEY PRESSED -------***************-----------------
#ifdef PC_BUILD                                                           //JM LONGPRESS FN
void btnFnPressed(GtkWidget *w, gpointer data) { 
#endif
#ifdef DMCP_BUILD
void btnFnPressed(void *w, void *data) {
#endif
  bool_t exexute_double_g;
  bool_t double_click_detected;

#ifdef INLINE_TEST
  if(testEnabled) { fnSwStart(1); }     //dr
#endif

  FN_timed_out_to_RELEASE_EXEC = false;

  //Change states according to PRESS/RELEASE incoming sequence
  if(FN_state == ST_2_REL1) {
    FN_state = ST_3_PRESS2;
  }
  else {
    FN_state =  ST_1_PRESS1;
  }

  if(FN_state == ST_3_PRESS2 && fnTimerGetStatus(TO_FN_EXEC) == TMR_COMPLETED) {
    //----------------Copied here
    char charKey[3];
    underline_softkey(FN_key_pressed-38, 3, false);   //Purposely in row 3 which does not exist, just to activate the clear previous line
    sprintf(charKey, "%c", FN_key_pressed + 11);
    hideFunctionName();
    clearRegisterLine(Y_POSITION_OF_REGISTER_T_LINE - 4, REGISTER_LINE_HEIGHT); //JM FN clear the previous shift function name
    refreshRegisterLine(REGISTER_T);

    //IF 2-->3 is longer than double click time, then move back to state 1
    FN_timeouts_in_progress = false;
    FN_state = ST_1_PRESS1;
  }

  FN_key_pressed = *((char *)data) - '0' + 37;                            //to render 38-43, as per original keypress

  if(fnTimerGetStatus(TO_FN_EXEC) == TMR_RUNNING) {         //vv dr new try
    if(FN_key_pressed_last != FN_key_pressed) {
      fnTimerExec(TO_FN_EXEC);
      FN_handle_timed_out_to_EXEC = true;
      exexute_double_g = false;
    }
    else {
      FN_handle_timed_out_to_EXEC = false;
      exexute_double_g = true;
      fnTimerStop(TO_FN_EXEC);
    }
  }
  else {
    FN_handle_timed_out_to_EXEC = true;
    exexute_double_g = false;
  }                                                         //^^

  if(FN_state == ST_1_PRESS1) {
    FN_key_pressed_last = FN_key_pressed;
  }


  //**************JM DOUBLE CLICK DETECTION ******************************* // JM FN-DOUBLE
  double_click_detected = false;                                            //JM FN-DOUBLE - Dip detection flag
  if(jm_G_DOUBLETAP) {
    if(exexute_double_g) {
      if(FN_key_pressed !=0 && FN_key_pressed == FN_key_pressed_last) {     //Identified valid double press dip, the same key in rapid succession
        shiftF = false; //R_shF();                                          //JM
        shiftG = true;  //S_shG();                                          //JM
        double_click_detected = true;                                       //JM --> FORCE INTO LONGPRESS
        FN_handle_timed_out_to_EXEC = false;                //dr
      }
    }
    else {
      FN_timeouts_in_progress = false;         //still in no shift mode
      fnTimerStop(TO_FN_LONG);                              //dr
    }
  }

  //STAGE 1 AND 3 GO HERE
  //**************JM LONGPRESS ****************************************************
  if( (FN_state == ST_1_PRESS1 || FN_state == ST_3_PRESS2) && (!FN_timeouts_in_progress || double_click_detected) && FN_key_pressed != 0) {
    FN_timeouts_in_progress = true;
    fnTimerStart(TO_FN_LONG, TO_FN_LONG, JM_TO_FN_LONG);    //dr
    FN_timed_out_to_NOP = false;
    if(!shiftF && !shiftG) {
      showFunctionName(nameFunction(FN_key_pressed-37,0),0);
      underline_softkey(FN_key_pressed-38, 0, true /*dontclear at first call*/);
    }
    else if(shiftF && !shiftG) {
      showFunctionName(nameFunction(FN_key_pressed-37,6),0);
      underline_softkey(FN_key_pressed-38, 1, true /*dontclear at first call*/);
    }
    else if(!shiftF && shiftG) {
      showFunctionName(nameFunction(FN_key_pressed-37,12),0);
      underline_softkey(FN_key_pressed-38, 2, true /*dontclear at first call*/);
    }                                                                      //further shifts are done within FN_handler
  }
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStop(1); }      //dr
#endif
}



//*************----------*************------- FN KEY RELEASED -------***************-----------------
#ifdef PC_BUILD                                                             //JM LONGPRESS FN
void btnFnReleased(GtkWidget *w, gpointer data) {                           //JM LONGPRESS FN
#endif
#ifdef DMCP_BUILD
void btnFnReleased(void *w, void *data) {
#endif
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStart(2); }     //dr
#endif

  if(FN_state == ST_3_PRESS2) {
    FN_state =  ST_4_REL2;
  }
  else {
    FN_state =  ST_2_REL1;
  }

  if(jm_G_DOUBLETAP && FN_state == ST_2_REL1 && FN_handle_timed_out_to_EXEC) {
    uint8_t                      offset =  0;
    if(shiftF && !shiftG)      { offset =  6; }
    else if(!shiftF && shiftG) { offset = 12; }
    fnTimerStart(TO_FN_EXEC, FN_key_pressed + offset, JM_FN_DOUBLE_TIMER);
  }


     // **************JM LONGPRESS EXECUTE****************************************************
  char charKey[3];
  bool_t EXEC_pri;
  EXEC_pri = (FN_timeouts_in_progress && (FN_key_pressed != 0));
  // EXEC_FROM_LONGPRESS_RELEASE     EXEC_FROM_LONGPRESS_TIMEOUT  EXEC FN primary
  if( (FN_timed_out_to_RELEASE_EXEC || FN_timed_out_to_NOP || EXEC_pri ))  {                  //JM DOUBLE: If slower ON-OFF than half the limit (250 ms)
    underline_softkey(FN_key_pressed-38, 3, false);   //Purposely in row 3 which does not exist, just to activate the clear previous line
    sprintf(charKey, "%c", FN_key_pressed + 11);
    hideFunctionName();
    clearRegisterLine(Y_POSITION_OF_REGISTER_T_LINE - 4, REGISTER_LINE_HEIGHT); //JM FN clear the previous shift function name
    refreshRegisterLine(REGISTER_T);

    if(!FN_timed_out_to_NOP && fnTimerGetStatus(TO_FN_EXEC) != TMR_RUNNING) {
      btnFnClicked(w, charKey);                                             //Execute
    }
    resetShiftState();
    FN_cancel();
  }
//**************JM LONGPRESS AND JM DOUBLE ^^ *********************************************   // JM FN-DOUBLE
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStop(2); }      //dr
#endif
}
//JM btnFnClicked is called by gui.c keyPressed
//JM btnFnClicked is called by wp43s.c program_main



void execFnTimeout(uint16_t key) {                          //dr - delayed call of the primary function key
  char charKey[3];
  sprintf(charKey, "%c", key + 11);
  int16_t fn = *((char *)charKey) - '0';

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
      executeFunction(fn, 0);          //JM FN NOMENU KEYS
    }
  }
}



void shiftCutoff(uint16_t unusedParamButMandatory) {        //dr - press shift three times within one second to call HOME timer
  fnTimerStop(TO_3S_CTFF);
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
      executeFunction(fn, 0);          //JM FN NOMENU KEYS
    }
  }
}



#define stringToKeyNumber(data)         ((*((char *)data) - '0')*10 + *(((char *)data)+1) - '0')
int16_t determineItem(const char *data) {
  int16_t result;
  const calcKey_t *key;
  fnTimerExec(TO_FN_EXEC);                                  //dr execute queued fn

  //43s:  key = userModeEnabled ? (kbd_usr + (*data - '0')*10 + *(data+1) - '0') : (kbd_std + (*data - '0')*10 + *(data+1) - '0');
  key = userModeEnabled && ((calcMode == CM_NORMAL) || (calcMode == CM_NIM)) ? (kbd_usr + stringToKeyNumber(data)) : (kbd_std + stringToKeyNumber(data));    //JM Added (calcMode == CM_NORMAL) to prevent user substitution in AIM and TAM

  allowScreenUpdate = true;

  switch(key->primary) {                              //JMSHOW
    case      KEY_UP1:
    case      KEY_DOWN1: break;
    default:  SHOWregis = 9999; break;     
  }
  //printf("###\n"); //JMEXEC

  JM_auto_drop_enabled = false;                       //JM TIMER CLRDROP
  if(key->primary == KEY_BACKSPACE) {
    if(fnTimerGetStatus(TO_CL_DROP) == TMR_RUNNING) {
      JM_auto_drop_enabled = true;
    }
    fnTimerStart(TO_CL_DROP, TO_CL_DROP, JM_CLRDROP_TIMER);
  }

  fnTimerStop(TO_FG_LONG);                                  //dr
  fnTimerStop(TO_FN_LONG);                                  //dr

                                                                                                                              //JM shifts

  // JM Shift f pressed  //JM shifts change f/g to a single function key toggle to match DM42 keyboard
  // JM Inserted new section and removed old f and g key processing sections
  if(key->primary == KEY_fg && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_NIM)) {   //JM shifts
    Shft_timeouts = true;                         //JM SHIFT NEW
    fnTimerStart(TO_FG_LONG, TO_FG_LONG, JM_TO_FG_LONG);    //vv dr
    if(ShiftTimoutMode) {
      fnTimerStart(TO_FG_TIMR, TO_FG_TIMR, JM_SHIFT_TIMER); //^^
    }
    resetTemporaryInformation();
                                                                                                                              //JM shifts
    if(lastErrorCode != 0) {                                                                                                  //JM shifts
      lastErrorCode = 0;                                                                                                      //JM shifts
      refreshStack();                                                                                                         //JM shifts
    }                                                                                                                         //JM shifts

    if(ShiftTimoutMode || Home3TimerMode) {
      if(Home3TimerMode) {
        if(fnTimerGetStatus(TO_3S_CTFF) == TMR_RUNNING) {
          JM_SHIFT_HOME_TIMER1++;
          if(JM_SHIFT_HOME_TIMER1 >= 3) {
            fnTimerStop(TO_3S_CTFF);
            shiftF = false;  // Set it up, for flags to be cleared below.
            shiftG = true;
            if(HOME3) {
              if((softmenuStackPointer > 0) && (softmenuStackPointer_MEM == softmenuStackPointer)) {                          //JM shifts
                popSoftmenu();                                                                                                //JM shifts
              }
              else {
                if(calcMode == CM_AIM) {                                                                                      //JM shifts
                  showSoftmenu(NULL, -MNU_ALPHA, true);                                                                       //JM shifts //JM ALPHA-HOME  ALPHA AIM OR NIM
                }
                else {                                                                                                        //JM SHIFTS
                  showSoftmenu(NULL, -MNU_HOME, true);                                                                        //JM shifts  //JM ALPHA-HOME
                }                                                                                                             //JM shifts                                                                                                                              //JM shifts
                softmenuStackPointer_MEM = softmenuStackPointer;                                                              //JM shifts
              }
            }                                                                                                                 //JM shifts
          }
        }
        if(fnTimerGetStatus(TO_3S_CTFF) == TMR_STOPPED) {
          JM_SHIFT_HOME_TIMER1 = 1;
          fnTimerStart(TO_3S_CTFF, TO_3S_CTFF, JM_TO_3S_CTFF);        //dr
        }
      }
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
    showShiftState();                                                                                                         //JM shifts

    return ITM_NOP;

  }                                                                                                                           //JM shifts


  // Shift f pressed 
  else if(key->primary == KEY_f && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_NIM || calcMode == CM_ASM)) {
    resetTemporaryInformation();

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
      refreshStack();
    }

    fnTimerStop(TO_FG_LONG);                                //dr
    fnTimerStop(TO_FG_TIMR);                                //dr

    shiftF = !shiftF;
    shiftG = false;                     //JM no shifted menu on g-shift-key as in WP43S
//    shiftStateChanged = true; //JM

    showShiftState();

    return ITM_NOP;
  }




  // Shift g pressed 
  else if(key->primary == KEY_g && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_NIM || calcMode == CM_ASM)) {
    resetTemporaryInformation();

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
      refreshStack();
    }

    fnTimerStop(TO_FG_LONG);                                //dr
    fnTimerStop(TO_FG_TIMR);                                //dr

    shiftG = !shiftG;
    shiftF = false;                     //JM no shifted menu on g-shift-key as in WP43S
//    shiftStateChanged = true; //JM

    showShiftState();

    return ITM_NOP;
  }

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

  //JM ASSIGN - GET FUNCTION NUMBER --------------------------------------------------------------------------------
  if(JM_ASN_MODE == 32766) {            //JM Check if JM ASSIGN IS IN PROGRESS AND CAPTURE THE FUNCTION AND KEY TO BE ASSIGNED
    //printf("%d\n", determineItem(key));    //JM GET FUNCTION NUMBER: If seek is pressed, a function can be chosen and pressed.
    JM_ASN_MODE = result;        //JM The result is the function number, item number, asnd is placed in
    fnKEYSELECT();                           //JM Place in auto trigger register, ready for next keypress
    result = KEY_EXIT1;                      //JM EXIT key to exit when done and cancel shifts
  }

  //JM ASSIGN - GET KEY & ASSIGN MEMORY FUNCTION JM_ASN_MODE
                                             //JM JM_ASN_MODE contains the command to be put on a key. 0 if not active
  else if(JM_ASN_MODE != 0) {                //JM GET KEY
    fnASSIGN(JM_ASN_MODE, stringToKeyNumber(data));          //JM CHECKS FOR INVALID KEYS IN HERE
    JM_ASN_MODE = 0;                         //JM Catchall - cancel the mode once it had the opportunity to be handled. Whether handled or not.
    result = KEY_EXIT1;                       //JM EXIT key to exit when done and cancel shifts
  }

  //JM NORMKEY _ CHANGE NORMAL MODE KEY SIGMA+ TO SOMETHING ELSE vv
  else if((calcMode == CM_NORMAL || calcMode == CM_NIM) && (!userModeEnabled && !shiftF && !shiftG && ( stringToKeyNumber(data) == 0) )) {
    //printf("%d", stringToKeyNumber(data));
    result = Norm_Key_00_VAR;
  } //JM ^^
  //JM    ^^^^^^^^^^^^^^^^^^^^^^^^^^^ --------------------------------------------------------------------------------


  if(result == KEY_BACKSPACE && calcMode == CM_NORMAL ) {
        JM_auto_clstk_enabled = true;      //JM TIMER CLRDROP ON DOUBLE BACKSPACE
        fnTimerStart(TO_CL_LONG, TO_CL_LONG, JM_TO_CL_LONG);    //dr
        if(JM_auto_drop_enabled) {         //JM TIMER CLRDROP ON DOUBLE BACKSPACE
          hideFunctionName();              //JM TIMER CLRDROP ON DOUBLE BACKSPACE
          restoreStack();                  //JM TIMER CLRDROP ON DOUBLE BACKSPACE
          fnDrop(NOPARAM);                 //JM TIMER CLRDROP ON DOUBLE BACKSPACE
          result = ITM_NULL;
          fnTimerStop(TO_CL_DROP);         //JM TIMER CLRDROP ON DOUBLE BACKSPACE
          STACK_LIFT_ENABLE;               //JM TIMER CLRDROP ON DOUBLE BACKSPACE
        }                                  //JM TIMER CLRDROP ON DOUBLE BACKSPACE
  }


  if(result == CHR_PROD_SIGN) {
    result = (productSign == PS_DOT ? CHR_DOT : CHR_CROSS);
  }

  resetShiftState();

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
  btnPressed(w, data);
  btnReleased(w, data);
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
  int16_t item = determineItem((char *)data);

  if(item != ITM_NOP && item != ITM_NULL) {
    processKeyAction(item);
    if(!keyActionProcessed) {
      showFunctionName(item, 10);
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
  Shft_timeouts = false;                         //JM SHIFT NEW
  JM_auto_clstk_enabled = false;                 //JM TIMER CLRCLSTK ON LONGPRESS
  if(showFunctionNameItem != 0) {
    int16_t item = showFunctionNameItem;
    hideFunctionName();
    if(item < 0) {
      showSoftmenu(NULL, item, false);
    }
    else {
      runFunction(item);
    }
  }
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

  if(lastErrorCode != 0 && item != KEY_EXIT1 && item != KEY_BACKSPACE) {
    lastErrorCode = 0;
    refreshStack();
  }

  resetTemporaryInformation();

  if(item != ITM_ENTER && item != KEY_EXIT1 && item != KEY_CC && item != KEY_BACKSPACE && item != KEY_UP1 && item != KEY_DOWN1 && item != KEY_dotD) {
    switch(calcMode) {
      case CM_NORMAL:
        if(item == ITM_EXPONENT || item==CHR_PERIOD || (CHR_0<=item && item<=CHR_9)) {
          addItemToNimBuffer(item);
          keyActionProcessed = true;
        }
        break;

      case CM_AIM:
        if(alphaCase==AC_LOWER && (CHR_A<=item && item<=CHR_Z)) {
          addItemToBuffer(item + 26);
          keyActionProcessed = true;
        }

        else if(alphaCase==AC_LOWER && ( (CHR_ALPHA<=item && item<=CHR_OMEGA) || (CHR_QOPPA<=item && item<=CHR_SAMPI) ))  {  //JM GREEK
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
        addItemToNimBuffer(item);
        keyActionProcessed = true;
        break;

      case CM_REGISTER_BROWSER:
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
        else if(item == ITM_RS) { // same as previous if! STRANGE
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

        keyActionProcessed = true;
        break;

      case CM_FLAG_BROWSER:
      case CM_FONT_BROWSER:
      case CM_ERROR_MESSAGE:
      case CM_BUG_ON_SCREEN:
        keyActionProcessed = true;
        break;

      case CM_CONFIRMATION:
        if(item == ITEM_CONF_Y || item == ITM_XEQ) { // Yes or XEQ
          calcMode = previousCalcMode;
          temporaryInformation = TI_NO_INFO;
          confirmedFunction(CONFIRMED);
          refreshStack();
        }

        else if(item == ITEM_CONF_N || item == KEY_EXIT1) { // No
          calcMode = previousCalcMode;
          temporaryInformation = TI_NO_INFO;
          refreshStack();
        }

        keyActionProcessed = true;
        break;

      default:
        sprintf(errorMessage, "In function btnPressed: %" FMT8U " is an unexpected value while processing calcMode!", calcMode);
        displayBugScreen(errorMessage);
    }
  }
}
#endif // END IF NOT TESTSUITE_BUILD



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
      if( !eRPN ) {                                    //JM NEWERPN
        STACK_LIFT_ENABLE;

        liftStack();
        copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
        refreshStack();
        //printf("ERPN--1\n");
        STACK_LIFT_DISABLE;
      }                                               //JM NEWERPN vv
      else {
        if(stackLiftEnabled) {
          liftStack();
          copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          refreshStack();
          //printf("ERPN--2\n");
        }   
      STACK_LIFT_DISABLE;                           //JM NEWERPN (COMMENT: THESE ARE NOT NEEDED AS IT GET OVERWRITTEN BY RUNFN)
      }                                               //JM NEWERPN ^^
      break;

    case CM_AIM:
      calcModeNormal();
      while(softmenuStackPointer > 0) {            //JMMENU POP OFF ALL MENUS
        popSoftmenu();
      }
 

      if(aimBuffer[0] == 0) {
        restoreStack();
      }
      else {
        int16_t mem = stringByteLength(aimBuffer);

        reallocateRegister(REGISTER_X, dtString, mem, AM_NONE);
        memcpy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, mem + 1);

        if( !eRPN ) {                                    //JM NEWERPN
          STACK_LIFT_ENABLE;
          liftStack();
          STACK_LIFT_DISABLE;
          copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          aimBuffer[0] = 0;
          refreshStack();
        } else {
          if(stackLiftEnabled) {
          liftStack();
          STACK_LIFT_DISABLE;
          copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          aimBuffer[0] = 0;
          refreshStack();          
          }
        }
      }

      refreshStack();
      break;

    case CM_NIM:
      closeNim();

      if( !eRPN ) {                                    //JM NEWERPN
        STACK_LIFT_ENABLE;
        liftStack();
        STACK_LIFT_DISABLE;
        copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
        refreshStack();
      } else {
        if(stackLiftEnabled) {
        liftStack();
        STACK_LIFT_DISABLE;
        copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
        refreshStack();          
        }
      }
      break;

    case CM_TAM:
      tamTransitionSystem(TT_ENTER);
      break;

    case CM_ASM:
    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FONT_BROWSER:
    case CM_ERROR_MESSAGE:
    case CM_BUG_ON_SCREEN:
      break;

    case CM_CONFIRMATION:
      calcMode = previousCalcMode;
      confirmedFunction(CONFIRMED);
      refreshStack();
      break;

    default:
      sprintf(errorMessage, "In function fnKeyEnter: unexpected calcMode value (%" FMT8U ") while processing key ENTER!", calcMode);
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
        refreshStack();
      }
      else if(softmenuStackPointer > 0) {
        popSoftmenu();
      }
      break;

    case CM_AIM:
        if(/*(*/ softmenuStackPointer == 1 /*) && (softmenuStack[softmenuStackPointer-1].softmenu == MY_ALPHA_MENU)*/) {      //JM ALPHA-HOME make sure we are at the bottom of the stack
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
        while(softmenuStackPointer > 0) {         //JMMENU POP OFF ALL MENUS
          popSoftmenu();
        }
      }
      break;

    case CM_NIM:
      addItemToNimBuffer(KEY_EXIT1);
      break;

    case CM_TAM:
      calcModeNormal();
      refreshStack();
      break;

    case CM_ASM:
      calcModeNormal();
      break;

    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FONT_BROWSER:
      rbr1stDigit = true;
      calcMode = previousCalcMode;
      clearScreen(false, true, true);
      refreshStack();
      showSoftmenuCurrentPart();
      oldTime[0] = 0;
      showDateTime();
      break;

    case CM_BUG_ON_SCREEN:
      calcMode = previousCalcMode;
      clearScreen(false, true, true);
      refreshStack();
      showSoftmenuCurrentPart();
      break;

    case CM_CONFIRMATION:
      calcMode = previousCalcMode;
      temporaryInformation = TI_NO_INFO;
      refreshStack();
      break;

    default:
      sprintf(errorMessage, "In function fnKeyExit: unexpected calcMode value (%" FMT8U ") while processing key EXIT!", calcMode);
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
void fnKeyCC(uint16_t unusedParamButMandatory) {    //JM Using unusedParamButMandatory
  #ifndef TESTSUITE_BUILD
  uint32_t dataTypeX;
  uint32_t dataTypeY;
printf("Complex %d %d\n",KEY_COMPLEX,unusedParamButMandatory);
  if(calcMode == CM_NORMAL || ((calcMode == CM_NIM) && (unusedParamButMandatory == KEY_COMPLEX))) {
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
          showInfoDialog("In function btnPressed:", errorMessage, NULL, NULL);
        #endif
      }
      return;
  }

  switch(calcMode) {
    case CM_NIM:
      addItemToNimBuffer(KEY_CC);
      break;

    case CM_ASM:
      break;

    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FONT_BROWSER:
      break;

    default:
      sprintf(errorMessage, "In function fnKeyCC: unexpected calcMode value (%" FMT8U ") while processing key CC!", calcMode);
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
        refreshStack();
      }
      else {
        runFunction(ITM_CLX);
      }
      break;

    case CM_AIM:
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

    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FONT_BROWSER:
      calcMode = previousCalcMode;
      clearScreen(false, true, true);
      refreshStack();
      showSoftmenuCurrentPart();
      oldTime[0] = 0;
      showDateTime();
      break;

    case CM_BUG_ON_SCREEN:
      calcMode = previousCalcMode;
      clearScreen(false, true, true);
      refreshStack();
      showSoftmenuCurrentPart();
      break;

    case CM_CONFIRMATION:
      calcMode = previousCalcMode;
      temporaryInformation = TI_NO_INFO;
      refreshStack();
      break;

    default:
      sprintf(errorMessage, "In function fnKeyBackspace: unexpected calcMode value (%" FMT8U ") while processing key BACKSPACE!", calcMode);
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
    case CM_NORMAL:                     //JMSHOW vv
      if(softmenuStackPointer == 0) {
        fnShow_SCROLL(1);}
        break;                          //JMSHOW ^^
    case CM_AIM:
    case CM_NIM:
    case CM_ASM:
      resetAlphaSelectionBuffer();
      if(softmenuStackPointer > 0  && softmenuStack[softmenuStackPointer - 1].softmenu != MY_ALPHA_MENU) {
        int16_t sm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
        if((sm == -MNU_alpha_omega || sm == -MNU_ALPHAintl) && alphaCase == AC_LOWER) {
          alphaCase = AC_UPPER;

         if(calcMode == CM_AIM || calcMode == CM_ASM)    //vv dr
         {
           showAlphaMode();
#ifdef PC_BUILD     //dr - new AIM
           calcModeAimGui();
#endif
         }                                               //^^
         softmenuStack[softmenuStackPointer - 1].softmenu--; // Switch to the upper case menu
         showSoftmenuCurrentPart();
       }

        else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH) && alphaCase == AC_LOWER) {
          alphaCase = AC_UPPER;
          if(calcMode == CM_AIM || calcMode == CM_ASM)    //vv dr
          {
            showAlphaMode();
#ifdef PC_BUILD     //dr - new AIM
            calcModeAimGui();
#endif
          }                                               //^^
        }
        else if(item == CHR_case) {   //vv JM
          showSoftmenuCurrentPart();
          setCatalogLastPos();
        }                             //^^

        else {
          itemShift = alphaSelectionMenu == ASM_NONE ? 18 : 6;

          if((item != CHR_case) && (softmenuStack[softmenuStackPointer - 1].firstItem + itemShift) < softmenu[softmenuStack[softmenuStackPointer-1].softmenu].numItems) {         //JM
          
            softmenuStack[softmenuStackPointer - 1].firstItem += itemShift;
            //JM Include or exclude HOME menu screens  //JMHOME
              #define A1 3   //HAAKON //jm_HOME_MIR
              #define A2 9   //HAAKON
              #define B1 10  //NIGEL  //jm_HOME_SUM
              #define B2 11  //NIGEL
              #define C1 12  //JACO   //jm_HOME_FIX
              #define C2 18  //JACO
              int16_t smm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
              //printf("--1:%d %d %d menuId:%d item:%d  \n",jm_HOME_MIR,jm_HOME_SUM,jm_HOME_FIX,smm,softmenuStack[softmenuStackPointer - 1].firstItem/18);
              if (!jm_HOME_MIR && smm == -MNU_HOME && softmenuStack[softmenuStackPointer - 1].firstItem == A1*18) {softmenuStack[softmenuStackPointer - 1].firstItem = (A2+1)*18;} 
              if (!jm_HOME_SUM && smm == -MNU_HOME && softmenuStack[softmenuStackPointer - 1].firstItem == B1*18) {softmenuStack[softmenuStackPointer - 1].firstItem = (B2+1)*18;} 
              if (!jm_HOME_FIX && smm == -MNU_HOME && softmenuStack[softmenuStackPointer - 1].firstItem == C1*18) {softmenuStack[softmenuStackPointer - 1].firstItem = (C2+1)*18;}
              //smm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
              //printf(   "--2:      menuId:%d item:%d  \n",smm,softmenuStack[softmenuStackPointer - 1].firstItem/18);
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
          if(calcMode == CM_AIM || calcMode == CM_ASM)    //vv dr
          {
            showAlphaMode();
#ifdef PC_BUILD     //dr - new AIM
            calcModeAimGui();
#endif
          }                                               //^^
      }
      break;

    case CM_TAM:
      addItemToBuffer(ITM_Max);
      break;

    case CM_REGISTER_BROWSER:
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
      break;

    case CM_FLAG_BROWSER:
      currentFlgScr = 3 - currentFlgScr;
      flagBrowser(NOPARAM);
     break;

    case CM_FONT_BROWSER:
      if(currentFntScr >= 2) {
        currentFntScr--;
      }
      fontBrowser(NOPARAM);
      break;

    default:
      sprintf(errorMessage, "In function fnKeyUp: unexpected calcMode value (%" FMT8U ") while processing key UP!", calcMode);
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
    case CM_NORMAL:                     //JMSHOW vv
      if(softmenuStackPointer == 0) {
        fnShow_SCROLL(2);}
        break;                          //JMSHOW ^^
     case CM_AIM:
    case CM_NIM:
    case CM_ASM:
      resetAlphaSelectionBuffer();
      if(softmenuStackPointer > 0  && softmenuStack[softmenuStackPointer - 1].softmenu != MY_ALPHA_MENU) {
        int16_t sm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
        if((sm == -MNU_ALPHA_OMEGA || sm == -MNU_ALPHAINTL) && alphaCase == AC_UPPER) {
          alphaCase = AC_LOWER;
          if(calcMode == CM_AIM || calcMode == CM_ASM)    //vv dr
          {
            showAlphaMode();
#ifdef PC_BUILD     //dr - new AIM
            calcModeAimGui();
#endif
          }                                               //^^
          softmenuStack[softmenuStackPointer - 1].softmenu++; // Switch to the lower case menu
          showSoftmenuCurrentPart();
        }
        else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH) && alphaCase == AC_UPPER) {
          alphaCase = AC_LOWER;
          if(calcMode == CM_AIM || calcMode == CM_ASM)    //vv dr
          {
            showAlphaMode();
#ifdef PC_BUILD     //dr - new AIM
            calcModeAimGui();
#endif
          }                                               //^^
        }
        else if(item == CHR_case) {   //vvJM
          showSoftmenuCurrentPart();
          setCatalogLastPos();
        }                             //^^
        else {
          itemShift = alphaSelectionMenu == ASM_NONE ? 18 : 6;

          if((softmenuStack[softmenuStackPointer - 1].firstItem - itemShift) >= 0) {
            softmenuStack[softmenuStackPointer - 1].firstItem -= itemShift;
            //JM Include or exclude HOME menu screens  //JMHOME
            int16_t smm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
            if (!jm_HOME_FIX && smm == -MNU_HOME && softmenuStack[softmenuStackPointer - 1].firstItem == C2*18) {softmenuStack[softmenuStackPointer - 1].firstItem = (C1-1)*18;}
            if (!jm_HOME_SUM && smm == -MNU_HOME && softmenuStack[softmenuStackPointer - 1].firstItem == B2*18) {softmenuStack[softmenuStackPointer - 1].firstItem = (B1-1)*18;} 
            if (!jm_HOME_MIR && smm == -MNU_HOME && softmenuStack[softmenuStackPointer - 1].firstItem == A2*18) {softmenuStack[softmenuStackPointer - 1].firstItem = (A1-1)*18;}
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
          if(calcMode == CM_AIM || calcMode == CM_ASM)    //vv dr
          {
            showAlphaMode();
#ifdef PC_BUILD     //dr - new AIM
            calcModeAimGui();
#endif
          }                                               //^^
      }
      break;

    case CM_TAM:
      addItemToBuffer(ITM_Min);
      break;


    case CM_REGISTER_BROWSER:
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
      break;

    case CM_FLAG_BROWSER:
      currentFlgScr = 3 - currentFlgScr;
      flagBrowser(NOPARAM);
      break;

    case CM_FONT_BROWSER:
      if(currentFntScr < numScreensNumericFont+numScreensStandardFont) {
        currentFntScr++;
      }
      fontBrowser(NOPARAM);
      break;

    default:
      sprintf(errorMessage, "In function fnKeyDown: unexpected calcMode value (%" FMT8U ") while processing key DOWN!", calcMode);
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
      if(fractionType != FT_NONE) {
        fractionType = FT_NONE;
        refreshStack();
      }
      else {
        runFunction(ITM_toREAL);
      }
      break;

    case CM_NIM:
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function btnPressed:", "the data type date is to be coded!", NULL, NULL);
      #endif
      break;

    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FONT_BROWSER:
      break;

    default:
      sprintf(errorMessage, "In function fnKeyDotD: unexpected calcMode value (%" FMT8U ") while processing key .d!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}



















#ifdef XXXXXX
/******************************************************************************************************************************VVVVVVV***********************/
/**************************************************************************************************************//*******************************************************/

Check from here on



/*
    else if((calcMode == CM_AIM) && (item == CHR_case) && (alphaCase == AC_LOWER)) {      //JM CASE JM CAPS
      alphaCase = AC_UPPER;                                                     //JM CASE JM CAPS
      showAlphaMode();                                                          //JM CASE JM CAPS
#ifdef PC_BUILD     //dr - new AIM
      calcModeAimGui();
#endif
    }                                                                           //JM CASE JM CAPS
*/

    else if((item == KEY_UP1) || ((calcMode == CM_AIM) && (item == CHR_case) && (alphaCase == AC_LOWER))) {    //JM
      if(calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_NIM || calcMode == CM_ASM) {



/*
    else if((calcMode == CM_AIM) && (item == CHR_case)  && (alphaCase == AC_UPPER)) {     //JM CASE JM CAPS
      alphaCase = AC_LOWER;                                                     //JM CASE JM CAPS
      showAlphaMode();                                                          //JM CASE JM CAPS
#ifdef PC_BUILD     //dr - new AIM
      calcModeAimGui();
#endif
    }                                                                          //JM CASE JM CAPS
*/

 
 /*******************************************************/
#endif

