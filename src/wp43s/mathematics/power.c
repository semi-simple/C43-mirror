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



void (* const power[12][12])(void) = {
// regX |    regY ==>    1            2            3            4         5            6            7            8            9            10            11           12
//      V                Big integer  real16       complex16    angle34   Time         Date         String       real16 mat   complex16 m  Small integer real34       complex34
/*  1 Big integer    */ {powBigIBigI, powRe16BigI, powCo16BigI, powError, powError,    powError,    powError,    powRm16BigI, powCm16BigI, powSmaIBigI,  powRe34BigI, powCo34BigI},
/*  2 real16         */ {powBigIRe16, powRe16Re16, powCo16Re16, powError, powError,    powError,    powError,    powRm16Re16, powCm16Re16, powSmaIRe16,  powRe34Re16, powCo34Re16},
/*  3 complex16      */ {powBigICo16, powRe16Co16, powCo16Co16, powError, powError,    powError,    powError,    powError,    powError,    powSmaICo16,  powRe34Co16, powCo34Co16},
/*  4 angle34        */ {powError,    powError,    powError,    powError, powError,    powError,    powError,    powError,    powError,    powError,     powError,    powError   },
/*  5 Time           */ {powError,    powError,    powError,    powError, powError,    powError,    powError,    powError,    powError,    powError,     powError,    powError   },
/*  6 Date           */ {powError,    powError,    powError,    powError, powError,    powError,    powError,    powError,    powError,    powError,     powError,    powError   },
/*  7 String         */ {powError,    powError,    powError,    powError, powError,    powError,    powError,    powError,    powError,    powError,     powError,    powError   },
/*  8 real16 mat     */ {powError,    powError,    powError,    powError, powError,    powError,    powError,    powError,    powError,    powError,     powError,    powError   },
/*  9 complex16 mat  */ {powError,    powError,    powError,    powError, powError,    powError,    powError,    powError,    powError,    powError,     powError,    powError   },
/* 10 Small integer  */ {powBigISmaI, powRe16SmaI, powCo16SmaI, powError, powError,    powError,    powError,    powRm16SmaI, powCm16SmaI, powSmaISmaI,  powRe34SmaI, powCo34SmaI},
/* 11 real34         */ {powBigIRe34, powRe16Re34, powCo16Re34, powError, powError,    powError,    powError,    powRm16Re34, powCm16Re34, powSmaIRe34,  powRe34Re34, powCo34Re34},
/* 12 complex34      */ {powBigICo34, powRe16Co34, powCo16Co34, powError, powError,    powError,    powError,    powError,    powError   , powSmaICo34,  powRe34Co34, powCo34Co34}
};



