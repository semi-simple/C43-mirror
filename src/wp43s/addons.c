/* This file is part of WP 43S.
 *
 * WP 43S is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WP 43S is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WP 43S.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ADDITIONAL C43 functions and routines */

/********************************************//**
 * \file addons.c
 ***********************************************/


/*

Math changes:

1. addon.c: Added fnArg_all which uses fnArg, but gives a result of 0 for a real 
   and longint input. The testSuite is not ifluenced. Not needed to modify |x|, 
   as it already works for a real and longint.
   (testSuite not in use for fnArg, therefore also not added)

2. bufferize.c: closenim: changed the default for (0 CC EXIT to 0) instead of i. 
   (testSuite not ifluenced).

3. addon.c: Added functions fnToPolar2 and fnToRect2 which uses the original 
   fnToPolar and fnToRect but first checks if X is a complex value, if it is,
   it does a POLAR or RECT command to change the display mode, and if not, 
   it calls R>P or P>R, using both standard functions.

Todo 


All the below: because both Last x and savestack does not work due to multiple steps.

  5. Added Star > Delta. Change and put in separate c file, and sort out savestack.

  6. vice versa

  7. SYM>ABC

  8. ABC>SYM

  9. e^theta. redo in math file, 

  10. three phase Ohms Law: 17,18,19


 Check for savestack in jm.c





*/





#include "wp43s.h"


void fneRPN(uint16_t state) {
  if(state == 1) eRPN = true;
  else if (state == 0) eRPN = false;
}



void fnCFGsettings(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
  runFunction(ITM_FF);
  showSoftmenu(-MNU_SYSFL);
  #endif
}



void fnClAIM(uint16_t unusedButMandatoryParameter) {  //clear input buffe
  #ifdef PC_BUILD
  jm_show_comment("^^^^fnClAIMa");
  #endif //PC_BUILD
  temporaryInformation = TI_NO_INFO;
  if(calcMode == CM_NIM) {
    strcpy(aimBuffer,"+");
    fnKeyBackspace(0);
    //printf("|%s|\n",aimBuffer);
  }
  lastIntegerBase = 0;
  #ifndef TESTSUITE_BUILD
    int8_t ix=0; while(ix<SOFTMENU_STACK_SIZE && softmenuStack[0].softmenuId != 0) {
      #ifdef PC_BUILD
      jm_show_comment("^^^^fnClAIMb");
      #endif //PC_BUILD
      popSoftmenu();
      ix++;
    }

    if(displayStack_m >=1 && displayStack_m <= 4 /*&& displayStack_m != 255*/) {                                                   //JMSHOI
      fnDisplayStack(displayStack_m);                                             //JMSHOI
      displayStack_m = 255;                                                       //JMSHOI
    } else {
      fnDisplayStack(4);  //removed because it clamps DSTACK to 4
    }                                                                             //JMSHOI

    calcModeNormal();
    refreshScreen();
    fnKeyExit(0);      //Call fnkeyExit to ensure the correct home screen is brought up, if HOME is selected.
  #endif
}


//fnArg for real and longints in addition to the standard complex. Simply returns 0 angle

void fnArg_all(uint16_t unusedButMandatoryParameter) {
longInteger_t li;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {       //JM vv add the obvious case that a number has 0/180 degrees. Why error for this.
    convertLongIntegerRegisterToLongInteger(REGISTER_X, li);
    if(longIntegerIsPositive(li) || longIntegerIsZero(li)) {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, currentAngularMode);
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
    } 
    else if(longIntegerIsNegative(li)) {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(const_180, REGISTER_REAL34_DATA(REGISTER_X));
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, currentAngularMode);
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
    }
    longIntegerFree(li);
  	adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }
  else 
  	fnArg(0);
}




void fnToPolar2(uint16_t unusedButMandatoryParameter) {
  if(getRegisterDataType(REGISTER_X) == dtComplex34) {
//    fnComplexMode(CM_POLAR);
      fnSetFlag(FLAG_POLAR);

  }
  else
    fnToPolar(0);
}



void fnToRect2(uint16_t unusedButMandatoryParameter) {
  if(getRegisterDataType(REGISTER_X) == dtComplex34) {
 //     fnComplexMode(CM_RECTANGULAR);
      fnClearFlag(FLAG_POLAR);
    }
    else
      fnToRect(0);
}



void fnRoundi2(uint16_t unusedButMandatoryParameter) {
  if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtShortInteger) {
    //nothing
  }
  else
    fnRoundi(0);
}


