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
// regX |    regY ==>    1            2            3            4            5            6            7            8            9            10            11           12
//      V                Long integer real16       complex16    angle34      Time         Date         String       real16 mat   complex16 m  Short integer real34       complex34
/*  1 Long integer   */ {addLonILonI, addRe16LonI, addCo16LonI, addAnglLonI, addTimeLonI, addDateLonI, addStriLonI, addError,    addError,    addShoILonI,  addRe34LonI, addCo34LonI},
/*  2 real16         */ {addLonIRe16, addRe16Re16, addCo16Re16, addAnglRe16, addTimeRe16, addDateRe16, addStriRe16, addError,    addError,    addShoIRe16,  addRe34Re16, addCo34Re16},
/*  3 complex16      */ {addLonICo16, addRe16Co16, addCo16Co16, addError,    addError,    addError,    addStriCo16, addError,    addError,    addShoICo16,  addRe34Co16, addCo34Co16},
/*  4 angle34        */ {addLonIAngl, addRe16Angl, addError,    addAnglAngl, addError,    addError,    addStriAngl, addError,    addError,    addShoIAngl,  addRe34Angl, addError   },
/*  5 Time           */ {addLonITime, addRe16Time, addError,    addError,    addTimeTime, addError,    addStriTime, addError,    addError,    addError,     addRe34Time, addError   },
/*  6 Date           */ {addLonIDate, addRe16Date, addError,    addError,    addError,    addError,    addStriDate, addError,    addError,    addError,     addRe34Date, addError   },
/*  7 String         */ {addError,    addError,    addError,    addError,    addError,    addError,    addStriStri, addError,    addError,    addError,     addError,    addError   },
/*  8 real16 mat     */ {addError,    addError,    addError,    addError,    addError,    addError,    addStriRm16, addRm16Rm16, addCm16Rm16, addError,     addError,    addError   },
/*  9 complex16 mat  */ {addError,    addError,    addError,    addError,    addError,    addError,    addStriCm16, addRm16Cm16, addCm16Cm16, addError,     addError,    addError   },
/* 10 Short integer  */ {addLonIShoI, addRe16ShoI, addCo16ShoI, addAnglShoI, addError,    addError,    addStriShoI, addError,    addError,    addShoIShoI,  addRe34ShoI, addCo34ShoI},
/* 11 real34         */ {addLonIRe34, addRe16Re34, addCo16Re34, addAnglRe34, addTimeRe34, addDateRe34, addStriRe34, addError,    addError,    addShoIRe34,  addRe34Re34, addCo34Re34},
/* 12 complex34      */ {addLonICo34, addRe16Co34, addCo16Co34, addError,    addError,    addError,    addStriCo34, addError,    addError,    addShoICo34,  addRe34Co34, addCo34Co34}
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



