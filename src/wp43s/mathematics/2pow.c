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
            twoPowLonI,  twoPowRe16, twoPowCo16, twoPowRe16, twoPowError, twoPowError, twoPowError, twoPowRm16, twoPowCm16, twoPowShoI,   twoPowRe34, twoPowCo34, twoPowRe34
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

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  twoPow[getRegisterDataType(REGISTER_X)]();

  adjustResult(result, false, true, opX, -1, -1);
}



void twoPowLonI(void) {
  longInteger_t power;

  convertLongIntegerRegisterToLongInteger(opX, power);
  if(longIntegerCompareUInt(power, MAX_LONG_INTEGER_SIZE_IN_BITS) <= 0) {
    longInteger_t res;

    longIntegerInit(res);
    longInteger2Pow(longIntegerToUInt(power), res);
    convertLongIntegerToLongIntegerRegister(res, result);
    longIntegerFree(res);
  }
  else {
    displayCalcErrorMessage(ERROR_OVERFLOW_PLUS_INF, ERR_REGISTER_LINE, REGISTER_X);
    #if(EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "2^ this value would result in a value exceeding %" FMT16S " bits!", MAX_LONG_INTEGER_SIZE_IN_BITS);
      longIntegerToAllocatedString(power, tmpStr3000, 10);
      showInfoDialog("In function twoPowLonI:", errorMessage, tmpStr3000, NULL);
    #endif
  }

  longIntegerFree(power);
}



void twoPowRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowRe16:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  real16Power(const16_2, REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
  setRegisterDataType(result, dtReal16, TAG_NONE);
}



void twoPowCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowCo16:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Copy(const16_2, REGISTER_REAL16_DATA(opY));

  powRe16Co16();

  freeTemporaryRegister(opY);
}



void twoPowAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowAn16:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  real16Power(const16_2, REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
  setRegisterDataType(result, dtReal16, TAG_NONE);
}



void twoPowRm16(void) {
  fnToBeCoded();
}



void twoPowCm16(void) {
  fnToBeCoded();
}



void twoPowShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_int2pow(*(REGISTER_SHORT_INTEGER_DATA(opX)));
}



void twoPowRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowRe34:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  real34Power(const34_2, REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  setRegisterDataType(result, dtReal34, TAG_NONE);
}



void twoPowCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowCo34:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Copy(const34_2, REGISTER_REAL34_DATA(opY));

  real34Power(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  freeTemporaryRegister(opY);
}



void twoPowAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowAn34:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  real34Power(const34_2, REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  setRegisterDataType(result, dtReal34, TAG_NONE);
}
