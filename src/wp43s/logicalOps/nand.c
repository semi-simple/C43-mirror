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
 * \file nand.c
 ***********************************************/

#include "wp43s.h"






void (* const logicalNand[9][9])(void) = {
// regX |    regY ==>   1             2             3            4            5            6            7            8             9
//      V               Long integer  Real34        Complex34    Time         Date         String       Real34 mat   Complex34 m   Short integer
/*  1 Long integer  */ {nandLonILonI, nandRealLonI, nandError24, nandError24, nandError24, nandError24, nandError24, nandError24,  nandError31 },
/*  2 Real34        */ {nandLonIReal, nandRealReal, nandError24, nandError24, nandError24, nandError24, nandError24, nandError24,  nandError31 },
/*  3 Complex34     */ {nandError24,  nandError24,  nandError24, nandError24, nandError24, nandError24, nandError24, nandError24,  nandError24 },
/*  4 Time          */ {nandError24,  nandError24,  nandError24, nandError24, nandError24, nandError24, nandError24, nandError24,  nandError24 },
/*  5 Date          */ {nandError24,  nandError24,  nandError24, nandError24, nandError24, nandError24, nandError24, nandError24,  nandError24 },
/*  6 String        */ {nandError24,  nandError24,  nandError24, nandError24, nandError24, nandError24, nandError24, nandError24,  nandError24 },
/*  7 Real34 mat    */ {nandError24,  nandError24,  nandError24, nandError24, nandError24, nandError24, nandError24, nandError24,  nandError24 },
/*  8 Complex34 mat */ {nandError24,  nandError24,  nandError24, nandError24, nandError24, nandError24, nandError24, nandError24,  nandError24 },
/*  9 Short integer */ {nandError31,  nandError31,  nandError24, nandError24, nandError24, nandError24, nandError24, nandError24,  nandShoIShoI}
};



/********************************************//**
 * \brief Data type error in NAND
 *
 * \param void
 * \return void
 ***********************************************/
void nandError24(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "%s NAND %s", getRegisterDataTypeName(REGISTER_Y, false, false), getRegisterDataTypeName(REGISTER_X, false, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "data type of one of the NAND parameters is not allowed");
    showInfoDialog("In function nandError24:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}

void nandError31(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "%s NAND %s", getRegisterDataTypeName(REGISTER_Y, false, false), getRegisterDataTypeName(REGISTER_X, false, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "NAND doesn't allow mixing data types real/long integer and short integer");
    showInfoDialog("In function nandError31:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL NAND regY ÷ regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnLogicalNand(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  logicalNand[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  if(lastErrorCode != 0) {
    restoreStack();
  }
  else {
    fnDropY(NOPARAM);
  }

  refreshStack();
}



void nandLonILonI(void) {
  longInteger_t x, res;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);
  convertLongIntegerRegisterToLongInteger(REGISTER_Y, res);

  if(longIntegerIsZero(x) || longIntegerIsZero(res)) {
    uIntToLongInteger(1, res);
  }
  else {
    uIntToLongInteger(0, res);
  }

  convertLongIntegerToLongIntegerRegister(res, REGISTER_X);

  longIntegerFree(x);
  longIntegerFree(res);
}



void nandLonIReal(void) {
  longInteger_t res;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, res);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) || longIntegerIsZero(res)) {
    uIntToLongInteger(1, res);
  }
  else {
    uIntToLongInteger(0, res);
  }

  convertLongIntegerToLongIntegerRegister(res, REGISTER_X);

  longIntegerFree(res);
}



void nandRealLonI(void) {
  longInteger_t res;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, res);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) || longIntegerIsZero(res)) {
    uIntToLongInteger(1, res);
  }
  else {
    uIntToLongInteger(0, res);
  }

  convertLongIntegerToLongIntegerRegister(res, REGISTER_X);

  longIntegerFree(res);
}



void nandRealReal(void) {
  longInteger_t res;

  longIntegerInit(res);
  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y))) {
    uIntToLongInteger(1, res);
  }
  else {
    uIntToLongInteger(0, res);
  }

  convertLongIntegerToLongIntegerRegister(res, REGISTER_X);

  longIntegerFree(res);
}



void nandShoIShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = ~(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & *(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y))) & shortIntegerMask;
  setRegisterShortIntegerBase(REGISTER_X, getRegisterShortIntegerBase(REGISTER_Y));
}