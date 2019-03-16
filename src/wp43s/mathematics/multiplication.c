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
// regX |    regY ==>    1            2            3            4            5            6            7            8            9            10            11           12
//      V                Long integer real16       complex16    Angle34      Time         Date         String       real16 mat   complex16 m  Short integer real34       complex34
/*  1 Long integer   */ {mulLonILonI, mulRe16LonI, mulCo16LonI, mulAnglLonI, mulTimeLonI, mulError,    mulError,    mulRm16LonI, mulCm16LonI, mulShoILonI,  mulRe34LonI, mulCo34LonI},
/*  2 real16         */ {mulLonIRe16, mulRe16Re16, mulCo16Re16, mulAnglRe16, mulTimeRe16, mulError,    mulError,    mulRm16Re16, mulCm16Re16, mulShoIRe16,  mulRe34Re16, mulCo34Re16},
/*  3 complex16      */ {mulLonICo16, mulRe16Co16, mulCo16Co16, mulError,    mulError,    mulError,    mulError,    mulRm16Co16, mulCm16Co16, mulShoICo16,  mulRe34Co16, mulCo34Co16},
/*  4 angle34        */ {mulLonIAngl, mulRe16Angl, mulError,    mulError,    mulError,    mulError,    mulError,    mulError,    mulError,    mulShoIAngl,  mulRe34Angl, mulError   },
/*  5 Time           */ {mulLonITime, mulRe16Time, mulError,    mulError,    mulError,    mulError,    mulError,    mulError,    mulError,    mulShoITime,  mulRe34Time, mulError   },
/*  6 Date           */ {mulError,    mulError,    mulError,    mulError,    mulError,    mulError,    mulError,    mulError,    mulError,    mulError,     mulError,    mulError   },
/*  7 String         */ {mulError,    mulError,    mulError,    mulError,    mulError,    mulError,    mulError,    mulError,    mulError,    mulError,     mulError,    mulError   },
/*  8 real16 mat     */ {mulLonIRm16, mulRe16Rm16, mulCo16Rm16, mulError,    mulError,    mulError,    mulError,    mulRm16Rm16, mulCm16Rm16, mulShoIRm16,  mulRe34Rm16, mulCo34Rm16},
/*  9 complex16 mat  */ {mulLonICm16, mulRe16Cm16, mulCo16Cm16, mulError,    mulError,    mulError,    mulError,    mulRm16Cm16, mulCm16Cm16, mulShoICm16,  mulRe34Cm16, mulCo34Cm16},
/* 10 Short integer  */ {mulLonIShoI, mulRe16ShoI, mulCo16ShoI, mulAnglShoI, mulTimeShoI, mulError,    mulError,    mulRm16ShoI, mulCm16ShoI, mulShoIShoI,  mulRe34ShoI, mulCo34ShoI},
/* 11 real34         */ {mulLonIRe34, mulRe16Re34, mulCo16Re34, mulAnglRe34, mulTimeRe34, mulError,    mulError,    mulRm16Re34, mulCm16Re34, mulShoIRe34,  mulRe34Re34, mulCo34Re34},
/* 12 complex34      */ {mulLonICo34, mulRe16Co34, mulCo16Co34, mulError,    mulError,    mulError,    mulError,    mulRm16Co34, mulCm16Co34, mulShoICo34,  mulRe34Co34, mulCo34Co34}
};



