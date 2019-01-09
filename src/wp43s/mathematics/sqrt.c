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



/********************************************//**
 * \brief regX ==> regL and sqrt(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnSquareRoot(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnSquareRoot");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_intSqrt(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);

    if(!real34IsNegative(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
		    real34SquareRoot(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
      convertRegister34To16(REGISTER_X);
    }
    else if(getFlag(FLAG_CPXRES)) {
      real34_t real34;

      real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_X), &real34);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, 0);
      real34SetPositiveSign(&real34);
		    real34SquareRoot(&real34, COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
      real34Zero(POINTER_TO_REGISTER_DATA(REGISTER_X));
      convertRegister34To16(REGISTER_X);
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

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    convertRegister16To34(REGISTER_X);

    if(!real34IsNegative(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
		    real34SquareRoot(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
      convertRegister34To16(REGISTER_X);
    }
    else if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, 0);
      real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_X));
      real34SetPositiveSign(POINTER_TO_REGISTER_DATA(REGISTER_X));

		    real34SquareRoot(POINTER_TO_REGISTER_DATA(REGISTER_X), COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));

      real34Zero(POINTER_TO_REGISTER_DATA(REGISTER_X));
      convertRegister34To16(REGISTER_X);
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // 1 = argument exceeds functions domain
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative real when flag I is not set!");
        showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
      #endif
    }
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    if(!real34IsNegative(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
      copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
		    real34SquareRoot(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_CPXRES)) {
      copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, 0);
      real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_X));
      real34SetPositiveSign(POINTER_TO_REGISTER_DATA(REGISTER_X));

		    real34SquareRoot(POINTER_TO_REGISTER_DATA(REGISTER_X), COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));

      real34Zero(POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // 1 = argument exceeds functions domain
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative real when flag I is not set!");
        showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
      #endif
    }
  }

  else if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    real16_t magnitude16, theta16;
    real34_t magnitude34;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    real16RectangularToPolar(REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), &magnitude16, &theta16);
    real16ToReal34(&magnitude16, &magnitude34);
    real34SquareRoot(&magnitude34, &magnitude34);
    real34ToReal16(&magnitude34, &magnitude16);
    real16Multiply(&theta16, const16_0_5, &theta16);
    convertAngle16FromTo(&theta16, angularMode, AM_RADIAN);
    real16PolarToRectangular(&magnitude16, &theta16, REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    real34_t magnitude34, theta34;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    real34RectangularToPolar(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), &magnitude34, &theta34);
    real34SquareRoot(&magnitude34, &magnitude34);
    real34Multiply(&theta34, const34_0_5, &theta34);
    convertAngle34FromTo(&theta34, angularMode, AM_RADIAN);
    real34PolarToRectangular(&magnitude34, &theta34, REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot extract the square root of %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
    #endif
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnSquareRoot");
  #endif
}
