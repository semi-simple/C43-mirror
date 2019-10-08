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

#include "wp43s.h"



void (* const idiv[12][12])(void) = {
// regX |    regY ==>   1             2             3          4          5          6          7          8          9           10            11            12
//      V               Long integer  Real16        Complex16  Angle16    Time       Date       String     Real16 mat Complex16 m Short integer Real34        Complex34
/*  1 Long integer  */ {idivLonILonI, idivRe16LonI, idivError, idivError, idivError, idivError, idivError, idivError, idivError,  idivShoILonI, idivRe34LonI, idivError},
/*  2 Real16        */ {idivLonIRe16, idivRe16Re16, idivError, idivError, idivError, idivError, idivError, idivError, idivError,  idivShoIRe16, idivRe34Re16, idivError},
/*  3 Complex16     */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError,    idivError},
/*  4 Angle16       */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError,    idivError},
/*  5 Time          */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError,    idivError},
/*  6 Date          */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError,    idivError},
/*  7 String        */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError,    idivError},
/*  8 Real16 mat    */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError,    idivError},
/*  9 Complex16 mat */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError,    idivError},
/* 10 Short integer */ {idivLonIShoI, idivRe16ShoI, idivError, idivError, idivError, idivError, idivError, idivError, idivError,  idivShoIShoI, idivRe34ShoI, idivError},
/* 11 Real34        */ {idivLonIRe34, idivRe16Re34, idivError, idivError, idivError, idivError, idivError, idivError, idivError,  idivShoIRe34, idivRe34Re34, idivError},
/* 12 Complex34     */ {idivError,    idivError,    idivError, idivError, idivError, idivError, idivError, idivError, idivError,  idivError,    idivError,    idivError}
};



/********************************************//**
 * \brief Data type error in IDiv
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void idivError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot IDIV %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnIDiv:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regY idiv regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnIDiv(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

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
      showInfoDialog("In function idivLonILonI:", "cannot IDIV a long integer by 0", NULL, NULL);
    #endif
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
 * \brief Y(long integer) idiv X(real16) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivLonIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivLonIRe16:", "cannot use NaN as X input of IDIV", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivLonIRe16:", "cannot IDIV a long integer by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y, x;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &y);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);
  realIcDivide(&y, &x, &x);
  convertRealIcToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
}



/********************************************//**
 * \brief Y(real16) idiv X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivRe16LonI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe16LonI:", "cannot use NaN as Y input of IDIV", NULL, NULL);
    #endif
    return;
  }

  realIc_t x;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &x);
  if(realIcIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe16LonI:", "cannot IDIV a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  realIcDivide(&y, &x, &x);
  convertRealIcToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
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
      showInfoDialog("In function idivLonIShoI:", "cannot IDIV a long integer by 0", NULL, NULL);
    #endif
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
      showInfoDialog("In function idivShoILonI:", "cannot IDIV a short integer by 0", NULL, NULL);
    #endif
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
void idivLonIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivLonIRe34:", "cannot use NaN as X input of IDIV", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivLonIRe34:", "cannot IDIV a long integer by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y, x;

  convertLongIntegerRegisterToRealIc(REGISTER_Y, &y);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);
  realIcDivide(&y, &x, &x);
  convertRealIcToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
}



/********************************************//**
 * \brief Y(real34) idiv X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivRe34LonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe34LonI:", "cannot use NaN as Y input of IDIV", NULL, NULL);
    #endif
    return;
  }

  realIc_t x;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &x);
  if(realIcIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe34LonI:", "cannot IDIV a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  realIcDivide(&y, &x, &x);
  convertRealIcToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
}



/******************************************************************************************************************************************************************************************/
/* real16 idiv ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16) idiv X(real16) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivRe16Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe16Re16:", "cannot use NaN as X input of IDIV", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe16Re16:", "cannot IDIV a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y, x;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);
  realIcDivide(&y, &x, &x);
  convertRealIcToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
}



/********************************************//**
 * \brief Y(real16) idiv X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivRe16ShoI(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe16ShoI:", "cannot use NaN as Y input of IDIV", NULL, NULL);
    #endif
    return;
  }

  realIc_t x;

  convertShortIntegerRegisterToRealIc(REGISTER_X, &x);
  if(realIcIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe16ShoI:", "cannot IDIV a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  realIcDivide(&y, &x, &x);
  convertRealIcToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
}



/********************************************//**
 * \brief Y(short integer) idiv X(real16) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivShoIRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivShoIRe16:", "cannot use NaN as X input of IDIV", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivShoIRe16:", "cannot IDIV a short integer by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y, x;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &y);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);
  realIcDivide(&y, &x, &x);
  convertRealIcToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
}



/********************************************//**
 * \brief Y(real16) idiv X(real34) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivRe16Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe16Re34:", "cannot use NaN as X input of IDIV", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe16Re34:", "cannot IDIV a real16 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y, x;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);
  realIcDivide(&y, &x, &x);
  convertRealIcToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
}



/********************************************//**
 * \brief Y(real34) idiv X(real16) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivRe34Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe34Re16:", "cannot use NaN as X input of IDIV", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe34Re16:", "cannot IDIV a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y, x;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);
  realIcDivide(&y, &x, &x);
  convertRealIcToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
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
      showInfoDialog("In function idivShoIShoI:", "cannot IDIV a short integer by 0", NULL, NULL);
    #endif
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
void idivShoIRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivShoIRe34:", "cannot use NaN as X input of IDIV", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivShoIRe34:", "cannot IDIV a short integer by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y, x;

  convertShortIntegerRegisterToRealIc(REGISTER_Y, &y);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);
  realIcDivide(&y, &x, &x);
  convertRealIcToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
}



/********************************************//**
 * \brief Y(real34) idiv X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void idivRe34ShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe34ShoI:", "cannot use NaN as Y input of IDIV", NULL, NULL);
    #endif
    return;
  }

  realIc_t x;

  convertShortIntegerRegisterToRealIc(REGISTER_X, &x);
  if(realIcIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe34ShoI:", "cannot IDIV a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  realIcDivide(&y, &x, &x);
  convertRealIcToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
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
void idivRe34Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe34Re34:", "cannot use NaN as X input of IDIV", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function idivRe34Re34:", "cannot IDIV a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  realIc_t y, x;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);
  realIcDivide(&y, &x, &x);
  convertRealIcToLongIntegerRegister(&x, REGISTER_X, DEC_ROUND_DOWN);
}