void fnRound2(uint16_t unusedButMandatoryParameter) {
  if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtShortInteger) {
    //nothing
  }
  else
    fnRound(0);
}


/*
* If in direct entry, accept h.ms, example 1.23 [.ms] would be 1:23:00. Do not change the ADM.
* If closed in X: and X is REAL/integer, then convert this to h.ms. Do not change the ADM.
* If closed in X: and X is already a Time in visible hms like 1:23:45, then change the time to REAL, then tag the REAL with d.ms (‘’) in the form 1°23’45.00’’. Do not change the ADM.
* if closed in X: and X is already d.ms, then convert X to time in h:ms.Do not change the ADM.
*/
// 
void fnTo_ms(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
//  uint8_t oldAngularMode1 = lastSetAngularMode;
  switch(calcMode) {                     //JM
    case CM_NIM:
      addItemToNimBuffer(ITM_ms);
      break;

    case CM_NORMAL:
      copySourceRegisterToDestRegister(REGISTER_L, TEMP_REGISTER_1);   // STO TMP

      switch(getRegisterDataType(REGISTER_X)) {                        //if integer, make a real
        case dtShortInteger :
          convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
          break;
        case dtLongInteger :
          convertLongIntegerRegisterToReal34Register (REGISTER_X, REGISTER_X);
          break;
        default : break;
      }

      if(getRegisterDataType(REGISTER_X) == dtReal34) {
        if(getRegisterAngularMode(REGISTER_X) == AM_DMS || getRegisterAngularMode(REGISTER_X) == AM_DEGREE) {
          fnKeyDotD(0);
        } 
        if(getRegisterAngularMode(REGISTER_X) == AM_NONE) {
          fnToHms(0);                   
        }

      }
      else
      if(getRegisterDataType(REGISTER_X) == dtTime) {
        fnToHr(0);
        setRegisterAngularMode(REGISTER_X, AM_DEGREE);
        fnCvtFromCurrentAngularMode(AM_DMS);
      }

      copySourceRegisterToDestRegister(TEMP_REGISTER_1, REGISTER_L);   // STO TMP
      break;



    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FONT_BROWSER:
    case CM_LISTXY:                     //JM
    case CM_GRAPH:                      //JM
      break;

      default:
        sprintf(errorMessage, "In function fnTo_ms: unexpected calcMode value (%" PRIu8 ") while processing key .ms!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // !TESTSUITE_BUILD
  }






void fnMultiplySI(uint16_t multiplier) {
    copySourceRegisterToDestRegister(REGISTER_L, TEMP_REGISTER_1);   // STO TMP
	char mult[20];
	char divi[20];
	mult[0]=0;
	divi[0]=0;
	switch (multiplier) {
		case 100 +  0: strcpy(mult,"1");                     break; //unity
		case 100 +  3: strcpy(mult, "1000");                 break; //kilo
		case 100 +  6: strcpy(mult, "1000000");              break; //mega
		case 100 +  9: strcpy(mult, "1000000000");           break; //giga
		case 100 + 12: strcpy(mult, "1000000000000");        break; //tera
		case 100 + 15: strcpy(mult, "1000000000000000");     break; //peta
		case 100 + 18: strcpy(mult, "1000000000000000000");  break; //exa
		case 100 -  3: strcpy(divi, "1000");                 break; //milli
		case 100 -  6: strcpy(divi, "1000000");              break; //micro
		case 100 -  9: strcpy(divi, "1000000000");           break; //nano
		case 100 - 12: strcpy(divi, "1000000000000");        break; //pico
		case 100 - 15: strcpy(divi, "1000000000000000");     break; //femto
		case 100 - 18: strcpy(divi, "1000000000000000000");  break; //atto
		default: break;
	}

	setSystemFlag(FLAG_ASLIFT);
	liftStack();
	longInteger_t lgInt;
	longIntegerInit(lgInt);

    if(mult[0]!=0) {
        stringToLongInteger(mult + (mult[0] == '+' ? 1 : 0), 10, lgInt);
        convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
        longIntegerFree(lgInt);
        fnMultiply(0);
    } else
    if(divi[0]!=0) {
        stringToLongInteger(divi + (divi[0] == '+' ? 1 : 0), 10, lgInt);
        convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
        longIntegerFree(lgInt);
        fnDivide(0);
    }

    adjustResult(REGISTER_X, false, false, REGISTER_X, REGISTER_Y, -1);
    copySourceRegisterToDestRegister(TEMP_REGISTER_1, REGISTER_L);   // STO TMP

}




void fn_cnst_op_j(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(const_1, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}

void fn_cnst_op_aa(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_1on2, REGISTER_REAL34_DATA(REGISTER_X));  //-0.5 - 0.866
  realToReal34(const_rt3on2, REGISTER_IMAG34_DATA(REGISTER_X));
  chsCplx();
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}

void fn_cnst_op_a(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_1on2, REGISTER_REAL34_DATA(REGISTER_X));  //-0.5 + 0.866i  : op a
  chsReal();
  realToReal34(const_rt3on2, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}

void fn_cnst_0_cpx(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));      // 0+i0
  realToReal34(const_0, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}

void fn_cnst_1_cpx(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_1, REGISTER_REAL34_DATA(REGISTER_X));      // 0+i0
  realToReal34(const_0, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}









//Rounding
void fnJM_2SI(uint16_t unusedButMandatoryParameter) {       //Convert Real to Longint; Longint to shortint; shortint to longint


  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      if(lastIntegerBase >= 2 && lastIntegerBase <= 16) {
//        fnChangeBase(lastIntegerBase);                  //This converts shortint, longint and real to shortint!
        convertLongIntegerRegisterToShortIntegerRegister(REGISTER_X, REGISTER_X);
        setRegisterShortIntegerBase(REGISTER_X, lastIntegerBase);
      }
      else {
//        fnChangeBase(10);                               //This converts shortint, longint and real to shortint!
        convertLongIntegerRegisterToShortIntegerRegister(REGISTER_X, REGISTER_X);
        setRegisterShortIntegerBase(REGISTER_X, 10);
      }
      break;
    case dtReal34:
      //ipReal();                                         //This converts real to longint!
      fnRoundi(0);
      break;
    case dtShortInteger:
      convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X); //This shortint to longint!
      lastIntegerBase = 0;                                                      //JMNIM clear lastintegerbase, to switch off hex mode
      fnRefreshState();                                                //JMNIM
      break;
    default:
      break;
  }
}






