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
//      V                Long integer real16       complex16    angle34      Time         Date         String       real16 mat   complex16 m  Short integer real34       complex34
/*  1 Long integer   */ {divLonILonI, divRe16LonI, divCo16LonI, divAnglLonI, divTimeLonI, divError,    divError,    divRm16LonI, divCm16LonI, divShoILonI,  divRe34LonI, divCo34LonI},
/*  2 real16         */ {divLonIRe16, divRe16Re16, divCo16Re16, divAnglRe16, divTimeRe16, divError,    divError,    divRm16Re16, divCm16Re16, divShoIRe16,  divRe34Re16, divCo34Re16},
/*  3 complex16      */ {divLonICo16, divRe16Co16, divCo16Co16, divError,    divError,    divError,    divError,    divRm16Co16, divCm16Co16, divShoICo16,  divRe34Co16, divCo34Co16},
/*  4 angle34        */ {divError,    divError,    divError,    divAnglAngl, divError,    divError,    divError,    divError,    divError,    divError,     divError,    divError   },
/*  5 Time           */ {divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,     divError,    divError   },
/*  6 Date           */ {divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,     divError,    divError   },
/*  7 String         */ {divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,     divError,    divError   },
/*  8 real16 mat     */ {divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,     divError,    divError   },
/*  9 complex16 mat  */ {divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,    divError,     divError,    divError   },
/* 10 Short integer  */ {divLonIShoI, divRe16ShoI, divCo16ShoI, divAnglShoI, divTimeShoI, divError,    divError,    divRm16ShoI, divCm16ShoI, divShoIShoI,  divRe34ShoI, divCo34ShoI},
/* 11 real34         */ {divLonIRe34, divRe16Re34, divCo16Re34, divAnglRe34, divTimeRe34, divError,    divError,    divRm16Re34, divCm16Re34, divShoIRe34,  divRe34Re34, divCo34Re34},
/* 12 complex34      */ {divLonICo34, divRe16Co34, divCo16Co34, divError,    divError,    divError,    divError,    divRm16Co34, divCm16Co34, divShoICo34,  divRe34Co34, divCo34Co34}
};



/********************************************//**
 * \brief Data type error in division
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void divError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot divide %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
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
    sprintf(errorMessage, "divide %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
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
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opY    = allocateTemporaryRegister();
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_Y, opY);
  copySourceRegisterToDestRegister(REGISTER_X, opX);

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



/********************************************//**
 * \brief opY(long integer) ÷ opX(long integer) ==> result(long integer or real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonILonI(void) {
  longInteger_t iOp1;
  longInteger_t iOp2;

  convertLongIntegerRegisterToLongInteger(opY, &iOp1);
  convertLongIntegerRegisterToLongInteger(opX, &iOp2);

  if(longIntegerIsZero(&iOp2)) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonILonI:", "cannot divide a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longIntegerDivideRemainder(&iOp1, &iOp2, &iOp1, &iOp2);

    if(longIntegerCompareUInt(&iOp2, 0) == LONG_INTEGER_EQUAL) {
      convertLongIntegerToLongIntegerRegister(&iOp1, result);
    }
    else {
      convertLongIntegerRegisterToLongInteger(opY, &iOp1);
      convertLongIntegerRegisterToLongInteger(opX, &iOp2);

      longIntegerToString(&iOp1, tmpStr3000, 10);
      reallocateRegister(opY, dtReal16, REAL16_SIZE, 0);
      stringToReal16(tmpStr3000, REGISTER_REAL16_DATA(opY));
      longIntegerToString(&iOp2, tmpStr3000, 10);
      reallocateRegister(opX, dtReal16, REAL16_SIZE, 0);
      stringToReal16(tmpStr3000, REGISTER_REAL16_DATA(opX));
      divRe16Re16();
    }
  }
}



/********************************************//**
 * \brief opY(long integer) ÷ opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonIRe16(void) {
  convertLongIntegerRegisterToReal16Register(opY, opY);
  divRe16Re16();
}



/********************************************//**
 * \brief opY(long integer) ÷ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonICo16(void) {
  convertLongIntegerRegisterToReal16Register(opY, opY);
  divRe16Co16();
}



/********************************************//**
 * \brief opY(long integer) ÷ opX(short integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonIShoI(void) {
  convertShortIntegerRegisterLongIntegerRegister(opX, opX);
  divLonILonI();
}



/********************************************//**
 * \brief opY(long integer) ÷ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonIRe34(void) {
  convertLongIntegerRegisterToReal34Register(opY, opY);
  divRe34Re34();
}



/********************************************//**
 * \brief opY(long integer) ÷ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonICo34(void) {
  convertLongIntegerRegisterToReal34Register(opY, opY);
  divRe34Co34();
}



/********************************************//**
 * \brief opY(real16) ÷ opX(long integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16LonI(void) {
  convertLongIntegerRegisterToReal16Register(opX, opX);
  divRe16Re16();
}



/********************************************//**
 * \brief opY(real16) ÷ opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16Re16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(opY)) && real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16Re16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      if(real16IsPositive(REGISTER_REAL16_DATA(opY))) {
        real16Copy(const16_plusInfinity, REGISTER_REAL16_DATA(result));
      }
      else {
        real16Copy(const16_minusInfinity, REGISTER_REAL16_DATA(result));
      }
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
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
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16Co16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

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
void divRe16ShoI(void) {
  convertShortIntegerRegisterToReal16Register(opX, opX);
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
 * \brief opY(complex16) ÷ opX(long integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16LonI(void) {
  convertLongIntegerRegisterToReal16Register(opX, opX);
  divCo16Re16();
}



/********************************************//**
 * \brief opY(complex16) ÷ opX(real16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16Re16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

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
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16Co16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

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
void divCo16ShoI(void) {
  convertShortIntegerRegisterToReal16Register(opX, opX);
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
 * \brief opY(angle) ÷ opX(long integer) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void divAnglLonI(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAnglLonI:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertLongIntegerRegisterToAngleRegister(opX, opX);
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
  if(real16IsNaN(REGISTER_ANGLE_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAnglRe16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

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
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY)) || angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAnglAngl:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  angleDivide(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  #if (ANGLE34 == 1)
    convertRegister34To16(result);
  #endif
  setRegisterDataType(result, dtReal16);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(angle) ÷ opX(short integer) ==> result(angle)
 *
 * \param void
 * \return void
 ***********************************************/
