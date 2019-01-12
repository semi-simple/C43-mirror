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
// regX |    regY ==>    1            2            3            4         5            6            7            8            9            10            11           12
//      V                Big integer  real16       complex16    Angle     Time                      String       real16 mat   complex16 m  Small integer real34       complex34
/*  1 Big integer    */ {mulBigIBigI, mulRe16BigI, mulCo16BigI, errorMul, mulTimeBigI, errorMul,    errorMul,    mulRm16BigI, mulCm16BigI, mulSmaIBigI,  mulRe34BigI, mulCo34BigI},
/*  2 real16         */ {mulBigIRe16, mulRe16Re16, mulCo16Re16, errorMul, mulTimeRe16, errorMul,    errorMul,    mulRm16Re16, mulCm16Re16, mulSmaIRe16,  mulRe34Re16, mulCo34Re16},
/*  3 complex16      */ {mulBigICo16, mulRe16Co16, mulCo16Co16, errorMul, errorMul,    errorMul,    errorMul,    mulRm16Co16, mulCm16Co16, mulSmaICo16,  mulRe34Co16, mulCo34Co16},
/*  4                */ {errorMul,    errorMul,    errorMul,    errorMul, errorMul,    errorMul,    errorMul,    errorMul,    errorMul,    errorMul,     errorMul,    errorMul   },
/*  5 Time           */ {mulBigITime, mulRe16Time, errorMul,    errorMul, errorMul,    errorMul,    errorMul,    errorMul,    errorMul,    mulSmaITime,  mulRe34Time, errorMul   },
/*  6 Date           */ {errorMul,    errorMul,    errorMul,    errorMul, errorMul,    errorMul,    errorMul,    errorMul,    errorMul,    errorMul,     errorMul,    errorMul   },
/*  7 String         */ {errorMul,    errorMul,    errorMul,    errorMul, errorMul,    errorMul,    errorMul,    errorMul,    errorMul,    errorMul,     errorMul,    errorMul   },
/*  8 real16 mat     */ {mulBigIRm16, mulRe16Rm16, mulCo16Rm16, errorMul, errorMul,    errorMul,    errorMul,    mulRm16Rm16, mulCm16Rm16, mulSmaIRm16,  mulRe34Rm16, mulCo34Rm16},
/*  9 complex16 mat  */ {mulBigICm16, mulRe16Cm16, mulCo16Cm16, errorMul, errorMul,    errorMul,    errorMul,    mulRm16Cm16, mulCm16Cm16, mulSmaICm16,  mulRe34Cm16, mulCo34Cm16},
/* 10 Small integer  */ {mulBigISmaI, mulRe16SmaI, mulCo16SmaI, errorMul, mulTimeSmaI, errorMul,    errorMul,    mulRm16SmaI, mulCm16SmaI, mulSmaISmaI,  mulRe34SmaI, mulCo34SmaI},
/* 11 real34         */ {mulBigIRe34, mulRe16Re34, mulCo16Re34, errorMul, mulTimeRe34, errorMul,    errorMul,    mulRm16Re34, mulCm16Re34, mulSmaIRe34,  mulRe34Re34, mulCo34Re34},
/* 12 complex34      */ {mulBigICo34, mulRe16Co34, mulCo16Co34, errorMul, errorMul,    errorMul,    errorMul,    mulRm16Co34, mulCm16Co34, mulSmaICo34,  mulRe34Co34, mulCo34Co34}
};



