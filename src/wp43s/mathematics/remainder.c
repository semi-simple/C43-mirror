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



void (* const rmd[9][9])(void) = {
// regX |    regY ==>   1            2            3         4         5         6         7          8           9
//      V               Long integer Real34       Complex34 Time      Date      String    Real34 mat Complex34 m Short integer
/*  1 Long integer  */ {rmdLonILonI, rmdRealLonI, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdShoILonI},
/*  2 Real34        */ {rmdLonIReal, rmdRealReal, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdShoIReal},
/*  3 Complex34     */ {rmdError,    rmdError,    rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdError   },
/*  4 Time          */ {rmdError,    rmdError,    rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdError   },
/*  5 Date          */ {rmdError,    rmdError,    rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdError   },
/*  6 String        */ {rmdError,    rmdError,    rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdError   },
/*  7 Real34 mat    */ {rmdError,    rmdError,    rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdError   },
/*  8 Complex34 mat */ {rmdError,    rmdError,    rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdError   },
/*  9 Short integer */ {rmdLonIShoI, rmdRealShoI, rmdError, rmdError, rmdError, rmdError, rmdError,  rmdError,   rmdShoIShoI}
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
void rmdLonIReal(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdLonIReal:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdLonIReal:", "cannot IDIVR a long integer by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realDivideRemainder(&y, &x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) rmd X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdRealLonI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRealLonI:", "cannot use NaN as Y input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRealLonI:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  realDivideRemainder(&y, &x, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
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
void rmdShoIReal(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdShoIReal:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdShoIReal:", "cannot IDIVR a short integer by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realDivideRemainder(&y, &x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) rmd X(short integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void rmdRealShoI(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRealShoI:", "cannot use NaN as Y input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realIsZero(&x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRealShoI:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t y;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);

  realDivideRemainder(&y, &x, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
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
void rmdRealReal(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRealReal:", "cannot use NaN as X input of IDIVR", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function rmdRealReal:", "cannot IDIVR a real34 by 0", NULL, NULL);
    #endif
    return;
  }

  real39_t x, y;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realDivideRemainder(&y, &x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}