/********************************************//**
 * \brief opY(long integer) + opX(long integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonILonI(void) {
  longInteger_t iOp1;
  longInteger_t iOp2;

  convertLongIntegerRegisterToLongInteger(opY, &iOp1);
  convertLongIntegerRegisterToLongInteger(opX, &iOp2);

  longIntegerAdd(&iOp1, &iOp2, &iOp1);

  convertLongIntegerToLongIntegerRegister(&iOp1, result);
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
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
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

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opX), REGISTER_COMPLEX16_DATA(result)); // result = opX
  convertLongIntegerRegisterToReal16Register(opY, opY);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(long integer) + opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIAngl(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addLonIAngl:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertLongIntegerRegisterToAngleRegister(opY, opY);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opY), angularMode);
  angleAdd(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
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
 * \brief opY(long integer) + opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIDate(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(long integer) + opX(64bits integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIShoI(void) {
  convertShortIntegerRegisterLongIntegerRegister(opX, opX);
  addLonILonI();
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
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
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

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(opX), REGISTER_COMPLEX34_DATA(result)); // result = opX
  convertLongIntegerRegisterToReal34Register(opY, opY);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real16) + opX(long integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



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

  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
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

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opX), REGISTER_COMPLEX16_DATA(result)); // result = opX
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
}



/********************************************//**
 * \brief opY(real16) + opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Angl(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Angl:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE34 == 1)
    convertRegister16To34(opY)
  #endif
  convertAngleToInternal(REGISTER_ANGLE_DATA(opY), angularMode);
  angleAdd(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
}



/********************************************//**
 * \brief opY(real16) + opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Time(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Time:", "cannot use NaN as an input of +", NULL, NULL);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Date:", "cannot use NaN as an input of +", NULL, NULL);
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
  convertShortIntegerRegisterToReal16Register(opX, opX);
  addRe16Re16();
}



/********************************************//**
 * \brief opY(real16) + opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Re34(void) {
  convertRegister16To34(opY);
  addRe34Re34();
}



/********************************************//**
 * \brief opY(real16) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Co34(void) {
  convertRegister16To34(opY);
  addRe34Co34();
}



/********************************************//**
 * \brief opY(complex16) + opX(long integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opY), REGISTER_COMPLEX16_DATA(result)); // result = opX
  convertLongIntegerRegisterToReal16Register(opX, opX);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
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

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(result)); // imaginary part
}



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

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Add(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex16) + opX(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16ShoI(void) {
  convertShortIntegerRegisterToReal16Register(opX, opX);
  addCo16Re16();
}



/********************************************//**
 * \brief opY(complex16) + opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Re34(void) {
  convertRegister16To34(opY);
  addCo34Re34();
}



/********************************************//**
 * \brief opY(complex16) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Co34(void) {
  convertRegister16To34(opY);
  addCo34Co34();
}



/********************************************//**
 * \brief opY(angle) + opX(long integer) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addAnglLonI(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAnglLonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertLongIntegerRegisterToAngleRegister(opX, opX);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opX), angularMode);
  angleAdd(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
}



/********************************************//**
 * \brief opY(angle) + opX(real16) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addAnglRe16(void) {
  if(real16IsNaN(REGISTER_ANGLE_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAnglRe16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE34 == 1)
    convertRegister16To34(opX)
  #endif
  convertAngleToInternal(REGISTER_ANGLE_DATA(opX), angularMode);
  angleAdd(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
}



/********************************************//**
 * \brief opY(angle) + opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addAnglAngl(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY)) || angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAnglAngl:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  angleAdd(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
}



/********************************************//**
 * \brief opY(angle) + opX(short integer) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addAnglShoI(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAnglShoI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertShortIntegerRegisterToAngleRegister(opX, opX);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opX), angularMode);
  angleAdd(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
}



/********************************************//**
 * \brief opY(angle) + opX(real34) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addAnglRe34(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAnglRe34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE16 == 1)
    convertRegister34To16(opX);
  #endif
  convertAngleToInternal(REGISTER_ANGLE_DATA(opX), angularMode);
  angleAdd(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
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
 * \brief opY(date) + opX(long integer) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateLonI(void) {
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
 * \brief opY(string) + opX(long integer) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriLonI(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  longIntegerToDisplayString(opX, tmpStr3000);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, 0);

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
  realToDisplayString(REGISTER_REAL16_DATA(opX), false, tmpStr3000, &numericFont, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, 0);

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

  reallocateRegister(result, dtString, len1 + len2, 0);

  memcpy(REGISTER_STRING_DATA(result)       , REGISTER_STRING_DATA(opY), len1    );
  memcpy(REGISTER_STRING_DATA(result) + len1, tmpStr3000,                len2 + 1);
}



/********************************************//**
 * \brief opY(string) + opX(angle) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriAngl(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  registerAngleToDisplayString(opX, tmpStr3000, &numericFont, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, 0);

  memcpy(REGISTER_STRING_DATA(result)       , REGISTER_STRING_DATA(opY), len1    );
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

  reallocateRegister(result, dtString, len1 + len2, 0);

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

  reallocateRegister(result, dtString, len1 + len2, 0);

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

  reallocateRegister(result, dtString, len1 + len2, 0);

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
  shortIntegerToDisplayString(opX, errorMessage, &font); // We use errorMeaage here because this string can become very long
  len2 = stringByteLength(errorMessage) + 1; // +1 for the trailing 0

  reallocateRegister(result, dtString, len1 + len2, 0);

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
  realToDisplayString(REGISTER_REAL34_DATA(opX), true, tmpStr3000, &numericFont, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, 0);

  memcpy(REGISTER_STRING_DATA(result)       , REGISTER_STRING_DATA(opY), len1    );
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

  reallocateRegister(result, dtString, len1 + len2, 0);

  memcpy(REGISTER_STRING_DATA(result)       , REGISTER_STRING_DATA(opY), len1    );
  memcpy(REGISTER_STRING_DATA(result) + len1, tmpStr3000,                len2 + 1);
}



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



/********************************************//**
 * \brief opY(complex16 matrix) + opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addCm16Cm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(64bits integer) + opX(long integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoILonI(void) {
  convertShortIntegerRegisterLongIntegerRegister(opY, opY);
  addLonILonI();
}



/********************************************//**
 * \brief opY(64bits integer) + opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoIRe16(void) {
  convertShortIntegerRegisterToReal16Register(opY, opY);
  addRe16Re16();
}



/********************************************//**
 * \brief opY(64bits integer) + opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoICo16(void) {
  convertShortIntegerRegisterToReal16Register(opY, opY);
  addRe16Co16();
}



/********************************************//**
 * \brief opY(short integer) + opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoIAngl(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addShoIAngl:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertShortIntegerRegisterToAngleRegister(opY, opY);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opY), angularMode);
  angleAdd(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
}



/********************************************//**
 * \brief opY(64bits integer) + opX(64bits integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoIShoI(void) {
  reallocateRegister(result, dtShortInteger, SHORT_INTEGER_SIZE, getRegisterBase(opY));
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_intAdd(*(REGISTER_SHORT_INTEGER_DATA(opY)), *(REGISTER_SHORT_INTEGER_DATA(opX)));
}



/********************************************//**
 * \brief opY(64bits integer) + opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoIRe34(void) {
  convertShortIntegerRegisterToReal34Register(opY, opY);
  addRe34Re34();
}



/********************************************//**
 * \brief opY(64bits integer) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoICo34(void) {
  convertShortIntegerRegisterToReal34Register(opY, opY);
  addRe34Co34();
}



/********************************************//**
 * \brief opY(real34) + opX(long integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) + opX(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Re16(void) {
  convertRegister16To34(opX);
  addRe34Re34();
}



/********************************************//**
 * \brief opY(real34) + opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Co16(void) {
  convertRegister16To34(opX);
  addRe34Co34();
}



/********************************************//**
 * \brief opY(real34) + opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Angl(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Angl:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE16 == 1)
    convertRegister34To16(opY);
  #endif
  convertAngleToInternal(REGISTER_ANGLE_DATA(opY), angularMode);
  angleAdd(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
}



/********************************************//**
 * \brief opY(real34) + opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Time(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Time:", "cannot use NaN as an input of +", NULL, NULL);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Date:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real34) + opX(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34ShoI(void) {
  convertShortIntegerRegisterToReal34Register(opX, opX);
  addRe34Re34();
}



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

  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
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

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34LonI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(opY), REGISTER_COMPLEX34_DATA(result)); // result = opY
  convertLongIntegerRegisterToReal34Register(opX, opX);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(complex34) + opX(real16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Re16(void) {
  convertRegister16To34(opX);
  addCo34Re34();
}



/********************************************//**
 * \brief opY(complex34) + opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Co16(void) {
  convertRegister16To34(opX);
  addCo34Co34();
}



/********************************************//**
 * \brief opY(complex34) + opX(64bits integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34ShoI(void) {
  convertShortIntegerRegisterToReal34Register(opX, opX);
  addCo34Re34();
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

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



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

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)               ); // real part
  real34Add(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}
