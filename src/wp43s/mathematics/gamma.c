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



void (* const Gamma[13])(void) = {
// regX ==> 1            2            3            4            5             6             7             8             9             10             11           12           13
//          Long integer Real16       Complex16    Angle16      Time          Date          String        Real16 mat    Complex16 m   Short integer  Real34       Complex34    Angle34
            gammaLonI,   gammaRe16,   gammaCo16,   gammaAn16,   gammaError,   gammaError,   gammaError,   gammaError,   gammaError,   gammaError,    gammaRe34,   gammaCo34,   gammaAn34
};

void (* const lnGamma[13])(void) = {
// regX ==> 1            2            3            4            5             6             7             8             9             10             11           12           13
//          Long integer Real16       Complex16    Angle16      Time          Date          String        Real16 mat    Complex16 m   Short integer  Real34       Complex34    Angle34
            lnGammaLonI, lnGammaRe16, lnGammaCo16, lnGammaAn16, lnGammaError, lnGammaError, lnGammaError, lnGammaError, lnGammaError, lnGammaError,  lnGammaRe34, lnGammaCo34, lnGammaAn34
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
  realIc_t a;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &a);
  WP34S_Gamma(&a, &a);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void lnGammaLonI(void) {
  realIc_t a;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &a);
  WP34S_LnGamma(&a, &a);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void gammaRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function gammaRe16:", "cannot use NaN as X input of gamma", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  WP34S_Gamma(&a, &a);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void lnGammaRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnGammaRe16:", "cannot use NaN as X input of lnGamma", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  WP34S_LnGamma(&a, &a);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void gammaCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function gammaCo16:", "cannot use NaN as X input of gamma", NULL, NULL);
    #endif
    return;
  }

  complexIc_t z;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &z.real);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &z.imag);

  WP34S_ComplexGamma(&z, &z);

  realIcToReal16(&z.real, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&z.imag, REGISTER_IMAG16_DATA(REGISTER_X));
}



void lnGammaCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnGammaCo16:", "cannot use NaN as X input of lnGamma", NULL, NULL);
    #endif
    return;
  }

  complexIc_t z;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &z.real);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &z.imag);

  WP34S_ComplexLnGamma(&z, &z);

  realIcToReal16(&z.real, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&z.imag, REGISTER_IMAG16_DATA(REGISTER_X));
}



void gammaAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function gammaAn16:", "cannot use NaN as X input of gamma", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  WP34S_Gamma(&a, &a);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void lnGammaAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnGammaAn16:", "cannot use NaN as X input of lnGamma", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  WP34S_LnGamma(&a, &a);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void gammaRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function gammaRe34:", "cannot use NaN as X input of gamma", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  WP34S_Gamma(&a, &a);
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}



void lnGammaRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnGammaRe34:", "cannot use NaN as X input of lnGamma", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  WP34S_LnGamma(&a, &a);
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}



void gammaCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function gammaCo34:", "cannot use NaN as X input of gamma", NULL, NULL);
    #endif
    return;
  }

  complexIc_t z;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &z.real);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &z.imag);

  WP34S_ComplexGamma(&z, &z);

  realIcToReal34(&z.real, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&z.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



void lnGammaCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnGammaCo34:", "cannot use NaN as X input of lnGamma", NULL, NULL);
    #endif
    return;
  }

  complexIc_t z;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &z.real);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &z.imag);

  WP34S_ComplexLnGamma(&z, &z);

  realIcToReal34(&z.real, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&z.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



void gammaAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function gammaAn34:", "cannot use NaN as X input of gamma", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  WP34S_Gamma(&a, &a);
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}



void lnGammaAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnGammaAn34:", "cannot use NaN as X input of lnGamma", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  WP34S_LnGamma(&a, &a);
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}
