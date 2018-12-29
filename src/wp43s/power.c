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
 * \file power.c
 ***********************************************/

#include "wp43s.h"



void (*power[12][12])(void) = {
// regX |    regY ==>    1            2            3            4         5            6            7            8            9            10            11           12
//      V                Big integer  real16       complex16              Time         Date         String       real16 mat   complex16 m  Small integer real34       complex34
/*  1 Big integer    */ {powBigIBigI, powRe16BigI, powCo16BigI, errorPow, errorPow,    errorPow,    errorPow,    powRm16BigI, powCm16BigI, powSmaIBigI,  powRe34BigI, powCo34BigI},
/*  2 real16         */ {powBigIRe16, powRe16Re16, powCo16Re16, errorPow, errorPow,    errorPow,    errorPow,    powRm16Re16, powCm16Re16, powSmaIRe16,  powRe34Re16, powCo34Re16},
/*  3 complex16      */ {powBigICo16, powRe16Co16, powCo16Co16, errorPow, errorPow,    errorPow,    errorPow,    errorPow,    errorPow,    powSmaICo16,  powRe34Co16, powCo34Co16},
/*  4                */ {errorPow,    errorPow,    errorPow,    errorPow, errorPow,    errorPow,    errorPow,    errorPow,    errorPow,    errorPow,     errorPow,    errorPow   },
/*  5 Time           */ {errorPow,    errorPow,    errorPow,    errorPow, errorPow,    errorPow,    errorPow,    errorPow,    errorPow,    errorPow,     errorPow,    errorPow   },
/*  6 Date           */ {errorPow,    errorPow,    errorPow,    errorPow, errorPow,    errorPow,    errorPow,    errorPow,    errorPow,    errorPow,     errorPow,    errorPow   },
/*  7 String         */ {errorPow,    errorPow,    errorPow,    errorPow, errorPow,    errorPow,    errorPow,    errorPow,    errorPow,    errorPow,     errorPow,    errorPow   },
/*  8 real16 mat     */ {errorPow,    errorPow,    errorPow,    errorPow, errorPow,    errorPow,    errorPow,    errorPow,    errorPow,    errorPow,     errorPow,    errorPow   },
/*  9 complex16 mat  */ {errorPow,    errorPow,    errorPow,    errorPow, errorPow,    errorPow,    errorPow,    errorPow,    errorPow,    errorPow,     errorPow,    errorPow   },
/* 10 Small integer  */ {powBigISmaI, powRe16SmaI, powCo16SmaI, errorPow, errorPow,    errorPow,    errorPow,    powRm16SmaI, powCm16SmaI, powSmaISmaI,  powRe34SmaI, powCo34SmaI},
/* 11 real34         */ {powBigIRe34, powRe16Re34, powCo16Re34, errorPow, errorPow,    errorPow,    errorPow,    powRm16Re34, powCm16Re34, powSmaIRe34,  powRe34Re34, powCo34Re34},
/* 12 complex34      */ {powBigICo34, powRe16Co34, powCo16Co34, errorPow, errorPow,    errorPow,    errorPow,    errorPow,    errorPow   , powSmaICo34,  powRe34Co34, powCo34Co34}
};



