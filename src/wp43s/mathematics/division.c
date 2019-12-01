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
// regX |    regY ==>   1            2            3            4         5            6         7         8            9            10            11           12
//      V               Long integer Real16       Complex16    Angle16   Time         Date      String    Real16 mat   Complex16 m  Short integer Real34       Complex34
/*  1 Long integer  */ {divLonILonI, divRe16LonI, divCo16LonI, divError, divTimeLonI, divError, divError, divRm16LonI, divCm16LonI, divShoILonI,  divRe34LonI, divCo34LonI},
/*  2 Real16        */ {divLonIRe16, divRe16Re16, divCo16Re16, divError, divTimeRe16, divError, divError, divRm16Re16, divCm16Re16, divShoIRe16,  divRe34Re16, divCo34Re16},
/*  3 Complex16     */ {divLonICo16, divRe16Co16, divCo16Co16, divError, divError,    divError, divError, divRm16Co16, divCm16Co16, divShoICo16,  divRe34Co16, divCo34Co16},
/*  4 Angle16       */ {divError,    divError,    divError,    divError, divError,    divError, divError, divError,    divError,    divError,     divError,    divError   },
/*  5 Time          */ {divError,    divError,    divError,    divError, divError,    divError, divError, divError,    divError,    divError,     divError,    divError   },
/*  6 Date          */ {divError,    divError,    divError,    divError, divError,    divError, divError, divError,    divError,    divError,     divError,    divError   },
/*  7 String        */ {divError,    divError,    divError,    divError, divError,    divError, divError, divError,    divError,    divError,     divError,    divError   },
/*  8 Real16 mat    */ {divError,    divError,    divError,    divError, divError,    divError, divError, divError,    divError,    divError,     divError,    divError   },
/*  9 Complex16 mat */ {divError,    divError,    divError,    divError, divError,    divError, divError, divError,    divError,    divError,     divError,    divError   },
/* 10 Short integer */ {divLonIShoI, divRe16ShoI, divCo16ShoI, divError, divTimeShoI, divError, divError, divRm16ShoI, divCm16ShoI, divShoIShoI,  divRe34ShoI, divCo34ShoI},
/* 11 Real34        */ {divLonIRe34, divRe16Re34, divCo16Re34, divError, divTimeRe34, divError, divError, divRm16Re34, divCm16Re34, divShoIRe34,  divRe34Re34, divCo34Re34},
/* 12 Complex34     */ {divLonICo34, divRe16Co34, divCo16Co34, divError, divError,    divError, divError, divRm16Co34, divCm16Co34, divShoICo34,  divRe34Co34, divCo34Co34}
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



void divCo39Co39(const complex39_t *numer, const complex39_t *denom, complex39_t *quotient) {
  real39_t realNumer, realDenom, a, b, c, d;

  realCopy(&numer->real, &a);
  realCopy(&numer->imag, &b);
  realCopy(&denom->real, &c);
  realCopy(&denom->imag, &d);

  // Denominator
  realMultiply(&c, &c, &realDenom, &ctxtReal39);                    // realDenom = c²
  realFMA(&d, &d, &realDenom, &realDenom, &ctxtReal39);             // realDenom = c² + d²

  // real part
  realMultiply(&a, &c, &realNumer, &ctxtReal39);                    // realNumer = a*c
  realFMA(&b, &d, &realNumer, &realNumer, &ctxtReal39);             // realNumer = a*c + b*d
  realDivide(&realNumer, &realDenom, &quotient->real, &ctxtReal39); // realPart = (a*c + b*d) / (c² + d²) = realNumer / realDenom

  // imaginary part
  realMultiply(&b, &c, &realNumer, &ctxtReal39);                    // realNumer = b*c
  realChangeSign(&a);                                               // a = -a
  realFMA(&a, &d, &realNumer, &realNumer, &ctxtReal39);             // realNumer = b*c - a*d
  realDivide(&realNumer, &realDenom, &quotient->imag, &ctxtReal39); // imagPart = (b*c - a*d) / (c² + d²) = realNumer / realDenom
}



