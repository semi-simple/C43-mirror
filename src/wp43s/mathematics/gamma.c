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
 * \file gamma.c
 ***********************************************/

#include "wp43s.h"



void (* const Gamma[12])(void) = {
// regX ==> 1            2            3            4             5             6             7             8             9             10             11           12
//          Long integer Real16       Complex16    Angle16       Time          Date          String        Real16 mat    Complex16 m   Short integer  Real34       Complex34
            gammaLonI,   gammaRe16,   gammaCo16,   gammaError,   gammaError,   gammaError,   gammaError,   gammaError,   gammaError,   gammaError,    gammaRe34,   gammaCo34
};

void (* const lnGamma[12])(void) = {
// regX ==> 1            2            3            4             5             6             7             8             9             10             11           12
//          Long integer Real16       Complex16    Angle16       Time          Date          String        Real16 mat    Complex16 m   Short integer  Real34       Complex34
            lnGammaLonI, lnGammaRe16, lnGammaCo16, lnGammaError, lnGammaError, lnGammaError, lnGammaError, lnGammaError, lnGammaError, lnGammaError,  lnGammaRe34, lnGammaCo34
};



/********************************************//**
 * \brief Data type error in gamma
 *
 * \param void
 * \return void
 ***********************************************/
void gammaError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate gamma(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("In function fnGamma:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Data type error in lnGamma
 *
 * \param void
 * \return void
 ***********************************************/
void lnGammaError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate lnGamma(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("In function fnLnGamma:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and gamma(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGamma(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Gamma[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



/********************************************//**
 * \brief regX ==> regL and lnGamma(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLnGamma(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  lnGamma[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void gammaLonI(void) {
  real39_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

  if(realIsInfinite(&x)) {
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function gammaLonI:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of gamma when flag D is not set", NULL, NULL);
      #endif
    }
    else {
      realToReal16((realIsPositive(&x) ? const_plusInfinity : const_NaN), REGISTER_REAL16_DATA(REGISTER_X));
    }
    return;
  }

  if(realCompareLessEqual(&x, const_0)) { // x <= 0 and is an integer
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function gammaLonI:", "cannot use a negative integer as X input of gamma when flag D is not set", NULL, NULL);
      #endif
    }
    else {
      realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    return;
  }

  WP34S_Gamma(&x, &x);
  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
}



void lnGammaLonI(void) {
  real39_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

  if(realIsInfinite(&x)) {
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnGammaLonI:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of gamma when flag D is not set", NULL, NULL);
      #endif
    }
    else {
      realToReal16((realIsPositive(&x) ? const_plusInfinity : const_NaN), REGISTER_REAL16_DATA(REGISTER_X));
    }
    return;
  }

  if(realCompareLessEqual(&x, const_0)) { // x <= 0 and is an integer
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnGammaLonI:", "cannot use a negative integer as X input of gamma when flag D is not set", NULL, NULL);
      #endif
    }
    else {
      realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    return;
  }

  WP34S_LnGamma(&x, &x);
  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
}



void gammaRe16(void) {
  setRegisterAngularMode(REGISTER_X, AM_NONE);

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function gammaRe16:", "cannot use NaN as X input of gamma", NULL, NULL);
    #endif
    return;
  }

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function gammaRe16:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of gamma when flag D is not set", NULL, NULL);
      #endif
    }
    else {
      realToReal16((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_X)) ? const_plusInfinity : const_NaN), REGISTER_REAL16_DATA(REGISTER_X));
    }
    return;
  }

  if(real16CompareLessEqual(REGISTER_REAL16_DATA(REGISTER_X), const16_0) && real16IsAnInteger(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function gammaRe16:", "cannot use a negative integer as X input of gamma when flag D is not set", NULL, NULL);
      #endif
    }
    else {
      realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    return;
  }

  real39_t x;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  WP34S_Gamma(&x, &x);
  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
}



void lnGammaRe16(void) {
  real39_t xReal, xImag;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &xReal);
  setRegisterAngularMode(REGISTER_X, AM_NONE);

  if(realIsNaN(&xReal)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnGammaRe16:", "cannot use NaN as X input of lnGamma", NULL, NULL);
    #endif
    return;
  }

  if(realIsInfinite(&xReal)) {
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lngammaRe16:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of lngamma when flag D is not set", NULL, NULL);
      #endif
    }
    else {
      realToReal16((real16IsPositive(&xReal) ? const_plusInfinity : const_NaN), REGISTER_REAL16_DATA(REGISTER_X));
    }
    return;
  }

  if(realCompareLessEqual(&xReal, const_0)) { // x <= 0
    if(realIsAnInteger(&xReal)) {
      if(!getFlag(FLAG_DANGER)) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function lngammaRe16:", "cannot use a negative integer as X input of lngamma when flag D is not set", NULL, NULL);
        #endif
      }
      else {
        realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
      }
      return;
    }
    else { // x is negative and not an integer
      realMinus(&xReal, &xImag, &ctxtReal39); // x.imag is used as a temp variable here
      realDivideRemainder(&xImag, const_2, &xImag, &ctxtReal39);
      if(realCompareGreaterThan(&xImag, const_1)) { // the result is a real
        WP34S_LnGamma(&xReal, &xReal);
        realToReal16(&xReal, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else { // the result is a complex
        if(getFlag(FLAG_CPXRES)) { // We can calculate a complex
          real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &xImag);
          reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
          WP34S_Gamma(&xReal, &xReal);
          realSetPositiveSign(&xReal);
          realLn(&xReal, &xReal, &ctxtReal39);
          realToReal16(&xReal, REGISTER_REAL16_DATA(REGISTER_X));
          realToIntegralValue(&xImag, &xImag, DEC_ROUND_FLOOR, &ctxtReal39);
          realMultiply(&xImag, const_pi, &xImag, &ctxtReal39);
          realToReal16(&xImag, REGISTER_IMAG16_DATA(REGISTER_X));
        }
        else { // Domain error
          displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            showInfoDialog("In function lngammaRe16:", "cannot use a as X input of lngamma if gamma(X)<0 when flag I is not set", NULL, NULL);
          #endif
        }
      }
    }
    return;
  }


  WP34S_LnGamma(&xReal, &xReal);
  realToReal16(&xReal, REGISTER_REAL16_DATA(REGISTER_X));
}



void gammaCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function gammaCo16:", "cannot use NaN as X input of gamma", NULL, NULL);
    #endif
    return;
  }

  real39_t zReal, zImag;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &zReal);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &zImag);

  WP34S_ComplexGamma(&zReal, &zImag, &zReal, &zImag);

  realToReal16(&zReal, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&zImag, REGISTER_IMAG16_DATA(REGISTER_X));
}



void lnGammaCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnGammaCo16:", "cannot use NaN as X input of lnGamma", NULL, NULL);
    #endif
    return;
  }

  real39_t zReal, zImag;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &zReal);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &zImag);

  WP34S_ComplexLnGamma(&zReal, &zImag, &zReal, &zImag);

  realToReal16(&zReal, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&zImag, REGISTER_IMAG16_DATA(REGISTER_X));
}



void gammaRe34(void) {
  setRegisterAngularMode(REGISTER_X, AM_NONE);

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function gammaRe34:", "cannot use NaN as X input of gamma", NULL, NULL);
    #endif
    return;
  }

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function gammaRe34:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of gamma when flag D is not set", NULL, NULL);
      #endif
    }
    else {
      realToReal34((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X)) ? const_plusInfinity : const_NaN), REGISTER_REAL34_DATA(REGISTER_X));
    }
    return;
  }

  if(real34CompareLessEqual(REGISTER_REAL34_DATA(REGISTER_X), const34_0) && real34IsAnInteger(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function gammaRe34:", "cannot use a negative integer as X input of gamma when flag D is not set", NULL, NULL);
      #endif
    }
    else {
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    return;
  }

  real39_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  WP34S_Gamma(&x, &x);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



void lnGammaRe34(void) {
  real39_t xReal, xImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  setRegisterAngularMode(REGISTER_X, AM_NONE);

  if(realIsNaN(&xReal)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnGammaRe34:", "cannot use NaN as X input of lnGamma", NULL, NULL);
    #endif
    return;
  }

  if(realIsInfinite(&xReal)) {
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lngammaRe34:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of lngamma when flag D is not set", NULL, NULL);
      #endif
    }
    else {
      realToReal34((real34IsPositive(&xReal) ? const_plusInfinity : const_NaN), REGISTER_REAL34_DATA(REGISTER_X));
    }
    return;
  }

  if(realCompareLessEqual(&xReal, const_0)) { // x <= 0
    if(realIsAnInteger(&xReal)) {
      if(!getFlag(FLAG_DANGER)) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function lngammaRe34:", "cannot use a negative integer as X input of lngamma when flag D is not set", NULL, NULL);
        #endif
      }
      else {
        realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
      return;
    }
    else { // x is negative and not an integer
      realMinus(&xReal, &xImag, &ctxtReal39); // x.imag is used as a temp variable here
      realDivideRemainder(&xImag, const_2, &xImag, &ctxtReal39);
      if(realCompareGreaterThan(&xImag, const_1)) { // the result is a real
        WP34S_LnGamma(&xReal, &xReal);
        realToReal34(&xReal, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else { // the result is a complex
        if(getFlag(FLAG_CPXRES)) { // We can calculate a complex
          real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xImag);
          reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
          WP34S_Gamma(&xReal, &xReal);
          realSetPositiveSign(&xReal);
          realLn(&xReal, &xReal, &ctxtReal39);
          realToReal34(&xReal, REGISTER_REAL34_DATA(REGISTER_X));
          realToIntegralValue(&xImag, &xImag, DEC_ROUND_FLOOR, &ctxtReal39);
          realMultiply(&xImag, const_pi, &xImag, &ctxtReal39);
          realToReal34(&xImag, REGISTER_IMAG34_DATA(REGISTER_X));
        }
        else { // Domain error
          displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            showInfoDialog("In function lngammaRe34:", "cannot use a as X input of lngamma if gamma(X)<0 when flag I is not set", NULL, NULL);
          #endif
        }
      }
    }
    return;
  }


  WP34S_LnGamma(&xReal, &xReal);
  realToReal34(&xReal, REGISTER_REAL34_DATA(REGISTER_X));
}



void gammaCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function gammaCo34:", "cannot use NaN as X input of gamma", NULL, NULL);
    #endif
    return;
  }

  real39_t zReal, zImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &zReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &zImag);

  WP34S_ComplexGamma(&zReal, &zImag, &zReal, &zImag);

  realToReal34(&zReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&zImag, REGISTER_IMAG34_DATA(REGISTER_X));
}



void lnGammaCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnGammaCo34:", "cannot use NaN as X input of lnGamma", NULL, NULL);
    #endif
    return;
  }

  real39_t zReal, zImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &zReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &zImag);

  WP34S_ComplexLnGamma(&zReal, &zImag, &zReal, &zImag);

  realToReal34(&zReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&zImag, REGISTER_IMAG34_DATA(REGISTER_X));
}
