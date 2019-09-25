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



void mulCoIcCoIc(const complexIc_t *factor1, const complexIc_t *factor2, complexIc_t *product) {
  realIc_t a, b, c, d;

  realIcCopy(&factor1->real, &a);
  realIcCopy(&factor1->imag, &b);
  realIcCopy(&factor2->real, &c);
  realIcCopy(&factor2->imag, &d);

  // imaginary part
  realIcMultiply(&a, &d, &product->imag);            // a*d
  realIcFMA(&b, &c, &product->imag, &product->imag); // a*d + b*c

  // real part
  realIcChangeSign(&b);                              // -b
  realIcMultiply(&a, &c, &product->real);            // a*c
  realIcFMA(&b, &d, &product->real, &product->real); // a*c - b*d
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

  realIc_t y, x;
  uint32_t xAngularMode;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &y);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realIcMultiply(&y, &x, &x);
    realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&x, xAngularMode, AM_DEGREE);
      realIcMultiply(&y, &x, &x);
      convertAngleIcFromTo(&x, AM_DEGREE, AM_DMS);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&x, xAngularMode, currentAngularMode);
      realIcMultiply(&y, &x, &x);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
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

  realIc_t y, x;
  uint32_t yAngularMode;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &x);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

  if(yAngularMode == AM_NONE) {
    realIcMultiply(&y, &x, &x);
    realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&y, yAngularMode, AM_DEGREE);
      realIcMultiply(&y, &x, &x);
      convertAngleIcFromTo(&x, AM_DEGREE, AM_DMS);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&y, yAngularMode, currentAngularMode);
      realIcMultiply(&y, &x, &x);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
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

  realIc_t a, c, d;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &d);

  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
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

  realIc_t a, b, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);

  realIcMultiply(&a, &c, &a);
  realIcMultiply(&b, &c, &b);

  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
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
 * \brief Y(long integer) × X(64bits integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIShoI(void) {
  longInteger_t y, x;

  convertShortIntegerRegisterLongIntegerRegister(REGISTER_X, REGISTER_X);

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerMultiply(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(64bits integer) × X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoILonI(void) {
  longInteger_t y, x;

  convertShortIntegerRegisterLongIntegerRegister(REGISTER_Y, REGISTER_Y);

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
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

  realIc_t y, x;
  uint32_t xAngularMode;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &y);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realIcMultiply(&y, &x, &x);
    realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&x, xAngularMode, AM_DEGREE);
      realIcMultiply(&y, &x, &x);
      convertAngleIcFromTo(&x, AM_DEGREE, AM_DMS);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&x, xAngularMode, currentAngularMode);
      realIcMultiply(&y, &x, &x);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
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

  realIc_t y, x;
  uint32_t yAngularMode;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &x);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(yAngularMode == AM_NONE) {
    realIcMultiply(&y, &x, &x);
    realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&y, yAngularMode, AM_DEGREE);
      realIcMultiply(&y, &x, &x);
      convertAngleIcFromTo(&x, AM_DEGREE, AM_DMS);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&y, yAngularMode, currentAngularMode);
      realIcMultiply(&y, &x, &x);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
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

  realIc_t a, c, d;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  realIcMultiply(&c, &a, &c);
  realIcMultiply(&d, &a, &d);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
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

  realIc_t a, b, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);

  realIcMultiply(&a, &c, &a);
  realIcMultiply(&b, &c, &b);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
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
    realIc_t y, x;

    if(yAngularMode == AM_NONE) {
      yAngularMode = currentAngularMode;
    }
    else if(xAngularMode == AM_NONE) {
      xAngularMode = currentAngularMode;
    }

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y);
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);

    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&y, yAngularMode, AM_DEGREE);
      convertAngleIcFromTo(&x, xAngularMode, AM_DEGREE);

      realIcMultiply(&y, &x, &x);

      convertAngleIcFromTo(&x, AM_DEGREE, AM_DMS);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngleIcFromTo(&y, yAngularMode, currentAngularMode);
      convertAngleIcFromTo(&x, xAngularMode, currentAngularMode);

      realIcMultiply(&y, &x, &x);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
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
 * \brief Y(real16) × X(64bits integer) ==> X(real16)
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

  realIc_t y, x;
  uint32_t yAngularMode;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertShortIntegerRegisterToRealIc(REGISTER_X, &x);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

  if(yAngularMode == AM_NONE) {
    realIcMultiply(&y, &x, &x);
    realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&y, yAngularMode, AM_DEGREE);
      realIcMultiply(&y, &x, &x);
      convertAngleIcFromTo(&x, AM_DEGREE, AM_DMS);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&y, yAngularMode, currentAngularMode);
      realIcMultiply(&y, &x, &x);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(64bits integer) × X(real16) ==> X(real16)
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

  realIc_t y, x;
  uint32_t xAngularMode;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &y);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realIcMultiply(&y, &x, &x);
    realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&x, xAngularMode, AM_DEGREE);
      realIcMultiply(&y, &x, &x);
      convertAngleIcFromTo(&x, AM_DEGREE, AM_DMS);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&x, xAngularMode, currentAngularMode);
      realIcMultiply(&y, &x, &x);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
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
    realIc_t y, x;

    if(yAngularMode == AM_NONE) {
      yAngularMode = currentAngularMode;
    }
    else if(xAngularMode == AM_NONE) {
      xAngularMode = currentAngularMode;
    }

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);

    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&y, yAngularMode, AM_DEGREE);
      convertAngleIcFromTo(&x, xAngularMode, AM_DEGREE);

      realIcMultiply(&y, &x, &x);

      convertAngleIcFromTo(&x, AM_DEGREE, AM_DMS);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngleIcFromTo(&y, yAngularMode, currentAngularMode);
      convertAngleIcFromTo(&x, xAngularMode, currentAngularMode);

      realIcMultiply(&y, &x, &x);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
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
    realIc_t y, x;

    if(yAngularMode == AM_NONE) {
      yAngularMode = currentAngularMode;
    }
    else if(xAngularMode == AM_NONE) {
      xAngularMode = currentAngularMode;
    }

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);

    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&y, yAngularMode, AM_DEGREE);
      convertAngleIcFromTo(&x, xAngularMode, AM_DEGREE);

      realIcMultiply(&y, &x, &x);

      convertAngleIcFromTo(&x, AM_DEGREE, AM_DMS);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngleIcFromTo(&y, yAngularMode, currentAngularMode);
      convertAngleIcFromTo(&x, xAngularMode, currentAngularMode);

      realIcMultiply(&y, &x, &x);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
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

  complexIc_t y, x;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y.real);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &y.imag);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);

  mulCoIcCoIc(&y, &x, &x);

  realIcToReal16(&x.real, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&x.imag, REGISTER_IMAG16_DATA(REGISTER_X));
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
 * \brief Y(complex16) × X(64bits integer) ==> X(complex16)
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

  realIc_t a, b, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);

  realIcMultiply(&a, &c, &a);
  realIcMultiply(&b, &c, &b);

  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(64bits integer) × X(complex16) ==> X(complex16)
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

  realIc_t a, c, d;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &d);

  realIcMultiply(&a, &c, &c);
  realIcMultiply(&a, &d, &d);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
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

  complexIc_t y, x;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y.real);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &y.imag);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  mulCoIcCoIc(&y, &x, &x);

  realIcToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
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

  complexIc_t y, x;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y.real);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_Y), &y.imag);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

  mulCoIcCoIc(&y, &x, &x);

  realIcToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* time × ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(time) × X(64bits integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(64bits integer) × X(time) ==> X(time)
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
 * \brief Y(real16 matrix) × X(64bits integer) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16ShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(64bits integer) × X(real16 matrix) ==> X(real16 matrix)
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
 * \brief Y(complex16 matrix) × X(64bits integer) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16ShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(64bits integer) × X(complex16 matrix) ==> X(complex16 matrix)
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
 * \brief Y(64bits integer) × X(64bits integer) ==> X(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIShoI(void) {
  setRegisterTag(REGISTER_X, getRegisterTag(REGISTER_Y));
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intMultiply(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



/********************************************//**
 * \brief Y(64bits integer) × X(real34) ==> X(real34)
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

  realIc_t y, x;
  uint32_t xAngularMode;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &y);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realIcMultiply(&y, &x, &x);
    realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&x, xAngularMode, AM_DEGREE);
      realIcMultiply(&y, &x, &x);
      convertAngleIcFromTo(&x, AM_DEGREE, AM_DMS);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&x, xAngularMode, currentAngularMode);
      realIcMultiply(&y, &x, &x);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) × X(64bits integer) ==> X(real34)
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

  realIc_t y, x;
  uint32_t yAngularMode;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertShortIntegerRegisterToRealIc(REGISTER_X, &x);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(yAngularMode == AM_NONE) {
    realIcMultiply(&y, &x, &x);
    realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&y, yAngularMode, AM_DEGREE);
      realIcMultiply(&y, &x, &x);
      convertAngleIcFromTo(&x, AM_DEGREE, AM_DMS);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&y, yAngularMode, currentAngularMode);
      realIcMultiply(&y, &x, &x);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(64bits integer) × X(complex34) ==> X(complex34)
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
 * \brief Y(complex34) × X(64bits integer) ==> X(complex34)
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
    realIc_t y, x;

    if(yAngularMode == AM_NONE) {
      yAngularMode = currentAngularMode;
    }
    else if(xAngularMode == AM_NONE) {
      xAngularMode = currentAngularMode;
    }

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);

    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&y, yAngularMode, AM_DEGREE);
      convertAngleIcFromTo(&x, xAngularMode, AM_DEGREE);

      realIcMultiply(&y, &x, &x);

      convertAngleIcFromTo(&x, AM_DEGREE, AM_DMS);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngleIcFromTo(&y, yAngularMode, currentAngularMode);
      convertAngleIcFromTo(&x, xAngularMode, currentAngularMode);

      realIcMultiply(&y, &x, &x);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
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

  complexIc_t y, x;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y.real);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_Y), &y.imag);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  mulCoIcCoIc(&y, &x, &x);

  realIcToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}
