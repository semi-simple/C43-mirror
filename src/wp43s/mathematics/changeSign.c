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
// regX ==> 1            2        3         4         5          6         7         8          9           10            11       12
//          Long integer real16   Complex16 Angle16   Time       Date      String    Real16 mat Complex16 m Short integer Real34   Complex34
            chsLonI,     chsRe16, chsCo16,  chsError, chsError,  chsError, chsError, chsRm16,   chsCm16,    chsShoI,      chsRe34, chsCo34
};



/********************************************//**
 * \brief Data type error in +/-
 *
 * \param void
 * \return void
 ***********************************************/
void chsError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot change the sign of %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnChangeSign:", errorMessage, NULL, NULL);
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
  saveStack();
  //copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  chs[getRegisterDataType(REGISTER_X)]();

  if(lastErrorCode == 0) {
    refreshRegisterLine(REGISTER_X);
  }
  else {
    restoreStack();
    refreshStack();
  }
}



void chsLonI(void) {
  switch(getRegisterLongIntegerSign(REGISTER_X)) {
    case LONG_INTEGER_POSITIVE : setRegisterLongIntegerSign(REGISTER_X, LONG_INTEGER_NEGATIVE); break;
    case LONG_INTEGER_NEGATIVE : setRegisterLongIntegerSign(REGISTER_X, LONG_INTEGER_POSITIVE); break;
    default : {}
  }
}



void chsRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function chsRe16:", "cannot use NaN as X input of +/-", NULL, NULL);
    #endif
    return;
  }

  if(!getFlag(FLAG_DANGER) && real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(real16IsPositive(REGISTER_REAL16_DATA(REGISTER_X)) ? ERROR_OVERFLOW_MINUS_INF : ERROR_OVERFLOW_PLUS_INF , ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function chsRe16:", "cannot change infinity sign while D flag is clear", NULL, NULL);
    #endif
    return;
  }

  real16ChangeSign(REGISTER_REAL16_DATA(REGISTER_X));

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    real16SetPositiveSign(REGISTER_REAL16_DATA(REGISTER_X));
  }
}



void chsCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function chsCo16:", "cannot use NaN as X input of +/-", NULL, NULL);
    #endif
    return;
  }

  if(!getFlag(FLAG_DANGER)) {
    if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
      displayCalcErrorMessage(real16IsPositive(REGISTER_REAL16_DATA(REGISTER_X)) ? ERROR_OVERFLOW_MINUS_INF : ERROR_OVERFLOW_PLUS_INF , ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function chsCo16:", "cannot change infinity sign of real part while D flag is clear", NULL, NULL);
      #endif
      return;
    }

    if(real16IsInfinite(REGISTER_IMAG16_DATA(REGISTER_X))) {
      displayCalcErrorMessage(real16IsPositive(REGISTER_IMAG16_DATA(REGISTER_X)) ? ERROR_OVERFLOW_MINUS_INF : ERROR_OVERFLOW_PLUS_INF , ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function chsCo16:", "cannot change infinity sign of imaginary part while D flag is clear", NULL, NULL);
      #endif
      return;
    }
  }

  complex16ChangeSign(REGISTER_COMPLEX16_DATA(REGISTER_X));

  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    real16SetPositiveSign(REGISTER_REAL16_DATA(REGISTER_X));
  }

  if(real16IsZero(REGISTER_IMAG16_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    real16SetPositiveSign(REGISTER_IMAG16_DATA(REGISTER_X));
  }
}



void chsRm16(void) {
  fnToBeCoded();
}



void chsCm16(void) {
  fnToBeCoded();
}



void chsShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intChs(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void chsRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function chsRe34:", "cannot use NaN as X input of +/-", NULL, NULL);
    #endif
    return;
  }

  if(!getFlag(FLAG_DANGER) && real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X)) ? ERROR_OVERFLOW_MINUS_INF : ERROR_OVERFLOW_PLUS_INF , ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function chsRe34:", "cannot change infinity sign while D flag is clear", NULL, NULL);
    #endif
    return;
  }

  real34ChangeSign(REGISTER_REAL34_DATA(REGISTER_X));

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    real34SetPositiveSign(REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void chsCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function chsCo34:", "cannot use NaN as X input of +/-", NULL, NULL);
    #endif
    return;
  }

  if(!getFlag(FLAG_DANGER)) {
    if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
      displayCalcErrorMessage(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X)) ? ERROR_OVERFLOW_MINUS_INF : ERROR_OVERFLOW_PLUS_INF , ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function chsCo34:", "cannot change infinity sign of real part while D flag is clear", NULL, NULL);
      #endif
      return;
    }

    if(real34IsInfinite(REGISTER_IMAG34_DATA(REGISTER_X))) {
      displayCalcErrorMessage(real34IsPositive(REGISTER_IMAG34_DATA(REGISTER_X)) ? ERROR_OVERFLOW_MINUS_INF : ERROR_OVERFLOW_PLUS_INF , ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function chsCo34:", "cannot change infinity sign of imaginary part while D flag is clear", NULL, NULL);
      #endif
      return;
    }
  }

  complex34ChangeSign(REGISTER_COMPLEX34_DATA(REGISTER_X));

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    real34SetPositiveSign(REGISTER_REAL34_DATA(REGISTER_X));
  }

  if(real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    real34SetPositiveSign(REGISTER_IMAG34_DATA(REGISTER_X));
  }
}
