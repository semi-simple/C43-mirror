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
 * \file modulo.c
 ***********************************************/

#include "wp43s.h"



void (* const mod[12][12])(void) = {
// regX |    regY ==>   1            2            3         4         5         6         7         8          9           10            11           12
//      V               Long integer Real16       Complex16 Angle16   Time      Date      String    Real16 mat Complex16 m Short integer Real34       Complex34
/*  1 Long integer  */ {modLonILonI, modRe16LonI, modError, modError, modError, modError, modError, modError,  modError,   modShoILonI,  modRe34LonI, modError},
/*  2 Real16        */ {modLonIRe16, modRe16Re16, modError, modError, modError, modError, modError, modError,  modError,   modShoIRe16,  modRe34Re16, modError},
/*  3 Complex16     */ {modError,    modError,    modError, modError, modError, modError, modError, modError,  modError,   modError,     modError,    modError},
/*  4 Angle16       */ {modError,    modError,    modError, modError, modError, modError, modError, modError,  modError,   modError,     modError,    modError},
/*  5 Time          */ {modError,    modError,    modError, modError, modError, modError, modError, modError,  modError,   modError,     modError,    modError},
/*  6 Date          */ {modError,    modError,    modError, modError, modError, modError, modError, modError,  modError,   modError,     modError,    modError},
/*  7 String        */ {modError,    modError,    modError, modError, modError, modError, modError, modError,  modError,   modError,     modError,    modError},
/*  8 Real16 mat    */ {modError,    modError,    modError, modError, modError, modError, modError, modError,  modError,   modError,     modError,    modError},
/*  9 Complex16 mat */ {modError,    modError,    modError, modError, modError, modError, modError, modError,  modError,   modError,     modError,    modError},
/* 10 Short integer */ {modLonIShoI, modRe16ShoI, modError, modError, modError, modError, modError, modError,  modError,   modShoIShoI,  modRe34ShoI, modError},
/* 11 Real34        */ {modLonIRe34, modRe16Re34, modError, modError, modError, modError, modError, modError,  modError,   modShoIRe34,  modRe34Re34, modError},
/* 12 Complex34     */ {modError,    modError,    modError, modError, modError, modError, modError, modError,  modError,   modError,     modError,    modError}
};



/********************************************//**
 * \brief Data type error in IDiv
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void modError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot RMD %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnMod:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regY mod regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnMod(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  mod[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, false, REGISTER_X, REGISTER_Y, -1);
  refreshRegisterLine(REGISTER_Y);
}



/******************************************************************************************************************************************************************************************/
/* long integer mod ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) mod X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void modLonILonI(void) {
  longInteger_t x;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modLonILonI:", "cannot IDIVR a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t y, remainder;

    convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);

    longIntegerInit(remainder);
    longIntegerDivideRemainder(y, x, remainder);
    longIntegerAdd(remainder, x, remainder);
    longIntegerDivideRemainder(remainder, x, remainder);

    convertLongIntegerToLongIntegerRegister(remainder, REGISTER_X);

    longIntegerFree(y);
    longIntegerFree(remainder);
  }

  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) mod X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void modLonIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modLonIRe16:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modLonIRe16:", "cannot IDIVR a long integer by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, r;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);

  realDivideRemainder(&y, &x, &r, &ctxtReal39);
  if(!realIsZero(&r) && realSign(&y) != realSign(&x)) {
    realAdd(&r, &x, &r, &ctxtReal39);
  }

  realToReal16(&r, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real16) mod X(long integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void modRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe16LonI:", "cannot use NaN as Y input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe16LonI:", "cannot IDIVR a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, r;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);

  realDivideRemainder(&y, &x, &r, &ctxtReal39);
  if(!realIsZero(&r) && realSign(&y) != realSign(&x)) {
    realAdd(&r, &x, &r, &ctxtReal39);
  }

  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
  realToReal16(&r, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) mod X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void modLonIShoI(void) {
  longInteger_t x;

  convertShortIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modLonIShoI:", "cannot IDIVR a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t y, remainder;

    convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);

    longIntegerInit(remainder);
    longIntegerDivideRemainder(y, x, remainder);
    longIntegerAdd(remainder, x, remainder);
    longIntegerDivideRemainder(remainder, x, remainder);

    convertLongIntegerToLongIntegerRegister(remainder, REGISTER_X);

    longIntegerFree(y);
    longIntegerFree(remainder);
  }

  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(short integer) mod X(long integer) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void modShoILonI(void) {
  longInteger_t x;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modShoILonI:", "cannot IDIVR a short integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t y, remainder;
    uint32_t baseY;

    baseY = getRegisterShortIntegerBase(REGISTER_Y);
    convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);

    longIntegerInit(remainder);
    longIntegerDivideRemainder(y, x, remainder);
    longIntegerAdd(remainder, x, remainder);
    longIntegerDivideRemainder(remainder, x, remainder);

    convertLongIntegerToShortIntegerRegister(remainder, baseY, REGISTER_X);

    longIntegerFree(y);
    longIntegerFree(remainder);
  }

  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) mod X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void modLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modLonIRe34:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modLonIRe34:", "cannot IDIVR a long integer by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, r;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realDivideRemainder(&y, &x, &r, &ctxtReal39);
  if(!realIsZero(&r) && realSign(&y) != realSign(&x)) {
    realAdd(&r, &x, &r, &ctxtReal39);
  }

  realToReal34(&r, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) mod X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void modRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe34LonI:", "cannot use NaN as Y input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe34LonI:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, r;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  realDivideRemainder(&y, &x, &r, &ctxtReal39);
  if(!realIsZero(&r) && realSign(&y) != realSign(&x)) {
    realAdd(&r, &x, &r, &ctxtReal39);
  }

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&r, REGISTER_REAL34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real16 mod ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16) mod X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void modRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe16Re16:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe16Re16:", "cannot IDIVR a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t x, y, r;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);

  realDivideRemainder(&y, &x, &r, &ctxtReal39);
  if(!realIsZero(&r) && realSign(&y) != realSign(&x)) {
    realAdd(&r, &x, &r, &ctxtReal39);
  }

  realToReal16(&r, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real16) mod X(short integer) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void modRe16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe16ShoI:", "cannot use NaN as Y input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe16ShoI:", "cannot IDIVR a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, r;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);

  realDivideRemainder(&y, &x, &r, &ctxtReal39);
  if(!realIsZero(&r) && realSign(&y) != realSign(&x)) {
    realAdd(&r, &x, &r, &ctxtReal39);
  }

  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
  realToReal16(&r, REGISTER_REAL16_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(short integer) mod X(real16) ==> X(real16)
 *
 * \param void
 * \return void
 ***********************************************/
void modShoIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modShoIRe16:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modShoIRe16:", "cannot IDIVR a short integer by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, r;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);

  realDivideRemainder(&y, &x, &r, &ctxtReal39);
  if(!realIsZero(&r) && realSign(&y) != realSign(&x)) {
    realAdd(&r, &x, &r, &ctxtReal39);
  }

  realToReal16(&r, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real16) mod X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void modRe16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe16Re34:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe16Re34:", "cannot IDIVR a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t x, y, r;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realDivideRemainder(&y, &x, &r, &ctxtReal39);
  if(!realIsZero(&r) && realSign(&y) != realSign(&x)) {
    realAdd(&r, &x, &r, &ctxtReal39);
  }

  realToReal34(&r, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) mod X(real16) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void modRe34Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe34Re16:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe34Re16:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t x, y, r;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);

  realDivideRemainder(&y, &x, &r, &ctxtReal39);
  if(!realIsZero(&r) && realSign(&y) != realSign(&x)) {
    realAdd(&r, &x, &r, &ctxtReal39);
  }

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&r, REGISTER_REAL34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* short integer mod ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(short integer) mod X(short integer) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void modShoIShoI(void) {
  longInteger_t x;

  convertShortIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modLonILonI:", "cannot IDIVR a short integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t y, remainder;
    uint32_t baseY;

    baseY = getRegisterShortIntegerBase(REGISTER_Y);
    convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);

    longIntegerInit(remainder);
    longIntegerDivideRemainder(y, x, remainder);
    longIntegerAdd(remainder, x, remainder);
    longIntegerDivideRemainder(remainder, x, remainder);

    convertLongIntegerToShortIntegerRegister(remainder, baseY, REGISTER_X);

    longIntegerFree(y);
    longIntegerFree(remainder);
  }

  longIntegerFree(x);

}



/********************************************//**
 * \brief Y(short integer) mod X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void modShoIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modShoIRe34:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modShoIRe34:", "cannot IDIVR a short integer by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x, r;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realDivideRemainder(&y, &x, &r, &ctxtReal39);
  if(!realIsZero(&r) && realSign(&y) != realSign(&x)) {
    realAdd(&r, &x, &r, &ctxtReal39);
  }

  realToReal34(&r, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) mod X(short integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void modRe34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe34ShoI:", "cannot use NaN as Y input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe34ShoI:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, r;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);

  realDivideRemainder(&y, &x, &r, &ctxtReal39);
  if(!realIsZero(&r) && realSign(&y) != realSign(&x)) {
    realAdd(&r, &x, &r, &ctxtReal39);
  }

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&r, REGISTER_REAL34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real34 mod ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) mod X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void modRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe34Re34:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function modRe34Re34:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t x, y, r;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realDivideRemainder(&y, &x, &r, &ctxtReal39);
  if(!realIsZero(&r) && realSign(&y) != realSign(&x)) {
    realAdd(&r, &x, &r, &ctxtReal39);
  }

  realToReal34(&r, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}
