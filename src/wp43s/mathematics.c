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
 * \file mathematics.c
 ***********************************************/

#include "wp43s.h"

/********************************************//**
 * \brief regX ==> regL and GDC(regY, regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnGcd(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnGcd");
  #endif

  bigInteger_t iOp1, iOp2;

  saveStack();

  if(getRegisterDataType(REGISTER_X) == dtSmallInteger && getRegisterDataType(REGISTER_Y) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_intGCD(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_Y)), *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerGcd(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtSmallInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertSmallIntegerRegisterBigIntegerRegister(REGISTER_X, REGISTER_X);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerGcd(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertSmallIntegerRegisterBigIntegerRegister(REGISTER_Y, REGISTER_Y);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerGcd(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot get the GDC from %s and %s!", getDataTypeName(getRegisterDataType(REGISTER_Y), true, false), getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnGdc:", errorMessage, NULL, NULL);
    #endif
  }

  if(lastErrorCode == 0) {
    fnDropY(NOPARAM);
  }
  else {
    restoreStack();
  }

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnGcd");
  #endif
}



/********************************************//**
 * \brief regX ==> regL and LCM(regY, regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnLcm(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnLcm");
  #endif

  bigInteger_t iOp1, iOp2;

  if(getRegisterDataType(REGISTER_X) == dtSmallInteger && getRegisterDataType(REGISTER_Y) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_intLCM(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_Y)), *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerLcm(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtSmallInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertSmallIntegerRegisterBigIntegerRegister(REGISTER_X, REGISTER_X);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerLcm(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertSmallIntegerRegisterBigIntegerRegister(REGISTER_Y, REGISTER_Y);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerLcm(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot get the LCM from %s and %s!", getDataTypeName(getRegisterDataType(REGISTER_Y), true, false), getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnLcm:", errorMessage, NULL, NULL);
    #endif
  }

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnLcm");
  #endif
}



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



/********************************************//**
 * \brief Returns the absolute value of an integer or a real and the magnitude of a complex
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnMagnitude(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnMagnitude");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, 0);
    real16Multiply(POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_X));
    real16FMA(POINTER_TO_REGISTER_DATA(REGISTER_L) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_L) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
    real16SquareRoot(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
  }

  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, 0);
    real34Multiply(POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_X));
    real34FMA(POINTER_TO_REGISTER_DATA(REGISTER_L) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_L) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
    real34SquareRoot(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real16SetPositiveSign(POINTER_TO_REGISTER_DATA(REGISTER_X));
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real34SetPositiveSign(POINTER_TO_REGISTER_DATA(REGISTER_X));
  }

  else if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_intAbs(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    setRegisterSign(REGISTER_X, 0);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "|x| doesn't work on %s!", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnMagnitude:", errorMessage, NULL, NULL);
    #endif
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnMagnitude");
  #endif
}



/********************************************//**
 * \brief regX ==> regL and 2^regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fn2Pow(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fn2Pow");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_int2pow(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    int16_t exponent;
    bigInteger_t power;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &power);
    if(bigIntegerCompareUInt(&power, MAX_BIG_INTEGER_SIZE_IN_BITS) != BIG_INTEGER_GREATER_THAN) {
      exponent = power.dp[0];

      bigInteger2Exp(exponent, &power);
      convertBigIntegerToBigIntegerRegister(&power, REGISTER_X);
    }
    else {
      displayCalcErrorMessage(4, REGISTER_T, REGISTER_X);
      #if(EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "2^ this value would result in a value exceeding %" FMT16S " bits!", MAX_BIG_INTEGER_SIZE_IN_BITS);
        bigIntegerToString(&power, tmpStr3000, 10);
        showInfoDialog("In function fn2Pow:", errorMessage, tmpStr3000, NULL);
      #endif
    }
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    op1 = allocateTemporaryRegister();
    reallocateRegister(op1, dtReal16, REAL16_SIZE, 0);
    real16Copy(const16_2, POINTER_TO_REGISTER_DATA(op1));

    op2 = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    result = REGISTER_X;
    powRe16Re16();
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    op1 = allocateTemporaryRegister();
    reallocateRegister(op1, dtReal34, REAL34_SIZE, 0);
    real34Copy(const34_2, POINTER_TO_REGISTER_DATA(op1));

    op2 = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    result = REGISTER_X;
    powRe34Re34();
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate the power of 2 of %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fn2Pow:", errorMessage, NULL, NULL);
    #endif
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fn2Pow");
  #endif
}



/********************************************//**
 * \brief regX ==> regL and 10^regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fn10Pow(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fn10Pow");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_int10pow(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    int16_t exponent;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    bigInteger_t temp;
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &temp);
    exponent = (bigIntegerCompareUInt(&temp, 1234) == BIG_INTEGER_GREATER_THAN ? 1234 : temp.dp[0]);

    uIntToBigInteger(10, &temp);
    op1 = allocateTemporaryRegister();
    convertBigIntegerToBigIntegerRegister(&temp, op1);

    uIntToBigInteger(exponent, &temp);
    op2 = allocateTemporaryRegister();
    convertBigIntegerToBigIntegerRegister(&temp, op2);

    result = allocateTemporaryRegister();
    powBigIBigI();

    copySourceRegisterToDestRegister(result, REGISTER_X);
    freeTemporaryRegister(result);
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    op1 = allocateTemporaryRegister();
    reallocateRegister(op1, dtReal16, REAL16_SIZE, 0);
    real16Copy(const16_10, POINTER_TO_REGISTER_DATA(op1));

    op2 = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    result = REGISTER_X;
    powRe16Re16();
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    op1 = allocateTemporaryRegister();
    reallocateRegister(op1, dtReal34, REAL34_SIZE, 0);
    real34Copy(const34_10, POINTER_TO_REGISTER_DATA(op1));

    op2 = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    result = REGISTER_X;
    powRe34Re34();
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate the power of 2 of %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fn2Pow:", errorMessage, NULL, NULL);
    #endif
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fn10Pow");
  #endif
}



void fnSin(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnSin");
  #endif

  bool_t real16 = getRegisterDataType(REGISTER_X) == dtReal16;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real16 = true;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34) {
    if(real16) {
      convertRegister16To34(REGISTER_X);
    }

    if(real34IsSpecial(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
      real34Copy(const34_NaN, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else {
    	 WP34S_cvt_2rad_sincos(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), NULL, REGISTER_X);
    	 setRegisterDataType(REGISTER_X, dtReal34);
    	 setRegisterDataInfo(REGISTER_X, 0);
    }

    if(real16) {
      convertRegister34To16(REGISTER_X);
    }

    roundRegister(REGISTER_X);
    refreshRegisterLine(REGISTER_X);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "X register is %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnSin:", errorMessage, "is not supported for cos(X)!", NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnSin");
  #endif
}



void fnCos(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCos");
  #endif

  bool_t real16 = getRegisterDataType(REGISTER_X) == dtReal16;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real16 = true;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34) {
    if(real16) {
      convertRegister16To34(REGISTER_X);
    }

    if(real34IsSpecial(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
      real34Copy(const34_NaN, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else {
    	 WP34S_cvt_2rad_sincos(NULL, REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REGISTER_X);
    	 setRegisterDataType(REGISTER_X, dtReal34);
    	 setRegisterDataInfo(REGISTER_X, 0);
    }

    if(real16) {
      convertRegister34To16(REGISTER_X);
    }

    roundRegister(REGISTER_X);
    refreshRegisterLine(REGISTER_X);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "X register is %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnCos:", errorMessage, "is not supported for cos(X)!", NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCos");
  #endif
}


void fnTan(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnTan");
  #endif

  bool_t real16 = getRegisterDataType(REGISTER_X) == dtReal16;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real16 = true;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34) {
    if(real16) {
      convertRegister16To34(REGISTER_X);
    }

    if(real34IsSpecial(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
      real34Copy(const34_NaN, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else {
      real34_t cos;
    	 WP34S_cvt_2rad_sincos(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), &cos, REGISTER_X);
    	 setRegisterDataType(REGISTER_X, dtReal34);
    	 setRegisterDataInfo(REGISTER_X, 0);
    	 real34Divide(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), &cos, REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
    }

    if(real16) {
      convertRegister34To16(REGISTER_X);
    }

    roundRegister(REGISTER_X);
    refreshRegisterLine(REGISTER_X);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "X register is %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnTan:", errorMessage, "is not supported for cos(X)!", NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnTan");
  #endif
}



void fnArctan(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnArctan");
  #endif

  dataType_t dataType = dtReal34;

  saveStack();

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    dataType = dtReal16;
  }

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    dataType = dtReal16;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    WP34S_do_atan(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
    convertRegisterAngleFromTo(REGISTER_X, AM_RADIAN, angularMode);

    if(dataType == dtReal16) {
      convertRegister34To16(REGISTER_X);
      if(angularMode == AM_DMS) {
        checkDms(REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
      }
    }

    temporaryInformation = TI_ANGLE;
    displayAngularMode = angularMode;
    refreshRegisterLine(REGISTER_X);
  }

  else {
    restoreStack();
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "X register is %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnArctan:", errorMessage, "is not supported for arctan(X)!", NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnArctan");
  #endif
}



void fnArccos(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnArccos");
  #endif

  dataType_t dataType = dtReal34;

  saveStack();

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    dataType = dtReal16;
  }

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    dataType = dtReal16;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    if(real34CompareAbsGreaterThan(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), const34_1)) {
      restoreStack();
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnArccos:", "|X| > 1", NULL, NULL);
      #endif
    }
    else {
      WP34S_do_acos(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
      convertRegisterAngleFromTo(REGISTER_X, AM_RADIAN, angularMode);

      if(dataType == dtReal16) {
        convertRegister34To16(REGISTER_X);
        if(angularMode == AM_DMS) {
          checkDms(REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
        }
      }

      temporaryInformation = TI_ANGLE;
      displayAngularMode = angularMode;
      refreshRegisterLine(REGISTER_X);
    }
  }

  else {
    restoreStack();
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "X register is %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnArccos:", errorMessage, "is not supported for arccos(X)!", NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnArccos");
  #endif
}



void fnArcsin(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnArcsin");
  #endif

  dataType_t dataType = dtReal34;

  saveStack();

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    dataType = dtReal16;
  }

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    dataType = dtReal16;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    if(real34CompareAbsGreaterThan(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), const34_1)) {
      restoreStack();
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnArcsin:", "|X| > 1", NULL, NULL);
      #endif
    }
    else {
      WP34S_do_asin(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
      convertRegisterAngleFromTo(REGISTER_X, AM_RADIAN, angularMode);

      if(dataType == dtReal16) {
        convertRegister34To16(REGISTER_X);
        if(angularMode == AM_DMS) {
          checkDms(REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
        }
      }

      temporaryInformation = TI_ANGLE;
      displayAngularMode = angularMode;
      refreshRegisterLine(REGISTER_X);
    }
  }

  else {
    restoreStack();
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "X register is %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnArcsin:", errorMessage, "is not supported for arcsin(X)!", NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnArcsin");
  #endif
}



void checkDms(real16_t *angleDms) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("checkDms");
  #endif

  int16_t  sign;
  real16_t degrees, minutes, seconds;

  sign = real16IsNegative(angleDms) ? -1 : 1;
  real16SetPositiveSign(angleDms);

  real16ToIntegral(angleDms, &degrees);
  real16Subtract(angleDms, &degrees, angleDms);

  real16Multiply(angleDms, const16_100, angleDms);
  real16ToIntegral(angleDms, &minutes);
  real16Subtract(angleDms, &minutes, angleDms);

  real16Multiply(angleDms, const16_100, &seconds);

  if(real16CompareGreaterEqual(&seconds, const16_60)) {
    real16Subtract(&seconds, const16_60, &seconds);
    real16Add(&minutes, const16_1, &minutes);
  }

  if(real16CompareGreaterEqual(&minutes, const16_60)) {
    real16Subtract(&minutes, const16_60, &minutes);
    real16Add(&degrees, const16_1, &degrees);
  }

  real16Divide(&minutes, const16_100, &minutes);
  real16Add(&degrees, &minutes, angleDms);
  real16Divide(&seconds, const16_10000, &seconds);
  real16Add(angleDms, &seconds, angleDms);

  if(sign == -1) {
    real34SetNegativeSign(angleDms);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("checkDms");
  #endif
}



void fnToPolar(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnToPolar");
  #endif

  if(   (getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34 || getRegisterDataType(REGISTER_X) == dtBigInteger)
     && (getRegisterDataType(REGISTER_Y) == dtReal16 || getRegisterDataType(REGISTER_Y) == dtReal34 || getRegisterDataType(REGISTER_Y) == dtBigInteger)) {
    bool_t real16 = false;

    if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      convertBigIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      real16 = true;
    }

    else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      if(getRegisterDataType(REGISTER_Y) == dtReal16) {
        convertRegister16To34(REGISTER_Y);
        real16 = true;
      }
    }

    else if(getRegisterDataType(REGISTER_Y) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      if(getRegisterDataType(REGISTER_X) == dtReal16) {
        convertRegister16To34(REGISTER_X);
        real16 = true;
      }
    }

    else if(getRegisterDataType(REGISTER_X) == dtReal16 && getRegisterDataType(REGISTER_Y) == dtReal34) {
      convertRegister16To34(REGISTER_X);
    }

    else if(getRegisterDataType(REGISTER_Y) == dtReal16 && getRegisterDataType(REGISTER_X) == dtReal34) {
      convertRegister16To34(REGISTER_Y);
    }

    if(getRegisterDataType(REGISTER_X) == dtReal16) {
      real16 = true;
      convertRegister16To34(REGISTER_X);
      convertRegister16To34(REGISTER_Y);
    }

    real34_t real34, imag34;

    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_X), &real34);
    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_Y), &imag34);
    real34RectangularToPolar(&real34, &imag34, REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_Y)));

    if(real16) {
      convertRegister34To16(REGISTER_X);
      convertRegister34To16(REGISTER_Y);
    }

    temporaryInformation = TI_RADIUS_THETA;

    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
  }
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot convert (%s, %s) to polar coordinates!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      showInfoDialog("In function fnToPolar:", errorMessage, NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnToPolar");
  #endif
}



void fnToRect(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnToRect");
  #endif

  if(   (getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34 || getRegisterDataType(REGISTER_X) == dtBigInteger)
     && (getRegisterDataType(REGISTER_Y) == dtReal16 || getRegisterDataType(REGISTER_Y) == dtReal34 || getRegisterDataType(REGISTER_Y) == dtBigInteger)) {
    bool_t real16 = false;

    if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      convertBigIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      real16 = true;
    }

    else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      if(getRegisterDataType(REGISTER_Y) == dtReal16) {
        convertRegister16To34(REGISTER_Y);
        real16 = true;
      }
    }

    else if(getRegisterDataType(REGISTER_Y) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      if(getRegisterDataType(REGISTER_X) == dtReal16) {
        convertRegister16To34(REGISTER_X);
        real16 = true;
      }
    }

    else if(getRegisterDataType(REGISTER_X) == dtReal16 && getRegisterDataType(REGISTER_Y) == dtReal34) {
      convertRegister16To34(REGISTER_X);
    }

    else if(getRegisterDataType(REGISTER_Y) == dtReal16 && getRegisterDataType(REGISTER_X) == dtReal34) {
      convertRegister16To34(REGISTER_Y);
    }

    if(getRegisterDataType(REGISTER_X) == dtReal16) {
      real16 = true;
      convertRegister16To34(REGISTER_X);
      convertRegister16To34(REGISTER_Y);
    }

    real34_t magnitude34, theta34;

    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_X), &magnitude34);
    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_Y), &theta34);
    convertAngle34FromTo(&theta34, angularMode, AM_RADIAN);
    real34PolarToRectangular(&magnitude34, &theta34, REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_Y)));

    if(real16) {
      convertRegister34To16(REGISTER_X);
      convertRegister34To16(REGISTER_Y);
    }

    temporaryInformation = TI_X_Y;

    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
  }
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot convert (%s, %s) to rectangular coordinates!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      showInfoDialog("In function fnToRect:", errorMessage, NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnToRect");
  #endif
}



void fnFactorial(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnFactorial");
  #endif

  int8_t dataType = dtReal34;
  bigInteger_t result;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &result);
    if(bigIntegerIsNegative(&result) || bigIntegerCompareUInt(&result, 294) == BIG_INTEGER_GREATER_THAN) {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      dataType = dtReal16;
    }
    else {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      dataType = dtBigInteger;
    }
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    dataType = dtReal16;
  }

  else if(getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate factorial of %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnFactorial:", errorMessage, NULL, NULL);
    #endif

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("fnFactorial");
    #endif

    return;
  }

  if(dataType == dtBigInteger) {
    uint16_t counter;

    counter = bigIntegerToUInt(&result);
    if(counter == 0) {
      uIntToBigInteger(1, &result);
    }
    else {
      for(counter--; counter>0; counter--) {
        bigIntegerMultiplyUInt(&result, counter, &result);
      }
    }

    convertBigIntegerToBigIntegerRegister(&result, REGISTER_X);
  }
  else {
    WP34S_real34Factorial(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  if(dataType == dtReal16) {
    convertRegister34To16(REGISTER_X);
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnFactorial");
  #endif
}



void fnLnGamma(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnLnGamma");
  #endif

  bool_t real16 = false;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real16 = true;
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    real16 = true;
  }

  else if(getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate ln" STD_GAMMA " of %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnLnGamma:", errorMessage, NULL, NULL);
    #endif

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("fnLnGamma");
    #endif

    return;
  }

  WP34S_real34LnGamma(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));

  if(real16) {
   convertRegister34To16(REGISTER_X);
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnLnGamma");
  #endif
}



void fnGamma(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnGamma");
  #endif

  bool_t real16 = false;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real16 = true;
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    real16 = true;
  }

  else if(getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate " STD_GAMMA " of %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnGamma:", errorMessage, NULL, NULL);
    #endif

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("fnGamma");
    #endif

    return;
  }

  WP34S_real34Gamma(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));

  if(real16) {
    convertRegister34To16(REGISTER_X);
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnGamma");
  #endif
}



void real16RectangularToPolar(const real16_t *real16, const real16_t *imag16, real16_t *magnitude16, real16_t *theta16) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16RectangularToPolar");
  #endif

  real34_t real34, imag34, magnitude34, theta34;

  real16ToReal34(real16, &real34);
  real16ToReal34(imag16, &imag34);

  real34RectangularToPolar(&real34, &imag34, &magnitude34, &theta34);

  real34ToReal16(&magnitude34, magnitude16);
  real34ToReal16(&theta34, theta16);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16RectangularToPolar");
  #endif
}



void real16PolarToRectangular(const real16_t *magnitude16, const real16_t *theta16, real16_t *real16, real16_t *imag16) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16PolarToRectangular");
  #endif

  real34_t real34, imag34, magnitude34, theta34;

  real16ToReal34(magnitude16, &magnitude34);
  real16ToReal34(theta16, &theta34);

  real34PolarToRectangular(&magnitude34, &theta34, &real34, &imag34);

  real34ToReal16(&real34, real16);
  real34ToReal16(&imag34, imag16);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16PolarToRectangular");
  #endif
}



void real34RectangularToPolar(const real34_t *real34, const real34_t *imag34, real34_t *magnitude34, real34_t *theta34) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34RectangularToPolar");
  #endif

  if(real34IsZero(real34) && real34IsZero(imag34)) {
    real34Zero(magnitude34);
    real34Zero(theta34);
  }
  else if(real34IsZero(real34) && real34IsNegative(imag34)) {
    real34AbsToReal34(imag34, magnitude34);
    real34Copy(const34_piOn2, theta34);
    real34SetNegativeSign(theta34);
  }
  else if(real34IsZero(real34) && real34IsPositive(imag34)) {
    real34Copy(imag34, magnitude34);
    real34Copy(const34_piOn2, theta34);
  }
  else { // real34 != 0
    // Magnitude
    real34Multiply(real34, real34, magnitude34);
    real34FMA(imag34, imag34, magnitude34, magnitude34);
    real34SquareRoot(magnitude34, magnitude34);

    // Angle
    real34Divide(imag34, real34, theta34);
    WP34S_do_atan(theta34, theta34);
  }

  if(real34IsNegative(real34)) {
    if(real34IsNegative(imag34)) {
      real34Subtract(theta34, const34_pi, theta34);
      if(real34CompareLessEqual(theta34, const34_minusPi)) {
        real34Add(theta34, const34_2pi, theta34);
      }
    }
    else {
      real34Add(theta34, const34_pi, theta34);
      if(real34CompareGreaterThan(theta34, const34_pi)) {
        real34Subtract(theta34, const34_2pi, theta34);
      }
    }
  }

  convertAngle34FromTo(theta34, AM_RADIAN, angularMode);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34RectangularToPolar");
  #endif
}



void real34PolarToRectangular(const real34_t *magnitude34, const real34_t *theta34, real34_t *real34, real34_t *imag34) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34PolarToRectangular");
  #endif

  real34_t sin, cos;

  WP34S_sincosTaylor(theta34, &sin, &cos);
  real34Multiply(magnitude34, &cos, real34);
  real34Multiply(magnitude34, &sin, imag34);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34PolarToRectangular");
  #endif
}



bool_t real16CompareGreaterThan(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareGreaterThan");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareGreaterThan");
  #endif

  return real16ToInt32(&compare) > 0;
}



bool_t real34CompareGreaterThan(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareGreaterThan");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareGreaterThan");
  #endif

  return real34ToInt32(&compare) > 0;
}



bool_t real51CompareGreaterThan(const real51_t *number1, const real51_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real51CompareGreaterThan");
  #endif

  real51_t compare;

  real51Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real51CompareGreaterThan");
  #endif

  return real51ToInt32(&compare) > 0;
}



bool_t real16CompareGreaterEqual(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareGreaterEqual");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareGreaterEqual");
  #endif

  return real16ToInt32(&compare) >= 0;
}



bool_t real34CompareGreaterEqual(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareGreaterEqual");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareGreaterEqual");
  #endif

  return real34ToInt32(&compare) >= 0;
}



bool_t real16CompareEqual(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareEqual");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareEqual");
  #endif

  return real16ToInt32(&compare) == 0;
}



bool_t real34CompareEqual(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareEqual");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareEqual");
  #endif

  return real34ToInt32(&compare) == 0;
}



bool_t real51CompareEqual(const real51_t *number1, const real51_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real51CompareEqual");
  #endif

  real51_t compare;

  real51Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real51CompareEqual");
  #endif

  return (real51ToInt32(&compare) == 0);
}



bool_t real16CompareLessEqual(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareLessEqual");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareLessEqual");
  #endif

  return real16ToInt32(&compare) <= 0;
}



bool_t real34CompareLessEqual(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareLessEqual");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareLessEqual");
  #endif

  return real34ToInt32(&compare) <= 0;
}



bool_t real51CompareLessEqual(const real51_t *number1, const real51_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real51CompareLessEqual");
  #endif

  real51_t compare;

  real51Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real51CompareLessEqual");
  #endif

  return real51ToInt32(&compare) <= 0;
}



bool_t real16CompareLessThan(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareLessThan");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareLessThan");
  #endif

  return real16ToInt32(&compare) < 0;
}



bool_t real34CompareLessThan(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareLessThan");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareLessThan");
  #endif

  return real34ToInt32(&compare) < 0;
}



bool_t real51CompareLessThan(const real51_t *number1, const real51_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real51CompareLessThan");
  #endif

  real51_t compare;

  real51Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real51CompareLessThan");
  #endif

  return real51ToInt32(&compare) < 0;
}



bool_t real16CompareAbsGreaterThan(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareAbsGreaterThan");
  #endif

  real16_t compare, num1, num2;

  real16Copy(number1, &num1);
  real16Copy(number2, &num2);
  real16SetPositiveSign(&num1);
  real16SetPositiveSign(&num2);
  real16Compare(&num1, &num2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareAbsGreaterThan");
  #endif

  return real16ToInt32(&compare) > 0;
}



bool_t real34CompareAbsGreaterThan(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareAbsGreaterThan");
  #endif

  real34_t compare, num1, num2;

  real34Copy(number1, &num1);
  real34Copy(number2, &num2);
  real34SetPositiveSign(&num1);
  real34SetPositiveSign(&num2);
  real34Compare(&num1, &num2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareAbsGreaterThan");
  #endif

  return real34ToInt32(&compare) > 0;
}



bool_t real16CompareAbsLessThan(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareAbsLessThan");
  #endif

  real16_t compare, num1, num2;

  real16Copy(number1, &num1);
  real16Copy(number2, &num2);
  real16SetPositiveSign(&num1);
  real16SetPositiveSign(&num2);
  real16Compare(&num1, &num2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareAbsLessThan");
  #endif

  return real16ToInt32(&compare) < 0;
}



bool_t real34CompareAbsLessThan(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareAbsLessThan");
  #endif

  real34_t compare, num1, num2;

  real34Copy(number1, &num1);
  real34Copy(number2, &num2);
  real34SetPositiveSign(&num1);
  real34SetPositiveSign(&num2);
  real34Compare(&num1, &num2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareAbsLessThan");
  #endif

  return real34ToInt32(&compare) < 0;
}
