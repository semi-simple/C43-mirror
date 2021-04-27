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
 * \file dot.c
 ***********************************************/

#include "mathematics/dot.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "fonts.h"
#include "items.h"
#include "matrix.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"

static void dotDataTypeError(void);


TO_QSPI void (* const dot[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX |    regY ==>    1                  2                  3                  4                  5                  6                  7                  8                  9                 10
//      V                Long integer       Real34             Complex34          Time               Date               String             Real34 mat         Complex34 mat      Short integer     Config data
/*  1 Long integer  */ { dotDataTypeError,  dotDataTypeError,  dotCplxLonI,       dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError, dotDataTypeError},
/*  2 Real34        */ { dotDataTypeError,  dotDataTypeError,  dotCplxReal,       dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError, dotDataTypeError},
/*  3 Complex34     */ { dotLonICplx,       dotRealCplx,       dotCplxCplx,       dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotShoICplx,      dotDataTypeError},
/*  4 Time          */ { dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError, dotDataTypeError},
/*  5 Date          */ { dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError, dotDataTypeError},
/*  6 String        */ { dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError, dotDataTypeError},
/*  7 Real34 mat    */ { dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotRemaRema,       dotCpmaRema,       dotDataTypeError, dotDataTypeError},
/*  8 Complex34 mat */ { dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotRemaCpma,       dotCpmaCpma,       dotDataTypeError, dotDataTypeError},
/*  9 Short integer */ { dotDataTypeError,  dotDataTypeError,  dotCplxShoI,       dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError, dotDataTypeError},
/* 10 Config data   */ { dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError,  dotDataTypeError, dotDataTypeError}
};


//=============================================================================
// Error handling
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Data type error in DOT
 *
 * \param void
 * \return void
 ***********************************************/
static void dotDataTypeError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);

  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot raise %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnDot:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
}

//static void dotSizeError() {
//  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);

//  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
//    sprintf(errorMessage, "cannot calculate DOT product, matrix size mismatch.");
//    moreInfoOnError("In function fnDot:", errorMessage, NULL, NULL);
//  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
//}

//=============================================================================
// Main function
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief regX ==> regL and Dot(regX, RegY) ==> regX
 * enables stack lift and refreshes the stack.
 * Calculate the dot (or scalar) product between complex and matrix
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnDot(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  dot[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
}

//=============================================================================
// Complex dot product calculation functionS
//-----------------------------------------------------------------------------

static void dotCplx(real_t *xReal, real_t *xImag, real_t *yReal, real_t *yImag, real_t *rReal, realContext_t *realContext) {
  real_t t;

  realMultiply(xReal, yReal, &t, realContext);     // t = xReal * yReal
  realMultiply(xImag, yImag, rReal, realContext);      // r = xImag * yImag
  realAdd(&t, rReal, rReal, realContext);     // r = r + t
}

/********************************************//**
 * \brief Dot(Y(real34), X(complex34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void dotRealCplx(void) {
  real_t xReal, xImag, yReal, yImag;
  real_t rReal;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(const34_0, &yImag);

  dotCplx(&xReal, &xImag, &yReal, &yImag, &rReal, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, amNone);
}

/********************************************//**
 * \brief Dot(Y(long integer), X(complex34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void dotLonICplx(void) {
  real_t xReal, xImag, yReal, yImag;
  real_t rReal;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  convertLongIntegerRegisterToReal(REGISTER_Y, &yReal, &ctxtReal39);
  real34ToReal(const34_0, &yImag);

  dotCplx(&xReal, &xImag, &yReal, &yImag, &rReal, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, amNone);
}

/********************************************//**
 * \brief Dot(Y(short integer), X(complex34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void dotShoICplx(void) {
  real_t xReal, xImag, yReal, yImag;
  real_t rReal;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  convertShortIntegerRegisterToReal(REGISTER_Y, &yReal, &ctxtReal39);
  real34ToReal(const34_0, &yImag);

  dotCplx(&xReal, &xImag, &yReal, &yImag, &rReal, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, amNone);
}

/********************************************//**
 * \brief Dot(Y(complex34), X(complex34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void dotCplxCplx(void) {
  real_t xReal, xImag, yReal, yImag;
  real_t rReal;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &yImag);

  dotCplx(&xReal, &xImag, &yReal, &yImag, &rReal, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, amNone);
}

/********************************************//**
 * \brief Dot(Y(complex34), X(real34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void dotCplxReal(void) {
  real_t xReal, xImag, yReal, yImag;
  real_t rReal;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(const34_0, &xImag);

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &yImag);

  dotCplx(&xReal, &xImag, &yReal, &yImag, &rReal, &ctxtReal39);

  realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, amNone);
}

/********************************************//**
 * \brief Dot(Y(complex34), X(long integer)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void dotCplxLonI(void) {
  real_t xReal, xImag, yReal, yImag;
  real_t rReal;

  convertLongIntegerRegisterToReal(REGISTER_X, &xReal, &ctxtReal39);
  real34ToReal(const34_0, &xImag);

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &yImag);

  dotCplx(&xReal, &xImag, &yReal, &yImag, &rReal, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, amNone);
}

/********************************************//**
 * \brief Dot(Y(complex34), X(short integer)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void dotCplxShoI(void) {
  real_t xReal, xImag, yReal, yImag;
  real_t rReal;

  convertShortIntegerRegisterToReal(REGISTER_X, &xReal, &ctxtReal39);
  real34ToReal(const34_0, &xImag);

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &yImag);

  dotCplx(&xReal, &xImag, &yReal, &yImag, &rReal, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, amNone);
}

//=============================================================================
// Matrix dot calculation function
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Dot(Y(real34 matrix), X(real34 matrix)) ==> X(real34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void dotRemaRema(void) {
#ifndef TESTSUITE_BUILD
  real34Matrix_t y, x;
  real34_t res;

  linkToRealMatrixRegister(REGISTER_Y, &y);
  linkToRealMatrixRegister(REGISTER_X, &x);

  if((realVectorSize(&y) == 0) || (realVectorSize(&x) == 0) || (realVectorSize(&y) != realVectorSize(&x))) {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "numbers of elements of %d" STD_CROSS "%d-matrix to %d" STD_CROSS "%d-matrix mismatch",
              x.header.matrixRows, x.header.matrixColumns,
              y.header.matrixRows, y.header.matrixColumns);
      moreInfoOnError("In function dotRemaRema:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    dotRealVectors(&y, &x, &res);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&res, REGISTER_REAL34_DATA(REGISTER_X));
  }
#endif // TESTSUITE_BUILD
}

/********************************************//**
 * \brief Dot(Y(complex34 matrix), X(real34 matrix)) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void dotCpmaRema(void) {
  itemToBeCoded(0);
}

/********************************************//**
 * \brief Dot(Y(real34 matrix), X(complex34 matrix)) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void dotRemaCpma(void) {
  itemToBeCoded(0);
}

/********************************************//**
 * \brief Dot(Y(complex34 matrix), X(complex34 matrix)) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void dotCpmaCpma(void) {
  itemToBeCoded(0);
}
