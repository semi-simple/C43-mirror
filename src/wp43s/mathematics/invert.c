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



void (* const invert[13])(void) = {
// regX ==> 1            2           3           4           5            6            7            8           9           10            11          12          13
//          Long integer Real16      Complex16   Angle16     Time         Date         String       Real16 mat  Complex16 m Short integer Real34      Complex34   Angle34
            invertLonI,  invertRe16, invertCo16, invertAn16, invertError, invertError, invertError, invertRm16, invertCm16, invertError,  invertRe34, invertCo34, invertAn34
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
  longInteger_t liX, liY;

  longIntegerInit(liY);
  intToLongInteger(1, liY);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, liX);

  if(longIntegerIsZero(liX)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function invertLonI:", "cannot divide a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t quotient, remainder;

    longIntegerInit(quotient);
    longIntegerInit(remainder);
    longIntegerDivideRemainder(liY, liX, quotient, remainder);

    if(longIntegerIsZero(remainder)) {
      convertLongIntegerToLongIntegerRegister(quotient, REGISTER_X);
    }
    else {
      longIntegerToAllocatedString(liY, tmpStr3000, 10);
      reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, TAG_NONE);
      stringToReal34(tmpStr3000, REGISTER_REAL34_DATA(REGISTER_Y));
      longIntegerToAllocatedString(liX, tmpStr3000, 10);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);
      stringToReal34(tmpStr3000, REGISTER_REAL34_DATA(REGISTER_X));

      real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));

      convertRegister34To16(REGISTER_X);
    }

    longIntegerFree(quotient);
    longIntegerFree(remainder);
  }

  longIntegerFree(liX);
  longIntegerFree(liY);
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
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const16_plusInfinity : const16_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function invertRe16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real16Divide(const16_1, REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
  }
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

  real34_t denom;
  complex34_t result;

  convertRegister16To34(REGISTER_X);

  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // c²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // c² + d²

  real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &denom, VARIABLE_REAL34_DATA(&result));
  real34Multiply(VARIABLE_REAL34_DATA(&result), const34_1, VARIABLE_REAL34_DATA(&result));
  real34ChangeSign(&denom);
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &denom, VARIABLE_IMAG34_DATA(&result));
  real34Multiply(VARIABLE_IMAG34_DATA(&result), const34_1, VARIABLE_IMAG34_DATA(&result));

  convertRegister34To16(REGISTER_X);
  real34ToReal16(VARIABLE_REAL34_DATA(&result), REGISTER_REAL16_DATA(REGISTER_X));
  real34ToReal16(VARIABLE_IMAG34_DATA(&result), REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief 1 ÷ X(angle16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void invertAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function invertAn16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const16_plusInfinity : const16_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
      setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function invertAn16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real16Divide(const16_1, REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
    setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
  }
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
      real34Copy(const34_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function invertRe34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(const34_1, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
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

  real34_t denom;
  complex34_t result;

  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // c²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // c² + d²

  real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &denom, VARIABLE_REAL34_DATA(&result));
  real34Multiply(VARIABLE_REAL34_DATA(&result), const34_1, VARIABLE_REAL34_DATA(&result));
  real34ChangeSign(&denom);
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &denom, VARIABLE_IMAG34_DATA(&result));
  real34Multiply(VARIABLE_IMAG34_DATA(&result), const34_1, VARIABLE_IMAG34_DATA(&result));

  complex34Copy(&result, REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief 1 ÷ X(angle34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void invertAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function invertAn34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
      setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function invertAn34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(const34_1, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
  }
}
