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
// regX ==> 1             2             3             4              5              6              7              8              9              10             11           12
//          Long integer  real16        complex16     Date           Time           Date           String         real16 mat     complex16 m    Short integer  real34       complex34
            gammaLonI,    gammaRe16,    gammaCo16,    gammaError,    gammaError,    gammaError,    gammaError,    gammaError,    gammaError,    gammaError,    gammaRe34,   gammaCo34
};

void (* const lnGamma[12])(void) = {
// regX ==> 1             2             3             4              5              6              7              8              9              10             11           12
//          Long integer  real16        complex16     Date           Time           Date           String         real16 mat     complex16 m    Short integer  real34       complex34
            lnGammaLonI,  lnGammaRe16,  lnGammaCo16,  lnGammaError,  lnGammaError,  lnGammaError,  lnGammaError,  lnGammaError,  lnGammaError,  lnGammaError,  lnGammaRe34, lnGammaCo34
};



/********************************************//**
 * \brief Data type error in gamma
 *
 * \param void
 * \return void
 ***********************************************/
void gammaError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
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
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
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

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  Gamma[getRegisterDataType(REGISTER_X)]();

  adjustResult(result, false, true, opX, -1, -1);
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

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  lnGamma[getRegisterDataType(REGISTER_X)]();

  adjustResult(result, false, true, opX, -1, -1);
}



void gammaLonI(void) {
  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  WP34S_real34Gamma(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  convertRegister34To16(result);
}



void lnGammaLonI(void) {
  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  WP34S_real34LnGamma(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  convertRegister34To16(result);
}



void gammaRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function gammaRe16:", "cannot use NaN as an input of gamma", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  WP34S_real34Gamma(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  convertRegister34To16(result);
}



void lnGammaRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnGammaRe16:", "cannot use NaN as an input of lnGamma", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  WP34S_real34LnGamma(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  convertRegister34To16(result);
}



void gammaCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function gammaCo16:", "cannot use NaN as an input of gamma", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



void lnGammaCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnGammaCo16:", "cannot use NaN as an input of lnGamma", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



void gammaRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function gammaRe34:", "cannot use NaN as an input of gamma", NULL, NULL);
    #endif
    return;
  }

  WP34S_real34Gamma(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



void lnGammaRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnGammaRe34:", "cannot use NaN as an input of lnGamma", NULL, NULL);
    #endif
    return;
  }

  WP34S_real34LnGamma(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



void gammaCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function gammaCo34:", "cannot use NaN as an input of gamma", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



void lnGammaCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnGammaCo34:", "cannot use NaN as an input of lnGamma", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}
