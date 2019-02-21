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
            sqrtBigI,    sqrtRe16,  sqrtCo16,  sqrtError,  sqrtError,  sqrtError,  sqrtError,  sqrtRm16,   sqrtCm16,    sqrtSmaI,      sqrtRe34,  sqrtCo34
};



/********************************************//**
 * \brief Data type error in sqrt
 *
 * \param void
 * \return void
 ***********************************************/
void sqrtError(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate sqrt for %s", getRegisterDataTypeName(REGISTER_X, true, false));
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
    sprintf(errorMessage, "sqrt(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
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
  if(Sqrt[getRegisterDataType(REGISTER_X)] != sqrtError) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    Sqrt[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(opX);

    if(lastErrorCode != 0) {
      restoreStack();
    }

    refreshStack();
  }
  else {
    sqrtError();
  }
}



void sqrtBigI(void) {
  bigInteger_t value;

  convertBigIntegerRegisterToBigInteger(opX, &value);

  if(!bigIntegerIsNegative(&value)) { // Positive or zero value
    bigInteger_t value, nn0, nn1;

    if(bigIntegerIsZero(&value)) {
      uIntToBigInteger(0, &nn1);
    }
    else {
      //value = opX;
      convertBigIntegerRegisterToBigInteger(opX, &value);

      // n0 = value / 2 + 1;
      bigIntegerDivide2(&value, &nn0);
      bigIntegerAddUInt(&nn0, 1, &nn0);

      // n1 = value / n0 + n0 / 2;
      bigIntegerDivide(&value, &nn0, &nn1);
      bigIntegerDivide2(&nn0, &nn0);
      bigIntegerAdd(&nn1, &nn0, &nn1);
      bigIntegerMultiply2(&nn0, &nn0);
      while(bigIntegerCompare(&nn0, &nn1) == BIG_INTEGER_GREATER_THAN) {
        //n0 = n1;
        bigIntegerCopy(&nn1, &nn0);

        //n1 = (n0 + value / n0) / 2;
        bigIntegerDivide(&value, &nn0, &nn1);
        bigIntegerAdd(&nn1, &nn0, &nn1);
        bigIntegerDivide2(&nn1, &nn1);
      }

      // n0 = n1 * n1;
      bigIntegerMultiply(&nn1, &nn1, &nn0);
      if(bigIntegerCompare(&nn0, &value) == BIG_INTEGER_GREATER_THAN) {
        bigIntegerSubtractUInt(&nn1, 1, &nn1);
      }

      if(bigIntegerCompare(&nn0, &value) == BIG_INTEGER_EQUAL) {
        convertBigIntegerToBigIntegerRegister(&nn1, result);
        return;
      }
      else {
        convertBigIntegerRegisterToReal34Register(opX, opX);
        reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
        real34SquareRoot(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
        convertRegister34To16(result);
      }
    }
  }
  else if(getFlag(FLAG_CPXRES)) { // Negative value
    real34_t real34;

    convertBigIntegerRegisterToReal34Register(opX, opX);
    real34Copy(REGISTER_REAL34_DATA(opX), &real34);
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
    real34SetPositiveSign(&real34);
    real34SquareRoot(&real34, REGISTER_IMAG34_DATA(result));
    real34Zero(REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
    //convertRegister34To16(REGISTER_X);
  }
  else {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // 1 = argument exceeds functions domain
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative big integer when flag I is not set!");
      showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
    #endif
  }
}



void sqrtRe16(void) {
  convertRegister16To34(opX);

  if(!real34IsNegative(REGISTER_REAL34_DATA(opX))) {
    reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
    real34SquareRoot(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
  else if(getFlag(FLAG_CPXRES)) {
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
    real34Copy(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result));
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
  uint8_t savedAngularMode = angularMode;

  angularMode = AM_RADIAN;
  convertRegister16To34(opX);
  real34RectangularToPolar(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opX), &magnitude34, &theta34);
  real34SquareRoot(&magnitude34, &magnitude34);
  real34Multiply(&theta34, const34_0_5, &theta34);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34PolarToRectangular(&magnitude34, &theta34, REGISTER_REAL34_DATA(result), REGISTER_IMAG34_DATA(result)); // theta34 in internal units
  convertRegister34To16(result);
  angularMode = savedAngularMode;
}



void sqrtRm16(void) {
  sqrtToBeCoded();
}



void sqrtCm16(void) {
  sqrtToBeCoded();
}



void sqrtSmaI(void) {
  *(REGISTER_SMALL_INTEGER_DATA(REGISTER_X)) = WP34S_intSqrt(*(REGISTER_SMALL_INTEGER_DATA(REGISTER_X)));
}



void sqrtRe34(void) {
  if(!real34IsNegative(REGISTER_REAL34_DATA(opX))) {
    real34SquareRoot(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
  else if(getFlag(FLAG_CPXRES)) {
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
    real34Copy(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result));
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
  uint8_t savedAngularMode = angularMode;

  angularMode = AM_RADIAN;
  real34RectangularToPolar(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opX), &magnitude34, &theta34);
  real34SquareRoot(&magnitude34, &magnitude34);
  real34Multiply(&theta34, const34_0_5, &theta34);
  real34PolarToRectangular(&magnitude34, &theta34, REGISTER_REAL34_DATA(result), REGISTER_IMAG34_DATA(result)); // theta34 in internal units
  angularMode = savedAngularMode;
}
