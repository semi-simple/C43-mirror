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
 * \file idivr.c
 ***********************************************/

#include "wp43s.h"



void (* const idivr[12][12])(void) = {
// regX |    regY ==>   1              2              3           4           5           6           7           8           9            10             11             12
//      V               Long integer   Real16         Complex16   Angle16     Time        Date        String      Real16 mat  Complex16 m  Short integer  Real34         Complex34
/*  1 Long integer  */ {idivrLonILonI, idivrRe16LonI, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError,  idivrShoILonI, idivrRe34LonI, idivrError},
/*  2 Real16        */ {idivrLonIRe16, idivrRe16Re16, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError,  idivrShoIRe16, idivrRe34Re16, idivrError},
/*  3 Complex16     */ {idivrError,    idivrError,    idivrError, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError,  idivrError,    idivrError,    idivrError},
/*  4 Angle16       */ {idivrError,    idivrError,    idivrError, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError,  idivrError,    idivrError,    idivrError},
/*  5 Time          */ {idivrError,    idivrError,    idivrError, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError,  idivrError,    idivrError,    idivrError},
/*  6 Date          */ {idivrError,    idivrError,    idivrError, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError,  idivrError,    idivrError,    idivrError},
/*  7 String        */ {idivrError,    idivrError,    idivrError, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError,  idivrError,    idivrError,    idivrError},
/*  8 Real16 mat    */ {idivrError,    idivrError,    idivrError, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError,  idivrError,    idivrError,    idivrError},
/*  9 Complex16 mat */ {idivrError,    idivrError,    idivrError, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError,  idivrError,    idivrError,    idivrError},
/* 10 Short integer */ {idivrLonIShoI, idivrRe16ShoI, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError,  idivrShoIShoI, idivrRe34ShoI, idivrError},
/* 11 Real34        */ {idivrLonIRe34, idivrRe16Re34, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError,  idivrShoIRe34, idivrRe34Re34, idivrError},
/* 12 Complex34     */ {idivrError,    idivrError,    idivrError, idivrError, idivrError, idivrError, idivrError, idivrError, idivrError,  idivrError,    idivrError,    idivrError}
};



