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

#include "wp43s.h"



void (* const subtraction[12][12])(void) = {
// regX |    regY ==>   1            2            3            4         5            6            7            8            9            10            11           12
//      V               Long integer Real16       Complex16    Angle16   Time         Date         String       Real16 mat   Complex16 m  Short integer Real34       Complex34
/*  1 Long integer  */ {subLonILonI, subRe16LonI, subCo16LonI, subError, subTimeLonI, subDateLonI, subError,    subError,    subError,    subShoILonI,  subRe34LonI, subCo34LonI},
/*  2 Real16        */ {subLonIRe16, subRe16Re16, subCo16Re16, subError, subTimeRe16, subDateRe16, subError,    subError,    subError,    subShoIRe16,  subRe34Re16, subCo34Re16},
/*  3 Complex16     */ {subLonICo16, subRe16Co16, subCo16Co16, subError, subError,    subError,    subError,    subError,    subError,    subShoICo16,  subRe34Co16, subCo34Co16},
/*  4 Angle16       */ {subError,    subError,    subError,    subError, subError,    subError,    subError,    subError,    subError,    subError,     subError,    subError   },
/*  5 Time          */ {subLonITime, subRe16Time, subError,    subError, subTimeTime, subError,    subError,    subError,    subError,    subError,     subRe34Time, subError   },
/*  6 Date          */ {subLonIDate, subRe16Date, subError,    subError, subError,    subDateDate, subError,    subError,    subError,    subError,     subRe34Date, subError   },
/*  7 String        */ {subError,    subError,    subError,    subError, subError,    subError,    subError,    subError,    subError,    subError,     subError,    subError   },
/*  8 Real16 mat    */ {subError,    subError,    subError,    subError, subError,    subError,    subError,    subRm16Rm16, subCm16Rm16, subError,     subError,    subError   },
/*  9 Complex16 mat */ {subError,    subError,    subError,    subError, subError,    subError,    subError,    subRm16Cm16, subCm16Cm16, subError,     subError,    subError   },
/* 10 Short integer */ {subLonIShoI, subRe16ShoI, subCo16ShoI, subError, subError,    subError,    subError,    subError,    subError,    subShoIShoI,  subRe34ShoI, subCo34ShoI},
/* 11 Real34        */ {subLonIRe34, subRe16Re34, subCo16Re34, subError, subTimeRe34, subDateRe34, subError,    subError,    subError,    subShoIRe34,  subRe34Re34, subCo34Re34},
/* 12 Complex34     */ {subLonICo34, subRe16Co34, subCo16Co34, subError, subError,    subError,    subError,    subError,    subError,    subShoICo34,  subRe34Co34, subCo34Co34}
};



/********************************************//**
 * \brief Data type error in subtraction
 *
 * \param void
 * \return void
 ***********************************************/
void subError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot subtract %s", getRegisterDataTypeName(REGISTER_X, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "from %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    showInfoDialog("In function fnSubtract:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief rexX ==> regL and regY - regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSubtract(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

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

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) - X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonIRe16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t xAngularMode;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realSubtract(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
      realSubtract(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real16) - X(long integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16LonI:", "cannot use NaN as Y input of -", NULL, NULL);
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
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realSubtract(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      realSubtract(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(long integer) - X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonICo16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c;

  convertLongIntegerRegisterToReal(REGISTER_Y, &a, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &c);

  realSubtract(&a, &c, &c, &ctxtReal39);

  realToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  real16ChangeSign(REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) - X(long integer) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16LonI:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c;
  real16_t b;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16Copy(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realSubtract(&a, &c, &c, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
  realToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  real16Copy(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) - X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonITime(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) - X(long integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(long integer) - X(date) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIDate(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(date) - X(long integer) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateLonI(void) {
  fnToBeCoded();
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
void subLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonIRe34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t xAngularMode;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realSubtract(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
      realSubtract(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) - X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34LonI:", "cannot use NaN as Y input of -", NULL, NULL);
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
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realSubtract(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      realSubtract(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(long integer) - X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonICo34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c;

  convertLongIntegerRegisterToReal(REGISTER_Y, &a, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &c);

  realSubtract(&a, &c, &c, &ctxtReal39);

  realToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) - X(long integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34LonI:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c;
  real34_t b;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realSubtract(&a, &c, &c, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  real34Copy(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real16 - ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16) - X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Re16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Re16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real16Subtract(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
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

      realSubtract(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);

      realSubtract(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real16) - X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Co16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Co16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real16Subtract(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X)); // real part
  real16ChangeSign(REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) - X(real16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16Re16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16Re16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real16Subtract(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
  complex16Copy(REGISTER_COMPLEX16_DATA(REGISTER_Y), REGISTER_COMPLEX16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) - X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Time(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Time:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) - X(real16) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subTimeRe16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16) - X(date) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Date(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Date:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(date) - X(real16) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subDateRe16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16) - X(short integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16ShoI:", "cannot use NaN as Y input of -", NULL, NULL);
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
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realSubtract(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      realSubtract(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(short integer) - X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subShoIRe16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t xAngularMode;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realSubtract(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
      realSubtract(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real16) - X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Re34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Re34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  convertRegister16To34(REGISTER_Y);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
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

      realSubtract(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);

      realSubtract(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) - X(real16) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Re16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Re16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  convertRegister16To34(REGISTER_X);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
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

      realSubtract(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);

      realSubtract(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real16) - X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Co34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Co34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) - X(real16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34Re16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34Re16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* complex16 - ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16) - X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16Co16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16Co16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real16Subtract(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X)); // real part
  real16Subtract(REGISTER_IMAG16_DATA(REGISTER_Y), REGISTER_IMAG16_DATA(REGISTER_X), REGISTER_IMAG16_DATA(REGISTER_X)); // imaginary part
}



/********************************************//**
 * \brief Y(complex16) - X(short integer) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16ShoI:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c;
  real16_t b;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16Copy(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertShortIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realSubtract(&a, &c, &c, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
  realToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  real16Copy(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(short integer) - X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subShoICo16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c;

  convertShortIntegerRegisterToReal(REGISTER_Y, &a, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &c);

  realSubtract(&a, &c, &c, &ctxtReal39);

  realToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  real16ChangeSign(REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) - X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16Re34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16Re34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) - X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Co16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Co16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) - X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16Co34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16Co34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Subtract(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}



/********************************************//**
 * \brief Y(complex34) - X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34Co16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34Co16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Subtract(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
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
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) - X(real34) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subTimeRe34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real34) - X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Time(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Time:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
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
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(date) - X(real34) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subDateRe34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real34) - X(date) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Date(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Date:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
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
void subRm16Rm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) - X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subRm16Cm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) - X(real16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subCm16Rm16(void) {
  fnToBeCoded();
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
void subCm16Cm16(void) {
  fnToBeCoded();
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
void subShoIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subShoIRe34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t xAngularMode;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realSubtract(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
      realSubtract(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) - X(short integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34ShoI:", "cannot use NaN as Y input of -", NULL, NULL);
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
    realSubtract(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realSubtract(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      realSubtract(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(short integer) - X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subShoICo34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));
}




/********************************************//**
 * \brief Y(complex34) - X(short integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34ShoI:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
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
void subRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Re34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Re34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
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

      realSubtract(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);

      realSubtract(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) - X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Co34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Co34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) - X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34Re34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34Re34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
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
void subCo34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34Co34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34Co34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Subtract(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}



