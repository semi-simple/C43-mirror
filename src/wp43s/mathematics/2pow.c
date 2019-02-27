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



void (* const twoPow[12])(void) = {
// regX ==> 1            2            3            4             5             6             7             8              9               10             11           12
//          Big integer  real16       complex16    Date          Time          Date          String        real16 mat     complex16 m     Small integer  real34       complex34
            twoPowBigI,  twoPowRe16,  twoPowCo16,  twoPowError,  twoPowError,  twoPowError,  twoPowError,  twoPowRm16,    twoPowCm16,     twoPowSmaI,    twoPowRe34,  twoPowCo34
};



/********************************************//**
 * \brief Data type error in exp
 *
 * \param void
 * \return void
 ***********************************************/
void twoPowError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate 2" STD_SUP_x " for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fn10Pow:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void twoPowToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "2^(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and 10^regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fn2Pow(uint16_t unusedParamButMandatory) {
  if(twoPow[getRegisterDataType(REGISTER_X)] != twoPowError) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    twoPow[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(opX);

    if(lastErrorCode != 0) {
      restoreStack();
      refreshStack();
    }
    else {
      refreshRegisterLine(REGISTER_X);
    }
  }
  else {
    twoPowError();
  }
}



void twoPowBigI(void) {
  int16_t exponent;
  bigInteger_t power;

  convertBigIntegerRegisterToBigInteger(opX, &power);
  if(bigIntegerCompareUInt(&power, MAX_BIG_INTEGER_SIZE_IN_BITS) != BIG_INTEGER_GREATER_THAN) {
    exponent = power.dp[0];

    bigInteger2Exp(exponent, &power);
    convertBigIntegerToBigIntegerRegister(&power, result);
  }
  else {
    displayCalcErrorMessage(4, ERR_REGISTER_LINE, REGISTER_X);
    #if(EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "2^ this value would result in a value exceeding %" FMT16S " bits!", MAX_BIG_INTEGER_SIZE_IN_BITS);
      bigIntegerToString(&power, tmpStr3000, 10);
      showInfoDialog("In function twoPowBigI:", errorMessage, tmpStr3000, NULL);
    #endif
  }
}



void twoPowRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowRe16:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtReal16, REAL16_SIZE, 0);
  real16Copy(const16_2, REGISTER_REAL16_DATA(opY));

  powRe16Re16();

  freeTemporaryRegister(opY);
}



void twoPowCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowCo16:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtReal16, REAL16_SIZE, 0);
  real16Copy(const16_2, REGISTER_REAL16_DATA(opY));

  powRe16Co16();

  freeTemporaryRegister(opY);
}



void twoPowRm16(void) {
  twoPowToBeCoded();
}



void twoPowCm16(void) {
  twoPowToBeCoded();
}



void twoPowSmaI(void) {
  *(REGISTER_SMALL_INTEGER_DATA(REGISTER_X)) = WP34S_int2pow(*(REGISTER_SMALL_INTEGER_DATA(REGISTER_X)));
}



void twoPowRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowRe34:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtReal34, REAL34_SIZE, 0);
  real34Copy(const34_2, REGISTER_REAL34_DATA(opY));

  powRe34Re34();

  freeTemporaryRegister(opY);
}



void twoPowCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function twoPowCo34:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtReal34, REAL34_SIZE, 0);
  real34Copy(const34_2, REGISTER_REAL34_DATA(opY));

  powRe34Co34();

  freeTemporaryRegister(opY);
}
