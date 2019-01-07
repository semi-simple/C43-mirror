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
