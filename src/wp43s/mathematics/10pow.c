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
 * \file 10pow.c
 ***********************************************/

#include "wp43s.h"



void (* const tenPow[13])(void) = {
// regX ==> 1            2           3           4           5            6            7            8           9           10            11          12          13
//          Long integer Real16      Complex16   Angle16     Time         Date         String       Real16 mat  Complex16 m Short integer Real34      Complex34   Angle34
            tenPowLonI,  tenPowRe16, tenPowCo16, tenPowAn16, tenPowError, tenPowError, tenPowError, tenPowRm16, tenPowCm16, tenPowShoI,   tenPowRe34, tenPowCo34, tenPowAn34
};



/********************************************//**
 * \brief Data type error in exp
 *
 * \param void
 * \return void
 ***********************************************/
void tenPowError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate 10" STD_SUP_x " for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fn10Pow:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and 10^regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fn10Pow(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  tenPow[getRegisterDataType(REGISTER_X)]();

  adjustResult(result, false, true, opX, -1, -1);
}



void tenPowLonI(void) {
  int16_t exponent;
  longInteger_t power;

  convertLongIntegerRegisterToLongInteger(opX, power);
  exponent = (longIntegerCompareUInt(power, 1234) > 0 ? 1234 : longIntegerToUInt(power));

  uIntToLongInteger(10, power);
  opY = allocateTemporaryRegister();
  convertLongIntegerToLongIntegerRegister(power, opY);

  uIntToLongInteger(exponent, power);
  convertLongIntegerToLongIntegerRegister(power, opX);

  powLonILonI();

  freeTemporaryRegister(opY);

  longIntegerFree(power);
}



void tenPowRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tenPowRe16:", "cannot use NaN as an input of 10^", NULL, NULL);
    #endif
    return;
  }

  real16Power(const16_10, REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
  setRegisterDataType(result, dtReal16, TAG_NONE);
}



void tenPowCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tenPowCo16:", "cannot use NaN as an input of 10^", NULL, NULL);
    #endif
    return;
  }

  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Copy(const16_10, REGISTER_REAL16_DATA(opY));

  powRe16Co16();

  freeTemporaryRegister(opY);
}



void tenPowAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tenPowAn16:", "cannot use NaN as an input of 10^", NULL, NULL);
    #endif
    return;
  }

  real16Power(const16_10, REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
  setRegisterDataType(result, dtReal16, TAG_NONE);
}



void tenPowRm16(void) {
  fnToBeCoded();
}



void tenPowCm16(void) {
  fnToBeCoded();
}



void tenPowShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_int10pow(*(REGISTER_SHORT_INTEGER_DATA(opX)));
}



void tenPowRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tenPowRe34:", "cannot use NaN as an input of 10^", NULL, NULL);
    #endif
    return;
  }

  real34Power(const34_10, REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  setRegisterDataType(result, dtReal34, TAG_NONE);
}



void tenPowCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tenPowCo34:", "cannot use NaN as an input of 10^", NULL, NULL);
    #endif
    return;
  }

  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Copy(const34_10, REGISTER_REAL34_DATA(opY));

  powRe34Co34();

  freeTemporaryRegister(opY);
}



void tenPowAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tenPowAn34:", "cannot use NaN as an input of 10^", NULL, NULL);
    #endif
    return;
  }

  real34Power(const34_10, REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  setRegisterDataType(result, dtReal34, TAG_NONE);
}
