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
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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

  division[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}



/**********************************************************************
 * In all the functions below:
 * if Y is a number then Y = a + ib
 * if X is a number then X = c + id
 * The variables a, b, c and d are used for intermediate calculations
 * The result is then X = (ac + bd)/(c² + d²) + i(bc - ad)/(c² + d²)
 * The variable denom is used to store (c² + d²)
 ***********************************************************************/

/******************************************************************************************************************************************************************************************/
/* long integer / ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) ÷ X(long integer) ==> X(long integer or real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonILonI(void) {
  longInteger_t a, c;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, a);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, c);

  if(longIntegerIsZero(c)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonILonI:", "cannot divide a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t quotient, remainder;

    longIntegerInit(quotient);
    longIntegerInit(remainder);
    longIntegerDivideRemainder(a, c, quotient, remainder);

    if(longIntegerIsZero(remainder)) {
      convertLongIntegerToLongIntegerRegister(quotient, REGISTER_X);
    }
    else {
      longIntegerToAllocatedString(a, tmpStr3000, 10);
      reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, TAG_NONE);
      stringToReal34(tmpStr3000, REGISTER_REAL34_DATA(REGISTER_Y));
      longIntegerToAllocatedString(c, tmpStr3000, 10);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);
      stringToReal34(tmpStr3000, REGISTER_REAL34_DATA(REGISTER_X));

      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);
      real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));

      convertRegister34To16(REGISTER_X);
    }

    longIntegerFree(quotient);
    longIntegerFree(remainder);
  }

  longIntegerFree(a);
  longIntegerFree(c);
}



/********************************************//**
 * \brief Y(long integer) ÷ X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonIRe16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(realIcIsZero(&a)) {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIRe16:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal16((realIcIsPositive(&a) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIRe16:", "cannot divide a real16 by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
    realIcDivide(&a, &c, &c);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(real16) ÷ X(long integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16LonI:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);

  if(realIcIsZero(&c)) {
    if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_Y))) {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIRe16:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal16((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIRe16:", "cannot divide a real16 by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
    realIcDivide(&a, &c, &c);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(long integer) ÷ X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonICo16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t denom, a, c, d;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &d);

  //   a        ac         ad
  // ------ = ------- - i-------
  // c + id   c² + d²    c² + d²

  // Denominator
  realIcMultiply(&c, &c, &denom);    // c²
  realIcFMA(&d, &d, &denom, &denom); // c² + d²

  // Real part
  realIcDivide(&c, &denom, &c);      // c / (c² + d²)
  realIcMultiply(&c, &a, &c);        // ac / (c² + d²)

  // Imaginary part
  realIcChangeSign(&denom);          // -(c² + d²)
  realIcDivide(&d, &denom, &d);      // -d / (c² + d²)
  realIcMultiply(&d, &a, &d);        // -ad / (c² + d²)

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) ÷ X(long integer) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16LonI:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);

  realIcDivide(&a, &c, &a);
  realIcDivide(&b, &c, &b);

  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) ÷ X(angle16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonIAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonIAn16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(realIcIsZero(&a)) {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIAn16:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal16((realIcIsPositive(&a) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIAn16:", "cannot divide a real16 by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
    realIcDivide(&a, &c, &c);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(angle16) ÷ X(long integer) ==> X(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16LonI:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtAngle16, REAL16_SIZE, currentAngularMode);

  if(realIcIsZero(&c)) {
    if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_Y))) {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divAn16LonI:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal16((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divAn16LonI:", "cannot divide a real16 by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);

    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
      realIcDivide(&a, &c, &c);
      convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
      realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
      realIcDivide(&a, &c, &c);
      realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
    }
  }
}



/********************************************//**
 * \brief Y(long integer) ÷ X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonIShoI(void) {
  longInteger_t a, c;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, a);
  convertShortIntegerRegisterLongIntegerRegister(REGISTER_X, REGISTER_X);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, c);

  if(longIntegerIsZero(c)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonIShoI:", "cannot divide a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longIntegerDivideRemainder(a, c, a, c);
    convertLongIntegerToLongIntegerRegister(a, REGISTER_X);
  }

  longIntegerFree(a);
  longIntegerFree(c);
}



/********************************************//**
 * \brief Y(64bits integer) ÷ X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoILonI(void) {
  longInteger_t a, c;

  convertShortIntegerRegisterLongIntegerRegister(REGISTER_Y, REGISTER_Y);
  convertLongIntegerRegisterToLongInteger(REGISTER_Y, a);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, c);

  if(longIntegerIsZero(c)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoILonI:", "cannot divide a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longIntegerDivideRemainder(a, c, a, c);
    convertLongIntegerToLongIntegerRegister(a, REGISTER_X);
  }

  longIntegerFree(a);
  longIntegerFree(c);
}



/********************************************//**
 * \brief Y(long integer) ÷ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonIRe34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(realIcIsZero(&a)) {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIRe34:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal34((realIcIsPositive(&a) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIRe34:", "cannot divide a real16 by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
    realIcDivide(&a, &c, &c);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(real34) ÷ X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34LonI:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);

  if(realIcIsZero(&c)) {
    if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y))) {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divRe34LonI:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal34((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divRe34LonI:", "cannot divide a real16 by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
    realIcDivide(&a, &c, &c);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(long integer) ÷ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonICo34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t denom, a, c, d;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  //   a        ac         ad
  // ------ = ------- - i-------
  // c + id   c² + d²    c² + d²

  // Denominator
  realIcMultiply(&c, &c, &denom);    // c²
  realIcFMA(&d, &d, &denom, &denom); // c² + d²

  // Real part
  realIcDivide(&c, &denom, &c);      // c / (c² + d²)
  realIcMultiply(&c, &a, &c);        // ac / (c² + d²)

  // Imaginary part
  realIcChangeSign(&denom);          // -(c² + d²)
  realIcDivide(&d, &denom, &d);      // -d / (c² + d²)
  realIcMultiply(&d, &a, &d);        // -ad / (c² + d²)

  realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ÷ X(long integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34LonI:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);

  realIcDivide(&a, &c, &a);
  realIcDivide(&b, &c, &b);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) ÷ X(angle34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonIAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonIAn34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &a);
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(realIcIsZero(&a)) {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIAn34:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal34((realIcIsPositive(&a) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIAn34:", "cannot divide a real34 by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
    realIcDivide(&a, &c, &c);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(angle34) ÷ X(long integer) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34LonI:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(realIcIsZero(&c)) {
    if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y))) {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divAn34LonI:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getFlag(FLAG_DANGER)) {
        realIcToReal34((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divAn34LonI:", "cannot divide a real34 by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);

    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
      realIcDivide(&a, &c, &c);
      convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
      realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
      realIcDivide(&a, &c, &c);
      realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }
}



/******************************************************************************************************************************************************************************************/
/* real16 / ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16) ÷ X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16Re16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16Re16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_Y)) && real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16Re16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16Re16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real16Divide(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(real16) ÷ X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16Co16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16Co16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t denom;

  convertRegister16To34(REGISTER_Y);
  convertRegister16To34(REGISTER_X);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom); // c²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // c² + d²

  // Real part
  real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &denom, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X));

  // Imaginary part
  real34ChangeSign(&denom);
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &denom, REGISTER_IMAG34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X));

  convertRegister34To16(REGISTER_X);
}



/********************************************//**
 * \brief Y(complex16) ÷ X(real16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16Re16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16Re16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real16Divide(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_Y)); // real part
  real16Divide(REGISTER_IMAG16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_IMAG16_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  complex16Copy(REGISTER_COMPLEX16_DATA(REGISTER_Y), REGISTER_COMPLEX16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) ÷ X(angle16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16An16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16An16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_Y)) && real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16An16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16An16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real16Divide(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
}



/********************************************//**
 * \brief Y(angle16) ÷ X(real16) ==> X(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16Re16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16Re16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_Y)) && real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16Re16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal16((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16Re16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    realIc_t a, c;

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
    setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);

    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
      convertAngleIcFromTo(&c, AM_DMS, AM_DEGREE);
      realIcDivide(&a, &c, &c);
      convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
      realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
      realIcDivide(&a, &c, &c);
      realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
    }
  }
}



/********************************************//**
 * \brief Y(real16) ÷ X(64bits integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16ShoI:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_Y)) && realIcIsZero(&c)) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16ShoI:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(realIcIsZero(&c)) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal16((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16ShoI:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
    realIcDivide(&a, &c, &c);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(64bits integer) ÷ X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoIRe16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &a);

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X)) && realIcIsZero(&a)) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIRe16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal16((realIcIsPositive(&a) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIRe16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
    realIcDivide(&a, &c, &c);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(real16) ÷ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16Re34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16Re34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16Re34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16Re34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(real34) ÷ X(real16) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34Re16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34Re16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34Re16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34Re16:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(real16) ÷ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16Co34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16Co34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t denom;

  convertRegister16To34(REGISTER_Y);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom); // c²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // c² + d²

  // Real part
  real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &denom, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X));

  // Imaginary part
  real34ChangeSign(&denom);
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &denom, REGISTER_IMAG34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ÷ X(real16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34Re16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34Re16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);
  real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) ÷ X(angle34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16An34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe16An34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16An34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16An34:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
}



/********************************************//**
 * \brief Y(angle34) ÷ X(real16) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34Re16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34Re16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34Re16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34Re16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34Re16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    realIc_t a, c;

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
    setRegisterDataType(REGISTER_X, dtAngle34, currentAngularMode);

    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
      convertAngleIcFromTo(&c, AM_DMS, AM_DEGREE);
      realIcDivide(&a, &c, &c);
      convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
      realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
      realIcDivide(&a, &c, &c);
      realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }
}



/******************************************************************************************************************************************************************************************/
/* complex16 / ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16) ÷ X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16Co16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16Co16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t numer, denom, realPart;

  convertRegister16To34(REGISTER_Y);
  convertRegister16To34(REGISTER_X);

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // denom = c²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // denom = c² + d²

  // real part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), &numer);    // numer = a*c
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), &numer, &numer); // numer = a*c + b*d
  real34Divide(&numer, &denom, &realPart);                                                       // realPart = (a*c + b*d) / (c² + d²) = numer / denom

  // imaginary part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), &numer);    // numer = b*c
  real34ChangeSign(REGISTER_REAL34_DATA(REGISTER_Y));                                            // a = -a
  real34FMA(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), &numer, &numer); // numer = b*c - a*d
  real34Divide(&numer, &denom, REGISTER_IMAG34_DATA(REGISTER_X));                                // im(X) = (b*c - a*d) / (c² + d²) = numer / denom

  // real part
  real34Copy(&realPart, REGISTER_REAL34_DATA(REGISTER_X));                                       // re(X) = realPart

  convertRegister34To16(REGISTER_X);
}



/********************************************//**
 * \brief Y(complex16) ÷ X(angle16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16An16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16An16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real16Divide(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_Y)); // real part
  real16Divide(REGISTER_IMAG16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_IMAG16_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  complex16Copy(REGISTER_COMPLEX16_DATA(REGISTER_Y), REGISTER_COMPLEX16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle16) ÷ X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16Co16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16Co16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t denom;

  convertRegister16To34(REGISTER_Y);
  convertRegister16To34(REGISTER_X);

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // denom = c²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // denom = c² + d²

  // real part
  real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &denom, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X));

  // imaginary part
  real34ChangeSign(&denom);
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &denom, REGISTER_IMAG34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X));

  convertRegister34To16(REGISTER_X);
}



/********************************************//**
 * \brief Y(complex16) ÷ X(64bits integer) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16ShoI:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, c;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);

  realIcDivide(&a, &c, &a);
  realIcDivide(&b, &c, &b);

  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(64bits integer) ÷ X(complex16) ==> X(complex16)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoICo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoICo16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t denom, a, c, d;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &a);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &d);

  //   a        ac         ad
  // ------ = ------- - i-------
  // c + id   c² + d²    c² + d²

  // Denominator
  realIcMultiply(&c, &c, &denom);    // c²
  realIcFMA(&d, &d, &denom, &denom); // c² + d²

  // Real part
  realIcDivide(&c, &denom, &c);      // c / (c² + d²)
  realIcMultiply(&c, &a, &c);        // ac / (c² + d²)

  // Imaginary part
  realIcChangeSign(&denom);          // -(c² + d²)
  realIcDivide(&d, &denom, &d);      // -d / (c² + d²)
  realIcMultiply(&d, &a, &d);        // -ad / (c² + d²)

  realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&d, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) ÷ X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16Re34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16Re34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);
  real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) ÷ X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34Co16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34Co16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t denom;

  convertRegister16To34(REGISTER_X);

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // denom = c²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // denom = c² + d²

  // real part
  real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &denom, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X));

  // imaginary part
  real34ChangeSign(&denom);
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &denom, REGISTER_IMAG34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) ÷ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16Co34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16Co34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t numer, denom, realPart;

  convertRegister16To34(REGISTER_Y);

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // denom = c²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // denom = c² + d²

  // real part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), &numer);    // numer = a*c
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), &numer, &numer); // numer = a*c + b*d
  real34Divide(&numer, &denom, &realPart);                                                       // realPart = (a*c + b*d) / (c² + d²) = numer / denom

  // imaginary part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), &numer);    // numer = b*c
  real34ChangeSign(REGISTER_REAL34_DATA(REGISTER_Y));                                            // a = -a
  real34FMA(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), &numer, &numer); // numer = b*c - a*d
  real34Divide(&numer, &denom, REGISTER_IMAG34_DATA(REGISTER_X));                                // im(X) = (b*c - a*d) / (c² + d²) = numer / denom

  // real part
  real34Copy(&realPart, REGISTER_REAL34_DATA(REGISTER_X));                                       // re(X) = realPart
}



/********************************************//**
 * \brief Y(complex16) ÷ X(angle34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16An34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo16An34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_Y);
  real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle34) ÷ X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34Co16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34Co16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t denom;

  convertRegister16To34(REGISTER_X);

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // denom = c²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // denom = c² + d²

  // real part
  real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &denom, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X));

  // imaginary part
  real34ChangeSign(&denom);
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &denom, REGISTER_IMAG34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ÷ X(complex16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Co16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34Co16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34Co16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t numer, denom, realPart;

  convertRegister16To34(REGISTER_X);

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // denom = c²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // denom = c² + d²

  // real part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), &numer);    // numer = a*c
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), &numer, &numer); // numer = a*c + b*d
  real34Divide(&numer, &denom, &realPart);                                                       // realPart = (a*c + b*d) / (c² + d²) = numer / denom

  // imaginary part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), &numer);    // numer = b*c
  real34ChangeSign(REGISTER_REAL34_DATA(REGISTER_Y));                                            // a = -a
  real34FMA(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), &numer, &numer); // numer = b*c - a*d
  real34Divide(&numer, &denom, REGISTER_IMAG34_DATA(REGISTER_X));                                // im(X) = (b*c - a*d) / (c² + d²) = numer / denom

  // real part
  real34Copy(&realPart, REGISTER_REAL34_DATA(REGISTER_X));                                       // re(X) = realPart
}



/******************************************************************************************************************************************************************************************/
/* angle16 / ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(angle16) ÷ X(angle16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16An16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16An16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_Y)) && real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16An16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16An16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    realIc_t a, c;

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c);

    // We need to have the same angular units before dividing (and preferably not DMS)
    if(getRegisterAngularMode(REGISTER_Y) == AM_DMS) {
      convertAngleIcFromTo(&a, AM_DMS, AM_DEGREE);
      setRegisterAngularMode(REGISTER_Y, AM_DEGREE);
    }
    if(getRegisterAngularMode(REGISTER_X) == AM_DMS) {
      convertAngleIcFromTo(&c, AM_DMS, AM_DEGREE);
      setRegisterAngularMode(REGISTER_X, AM_DEGREE);
    }
    if(getRegisterAngularMode(REGISTER_Y) != getRegisterAngularMode(REGISTER_X)) {
      convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), getRegisterAngularMode(REGISTER_Y));
    }

    realIcDivide(&a, &c, &c);
    realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
}



/********************************************//**
 * \brief Y(angle16) ÷ X(64bits integer) ==> X(angle16)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16ShoI:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtAngle16, REAL16_SIZE, currentAngularMode);

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_Y)) && realIcIsZero(&c)) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16ShoI:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(realIcIsZero(&c)) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal16((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16ShoI:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);

    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
      realIcDivide(&a, &c, &c);
      convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
      realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
      checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
      realIcDivide(&a, &c, &c);
      realIcToReal16(&c, REGISTER_REAL16_DATA(REGISTER_X));
    }
  }
}



/********************************************//**
 * \brief Y(short integer) ÷ X(angle16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoIAn16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
      realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIAn16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
      realIcToReal16((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIAn16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    convertRegister16To34(REGISTER_X);
    real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
    convertRegister34To16(REGISTER_X);
  }

  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
}



/********************************************//**
 * \brief Y(angle16) ÷ X(real34) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16Re34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16Re34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16Re34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16Re34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16Re34:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    realIc_t a, c;

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
    setRegisterDataType(REGISTER_X, dtAngle34, currentAngularMode);

    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
      convertAngleIcFromTo(&c, AM_DMS, AM_DEGREE);
      realIcDivide(&a, &c, &c);
      convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
      realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
      realIcDivide(&a, &c, &c);
      realIcToReal34(&c, REGISTER_REAL16_DATA(REGISTER_X));
    }
  }
}



/********************************************//**
 * \brief Y(real34) ÷ X(angle16) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34An16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34An16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34An16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34An16:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
}



/********************************************//**
 * \brief Y(angle16) ÷ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16Co34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16Co34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16Co34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t denom;

  convertRegister16To34(REGISTER_Y);

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // denom = c²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // denom = c² + d²

  // real part
  real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &denom, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X));

  // imaginary part
  real34ChangeSign(&denom);
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &denom, REGISTER_IMAG34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ÷ X(angle16) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34An16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34An16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);
  real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle16) ÷ X(angle34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn16An34(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16An34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn16An34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16An34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16An34:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    realIc_t a, c;

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &a);
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

    // We need to have the same angular units before dividing (and preferably not DMS)
    if(getRegisterAngularMode(REGISTER_Y) == AM_DMS) {
      convertAngleIcFromTo(&a, AM_DMS, AM_DEGREE);
      setRegisterAngularMode(REGISTER_Y, AM_DEGREE);
    }
    if(getRegisterAngularMode(REGISTER_X) == AM_DMS) {
      convertAngleIcFromTo(&c, AM_DMS, AM_DEGREE);
      setRegisterAngularMode(REGISTER_X, AM_DEGREE);
    }
    if(getRegisterAngularMode(REGISTER_Y) != getRegisterAngularMode(REGISTER_X)) {
      convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), getRegisterAngularMode(REGISTER_Y));
    }

    realIcDivide(&a, &c, &c);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
}



/********************************************//**
 * \brief Y(angle34) ÷ X(angle16) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34An16(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34An16:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34An16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(REGISTER_X);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34An16:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34An16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    realIc_t a, c;

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

    // We need to have the same angular units before dividing (and preferably not DMS)
    if(getRegisterAngularMode(REGISTER_Y) == AM_DMS) {
      convertAngleIcFromTo(&a, AM_DMS, AM_DEGREE);
      setRegisterAngularMode(REGISTER_Y, AM_DEGREE);
    }
    if(getRegisterAngularMode(REGISTER_X) == AM_DMS) {
      convertAngleIcFromTo(&c, AM_DMS, AM_DEGREE);
      setRegisterAngularMode(REGISTER_X, AM_DEGREE);
    }
    if(getRegisterAngularMode(REGISTER_Y) != getRegisterAngularMode(REGISTER_X)) {
      convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), getRegisterAngularMode(REGISTER_Y));
    }

    realIcDivide(&a, &c, &c);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
}



/******************************************************************************************************************************************************************************************/
/* time / ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(time) ÷ X(long integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) ÷ X(real16) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divTimeRe16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) ÷ X(angle16) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divTimeAn16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) ÷ X(64bits integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) ÷ X(real34) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divTimeRe34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) ÷ X(angle34) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divTimeAn34:", "cannot use NaN as X input of /", NULL, NULL);
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
 * \brief Y(real16 matrix) ÷ X(long integer) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16LonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ÷ X(real16) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRm16Re16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ÷ X(complex16) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRm16Co16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ÷ X(angle16) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRm16An16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ÷ X(64bits integer) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16ShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ÷ X(real34) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRm16Re34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ÷ X(complex34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRm16Co34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ÷ X(angle34) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRm16An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRm16An34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* complex16 matrix / ...                                                                                                                                                                 */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16 matrix) ÷ X(long integer) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16LonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ÷ X(real16) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCm16Re16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ÷ X(complex16) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCm16Co16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ÷ X(angle16) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCm16An16:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ÷ X(64bits integer) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16ShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ÷ X(real34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCm16Re34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ÷ X(complex34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCm16Co34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ÷ X(angle34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCm16An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCm16An34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* short integer / ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(64bits integer) ÷ X(64bits integer) ==> X(64bits integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intDivide(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
  setRegisterShortIntegerBase(REGISTER_X, getRegisterShortIntegerBase(REGISTER_Y));
}



/********************************************//**
 * \brief Y(64bits integer) ÷ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoIRe34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIRe34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIRe34:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(real34) ÷ X(64bits integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34ShoI:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34ShoI:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34ShoI:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(64bits integer) ÷ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoICo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoICo34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t denom;

  convertShortIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // c²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // c² + d²

  // Real part
  real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &denom, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X));

  // Imaginary part
  real34ChangeSign(&denom);
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &denom, REGISTER_IMAG34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ÷ X(64bits integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34ShoI:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(64bits integer) ÷ X(angle34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoIAn34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  convertShortIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIAn34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divShoIAn34:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
}



/********************************************//**
 * \brief Y(angle34) ÷ X(64bits integer) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34ShoI:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, c;

  convertShortIntegerRegisterToRealIc(REGISTER_X, &c);
  reallocateRegister(REGISTER_X, dtAngle34, REAL34_SIZE, currentAngularMode);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && realIcIsZero(&c)) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16ShoI:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(realIcIsZero(&c)) {
    if(getFlag(FLAG_DANGER)) {
      realIcToReal34((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn16ShoI:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);

    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
      realIcDivide(&a, &c, &c);
      convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
      realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
      realIcDivide(&a, &c, &c);
      realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }
}



/******************************************************************************************************************************************************************************************/
/* real34 / ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) ÷ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34Re34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34Re34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34Re34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34Re34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(real34) ÷ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34Co34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34Co34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t denom;

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // denom = c²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // denom = c² + d²

  // real part
  real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &denom, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X));

  // imaginary part
  real34ChangeSign(&denom);
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &denom, REGISTER_IMAG34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ÷ X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34Re34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34Re34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real34) ÷ X(angle34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRe34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34An34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divRe34An34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34An34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34An34:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
}



/********************************************//**
 * \brief Y(angle34) ÷ X(real34) ==> X(angle34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34Re34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34Re34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34Re34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34Re34:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    realIc_t a, c;

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);
    setRegisterDataType(REGISTER_X, dtAngle34, currentAngularMode);

    if(currentAngularMode == AM_DMS) {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), AM_DEGREE);
      convertAngleIcFromTo(&c, AM_DMS, AM_DEGREE);
      realIcDivide(&a, &c, &c);
      convertAngleIcFromTo(&c, AM_DEGREE, AM_DMS);
      realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngleIcFromTo(&a, getRegisterAngularMode(REGISTER_Y), currentAngularMode);
      realIcDivide(&a, &c, &c);
      realIcToReal34(&c, REGISTER_REAL16_DATA(REGISTER_X));
    }
  }
}



/******************************************************************************************************************************************************************************************/
/* complex34 / ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex34) ÷ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34Co34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34Co34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t numer, denom, realPart;

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // denom = c*c
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // denom = c*c + d*d

  // real part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), &numer);    // numer = a*c
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), &numer, &numer); // numer = a*c + b*d
  real34Divide(&numer, &denom, &realPart);                                                       // realPart = (a*c + b*d) / (c*c + d*d) = numer / denom

  // imaginary part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), &numer);    // numer = b*c
  real34ChangeSign(REGISTER_REAL34_DATA(REGISTER_Y)); // -a
  real34FMA(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), &numer, &numer); // numer = b*c - a*d
  real34Divide(&numer, &denom, REGISTER_IMAG34_DATA(REGISTER_X));                                // im(X) = (b*c - a*d) / (c*c + d*d)

  // real part
  real34Copy(&realPart, REGISTER_REAL34_DATA(REGISTER_X));                                       // re(X) = realPart
}



/********************************************//**
 * \brief Y(complex34) ÷ X(angle34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCo34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34An34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divCo34An34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(angle34) ÷ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34Co34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34Co34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  real34_t denom;

  // Denominator
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &denom);    // denom = c²
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &denom, &denom); // denom = c² + d²

  // real part
  real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &denom, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X));

  // imaginary part
  real34ChangeSign(&denom);
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &denom, REGISTER_IMAG34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* angle34 / ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(angle34) ÷ X(angle34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divAn34An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34An34:", "cannot use NaN as Y input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divAn34An34:", "cannot use NaN as X input of /", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34An34:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divAn34An34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    realIc_t a, c;

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &a);
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c);

    // We need to have the same angular units before dividing (and preferably not DMS)
    if(getRegisterAngularMode(REGISTER_Y) == AM_DMS) {
      convertAngleIcFromTo(&a, AM_DMS, AM_DEGREE);
      setRegisterAngularMode(REGISTER_Y, AM_DEGREE);
    }
    if(getRegisterAngularMode(REGISTER_X) == AM_DMS) {
      convertAngleIcFromTo(&c, AM_DMS, AM_DEGREE);
      setRegisterAngularMode(REGISTER_X, AM_DEGREE);
    }
    if(getRegisterAngularMode(REGISTER_Y) != getRegisterAngularMode(REGISTER_X)) {
      convertAngleIcFromTo(&c, getRegisterAngularMode(REGISTER_X), getRegisterAngularMode(REGISTER_Y));
    }

    realIcDivide(&a, &c, &c);
    realIcToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
}
