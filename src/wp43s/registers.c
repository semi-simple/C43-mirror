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
 * \file registers.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief returns the data type of a register
 *
 * \param[in] r calcRegister_t Register number
 * \return uint32_t      Data type
 ***********************************************/
uint32_t getRegisterDataType(calcRegister_t regist) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    return ((reg[regist] >> OFFSET_REGISTER_DATA_TYPE) & ((1u << LENGTH_REGISTER_DATA_TYPE) - 1u));
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < numberOfLocalRegisters) {
        return (*POINTER_TO_LOCAL_REGISTER(regist) >> OFFSET_REGISTER_DATA_TYPE) & ((1u << LENGTH_REGISTER_DATA_TYPE) - 1u);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfLocalRegisters-1);
        showInfoDialog("In function getRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function getRegisterDataType:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }
  else if(regist < FIRST_TEMPORARY_REGISTER) { // Named register
    if(numberOfNamedRegisters > 0) {
      regist -= 1000;
      if(regist < numberOfNamedRegisters) {
        return (*POINTER_TO_NAMED_REGISTER(regist) >> OFFSET_REGISTER_DATA_TYPE) & ((1u << LENGTH_REGISTER_DATA_TYPE) - 1u);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedRegisters - 1);
        showInfoDialog("In function getRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function getRegisterDataType: no named registers defined!");
    }
  }
  else if(regist < SAVED_REGISTER_X) { // Temporary register
    if(regist < FIRST_TEMPORARY_REGISTER+NUMBER_OF_TEMPORARY_REGISTERS) {
      return (tempRegister[regist - FIRST_TEMPORARY_REGISTER] >> OFFSET_REGISTER_DATA_TYPE) & ((1u << LENGTH_REGISTER_DATA_TYPE) - 1u);
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "temporary register %" FMT16S, regist - FIRST_TEMPORARY_REGISTER);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16S, FIRST_TEMPORARY_REGISTER + NUMBER_OF_TEMPORARY_REGISTERS-1);
      showInfoDialog("In function getRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else if(regist < 4000) { // Saved stack register
    if(regist <= SAVED_REGISTER_L) {
      return (savedStackRegister[regist - SAVED_REGISTER_X] >> OFFSET_REGISTER_DATA_TYPE) & ((1u << LENGTH_REGISTER_DATA_TYPE) - 1u);
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "saved stack register %" FMT16S, regist - SAVED_REGISTER_X);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to 7");
      showInfoDialog("In function getRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else {
    sprintf(errorMessage, "In function getRegisterDataType: register=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
  return 31u;
}



/********************************************//**
 * \brief returns the data pointer of a register
 *
 * \param[in] r calcRegister_t Register number
 * \return uint32_t      Data pointer
 ***********************************************/
uint32_t getRegisterDataPointer(calcRegister_t regist) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    return ((reg[regist] >> OFFSET_REGISTER_POINTER) & ((1u << LENGTH_REGISTER_POINTER) - 1u));
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < numberOfLocalRegisters) {
        return (*POINTER_TO_LOCAL_REGISTER(regist) >> OFFSET_REGISTER_POINTER) & ((1u << LENGTH_REGISTER_POINTER) - 1u);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfLocalRegisters-1);
        showInfoDialog("In function getRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function getRegisterDataPointer:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }
  else if(regist < FIRST_TEMPORARY_REGISTER) { // Named register
    if(numberOfNamedRegisters > 0) {
      regist -= 1000;
      if(regist < numberOfNamedRegisters) {
        return (*POINTER_TO_NAMED_REGISTER(regist) >> OFFSET_REGISTER_POINTER) & ((1u << LENGTH_REGISTER_POINTER) - 1u);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedRegisters - 1);
        showInfoDialog("In function getRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function getRegisterDataPointer: no named registers defined!");
    }
  }
  else if(regist < SAVED_REGISTER_X) { // Temporary register
    if(regist < FIRST_TEMPORARY_REGISTER+NUMBER_OF_TEMPORARY_REGISTERS) {
      return (tempRegister[regist - FIRST_TEMPORARY_REGISTER] >> OFFSET_REGISTER_POINTER) & ((1u << LENGTH_REGISTER_POINTER) - 1u);
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "temporary register %" FMT16S, regist - FIRST_TEMPORARY_REGISTER);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16S, FIRST_TEMPORARY_REGISTER + NUMBER_OF_TEMPORARY_REGISTERS - 1);
      showInfoDialog("In function getRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else if(regist < 4000) { // Saved stack register
    if(regist <= SAVED_REGISTER_L) {
      return (savedStackRegister[regist - SAVED_REGISTER_X] >> OFFSET_REGISTER_POINTER) & ((1u << LENGTH_REGISTER_POINTER) - 1u);
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "saved stack register %" FMT16S, regist - SAVED_REGISTER_X);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to 7");
      showInfoDialog("In function getRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else {
    sprintf(errorMessage, "In function getRegisterDataPointer: r=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
  return 0;
}



/********************************************//**
 * \brief returns the data information of a register:
 *        Angular mode or base
 *
 * \param[in] r calcRegister_t Register number
 * \return uint32_t      Angular mode
 ***********************************************/
uint32_t getRegisterDataInfo(calcRegister_t regist) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    return ((reg[regist] >> OFFSET_REGISTER_INFORMATION) & ((1u << LENGTH_REGISTER_INFORMATION) - 1u));
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < numberOfLocalRegisters) {
        return (*POINTER_TO_LOCAL_REGISTER(regist) >> OFFSET_REGISTER_INFORMATION) & ((1u << LENGTH_REGISTER_INFORMATION) - 1u);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfLocalRegisters - 1);
        showInfoDialog("In function getRegisterDataInfo:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
     showInfoDialog("In function getRegisterDataInfo:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }
  else if(regist < FIRST_TEMPORARY_REGISTER) { // Named register
    if(numberOfNamedRegisters > 0) {
      regist -= 1000;
      if(regist < numberOfNamedRegisters) {
        return (*POINTER_TO_NAMED_REGISTER(regist) >> OFFSET_REGISTER_INFORMATION) & ((1u << LENGTH_REGISTER_INFORMATION) - 1u);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedRegisters - 1);
        showInfoDialog("In function getRegisterDataInfo:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function getRegisterDataInfo: no named registers defined!");
    }
  }
  else if(regist < SAVED_REGISTER_X) { // Temporary register register
    if(regist < FIRST_TEMPORARY_REGISTER+NUMBER_OF_TEMPORARY_REGISTERS) {
      return (tempRegister[regist - FIRST_TEMPORARY_REGISTER] >> OFFSET_REGISTER_INFORMATION) & ((1u << LENGTH_REGISTER_INFORMATION) - 1u);
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "temporary register %" FMT16S, regist - FIRST_TEMPORARY_REGISTER);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16S, FIRST_TEMPORARY_REGISTER + NUMBER_OF_TEMPORARY_REGISTERS - 1);
      showInfoDialog("In function getRegisterDataInfo:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else if(regist < 4000) { // Saved stack register
    if(regist <= SAVED_REGISTER_L) {
      return (savedStackRegister[regist - SAVED_REGISTER_X] >> OFFSET_REGISTER_INFORMATION) & ((1u << LENGTH_REGISTER_INFORMATION) - 1u);
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "saved stack register %" FMT16S, regist - SAVED_REGISTER_X);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to 7");
      showInfoDialog("In function getRegisterDataInfo:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else {
    sprintf(errorMessage, "In function getRegisterDataInfo: r=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
  return 0;
}



/********************************************//**
 * \brief returns the length of the memory area of the name of the named register
 *
 * \param[in] r calcRegister_t Register number
 * \return uint32_t      Length
 ***********************************************/
uint32_t getRegisterNameLength(calcRegister_t regist) {
  if(1000 <= regist && regist <= 1999) { // Named register
    if(numberOfNamedRegisters > 0) {
      regist -= 1000;
      if(regist < numberOfNamedRegisters) {
        return (*POINTER_TO_NAMED_REGISTER(regist) >> OFFSET_REGISTER_NAME_LENGTH) & ((1u << LENGTH_REGISTER_NAME_LENGTH) - 1u);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedRegisters - 1);
        showInfoDialog("In function getRegisterNameLength:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
     #endif
    }
    else {
      displayBugScreen("In function getRegisterNameLength: no named registers defined!");
    }
  }
  else {
    displayBugScreen("In function getRegisterNameLength: this function can be called only for a named register!");
  }
  return 0;
}



/********************************************//**
 * \brief returns the pointer to the memory area of the name of the named register
 *
 * \param[in] r calcRegister_t Register number
 * \return uint32_t      Pointer to the name
 ***********************************************/
uint32_t getRegisterNamePointer(calcRegister_t regist) {
  if(1000 <= regist && regist <= 1999) { // Named register
    if(numberOfNamedRegisters > 0) {
      regist -= 1000;
      if(regist < numberOfNamedRegisters) {
        return ((uint32_t)(*POINTER_TO_POINTER_TO_NAMED_REGISTER_NAME(regist))) << 1;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedRegisters - 1);
        showInfoDialog("In function getRegisterNamePointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function getRegisterNamePointer: no named registers defined!");
    }
   }
  else {
    displayBugScreen("In function getRegisterNamePointer: this function can be called only for a named register!");
  }
  return 0;
}



/********************************************//**
 * \brief Sets the data type of a register
 *
 * \param[in] r calcRegister_t        Register number
 * \param[in] dataType uint16_t Data type
 * \return void
 ***********************************************/
void setRegisterDataType(calcRegister_t regist, uint16_t dataType) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    reg[regist] = (reg[regist] & ~(((1u << LENGTH_REGISTER_DATA_TYPE) - 1u) << OFFSET_REGISTER_DATA_TYPE)) | (dataType << OFFSET_REGISTER_DATA_TYPE);
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < numberOfLocalRegisters) {
        *POINTER_TO_LOCAL_REGISTER(regist) = (*POINTER_TO_LOCAL_REGISTER(regist) & ~(((1u << LENGTH_REGISTER_DATA_TYPE) - 1u) << OFFSET_REGISTER_DATA_TYPE)) | (dataType << OFFSET_REGISTER_DATA_TYPE);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfLocalRegisters - 1);
        showInfoDialog("In function setRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
     showInfoDialog("In function setRegisterDataType:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }
  else if(regist < FIRST_TEMPORARY_REGISTER) { // Named register
    if(numberOfNamedRegisters > 0) {
      regist -= 1000;
      if(regist < numberOfNamedRegisters) {
        *POINTER_TO_NAMED_REGISTER(regist) = (*POINTER_TO_NAMED_REGISTER(regist) & ~(((1u << LENGTH_REGISTER_DATA_TYPE) - 1u) << OFFSET_REGISTER_DATA_TYPE)) | (dataType << OFFSET_REGISTER_DATA_TYPE);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedRegisters - 1);
        showInfoDialog("In function setRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function setRegisterDataType: no named registers defined!");
    }
  }
  else if(regist < SAVED_REGISTER_X) { // Temporary register
    if(regist < FIRST_TEMPORARY_REGISTER+NUMBER_OF_TEMPORARY_REGISTERS) {
      tempRegister[regist - FIRST_TEMPORARY_REGISTER] = (tempRegister[regist - FIRST_TEMPORARY_REGISTER] & ~(((1u << LENGTH_REGISTER_DATA_TYPE) - 1u) << OFFSET_REGISTER_DATA_TYPE)) | (dataType << OFFSET_REGISTER_DATA_TYPE);
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "temporary register %" FMT16S, regist - FIRST_TEMPORARY_REGISTER);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16S, FIRST_TEMPORARY_REGISTER + NUMBER_OF_TEMPORARY_REGISTERS - 1);
      showInfoDialog("In function setRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else if(regist < 4000) { // Saved stack register
    if(regist <= SAVED_REGISTER_L) {
      savedStackRegister[regist - SAVED_REGISTER_X] = (savedStackRegister[regist - SAVED_REGISTER_X] & ~(((1u << LENGTH_REGISTER_DATA_TYPE) - 1u) << OFFSET_REGISTER_DATA_TYPE)) | (dataType << OFFSET_REGISTER_DATA_TYPE);
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "saved stack register %" FMT16S, regist - SAVED_REGISTER_X);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to 7");
      showInfoDialog("In function setRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else {
    sprintf(errorMessage, "In function setRegisterDataType: r=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
}



/********************************************//**
 * \brief Sets the data pointer of a register
 *
 * \param[in] r calcRegister_t        Register number
 * \param[in] dataType uint16_t Data pointer
 * \return void
 ***********************************************/
void setRegisterDataPointer(calcRegister_t regist, uint32_t dataPointer) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    reg[regist] = (reg[regist] & ~(((1u << LENGTH_REGISTER_POINTER) - 1u) << OFFSET_REGISTER_POINTER)) | (dataPointer << OFFSET_REGISTER_POINTER);
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < numberOfLocalRegisters) {
        *POINTER_TO_LOCAL_REGISTER(regist) = (*POINTER_TO_LOCAL_REGISTER(regist) & ~(((1u << LENGTH_REGISTER_POINTER) - 1u) << OFFSET_REGISTER_POINTER)) | (dataPointer << OFFSET_REGISTER_POINTER);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfLocalRegisters - 1);
        showInfoDialog("In function setRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function setRegisterDataPointer:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }
  else if(regist < FIRST_TEMPORARY_REGISTER) { // Named register
    if(numberOfNamedRegisters > 0) {
      regist -= 1000u;
      if(regist < numberOfNamedRegisters) {
        *POINTER_TO_NAMED_REGISTER(regist) = (*POINTER_TO_NAMED_REGISTER(regist) & ~(((1u << LENGTH_REGISTER_POINTER) - 1u) << OFFSET_REGISTER_POINTER)) | (dataPointer << OFFSET_REGISTER_POINTER);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedRegisters - 1);
        showInfoDialog("In function setRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function setRegisterDataPointer:", "no local registers defined!", NULL, NULL);
    }
    #endif
  }
  else if(regist < SAVED_REGISTER_X) { // Temporary register
    if(regist < FIRST_TEMPORARY_REGISTER+NUMBER_OF_TEMPORARY_REGISTERS) {
      tempRegister[regist - FIRST_TEMPORARY_REGISTER] = (tempRegister[regist - FIRST_TEMPORARY_REGISTER] & ~(((1u << LENGTH_REGISTER_POINTER) - 1u) << OFFSET_REGISTER_POINTER)) | (dataPointer << OFFSET_REGISTER_POINTER);
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "temporary register %" FMT16S, regist - FIRST_TEMPORARY_REGISTER);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16S, FIRST_TEMPORARY_REGISTER + NUMBER_OF_TEMPORARY_REGISTERS - 1);
      showInfoDialog("In function setRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else if(regist < 4000) { // Saved stack register
    if(regist <= SAVED_REGISTER_L) {
      savedStackRegister[regist - SAVED_REGISTER_X] = (savedStackRegister[regist - SAVED_REGISTER_X] & ~(((1u << LENGTH_REGISTER_POINTER) - 1u) << OFFSET_REGISTER_POINTER)) | (dataPointer << OFFSET_REGISTER_POINTER);
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "saved stack register %" FMT16S, regist - SAVED_REGISTER_X);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to 7");
      showInfoDialog("In function setRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else {
    sprintf(errorMessage, "In function setRegisterDataPointer: r=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
}



/********************************************//**
 * \brief Sets the data information of a register:
 *        angular mode or base
 *
 * \param[in] r calcRegister_t        Register number
 * \param[in] dataType uint16_t Angular mode
 * \return void
 ***********************************************/
void setRegisterDataInfo(calcRegister_t regist, uint16_t dataInfo) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    reg[regist] = (reg[regist] & ~(((1u << LENGTH_REGISTER_INFORMATION) - 1u) << OFFSET_REGISTER_INFORMATION)) | (dataInfo << OFFSET_REGISTER_INFORMATION);
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < numberOfLocalRegisters) {
        *POINTER_TO_LOCAL_REGISTER(regist) = (*POINTER_TO_LOCAL_REGISTER(regist) & ~(((1u << LENGTH_REGISTER_INFORMATION) - 1u) << OFFSET_REGISTER_INFORMATION)) | (dataInfo << OFFSET_REGISTER_INFORMATION);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfLocalRegisters - 1);
        showInfoDialog("In function setRegisterDataInfo:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function setRegisterDataInfo:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }
  else if(regist < FIRST_TEMPORARY_REGISTER) { // Named register
    if(numberOfNamedRegisters > 0) {
      regist -= 1000u;
      if(regist < numberOfNamedRegisters) {
        *POINTER_TO_NAMED_REGISTER(regist) = (*POINTER_TO_NAMED_REGISTER(regist) & ~(((1u << LENGTH_REGISTER_INFORMATION) - 1u) << OFFSET_REGISTER_INFORMATION)) | (dataInfo << OFFSET_REGISTER_INFORMATION);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedRegisters - 1);
        showInfoDialog("In function setRegisterDataInfo:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function setRegisterDataInfo: no named registers defined!");
    }
   }
  else if(regist < SAVED_REGISTER_X) { // Temporary register
    if(regist < FIRST_TEMPORARY_REGISTER+NUMBER_OF_TEMPORARY_REGISTERS) {
      tempRegister[regist - FIRST_TEMPORARY_REGISTER] = (tempRegister[regist - FIRST_TEMPORARY_REGISTER] & ~(((1u << LENGTH_REGISTER_INFORMATION) - 1u) << OFFSET_REGISTER_INFORMATION)) | (dataInfo << OFFSET_REGISTER_INFORMATION);
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "temporary register %" FMT16S, regist - FIRST_TEMPORARY_REGISTER);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16S, FIRST_TEMPORARY_REGISTER + NUMBER_OF_TEMPORARY_REGISTERS - 1);
      showInfoDialog("In function setRegisterDataInfo:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else if(regist < 4000) { // Saved stack register
    if(regist <= SAVED_REGISTER_L) {
      savedStackRegister[regist - SAVED_REGISTER_X] = (savedStackRegister[regist - SAVED_REGISTER_X] & ~(((1u << LENGTH_REGISTER_INFORMATION) - 1u) << OFFSET_REGISTER_INFORMATION)) | (dataInfo << OFFSET_REGISTER_INFORMATION);
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "saved stack register %" FMT16S, regist - SAVED_REGISTER_X);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to 7");
      showInfoDialog("In function setRegisterDataInfo:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else {
    sprintf(errorMessage, "In function setRegisterDataInfo: r=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
}



/********************************************//**
 * \brief Sets the length of the memory area of the name of the named register
 *
 * \param[in] r calcRegister_t        Register number
 * \param[in] dataType uint16_t Length
 * \return void
 ***********************************************/
void setRegisterNameLength(calcRegister_t regist, uint16_t length) {
  if(1000 <= regist && regist <= 1999) { // Named register
    if(numberOfNamedRegisters > 0) {
      regist -= 1000;
      if(regist < numberOfNamedRegisters) {
        *POINTER_TO_NAMED_REGISTER(regist) = (*POINTER_TO_NAMED_REGISTER(regist) & ~(((1u << LENGTH_REGISTER_NAME_LENGTH) - 1u) << OFFSET_REGISTER_NAME_LENGTH)) | (length << OFFSET_REGISTER_NAME_LENGTH);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedRegisters - 1);
        showInfoDialog("In function setRegisterNameLength:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function setRegisterNameLength: no named registers defined!");
    }
  }
  else {
    displayBugScreen("In function setRegisterNameLength: this function can be called only for a named register!");
  }
}



/********************************************//**
 * \brief Sets the pointer to the memory area of the name of the named register
 *
 * \param[in] r calcRegister_t        Register number
 * \param[in] dataType uint16_t Pointer
 * \return void
 ***********************************************/
void setRegisterNamePointer(calcRegister_t regist, uint32_t pointer) {
  if(1000 <= regist && regist <= 1999) { // Named register
    if(numberOfNamedRegisters > 0) {
      regist -= 1000;
      if(regist < numberOfNamedRegisters) {
        *POINTER_TO_POINTER_TO_NAMED_REGISTER_NAME(regist) = pointer >> 1;
      }
      else {
        sprintf(errorMessage, "In function setRegisterNamePointer: named register %" FMT16S " is not defined! Must be from 0 to %" FMT16U, regist, numberOfNamedRegisters - 1);
        displayBugScreen(errorMessage);
      }
    }
    else {
      displayBugScreen("In function setRegisterNamePointer: no named registers defined!");
    }
  }
  else {
    displayBugScreen("In function setRegisterNamePointer: this function can be called only for a named register!");
  }
}



/********************************************//**
 * \brief Allocates memory at the beginning of the
 * free memory space
 *
 * \param[in] numBytes uint32_t Number of bytes to allocate
 * \return uint32_t             Pointer to the allocated memory
 ***********************************************/
uint32_t allocateMemory(uint32_t numBytes) {
  // only allocate an even number of bytes
  if(numBytes % 2u) {
    numBytes++;
  }

  if(numBytes <= lastFreeByte - firstFreeByte + 1u) {
    firstFreeByte += numBytes;
    return firstFreeByte - numBytes;
  }
  else {
    displayCalcErrorMessage(11, ERR_REGISTER_LINE, REGISTER_T);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Trying to allocate %" FMT32U " bytes", numBytes);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "but only %" FMT32U " left!", lastFreeByte - firstFreeByte + 1u);
      showInfoDialog("In Function allocateMemory:", "out of memory!", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2);
    #endif
    return 0;
  }
}



/********************************************//**
 * \brief Allocates memory at a given address
 *
 * \param[in] address uint32_t  Address where to allocate memory
 * \param[in] numBytes uint32_t Number of bytes to allocate
 * \return void
 ***********************************************/
void allocateMemoryInsert(uint32_t address, uint32_t numBytes) {
  // only allocate an even number of bytes
  if(numBytes % 2u) {
    numBytes++;
  }

  if(numBytes <= lastFreeByte - firstFreeByte + 1u) {
    if(address <= firstFreeByte) {
      uint16_t r;
      uint32_t addr;

      // Adjust the pointers to the moved area: global registers pointers
      for(r=0; r<FIRST_LOCAL_REGISTER; r++) {
        addr = getRegisterDataPointer(r);
        if(address <= addr && addr < firstFreeByte) {
          setRegisterDataPointer(r, addr + numBytes);
        }
      }

      // Adjust the pointers to the moved area: local registers pointers
      for(r=FIRST_LOCAL_REGISTER; r<FIRST_LOCAL_REGISTER+numberOfLocalRegisters; r++) {
        addr = getRegisterDataPointer(r);
        if(address <= addr && addr < firstFreeByte) {
          setRegisterDataPointer(r, addr + numBytes);
        }
      }

      // Adjust the pointers to the moved area: named registers pointers
      for(r=1000u; r<1000u+numberOfNamedRegisters; r++) {
        addr = getRegisterDataPointer(r);
        if(address <= addr && addr < firstFreeByte) {
          setRegisterDataPointer(r, addr + numBytes);
        }

        addr = getRegisterNamePointer(r);
        if(address <= addr && addr < firstFreeByte) {
          setRegisterNamePointer(r, addr + numBytes);
        }
      }

      // Adjust the pointers to the moved area: temporary registers pointers
      for(r=FIRST_TEMPORARY_REGISTER; r<FIRST_TEMPORARY_REGISTER+NUMBER_OF_TEMPORARY_REGISTERS; r++) {
        addr = getRegisterDataPointer(r);
        if(address <= addr && addr < firstFreeByte) {
          setRegisterDataPointer(r, addr + numBytes);
        }
      }

      // Adjust the pointers to the moved area: temporary registers pointers
      for(r=SAVED_REGISTER_X; r<=SAVED_REGISTER_L; r++) {
        addr = getRegisterDataPointer(r);
        if(address <= addr && addr < firstFreeByte) {
          setRegisterDataPointer(r, addr + numBytes);
        }
      }

      // Adjust the pointers to the moved area: all local registers
      // It's MANDATORY that following block executes after the adjustment of the local register data pointers
      // because the actual memory move is only done after!
      if(address <= allLocalRegisterPointer && allLocalRegisterPointer < firstFreeByte) {
        allLocalRegisterPointer += numBytes;
      }

      // Adjust the pointers to the moved area: all named registers
      // It's MANDATORY that the following block executes after the adjustment of the named register data pointers
      // because the actual memory move is only done after!
      if(address <= allNamedRegisterPointer && allNamedRegisterPointer < firstFreeByte) {
        allNamedRegisterPointer += numBytes;
      }

      if(address <= statisticalSumsPointer && statisticalSumsPointer < firstFreeByte) {
        statisticalSumsPointer += numBytes;
      }

      // Move the memory
      memmove(ram + address + numBytes, ram + address, firstFreeByte - address);

      // Adjust the 1st free byte
      firstFreeByte += numBytes;
    }
  }
  else {
    displayCalcErrorMessage(11, ERR_REGISTER_LINE, REGISTER_T);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage,      "Trying to allocate %" FMT32U " bytes", numBytes);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "while only %" FMT32U " left!", lastFreeByte - firstFreeByte + 1u);
      showInfoDialog("In Function allocateMemoryInsert:", "out of memory!", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2);
    #endif
  }
}



/********************************************//**
 * \brief Frees memory at the given address
 *
 * \param[in] address uint32_t  Address where to free the memory
 * \param[in] numBytes uint32_t Number of bytes to free
 * \return void
 ***********************************************/
void freeMemory(uint32_t address, uint32_t numBytes) {
  // only free an even number of bytes
  if(numBytes % 2u) {
    numBytes++;
  }

  if(numBytes > 0) { // There is something to do
    if(address + numBytes > firstFreeByte) {
      displayBugScreen("in function freeMemory: the memory area to free is not in the allocated memory area!");
    }
    else {
      uint32_t addr;
      calcRegister_t regist;

      // Adjust the pointers to the moved area: global registers pointers
      for(regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
        addr = getRegisterDataPointer(regist);
        if(address+numBytes <= addr && addr < firstFreeByte) {
          setRegisterDataPointer(regist, addr - numBytes);
        }
      }

      // Adjust the pointers to the moved area: local registers pointers
      for(regist=FIRST_LOCAL_REGISTER; regist<FIRST_LOCAL_REGISTER+numberOfLocalRegisters; regist++) {
        addr = getRegisterDataPointer(regist);
        if(address+numBytes <= addr && addr < firstFreeByte) {
          setRegisterDataPointer(regist, addr - numBytes);
        }
      }

      // Adjust the pointers to the moved area: named registers pointers
      for(regist=1000; regist<1000+numberOfNamedRegisters; regist++) {
        addr = getRegisterDataPointer(regist);
        if(address+numBytes <= addr && addr < firstFreeByte) {
          setRegisterDataPointer(regist, addr - numBytes);
        }

        addr = getRegisterNamePointer(regist);
        if(address+numBytes <= addr && addr < firstFreeByte) {
          setRegisterNamePointer(regist, addr - numBytes);
        }
      }

      // Adjust the pointers to the moved area: temporary registers pointers
      for(regist=FIRST_TEMPORARY_REGISTER; regist<FIRST_TEMPORARY_REGISTER+NUMBER_OF_TEMPORARY_REGISTERS; regist++) {
        addr = getRegisterDataPointer(regist);
        if(address+numBytes <= addr && addr < firstFreeByte) {
          setRegisterDataPointer(regist, addr - numBytes);
        }
      }

      // Adjust the pointers to the moved area: saved stack registers pointers
      for(regist=SAVED_REGISTER_X; regist<=SAVED_REGISTER_L; regist++) {
        addr = getRegisterDataPointer(regist);
        if(address+numBytes <= addr && addr < firstFreeByte) {
          setRegisterDataPointer(regist, addr - numBytes);
        }
      }

      // Adjust the pointers to the moved area: all local registers
      // It's MANDATORY that the following block executes after the adjustment of the local register data pointers
      // because the actual memory move is only done after!
      if(address+numBytes <= allLocalRegisterPointer && allLocalRegisterPointer < firstFreeByte) {
        allLocalRegisterPointer -= numBytes;
      }

      // Adjust the pointers to the moved area: all named registers
      // It's MANDATORY that following block executes after the adjustment of the named register data pointers
      // because the actual memory move is only done after!
      if(address+numBytes <= allNamedRegisterPointer && allNamedRegisterPointer < firstFreeByte) {
        allNamedRegisterPointer -= numBytes;
      }

      if(address+numBytes <= statisticalSumsPointer && statisticalSumsPointer < firstFreeByte) {
        statisticalSumsPointer -= numBytes;
      }

      // Move the memory
      memmove(ram + address, ram + address + numBytes, firstFreeByte - address);

      // Adjust the 1st free byte
      firstFreeByte -= numBytes;
    }
  }
}



/********************************************//**
 * \brief Allocates local registers. Works when increasing
 * and when decreasing the number of local registers.
 *
 * \param[in] numberOfRegistersToAllocate uint16_t Number of registers to allocate
 * \return void
 ***********************************************/
void allocateLocalRegisters(uint16_t numberOfRegistersToAllocate) {
  if(numberOfRegistersToAllocate != numberOfLocalRegisters) { // There is something to do
    uint16_t numRegs, r;

    if(numberOfRegistersToAllocate == 0) { // free the allocated memory for the local registers
      numRegs = numberOfLocalRegisters;

      // free memory allocated to the data of the local registers
      for(r=FIRST_LOCAL_REGISTER; r<FIRST_LOCAL_REGISTER+numRegs; r++) {
        freeRegisterData(r);
      }

      numberOfLocalRegisters = 0; // This must be done before the freeMemory call
      numberOfLocalFlags = 0;
      freeMemory(allLocalRegisterPointer, 2 + 4*numRegs);

      allLocalRegisterPointer = 0;
    }

    else if(numberOfLocalRegisters == 0) { // Allocate memory
      allLocalRegisterPointer = allocateMemory(2u + 4u*numberOfRegistersToAllocate);
      numberOfLocalRegisters = numberOfRegistersToAllocate;
      numberOfLocalFlags = 16;

      // clear all local flags
      *POINTER_TO_LOCAL_FLAGS = 0;

      // All the new local registers are real16s initialized to 0.0
      for(r=FIRST_LOCAL_REGISTER; r<FIRST_LOCAL_REGISTER+numberOfRegistersToAllocate; r++) {
        setRegisterDataType(r, dtReal16);
        setRegisterDataPointer(r, allocateMemory(REAL16_SIZE));
        real16Zero(REGISTER_REAL16_DATA(r));
      }
    }

    else if(numberOfRegistersToAllocate > numberOfLocalRegisters) { // increase the number of local register
      allocateMemoryInsert(allLocalRegisterPointer + 2u + 4u*numberOfLocalRegisters, 4u*(numberOfRegistersToAllocate-numberOfLocalRegisters));

      uint16_t oldNumRegs = numberOfLocalRegisters;
      numberOfLocalRegisters = numberOfRegistersToAllocate;

      // All the new local registers are real16s initialized to 0.0
      for(r=FIRST_LOCAL_REGISTER+oldNumRegs; r<FIRST_LOCAL_REGISTER+numberOfRegistersToAllocate; r++) {
        setRegisterDataType(r, dtReal16);
        setRegisterDataPointer(r, allocateMemory(REAL16_SIZE));
        real16Zero(REGISTER_REAL16_DATA(r));
      }
    }

    else { // reduce the number of local register
      // free memory allocated to the data of the deleted local registers
      for(r=numberOfLocalRegisters - 1; r>=numberOfRegistersToAllocate; r--) {
        freeRegisterData(FIRST_LOCAL_REGISTER + r);
      }

      // free memory allocated to the deleted local registers
      freeMemory(allLocalRegisterPointer + 2u + 4*numberOfRegistersToAllocate, 4u*(numberOfLocalRegisters - numberOfRegistersToAllocate));
      numberOfLocalRegisters = numberOfRegistersToAllocate; // This must be done after the freeMemory call
    }
  }
}



/********************************************//**
 * \brief Allocates 1 named register.
 *
 * \param[in] registerName const char* Register name
 * \return void
 ***********************************************/
void allocateNamedRegister(const char *registerName) {
  uint32_t len;
  calcRegister_t regist;

  if(stringGlyphLength(registerName) < 1 || stringGlyphLength(registerName) > 7) {
    #ifdef PC_BUILD
      sprintf(errorMessage, "the name %s", registerName);
      showInfoDialog("In function allocateNamedRegister:", errorMessage, "is incorrect! The length must be", "from 1 to 7 glyphs!");
    #endif
    return;
  }

  if(numberOfNamedRegisters == 0) { // First named register
    allNamedRegisterPointer = allocateMemory(6);
    numberOfNamedRegisters = 1;

    regist = 1000;
  }
  else {
    regist = numberOfNamedRegisters;
    if(regist == 999u) {
      #ifdef PC_BUILD
        showInfoDialog("In function allocateNamedRegister:", "you can allocate up to", "999 named registers!", NULL);
      #endif
      return;
    }

    numberOfNamedRegisters++;
    allocateMemoryInsert(allNamedRegisterPointer + 6u*regist, 6u);

    regist += 1000;
  }

  // The new named register is a real16 initialized to 0.0
  setRegisterDataType(regist, dtReal16);

  len = stringByteLength(registerName) + 1;
  if(len % 2) {
    len++;
  }

  setRegisterNamePointer(regist, allocateMemory(len));
  setRegisterNameLength(regist, len>>1);
  memcpy(ram + getRegisterNamePointer(regist), registerName, len);

  setRegisterDataPointer(regist, allocateMemory(REAL16_SIZE));
  real16Zero(REGISTER_REAL16_DATA(regist));
}



/********************************************//**
 * \brief Sets the max length of string
 *
 * \param[in] r calcRegister_t            Register number
 * \param[in] maxStringLen uint16_t Max length of the string
 * \return void
 ***********************************************/
void setRegisterMaxDataLength(calcRegister_t regist, uint16_t maxDatagLen) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    *(uint16_t *)(ram + ((reg[regist] >> OFFSET_REGISTER_POINTER) & ((1u << LENGTH_REGISTER_POINTER) - 1u))) = maxDatagLen;
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      if(regist-FIRST_LOCAL_REGISTER < numberOfLocalRegisters) {
        *(uint16_t *)(REGISTER_DATA(regist)) = maxDatagLen;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S " is not defined!", regist - FIRST_LOCAL_REGISTER);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfLocalRegisters - 1);
        showInfoDialog("In function setRegisterMaxDataLength:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
     showInfoDialog("In function setRegisterMaxDataLength:", "no local registers defined!", NULL, NULL);
    }
    #endif
  }
  else if(regist < FIRST_TEMPORARY_REGISTER) { // Named register
    if(numberOfNamedRegisters > 0) {
      if(regist-1000 < numberOfNamedRegisters) {
        *(uint16_t *)(REGISTER_DATA(regist)) = maxDatagLen;
      }
      else {
        sprintf(errorMessage, "In function setRegisterMaxDataLength: named register %" FMT16S " is not defined! Must be from 0 to %" FMT16U, regist - 1000, numberOfNamedRegisters - 1);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function setRegisterMaxDataLength:", "no named registers defined!", NULL, NULL);
    }
    #endif
  }
  else if(regist < SAVED_REGISTER_X) { // Temporary register
    if(regist < FIRST_TEMPORARY_REGISTER+NUMBER_OF_TEMPORARY_REGISTERS) {
      *(uint16_t *)(REGISTER_DATA(regist)) = maxDatagLen;
    }
    else {
      sprintf(errorMessage, "In function setRegisterMaxDataLength: temporary register %" FMT16S " is not defined! Must be from 0 to %" FMT16S "!", regist - FIRST_TEMPORARY_REGISTER, FIRST_TEMPORARY_REGISTER + NUMBER_OF_TEMPORARY_REGISTERS - 1);
      displayBugScreen(errorMessage);
    }
  }
  else if(regist < 4000) { // Saved stack register
    if(regist <= SAVED_REGISTER_L) {
      *(uint16_t *)(REGISTER_DATA(regist)) = maxDatagLen;
    }
    else {
      sprintf(errorMessage, "In function setRegisterMaxDataLength: saved stack register %" FMT16S " is not defined! Must be from 0 to 7!", regist - SAVED_REGISTER_X);
      displayBugScreen(errorMessage);
    }
  }
  else {
    sprintf(errorMessage, "In function setRegisterMaxDataLength: regist=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
}



/********************************************//**
 * \brief Returns the max length of a string in bytes
 *
 * \param[in] r calcRegister_t Register number
 * \return uint16_t      Number of bytes
 *
 ***********************************************/
uint16_t getRegisterMaxDataLength(calcRegister_t regist) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    return *(uint16_t *)(ram + ((reg[regist] >> OFFSET_REGISTER_POINTER) & ((1u << LENGTH_REGISTER_POINTER) - 1u)));
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      if(regist-FIRST_LOCAL_REGISTER < numberOfLocalRegisters) {
        return *(uint16_t *)(REGISTER_DATA(regist));
      }
      else {
        sprintf(errorMessage, "In function getRegisterMaxDataLength: local register %" FMT16S " is not defined! Must be from 0 to %" FMT16U, regist - FIRST_LOCAL_REGISTER, numberOfLocalRegisters - 1);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function getRegisterMaxStringLength:", "no local registers defined!", NULL, NULL);
    }
    #endif
  }
  else if(regist < FIRST_TEMPORARY_REGISTER) { // Named register
    if(numberOfNamedRegisters > 0) {
      if(regist-1000 < numberOfNamedRegisters) {
        return *(uint16_t *)(REGISTER_DATA(regist));
      }
      else {
        sprintf(errorMessage, "In function getRegisterMaxDataLength: named register %" FMT16S " is not defined! Must be from 0 to %" FMT16U, regist - 1000, numberOfNamedRegisters - 1);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function getRegisterMaxStringLength:", "no local registers defined!", NULL, NULL);
    }
    #endif
  }
  else if(regist < SAVED_REGISTER_X) { // Temporary register
    if(regist < FIRST_TEMPORARY_REGISTER+NUMBER_OF_TEMPORARY_REGISTERS) {
      return *(uint16_t *)(REGISTER_DATA(regist));
    }
    else {
      sprintf(errorMessage, "In function getRegisterMaxDataLength: temporary register %" FMT16S " is not defined! Must be from 0 to %" FMT16S "!", regist - FIRST_TEMPORARY_REGISTER, FIRST_TEMPORARY_REGISTER + NUMBER_OF_TEMPORARY_REGISTERS - 1);
      displayBugScreen(errorMessage);
    }
  }
  else if(regist < 4000) { // Saved stack register
    if(regist <= SAVED_REGISTER_L) {
      return *(uint16_t *)(REGISTER_DATA(regist));
    }
    else {
      sprintf(errorMessage, "In function getRegisterMaxDataLength: saved stack register %" FMT16S " is not defined! Must be from 0 to 7!", regist - SAVED_REGISTER_X);
      displayBugScreen(errorMessage);
    }
  }
  return 0;
}



/********************************************//**
 * \brief Returns the full data size of a register
 *
 * \param[in] r calcRegister_t Register number
 * \return uint32_t      Number of bytes. For a string this
 *                       is the number of bytes reserved for
 *                       the string (including the ending 0)
 *                       plus 2 bytes holding the max size
 *                       of the string.
 ***********************************************/
uint32_t getRegisterFullSize(calcRegister_t regist) {
  uint32_t dataType = getRegisterDataType(regist);

  if     (dataType == dtReal16      ) return REAL16_SIZE;
  else if(dataType == dtComplex16   ) return COMPLEX16_SIZE;
  else if(dataType == dtReal34      ) return REAL34_SIZE;
  else if(dataType == dtAngle       ) return ANGLE_SIZE;
  else if(dataType == dtComplex34   ) return COMPLEX34_SIZE;
  else if(dataType == dtShortInteger) return SHORT_INTEGER_SIZE;
  else if(dataType == dtString      ) return *(REGISTER_DATA_MAX_LEN(regist)) + 2; // +2 because of the 2 first bytes holding the size
  else if(dataType == dtLongInteger ) return *(REGISTER_DATA_MAX_LEN(regist)) + 2; // +2 because of the 2 first bytes holding the size
  else {
    sprintf(errorMessage, "In function getRegisterFullSize: data type %s is unknown!", getDataTypeName(dataType, true, false));
    displayBugScreen(errorMessage);
  }
  return 0;
}



/********************************************//**
 * \brief Returns the data only size of a register
 *
 * \param[in] r calcRegister_t Register number
 * \return uint32_t      Number of bytes. For a string this
 *                       is the number of bytes reserved for
 *                       the string (including the ending 0)
 *                       plus 2 bytes holding the max size
 *                       of the string.
 ***********************************************/
uint32_t getRegisterDataOnlySize(calcRegister_t regist) {
  uint32_t dataType = getRegisterDataType(regist);

  if     (dataType == dtReal16      ) return REAL16_SIZE;
  else if(dataType == dtComplex16   ) return COMPLEX16_SIZE;
  else if(dataType == dtReal34      ) return REAL34_SIZE;
  else if(dataType == dtAngle       ) return ANGLE_SIZE;
  else if(dataType == dtComplex34   ) return COMPLEX34_SIZE;
  else if(dataType == dtShortInteger) return SHORT_INTEGER_SIZE;
  else if(dataType == dtString      ) return *(REGISTER_DATA_MAX_LEN(regist));
  else if(dataType == dtLongInteger ) return *(REGISTER_DATA_MAX_LEN(regist));
  else {
    sprintf(errorMessage, "In function getRegisterDataOnlySize: data type %s is unknown!", getDataTypeName(dataType, true, false));
    displayBugScreen(errorMessage);
  }
  return 0;
}



/********************************************//**
 * \brief Clears a register, that is, sets it to 0,0 real16
 *
 * \param[in] r calcRegister_t Register number
 * \return void
 ***********************************************/
void clearRegister(calcRegister_t regist) {
  if(getRegisterDataType(regist) != dtReal16) {
    reallocateRegister(regist, dtReal16, REAL16_SIZE, 0);
  }

  real16Zero(REGISTER_REAL16_DATA(regist));
}



/********************************************//**
 * \brief Clears all the regs (globals and locals),
 * that is sets them to 0,0 real16s
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnClearRegisters(uint16_t unusedParamButMandatory) {
  calcRegister_t regist;

  for(regist=0; regist<REGISTER_X; regist++) {
    clearRegister(regist);
  }

  for(regist=0; regist<numberOfLocalRegisters; regist++) {
    clearRegister(FIRST_LOCAL_REGISTER + regist);
  }

  if(stackSize == SS_4) {
    for(regist=REGISTER_A; regist<=REGISTER_D; regist++) {
      clearRegister(FIRST_LOCAL_REGISTER + regist);
    }
  }

  for(regist=REGISTER_I; regist<=REGISTER_K; regist++) {
    clearRegister(FIRST_LOCAL_REGISTER + regist);
  }
}



/********************************************//**
 * \brief Sets X to the number of local registers
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetLocR(uint16_t unusedParamButMandatory) {
  longInteger_t locR;

  liftStack();

  uIntToLongInteger(numberOfLocalRegisters, &locR);
  convertLongIntegerToLongIntegerRegister(&locR, REGISTER_X);

  refreshStack();
}



/********************************************//**
 * \brief Convert X to a real16 or complex
 *        if it is not already. Throws an
 *        error if X is not a single or double
 *        precision real or complex.
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConvertXToReal16(uint16_t unusedParamButMandatory) {
  if(getRegisterDataType(REGISTER_X) == dtReal34 || getRegisterDataType(REGISTER_X) == dtComplex34) {
    convertRegister34To16(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) != dtReal16 && getRegisterDataType(REGISTER_X) != dtComplex16) {
    displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot convert %s to a SP!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnConvertXToReal16:", errorMessage, NULL, NULL);
    #endif
  }

  refreshRegisterLine(REGISTER_X);
}



/********************************************//**
 * \brief Convert X to a real34 or complex
 *        if it is not already. Throws an
 *        error if X is not a single or double
 *        precision real or complex.
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConvertXToReal34(uint16_t unusedParamButMandatory) {
  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtComplex16) {
    convertRegister16To34(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) != dtReal34 && getRegisterDataType(REGISTER_X) != dtComplex34) {
    displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot convert %s to a DP!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnConvertXToReal34:", errorMessage, NULL, NULL);
    #endif
  }

  refreshRegisterLine(REGISTER_X);
}



/********************************************//**
 * \brief Rounding the register r to the number
 *        of significant digits returned by
 *        getSignificantDigits() following the
 *        rounding rule stored in the context
 *        structure.
 *
 * \param[in] r calcRegister_t Register number
 * \return void
 ***********************************************/
void roundRegister(calcRegister_t regist) { // TODO: we can make better here! Do not use an intermediate string
  #if (ANGLE16 == 1)
  if(getRegisterDataType(regist) == dtReal16 || getRegisterDataType(regist) == dtAngle) {
  #endif
  #if (ANGLE34 == 1)
  if(getRegisterDataType(regist) == dtReal16) {
  #endif
    if(real16IsZero(REGISTER_REAL16_DATA(regist))) {
      real16SetPositiveSign(REGISTER_REAL16_DATA(regist));
    }

    if(significantDigits == 0 || significantDigits >= 16) {
      return;
    }

    real51_t tmp;

    real16ToString(REGISTER_REAL16_DATA(regist), tmpStr3000);

    ctxtReal51.digits = significantDigits;
    stringToReal51Ctxt(tmpStr3000, &tmp, &ctxtReal51);
    ctxtReal51.digits = 51;

    real51ToReal16(&tmp, REGISTER_REAL16_DATA(regist));
  }

  #if (ANGLE16 == 1)
  else if(getRegisterDataType(regist) == dtReal34) {
  #endif
  #if (ANGLE34 == 1)
  else if(getRegisterDataType(regist) == dtReal34 || getRegisterDataType(regist) == dtAngle) {
  #endif
    if(real34IsZero(REGISTER_REAL34_DATA(regist))) {
      real34SetPositiveSign(REGISTER_REAL34_DATA(regist));
    }

    if(significantDigits == 0 || significantDigits >= 34) {
      return;
    }

    real51_t tmp;

    real34ToString(REGISTER_REAL34_DATA(regist), tmpStr3000);

    ctxtReal51.digits = significantDigits;
    stringToReal51Ctxt(tmpStr3000, &tmp, &ctxtReal51);
    ctxtReal51.digits = 51;

    real51ToReal34(&tmp, REGISTER_REAL34_DATA(regist));
  }

  else if(getRegisterDataType(regist) == dtComplex16) {
    if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
      real16SetPositiveSign(REGISTER_REAL16_DATA(REGISTER_X));
    }

    if(real16IsZero(REGISTER_IMAG16_DATA(REGISTER_X))) {
      real16SetPositiveSign(REGISTER_IMAG16_DATA(REGISTER_X));
    }

    if(significantDigits == 0 || significantDigits >= 16) {
      return;
    }

    real51_t tmp;

    ctxtReal51.digits = significantDigits;

    real16ToString(REGISTER_REAL16_DATA(regist), tmpStr3000);
    stringToReal51Ctxt(tmpStr3000, &tmp, &ctxtReal51);
    real51ToReal16(&tmp, REGISTER_REAL16_DATA(regist));

    real16ToString(REGISTER_IMAG16_DATA(regist), tmpStr3000);
    stringToReal51Ctxt(tmpStr3000, &tmp, &ctxtReal51);
    real51ToReal16(&tmp, REGISTER_IMAG16_DATA(regist));

    ctxtReal51.digits = 51;
  }

  else if(getRegisterDataType(regist) == dtComplex34) {
    if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
      real34SetPositiveSign(REGISTER_REAL34_DATA(REGISTER_X));
    }

    if(real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X))) {
      real34SetPositiveSign(REGISTER_IMAG34_DATA(REGISTER_X));
    }

    if(significantDigits == 0 || significantDigits >= 34) {
      return;
    }

    real51_t tmp;

    ctxtReal51.digits = significantDigits;

    real34ToString(REGISTER_REAL34_DATA(regist), tmpStr3000);
    stringToReal51Ctxt(tmpStr3000, &tmp, &ctxtReal51);
    real51ToReal34(&tmp, REGISTER_REAL34_DATA(regist));

    real34ToString(REGISTER_IMAG34_DATA(regist), tmpStr3000);
    stringToReal51Ctxt(tmpStr3000, &tmp, &ctxtReal51);
    real51ToReal34(&tmp, REGISTER_IMAG34_DATA(regist));

    ctxtReal51.digits = 51;
  }

  else {
    sprintf(errorMessage, "In function roundRegister: %s cannot be rounded!", getRegisterDataTypeName(regist, true, false));
    displayBugScreen(errorMessage);
  }
}



/********************************************//**
 * \brief Duplicates register source to register destination
 *
 * \param[in] sourceRegister Source register
 * \param[in] destRegister   Destination register
 * \return void
 ***********************************************/
void copySourceRegisterToDestRegister(calcRegister_t sourceRegister, calcRegister_t destRegister) {
  if(getRegisterDataType(destRegister) != getRegisterDataType(sourceRegister) || getRegisterFullSize(destRegister) != getRegisterFullSize(sourceRegister) || getRegisterDataInfo(destRegister) != getRegisterDataInfo(sourceRegister)) {
    reallocateRegister(destRegister, getRegisterDataType(sourceRegister), getRegisterDataOnlySize(sourceRegister), getRegisterDataInfo(sourceRegister));
  }

  memcpy(REGISTER_DATA(destRegister), REGISTER_DATA(sourceRegister), getRegisterFullSize(sourceRegister));
}



/********************************************//**
 * \brief Stores X in an other register
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStore(uint16_t r) {
  calcRegister_t regist = r;

  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, regist);
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnStore:", errorMessage, "is not defined!", NULL);
  }
  #endif

  refreshStack();
}



/********************************************//**
 * \brief Adds X to a register
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreAdd(uint16_t r) {
  calcRegister_t regist = r;

  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    if(addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(regist)] != addError) {
      result = regist;
      opY    = allocateTemporaryRegister();
      opX    = allocateTemporaryRegister();
      copySourceRegisterToDestRegister(regist, opY);
      copySourceRegisterToDestRegister(REGISTER_X, opX);

      addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(regist)]();
      freeTemporaryRegister(opY);
      freeTemporaryRegister(opX);

      refreshStack();
    }
    else {
     addError();
    }
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnStorePlus:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Subtracts X from a register
 *
 * \param[in] registerNumber calcRegister_t
 * \return void
 ***********************************************/
void fnStoreSub(uint16_t r) {
  calcRegister_t regist = r;

  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    if(subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(regist)] != subError) {
      result = regist;
      opY    = allocateTemporaryRegister();
      opX    = allocateTemporaryRegister();
      copySourceRegisterToDestRegister(regist, opY);
      copySourceRegisterToDestRegister(REGISTER_X, opX);

      subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(regist)]();
      freeTemporaryRegister(opY);
      freeTemporaryRegister(opX);

      refreshStack();
    }
    else {
      subError();
    }
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnStoreMinus:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Multiplies a register by X
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreMult(uint16_t r) {
  calcRegister_t regist = r;

  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    if(multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(regist)] != mulError) {
      result = regist;
      opY    = allocateTemporaryRegister();
      opX    = allocateTemporaryRegister();
      copySourceRegisterToDestRegister(regist, opY);
      copySourceRegisterToDestRegister(REGISTER_X, opX);

      multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(regist)]();
      freeTemporaryRegister(opY);
      freeTemporaryRegister(opX);

      refreshStack();
    }
    else {
      mulError();
    }
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnStoreMult:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Divides a register by X
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreDiv(uint16_t r) {
  calcRegister_t regist = r;

  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters)   {
    if(division[getRegisterDataType(REGISTER_X)][getRegisterDataType(regist)] != divError)     {
      result = regist;
      opY    = allocateTemporaryRegister();
      opX    = allocateTemporaryRegister();
      copySourceRegisterToDestRegister(regist, opY);
      copySourceRegisterToDestRegister(REGISTER_X, opX);

      division[getRegisterDataType(REGISTER_X)][getRegisterDataType(regist)]();
      freeTemporaryRegister(opY);
      freeTemporaryRegister(opX);

      refreshStack();
    }
    else {
      divError();
    }
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
      showInfoDialog("In function fnStoreDiv:", errorMessage, "is not defined!", NULL);
    }
  #endif
}



/********************************************//**
 * \brief Keeps in a register min(X, register)
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreMin(uint16_t r) {
  #ifdef PC_BUILD
    showInfoDialog("In function fnStoreMin:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Keeps in a register max(X, register)
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreMax(uint16_t r) {
  #ifdef PC_BUILD
    showInfoDialog("In function fnStoreMax:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Stores the configuration
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnStoreConfig(uint16_t r) {
  #ifdef PC_BUILD
    showInfoDialog("In function fnStoreConfig:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Stores the stack
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnStoreStack(uint16_t r) {
  #ifdef PC_BUILD
    showInfoDialog("In function fnStoreStack:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Stores X in the element I,J of a matrix
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnStoreElement(uint16_t r) {
  #ifdef PC_BUILD
    showInfoDialog("In function fnStoreElement:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Stores X and Y in the indexes I and J
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnStoreIJ(uint16_t r) {
  #ifdef PC_BUILD
    showInfoDialog("In function fnStoreIJ:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Recalls a register in X
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecall(uint16_t r) {
  calcRegister_t regist = r;

  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    if(REGISTER_X <= regist && regist <= getStackTop()) {
      calcRegister_t temp = allocateTemporaryRegister();

      copySourceRegisterToDestRegister(regist, temp);
      liftStack();
      copySourceRegisterToDestRegister(temp, REGISTER_X);
      freeTemporaryRegister(temp);
    }
    else {
      liftStack();
      copySourceRegisterToDestRegister(regist, REGISTER_X);
    }
    refreshStack();
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnRecall:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Adds a register to X
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallAdd(uint16_t r) {
  calcRegister_t regist = r;

  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    if(addition[getRegisterDataType(regist)][getRegisterDataType(REGISTER_X)] != addError) {
      copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

      result = REGISTER_X;
      opY    = allocateTemporaryRegister();
      opX    = allocateTemporaryRegister();
      copySourceRegisterToDestRegister(REGISTER_X, opY);
      copySourceRegisterToDestRegister(regist, opX);

      addition[getRegisterDataType(regist)][getRegisterDataType(REGISTER_X)]();
      freeTemporaryRegister(opY);
      freeTemporaryRegister(opX);

      refreshStack();
    }
    else {
      addError();
    }
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnRecallPlus:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Subtracts a register from X
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallSub(uint16_t r) {
  calcRegister_t regist = r;

  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    if(subtraction[getRegisterDataType(regist)][getRegisterDataType(REGISTER_X)] != subError) {
      copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

      result = REGISTER_X;
      opY    = allocateTemporaryRegister();
      opX    = allocateTemporaryRegister();
      copySourceRegisterToDestRegister(REGISTER_X, opY);
      copySourceRegisterToDestRegister(regist, opX);

      subtraction[getRegisterDataType(regist)][getRegisterDataType(REGISTER_X)]();
      freeTemporaryRegister(opY);
      freeTemporaryRegister(opX);

      refreshStack();
    }
    else {
      subError();
    }
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnRecallMinus:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Multiplies X by a register
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallMult(uint16_t r) {
  calcRegister_t regist = r;

  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    if(multiplication[getRegisterDataType(regist)][getRegisterDataType(REGISTER_X)] != mulError) {
      copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

      result = REGISTER_X;
      opY    = allocateTemporaryRegister();
      opX    = allocateTemporaryRegister();
      copySourceRegisterToDestRegister(REGISTER_X, opY);
      copySourceRegisterToDestRegister(regist, opX);

      multiplication[getRegisterDataType(regist)][getRegisterDataType(REGISTER_X)]();
      freeTemporaryRegister(opY);
      freeTemporaryRegister(opX);

      refreshStack();
    }
    else {
      mulError();
    }
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnRecallMult:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Divides X by a register
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallDiv(uint16_t r) {
  calcRegister_t regist = r;

  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    if(division[getRegisterDataType(regist)][getRegisterDataType(REGISTER_X)] != divError) {
      copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

      result = REGISTER_X;
      opY    = allocateTemporaryRegister();
      opX    = allocateTemporaryRegister();
      copySourceRegisterToDestRegister(REGISTER_X, opY);
      copySourceRegisterToDestRegister(regist, opX);

      division[getRegisterDataType(regist)][getRegisterDataType(REGISTER_X)]();
      freeTemporaryRegister(opY);
      freeTemporaryRegister(opX);

      refreshStack();
    }
    else {
      divError();
    }
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnRecallDiv:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Keeps in X min(X, register)
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallMin(uint16_t r) {
  #ifdef PC_BUILD
    showInfoDialog("In function fnRecallMin:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Keeps in X max(X, register)
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallMax(uint16_t r) {
  #ifdef PC_BUILD
    showInfoDialog("In function fnRecallMax:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Recalls a configuration
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallConfig(uint16_t r) {
  #ifdef PC_BUILD
    showInfoDialog("In function fnRecallConfig:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Recalls a stack
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallStack(uint16_t r) {
  #ifdef PC_BUILD
    showInfoDialog("In function fnRecallStack:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Recalls the matrix element I,J in X
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallElement(uint16_t r) {
  #ifdef PC_BUILD
    showInfoDialog("In function fnRecallElement:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Recalls the indexes I and J in X and Y
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallIJ(uint16_t r) {
  #ifdef PC_BUILD
    showInfoDialog("In function fnRecallIJ:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Tests if X is less than an other register
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnXLessThan(uint16_t unusedParamButMandatory) {
  #ifdef PC_BUILD
    showInfoDialog("In function fnXLessThan:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief returns the integer part of the value of a register
 *
 * \param regist calcRegister_t Register
 * \return void
 ***********************************************/
int16_t indirectAddressing(calcRegister_t regist, int16_t minValue, int16_t maxValue) {
  int16_t value;

  if(regist >= FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    displayCalcErrorMessage(8, ERR_REGISTER_LINE, REGISTER_X);
    #ifdef PC_BUILD
      sprintf(errorMessage, "local indirection register .%02d", regist - FIRST_LOCAL_REGISTER);
      showInfoDialog("In function indirectAddressing:", errorMessage, "is not defined!", NULL);
    #endif
    return 9999;
  }

  else if(getRegisterDataType(regist) == dtReal16) {
    if(real16CompareLessThan(REGISTER_REAL16_DATA(regist), const16_0) || real16CompareGreaterEqual(REGISTER_REAL16_DATA(regist), const16_1000)) {
      displayCalcErrorMessage(8, ERR_REGISTER_LINE, REGISTER_X);
      #ifdef PC_BUILD
        real16ToString(REGISTER_REAL16_DATA(regist), errorMessage + 200);
        sprintf(errorMessage, "register %" FMT16S " = %s:", regist, errorMessage + 200);
        showInfoDialog("In function indirectAddressing:", errorMessage, "this value is negative or too big!", NULL);
      #endif
      return 9999;
    }
    value = real16ToInt32(REGISTER_REAL16_DATA(regist));
  }

  else if(getRegisterDataType(regist) == dtReal34) {
    if(real34CompareLessThan(REGISTER_REAL34_DATA(regist), const34_0) || real34CompareGreaterEqual(REGISTER_REAL34_DATA(regist), const34_180)) {
      displayCalcErrorMessage(8, ERR_REGISTER_LINE, REGISTER_X);
      #ifdef PC_BUILD
        real34ToString(REGISTER_REAL34_DATA(regist), errorMessage + 200);
        sprintf(errorMessage, "register %" FMT16S " = %s:", regist, errorMessage + 200);
        showInfoDialog("In function indirectAddressing:", errorMessage, "this value is negative or too big!", NULL);
      #endif
      return 9999;
    }
    value = real34ToInt32(REGISTER_REAL34_DATA(regist));
  }

  else if(getRegisterDataType(regist) == dtLongInteger) {
    longInteger_t longInteger;

    convertLongIntegerRegisterToLongInteger(regist, &longInteger);
    if(longIntegerIsNegative(&longInteger) || longIntegerCompareUInt(&longInteger, 180) == LONG_INTEGER_GREATER_THAN) {
      displayCalcErrorMessage(8, ERR_REGISTER_LINE, REGISTER_X);
      #ifdef PC_BUILD
        longIntegerToString(&longInteger, errorMessage + 200, 10);
        sprintf(errorMessage, "register %" FMT16S " = %s:", regist, errorMessage + 200);
        showInfoDialog("In function indirectAddressing:", errorMessage, "this value is negative or too big!", NULL);
      #endif
      return 9999;
    }
    value = longIntegerToUInt(&longInteger);
  }

  else if(getRegisterDataType(regist) == dtShortInteger) {
    uint64_t val;
    int16_t sign;

    convertShortIntegerRegisterToUInt64(regist, &sign, &val);
    if(sign == 1 || val > 180) {
      displayCalcErrorMessage(8, ERR_REGISTER_LINE, REGISTER_X);
      #ifdef PC_BUILD
        const font_t *font;

        font = &standardFont;
        shortIntegerToDisplayString(regist, errorMessage + 200, &font);
        sprintf(errorMessage, "register %" FMT16S " = %s:", regist, errorMessage + 200);
        showInfoDialog("In function indirectAddressing:", errorMessage, "this value is negative or too big!", NULL);
      #endif
      return 9999;
    }
    value = val;
  }

  else {
    displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
    #ifdef PC_BUILD
      sprintf(errorMessage, "register %" FMT16S " is %s:", regist, getRegisterDataTypeName(regist, true, false));
      showInfoDialog("In function indirectAddressing:", errorMessage, "not suited for indirect addressing!", NULL);
    #endif
    return 9999;
  }

  if(minValue <= value && value <= maxValue) {
    return value;
  }
  else {
    displayCalcErrorMessage(8, ERR_REGISTER_LINE, REGISTER_X);
    #ifdef PC_BUILD
      sprintf(errorMessage, "value = %d! Should be from %d to %d.", value, minValue, maxValue);
      showInfoDialog("In function indirectAddressing:", errorMessage, NULL, NULL);
    #endif
    return 9999;
  }
}



/********************************************//**
 * \brief Prints the content of a register to the console
 *
 * \param r calcRegister_t Register number
 * \return void
 ***********************************************/
void printRegisterToConsole(calcRegister_t regist, int16_t line) {
  char str[1000];

  if(getRegisterDataType(regist) == dtReal16) {
    real16ToString(REGISTER_REAL16_DATA(regist), str);
    #ifdef PC_BUILD
      printf("real16 %s", str);
    #endif

    #ifdef DMCP_BUILD
      sprintf(errorMessage, "real16 %s", str);
      lcd_putsAt(t20, line, errorMessage);
    #endif
  }

  else if(getRegisterDataType(regist) == dtReal34) {
    real34ToString(REGISTER_REAL34_DATA(regist), str);
    #ifdef PC_BUILD
      printf("real34 %s", str);
    #endif

    #ifdef DMCP_BUILD
      sprintf(errorMessage, "real34 %s", str);
      lcd_putsAt(t20, line, errorMessage);
    #endif
  }


  else if(getRegisterDataType(regist) == dtAngle) {
    #if (ANGLE16 == 1)
      real16ToString(REGISTER_REAL16_DATA(regist), str);
      real16_t value;
      real16Copy(REGISTER_REAL16_DATA(regist), &value);
      convertAngle16FromInternal(&value, getRegisterAngularMode(regist));
      real16ToString(&value, str + 200);
    #endif
    #if (ANGLE34 == 1)
      real34ToString(REGISTER_REAL34_DATA(regist), str);
      real34_t value;
      real34Copy(REGISTER_REAL34_DATA(regist), &value);
      convertAngle34FromInternal(&value, getRegisterAngularMode(regist));
      real34ToString(&value, str + 200);
    #endif


    #ifdef PC_BUILD
      printf("angle %s (%s %s)", str, str + 200, getAngularModeName(getRegisterAngularMode(regist)));
    #endif

    #ifdef DMCP_BUILD
      sprintf(errorMessage, "angle %s (%s %s)", str, str + 200, getAngularModeName(getRegisterAngularMode(regist)));
      lcd_putsAt(t20, line, errorMessage);
    #endif
  }

  else if(getRegisterDataType(regist) == dtComplex16) {
    real16ToString(REGISTER_REAL16_DATA(regist), str);
    #ifdef PC_BUILD
      printf("complex16 %s + ", str);
    #endif

    #ifdef DMCP_BUILD
      sprintf(errorMessage, "complex16 %s", str);
      lcd_putsAt(t20, line, errorMessage);
    #endif

    real16ToString(REGISTER_IMAG16_DATA(regist), str);
    #ifdef PC_BUILD
      printf("%si", str);
    #endif

    #ifdef DMCP_BUILD
      sprintf(errorMessage, "%si", str);
      lcd_putsAt(t20, line, errorMessage);
    #endif
  }

  else if(getRegisterDataType(regist) == dtComplex34) {
    real34ToString(REGISTER_REAL34_DATA(regist), str);
    #ifdef PC_BUILD
      printf("complex34 %s + ", str);
    #endif

    #ifdef DMCP_BUILD
      sprintf(errorMessage, "complex34 %s", str);
      lcd_putsAt(t20, line, errorMessage);
    #endif

    real34ToString(REGISTER_IMAG34_DATA(regist), str);
    #ifdef PC_BUILD
      printf("%si", str);
    #endif

    #ifdef DMCP_BUILD
      sprintf(errorMessage, "%si", str);
      lcd_putsAt(t20, line, errorMessage);
    #endif
  }

  else if(getRegisterDataType(regist) == dtString) {
    stringToUtf8(REGISTER_STRING_DATA(regist), (uint8_t *)str);
    #ifdef PC_BUILD
      printf("STR (%" FMT16U ") %s", *(REGISTER_DATA_MAX_LEN(regist)), str);
    #endif

    #ifdef DMCP_BUILD
      sprintf(errorMessage, "STR (%" FMT16U ") %s", *(REGISTER_DATA_MAX_LEN(regist)), str);
      lcd_putsAt(t20, line, errorMessage);
    #endif
  }

  else if(getRegisterDataType(regist) == dtShortInteger) {
    uint64_t value = *(REGISTER_SHORT_INTEGER_DATA(regist));
    #ifdef PC_BUILD
      printf("SI %08x-%08x", (unsigned int)(value>>32), (unsigned int)(value&0xffffffff));
    #endif

    #ifdef DMCP_BUILD
      sprintf(errorMessage, "SI %08x-%08x", (unsigned int)(value>>32), (unsigned int)(value&0xffffffff));
      lcd_putsAt(t20, line, errorMessage);
    #endif
  }

  else if(getRegisterDataType(regist) == dtLongInteger) {
    longInteger_t tmp;

    convertLongIntegerRegisterToLongInteger(regist, &tmp);
    longIntegerToString(&tmp, str, 10);
    #ifdef PC_BUILD
      printf("BI (%" FMT16U ") %s", *(REGISTER_DATA_MAX_LEN(regist)), str);
    #endif

    #ifdef DMCP_BUILD
      sprintf(errorMessage, "BI (%" FMT16U ") %s", *(REGISTER_DATA_MAX_LEN(regist)), str);
      lcd_putsAt(t20, line, errorMessage);
    #endif
  }

  else {
    sprintf(errorMessage, "In printRegisterToConsole: data type %s not supported", getRegisterDataTypeName(regist ,false, false));
    displayBugScreen(errorMessage);
  }
}



void printReal16ToConsole(const real16_t *value) {
  char str[100];

  real16ToString(value, str);
  printf("real16 %s", str);
}



void printComplex16ToConsole(const complex16_t *value) {
  char str[100];

  real16ToString((real16_t *)value, str);
  printf("complex16 %s + ", str);
  real16ToString((real16_t *)value + 1, str);
  printf("%si", str);
}



void printReal34ToConsole(const real34_t *value) {
  char str[100];

  real34ToString(value, str);
  printf("real34 %s", str);
}



void printComplex34ToConsole(const complex34_t *value) {
  char str[100];

  real34ToString((real34_t *)value, str);
  printf("complex34 %s + ", str);
  real34ToString((real34_t *)value + 1, str);
  printf("%si", str);
}



void printReal51ToConsole(const real51_t *value) {
  char str[1000];

  real51ToString(value, str);
  printf("real51 %s", str);
}



void printReal450ToConsole(const real450_t *value) {
  char str[1000];

  real450ToString((decNumber *)value, str);
  printf("real450 %s", str);
}



/********************************************//**
 * \brief Prints the content of a long integer to the console
 *
 * \param r int16_t Register number
 * \return void
 ***********************************************/
void printLongIntegerToConsole(longInteger_t *value) {
  char str[1000];

  longIntegerToString(value, str, 10);
  printf("BI (%d) %s", value->used * SIZEOF_FP_DIGIT, str);
}



void reallocateRegister(calcRegister_t regist, uint32_t dataType, uint32_t dataSize, uint32_t dataInfo) { // dataSize without trailing 0 and without data length
  if(dataType == dtReal16 && dataSize != REAL16_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT32U " is an unexpected numByte value for a real16! It should be REAL16_SIZE=%" FMT32U "!", dataSize, (uint32_t)REAL16_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtReal34 && dataSize != REAL34_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT32U " is an unexpected numByte value for a real34! It should be REAL34_SIZE=%" FMT32U "!", dataSize, (uint32_t)REAL34_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtAngle && dataSize != ANGLE_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT32U " is an unexpected numByte value for an angle! It should be ANGLE_SIZE=%" FMT32U "!", dataSize, (uint32_t)REAL34_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtComplex16 && dataSize != COMPLEX16_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT32U " is an unexpected numByte value for a complex16! It should be COMPLEX16_SIZE=%" FMT32U "!", dataSize, (uint32_t)COMPLEX16_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtComplex34 && dataSize != COMPLEX34_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT32U " is an unexpected numByte value for a complex34! It should be COMPLEX34_SIZE=%" FMT32U "!", dataSize, (uint32_t)COMPLEX34_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtShortInteger && dataSize != SHORT_INTEGER_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT32U " is an unexpected numByte value for an integer! It should be SHORT_INTEGER_SIZE=%" FMT32U "!", dataSize, (uint32_t)SHORT_INTEGER_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtString) {
    dataSize += 3; // +2 For the length of the string and +1 for the trailing 0
    if(dataSize % 2 != 0) {
      dataSize++;   // To be even
    }
  }
  else if(dataType == dtLongInteger) {
    if(dataSize % SIZEOF_FP_DIGIT != 0) {
      sprintf(errorMessage, "In function reallocateRegister: the value of dataSize (%" FMT32U ") for a long integer must be a multiple of %d!", dataSize, SIZEOF_FP_DIGIT);
      displayBugScreen(errorMessage);
      dataSize = ((dataSize / SIZEOF_FP_DIGIT) + 1) * SIZEOF_FP_DIGIT;
    }
    dataSize += 2; // +2 For the length of the data
  }

  if(getRegisterDataType(regist) != dataType || ((getRegisterDataType(regist) == dtString || getRegisterDataType(regist) == dtLongInteger) && getRegisterMaxDataLength(regist)+2 != (uint16_t)dataSize)) {
    freeRegisterData(regist);
    setRegisterDataPointer(regist, allocateMemory(dataSize));
    setRegisterDataType(regist, dataType);
    if(dataType == dtString || dataType == dtLongInteger) {
      setRegisterMaxDataLength(regist, dataSize - 2);
    }
  }

  setRegisterDataInfo(regist, dataInfo);
}


//uint32_t numberOfTemporaryRegistersAllocated = 0;
//uint32_t numberOfTemporaryRegisterAllocations = 0;

calcRegister_t allocateTemporaryRegister(void) {
  //printf("numberOfTemporaryRegisterAllocations = %" FMT32U "\n", ++numberOfTemporaryRegisterAllocations);
  for(calcRegister_t i=0; i<NUMBER_OF_TEMPORARY_REGISTERS; i++) {
    if(!tempRegistersInUse[i]) {
      tempRegistersInUse[i] = true;
      //numberOfTemporaryRegistersAllocated++;
      //printf("     numberOfTemporaryRegistersAllocated = %" FMT32U "\n", numberOfTemporaryRegistersAllocated);
      return FIRST_TEMPORARY_REGISTER + i;
    }
  }

  displayBugScreen("In function allocateTemporaryRegister: there is no more temporary register to allocate! Increase the value of NUMBER_OF_TEMPORARY_REGISTERS in registers.h");
  return FIRST_TEMPORARY_REGISTER;
}



void freeTemporaryRegister(calcRegister_t tempRegister) {
  if(FIRST_TEMPORARY_REGISTER <= tempRegister && tempRegister < FIRST_TEMPORARY_REGISTER + NUMBER_OF_TEMPORARY_REGISTERS) {
    tempRegistersInUse[tempRegister - FIRST_TEMPORARY_REGISTER] = false;
    //numberOfTemporaryRegistersAllocated--;
    //printf("     numberOfTemporaryRegistersAllocated = %" FMT32U "\n", numberOfTemporaryRegistersAllocated);
  }
  else {
    sprintf(errorMessage, "In function freeTemporaryRegister: %" FMT16S " is an unexpected value for tempRegister!\ntempRegister must be from 2000 to %" FMT16S, tempRegister, 1999 + NUMBER_OF_TEMPORARY_REGISTERS);
    displayBugScreen(errorMessage);
  }
}
