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



void (* const subtraction[13][13])(void) = {
// regX |    regY ==>   1            2            3            4            5            6            7            8            9            10            11           12           13
//      V               Long integer Real16       Complex16    Angle16      Time         Date         String       Real16 mat   Complex16 m  Short integer Real34       Complex34    Angle34
/*  1 Long integer  */ {subLonILonI, subRe16LonI, subCo16LonI, subAn16LonI, subTimeLonI, subDateLonI, subError,    subError,    subError,    subShoILonI,  subRe34LonI, subCo34LonI, subAn34LonI},
/*  2 Real16        */ {subLonIRe16, subRe16Re16, subCo16Re16, subAn16Re16, subTimeRe16, subDateRe16, subError,    subError,    subError,    subShoIRe16,  subRe34Re16, subCo34Re16, subAn34Re16},
/*  3 Complex16     */ {subLonICo16, subRe16Co16, subCo16Co16, subAn16Co16, subError,    subError,    subError,    subError,    subError,    subShoICo16,  subRe34Co16, subCo34Co16, subAn34Co16},
/*  4 Angle16       */ {subLonIAn16, subRe16An16, subCo16An16, subAn16An16, subTimeAn16, subDateAn16, subError,    subError,    subError,    subShoIAn16,  subRe34An16, subCo34An16, subAn34An16},
/*  5 Time          */ {subLonITime, subRe16Time, subError,    subAn16Time, subTimeTime, subError,    subError,    subError,    subError,    subError,     subRe34Time, subError,    subAn34Time},
/*  6 Date          */ {subLonIDate, subRe16Date, subError,    subAn16Date, subError,    subDateDate, subError,    subError,    subError,    subError,     subRe34Date, subError,    subAn34Date},
/*  7 String        */ {subError,    subError,    subError,    subError,    subError,    subError,    subError,    subError,    subError,    subError,     subError,    subError,    subError   },
/*  8 Real16 mat    */ {subError,    subError,    subError,    subError,    subError,    subError,    subError,    subRm16Rm16, subCm16Rm16, subError,     subError,    subError,    subError   },
/*  9 Complex16 mat */ {subError,    subError,    subError,    subError,    subError,    subError,    subError,    subRm16Cm16, subCm16Cm16, subError,     subError,    subError,    subError   },
/* 10 Short integer */ {subLonIShoI, subRe16ShoI, subCo16ShoI, subAn16ShoI, subError,    subError,    subError,    subError,    subError,    subShoIShoI,  subRe34ShoI, subCo34ShoI, subAn34ShoI},
/* 11 Real34        */ {subLonIRe34, subRe16Re34, subCo16Re34, subAn16Re34, subTimeRe34, subDateRe34, subError,    subError,    subError,    subShoIRe34,  subRe34Re34, subCo34Re34, subAn34Re34},
/* 12 Complex34     */ {subLonICo34, subRe16Co34, subCo16Co34, subAn16Co34, subError,    subError,    subError,    subError,    subError,    subShoICo34,  subRe34Co34, subCo34Co34, subAn34Co34},
/* 13 Angle34       */ {subLonIAn34, subRe16An34, subCo16An34, subAn16An34, subTimeAn34, subDateAn34, subError,    subError,    subError,    subShoIAn34,  subRe34An34, subCo34An34, subAn34An34},
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



/**********************************************************************
 * In all the functions below:
 * if Y is a number then Y = a + ib
 * if X is a number then X = c + id
 * The variables a, b, c and d are used for intermediate calculations
 * The result is then X = (a - c) + i(b - d)
 ***********************************************************************/

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
  longInteger_t a, c;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, a);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, c);

  longIntegerSubtract(a, c, c);

  convertLongIntegerToLongIntegerRegister(c, REGISTER_X);

  longIntegerFree(a);
  longIntegerFree(c);
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

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  realIcSubtract(&a, &c, &c);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
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

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);

  realIcSubtract(&a, &c, &c);

  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
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

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  realIcSubtract(&a, &c, &c);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
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

  realIc_t a, c;
  real16_t b;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16Copy(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);

  realIcSubtract(&a, &c, &c);

  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  real16Copy(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) - X(angle16) ==> X(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonIAn16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&c, getRegisterTag(REGISTER_X), AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
    checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&c, getRegisterTag(REGISTER_X), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(angle16) - X(long integer) ==> X(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16LonI:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    reallocateRegister(REGISTER_X, dtAngle16, REAL16_SIZE, AM_DMS);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
    checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    reallocateRegister(REGISTER_X, dtAngle16, REAL16_SIZE, currentAngularMode);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  }
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
 * \brief Y(long integer) - X(64bits integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIShoI(void) {
  longInteger_t a, c;

  convertShortIntegerRegisterLongIntegerRegister(REGISTER_X, REGISTER_X);

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, a);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, c);

  longIntegerSubtract(a, c, c);

  convertLongIntegerToLongIntegerRegister(c, REGISTER_X);

  longIntegerFree(a);
  longIntegerFree(c);
}



