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
 * \file arcsin.c
 ***********************************************/

#include "wp43s.h"



void (* const arcsin[13])(void) = {
// regX ==> 1            2           3           4           5            6            7            8           9           10            11          12          13
//          Long integer Real16      Complex16   Angle16     Time         Date         String       Real16 mat  Complex16 m Short integer Real34      Complex34   Angle34
            arcsinLonI,  arcsinRe16, arcsinCo16, arcsinRe16, arcsinError, arcsinError, arcsinError, arcsinRm16, arcsinCm16, arcsinError,  arcsinRe34, arcsinCo34, arcsinRe34
};



/********************************************//**
 * \brief Data type error in arcsin
 *
 * \param void
 * \return void
 ***********************************************/
void arcsinError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate arcsin for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnArcsin:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and arcsin(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnArcsin(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  arcsin[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void arcsinLonI(void) {
 convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  if(real16CompareAbsGreaterThan(REGISTER_REAL16_DATA(REGISTER_X), const16_1)) {
    if(getFlag(FLAG_CPXRES)) {
      real16_t temp;

      real16Copy(REGISTER_REAL16_DATA(REGISTER_X), &temp);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
      real16ToReal34(&temp, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arcsinCo34();
      convertRegister34To16(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arcsinLonI:", "|X| > 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }

  if(!real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(real16IsNegative(REGISTER_REAL16_DATA(REGISTER_X))) {
      real16Copy(const16_0_5, REGISTER_REAL16_DATA(REGISTER_X));
      real16ChangeSign(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      real16Copy(const16_0_5, REGISTER_REAL16_DATA(REGISTER_X));
    }
    convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_MULTPI, currentAngularMode);
  }

  setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);
}



void arcsinRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinRe16:", "cannot use NaN as X input of arcsin", NULL, NULL);
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
      arcsinCo34();
      convertRegister34To16(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arcsinRe16:", "|X| > 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }

  convertRegister16To34(REGISTER_X);
  WP34S_do_asin(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, currentAngularMode);
  convertRegister34To16(REGISTER_X);
  setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
  }
}



void arcsinCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinCo16:", "cannot use NaN as X input of arcsin", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);

  // arcsin(z) = -i.ln(iz + sqrt(1 - z²))
  real34_t real34, imag34, magnitude34, theta34;

  // calculate z²   real part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &real34);
  real34ChangeSign(&real34);
  real34FMA(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &real34, &real34);

  // calculate z²   imaginary part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &imag34);
  real34Multiply(&imag34, const34_2, &imag34);

  // calculate 1 - z²
  real34Subtract(const34_1, &real34, &real34);
  real34ChangeSign(&imag34);

  // calculate sqrt(1 - z²)
  real34RectangularToPolar(&real34, &imag34, &magnitude34, &theta34);
  real34SquareRoot(&magnitude34, &magnitude34);
  real34Multiply(&theta34, const34_0_5, &theta34);
  real34PolarToRectangular(&magnitude34, &theta34, &real34, &imag34);

  // calculate iz + sqrt(1 - z²)
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));
  real34Add(&real34, REGISTER_IMAG34_DATA(REGISTER_X), &real34);
  real34Add(&imag34, REGISTER_REAL34_DATA(REGISTER_X), &imag34);

  // calculate ln(iz + sqrt(1 - z²))
  real34RectangularToPolar(&real34, &imag34, &real34, REGISTER_REAL34_DATA(REGISTER_X));
  real34Ln(&real34, &real34);

  // calculate = -i.ln(iz + sqrt(1 - z²))
  real34ChangeSign(&real34);
  real34Copy(&real34, REGISTER_IMAG34_DATA(REGISTER_X));

  convertRegister34To16(REGISTER_X);
}



void arcsinRm16(void) {
  fnToBeCoded();
}



void arcsinCm16(void) {
  fnToBeCoded();
}



void arcsinRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinRe34:", "cannot use NaN as X input of arcsin", NULL, NULL);
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
      arcsinCo34();
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arcsinRe34:", "|X| > 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }

  WP34S_do_asin(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, currentAngularMode);
  setRegisterDataType(REGISTER_X, dtAngle34, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void arcsinCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinCo34:", "cannot use NaN as X input of arcsin", NULL, NULL);
    #endif
    return;
  }

  // arcsin(z) = -i.ln(iz + sqrt(1 - z²))
  real34_t real34, imag34, magnitude34, theta34;

  // calculate z²   real part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &real34);
  real34ChangeSign(&real34);
  real34FMA(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &real34, &real34);

  // calculate z²   imaginary part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &imag34);
  real34Multiply(&imag34, const34_2, &imag34);

  // calculate 1 - z²
  real34Subtract(const34_1, &real34, &real34);
  real34ChangeSign(&imag34);

  // calculate sqrt(1 - z²)
  real34RectangularToPolar(&real34, &imag34, &magnitude34, &theta34);
  real34SquareRoot(&magnitude34, &magnitude34);
  real34Multiply(&theta34, const34_0_5, &theta34);
  real34PolarToRectangular(&magnitude34, &theta34, &real34, &imag34);

  // calculate iz + sqrt(1 - z²)
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));
  real34Add(&real34, REGISTER_IMAG34_DATA(REGISTER_X), &real34);
  real34Add(&imag34, REGISTER_REAL34_DATA(REGISTER_X), &imag34);

  // calculate ln(iz + sqrt(1 - z²))
  real34RectangularToPolar(&real34, &imag34, &real34, REGISTER_REAL34_DATA(REGISTER_X));
  real34Ln(&real34, &real34);

  // calculate = -i.ln(iz + sqrt(1 - z²))
  real34ChangeSign(&real34);
  real34Copy(&real34, REGISTER_IMAG34_DATA(REGISTER_X));
}
