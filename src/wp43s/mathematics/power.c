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



void (* const power[13][13])(void) = {
// regX |    regY ==>   1            2            3            4            5         6         7         8            9            10            11           12           13
//      V               Long integer Real16       Complex16    Angle16      Time      Date      String    Real16 mat   Complex16 m  Short integer Real34       Complex34    Angle34
/*  1 Long integer  */ {powLonILonI, powRe16LonI, powCo16LonI, powAn16LonI, powError, powError, powError, powRm16LonI, powCm16LonI, powShoILonI,  powRe34LonI, powCo34LonI, powAn34LonI},
/*  2 Real16        */ {powLonIRe16, powRe16Re16, powCo16Re16, powAn16Re16, powError, powError, powError, powRm16Re16, powCm16Re16, powShoIRe16,  powRe34Re16, powCo34Re16, powAn34Re16},
/*  3 Complex16     */ {powLonICo16, powRe16Co16, powCo16Co16, powAn16Co16, powError, powError, powError, powRm16Co16, powCm16Co16, powShoICo16,  powRe34Co16, powCo34Co16, powAn34Co16},
/*  4 Angle16       */ {powLonIAn16, powRe16An16, powCo16An16, powAn16An16, powError, powError, powError, powRm16An16, powCm16An16, powShoIAn16,  powRe34An16, powCo34An16, powAn34An16},
/*  5 Time          */ {powError,    powError,    powError,    powError,    powError, powError, powError, powError,    powError,    powError,     powError,    powError,    powError   },
/*  6 Date          */ {powError,    powError,    powError,    powError,    powError, powError, powError, powError,    powError,    powError,     powError,    powError,    powError   },
/*  7 String        */ {powError,    powError,    powError,    powError,    powError, powError, powError, powError,    powError,    powError,     powError,    powError,    powError   },
/*  8 Real16 mat    */ {powError,    powError,    powError,    powError,    powError, powError, powError, powError,    powError,    powError,     powError,    powError,    powError   },
/*  9 Complex16 mat */ {powError,    powError,    powError,    powError,    powError, powError, powError, powError,    powError,    powError,     powError,    powError,    powError   },
/* 10 Short integer */ {powLonIShoI, powRe16ShoI, powCo16ShoI, powAn16ShoI, powError, powError, powError, powRm16ShoI, powCm16ShoI, powShoIShoI,  powRe34ShoI, powCo34ShoI, powAn34ShoI},
/* 11 Real34        */ {powLonIRe34, powRe16Re34, powCo16Re34, powAn16Re34, powError, powError, powError, powRm16Re34, powCm16Re34, powShoIRe34,  powRe34Re34, powCo34Re34, powAn34Re34},
/* 12 Complex34     */ {powLonICo34, powRe16Co34, powCo16Co34, powAn16Co34, powError, powError, powError, powRm16Co34, powCm16Co34, powShoICo34,  powRe34Co34, powCo34Co34, powAn34Co34},
/* 13 Angle34       */ {powLonIAn34, powRe16An34, powCo16An34, powAn16An34, powError, powError, powError, powRm16An34, powCm16An34, powShoIAn34,  powRe34An34, powCo34An34, powAn34An34}
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

  adjustResult(result, true, true, opX, opY, -1);
}



