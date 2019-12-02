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
 * \file invert.c
 ***********************************************/

#include "wp43s.h"



void (* const invert[12])(void) = {
// regX ==> 1            2           3           4            5            6            7            8           9           10            11          12
//          Long integer Real16      Complex16   Angle16      Time         Date         String       Real16 mat  Complex16 m Short integer Real34      Complex34
            invertLonI,  invertRe16, invertCo16, invertError, invertError, invertError, invertError, invertRm16, invertCm16, invertError,  invertRe34, invertCo34
};



/********************************************//**
 * \brief Data type error in invert
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void invertError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot invert %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnInvert:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and 1 ÷ regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnInvert(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  invert[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



/********************************************//**
 * \brief 1 ÷ X(long integer) ==> X(long integer or real16)
 *
 * \param void
 * \return void
 ***********************************************/
void invertLonI(void) {
  longInteger_t a;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, a);

  if(longIntegerIsZero(a)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function invertLonI:", "cannot divide a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t quotient, remainder, one;

    longIntegerInit(one);
    intToLongInteger(1, one);

    longIntegerInit(quotient);
    longIntegerInit(remainder);
    longIntegerDivideQuotientRemainder(one, a, quotient, remainder);

    if(longIntegerIsZero(remainder)) {
      convertLongIntegerToLongIntegerRegister(quotient, REGISTER_X);
    }
    else {
      real39_t reX;

      longIntegerToAllocatedString(a, tmpStr3000, TMP_STR_LENGTH);
      stringToReal(tmpStr3000, &reX, &ctxtReal39);

      realDivide(const_1, &reX, &reX, &ctxtReal39);

      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      realToReal16(&reX, REGISTER_REAL16_DATA(REGISTER_X));
    }

    longIntegerFree(quotient);
    longIntegerFree(remainder);
    longIntegerFree(one);
  }

  longIntegerFree(a);
}



/********************************************//**
 * \brief 1 ÷ X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void invertRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function invertRe16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      realToReal16((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function invertRe16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function invertRe16:", "cannot divide a real16 by " STD_PLUS_MINUS STD_INFINITY " when flag D is not set", NULL, NULL);
    #endif
  }

  else {
    real16Divide(const16_1, REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief 1 ÷ X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void invertCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function invertCo16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b, denom;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  realMultiply(&a, &a, &denom, &ctxtReal39);    // c²
  realFMA(&b, &b, &denom, &denom, &ctxtReal39); // c² + d²

  realDivide(&a, &denom, &a, &ctxtReal39);
  realChangeSign(&denom);
  realDivide(&b, &denom, &b, &ctxtReal39);

  realToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



void invertRm16(void) {
  fnToBeCoded();
}



void invertCm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief 1 ÷ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void invertRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function invertRe34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function invertRe34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function invertRe34:", "cannot divide a real34 by " STD_PLUS_MINUS STD_INFINITY " when flag D is not set", NULL, NULL);
    #endif
  }

  else {
    real34Divide(const34_1, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief 1 ÷ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void invertCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function invertCo34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b, denom;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  realMultiply(&a, &a, &denom, &ctxtReal39);    // c²
  realFMA(&b, &b, &denom, &denom, &ctxtReal39); // c² + d²

  realDivide(&a, &denom, &a, &ctxtReal39);
  realChangeSign(&denom);
  realDivide(&b, &denom, &b, &ctxtReal39);

  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}
