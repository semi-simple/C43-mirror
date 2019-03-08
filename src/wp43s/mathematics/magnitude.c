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
 * \file magnitude.c
 ***********************************************/

#include "wp43s.h"



void (* const magnitude[12])(void) = {
// regX ==> 1               2              3              4               5               6               7               8              9              10             11             12
//          Long integer    real16         complex16      Date            Time            Date            String          real16 mat     complex16 m    Short integer  real34         complex34
            magnitudeLonI,  magnitudeRe16, magnitudeCo16, magnitudeError, magnitudeError, magnitudeError, magnitudeError, magnitudeRm16, magnitudeCm16, magnitudeShoI, magnitudeRe34, magnitudeCo34
};



/********************************************//**
 * \brief Data type error in |x|
 *
 * \param void
 * \return void
 ***********************************************/
void magnitudeError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate |x| for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnMagnitude:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void magnitudeToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "|%s|", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Returns the absolute value of an integer or a real and the magnitude of a complex
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnMagnitude(uint16_t unusedParamButMandatory) {
  if(magnitude[getRegisterDataType(REGISTER_X)] != magnitudeError) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    magnitude[getRegisterDataType(REGISTER_X)]();

    if(lastErrorCode != 0) {
      restoreStack();
      refreshStack();
    }
    else {
      refreshRegisterLine(REGISTER_X);
    }
  }
  else {
    magnitudeError();
  }
}



void magnitudeLonI(void) {
  setRegisterSign(result, 0);
}



void magnitudeRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function magnitudeRe16:", "cannot use NaN as an input of |x|", NULL, NULL);
    #endif
    return;
  }

  real16SetPositiveSign(REGISTER_REAL16_DATA(result));
}



void magnitudeCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function magnitudeCo16:", "cannot use NaN as an input of |x|", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Multiply(REGISTER_REAL16_DATA(REGISTER_L), REGISTER_REAL16_DATA(REGISTER_L), REGISTER_REAL16_DATA(result));
  real16FMA(REGISTER_IMAG16_DATA(REGISTER_L), REGISTER_IMAG16_DATA(REGISTER_L), REGISTER_REAL16_DATA(result), REGISTER_REAL16_DATA(result));
  real16SquareRoot(REGISTER_REAL16_DATA(result), REGISTER_REAL16_DATA(result));
}



void magnitudeRm16(void) {
  magnitudeToBeCoded();
}



void magnitudeCm16(void) {
  magnitudeToBeCoded();
}



void magnitudeShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_intAbs(*(REGISTER_SHORT_INTEGER_DATA(result)));
}



void magnitudeRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function magnitudeRe34:", "cannot use NaN as an input of |x|", NULL, NULL);
    #endif
    return;
  }

  real34SetPositiveSign(REGISTER_REAL34_DATA(result));
}



void magnitudeCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function magnitudeCo34:", "cannot use NaN as an input of |x|", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_L), REGISTER_REAL34_DATA(REGISTER_L), REGISTER_REAL34_DATA(result));
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_L), REGISTER_IMAG34_DATA(REGISTER_L), REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(result));
  real34SquareRoot(REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(result));
}
