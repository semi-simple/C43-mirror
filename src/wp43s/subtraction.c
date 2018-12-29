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
// regX |    regY ==>    1            2            3            4         5            6            7            8            9            10            11           12
//      V                Big integer  real16       complex16              Time         Date         String       real16 mat   complex16 m  Small integer real34       complex34
/*  1 Big integer    */ {subBigIBigI, subRe16BigI, subCo16BigI, errorSub, subTimeBigI, subDateBigI, errorSub,    errorSub,    errorSub,    subSmaIBigI,  subRe34BigI, subCo34BigI},
/*  2 real16         */ {subBigIRe16, subRe16Re16, subCo16Re16, errorSub, subTimeRe16, subDateRe16, errorSub,    errorSub,    errorSub,    subSmaIRe16,  subRe34Re16, subCo34Re16},
/*  3 complex16      */ {subBigICo16, subRe16Co16, subCo16Co16, errorSub, errorSub,    errorSub,    errorSub,    errorSub,    errorSub,    subSmaICo16,  subRe34Co16, subCo34Co16},
/*  4                */ {errorSub,    errorSub,    errorSub,    errorSub, errorSub,    errorSub,    errorSub,    errorSub,    errorSub,    errorSub,     errorSub,    errorSub   },
/*  5 Time           */ {subBigITime, subRe16Time, errorSub,    errorSub, subTimeTime, errorSub,    errorSub,    errorSub,    errorSub,    errorSub,     subRe34Time, errorSub   },
/*  6 Date           */ {subBigIDate, subRe16Date, errorSub,    errorSub, errorSub,    subDateDate, errorSub,    errorSub,    errorSub,    errorSub,     subRe34Date, errorSub   },
/*  7 String         */ {errorSub,    errorSub,    errorSub,    errorSub, errorSub,    errorSub,    errorSub,    errorSub,    errorSub,    errorSub,     errorSub,    errorSub   },
/*  8 real16 mat     */ {errorSub,    errorSub,    errorSub,    errorSub, errorSub,    errorSub,    errorSub,    subRm16Rm16, subCm16Rm16, errorSub,     errorSub,    errorSub   },
/*  9 complex16 mat  */ {errorSub,    errorSub,    errorSub,    errorSub, errorSub,    errorSub,    errorSub,    subRm16Cm16, subCm16Cm16, errorSub,     errorSub,    errorSub   },
/* 10 Small integer  */ {subBigISmaI, subRe16SmaI, subCo16SmaI, errorSub, errorSub,    errorSub,    errorSub,    errorSub,    errorSub,    subSmaISmaI,  subRe34SmaI, subCo34SmaI},
/* 11 real34         */ {subBigIRe34, subRe16Re34, subCo16Re34, errorSub, subTimeRe34, subDateRe34, errorSub,    errorSub,    errorSub,    subSmaIRe34,  subRe34Re34, subCo34Re34},
/* 12 complex34      */ {subBigICo34, subRe16Co34, subCo16Co34, errorSub, errorSub,    errorSub,    errorSub,    errorSub,    errorSub,    subSmaICo34,  subRe34Co34, subCo34Co34}
};



/********************************************//**
 * \brief Data type error in subtraction
 *
 * \param void
 * \return void
 ***********************************************/
void errorSub(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("errorSub");
  #endif

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot subtract %s", getRegisterDataTypeName(op2, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "from %s", getRegisterDataTypeName(op1, true, false));
    showInfoDialog("In function fnSubtract:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("errorSub");
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void subToBeCoded(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subToBeCoded");
  #endif

  #ifdef PC_BUILD
    sprintf(errorMessage, "subtract %s", getRegisterDataTypeName(op2, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(op1, true, false));
    showInfoDialog("Operation to be coded:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subToBeCoded");
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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnSubtract");
  #endif

  if(subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)] != errorSub) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    op1    = allocateTemporaryRegister();
    op2    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_Y, op1);
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();
    freeTemporaryRegister(op1);
    freeTemporaryRegister(op2);

    fnDropY(NOPARAM);
    refreshStack();
  }
  else {
    errorSub();
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnSubtract");
  #endif
}



