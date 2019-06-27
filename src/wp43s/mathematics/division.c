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



void (* const division[13][13])(void) = {
// regX |    regY ==>   1            2            3            4            5            6         7         8            9            10            11           12           13
//      V               Long integer Real16       Complex16    Angle16      Time         Date      String    Real16 mat   Complex16 m  Short integer Real34       Complex34    Angle34
/*  1 Long integer  */ {divLonILonI, divRe16LonI, divCo16LonI, divAn16LonI, divTimeLonI, divError, divError, divRm16LonI, divCm16LonI, divShoILonI,  divRe34LonI, divCo34LonI, divAn34LonI},
/*  2 Real16        */ {divLonIRe16, divRe16Re16, divCo16Re16, divAn16Re16, divTimeRe16, divError, divError, divRm16Re16, divCm16Re16, divShoIRe16,  divRe34Re16, divCo34Re16, divAn34Re16},
/*  3 Complex16     */ {divLonICo16, divRe16Co16, divCo16Co16, divAn16Co16, divError,    divError, divError, divRm16Co16, divCm16Co16, divShoICo16,  divRe34Co16, divCo34Co16, divAn34Co16},
/*  4 Angle16       */ {divLonIAn16, divRe16An16, divCo16An16, divAn16An16, divTimeAn16, divError, divError, divRm16An16, divCm16An16, divShoIAn16,  divRe34An16, divCo34An16, divAn34An16},
/*  5 Time          */ {divError,    divError,    divError,    divError,    divError,    divError, divError, divError,    divError,    divError,     divError,    divError,    divError   },
/*  6 Date          */ {divError,    divError,    divError,    divError,    divError,    divError, divError, divError,    divError,    divError,     divError,    divError,    divError   },
/*  7 String        */ {divError,    divError,    divError,    divError,    divError,    divError, divError, divError,    divError,    divError,     divError,    divError,    divError   },
/*  8 Real16 mat    */ {divError,    divError,    divError,    divError,    divError,    divError, divError, divError,    divError,    divError,     divError,    divError,    divError   },
/*  9 Complex16 mat */ {divError,    divError,    divError,    divError,    divError,    divError, divError, divError,    divError,    divError,     divError,    divError,    divError   },
/* 10 Short integer */ {divLonIShoI, divRe16ShoI, divCo16ShoI, divAn16ShoI, divTimeShoI, divError, divError, divRm16ShoI, divCm16ShoI, divShoIShoI,  divRe34ShoI, divCo34ShoI, divAn34ShoI},
/* 11 Real34        */ {divLonIRe34, divRe16Re34, divCo16Re34, divAn16Re34, divTimeRe34, divError, divError, divRm16Re34, divCm16Re34, divShoIRe34,  divRe34Re34, divCo34Re34, divAn34Re34},
/* 12 Complex34     */ {divLonICo34, divRe16Co34, divCo16Co34, divAn16Co34, divError,    divError, divError, divRm16Co34, divCm16Co34, divShoICo34,  divRe34Co34, divCo34Co34, divAn34Co34},
/* 13 Angle34       */ {divLonIAn34, divRe16An34, divCo16An34, divAn16An34, divTimeAn34, divError, divError, divRm16An34, divCm16An34, divShoIAn34,  divRe34An34, divCo34An34, divAn34An34}
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

  adjustResult(result, true, true, opX, opY, -1);
}



