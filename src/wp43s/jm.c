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

/********************************************//** //JM
 * \file jm.c JM_TEST                         
 ***********************************************/

/* ADDITIONAL WP43C functions and routines */


#include "wp43s.h"


/********************************************//**
 * RESET TIME FOR SHIFT CANCELLING
 * THIS IS STANDALONE RESET FOR SHIFT TO BE SET BY EMU KEYS. IT ALSO GETS RESET IN KEYBOARD.C
 *
 * FROM keyboard.c 
 ***********************************************/

void Reset_Shift_Mem(void) {                            //JM
      #ifdef DMCP_BUILD                                 //JM TIMER DMCP SHIFTCANCEL
      now = sys_current_ms();                           //JM TIMER DMCP SHIFTCANCEL
      #endif                                            //JM
      #ifdef PC_BUILD                                   //JM TIMER EMULATOR SHIFTCANCEL
      now = g_get_monotonic_time();                     //JM usec  //JM TIMER EMULATOR SHIFTCANCEL
      #endif                                            //JM
      now_MEM = now;                                    //JM TIMER -- any last key pressed
}



/********************************************//**
 * NOT TESTED YET. NOT WORKING. CALLED FROM ## in BASE
 *
 * FROM keyboard.c 
 ***********************************************/
void fnBASE_Hash(uint16_t unusedParamButMandatory) {
      shiftF = false;       //JM
      shiftG = true;        //JM
      Reset_Shift_Mem();    //JM
//      calcMode = CM_NIM;    //JM Trying to put the calculator into Number Input Mode

  #ifdef PC_BUILD
    btnClicked(NULL, "01");
  #endif

  #ifdef DMCP_BUILD
    btnClicked(NULL, "01");
  #endif
  

//   addItemToNimBuffer(/*CHR_NUMBER_SIGN*/KEY_HASH); //Trying out different things
//The point is I am trying to do: 12 # 10, i.e. activate # while input buffer is active, like the true button.

}





/********************************************//**
 * \Set SIGFIG mode
 *
 * FROM DISPLAY.C 
 ***********************************************/
void fnDisplayFormatSigFig(uint16_t displayFormatN) {             //JM SIGFIG
   UNITDisplay = false;                                           //JM SIGFIG display Reset
   SigFigMode = displayFormatN;                                   //JM SIGFIG
//   fnInfo(SigFigMode);                                          //JM SIGFIG
   if (displayFormatN != 0) {                                     //JM SIGFIG
     fnDisplayFormatFix(displayFormatN);                          //JM SIGFIG
   }                                                              //JM SIGFIG
}                                                                 //JM SIGFIG

/********************************************//**
 * \Set UNIT mode
 *
 * FROM DISPLAY.C 
 ***********************************************/
void fnDisplayFormatUnit(uint16_t displayFormatN) {               //JM UNIT
   SigFigMode = 0;                                                //JM UNIT Sigfig works in FIX mode and it makes not sense in UNIT (ENG) mode
   UNITDisplay = true;                                            //JM UNIT display 
   //fnInfo(UNITDisplay);                                         //JM UNIT
   if (displayFormatN != 0) {                                     //JM UNIT
      //original:      fnDisplayFormatEng(displayFormatN);                          //JM UNIT
      displayFormat = DF_ENG;
      displayFormatDigits = displayFormatN;
      displayRealAsFraction = false;
      if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
        convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
      }
      #ifdef PC_BUILD
        else if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
          showInfoDialog("In function fnDisplayFormatEng:", "converting an integer to a real16", "is to be coded", NULL);
        }
      #endif
      refreshStack();
   }                                                              //JM UNIT
}                                                                 //JM UNIT


/********************************************//**   //JM LastX
 * \brief Restores Last X and refreshes the stack   //JM LastX
 *                                                  //JM LastX
 * \param[in] unusedParamButMandatory uint16_t      //JM LastX
 * \return void                                     //JM LastX
 ***********************************************/   //JM LastX
