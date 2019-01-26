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
 * \file division.c
 ***********************************************/

#include "wp43s.h"



void (* const division[12][12])(void) = {
// regX |    regY ==>    1            2            3            4            5            6            7            8            9            10            11           12
//      V                Big integer  real16       complex16    angle34      Time         Date         String       real16 mat   complex16 m  Small integer real34       complex34
/*  1 Big integer    */ {divBigIBigI, divRe16BigI, divCo16BigI, divAnglBigI, divTimeBigI, errorDiv,    errorDiv,    divRm16BigI, divCm16BigI, divSmaIBigI,  divRe34BigI, divCo34BigI},
/*  2 real16         */ {divBigIRe16, divRe16Re16, divCo16Re16, divAnglRe16, divTimeRe16, errorDiv,    errorDiv,    divRm16Re16, divCm16Re16, divSmaIRe16,  divRe34Re16, divCo34Re16},
/*  3 complex16      */ {divBigICo16, divRe16Co16, divCo16Co16, errorDiv,    errorDiv,    errorDiv,    errorDiv,    divRm16Co16, divCm16Co16, divSmaICo16,  divRe34Co16, divCo34Co16},
/*  4 angle34        */ {errorDiv,    errorDiv,    errorDiv,    divAnglAngl, errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,     errorDiv,    errorDiv   },
/*  5 Time           */ {errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,     errorDiv,    errorDiv   },
/*  6 Date           */ {errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,     errorDiv,    errorDiv   },
/*  7 String         */ {errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,     errorDiv,    errorDiv   },
/*  8 real16 mat     */ {errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,     errorDiv,    errorDiv   },
/*  9 complex16 mat  */ {errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,     errorDiv,    errorDiv   },
/* 10 Small integer  */ {divBigISmaI, divRe16SmaI, divCo16SmaI, divAnglSmaI, divTimeSmaI, errorDiv,    errorDiv,    divRm16SmaI, divCm16SmaI, divSmaISmaI,  divRe34SmaI, divCo34SmaI},
/* 11 real34         */ {divBigIRe34, divRe16Re34, divCo16Re34, divAnglRe34, divTimeRe34, errorDiv,    errorDiv,    divRm16Re34, divCm16Re34, divSmaIRe34,  divRe34Re34, divCo34Re34},
/* 12 complex34      */ {divBigICo34, divRe16Co34, divCo16Co34, errorDiv,    errorDiv,    errorDiv,    errorDiv,    divRm16Co34, divCm16Co34, divSmaICo34,  divRe34Co34, divCo34Co34}
};



/********************************************//**
 * \brief Data type error in division
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void errorDiv(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot divide %s", getRegisterDataTypeName(opY, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(opX, true, false));
    showInfoDialog("In function fnDivide:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void divToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "divide %s", getRegisterDataTypeName(opY, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(opX, true, false));
    showInfoDialog("Operation to be coded:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regY ÷ regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnDivide(uint16_t unusedParamButMandatory) {
  if(division[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)] != errorDiv) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opY    = allocateTemporaryRegister();
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_Y, opY);
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    saveStack();

    division[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();
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
    errorDiv();
  }
}



/********************************************//**
 * \brief regX ==> regL and 1 ÷ regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param unusedParamButMandatory uint16_t
 * \return void
 *
 ***********************************************/
void fnInvert(uint16_t unusedParamButMandatory) {
  saveStack();
  result = REGISTER_X;
  opY    = allocateTemporaryRegister();
  opX    = allocateTemporaryRegister();

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    reallocateRegister(opY, dtReal16, REAL16_SIZE, 0);
    int32ToReal16(1, REGISTER_REAL16_DATA(opY));
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    divRe16Re16();

    if(lastErrorCode != 0) {
      restoreStack();
    }

    refreshStack();
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);

    if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnInvert:", "cannot invert 0", NULL, NULL);
      #endif
    }
    else {
      reallocateRegister(opY, dtReal34, REAL34_SIZE, 0);
      int32ToReal34(1, REGISTER_REAL34_DATA(opY));
      copySourceRegisterToDestRegister(REGISTER_X, opX);

      divRe34Re34();
      convertRegister34To16(REGISTER_X);
    }

    if(lastErrorCode != 0) {
      restoreStack();
    }

    refreshStack();
  }

  else if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    reallocateRegister(opY, dtReal16, REAL16_SIZE, 0);
    int32ToReal16(1, REGISTER_REAL16_DATA(opY));
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    divRe16Co16();

    if(lastErrorCode != 0) {
      restoreStack();
    }

    refreshStack();
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    reallocateRegister(opY, dtReal34, REAL34_SIZE, 0);
    int32ToReal34(1, REGISTER_REAL34_DATA(opY));
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    divRe34Re34();

    if(lastErrorCode != 0) {
      restoreStack();
    }

    refreshStack();
  }

  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    reallocateRegister(opY, dtReal34, REAL34_SIZE, 0);
    int32ToReal34(1, REGISTER_REAL34_DATA(opY));
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    divRe34Co34();

    if(lastErrorCode != 0) {
      restoreStack();
    }

    refreshStack();
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnInvert:", "can invert only real16, real34,", "complex16 and complex34", NULL);
    #endif
  }

  freeTemporaryRegister(opY);
  freeTemporaryRegister(opX);
}