/********************************************//**
 * \brief Data type error in power
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void powError(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot raise %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnPower:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void powToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "%s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "raised to %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("Operation to be coded:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
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
  if(power[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)] != powError) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opY    = allocateTemporaryRegister();
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_Y, opY);
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    power[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();
    freeTemporaryRegister(opY);
    freeTemporaryRegister(opX);

    if(lastErrorCode != 0) {
      fnDropY(NOPARAM);
    }
    else {
      restoreStack();
    }

    refreshStack();
  }
  else {
    powError();
  }
}



/********************************************//**
 * \brief opY(big integer) ^ opX(big integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powBigIBigI(void) {
	 int32_t exponentSign, baseSign, powerSign;
  bigInteger_t base;
  bigInteger_t exponent;

  convertBigIntegerRegisterToBigInteger(opY, &base);
  convertBigIntegerRegisterToBigInteger(opX, &exponent);

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
	  	return;
  }

 	if(bigIntegerIsZero(&exponent)) {
	  	uIntToBigInteger(1, &base);
	  	convertBigIntegerToBigIntegerRegister(&base, result);
	  	return;
  }
	 else if(bigIntegerIsZero(&base) || exponentSign) {
	  	bigIntegerSetZero(&base);
	  	convertBigIntegerToBigIntegerRegister(&base, result);
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
}



/********************************************//**
 * \brief opY(big integer) ^ opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powBigIRe16(void) {
  convertBigIntegerRegisterToReal16Register(opY, opY);
  powRe16Re16();
}



/********************************************//**
 * \brief opY(big integer) ^ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powBigICo16(void) {
  convertBigIntegerRegisterToReal16Register(opY, opY);
  powRe16Co16();
}



/********************************************//**
 * \brief opY(big integer) ^ opX(64bits integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powBigISmaI(void) {
  convertSmallIntegerRegisterBigIntegerRegister(opX, opX);
  powBigIBigI();
}



/********************************************//**
 * \brief opY(big integer) ^ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powBigIRe34(void) {
  convertBigIntegerRegisterToReal34Register(opY, opY);
  powRe34Re34();
}



/********************************************//**
 * \brief opY(big integer) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powBigICo34(void) {
  convertBigIntegerRegisterToReal34Register(opY, opY);
  powRe34Co34();
}



/********************************************//**
 * \brief opY(real16) ^ opX(big integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16BigI(void) {
  convertBigIntegerRegisterToReal16Register(opX, opX);
  powRe16Re16();
}



/********************************************//**
 * \brief opY(real16) ^ opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16Re16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Power(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(real16) ^ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16Co16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(real16) ^ opX(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16SmaI(void) {
  convertSmallIntegerRegisterToReal16Register(opX, opX);
  powRe16Re16();
}



/********************************************//**
 * \brief opY(real16) ^ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16Re34(void) {
  convertRegister16To34(opY);
  powRe34Re34();
}



/********************************************//**
 * \brief opY(real16) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16Co34(void) {
  convertRegister16To34(opY);
  powRe34Co34();
}



/********************************************//**
 * \brief opY(complex16) ^ opX(big integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16BigI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16BigI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(complex16) ^ opX(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16Re16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(complex16) ^ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16Co16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(complex16) ^ opX(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16SmaI(void) {
  convertSmallIntegerRegisterToReal16Register(opX, opX);
  powCo16Re16();
}



/********************************************//**
 * \brief opY(complex16) ^ opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Re34(void) {
  convertRegister16To34(opY);
  powCo34Re34();
}



/********************************************//**
 * \brief opY(complex16) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Co34(void) {
  convertRegister16To34(opY);
  powCo34Co34();
}



/********************************************//**
 * \brief opY(real16 matrix) ^ opX(big integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16BigI(void) {
  powToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ^ opX(real16) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRm16Re16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ^ opX(complex16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRm16Co16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ^ opX(64bits integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16SmaI(void) {
  powToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ^ opX(real34) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRm16Re34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ^ opX(complex34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRm16Co34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ^ opX(big integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16BigI(void) {
  powToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ^ opX(real16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCm16Re16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ^ opX(complex16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCm16Co16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ^ opX(64bits integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16SmaI(void) {
  powToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ^ opX(real34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCm16Re34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ^ opX(complex34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCm16Co34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(big integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powSmaIBigI(void) {
  convertSmallIntegerRegisterBigIntegerRegister(opY, opY);
  powBigIBigI();
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powSmaIRe16(void) {
  convertSmallIntegerRegisterToReal16Register(opY, opY);
  powRe16Re16();
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powSmaICo16(void) {
  convertSmallIntegerRegisterToReal16Register(opY, opY);
  powRe16Co16();
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(64bits integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powSmaISmaI(void) {
  reallocateRegister(result, dtSmallInteger, SMALL_INTEGER_SIZE, getRegisterBase(opY));
  *(REGISTER_SMALL_INTEGER_DATA(result)) = WP34S_intPower(*(REGISTER_SMALL_INTEGER_DATA(opY)), *(REGISTER_SMALL_INTEGER_DATA(opX)));
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powSmaIRe34(void) {
  convertSmallIntegerRegisterToReal34Register(opY, opY);
  powRe34Re34();
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powSmaICo34(void) {
  powToBeCoded();
}



/********************************************//**
 * \brief opY(real34) ^ opX(big integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34BigI(void) {
  convertBigIntegerRegisterToReal34Register(opX, opX);
  powRe34Re34();
}



/********************************************//**
 * \brief opY(real34) ^ opX(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34Re16(void) {
  convertRegister16To34(opX);
  powRe34Re34();
}



/********************************************//**
 * \brief opY(real34) ^ opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34Co16(void) {
  convertRegister16To34(opX);
  powRe34Co34();
}



/********************************************//**
 * \brief opY(real34) ^ opX(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34SmaI(void) {
  convertSmallIntegerRegisterToReal34Register(opX, opX);
  powRe34Re34();
}



/********************************************//**
 * \brief opY(real34) ^ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34Re34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real34) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34Co34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(complex34) ^ opX(big integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34BigI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34BigI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(complex34) ^ opX(real16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Re16(void) {
  convertRegister16To34(opX);
  powCo34Re34();
}



/********************************************//**
 * \brief opY(complex34) ^ opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Co16(void) {
  convertRegister16To34(opX);
  powCo34Co34();
}



/********************************************//**
 * \brief opY(complex34) ^ opX(64bits integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34SmaI(void) {
  convertSmallIntegerRegisterToReal34Register(opX, opX);
  powCo34Re34();
}



/********************************************//**
 * \brief opY(complex34) ^ opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34Re34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}



/********************************************//**
 * \brief opY(complex34) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34Co34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
}