/********************************************//**
 * \brief Data type error in multiplication
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void errorMul(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("errorMul");
  #endif

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot multiply %s", getRegisterDataTypeName(op1, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(op2, true, false));
    showInfoDialog("In function fnMultiply:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("errorMul");
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void mulToBeCoded(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulToBeCoded");
  #endif

  #ifdef PC_BUILD
    sprintf(errorMessage, "multiply %s", getRegisterDataTypeName(op1, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(op2, true, false));
    showInfoDialog("Operation to be coded:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulToBeCoded");
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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnMultiply");
  #endif

  if(multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)] != errorMul) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    op1    = allocateTemporaryRegister();
    op2    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_Y, op1);
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();
    freeTemporaryRegister(op1);
    freeTemporaryRegister(op2);

    fnDropY(NOPARAM);
    refreshStack();
  }
  else {
    errorMul();
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnMultiply");
  #endif
}



/********************************************//**
 * \brief op1(big integer) × op2(big integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulBigIBigI(void) {
  bigInteger_t iOp1;
  bigInteger_t iOp2;

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulBigIBigI");
  #endif

  convertBigIntegerRegisterToBigInteger(op1, &iOp1);
  convertBigIntegerRegisterToBigInteger(op2, &iOp2);

  bigIntegerMultiply(&iOp1, &iOp2, &iOp1);

  convertBigIntegerToBigIntegerRegister(&iOp1, result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulBigIBigI");
  #endif
}



/********************************************//**
 * \brief op1(big integer) × op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulBigIRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulBigIRe16");
  #endif

  convertBigIntegerRegisterToReal16Register(op1, op1);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulBigIRe16");
  #endif
}



/********************************************//**
 * \brief op1(big integer) × op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulBigICo16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulBigICo16");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // result = op2
  convertBigIntegerRegisterToReal16Register(op1, op1);
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE);

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulBigICo16");
  #endif
}



/********************************************//**
 * \brief op1(big integer) × op2(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulBigITime(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulBigITime");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulBigITime");
  #endif
}



/********************************************//**
 * \brief op1(big integer) × op2(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulBigIRm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulBigIRm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulBigIRm16");
  #endif
}



/********************************************//**
 * \brief op1(big integer) × op2(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulBigICm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulBigICm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulBigICm16");
  #endif
}



/********************************************//**
 * \brief op1(big integer) × op2(64bits integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulBigISmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulBigISmaI");
  #endif

  convertSmallIntegerRegisterBigIntegerRegister(op2, op2);
  mulBigIBigI();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulBigISmaI");
  #endif
}



/********************************************//**
 * \brief op1(big integer) × op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulBigIRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulBigIRe34");
  #endif

  convertBigIntegerRegisterToReal34Register(op1, op1);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulBigIRe34");
  #endif
}



/********************************************//**
 * \brief op1(big integer) × op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulBigICo34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulBigICo34");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // result = op2
  convertBigIntegerRegisterToReal34Register(op1, op1);
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE);

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulBigICo34");
  #endif
}



/********************************************//**
 * \brief op1(real16) × op2(big integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe16BigI");
  #endif

  convertBigIntegerRegisterToReal16Register(op2, op2);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe16BigI");
  #endif
}



/********************************************//**
 * \brief op1(real16) × op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe16Re16");
  #endif

  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe16Re16");
  #endif
}



/********************************************//**
 * \brief op1(real16) × op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe16Co16");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // result = op2
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(result), POINTER_TO_REGISTER_DATA(result)); // real part
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe16Co16");
  #endif
}



/********************************************//**
 * \brief op1(real16) × op2(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Time(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe16Time");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe16Time");
  #endif
}



/********************************************//**
 * \brief op1(real16) × op2(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Rm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe16Rm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe16Rm16");
  #endif
}



/********************************************//**
 * \brief op1(real16) × op2(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Cm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe16Cm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe16Cm16");
  #endif
}



/********************************************//**
 * \brief op1(real16) × op2(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe16SmaI");
  #endif

  convertSmallIntegerRegisterToReal16Register(op2, op2);
  mulRe16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(real16) × op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe16Re34");
  #endif

  convertRegister16To34(op1);
  mulRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe16Re34");
  #endif
}



/********************************************//**
 * \brief op1(real16) × op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe16Co34");
  #endif

  convertRegister16To34(op1);
  mulRe34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe16Co34");
  #endif
}



/********************************************//**
 * \brief op1(complex16) × op2(big integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo16BigI");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(result)); // result = op2
  convertBigIntegerRegisterToReal16Register(op2, op2);
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));
  real16Multiply(POINTER_TO_REGISTER_DATA(op1) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE);

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo16BigI");
  #endif
}



/********************************************//**
 * \brief op1(complex16) × op2(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo16Re16");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // real part
  real16Multiply(POINTER_TO_REGISTER_DATA(op1) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo16Re16");
  #endif
}



/********************************************//**
 * \brief op1(complex16) × op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo16Co16");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);

  // imaginary part
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE);
  real16FMA(POINTER_TO_REGISTER_DATA(op1) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE);

  //real part
  real16ChangeSign(POINTER_TO_REGISTER_DATA(op1) + REAL16_SIZE);
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));
  real16FMA(POINTER_TO_REGISTER_DATA(op1) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(op2) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(result), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo16Co16");
  #endif
}



/********************************************//**
 * \brief op1(complex16) × op2(real16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Rm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo16Rm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo16Rm16");
  #endif
}



/********************************************//**
 * \brief op1(complex16) × op2(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Cm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo16Cm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo16Cm16");
  #endif
}



/********************************************//**
 * \brief op1(complex16) × op2(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo16SmaI");
  #endif

  convertSmallIntegerRegisterToReal16Register(op2, op2);
  mulCo16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(complex16) × op2(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo16Re34");
  #endif

  convertRegister16To34(op1);
  mulCo34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo16Re34");
  #endif
}



/********************************************//**
 * \brief op1(complex16) × op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo16Co34");
  #endif

  convertRegister16To34(op1);
  mulCo34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo16Co34");
  #endif
}



/********************************************//**
 * \brief op1(time) × op2(big integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulTimeBigI");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulTimeBigI");
  #endif
}



/********************************************//**
 * \brief op1(time) × op2(real16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulTimeRe16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulTimeRe16");
  #endif
}



/********************************************//**
 * \brief op1(time) × op2(64bits integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeSmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulTimeSmaI");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulTimeSmaI");
  #endif
}



/********************************************//**
 * \brief op1(time) × op2(real34) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulTimeRe34");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulTimeRe34");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) × op2(big integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRm16BigI");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRm16BigI");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) × op2(real16) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRm16Re16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRm16Re16");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) × op2(complex16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRm16Co16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRm16Co16");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) × op2(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Rm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRm16Rm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRm16Rm16");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) × op2(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Cm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRm16Cm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRm16Cm16");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) × op2(64bits integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRm16SmaI");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRm16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) × op2(real34) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRm16Re34");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRm16Re34");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) × op2(complex34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRm16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRm16Co34");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRm16Co34");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) × op2(big integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCm16BigI");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCm16BigI");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) × op2(real16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCm16Re16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCm16Re16");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) × op2(complex16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCm16Co16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCm16Co16");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) × op2(real16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Rm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCm16Rm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCm16Rm16");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) × op2(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Cm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCm16Cm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCm16Cm16");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) × op2(64bits integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCm16SmaI");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCm16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) × op2(real34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCm16Re34");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCm16Re34");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) × op2(complex34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCm16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCm16Co34");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCm16Co34");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) × op2(big integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulSmaIBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulSmaIBigI");
  #endif

  convertSmallIntegerRegisterBigIntegerRegister(op1, op1);
  mulBigIBigI();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulSmaIBigI");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) × op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulSmaIRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulSmaIRe16");
  #endif

  convertSmallIntegerRegisterToReal16Register(op1, op1);
  mulRe16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulSmaIRe16");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) × op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void mulSmaICo16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulSmaICo16");
  #endif

  convertSmallIntegerRegisterToReal16Register(op1, op1);
  mulRe16Co16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulSmaICo16");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) × op2(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulSmaITime(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulSmaITime");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulSmaITime");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) × op2(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulSmaIRm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulSmaIRm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulSmaIRm16");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) × op2(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulSmaICm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulSmaICm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulSmaICm16");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) × op2(64bits integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulSmaISmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulSmaISmaI");
  #endif

  reallocateRegister(result, dtSmallInteger, SMALL_INTEGER_SIZE, getRegisterBase(op1));
  *(uint64_t *)(POINTER_TO_REGISTER_DATA(result)) = WP34S_intMultiply(*(uint64_t *)(POINTER_TO_REGISTER_DATA(op1)), *(uint64_t *)(POINTER_TO_REGISTER_DATA(op2)));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulSmaISmaI");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) × op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulSmaIRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulSmaIRe34");
  #endif

  convertSmallIntegerRegisterToReal34Register(op1, op1);
  mulRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulSmaIRe34");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) × op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulSmaICo34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulSmaICo34");
  #endif

  convertSmallIntegerRegisterToReal34Register(op1, op1);
  mulRe34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulSmaICo34");
  #endif
}



/********************************************//**
 * \brief op1(real34) × op2(big integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe34BigI");
  #endif

  convertBigIntegerRegisterToReal34Register(op2, op2);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe34BigI");
  #endif
}



/********************************************//**
 * \brief op1(real34) × op2(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe34Re16");
  #endif

  convertRegister16To34(op2);
  mulRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe34Re16");
  #endif
}



/********************************************//**
 * \brief op1(real34) × op2(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe34Co16");
  #endif

  convertRegister16To34(op2);
  mulRe34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe34Co16");
  #endif
}



/********************************************//**
 * \brief op1(real34) × op2(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Time(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe34Time");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe34Time");
  #endif
}



/********************************************//**
 * \brief op1(real34) × op2(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Rm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe34Rm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe34Rm16");
  #endif
}



/********************************************//**
 * \brief op1(real34) × op2(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Cm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe34Cm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe34Cm16");
  #endif
}



/********************************************//**
 * \brief op1(real34) × op2(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe34SmaI");
  #endif

  convertSmallIntegerRegisterToReal34Register(op2, op2);
  mulRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe34SmaI");
  #endif
}



/********************************************//**
 * \brief op1(real34) × op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe34Re34");
  #endif

  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe34Re34");
  #endif
}



/********************************************//**
 * \brief op1(real34) × op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRe34Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulRe34Co34");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // real part
  real34Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulRe34Co34");
  #endif
}



/********************************************//**
 * \brief op1(complex34) × op2(big integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo34BigI");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(result)); // result = op1
  convertBigIntegerRegisterToReal34Register(op2, op2);
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));
  real16Multiply(POINTER_TO_REGISTER_DATA(op1) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE);

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo34BigI");
  #endif
}



/********************************************//**
 * \brief op1(complex34) × op2(real16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo34Re16");
  #endif

  convertRegister16To34(op2);
  mulCo34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo34Re16");
  #endif
}



/********************************************//**
 * \brief op1(complex34) × op2(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo34Co16");
  #endif

  convertRegister16To34(op2);
  mulCo34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo34Co16");
  #endif
}



/********************************************//**
 * \brief op1(complex34) × op2(real16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Rm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo34Rm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo34Rm16");
  #endif
}



/********************************************//**
 * \brief op1(complex34) × op2(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Cm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo34Cm16");
  #endif

  mulToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo34Cm16");
  #endif
}



/********************************************//**
 * \brief op1(complex34) × op2(64bits integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo34SmaI");
  #endif

  convertSmallIntegerRegisterToReal34Register(op2, op2);
  mulCo34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo34SmaI");
  #endif
}



/********************************************//**
 * \brief op1(complex34) × op2(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo34Re34");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // real part
  real34Multiply(POINTER_TO_REGISTER_DATA(op1) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo34Re34");
  #endif
}



/********************************************//**
 * \brief op1(complex34) × op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCo34Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("mulCo34Co34");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);

  // imaginary part
  real34Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE);
  real34FMA(POINTER_TO_REGISTER_DATA(op1) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE);

  //real part
  real34ChangeSign(POINTER_TO_REGISTER_DATA(op1) + REAL34_SIZE);
  real34Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));
  real34FMA(POINTER_TO_REGISTER_DATA(op1) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("mulCo34Co34");
  #endif
}