/********************************************//**
 * \Set SIGFIG mode
 *
 * FROM DISPLAY.C
 ***********************************************/
void fnDisplayFormatSigFig(uint16_t displayFormatN) {   //DONE          //JM SIGFIG
  displayFormat = DF_FIX;
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
  SigFigMode = displayFormatN;                                    //JM SIGFIG
  UNITDisplay = false;                                            //JM SIGFIG display Reset

  fnRefreshState();
}                                                                 //JM SIGFIG



/********************************************//**
 * \Set UNIT mode
 *
 * FROM DISPLAY.C
 ***********************************************/
void fnDisplayFormatUnit(uint16_t displayFormatN) {    //DONE           //JM UNIT
  displayFormat = DF_ENG;
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
  SigFigMode = 0;                                                 //JM UNIT Sigfig works in FIX mode and it makes not sense in UNIT (ENG) mode
  UNITDisplay = true;                                             //JM UNIT display

  fnRefreshState();
// Convert longint to real, to force UNIT to work. 
//  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
//    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
//  }
}                                                                 //JM UNIT



/* JM UNIT********************************************//**                                                JM UNIT
 * \brief Adds the power of 10 using numeric font to displayString                                        JM UNIT
 *        Converts to units like m, M, k, etc.                                                            JM UNIT
 * \param[out] displayString char*     Result string                                                      JM UNIT
 * \param[in]  exponent int32_t Power of 10 to format                                                     JM UNIT
 * \return void                                                                                           JM UNIT
 ***********************************************                                                          JM UNIT */