/******************************************************************************************************************************************************************************************/
/* long integer ^ ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

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
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonIRe16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal16Register(opY, opY);
  real16Power(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(real16) ^ opX(long integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16LonI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Power(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(long integer) ^ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonICo16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34Ln(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));

  // X = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opX), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opX), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
}



/********************************************//**
 * \brief opY(complex16) ^ opX(long integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16LonI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34_t magnitude34, theta34;
  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // Y = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opY));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opY), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opY), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
}



/********************************************//**
 * \brief opY(long integer) ^ opX(angle16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonIAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonIRe16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal16Register(opY, opY);
  setRegisterDataType(result, dtReal16, TAG_NONE);
  real16Power(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(angle16) ^ opX(long integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16LonI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Power(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(long integer) ^ opX(64bits integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonIShoI(void) {
	 int32_t exponentSign, baseSign, powerSign;
  longInteger_t base;
  longInteger_t exponent;

  convertShortIntegerRegisterLongIntegerRegister(opX, opX);
  convertLongIntegerRegisterToLongInteger(opY, &base);
  convertLongIntegerRegisterToLongInteger(opX, &exponent);

  baseSign = base.sign;
  longIntegerSetPositiveSign(&base);

  exponentSign = exponent.sign;
  longIntegerSetPositiveSign(&exponent);

 	if(longIntegerIsZero(&exponent) && longIntegerIsZero(&base)) {
	   displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonIShoI: Cannot calculate 0^0!", NULL, NULL, NULL);
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
 * \brief opY(64bits integer) ^ opX(long integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoILonI(void) {
	 int32_t exponentSign, baseSign, powerSign;
  longInteger_t base;
  longInteger_t exponent;

  convertShortIntegerRegisterLongIntegerRegister(opY, opY);
  convertLongIntegerRegisterToLongInteger(opY, &base);
  convertLongIntegerRegisterToLongInteger(opX, &exponent);

  baseSign = base.sign;
  longIntegerSetPositiveSign(&base);

  exponentSign = exponent.sign;
  longIntegerSetPositiveSign(&exponent);

 	if(longIntegerIsZero(&exponent) && longIntegerIsZero(&base)) {
	   displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoILonI: Cannot calculate 0^0!", NULL, NULL, NULL);
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
 * \brief opY(long integer) ^ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonIRe34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) ^ opX(long integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34LonI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(long integer) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonICo34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);

  // Y = ln(Y)
  real34Ln(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));

  // X = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opX), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opX), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(complex34) ^ opX(long integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34LonI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34_t magnitude34, theta34;
  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // Y = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opY));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opY), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opY), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(long integer) ^ opX(angle34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonIAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonIAn34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  setRegisterDataType(result, dtReal34, TAG_NONE);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(angle34) ^ opX(long integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34LonI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/******************************************************************************************************************************************************************************************/
/* real16 ^ ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

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

  convertRegister16To34(opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34Ln(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));

  // X = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opX), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opX), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
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

  convertRegister16To34(opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34_t magnitude34, theta34;
  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // Y = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opY));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opY), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opY), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
}



/********************************************//**
 * \brief opY(real16) ^ opX(angle16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16An16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  setRegisterDataType(result, dtReal16, TAG_NONE);
  real16Power(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(angle16) ^ opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16Re16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  real16Power(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(real16) ^ opX(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16ShoI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Power(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoIRe16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opY, opY);
  real16Power(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(real16) ^ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16Re34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) ^ opX(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34Re16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real16) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16Co34(void) {
  convertRegister16To34(opY);
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16Co34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);

  // Y = ln(Y)
  real34Ln(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));

  // X = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opX), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opX), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(complex34) ^ opX(real16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34Re16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34_t magnitude34, theta34;
  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // Y = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opY));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opY), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opY), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(real16) ^ opX(angle34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16An34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  setRegisterDataType(result, dtReal34, TAG_NONE);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(angle34) ^ opX(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34Re16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/******************************************************************************************************************************************************************************************/
/* complex16 ^ ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

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

  convertRegister16To34(opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34_t magnitude34, theta34;

  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // result = X * ln(Y)
  // imaginary part
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result));
  real34FMA(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result), REGISTER_IMAG34_DATA(result));
  // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  real34FMA(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(result));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(result), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(result), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
}



/********************************************//**
 * \brief opY(complex16) ^ opX(angle16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16An16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34_t magnitude34, theta34;
  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // Y = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opY));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opY), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opY), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
}



/********************************************//**
 * \brief opY(angle16) ^ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16Co16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }


  convertRegister16To34(opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34Ln(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));

  // X = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opX), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opX), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
}



/********************************************//**
 * \brief opY(complex16) ^ opX(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16ShoI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  convertShortIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34_t magnitude34, theta34;
  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // Y = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opY));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opY), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opY), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoICo16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opY, opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34Ln(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));

  // X = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opX), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opX), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
}



/********************************************//**
 * \brief opY(complex16) ^ opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16Re34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34_t magnitude34, theta34;
  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // Y = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opY));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opY), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opY), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) ^ opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34Co16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34Ln(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));

  // X = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opX), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opX), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(complex16) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16Co34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);

  // Y = ln(Y)
  real34_t magnitude34, theta34;

  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // result = X * ln(Y)
  // imaginary part
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result));
  real34FMA(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result), REGISTER_IMAG34_DATA(result));
  // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  real34FMA(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(result));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(result), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(result), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(complex34) ^ opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34Co16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34_t magnitude34, theta34;

  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // result = X * ln(Y)
  // imaginary part
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result));
  real34FMA(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result), REGISTER_IMAG34_DATA(result));
  // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  real34FMA(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(result));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(result), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(result), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(complex16) ^ opX(angle34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16An34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34_t magnitude34, theta34;
  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // Y = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opY));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opY), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opY), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(angle34) ^ opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34Co16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34Ln(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));

  // X = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opX), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opX), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/******************************************************************************************************************************************************************************************/
