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
 * \file log10.c
 ***********************************************/

#include "wp43s.h"



void (* const logBase10[12])(void) = {
// regX ==> 1            2          3          4           5           6           7           8          9           10            11         12
//          Long integer Real16     Complex16  Angle16     Time        Date        String      Real16 mat Complex16 m Short integer Real34     Complex34
            log10LonI,   log10Re16, log10Co16, log10Error, log10Error, log10Error, log10Error, log10Rm16, log10Cm16,  log10ShoI,    log10Re34, log10Co34
};



/********************************************//**
 * \brief Data type error in log10
 *
 * \param void
 * \return void
 ***********************************************/
void log10Error(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate log10 for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnLog10:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and log10(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLog10(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  logBase10[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 ***********************************************************************/

void log10LonI(void) {
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
        showInfoDialog("In function log10LonI:", "cannot calculate log10(0)", NULL, NULL);
      #endif
    }
  }
  else {
    realIc_t x;

    convertLongIntegerRegisterToRealIc(REGISTER_X, &x);

    if(longIntegerIsPositive(lgInt)) {
      WP34S_Ln(&x, &x);
      realIcDivide(&x, const_ln10, &x);
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realIcSetPositiveSign(&x);
      WP34S_Ln(&x, &x);
      realIcDivide(&x, const_ln10, &x);
      reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      realIcDivide(const_pi, const_ln10, &x);
      realIcToReal16(&x, REGISTER_IMAG16_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10LonI:", "cannot calculate log10 of a negative number when CPXRES is not set!", NULL, NULL);
      #endif
    }
  }

  longIntegerFree(lgInt);
}



void log10Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log10Re16:", "cannot use NaN as X input of log10", NULL, NULL);
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
        showInfoDialog("In function log10Re16:", "cannot calculate log10(0)", NULL, NULL);
      #endif
    }
  }

  else if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Re16:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of log10 when flag D is not set", NULL, NULL);
      #endif
      return;
    }
    else {
      if(real16IsPositive(REGISTER_REAL16_DATA(REGISTER_X))) {
        realIcToReal16(const_plusInfinity, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
      }
    }
  }

  else {
    realIc_t x;

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);
    if(real16IsPositive(REGISTER_REAL16_DATA(REGISTER_X))) {
      WP34S_Ln(&x, &x);
      realIcDivide(&x, const_ln10, &x);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realIcSetPositiveSign(&x);
      WP34S_Ln(&x, &x);
      realIcDivide(&x, const_ln10, &x);
      reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      realIcDivide(const_pi, const_ln10, &x);
      realIcToReal16(&x, REGISTER_IMAG16_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Re16:", "cannot calculate log10 of a negative number when CPXRES is not set!", NULL, NULL);
      #endif
    }
  }

  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void log10Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log10Co16:", "cannot use NaN as X input of log10", NULL, NULL);
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
        showInfoDialog("In function log10Co16:", "cannot calculate log10(0)", NULL, NULL);
      #endif
    }
  }
  else {
    realIc_t a, b;

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
    real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

    realIcRectangularToPolar(&a, &b, &a, &b);
    WP34S_Ln(&a, &a);
    realIcDivide(&a, const_ln10, &a);
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
    realIcDivide(&b, const_ln10, &b);
    realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
  }
}



void log10Rm16(void) {
  fnToBeCoded();
}



void log10Cm16(void) {
  fnToBeCoded();
}



void log10ShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intLog10(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void log10Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log10Re34:", "cannot use NaN as X input of log10", NULL, NULL);
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
        showInfoDialog("In function log10Re34:", "cannot calculate log10(0)", NULL, NULL);
      #endif
    }
  }

  else if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Re34:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of log10 when flag D is not set", NULL, NULL);
      #endif
      return;
    }
    else {
      if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
        realIcToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        realIcToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
    }
  }

  else {
    realIc_t x;

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);
    if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
      WP34S_Ln(&x, &x);
      realIcDivide(&x, const_ln10, &x);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realIcSetPositiveSign(&x);
      WP34S_Ln(&x, &x);
      realIcDivide(&x, const_ln10, &x);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      realIcDivide(const_pi, const_ln10, &x);
      realIcToReal34(&x, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      realIcToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Re34:", "cannot calculate log10 of a negative number when CPXRES is not set!", NULL, NULL);
      #endif
    }
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void log10Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log10Co34:", "cannot use NaN as X input of log10", NULL, NULL);
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
        showInfoDialog("In function log10Co34:", "cannot calculate log10(0)", NULL, NULL);
      #endif
    }
  }
  else {
    realIc_t a, b;

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
    real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

    realIcRectangularToPolar(&a, &b, &a, &b);
    WP34S_Ln(&a, &a);
    realIcDivide(&a, const_ln10, &a);
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
    realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
    realIcDivide(&b, const_ln10, &b);
    realIcToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
  }
}
