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
// regX |    regY ==>    1            2            3            4         5            6            7            8            9            10            11           12
//      V                Big integer  real16       complex16              Time         Date         String       real16 mat   complex16 m  Small integer real34       complex34
/*  1 Big integer    */ {divBigIBigI, divRe16BigI, divCo16BigI, errorDiv, divTimeBigI, errorDiv,    errorDiv,    divRm16BigI, divCm16BigI, divSmaIBigI,  divRe34BigI, divCo34BigI},
/*  2 real16         */ {divBigIRe16, divRe16Re16, divCo16Re16, errorDiv, divTimeRe16, errorDiv,    errorDiv,    divRm16Re16, divCm16Re16, divSmaIRe16,  divRe34Re16, divCo34Re16},
/*  3 complex16      */ {divBigICo16, divRe16Co16, divCo16Co16, errorDiv, errorDiv,    errorDiv,    errorDiv,    divRm16Co16, divCm16Co16, divSmaICo16,  divRe34Co16, divCo34Co16},
/*  4                */ {errorDiv,    errorDiv,    errorDiv,    errorDiv, errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,     errorDiv,    errorDiv   },
/*  5 Time           */ {errorDiv,    errorDiv,    errorDiv,    errorDiv, errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,     errorDiv,    errorDiv   },
/*  6 Date           */ {errorDiv,    errorDiv,    errorDiv,    errorDiv, errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,     errorDiv,    errorDiv   },
/*  7 String         */ {errorDiv,    errorDiv,    errorDiv,    errorDiv, errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,     errorDiv,    errorDiv   },
/*  8 real16 mat     */ {errorDiv,    errorDiv,    errorDiv,    errorDiv, errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,     errorDiv,    errorDiv   },
/*  9 complex16 mat  */ {errorDiv,    errorDiv,    errorDiv,    errorDiv, errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,    errorDiv,     errorDiv,    errorDiv   },
/* 10 Small integer  */ {divBigISmaI, divRe16SmaI, divCo16SmaI, errorDiv, divTimeSmaI, errorDiv,    errorDiv,    divRm16SmaI, divCm16SmaI, divSmaISmaI,  divRe34SmaI, divCo34SmaI},
/* 11 real34         */ {divBigIRe34, divRe16Re34, divCo16Re34, errorDiv, divTimeRe34, errorDiv,    errorDiv,    divRm16Re34, divCm16Re34, divSmaIRe34,  divRe34Re34, divCo34Re34},
/* 12 complex34      */ {divBigICo34, divRe16Co34, divCo16Co34, errorDiv, errorDiv,    errorDiv,    errorDiv,    divRm16Co34, divCm16Co34, divSmaICo34,  divRe34Co34, divCo34Co34}
};



