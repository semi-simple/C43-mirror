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
// regX |    regY ==>    1            2            3            4            5            6            7            8            9            10            11           12
//      V                Long integer real16       complex16    angle34      Time         Date         String       real16 mat   complex16 m  Short integer real34       complex34
/*  1 Long integer   */ {subLonILonI, subRe16LonI, subCo16LonI, subAnglLonI, subTimeLonI, subDateLonI, subError,    subError,    subError,    subShoILonI,  subRe34LonI, subCo34LonI},
/*  2 real16         */ {subLonIRe16, subRe16Re16, subCo16Re16, subAnglRe16, subTimeRe16, subDateRe16, subError,    subError,    subError,    subShoIRe16,  subRe34Re16, subCo34Re16},
/*  3 complex16      */ {subLonICo16, subRe16Co16, subCo16Co16, subError,    subError,    subError,    subError,    subError,    subError,    subShoICo16,  subRe34Co16, subCo34Co16},
/*  4 angle34        */ {subLonIAngl, subRe16Angl, subError,    subAnglAngl, subError,    subError,    subError,    subError,    subError,    subShoIAngl,  subRe34Angl, subError   },
/*  5 Time           */ {subLonITime, subRe16Time, subError,    subError,    subTimeTime, subError,    subError,    subError,    subError,    subError,     subRe34Time, subError   },
/*  6 Date           */ {subLonIDate, subRe16Date, subError,    subError,    subError,    subDateDate, subError,    subError,    subError,    subError,     subRe34Date, subError   },
/*  7 String         */ {subError,    subError,    subError,    subError,    subError,    subError,    subError,    subError,    subError,    subError,     subError,    subError   },
/*  8 real16 mat     */ {subError,    subError,    subError,    subError,    subError,    subError,    subError,    subRm16Rm16, subCm16Rm16, subError,     subError,    subError   },
/*  9 complex16 mat  */ {subError,    subError,    subError,    subError,    subError,    subError,    subError,    subRm16Cm16, subCm16Cm16, subError,     subError,    subError   },
/* 10 Short integer  */ {subLonIShoI, subRe16ShoI, subCo16ShoI, subAnglShoI, subError,    subError,    subError,    subError,    subError,    subShoIShoI,  subRe34ShoI, subCo34ShoI},
/* 11 real34         */ {subLonIRe34, subRe16Re34, subCo16Re34, subAnglRe34, subTimeRe34, subDateRe34, subError,    subError,    subError,    subShoIRe34,  subRe34Re34, subCo34Re34},
/* 12 complex34      */ {subLonICo34, subRe16Co34, subCo16Co34, subError,    subError,    subError,    subError,    subError,    subError,    subShoICo34,  subRe34Co34, subCo34Co34}
};



/********************************************//**
 * \brief Data type error in subtraction
 *
 * \param void
 * \return void
 ***********************************************/
void subError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
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



