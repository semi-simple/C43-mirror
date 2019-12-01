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
 * \file multiplication.c
 ***********************************************/

#include "wp43s.h"



void (* const multiplication[12][12])(void) = {
// regX |    regY ==>   1            2            3            4            5            6            7            8            9            10            11           12
//      V               Long integer Real16       Complex16    Angle16      Time         Date         String       Real16 mat   Complex16 m  Short integer Real34       Complex34
/*  1 Long integer  */ {mulLonILonI, mulRe16LonI, mulCo16LonI, mulError, mulTimeLonI, mulError,    mulError,    mulRm16LonI, mulCm16LonI, mulShoILonI,  mulRe34LonI, mulCo34LonI},
/*  2 Real16        */ {mulLonIRe16, mulRe16Re16, mulCo16Re16, mulError, mulTimeRe16, mulError,    mulError,    mulRm16Re16, mulCm16Re16, mulShoIRe16,  mulRe34Re16, mulCo34Re16},
/*  3 Complex16     */ {mulLonICo16, mulRe16Co16, mulCo16Co16, mulError, mulError,    mulError,    mulError,    mulRm16Co16, mulCm16Co16, mulShoICo16,  mulRe34Co16, mulCo34Co16},
/*  4 Angle16       */ {mulError,    mulError,    mulError,    mulError, mulError,    mulError,    mulError,    mulError,    mulError,    mulError,     mulError,    mulError   },
/*  5 Time          */ {mulLonITime, mulRe16Time, mulError,    mulError, mulError,    mulError,    mulError,    mulError,    mulError,    mulShoITime,  mulRe34Time, mulError   },
/*  6 Date          */ {mulError,    mulError,    mulError,    mulError, mulError,    mulError,    mulError,    mulError,    mulError,    mulError,     mulError,    mulError   },
/*  7 String        */ {mulError,    mulError,    mulError,    mulError, mulError,    mulError,    mulError,    mulError,    mulError,    mulError,     mulError,    mulError   },
/*  8 Real16 mat    */ {mulLonIRm16, mulRe16Rm16, mulCo16Rm16, mulError, mulError,    mulError,    mulError,    mulRm16Rm16, mulCm16Rm16, mulShoIRm16,  mulRe34Rm16, mulCo34Rm16},
/*  9 Complex16 mat */ {mulLonICm16, mulRe16Cm16, mulCo16Cm16, mulError, mulError,    mulError,    mulError,    mulRm16Cm16, mulCm16Cm16, mulShoICm16,  mulRe34Cm16, mulCo34Cm16},
/* 10 Short integer */ {mulLonIShoI, mulRe16ShoI, mulCo16ShoI, mulError, mulTimeShoI, mulError,    mulError,    mulRm16ShoI, mulCm16ShoI, mulShoIShoI,  mulRe34ShoI, mulCo34ShoI},
/* 11 Real34        */ {mulLonIRe34, mulRe16Re34, mulCo16Re34, mulError, mulTimeRe34, mulError,    mulError,    mulRm16Re34, mulCm16Re34, mulShoIRe34,  mulRe34Re34, mulCo34Re34},
/* 12 Complex34     */ {mulLonICo34, mulRe16Co34, mulCo16Co34, mulError, mulError,    mulError,    mulError,    mulRm16Co34, mulCm16Co34, mulShoICo34,  mulRe34Co34, mulCo34Co34}
};



