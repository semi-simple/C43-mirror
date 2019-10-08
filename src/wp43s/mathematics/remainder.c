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
 * \file remainder.c
 ***********************************************/

#include "wp43s.h"



void (* const rmd[12][12])(void) = {
// regX |    regY ==>   1            2            3         4         5         6         7         8          9           10            11           12
//      V               Long integer Real16       Complex16 Angle16   Time      Date      String    Real16 mat Complex16 m Short integer Real34       Complex34
/*  1 Long integer  */ {rmdLonILonI, rmdRe16LonI, rmdError, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdShoILonI,  rmdRe34LonI, rmdError},
/*  2 Real16        */ {rmdLonIRe16, rmdRe16Re16, rmdError, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdShoIRe16,  rmdRe34Re16, rmdError},
/*  3 Complex16     */ {rmdError,    rmdError,    rmdError, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdError,     rmdError,    rmdError},
/*  4 Angle16       */ {rmdError,    rmdError,    rmdError, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdError,     rmdError,    rmdError},
/*  5 Time          */ {rmdError,    rmdError,    rmdError, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdError,     rmdError,    rmdError},
/*  6 Date          */ {rmdError,    rmdError,    rmdError, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdError,     rmdError,    rmdError},
/*  7 String        */ {rmdError,    rmdError,    rmdError, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdError,     rmdError,    rmdError},
/*  8 Real16 mat    */ {rmdError,    rmdError,    rmdError, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdError,     rmdError,    rmdError},
/*  9 Complex16 mat */ {rmdError,    rmdError,    rmdError, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdError,     rmdError,    rmdError},
/* 10 Short integer */ {rmdLonIShoI, rmdRe16ShoI, rmdError, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdShoIShoI,  rmdRe34ShoI, rmdError},
/* 11 Real34        */ {rmdLonIRe34, rmdRe16Re34, rmdError, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdShoIRe34,  rmdRe34Re34, rmdError},
/* 12 Complex34     */ {rmdError,    rmdError,    rmdError, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdError,     rmdError,    rmdError}
};



/********************************************//**
 * \brief Data type error in IDiv
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void rmdError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot RMD %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnRmd:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regY rmd regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnRmd(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  rmd[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, false, REGISTER_X, REGISTER_Y, -1);
  refreshRegisterLine(REGISTER_Y);
}



/******************************************************************************************************************************************************************************************/
/* long integer rmd ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) rmd X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdLonILonI(void) {
  longInteger_t x;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdLonILonI:", "cannot IDIVR a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t y, remainder;

    convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);

    longIntegerInit(remainder);
    longIntegerDivideRemainder(y, x, remainder);

    convertLongIntegerToLongIntegerRegister(remainder, REGISTER_X);

    longIntegerFree(y);
    longIntegerFree(remainder);
  }

  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) rmd X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdLonIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdLonIRe16:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdLonIRe16:", "cannot IDIVR a long integer by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y, x;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &y);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);

  realIcDivideRemainder(&y, &x, &x);
  realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real16) rmd X(long integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe16LonI:", "cannot use NaN as Y input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  realIc_t x;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &x);
  if(realIcIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe16LonI:", "cannot IDIVR a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y);

  realIcDivideRemainder(&y, &x, &x);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
  realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) rmd X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdLonIShoI(void) {
  longInteger_t x;

  convertShortIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdLonIShoI:", "cannot IDIVR a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t y, remainder;

    convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);

    longIntegerInit(remainder);
    longIntegerDivideRemainder(y, x, remainder);

    convertLongIntegerToLongIntegerRegister(remainder, REGISTER_X);

    longIntegerFree(y);
    longIntegerFree(remainder);
  }

  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(short integer) rmd X(long integer) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdShoILonI(void) {
  longInteger_t x;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdShoILonI:", "cannot IDIVR a short integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t y, remainder;
    uint32_t baseY;

    baseY = getRegisterShortIntegerBase(REGISTER_Y);
    convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);

    longIntegerInit(remainder);
    longIntegerDivideRemainder(y, x, remainder);

    convertLongIntegerToShortIntegerRegister(remainder, baseY, REGISTER_X);

    longIntegerFree(y);
    longIntegerFree(remainder);
  }

  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) rmd X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdLonIRe34:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdLonIRe34:", "cannot IDIVR a long integer by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y, x;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &y);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realIcDivideRemainder(&y, &x, &x);
  realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) rmd X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe34LonI:", "cannot use NaN as Y input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  realIc_t x;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &x);
  if(realIcIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe34LonI:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  realIcDivideRemainder(&y, &x, &x);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real16 rmd ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16) rmd X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe16Re16:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe16Re16:", "cannot IDIVR a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t x, y;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);

  realIcDivideRemainder(&y, &x, &x);
  realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real16) rmd X(short integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdRe16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe16ShoI:", "cannot use NaN as Y input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  realIc_t x;

  convertShortIntegerRegisterToRealIc(REGISTER_X, &x);
  if(realIcIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe16ShoI:", "cannot IDIVR a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  realIcDivideRemainder(&y, &x, &x);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
  realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(short integer) rmd X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdShoIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdShoIRe16:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdShoIRe16:", "cannot IDIVR a short integer by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y, x;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &y);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);

  realIcDivideRemainder(&y, &x, &x);
  realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real16) rmd X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdRe16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe16Re34:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe16Re34:", "cannot IDIVR a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t x, y;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realIcDivideRemainder(&y, &x, &x);
  realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) rmd X(real16) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdRe34Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe34Re16:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe34Re16:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t x, y;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);

  realIcDivideRemainder(&y, &x, &x);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* short integer rmd ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(short integer) rmd X(short integer) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdShoIShoI(void) {
  longInteger_t x;

  convertShortIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdLonILonI:", "cannot IDIVR a short integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t y, remainder;
    uint32_t baseY;

    baseY = getRegisterShortIntegerBase(REGISTER_Y);
    convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);

    longIntegerInit(remainder);
    longIntegerDivideRemainder(y, x, remainder);

    convertLongIntegerToShortIntegerRegister(remainder, baseY, REGISTER_X);

    longIntegerFree(y);
    longIntegerFree(remainder);
  }

  longIntegerFree(x);

}



/********************************************//**
 * \brief Y(short integer) rmd X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdShoIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdShoIRe34:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdShoIRe34:", "cannot IDIVR a short integer by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y, x;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &y);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realIcDivideRemainder(&y, &x, &x);
  realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) rmd X(short integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdRe34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe34ShoI:", "cannot use NaN as Y input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  realIc_t x;

  convertShortIntegerRegisterToRealIc(REGISTER_X, &x);
  if(realIcIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe34ShoI:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);

  realIcDivideRemainder(&y, &x, &x);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real34 rmd ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) rmd X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe34Re34:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRe34Re34:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t x, y;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realIcDivideRemainder(&y, &x, &x);
  realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}
