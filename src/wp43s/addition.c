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
// regX |    regY ==>    1            2            3            4         5            6            7            8            9            10            11           12
//      V                Big integer  real16       complex16              Time         Date         String       real16 mat   complex16 m  Small integer real34       complex34
/*  1 Big integer    */ {addBigIBigI, addRe16BigI, addCo16BigI, errorAdd, addTimeBigI, addDateBigI, addStriBigI, errorAdd,    errorAdd,    addSmaIBigI,  addRe34BigI, addCo34BigI},
/*  2 real16         */ {addBigIRe16, addRe16Re16, addCo16Re16, errorAdd, addTimeRe16, addDateRe16, addStriRe16, errorAdd,    errorAdd,    addSmaIRe16,  addRe34Re16, addCo34Re16},
/*  3 complex16      */ {addBigICo16, addRe16Co16, addCo16Co16, errorAdd, errorAdd,    errorAdd,    addStriCo16, errorAdd,    errorAdd,    addSmaICo16,  addRe34Co16, addCo34Co16},
/*  4                */ {errorAdd,    errorAdd,    errorAdd,    errorAdd, errorAdd,    errorAdd,    errorAdd,    errorAdd,    errorAdd,    errorAdd,     errorAdd,    errorAdd   },
/*  5 Time           */ {addBigITime, addRe16Time, errorAdd,    errorAdd, addTimeTime, errorAdd,    addStriTime, errorAdd,    errorAdd,    errorAdd,     addRe34Time, errorAdd   },
/*  6 Date           */ {addBigIDate, addRe16Date, errorAdd,    errorAdd, errorAdd,    errorAdd,    addStriDate, errorAdd,    errorAdd,    errorAdd,     addRe34Date, errorAdd   },
/*  7 String         */ {errorAdd,    errorAdd,    errorAdd,    errorAdd, errorAdd,    errorAdd,    addStriStri, errorAdd,    errorAdd,    errorAdd,     errorAdd,    errorAdd   },
/*  8 real16 mat     */ {errorAdd,    errorAdd,    errorAdd,    errorAdd, errorAdd,    errorAdd,    addStriRm16, addRm16Rm16, addCm16Rm16, errorAdd,     errorAdd,    errorAdd   },
/*  9 complex16 mat  */ {errorAdd,    errorAdd,    errorAdd,    errorAdd, errorAdd,    errorAdd,    addStriCm16, addRm16Cm16, addCm16Cm16, errorAdd,     errorAdd,    errorAdd   },
/* 10 Small integer  */ {addBigISmaI, addRe16SmaI, addCo16SmaI, errorAdd, errorAdd,    errorAdd,    addStriSmaI, errorAdd,    errorAdd,    addSmaISmaI,  addRe34SmaI, addCo34SmaI},
/* 11 real34         */ {addBigIRe34, addRe16Re34, addCo16Re34, errorAdd, addTimeRe34, addDateRe34, addStriRe34, errorAdd,    errorAdd,    addSmaIRe34,  addRe34Re34, addCo34Re34},
/* 12 complex34      */ {addBigICo34, addRe16Co34, addCo16Co34, errorAdd, errorAdd,    errorAdd,    addStriCo34, errorAdd,    errorAdd,    addSmaICo34,  addRe34Co34, addCo34Co34}
};



/********************************************//**
 * \brief Data type error in addition
 *
 * \param void
 * \return void
 ***********************************************/
void errorAdd(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("errorAdd");
  #endif

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot add %s", getRegisterDataTypeName(op2, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(op1, true, false));
    showInfoDialog("In function fnAdd:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("errorAdd");
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void addToBeCoded(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addToBeCoded");
  #endif

  #ifdef PC_BUILD
    sprintf(errorMessage, "add %s", getRegisterDataTypeName(op2, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(op1, true, false));
    showInfoDialog("Operation to be coded:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addToBeCoded");
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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnAdd");
  #endif

  if(addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)] != errorAdd) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    op1    = allocateTemporaryRegister();
    op2    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_Y, op1);
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();
    freeTemporaryRegister(op1);
    freeTemporaryRegister(op2);

    fnDropY(NOPARAM);
    refreshStack();
  }
  else {
    errorAdd();
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnAdd");
  #endif
}