/********************************************//**
 * \brief Data type error in IDiv
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void idivrError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot IDIVR %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnIDivR:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regY idivr regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnIDivR(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  idivr[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, false, false, REGISTER_X, REGISTER_Y, -1);
  adjustResult(REGISTER_Y, false, false, REGISTER_X, REGISTER_Y, -1);
}



/******************************************************************************************************************************************************************************************/
/* long integer idivr ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) idivr X(long integer) ==> X(long integer), Y(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrLonILonI(void) {
  longInteger_t x;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrLonILonI:", "cannot IDIVR a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t y, quotient, remainder;

    convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);

    longIntegerInit(quotient);
    longIntegerInit(remainder);
    longIntegerDivideQuotientRemainder(y, x, quotient, remainder);

    convertLongIntegerToLongIntegerRegister(quotient, REGISTER_X);
    convertLongIntegerToLongIntegerRegister(remainder, REGISTER_Y);

    longIntegerFree(y);
    longIntegerFree(quotient);
    longIntegerFree(remainder);
  }

  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) idivr X(real16) ==> X(long integer), Y(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrLonIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrLonIRe16:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrLonIRe16:", "cannot IDIVR a long integer by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, q;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  realDivide(&y, &x, &q, &ctxtReal39);
  realToIntegralValue(&q, &q, DEC_ROUND_DOWN, &ctxtReal39);
  convertRealToLongIntegerRegister(&q, REGISTER_X, DEC_ROUND_DOWN);

  realDivideRemainder(&y, &x, &y, &ctxtReal39);
  reallocateRegister(REGISTER_Y, dtReal16, REAL16_SIZE, AM_NONE);
  realToReal16(&y, REGISTER_REAL16_DATA(REGISTER_Y));
}



/********************************************//**
 * \brief Y(real16) idivr X(long integer) ==> X(long integer), Y(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe16LonI:", "cannot use NaN as Y input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe16LonI:", "cannot IDIVR a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, q;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  realDivide(&y, &x, &q, &ctxtReal39);
  realToIntegralValue(&q, &q, DEC_ROUND_DOWN, &ctxtReal39);
  convertRealToLongIntegerRegister(&q, REGISTER_X, DEC_ROUND_DOWN);

  realDivideRemainder(&y, &x, &y, &ctxtReal39);
  realToReal16(&y, REGISTER_REAL16_DATA(REGISTER_Y));
  setRegisterAngularMode(REGISTER_Y, AM_NONE);
}



/********************************************//**
 * \brief Y(long integer) idivr X(short integer) ==> X(long integer), Y(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrLonIShoI(void) {
  longInteger_t x;

  convertShortIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrLonIShoI:", "cannot IDIVR a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t y, quotient, remainder;

    convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);

    longIntegerInit(quotient);
    longIntegerInit(remainder);
    longIntegerDivideQuotientRemainder(y, x, quotient, remainder);

    convertLongIntegerToLongIntegerRegister(quotient, REGISTER_X);
    convertLongIntegerToLongIntegerRegister(remainder, REGISTER_Y);

    longIntegerFree(y);
    longIntegerFree(quotient);
    longIntegerFree(remainder);
  }

  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(short integer) idivr X(long integer) ==> X(long integer), Y(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrShoILonI(void) {
  longInteger_t x;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrShoILonI:", "cannot IDIVR a short integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t y, quotient, remainder;
    uint32_t baseY;

    baseY = getRegisterShortIntegerBase(REGISTER_Y);
    convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);

    longIntegerInit(quotient);
    longIntegerInit(remainder);
    longIntegerDivideQuotientRemainder(y, x, quotient, remainder);

    convertLongIntegerToLongIntegerRegister(quotient, REGISTER_X);
    convertLongIntegerToShortIntegerRegister(remainder, baseY, REGISTER_Y);

    longIntegerFree(y);
    longIntegerFree(quotient);
    longIntegerFree(remainder);
  }

  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) idivr X(real34) ==> X(long integer), Y(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrLonIRe34:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrLonIRe34:", "cannot IDIVR a long integer by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, q;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  realDivide(&y, &x, &q, &ctxtReal39);
  realToIntegralValue(&q, &q, DEC_ROUND_DOWN, &ctxtReal39);
  convertRealToLongIntegerRegister(&q, REGISTER_X, DEC_ROUND_DOWN);

  realDivideRemainder(&y, &x, &y, &ctxtReal39);
  reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));
}



/********************************************//**
 * \brief Y(real34) idivr X(long integer) ==> X(long integer), Y(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe34LonI:", "cannot use NaN as Y input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe34LonI:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, q;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  realDivide(&y, &x, &q, &ctxtReal39);
  realToIntegralValue(&q, &q, DEC_ROUND_DOWN, &ctxtReal39);
  convertRealToLongIntegerRegister(&q, REGISTER_X, DEC_ROUND_DOWN);

  realDivideRemainder(&y, &x, &y, &ctxtReal39);
  realToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));
  setRegisterAngularMode(REGISTER_Y, AM_NONE);
}



/******************************************************************************************************************************************************************************************/
/* real16 idivr ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16) idivr X(real16) ==> X(long integer), Y(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe16Re16:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe16Re16:", "cannot IDIVR a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t x, y, q;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  realDivide(&y, &x, &q, &ctxtReal39);
  realToIntegralValue(&q, &q, DEC_ROUND_DOWN, &ctxtReal39);
  convertRealToLongIntegerRegister(&q, REGISTER_X, DEC_ROUND_DOWN);

  realDivideRemainder(&y, &x, &y, &ctxtReal39);
  realToReal16(&y, REGISTER_REAL16_DATA(REGISTER_Y));
  setRegisterAngularMode(REGISTER_Y, AM_NONE);
}



/********************************************//**
 * \brief Y(real16) idivr X(short integer) ==> X(long integer), Y(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrRe16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe16ShoI:", "cannot use NaN as Y input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe16ShoI:", "cannot IDIVR a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, q;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  realDivide(&y, &x, &q, &ctxtReal39);
  realToIntegralValue(&q, &q, DEC_ROUND_DOWN, &ctxtReal39);
  convertRealToLongIntegerRegister(&q, REGISTER_X, DEC_ROUND_DOWN);

  realDivideRemainder(&y, &x, &y, &ctxtReal39);
  realToReal16(&y, REGISTER_REAL16_DATA(REGISTER_Y));
  setRegisterAngularMode(REGISTER_Y, AM_NONE);
}



/********************************************//**
 * \brief Y(short integer) idivr X(real16) ==> X(long integer), Y(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrShoIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrShoIRe16:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrShoIRe16:", "cannot IDIVR a short integer by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, q;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  realDivide(&y, &x, &q, &ctxtReal39);
  realToIntegralValue(&q, &q, DEC_ROUND_DOWN, &ctxtReal39);
  convertRealToLongIntegerRegister(&q, REGISTER_X, DEC_ROUND_DOWN);

  realDivideRemainder(&y, &x, &y, &ctxtReal39);
  reallocateRegister(REGISTER_Y, dtReal16, REAL16_SIZE, AM_NONE);
  realToReal16(&y, REGISTER_REAL16_DATA(REGISTER_Y));
}



/********************************************//**
 * \brief Y(real16) idivr X(real34) ==> X(long integer), Y(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrRe16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe16Re34:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe16Re34:", "cannot IDIVR a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t x, y, q;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  realDivide(&y, &x, &q, &ctxtReal39);
  realToIntegralValue(&q, &q, DEC_ROUND_DOWN, &ctxtReal39);
  convertRealToLongIntegerRegister(&q, REGISTER_X, DEC_ROUND_DOWN);

  realDivideRemainder(&y, &x, &y, &ctxtReal39);
  reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));
}



/********************************************//**
 * \brief Y(real34) idivr X(real16) ==> X(long integer), Y(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrRe34Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe34Re16:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe34Re16:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t x, y, q;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  realDivide(&y, &x, &q, &ctxtReal39);
  realToIntegralValue(&q, &q, DEC_ROUND_DOWN, &ctxtReal39);
  convertRealToLongIntegerRegister(&q, REGISTER_X, DEC_ROUND_DOWN);

  realDivideRemainder(&y, &x, &y, &ctxtReal39);
  realToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));
  setRegisterAngularMode(REGISTER_Y, AM_NONE);
}



/******************************************************************************************************************************************************************************************/
/* short integer idivr ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(short integer) idivr X(short integer) ==> X(short integer), Y(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrShoIShoI(void) {
  longInteger_t x;

  convertShortIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrLonILonI:", "cannot IDIVR a short integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t y, quotient, remainder;
    uint32_t baseY;

    baseY = getRegisterShortIntegerBase(REGISTER_Y);
    convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);

    longIntegerInit(quotient);
    longIntegerInit(remainder);
    longIntegerDivideQuotientRemainder(y, x, quotient, remainder);

    convertLongIntegerToShortIntegerRegister(quotient, baseY, REGISTER_X);
    convertLongIntegerToShortIntegerRegister(remainder, baseY, REGISTER_Y);

    longIntegerFree(y);
    longIntegerFree(quotient);
    longIntegerFree(remainder);
  }

  longIntegerFree(x);

}



/********************************************//**
 * \brief Y(short integer) idivr X(real34) ==> X(long integer), Y(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrShoIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrShoIRe34:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrShoIRe34:", "cannot IDIVR a short integer by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, q;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  realDivide(&y, &x, &q, &ctxtReal39);
  realToIntegralValue(&q, &q, DEC_ROUND_DOWN, &ctxtReal39);
  convertRealToLongIntegerRegister(&q, REGISTER_X, DEC_ROUND_DOWN);

  realDivideRemainder(&y, &x, &y, &ctxtReal39);
  reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));
}



/********************************************//**
 * \brief Y(real34) idivr X(short integer) ==> X(long integer), Y(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrRe34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe34ShoI:", "cannot use NaN as Y input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe34ShoI:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, q;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  realDivide(&y, &x, &q, &ctxtReal39);
  realToIntegralValue(&q, &q, DEC_ROUND_DOWN, &ctxtReal39);
  convertRealToLongIntegerRegister(&q, REGISTER_X, DEC_ROUND_DOWN);

  realDivideRemainder(&y, &x, &y, &ctxtReal39);
  realToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));
  setRegisterAngularMode(REGISTER_Y, AM_NONE);
}



/******************************************************************************************************************************************************************************************/
/* real34 idivr ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) idivr X(real34) ==> X(long integer), Y(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void idivrRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe34Re34:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivrRe34Re34:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t x, y, q;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  realDivide(&y, &x, &q, &ctxtReal39);
  realToIntegralValue(&q, &q, DEC_ROUND_DOWN, &ctxtReal39);
  convertRealToLongIntegerRegister(&q, REGISTER_X, DEC_ROUND_DOWN);

  realDivideRemainder(&y, &x, &y, &ctxtReal39);
  realToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));
  setRegisterAngularMode(REGISTER_Y, AM_NONE);
}
