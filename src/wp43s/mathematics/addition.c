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
 * \file addition.c
 ***********************************************/

#include "wp43s.h"



void (* const addition[12][12])(void) = {
// regX |    regY ==>   1            2            3            4         5            6            7            8            9            10            11           12
//      V               Long integer Real16       Complex16    Angle16   Time         Date         String       Real16 mat   Complex16 m  Short integer Real34       Complex34
/*  1 Long integer  */ {addLonILonI, addRe16LonI, addCo16LonI, addError, addTimeLonI, addDateLonI, addStriLonI, addError,    addError,    addShoILonI,  addRe34LonI, addCo34LonI},
/*  2 Real16        */ {addLonIRe16, addRe16Re16, addCo16Re16, addError, addTimeRe16, addDateRe16, addStriRe16, addError,    addError,    addShoIRe16,  addRe34Re16, addCo34Re16},
/*  3 Complex16     */ {addLonICo16, addRe16Co16, addCo16Co16, addError, addError,    addError,    addStriCo16, addError,    addError,    addShoICo16,  addRe34Co16, addCo34Co16},
/*  4 Angle16       */ {addError,    addError,    addError,    addError, addError,    addError,    addError,    addError,    addError,    addError,     addError,    addError   },
/*  5 Time          */ {addLonITime, addRe16Time, addError,    addError, addTimeTime, addError,    addStriTime, addError,    addError,    addError,     addRe34Time, addError   },
/*  6 Date          */ {addLonIDate, addRe16Date, addError,    addError, addError,    addError,    addStriDate, addError,    addError,    addError,     addRe34Date, addError   },
/*  7 String        */ {addError,    addError,    addError,    addError, addError,    addError,    addStriStri, addError,    addError,    addError,     addError,    addError   },
/*  8 Real16 mat    */ {addError,    addError,    addError,    addError, addError,    addError,    addStriRm16, addRm16Rm16, addCm16Rm16, addError,     addError,    addError   },
/*  9 Complex16 mat */ {addError,    addError,    addError,    addError, addError,    addError,    addStriCm16, addRm16Cm16, addCm16Cm16, addError,     addError,    addError   },
/* 10 Short integer */ {addLonIShoI, addRe16ShoI, addCo16ShoI, addError, addError,    addError,    addStriShoI, addError,    addError,    addShoIShoI,  addRe34ShoI, addCo34ShoI},
/* 11 Real34        */ {addLonIRe34, addRe16Re34, addCo16Re34, addError, addTimeRe34, addDateRe34, addStriRe34, addError,    addError,    addShoIRe34,  addRe34Re34, addCo34Re34},
/* 12 Complex34     */ {addLonICo34, addRe16Co34, addCo16Co34, addError, addError,    addError,    addStriCo34, addError,    addError,    addShoICo34,  addRe34Co34, addCo34Co34}
};



/********************************************//**
 * \brief Data type error in addition
 *
 * \param void
 * \return void
 ***********************************************/
void addError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot add %s", getRegisterDataTypeName(REGISTER_X, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    showInfoDialog("In function fnAdd:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regY + regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnAdd(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}



