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


//fnArg for real and longints in addition to the standard complex. Simply returns 0 angle

void fnArg_all(uint16_t unusedParamButMandatory) {

  if(getRegisterDataType(REGISTER_X) == dtReal34 || getRegisterDataType(REGISTER_X) == dtLongInteger) {       //JM vv add the obvious case that a number has 0 degrees. Why error for this.

	reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, currentAngularMode);
	realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));

	adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }
  else 
  	fnArg(0);
}



void fnToPolar2(uint16_t unusedParamButMandatory) {
  //saveStack();       //Not savestack because fnToPolar already has savestack and fnComplexMode does not need.
  if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    fnComplexMode(CM_POLAR);
  }
  else
    fnToPolar(0);
}



void fnToRect2(uint16_t unusedParamButMandatory) {
  //saveStack();       //Not savestack because fnToRect already has savestack and fnComplexMode does not need.
  if(getRegisterDataType(REGISTER_X) == dtComplex34) {
      fnComplexMode(CM_RECTANGULAR);
    }
    else
      fnToRect(0);
}





void fnTo_ms(uint16_t unusedParamButMandatory) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_L, TEMP_REGISTER);   // STO TMP

    if(getRegisterDataType(REGISTER_X) == dtShortInteger) {convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);}
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)  {convertLongIntegerRegisterToReal34Register (REGISTER_X, REGISTER_X);}
    if(getRegisterDataType(REGISTER_X) == dtReal34) {
      if(getRegisterAngularMode(REGISTER_X) == AM_NONE) {setRegisterAngularMode(REGISTER_X, currentAngularMode);}
      if(getRegisterAngularMode(REGISTER_X) == AM_NONE) {printf("Still ADM AM_NONE. Something is wrong\n");}

      if(getRegisterAngularMode(REGISTER_X) != AM_DMS /*&& getRegisterAngularMode(REGISTER_X) != AM_HMS*/) {
        #ifndef TESTSUITE_BUILD
          runFunction(ITM_toDMS);
        #endif
      }
      else 
      {
/*    if(getRegisterAngularMode(REGISTER_X) == AM_DMS ) {   //JM wait for future HMS
        runFunction(ITM_toHMS); break;
      }
      else { */
      #ifndef TESTSUITE_BUILD
        switch (getRegisterAngularMode(REGISTER_X)) {
          case AM_DEGREE: {runFunction(ITM_DEGto);  } break;
          case AM_DMS   : {runFunction(ITM_DMSto);  } break;
          case AM_GRAD  : {runFunction(ITM_GRADto); } break;
          case AM_RADIAN: {runFunction(ITM_RADto);  } break;
          case AM_MULTPI: {runFunction(ITM_MULPIto);} break;
          default: break;
        }
      #endif
      }
    }
    copySourceRegisterToDestRegister(TEMP_REGISTER, REGISTER_L);   // STO TMP
    refreshRegisterLine(REGISTER_X);
  }






void fnMultiplySI(uint16_t multiplier) {
    //saveStack();  not needed as this is included in mult and div
    copySourceRegisterToDestRegister(REGISTER_L, TEMP_REGISTER);   // STO TMP
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

	STACK_LIFT_ENABLE;
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

    adjustResult(REGISTER_X, true, false, REGISTER_X, REGISTER_Y, -1);
    copySourceRegisterToDestRegister(TEMP_REGISTER, REGISTER_L);   // STO TMP
    refreshRegisterLine(REGISTER_X);

}




void fn_cnst_op_j(uint16_t unusedParamButMandatory) {
  saveStack();
  STACK_LIFT_ENABLE;
  liftStack();
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(const_1, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  refreshStack();
}

void fn_cnst_op_aa(uint16_t unusedParamButMandatory) {
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_1on2, REGISTER_REAL34_DATA(REGISTER_X));  //-0.5 - 0.866
  realToReal34(const_rt3on2, REGISTER_IMAG34_DATA(REGISTER_X));
  fnChangeSign(ITM_CHS);
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  refreshStack();
}

void fn_cnst_op_a(uint16_t unusedParamButMandatory) {
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_1on2, REGISTER_REAL34_DATA(REGISTER_X));  //-0.5 + 0.866i  : op a
  fnChangeSign(ITM_CHS);
  realToReal34(const_rt3on2, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  refreshStack();
}

void fn_cnst_0_cpx(uint16_t unusedParamButMandatory) {
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));      // 0+i0
  realToReal34(const_0, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  refreshStack();
}

void fn_cnst_1_cpx(uint16_t unusedParamButMandatory) {
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_1, REGISTER_REAL34_DATA(REGISTER_X));      // 0+i0
  realToReal34(const_0, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  refreshStack();
}



