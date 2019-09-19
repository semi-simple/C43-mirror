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



void (* const arctan[13])(void) = {
// regX ==> 1            2           3           4            5           6            7            8           9           10            11          12          13
//          Long integer Real16      Complex16   Angle16     Time         Date         String       Real16 mat  Complex16 m Short integer Real34      Complex34   Angle34
            arctanLonI,  arctanRe16, arctanCo16, arctanRe16, arctanError, arctanError, arctanError, arctanRm16, arctanCm16, arctanError,  arctanRe34, arctanCo34, arctanRe34
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



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 ***********************************************************************/

void arctanLonI(void) {
  realIc_t a;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &a);
  WP34S_Atan(&a, &a);
  convertAngleIcFromTo(&a, AM_RADIAN, currentAngularMode);
  reallocateRegister(REGISTER_X, dtAngle16, REAL16_SIZE, currentAngularMode);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
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
        realIcToReal16(const_0_5, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        realIcToReal16(const_0_5, REGISTER_REAL16_DATA(REGISTER_X));
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
    realIc_t a;

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
    WP34S_Atan(&a, &a);
    convertAngleIcFromTo(&a, AM_RADIAN, currentAngularMode);
    realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);

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

  realIc_t a, b, numer, denom;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))

  // calculate (1 - iz) / (1 + iz)  with z = a + bi

  // 1 - (a + bi)i      1 - (a² + b²)             - 2a
  // -------------  =  ----------------  +  ---------------- i
  // 1 + (a + bi)i     a² + b² - 2b + 1     a² + b² - 2b + 1

  realIcMultiply(&a, &a, &denom);          // denom = a²
  realIcFMA(&b, &b, &denom, &denom);       // denom = a² + b²
  realIcSubtract(const_1, &denom, &numer); // numer = 1 - (a² + b²)
  realIcChangeSign(&b);                    // b = -b
  realIcFMA(&b, const_2, &denom, &denom);  // denom = a² + b² - 2b
  realIcAdd(&denom, const_1, &denom);      // denom = a² + b² - 2b + 1
  realIcMultiply(&a, const_2, &b);         // imag part = 2a
  realIcChangeSign(&b);                    // imag part = -2a
  realIcDivide(&numer, &denom, &a);        // real part = numer / denom
  realIcDivide(&b, &denom, &b);            // imag part = -2a / denom

  // calculate ln((1 - iz) / (1 + iz))
  realIcRectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))
  realIcMultiply(&a, const_0_5, &a);
  realIcMultiply(&b, const_0_5, &b);
  realIcChangeSign(&b);

  realIcToReal16(&b, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&a, REGISTER_IMAG16_DATA(REGISTER_X));
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
        realIcToReal34(const_0_5, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        realIcToReal34(const_0_5, REGISTER_REAL34_DATA(REGISTER_X));
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
    realIc_t a;

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
    WP34S_Atan(&a, &a);
    convertAngleIcFromTo(&a, AM_RADIAN, currentAngularMode);
    realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtAngle34, currentAngularMode);

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

  realIc_t a, b, numer, denom;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))

  // calculate (1 - iz) / (1 + iz)  with z = a + bi

  // 1 - (a + bi)i      1 - (a² + b²)             - 2a
  // -------------  =  ----------------  +  ---------------- i
  // 1 + (a + bi)i     a² + b² - 2b + 1     a² + b² - 2b + 1

  realIcMultiply(&a, &a, &denom);          // denom = a²
  realIcFMA(&b, &b, &denom, &denom);       // denom = a² + b²
  realIcSubtract(const_1, &denom, &numer); // numer = 1 - (a² + b²)
  realIcChangeSign(&b);                    // b = -b
  realIcFMA(&b, const_2, &denom, &denom);  // denom = a² + b² - 2b
  realIcAdd(&denom, const_1, &denom);      // denom = a² + b² - 2b + 1
  realIcMultiply(&a, const_2, &b);         // imag part = 2a
  realIcChangeSign(&b);                    // imag part = -2a
  realIcDivide(&numer, &denom, &a);        // real part = numer / denom
  realIcDivide(&b, &denom, &b);            // imag part = -2a / denom

  // calculate ln((1 - iz) / (1 + iz))
  realIcRectangularToPolar(&a, &b, &a, &b);
  WP34S_Ln(&a, &a);

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))
  realIcMultiply(&a, const_0_5, &a);
  realIcMultiply(&b, const_0_5, &b);
  realIcChangeSign(&b);

  realIcToReal34(&b, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
}
