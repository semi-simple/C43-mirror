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



void (* const fact[12])(void) = {
// regX ==> 1            2         3         4          5          6          7          8          9           10            11        12
//          Long integer Real16    Complex16 Angle16    Time       Date       String     Real16 mat Complex16 m Short integer Real34    Complex34
            factLonI,    factRe16, factCo16, factError, factError, factError, factError, factError, factError,  factShoI,     factRe34, factCo34
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

  fact[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void factLonI(void) {
  longInteger_t x, fact;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsNegative(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      longIntegerToDisplayString(REGISTER_X, errorMessage + 100, sizeof(errorMessage) - 100, SCREEN_WIDTH);
      sprintf(errorMessage, "cannot calculate factorial(%s)", errorMessage + 100);
      showInfoDialog("In function factLonI:", errorMessage, NULL, NULL);
    #endif
    longIntegerFree(x);
    return;
  }

  if(longIntegerCompareUInt(x, 450) > 0) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      longIntegerToDisplayString(REGISTER_X, errorMessage + 100, sizeof(errorMessage) - 100, SCREEN_WIDTH);
      sprintf(errorMessage, "cannot calculate factorial(%s), the limit is 450, it's to ensure that the 3328 bits limit is not exceeded", errorMessage + 100);
      showInfoDialog("In function factLonI:", errorMessage, NULL, NULL);
    #endif
    longIntegerFree(x);
    return;
  }


  #if (__linux__ == 1)
    uint32_t n = longIntegerToUInt(x);
    //The more precise formula below is: (n*ln(n) - n + (ln(8n³ + 4n² + n + 1/30))/6 + ln(pi)/2) / ln(2)
    longIntegerInitSizeInBits(fact, 1 + (uint32_t)((n * log(n) - n) / log(2)));
    uIntToLongInteger(1, fact);
    for(uint32_t i=2; i<=n; i++) {
      longIntegerMultiplyUInt(fact, i, fact);
    }
  #else
    longIntegerInit(fact);
    longIntegerFactorial(longIntegerToUInt(x), fact); //TODO why this line fails?
  #endif

  convertLongIntegerToLongIntegerRegister(fact, REGISTER_X);
  longIntegerFree(fact);
  longIntegerFree(x);
}



void factRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function factRe16:", "cannot use NaN as X input of x!", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  WP34S_Factorial(&x, &x);
  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void factCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function factCo16:", "cannot use NaN as X input of x!", NULL, NULL);
    #endif
    return;
  }

  complex39_t z;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &z.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &z.imag);

  realAdd(&z.real, const_1, &z.real, &ctxtReal39);
  WP34S_ComplexGamma(&z, &z);

  realToReal16(&z.real, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&z.imag, REGISTER_IMAG16_DATA(REGISTER_X));
}



void factShoI(void) {
  int16_t sign;
  uint64_t value;

  convertShortIntegerRegisterToUInt64(REGISTER_X, &sign, &value);

  if(sign == 1) { // Negative value
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      longIntegerToDisplayString(REGISTER_X, errorMessage + 100, sizeof(errorMessage) - 100, SCREEN_WIDTH);
      sprintf(errorMessage, "cannot calculate factorial(%s)", errorMessage + 100);
      showInfoDialog("In function factShoI:", errorMessage, NULL, NULL);
    #endif
    return;
  }

  if(value > 20) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      longIntegerToDisplayString(REGISTER_X, errorMessage + 100, sizeof(errorMessage) - 100, SCREEN_WIDTH);
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

  convertUInt64ToShortIntegerRegister(0, fact, getRegisterTag(REGISTER_X), REGISTER_X);
}



void factRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function factRe34:", "cannot use NaN as X input of x!", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  WP34S_Factorial(&x, &x);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void factCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function factCo34:", "cannot use NaN as X input of x!", NULL, NULL);
    #endif
    return;
  }

  complex39_t z;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &z.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &z.imag);

  realAdd(&z.real, const_1, &z.real, &ctxtReal39);
  WP34S_ComplexGamma(&z, &z);

  realToReal34(&z.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&z.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}
