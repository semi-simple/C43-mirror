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



void (* const addition[13][13])(void) = {
// regX |    regY ==>   1            2            3            4            5            6            7            8            9            10            11           12           13
//      V               Long integer Real16       Complex16    Angle16      Time         Date         String       Real16 mat   Complex16 m  Short integer Real34       Complex34    Angle34
/*  1 Long integer  */ {addLonILonI, addRe16LonI, addCo16LonI, addAn16LonI, addTimeLonI, addDateLonI, addStriLonI, addError,    addError,    addShoILonI,  addRe34LonI, addCo34LonI, addAn34LonI},
/*  2 Real16        */ {addLonIRe16, addRe16Re16, addCo16Re16, addAn16Re16, addTimeRe16, addDateRe16, addStriRe16, addError,    addError,    addShoIRe16,  addRe34Re16, addCo34Re16, addAn34Re16},
/*  3 Complex16     */ {addLonICo16, addRe16Co16, addCo16Co16, addAn16Co16, addError,    addError,    addStriCo16, addError,    addError,    addShoICo16,  addRe34Co16, addCo34Co16, addAn34Co16},
/*  4 Angle16       */ {addLonIAn16, addRe16An16, addCo16An16, addAn16An16, addTimeAn16, addDateAn16, addStriAn16, addError,    addError,    addShoIAn16,  addRe34An16, addCo34An16, addAn34An16},
/*  5 Time          */ {addLonITime, addRe16Time, addError,    addAn16Time, addTimeTime, addError,    addStriTime, addError,    addError,    addError,     addRe34Time, addError,    addAn34Time},
/*  6 Date          */ {addLonIDate, addRe16Date, addError,    addAn16Date, addError,    addError,    addStriDate, addError,    addError,    addError,     addRe34Date, addError,    addAn34Date},
/*  7 String        */ {addError,    addError,    addError,    addError,    addError,    addError,    addStriStri, addError,    addError,    addError,     addError,    addError,    addError   },
/*  8 Real16 mat    */ {addError,    addError,    addError,    addError,    addError,    addError,    addStriRm16, addRm16Rm16, addCm16Rm16, addError,     addError,    addError,    addError   },
/*  9 Complex16 mat */ {addError,    addError,    addError,    addError,    addError,    addError,    addStriCm16, addRm16Cm16, addCm16Cm16, addError,     addError,    addError,    addError   },
/* 10 Short integer */ {addLonIShoI, addRe16ShoI, addCo16ShoI, addAn16ShoI, addError,    addError,    addStriShoI, addError,    addError,    addShoIShoI,  addRe34ShoI, addCo34ShoI, addAn34ShoI},
/* 11 Real34        */ {addLonIRe34, addRe16Re34, addCo16Re34, addAn16Re34, addTimeRe34, addDateRe34, addStriRe34, addError,    addError,    addShoIRe34,  addRe34Re34, addCo34Re34, addAn34Re34},
/* 12 Complex34     */ {addLonICo34, addRe16Co34, addCo16Co34, addAn16Co34, addError,    addError,    addStriCo34, addError,    addError,    addShoICo34,  addRe34Co34, addCo34Co34, addAn34Co34},
/* 13 Angle34       */ {addLonIAn34, addRe16An34, addCo16An34, addAn16An34, addTimeAn34, addDateAn34, addStriAn34, addError,    addError,    addShoIAn34,  addRe34An34, addCo34An34, addAn34An34}
};



/********************************************//**
 * \brief Data type error in addition
 *
 * \param void
 * \return void
 ***********************************************/
void addError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
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

  result = REGISTER_X;
  opY    = allocateTemporaryRegister();
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_Y, opY);
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(result, true, true, opX, opY, -1);
}



