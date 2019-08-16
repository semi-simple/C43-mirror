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
 * \file arccos.c
 ***********************************************/

#include "wp43s.h"



void (* const arccos[13])(void) = {
// regX ==> 1            2           3           4           5            6            7            8           9           10            11          12          13
//          Long integer Real16      Complex16   Angle16     Time         Date         String       Real16 mat  Complex16 m Short integer Real34      Complex34   Angle34
            arccosLonI,  arccosRe16, arccosCo16, arccosRe16, arccosError, arccosError, arccosError, arccosRm16, arccosCm16, arccosError,  arccosRe34, arccosCo34, arccosRe34
};



/********************************************//**
 * \brief Data type error in arccos
 *
 * \param void
 * \return void
 ***********************************************/
void arccosError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate arccos for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnArccos:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and arccos(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnArccos(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  arccos[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void arccosLonI(void) {
 convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  if(real16CompareAbsGreaterThan(REGISTER_REAL16_DATA(REGISTER_X), const16_1)) {
    if(getFlag(FLAG_CPXRES)) {
      real16_t temp;

      real16Copy(REGISTER_REAL16_DATA(REGISTER_X), &temp);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
      real16ToReal34(&temp, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arccosCo34();
      convertRegister34To16(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arccosLonI:", "|X| > 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    real16Copy(const16_0_5, REGISTER_REAL16_DATA(REGISTER_X));
    convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_MULTPI, currentAngularMode);
  }
  else {
    if(real16IsNegative(REGISTER_REAL16_DATA(REGISTER_X))) {
      real16Copy(const16_1, REGISTER_REAL16_DATA(REGISTER_X));
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_MULTPI, currentAngularMode);
    }
    else{
      real16Zero(REGISTER_REAL16_DATA(REGISTER_X));
    }
  }

  setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);
}



void arccosRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosRe16:", "cannot use NaN as X input of arccos", NULL, NULL);
    #endif
    return;
  }

  if(real16CompareAbsGreaterThan(REGISTER_REAL16_DATA(REGISTER_X), const16_1)) {
    if(getFlag(FLAG_CPXRES)) {
      real16_t temp;

      real16Copy(REGISTER_REAL16_DATA(REGISTER_X), &temp);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
      real16ToReal34(&temp, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arccosCo34();
      convertRegister34To16(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arccosRe16:", "|X| > 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }

  convertRegister16To34(REGISTER_X);
  WP34S_do_acos(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, currentAngularMode);
  convertRegister34To16(REGISTER_X);
  setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
  }
}



void arccosCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosCo16:", "cannot use NaN as X input of arccos", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);

  // arccos(z) = -i.ln(z + sqrt(z² - 1))
  real34_t real34, imag34, magnitude34, theta34;

  // calculate z²   real part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &real34);
  real34ChangeSign(&real34);
  real34FMA(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &real34, &real34);

  // calculate z²   imaginary part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &imag34);
  real34Multiply(&imag34, const34_2, &imag34);

  // calculate z² - 1
  real34Subtract(&real34, const34_1, &real34);

  // calculate sqrt(z² - 1)
  real34RectangularToPolar(&real34, &imag34, &magnitude34, &theta34);
  real34SquareRoot(&magnitude34, &magnitude34);
  real34Multiply(&theta34, const34_0_5, &theta34);
  real34PolarToRectangular(&magnitude34, &theta34, &real34, &imag34);

  // calculate z + sqrt(z² - 1)
  real34Add(REGISTER_REAL34_DATA(REGISTER_X), &real34, &real34);
  real34Add(REGISTER_IMAG34_DATA(REGISTER_X), &imag34, &imag34);

  // calculate ln(z + sqtr(z² - 1))
  real34RectangularToPolar(&real34, &imag34, &real34, REGISTER_REAL34_DATA(REGISTER_X));
  real34Ln(&real34, &real34);

  // calculate = -i.ln(z + sqtr(z² - 1))
  real34ChangeSign(&real34);
  real34Copy(&real34, REGISTER_IMAG34_DATA(REGISTER_X));

  convertRegister34To16(REGISTER_X);
}



void arccosRm16(void) {
  fnToBeCoded();
}



void arccosCm16(void) {
  fnToBeCoded();
}



void arccosRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosRe34:", "cannot use NaN as X input of arccos", NULL, NULL);
    #endif
    return;
  }

  if(real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(REGISTER_X), const34_1)) {
    if(getFlag(FLAG_CPXRES)) {
      real34_t temp;

      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &temp);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
      real34Copy(&temp, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arccosCo34();
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arccosRe34:", "|X| > 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }

  WP34S_do_acos(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, currentAngularMode);
  setRegisterDataType(REGISTER_X, dtAngle34, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void arccosCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosCo34:", "cannot use NaN as X input of arccos", NULL, NULL);
    #endif
    return;
  }

  // arccos(z) = -i.ln(z + sqrt(z² - 1))
  real34_t real34, imag34, magnitude34, theta34;

  // calculate z²   real part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &real34);
  real34ChangeSign(&real34);
  real34FMA(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &real34, &real34);

  // calculate z²   imaginary part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &imag34);
  real34Multiply(&imag34, const34_2, &imag34);

  // calculate z² - 1
  real34Subtract(&real34, const34_1, &real34);

  // calculate sqrt(z² - 1)
  real34RectangularToPolar(&real34, &imag34, &magnitude34, &theta34);
  real34SquareRoot(&magnitude34, &magnitude34);
  real34Multiply(&theta34, const34_0_5, &theta34);
  real34PolarToRectangular(&magnitude34, &theta34, &real34, &imag34);

  // calculate z + sqrt(z² - 1)
  real34Add(REGISTER_REAL34_DATA(REGISTER_X), &real34, &real34);
  real34Add(REGISTER_IMAG34_DATA(REGISTER_X), &imag34, &imag34);

  // calculate ln(z + sqrt(z² - 1))
  real34RectangularToPolar(&real34, &imag34, &real34, REGISTER_REAL34_DATA(REGISTER_X));
  real34Ln(&real34, &real34);

  // calculate = -i.ln(z + sqrt(z² - 1))
  real34ChangeSign(&real34);
  real34Copy(&real34, REGISTER_IMAG34_DATA(REGISTER_X));
}