void divCo51Co51(const complex51_t *numer, const complex51_t *denom, complex51_t *quotient) {
  real51_t realNumer, realDenom, a, b, c, d;

  realCopy(&numer->real, &a);
  realCopy(&numer->imag, &b);
  realCopy(&denom->real, &c);
  realCopy(&denom->imag, &d);

  // Denominator
  realMultiply(&c, &c, &realDenom, &ctxtReal51);                    // realDenom = c²
  realFMA(&d, &d, &realDenom, &realDenom, &ctxtReal51);             // realDenom = c² + d²

  // real part
  realMultiply(&a, &c, &realNumer, &ctxtReal51);                    // realNumer = a*c
  realFMA(&b, &d, &realNumer, &realNumer, &ctxtReal51);             // realNumer = a*c + b*d
  realDivide(&realNumer, &realDenom, &quotient->real, &ctxtReal51); // realPart = (a*c + b*d) / (c² + d²) = realNumer / realDenom

  // imaginary part
  realMultiply(&b, &c, &realNumer, &ctxtReal51);                    // realNumer = b*c
  realChangeSign(&a);                                               // a = -a
  realFMA(&a, &d, &realNumer, &realNumer, &ctxtReal51);             // realNumer = b*c - a*d
  realDivide(&realNumer, &realDenom, &quotient->imag, &ctxtReal51); // imagPart = (b*c - a*d) / (c² + d²) = realNumer / realDenom
}



void divCo75Co75(const complex75_t *numer, const complex75_t *denom, complex75_t *quotient) {
  real75_t realNumer, realDenom, a, b, c, d;

  realCopy(&numer->real, &a);
  realCopy(&numer->imag, &b);
  realCopy(&denom->real, &c);
  realCopy(&denom->imag, &d);

  // Denominator
  realMultiply(&c, &c, &realDenom, &ctxtReal75);                    // realDenom = c²
  realFMA(&d, &d, &realDenom, &realDenom, &ctxtReal75);             // realDenom = c² + d²

  // real part
  realMultiply(&a, &c, &realNumer, &ctxtReal75);                    // realNumer = a*c
  realFMA(&b, &d, &realNumer, &realNumer, &ctxtReal75);             // realNumer = a*c + b*d
  realDivide(&realNumer, &realDenom, &quotient->real, &ctxtReal75); // realPart = (a*c + b*d) / (c² + d²) = realNumer / realDenom

  // imaginary part
  realMultiply(&b, &c, &realNumer, &ctxtReal75);                    // realNumer = b*c
  realChangeSign(&a);                                               // a = -a
  realFMA(&a, &d, &realNumer, &realNumer, &ctxtReal75);             // realNumer = b*c - a*d
  realDivide(&realNumer, &realDenom, &quotient->imag, &ctxtReal75); // imagPart = (b*c - a*d) / (c² + d²) = realNumer / realDenom
}



void divRe39Co39(const real_t *numer, const complex39_t *denom, complex39_t *quotient) {
  real39_t realNumer, realDenom, a, c, d;

  realCopy(numer, &a);
  realCopy(&denom->real, &c);
  realCopy(&denom->imag, &d);

  // Denominator
  realMultiply(&c, &c, &realDenom, &ctxtReal39);                    // realDenom = c²
  realFMA(&d, &d, &realDenom, &realDenom, &ctxtReal39);             // realDenom = c² + d²

  // real part
  realMultiply(&a, &c, &realNumer, &ctxtReal39);                    // realNumer = a*c
  realDivide(&realNumer, &realDenom, &quotient->real, &ctxtReal39); // realPart  = (a*c) / (c² + d²) = realNumer / realDenom

  // imaginary part
  realChangeSign(&a);                                               // a = -a
  realMultiply(&a, &d, &realNumer, &ctxtReal39);                    // realNumer = -a*d
  realDivide(&realNumer, &realDenom, &quotient->imag, &ctxtReal39); // imagPart  = -(a*d) / (c² + d²) = realNumer / realDenom
}



