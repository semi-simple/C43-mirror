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
//      V                Big integer  real16       complex16    angle34      Time         Date         String       real16 mat   complex16 m  Small integer real34       complex34
/*  1 Big integer    */ {subBigIBigI, subRe16BigI, subCo16BigI, subAnglBigI, subTimeBigI, subDateBigI, subError,    subError,    subError,    subSmaIBigI,  subRe34BigI, subCo34BigI},
/*  2 real16         */ {subBigIRe16, subRe16Re16, subCo16Re16, subAnglRe16, subTimeRe16, subDateRe16, subError,    subError,    subError,    subSmaIRe16,  subRe34Re16, subCo34Re16},
/*  3 complex16      */ {subBigICo16, subRe16Co16, subCo16Co16, subError,    subError,    subError,    subError,    subError,    subError,    subSmaICo16,  subRe34Co16, subCo34Co16},
/*  4 angle34        */ {subBigIAngl, subRe16Angl, subError,    subAnglAngl, subError,    subError,    subError,    subError,    subError,    subSmaIAngl,  subRe34Angl, subError   },
/*  5 Time           */ {subBigITime, subRe16Time, subError,    subError,    subTimeTime, subError,    subError,    subError,    subError,    subError,     subRe34Time, subError   },
/*  6 Date           */ {subBigIDate, subRe16Date, subError,    subError,    subError,    subDateDate, subError,    subError,    subError,    subError,     subRe34Date, subError   },
/*  7 String         */ {subError,    subError,    subError,    subError,    subError,    subError,    subError,    subError,    subError,    subError,     subError,    subError   },
/*  8 real16 mat     */ {subError,    subError,    subError,    subError,    subError,    subError,    subError,    subRm16Rm16, subCm16Rm16, subError,     subError,    subError   },
/*  9 complex16 mat  */ {subError,    subError,    subError,    subError,    subError,    subError,    subError,    subRm16Cm16, subCm16Cm16, subError,     subError,    subError   },
/* 10 Small integer  */ {subBigISmaI, subRe16SmaI, subCo16SmaI, subAnglSmaI, subError,    subError,    subError,    subError,    subError,    subSmaISmaI,  subRe34SmaI, subCo34SmaI},
/* 11 real34         */ {subBigIRe34, subRe16Re34, subCo16Re34, subAnglRe34, subTimeRe34, subDateRe34, subError,    subError,    subError,    subSmaIRe34,  subRe34Re34, subCo34Re34},
/* 12 complex34      */ {subBigICo34, subRe16Co34, subCo16Co34, subError,    subError,    subError,    subError,    subError,    subError,    subSmaICo34,  subRe34Co34, subCo34Co34}
};



/********************************************//**
 * \brief Data type error in subtraction
 *
 * \param void
 * \return void
 ***********************************************/
void subError(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot subtract %s", getRegisterDataTypeName(REGISTER_X, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "from %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    showInfoDialog("In function fnSubtract:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void subToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "subtract %s", getRegisterDataTypeName(REGISTER_X, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    showInfoDialog("Operation to be coded:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
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
  if(subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)] != subError) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opY    = allocateTemporaryRegister();
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_Y, opY);
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();
    freeTemporaryRegister(opY);
    freeTemporaryRegister(opX);

    fnDropY(NOPARAM);
    refreshStack();
  }
  else {
    subError();
  }
}