/* angle16 ^ ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(angle16) ^ opX(angle16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16An16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  setRegisterDataType(result, dtReal16, TAG_NONE);
  real16Power(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(angle16) ^ opX(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16ShoI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Power(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(angle16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoIAn16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opY, opY);
  setRegisterDataType(result, dtReal16, TAG_NONE);
  real16Power(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(angle16) ^ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16Re34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) ^ opX(angle16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function real34IsNaN:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(angle16) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16Co34(void) {
  convertRegister16To34(opY);
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16Co34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);

  // Y = ln(Y)
  real34Ln(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));

  // X = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opX), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opX), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(complex34) ^ opX(angle16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34An16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34_t magnitude34, theta34;
  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // Y = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opY));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opY), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opY), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(angle16) ^ opX(angle34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16An34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  setRegisterDataType(result, dtReal34, TAG_NONE);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(angle34) ^ opX(angle16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34An16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/******************************************************************************************************************************************************************************************/
/* time ^ ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/******************************************************************************************************************************************************************************************/
/* date ^ ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/******************************************************************************************************************************************************************************************/
/* string ^ ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/******************************************************************************************************************************************************************************************/
/* real16 matrix ^ ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(real16 matrix) ^ opX(long integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16LonI(void) {
  fnToBeCoded();
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

  fnToBeCoded();
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

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ^ opX(angle16) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRm16An16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ^ opX(64bits integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16ShoI(void) {
  fnToBeCoded();
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

  fnToBeCoded();
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

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ^ opX(angle34) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRm16An34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* complex16 matrix ^ ...                                                                                                                                                                 */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(complex16 matrix) ^ opX(long integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16LonI(void) {
  fnToBeCoded();
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

  fnToBeCoded();
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

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ^ opX(angle16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCm16An16:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ^ opX(64bits integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16ShoI(void) {
  fnToBeCoded();
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

  fnToBeCoded();
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

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ^ opX(angle34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCm16An34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* short integer ^ ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(64bits integer) ^ opX(64bits integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIShoI(void) {
  setRegisterDataType(result, dtShortInteger, getRegisterTag(opY));
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_intPower(*(REGISTER_SHORT_INTEGER_DATA(opY)), *(REGISTER_SHORT_INTEGER_DATA(opX)));
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoIRe34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opY, opY);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) ^ opX(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34ShoI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoICo34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opY, opY);

  // Y = ln(Y)
  real34Ln(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));

  // X = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opX), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opX), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(complex34) ^ opX(64bits integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34ShoI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34_t magnitude34, theta34;
  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // Y = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opY));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opY), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opY), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(64bits integer) ^ opX(angle34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoIAn34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opY, opY);
  setRegisterDataType(result, dtReal34, TAG_NONE);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(angle34) ^ opX(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34ShoI:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/******************************************************************************************************************************************************************************************/
/* real34 ^ ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

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

  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
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

  // Y = ln(Y)
  real34Ln(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));

  // X = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opX), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opX), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
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

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34_t magnitude34, theta34;
  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // Y = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opY));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opY), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opY), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) ^ opX(angle34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34An34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  setRegisterDataType(result, dtReal34, TAG_NONE);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(angle34) ^ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34Re34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/******************************************************************************************************************************************************************************************/
/* complex34 + ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

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

  // Y = ln(Y)
  real34_t magnitude34, theta34;

  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // result = X * ln(Y)
  // imaginary part
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result));
  real34FMA(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result), REGISTER_IMAG34_DATA(result));
  // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  real34FMA(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(result));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(result), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(result), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(complex34) ^ opX(angle34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34An34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Y = ln(Y)
  real34_t magnitude34, theta34;
  real34RectangularToPolar(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opY), &magnitude34, &theta34);
  real34Ln(&magnitude34, REGISTER_REAL34_DATA(opY));
  real34Copy(&theta34, REGISTER_IMAG34_DATA(opY));

  // Y = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opY));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opY), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opY), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(angle34) ^ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34Co34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  // Y = ln(Y)
  real34Ln(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opY));

  // X = X * ln(Y)
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX));

  // result = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(opX), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opX), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}



/******************************************************************************************************************************************************************************************/
/* angle34 + ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(angle34) ^ opX(angle34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34An34:", "cannot use NaN as an input of ^", NULL, NULL);
    #endif
    return;
  }

  setRegisterDataType(result, dtReal34, TAG_NONE);
  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}
