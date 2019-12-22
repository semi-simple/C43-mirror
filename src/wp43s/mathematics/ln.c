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



void lnCo39(const real39_t *zReal, const real39_t *zImag, real39_t *lnReal, real39_t *lnImag) {
  if(realIsSpecial(zReal) || realIsSpecial(zImag)) {
    if(realIsNaN(zReal) || realIsNaN(zImag)) {
      realCopy(const_NaN, lnReal);
      realCopy(const_NaN, lnImag);
    }
    else {
      realCopy(const_plusInfinity, lnReal);

      if(realIsNegative(zImag)) {
        realCopy(const_minusInfinity, lnImag);
      }
      else {
        realCopy(const_plusInfinity, lnImag);
      }
    }
    return;
  }

  if(realIsZero(zImag)) {
    if(realIsZero(zReal)) {
      realCopy(const_NaN, lnReal);
      realCopy(const_NaN, lnImag);
    }
    else {
      if(realIsNegative(zReal)) {
        realCopy(zReal, lnReal);
        realSetPositiveSign(lnReal);
        WP34S_Ln(lnReal, lnReal);
        realCopy(const_pi, lnImag);
      }
      else {
        WP34S_Ln(zReal, lnReal);
        realZero(lnImag);
      }
    }
   return;
  }

  real39RectangularToPolar(zReal, zImag, lnReal, lnImag);
  WP34S_Ln(lnReal, lnReal);
}



void lnCo51(const real51_t *zReal, const real51_t *zImag, real51_t *lnReal, real51_t *lnImag) {
  if(realIsSpecial(zReal) || realIsSpecial(zImag)) {
    if(realIsNaN(zReal) || realIsNaN(zImag)) {
      realCopy(const_NaN, lnReal);
      realCopy(const_NaN, lnImag);
    }
    else {
      realCopy(const_plusInfinity, lnReal);

      if(realIsNegative(zImag)) {
        realCopy(const_minusInfinity, lnImag);
      }
      else {
        realCopy(const_plusInfinity, lnImag);
      }
    }
    return;
  }

  if(realIsZero(zImag)) {
    if(realIsZero(zReal)) {
      realCopy(const_NaN, lnReal);
      realCopy(const_NaN, lnImag);
    }
    else {
      if(realIsNegative(zReal)) {
        realCopy(zReal, lnReal);
        realSetPositiveSign(lnReal);
        realLn((real_t *)lnReal, (real_t *)lnReal, &ctxtReal51);
        realCopy(const_pi, lnImag);
      }
      else {
        realLn((real_t *)zReal, (real_t *)lnReal, &ctxtReal51);
        realZero(lnImag);
      }
    }
   return;
  }

  real51RectangularToPolar((real_t *)zReal, (real_t *)zImag, (real_t *)lnReal, (real_t *)lnImag);
  realLn((real_t *)lnReal, (real_t *)lnReal, &ctxtReal51);
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
      realToReal16(const_minusInfinity, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnLonI:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else {
    real39_t x;

    convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);

    if(longIntegerIsPositive(lgInt)) {
      WP34S_Ln(&x, &x);
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realSetPositiveSign(&x);
      WP34S_Ln(&x, &x);
      reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      realToReal16(const_pi, REGISTER_IMAG16_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
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
      realToReal16(const_minusInfinity, REGISTER_REAL16_DATA(REGISTER_X));
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
        realToReal16(const_plusInfinity, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
      }
    }
  }

  else {
    real39_t x;

    real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
    if(real16IsPositive(REGISTER_REAL16_DATA(REGISTER_X))) {
      WP34S_Ln(&x, &x);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realSetPositiveSign(&x);
      WP34S_Ln(&x, &x);
      reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      realToReal16(const_pi, REGISTER_IMAG16_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
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
      realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
      realToReal16(const_NaN, REGISTER_IMAG16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnCo16:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else {
    real39_t xReal, xImag;

    real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &xReal);
    real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &xImag);

    lnCo39(&xReal, &xImag, &xReal, &xImag);

    realToReal16(&xReal, REGISTER_REAL16_DATA(REGISTER_X));
    realToReal16(&xImag, REGISTER_IMAG16_DATA(REGISTER_X));
  }
}



void lnRm16(void) {
  fnToBeCoded();
}



void lnCm16(void) {
  fnToBeCoded();
}



void lnShoI(void) {
  real39_t x;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

  if(realIsZero(&x)) {
    if(getFlag(FLAG_DANGER)) {
      realToReal16(const_minusInfinity, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnShoI:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else {
    if(realIsPositive(&x)) {
      WP34S_Ln(&x, &x);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realSetPositiveSign(&x);
      WP34S_Ln(&x, &x);
      reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      realToReal16(const_pi, REGISTER_IMAG16_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
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
      realToReal34(const_minusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
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
        realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
    }
  }

  else {
    real39_t x;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
      WP34S_Ln(&x, &x);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realSetPositiveSign(&x);
      WP34S_Ln(&x, &x);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_pi, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
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
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_NaN, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnCo34:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else {
    real39_t xReal, xImag;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
    real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

    lnCo39(&xReal, &xImag, &xReal, &xImag);

    realToReal34(&xReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&xImag, REGISTER_IMAG34_DATA(REGISTER_X));
  }
}