/********************************************//**
 * \brief opY(long integer) - opX(long integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonILonI(void) {
  longInteger_t iOp1;
  longInteger_t iOp2;

  convertLongIntegerRegisterToLongInteger(opY, &iOp1);
  convertLongIntegerRegisterToLongInteger(opX, &iOp2);

  longIntegerSubtract(&iOp1, &iOp2, &iOp1);

  convertLongIntegerToLongIntegerRegister(&iOp1, result);
}



/********************************************//**
 * \brief opY(long integer) - opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonIRe16:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal16Register(opY, opY);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonICo16:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opX), REGISTER_COMPLEX16_DATA(result)); // result = opX
  convertLongIntegerRegisterToReal16Register(opY, opY);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(long integer) - opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIAngl(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonIAngl:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertLongIntegerRegisterToAngleRegister(opY, opY);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opY), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
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
 * \brief opY(long integer) - opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIDate(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(long integer) - opX(64bits integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIShoI(void) {
  convertShortIntegerRegisterLongIntegerRegister(opX, opX);
  subLonILonI();
}



/********************************************//**
 * \brief opY(long integer) - opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonIRe34:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subLonICo34:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(opX), REGISTER_COMPLEX34_DATA(result)); // result = opX
  convertLongIntegerRegisterToReal34Register(opY, opY);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real16) - opX(long integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16LonI:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(real16) - opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Re16:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Co16:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opX), REGISTER_COMPLEX16_DATA(result)); // result = opX
  complex16ChangeSign(REGISTER_REAL16_DATA(result)); // result = -result
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(result), REGISTER_REAL16_DATA(result)); // real part
}



/********************************************//**
 * \brief opY(real16) - opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Angl(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Angl:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE34 == 1)
    convertRegister16To34(opY)
  #endif
  convertAngleToInternal(REGISTER_ANGLE_DATA(opY), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
}



/********************************************//**
 * \brief opY(real16) - opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Time(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Time:", "cannot use NaN as an input of -", NULL, NULL);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe16Date:", "cannot use NaN as an input of -", NULL, NULL);
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
  convertShortIntegerRegisterToReal16Register(opX, opX);
  subRe16Re16();
}



/********************************************//**
 * \brief opY(real16) - opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Re34(void) {
  convertRegister16To34(opY);
  subRe34Re34();
}



/********************************************//**
 * \brief opY(real16) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Co34(void) {
  convertRegister16To34(opY);
  subRe34Co34();
}



/********************************************//**
 * \brief opY(complex16) - opX(long integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16LonI:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opY), REGISTER_COMPLEX16_DATA(result)); // result = opX
  convertLongIntegerRegisterToReal16Register(opX, opX);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



/********************************************//**
 * \brief opY(complex16) - opX(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16Re16:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex16) - opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo16Co16:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Subtract(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex16) - opX(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16ShoI(void) {
  convertShortIntegerRegisterToReal16Register(opX, opX);
  subCo16Re16();
}



/********************************************//**
 * \brief opY(complex16) - opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Re34(void) {
  convertRegister16To34(opY);
  subCo34Re34();
}



/********************************************//**
 * \brief opY(complex16) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Co34(void) {
  convertRegister16To34(opY);
  subCo34Co34();
}



/********************************************//**
 * \brief opY(angle) - opX(long integer) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subAnglLonI(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAnglLonI:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertLongIntegerRegisterToAngleRegister(opX, opX);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opX), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
}



/********************************************//**
 * \brief opY(angle) - opX(real16) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subAnglRe16(void) {
  if(real16IsNaN(REGISTER_ANGLE_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAnglRe16:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE34 == 1)
    convertRegister16To34(opX)
  #endif
  convertAngleToInternal(REGISTER_ANGLE_DATA(opX), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
}



/********************************************//**
 * \brief opY(angle) - opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subAnglAngl(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY)) || angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAnglAngl:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
}



/********************************************//**
 * \brief opY(angle) - opX(short integer) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subAnglShoI(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAnglShoI:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertShortIntegerRegisterToAngleRegister(opX, opX);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opX), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
}



/********************************************//**
 * \brief opY(angle) - opX(real34) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subAnglRe34(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subAnglRe34:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE16 == 1)
    convertRegister34To16(opX);
  #endif
  convertAngleToInternal(REGISTER_ANGLE_DATA(opX), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
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
 * \brief opY(time) - opX(real16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subTimeRe16:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subTimeRe34:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

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
 * \brief opY(date) - opX(real16) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subDateRe16:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subDateRe34:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



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



/********************************************//**
 * \brief opY(complex16 matrix) - opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subCm16Cm16(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief opY(64bits integer) - opX(long integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoILonI(void) {
  convertShortIntegerRegisterLongIntegerRegister(opY, opY);
  subLonILonI();
}



/********************************************//**
 * \brief opY(64bits integer) - opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoIRe16(void) {
  convertShortIntegerRegisterToReal16Register(opY, opY);
  subRe16Re16();
}



/********************************************//**
 * \brief opY(64bits integer) - opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoICo16(void) {
  convertShortIntegerRegisterToReal16Register(opY, opY);
  subRe16Co16();
}



/********************************************//**
 * \brief opY(short integer) - opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoIAngl(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subShoIAngl:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertShortIntegerRegisterToAngleRegister(opY, opY);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opY), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
}



/********************************************//**
 * \brief opY(Short integer) - opX(Short integer) ==> result(Short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoIShoI(void) {
  reallocateRegister(result, dtShortInteger, SHORT_INTEGER_SIZE, getRegisterBase(opY));
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_intSubtract(*(REGISTER_SHORT_INTEGER_DATA(opY)), *(REGISTER_SHORT_INTEGER_DATA(opX)));
}



/********************************************//**
 * \brief opY(64bits integer) - opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoIRe34(void) {
  convertShortIntegerRegisterToReal34Register(opY, opY);
  subRe34Re34();
}



/********************************************//**
 * \brief opY(64bits integer) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subShoICo34(void) {
  convertShortIntegerRegisterToReal34Register(opY, opY);
  subRe34Co34();
}



/********************************************//**
 * \brief opY(real34) - opX(long integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34LonI:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) - opX(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Re16(void) {
  convertRegister16To34(opX);
  subRe34Re34();
}



/********************************************//**
 * \brief opY(real34) - opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Co16(void) {
  convertRegister16To34(opX);
  subRe34Co34();
}



/********************************************//**
 * \brief opY(real34) - opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Angl(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Angl:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE16 == 1)
    convertRegister34To16(opY);
  #endif
  convertAngleToInternal(REGISTER_ANGLE_DATA(opY), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);
}



/********************************************//**
 * \brief opY(real34) - opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Time(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Time:", "cannot use NaN as an input of -", NULL, NULL);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Date:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real34) - opX(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34ShoI(void) {
  convertShortIntegerRegisterToReal34Register(opX, opX);
  subRe34Re34();
}



/********************************************//**
 * \brief opY(real34) - opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Re34:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(real34) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subRe34Co34:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
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
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34LonI:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(opY), REGISTER_COMPLEX34_DATA(result)); // result = opY
  convertLongIntegerRegisterToReal34Register(opX, opX);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



/********************************************//**
 * \brief opY(complex34) - opX(real16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Re16(void) {
  convertRegister16To34(opX);
  subCo34Re34();
}



/********************************************//**
 * \brief opY(complex34) - opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Co16(void) {
  convertRegister16To34(opX);
  subCo34Co34();
}



/********************************************//**
 * \brief opY(complex34) - opX(64bits integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34ShoI(void) {
  convertShortIntegerRegisterToReal34Register(opX, opX);
  subCo34Re34();
}



/********************************************//**
 * \brief opY(complex34) - opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34Re34:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(complex34) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function subCo34Co34:", "cannot use NaN as an input of -", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Subtract(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}
