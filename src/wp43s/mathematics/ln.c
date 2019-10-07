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
 * \file ln.c
 ***********************************************/

#include "wp43s.h"



void (* const ln[12])(void) = {
// regX ==> 1            2       3         4        5        6        7        8          9           10            11      12
//          Long integer Real16  Complex16 Angle16  Time     Date     String   Real16 mat Complex16 m Short integer Real34  Complex34
            lnLonI,      lnRe16, lnCo16,   lnError, lnError, lnError, lnError, lnRm16,    lnCm16,     lnShoI,       lnRe34, lnCo34
};



/********************************************//**
 * \brief Data type error in ln
 *
 * \param void
 * \return void
 ***********************************************/
void lnError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Ln for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnLn:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and ln(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLn(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  ln[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void lnCoIc(const complexIc_t *z, complexIc_t *ln) {
  if(realIcIsSpecial(&z->real) || realIcIsSpecial(&z->imag)) {
    if(realIcIsNaN(&z->real) || realIcIsNaN(&z->imag)) {
      realIcCopy(const_NaN, &ln->real);
      realIcCopy(const_NaN, &ln->imag);
    }
    else {
      realIcCopy(const_plusInfinity, &ln->real);

      if(realIcIsNegative(&z->imag)) {
        realIcCopy(const_minusInfinity, &ln->imag);
      }
      else {
        realIcCopy(const_plusInfinity, &ln->imag);
      }
    }
    return;
  }

  if(realIcIsZero(&z->imag)) {
    if(realIcIsZero(&z->real)) {
      realIcCopy(const_NaN, &ln->real);
      realIcCopy(const_NaN, &ln->imag);
    }
    else {
      if(realIcIsNegative(&z->real)) {
        realIcCopy(&z->real, &ln->real);
        realIcSetPositiveSign(&ln->real);
        WP34S_Ln(&ln->real, &ln->real);
        realIcCopy(const_pi, &ln->imag);
      }
      else {
        WP34S_Ln(&z->real, &ln->real);
        realIcZero(&ln->imag);
      }
    }
   return;
  }

  realIcRectangularToPolar(&z->real, &z->imag, &ln->real, &ln->imag);
  WP34S_Ln(&ln->real, &ln->real);
}



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 ***********************************************************************/

void lnLonI(void) {
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
        showInfoDialog("In function lnLonI:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else {
    realIc_t x;

    convertLongIntegerRegisterToRealIc(REGISTER_X, &x);

    if(longIntegerIsPositive(lgInt)) {
      WP34S_Ln(&x, &x);
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realIcSetPositiveSign(&x);
      WP34S_Ln(&x, &x);
      reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      realIcToReal16(const_pi, REGISTER_IMAG16_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnLonI:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
      #endif
    }
  }

  longIntegerFree(lgInt);
}



void lnRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnRe16:", "cannot use NaN as X input of Ln", NULL, NULL);
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
        showInfoDialog("In function lnRe16:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }

  else if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnRe16:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of ln when flag D is not set", NULL, NULL);
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
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realIcSetPositiveSign(&x);
      WP34S_Ln(&x, &x);
      reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      realIcToReal16(const_pi, REGISTER_IMAG16_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnRe16:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
      #endif
    }
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void lnCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnCo16:", "cannot use NaN as X input of Ln", NULL, NULL);
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
        showInfoDialog("In function lnCo16:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else {
    complexIc_t x;

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
    real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);

    lnCoIc(&x, &x);

    realIcToReal16(&x.real, REGISTER_REAL16_DATA(REGISTER_X));
    realIcToReal16(&x.imag, REGISTER_IMAG16_DATA(REGISTER_X));
  }
}



void lnRm16(void) {
  fnToBeCoded();
}



void lnCm16(void) {
  fnToBeCoded();
}



void lnShoI(void) {
  realIc_t x;

  convertShortIntegerRegisterToRealIc(REGISTER_X, &x);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

  if(realIcIsZero(&x)) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal16(const_minusInfinity, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnShoI:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else {
    if(realIcIsPositive(&x)) {
      WP34S_Ln(&x, &x);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realIcSetPositiveSign(&x);
      WP34S_Ln(&x, &x);
      reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      realIcToReal16(const_pi, REGISTER_IMAG16_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnShoI:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
      #endif
    }
  }
}



void lnRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnRe34:", "cannot use NaN as X input of Ln", NULL, NULL);
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
        showInfoDialog("In function lnRe34:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }

  else if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnRe34:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of ln when flag D is not set", NULL, NULL);
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
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realIcSetPositiveSign(&x);
      WP34S_Ln(&x, &x);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      realIcToReal34(const_pi, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      realIcToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnRe34:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
      #endif
    }
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void lnCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnCo34:", "cannot use NaN as X input of Ln", NULL, NULL);
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
        showInfoDialog("In function lnCo34:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else {
    complexIc_t x;

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
    real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

    lnCoIc(&x, &x);

    realIcToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
    realIcToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
  }
}
