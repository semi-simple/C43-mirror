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
 * \file factorial.c
 ***********************************************/

#include "wp43s.h"



void (* const fact[13])(void) = {
// regX ==> 1            2         3         4         5          6          7          8          9           10            11        12        13
//          Long integer Real16    Complex16 Angle16   Time       Date       String     Real16 mat Complex16 m Short integer Real34    Complex34 Angle34
            factLonI,    factRe16, factCo16, factAn16, factError, factError, factError, factError, factError,  factShoI,     factRe34, factCo34, factAn34
};



/********************************************//**
 * \brief Data type error in fact
 *
 * \param void
 * \return void
 ***********************************************/
void factError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate x! for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnFactorial:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and fact(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFactorial(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  fact[getRegisterDataType(REGISTER_X)]();

  adjustResult(result, false, true, opX, -1, -1);
}



void factLonI(void) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(opX, lgInt);

  if(longIntegerIsNegative(lgInt)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      longIntegerToDisplayString(opX, errorMessage + 100, SCREEN_WIDTH);
      sprintf(errorMessage, "cannot calculate factorial(%s)", errorMessage + 100);
      showInfoDialog("In function factLonI:", errorMessage, NULL, NULL);
    #endif
    longIntegerFree(lgInt);
    return;
  }

  if(longIntegerCompareUInt(lgInt, 294) > 0) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      longIntegerToDisplayString(opX, errorMessage + 100, SCREEN_WIDTH);
      sprintf(errorMessage, "cannot calculate factorial(%s)", errorMessage + 100);
      showInfoDialog("In function factLonI:", errorMessage, NULL, NULL);
    #endif
    longIntegerFree(lgInt);
    return;
  }

  longInteger_t fact;
  longIntegerInit(fact);
  longIntegerFactorial(longIntegerToUInt(lgInt), fact);
  convertLongIntegerToLongIntegerRegister(fact, result);
  longIntegerFree(fact);
  longIntegerFree(lgInt);
}



void factRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function factRe16:", "cannot use NaN as an input of x!", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  WP34S_real34Factorial(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  convertRegister34To16(result);
}



void factCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function factCo16:", "cannot use NaN as an input of x!", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



void factAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function factRe16:", "cannot use NaN as an input of x!", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  WP34S_real34Factorial(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  convertRegister34To16(result);
}



void factShoI(void) {
  int16_t sign;
  uint64_t value;

  convertShortIntegerRegisterToUInt64(opX, &sign, &value);

  if(sign == 1) { // Negative value
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      longIntegerToDisplayString(opX, errorMessage + 100, SCREEN_WIDTH);
      sprintf(errorMessage, "cannot calculate factorial(%s)", errorMessage + 100);
      showInfoDialog("In function factShoI:", errorMessage, NULL, NULL);
    #endif
    return;
  }

  if(value > 20) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      longIntegerToDisplayString(opX, errorMessage + 100, SCREEN_WIDTH);
      sprintf(errorMessage, "cannot calculate factorial(%s)", errorMessage + 100);
      showInfoDialog("In function factShoI:", errorMessage, NULL, NULL);
    #endif
    return;
  }

  uint64_t fact;

  if(value <= 1) {
    fact = 1;
  }
  else {
    uint32_t counter;

    fact = value;
    counter = value - 1;
    while(counter > 1) {
      fact *= counter--;
    }
  }

  if(fact > shortIntegerMask) {
    fnSetFlag(FLAG_OVERFLOW);
  }

  convertUInt64ToShortIntegerRegister(0, fact, getRegisterTag(opX), result);
}



void factRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function factRe34:", "cannot use NaN as an input of x!", NULL, NULL);
    #endif
    return;
  }

  WP34S_real34Factorial(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



void factCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function factCo34:", "cannot use NaN as an input of x!", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



void factAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function factAn34:", "cannot use NaN as an input of x!", NULL, NULL);
    #endif
    return;
  }

  WP34S_real34Factorial(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  setRegisterDataType(result, dtReal34, TAG_NONE);
}
