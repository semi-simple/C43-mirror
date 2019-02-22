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
//      V                Big integer  real16       complex16    angle34      Time         Date         String       real16 mat   complex16 m  Small integer real34       complex34
/*  1 Big integer    */ {addBigIBigI, addRe16BigI, addCo16BigI, addAnglBigI, addTimeBigI, addDateBigI, addStriBigI, addError,    addError,    addSmaIBigI,  addRe34BigI, addCo34BigI},
/*  2 real16         */ {addBigIRe16, addRe16Re16, addCo16Re16, addAnglRe16, addTimeRe16, addDateRe16, addStriRe16, addError,    addError,    addSmaIRe16,  addRe34Re16, addCo34Re16},
/*  3 complex16      */ {addBigICo16, addRe16Co16, addCo16Co16, addError,    addError,    addError,    addStriCo16, addError,    addError,    addSmaICo16,  addRe34Co16, addCo34Co16},
/*  4 angle34        */ {addBigIAngl, addRe16Angl, addError,    addAnglAngl, addError,    addError,    addStriAngl, addError,    addError,    addSmaIAngl,  addRe34Angl, addError   },
/*  5 Time           */ {addBigITime, addRe16Time, addError,    addError,    addTimeTime, addError,    addStriTime, addError,    addError,    addError,     addRe34Time, addError   },
/*  6 Date           */ {addBigIDate, addRe16Date, addError,    addError,    addError,    addError,    addStriDate, addError,    addError,    addError,     addRe34Date, addError   },
/*  7 String         */ {addError,    addError,    addError,    addError,    addError,    addError,    addStriStri, addError,    addError,    addError,     addError,    addError   },
/*  8 real16 mat     */ {addError,    addError,    addError,    addError,    addError,    addError,    addStriRm16, addRm16Rm16, addCm16Rm16, addError,     addError,    addError   },
/*  9 complex16 mat  */ {addError,    addError,    addError,    addError,    addError,    addError,    addStriCm16, addRm16Cm16, addCm16Cm16, addError,     addError,    addError   },
/* 10 Small integer  */ {addBigISmaI, addRe16SmaI, addCo16SmaI, addAnglSmaI, addError,    addError,    addStriSmaI, addError,    addError,    addSmaISmaI,  addRe34SmaI, addCo34SmaI},
/* 11 real34         */ {addBigIRe34, addRe16Re34, addCo16Re34, addAnglRe34, addTimeRe34, addDateRe34, addStriRe34, addError,    addError,    addSmaIRe34,  addRe34Re34, addCo34Re34},
/* 12 complex34      */ {addBigICo34, addRe16Co34, addCo16Co34, addError,    addError,    addError,    addStriCo34, addError,    addError,    addSmaICo34,  addRe34Co34, addCo34Co34}
};



/********************************************//**
 * \brief Data type error in addition
 *
 * \param void
 * \return void
 ***********************************************/
void addError(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot add %s", getRegisterDataTypeName(REGISTER_X, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    showInfoDialog("In function fnAdd:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void addToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "add %s", getRegisterDataTypeName(REGISTER_X, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    showInfoDialog("Operation to be coded:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
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
  if(addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)] != addError) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opY    = allocateTemporaryRegister();
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_Y, opY);
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();
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
  else {
    addError();
  }
}



