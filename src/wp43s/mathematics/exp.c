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
 * \file exp.c
 ***********************************************/

#include "wp43s.h"



void (* const Exp[12])(void) = {
// regX ==> 1            2        3         4         5         6         7         8          9           10            11       12
//          Long integer Real16   Complex16 Anfle16   Time      Date      String    Real16 mat Complex16 m Short integer Real34   Complex34
            expLonI,     expRe16, expCo16,  expError, expError, expError, expError, expRm16,   expCm16,    expShoI,      expRe34, expCo34
};



/********************************************//**
 * \brief Data type error in exp
 *
 * \param void
 * \return void
 ***********************************************/
void expError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Exp for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnExp:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and exp(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnExp(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Exp[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void expCo39(const complex39_t *z, complex39_t *res) {
  real39_t expa, sin, cos;

  if(realIsZero(&z->imag)) {
   realExp(&z->real, &res->real, &ctxtReal39);
   realZero(&res->imag);
   return;
  }

  if(realIsSpecial(&z->real) || realIsSpecial(&z->imag)) {
    realCopy(const_NaN, &res->real);
    realCopy(const_NaN, &res->imag);
    return;
  }

 realExp(&z->real, &expa, &ctxtReal39);
 WP34S_Cvt2RadSinCosTan(&z->imag, AM_RADIAN, &sin, &cos, NULL);
 realMultiply(&expa, &cos, &res->real, &ctxtReal39);
 realMultiply(&expa, &sin, &res->imag, &ctxtReal39);
}



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 ***********************************************************************/

void expLonI(void) {
  real39_t a;

  convertLongIntegerRegisterToReal(REGISTER_X, &a, &ctxtReal39);
  realExp(&a, &a, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
  realToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void expRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expRe16:", "cannot use NaN as X input of exp", NULL, NULL);
    #endif
    return;
  }

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expRe16:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of exp when flag D is not set", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  realExp(&x, &x, &ctxtReal39);
  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void expCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expCo16:", "cannot use NaN as X input of exp", NULL, NULL);
    #endif
    return;
  }

  complex39_t z;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &z.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &z.imag);

  expCo39(&z, &z);

  realToReal16(&z.real, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&z.imag, REGISTER_IMAG16_DATA(REGISTER_X));
}



void expRm16(void) {
  fnToBeCoded();
}



void expCm16(void) {
  fnToBeCoded();
}



void expShoI(void) {
  real39_t x;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  realExp(&x, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
}



void expRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expRe34:", "cannot use NaN as X input of exp", NULL, NULL);
    #endif
    return;
  }

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expRe34:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of exp when flag D is not set", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  realExp(&x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void expCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expCo34:", "cannot use NaN as X input of exp", NULL, NULL);
    #endif
    return;
  }

  complex39_t z;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &z.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &z.imag);

  expCo39(&z, &z);

  realToReal34(&z.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&z.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}
