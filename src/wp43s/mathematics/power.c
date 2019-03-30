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
//      V                Long integer real16       complex16    angle34   Time         Date         String       real16 mat   complex16 m  Short integer real34       complex34
/*  1 Long integer   */ {powLonILonI, powRe16LonI, powCo16LonI, powError, powError,    powError,    powError,    powRm16LonI, powCm16LonI, powShoILonI,  powRe34LonI, powCo34LonI},
/*  2 real16         */ {powLonIRe16, powRe16Re16, powCo16Re16, powError, powError,    powError,    powError,    powRm16Re16, powCm16Re16, powShoIRe16,  powRe34Re16, powCo34Re16},
/*  3 complex16      */ {powLonICo16, powRe16Co16, powCo16Co16, powError, powError,    powError,    powError,    powError,    powError,    powShoICo16,  powRe34Co16, powCo34Co16},
/*  4 angle34        */ {powError,    powError,    powError,    powError, powError,    powError,    powError,    powError,    powError,    powError,     powError,    powError   },
/*  5 Time           */ {powError,    powError,    powError,    powError, powError,    powError,    powError,    powError,    powError,    powError,     powError,    powError   },
/*  6 Date           */ {powError,    powError,    powError,    powError, powError,    powError,    powError,    powError,    powError,    powError,     powError,    powError   },
/*  7 String         */ {powError,    powError,    powError,    powError, powError,    powError,    powError,    powError,    powError,    powError,     powError,    powError   },
/*  8 real16 mat     */ {powError,    powError,    powError,    powError, powError,    powError,    powError,    powError,    powError,    powError,     powError,    powError   },
/*  9 complex16 mat  */ {powError,    powError,    powError,    powError, powError,    powError,    powError,    powError,    powError,    powError,     powError,    powError   },
/* 10 Short integer  */ {powLonIShoI, powRe16ShoI, powCo16ShoI, powError, powError,    powError,    powError,    powRm16ShoI, powCm16ShoI, powShoIShoI,  powRe34ShoI, powCo34ShoI},
/* 11 real34         */ {powLonIRe34, powRe16Re34, powCo16Re34, powError, powError,    powError,    powError,    powRm16Re34, powCm16Re34, powShoIRe34,  powRe34Re34, powCo34Re34},
/* 12 complex34      */ {powLonICo34, powRe16Co34, powCo16Co34, powError, powError,    powError,    powError,    powError,    powError   , powShoICo34,  powRe34Co34, powCo34Co34}
};