/********************************************//**
 * \brief op1(big integer) + op2(big integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigIBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addBigIBigI");
  #endif

  bigInteger_t iOp1;
  bigInteger_t iOp2;

  convertBigIntegerRegisterToBigInteger(op1, &iOp1);
  convertBigIntegerRegisterToBigInteger(op2, &iOp2);

  bigIntegerAdd(&iOp1, &iOp2, &iOp1);

  convertBigIntegerToBigIntegerRegister(&iOp1, result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addBigIBigI");
  #endif
}



/********************************************//**
 * \brief op1(big integer) + op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigIRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addBigIRe16");
  #endif

  convertBigIntegerRegisterToReal16Register(op1, op1);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addBigIRe16");
  #endif
}



/********************************************//**
 * \brief op1(big integer) + op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigICo16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addBigICo16");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // result = op2
  convertBigIntegerRegisterToReal16Register(op1, op1);
  real16Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addBigICo16");
  #endif
}



/********************************************//**
 * \brief op1(big integer) + op2(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigITime(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addBigITime");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addBigITime");
  #endif
}



/********************************************//**
 * \brief op1(big integer) + op2(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigIDate(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addBigIDate");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addBigIDate");
  #endif
}



/********************************************//**
 * \brief op1(big integer) + op2(64bits integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigISmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addBigISmaI");
  #endif

  convertSmallIntegerRegisterBigIntegerRegister(op2, op2);
  addBigIBigI();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addBigISmaI");
  #endif
}



/********************************************//**
 * \brief op1(big integer) + op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigIRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addBigIRe34");
  #endif

  convertBigIntegerRegisterToReal34Register(op1, op1);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addBigIRe34");
  #endif
}



/********************************************//**
 * \brief op1(big integer) + op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addBigICo34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addBigICo34");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // result = op2
  convertBigIntegerRegisterToReal34Register(op1, op1);
  real16Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addBigICo34");
  #endif
}



/********************************************//**
 * \brief op1(real16) + op2(big integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe16BigI");
  #endif

  convertBigIntegerRegisterToReal16Register(op2, op2);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe16BigI");
  #endif
}



/********************************************//**
 * \brief op1(real16) + op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe16Re16");
  #endif

  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe16Re16");
  #endif
}



/********************************************//**
 * \brief op1(real16) + op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe16Co16");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // result = op2
  real16Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // real part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe16Co16");
  #endif
}



/********************************************//**
 * \brief op1(real16) + op2(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Time(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe16Time");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe16Time");
  #endif
}



/********************************************//**
 * \brief op1(real16) + op2(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Date(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe16Date");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe16Date");
  #endif
}



/********************************************//**
 * \brief op1(real16) + op2(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe16SmaI");
  #endif

  convertSmallIntegerRegisterToReal16Register(op2, op2);
  addRe16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(real16) + op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe16Re34");
  #endif

  convertRegister16To34(op1);
  addRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe16Re34");
  #endif
}



/********************************************//**
 * \brief op1(real16) + op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe16Co34");
  #endif

  convertRegister16To34(op1);
  addRe34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe16Co34");
  #endif
}



/********************************************//**
 * \brief op1(complex16) + op2(big integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addCo16BigI");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(result)); // result = op2
  convertBigIntegerRegisterToReal16Register(op2, op2);
  real16Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addCo16BigI");
  #endif
}



/********************************************//**
 * \brief op1(complex16) + op2(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addCo16Re16");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // real part
  real16Copy(POINTER_TO_REGISTER_DATA(op1) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addCo16Re16");
  #endif
}



/********************************************//**
 * \brief op1(complex16) + op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addCo16Co16");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Add(POINTER_TO_REGISTER_DATA(op1)               , POINTER_TO_REGISTER_DATA(op2),                POINTER_TO_REGISTER_DATA(result)); // real part
  real16Add(POINTER_TO_REGISTER_DATA(op1) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(op2) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addCo16Co16");
  #endif
}



/********************************************//**
 * \brief op1(complex16) + op2(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addCo16SmaI");
  #endif

  convertSmallIntegerRegisterToReal16Register(op2, op2);
  addCo16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addCo16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(complex16) + op2(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addCo16Re34");
  #endif

  convertRegister16To34(op1);
  addCo34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addCo16Re34");
  #endif
}



/********************************************//**
 * \brief op1(complex16) + op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addCo16Co34");
  #endif

  convertRegister16To34(op1);
  addCo34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addCo16Co34");
  #endif
}



/********************************************//**
 * \brief op1(time) + op2(big integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addTimeBigI");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addTimeBigI");
  #endif
}



/********************************************//**
 * \brief op1(time) + op2(real16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addTimeRe16");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addTimeRe16");
  #endif
}



/********************************************//**
 * \brief op1(time) + op2(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeTime(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addTimeTime");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addTimeTime");
  #endif
}



/********************************************//**
 * \brief op1(time) + op2(real34) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addTimeRe34");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addTimeRe34");
  #endif
}



/********************************************//**
 * \brief op1(date) + op2(big integer) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addDateBigI");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addDateBigI");
  #endif
}



/********************************************//**
 * \brief op1(date) + op2(real16) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addDateRe16");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addDateRe16");
  #endif
}



/********************************************//**
 * \brief op1(date) + op2(real34) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addDateRe34");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addDateRe34");
  #endif
}



/********************************************//**
 * \brief op1(string) + op2(big integer) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addStriBigI");
  #endif

  int16_t len1, len2;

  len1 = stringByteLength(POINTER_TO_REGISTER_STRING(op1));
  bigIntegerToDisplayString(op2, tmpStr3000);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, 0);

  memcpy(POINTER_TO_REGISTER_STRING(result)       , POINTER_TO_REGISTER_STRING(op1), len1    );
  memcpy(POINTER_TO_REGISTER_STRING(result) + len1, tmpStr3000,                      len2 + 1);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addStriBigI");
  #endif
}



/********************************************//**
 * \brief op1(string) + op2(real16) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addStriRe16");
  #endif

  int16_t len1, len2;

  len1 = stringByteLength(POINTER_TO_REGISTER_STRING(op1));
  real16ToDisplayString(REAL16_POINTER(POINTER_TO_REGISTER_DATA(op2)), false, tmpStr3000);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, 0);

  memcpy(POINTER_TO_REGISTER_STRING(result)       , POINTER_TO_REGISTER_STRING(op1), len1    );
  memcpy(POINTER_TO_REGISTER_STRING(result) + len1, tmpStr3000,                      len2 + 1);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addStriRe16");
  #endif
}



/********************************************//**
 * \brief op1(string) + op2(complex16) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriCo16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addStriCo16");
  #endif

  int16_t len1, len2;

  len1 = stringByteLength(POINTER_TO_REGISTER_STRING(op1));
  complex16ToDisplayString(COMPLEX16_POINTER(POINTER_TO_REGISTER_DATA(op2)), tmpStr3000);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, 0);

  memcpy(POINTER_TO_REGISTER_STRING(result)       , POINTER_TO_REGISTER_STRING(op1), len1    );
  memcpy(POINTER_TO_REGISTER_STRING(result) + len1, tmpStr3000,                      len2 + 1);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addStriCo16");
  #endif
}



/********************************************//**
 * \brief op1(string) + op2(time) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriTime(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addStriTime");
  #endif

  int16_t len1, len2;

  len1 = stringByteLength(POINTER_TO_REGISTER_STRING(op1));
  timeToDisplayString(op2, tmpStr3000);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2 , 0);

  memcpy(POINTER_TO_REGISTER_STRING(result)       , POINTER_TO_REGISTER_STRING(op1), len1    );
  memcpy(POINTER_TO_REGISTER_STRING(result) + len1, tmpStr3000,                      len2 + 1);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addStriTime");
  #endif
}



/********************************************//**
 * \brief op1(string) + op2(date) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriDate(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addStriDate");
  #endif

  int16_t len1, len2;

  len1 = stringByteLength(POINTER_TO_REGISTER_STRING(op1));
  dateToDisplayString(op2, tmpStr3000);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, 0);

  memcpy(POINTER_TO_REGISTER_STRING(result)       , POINTER_TO_REGISTER_STRING(op1), len1   );
  memcpy(POINTER_TO_REGISTER_STRING(result) + len1, tmpStr3000,                      len2 + 1);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addStriDate");
  #endif
}



/********************************************//**
 * \brief op1(string) + op2(string) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriStri(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addStriStri");
  #endif

  int16_t len1, len2;

  len1 = stringByteLength(POINTER_TO_REGISTER_STRING(op1));
  len2 = stringByteLength(POINTER_TO_REGISTER_STRING(op2));

  reallocateRegister(result, dtString, len1 + len2, 0);

  memcpy(POINTER_TO_REGISTER_STRING(result)       , POINTER_TO_REGISTER_STRING(op1), len1    );
  memcpy(POINTER_TO_REGISTER_STRING(result) + len1, POINTER_TO_REGISTER_STRING(op2), len2 + 1);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addStriStri");
  #endif
}



/********************************************//**
 * \brief op1(string) + op2(real16 matrix) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriRm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addStriRm16");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addStriRm16");
  #endif
}



/********************************************//**
 * \brief op1(string) + op2(complex16 matrix) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriCm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addStriCm16");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addStriCm16");
  #endif
}



/********************************************//**
 * \brief op1(string) + op2(64bits integer) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriSmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addStriSmaI");
  #endif

  int16_t len1, len2;
  const font_t *font;

  len1 = stringByteLength(POINTER_TO_REGISTER_STRING(op1));
  font = &standardFont;
  smallIntegerToDisplayString(op2, errorMessage, &font); // We use errorMeaage here because this string can become very long
  len2 = stringByteLength(errorMessage) + 1; // +1 for the trailing 0

  reallocateRegister(result, dtString, len1 + len2, 0);

  memcpy(POINTER_TO_REGISTER_STRING(result)       , POINTER_TO_REGISTER_STRING(op1), len1    );
  memcpy(POINTER_TO_REGISTER_STRING(result) + len1, errorMessage,                    len2 + 1);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addStriSmaI");
  #endif
}



/********************************************//**
 * \brief op1(string) + op2(real34) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addStriRe34");
  #endif

  int16_t len1, len2;

  len1 = stringByteLength(POINTER_TO_REGISTER_STRING(op1));
  real34ToDisplayString(REAL34_POINTER(POINTER_TO_REGISTER_DATA(op2)), tmpStr3000);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, 0);

  memcpy(POINTER_TO_REGISTER_STRING(result)       , POINTER_TO_REGISTER_STRING(op1), len1    );
  memcpy(POINTER_TO_REGISTER_STRING(result) + len1, tmpStr3000,                      len2 + 1);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addStriRe34");
  #endif
}



/********************************************//**
 * \brief op1(string) + op2(complex34) ==> result(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriCo34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addStriCo34");
  #endif

  int16_t len1, len2;

  len1 = stringByteLength(POINTER_TO_REGISTER_STRING(op1));
  complex34ToDisplayString(COMPLEX34_POINTER(POINTER_TO_REGISTER_DATA(op2)), tmpStr3000);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(result, dtString, len1 + len2, 0);

  memcpy(POINTER_TO_REGISTER_STRING(result)       , POINTER_TO_REGISTER_STRING(op1), len1    );
  memcpy(POINTER_TO_REGISTER_STRING(result) + len1, tmpStr3000,                      len2 + 1);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addStriCo34");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) + op2(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addRm16Rm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRm16Rm16");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRm16Rm16");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) + op2(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addRm16Cm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRm16Cm16");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRm16Cm16");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) + op2(real16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addCm16Rm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addCm16Rm16");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addCm16Rm16");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) + op2(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addCm16Cm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addCm16Cm16");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addCm16Cm16");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) + op2(big integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addSmaIBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addSmaIBigI");
  #endif

  convertSmallIntegerRegisterBigIntegerRegister(op1, op1);
  addBigIBigI();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addSmaIBigI");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) + op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void addSmaIRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addSmaIRe16");
  #endif

  convertSmallIntegerRegisterToReal16Register(op1, op1);
  addRe16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addSmaIRe16");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) + op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void addSmaICo16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addSmaICo16");
  #endif

  convertSmallIntegerRegisterToReal16Register(op1, op1);
  addRe16Co16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addSmaICo16");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) + op2(64bits integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addSmaISmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addSmaISmaI");
  #endif

  reallocateRegister(result, dtSmallInteger, SMALL_INTEGER_SIZE, getRegisterBase(op1));
  *(uint64_t *)(POINTER_TO_REGISTER_DATA(result)) = WP34S_intAdd(*(uint64_t *)(POINTER_TO_REGISTER_DATA(op1)), *(uint64_t *)(POINTER_TO_REGISTER_DATA(op2)));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addSmaISmaI");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) + op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addSmaIRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addSmaIRe34");
  #endif

  convertSmallIntegerRegisterToReal34Register(op1, op1);
  addRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addSmaIRe34");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) + op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addSmaICo34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addSmaICo34");
  #endif

  convertSmallIntegerRegisterToReal34Register(op1, op1);
  addRe34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addSmaICo34");
  #endif
}



/********************************************//**
 * \brief op1(real34) + op2(big integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe34BigI");
  #endif

  convertBigIntegerRegisterToReal34Register(op2, op2);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe34BigI");
  #endif
}



/********************************************//**
 * \brief op1(real34) + op2(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe34Re16");
  #endif

  convertRegister16To34(op2);
  addRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe34Re16");
  #endif
}



/********************************************//**
 * \brief op1(real34) + op2(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe34Co16");
  #endif

  convertRegister16To34(op2);
  addRe34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe34Co16");
  #endif
}



/********************************************//**
 * \brief op1(real34) + op2(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Time(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe34Time");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe34Time");
  #endif
}



/********************************************//**
 * \brief op1(real34) + op2(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Date(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe34Date");
  #endif

  addToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe34Date");
  #endif
}



/********************************************//**
 * \brief op1(real34) + op2(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe34SmaI");
  #endif

  convertSmallIntegerRegisterToReal34Register(op2, op2);
  addRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe34SmaI");
  #endif
}



/********************************************//**
 * \brief op1(real34) + op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe34Re34");
  #endif

  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe34Re34");
  #endif
}



/********************************************//**
 * \brief op1(real34) + op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRe34Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addRe34Co34");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // real part
  real34Copy(POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addRe34Co34");
  #endif
}



/********************************************//**
 * \brief op1(complex34) + op2(big integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addCo34BigI");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(result)); // result = op1
  convertBigIntegerRegisterToReal34Register(op2, op2);
  real16Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addCo34BigI");
  #endif
}



/********************************************//**
 * \brief op1(complex34) + op2(real16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addCo34Re16");
  #endif

  convertRegister16To34(op2);
  addCo34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addCo34Re16");
  #endif
}



/********************************************//**
 * \brief op1(complex34) + op2(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addCo34Co16");
  #endif

  convertRegister16To34(op2);
  addCo34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addCo34Co16");
  #endif
}



/********************************************//**
 * \brief op1(complex34) + op2(64bits integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addCo34SmaI");
  #endif

  convertSmallIntegerRegisterToReal34Register(op2, op2);
  addCo34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addCo34SmaI");
  #endif
}



/********************************************//**
 * \brief op1(complex34) + op2(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addCo34Re34");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // real part
  real34Copy(POINTER_TO_REGISTER_DATA(op1) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addCo34Re34");
  #endif
}



/********************************************//**
 * \brief op1(complex34) + op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCo34Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("addCo34Co34");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Add(POINTER_TO_REGISTER_DATA(op1)               , POINTER_TO_REGISTER_DATA(op2)               , POINTER_TO_REGISTER_DATA(result)               ); // real part
  real34Add(POINTER_TO_REGISTER_DATA(op1) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("addCo34Co34");
  #endif
}
