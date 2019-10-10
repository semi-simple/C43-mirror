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
 * \file arctanh.c
 ***********************************************/

#include "wp43s.h"



void (* const arctanh[12])(void) = {
// regX ==> 1             2            3            4             5             6             7             8            9            10             11           12
//          Long integer  Real16       Complex16    Angle16       Time          Date          String        Real16 mat   Complex16 m  Short integer  Real34       Complex34
            arctanhLonI,  arctanhRe16, arctanhCo16, arctanhError, arctanhError, arctanhError, arctanhError, arctanhRm16, arctanhCm16, arctanhError,  arctanhRe34, arctanhCo34
};



/********************************************//**
 * \brief Data type error in arctanh
 *
 * \param void
 * \return void
 ***********************************************/
void arctanhError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate arctanh for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnArctanh:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and arctanh(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnArctanh(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  arctanh[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void arctanhLonI(void) {
  realIc_t x;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &x);

  if(realIcIsZero(&x)) {
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
    real16Zero(REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(realIcCompareEqual(&x, const_1)) {
      if(getFlag(FLAG_DANGER)) {
        reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
        realIcToReal16(const_plusInfinity, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function arctanhLonI:", "X = 1", "and DANGER flag is not set!", NULL);
        #endif
       }
    }
    else if(realIcCompareEqual(&x, const__1)) {
      if(getFlag(FLAG_DANGER)) {
        reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
        realIcToReal16(const_minusInfinity, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function arctanhLonI:", "X = -1", "and DANGER flag is not set!", NULL);
        #endif
       }
    }
    else {
      if(getFlag(FLAG_CPXRES)) {
        reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
        realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
        real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
        arctanhCo34();
        convertRegister34To16(REGISTER_X);
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function arctanhLonI:", "|X| > 1", "and CPXRES is not set!", NULL);
        #endif
      }
    }
  }
}



void arctanhRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arctanhRe16:", "cannot use NaN as X input of arctanh", NULL, NULL);
    #endif
    return;
  }

  realIc_t x;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);

  if(realIcIsZero(&x)) {
    real16Zero(REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(realIcCompareEqual(&x, const_1)) {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal16(const_plusInfinity, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function arctanhRe16:", "X = 1", "and DANGER flag is not set!", NULL);
        #endif
       }
    }
    else if(realIcCompareEqual(&x, const__1)) {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal16(const_minusInfinity, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function arctanhRe16:", "X = -1", "and DANGER flag is not set!", NULL);
        #endif
       }
    }
    else {
      if(realIcCompareAbsGreaterThan(&x, const_1)) {
        if(getFlag(FLAG_CPXRES)) {
          reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
          realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
          real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
          arctanhCo34();
          convertRegister34To16(REGISTER_X);
        }
        else {
          displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            showInfoDialog("In function arctanhRe16:", "|X| > 1", "and CPXRES is not set!", NULL);
          #endif
        }
      }
      else {
        WP34S_ArcTanh(&x, &x);
        realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      }
    }
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void arctanhCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arctanhCo16:", "cannot use NaN as X input of arctanh", NULL, NULL);
    #endif
    return;
  }

  //                    1       1 + (a + ib)
  // arctanh(a + i b) = - * ln( ------------ )
  //                    2       1 - (a + ib)

  complexIc_t numer, denom;

  // numer = 1 + (a + ib)
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &numer.real);
  realIcAdd(&numer.real, const_1, &numer.real);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &numer.imag);

  // denom = 1 - (a + ib)
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &denom.real);
  realIcSubtract(const_1, &denom.real, &denom.real);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &denom.imag);
  realIcChangeSign(&denom.imag);

  // numer = (1 + (a + ib)) / (1 - (a + ib)
  divCoIcCoIc(&numer, &denom, &numer);

  // numer = ln((1 + (a + ib)) / (1 - (a + ib))
  lnCoIc(&numer, &numer);

  // 1/2 * ln((1 + (a + ib)) / (1 - (a + ib))
  realIcMultiply(&numer.real, const_1on2, &numer.real);
  realIcMultiply(&numer.imag, const_1on2, &numer.imag);

  realIcToReal16(&numer.real, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&numer.imag, REGISTER_IMAG16_DATA(REGISTER_X));
}



void arctanhRm16(void) {
  fnToBeCoded();
}



void arctanhCm16(void) {
  fnToBeCoded();
}



void arctanhRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arctanhRe34:", "cannot use NaN as X input of arctanh", NULL, NULL);
    #endif
    return;
  }

  realIc_t x;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);

  if(realIcIsZero(&x)) {
    real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(realIcCompareEqual(&x, const_1)) {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function arctanhRe34:", "X = 1", "and DANGER flag is not set!", NULL);
        #endif
       }
    }
    else if(realIcCompareEqual(&x, const__1)) {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal34(const_minusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function arctanhRe34:", "X = -1", "and DANGER flag is not set!", NULL);
        #endif
       }
    }
    else {
      if(realIcCompareAbsGreaterThan(&x, const_1)) {
        if(getFlag(FLAG_CPXRES)) {
          reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
          realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
          real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
          arctanhCo34();
        }
        else {
          displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            showInfoDialog("In function arctanhRe34:", "|X| > 1", "and CPXRES is not set!", NULL);
          #endif
        }
      }
      else {
        WP34S_ArcTanh(&x, &x);
        realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      }
    }
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void arctanhCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arctanhCo34:", "cannot use NaN as X input of arctanh", NULL, NULL);
    #endif
    return;
  }

  //                    1       1 + (a + ib)
  // arctanh(a + i b) = - * ln( ------------ )
  //                    2       1 - (a + ib)

  complexIc_t numer, denom;

  // numer = 1 + (a + ib)
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &numer.real);
  realIcAdd(&numer.real, const_1, &numer.real);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &numer.imag);

  // denom = 1 - (a + ib)
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &denom.real);
  realIcSubtract(const_1, &denom.real, &denom.real);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &denom.imag);
  realIcChangeSign(&denom.imag);

  // numer = (1 + (a + ib)) / (1 - (a + ib)
  divCoIcCoIc(&numer, &denom, &numer);

  // numer = ln((1 + (a + ib)) / (1 - (a + ib))
  lnCoIc(&numer, &numer);

  // 1/2 * ln((1 + (a + ib)) / (1 - (a + ib))
  realIcMultiply(&numer.real, const_1on2, &numer.real);
  realIcMultiply(&numer.imag, const_1on2, &numer.imag);

  realIcToReal34(&numer.real, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&numer.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}