void exponentToUnitDisplayString(int32_t exponent, char *displayString, char *displayValueString, bool_t nimMode, const char *separator) {               //JM UNIT
  if     (exponent == -15) { displayString[0] = ' '; displayString[1] = 'f'; displayString[2] = 0; }    //JM UNIT
  else if(exponent == -12) { displayString[0] = ' '; displayString[1] = 'p'; displayString[2] = 0; }    //JM UNIT
  else if(exponent == -9 ) { displayString[0] = ' '; displayString[1] = 'n'; displayString[2] = 0; }    //JM UNIT
  else if(exponent == -6 ) { displayString[0] = ' '; displayString[1] = STD_mu[0]; displayString[2] = STD_mu[1]; displayString[3] = 0; }   //JM UNIT
  else if(exponent == -3 ) { displayString[0] = ' '; displayString[1] = 'm'; displayString[2] = 0; }    //JM UNIT
  else if(exponent ==  3 ) { displayString[0] = ' '; displayString[1] = 'k'; displayString[2] = 0; }    //JM UNIT
  else if(exponent ==  6 ) { displayString[0] = ' '; displayString[1] = 'M'; displayString[2] = 0; }    //JM UNIT
  else if(exponent ==  9 ) { displayString[0] = ' '; displayString[1] = 'G'; displayString[2] = 0; }    //JM UNIT
  else if(exponent == 12 ) { displayString[0] = ' '; displayString[1] = 'T'; displayString[2] = 0; }    //JM UNIT
  else {                                                                                                //JM UNIT
    strcpy(displayString, PRODUCT_SIGN);                                                                //JM UNIT Below, copy of
    displayString += 2;                                                                                 //JM UNIT exponentToDisplayString in display.c
    strcpy(displayString, STD_SUB_10);                                                                  //JM UNIT
    displayString += 2;                                                                                 //JM UNIT
    displayString[0] = 0;                                                                               //JM UNIT
    if(nimMode) {                                                                                       //JM UNIT
      if(exponent != 0) {                                                                               //JM UNIT
        supNumberToDisplayString(exponent, displayString, displayValueString, false, separator);                                 //JM UNIT
      }                                                                                                 //JM UNIT
    }                                                                                                   //JM UNIT
    else {                                                                                              //JM UNIT
      supNumberToDisplayString(exponent, displayString, displayValueString, false, separator);                                   //JM UNIT
    }                                                                                                   //JM UNIT
  }                                                                                                     //JM UNIT
}                                                                                                       //JM UNIT


//change the current state from the old state?

void fnAngularModeJM(uint16_t AMODE) {    //Setting to HMS does not change AM
  if (AMODE == TM_HMS) {
    if(getRegisterDataType(REGISTER_X) == dtTime) return;
    if(getRegisterDataType(REGISTER_X) == dtReal34 && getRegisterAngularMode(REGISTER_X) != AM_NONE) fnCvtFromCurrentAngularMode(AM_DEGREE); 
    fnKeyDotD(0);
    fnToHms(0); //covers longint & real
  } else {
    if(getRegisterDataType(REGISTER_X) == dtTime) {
      fnToHr(0);
      setRegisterAngularMode(REGISTER_X, AM_DEGREE);
      fnCvtFromCurrentAngularMode(AMODE);
      fnAngularMode(AMODE);
    }
    fnKeyDotD(0);
    fnCvtFromCurrentAngularMode(AMODE);
    fnAngularMode(AMODE);
  }
  #ifndef TESTSUITE_BUILD
    fnRefreshState();
    refreshStatusBar();
  #endif //!TESTSUITE_BUILD
}



void shrinkNimBuffer (void) {                              //JMNIM vv
  int16_t lastChar;                                        //if digits in NIMBUFFER, ensure switch to NIM,
  int16_t hexD = 0;
  bool_t  reached_end = false;
  lastChar = strlen(aimBuffer) - 1;
  if(lastChar >= 1) {
    int16_t ix = 0;
    while(aimBuffer[ix]!=0) {                              //if # found in nimbuffer, return and do nothing
      if(aimBuffer[ix] >= 'A') {
        hexD++;
      }
      if(aimBuffer[ix]=='#' || aimBuffer[ix]=='.' || reached_end) {               //chr(35) = "#"
        aimBuffer[ix]=0; 
        reached_end = true;
        //printf(">>> ***A # found. hexD=%d\n",hexD);
      }
      else {
        //printf(">>> ***B # not found in %s:%d=%d hexD=%d\n",nimBuffer,ix,nimBuffer[ix],hexD);
      }
      ix++;
    }
    if (hexD > 0) nimNumberPart = NP_INT_16;
    else nimNumberPart = NP_INT_10;
//    calcMode = CM_NIM;
  }
}                                                          //JMNIM ^^