/******************************************************************************************************************************************************************************************/
/* long integer + ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(long integer) + opX(long integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonILonI(void) {
  longInteger_t iOp1, iOp2;

  convertLongIntegerRegisterToLongInteger(opY, iOp1);
  convertLongIntegerRegisterToLongInteger(opX, iOp2);

  longIntegerAdd(iOp1, iOp2, iOp1);

  convertLongIntegerToLongIntegerRegister(iOp1, result);

  longIntegerFree(iOp2);
  longIntegerFree(iOp1);
}



/********************************************//**
 * \brief opY(long integer) + opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addLonIRe16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal16Register(opY, opY);
  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(real16) + opX(long integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(long integer) + opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addLonICo16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  convertLongIntegerRegisterToReal16Register(opY, opY);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex16) + opX(long integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  convertLongIntegerRegisterToReal16Register(opX, opX);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(long integer) + opX(angle16) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addLonIAn16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterTag(opX), AM_DEGREE);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    convertRegister34To16(result);
    checkDms16(REGISTER_REAL16_DATA(result));
  }
  else {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterTag(opX), currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(angle16) + opX(long integer) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn16LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), AM_DEGREE);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    convertRegister34To16(result);
    checkDms16(REGISTER_REAL16_DATA(result));
  }
  else {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(long integer) + opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonITime(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(time) + opX(long integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(long integer) + opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIDate(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(date) + opX(long integer) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(long integer) + opX(64bits integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIShoI(void) {
  longInteger_t iOp1, iOp2;

  convertShortIntegerRegisterLongIntegerRegister(opX, opX);

  convertLongIntegerRegisterToLongInteger(opY, iOp1);
  convertLongIntegerRegisterToLongInteger(opX, iOp2);

  longIntegerAdd(iOp1, iOp2, iOp1);

  convertLongIntegerToLongIntegerRegister(iOp1, result);

  longIntegerFree(iOp2);
  longIntegerFree(iOp1);
}



/********************************************//**
 * \brief opY(64bits integer) + opX(long integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoILonI(void) {
  longInteger_t iOp1, iOp2;

  convertShortIntegerRegisterLongIntegerRegister(opY, opY);

  convertLongIntegerRegisterToLongInteger(opY, iOp1);
  convertLongIntegerRegisterToLongInteger(opX, iOp2);

  longIntegerAdd(iOp1, iOp2, iOp1);

  convertLongIntegerToLongIntegerRegister(iOp1, result);

  longIntegerFree(iOp2);
  longIntegerFree(iOp1);
}



/********************************************//**
 * \brief opY(long integer) + opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addLonIRe34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) + opX(long integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(long integer) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addLonICo34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex34) + opX(long integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(long integer) + opX(angle34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addLonIAn34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterTag(opX), AM_DEGREE);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterTag(opX), currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle34) + opX(long integer) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn34LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), AM_DEGREE);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/******************************************************************************************************************************************************************************************/
/* real16 + ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(real16) + opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Re16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(real16) + opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Co16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex16) + opX(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16Re16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(real16) + opX(angle16) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16An16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  convertRegister16To34(opX);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), AM_DMS, AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, AM_DMS);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    convertRegister34To16(result);
    checkDms16(REGISTER_REAL16_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(angle16) + opX(real16) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn16Re16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  convertRegister16To34(opX);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), AM_DMS, AM_DEGREE);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, AM_DMS);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    convertRegister34To16(result);
    checkDms16(REGISTER_REAL16_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(real16) + opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Time(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Time:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(time) + opX(real16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addTimeRe16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real16) + opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Date(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Date:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(date) + opX(real16) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addDateRe16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real16) + opX(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16ShoI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(64bits integer) + opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addShoIRe16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opY, opY);
  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(real16) + opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) + opX(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Re16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);

  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real16) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Co34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex34) + opX(real16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34Re16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(real16) + opX(angle34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16An34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), AM_DMS, AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, AM_DMS);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle34) + opX(real16) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn34Re16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), AM_DMS, AM_DEGREE);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, AM_DMS);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/******************************************************************************************************************************************************************************************/
/* complex16 + ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(complex16) + opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16Co16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Add(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex16) + opX(angle16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16An16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(angle16) + opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn16Co16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex16) + opX(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16ShoI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(64bits integer) + opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addShoICo16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opY, opY);
  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex16) + opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(real34) + opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Co16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex16) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16Co34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Add(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex34) + opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34Co16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Add(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex16) + opX(angle34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16An34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(angle34) + opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn34Co16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/******************************************************************************************************************************************************************************************/