/********************************************//**
 * \brief rexX ==> regL and -regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnChangeSign(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnChangeSign");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    //copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real16ChangeSign(POINTER_TO_REGISTER_DATA(REGISTER_X));
    if(real16IsZero(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
      real16SetPositiveSign(POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    refreshRegisterLine(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    //copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real34ChangeSign(POINTER_TO_REGISTER_DATA(REGISTER_X));
    if(real34IsZero(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
      real34SetPositiveSign(POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    refreshRegisterLine(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    //copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real16ChangeSign(POINTER_TO_REGISTER_DATA(REGISTER_X));
    if(real16IsZero(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
      real16SetPositiveSign(POINTER_TO_REGISTER_DATA(REGISTER_X));
    }

    real16ChangeSign(POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL16_SIZE);
    if(real16IsZero(POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL16_SIZE)) {
      real16SetPositiveSign(POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL16_SIZE);
    }

    refreshRegisterLine(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    //copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real34ChangeSign(POINTER_TO_REGISTER_DATA(REGISTER_X));
    if(real34IsZero(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
      real34SetPositiveSign(POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    real34ChangeSign(POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL34_SIZE);

    if(real34IsZero(POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL34_SIZE)) {
      real34SetPositiveSign(POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL34_SIZE);
    }

    refreshRegisterLine(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    //copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    setRegisterDataInfo(REGISTER_X, getRegisterDataInfo(REGISTER_X) ^ 1);
    refreshRegisterLine(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    //copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_intChs(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
    refreshRegisterLine(REGISTER_X);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
     sprintf(errorMessage, "Cannot change the sign of %s", getRegisterDataTypeName(REGISTER_X, true, false));
     showInfoDialog("In function fnChangeSign:", errorMessage, NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnChangeSign");
  #endif
}



/********************************************//**
 * \brief op1(big integer) - op2(big integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigIBigI(void) {
  bigInteger_t iOp1;
  bigInteger_t iOp2;

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subBigIBigI");
  #endif

  convertBigIntegerRegisterToBigInteger(op1, &iOp1);
  convertBigIntegerRegisterToBigInteger(op2, &iOp2);

  bigIntegerSubtract(&iOp1, &iOp2, &iOp1);

  convertBigIntegerToBigIntegerRegister(&iOp1, result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subBigIBigI");
  #endif
}



/********************************************//**
 * \brief op1(big integer) - op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigIRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subBigIRe16");
  #endif

  convertBigIntegerRegisterToReal16Register(op1, op1);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subBigIRe16");
  #endif
}



/********************************************//**
 * \brief op1(big integer) - op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigICo16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subBigICo16");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // result = op2
  convertBigIntegerRegisterToReal16Register(op1, op1);
  real16Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subBigICo16");
  #endif
}



/********************************************//**
 * \brief op1(big integer) - op2(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigITime(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subBigITime");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subBigITime");
  #endif
}



/********************************************//**
 * \brief op1(big integer) - op2(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigIDate(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subBigIDate");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subBigIDate");
  #endif
}



/********************************************//**
 * \brief op1(big integer) - op2(64bits integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigISmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subBigISmaI");
  #endif

  convertSmallIntegerRegisterBigIntegerRegister(op2, op2);
  subBigIBigI();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subBigISmaI");
  #endif
}



/********************************************//**
 * \brief op1(big integer) - op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigIRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subBigIRe34");
  #endif

  convertBigIntegerRegisterToReal34Register(op1, op1);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subBigIRe34");
  #endif
}



/********************************************//**
 * \brief op1(big integer) - op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subBigICo34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subBigICo34");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // result = op2
  convertBigIntegerRegisterToReal34Register(op1, op1);
  real16Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subBigICo34");
  #endif
}



/********************************************//**
 * \brief op1(real16) - op2(big integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe16BigI");
  #endif

  convertBigIntegerRegisterToReal16Register(op2, op2);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe16BigI");
  #endif
}



/********************************************//**
 * \brief op1(real16) - op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe16Re16");
  #endif

  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe16Re16");
  #endif
}



/********************************************//**
 * \brief op1(real16) - op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe16Co16");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // result = op2
  complex16ChangeSign(POINTER_TO_REGISTER_DATA(result)); // result = -result
  real16Add(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(result), POINTER_TO_REGISTER_DATA(result)); // real part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe16Co16");
  #endif
}



/********************************************//**
 * \brief op1(real16) - op2(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Time(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe16Time");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe16Time");
  #endif
}



/********************************************//**
 * \brief op1(real16) - op2(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Date(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe16Date");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe16Date");
  #endif
}



/********************************************//**
 * \brief op1(real16) - op2(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe16SmaI");
  #endif

  convertSmallIntegerRegisterToReal16Register(op2, op2);
  subRe16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(real16) - op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe16Re34");
  #endif

  convertRegister16To34(op1);
  subRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe16Re34");
  #endif
}



/********************************************//**
 * \brief op1(real16) - op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe16Co34");
  #endif

  convertRegister16To34(op1);
  subRe34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe16Co34");
  #endif
}



/********************************************//**
 * \brief op1(complex16) - op2(big integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subCo16BigI");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  complex16Copy(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(result)); // result = op2
  convertBigIntegerRegisterToReal16Register(op2, op2);
  real16Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subCo16BigI");
  #endif
}



/********************************************//**
 * \brief op1(complex16) - op2(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subCo16Re16");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // real part
  real16Copy(POINTER_TO_REGISTER_DATA(op1) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subCo16Re16");
  #endif
}



/********************************************//**
 * \brief op1(complex16) - op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subCo16Co16");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // real part
  real16Subtract(POINTER_TO_REGISTER_DATA(op1) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(op2) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subCo16Co16");
  #endif
}



/********************************************//**
 * \brief op1(complex16) - op2(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subCo16SmaI");
  #endif

  convertSmallIntegerRegisterToReal16Register(op2, op2);
  subCo16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subCo16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(complex16) - op2(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subCo16Re34");
  #endif

  convertRegister16To34(op1);
  subCo34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subCo16Re34");
  #endif
}



/********************************************//**
 * \brief op1(complex16) - op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subCo16Co34");
  #endif

  convertRegister16To34(op1);
  subCo34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subCo16Co34");
  #endif
}



/********************************************//**
 * \brief op1(time) - op2(big integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subTimeBigI");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subTimeBigI");
  #endif
}



/********************************************//**
 * \brief op1(time) - op2(real16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subTimeRe16");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subTimeRe16");
  #endif
}



/********************************************//**
 * \brief op1(time) - op2(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeTime(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subTimeTime");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subTimeTime");
  #endif
}



/********************************************//**
 * \brief op1(time) - op2(real34) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subTimeRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subTimeRe34");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subTimeRe34");
  #endif
}



/********************************************//**
 * \brief op1(date) - op2(big integer) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subDateBigI");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subDateBigI");
  #endif
}



/********************************************//**
 * \brief op1(date) - op2(real16) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subDateRe16");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subDateRe16");
  #endif
}



/********************************************//**
 * \brief op1(date) - op2(date) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateDate(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subDateDate");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subDateDate");
  #endif
}



/********************************************//**
 * \brief op1(date) - op2(real34) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subDateRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subDateRe34");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subDateRe34");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) - op2(real16 matrix) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subRm16Rm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRm16Rm16");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRm16Rm16");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) - op2(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subRm16Cm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRm16Cm16");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRm16Cm16");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) - op2(real16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subCm16Rm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subCm16Rm16");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subCm16Rm16");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) - op2(complex16 matrix) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void subCm16Cm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subCm16Cm16");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subCm16Cm16");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) - op2(big integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subSmaIBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subSmaIBigI");
  #endif

  convertSmallIntegerRegisterBigIntegerRegister(op1, op1);
  subBigIBigI();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subSmaIBigI");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) - op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void subSmaIRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subSmaIRe16");
  #endif

  convertSmallIntegerRegisterToReal16Register(op1, op1);
  subRe16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subSmaIRe16");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) - op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void subSmaICo16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subSmaICo16");
  #endif

  convertSmallIntegerRegisterToReal16Register(op1, op1);
  subRe16Co16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subSmaICo16");
  #endif
}



/********************************************//**
 * \brief op1(Small integer) - op2(Small integer) ==> result(Small integer)
 *
 * \param void
 * \return void
 ***********************************************/
void subSmaISmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subSmaISmaI");
  #endif

  reallocateRegister(result, dtSmallInteger, SMALL_INTEGER_SIZE, getRegisterBase(op1));
  *(uint64_t *)(POINTER_TO_REGISTER_DATA(result)) = WP34S_intSubtract(*(uint64_t *)(POINTER_TO_REGISTER_DATA(op1)), *(uint64_t *)(POINTER_TO_REGISTER_DATA(op2)));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subSmaISmaI");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) - op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subSmaIRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subSmaIRe34");
  #endif

  convertSmallIntegerRegisterToReal34Register(op1, op1);
  subRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subSmaIRe34");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) - op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subSmaICo34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subSmaICo34");
  #endif

  convertSmallIntegerRegisterToReal34Register(op1, op1);
  subRe34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subSmaICo34");
  #endif
}



/********************************************//**
 * \brief op1(real34) - op2(big integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe34BigI");
  #endif

  convertBigIntegerRegisterToReal34Register(op2, op2);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe34BigI");
  #endif
}



/********************************************//**
 * \brief op1(real34) - op2(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe34Re16");
  #endif

  convertRegister16To34(op2);
  subRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe34Re16");
  #endif
}



/********************************************//**
 * \brief op1(real34) - op2(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe34Co16");
  #endif

  convertRegister16To34(op2);
  subRe34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe34Co16");
  #endif
}



/********************************************//**
 * \brief op1(real34) - op2(time) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Time(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe34Time");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe34Time");
  #endif
}



/********************************************//**
 * \brief op1(real34) - op2(date) ==> result(date)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Date(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe34Date");
  #endif

  subToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe34Date");
  #endif
}



/********************************************//**
 * \brief op1(real34) - op2(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe34SmaI");
  #endif

  convertSmallIntegerRegisterToReal34Register(op2, op2);
  subRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe34SmaI");
  #endif
}



/********************************************//**
 * \brief op1(real34) - op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe34Re34");
  #endif

  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe34Re34");
  #endif
}



/********************************************//**
 * \brief op1(real34) - op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subRe34Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subRe34Co34");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // real part
  real34Copy(POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE); // imaginary part
  real34ChangeSign(POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE);

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subRe34Co34");
  #endif
}



/********************************************//**
 * \brief op1(complex34) - op2(big integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subCo34BigI");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(result)); // result = op1
  convertBigIntegerRegisterToReal34Register(op2, op2);
  real16Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subCo34BigI");
  #endif
}



/********************************************//**
 * \brief op1(complex34) - op2(real16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subCo34Re16");
  #endif

  convertRegister16To34(op2);
  subCo34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subCo34Re16");
  #endif
}



/********************************************//**
 * \brief op1(complex34) - op2(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subCo34Co16");
  #endif

  convertRegister16To34(op2);
  subCo34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subCo34Co16");
  #endif
}



/********************************************//**
 * \brief op1(complex34) - op2(64bits integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subCo34SmaI");
  #endif

  convertSmallIntegerRegisterToReal34Register(op2, op2);
  subCo34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subCo34SmaI");
  #endif
}



/********************************************//**
 * \brief op1(complex34) - op2(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subCo34Re34");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // real part
  real34Copy(POINTER_TO_REGISTER_DATA(op1) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subCo34Re34");
  #endif
}



/********************************************//**
 * \brief op1(complex34) - op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void subCo34Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subCo34Co34");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Subtract(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // real part
  real34Subtract(POINTER_TO_REGISTER_DATA(op1) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subCo34Co34");
  #endif
}
