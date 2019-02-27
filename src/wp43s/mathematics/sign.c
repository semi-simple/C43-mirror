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
 * \file dign.c
 ***********************************************/

#include "wp43s.h"



void (* const sign[12])(void) = {
//  regX ==> 1            2         3                4          5          6          7          8           9            10             11        12
//           Big integer  real16    complex16        Date       Time       Date       String     real16 mat  complex16 m  Small integer  real34    complex34
             signBigI,    signRe16, unitVectorCo16,  signError, signError, signError, signError, signRm16,   signError,   signSmaI,      signRe34, unitVectorCo34
};



/********************************************//**
 * \brief Data type error in sign
 *
 * \param void
 * \return void
 ***********************************************/
void signError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate the sign of %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnSign:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void signToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "sign(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
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
  if(sign[getRegisterDataType(REGISTER_X)] != signError) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    sign[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(opX);

    if(lastErrorCode != 0) {
      restoreStack();
      refreshStack();
    }
    else {
      refreshRegisterLine(REGISTER_X);
    }
  }
  else {
    signError();
  }
}



void signBigI(void) {
  bigInteger_t temp;

  convertBigIntegerRegisterToBigInteger(opX, &temp);

  if(bigIntegerIsZero(&temp)) {
    uIntToBigInteger(0, &temp);
  }
  else if(bigIntegerCompareUInt(&temp, 0) < 0) {
    uIntToBigInteger(1, &temp);
    bigIntegerSetNegativeSign(&temp);
  }
  else {
    uIntToBigInteger(1, &temp);
  }

  convertBigIntegerToBigIntegerRegister(&temp, result);
}



void signRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function signRe16:", "cannot use NaN as an input of sign", NULL, NULL);
    #endif
    return;
  }

  bigInteger_t temp;

  if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    uIntToBigInteger(0, &temp);
  }
  else if(real16IsNegative(REGISTER_REAL16_DATA(opX))) {
    uIntToBigInteger(1, &temp);
    bigIntegerSetNegativeSign(&temp);
  }
  else {
    uIntToBigInteger(1, &temp);
  }

  convertBigIntegerToBigIntegerRegister(&temp, result);
}



void signRm16(void) {
  signToBeCoded();
}



void signSmaI(void) {
  bigInteger_t temp;
  int64_t sign = WP34S_intSign(*(REGISTER_SMALL_INTEGER_DATA(opX)));

  if(sign == 0) {
    uIntToBigInteger(0, &temp);
  }
  else if(sign == -1) {
    uIntToBigInteger(1, &temp);
    bigIntegerSetNegativeSign(&temp);
  }
  else {
    uIntToBigInteger(1, &temp);
  }

  convertBigIntegerToBigIntegerRegister(&temp, result);
}



void signRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function signRe34:", "cannot use NaN as an input of sign", NULL, NULL);
    #endif
    return;
  }

  bigInteger_t temp;

  if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    uIntToBigInteger(0, &temp);
  }
  else if(real34IsNegative(REGISTER_REAL34_DATA(opX))) {
    uIntToBigInteger(1, &temp);
    bigIntegerSetNegativeSign(&temp);
  }
  else {
    uIntToBigInteger(1, &temp);
  }

  convertBigIntegerToBigIntegerRegister(&temp, result);
}
