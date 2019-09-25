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
 * \file log2.c
 ***********************************************/

#include "wp43s.h"



void (* const logBase2[12])(void) = {
// regX ==> 1            2         3         4          5          6          7          8          9           10            11        12
//          Long integer Real16    Complex16 Angle16    Time       Date       String     Real16 mat Complex16 m Short integer Real34    Complex34
            log2LonI,    log2Re16, log2Co16, log2Error, log2Error, log2Error, log2Error, log2Rm16,  log2Cm16,   log2ShoI,     log2Re34, log2Co34
};



/********************************************//**
 * \brief Data type error in log2
 *
 * \param void
 * \return void
 ***********************************************/
void log2Error(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate log2 for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnLog2:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and log2(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLog2(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  logBase2[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 ***********************************************************************/

void log2LonI(void) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);

  if(longIntegerIsZero(lgInt)) {
    if(getFlag(FLAG_DANGER)) {
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      realIcToReal16(const_minusInfinity, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2LonI:", "cannot calculate log2(0)", NULL, NULL);
      #endif
    }
  }
  else {
    realIc_t x;

    convertLongIntegerRegisterToRealIc(REGISTER_X, &x);

    if(longIntegerIsPositive(lgInt)) {
      WP34S_Ln(&x, &x);
      realIcDivide(&x, const_ln2, &x);
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realIcSetPositiveSign(&x);
      WP34S_Ln(&x, &x);
      realIcDivide(&x, const_ln2, &x);
      reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      realIcDivide(const_pi, const_ln2, &x);
      realIcToReal16(&x, REGISTER_IMAG16_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2LonI:", "cannot calculate log2 of a negative number when CPXRES is not set!", NULL, NULL);
      #endif
    }
  }

  longIntegerFree(lgInt);
}



void log2Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2Re16:", "cannot use NaN as X input of log2", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal16(const_minusInfinity, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2Re16:", "cannot calculate log2(0)", NULL, NULL);
      #endif
    }
  }
  else {
    realIc_t x;

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);
    if(real16IsPositive(REGISTER_REAL16_DATA(REGISTER_X))) {
      WP34S_Ln(&x, &x);
      realIcDivide(&x, const_ln2, &x);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realIcSetPositiveSign(&x);
      WP34S_Ln(&x, &x);
      realIcDivide(&x, const_ln2, &x);
      reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      realIcDivide(const_pi, const_ln2, &x);
      realIcToReal16(&x, REGISTER_IMAG16_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2Re16:", "cannot calculate log2 of a negative number when CPXRES is not set!", NULL, NULL);
      #endif
    }
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void log2Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2Co16:", "cannot use NaN as X input of log2", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X)) && real16IsZero(REGISTER_IMAG16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
      realIcToReal16(const_NaN, REGISTER_IMAG16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2Co16:", "cannot calculate log2(0)", NULL, NULL);
      #endif
    }
  }
  else {
    realIc_t a, b;

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
    real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

    realIcRectangularToPolar(&a, &b, &a, &b);
    WP34S_Ln(&a, &a);
    realIcDivide(&a, const_ln2, &a);
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
    realIcDivide(&b, const_ln2, &b);
    realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
  }
}



void log2Rm16(void) {
  fnToBeCoded();
}



void log2Cm16(void) {
  fnToBeCoded();
}



void log2ShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intLog2(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void log2Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2Re34:", "cannot use NaN as X input of log2", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal34(const_minusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2Re34:", "cannot calculate log2(0)", NULL, NULL);
      #endif
    }
  }
  else {
    realIc_t a;

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
    if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
      WP34S_Ln(&a, &a);
      realIcDivide(&a, const_ln2, &a);
      realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realIcSetPositiveSign(&a);
      WP34S_Ln(&a, &a);
      realIcDivide(&a, const_ln2, &a);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
      realIcDivide(const_pi, const_ln2, &a);
      realIcToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      realIcToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2Re34:", "cannot calculate log2 of a negative number when CPXRES is not set!", NULL, NULL);
      #endif
    }
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void log2Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2Co34:", "cannot use NaN as X input of log2", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      realIcToReal34(const_NaN, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2Co34:", "cannot calculate log2(0)", NULL, NULL);
      #endif
    }
  }
  else {
    realIc_t a, b;

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
    real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

    realIcRectangularToPolar(&a, &b, &a, &b);
    WP34S_Ln(&a, &a);
    realIcDivide(&a, const_ln2, &a);
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
    realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
    realIcDivide(&b, const_ln2, &b);
    realIcToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
  }
}
