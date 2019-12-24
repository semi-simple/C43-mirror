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



void (* const twoPow[9])(void) = {
// regX ==> 1            2           3           4            5            6            7           8           9
//          Long integer Real34      Complex34   Time         Date         String       Real34 mat  Complex34 m Short integer
            twoPowLonI,  twoPowReal, twoPowCplx, twoPowError, twoPowError, twoPowError, twoPowRema, twoPowCxma, twoPowShoI
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



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 ***********************************************************************/

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



void twoPowRema(void) {
  fnToBeCoded();
}



void twoPowCxma(void) {
  fnToBeCoded();
}



void twoPowShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_int2pow(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void twoPowReal(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowReal:", "cannot use NaN as X input of 2^", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  realPower(const_2, &x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void twoPowCplx(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowCplx:", "cannot use NaN as X input of 2^", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b, factor;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // ln(2) * (a + bi) --> (a + bi)
  realMultiply(const_ln2, &a, &a, &ctxtReal39);
  realMultiply(const_ln2, &b, &b, &ctxtReal39);

  // exp(ln(2) * (a + bi)) --> (a + bi)
  realExp(&a, &factor, &ctxtReal39);
  real39PolarToRectangular(const_1, &b, &a, &b);
  realMultiply(&factor, &a, &a, &ctxtReal39);
  realMultiply(&factor, &b, &b, &ctxtReal39);

  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}