/********************************************//**
 * \brief Data type error in multiplication
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void mulError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot multiply %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnMultiply:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regY × regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnMultiply(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}



void mulCo39Co39(const complex39_t *factor1, const complex39_t *factor2, complex39_t *product) {
  real39_t a, b, c, d;

  realCopy(&factor1->real, &a);
  realCopy(&factor1->imag, &b);
  realCopy(&factor2->real, &c);
  realCopy(&factor2->imag, &d);

  // imaginary part
  realMultiply(&a, &d, &product->imag, &ctxtReal39);            // a*d
  realFMA(&b, &c, &product->imag, &product->imag, &ctxtReal39); // a*d + b*c

  // real part
  realChangeSign(&b);                                           // -b
  realMultiply(&a, &c, &product->real, &ctxtReal39);            // a*c
  realFMA(&b, &d, &product->real, &product->real, &ctxtReal39); // a*c - b*d
}



void mulCo51Co51(const complex51_t *factor1, const complex51_t *factor2, complex51_t *product) {
  real51_t a, b, c, d;

  realCopy(&factor1->real, &a);
  realCopy(&factor1->imag, &b);
  realCopy(&factor2->real, &c);
  realCopy(&factor2->imag, &d);

  // imaginary part
  realMultiply(&a, &d, &product->imag, &ctxtReal51);            // a*d
  realFMA(&b, &c, &product->imag, &product->imag, &ctxtReal51); // a*d + b*c

  // real part
  realChangeSign(&b);                                           // -b
  realMultiply(&a, &c, &product->real, &ctxtReal51);            // a*c
  realFMA(&b, &d, &product->real, &product->real, &ctxtReal51); // a*c - b*d
}



void mulCo75Co75(const complex75_t *factor1, const complex75_t *factor2, complex75_t *product) {
  real75_t a, b, c, d;

  realCopy(&factor1->real, &a);
  realCopy(&factor1->imag, &b);
  realCopy(&factor2->real, &c);
  realCopy(&factor2->imag, &d);

  // imaginary part
  realMultiply(&a, &d, &product->imag, &ctxtReal75);            // a*d
  realFMA(&b, &c, &product->imag, &product->imag, &ctxtReal75); // a*d + b*c

  // real part
  realChangeSign(&b);                                           // -b
  realMultiply(&a, &c, &product->real, &ctxtReal75);            // a*c
  realFMA(&b, &d, &product->real, &product->real, &ctxtReal75); // a*c - b*d
}



/******************************************************************************************************************************************************************************************/
/* long integer × ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) × X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonILonI(void) {
  longInteger_t y, x;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerMultiply(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) × X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulLonIRe16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t xAngularMode;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realMultiply(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real16) × X(long integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16LonI:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t yAngularMode;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

  if(yAngularMode == AM_NONE) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realMultiply(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(long integer) × X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulLonICo16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c, d;

  convertLongIntegerRegisterToReal(REGISTER_Y, &a, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &c);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &d);

  realMultiply(&a, &c, &c, &ctxtReal39);
  realMultiply(&a, &d, &d, &ctxtReal39);

  realToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) × X(long integer) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16LonI:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b, c;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realMultiply(&a, &c, &a, &ctxtReal39);
  realMultiply(&b, &c, &b, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
  realToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) × X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonITime(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) × X(long integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(long integer) × X(real16 matrix) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIRm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) × X(long integer) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16LonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(long integer) × X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonICm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) × X(long integer) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16LonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(long integer) × X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIShoI(void) {
  longInteger_t y, x;

  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerMultiply(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(short integer) × X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoILonI(void) {
  longInteger_t y, x;

  convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerMultiply(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) × X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulLonIRe34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t xAngularMode;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realMultiply(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) × X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34LonI:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t yAngularMode;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(yAngularMode == AM_NONE) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realMultiply(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(long integer) × X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulLonICo34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c, d;

  convertLongIntegerRegisterToReal(REGISTER_Y, &a, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  realMultiply(&c, &a, &c, &ctxtReal39);
  realMultiply(&d, &a, &d, &ctxtReal39);

  realToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) × X(long integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo34LonI:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b, c;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realMultiply(&a, &c, &a, &ctxtReal39);
  realMultiply(&b, &c, &b, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real16 × ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16) × X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Re16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Re16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real16Multiply(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
  }
  else if(yAngularMode != AM_NONE && xAngularMode != AM_NONE) {
    real16Multiply(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, AM_NONE);
  }
  else {
    real39_t y, x;

    if(yAngularMode == AM_NONE) {
      yAngularMode = currentAngularMode;
    }
    else if(xAngularMode == AM_NONE) {
      xAngularMode = currentAngularMode;
    }

    real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
    real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);

    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);

      realMultiply(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);

      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real16) × X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Co16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Co16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real16Multiply(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X)); // real part
  real16Multiply(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_IMAG16_DATA(REGISTER_X), REGISTER_IMAG16_DATA(REGISTER_X)); // imaginary part
}



/********************************************//**
 * \brief Y(complex16) × X(real16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16Re16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16Re16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real16Multiply(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_Y)); // real part
  real16Multiply(REGISTER_IMAG16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_IMAG16_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
  complex16Copy(REGISTER_COMPLEX16_DATA(REGISTER_Y), REGISTER_COMPLEX16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) × X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Time(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Time:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) × X(real16) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulTimeRe16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16) × X(real16 matrix) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Rm16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Rm16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) × X(real16) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRm16Re16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16) × X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Cm16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Cm16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) × X(real16) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCm16Re16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16) × X(short integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16ShoI:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t yAngularMode;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

  if(yAngularMode == AM_NONE) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realMultiply(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(short integer) × X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulShoIRe16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t xAngularMode;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realMultiply(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real16) × X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Re34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Re34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  convertRegister16To34(REGISTER_Y);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(yAngularMode != AM_NONE && xAngularMode != AM_NONE) {
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, AM_NONE);
  }
  else {
    real39_t y, x;

    if(yAngularMode == AM_NONE) {
      yAngularMode = currentAngularMode;
    }
    else if(xAngularMode == AM_NONE) {
      xAngularMode = currentAngularMode;
    }

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);

      realMultiply(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);

      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) × X(real16) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34Re16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34Re16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  convertRegister16To34(REGISTER_X);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(yAngularMode != AM_NONE && xAngularMode != AM_NONE) {
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, AM_NONE);
  }
  else {
    real39_t y, x;

    if(yAngularMode == AM_NONE) {
      yAngularMode = currentAngularMode;
    }
    else if(xAngularMode == AM_NONE) {
      xAngularMode = currentAngularMode;
    }

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);

      realMultiply(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);

      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real16) × X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Co34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Co34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}



/********************************************//**
 * \brief Y(complex34) × X(real16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo34Re16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo34Re16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* complex16 × ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16) × X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16Co16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16Co16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  complex39_t y, x;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &y.imag);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);

  mulCo39Co39(&y, &x, &x);

  realToReal16(&x.real, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&x.imag, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) × X(real16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Rm16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16Rm16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) × X(complex16) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRm16Co16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16) × X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Cm16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16Cm16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) × X(complex16) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCm16Co16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16) × X(short integer) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16ShoI:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b, c;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertShortIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realMultiply(&a, &c, &a, &ctxtReal39);
  realMultiply(&b, &c, &b, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
  realToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(short integer) × X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulShoICo16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c, d;

  convertShortIntegerRegisterToReal(REGISTER_Y, &a, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &c);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &d);

  realMultiply(&a, &c, &c, &ctxtReal39);
  realMultiply(&a, &d, &d, &ctxtReal39);

  realToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) × X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16Re34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16Re34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) × X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34Co16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34Co16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}



/********************************************//**
 * \brief Y(complex16) × X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16Co34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16Co34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  complex39_t y, x;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &y.imag);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  mulCo39Co39(&y, &x, &x);

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) × X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo34Co16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo34Co16:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  complex39_t y, x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &y.imag);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

  mulCo39Co39(&y, &x, &x);

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* time × ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(time) × X(short integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(short integer) × X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoITime(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) × X(real34) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulTimeRe34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real34) × X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Time(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34Time:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* time × ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/



/******************************************************************************************************************************************************************************************/
/* string × ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/



/******************************************************************************************************************************************************************************************/
/* real16 matrix × ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16 matrix) × X(real16 matrix) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Rm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) × X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Cm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) × X(real16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Rm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) × X(short integer) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16ShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(short integer) × X(real16 matrix) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIRm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) × X(real34) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRm16Re34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real34) × X(real16 matrix) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Rm16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34Rm16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) × X(complex34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRm16Co34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex34) × X(real16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Rm16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRm16Co34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* complex16 matrix × ...                                                                                                                                                                 */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16 matrix) × X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Cm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) × X(short integer) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16ShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(short integer) × X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoICm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) × X(real34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCm16Re34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real34) × X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Cm16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34Cm16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) × X(complex34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCm16Co34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex34) × X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Cm16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo34Cm16:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* short integer × ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(short integer) × X(short integer) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIShoI(void) {
  setRegisterTag(REGISTER_X, getRegisterTag(REGISTER_Y));
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intMultiply(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



/********************************************//**
 * \brief Y(short integer) × X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulShoIRe34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t xAngularMode;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realMultiply(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) × X(short integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34ShoI:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t yAngularMode;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(yAngularMode == AM_NONE) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realMultiply(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(short integer) × X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulShoICo34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}



/********************************************//**
 * \brief Y(complex34) × X(short integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo34ShoI:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real34 × ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) × X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34Re34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34Re34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(yAngularMode != AM_NONE && xAngularMode != AM_NONE) {
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, AM_NONE);
  }
  else {
    real39_t y, x;

    if(yAngularMode == AM_NONE) {
      yAngularMode = currentAngularMode;
    }
    else if(xAngularMode == AM_NONE) {
      xAngularMode = currentAngularMode;
    }

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);

      realMultiply(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);

      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) × X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34Co34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34Co34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}



/********************************************//**
 * \brief Y(complex34) × X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo34Re34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo34Re34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X)); // imaginary part
}



/******************************************************************************************************************************************************************************************/
/* complex34 × ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex34) × X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo34Co34:", "cannot use NaN as Y input of x", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo34Co34:", "cannot use NaN as X input of x", NULL, NULL);
    #endif
    return;
  }

  complex39_t y, x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &y.imag);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  mulCo39Co39(&y, &x, &x);

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}