/********************************************//**
 * \brief Data type error in multiplication
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void mulError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot multiply %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnMultiply:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void mulToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "multiply %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("Operation to be coded:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
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

  result = REGISTER_X;
  opY    = allocateTemporaryRegister();
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_Y, opY);
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();
  freeTemporaryRegister(opY);
  freeTemporaryRegister(opX);

  if(lastErrorCode == 0) {
    fnDropY(NOPARAM);
  }
  else {
    restoreStack();
  }

  refreshStack();
}



/********************************************//**
 * \brief opY(long integer) × opX(long integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonILonI(void) {
  longInteger_t iOp1;
  longInteger_t iOp2;

  convertLongIntegerRegisterToLongInteger(opY, &iOp1);
  convertLongIntegerRegisterToLongInteger(opX, &iOp2);

  longIntegerMultiply(&iOp1, &iOp2, &iOp1);

  convertLongIntegerToLongIntegerRegister(&iOp1, result);
}



/********************************************//**
 * \brief opY(long integer) × opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulLonIRe16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal16Register(opY, opY);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Multiply(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(long integer) × opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulLonICo16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opX), REGISTER_COMPLEX16_DATA(result)); // result = opX
  convertLongIntegerRegisterToReal16Register(opY, opY);
  real16Multiply(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
  real16Multiply(REGISTER_REAL16_DATA(opY), REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(long integer) × opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIAngl(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulLonIAngl:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertLongIntegerRegisterToAngleRegister(opY, opY);
  angleMultiply(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(long integer) × opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonITime(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(long integer) × opX(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIRm16(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(long integer) × opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonICm16(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(long integer) × opX(64bits integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIShoI(void) {
  convertShortIntegerRegisterLongIntegerRegister(opX, opX);
  mulLonILonI();
}



/********************************************//**
 * \brief opY(long integer) × opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulLonIRe34:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(long integer) × opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulLonICo34:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(opX), REGISTER_COMPLEX34_DATA(result)); // result = opX
  convertLongIntegerRegisterToReal34Register(opY, opY);
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) × opX(long integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16LonI:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Multiply(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) × opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Re16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Multiply(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) × opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Co16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opX), REGISTER_COMPLEX16_DATA(result)); // result = opX
  real16Multiply(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(result), REGISTER_REAL16_DATA(result)); // real part
  real16Multiply(REGISTER_REAL16_DATA(opY), REGISTER_IMAG16_DATA(result), REGISTER_IMAG16_DATA(result)); // imaginary part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) × opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Angl(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Angl:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE34 == 1)
    convertRegister16To34(opY)
  #endif
  angleMultiply(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) × opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Time(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Time:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(real16) × opX(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Rm16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Rm16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(real16) × opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Cm16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe16Cm16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(real16) × opX(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16ShoI(void) {
  convertShortIntegerRegisterToReal16Register(opX, opX);
  mulRe16Re16();
}



/********************************************//**
 * \brief opY(real16) × opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Re34(void) {
  convertRegister16To34(opY);
  mulRe34Re34();
}



/********************************************//**
 * \brief opY(real16) × opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Co34(void) {
  convertRegister16To34(opY);
  mulRe34Co34();
}



/********************************************//**
 * \brief opY(complex16) × opX(long integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16LonI:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opY), REGISTER_COMPLEX16_DATA(result)); // result = opX
  convertLongIntegerRegisterToReal16Register(opX, opX);
  real16Multiply(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
  real16Multiply(REGISTER_IMAG16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_IMAG16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex16) × opX(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16Re16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Multiply(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Multiply(REGISTER_IMAG16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex16) × opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16Co16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);

  // imaginary part
  real16Multiply(REGISTER_REAL16_DATA(opY), REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result));
  real16FMA(REGISTER_IMAG16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_IMAG16_DATA(result), REGISTER_IMAG16_DATA(result));

  //real part
  real16ChangeSign(REGISTER_IMAG16_DATA(opY));
  real16Multiply(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
  real16FMA(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(opX), REGISTER_REAL16_DATA(result), REGISTER_REAL16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex16) × opX(real16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Rm16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16Rm16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(complex16) × opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Cm16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo16Cm16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(complex16) × opX(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16ShoI(void) {
  convertShortIntegerRegisterToReal16Register(opX, opX);
  mulCo16Re16();
}



/********************************************//**
 * \brief opY(complex16) × opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Re34(void) {
  convertRegister16To34(opY);
  mulCo34Re34();
}



/********************************************//**
 * \brief opY(complex16) × opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Co34(void) {
  convertRegister16To34(opY);
  mulCo34Co34();
}



/********************************************//**
 * \brief opY(angle) × opX(long integer) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void mulAnglLonI(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulAnglLonI:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertLongIntegerRegisterToAngleRegister(opX, opX);
  angleMultiply(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) × opX(real16) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void mulAnglRe16(void) {
  if(real16IsNaN(REGISTER_ANGLE_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulAnglRe16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE34 == 1)
    convertRegister16To34(opX)
  #endif
  angleMultiply(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) × opX(short integer) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void mulAnglShoI(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulAnglShoI:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertShortIntegerRegisterToAngleRegister(opX, opX);
  angleMultiply(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) × opX(real34) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void mulAnglRe34(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulAnglRe34:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE16 == 1)
    convertRegister34To16(opX);
  #endif
  angleMultiply(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(time) × opX(long integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeLonI(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(time) × opX(real16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulTimeRe16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(time) × opX(64bits integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeShoI(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(time) × opX(real34) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulTimeRe34:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) × opX(long integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16LonI(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) × opX(real16) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRm16Re16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) × opX(complex16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRm16Co16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) × opX(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Rm16(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) × opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Cm16(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) × opX(64bits integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16ShoI(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) × opX(real34) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRm16Re34:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) × opX(complex34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRm16Co34:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) × opX(long integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16LonI(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) × opX(real16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCm16Re16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) × opX(complex16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCm16Co16:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) × opX(real16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Rm16(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) × opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Cm16(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) × opX(64bits integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16ShoI(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) × opX(real34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCm16Re34:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) × opX(complex34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCm16Co34:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  mulToBeCoded();
}



/********************************************//**
 * \brief opY(64bits integer) × opX(long integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoILonI(void) {
  convertShortIntegerRegisterLongIntegerRegister(opY, opY);
  mulLonILonI();
}



/********************************************//**
 * \brief opY(64bits integer) × opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIRe16(void) {
  convertShortIntegerRegisterToReal16Register(opY, opY);
  mulRe16Re16();
}



/********************************************//**
 * \brief opY(64bits integer) × opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoICo16(void) {
  convertShortIntegerRegisterToReal16Register(opY, opY);
  mulRe16Co16();
}



/********************************************//**
 * \brief opY(short integer) × opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIAngl(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulShoIAngl:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertShortIntegerRegisterToAngleRegister(opY, opY);
  angleMultiply(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(64bits integer) × opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoITime(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(64bits integer) × opX(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIRm16(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(64bits integer) × opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoICm16(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(64bits integer) × opX(64bits integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIShoI(void) {
  reallocateRegister(result, dtShortInteger, SHORT_INTEGER_SIZE, getRegisterBase(opY));
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_intMultiply(*(REGISTER_SHORT_INTEGER_DATA(opY)), *(REGISTER_SHORT_INTEGER_DATA(opX)));
}



/********************************************//**
 * \brief opY(64bits integer) × opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIRe34(void) {
  convertShortIntegerRegisterToReal34Register(opY, opY);
  mulRe34Re34();
}



/********************************************//**
 * \brief opY(64bits integer) × opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoICo34(void) {
  convertShortIntegerRegisterToReal34Register(opY, opY);
  mulRe34Co34();
}



/********************************************//**
 * \brief opY(real34) × opX(long integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34LonI:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real34) × opX(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Re16(void) {
  convertRegister16To34(opX);
  mulRe34Re34();
}



/********************************************//**
 * \brief opY(real34) × opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Co16(void) {
  convertRegister16To34(opX);
  mulRe34Co34();
}



/********************************************//**
 * \brief opY(real34) × opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Angl(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34Angl:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE16 == 1)
    convertRegister34To16(opY);
  #endif
  angleMultiply(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real34) × opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Time(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(real34) × opX(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Rm16(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(real34) × opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Cm16(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(real34) × opX(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34ShoI(void) {
  convertShortIntegerRegisterToReal34Register(opX, opX);
  mulRe34Re34();
}



/********************************************//**
 * \brief opY(real34) × opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34Re34:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real34) × opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulRe34Co34:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex34) × opX(long integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo34LonI:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(opY), REGISTER_COMPLEX34_DATA(result)); // result = opY
  convertLongIntegerRegisterToReal34Register(opX, opX);
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  real34Multiply(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex34) × opX(real16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Re16(void) {
  convertRegister16To34(opX);
  mulCo34Re34();
}



/********************************************//**
 * \brief opY(complex34) × opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Co16(void) {
  convertRegister16To34(opX);
  mulCo34Co34();
}



/********************************************//**
 * \brief opY(complex34) × opX(real16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Rm16(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(complex34) × opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Cm16(void) {
  mulToBeCoded();
}



/********************************************//**
 * \brief opY(complex34) × opX(64bits integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34ShoI(void) {
  convertShortIntegerRegisterToReal34Register(opX, opX);
  mulCo34Re34();
}



/********************************************//**
 * \brief opY(complex34) × opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo34Re34:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Multiply(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex34) × opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function mulCo34Co34:", "cannot use NaN as an input of x", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);

  // imaginary part
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result));
  real34FMA(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result), REGISTER_IMAG34_DATA(result));

  //real part
  real34ChangeSign(REGISTER_IMAG34_DATA(opY));
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  real34FMA(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(result));

  roundRegister(result);
}