/********************************************//**
 * \brief Data type error in power
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void powError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
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

  if(lastErrorCode == 0) {
    fnDropY(NOPARAM);
  }
  else {
    restoreStack();
  }

  refreshStack();
}



/********************************************//**
 * \brief opY(long integer) ^ opX(long integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonILonI(void) {
	 int32_t exponentSign, baseSign, powerSign;
  longInteger_t base;
  longInteger_t exponent;

  convertLongIntegerRegisterToLongInteger(opY, &base);
  convertLongIntegerRegisterToLongInteger(opX, &exponent);

  baseSign = base.sign;
  longIntegerSetPositiveSign(&base);

  exponentSign = exponent.sign;
  longIntegerSetPositiveSign(&exponent);

 	if(longIntegerIsZero(&exponent) && longIntegerIsZero(&base)) {
	   displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonILonI: Cannot calculate 0^0!", NULL, NULL, NULL);
    #endif

	  	longIntegerSetZero(&base);
	  	convertLongIntegerToLongIntegerRegister(&base, result);
	  	return;
  }

 	if(longIntegerIsZero(&exponent)) {
	  	uIntToLongInteger(1, &base);
	  	convertLongIntegerToLongIntegerRegister(&base, result);
	  	return;
  }
	 else if(longIntegerIsZero(&base) || exponentSign) {
	  	longIntegerSetZero(&base);
	  	convertLongIntegerToLongIntegerRegister(&base, result);
	  	return;
	 }

	 powerSign = (baseSign && longIntegerIsOdd(&exponent));	// Determine the sign of the result

  // The int_power_helper function
  longInteger_t power;
 	uIntToLongInteger(1, &power);

  for(uint32_t i=0; !longIntegerIsZero(&exponent); i++) {
    if(longIntegerIsOdd(&exponent)) {
     longIntegerMultiply(&power, &base, &power);
    }

    longIntegerDivideUInt(&exponent, 2, &exponent);

    if(!longIntegerIsZero(&exponent)) {
      longIntegerSquare(&base, &base);
    }
  }

  if(powerSign) {
    longIntegerSetNegativeSign(&power);
  }

  convertLongIntegerToLongIntegerRegister(&power, result);
}



/********************************************//**
 * \brief opY(long integer) ^ opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonIRe16(void) {
  convertLongIntegerRegisterToReal16Register(opY, opY);
  powRe16Re16();
}



/********************************************//**
 * \brief opY(long integer) ^ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonICo16(void) {
  convertLongIntegerRegisterToReal16Register(opY, opY);
  powRe16Co16();
}



/********************************************//**
 * \brief opY(long integer) ^ opX(64bits integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonIShoI(void) {
  convertShortIntegerRegisterLongIntegerRegister(opX, opX);
  powLonILonI();
}



/********************************************//**
 * \brief opY(long integer) ^ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonIRe34(void) {
  convertLongIntegerRegisterToReal34Register(opY, opY);
  powRe34Re34();
}



/********************************************//**
 * \brief opY(long integer) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonICo34(void) {
  convertLongIntegerRegisterToReal34Register(opY, opY);
  powRe34Co34();
}



/********************************************//**
 * \brief opY(real16) ^ opX(long integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16LonI(void) {
  convertLongIntegerRegisterToReal16Register(opX, opX);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16Co16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
  fnSetFlag(FLAG_CPXRES);
}



/********************************************//**
 * \brief opY(real16) ^ opX(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16ShoI(void) {
  convertShortIntegerRegisterToReal16Register(opX, opX);
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
 * \brief opY(complex16) ^ opX(long integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16LonI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
  fnSetFlag(FLAG_CPXRES);
}



/********************************************//**
 * \brief opY(complex16) ^ opX(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16Re16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
  fnSetFlag(FLAG_CPXRES);
}



/********************************************//**
 * \brief opY(complex16) ^ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16Co16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
  fnSetFlag(FLAG_CPXRES);
}



/********************************************//**
 * \brief opY(complex16) ^ opX(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16ShoI(void) {
  convertShortIntegerRegisterToReal16Register(opX, opX);
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
 * \brief opY(real16 matrix) ^ opX(long integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16LonI(void) {
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRm16Co16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
  fnSetFlag(FLAG_CPXRES);
}



/********************************************//**
 * \brief opY(real16 matrix) ^ opX(64bits integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16ShoI(void) {
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRm16Co34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
  fnSetFlag(FLAG_CPXRES);
}



/********************************************//**
 * \brief opY(complex16 matrix) ^ opX(long integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16LonI(void) {
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCm16Co16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
  fnSetFlag(FLAG_CPXRES);
}



/********************************************//**
 * \brief opY(complex16 matrix) ^ opX(64bits integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16ShoI(void) {
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCm16Co34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
  fnSetFlag(FLAG_CPXRES);
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(long integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoILonI(void) {
  convertShortIntegerRegisterLongIntegerRegister(opY, opY);
  powLonILonI();
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIRe16(void) {
  convertShortIntegerRegisterToReal16Register(opY, opY);
  powRe16Re16();
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoICo16(void) {
  convertShortIntegerRegisterToReal16Register(opY, opY);
  powRe16Co16();
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(64bits integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIShoI(void) {
  reallocateRegister(result, dtShortInteger, SHORT_INTEGER_SIZE, getRegisterBase(opY));
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_intPower(*(REGISTER_SHORT_INTEGER_DATA(opY)), *(REGISTER_SHORT_INTEGER_DATA(opX)));
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIRe34(void) {
  convertShortIntegerRegisterToReal34Register(opY, opY);
  powRe34Re34();
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoICo34(void) {
  powToBeCoded();
  fnSetFlag(FLAG_CPXRES);
}



/********************************************//**
 * \brief opY(real34) ^ opX(long integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34LonI(void) {
  convertLongIntegerRegisterToReal34Register(opX, opX);
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
void powRe34ShoI(void) {
  convertShortIntegerRegisterToReal34Register(opX, opX);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34Co34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
  fnSetFlag(FLAG_CPXRES);
}



/********************************************//**
 * \brief opY(complex34) ^ opX(long integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34LonI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
  fnSetFlag(FLAG_CPXRES);
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
void powCo34ShoI(void) {
  convertShortIntegerRegisterToReal34Register(opX, opX);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34Re34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
  fnSetFlag(FLAG_CPXRES);
}



/********************************************//**
 * \brief opY(complex34) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34Co34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  powToBeCoded();
  fnSetFlag(FLAG_CPXRES);
}