void divRe51Co51(const real_t *numer, const complex51_t *denom, complex51_t *quotient) {
  real51_t realNumer, realDenom, a, c, d;

  realCopy(numer, &a);
  realCopy(&denom->real, &c);
  realCopy(&denom->imag, &d);

  // Denominator
  realMultiply(&c, &c, &realDenom, &ctxtReal51);                    // realDenom = c²
  realFMA(&d, &d, &realDenom, &realDenom, &ctxtReal51);             // realDenom = c² + d²

  // real part
  realMultiply(&a, &c, &realNumer, &ctxtReal51);                    // realNumer = a*c
  realDivide(&realNumer, &realDenom, &quotient->real, &ctxtReal51); // realPart  = (a*c) / (c² + d²) = realNumer / realDenom

  // imaginary part
  realChangeSign(&a);                                               // a = -a
  realMultiply(&a, &d, &realNumer, &ctxtReal51);                    // realNumer = -a*d
  realDivide(&realNumer, &realDenom, &quotient->imag, &ctxtReal51); // imagPart  = -(a*d) / (c² + d²) = realNumer / realDenom
}



void divRe75Co75(const real_t *numer, const complex75_t *denom, complex75_t *quotient) {
  real75_t realNumer, realDenom, a, c, d;

  realCopy(numer, &a);
  realCopy(&denom->real, &c);
  realCopy(&denom->imag, &d);

  // Denominator
  realMultiply(&c, &c, &realDenom, &ctxtReal75);                    // realDenom = c²
  realFMA(&d, &d, &realDenom, &realDenom, &ctxtReal75);             // realDenom = c² + d²

  // real part
  realMultiply(&a, &c, &realNumer, &ctxtReal75);                    // realNumer = a*c
  realDivide(&realNumer, &realDenom, &quotient->real, &ctxtReal75); // realPart  = (a*c) / (c² + d²) = realNumer / realDenom

  // imaginary part
  realChangeSign(&a);                                               // a = -a
  realMultiply(&a, &d, &realNumer, &ctxtReal75);                    // realNumer = -a*d
  realDivide(&realNumer, &realDenom, &quotient->imag, &ctxtReal75); // imagPart  = -(a*d) / (c² + d²) = realNumer / realDenom
}



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
  longInteger_t x;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonILonI:", "cannot divide a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t y, quotient, remainder;

    convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
    longIntegerInit(quotient);
    longIntegerInit(remainder);
    longIntegerDivideQuotientRemainder(y, x, quotient, remainder);

    if(longIntegerIsZero(remainder)) {
      convertLongIntegerToLongIntegerRegister(quotient, REGISTER_X);
    }
    else {
      real39_t xIc, yIc;

      convertLongIntegerRegisterToReal(REGISTER_Y, &yIc, &ctxtReal39);
      convertLongIntegerRegisterToReal(REGISTER_X, &xIc, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

      realDivide(&yIc, &xIc, &xIc, &ctxtReal39);
      realToReal16(&xIc, REGISTER_REAL16_DATA(REGISTER_X));
    }

    longIntegerFree(quotient);
    longIntegerFree(remainder);
    longIntegerFree(y);
  }

  longIntegerFree(x);
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

  real39_t y, x;

  setRegisterAngularMode(REGISTER_X, AM_NONE);
  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(realIsZero(&y)) {
      if(getFlag(FLAG_DANGER)) {
        realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
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
        realToReal16((realIsPositive(&y) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIRe16:", "cannot divide a long integer by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
    realDivide(&y, &x, &x, &ctxtReal39);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
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

  real39_t y, x;
  uint32_t yAngularMode;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

  if(realIsZero(&x)) {
    if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_Y))) {
      if(getFlag(FLAG_DANGER)) {
        realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
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
        realToReal16((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
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
    real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
    yAngularMode = getRegisterAngularMode(REGISTER_Y);

    if(yAngularMode == AM_NONE) {
      realDivide(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      if(currentAngularMode == AM_DMS) {
        convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
        realDivide(&y, &x, &x, &ctxtReal39);
        convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
        realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
        checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
        realDivide(&y, &x, &x, &ctxtReal39);
        realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      }
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
    }
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

  real39_t y;
  complex39_t x;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);

  divRe39Co39(&y, &x, &x);

  realToReal16(&x.real, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&x.imag, REGISTER_IMAG16_DATA(REGISTER_X));
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

  real39_t a, b, c;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);

  realDivide(&a, &c, &a, &ctxtReal39);
  realDivide(&b, &c, &b, &ctxtReal39);

  realToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
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
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, c);

  if(longIntegerIsZero(c)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonIShoI:", "cannot divide a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longIntegerDivideQuotientRemainder(a, c, a, c);
    convertLongIntegerToLongIntegerRegister(a, REGISTER_X);
  }

  longIntegerFree(a);
  longIntegerFree(c);
}



/********************************************//**
 * \brief Y(short integer) ÷ X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoILonI(void) {
  longInteger_t a, c;

  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Y, REGISTER_Y);
  convertLongIntegerRegisterToLongInteger(REGISTER_Y, a);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, c);

  if(longIntegerIsZero(c)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoILonI:", "cannot divide a short integer by 0", NULL, NULL);
    #endif
  }
  else {
    longIntegerDivideQuotientRemainder(a, c, a, c);
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

  real39_t y, x;

  setRegisterAngularMode(REGISTER_X, AM_NONE);
  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(realIsZero(&y)) {
      if(getFlag(FLAG_DANGER)) {
        realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
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
        realToReal34((realIsPositive(&y) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIRe34:", "cannot divide a long integer by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    realDivide(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
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

  real39_t y, x;
  uint32_t yAngularMode;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(realIsZero(&x)) {
    if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y))) {
      if(getFlag(FLAG_DANGER)) {
        realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
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
        realToReal34((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIRe34:", "cannot divide a real34 by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    yAngularMode = getRegisterAngularMode(REGISTER_Y);

    if(yAngularMode == AM_NONE) {
      realDivide(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      if(currentAngularMode == AM_DMS) {
        convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
        realDivide(&y, &x, &x, &ctxtReal39);
        convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
        realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
        checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
        realDivide(&y, &x, &x, &ctxtReal39);
        realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      }
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
    }
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

  real39_t y;
  complex39_t x;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  divRe39Co39(&y, &x, &x);

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
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

  real39_t a, b, c;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realDivide(&a, &c, &a, &ctxtReal39);
  realDivide(&b, &c, &b, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
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
      realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
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
      realToReal16((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16Re16:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real39_t y, x;
    uint32_t yAngularMode, xAngularMode;

    real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
    yAngularMode = getRegisterAngularMode(REGISTER_Y);
    real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
    xAngularMode = getRegisterAngularMode(REGISTER_X);

    if(yAngularMode != AM_NONE && xAngularMode != AM_NONE) { // angle / angle
      if(yAngularMode == AM_DMS) {
        convertAngle39FromTo(&y, AM_DMS, AM_DEGREE);
        yAngularMode = AM_DEGREE;
      }
      if(xAngularMode == AM_DMS) {
        convertAngle39FromTo(&x, AM_DMS, AM_DEGREE);
        xAngularMode = AM_DEGREE;
      }
      convertAngle39FromTo(&x, xAngularMode, yAngularMode);
      realDivide(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, AM_NONE);
    }
    else if(yAngularMode == AM_NONE) { // real / (real or angle)
      real16Divide(REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, AM_NONE);
    }
    else { // angle / real
      if(yAngularMode == AM_DMS) {
        convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
        yAngularMode = AM_DEGREE;
      }

      realDivide(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, yAngularMode, currentAngularMode);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
      if(currentAngularMode == AM_DMS) {
        checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
      }
    }
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

  real39_t y;
  complex39_t x;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);

  divRe39Co39(&y, &x, &x);

  realToReal16(&x.real, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&x.imag, REGISTER_IMAG16_DATA(REGISTER_X));
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
  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
  complex16Copy(REGISTER_COMPLEX16_DATA(REGISTER_Y), REGISTER_COMPLEX16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(real16) ÷ X(short integer) ==> X(real16)
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

  real39_t y, x;
  uint32_t yAngularMode;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

  if(realIsZero(&x)) {
    if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_Y))) {
      if(getFlag(FLAG_DANGER)) {
        realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divRe16ShoI:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getFlag(FLAG_DANGER)) {
        realToReal16((real16IsPositive(REGISTER_REAL16_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divRe16ShoI:", "cannot divide a real16 by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
    yAngularMode = getRegisterAngularMode(REGISTER_Y);

    if(yAngularMode == AM_NONE) {
      realDivide(&y, &x, &x, &ctxtReal39);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      if(currentAngularMode == AM_DMS) {
        convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
        realDivide(&y, &x, &x, &ctxtReal39);
        convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
        realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
        checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
        realDivide(&y, &x, &x, &ctxtReal39);
        realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      }
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
    }
  }
}



/********************************************//**
 * \brief Y(short integer) ÷ X(real16) ==> X(real16)
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

  real39_t y, x;

  setRegisterAngularMode(REGISTER_X, AM_NONE);
  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    if(realIsZero(&y)) {
      if(getFlag(FLAG_DANGER)) {
        realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
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
        realToReal16((realIsPositive(&y) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIRe16:", "cannot divide a long integer by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
    realDivide(&y, &x, &x, &ctxtReal39);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
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
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
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
      realToReal34((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe16Re34:", "cannot divide a real16 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real39_t y, x;
    uint32_t yAngularMode, xAngularMode;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    yAngularMode = getRegisterAngularMode(REGISTER_Y);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    xAngularMode = getRegisterAngularMode(REGISTER_X);

    if(yAngularMode != AM_NONE && xAngularMode != AM_NONE) { // angle / angle
      if(yAngularMode == AM_DMS) {
        convertAngle39FromTo(&y, AM_DMS, AM_DEGREE);
        yAngularMode = AM_DEGREE;
      }
      if(xAngularMode == AM_DMS) {
        convertAngle39FromTo(&x, AM_DMS, AM_DEGREE);
        xAngularMode = AM_DEGREE;
      }
      convertAngle39FromTo(&x, xAngularMode, yAngularMode);
      realDivide(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, AM_NONE);
    }
    else if(yAngularMode == AM_NONE) { // real / (real or angle)
      real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, AM_NONE);
    }
    else { // angle / real
      if(yAngularMode == AM_DMS) {
        convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
        yAngularMode = AM_DEGREE;
      }

      realDivide(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, yAngularMode, currentAngularMode);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
      if(currentAngularMode == AM_DMS) {
        checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
      }
    }
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
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
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
      realToReal34((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34Re16:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real39_t y, x;
    uint32_t yAngularMode, xAngularMode;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    yAngularMode = getRegisterAngularMode(REGISTER_Y);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    xAngularMode = getRegisterAngularMode(REGISTER_X);

    if(yAngularMode != AM_NONE && xAngularMode != AM_NONE) { // angle / angle
      if(yAngularMode == AM_DMS) {
        convertAngle39FromTo(&y, AM_DMS, AM_DEGREE);
        yAngularMode = AM_DEGREE;
      }
      if(xAngularMode == AM_DMS) {
        convertAngle39FromTo(&x, AM_DMS, AM_DEGREE);
        xAngularMode = AM_DEGREE;
      }
      convertAngle39FromTo(&x, xAngularMode, yAngularMode);
      realDivide(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, AM_NONE);
    }
    else if(yAngularMode == AM_NONE) { // real / (real or angle)
      real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, AM_NONE);
    }
    else { // angle / real
      if(yAngularMode == AM_DMS) {
        convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
        yAngularMode = AM_DEGREE;
      }

      realDivide(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, yAngularMode, currentAngularMode);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
      if(currentAngularMode == AM_DMS) {
        checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
      }
    }
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

  real39_t y;
  complex39_t x;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  divRe39Co39(&y, &x, &x);

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
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
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
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

  complex39_t y, x;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &y.imag);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);

  divCo39Co39(&y, &x, &x);

  realToReal16(&x.real, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&x.imag, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex16) ÷ X(short integer) ==> X(complex16)
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

  real39_t a, b, c;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &a);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &b);
  convertShortIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realDivide(&a, &c, &a, &ctxtReal39);
  realDivide(&b, &c, &b, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
  realToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(short integer) ÷ X(complex16) ==> X(complex16)
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

  real39_t y;
  complex39_t x;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);

  divRe39Co39(&y, &x, &x);

  realToReal16(&x.real, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&x.imag, REGISTER_IMAG16_DATA(REGISTER_X));
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
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
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

  real39_t y;
  complex39_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

  divRe39Co39(&y, &x, &x);

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
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

  complex39_t y, x;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &y.imag);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  divCo39Co39(&y, &x, &x);

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
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

  complex39_t y, x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &y.imag);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &x.imag);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

  divCo39Co39(&y, &x, &x);

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
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
 * \brief Y(time) ÷ X(short integer) ==> X(time)
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
 * \brief Y(real16 matrix) ÷ X(short integer) ==> X(real16 matrix)
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
 * \brief Y(complex16 matrix) ÷ X(short integer) ==> X(complex16 matrix)
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



/******************************************************************************************************************************************************************************************/
/* short integer / ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(short integer) ÷ X(short integer) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIShoI(void) {
  int16_t sign;
  uint64_t value;

  convertShortIntegerRegisterToUInt64(REGISTER_X, &sign, &value);
  if(value == 0) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoIShoI:", "cannot divide a short integer by 0", NULL, NULL);
    #endif
  }
  else {
    *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intDivide(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
    setRegisterShortIntegerBase(REGISTER_X, getRegisterShortIntegerBase(REGISTER_Y));
  }
}



/********************************************//**
 * \brief Y(short integer) ÷ X(real34) ==> X(real34)
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

  real39_t y, x;

  setRegisterAngularMode(REGISTER_X, AM_NONE);
  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(realIsZero(&y)) {
      if(getFlag(FLAG_DANGER)) {
        realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divShoIRe34:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getFlag(FLAG_DANGER)) {
        realToReal34((realIsPositive(&y) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divShoIRe34:", "cannot divide a short integer by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    realDivide(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(real34) ÷ X(short integer) ==> X(real34)
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

  real39_t y, x;
  uint32_t yAngularMode;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(realIsZero(&x)) {
    if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y))) {
      if(getFlag(FLAG_DANGER)) {
        realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divRe34ShoI:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getFlag(FLAG_DANGER)) {
        realToReal34((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divRe34ShoI:", "cannot divide a real34 by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    yAngularMode = getRegisterAngularMode(REGISTER_Y);

    if(yAngularMode == AM_NONE) {
      realDivide(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      if(currentAngularMode == AM_DMS) {
        convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
        realDivide(&y, &x, &x, &ctxtReal39);
        convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
        realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
        checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
        realDivide(&y, &x, &x, &ctxtReal39);
        realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      }
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
    }
  }
}



/********************************************//**
 * \brief Y(short integer) ÷ X(complex34) ==> X(complex34)
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

  real39_t y;
  complex39_t x;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  divRe39Co39(&y, &x, &x);

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ÷ X(short integer) ==> X(complex34)
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
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
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
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
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
      realToReal34((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRe34Re34:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real39_t y, x;
    uint32_t yAngularMode, xAngularMode;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    yAngularMode = getRegisterAngularMode(REGISTER_Y);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    xAngularMode = getRegisterAngularMode(REGISTER_X);

    if(yAngularMode != AM_NONE && xAngularMode != AM_NONE) { // angle / angle
      if(yAngularMode == AM_DMS) {
        convertAngle39FromTo(&y, AM_DMS, AM_DEGREE);
        yAngularMode = AM_DEGREE;
      }
      if(xAngularMode == AM_DMS) {
        convertAngle39FromTo(&x, AM_DMS, AM_DEGREE);
        xAngularMode = AM_DEGREE;
      }
      convertAngle39FromTo(&x, xAngularMode, yAngularMode);
      realDivide(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, AM_NONE);
    }
    else if(yAngularMode == AM_NONE) { // real / (real or angle)
      real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, AM_NONE);
    }
    else { // angle / real
      if(yAngularMode == AM_DMS) {
        convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
        yAngularMode = AM_DEGREE;
      }

      realDivide(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, yAngularMode, currentAngularMode);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
      if(currentAngularMode == AM_DMS) {
        checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
      }
    }
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

  real39_t y;
  complex39_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  divRe39Co39(&y, &x, &x);

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
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
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
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

  complex39_t y, x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &y.imag);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &x.imag);

  divCo39Co39(&y, &x, &x);

  realToReal34(&x.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&x.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}