/******************************************************************************************************************************************************************************************/
/* long integer + ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) + X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonILonI(void) {
  longInteger_t y, x;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerAdd(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) + X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addLonIRe16:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t xAngularMode;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realAdd(&y, &x, &x, &ctxtReal39);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real16) + X(long integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16LonI:", "cannot use NaN as Y input of +", NULL, NULL);
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
    realAdd(&y, &x, &x, &ctxtReal39);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(long integer) + X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addLonICo16:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c;

  convertLongIntegerRegisterToReal(REGISTER_Y, &a, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &c);

  realAdd(&a, &c, &c, &ctxtReal39);

  realToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) + X(long integer) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16LonI:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c;
  real16_t b;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16Copy(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realAdd(&a, &c, &c, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
  realToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  real16Copy(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) + X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonITime(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) + X(long integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(long integer) + X(date) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIDate(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(date) + X(long integer) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(long integer) + X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIShoI(void) {
  longInteger_t y, x;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertShortIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerAdd(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(short integer) + X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoILonI(void) {
  longInteger_t y, x;

  convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerAdd(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) + X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addLonIRe34:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t xAngularMode;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realAdd(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) + X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34LonI:", "cannot use NaN as Y input of +", NULL, NULL);
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
    realAdd(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(long integer) + X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addLonICo34:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c;

  convertLongIntegerRegisterToReal(REGISTER_Y, &a, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &c);

  realAdd(&a, &c, &c, &ctxtReal39);

  realToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) + X(long integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34LonI:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c;
  real34_t b;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realAdd(&a, &c, &c, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  real34Copy(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real16 + ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16) + X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Re16:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Re16:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real16Add(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
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

      realAdd(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);

      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real16) + X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Co16:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Co16:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real16Add(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X)); // real part
}



/********************************************//**
 * \brief Y(complex16) + X(real16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16Re16:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16Re16:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real16Add(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
  complex16Copy(REGISTER_COMPLEX16_DATA(REGISTER_Y), REGISTER_COMPLEX16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) + X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Time(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Time:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) + X(real16) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addTimeRe16:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16) + X(date) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Date(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Date:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(date) + X(real16) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addDateRe16:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16) + X(short integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16ShoI:", "cannot use NaN as Y input of +", NULL, NULL);
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
    realAdd(&y, &x, &x, &ctxtReal39);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(short integer) + X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addShoIRe16:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t xAngularMode;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realAdd(&y, &x, &x, &ctxtReal39);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real16) + X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Re34:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Re34:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  convertRegister16To34(REGISTER_Y);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
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

      realAdd(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);

      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) + X(real16) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Re16:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Re16:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  convertRegister16To34(REGISTER_X);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
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

      realAdd(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);

      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real16) + X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Co34:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Co34:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);
  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
}



/********************************************//**
 * \brief Y(complex34) + X(real16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34Re16:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34Re16:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);
  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* complex16 + ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16) + X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16Co16:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16Co16:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real16Add(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X)); // real part
  real16Add(REGISTER_IMAG16_DATA(REGISTER_Y), REGISTER_IMAG16_DATA(REGISTER_X), REGISTER_IMAG16_DATA(REGISTER_X)); // imaginary part
}



/********************************************//**
 * \brief Y(complex16) + X(short integer) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16ShoI:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c;
  real16_t b;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16Copy(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertShortIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realAdd(&a, &c, &c, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
  realToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  real16Copy(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(short integer) + X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addShoICo16:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c;

  convertShortIntegerRegisterToReal(REGISTER_Y, &a, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &c);

  realAdd(&a, &c, &c, &ctxtReal39);

  realToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) + X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16Re34:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16Re34:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);
  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) + X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Co16:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Co16:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);
  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
}



/********************************************//**
 * \brief Y(complex16) + X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16Co34:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16Co34:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);
  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Add(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}



/********************************************//**
 * \brief Y(complex34) + X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34Co16:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34Co16:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);
  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Add(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}



/******************************************************************************************************************************************************************************************/
/* time + ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(time) + X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeTime(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) + X(real34) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addTimeRe34:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real34) + X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Time(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Time:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* date + ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(date) + X(real34) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addDateRe34:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real34) + X(date) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Date(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Date:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* string + ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(string) + X(long integer) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriLonI(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  longIntegerToDisplayString(REGISTER_X, tmpStr3000, TMP_STR_LENGTH, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X),        REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpStr3000,                       len2 + 1);
}



/********************************************//**
 * \brief Y(string) + X(real16) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriRe16(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  real16ToDisplayString(REGISTER_REAL16_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), tmpStr3000, &standardFont, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X),        REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpStr3000,                       len2 + 1);
}



/********************************************//**
 * \brief Y(string) + X(complex16) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriCo16(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  complex16ToDisplayString(REGISTER_COMPLEX16_DATA(REGISTER_X), tmpStr3000, &numericFont, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpStr3000,                       len2 + 1);
}



/********************************************//**
 * \brief Y(string) + X(time) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriTime(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  timeToDisplayString(REGISTER_X, tmpStr3000);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpStr3000,                       len2 + 1);
}



/********************************************//**
 * \brief Y(string) + X(date) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriDate(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  dateToDisplayString(REGISTER_X, tmpStr3000);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpStr3000,                       len2 + 1);
}



/********************************************//**
 * \brief Y(string) + X(string) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriStri(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  len2 = stringByteLength(REGISTER_STRING_DATA(REGISTER_X));

  memcpy(tmpStr3000, REGISTER_STRING_DATA(REGISTER_X), min(TMP_STR_LENGTH, len2 + 1));
  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpStr3000,                       len2 + 1);
}



/********************************************//**
 * \brief Y(string) + X(real16 matrix) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriRm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(string) + X(complex16 matrix) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriCm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(string) + X(short integer) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriShoI(void) {
  int16_t len1, len2;
  const font_t *font;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  font = &standardFont;
  shortIntegerToDisplayString(REGISTER_X, errorMessage, &font); // We use errorMessage here because this string can become very long
  len2 = stringByteLength(errorMessage) + 1; // +1 for the trailing 0

  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, errorMessage,                     len2 + 1);
}



/********************************************//**
 * \brief Y(string) + X(real34) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriRe34(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  real34ToDisplayString(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), tmpStr3000, &standardFont, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X),        REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpStr3000,                       len2 + 1);
}



/********************************************//**
 * \brief Y(string) + X(complex34) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriCo34(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  complex34ToDisplayString(REGISTER_COMPLEX34_DATA(REGISTER_X), tmpStr3000, &numericFont, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpStr3000,                       len2 + 1);
}



/******************************************************************************************************************************************************************************************/
/* real16 matrix + ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16 matrix) + X(real16 matrix) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addRm16Rm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) + X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addRm16Cm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) + X(real16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addCm16Rm16(void) {
  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* complex16 matrix + ...                                                                                                                                                                 */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16 matrix) + X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addCm16Cm16(void) {
  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* short integer + ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(short integer) + X(short integer) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoIShoI(void) {
  setRegisterTag(REGISTER_X, getRegisterTag(REGISTER_Y));
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intAdd(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



/********************************************//**
 * \brief Y(short integer) + X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addShoIRe34:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
  uint32_t xAngularMode;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realAdd(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) + X(short integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34ShoI:", "cannot use NaN as Y input of +", NULL, NULL);
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
    realAdd(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(short integer) + X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addShoICo34:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
}



/********************************************//**
 * \brief Y(complex34) + X(short integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34ShoI:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real34 + ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) + X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Re34:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Re34:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
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

      realAdd(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);

      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) + X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Co34:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Co34:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
}



/********************************************//**
 * \brief Y(complex34) + X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34Re34:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34Re34:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X)); // imaginary part
}



/******************************************************************************************************************************************************************************************/
/* complex34 + ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex34) + X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34Co34:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34Co34:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Add(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}
