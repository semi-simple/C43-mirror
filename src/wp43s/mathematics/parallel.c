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
 * \file parallel.c
 ***********************************************/

#include "wp43s.h"



void (* const parallel[12][12])(void) = {
// regX |    regY ==>   1                 2                 3                 4                 5              6              7              8              9              10              11                12
//      V               Long integer      Real16            Complex16         Angle16           Time           Date           String         Real16 mat     Complex16 m    Short integer   Real34            Complex34
/*  1 Long integer  */ {parallelLonILonI, parallelRe16LonI, parallelCo16LonI, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError,  parallelRe34LonI, parallelCo34LonI},
/*  2 Real16        */ {parallelLonIRe16, parallelRe16Re16, parallelCo16Re16, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError,  parallelRe34Re16, parallelCo34Re16},
/*  3 Complex16     */ {parallelLonICo16, parallelRe16Co16, parallelCo16Co16, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError,  parallelRe34Co16, parallelCo34Co16},
/*  4 Angle16       */ {parallelError,    parallelError,    parallelError,    parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError,  parallelError,    parallelError   },
/*  5 Time          */ {parallelError,    parallelError,    parallelError,    parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError,  parallelError,    parallelError   },
/*  6 Date          */ {parallelError,    parallelError,    parallelError,    parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError,  parallelError,    parallelError   },
/*  7 String        */ {parallelError,    parallelError,    parallelError,    parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError,  parallelError,    parallelError   },
/*  8 Real16 mat    */ {parallelError,    parallelError,    parallelError,    parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError,  parallelError,    parallelError   },
/*  9 Complex16 mat */ {parallelError,    parallelError,    parallelError,    parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError,  parallelError,    parallelError   },
/* 10 Short integer */ {parallelError,    parallelError,    parallelError,    parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError,  parallelError,    parallelError   },
/* 11 Real34        */ {parallelLonIRe34, parallelRe16Re34, parallelCo16Re34, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError,  parallelRe34Re34, parallelCo34Re34},
/* 12 Complex34     */ {parallelLonICo34, parallelRe16Co34, parallelCo16Co34, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError,  parallelRe34Co34, parallelCo34Co34}
};