/********************************************//**
 * \brief rexX ==> regL and -regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnChangeSign(uint16_t unusedParamButMandatory) {
  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    real16ChangeSign(REGISTER_REAL16_DATA(REGISTER_X));
    if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
      real16SetPositiveSign(REGISTER_REAL16_DATA(REGISTER_X));
    }
    refreshRegisterLine(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34ChangeSign(REGISTER_REAL34_DATA(REGISTER_X));
    if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
      real34SetPositiveSign(REGISTER_REAL34_DATA(REGISTER_X));
    }
    refreshRegisterLine(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtAngle) {
    #if (ANGLE16 == 1)
      real16ChangeSign(REGISTER_REAL16_DATA(REGISTER_X));
      if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
        real16SetPositiveSign(REGISTER_REAL16_DATA(REGISTER_X));
      }
    #endif

    #if (ANGLE34 == 1)
      real34ChangeSign(REGISTER_REAL34_DATA(REGISTER_X));
      if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
        real34SetPositiveSign(REGISTER_REAL34_DATA(REGISTER_X));
      }
    #endif

    refreshRegisterLine(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    real16ChangeSign(REGISTER_REAL16_DATA(REGISTER_X));
    if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
      real16SetPositiveSign(REGISTER_REAL16_DATA(REGISTER_X));
    }

    real16ChangeSign(REGISTER_IMAG16_DATA(REGISTER_X));
    if(real16IsZero(REGISTER_IMAG16_DATA(REGISTER_X))) {
      real16SetPositiveSign(REGISTER_IMAG16_DATA(REGISTER_X));
    }

    refreshRegisterLine(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    real34ChangeSign(REGISTER_REAL34_DATA(REGISTER_X));
    if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
      real34SetPositiveSign(REGISTER_REAL34_DATA(REGISTER_X));
    }
    real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_X));

    if(real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X))) {
      real34SetPositiveSign(REGISTER_IMAG34_DATA(REGISTER_X));
    }

    refreshRegisterLine(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    setRegisterDataInfo(REGISTER_X, getRegisterDataInfo(REGISTER_X) ^ 1);
    refreshRegisterLine(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    *(REGISTER_SMALL_INTEGER_DATA(REGISTER_X)) = WP34S_intChs(*(REGISTER_SMALL_INTEGER_DATA(REGISTER_X)));
    refreshRegisterLine(REGISTER_X);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
     sprintf(errorMessage, "Cannot change the sign of %s", getRegisterDataTypeName(REGISTER_X, true, false));
     showInfoDialog("In function fnChangeSign:", errorMessage, NULL, NULL);
    #endif
  }
}



/********************************************//**
 * \brief opY(big integer) - opX(big integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigIBigI(void) {
  bigInteger_t iOp1;
  bigInteger_t iOp2;

  convertBigIntegerRegisterToBigInteger(opY, &iOp1);
  convertBigIntegerRegisterToBigInteger(opX, &iOp2);

  bigIntegerSubtract(&iOp1, &iOp2, &iOp1);

  convertBigIntegerToBigIntegerRegister(&iOp1, result);
}



/********************************************//**
 * \brief opY(big integer) - opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigIRe16(void) {
  convertBigIntegerRegisterToReal16Register(opY, opY);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(big integer) - opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigICo16(void) {
  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opX), REGISTER_COMPLEX16_DATA(result)); // result = opX
  convertBigIntegerRegisterToReal16Register(opY, opY);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(big integer) - opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigIAngl(void) {
  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertBigIntegerRegisterToAngleRegister(opY, opY);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opY), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(big integer) - opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigITime(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(big integer) - opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigIDate(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(big integer) - opX(64bits integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigISmaI(void) {
  convertSmallIntegerRegisterBigIntegerRegister(opX, opX);
  subBigIBigI();
}



/********************************************//**
 * \brief opY(big integer) - opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigIRe34(void) {
  convertBigIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(big integer) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigICo34(void) {
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(opX), REGISTER_COMPLEX34_DATA(result)); // result = opX
  convertBigIntegerRegisterToReal34Register(opY, opY);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) - opX(big integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16BigI(void) {
  convertBigIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) - opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Re16(void) {
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) - opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Co16(void) {
  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opX), REGISTER_COMPLEX16_DATA(result)); // result = opX
  complex16ChangeSign(REGISTER_REAL16_DATA(result)); // result = -result
  real16Add(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(result), REGISTER_REAL16_DATA(result)); // real part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) - opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Angl(void) {
  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE34 == 1)
    convertRegister16To34(opY)
  #endif
  convertAngleToInternal(REGISTER_ANGLE_DATA(opY), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) - opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Time(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(real16) - opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Date(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(real16) - opX(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16SmaI(void) {
  convertSmallIntegerRegisterToReal16Register(opX, opX);
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
 * \brief opY(complex16) - opX(big integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16BigI(void) {
  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(REGISTER_COMPLEX16_DATA(opY), REGISTER_COMPLEX16_DATA(result)); // result = opX
  convertBigIntegerRegisterToReal16Register(opX, opX);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex16) - opX(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Re16(void) {
  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Copy(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(result)); // imaginary part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex16) - opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Co16(void) {
  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Subtract(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Subtract(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex16) - opX(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16SmaI(void) {
  convertSmallIntegerRegisterToReal16Register(opX, opX);
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
 * \brief opY(angle) - opX(big integer) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subAnglBigI(void) {
  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertBigIntegerRegisterToAngleRegister(opX, opX);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opX), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) - opX(real16) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subAnglRe16(void) {
  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE34 == 1)
    convertRegister16To34(opX)
  #endif
  convertAngleToInternal(REGISTER_ANGLE_DATA(opX), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) - opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subAnglAngl(void) {
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) - opX(small integer) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subAnglSmaI(void) {
  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertSmallIntegerRegisterToAngleRegister(opX, opX);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opX), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) - opX(real34) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subAnglRe34(void) {
  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE16 == 1)
    convertRegister34To16(opX);
  #endif
  convertAngleToInternal(REGISTER_ANGLE_DATA(opX), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(time) - opX(big integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeBigI(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(time) - opX(real16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeRe16(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(time) - opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeTime(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(time) - opX(real34) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeRe34(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(date) - opX(big integer) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateBigI(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(date) - opX(real16) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateRe16(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(date) - opX(date) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateDate(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(date) - opX(real34) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateRe34(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) - opX(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subRm16Rm16(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) - opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subRm16Cm16(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) - opX(real16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subCm16Rm16(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) - opX(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subCm16Cm16(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(64bits integer) - opX(big integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subSmaIBigI(void) {
  convertSmallIntegerRegisterBigIntegerRegister(opY, opY);
  subBigIBigI();
}



/********************************************//**
 * \brief opY(64bits integer) - opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subSmaIRe16(void) {
  convertSmallIntegerRegisterToReal16Register(opY, opY);
  subRe16Re16();
}



/********************************************//**
 * \brief opY(64bits integer) - opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subSmaICo16(void) {
  convertSmallIntegerRegisterToReal16Register(opY, opY);
  subRe16Co16();
}



/********************************************//**
 * \brief opY(small integer) - opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void subSmaIAngl(void) {
  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertSmallIntegerRegisterToAngleRegister(opY, opY);
  convertAngleToInternal(REGISTER_ANGLE_DATA(opY), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(Small integer) - opX(Small integer) ==> result(Small integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subSmaISmaI(void) {
  reallocateRegister(result, dtSmallInteger, SMALL_INTEGER_SIZE, getRegisterBase(opY));
  *(REGISTER_SMALL_INTEGER_DATA(result)) = WP34S_intSubtract(*(REGISTER_SMALL_INTEGER_DATA(opY)), *(REGISTER_SMALL_INTEGER_DATA(opX)));
}



/********************************************//**
 * \brief opY(64bits integer) - opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subSmaIRe34(void) {
  convertSmallIntegerRegisterToReal34Register(opY, opY);
  subRe34Re34();
}



/********************************************//**
 * \brief opY(64bits integer) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subSmaICo34(void) {
  convertSmallIntegerRegisterToReal34Register(opY, opY);
  subRe34Co34();
}



/********************************************//**
 * \brief opY(real34) - opX(big integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34BigI(void) {
  convertBigIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  roundRegister(result);
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
  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE16 == 1)
    convertRegister34To16(opY);
  #endif
  convertAngleToInternal(REGISTER_ANGLE_DATA(opY), angularMode);
  angleSubtract(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real34) - opX(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Time(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(real34) - opX(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Date(void) {
  subToBeCoded();
}



/********************************************//**
 * \brief opY(real34) - opX(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34SmaI(void) {
  convertSmallIntegerRegisterToReal34Register(opX, opX);
  subRe34Re34();
}



/********************************************//**
 * \brief opY(real34) - opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Re34(void) {

  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real34) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Co34(void) {
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
  real34ChangeSign(REGISTER_IMAG34_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex34) - opX(big integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34BigI(void) {
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(opY), REGISTER_COMPLEX34_DATA(result)); // result = opY
  convertBigIntegerRegisterToReal34Register(opX, opX);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  roundRegister(result);
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
void subCo34SmaI(void) {
  convertSmallIntegerRegisterToReal34Register(opX, opX);
  subCo34Re34();
}



/********************************************//**
 * \brief opY(complex34) - opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Re34(void) {
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(result)); // imaginary part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex34) - opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Co34(void) {
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Subtract(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Subtract(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part

  roundRegister(result);
}