void divAnglShoI(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAnglShoI:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  convertShortIntegerRegisterToAngleRegister(opX, opX);
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
  if(angleIsNaN(REGISTER_ANGLE_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAnglRe34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtAngle, ANGLE_SIZE, 0);
  #if (ANGLE16 == 1)
    convertRegister34To16(opX);
  #endif
  angleDivide(REGISTER_ANGLE_DATA(opY), REGISTER_ANGLE_DATA(opX), REGISTER_ANGLE_DATA(result));
  setRegisterAngularMode(result, angularMode);

  roundRegister(result);
}



/********************************************//**
 * \brief opY(time) ÷ opX(long integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeLonI(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(time) ÷ opX(real16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divTimeRe16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  divToBeCoded();
}



/********************************************//**
 * \brief opY(time) ÷ opX(64bits integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeShoI(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(time) ÷ opX(real34) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divTimeRe34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  divToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(long integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16LonI(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(real16) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRm16Re16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  divToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(complex16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRm16Co16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  divToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(64bits integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16ShoI(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(real34) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRm16Re34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  divToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(complex34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRm16Co34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  divToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(long integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16LonI(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(real16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCm16Re16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  divToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(complex16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCm16Co16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  divToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(64bits integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16ShoI(void) {
  divToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(real34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCm16Re34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  divToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(complex34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCm16Co34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  divToBeCoded();
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(long integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoILonI(void) {
  convertShortIntegerRegisterLongIntegerRegister(opY, opY);
  divLonILonI();
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIRe16(void) {
  convertShortIntegerRegisterToReal16Register(opY, opY);
  divRe16Re16();
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoICo16(void) {
  convertShortIntegerRegisterToReal16Register(opY, opY);
  divRe16Co16();
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(64bits integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIShoI(void) {
  reallocateRegister(result, dtShortInteger, SHORT_INTEGER_SIZE, getRegisterBase(opY));
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_intDivide(*(REGISTER_SHORT_INTEGER_DATA(opY)), *(REGISTER_SHORT_INTEGER_DATA(opX)));
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIRe34(void) {
  convertShortIntegerRegisterToReal34Register(opY, opY);
  divRe34Re34();
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoICo34(void) {
  convertShortIntegerRegisterToReal34Register(opY, opY);
  divRe34Co34();
}



/********************************************//**
 * \brief opY(real34) ÷ opX(long integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34LonI(void) {
  convertLongIntegerRegisterToReal34Register(opX, opX);
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
void divRe34ShoI(void) {
  convertShortIntegerRegisterToReal34Register(opX, opX);
  divRe34Re34();
}



/********************************************//**
 * \brief opY(real34) ÷ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34Re34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34Re34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      if(real34IsPositive(REGISTER_REAL34_DATA(opY))) {
        real34Copy(const34_plusInfinity, REGISTER_REAL34_DATA(result));
      }
      else {
        real34Copy(const34_minusInfinity, REGISTER_REAL34_DATA(result));
      }
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
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
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34Co34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

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
 * \brief opY(complex34) ÷ opX(long integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34LonI(void) {
  convertLongIntegerRegisterToReal34Register(opX, opX);
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
void divCo34ShoI(void) {
  convertShortIntegerRegisterToReal34Register(opX, opX);
  divCo34Re34();
}



/********************************************//**
 * \brief opY(complex34) ÷ opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34Re34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

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
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34Co34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

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