void fnLastX(uint16_t unusedParamButMandatory) {    //JM LastX
                                                    //JM LastX
  reg[REGISTER_X] = reg[REGISTER_L];                //JM LastX
  refreshRegisterLine(REGISTER_X);                  //JM LastX
}                                                   //JM LastX



/********************************************//**
 * \brief Sets/resets flag
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSeteRPN(uint16_t unusedParamButMandatory) {                //JM eRPN     Set/Reset setting to allow eRPN
   eRPN = !eRPN;                                                  //JM eRPN      
   fnInfo(eRPN);                                                  //JM eRPN 
}                                                                 //JM eRPN 

void fnSetHOME3(uint16_t unusedParamButMandatory) {               //JM HOME.3    Set/Reset setting to allow triple click for HOME
   HOME3 = !HOME3;                                                //JM HOME.3
   fnInfo(HOME3);                                                 //JM HOME.3
}                                                                 //JM HOME.3

void fnSetSetJM(uint16_t What) {                                  //JM SHIFT TIM CCL    Set/Reset setting to allow timer shift cancel
  if(What == 1) {
   eRPN = !eRPN;                                                  //JM eRPN      
   fnInfo(eRPN);                                                  //JM eRPN 
  } else
  if(What == 2) {
   HOME3 = !HOME3;                                                //JM HOME.3
   fnInfo(HOME3);                                                 //JM HOME.3
  } else
  if(What == 3) {
    SHTIM = !SHTIM;                                                //JM SHIFT TIM CCL
    fnInfo(SHTIM);                                                 //JM SHIFT TIM CCL
  }
}                                                                 //JM SHIFT TIM CCL



/********************************************//**
 * \brief Displays TRUE/FALSE information
 *
 * \param[in] Info uint16_t
 * \return void
 ***********************************************/
void fnInfo(uint16_t f) {
  if( (f == false) ) {
    temporaryInformation = TI_FALSE;
  }
  else {
    temporaryInformation = TI_TRUE;
  }

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(REGISTER_X);
}



/********************************************//**
 * \brief Sets X to the flag value
 * \param[in] What to display uint16_t
 * \return void
 ***********************************************/
void fnShowJM(uint16_t What) {
  longInteger_t mem;
  longIntegerInit(mem);
  liftStack();
  
  if(What == 1 && eRPN == true) { stringToLongInteger("1",10,mem); }
  else 
  if(What == 1 && eRPN == false) { stringToLongInteger("0",10,mem); }
  else
  if(What == 2 && HOME3 == true) { stringToLongInteger("1",10,mem); }
  else
  if(What == 2 && HOME3 == false) { stringToLongInteger("0",10,mem); }
  else
  if(What == 3 && SHTIM == true) { stringToLongInteger("1",10,mem); }
  else
  if(What == 3 && SHTIM == false) { stringToLongInteger("0",10,mem); }

  convertLongIntegerToLongIntegerRegister(mem, REGISTER_X);
  longIntegerFree(mem);

  refreshStack();
}







/********************************************//**
 * \brief Draws the dots on the margins of the f and g lines on screen
 *
 * \param[in] Info uint16_t
 * \return void
 ***********************************************/