void fnChangeBaseJM (uint16_t BASE) {
  //printf(">>> §§§ fnChangeBaseJMa Calmode:%d, nimbuffer:%s, lastbase:%d, nimnumberpart:%d\n",calcMode,nimBuffer,lastIntegerBase, nimNumberPart);
  shrinkNimBuffer();
  //printf(">>> §§§ fnChangeBaseJMb Calmode:%d, nimbuffer:%s, lastbase:%d, nimnumberpart:%d\n",calcMode,nimBuffer,lastIntegerBase, nimNumberPart);

  if(BASE == lastIntegerBase) {
    lastIntegerBase = 0;
    fnRefreshState();
  }
  else {
    fnChangeBase(BASE);
  }
  #ifndef TESTSUITE_BUILD
  nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);
  #endif
}





void fnChangeBaseMNU(uint16_t BASE) {
#ifndef TESTSUITE_BUILD
    //printf(">>> §§§ fnChangeBaseMNUa Calmode:%d, nimbuffer:%s, lastbase:%d, nimnumberpart:%d\n",calcMode,nimBuffer,lastIntegerBase, nimNumberPart);
    shrinkNimBuffer();
    //printf(">>> §§§ fnChangeBaseMNUb Calmode:%d, nimbuffer:%s, lastbase:%d, nimnumberpart:%d\n",calcMode,nimBuffer,lastIntegerBase, nimNumberPart);

    if(lastIntegerBase == 0 && calcMode == CM_NORMAL && BASE > 1 && BASE <=16) {
      //printf(">>> §§§fnChangeBaseMNc CM_NORMAL: convert non-shortint-mode to %d & return\n",BASE);
      fnChangeBaseJM(BASE);
      return;
    }

    if(calcMode == CM_NORMAL && BASE == NOPARAM) {
      //printf(">>> §§§fnChangeBaseMNd CM_NORMAL: convert non-shortint-mode to TAM\n");
      runFunction(ITM_toINT);
      return;
    }

    if(BASE > 1 && BASE <=16 ) {                                                //BASIC CONVERSION IN X REGISTER, OR DIGITS IN NIMBUFFER IF IN CM_NORMAL
      //printf(">>> §§§1 convert base to %d & return\n",BASE);
      fnChangeBaseJM(BASE);
      nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);
      return;
    }

    if(aimBuffer[0]==0 && calcMode == CM_NORMAL && BASE == NOPARAM) {           //IF # FROM MENU & TAM.
      //printf(">>> §§§2 # FROM MENU: nimBuffer[0]=0; use runfunction\n");
      runFunction(ITM_toINT);
      nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);
      return;
    }

    if(aimBuffer[0]!=0 && calcMode == CM_NIM) {                                 //IF # FROM MENU, while in NIM, just add to NimBuffer
      //printf(">>> §§§3 # nimBuffer in use, addItemToNimBuffer\n");
      addItemToNimBuffer (ITM_toINT);
      nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);
      return;
    }

#endif
}


/********************************************//**
 * \brief Set Input_Default
 *
 * \param[in] inputDefault uint16_t
 * \return void
 ***********************************************/
void fnInDefault(uint16_t inputDefault) {              //DONE
  Input_Default = inputDefault;

  if(Input_Default == ID_SI) {
    lastIntegerBase = 10;
  }
  else {
    lastIntegerBase = 0;
  }

  fnRefreshState();
}



void fnByteShortcutsS(uint16_t size) {                //JM POC BASE2 vv
  fnSetWordSize(size);
  fnIntegerMode(SIM_2COMPL);
}


void fnByteShortcutsU(uint16_t size) {
  fnSetWordSize(size);
  fnIntegerMode(SIM_UNSIGN);  
}

void fnByte(uint16_t command) {
  switch(command) {
    case 1: fnSl(1); break;
    case 2: fnSr(1); break;
    case 3: fnRl(1); break;
    case 4: fnRr(1); break;
    case 5: fnSwapEndian(16);break;            //FWORD
    case 6: fnSwapEndian(8); break;           //FBYTE
    default: break;
  }
}                                                      //JM POC BASE2 ^^







void fnP_All_Regs(uint16_t option){

  #ifndef TESTSUITE_BUILD
    create_filename(".REGS.TSV");

    #if (VERBOSE_LEVEL >= 1) 
      clearScreen();
      print_linestr("Output regs to drive:",true);
      print_linestr(filename_csv,false);          
    #endif

    switch (option)
    {
    case 0:           //All registers
      {
         stackregister_csv_out(REGISTER_X,REGISTER_D);
         stackregister_csv_out(REGISTER_L,REGISTER_K);
         stackregister_csv_out(0,99);
         //stackregister_csv_out(FIRST_LOCAL_REGISTER,FIRST_LOCAL_REGISTER+100);
      }
      break;

    case 1:           //Stack only
      {
         stackregister_csv_out(REGISTER_X,REGISTER_D);
      }
      break;

    case 2:           //Global Registers
      {
         stackregister_csv_out(0,99);
      }
      break;

    case 3:           //USER Registers
      {
         stackregister_csv_out(FIRST_LOCAL_REGISTER,LAST_LOCAL_REGISTER);
      }
      break;

    default:
      break;
    }
  #endif //TESTSUITE_BUILD
}




