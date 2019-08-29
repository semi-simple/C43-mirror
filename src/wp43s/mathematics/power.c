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
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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

  power[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}



/**********************************************************************
 * In all the functions below:
 * if Y is a number then Y = a + ib
 * if X is a number then X = c + id
 * The variables a, b, c and d are used for intermediate calculations
 ***********************************************************************/

/******************************************************************************************************************************************************************************************/
/* long integer ^ ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) ^ X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonILonI(void) {
	 int32_t exponentSign, baseSign;
  longInteger_t base, exponent;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, base);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, exponent);

  baseSign = longIntegerSign(base);
  longIntegerSetPositiveSign(base);

  exponentSign = longIntegerSign(exponent);
  longIntegerSetPositiveSign(exponent);

 	if(longIntegerIsZero(exponent) && longIntegerIsZero(base)) {
	   displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonILonI: Cannot calculate 0^0!", NULL, NULL, NULL);
    #endif

    longIntegerFree(base);
    longIntegerFree(exponent);
	  	return;
  }

 	if(longIntegerIsZero(exponent)) {
	  	uIntToLongInteger(1, base);
	  	convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
	  	return;
  }
	 else if(longIntegerIsZero(base)) {
	  	uIntToLongInteger(0, base);
	  	convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
	  	return;
	 }
	 else if(exponentSign == -1) {
	  	uIntToLongInteger(0, base);
	  	convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
	  	return;
	 }

  longInteger_t power;

  longIntegerInit(power);
 	uIntToLongInteger(1, power);

  for(uint32_t i=0; !longIntegerIsZero(exponent); i++) {
    if(longIntegerIsOdd(exponent)) {
     longIntegerMultiply(power, base, power);
    }

    longIntegerDivideUInt(exponent, 2, exponent);

    if(!longIntegerIsZero(exponent)) {
      longIntegerSquare(base, base);
    }
  }

  if(baseSign == -1 && longIntegerIsOdd(exponent)) {
    longIntegerSetNegativeSign(power);
  }

  convertLongIntegerToLongIntegerRegister(power, REGISTER_X);

  longIntegerFree(power);
  longIntegerFree(base);
  longIntegerFree(exponent);
}



/********************************************//**
 * \brief Y(long integer) ^ X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonIRe16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  realIcPower(&a, &c, &c);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) ^ X(long integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16LonI:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) ^ X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonICo16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c, d;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &d);

  // ln(a) --> a
  WP34S_Ln(&a, &a);

  // (c + di) * ln(a) --> c +di
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  // exp((c + di) * ln(a)) --> c +di
  realIcExp(&c, &a);
  realIcPolarToRectangular(const_1, &d, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) ^ X(long integer) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16LonI:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);

  // ln(a + bi) --> a + bi
  realIcRectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // c * ln(a + bi) -- > a + bi
  realIcMultiply(&c, &a, &a);
  realIcMultiply(&c, &b, &b);

  // exp(c * ln(a + bi)) -- > c + di
  realIcExp(&a, &a);
  realIcPolarToRectangular(const_1, &b, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) ^ X(angle16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonIAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonIAn16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle16) ^ X(long integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16LonI:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) ^ X(64bits integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonIShoI(void) {
	 int32_t exponentSign, baseSign;
  longInteger_t base, exponent;

  convertShortIntegerRegisterLongIntegerRegister(REGISTER_X, REGISTER_X);
  convertLongIntegerRegisterToLongInteger(REGISTER_Y, base);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, exponent);

  baseSign = longIntegerSign(base);
  longIntegerSetPositiveSign(base);

  exponentSign = longIntegerSign(exponent);
  longIntegerSetPositiveSign(exponent);

 	if(longIntegerIsZero(exponent) && longIntegerIsZero(base)) {
	   displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonIShoI: Cannot calculate 0^0!", NULL, NULL, NULL);
    #endif

    longIntegerFree(base);
    longIntegerFree(exponent);
	  	return;
  }

 	if(longIntegerIsZero(exponent)) {
	  	uIntToLongInteger(1, base);
	  	convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
	  	return;
  }
	 else if(longIntegerIsZero(base)) {
	  	uIntToLongInteger(0, base);
	  	convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
	  	return;
	 }
	 else if(exponentSign == -1) {
	  	uIntToLongInteger(0, base);
	  	convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
	  	return;
	 }

  longInteger_t power;

  longIntegerInit(power);
 	uIntToLongInteger(1, power);

  for(uint32_t i=0; !longIntegerIsZero(exponent); i++) {
    if(longIntegerIsOdd(exponent)) {
     longIntegerMultiply(power, base, power);
    }

    longIntegerDivideUInt(exponent, 2, exponent);

    if(!longIntegerIsZero(exponent)) {
      longIntegerSquare(base, base);
    }
  }

  if(baseSign == -1 && longIntegerIsOdd(exponent)) {
    longIntegerSetNegativeSign(power);
  }

  convertLongIntegerToLongIntegerRegister(power, REGISTER_X);

  longIntegerFree(power);
  longIntegerFree(base);
  longIntegerFree(exponent);
}



/********************************************//**
 * \brief Y(64bits integer) ^ X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoILonI(void) {
	 int32_t exponentSign, baseSign;
  longInteger_t base, exponent;

  convertShortIntegerRegisterLongIntegerRegister(REGISTER_Y, REGISTER_Y);
  convertLongIntegerRegisterToLongInteger(REGISTER_Y, base);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, exponent);

  baseSign = longIntegerSign(base);
  longIntegerSetPositiveSign(base);

  exponentSign = longIntegerSign(exponent);
  longIntegerSetPositiveSign(exponent);

 	if(longIntegerIsZero(exponent) && longIntegerIsZero(base)) {
	   displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoILonI: Cannot calculate 0^0!", NULL, NULL, NULL);
    #endif

    longIntegerFree(base);
    longIntegerFree(exponent);
	  	return;
  }

 	if(longIntegerIsZero(exponent)) {
	  	uIntToLongInteger(1, base);
	  	convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
	  	return;
  }
	 else if(longIntegerIsZero(base)) {
	  	uIntToLongInteger(0, base);
	  	convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
	  	return;
	 }
	 else if(exponentSign == -1) {
	  	uIntToLongInteger(0, base);
	  	convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
	  	return;
	 }

  longInteger_t power;

  longIntegerInit(power);
 	uIntToLongInteger(1, power);

  for(uint32_t i=0; !longIntegerIsZero(exponent); i++) {
    if(longIntegerIsOdd(exponent)) {
     longIntegerMultiply(power, base, power);
    }

    longIntegerDivideUInt(exponent, 2, exponent);

    if(!longIntegerIsZero(exponent)) {
      longIntegerSquare(base, base);
    }
  }

  if(baseSign == -1 && longIntegerIsOdd(exponent)) {
    longIntegerSetNegativeSign(power);
  }

  convertLongIntegerToLongIntegerRegister(power, REGISTER_X);

  longIntegerFree(power);
  longIntegerFree(base);
  longIntegerFree(exponent);
}



/********************************************//**
 * \brief Y(long integer) ^ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonIRe34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) ^ X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34LonI:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) ^ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonICo34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c, d;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  // ln(a) --> a
  WP34S_Ln(&a, &a);

  // (c + di) * ln(a) --> c +di
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  // exp((c + di) * ln(a)) --> c +di
  realIcExp(&c, &a);
  realIcPolarToRectangular(const_1, &d, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ^ X(long integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34LonI:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // ln(a + bi) --> a + bi
  realIcRectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // c * ln(a + bi) -- > a + bi
  realIcMultiply(&c, &a, &a);
  realIcMultiply(&c, &b, &b);

  // exp(c * ln(a + bi)) -- > c + di
  realIcExp(&a, &a);
  realIcPolarToRectangular(const_1, &b, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) ^ X(angle34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonIAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonIAn34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle34) ^ X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34LonI:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real16 ^ ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16) ^ X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16Re16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16Re16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  realIcPower(&a, &c, &c);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) ^ X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16Co16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16Co16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c, d;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &d);

  // ln(a) --> a
  WP34S_Ln(&a, &a);

  // (c + di) * ln(a) --> c +di
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  // exp((c + di) * ln(a)) --> c +di
  realIcExp(&c, &a);
  realIcPolarToRectangular(const_1, &d, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) ^ X(real16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16Re16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16Re16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);

  // ln(a + bi) --> a + bi
  realIcRectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // c * ln(a + bi) -- > a + bi
  realIcMultiply(&c, &a, &a);
  realIcMultiply(&c, &b, &b);

  // exp(c * ln(a + bi)) -- > c + di
  realIcExp(&a, &a);
  realIcPolarToRectangular(const_1, &b, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) ^ X(angle16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16An16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16An16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle16) ^ X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16Re16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16Re16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  realIcPower(&a, &c, &c);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) ^ X(64bits integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16ShoI:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(64bits integer) ^ X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoIRe16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  realIcPower(&a, &c, &c);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) ^ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16Re34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16Re34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) ^ X(real16) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34Re16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34Re16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) ^ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16Co34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16Co34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c, d;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  // ln(a) --> a
  WP34S_Ln(&a, &a);

  // (c + di) * ln(a) --> c +di
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  // exp((c + di) * ln(a)) --> c +di
  realIcExp(&c, &a);
  realIcPolarToRectangular(const_1, &d, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ^ X(real16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34Re16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34Re16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // ln(a + bi) --> a + bi
  realIcRectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // c * ln(a + bi) -- > a + bi
  realIcMultiply(&c, &a, &a);
  realIcMultiply(&c, &b, &b);

  // exp(c * ln(a + bi)) -- > c + di
  realIcExp(&a, &a);
  realIcPolarToRectangular(const_1, &b, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) ^ X(angle34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16An34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe16An34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle34) ^ X(real16) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34Re16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34Re16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* complex16 ^ ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16) ^ X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16Co16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16Co16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }


  realIc_t a, b, c, d, theta;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &d);

  realIcRectangularToPolar(&a, &b, &a, &theta);
  WP34S_Ln(&a, &a);

  realIcMultiply(&a, &d, &b);
  realIcFMA(&theta, &c, &b, &b);
  realIcChangeSign(&theta);
  realIcMultiply(&a, &c, &a);
  realIcFMA(&theta, &d, &a, &a);

  realIcExp(&a, &c);
  realIcPolarToRectangular(const_1, &b, &a, &b);
  realIcMultiply(&c, &b, &d);
  realIcMultiply(&c, &a, &c);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) ^ X(angle16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16An16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16An16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);

  // ln(a + bi) --> a + bi
  realIcRectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // c * ln(a + bi) -- > a + bi
  realIcMultiply(&c, &a, &a);
  realIcMultiply(&c, &b, &b);

  // exp(c * ln(a + bi)) -- > c + di
  realIcExp(&a, &a);
  realIcPolarToRectangular(const_1, &b, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle16) ^ X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16Co16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16Co16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c, d;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &d);

  // ln(a) --> a
  WP34S_Ln(&a, &a);

  // (c + di) * ln(a) --> c +di
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  // exp((c + di) * ln(a)) --> c +di
  realIcExp(&c, &a);
  realIcPolarToRectangular(const_1, &d, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) ^ X(64bits integer) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16ShoI:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);

  // ln(a + bi) --> a + bi
  realIcRectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // c * ln(a + bi) -- > a + bi
  realIcMultiply(&c, &a, &a);
  realIcMultiply(&c, &b, &b);

  // exp(c * ln(a + bi)) -- > c + di
  realIcExp(&a, &a);
  realIcPolarToRectangular(const_1, &b, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(64bits integer) ^ X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoICo16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c, d;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &d);

  // ln(a) --> a
  WP34S_Ln(&a, &a);

  // (c + di) * ln(a) --> c +di
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  // exp((c + di) * ln(a)) --> c +di
  realIcExp(&c, &a);
  realIcPolarToRectangular(const_1, &d, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) ^ X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16Re34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16Re34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // ln(a + bi) --> a + bi
  realIcRectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // c * ln(a + bi) -- > a + bi
  realIcMultiply(&c, &a, &a);
  realIcMultiply(&c, &b, &b);

  // exp(c * ln(a + bi)) -- > c + di
  realIcExp(&a, &a);
  realIcPolarToRectangular(const_1, &b, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) ^ X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34Co16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34Co16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c, d;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &d);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // ln(a) --> a
  WP34S_Ln(&a, &a);

  // (c + di) * ln(a) --> c +di
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  // exp((c + di) * ln(a)) --> c +di
  realIcExp(&c, &a);
  realIcPolarToRectangular(const_1, &d, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) ^ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16Co34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16Co34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d, theta;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  realIcRectangularToPolar(&a, &b, &a, &theta);
  WP34S_Ln(&a, &a);

  realIcMultiply(&a, &d, &b);
  realIcFMA(&theta, &c, &b, &b);
  realIcChangeSign(&theta);
  realIcMultiply(&a, &c, &a);
  realIcFMA(&theta, &d, &a, &a);

  realIcExp(&a, &c);
  realIcPolarToRectangular(const_1, &b, &a, &b);
  realIcMultiply(&c, &b, &d);
  realIcMultiply(&c, &a, &c);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ^ X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34Co16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34Co16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d, theta;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &d);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  realIcRectangularToPolar(&a, &b, &a, &theta);
  WP34S_Ln(&a, &a);

  realIcMultiply(&a, &d, &b);
  realIcFMA(&theta, &c, &b, &b);
  realIcChangeSign(&theta);
  realIcMultiply(&a, &c, &a);
  realIcFMA(&theta, &d, &a, &a);

  realIcExp(&a, &c);
  realIcPolarToRectangular(const_1, &b, &a, &b);
  realIcMultiply(&c, &b, &d);
  realIcMultiply(&c, &a, &c);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) ^ X(angle34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16An34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo16An34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // ln(a + bi) --> a + bi
  realIcRectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // c * ln(a + bi) -- > a + bi
  realIcMultiply(&c, &a, &a);
  realIcMultiply(&c, &b, &b);

  // exp(c * ln(a + bi)) -- > c + di
  realIcExp(&a, &a);
  realIcPolarToRectangular(const_1, &b, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle34) ^ X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34Co16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34Co16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c, d;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &d);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // ln(a) --> a
  WP34S_Ln(&a, &a);

  // (c + di) * ln(a) --> c +di
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  // exp((c + di) * ln(a)) --> c +di
  realIcExp(&c, &a);
  realIcPolarToRectangular(const_1, &d, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* angle16 ^ ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(angle16) ^ X(angle16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16An16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16An16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);

  realIcPower(&a, &c, &c);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle16) ^ X(64bits integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16ShoI:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(64bits integer) ^ X(angle16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoIAn16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle16) ^ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16Re34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16Re34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) ^ X(angle16) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34An16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34An16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle16) ^ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16Co34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16Co34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c, d;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  // ln(a) --> a
  WP34S_Ln(&a, &a);

  // (c + di) * ln(a) --> c +di
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  // exp((c + di) * ln(a)) --> c +di
  realIcExp(&c, &a);
  realIcPolarToRectangular(const_1, &d, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ^ X(angle16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34An16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34An16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // ln(a + bi) --> a + bi
  realIcRectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // c * ln(a + bi) -- > a + bi
  realIcMultiply(&c, &a, &a);
  realIcMultiply(&c, &b, &b);

  // exp(c * ln(a + bi)) -- > c + di
  realIcExp(&a, &a);
  realIcPolarToRectangular(const_1, &b, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle16) ^ X(angle34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16An34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn16An34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);

  realIcPower(&a, &c, &c);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle34) ^ X(angle16) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34An16:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34An16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);

  realIcPower(&a, &c, &c);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
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
 * \brief Y(real16 matrix) ^ X(long integer) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16LonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ^ X(real16) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRm16Re16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ^ X(complex16) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRm16Co16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ^ X(angle16) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRm16An16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ^ X(64bits integer) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16ShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ^ X(real34) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRm16Re34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ^ X(complex34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRm16Co34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ^ X(angle34) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRm16An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRm16An34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* complex16 matrix ^ ...                                                                                                                                                                 */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16 matrix) ^ X(long integer) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16LonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ^ X(real16) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCm16Re16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ^ X(complex16) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCm16Co16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ^ X(angle16) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCm16An16:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ^ X(64bits integer) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16ShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ^ X(real34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCm16Re34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ^ X(complex34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCm16Co34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ^ X(angle34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCm16An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCm16An34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* short integer ^ ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(64bits integer) ^ X(64bits integer) ==> X(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIShoI(void) {
  setRegisterShortIntegerBase(REGISTER_X, getRegisterShortIntegerBase(REGISTER_Y));
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intPower(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



/********************************************//**
 * \brief Y(64bits integer) ^ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoIRe34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) ^ X(64bits integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34ShoI:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(64bits integer) ^ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoICo34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c, d;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  // ln(a) --> a
  WP34S_Ln(&a, &a);

  // (c + di) * ln(a) --> c +di
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  // exp((c + di) * ln(a)) --> c +di
  realIcExp(&c, &a);
  realIcPolarToRectangular(const_1, &d, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ^ X(64bits integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34ShoI:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // ln(a + bi) --> a + bi
  realIcRectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // c * ln(a + bi) -- > a + bi
  realIcMultiply(&c, &a, &a);
  realIcMultiply(&c, &b, &b);

  // exp(c * ln(a + bi)) -- > c + di
  realIcExp(&a, &a);
  realIcPolarToRectangular(const_1, &b, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(64bits integer) ^ X(angle34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoIAn34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle34) ^ X(64bits integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34ShoI:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real34 ^ ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) ^ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34Re34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34Re34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) ^ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34Co34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34Co34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c, d;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  // ln(a) --> a
  WP34S_Ln(&a, &a);

  // (c + di) * ln(a) --> c +di
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  // exp((c + di) * ln(a)) --> c +di
  realIcExp(&c, &a);
  realIcPolarToRectangular(const_1, &d, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ^ X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34Re34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34Re34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // ln(a + bi) --> a + bi
  realIcRectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // c * ln(a + bi) -- > a + bi
  realIcMultiply(&c, &a, &a);
  realIcMultiply(&c, &b, &b);

  // exp(c * ln(a + bi)) -- > c + di
  realIcExp(&a, &a);
  realIcPolarToRectangular(const_1, &b, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) ^ X(angle34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRe34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34An34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powRe34An34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle34) ^ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34Re34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34Re34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  realIcPower(&a, &c, &c);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* complex34 + ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex34) ^ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34Co34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34Co34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d, theta;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  realIcRectangularToPolar(&a, &b, &a, &theta);
  WP34S_Ln(&a, &a);

  realIcMultiply(&a, &d, &b);
  realIcFMA(&theta, &c, &b, &b);
  realIcChangeSign(&theta);
  realIcMultiply(&a, &c, &a);
  realIcFMA(&theta, &d, &a, &a);

  realIcExp(&a, &c);
  realIcPolarToRectangular(const_1, &b, &a, &b);
  realIcMultiply(&c, &b, &d);
  realIcMultiply(&c, &a, &c);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ^ X(angle34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCo34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34An34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powCo34An34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c, d;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // ln(a + bi) --> a + bi
  realIcRectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // c * ln(a + bi) -- > a + bi
  realIcMultiply(&c, &a, &a);
  realIcMultiply(&c, &b, &b);

  // exp(c * ln(a + bi)) -- > c + di
  realIcExp(&a, &a);
  realIcPolarToRectangular(const_1, &b, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle34) ^ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34Co34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34Co34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c, d;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  // ln(a) --> a
  WP34S_Ln(&a, &a);

  // (c + di) * ln(a) --> c +di
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  // exp((c + di) * ln(a)) --> c +di
  realIcExp(&c, &a);
  realIcPolarToRectangular(const_1, &d, &c, &d);
  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* angle34 + ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(angle34) ^ X(angle34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powAn34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34An34:", "cannot use NaN as Y input of ^", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powAn34An34:", "cannot use NaN as X input of ^", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);

  realIcPower(&a, &c, &c);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}
