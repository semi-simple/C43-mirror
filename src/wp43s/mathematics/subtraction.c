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
 * \file subtraction.c
 ***********************************************/

#include "mathematics/subtraction.h"

#include "conversionAngles.h"
#include "debug.h"
#include "error.h"
#include "fonts.h"
#include "integers.h"
#include "items.h"
#include "c43Extensions/jm.h"
#include "mathematics/addition.h"
#include "matrix.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"



TO_QSPI void (* const subtraction[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX |    regY ==>   1            2            3            4            5            6         7            8            9             10
//      V               Long integer Real34       Complex34    Time         Date         String    Real34 mat   Complex34 m  Short integer Config data
/*  1 Long integer  */ {subLonILonI, subRealLonI, subCplxLonI, subTimeLonI, subDateLonI, subError, subError,    subError,    subShoILonI,  subError},
/*  2 Real34        */ {subLonIReal, subRealReal, subCplxReal, subTimeReal, subDateReal, subError, subError,    subError,    subShoIReal,  subError},
/*  3 Complex34     */ {subLonICplx, subRealCplx, subCplxCplx, subError,    subError,    subError, subError,    subError,    subShoICplx,  subError},
/*  4 Time          */ {subLonITime, subRealTime, subError,    subTimeTime, subError,    subError, subError,    subError,    subError,     subError},
/*  5 Date          */ {subError,    subError,    subError,    subError,    subDateDate, subError, subError,    subError,    subError,     subError},
/*  6 String        */ {subError,    subError,    subError,    subError,    subError,    subError, subError,    subError,    subError,     subError},
/*  7 Real34 mat    */ {subError,    subError,    subError,    subError,    subError,    subError, subRemaRema, subCxmaRema, subError,     subError},
/*  8 Complex34 mat */ {subError,    subError,    subError,    subError,    subError,    subError, subRemaCxma, subCxmaCxma, subError,     subError},
/*  9 Short integer */ {subLonIShoI, subRealShoI, subCplxShoI, subError,    subError,    subError, subError,    subError,    subShoIShoI,  subError},
/* 10 Config data   */ {subError,    subError,    subError,    subError,    subError,    subError, subError,    subError,    subError,     subError}
};



/********************************************//**
 * \brief Data type error in subtraction
 *
 * \param void
 * \return void
 ***********************************************/
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
void subError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    sprintf(errorMessage, "cannot subtract %s", getRegisterDataTypeName(REGISTER_X, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "from %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    moreInfoOnError("In function fnSubtract:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
}
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)



/********************************************//**
 * \brief rexX ==> regL and regY - regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnSubtract(uint16_t unusedButMandatoryParameter) {
  if(!saveLastX()) return;

  subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}



/******************************************************************************************************************************************************************************************/
/* long integer - ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) - X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonILonI(void) {
  longInteger_t y, x;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerSubtract(y, x, x);

  if(running_program_jm) temporaryInformation = longIntegerIsZero(x) ? TI_TRUE : TI_FALSE;    //JM Temporary hack to do DSZ
  
  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) - X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonITime(void) {
  convertLongIntegerRegisterToTimeRegister(REGISTER_Y, REGISTER_Y);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(time) - X(long integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeLonI(void) {
  convertLongIntegerRegisterToTimeRegister(REGISTER_X, REGISTER_X);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(date) - X(long integer) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateLonI(void) {
  real34_t val;
  convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  int32ToReal34(86400, &val);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), &val, &val);
  reallocateRegister(REGISTER_X, dtDate, REAL34_SIZE, amNone);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), &val, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) - X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIShoI(void) {
  longInteger_t y, x;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertShortIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerSubtract(y, x, x);

  if(running_program_jm)   temporaryInformation = longIntegerIsZero(x) ? TI_TRUE : TI_FALSE;    //JM Temporary hack to do DSZ

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(short integer) - X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoILonI(void) {
  longInteger_t y, x;

  convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerSubtract(y, x, x);

  if(running_program_jm)   temporaryInformation = longIntegerIsZero(x) ? TI_TRUE : TI_FALSE;    //JM Temporary hack to do DSZ

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) - X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIReal(void) {
  real_t y, x;
  angularMode_t xAngularMode;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == amNone) {
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleFromTo(&x, xAngularMode, currentAngularMode, &ctxtReal39);
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }

if(running_program_jm) temporaryInformation = real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) ? TI_TRUE : TI_FALSE;    //JM Temporary hack to do DSZ

}



/********************************************//**
 * \brief Y(real34) - X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRealLonI(void) {
  real_t y, x;
  angularMode_t yAngularMode;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);

  if(yAngularMode == amNone) {
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleFromTo(&y, yAngularMode, currentAngularMode, &ctxtReal39);
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }

if(running_program_jm) temporaryInformation = real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) ? TI_TRUE : TI_FALSE;    //JM Temporary hack to do DSZ

}



/********************************************//**
 * \brief Y(long integer) - X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonICplx(void) {
  real_t a, c;

  convertLongIntegerRegisterToReal(REGISTER_Y, &a, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &c);

  realSubtract(&a, &c, &c, &ctxtReal39);

  realToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));

  if(running_program_jm) temporaryInformation = (real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) ? TI_TRUE : TI_FALSE) & (real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)) ? TI_TRUE : TI_FALSE);    //JM Temporary hack to do DSZ

}



/********************************************//**
 * \brief Y(complex34) - X(long integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCplxLonI(void) {
  real_t a, c;
  real34_t b;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realSubtract(&a, &c, &c, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
  realToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  real34Copy(&b, REGISTER_IMAG34_DATA(REGISTER_X));

  if(running_program_jm) temporaryInformation = (real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) ? TI_TRUE : TI_FALSE) & (real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)) ? TI_TRUE : TI_FALSE);    //JM Temporary hack to do DSZ

}



/******************************************************************************************************************************************************************************************/
/* time - ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(time) - X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeTime(void) {
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(time) - X(real34) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeReal(void) {
  angularMode_t xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == amNone) {
    convertReal34RegisterToTimeRegister(REGISTER_X, REGISTER_X);
    real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    subError();
  }
}



/********************************************//**
 * \brief Y(real34) - X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subRealTime(void) {
  angularMode_t yAngularMode = getRegisterAngularMode(REGISTER_Y);

  if(yAngularMode == amNone) {
    convertReal34RegisterToTimeRegister(REGISTER_Y, REGISTER_Y);
    real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    subError();
  }
}



/******************************************************************************************************************************************************************************************/
/* date - ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(date) - X(date) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateDate(void) {
  real34_t val;

  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y));
  int32ToReal34(86400, &val);
  real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), &val, &val);
  convertReal34ToLongIntegerRegister(&val, REGISTER_X, DEC_ROUND_DOWN);
}



/********************************************//**
 * \brief Y(date) - X(real34) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateReal(void) {
  angularMode_t xAngularMode = getRegisterAngularMode(REGISTER_X);
  real34_t val;

  if(xAngularMode == amNone) {
    int32ToReal34(86400, &val);
    real34ToIntegralValue(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), roundingMode);
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), &val, &val);
    reallocateRegister(REGISTER_X, dtDate, REAL34_SIZE, amNone);
    real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), &val, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    addError();
  }
}



/******************************************************************************************************************************************************************************************/
/* string - ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/******************************************************************************************************************************************************************************************/
/* real16 matrix - ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16 matrix) - X(real16 matrix) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subRemaRema(void) {
#ifndef TESTSUITE_BUILD
  real34Matrix_t y, x;

  linkToRealMatrixRegister(REGISTER_Y, &y);
  convertReal34MatrixRegisterToReal34Matrix(REGISTER_X, &x);

  subtractRealMatrices(&y, &x, &x);
  if(x.matrixElements) {
    convertReal34MatrixToReal34MatrixRegister(&x, REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot subtract %d" STD_CROSS "%d-matrix from %d" STD_CROSS "%d-matrix",
              x.header.matrixRows, x.header.matrixColumns,
              y.header.matrixRows, y.header.matrixColumns);
      moreInfoOnError("In function subRemaRema:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }

  realMatrixFree(&x);
#endif // TESTSUITE_BUILD
}



/********************************************//**
 * \brief Y(real16 matrix) - X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subRemaCxma(void) {
#ifndef TESTSUITE_BUILD
  convertReal34MatrixRegisterToComplex34MatrixRegister(REGISTER_Y, REGISTER_Y);
  subCxmaCxma();
#endif // TESTSUITE_BUILD
}



/********************************************//**
 * \brief Y(complex16 matrix) - X(real16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subCxmaRema(void) {
#ifndef TESTSUITE_BUILD
  convertReal34MatrixRegisterToComplex34MatrixRegister(REGISTER_X, REGISTER_X);
  subCxmaCxma();
#endif // TESTSUITE_BUILD
}



/******************************************************************************************************************************************************************************************/
/* complex16 matrix - ...                                                                                                                                                                 */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16 matrix) - X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subCxmaCxma(void) {
#ifndef TESTSUITE_BUILD
  complex34Matrix_t y, x;

  linkToComplexMatrixRegister(REGISTER_Y, &y);
  convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_X, &x);

  subtractComplexMatrices(&y, &x, &x);
  if(x.matrixElements) {
    convertComplex34MatrixToComplex34MatrixRegister(&x, REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot add %d" STD_CROSS "%d-matrix to %d" STD_CROSS "%d-matrix",
              x.header.matrixRows, x.header.matrixColumns,
              y.header.matrixRows, y.header.matrixColumns);
      moreInfoOnError("In function addRemaRema:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }

  complexMatrixFree(&x);
#endif // TESTSUITE_BUILD
}



/******************************************************************************************************************************************************************************************/
/* short integer + ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(Short integer) - X(Short integer) ==> X(Short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoIShoI(void) {
  setRegisterTag(REGISTER_X, getRegisterTag(REGISTER_Y));
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intSubtract(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



/********************************************//**
 * \brief Y(short integer) - X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoIReal(void) {
  real_t y, x;
  angularMode_t xAngularMode = getRegisterAngularMode(REGISTER_X);

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  if(xAngularMode == amNone) {
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleFromTo(&x, xAngularMode, currentAngularMode, &ctxtReal39);
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }

if(running_program_jm) temporaryInformation = real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) ? TI_TRUE : TI_FALSE;    //JM Temporary hack to do DSZ

}



/********************************************//**
 * \brief Y(real34) - X(short integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRealShoI(void) {
  real_t y, x;
  angularMode_t yAngularMode = getRegisterAngularMode(REGISTER_Y);

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);

  if(yAngularMode == amNone) {
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleFromTo(&y, yAngularMode, currentAngularMode, &ctxtReal39);
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }

if(running_program_jm) temporaryInformation = real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) ? TI_TRUE : TI_FALSE;    //JM Temporary hack to do DSZ

}



/********************************************//**
 * \brief Y(short integer) - X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoICplx(void) {
  convertShortIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));
  if(running_program_jm) temporaryInformation = (real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) ? TI_TRUE : TI_FALSE) & (real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)) ? TI_TRUE : TI_FALSE);    //JM Temporary hack to do DSZ
}




/********************************************//**
 * \brief Y(complex34) - X(short integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCplxShoI(void) {
  convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
  if(running_program_jm) temporaryInformation = (real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) ? TI_TRUE : TI_FALSE) & (real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)) ? TI_TRUE : TI_FALSE);    //JM Temporary hack to do DSZ
}



/******************************************************************************************************************************************************************************************/
/* real34 - ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) - X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRealReal(void) {
  angularMode_t yAngularMode = getRegisterAngularMode(REGISTER_Y);
  angularMode_t xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(yAngularMode == amNone && xAngularMode == amNone) {
    real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    real_t y, x;

    if(yAngularMode == amNone) {
      yAngularMode = currentAngularMode;
    }
    else if(xAngularMode == amNone) {
      xAngularMode = currentAngularMode;
    }

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

    convertAngleFromTo(&y, yAngularMode, currentAngularMode, &ctxtReal39);
    convertAngleFromTo(&x, xAngularMode, currentAngularMode, &ctxtReal39);

    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }

if(running_program_jm) temporaryInformation = real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) ? TI_TRUE : TI_FALSE;    //JM Temporary hack to do DSZ

}



/********************************************//**
 * \brief Y(real34) - X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRealCplx(void) {
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) - X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCplxReal(void) {
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* complex34 - ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex34) - X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCplxCplx(void) {
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Subtract(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}
