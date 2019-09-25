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
 * \file lcm.c
 ***********************************************/

#include "wp43s.h"



void (* const lcm[12][12])(void) = {
// regX |    regY ==>   1            2         3         4         5         6         7         8          9           10            11        12
//      V               Long integer Real16    Complex16 Angle16   Time      Date      String    Real16 mat Complex16 m Short integer Real34    Complex34
/*  1 Long integer  */ {lcmLonILonI, lcmError, lcmError, lcmError, lcmError, lcmError, lcmError, lcmError,  lcmError,   lcmShoILonI,  lcmError, lcmError},
/*  2 Real16        */ {lcmError,    lcmError, lcmError, lcmError, lcmError, lcmError, lcmError, lcmError,  lcmError,   lcmError,     lcmError, lcmError},
/*  3 Complex16     */ {lcmError,    lcmError, lcmError, lcmError, lcmError, lcmError, lcmError, lcmError,  lcmError,   lcmError,     lcmError, lcmError},
/*  4 Angle16       */ {lcmError,    lcmError, lcmError, lcmError, lcmError, lcmError, lcmError, lcmError,  lcmError,   lcmError,     lcmError, lcmError},
/*  5 Time          */ {lcmError,    lcmError, lcmError, lcmError, lcmError, lcmError, lcmError, lcmError,  lcmError,   lcmError,     lcmError, lcmError},
/*  6 Date          */ {lcmError,    lcmError, lcmError, lcmError, lcmError, lcmError, lcmError, lcmError,  lcmError,   lcmError,     lcmError, lcmError},
/*  7 String        */ {lcmError,    lcmError, lcmError, lcmError, lcmError, lcmError, lcmError, lcmError,  lcmError,   lcmError,     lcmError, lcmError},
/*  8 Real16 mat    */ {lcmError,    lcmError, lcmError, lcmError, lcmError, lcmError, lcmError, lcmError,  lcmError,   lcmError,     lcmError, lcmError},
/*  9 Complex16 mat */ {lcmError,    lcmError, lcmError, lcmError, lcmError, lcmError, lcmError, lcmError,  lcmError,   lcmError,     lcmError, lcmError},
/* 10 Short integer */ {lcmLonIShoI, lcmError, lcmError, lcmError, lcmError, lcmError, lcmError, lcmError,  lcmError,   lcmShoIShoI,  lcmError, lcmError},
/* 11 Real34        */ {lcmError,    lcmError, lcmError, lcmError, lcmError, lcmError, lcmError, lcmError,  lcmError,   lcmError,     lcmError, lcmError},
/* 12 Complex34     */ {lcmError,    lcmError, lcmError, lcmError, lcmError, lcmError, lcmError, lcmError,  lcmError,   lcmError,     lcmError, lcmError}
};



/********************************************//**
 * \brief Data type error in lcm
 *
 * \param void
 * \return void
 ***********************************************/
void lcmError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate lcm (%s, %s)", getRegisterDataTypeName(REGISTER_Y, true, false), getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnLcm:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and LCM(regY, regX) ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLcm(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  lcm[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  if(lastErrorCode == 0) {
    fnDropY(NOPARAM);
  }
  else {
    restoreStack();
  }

  refreshStack();
}



void lcmLonILonI(void) {
  longInteger_t liX, liY;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, liY);
  longIntegerSetPositiveSign(liY);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, liX);
  longIntegerSetPositiveSign(liX);

  longIntegerLcm(liY, liX, liX);

  convertLongIntegerToLongIntegerRegister(liX, REGISTER_X);

  longIntegerFree(liX);
  longIntegerFree(liY);
}



void lcmLonIShoI(void) {
  longInteger_t liX, liY;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, liY);
  longIntegerSetPositiveSign(liY);
  convertShortIntegerRegisterLongIntegerRegister(REGISTER_X, REGISTER_X);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, liX);
  longIntegerSetPositiveSign(liX);

  longIntegerLcm(liY, liX, liX);

  convertLongIntegerToLongIntegerRegister(liX, REGISTER_X);

  longIntegerFree(liX);
  longIntegerFree(liY);
}



void lcmShoILonI(void) {
  longInteger_t liX, liY;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, liX);
  longIntegerSetPositiveSign(liX);
  convertShortIntegerRegisterLongIntegerRegister(REGISTER_Y, REGISTER_Y);
  convertLongIntegerRegisterToLongInteger(REGISTER_Y, liY);
  longIntegerSetPositiveSign(liY);

  longIntegerLcm(liY, liX, liX);

  convertLongIntegerToLongIntegerRegister(liX, REGISTER_X);

  longIntegerFree(liX);
  longIntegerFree(liY);
}



void lcmShoIShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intLCM(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}
