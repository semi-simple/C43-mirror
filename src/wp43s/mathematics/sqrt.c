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
 * \file sqrt.c
 ***********************************************/

#include "wp43s.h"



void (* const Sqrt[12])(void) = {
// regX ==> 1            2          3          4           5           6           7           8           9            10             11         12
//          Big integer  real16     complex16  angle       Time        Date        String      real16 mat  complex16 m  Small integer  real34     complex34
            sqrtBigI,    sqrtRe16,  sqrtCo16,  errorSqrt,  errorSqrt,  errorSqrt,  errorSqrt,  sqrtRm16,   sqrtCm16,    sqrtSmaI,      sqrtRe34,  sqrtCo34
};



/********************************************//**
 * \brief Data type error in sqrt
 *
 * \param void
 * \return void
 ***********************************************/
void errorSqrt(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate sqrt for %s", getRegisterDataTypeName(op1, true, false));
    showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void sqrtToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "sqrt(%s)", getRegisterDataTypeName(op1, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and sqrt(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSquareRoot(uint16_t unusedParamButMandatory) {
  if(Sqrt[getRegisterDataType(REGISTER_X)] != errorSqrt) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    op1    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op1);

    Sqrt[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(op1);

    if(lastErrorCode != 0) {
      restoreStack();
    }

    refreshStack();
  }
  else {
    errorSqrt();
  }
}



void sqrtBigI(void) {
  convertBigIntegerRegisterToReal34Register(op1, op1);

  if(!real34IsNegative(REGISTER_REAL34_DATA(op1))) {
    reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
    real34SquareRoot(REGISTER_REAL34_DATA(op1), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
  else if(getFlag(FLAG_CPXRES)) {
    real34_t real34;

    real34Copy(REGISTER_REAL34_DATA(op1), &real34);
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
    real34SetPositiveSign(&real34);
    real34SquareRoot(&real34, REGISTER_IMAG34_DATA(result));
    real34Zero(REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
  else {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // 1 = argument exceeds functions domain
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative big integer when flag I is not set!");
      showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
    #endif
  }

  convertRegister34To16(REGISTER_X);
}



void sqrtRe16(void) {
  convertRegister16To34(op1);

  if(!real34IsNegative(REGISTER_REAL34_DATA(op1))) {
    reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
    real34SquareRoot(REGISTER_REAL34_DATA(op1), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
  else if(getFlag(FLAG_CPXRES)) {
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
    real34Copy(REGISTER_REAL34_DATA(op1), REGISTER_IMAG34_DATA(result));
    real34SetPositiveSign(REGISTER_IMAG34_DATA(result));
    real34Zero(REGISTER_REAL34_DATA(result));
    real34SquareRoot(REGISTER_IMAG34_DATA(result), REGISTER_IMAG34_DATA(result));
    convertRegister34To16(result);
  }
  else {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // 1 = argument exceeds functions domain
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative real when flag I is not set!");
      showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
    #endif
  }
}



void sqrtCo16(void) {
  real34_t magnitude34, theta34;

  convertRegister16To34(op1);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34RectangularToPolar(REGISTER_REAL34_DATA(op1), REGISTER_IMAG34_DATA(op1), &magnitude34, &theta34);
  real34SquareRoot(&magnitude34, &magnitude34);
  real34Multiply(&theta34, const34_0_5, &theta34);
  convertAngle34FromTo(&theta34, angularMode, AM_RADIAN);
  real34PolarToRectangular(&magnitude34, &theta34, REGISTER_REAL34_DATA(result), REGISTER_IMAG34_DATA(result));
  convertRegister34To16(result);
}



void sqrtRm16(void) {
  sqrtToBeCoded();
}



void sqrtCm16(void) {
  sqrtToBeCoded();
}



void sqrtSmaI(void) {
  *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_intSqrt(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
}



void sqrtRe34(void) {
  if(!real34IsNegative(REGISTER_REAL34_DATA(op1))) {
    real34SquareRoot(REGISTER_REAL34_DATA(op1), REGISTER_REAL34_DATA(result));
  }
  else if(getFlag(FLAG_CPXRES)) {
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
    real34Copy(REGISTER_REAL34_DATA(op1), REGISTER_IMAG34_DATA(result));
    real34SetPositiveSign(REGISTER_IMAG34_DATA(result));
    real34Zero(REGISTER_REAL34_DATA(result));
    real34SquareRoot(REGISTER_IMAG34_DATA(result), REGISTER_IMAG34_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // 1 = argument exceeds functions domain
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative real when flag I is not set!");
      showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
    #endif
  }
}



void sqrtCo34(void) {
  real34_t magnitude34, theta34;

  real34RectangularToPolar(REGISTER_REAL34_DATA(op1), REGISTER_IMAG34_DATA(op1), &magnitude34, &theta34);
  real34SquareRoot(&magnitude34, &magnitude34);
  real34Multiply(&theta34, const34_0_5, &theta34);
  convertAngle34FromTo(&theta34, angularMode, AM_RADIAN);
  real34PolarToRectangular(&magnitude34, &theta34, REGISTER_REAL34_DATA(result), REGISTER_IMAG34_DATA(result));
}
