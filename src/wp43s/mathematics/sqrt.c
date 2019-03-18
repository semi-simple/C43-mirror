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
// regX ==> 1             2          3          4           5           6           7           8           9            10             11         12
//          Long integer  real16     complex16  angle       Time        Date        String      real16 mat  complex16 m  Short integer  real34     complex34
            sqrtLonI,     sqrtRe16,  sqrtCo16,  sqrtError,  sqrtError,  sqrtError,  sqrtError,  sqrtRm16,   sqrtCm16,    sqrtShoI,      sqrtRe34,  sqrtCo34
};



/********************************************//**
 * \brief Data type error in sqrt
 *
 * \param void
 * \return void
 ***********************************************/
void sqrtError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
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
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  Sqrt[getRegisterDataType(REGISTER_X)]();
  freeTemporaryRegister(opX);

  if(lastErrorCode == 0) {
    refreshRegisterLine(REGISTER_X);
  }
  else {
    restoreStack();
    refreshStack();
  }
}



void sqrtLonI(void) {
  longInteger_t value;

  convertLongIntegerRegisterToLongInteger(opX, &value);

  if(!longIntegerIsNegative(&value)) { // Positive or zero value
    longInteger_t nn0, nn1, nn2;

    if(longIntegerIsZero(&value)) {
      uIntToLongInteger(0, &nn1);
    }
    else {
      // n0 = value / 2 + 1;
      longIntegerDivideUInt(&value, 2, &nn0);
      longIntegerAddUInt(&nn0, 1, &nn0);

      // n1 = value / n0 + n0 / 2;
      longIntegerDivide(&value, &nn0, &nn1);
      longIntegerDivideUInt(&nn0, 2, &nn2);
      longIntegerAdd(&nn1, &nn2, &nn1);
      while(longIntegerCompare(&nn0, &nn1) == LONG_INTEGER_GREATER_THAN) {
        //n0 = n1;
        longIntegerCopy(&nn1, &nn0);

        //n1 = (n0 + value / n0) / 2;
        longIntegerDivide(&value, &nn0, &nn1);
        longIntegerAdd(&nn1, &nn0, &nn1);
        longIntegerDivideUInt(&nn1, 2, &nn1);
      }

      // n0 = n1 * n1;
      longIntegerMultiply(&nn1, &nn1, &nn0);
      if(longIntegerCompare(&nn0, &value) == LONG_INTEGER_GREATER_THAN) {
        longIntegerSubtractUInt(&nn1, 1, &nn1);
      }

      if(longIntegerCompare(&nn0, &value) == LONG_INTEGER_EQUAL) {
        convertLongIntegerToLongIntegerRegister(&nn1, result);
        return;
      }
      else {
        convertLongIntegerRegisterToReal34Register(opX, opX);
        reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
        real34SquareRoot(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
        convertRegister34To16(result);
      }
    }
  }
  else if(getFlag(FLAG_CPXRES)) { // Negative value
    real34_t real34;

    convertLongIntegerRegisterToReal34Register(opX, opX);
    real34Copy(REGISTER_REAL34_DATA(opX), &real34);
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
    real34SetPositiveSign(&real34);
    real34SquareRoot(&real34, REGISTER_IMAG34_DATA(result));
    real34Zero(REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
  else {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // 1 = argument exceeds functions domain
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative long integer when flag I is not set!");
      showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
    #endif
  }
}



void sqrtRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sqrtRe16:", "cannot use NaN as an input of sqrt", NULL, NULL);
    #endif
    return;
  }

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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // 1 = argument exceeds functions domain
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative real when flag I is not set!");
      showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
    #endif
  }
}



void sqrtCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sqrtCo16:", "cannot use NaN as an input of sqrt", NULL, NULL);
    #endif
    return;
  }

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



void sqrtShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_intSqrt(*(REGISTER_SHORT_INTEGER_DATA(opX)));
}



void sqrtRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sqrtRe34:", "cannot use NaN as an input of sqrt", NULL, NULL);
    #endif
    return;
  }

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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // 1 = argument exceeds functions domain
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative real when flag I is not set!");
      showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
    #endif
  }
}



void sqrtCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sqrtCo34:", "cannot use NaN as an input of sqrt", NULL, NULL);
    #endif
    return;
  }

  real34_t magnitude34, theta34;
  uint8_t savedAngularMode = angularMode;

  angularMode = AM_RADIAN;
  real34RectangularToPolar(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opX), &magnitude34, &theta34);
  real34SquareRoot(&magnitude34, &magnitude34);
  real34Multiply(&theta34, const34_0_5, &theta34);
  real34PolarToRectangular(&magnitude34, &theta34, REGISTER_REAL34_DATA(result), REGISTER_IMAG34_DATA(result)); // theta34 in internal units
  angularMode = savedAngularMode;
}
