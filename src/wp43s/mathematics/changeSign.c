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
 * \file changeSign.c
 ***********************************************/

#include "wp43s.h"



void (* const chs[12])(void) = {
// regX ==> 1            2         3          4         5          6          7          8            9             10              11        12
//          Big integer  real16    complex16  angle     Time       Date       String     real16 mat   complex16 m   Small integer   real34    complex34
            chsBigI,     chsRe16,  chsCo16,   chsAngl,  chsError,  chsError,  chsError,  chsRm16,     chsCm16,      chsError,       chsRe34,  chsCo34
};



/********************************************//**
 * \brief Data type error in +/-
 *
 * \param void
 * \return void
 ***********************************************/
void chsError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot change sign %s", getRegisterDataTypeName(REGISTER_X, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "from %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    showInfoDialog("In function fnChangeSign:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void chsToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "change sign %s", getRegisterDataTypeName(REGISTER_X, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    showInfoDialog("Operation to be coded:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief rexX ==> regL and -regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnChangeSign(uint16_t unusedParamButMandatory) {
  if(chs[getRegisterDataType(REGISTER_X)] != chsError) {
    saveStack();
    //copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    chs[getRegisterDataType(REGISTER_X)]();

    if(lastErrorCode != 0) {
      restoreStack();
      refreshStack();
    }
    else {
      refreshRegisterLine(REGISTER_X);
    }
  }
  else {
    chsError();
  }
}



void chsBigI(void) {
  setRegisterSign(result, getRegisterDataInfo(result) ^ 1);
}



void chsRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function chsRe16:", "cannot use NaN as an input of +/-", NULL, NULL);
    #endif
    return;
  }

  real16ChangeSign(REGISTER_REAL16_DATA(result));
  if(real16IsZero(REGISTER_REAL16_DATA(result))) {
    real16SetPositiveSign(REGISTER_REAL16_DATA(result));
  }
}



void chsCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function chsCo16:", "cannot use NaN as an input of +/-", NULL, NULL);
    #endif
    return;
  }

  real16ChangeSign(REGISTER_REAL16_DATA(result));
  if(real16IsZero(REGISTER_REAL16_DATA(result))) {
    real16SetPositiveSign(REGISTER_REAL16_DATA(result));
  }

  real16ChangeSign(REGISTER_IMAG16_DATA(result));
  if(real16IsZero(REGISTER_IMAG16_DATA(result))) {
    real16SetPositiveSign(REGISTER_IMAG16_DATA(result));
  }
}



void chsAngl(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function chsAngl:", "cannot use NaN as an input of +/-", NULL, NULL);
    #endif
    return;
  }

  #if (ANGLE16 == 1)
    real16ChangeSign(REGISTER_REAL16_DATA(result));
    if(real16IsZero(REGISTER_REAL16_DATA(result))) {
      real16SetPositiveSign(REGISTER_REAL16_DATA(result));
    }
  #endif

  #if (ANGLE34 == 1)
    real34ChangeSign(REGISTER_REAL34_DATA(result));
    if(real34IsZero(REGISTER_REAL34_DATA(result))) {
      real34SetPositiveSign(REGISTER_REAL34_DATA(result));
    }
  #endif
}



void chsRm16(void) {
  chsToBeCoded();
}



void chsCm16(void) {
  chsToBeCoded();
}



void chsSmaI(void) {
  *(REGISTER_SMALL_INTEGER_DATA(result)) = WP34S_intChs(*(REGISTER_SMALL_INTEGER_DATA(result)));
}



void chsRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function chsRe34:", "cannot use NaN as an input of +/-", NULL, NULL);
    #endif
    return;
  }

  real34ChangeSign(REGISTER_REAL34_DATA(result));
  if(real34IsZero(REGISTER_REAL34_DATA(result))) {
    real34SetPositiveSign(REGISTER_REAL34_DATA(result));
  }
}



void chsCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function chsCo34:", "cannot use NaN as an input of +/-", NULL, NULL);
    #endif
    return;
  }

  real34ChangeSign(REGISTER_REAL34_DATA(result));
  if(real34IsZero(REGISTER_REAL34_DATA(result))) {
    real34SetPositiveSign(REGISTER_REAL34_DATA(result));
  }
  real34ChangeSign(REGISTER_IMAG34_DATA(result));

  if(real34IsZero(REGISTER_IMAG34_DATA(result))) {
    real34SetPositiveSign(REGISTER_IMAG34_DATA(result));
  }
}
