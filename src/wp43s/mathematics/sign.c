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
 * \file sign.c
 ***********************************************/

#include "wp43s.h"



void (* const sign[13])(void) = {
// regX ==> 1            2         3               4         5          6          7          8          9           10            11        12              13
//          Long integer Real16    Complex16       Angle16   Time       Date       String     Real16 mat Complex16 m Short integer Real34    Complex34       Angle34
            signLonI,    signRe16, unitVectorCo16, signRe16, signError, signError, signError, signRm16,  signError,  signShoI,     signRe34, unitVectorCo34, signRe34
};



/********************************************//**
 * \brief Data type error in sign
 *
 * \param void
 * \return void
 ***********************************************/
void signError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate the sign of %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnSign:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and sign(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSign(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  sign[getRegisterDataType(REGISTER_X)]();

  adjustResult(result, false, true, opX, -1, -1);
}



void signLonI(void) {
  longInteger_t lgInt;

  longIntegerInit(lgInt);

  switch(getRegisterLongIntegerSign(opX)) {
    case LONG_INTEGER_POSITIVE:
      intToLongInteger(1, lgInt);
      break;

    case LONG_INTEGER_NEGATIVE:
      intToLongInteger(-1, lgInt);
      break;

    default: {}
  }

  convertLongIntegerToLongIntegerRegister(lgInt, result);
  longIntegerFree(lgInt);
}



void signRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function signRe16:", "cannot use NaN as an input of sign", NULL, NULL);
    #endif
    return;
  }

  longInteger_t lgInt;

  longIntegerInit(lgInt);

  if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    uIntToLongInteger(0, lgInt);
  }
  else if(real16IsNegative(REGISTER_REAL16_DATA(opX))) {
    uIntToLongInteger(1, lgInt);
    longIntegerSetNegativeSign(lgInt);
  }
  else {
    uIntToLongInteger(1, lgInt);
  }

  convertLongIntegerToLongIntegerRegister(lgInt, result);
  longIntegerFree(lgInt);
}



void signRm16(void) {
  fnToBeCoded();
}



void signShoI(void) {
  longInteger_t lgInt;

  longIntegerInit(lgInt);

  switch(WP34S_intSign(*(REGISTER_SHORT_INTEGER_DATA(opX)))) {
    case -1 :
      uIntToLongInteger(1, lgInt);
      longIntegerSetNegativeSign(lgInt);
      break;

    case 0 :
      uIntToLongInteger(0, lgInt);
      break;

    case 1 :
      uIntToLongInteger(1, lgInt);
      break;

    default :
      uIntToLongInteger(0, lgInt);
      sprintf(errorMessage, "In function signShoI: %" FMT64U " is an unexpected value returned by WP34S_intSign!", WP34S_intSign(*(REGISTER_SHORT_INTEGER_DATA(opX))));
      displayBugScreen(errorMessage);
  }

  convertLongIntegerToLongIntegerRegister(lgInt, result);

  longIntegerFree(lgInt);
}



void signRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function signRe34:", "cannot use NaN as an input of sign", NULL, NULL);
    #endif
    return;
  }

  longInteger_t lgInt;
  longIntegerInit(lgInt);

  if(!real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(real34IsNegative(REGISTER_REAL34_DATA(opX))) {
      intToLongInteger(-1, lgInt);
    }
    else {
      intToLongInteger(1, lgInt);
    }
  }

  convertLongIntegerToLongIntegerRegister(lgInt, result);
  longIntegerFree(lgInt);
}