/* angle16 + ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(angle16) + opX(angle16) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn16An16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  convertRegister16To34(opX);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, AM_DMS);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    convertRegister34To16(result);
    checkDms16(REGISTER_REAL16_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(angle16) + opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn16Time(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn16Time:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(time) + opX(angle16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addTimeAn16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(angle16) + opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn16Date(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn16Date:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(date) + opX(angle16) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addDateAn16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(angle16) + opX(64bits integer) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn16ShoI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  convertShortIntegerRegisterToReal34Register(opX, opX);
  convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  convertRegister34To16(result);
}



/********************************************//**
 * \brief opY(64bits integer) + opX(angle16) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoIAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addShoIAn16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opY, opY);
  convertRegister16To34(opX);
  convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  convertRegister34To16(result);
}



/********************************************//**
 * \brief opY(angle16) + opX(real34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn16Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), AM_DMS, AM_DEGREE);

    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(real34) + opX(angle16) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34An16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), AM_DMS, AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, AM_DMS);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);

    reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle16) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn16Co34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  convertRegister16To34(opY);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex34) + opX(angle16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34An16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  convertRegister16To34(opX);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(angle16) + opX(angle34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn16An34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);

    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle34) + opX(angle16) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn34An16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);

    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/******************************************************************************************************************************************************************************************/