/********************************************//**
 * \brief opY(big integer) + opX(big integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigIBigI(void) {
  bigInteger_t iOp1;
  bigInteger_t iOp2;

  convertBigIntegerRegisterToBigInteger(opY, &iOp1);
  convertBigIntegerRegisterToBigInteger(opX, &iOp2);

  bigIntegerAdd(&iOp1, &iOp2, &iOp1);

  convertBigIntegerToBigIntegerRegister(&iOp1, result);
}



/********************************************//**
 * \brief opY(big integer) + opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addBigIRe16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertBigIntegerRegisterToReal16Register(opY, opY);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(big integer) + opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addBigICo16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opX), REGISTER_COMPLEX16_DATA(result)); // result = opX
  convertBigIntegerRegisterToReal16Register(opY, opY);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(big integer) + opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigIAngl(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addBigIAngl:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertBigIntegerRegisterToAngleRegister(opY, opY);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opY), angularMode);
  angleAdd(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(big integer) + opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigITime(void) {
  addToBeCoded();
}



/********************************************//**
 * \brief opY(big integer) + opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigIDate(void) {
  addToBeCoded();
}



/********************************************//**
 * \brief opY(big integer) + opX(64bits integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigISmaI(void) {
  convertSmallIntegerRegisterBigIntegerRegister(opX, opX);
  addBigIBigI();
}



/********************************************//**
 * \brief opY(big integer) + opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addBigIRe34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertBigIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(big integer) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addBigICo34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(opX), REGISTER_COMPLEX34_DATA(result)); // result = opX
  convertBigIntegerRegisterToReal34Register(opY, opY);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) + opX(big integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16BigI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16BigI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertBigIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) + opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Re16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) + opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Co16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opX), REGISTER_COMPLEX16_DATA(result)); // result = opX
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) + opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Angl(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
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

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) + opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Time(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Time:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  addToBeCoded();
}



/********************************************//**
 * \brief opY(real16) + opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Date(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe16Date:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  addToBeCoded();
}



/********************************************//**
 * \brief opY(real16) + opX(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16SmaI(void) {
  convertSmallIntegerRegisterToReal16Register(opX, opX);
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
 * \brief opY(complex16) + opX(big integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16BigI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16BigI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opY), REGISTER_COMPLEX16_DATA(result)); // result = opX
  convertBigIntegerRegisterToReal16Register(opX, opX);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex16) + opX(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16Re16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(result)); // imaginary part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex16) + opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo16Co16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Add(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex16) + opX(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16SmaI(void) {
  convertSmallIntegerRegisterToReal16Register(opX, opX);
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
 * \brief opY(angle) + opX(big integer) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addAnglBigI(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAnglBigI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertBigIntegerRegisterToAngleRegister(opX, opX);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opX), angularMode);
  angleAdd(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) + opX(real16) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addAnglRe16(void) {
  if(real16IsNaN(REGISTER_ANGLE_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
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

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) + opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addAnglAngl(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY)) || angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAnglAngl:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  angleAdd(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) + opX(small integer) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addAnglSmaI(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addAnglSmaI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertSmallIntegerRegisterToAngleRegister(opX, opX);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opX), angularMode);
  angleAdd(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) + opX(real34) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addAnglRe34(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
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

  roundRegister(result);
}



/********************************************//**
 * \brief opY(time) + opX(big integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeBigI(void) {
  addToBeCoded();
}



/********************************************//**
 * \brief opY(time) + opX(real16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addTimeRe16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  addToBeCoded();
}



/********************************************//**
 * \brief opY(time) + opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeTime(void) {
  addToBeCoded();
}



/********************************************//**
 * \brief opY(time) + opX(real34) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addTimeRe34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  addToBeCoded();
}



/********************************************//**
 * \brief opY(date) + opX(big integer) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateBigI(void) {
  addToBeCoded();
}



/********************************************//**
 * \brief opY(date) + opX(real16) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addDateRe16:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  addToBeCoded();
}



/********************************************//**
 * \brief opY(date) + opX(real34) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addDateRe34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  addToBeCoded();
}



/********************************************//**
 * \brief opY(string) + opX(big integer) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriBigI(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  bigIntegerToDisplayString(opX, tmpStr3000);
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
  real16ToDisplayString(REGISTER_REAL16_DATA(opX), false, tmpStr3000);
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
  complex16ToDisplayString(REGISTER_COMPLEX16_DATA(opX), tmpStr3000);
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
  registerAngleToDisplayString(opX, tmpStr3000);
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
  addToBeCoded();
}



/********************************************//**
 * \brief opY(string) + opX(complex16 matrix) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriCm16(void) {
  addToBeCoded();
}



/********************************************//**
 * \brief opY(string) + opX(64bits integer) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriSmaI(void) {
  int16_t len1, len2;
  const font_t *font;

  len1 = stringByteLength(REGISTER_STRING_DATA(opY));
  font = &standardFont;
  smallIntegerToDisplayString(opX, errorMessage, &font); // We use errorMeaage here because this string can become very long
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
  real34ToDisplayString(REGISTER_REAL34_DATA(opX), tmpStr3000);
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
  complex34ToDisplayString(REGISTER_COMPLEX34_DATA(opX), tmpStr3000);
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
  addToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) + opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addRm16Cm16(void) {
  addToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) + opX(real16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addCm16Rm16(void) {
  addToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) + opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addCm16Cm16(void) {
  addToBeCoded();
}



/********************************************//**
 * \brief opY(64bits integer) + opX(big integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addSmaIBigI(void) {
  convertSmallIntegerRegisterBigIntegerRegister(opY, opY);
  addBigIBigI();
}



/********************************************//**
 * \brief opY(64bits integer) + opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addSmaIRe16(void) {
  convertSmallIntegerRegisterToReal16Register(opY, opY);
  addRe16Re16();
}



/********************************************//**
 * \brief opY(64bits integer) + opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addSmaICo16(void) {
  convertSmallIntegerRegisterToReal16Register(opY, opY);
  addRe16Co16();
}



/********************************************//**
 * \brief opY(small integer) + opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void addSmaIAngl(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addSmaIAngl:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertSmallIntegerRegisterToAngleRegister(opY, opY);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opY), angularMode);
  angleAdd(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(64bits integer) + opX(64bits integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addSmaISmaI(void) {
  reallocateRegister(result, dtSmallInteger, SMALL_INTEGER_SIZE, getRegisterBase(opY));
  *(REGISTER_SMALL_INTEGER_DATA(result)) = WP34S_intAdd(*(REGISTER_SMALL_INTEGER_DATA(opY)), *(REGISTER_SMALL_INTEGER_DATA(opX)));
}



/********************************************//**
 * \brief opY(64bits integer) + opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addSmaIRe34(void) {
  convertSmallIntegerRegisterToReal34Register(opY, opY);
  addRe34Re34();
}



/********************************************//**
 * \brief opY(64bits integer) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addSmaICo34(void) {
  convertSmallIntegerRegisterToReal34Register(opY, opY);
  addRe34Co34();
}



/********************************************//**
 * \brief opY(real34) + opX(big integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34BigI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34BigI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  convertBigIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  roundRegister(result);
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
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
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

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real34) + opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Time(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Time:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  addToBeCoded();
}



/********************************************//**
 * \brief opY(real34) + opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Date(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Date:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  addToBeCoded();
}



/********************************************//**
 * \brief opY(real34) + opX(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34SmaI(void) {
  convertSmallIntegerRegisterToReal34Register(opX, opX);
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
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real34) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRe34Co34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex34) + opX(big integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34BigI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34BigI:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(opY), REGISTER_COMPLEX34_DATA(result)); // result = opY
  convertBigIntegerRegisterToReal34Register(opX, opX);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  roundRegister(result);
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
void addCo34SmaI(void) {
  convertSmallIntegerRegisterToReal34Register(opX, opX);
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
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex34) + opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCo34Re34:", "cannot use NaN as an input of +", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Add(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)               ); // real part
  real34Add(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part

  roundRegister(result);
}
