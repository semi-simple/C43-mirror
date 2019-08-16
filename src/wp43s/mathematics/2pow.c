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
 * \file 2pow.c
 ***********************************************/

#include "wp43s.h"



void (* const twoPow[13])(void) = {
// regX ==> 1            2           3           4           5            6            7            8           9           10            11          12          13
//          Long integer Real16      Complex16   Angle16     Time         Date         String       Real16 mat  Complex16 m Short integer Real34      Complex34   Angle34
            twoPowLonI,  twoPowRe16, twoPowCo16, twoPowAn16, twoPowError, twoPowError, twoPowError, twoPowRm16, twoPowCm16, twoPowShoI,   twoPowRe34, twoPowCo34, twoPowAn34
};



/********************************************//**
 * \brief Data type error in exp
 *
 * \param void
 * \return void
 ***********************************************/
void twoPowError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate 2" STD_SUP_x " for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fn2Pow:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and 2^regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fn2Pow(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  twoPow[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void twoPowLonI(void) {
	 int32_t exponentSign;
  longInteger_t base, exponent;

  longIntegerInit(base);
  intToLongInteger(2, base);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, exponent);

  longIntegerSetPositiveSign(base);

  exponentSign = longIntegerSign(exponent);
  longIntegerSetPositiveSign(exponent);

 	if(longIntegerIsZero(exponent) && longIntegerIsZero(base)) {
	   displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowLonI: Cannot calculate 0^0!", NULL, NULL, NULL);
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

  convertLongIntegerToLongIntegerRegister(power, REGISTER_X);

  longIntegerFree(power);
  longIntegerFree(base);
  longIntegerFree(exponent);
}



void twoPowRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowRe16:", "cannot use NaN X an input of 2^", NULL, NULL);
    #endif
    return;
  }

  real16Power(const16_2, REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
}



void twoPowCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowCo16:", "cannot use NaN as X input of 2^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);

  // X = X * ln(2)
  real34Multiply(const34_ln2, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(const34_ln2, REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X));

  // X = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(REGISTER_X), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(REGISTER_X), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(REGISTER_X));

  convertRegister34To16(REGISTER_X);
}



void twoPowAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowAn16:", "cannot use NaN as X input of 2^", NULL, NULL);
    #endif
    return;
  }

  real16Power(const16_2, REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
}



void twoPowRm16(void) {
  fnToBeCoded();
}



void twoPowCm16(void) {
  fnToBeCoded();
}



void twoPowShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_int2pow(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void twoPowRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowRe34:", "cannot use NaN as X input of 2^", NULL, NULL);
    #endif
    return;
  }

  real34Power(const34_2, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
}



void twoPowCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowCo34:", "cannot use NaN as X input of 2^", NULL, NULL);
    #endif
    return;
  }

  // X = X * ln(2)
  real34Multiply(const34_ln2, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(const34_ln2, REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X));

  // X = exp(X * ln(Y))
  real34_t factor, real34, imag34;
  real34Exp(REGISTER_REAL34_DATA(REGISTER_X), &factor);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(REGISTER_X), &real34, &imag34); // X in radian
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(REGISTER_X));
}



void twoPowAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowAn34:", "cannot use NaN as X input of 2^", NULL, NULL);
    #endif
    return;
  }

  real34Power(const34_2, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
}