/******************************************************************************************************************************************************************************************/
/* long integer / ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

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
      reallocateRegister(opY, dtReal16, REAL16_SIZE, TAG_NONE);
      stringToReal16(tmpStr3000, REGISTER_REAL16_DATA(opY));
      longIntegerToString(&iOp2, tmpStr3000, 10);
      reallocateRegister(opX, dtReal16, REAL16_SIZE, TAG_NONE);
      stringToReal16(tmpStr3000, REGISTER_REAL16_DATA(opX));

      reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
      real16Divide(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
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
  convertLongIntegerRegisterToReal34Register(opY, opY);

  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonIRe16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divLonIRe16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const16_plusInfinity : const16_minusInfinity), REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divLonIRe16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    convertRegister16To34(opX);
    reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(real16) ÷ opX(long integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16LonI(void) {
  convertLongIntegerRegisterToReal34Register(opX, opX);

  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16LonI:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);

  if(real16IsZero(REGISTER_REAL16_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16LonI:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(opY)) ? const16_plusInfinity : const16_minusInfinity), REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16LonI:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    convertRegister16To34(opY);
    reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(long integer) ÷ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonICo16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t temp;

  convertLongIntegerRegisterToReal34Register(opY, opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX), &temp); // c*c
  real34FMA(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX), &temp, &temp); // c*c + d*d

  real34Divide(REGISTER_REAL34_DATA(opX), &temp, REGISTER_REAL34_DATA(result));
  real34Multiply(REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(result));
  real34ChangeSign(&temp);
  real34Divide(REGISTER_IMAG34_DATA(opX), &temp, REGISTER_IMAG34_DATA(result));
  real34Multiply(REGISTER_IMAG34_DATA(result), REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
}



/********************************************//**
 * \brief opY(complex16) ÷ opX(long integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16LonI:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Divide(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Divide(REGISTER_IMAG16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(long integer) ÷ opX(angle16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonIAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonIAn16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
  convertLongIntegerRegisterToReal16Register(opY, opY);

  if(real16IsZero(REGISTER_REAL16_DATA(opY)) && real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divLonIAn16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(opY)) ? const16_plusInfinity : const16_minusInfinity), REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divLonIAn16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real16Divide(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle16) ÷ opX(long integer) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16LonI:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtAngle16, REAL16_SIZE, currentAngularMode);

  if(real16IsZero(REGISTER_REAL16_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16LonI:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(opY)) ? const16_plusInfinity : const16_minusInfinity), REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16LonI:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    convertRegister16To34(opY);
    reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

    if(currentAngularMode == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), AM_DEGREE);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
      convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
      convertRegister34To16(result);
      checkDms16(REGISTER_REAL16_DATA(result));
    }
    else {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), currentAngularMode);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
      convertRegister34To16(result);
    }
  }
}



/********************************************//**
 * \brief opY(long integer) ÷ opX(short integer) ==> result(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonIShoI(void) {
  longInteger_t iOp1;
  longInteger_t iOp2;

  convertShortIntegerRegisterLongIntegerRegister(opX, opX);
  convertLongIntegerRegisterToLongInteger(opY, &iOp1);
  convertLongIntegerRegisterToLongInteger(opX, &iOp2);

  if(longIntegerIsZero(&iOp2)) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonIShoI:", "cannot divide a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longIntegerDivideRemainder(&iOp1, &iOp2, &iOp1, &iOp2);
    convertLongIntegerToLongIntegerRegister(&iOp1, result);
  }
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(long integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoILonI(void) {
  longInteger_t iOp1;
  longInteger_t iOp2;

  convertShortIntegerRegisterLongIntegerRegister(opY, opY);
  convertLongIntegerRegisterToLongInteger(opY, &iOp1);
  convertLongIntegerRegisterToLongInteger(opX, &iOp2);

  if(longIntegerIsZero(&iOp2)) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoILonI:", "cannot divide a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longIntegerDivideRemainder(&iOp1, &iOp2, &iOp1, &iOp2);
    convertLongIntegerToLongIntegerRegister(&iOp1, result);
  }
}



/********************************************//**
 * \brief opY(long integer) ÷ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonIRe34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divLonIRe34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divLonIRe34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(real34) ÷ opX(long integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34Re34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

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
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34Re34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(long integer) ÷ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonICo34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t temp;

  convertLongIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

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
}



/********************************************//**
 * \brief opY(complex34) ÷ opX(long integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34LonI:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Divide(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(long integer) ÷ opX(angle34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonIAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonIAn34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divLonIAn34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divLonIAn34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle34) ÷ opX(long integer) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34LonI:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34LonI:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34LonI:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterAngularMode(opY), AM_DEGREE);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
      convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
      checkDms34(REGISTER_REAL34_DATA(result));
    }
    else {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), currentAngularMode);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    }
  }
}



/******************************************************************************************************************************************************************************************/
/* real16 / ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

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
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(opY)) ? const16_plusInfinity : const16_minusInfinity), REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16Re16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real16Divide(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
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

  real34_t temp;

  convertRegister16To34(opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX), &temp); // c*c
  real34FMA(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX), &temp, &temp); // c*c + d*d

  real34Divide(REGISTER_REAL34_DATA(opX), &temp, REGISTER_REAL34_DATA(result));
  real34Multiply(REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(result));
  real34ChangeSign(&temp);
  real34Divide(REGISTER_IMAG34_DATA(opX), &temp, REGISTER_IMAG34_DATA(result));
  real34Multiply(REGISTER_IMAG34_DATA(result), REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
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

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Divide(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Divide(REGISTER_IMAG16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(real16) ÷ opX(angle16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16An16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  setRegisterDataType(result, dtReal16, TAG_NONE);

  if(real16IsZero(REGISTER_REAL16_DATA(opY)) && real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16An16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(opY)) ? const16_plusInfinity : const16_minusInfinity), REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16An16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real16Divide(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle16) ÷ opX(real16) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16Re16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(opY)) && real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      setRegisterDataType(result, dtAngle16, currentAngularMode);
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16Re16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      setRegisterDataType(result, dtAngle16, currentAngularMode);
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(opY)) ? const16_plusInfinity : const16_minusInfinity), REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16Re16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    convertRegister16To34(opY);
    convertRegister16To34(opX);
    reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

    if(currentAngularMode == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), AM_DEGREE);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
      convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
      convertRegister34To16(result);
      checkDms16(REGISTER_REAL16_DATA(result));
    }
    else {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), currentAngularMode);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
      convertRegister34To16(result);
    }
  }
}



/********************************************//**
 * \brief opY(real16) ÷ opX(64bits integer) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16ShoI:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);

  if(real16IsZero(REGISTER_REAL16_DATA(opY)) && real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16ShoI:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(opY)) ? const16_plusInfinity : const16_minusInfinity), REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16ShoI:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real16Divide(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
  }
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(real16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoIRe16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opY, opY);
  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);

  if(real16IsZero(REGISTER_REAL16_DATA(opY)) && real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIRe16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(opY)) ? const16_plusInfinity : const16_minusInfinity), REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIRe16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real16Divide(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
  }
}



/********************************************//**
 * \brief opY(real16) ÷ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16Re34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16Re34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16Re34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(real34) ÷ opX(real16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34Re16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34Re16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34Re16:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(real16) ÷ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16Co34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t temp;

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

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
}



/********************************************//**
 * \brief opY(complex34) ÷ opX(real16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34Re16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Divide(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(real16) ÷ opX(angle34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16An34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16An34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16An34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle34) ÷ opX(real16) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34Re16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34Re16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34Re16:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), AM_DEGREE);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
      convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
      checkDms34(REGISTER_REAL34_DATA(result));
    }
    else {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), currentAngularMode);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    }
  }
}



/******************************************************************************************************************************************************************************************/
/* complex16 / ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

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

  real34_t temp;

  convertRegister16To34(opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

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

  convertRegister34To16(result);
}



/********************************************//**
 * \brief opY(complex16) ÷ opX(angle16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16An16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Divide(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Divide(REGISTER_IMAG16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(angle16) ÷ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16Co16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t temp;

  convertRegister16To34(opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX), &temp); // c*c
  real34FMA(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX), &temp, &temp); // c*c + d*d

  real34Divide(REGISTER_REAL34_DATA(opX), &temp, REGISTER_REAL34_DATA(result));
  real34Multiply(REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(result));
  real34ChangeSign(&temp);
  real34Divide(REGISTER_IMAG34_DATA(opX), &temp, REGISTER_IMAG34_DATA(result));
  real34Multiply(REGISTER_IMAG34_DATA(result), REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
}



/********************************************//**
 * \brief opY(complex16) ÷ opX(64bits integer) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16ShoI:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal16Register(opX, opX);
  reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  real16Divide(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result)); // real part
  real16Divide(REGISTER_IMAG16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_IMAG16_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(complex16) ==> result(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoICo16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t temp;

  convertShortIntegerRegisterToReal34Register(opY, opY);
  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX), &temp); // c*c
  real34FMA(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX), &temp, &temp); // c*c + d*d

  real34Divide(REGISTER_REAL34_DATA(opX), &temp, REGISTER_REAL34_DATA(result));
  real34Multiply(REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(result));
  real34ChangeSign(&temp);
  real34Divide(REGISTER_IMAG34_DATA(opX), &temp, REGISTER_IMAG34_DATA(result));
  real34Multiply(REGISTER_IMAG34_DATA(result), REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
}



/********************************************//**
 * \brief opY(complex16) ÷ opX(real34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16Re34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Divide(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(real34) ÷ opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34Co34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t temp;

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

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
}



/********************************************//**
 * \brief opY(complex16) ÷ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16Co34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t temp;

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

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
}



/********************************************//**
 * \brief opY(complex16) ÷ opX(angle34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_IMAG16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16An34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Divide(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(angle34) ÷ opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34Co16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t temp;

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

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
}



/********************************************//**
 * \brief opY(complex34) ÷ opX(complex16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34Co16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t temp;

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

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
}



/******************************************************************************************************************************************************************************************/
/* angle16 / ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(angle16) ÷ opX(angle16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16An16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  setRegisterDataType(result, dtReal16, TAG_NONE);

  if(real16IsZero(REGISTER_REAL16_DATA(opY)) && real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16An16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(opY)) ? const16_plusInfinity : const16_minusInfinity), REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16An16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    convertRegister16To34(opY);
    convertRegister16To34(opX);
    reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

    // We need to have the same angular units before dividing (and preferably not DMS)
    if(getRegisterAngularMode(opY) == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), AM_DMS, AM_DEGREE);
      setRegisterAngularMode(opY, AM_DEGREE);
    }
    if(getRegisterAngularMode(opX) == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opX), AM_DMS, AM_DEGREE);
      setRegisterAngularMode(opX, AM_DEGREE);
    }
    if(getRegisterAngularMode(opY) != getRegisterAngularMode(opX)) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), getRegisterAngularMode(opY));
    }

    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

    convertRegister34To16(result);
  }
}



/********************************************//**
 * \brief opY(angle16) ÷ opX(64bits integer) ==> result(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16ShoI:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtAngle16, REAL16_SIZE, currentAngularMode);

  if(real16IsZero(REGISTER_REAL16_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16ShoI:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(opY)) ? const16_plusInfinity : const16_minusInfinity), REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16ShoI:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    convertRegister16To34(opY);
    reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

    if(currentAngularMode == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), AM_DEGREE);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
      convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
      convertRegister34To16(result);
      checkDms16(REGISTER_REAL16_DATA(result));
    }
    else {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), currentAngularMode);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
      convertRegister34To16(result);
    }
  }
}



/********************************************//**
 * \brief opY(short integer) ÷ opX(angle16) ==> result(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIAn16(void) {
  convertShortIntegerRegisterToReal16Register(opY, opY);

  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoIAn16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);

  if(real16IsZero(REGISTER_REAL16_DATA(opY)) && real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIAn16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(opY)) ? const16_plusInfinity : const16_minusInfinity), REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIAn16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real16Divide(REGISTER_REAL16_DATA(opY), REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle16) ÷ opX(real34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16Re34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16Re34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16Re34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), AM_DEGREE);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
      convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
      checkDms34(REGISTER_REAL34_DATA(result));
    }
    else {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), currentAngularMode);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    }
  }
}



/********************************************//**
 * \brief opY(real34) ÷ opX(angle16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34An16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34An16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34An16:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle16) ÷ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16Co34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t temp;

  convertRegister16To34(opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

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
}



/********************************************//**
 * \brief opY(complex34) ÷ opX(angle16) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34An16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Divide(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(angle16) ÷ opX(angle34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16An34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16An34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16An34:", "cannot divide an angle16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    convertRegister16To34(opY);

    // We need to have the same angular units before dividing (and preferably not DMS)
    if(getRegisterAngularMode(opY) == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), AM_DMS, AM_DEGREE);
      setRegisterAngularMode(opY, AM_DEGREE);
    }
    if(getRegisterAngularMode(opX) == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opX), AM_DMS, AM_DEGREE);
      setRegisterAngularMode(opX, AM_DEGREE);
    }
    if(getRegisterAngularMode(opY) != getRegisterAngularMode(opX)) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), getRegisterAngularMode(opY));
    }

    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle34) ÷ opX(angle16) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34An16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34An16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34An16:", "cannot divide an angle34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    convertRegister16To34(opX);

    // We need to have the same angular units before dividing (and preferably not DMS)
    if(getRegisterAngularMode(opY) == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), AM_DMS, AM_DEGREE);
      setRegisterAngularMode(opY, AM_DEGREE);
    }
    if(getRegisterAngularMode(opX) == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opX), AM_DMS, AM_DEGREE);
      setRegisterAngularMode(opX, AM_DEGREE);
    }
    if(getRegisterAngularMode(opY) != getRegisterAngularMode(opX)) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), getRegisterAngularMode(opY));
    }

    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/******************************************************************************************************************************************************************************************/
