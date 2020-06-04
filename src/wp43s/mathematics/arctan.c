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



void (* const arctan[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1            2           3           4            5            6            7           8           9             10
//          Long integer Real34      Complex34   Time         Date         String       Real34 mat  Complex34 m Short integer Config data
            arctanLonI,  arctanReal, arctanCplx, arctanError, arctanError, arctanError, arctanRema, arctanCxma, arctanError,  arctanError
};



/********************************************//**
 * \brief Data type error in arctan
 *
 * \param void
 * \return void
 ***********************************************/
void arctanError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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



/********************************************//**
 * \brief regX ==> regL and arg(regX) = arctan(Im(regX) / Re(regX)) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnArg(uint16_t unusedParamButMandatory) {
  real_t real, imag;

  if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &real);
    real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &imag);
    realRectangularToPolar(&real, &imag, &real, &imag, &ctxtReal39);
    convertAngleFromTo(&imag, AM_RADIAN, currentAngularMode, &ctxtReal39);

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, currentAngularMode);
    realToReal34(&imag, REGISTER_REAL34_DATA(REGISTER_X));

    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate arg for %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnArg:", errorMessage, NULL, NULL);
    #endif
  }
}



void arctanLonI(void) {
  real_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  WP34S_Atan(&x, &x, &ctxtReal39);
  convertAngleFromTo(&x, AM_RADIAN, currentAngularMode, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, currentAngularMode);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



void arctanRema(void) {
  fnToBeCoded();
}



void arctanCxma(void) {
  fnToBeCoded();
}



void arctanReal(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getSystemFlag(FLAG_SPCRES)) {
      if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
        realToReal34(const_1on2, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        realToReal34(const_1on2, REGISTER_REAL34_DATA(REGISTER_X));
        real34SetNegativeSign(REGISTER_REAL34_DATA(REGISTER_X));
      }
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_MULTPI, currentAngularMode);
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arctanReal:", "X = " STD_PLUS_MINUS STD_INFINITY, NULL, NULL);
      #endif
      return;
    }
  }
  else {
    real_t x;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    WP34S_Atan(&x, &x, &ctxtReal39);
    convertAngleFromTo(&x, AM_RADIAN, currentAngularMode, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterAngularMode(REGISTER_X, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void arctanCplx(void) {
  real_t a, b, numer, denom;

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
  realRectangularToPolar(&a, &b, &a, &b, &ctxtReal39);
  WP34S_Ln(&a, &a, &ctxtReal39);

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))
  realMultiply(&a, const_1on2, &a, &ctxtReal39);
  realMultiply(&b, const_1on2, &b, &ctxtReal39);
  realChangeSign(&b);

  realToReal34(&b, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
}
