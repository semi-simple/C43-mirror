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
 * \file shiftDigits.c
 ***********************************************/

#include "wp43s.h"


/********************************************//**
 * \brief regX ==> regL and SDL(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSdl(uint16_t numberOfShifts) {
  int32_t i;
  
  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    longInteger_t base_longInt;
    longIntegerInit(base_longInt);
    intToLongInteger(10, base_longInt);
    
    longInteger_t result;
    longIntegerInit(result);
    
    longInteger_t remainder;
    longIntegerInit(remainder);
    
    convertLongIntegerRegisterToLongInteger(REGISTER_X, result);
    
    int abort = 0;
    
    for(i=1; i<=numberOfShifts; i++) {
      longIntegerDivideRemainder(result, base_longInt, remainder);
      longIntegerToInt(remainder, abort);
      if (abort != 0) {
        convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
        goto FNSDL_CHG_DATATYPE;
      }
      longIntegerDivide(result, base_longInt, result);
    }
    
    convertLongIntegerToLongIntegerRegister(result, REGISTER_X);
    
    refreshRegisterLine(REGISTER_X);
  }
     
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    
    FNSDL_CHG_DATATYPE: ;
    
    real34_t base_real_34;
    int32ToReal34((int32_t) 10, &base_real_34);
    
    for(i=1; i<=numberOfShifts; i++) {
      real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &base_real_34, REGISTER_REAL34_DATA(REGISTER_X));
    }

    refreshRegisterLine(REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot SDL %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnSdl:", errorMessage, NULL, NULL);
    #endif
  }
}



/********************************************//**
 * \brief regX ==> regL and SDR(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSdr(uint16_t numberOfShifts) {
  int32_t i;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    longInteger_t base_longInt;
    longIntegerInit(base_longInt);
    intToLongInteger(10, base_longInt);
    
    longInteger_t result;
    longIntegerInit(result);
    
    convertLongIntegerRegisterToLongInteger(REGISTER_X, result);
    
    for(i=1; i<=numberOfShifts; i++) {
      longIntegerMultiply(result, base_longInt, result);
    }
    
    convertLongIntegerToLongIntegerRegister(result, REGISTER_X);
    
    refreshRegisterLine(REGISTER_X);
  }
  
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    real34_t base_real_34;
    int32ToReal34((int32_t) 10, &base_real_34);
    
    for(i=1; i<=numberOfShifts; i++) {
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), &base_real_34, REGISTER_REAL34_DATA(REGISTER_X));
    }
    refreshRegisterLine(REGISTER_X);
  }
  
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot SDR %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnSdr:", errorMessage, NULL, NULL);
    #endif
  }
}