/********************************************//**
 * \brief Data type error in parallel function
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void parallelError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot multiply %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnMultiply:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regY || regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnParallel(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  parallel[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}



/******************************************************************************************************************************************************************************************/
/* long integer || ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) || X(long integer) ==> X(Real16)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelLonILonI(void) {
  real39_t y, x, product;

  // y || x = xy / (x+y)
  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

  if(!realIsZero(&x)) {
    realMultiply(&y, &x, &product, &ctxtReal39);
    realAdd(&y, &x, &y, &ctxtReal39);
    realDivide(&product, &y, &x, &ctxtReal39);
  }

  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) || X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelLonIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelLonIRe16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, product;

  // y || x = xy / (x+y)
  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);

  if(!realIsZero(&x)) {
    realMultiply(&y, &x, &product, &ctxtReal39);
    realAdd(&y, &x, &y, &ctxtReal39);
    realDivide(&product, &y, &x, &ctxtReal39);
  }

  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real16) || X(long integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe16LonI:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, product;

  // y || x = xy / (x+y)
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

  if(!realIsZero(&x)) {
    realMultiply(&y, &x, &product, &ctxtReal39);
    realAdd(&y, &x, &y, &ctxtReal39);
    realDivide(&product, &y, &x, &ctxtReal39);
  }

  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(long integer) || X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelLonICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelLonICo16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y;
  complex39_t x, product, sum;

  // y || x = xy / (x + y)
  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);

  if(!realIsZero(&x.real) || !realIsZero(&x.imag)) {
    realMultiply(&y, &x.real, &product.real, &ctxtReal39);
    realMultiply(&y, &x.imag, &product.imag, &ctxtReal39);
    realAdd(&y, &x.real, &sum.real, &ctxtReal39);
    realCopy(&x.imag, &sum.imag);
    divCo39Co39(&product, &sum, &x);
  }

  realToReal16(&x.real, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&x.imag, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) || X(long integer) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelCo16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo16LonI:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t x;
  complex39_t y, product, sum;

  // y || x = xy / (x + y)
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &y.imag);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);

  if(!realIsZero(&y.real) || !realIsZero(&y.imag)) {
    realMultiply(&x, &y.real, &product.real, &ctxtReal39);
    realMultiply(&x, &y.imag, &product.imag, &ctxtReal39);
    realAdd(&x, &y.real, &sum.real, &ctxtReal39);
    realCopy(&y.imag, &sum.imag);
    divCo39Co39(&product, &sum, &y);
  }

  realToReal16(&y.real, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&y.imag, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) || X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelLonIRe34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, product;

  // y || x = xy / (x+y)
  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  if(!realIsZero(&x)) {
    realMultiply(&y, &x, &product, &ctxtReal39);
    realAdd(&y, &x, &y, &ctxtReal39);
    realDivide(&product, &y, &x, &ctxtReal39);
  }

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) || X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe34LonI:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, product;

  // y || x = xy / (x+y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(!realIsZero(&x)) {
    realMultiply(&y, &x, &product, &ctxtReal39);
    realAdd(&y, &x, &y, &ctxtReal39);
    realDivide(&product, &y, &x, &ctxtReal39);
  }

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(long integer) || X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelLonICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelLonICo34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y;
  complex39_t x, product, sum;

  // y || x = xy / (x + y)
  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  if(!realIsZero(&x.real) || !realIsZero(&x.imag)) {
    realMultiply(&y, &x.real, &product.real, &ctxtReal39);
    realMultiply(&y, &x.imag, &product.imag, &ctxtReal39);
    realAdd(&y, &x.real, &sum.real, &ctxtReal39);
    realCopy(&x.imag, &sum.imag);
    divCo39Co39(&product, &sum, &x);
  }

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) || X(long integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelCo34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo34LonI:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t x;
  complex39_t y, product, sum;

  // y || x = xy / (x + y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &y.imag);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

  if(!realIsZero(&y.real) || !realIsZero(&y.imag)) {
    realMultiply(&x, &y.real, &product.real, &ctxtReal39);
    realMultiply(&x, &y.imag, &product.imag, &ctxtReal39);
    realAdd(&x, &y.real, &sum.real, &ctxtReal39);
    realCopy(&y.imag, &sum.imag);
    divCo39Co39(&product, &sum, &y);
  }

  realToReal34(&y.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&y.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real16 || ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16) || X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe16Re16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe16Re16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, product;

  // y || x = xy / (x+y)
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);

  if(!realIsZero(&x)) {
    realMultiply(&y, &x, &product, &ctxtReal39);
    realAdd(&y, &x, &y, &ctxtReal39);
    realDivide(&product, &y, &x, &ctxtReal39);
  }

  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real16) || X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelRe16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe16Co16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe16Co16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y;
  complex39_t x, product, sum;

  // y || x = xy / (x + y)
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);

  if(!realIsZero(&x.real) || !realIsZero(&x.imag)) {
    realMultiply(&y, &x.real, &product.real, &ctxtReal39);
    realMultiply(&y, &x.imag, &product.imag, &ctxtReal39);
    realAdd(&y, &x.real, &sum.real, &ctxtReal39);
    realCopy(&x.imag, &sum.imag);
    divCo39Co39(&product, &sum, &x);
  }

  realToReal16(&x.real, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&x.imag, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) || X(real16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelCo16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo16Re16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo16Re16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t x;
  complex39_t y, product, sum;

  // y || x = xy / (x + y)
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &y.imag);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);

  if(!realIsZero(&y.real) || !realIsZero(&y.imag)) {
    realMultiply(&x, &y.real, &product.real, &ctxtReal39);
    realMultiply(&x, &y.imag, &product.imag, &ctxtReal39);
    realAdd(&x, &y.real, &sum.real, &ctxtReal39);
    realCopy(&y.imag, &sum.imag);
    divCo39Co39(&product, &sum, &y);
  }

  realToReal16(&y.real, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&y.imag, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) || X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelRe16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe16Re34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe16Re34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, product;

  // y || x = xy / (x+y)
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  if(!realIsZero(&x)) {
    realMultiply(&y, &x, &product, &ctxtReal39);
    realAdd(&y, &x, &y, &ctxtReal39);
    realDivide(&product, &y, &x, &ctxtReal39);
  }

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) || X(real16) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelRe34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe34Re16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe34Re16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, product;

  // y || x = xy / (x+y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(!realIsZero(&x)) {
    realMultiply(&y, &x, &product, &ctxtReal39);
    realAdd(&y, &x, &y, &ctxtReal39);
    realDivide(&product, &y, &x, &ctxtReal39);
  }

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real16) || X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelRe16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe16Co34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe16Co34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y;
  complex39_t x, product, sum;

  // y || x = xy / (x + y)
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  if(!realIsZero(&x.real) || !realIsZero(&x.imag)) {
    realMultiply(&y, &x.real, &product.real, &ctxtReal39);
    realMultiply(&y, &x.imag, &product.imag, &ctxtReal39);
    realAdd(&y, &x.real, &sum.real, &ctxtReal39);
    realCopy(&x.imag, &sum.imag);
    divCo39Co39(&product, &sum, &x);
  }

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) || X(real16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelCo34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo34Re16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo34Re16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t x;
  complex39_t y, product, sum;

  // y || x = xy / (x + y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &y.imag);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

  if(!realIsZero(&y.real) || !realIsZero(&y.imag)) {
    realMultiply(&x, &y.real, &product.real, &ctxtReal39);
    realMultiply(&x, &y.imag, &product.imag, &ctxtReal39);
    realAdd(&x, &y.real, &sum.real, &ctxtReal39);
    realCopy(&y.imag, &sum.imag);
    divCo39Co39(&product, &sum, &y);
  }

  realToReal34(&y.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&y.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* complex16 || ...                                                                                                                                                                       */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16) || X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelCo16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo16Co16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo16Co16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  complex39_t y, x, product, sum;

  // y || x = xy / (x + y)
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &y.imag);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);

  if(!realIsZero(&x.real) || !realIsZero(&x.imag)) {
    mulCo39Co39(&y, &x, &product);
    realAdd(&y.real, &x.real, &sum.real, &ctxtReal39);
    realAdd(&y.imag, &x.imag, &sum.imag, &ctxtReal39);
    divCo39Co39(&product, &sum, &x);
  }

  realToReal16(&x.real, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&x.imag, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) || X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelCo16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo16Re34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo16Re34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t x;
  complex39_t y, product, sum;

  // y || x = xy / (x + y)
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &y.imag);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

  if(!realIsZero(&y.real) || !realIsZero(&y.imag)) {
    realMultiply(&x, &y.real, &product.real, &ctxtReal39);
    realMultiply(&x, &y.imag, &product.imag, &ctxtReal39);
    realAdd(&x, &y.real, &sum.real, &ctxtReal39);
    realCopy(&y.imag, &sum.imag);
    divCo39Co39(&product, &sum, &y);
  }

  realToReal34(&y.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&y.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) || X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelRe34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe34Co16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe34Co16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y;
  complex39_t x, product, sum;

  // y || x = xy / (x + y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

  if(!realIsZero(&x.real) || !realIsZero(&x.imag)) {
    realMultiply(&y, &x.real, &product.real, &ctxtReal39);
    realMultiply(&y, &x.imag, &product.imag, &ctxtReal39);
    realAdd(&y, &x.real, &sum.real, &ctxtReal39);
    realCopy(&x.imag, &sum.imag);
    divCo39Co39(&product, &sum, &x);
  }

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) || X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelCo16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo16Co34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo16Co34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  complex39_t y, x, product, sum;

  // y || x = xy / (x + y)
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &y.imag);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  if(!realIsZero(&x.real) || !realIsZero(&x.real)) {
    mulCo39Co39(&y, &x, &product);
    realAdd(&y.real, &x.real, &sum.real, &ctxtReal39);
    realAdd(&y.imag, &x.imag, &sum.imag, &ctxtReal39);
    divCo39Co39(&product, &sum, &x);
  }

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) || X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelCo34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo34Co16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo34Co16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  complex39_t y, x, product, sum;

  // y || x = xy / (x + y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &y.imag);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

  if(!realIsZero(&x.real) || !realIsZero(&x.real)) {
    mulCo39Co39(&y, &x, &product);
    realAdd(&y.real, &x.real, &sum.real, &ctxtReal39);
    realAdd(&y.imag, &x.imag, &sum.imag, &ctxtReal39);
    divCo39Co39(&product, &sum, &x);
  }

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real34 || ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) || X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe34Re34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe34Re34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, product;

  // y || x = xy / (x+y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  if(!realIsZero(&x)) {
    realMultiply(&y, &x, &product, &ctxtReal39);
    realAdd(&y, &x, &y, &ctxtReal39);
    realDivide(&product, &y, &x, &ctxtReal39);
  }

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) || X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelRe34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe34Co34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelRe34Co34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y;
  complex39_t x, product, sum;

  // y || x = xy / (x + y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  if(!realIsZero(&x.real) || !realIsZero(&x.imag)) {
    realMultiply(&y, &x.real, &product.real, &ctxtReal39);
    realMultiply(&y, &x.imag, &product.imag, &ctxtReal39);
    realAdd(&y, &x.real, &sum.real, &ctxtReal39);
    realCopy(&x.imag, &sum.imag);
    divCo39Co39(&product, &sum, &x);
  }

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) || X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelCo34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo34Re34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo34Re34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t x;
  complex39_t y, product, sum;

  // y || x = xy / (x + y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &y.imag);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

  if(!realIsZero(&y.real) || !realIsZero(&y.imag)) {
    realMultiply(&x, &y.real, &product.real, &ctxtReal39);
    realMultiply(&x, &y.imag, &product.imag, &ctxtReal39);
    realAdd(&x, &y.real, &sum.real, &ctxtReal39);
    realCopy(&y.imag, &sum.imag);
    divCo39Co39(&product, &sum, &y);
  }

  realToReal34(&y.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&y.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* complex34 || ...                                                                                                                                                                       */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex34) || X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelCo34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo34Co34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function parallelCo34Co34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  complex39_t y, x, product, sum;

  // y || x = xy / (x + y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &y.imag);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  if(!realIsZero(&x.real) || !realIsZero(&x.imag)) {
    mulCo39Co39(&y, &x, &product);
    realAdd(&y.real, &x.real, &sum.real, &ctxtReal39);
    realAdd(&y.imag, &x.imag, &sum.imag, &ctxtReal39);
    divCo39Co39(&product, &sum, &x);
  }

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}
