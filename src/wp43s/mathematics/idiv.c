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
 * \file idiv.c
 ***********************************************/

#include "mathematics/idiv.h"

#include "debug.h"
#include "error.h"
#include "integers.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"



TO_QSPI void (* const idiv[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX |    regY ==>   1             2             3          4          5          6          7          8           9             10
//      V               Long integer  Real34        Complex34  Time       Date       String     Real34 mat Complex34 m Short integer Config data
/*  1 Long integer  */ {idivLonILonI, idivRealLonI, idivError, idivError, idivError, idivError, idivError, idivError,  idivShoILonI, idivError},
/*  2 Real34        */ {idivLonIReal, idivRealReal, idivError, idivError, idivError, idivError, idivError, idivError,  idivShoIReal, idivError},
/*  3 Complex34     */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError},
/*  4 Time          */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError},
/*  5 Date          */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError},
/*  6 String        */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError},
/*  7 Real34 mat    */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError},
/*  8 Complex34 mat */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError},
/*  9 Short integer */ {idivLonIShoI, idivRealShoI, idivError, idivError, idivError, idivError, idivError, idivError,  idivShoIShoI, idivError},
/* 10 Config data   */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError}
};



/********************************************//**
 * \brief Data type error in IDiv
 *
 * \param[in] unusedButMandatoryParameter
 * \return void
 ***********************************************/
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
void idivError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    sprintf(errorMessage, "cannot IDIV %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnIDiv:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
}
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)



/********************************************//**
 * \brief regX ==> regL and regY idiv regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter
 * \return void
 ***********************************************/
void fnIDiv(uint16_t unusedButMandatoryParameter) {
  if(!saveLastX()) return;

  idiv[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, false, REGISTER_X, REGISTER_Y, -1);
}



/******************************************************************************************************************************************************************************************/
/* long integer idiv ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) idiv X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivLonILonI(void) {
  longInteger_t x;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function idivLonILonI:", "cannot IDIV a long integer by 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    longInteger_t y;

    convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
    longIntegerDivide(y, x, x);
    convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

    longIntegerFree(y);
  }

  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) idiv X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivLonIShoI(void) {
  longInteger_t x;

  convertShortIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function idivLonIShoI:", "cannot IDIV a long integer by 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    longInteger_t y;

    convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
    longIntegerDivide(y, x, x);
    convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

    longIntegerFree(y);
  }

  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(short integer) idiv X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivShoILonI(void) {
  longInteger_t x;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function idivShoILonI:", "cannot IDIV a short integer by 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    longInteger_t y;

    convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);
    longIntegerDivide(y, x, x);
    convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

    longIntegerFree(y);
  }

  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) idiv X(real34) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivLonIReal(void) {
  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function idivLonIReal:", "cannot IDIV a long integer by 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  real_t y, x;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  realDivide(&y, &x, &x, &ctxtReal39);
  convertRealToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
}



/********************************************//**
 * \brief Y(real34) idiv X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivRealLonI(void) {
  real_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function idivRealLonI:", "cannot IDIV a real34 by 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  real_t y;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  realDivide(&y, &x, &x, &ctxtReal39);
  convertRealToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
}



/******************************************************************************************************************************************************************************************/
/* short integer idiv ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(short integer) idiv X(short integer) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivShoIShoI(void) {
  int16_t sign;
  uint64_t value;

  convertShortIntegerRegisterToUInt64(REGISTER_X, &sign, &value);
  if(value == 0) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function idivShoIShoI:", "cannot IDIV a short integer by 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intDivide(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
    setRegisterShortIntegerBase(REGISTER_X, getRegisterShortIntegerBase(REGISTER_Y));
  }
}



/********************************************//**
 * \brief Y(short integer) idiv X(real34) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivShoIReal(void) {
  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function idivShoIReal:", "cannot IDIV a short integer by 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  real_t y, x;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  realDivide(&y, &x, &x, &ctxtReal39);
  convertRealToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
}



/********************************************//**
 * \brief Y(real34) idiv X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivRealShoI(void) {
  real_t x;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function idivRealShoI:", "cannot IDIV a real34 by 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  real_t y;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  realDivide(&y, &x, &x, &ctxtReal39);
  convertRealToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
}



/******************************************************************************************************************************************************************************************/
/* real34 idiv ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) idiv X(real34) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivRealReal(void) {
  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function idivRealReal:", "cannot IDIV a real34 by 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  real_t y, x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  realDivide(&y, &x, &x, &ctxtReal39);
  convertRealToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
}