/********************************************//**
 * \brief opY(big integer) ÷ opX(big integer) ==> result(big integer or real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divBigIBigI(void) {
  bigInteger_t iOp1;
  bigInteger_t iOp2;

  convertBigIntegerRegisterToBigInteger(opY, &iOp1);
  convertBigIntegerRegisterToBigInteger(opX, &iOp2);

  if(bigIntegerIsZero(&iOp2)) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divBigIBigI:", "cannot divide an infinite integer by 0", NULL, NULL);
    #endif
  }
  else {
    bigIntegerDivideRemainder(&iOp1, &iOp2, &iOp1, &iOp2);

    if(bigIntegerCompareUInt(&iOp2, 0) == BIG_INTEGER_EQUAL) {
      convertBigIntegerToBigIntegerRegister(&iOp1, result);
    }
    else {
      convertBigIntegerRegisterToBigInteger(opY, &iOp1);
      convertBigIntegerRegisterToBigInteger(opX, &iOp2);

      bigIntegerToString(&iOp1, tmpStr3000, 10);
      reallocateRegister(opY, dtReal16, REAL16_SIZE, 0);
      stringToReal16(tmpStr3000, REGISTER_REAL16_DATA(opY));
      bigIntegerToString(&iOp2, tmpStr3000, 10);
      reallocateRegister(opX, dtReal16, REAL16_SIZE, 0);
      stringToReal16(tmpStr3000, REGISTER_REAL16_DATA(opX));
      divRe16Re16();
    }
  }
}



/********************************************//**
 * \brief opY(big integer) ÷ opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divBigIRe16(void) {
  convertBigIntegerRegisterToReal16Register(opY, opY);

  if(real16IsZero(REGISTER_REAL16_DATA(opY)) && real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divBigIRe16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_plusInfinity, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divBigIRe16:", "cannot divide an infinite integer by 0", NULL, NULL);
      #endif
    }
  }

  else {
    reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
    real16Divide(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

    roundRegister(result);
  }
}



/********************************************//**
 * \brief opY(big integer) ÷ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divBigICo16(void) {
  convertBigIntegerRegisterToReal16Register(opY, opY);
  divRe16Co16();
}



/********************************************//**
 * \brief opY(big integer) ÷ opX(small integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divBigISmaI(void) {
  convertSmallIntegerRegisterBigIntegerRegister(opX, opX);
  divBigIBigI();
}



/********************************************//**
 * \brief opY(big integer) ÷ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divBigIRe34(void) {
  convertBigIntegerRegisterToReal34Register(opY, opY);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divBigIRe34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_plusInfinity, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divBigIRe34:", "cannot divide an infinite integer by 0", NULL, NULL);
      #endif
    }
  }

  else {
    reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    roundRegister(result);
  }
}



/********************************************//**
 * \brief opY(big integer) ÷ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divBigICo34(void) {
  convertBigIntegerRegisterToReal34Register(opY, opY);
  divRe34Co34();
}



/********************************************//**
 * \brief opY(real16) ÷ opX(big integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16BigI(void) {
  convertBigIntegerRegisterToReal16Register(opX, opX);
  divRe16Re16();
}



/********************************************//**
 * \brief opY(real16) ÷ opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Re16(void) {
  if(real16IsZero(REGISTER_REAL16_DATA(opY)) && real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16Re16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_plusInfinity, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16Re16:", "cannot divide an real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
    real16Divide(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));

    roundRegister(result);
  }
}



/********************************************//**
 * \brief opY(real16) ÷ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Co16(void) {
  real16_t temp;

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Multiply(REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(opX), &temp); // c*c
  real16FMA(REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(opX), &temp, &temp); // c*c + d*d

  real16Divide(REGISTER_REAL16_DATA(opX), &temp, REGISTER_REAL16_DATA(result));
  real16Multiply(REGISTER_REAL16_DATA(result), REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(result));
  real16ChangeSign(&temp);
  real16Divide(REGISTER_IMAG16_DATA(opX), &temp, REGISTER_IMAG16_DATA(result));
  real16Multiply(REGISTER_IMAG16_DATA(result), REGISTER_REAL16_DATA(opY), REGISTER_IMAG16_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(real16) ÷ opX(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16SmaI(void) {
  convertSmallIntegerRegisterToReal16Register(opX, opX);
  divRe16Re16();
}



/********************************************//**
 * \brief opY(real16) ÷ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Re34(void) {
  convertRegister16To34(opY);
  divRe34Re34();
}



/********************************************//**
 * \brief opY(real16) ÷ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Co34(void) {
  convertRegister16To34(opY);
  divRe34Co34();
}



/********************************************//**
 * \brief opY(complex16) ÷ opX(big integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16BigI(void) {
  convertBigIntegerRegisterToReal16Register(opX, opX);
  divCo16Re16();
}



/********************************************//**
 * \brief opY(complex16) ÷ opX(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Re16(void) {
  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Divide(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Divide(REGISTER_IMAG16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex16) ÷ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Co16(void) {
  real16_t temp;

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);

  // Denominator
  real16Multiply(REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(opX), &temp); // c*c
  real16FMA(REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(opX), &temp, &temp); // c*c + d*d

  // real part
  real16Multiply(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // a*c
  real16FMA(REGISTER_IMAG16_DATA(opY), REGISTER_IMAG16_DATA(opX), REGISTER_REAL16_DATA(result), REGISTER_REAL16_DATA(result)); // a*c + b*d
  real16Divide(REGISTER_REAL16_DATA(result), &temp, REGISTER_REAL16_DATA(result)); // (a*c + b*d) / (c*c + d*d)

  // imaginary part
  real16Multiply(REGISTER_IMAG16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_IMAG16_DATA(result)); // b*c
  real16ChangeSign(REGISTER_REAL16_DATA(opY)); // -a
  real16FMA(REGISTER_REAL16_DATA(opY), REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(result), REGISTER_IMAG16_DATA(result)); // b*c - a*d
  real16Divide(REGISTER_IMAG16_DATA(result), &temp, REGISTER_IMAG16_DATA(result)); // (b*c - a*d) / (c*c + d*d)

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex16) ÷ opX(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16SmaI(void) {
  convertSmallIntegerRegisterToReal16Register(opX, opX);
  divCo16Re16();
}



/********************************************//**
 * \brief opY(complex16) ÷ opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Re34(void) {
  convertRegister16To34(opY);
  divCo34Re34();
}



/********************************************//**
 * \brief opY(complex16) ÷ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Co34(void) {
  convertRegister16To34(opY);
  divCo34Co34();
}



/********************************************//**
 * \brief opY(angle) ÷ opX(big integer) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void divAnglBigI(void) {
  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertBigIntegerRegisterToAngleRegister(opX, opX);
  angleDivide(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) ÷ opX(real16) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void divAnglRe16(void) {
  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE34 == 1)
    convertRegister16To34(opX)
  #endif
  angleDivide(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) ÷ opX(angle) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void divAnglAngl(void) {
  angleDivide(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  #if (ANGLE34 == 1)
    convertRegister34To16(result);
  #endif
  setRegisterDataType(result, dtReal16);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) ÷ opX(small integer) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void divAnglSmaI(void) {
  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertSmallIntegerRegisterToAngleRegister(opX, opX);
  angleDivide(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) ÷ opX(real34) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void divAnglRe34(void) {
  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE16 == 1)
    convertRegister34To16(opX);
  #endif
  angleDivide(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(time) ÷ opX(big integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeBigI(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(time) ÷ opX(real16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeRe16(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(time) ÷ opX(64bits integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeSmaI(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(time) ÷ opX(real34) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeRe34(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(big integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16BigI(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(real16) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Re16(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(complex16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Co16(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(64bits integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16SmaI(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(real34) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Re34(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(complex34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Co34(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(big integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16BigI(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(real16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Re16(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(complex16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Co16(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(64bits integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16SmaI(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(real34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Re34(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(complex34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Co34(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(big integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divSmaIBigI(void) {
  convertSmallIntegerRegisterBigIntegerRegister(opY, opY);
  divBigIBigI();
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divSmaIRe16(void) {
  convertSmallIntegerRegisterToReal16Register(opY, opY);
  divRe16Re16();
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divSmaICo16(void) {
  convertSmallIntegerRegisterToReal16Register(opY, opY);
  divRe16Co16();
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(64bits integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divSmaISmaI(void) {
  reallocateRegister(result, dtSmallInteger, SMALL_INTEGER_SIZE, getRegisterBase(opY));
  *(REGISTER_SMALL_INTEGER_DATA(result)) = WP34S_intDivide(*(REGISTER_SMALL_INTEGER_DATA(opY)), *(REGISTER_SMALL_INTEGER_DATA(opX)));
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divSmaIRe34(void) {
  convertSmallIntegerRegisterToReal34Register(opY, opY);
  divRe34Re34();
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divSmaICo34(void) {
  convertSmallIntegerRegisterToReal34Register(opY, opY);
  divRe34Co34();
}



/********************************************//**
 * \brief opY(real34) ÷ opX(big integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34BigI(void) {
  convertBigIntegerRegisterToReal34Register(opX, opX);
  divRe34Re34();
}



/********************************************//**
 * \brief opY(real34) ÷ opX(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Re16(void) {
  convertRegister16To34(opX);
  divRe34Re34();
}



/********************************************//**
 * \brief opY(real34) ÷ opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Co16(void) {
  convertRegister16To34(opX);
  divRe34Co34();
}



/********************************************//**
 * \brief opY(real34) ÷ opX(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34SmaI(void) {
  convertSmallIntegerRegisterToReal34Register(opX, opX);
  divRe34Re34();
}



/********************************************//**
 * \brief opY(real34) ÷ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Re34(void) {
  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34Re34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_plusInfinity, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34Re34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    roundRegister(result);
  }
}



/********************************************//**
 * \brief opY(real34) ÷ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Co34(void) {
  real34_t temp;

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX), &temp); // c*c
  real34FMA(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX), &temp, &temp); // c*c + d*d

  // Real part
  real34Divide(REGISTER_REAL34_DATA(opY), &temp, REGISTER_REAL34_DATA(result));
  real34Multiply(REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  // Imaginary part
  real34ChangeSign(&temp);
  real34Divide(REGISTER_REAL34_DATA(opY), &temp, REGISTER_IMAG34_DATA(result));
  real34Multiply(REGISTER_IMAG34_DATA(result), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result));

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex34) ÷ opX(big integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34BigI(void) {
  convertBigIntegerRegisterToReal34Register(opX, opX);
  divCo34Re34();
}



/********************************************//**
 * \brief opY(complex34) ÷ opX(real16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Re16(void) {
  convertRegister16To34(opX);
  divCo34Re34();
}



/********************************************//**
 * \brief opY(complex34) ÷ opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Co16(void) {
  convertRegister16To34(opX);
  divCo34Co34();
}



/********************************************//**
 * \brief opY(complex34) ÷ opX(64bits integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34SmaI(void) {
  convertSmallIntegerRegisterToReal34Register(opX, opX);
  divCo34Re34();
}



/********************************************//**
 * \brief opY(complex34) ÷ opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Re34(void) {
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Divide(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part

  roundRegister(result);
}



/********************************************//**
 * \brief opY(complex34) ÷ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Co34(void) {
  real34_t temp;

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX), &temp); // c*c
  real34FMA(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX), &temp, &temp); // c*c + d*d

  // real part
  real34Multiply(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // a*c
  real34FMA(REGISTER_IMAG34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(result)); // a*c + b*d
  real34Divide(REGISTER_REAL34_DATA(result), &temp, REGISTER_REAL34_DATA(result)); // (a*c + b*d) / (c*c + d*d)

  // imaginary part
  real34Multiply(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result)); // b*c
  real34ChangeSign(REGISTER_REAL34_DATA(opY)); // -a
  real34FMA(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(result), REGISTER_IMAG34_DATA(result)); // b*c - a*d
  real34Divide(REGISTER_IMAG34_DATA(result), &temp, REGISTER_IMAG34_DATA(result)); // (b*c - a*d) / (c*c + d*d)

  roundRegister(result);
}
