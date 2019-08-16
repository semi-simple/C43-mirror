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



void arctanLonI(void) {
  convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  WP34S_do_atan(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, currentAngularMode);
  convertRegister34To16(REGISTER_X);
  setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);
}



void arctanRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arctanRe16:", "cannot use NaN as X input of arctan", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_0_5, REGISTER_REAL34_DATA(REGISTER_X));
      if(real34IsNegative(REGISTER_REAL34_DATA(REGISTER_X))) {
        real34SetNegativeSign(REGISTER_REAL34_DATA(REGISTER_X));
      }
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_MULTPI, currentAngularMode);
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
    WP34S_do_atan(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
    convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, currentAngularMode);
    convertRegister34To16(REGISTER_X);
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

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))
  real34_t numer, denom;

  convertRegister16To34(REGISTER_X);

  // calculate (1 - iz) / (1 + iz)  with z = a + bi

  // 1 - (a + bi)i      1 - (a² + b²)             - 2a
  // -------------  =  ----------------  +  ---------------- i
  // 1 + (a + bi)i     a² + b² - 2b + 1     a² + b² - 2b + 1

  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // denom = a²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // denom = a² + b²
  real34Subtract(const34_1, &denom, &numer);                                                     // numer = 1 - (a² + b²)
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));                                            // b = -b
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), const34_2, &denom, &denom);                        // denom = a² + b² - 2b
  real34Add(&denom, const34_1, &denom);                                                          // denom = a² + b² - 2b + 1
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), const34_2, REGISTER_IMAG34_DATA(REGISTER_X)); // imag part = 2a
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));                                            // imag part = -2a
  real34Divide(&numer, &denom, REGISTER_REAL34_DATA(REGISTER_X));                                // real part = numer / denom
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &denom, REGISTER_IMAG34_DATA(REGISTER_X));      // imag part = -2a / denom

  // calculate ln((1 - iz) / (1 + iz))
  real34RectangularToPolar(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X));
  real34Ln(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), const34_0_5, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), const34_0_5, REGISTER_IMAG34_DATA(REGISTER_X));
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), &numer);
  real34ChangeSign(&numer);
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X));
  real34Copy(&numer, REGISTER_REAL34_DATA(REGISTER_X));

  convertRegister34To16(REGISTER_X);
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

  setRegisterTag(REGISTER_X, currentAngularMode);
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_0_5, REGISTER_REAL34_DATA(REGISTER_X));
      if(real34IsNegative(REGISTER_REAL34_DATA(REGISTER_X))) {
        real34SetNegativeSign(REGISTER_REAL34_DATA(REGISTER_X));
      }
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_MULTPI, currentAngularMode);
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arctanRe34:", "X = " STD_PLUS_MINUS STD_INFINITY " and Danger flag not set!", NULL, NULL);
      #endif
      return;
    }
  }
  else {
    WP34S_do_atan(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
    convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, currentAngularMode);
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

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))
  real34_t numer, denom;

  convertRegister16To34(REGISTER_X);

  // calculate (1 - iz) / (1 + iz)  with z = a + bi

  // 1 - (a + bi)i      1 - (a² + b²)             - 2a
  // -------------  =  ----------------  +  ---------------- i
  // 1 + (a + bi)i     a² + b² - 2b + 1     a² + b² - 2b + 1

  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // denom = a²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // denom = a² + b²
  real34Subtract(const34_1, &denom, &numer);                                                     // numer = 1 - (a² + b²)
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));                                            // b = -b
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), const34_2, &denom, &denom);                        // denom = a² + b² - 2b
  real34Add(&denom, const34_1, &denom);                                                          // denom = a² + b² - 2b + 1
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), const34_2, REGISTER_IMAG34_DATA(REGISTER_X)); // imag part = 2a
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));                                            // imag part = -2a
  real34Divide(&numer, &denom, REGISTER_REAL34_DATA(REGISTER_X));                                // real part = numer / denom
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &denom, REGISTER_IMAG34_DATA(REGISTER_X));      // imag part = -2a / denom

  // calculate ln((1 - iz) / (1 + iz))
  real34RectangularToPolar(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X));
  real34Ln(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), const34_0_5, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), const34_0_5, REGISTER_IMAG34_DATA(REGISTER_X));
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), &numer);
  real34ChangeSign(&numer);
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X));
  real34Copy(&numer, REGISTER_REAL34_DATA(REGISTER_X));
}