void printf_cpx(calcRegister_t regist) {
  #ifdef PC_BUILD
  if(getRegisterDataType(regist) == dtReal34 || getRegisterDataType(regist) == dtComplex34) {
    real34ToString(REGISTER_REAL34_DATA(regist), tmpString);
    if(strchr(tmpString, '.') == NULL && strchr(tmpString, 'E') == NULL) {
      strcat(tmpString, ".");
    }
    printf("Reg(%d) REAL = %s ",regist, tmpString);
  }
  if(getRegisterDataType(regist) == dtComplex34) {
    real34ToString(REGISTER_IMAG34_DATA(regist), tmpString);
    if(strchr(tmpString, '.') == NULL && strchr(tmpString, 'E') == NULL) {
      strcat(tmpString, ".");
    }
    printf("IMAG = %s ",tmpString);
  }
  if(getRegisterDataType(regist) != dtReal34 && getRegisterDataType(regist) != dtComplex34) printf("Neither real nor complex");
  #endif //PC_BUILD
}


void print_stck(){
  #ifdef PC_BUILD
  printf("Lasterrorcode=%d\n",lastErrorCode);
  printf("REGISTER T: ");printf_cpx(REGISTER_T);printf("\n");
  printf("REGISTER Z: ");printf_cpx(REGISTER_Z);printf("\n");
  printf("REGISTER Y: ");printf_cpx(REGISTER_Y);printf("\n");
  printf("REGISTER X: ");printf_cpx(REGISTER_X);printf("\n");
  #endif //PC_BUILD
}


void doubleToXRegisterReal34(double x) {             //Convert from double to X register REAL34
    setSystemFlag(FLAG_ASLIFT);
    liftStack();
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    snprintf(tmpString, TMP_STR_LENGTH, "%.16e", x);
    stringToReal34(tmpString, REGISTER_REAL34_DATA(REGISTER_X));
    //adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
    setSystemFlag(FLAG_ASLIFT);
}



void fnStrtoX(char aimBuffer[]) {      //DONE
  setSystemFlag(FLAG_ASLIFT);   // 5
  liftStack();
  int16_t mem = stringByteLength(aimBuffer);
  reallocateRegister(REGISTER_X, dtString, mem, AM_NONE);
  xcopy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, mem + 1);
  setSystemFlag(FLAG_ASLIFT);
}



void fnStrInputReal34(char inp1[]) {  // CONVERT STRING to REAL IN X      //DONE
  tmpString[0] = 0;
  strcat(tmpString, inp1);
  setSystemFlag(FLAG_ASLIFT);   // 5
  liftStack();
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  stringToReal34(tmpString, REGISTER_REAL34_DATA(REGISTER_X));
  setSystemFlag(FLAG_ASLIFT);
}



void fnStrInputLongint(char inp1[]) {  // CONVERT STRING to Longint X      //DONE
  tmpString[0]=0;
  strcat(tmpString, inp1);
  setSystemFlag(FLAG_ASLIFT);   // 5
  liftStack();

  longInteger_t lgInt;
  longIntegerInit(lgInt);
  stringToLongInteger(tmpString + (tmpString[0] == '+' ? 1 : 0), 10, lgInt);
  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
  longIntegerFree(lgInt);
  setSystemFlag(FLAG_ASLIFT);
}




void fnRCL(int16_t inp) {      //DONE
  setSystemFlag(FLAG_ASLIFT);
  if(inp == TEMP_REGISTER_1) {
    liftStack();
    copySourceRegisterToDestRegister(inp, REGISTER_X);
  } else {
  fnRecall(inp);
  }
}



double convert_to_double(calcRegister_t regist) {    //Convert from X register to double
  double y;
  real_t tmpy;
    doubleToXRegisterReal34(1.0);
    fnMultiply(0);
    real34ToReal(REGISTER_REAL34_DATA(regist), &tmpy);
    realToString(&tmpy, tmpString);
    y = strtof (tmpString, NULL);
    return y;
  }