/********************************************//**
 * \brief Y(64bits integer) - X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoILonI(void) {
  longInteger_t a, c;

  convertShortIntegerRegisterLongIntegerRegister(REGISTER_Y, REGISTER_Y);

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, a);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, c);

  longIntegerSubtract(a, c, c);

  convertLongIntegerToLongIntegerRegister(c, REGISTER_X);

  longIntegerFree(a);
  longIntegerFree(c);
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

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  realIcSubtract(&a, &c, &c);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
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

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);

  realIcSubtract(&a, &c, &c);

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
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

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  realIcSubtract(&a, &c, &c);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
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

  realIc_t a, c;
  real34_t b;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);

  realIcSubtract(&a, &c, &c);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  real34Copy(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) - X(angle34) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonIAn34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&c, getRegisterTag(REGISTER_X), AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&c, getRegisterTag(REGISTER_X), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterAngularMode(REGISTER_X, currentAngularMode);
}



/********************************************//**
 * \brief Y(angle34) - X(long integer) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34LonI:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    reallocateRegister(REGISTER_X, dtAngle34, REAL34_SIZE, AM_DMS);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    reallocateRegister(REGISTER_X, dtAngle34, REAL34_SIZE, currentAngularMode);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }
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

  real16Subtract(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
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
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  complex16Copy(REGISTER_COMPLEX16_DATA(REGISTER_Y), REGISTER_COMPLEX16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) - X(angle16) ==> X(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16An16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16An16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&a, AM_DMS, AM_DEGREE);
    convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
    checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);
}



/********************************************//**
 * \brief Y(angle16) - X(real16) ==> X(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Re16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Re16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
    convertAngleIcFromTo(&c, AM_DMS, AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    reallocateRegister(REGISTER_X, dtAngle16, REAL16_SIZE, AM_DMS);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
    checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    reallocateRegister(REGISTER_X, dtAngle16, REAL16_SIZE, currentAngularMode);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  }
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
 * \brief Y(real16) - X(64bits integer) ==> X(real16)
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

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);

  realIcSubtract(&a, &c, &c);

  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(64bits integer) - X(real16) ==> X(real16)
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

  realIc_t a, c;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  realIcSubtract(&a, &c, &c);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
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

  convertRegister16To34(REGISTER_Y);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
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

  convertRegister16To34(REGISTER_X);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
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
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) - X(angle34) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16An34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16An34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&a, AM_DMS, AM_DEGREE);
    convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(angle34) - X(real16) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Re16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Re16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
    convertAngleIcFromTo(&c, AM_DMS, AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    reallocateRegister(REGISTER_X, dtAngle34, REAL34_SIZE, AM_DMS);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    reallocateRegister(REGISTER_X, dtAngle34, REAL34_SIZE, currentAngularMode);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }
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
 * \brief Y(complex16) - X(angle16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16An16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16An16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real16Subtract(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  complex16Copy(REGISTER_COMPLEX16_DATA(REGISTER_Y), REGISTER_COMPLEX16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle16) - X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Co16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Co16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real16Subtract(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X)); // real part
  real16ChangeSign(REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) - X(64bits integer) ==> X(complex16)
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

  realIc_t a, c;
  real16_t b;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16Copy(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);

  realIcSubtract(&a, &c, &c);

  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  real16Copy(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(64bits integer) - X(complex16) ==> X(complex16)
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

  realIc_t a, c;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  realIcSubtract(&a, &c, &c);

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
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
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
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



/********************************************//**
 * \brief Y(complex16) - X(angle34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16An34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16An34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle34) - X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Co16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Co16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* angle16 - ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(angle16) - X(angle16) ==> X(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16An16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16An16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);
  convertRegister16To34(REGISTER_X);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_Y), getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), AM_DEGREE);

    real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));

    convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, AM_DMS);
    convertRegister34To16(REGISTER_X);
    checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_Y), getRegisterAngularMode(REGISTER_Y), currentAngularMode);
    convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), currentAngularMode);

    real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
    convertRegister34To16(REGISTER_X);
  }
}



/********************************************//**
 * \brief Y(angle16) - X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16Time(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Time:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) - X(angle16) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subTimeAn16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(angle16) - X(date) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16Date(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Date:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(date) - X(angle16) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subDateAn16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(angle16) - X(64bits integer) ==> X(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16ShoI:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    reallocateRegister(REGISTER_X, dtAngle16, REAL16_SIZE, AM_DMS);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
    checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    reallocateRegister(REGISTER_X, dtAngle16, REAL16_SIZE, currentAngularMode);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(64bits integer) - X(angle16) ==> X(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoIAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subShoIAn16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&c, getRegisterTag(REGISTER_X), AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
    checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&c, getRegisterTag(REGISTER_X), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(angle16) - X(real34) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Re34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Re34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
    convertAngleIcFromTo(&c, AM_DMS, AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    setRegisterDataType(REGISTER_X, dtAngle34, AM_DMS);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    setRegisterDataType(REGISTER_X, dtAngle34, AM_DMS);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(real34) - X(angle16) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34An16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34An16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&a, AM_DMS, AM_DEGREE);
    convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    reallocateRegister(REGISTER_X, dtAngle34, REAL34_SIZE, AM_DMS);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    reallocateRegister(REGISTER_X, dtAngle34, REAL34_SIZE, currentAngularMode);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(angle16) - X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Co34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Co34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) - X(angle16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34An16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34An16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X)); // imaginary part
}



/********************************************//**
 * \brief Y(angle16) - X(angle34) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16An34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16An34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
    convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
    convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterAngularMode(REGISTER_X, currentAngularMode);
}



/********************************************//**
 * \brief Y(angle34) - X(angle16) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34An16:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34An16:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
    convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    reallocateRegister(REGISTER_X, dtAngle34, REAL34_SIZE, AM_DMS);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
    convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    reallocateRegister(REGISTER_X, dtAngle34, REAL34_SIZE, currentAngularMode);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }
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



/********************************************//**
 * \brief Y(time) - X(angle34) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subTimeAn34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(angle34) - X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34Time(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Time:", "cannot use NaN as Y input of -", NULL, NULL);
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



/********************************************//**
 * \brief Y(date) - X(angle34) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subDateAn34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(angle34) - X(date) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34Date(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Date:", "cannot use NaN as Y input of -", NULL, NULL);
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
 * \brief Y(64bits integer) - X(real34) ==> X(real34)
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

  convertShortIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) - X(64bits integer) ==> X(real34)
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

  convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(64bits integer) - X(complex34) ==> X(complex34)
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
 * \brief Y(complex34) - X(64bits integer) ==> X(complex34)
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
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(64bits integer) - X(angle34) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoIAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subShoIAn34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  convertAngleIcFromTo(&c, getRegisterTag(REGISTER_X), currentAngularMode);
  realIcSubtract(&a, &c, &c);

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, currentAngularMode);
}



/********************************************//**
 * \brief Y(angle34) - X(64bits integer) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34ShoI:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);

  convertAngleIcFromTo(&a, getRegisterTag(REGISTER_Y), currentAngularMode);
  realIcSubtract(&a, &c, &c);

  reallocateRegister(REGISTER_X, dtAngle34, REAL34_SIZE, currentAngularMode);
  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
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

  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
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
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) - X(angle34) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34An34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34An34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&a, AM_DMS, AM_DEGREE);
    convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(angle34) - X(real34) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Re34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Re34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
    convertAngleIcFromTo(&c, AM_DMS, AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    setRegisterDataType(REGISTER_X, dtAngle34, AM_DMS);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    setRegisterDataType(REGISTER_X, dtAngle34, AM_DMS);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }
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



/********************************************//**
 * \brief Y(complex34) - X(angle34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34An34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34An34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle34) - X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Co34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Co34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  real34Subtract(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* angle34 - ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(angle34) - X(angle34) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34An34:", "cannot use NaN as Y input of -", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34An34:", "cannot use NaN as X input of -", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

  if(currentAngularMode == AM_DMS) {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
    convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), AM_DEGREE);
    realIcSubtract(&a, &c, &c);
    convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
    convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), currentAngularMode);
    realIcSubtract(&a, &c, &c);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterAngularMode(REGISTER_X, currentAngularMode);
}