/********************************************//**
 * \brief Data type error in division
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void errorDiv(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("errorDiv");
  #endif

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot divide %s", getRegisterDataTypeName(op1, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(op2, true, false));
    showInfoDialog("In function fnDivide:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("errorDiv");
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void divToBeCoded(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divToBeCoded");
  #endif

  #ifdef PC_BUILD
    sprintf(errorMessage, "divide %s", getRegisterDataTypeName(op1, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(op2, true, false));
    showInfoDialog("Operation to be coded:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divToBeCoded");
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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnDivide");
  #endif

  if(division[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)] != errorDiv) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    op1    = allocateTemporaryRegister();
    op2    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_Y, op1);
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    saveStack();

    division[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();
    freeTemporaryRegister(op1);
    freeTemporaryRegister(op2);

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

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnDivide");
  #endif
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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnInvert");
  #endif

  saveStack();
  result = REGISTER_X;
  op1    = allocateTemporaryRegister();
  op2    = allocateTemporaryRegister();

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    reallocateRegister(op1, dtReal16, REAL16_SIZE, 0);
    int32ToReal16(1, POINTER_TO_REGISTER_DATA(op1));
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    divRe16Re16();

    if(lastErrorCode != 0) {
      restoreStack();
    }

    refreshStack();
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);

    if(real34IsZero(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnInvert:", "cannot invert 0", NULL, NULL);
      #endif
    }
    else {
      reallocateRegister(op1, dtReal34, REAL34_SIZE, 0);
      int32ToReal34(1, POINTER_TO_REGISTER_DATA(op1));
      copySourceRegisterToDestRegister(REGISTER_X, op2);

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

    reallocateRegister(op1, dtReal16, REAL16_SIZE, 0);
    int32ToReal16(1, POINTER_TO_REGISTER_DATA(op1));
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    divRe16Co16();

    if(lastErrorCode != 0) {
      restoreStack();
    }

    refreshStack();
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    reallocateRegister(op1, dtReal34, REAL34_SIZE, 0);
    int32ToReal34(1, POINTER_TO_REGISTER_DATA(op1));
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    divRe34Re34();

    if(lastErrorCode != 0) {
      restoreStack();
    }

    refreshStack();
  }

  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    reallocateRegister(op1, dtReal34, REAL34_SIZE, 0);
    int32ToReal34(1, POINTER_TO_REGISTER_DATA(op1));
    copySourceRegisterToDestRegister(REGISTER_X, op2);

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

  freeTemporaryRegister(op1);
  freeTemporaryRegister(op2);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnInvert");
  #endif
}



/********************************************//**
 * \brief op1(big integer) ÷ op2(big integer) ==> result(big integer or real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divBigIBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divBigIBigI");
  #endif

  bigInteger_t iOp1;
  bigInteger_t iOp2;

  convertBigIntegerRegisterToBigInteger(op1, &iOp1);
  convertBigIntegerRegisterToBigInteger(op2, &iOp2);

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
      convertBigIntegerRegisterToBigInteger(op1, &iOp1);
      convertBigIntegerRegisterToBigInteger(op2, &iOp2);

      bigIntegerToString(&iOp1, tmpStr3000, 10);
      reallocateRegister(op1, dtReal16, REAL16_SIZE, 0);
      stringToReal16(tmpStr3000, POINTER_TO_REGISTER_DATA(op1));
      bigIntegerToString(&iOp2, tmpStr3000, 10);
      reallocateRegister(op2, dtReal16, REAL16_SIZE, 0);
      stringToReal16(tmpStr3000, POINTER_TO_REGISTER_DATA(op2));
      divRe16Re16();
    }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divBigIBigI");
  #endif
}



/********************************************//**
 * \brief op1(big integer) ÷ op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divBigIRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divBigIRe16");
  #endif

  convertBigIntegerRegisterToReal16Register(op1, op1);

  if(real16IsZero(POINTER_TO_REGISTER_DATA(op1)) && real16IsZero(POINTER_TO_REGISTER_DATA(op2))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, POINTER_TO_REGISTER_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divBigIRe16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(POINTER_TO_REGISTER_DATA(op2))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_plusInfinity, POINTER_TO_REGISTER_DATA(result));
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
    real16Divide(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

    roundRegister(result);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divBigIRe16");
  #endif
}



/********************************************//**
 * \brief op1(big integer) ÷ op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divBigICo16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divBigICo16");
  #endif

  convertBigIntegerRegisterToReal16Register(op1, op1);
  divRe16Co16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divBigICo16");
  #endif
}



/********************************************//**
 * \brief op1(big integer) ÷ op2(small integer) ==> result(big integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divBigISmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divBigISmaI");
  #endif

  convertSmallIntegerRegisterBigIntegerRegister(op2, op2);
  divBigIBigI();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divBigISmaI");
  #endif
}



/********************************************//**
 * \brief op1(big integer) ÷ op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divBigIRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divBigIRe34");
  #endif

  convertBigIntegerRegisterToReal34Register(op1, op1);

  if(real34IsZero(POINTER_TO_REGISTER_DATA(op1)) && real34IsZero(POINTER_TO_REGISTER_DATA(op2))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, POINTER_TO_REGISTER_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divBigIRe34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(POINTER_TO_REGISTER_DATA(op2))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_plusInfinity, POINTER_TO_REGISTER_DATA(result));
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
    real34Divide(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

    roundRegister(result);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divBigIRe34");
  #endif
}



/********************************************//**
 * \brief op1(big integer) ÷ op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divBigICo34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divBigICo34");
  #endif

  convertBigIntegerRegisterToReal34Register(op1, op1);
  divRe34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divBigICo34");
  #endif
}



/********************************************//**
 * \brief op1(real16) ÷ op2(big integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRe16BigI");
  #endif

  convertBigIntegerRegisterToReal16Register(op2, op2);
  divRe16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRe16BigI");
  #endif
}



/********************************************//**
 * \brief op1(real16) ÷ op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRe16Re16");
  #endif

  if(real16IsZero(POINTER_TO_REGISTER_DATA(op1)) && real16IsZero(POINTER_TO_REGISTER_DATA(op2))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, POINTER_TO_REGISTER_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16Re16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(POINTER_TO_REGISTER_DATA(op2))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_plusInfinity, POINTER_TO_REGISTER_DATA(result));
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
    real16Divide(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

    roundRegister(result);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRe16Re16");
  #endif
}



/********************************************//**
 * \brief op1(real16) ÷ op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRe16Co16");
  #endif

  real16_t temp;

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Multiply(POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(op2), &temp); // c*c
  real16FMA(POINTER_TO_REGISTER_DATA(op2) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(op2) + REAL16_SIZE, &temp, &temp); // c*c + d*d

  real16Divide(POINTER_TO_REGISTER_DATA(op2), &temp, POINTER_TO_REGISTER_DATA(result));
  real16Multiply(POINTER_TO_REGISTER_DATA(result), POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(result));
  real16ChangeSign(&temp);
  real16Divide(POINTER_TO_REGISTER_DATA(op2) + REAL16_SIZE, &temp, POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE);
  real16Multiply(POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE);

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRe16Co16");
  #endif
}



/********************************************//**
 * \brief op1(real16) ÷ op2(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRe16SmaI");
  #endif

  convertSmallIntegerRegisterToReal16Register(op2, op2);
  divRe16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRe16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(real16) ÷ op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRe16Re34");
  #endif

  convertRegister16To34(op1);
  divRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRe16Re34");
  #endif
}



/********************************************//**
 * \brief op1(real16) ÷ op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRe16Co34");
  #endif

  convertRegister16To34(op1);
  divRe34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRe16Co34");
  #endif
}



/********************************************//**
 * \brief op1(complex16) ÷ op2(big integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCo16BigI");
  #endif

  convertBigIntegerRegisterToReal16Register(op2, op2);
  divCo16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCo16BigI");
  #endif
}



/********************************************//**
 * \brief op1(complex16) ÷ op2(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCo16Re16");
  #endif

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
  real16Divide(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // real part
  real16Divide(POINTER_TO_REGISTER_DATA(op1) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCo16Re16");
  #endif
}



/********************************************//**
 * \brief op1(complex16) ÷ op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCo16Co16");
  #endif

  real16_t temp;

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);

  // Denominator
  real16Multiply(POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(op2), &temp); // c*c
  real16FMA(POINTER_TO_REGISTER_DATA(op2) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(op2) + REAL16_SIZE, &temp, &temp); // c*c + d*d

  // real part
  real16Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // a*c
  real16FMA(POINTER_TO_REGISTER_DATA(op1) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(op2) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(result), POINTER_TO_REGISTER_DATA(result)); // a*c + b*d
  real16Divide(POINTER_TO_REGISTER_DATA(result), &temp, POINTER_TO_REGISTER_DATA(result)); // (a*c + b*d) / (c*c + d*d)

  // imaginary part
  real16Multiply(POINTER_TO_REGISTER_DATA(op1) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE); // b*c
  real16ChangeSign(POINTER_TO_REGISTER_DATA(op1)); // -a
  real16FMA(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE); // b*c - a*d
  real16Divide(POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE, &temp, POINTER_TO_REGISTER_DATA(result) + REAL16_SIZE); // (b*c - a*d) / (c*c + d*d)

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCo16Co16");
  #endif
}



/********************************************//**
 * \brief op1(complex16) ÷ op2(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCo16SmaI");
  #endif

  convertSmallIntegerRegisterToReal16Register(op2, op2);
  divCo16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCo16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(complex16) ÷ op2(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCo16Re34");
  #endif

  convertRegister16To34(op1);
  divCo34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCo16Re34");
  #endif
}



/********************************************//**
 * \brief op1(complex16) ÷ op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCo16Co34");
  #endif

  convertRegister16To34(op1);
  divCo34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCo16Co34");
  #endif
}



/********************************************//**
 * \brief op1(time) ÷ op2(big integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divTimeBigI");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divTimeBigI");
  #endif
}



/********************************************//**
 * \brief op1(time) ÷ op2(real16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divTimeRe16");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divTimeRe16");
  #endif
}



/********************************************//**
 * \brief op1(time) ÷ op2(64bits integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeSmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divTimeSmaI");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divTimeSmaI");
  #endif
}



/********************************************//**
 * \brief op1(time) ÷ op2(real34) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divTimeRe34");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divTimeRe34");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) ÷ op2(big integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRm16BigI");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRm16BigI");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) ÷ op2(real16) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRm16Re16");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRm16Re16");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) ÷ op2(complex16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRm16Co16");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRm16Co16");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) ÷ op2(64bits integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRm16SmaI");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRm16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) ÷ op2(real34) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRm16Re34");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRm16Re34");
  #endif
}



/********************************************//**
 * \brief op1(real16 matrix) ÷ op2(complex34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRm16Co34");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRm16Co34");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) ÷ op2(big integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCm16BigI");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCm16BigI");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) ÷ op2(real16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCm16Re16");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCm16Re16");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) ÷ op2(complex16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCm16Co16");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCm16Co16");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) ÷ op2(64bits integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCm16SmaI");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCm16SmaI");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) ÷ op2(real34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCm16Re34");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCm16Re34");
  #endif
}



/********************************************//**
 * \brief op1(complex16 matrix) ÷ op2(complex34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCm16Co34");
  #endif

  divToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCm16Co34");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) ÷ op2(big integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divSmaIBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divSmaIBigI");
  #endif

  convertSmallIntegerRegisterBigIntegerRegister(op1, op1);
  divBigIBigI();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divSmaIBigI");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) ÷ op2(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divSmaIRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divSmaIRe16");
  #endif

  convertSmallIntegerRegisterToReal16Register(op1, op1);
  divRe16Re16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divSmaIRe16");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) ÷ op2(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divSmaICo16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divSmaICo16");
  #endif

  convertSmallIntegerRegisterToReal16Register(op1, op1);
  divRe16Co16();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divSmaICo16");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) ÷ op2(64bits integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divSmaISmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divSmaISmaI");
  #endif

  reallocateRegister(result, dtSmallInteger, SMALL_INTEGER_SIZE, getRegisterBase(op1));
  *(uint64_t *)(POINTER_TO_REGISTER_DATA(result)) = WP34S_intDivide(*(uint64_t *)(POINTER_TO_REGISTER_DATA(op1)), *(uint64_t *)(POINTER_TO_REGISTER_DATA(op2)));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divSmaISmaI");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) ÷ op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divSmaIRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divSmaIRe34");
  #endif

  convertSmallIntegerRegisterToReal34Register(op1, op1);
  divRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divSmaIRe34");
  #endif
}



/********************************************//**
 * \brief op1(64bits integer) ÷ op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divSmaICo34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divSmaICo34");
  #endif

  convertSmallIntegerRegisterToReal34Register(op1, op1);
  divRe34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divSmaICo34");
  #endif
}



/********************************************//**
 * \brief op1(real34) ÷ op2(big integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRe34BigI");
  #endif

  convertBigIntegerRegisterToReal34Register(op2, op2);
  divRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRe34BigI");
  #endif
}



/********************************************//**
 * \brief op1(real34) ÷ op2(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRe34Re16");
  #endif

  convertRegister16To34(op2);
  divRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRe34Re16");
  #endif
}



/********************************************//**
 * \brief op1(real34) ÷ op2(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRe34Co16");
  #endif

  convertRegister16To34(op2);
  divRe34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRe34Co16");
  #endif
}



/********************************************//**
 * \brief op1(real34) ÷ op2(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRe34SmaI");
  #endif

  convertSmallIntegerRegisterToReal34Register(op2, op2);
  divRe34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRe34SmaI");
  #endif
}



/********************************************//**
 * \brief op1(real34) ÷ op2(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRe34Re34");
  #endif

  if(real34IsZero(POINTER_TO_REGISTER_DATA(op1)) && real34IsZero(POINTER_TO_REGISTER_DATA(op2))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, POINTER_TO_REGISTER_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34Re34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(POINTER_TO_REGISTER_DATA(op2))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_plusInfinity, POINTER_TO_REGISTER_DATA(result));
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
    real34Divide(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

    roundRegister(result);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRe34Re34");
  #endif
}



/********************************************//**
 * \brief op1(real34) ÷ op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divRe34Co34");
  #endif

  real34_t temp;

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);

  // Denominator
  real34Multiply(POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(op2), &temp); // c*c
  real34FMA(POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, &temp, &temp); // c*c + d*d

  // Real part
  real34Divide(POINTER_TO_REGISTER_DATA(op1), &temp, POINTER_TO_REGISTER_DATA(result));
  real34Multiply(POINTER_TO_REGISTER_DATA(result), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result));

  // Imaginary part
  real34ChangeSign(&temp);
  real34Divide(POINTER_TO_REGISTER_DATA(op1), &temp, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE);
  real34Multiply(POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE);

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divRe34Co34");
  #endif
}



/********************************************//**
 * \brief op1(complex34) ÷ op2(big integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCo34BigI");
  #endif

  convertBigIntegerRegisterToReal34Register(op2, op2);
  divCo34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCo34BigI");
  #endif
}



/********************************************//**
 * \brief op1(complex34) ÷ op2(real16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCo34Re16");
  #endif

  convertRegister16To34(op2);
  divCo34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCo34Re16");
  #endif
}



/********************************************//**
 * \brief op1(complex34) ÷ op2(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCo34Co16");
  #endif

  convertRegister16To34(op2);
  divCo34Co34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCo34Co16");
  #endif
}



/********************************************//**
 * \brief op1(complex34) ÷ op2(64bits integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCo34SmaI");
  #endif

  convertSmallIntegerRegisterToReal34Register(op2, op2);
  divCo34Re34();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCo34SmaI");
  #endif
}



/********************************************//**
 * \brief op1(complex34) ÷ op2(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCo34Re34");
  #endif

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Divide(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // real part
  real34Divide(POINTER_TO_REGISTER_DATA(op1) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE); // imaginary part

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCo34Re34");
  #endif
}



/********************************************//**
 * \brief op1(complex34) ÷ op2(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("divCo34Co34");
  #endif

  real34_t temp;

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);

  // Denominator
  real34Multiply(POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(op2), &temp); // c*c
  real34FMA(POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, &temp, &temp); // c*c + d*d

  // real part
  real34Multiply(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result)); // a*c
  real34FMA(POINTER_TO_REGISTER_DATA(op1) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result), POINTER_TO_REGISTER_DATA(result)); // a*c + b*d
  real34Divide(POINTER_TO_REGISTER_DATA(result), &temp, POINTER_TO_REGISTER_DATA(result)); // (a*c + b*d) / (c*c + d*d)

  // imaginary part
  real34Multiply(POINTER_TO_REGISTER_DATA(op1) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(op2), POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE); // b*c
  real34ChangeSign(POINTER_TO_REGISTER_DATA(op1)); // -a
  real34FMA(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(op2) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE); // b*c - a*d
  real34Divide(POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE, &temp, POINTER_TO_REGISTER_DATA(result) + REAL34_SIZE); // (b*c - a*d) / (c*c + d*d)

  roundRegister(result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("divCo34Co34");
  #endif
}
