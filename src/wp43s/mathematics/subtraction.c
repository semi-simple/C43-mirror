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

  result = REGISTER_X;
  opY    = allocateTemporaryRegister();
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_Y, opY);
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(result, true, true, opX, opY, -1);
}



/******************************************************************************************************************************************************************************************/
/* long integer - ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(long integer) - opX(long integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonILonI(void) {
  longInteger_t iOp1, iOp2;

  convertLongIntegerRegisterToLongInteger(opY, iOp1);
  convertLongIntegerRegisterToLongInteger(opX, iOp2);

  longIntegerSubtract(iOp1, iOp2, iOp1);

  convertLongIntegerToLongIntegerRegister(iOp1, result);

  longIntegerFree(iOp2);
  longIntegerFree(iOp1);
}



/********************************************//**
 * \brief opY(long integer) - opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonIRe16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal16Register(opY, opY);
  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(real16) - opX(long integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(long integer) - opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonICo16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  convertLongIntegerRegisterToReal16Register(opY, opY);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
  real16ChangeSign(REGISTER_IMAG16_DATA(result));
}



/********************************************//**
 * \brief opY(complex16) - opX(long integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  convertLongIntegerRegisterToReal16Register(opX, opX);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(long integer) - opX(angle16) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonIAn16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterTag(opX), AM_DEGREE);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    convertRegister34To16(result);
    checkDms16(REGISTER_REAL16_DATA(result));
  }
  else {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterTag(opX), currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(angle16) - opX(long integer) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), AM_DEGREE);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    convertRegister34To16(result);
    checkDms16(REGISTER_REAL16_DATA(result));
  }
  else {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(long integer) - opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonITime(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(time) - opX(long integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(long integer) - opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIDate(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(date) - opX(long integer) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(long integer) - opX(64bits integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIShoI(void) {
  longInteger_t iOp1, iOp2;

  convertShortIntegerRegisterLongIntegerRegister(opX, opX);

  convertLongIntegerRegisterToLongInteger(opY, iOp1);
  convertLongIntegerRegisterToLongInteger(opX, iOp2);

  longIntegerSubtract(iOp1, iOp2, iOp1);

  convertLongIntegerToLongIntegerRegister(iOp1, result);

  longIntegerFree(iOp2);
  longIntegerFree(iOp1);
}



/********************************************//**
 * \brief opY(64bits integer) - opX(long integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoILonI(void) {
  longInteger_t iOp1, iOp2;

  convertShortIntegerRegisterLongIntegerRegister(opY, opY);

  convertLongIntegerRegisterToLongInteger(opY, iOp1);
  convertLongIntegerRegisterToLongInteger(opX, iOp2);

  longIntegerSubtract(iOp1, iOp2, iOp1);

  convertLongIntegerToLongIntegerRegister(iOp1, result);

  longIntegerFree(iOp2);
  longIntegerFree(iOp1);
}



/********************************************//**
 * \brief opY(long integer) - opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonIRe34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) - opX(long integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(long integer) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonICo34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
  real34ChangeSign(REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(complex34) - opX(long integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(long integer) - opX(angle34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonIAn34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterTag(opX), AM_DEGREE);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterTag(opX), currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle34) - opX(long integer) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), AM_DEGREE);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/******************************************************************************************************************************************************************************************/
/* real16 - ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(real16) - opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Re16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(real16) - opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Co16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
  real16ChangeSign(REGISTER_IMAG16_DATA(result));
}



/********************************************//**
 * \brief opY(complex16) - opX(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16Re16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(real16) - opX(angle16) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16An16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  convertRegister16To34(opX);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), AM_DMS, AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, AM_DMS);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    convertRegister34To16(result);
    checkDms16(REGISTER_REAL16_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(angle16) - opX(real16) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Re16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  convertRegister16To34(opX);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), AM_DMS, AM_DEGREE);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, AM_DMS);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    convertRegister34To16(result);
    checkDms16(REGISTER_REAL16_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(real16) - opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Time(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Time:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(time) - opX(real16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subTimeRe16:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real16) - opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Date(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Date:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(date) - opX(real16) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subDateRe16:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real16) - opX(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16ShoI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(64bits integer) - opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subShoIRe16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opY, opY);
  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(real16) - opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) - opX(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Re16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);

  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real16) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Co34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
  real34ChangeSign(REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(complex34) - opX(real16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34Re16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(real16) - opX(angle34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16An34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), AM_DMS, AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, AM_DMS);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle34) - opX(real16) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Re16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), AM_DMS, AM_DEGREE);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, AM_DMS);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/******************************************************************************************************************************************************************************************/