/* time / ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(time) ÷ opX(long integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeLonI(void) {
  fnToBeCoded();
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

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(time) ÷ opX(angle16) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divTimeAn16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(time) ÷ opX(64bits integer) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeShoI(void) {
  fnToBeCoded();
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

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(time) ÷ opX(angle34) ==> result(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divTimeAn34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* date / ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/******************************************************************************************************************************************************************************************/
/* string / ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/******************************************************************************************************************************************************************************************/
/* real16 matrix / ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(long integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16LonI(void) {
  fnToBeCoded();
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

  fnToBeCoded();
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

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(angle16) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRm16An16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(64bits integer) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16ShoI(void) {
  fnToBeCoded();
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

  fnToBeCoded();
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

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(real16 matrix) ÷ opX(angle34) ==> result(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRm16An34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* complex16 matrix / ...                                                                                                                                                                 */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(long integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16LonI(void) {
  fnToBeCoded();
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

  fnToBeCoded();
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

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(angle16) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCm16An16:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(64bits integer) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16ShoI(void) {
  fnToBeCoded();
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

  fnToBeCoded();
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

  fnToBeCoded();
}



/********************************************//**
 * \brief opY(complex16 matrix) ÷ opX(angle34) ==> result(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCm16An34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* short integer / ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(64bits integer) ÷ opX(64bits integer) ==> result(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_intDivide(*(REGISTER_SHORT_INTEGER_DATA(opY)), *(REGISTER_SHORT_INTEGER_DATA(opX)));
  setRegisterShortIntegerBase(result, getRegisterShortIntegerBase(opY));
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(real34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoIRe34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIRe34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIRe34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(real34) ÷ opX(64bits integer) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34ShoI:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34ShoI:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34ShoI:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoICo34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t temp;

  convertShortIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

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
}



/********************************************//**
 * \brief opY(complex34) ÷ opX(64bits integer) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34ShoI:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Divide(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(64bits integer) ÷ opX(angle34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoIAn34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opY, opY);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIAn34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIAn34:", "cannot divide a short integer by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle34) ÷ opX(64bits integer) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34ShoI:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34ShoI:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34ShoI:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), AM_DEGREE);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
      convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
      checkDms34(REGISTER_REAL34_DATA(result));
    }
    else {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), currentAngularMode);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    }
  }
}



/******************************************************************************************************************************************************************************************/
/* real34 / ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

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
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34Re34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
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

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

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

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Divide(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(real34) ÷ opX(angle34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34An34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  setRegisterDataType(result, dtReal34, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34An34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34An34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}



/********************************************//**
 * \brief opY(angle34) ÷ opX(real34) ==> result(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34Re34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  setRegisterDataType(result, dtAngle34, currentAngularMode);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34Re34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34Re34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), AM_DEGREE);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
      convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_DEGREE, AM_DMS);
      checkDms34(REGISTER_REAL34_DATA(result));
    }
    else {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), getRegisterTag(opY), currentAngularMode);
      real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    }
  }
}



/******************************************************************************************************************************************************************************************/
/* complex34 / ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

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

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

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
}



/********************************************//**
 * \brief opY(complex34) ÷ opX(angle34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_IMAG34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34An34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result)); // real part
  real34Divide(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result)); // imaginary part
}



/********************************************//**
 * \brief opY(angle34) ÷ opX(complex34) ==> result(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34Co34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t temp;

  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

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
}



/******************************************************************************************************************************************************************************************/
/* angle34 / ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief opY(angle34) ÷ opX(angle34) ==> result(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opY)) || real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34An34:", "cannot use NaN as an input of /", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opY)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34An34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(opY)) ? const34_plusInfinity : const34_minusInfinity), REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X); // error 1 = domain error
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34An34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    // We need to have the same angular units before dividing (and preferably not DMS)
    if(getRegisterAngularMode(opY) == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opY), AM_DMS, AM_DEGREE);
      setRegisterAngularMode(opY, AM_DEGREE);
    }
    if(getRegisterAngularMode(opX) == AM_DMS) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opX), AM_DMS, AM_DEGREE);
      setRegisterAngularMode(opX, AM_DEGREE);
    }
    if(getRegisterAngularMode(opY) != getRegisterAngularMode(opX)) {
      convertAngle34FromTo(REGISTER_REAL34_DATA(opX), getRegisterAngularMode(opX), getRegisterAngularMode(opY));
    }

    real34Divide(REGISTER_REAL34_DATA(opY), REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  }
}