/********************************************//**
 * \brief Data type error in power
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void errorPow(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("errorPow");
  #endif

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot raise %s", getRegisterDataTypeName(op1, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(op2, true, false));
    showInfoDialog("In function fnPower:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("errorPow");
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void powToBeCoded(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powToBeCoded");
  #endif

  #ifdef PC_BUILD
    sprintf(errorMessage, "%s", getRegisterDataTypeName(op1, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "raised to %s", getRegisterDataTypeName(op2, true, false));
    showInfoDialog("Operation to be coded:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powToBeCoded");
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regY ^ regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnPower(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnPower");
  #endif

  if(power[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)] != errorPow) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    op1    = allocateTemporaryRegister();
    op2    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_Y, op1);
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    power[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();
    freeTemporaryRegister(op1);
    freeTemporaryRegister(op2);

    fnDropY(NOPARAM);
    refreshStack();
  }
  else {
    errorPow();
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnPower");
  #endif
}



/********************************************//**
 * \brief op1(big integer) ^ op2(big integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powBigIBigI(void) {
	 int32_t exponentSign, baseSign, powerSign;
  bigInteger_t base;
  bigInteger_t exponent;

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powBigIBigI");
  #endif

  convertBigIntegerRegisterToBigInteger(op1, &base);
  convertBigIntegerRegisterToBigInteger(op2, &exponent);

  baseSign = base.sign;
  bigIntegerSetPositiveSign(&base);

  exponentSign = exponent.sign;
  bigIntegerSetPositiveSign(&exponent);

 	if(bigIntegerIsZero(&exponent) && bigIntegerIsZero(&base)) {
	   displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powBigIBigI: Cannot calculate 0^0!", NULL, NULL, NULL);
    #endif

	  	bigIntegerSetZero(&base);
	  	convertBigIntegerToBigIntegerRegister(&base, result);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("powBigIBigI");
    #endif

	  	return;
  }

 	if(bigIntegerIsZero(&exponent)) {
	  	uIntToBigInteger(1, &base);
	  	convertBigIntegerToBigIntegerRegister(&base, result);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("powBigIBigI");
    #endif

	  	return;
  }
	 else if(bigIntegerIsZero(&base) || exponentSign) {
	  	bigIntegerSetZero(&base);
	  	convertBigIntegerToBigIntegerRegister(&base, result);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("powBigIBigI");
    #endif

	  	return;
	 }

	 powerSign = (baseSign && bigIntegerIsOdd(&exponent));	// Determine the sign of the result

  // The int_power_helper function
  bigInteger_t power;
 	uIntToBigInteger(1, &power);

  for(uint32_t i=0; !bigIntegerIsZero(&exponent); i++) {
    if(bigIntegerIsOdd(&exponent)) {
     bigIntegerMultiply(&power, &base, &power);
    }

    bigIntegerDivide2(&exponent, &exponent);

    if(!bigIntegerIsZero(&exponent)) {
      bigIntegerSquare(&base, &base);
    }
  }

  if(powerSign) {
    bigIntegerSetNegativeSign(&power);
  }

  convertBigIntegerToBigIntegerRegister(&power, result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powBigIBigI");
  #endif
}



/********************************************//**
 * \brief op1(big integer) ^ op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powBigIRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powBigIRe16");
  #endif

  convertBigIntegerRegisterToReal16Register(op1, op1);
  powRe16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powBigIRe16");
  #endif
}



/********************************************//**
 * \brief op1(big integer) ^ op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powBigICo16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powBigICo16");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powBigICo16");
  #endif
}



/********************************************//**
 * \brief op1(big integer) ^ op2(64bits integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powBigISmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powBigISmaI");
  #endif

  convertSmallIntegerRegisterBigIntegerRegister(op2, op2);
  powBigIBigI();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powBigISmaI");
  #endif
}



/********************************************//**
 * \brief op1(big integer) ^ op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powBigIRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powBigIRe34");
  #endif

  convertBigIntegerRegisterToReal34Register(op1, op1);
  powRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powBigIRe34");
  #endif
}



/********************************************//**
 * \brief op1(big integer) ^ op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powBigICo34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powBigICo34");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powBigICo34");
  #endif
}



/********************************************//**
 * \brief op1(real16) ^ op2(big integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRe16BigI");
  #endif

  convertBigIntegerRegisterToReal16Register(op2, op2);
  powRe16Re16();

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRe16BigI");
  #endif
}



/********************************************//**
 * \brief op1(real16) ^ op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRe16Re16");
  #endif

  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Power(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRe16Re16");
  #endif
}



/********************************************//**
 * \brief op1(real16) ^ op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRe16Co16");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRe16Co16");
  #endif
}



/********************************************//**
 * \brief op1(real16) ^ op2(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRe16SmaI");
  #endif

  convertSmallIntegerRegisterToReal16Register(op2, op2);
  powRe16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRe16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(real16) ^ op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRe16Re34");
  #endif

  convertRegister16To34(op1);
  powRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRe16Re34");
  #endif
}



/********************************************//**
 * \brief op1(real16) ^ op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRe16Co34");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRe16Co34");
  #endif
}



/********************************************//**
 * \brief op1(complex16) ^ op2(big integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCo16BigI");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCo16BigI");
  #endif
}



/********************************************//**
 * \brief op1(complex16) ^ op2(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCo16Re16");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCo16Re16");
  #endif
}



/********************************************//**
 * \brief op1(complex16) ^ op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCo16Co16");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCo16Co16");
  #endif
}



/********************************************//**
 * \brief op1(complex16) ^ op2(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCo16SmaI");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCo16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(complex16) ^ op2(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCo16Re34");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCo16Re34");
  #endif
}



/********************************************//**
 * \brief op1(complex16) ^ op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCo16Co34");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCo16Co34");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) ^ op2(big integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRm16BigI");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRm16BigI");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) ^ op2(real16) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRm16Re16");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRm16Re16");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) ^ op2(complex16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRm16Co16");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRm16Co16");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) ^ op2(64bits integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRm16SmaI");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRm16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) ^ op2(real34) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRm16Re34");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRm16Re34");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) ^ op2(complex34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRm16Co34");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRm16Co34");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) ^ op2(big integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCm16BigI");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCm16BigI");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) ^ op2(real16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCm16Re16");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCm16Re16");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) ^ op2(complex16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCm16Co16");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCm16Co16");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) ^ op2(64bits integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCm16SmaI");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCm16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) ^ op2(real34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCm16Re34");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCm16Re34");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) ^ op2(complex34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCm16Co34");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCm16Co34");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) ^ op2(big integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powSmaIBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powSmaIBigI");
  #endif

  convertSmallIntegerRegisterBigIntegerRegister(op1, op1);
  powBigIBigI();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powSmaIBigI");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) ^ op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powSmaIRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powSmaIRe16");
  #endif

  convertSmallIntegerRegisterToReal16Register(op1, op1);
  powRe16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powSmaIRe16");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) ^ op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powSmaICo16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powSmaICo16");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powSmaICo16");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) ^ op2(64bits integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powSmaISmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powSmaISmaI");
  #endif

  reallocateRegister(result, dtSmallInteger, SMALL_INTEGER_SIZE, getRegisterBase(op1));
  *(uint64_t *)(POINTER_TO_REGISTER_DATA(result)) = WP34S_intPower(*(uint64_t *)(POINTER_TO_REGISTER_DATA(op1)), *(uint64_t *)(POINTER_TO_REGISTER_DATA(op2)));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powSmaISmaI");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) ^ op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powSmaIRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powSmaIRe34");
  #endif

  convertSmallIntegerRegisterToReal34Register(op1, op1);
  powRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powSmaIRe34");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) ^ op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powSmaICo34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powSmaICo34");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powSmaICo34");
  #endif
}



/********************************************//**
 * \brief op1(real34) ^ op2(big integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRe34BigI");
  #endif

  convertBigIntegerRegisterToReal34Register(op2, op2);
  powRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRe34BigI");
  #endif
}



/********************************************//**
 * \brief op1(real34) ^ op2(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRe34Re16");
  #endif

  convertRegister16To34(op2);
  powRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRe34Re16");
  #endif
}



/********************************************//**
 * \brief op1(real34) ^ op2(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRe34Co16");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRe34Co16");
  #endif
}



/********************************************//**
 * \brief op1(real34) ^ op2(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRe34SmaI");
  #endif

  convertSmallIntegerRegisterToReal34Register(op2, op2);
  powRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRe34SmaI");
  #endif
}



/********************************************//**
 * \brief op1(real34) ^ op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRe34Re34");
  #endif

  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Power(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRe34Re34");
  #endif
}



/********************************************//**
 * \brief op1(real34) ^ op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powRe34Co34");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powRe34Co34");
  #endif
}



/********************************************//**
 * \brief op1(complex34) ^ op2(big integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCo34BigI");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCo34BigI");
  #endif
}



/********************************************//**
 * \brief op1(complex34) ^ op2(real16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCo34Re16");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCo34Re16");
  #endif
}



/********************************************//**
 * \brief op1(complex34) ^ op2(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCo34Co16");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCo34Co16");
  #endif
}



/********************************************//**
 * \brief op1(complex34) ^ op2(64bits integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCo34SmaI");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCo34SmaI");
  #endif
}



/********************************************//**
 * \brief op1(complex34) ^ op2(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCo34Re34");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCo34Re34");
  #endif
}



/********************************************//**
 * \brief op1(complex34) ^ op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("powCo34Co34");
  #endif

  powToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("powCo34Co34");
  #endif
}