/* complex16 - ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(complex16) - opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16Co16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Subtract(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex16) - opX(angle16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16An16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(angle16) - opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Co16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16ChangeSign(REGISTER_IMAG16_DATA(opX));
  real16Copy(REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex16) - opX(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16ShoI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(64bits integer) - opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subShoICo16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opY, opY);
  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
  real16ChangeSign(REGISTER_IMAG16_DATA(result));
}



/********************************************//**
 * \brief opY(complex16) - opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(real34) - opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Co16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
  real34ChangeSign(REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(complex16) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16Co34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Subtract(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex34) - opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34Co16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Subtract(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex16) - opX(angle34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16An34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(angle34) - opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Co16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(opX));
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/******************************************************************************************************************************************************************************************/
/* angle16 - ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(angle16) - opX(angle16) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16An16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);

    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    convertRegister34To16(result);
    checkDms16(REGISTER_REAL16_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);

    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(angle16) - opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16Time(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Time:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(time) - opX(angle16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subTimeAn16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(angle16) - opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16Date(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Date:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(date) - opX(angle16) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subDateAn16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(angle16) - opX(64bits integer) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16ShoI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  convertShortIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    convertRegister34To16(result);
    checkDms16(REGISTER_REAL16_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(64bits integer) - opX(angle16) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoIAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subShoIAn16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opY, opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    convertRegister34To16(result);
    checkDms16(REGISTER_REAL16_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(angle16) - opX(real34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), AM_DMS, AM_DEGREE);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(real34) - opX(angle16) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34An16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), AM_DMS, AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle16) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16Co34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(opX));
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex34) - opX(angle16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34An16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(angle16) - opX(angle34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn16An34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle34) - opX(angle16) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34An16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/******************************************************************************************************************************************************************************************/
/* time - ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(time) - opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeTime(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(time) - opX(real34) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subTimeRe34:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real34) - opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Time(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Time:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(time) - opX(angle34) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subTimeAn34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(angle34) - opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34Time(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Time:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* date - ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(date) - opX(date) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateDate(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(date) - opX(real34) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subDateRe34:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real34) - opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Date(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Date:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(date) - opX(angle34) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subDateAn34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(angle34) - opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34Date(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Date:", "cannot use NaN as an input of +", NULL, NULL);
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
 * \brief opY(real16 matrix) - opX(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subRm16Rm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) - opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subRm16Cm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) - opX(real16 matrix) ==> result(complex16 matrix)
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
 * \brief opY(complex16 matrix) - opX(complex16 matrix) ==> result(complex16 matrix)
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
 * \brief opY(Short integer) - opX(Short integer) ==> result(Short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoIShoI(void) {
  reallocateRegister(result, dtShortInteger, SHORT_INTEGER_SIZE, getRegisterTag(opY));
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_intSubtract(*(REGISTER_SHORT_INTEGER_DATA(opY)), *(REGISTER_SHORT_INTEGER_DATA(opX)));
}



/********************************************//**
 * \brief opY(64bits integer) - opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subShoIRe34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) - opX(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34ShoI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(64bits integer) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subShoICo34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
  real34ChangeSign(REGISTER_IMAG34_DATA(result));
}




/********************************************//**
 * \brief opY(complex34) - opX(64bits integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34ShoI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(64bits integer) - opX(angle34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoIAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subShoIAn34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opY, opY);

  convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterTag(opX), currentAngularMode);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(angle34) - opX(64bits integer) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34ShoI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opX, opX);

  convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), currentAngularMode);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/******************************************************************************************************************************************************************************************/
/* real34 - ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(real34) - opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Co34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
  real34ChangeSign(REGISTER_IMAG34_DATA(result));
}



/********************************************//**
 * \brief opY(complex34) - opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(real34) - opX(angle34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34An34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  setRegisterAngularMode(result, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), AM_DMS, AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle34) - opX(real34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  setRegisterDataType(result, dtAngle34, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), AM_DMS, AM_DEGREE);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/******************************************************************************************************************************************************************************************/
/* complex34 - ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(complex34) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34Co34:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Subtract(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex34) - opX(angle34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34An34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(angle34) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34Co34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34ChangeSign(REGISTER_IMAG34_DATA(opX));
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/******************************************************************************************************************************************************************************************/
/* angle34 - ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(angle34) - opX(angle34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void subAn34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAn34An34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);

    setRegisterTag(result, AM_DMS);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);

    setRegisterTag(result, currentAngularMode);
    real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}
