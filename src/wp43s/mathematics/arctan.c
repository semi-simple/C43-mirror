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
 * \file arctan.c
 ***********************************************/

#include "wp43s.h"



void (* const arctan[12])(void) = {
// regX ==> 1            2           3           4            5            6            7            8           9           10            11          12
//          Long integer Real16      Complex16   Angle16      Time         Date         String       Real16 mat  Complex16 m Short integer Real34      Complex34
            arctanLonI,  arctanRe16, arctanCo16, arctanError, arctanError, arctanError, arctanError, arctanRm16, arctanCm16, arctanError,  arctanRe34, arctanCo34
};



/********************************************//**
 * \brief Data type error in arctan
 *
 * \param void
 * \return void
 ***********************************************/
void arctanError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate arctan for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnArctan:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and arctan(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnArctan(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  arctan[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void arctanLonI(void) {
  real39_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  WP34S_Atan(&x, &x);
  convertAngle39FromTo(&x, AM_RADIAN, currentAngularMode);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, currentAngularMode);
  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
}



void arctanRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arctanRe16:", "cannot use NaN as X input of arctan", NULL, NULL);
    #endif
    return;
  }

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      if(real16IsPositive(REGISTER_REAL16_DATA(REGISTER_X))) {
        realToReal16(const_0_5, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        realToReal16(const_0_5, REGISTER_REAL16_DATA(REGISTER_X));
        real16SetNegativeSign(REGISTER_REAL16_DATA(REGISTER_X));
      }
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_MULTPI, currentAngularMode);
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arctanRe16:", "X = " STD_PLUS_MINUS STD_INFINITY, NULL, NULL);
      #endif
      return;
    }
  }
  else {
    real39_t x;

    real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
    WP34S_Atan(&x, &x);
    convertAngle39FromTo(&x, AM_RADIAN, currentAngularMode);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }

  setRegisterAngularMode(REGISTER_X, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
  }
}



void arctanCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arctanCo16:", "cannot use NaN as X input of arctan", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b, numer, denom;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))

  // calculate (1 - iz) / (1 + iz)  with z = a + bi

  // 1 - (a + bi)i      1 - (a² + b²)             - 2a
  // -------------  =  ----------------  +  ---------------- i
  // 1 + (a + bi)i     a² + b² - 2b + 1     a² + b² - 2b + 1

  realMultiply(&a, &a, &denom, &ctxtReal39);          // denom = a²
  realFMA(&b, &b, &denom, &denom, &ctxtReal39);       // denom = a² + b²
  realSubtract(const_1, &denom, &numer, &ctxtReal39); // numer = 1 - (a² + b²)
  realChangeSign(&b);                                 // b = -b
  realFMA(&b, const_2, &denom, &denom, &ctxtReal39);  // denom = a² + b² - 2b
  realAdd(&denom, const_1, &denom, &ctxtReal39);      // denom = a² + b² - 2b + 1
  realMultiply(&a, const_2, &b, &ctxtReal39);         // imag part = 2a
  realChangeSign(&b);                                 // imag part = -2a
  realDivide(&numer, &denom, &a, &ctxtReal39);        // real part = numer / denom
  realDivide(&b, &denom, &b, &ctxtReal39);            // imag part = -2a / denom

  // calculate ln((1 - iz) / (1 + iz))
  real39RectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))
  realMultiply(&a, const_0_5, &a, &ctxtReal39);
  realMultiply(&b, const_0_5, &b, &ctxtReal39);
  realChangeSign(&b);

  realToReal16(&b, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&a, REGISTER_IMAG16_DATA(REGISTER_X));
}



void arctanRm16(void) {
  fnToBeCoded();
}



void arctanCm16(void) {
  fnToBeCoded();
}



void arctanRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arctanRe34:", "cannot use NaN as X input of arctan", NULL, NULL);
    #endif
    return;
  }

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
        realToReal34(const_0_5, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        realToReal34(const_0_5, REGISTER_REAL34_DATA(REGISTER_X));
        real34SetNegativeSign(REGISTER_REAL34_DATA(REGISTER_X));
      }
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_MULTPI, currentAngularMode);
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arctanRe34:", "X = " STD_PLUS_MINUS STD_INFINITY, NULL, NULL);
      #endif
      return;
    }
  }
  else {
    real39_t x;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    WP34S_Atan(&x, &x);
    convertAngle39FromTo(&x, AM_RADIAN, currentAngularMode);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterAngularMode(REGISTER_X, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void arctanCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arctanCo34:", "cannot use NaN as X input of arctan", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b, numer, denom;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))

  // calculate (1 - iz) / (1 + iz)  with z = a + bi

  // 1 - (a + bi)i      1 - (a² + b²)             - 2a
  // -------------  =  ----------------  +  ---------------- i
  // 1 + (a + bi)i     a² + b² - 2b + 1     a² + b² - 2b + 1

  realMultiply(&a, &a, &denom, &ctxtReal39);          // denom = a²
  realFMA(&b, &b, &denom, &denom, &ctxtReal39);       // denom = a² + b²
  realSubtract(const_1, &denom, &numer, &ctxtReal39); // numer = 1 - (a² + b²)
  realChangeSign(&b);                                 // b = -b
  realFMA(&b, const_2, &denom, &denom, &ctxtReal39);  // denom = a² + b² - 2b
  realAdd(&denom, const_1, &denom, &ctxtReal39);      // denom = a² + b² - 2b + 1
  realMultiply(&a, const_2, &b, &ctxtReal39);         // imag part = 2a
  realChangeSign(&b);                                 // imag part = -2a
  realDivide(&numer, &denom, &a, &ctxtReal39);        // real part = numer / denom
  realDivide(&b, &denom, &b, &ctxtReal39);            // imag part = -2a / denom

  // calculate ln((1 - iz) / (1 + iz))
  real39RectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))
  realMultiply(&a, const_0_5, &a, &ctxtReal39);
  realMultiply(&b, const_0_5, &b, &ctxtReal39);
  realChangeSign(&b);

  realToReal34(&b, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
}
