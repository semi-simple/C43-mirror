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
 * \file setClearFlipBits.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief regX ==> regL and CB(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCb(uint16_t bit) { // bit from 1=LSB to shortIntegerWordSize=MSB
  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) &= ~((uint64_t)1 << (bit - 1));

    refreshStack();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot CB %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnCb:", errorMessage, NULL, NULL);
    #endif
  }
}



/********************************************//**
 * \brief regX ==> regL and SB(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSb(uint16_t bit) { // bit from 1=LSB to shortIntegerWordSize=MSB
  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) |= (uint64_t)1 << (bit - 1);

    refreshStack();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot SB %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnSb:", errorMessage, NULL, NULL);
    #endif
  }
}



/********************************************//**
 * \brief regX ==> regL and FB(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFb(uint16_t bit) { // bit from 1=LSB to shortIntegerWordSize=MSB
  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) ^= (uint64_t)1 << (bit - 1);

    refreshStack();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot FB %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnFb:", errorMessage, NULL, NULL);
    #endif
  }
}



/********************************************//**
 * \brief BC(regX)
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnBc(uint16_t bit) { // bit from 1=LSB to shortIntegerWordSize=MSB
  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    if(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & ((uint64_t)1 << (bit - 1))) {
      temporaryInformation = TI_FALSE;
    }
    else {
      temporaryInformation = TI_TRUE;
    }

    refreshRegisterLine(REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot BC %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnBc:", errorMessage, NULL, NULL);
    #endif
  }
}



/********************************************//**
 * \brief BS(regX)
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnBs(uint16_t bit) { // bit from 1=LSB to shortIntegerWordSize=MSB
  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    if(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & ((uint64_t)1 << (bit - 1))) {
      temporaryInformation = TI_TRUE;
    }
    else {
      temporaryInformation = TI_FALSE;
    }

    refreshRegisterLine(REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot BS %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnBs:", errorMessage, NULL, NULL);
    #endif
  }
}