void JM_DOT(int16_t xx, int16_t yy) {                          // To draw the dots for f/g on screen

setPixel (xx+4,yy+7);
setPixel (xx+5,yy+6);
setPixel (xx+6,yy+6);
setPixel (xx+6,yy+5);
setPixel (xx+7,yy+4);
setPixel (xx+6,yy+3);
setPixel (xx+6,yy+2);
setPixel (xx+5,yy+2);
setPixel (xx+4,yy+2);
setPixel (xx+3,yy+2);
setPixel (xx+2,yy+2);
setPixel (xx+2,yy+3);
setPixel (xx+2,yy+4);
setPixel (xx+2,yy+5);
setPixel (xx+2,yy+6);
setPixel (xx+3,yy+6);
setPixel (xx+4,yy+6);
setPixel (xx+5,yy+5);
setPixel (xx+6,yy+4);
setPixel (xx+5,yy+3);
setPixel (xx+3,yy+3);
setPixel (xx+3,yy+5);
clearPixel (xx+4,yy+7);
clearPixel (xx+5,yy+7);
clearPixel (xx+6,yy+7);
clearPixel (xx+6,yy+6);
clearPixel (xx+7,yy+6);
clearPixel (xx+7,yy+5);
clearPixel (xx+7,yy+4);
clearPixel (xx+7,yy+3);
clearPixel (xx+6,yy+2);
clearPixel (xx+6,yy+1);
clearPixel (xx+5,yy+1);
clearPixel (xx+4,yy+1);
clearPixel (xx+3,yy+1);
clearPixel (xx+2,yy+2);
clearPixel (xx+1,yy+3);
clearPixel (xx+1,yy+4);
clearPixel (xx+1,yy+5);
clearPixel (xx+1,yy+6);
clearPixel (xx+2,yy+6);
clearPixel (xx+3,yy+7);
}






/* JM UNIT********************************************//**                                            JM UNIT 
 * \brief Adds the power of 10 using numeric font to displayString                                    JM UNIT 
 *        Converts to units like m, M, k, etc.                                                        JM UNIT 
 * \param[out] displayString char*     Result string                                                  JM UNIT
 * \param[in]  exponent int32_t Power of 10 to format                                                 JM UNIT
 * \return void                                                                                       JM UNIT
 ***********************************************                                                      JM UNIT */
void exponentToUnitDisplayString(int32_t exponent, char *displayString, bool_t nimMode) {             //JM UNIT
  if(exponent == -15) { displayString[0] = ' '; displayString[1] = 'f'; displayString[2] = 0; } else  //JM UNIT
  if(exponent == -12) { displayString[0] = ' '; displayString[1] = 'p'; displayString[2] = 0; } else  //JM UNIT
  if(exponent == -9 ) { displayString[0] = ' '; displayString[1] = 'n'; displayString[2] = 0; } else  //JM UNIT
  if(exponent == -6 ) { displayString[0] = ' '; displayString[1] = STD_mu[0]; displayString[2] = STD_mu[1]; displayString[3] = 0; } else  //JM UNIT
  if(exponent == -3 ) { displayString[0] = ' '; displayString[1] = 'm'; displayString[2] = 0; } else  //JM UNIT
  if(exponent ==  3 ) { displayString[0] = ' '; displayString[1] = 'k'; displayString[2] = 0; } else  //JM UNIT
  if(exponent ==  6 ) { displayString[0] = ' '; displayString[1] = 'M'; displayString[2] = 0; } else  //JM UNIT
  if(exponent ==  9 ) { displayString[0] = ' '; displayString[1] = 'G'; displayString[2] = 0; } else  //JM UNIT
  if(exponent == 12 ) { displayString[0] = ' '; displayString[1] = 'T'; displayString[2] = 0; }        //JM UNIT
  else {                                                                                              //JM UNIT
    strcpy(displayString, PRODUCT_SIGN);                                                              //JM UNIT Below, copy of 
    displayString += 2;                                                                               //JM UNIT exponentToDisplayString in display.c
    strcpy(displayString, NUM_SUB_10);                                                                //JM UNIT
    displayString += 2;                                                                               //JM UNIT
    displayString[0] = 0;                                                                             //JM UNIT
    if(nimMode) {                                                                                     //JM UNIT
      if(exponent != 0) {                                                                             //JM UNIT
        supNumberToDisplayString(exponent, displayString, false);                                     //JM UNIT
      }                                                                                               //JM UNIT
    }                                                                                                 //JM UNIT
    else {                                                                                            //JM UNIT
      supNumberToDisplayString(exponent, displayString, false);                                       //JM UNIT
    }                                                                                                 //JM UNIT
  }                                                                                                   //JM UNIT
}                                                                                                     //JM UNIT