/* time + ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(time) + opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeTime(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(time) + opX(real34) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addTimeRe34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real34) + opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Time(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Time:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(time) + opX(angle34) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addTimeAn3434:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(angle34) + opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn34Time(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn34Time:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* date + ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(date) + opX(real34) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addDateRe34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real34) + opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Date(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Date:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(date) + opX(angle34) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addDateAn34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(angle34) + opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn34Date(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn34Date:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* string + ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(string) + opX(long integer) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriLonI(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  longIntegerToDisplayString(opX, tmpStr3000, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, TAG_NONE);

  memcpy(REGISTER_STRING_DATA(result),        REGISTER_STRING_DATA(opY), len1    );
  memcpy(REGISTER_STRING_DATA(result) + len1, tmpStr3000,                len2 + 1);
}



/********************************************//**
 * \brief opY(string) + opX(real16) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriRe16(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  realToDisplayString(REGISTER_REAL16_DATA(opX), false, getRegisterTag(opX), tmpStr3000, &standardFont, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, TAG_NONE);

  memcpy(REGISTER_STRING_DATA(result),        REGISTER_STRING_DATA(opY), len1    );
  memcpy(REGISTER_STRING_DATA(result) + len1, tmpStr3000,                len2 + 1);
}



/********************************************//**
 * \brief opY(string) + opX(complex16) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriCo16(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  complexToDisplayString(REGISTER_COMPLEX16_DATA(opX), false, tmpStr3000, &numericFont, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, TAG_NONE);

  memcpy(REGISTER_STRING_DATA(result)       , REGISTER_STRING_DATA(opY), len1    );
  memcpy(REGISTER_STRING_DATA(result) + len1, tmpStr3000,                len2 + 1);
}



/********************************************//**
 * \brief opY(string) + opX(angle16) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriAn16(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  angle16ToDisplayString(REGISTER_REAL16_DATA(opX), getRegisterTag(opX), tmpStr3000, &standardFont, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, TAG_NONE);

  memcpy(REGISTER_STRING_DATA(result),        REGISTER_STRING_DATA(opY), len1    );
  memcpy(REGISTER_STRING_DATA(result) + len1, tmpStr3000,                len2 + 1);
}



/********************************************//**
 * \brief opY(string) + opX(time) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriTime(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  timeToDisplayString(opX, tmpStr3000);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, TAG_NONE);

  memcpy(REGISTER_STRING_DATA(result)       , REGISTER_STRING_DATA(opY), len1    );
  memcpy(REGISTER_STRING_DATA(result) + len1, tmpStr3000,                len2 + 1);
}



/********************************************//**
 * \brief opY(string) + opX(date) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriDate(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  dateToDisplayString(opX, tmpStr3000);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, TAG_NONE);

  memcpy(REGISTER_STRING_DATA(result)       , REGISTER_STRING_DATA(opY), len1    );
  memcpy(REGISTER_STRING_DATA(result) + len1, tmpStr3000,                len2 + 1);
}



/********************************************//**
 * \brief opY(string) + opX(string) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriStri(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  len2 = stringByteLength(REGISTER_STRING_DATA(opX));

  reallocateRegister(result, dtString, len1 + len2, TAG_NONE);

  memcpy(REGISTER_STRING_DATA(result)       , REGISTER_STRING_DATA(opY), len1    );
  memcpy(REGISTER_STRING_DATA(result) + len1, REGISTER_STRING_DATA(opX), len2 + 1);
}



/********************************************//**
 * \brief opY(string) + opX(real16 matrix) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriRm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(string) + opX(complex16 matrix) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriCm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(string) + opX(64bits integer) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriShoI(void) {
  int16_t len1, len2;
  const font_t *font;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  font = &standardFont;
  shortIntegerToDisplayString(opX, errorMessage, &font); // We use errorMessage here because this string can become very long
  len2 = stringByteLength(errorMessage) + 1; // +1 for the trailing 0

  reallocateRegister(result, dtString, len1 + len2, TAG_NONE);

  memcpy(REGISTER_STRING_DATA(result)       , REGISTER_STRING_DATA(opY), len1    );
  memcpy(REGISTER_STRING_DATA(result) + len1, errorMessage,              len2 + 1);
}



/********************************************//**
 * \brief opY(string) + opX(real34) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriRe34(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  realToDisplayString(REGISTER_REAL34_DATA(opX), true, getRegisterTag(opX), tmpStr3000, &standardFont, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, TAG_NONE);

  memcpy(REGISTER_STRING_DATA(result),        REGISTER_STRING_DATA(opY), len1    );
  memcpy(REGISTER_STRING_DATA(result) + len1, tmpStr3000,                len2 + 1);
}



/********************************************//**
 * \brief opY(string) + opX(complex34) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriCo34(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  complexToDisplayString(REGISTER_COMPLEX34_DATA(opX), true, tmpStr3000, &numericFont, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, TAG_NONE);

  memcpy(REGISTER_STRING_DATA(result)       , REGISTER_STRING_DATA(opY), len1    );
  memcpy(REGISTER_STRING_DATA(result) + len1, tmpStr3000,                len2 + 1);
}



/********************************************//**
 * \brief opY(string) + opX(angle34) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriAn34(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  angle34ToDisplayString(REGISTER_REAL34_DATA(opX), getRegisterTag(opX), tmpStr3000, &standardFont, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, TAG_NONE);

  memcpy(REGISTER_STRING_DATA(result),        REGISTER_STRING_DATA(opY), len1    );
  memcpy(REGISTER_STRING_DATA(result) + len1, tmpStr3000,                len2 + 1);
}



/******************************************************************************************************************************************************************************************/
/* real16 matrix + ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(real16 matrix) + opX(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addRm16Rm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) + opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addRm16Cm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) + opX(real16 matrix) ==> result(complex16 matrix)
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
 * \brief opY(complex16 matrix) + opX(complex16 matrix) ==> result(complex16 matrix)
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
 * \brief opY(64bits integer) + opX(64bits integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoIShoI(void) {
  setRegisterTag(result, getRegisterTag(opY));
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_intAdd(*(REGISTER_SHORT_INTEGER_DATA(opY)), *(REGISTER_SHORT_INTEGER_DATA(opX)));
}



/********************************************//**
 * \brief opY(64bits integer) + opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addShoIRe34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) + opX(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34ShoI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(64bits integer) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addShoICo34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex34) + opX(64bits integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34ShoI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(64bits integer) + opX(angle34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoIAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addShoIAn34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opY, opY);

  convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterTag(opX), currentAngularMode);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(angle34) + opX(64bits integer) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn34ShoI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opX, opX);

  convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), currentAngularMode);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/******************************************************************************************************************************************************************************************/
/* real34 + ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(real34) + opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Co34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex34) + opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(real34) + opX(angle34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34An34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), AM_DMS, AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);

    setRegisterTag(result, AM_DMS);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);

    setRegisterTag(result, currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle34) + opX(real34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn34Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  setRegisterDataType(result, dtAngle34, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), AM_DMS, AM_DEGREE);

    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/******************************************************************************************************************************************************************************************/
/* complex34 + ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(complex34) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Add(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex34) + opX(angle34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34An34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(angle34) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn34Co34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/******************************************************************************************************************************************************************************************/
/* angle34 + ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(angle34) + opX(angle34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void addAn34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAn34An34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  if(currentAngularMode == AM_DMS) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), AM_DEGREE);

    setRegisterTag(result, AM_DMS);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
    checkDms34(REGISTER_REAL34_DATA(result));
  }
  else { //current angular mode is not DMS
    convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), currentAngularMode);
    convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), currentAngularMode);

    setRegisterTag(result, currentAngularMode);
    real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}
