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
    return reg[regist].dataType;
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < numberOfLocalRegisters) {
        return POINTER_TO_LOCAL_REGISTER(regist)->dataType;
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
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < numberOfNamedVariables) {
        return POINTER_TO_NAMED_VARIABLE(regist)->dataType;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedVariables - 1);
        showInfoDialog("In function getRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function getRegisterDataType: no named variables defined!");
    }
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      return savedStackRegister[regist - SAVED_REGISTER_X].dataType;
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
void *getRegisterDataPointer(calcRegister_t regist) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    return RAMPTR_TO_MEMPTR(reg[regist].dataPointer);
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < numberOfLocalRegisters) {
        return RAMPTR_TO_MEMPTR(POINTER_TO_LOCAL_REGISTER(regist)->dataPointer);
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
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < numberOfNamedVariables) {
        return RAMPTR_TO_MEMPTR(POINTER_TO_NAMED_VARIABLE(regist)->dataPointer);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedVariables - 1);
        showInfoDialog("In function getRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function getRegisterDataPointer: no named variables defined!");
    }
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      return RAMPTR_TO_MEMPTR(savedStackRegister[regist - SAVED_REGISTER_X].dataPointer);
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
uint32_t getRegisterTag(calcRegister_t regist) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    return reg[regist].tag;
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < numberOfLocalRegisters) {
        return POINTER_TO_LOCAL_REGISTER(regist)->tag;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfLocalRegisters - 1);
        showInfoDialog("In function getRegisterTag:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
     showInfoDialog("In function getRegisterTag:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < numberOfNamedVariables) {
        return POINTER_TO_NAMED_VARIABLE(regist)->tag;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedVariables - 1);
        showInfoDialog("In function getRegisterTag:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function getRegisterTag: no named variables defined!");
    }
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      return savedStackRegister[regist - SAVED_REGISTER_X].tag;
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "saved stack register %" FMT16S, regist - SAVED_REGISTER_X);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to 7");
      showInfoDialog("In function getRegisterTag:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else {
    sprintf(errorMessage, "In function getRegisterTag: r=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
  return 0;
}



/********************************************//**
 * \brief returns the length of the memory area of the name of the named variable
 *
 * \param[in] r calcRegister_t Register number
 * \return uint32_t      Length
 ***********************************************/
uint32_t getRegisterNameLength(calcRegister_t regist) {
  if(1000 <= regist && regist <= 1999) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < numberOfNamedVariables) {
        return POINTER_TO_NAMED_VARIABLE(regist)->variableNameLen;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedVariables - 1);
        showInfoDialog("In function getRegisterNameLength:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
     #endif
    }
    else {
      displayBugScreen("In function getRegisterNameLength: no named variables defined!");
    }
  }
  else {
    displayBugScreen("In function getRegisterNameLength: this function can be called only for a named variable!");
  }
  return 0;
}



/********************************************//**
 * \brief returns the pointer to the memory area of the name of the named variable
 *
 * \param[in] r calcRegister_t Register number
 * \return uint32_t      Pointer to the name
 ***********************************************/
char *getRegisterNamePointer(calcRegister_t regist) {
  if(1000 <= regist && regist <= 1999) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < numberOfNamedVariables) {
        return ram + (((uint32_t)(*POINTER_TO_POINTER_TO_NAMED_VARIABLE_NAME(regist))) << 1);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedVariables - 1);
        showInfoDialog("In function getRegisterNamePointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function getRegisterNamePointer: no named variables defined!");
    }
   }
  else {
    displayBugScreen("In function getRegisterNamePointer: this function can be called only for a named variable!");
  }
  return 0;
}



/********************************************//**
 * \brief Sets the data type of a register
 *
 * \param[in] r calcRegister_t        Register number
 * \param[in] dataType uint16_t Data type
 * \param[in] tag      uint16_t Tag
 * \return void
 ***********************************************/
void setRegisterDataType(calcRegister_t regist, uint16_t dataType, uint32_t tag) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    reg[regist].dataType = dataType;
    reg[regist].tag = tag;
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < numberOfLocalRegisters) {
        POINTER_TO_LOCAL_REGISTER(regist)->dataType = dataType;
        POINTER_TO_LOCAL_REGISTER(regist)->tag = tag;
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
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < numberOfNamedVariables) {
        POINTER_TO_NAMED_VARIABLE(regist)->dataType = dataType;
        POINTER_TO_NAMED_VARIABLE(regist)->tag = tag;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedVariables - 1);
        showInfoDialog("In function setRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function setRegisterDataType: no named variables defined!");
    }
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      savedStackRegister[regist - SAVED_REGISTER_X].dataType = dataType;
      savedStackRegister[regist - SAVED_REGISTER_X].tag = tag;
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
void setRegisterDataPointer(calcRegister_t regist, void *memPtr) {
  uint32_t dataPointer = MEMPTR_TO_RAMPTR(memPtr);

  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    reg[regist].dataPointer = dataPointer;
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < numberOfLocalRegisters) {
        POINTER_TO_LOCAL_REGISTER(regist)->dataPointer = dataPointer;
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
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= 1000u;
      if(regist < numberOfNamedVariables) {
        POINTER_TO_NAMED_VARIABLE(regist)->dataPointer = dataPointer;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedVariables - 1);
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
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      savedStackRegister[regist - SAVED_REGISTER_X].dataPointer = dataPointer;
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
void setRegisterTag(calcRegister_t regist, uint32_t tag) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    reg[regist].tag = tag;
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < numberOfLocalRegisters) {
        POINTER_TO_LOCAL_REGISTER(regist)->tag = tag;
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
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= 1000u;
      if(regist < numberOfNamedVariables) {
        POINTER_TO_NAMED_VARIABLE(regist)->tag = tag;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedVariables - 1);
        showInfoDialog("In function setRegisterDataInfo:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function setRegisterDataInfo: no named registers defined!");
    }
   }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      savedStackRegister[regist - SAVED_REGISTER_X].tag = tag;
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
 * \brief Sets the length of the memory area of the name of the named variable
 *
 * \param[in] r calcRegister_t        Register number
 * \param[in] dataType uint16_t Length
 * \return void
 ***********************************************/
void setRegisterNameLength(calcRegister_t regist, uint16_t length) {
  if(1000 <= regist && regist <= 1999) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < numberOfNamedVariables) {
        POINTER_TO_NAMED_VARIABLE(regist)->variableNameLen = length;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, numberOfNamedVariables - 1);
        showInfoDialog("In function setRegisterNameLength:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function setRegisterNameLength: no named registers defined!");
    }
  }
  else {
    displayBugScreen("In function setRegisterNameLength: this function can be called only for a named variable!");
  }
}



/********************************************//**
 * \brief Sets the pointer to the memory area of the name of the named variable
 *
 * \param[in] r calcRegister_t        Register number
 * \param[in] dataType uint16_t Pointer
 * \return void
 ***********************************************/
void setRegisterNamePointer(calcRegister_t regist, void *namePointer) {
  if(1000 <= regist && regist <= 1999) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < numberOfNamedVariables) {
        *POINTER_TO_POINTER_TO_NAMED_VARIABLE_NAME(regist) = MEMPTR_TO_RAMPTR(namePointer) >> 1;
      }
      else {
        sprintf(errorMessage, "In function setRegisterNamePointer: named variable %" FMT16S " is not defined! Must be from 0 to %" FMT16U, regist, numberOfNamedVariables - 1);
        displayBugScreen(errorMessage);
      }
    }
    else {
      displayBugScreen("In function setRegisterNamePointer: no named variables defined!");
    }
  }
  else {
    displayBugScreen("In function setRegisterNamePointer: this function can be called only for a named variable!");
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
      freeWp43s(allLocalRegisterPointer, 2 + 4*numRegs);

      allLocalRegisterPointer = NULL;
    }

    else if(numberOfLocalRegisters == 0) { // Allocate memory
      allLocalRegisterPointer = allocWp43s(2u + 4u*numberOfRegistersToAllocate);
      numberOfLocalRegisters = numberOfRegistersToAllocate;
      numberOfLocalFlags = 16;

      // clear all local flags
      *POINTER_TO_LOCAL_FLAGS = 0;

      // All the new local registers are real16s initialized to 0.0
      for(r=FIRST_LOCAL_REGISTER; r<FIRST_LOCAL_REGISTER+numberOfRegistersToAllocate; r++) {
        setRegisterDataType(r, dtReal16, TAG_NONE);
        setRegisterDataPointer(r, allocWp43s(REAL16_SIZE));
        real16Zero(REGISTER_REAL16_DATA(r));
      }
    }

    else if(numberOfRegistersToAllocate > numberOfLocalRegisters) { // increase the number of local register
      reallocWp43s(allLocalRegisterPointer,  2u + 4u*numberOfLocalRegisters, 2u + 4u*numberOfRegistersToAllocate);

      uint16_t oldNumRegs = numberOfLocalRegisters;
      numberOfLocalRegisters = numberOfRegistersToAllocate;

      // All the new local registers are real16s initialized to 0.0
      for(r=FIRST_LOCAL_REGISTER+oldNumRegs; r<FIRST_LOCAL_REGISTER+numberOfRegistersToAllocate; r++) {
        setRegisterDataType(r, dtReal16, TAG_NONE);
        setRegisterDataPointer(r, allocWp43s(REAL16_SIZE));
        real16Zero(REGISTER_REAL16_DATA(r));
      }
    }

    else { // reduce the number of local register
      // free memory allocated to the data of the deleted local registers
      for(r=numberOfLocalRegisters - 1; r>=numberOfRegistersToAllocate; r--) {
        freeRegisterData(FIRST_LOCAL_REGISTER + r);
      }

      // free memory allocated to the deleted local registers
      freeWp43s(allLocalRegisterPointer + 2u + 4*numberOfRegistersToAllocate, 4u*(numberOfLocalRegisters - numberOfRegistersToAllocate));
      numberOfLocalRegisters = numberOfRegistersToAllocate; // This must be done after the freeMemory call
    }
  }
}



/********************************************//**
 * \brief Allocates 1 named variable.
 *
 * \param[in] variableName const char* Register name
 * \return void
 ***********************************************/
void allocateNamedVariable(const char *variableName) {
  uint32_t len;
  calcRegister_t regist;

  if(stringGlyphLength(variableName) < 1 || stringGlyphLength(variableName) > 7) {
    #ifdef PC_BUILD
      sprintf(errorMessage, "the name %s", variableName);
      showInfoDialog("In function allocateNamedVariable:", errorMessage, "is incorrect! The length must be", "from 1 to 7 glyphs!");
    #endif
    return;
  }

  if(numberOfNamedVariables == 0) { // First named variable
    allNamedVariablePointer = allocWp43s(6);
    numberOfNamedVariables = 1;

    regist = 1000;
  }
  else {
    regist = numberOfNamedVariables;
    if(regist == 999u) {
      #ifdef PC_BUILD
        showInfoDialog("In function allocateNamedVariable:", "you can allocate up to", "999 named variables!", NULL);
      #endif
      return;
    }

    reallocWp43s(allNamedVariablePointer, 6u*numberOfNamedVariables, 6u*(numberOfNamedVariables + 1));
    numberOfNamedVariables++;

    regist += 1000;
  }

  // The new named variable is a real16 initialized to 0.0
  setRegisterDataType(regist, dtReal16, TAG_NONE);

  len = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(stringByteLength(variableName) + 1)); // +1 for the trailing zero

  setRegisterNamePointer(regist, allocWp43s(len));
  setRegisterNameLength(regist, len>>1);
  memcpy(getRegisterNamePointer(regist), variableName, len);

  setRegisterDataPointer(regist, allocWp43s(REAL16_SIZE));
  real16Zero(REGISTER_REAL16_DATA(regist));
}



/********************************************//**
 * \brief Sets the max length of string
 *
 * \param[in] r calcRegister_t            Register number
 * \param[in] maxStringLen uint16_t Max length of the string
 * \return void
 ***********************************************/
void setRegisterMaxDataLength(calcRegister_t regist, uint32_t maxDataLen) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    *(dataSize_t *)RAMPTR_TO_MEMPTR(reg[regist].dataPointer) = maxDataLen;
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      if(regist-FIRST_LOCAL_REGISTER < numberOfLocalRegisters) {
        *(dataSize_t *)REGISTER_DATA(regist) = maxDataLen;
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
  else if(regist < SAVED_REGISTER_X) { // Named register
    if(numberOfNamedVariables > 0) {
      if(regist-1000 < numberOfNamedVariables) {
        *(dataSize_t *)REGISTER_DATA(regist) = maxDataLen;
      }
      else {
        sprintf(errorMessage, "In function setRegisterMaxDataLength: named register %" FMT16S " is not defined! Must be from 0 to %" FMT16U, regist - 1000, numberOfNamedVariables - 1);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function setRegisterMaxDataLength:", "no named registers defined!", NULL, NULL);
    }
    #endif
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      *(dataSize_t *)REGISTER_DATA(regist) = maxDataLen;
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
uint32_t getRegisterMaxDataLength(calcRegister_t regist) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    return *(dataSize_t *)RAMPTR_TO_MEMPTR(reg[regist].dataPointer);
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      if(regist-FIRST_LOCAL_REGISTER < numberOfLocalRegisters) {
        return *(dataSize_t *)REGISTER_DATA(regist);
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
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(numberOfNamedVariables > 0) {
      if(regist-1000 < numberOfNamedVariables) {
        return *(dataSize_t *)REGISTER_DATA(regist);
      }
      else {
        sprintf(errorMessage, "In function getRegisterMaxDataLength: named variable %" FMT16S " is not defined! Must be from 0 to %" FMT16U, regist - 1000, numberOfNamedVariables - 1);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function getRegisterMaxStringLength:", "no local registers defined!", NULL, NULL);
    }
    #endif
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      return *(dataSize_t *)REGISTER_DATA(regist);
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
  switch(getRegisterDataType(regist)) {
    case dtLongInteger:  return *(REGISTER_DATA_MAX_LEN(regist)) + sizeof(dataSize_t);
    case dtReal16:       return REAL16_SIZE;
    case dtComplex16:    return COMPLEX16_SIZE;
    case dtAngle16:      return REAL16_SIZE;
    //case dtTime:
    //case dtDate:
    case dtString:       return *(REGISTER_DATA_MAX_LEN(regist)) + sizeof(dataSize_t);
    //case dtReal16Matrix:
    //case dtComplex16Matrix:
    case dtShortInteger: return SHORT_INTEGER_SIZE;
    case dtReal34:       return REAL34_SIZE;
    case dtComplex34:    return COMPLEX34_SIZE;
    case dtAngle34:      return REAL34_SIZE;

    default:
      sprintf(errorMessage, "In function getRegisterFullSize: data type %s is unknown!", getDataTypeName(getRegisterDataType(regist), true, false));
      displayBugScreen(errorMessage);
      return 0;
  }
}



/********************************************//**
 * \brief Clears a register, that is, sets it to 0,0 real16
 *
 * \param[in] r calcRegister_t Register number
 * \return void
 ***********************************************/
void clearRegister(calcRegister_t regist) {
  if(getRegisterDataType(regist) != dtReal16 || getRegisterDataType(regist) != dtAngle16) {
    reallocateRegister(regist, dtReal16, REAL16_SIZE, TAG_NONE);
    real16Zero(REGISTER_REAL16_DATA(regist));
  }
  else{
    real16Zero(REGISTER_REAL16_DATA(regist));
    setRegisterTag(regist, TAG_NONE);
  }
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

  longIntegerInit(locR);
  uIntToLongInteger(numberOfLocalRegisters, locR);
  convertLongIntegerToLongIntegerRegister(locR, REGISTER_X);
  longIntegerFree(locR);

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
  if(getRegisterDataType(REGISTER_X) == dtReal34 || getRegisterDataType(REGISTER_X) == dtComplex34 || getRegisterDataType(REGISTER_X) == dtAngle34) {
    convertRegister34To16(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) != dtReal16 && getRegisterDataType(REGISTER_X) != dtComplex16) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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
  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtComplex16 || getRegisterDataType(REGISTER_X) == dtAngle16) {
    convertRegister16To34(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) != dtReal34 && getRegisterDataType(REGISTER_X) != dtComplex34) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot convert %s to a DP!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnConvertXToReal34:", errorMessage, NULL, NULL);
    #endif
  }

  refreshRegisterLine(REGISTER_X);
}



void adjustResult(calcRegister_t res, bool_t dropY, bool_t setCpxRes, calcRegister_t op1, calcRegister_t op2, calcRegister_t op3) {
  uint32_t resultDataType;
  bool_t oneArgumentIsComplex = false;

  if(op1 >= 0) {
    oneArgumentIsComplex = oneArgumentIsComplex || getRegisterDataType(op1) == dtComplex16 || getRegisterDataType(op1) == dtComplex34;
  }

  if(op2 >= 0) {
    oneArgumentIsComplex = oneArgumentIsComplex || getRegisterDataType(op2) == dtComplex16 || getRegisterDataType(op2) == dtComplex34;
  }

  if(op3 >= 0) {
    oneArgumentIsComplex = oneArgumentIsComplex || getRegisterDataType(op3) == dtComplex16 || getRegisterDataType(op3) == dtComplex34;
  }

  if(lastErrorCode != 0) {
    restoreStack();
    refreshStack();
    return;
  }

  resultDataType = getRegisterDataType(res);
  if(getFlag(FLAG_DANGER) == false) {
    // D is clear: test infinite values
    switch(resultDataType) {
      case dtReal16:
      case dtAngle16:
        if(real16IsInfinite(REGISTER_REAL16_DATA(res))) {
          displayCalcErrorMessage(real16IsPositive(REGISTER_REAL16_DATA(res)) ? ERROR_OVERFLOW_PLUS_INF : ERROR_OVERFLOW_MINUS_INF , ERR_REGISTER_LINE, res);
        }
        break;

      case dtReal34:
      case dtAngle34:
        if(real34IsInfinite(REGISTER_REAL34_DATA(res))) {
          displayCalcErrorMessage(real34IsPositive(REGISTER_REAL34_DATA(res)) ? ERROR_OVERFLOW_PLUS_INF : ERROR_OVERFLOW_MINUS_INF , ERR_REGISTER_LINE, res);
        }
        break;

      case dtComplex16:
        if(real16IsInfinite(REGISTER_REAL16_DATA(res))) {
          displayCalcErrorMessage(real16IsPositive(REGISTER_REAL16_DATA(res)) ? ERROR_OVERFLOW_PLUS_INF : ERROR_OVERFLOW_MINUS_INF , ERR_REGISTER_LINE, res);
        }
        else if(real16IsInfinite(REGISTER_IMAG16_DATA(res))) {
          displayCalcErrorMessage(real16IsPositive(REGISTER_IMAG16_DATA(res)) ? ERROR_OVERFLOW_PLUS_INF : ERROR_OVERFLOW_MINUS_INF , ERR_REGISTER_LINE, res);
        }
        break;

      case dtComplex34:
        if(real34IsInfinite(REGISTER_REAL34_DATA(res))) {
          displayCalcErrorMessage(real34IsPositive(REGISTER_REAL34_DATA(res)) ? ERROR_OVERFLOW_PLUS_INF : ERROR_OVERFLOW_MINUS_INF , ERR_REGISTER_LINE, res);
        }
        else if(real34IsInfinite(REGISTER_IMAG34_DATA(res))) {
          displayCalcErrorMessage(real34IsPositive(REGISTER_IMAG34_DATA(res)) ? ERROR_OVERFLOW_PLUS_INF : ERROR_OVERFLOW_MINUS_INF , ERR_REGISTER_LINE, res);
        }
        break;

      default:
        break;
    }
  }

  if(lastErrorCode != 0) {
    restoreStack();
    refreshStack();
    return;
  }

  if(setCpxRes && oneArgumentIsComplex && resultDataType != dtString) {
    fnSetFlag(FLAG_CPXRES);
  }

  // Round the register value
  switch(resultDataType) {
    realIc_t tmp;

    case dtReal16:
    case dtAngle16:
      if(real16IsZero(REGISTER_REAL16_DATA(res))) {
        real16SetPositiveSign(REGISTER_REAL16_DATA(res));
      }

      if(significantDigits == 0 || significantDigits >= 16) {
        break;
      }

      ctxtRealIc.digits = significantDigits;
      real16ToRealIc(REGISTER_REAL16_DATA(res), &tmp);
      ctxtRealIc.digits = DIGITS_FOR_34_DIGITS_INTERMEDIATE_CALCULATIONS;
      realIcToReal16(&tmp, REGISTER_REAL16_DATA(res));
      break;

    case dtReal34:
    case dtAngle34:
      if(real34IsZero(REGISTER_REAL34_DATA(res))) {
        real34SetPositiveSign(REGISTER_REAL34_DATA(res));
      }

      if(significantDigits == 0 || significantDigits >= 34) {
        break;
      }

      ctxtRealIc.digits = significantDigits;
      real34ToRealIc(REGISTER_REAL34_DATA(res), &tmp);
      ctxtRealIc.digits = DIGITS_FOR_34_DIGITS_INTERMEDIATE_CALCULATIONS;
      realIcToReal34(&tmp, REGISTER_REAL34_DATA(res));
      break;

    case dtComplex16:
      if(real16IsZero(REGISTER_REAL16_DATA(res))) {
        real16SetPositiveSign(REGISTER_REAL16_DATA(res));
      }

      if(real16IsZero(REGISTER_IMAG16_DATA(res))) {
        real16SetPositiveSign(REGISTER_IMAG16_DATA(res));
      }

      if(significantDigits == 0 || significantDigits >= 16) {
        break;
      }

      ctxtRealIc.digits = significantDigits;
      real16ToRealIc(REGISTER_REAL16_DATA(res), &tmp);
      realIcToReal16(&tmp, REGISTER_REAL16_DATA(res));
      real16ToRealIc(REGISTER_IMAG16_DATA(res), &tmp);
      realIcToReal16(&tmp, REGISTER_IMAG16_DATA(res));
      ctxtRealIc.digits = DIGITS_FOR_34_DIGITS_INTERMEDIATE_CALCULATIONS;
      break;

    case dtComplex34:
      if(real34IsZero(REGISTER_REAL34_DATA(res))) {
        real34SetPositiveSign(REGISTER_REAL34_DATA(res));
      }

      if(real34IsZero(REGISTER_IMAG34_DATA(res))) {
        real34SetPositiveSign(REGISTER_IMAG34_DATA(res));
      }

      if(significantDigits == 0 || significantDigits >= 34) {
        break;
      }

      ctxtRealIc.digits = significantDigits;
      real34ToRealIc(REGISTER_REAL34_DATA(res), &tmp);
      realIcToReal34(&tmp, REGISTER_REAL34_DATA(res));
      real34ToRealIc(REGISTER_IMAG34_DATA(res), &tmp);
      realIcToReal34(&tmp, REGISTER_IMAG34_DATA(res));
      ctxtRealIc.digits = DIGITS_FOR_34_DIGITS_INTERMEDIATE_CALCULATIONS;
      break;

    default:
      break;
  }

  if(dropY) {
    fnDropY(NOPARAM);
    refreshStack();
  }
  else {
    refreshRegisterLine(res);
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
  if(   getRegisterDataType(destRegister) != getRegisterDataType(sourceRegister)
     || getRegisterFullSize(destRegister) != getRegisterFullSize(sourceRegister)
     || getRegisterTag(destRegister)      != getRegisterTag(sourceRegister     )) {
    uint32_t size;

    switch(getRegisterDataType(sourceRegister)) {
      case dtLongInteger:  size = *(REGISTER_DATA_MAX_LEN(sourceRegister));     break;
      case dtReal16:       size = REAL16_SIZE;                                  break;
      case dtComplex16:    size = COMPLEX16_SIZE;                               break;
      case dtAngle16:      size = REAL16_SIZE;                                  break;
      //case dtTime:
      //case dtDate:
      case dtString:       size = *(REGISTER_DATA_MAX_LEN(sourceRegister)) - 1; break;
      //case dtReal16Matrix:
      //case dtComplex16Matrix:
      case dtShortInteger: size = SHORT_INTEGER_SIZE;                           break;
      case dtReal34:       size = REAL34_SIZE;                                  break;
      case dtComplex34:    size = COMPLEX34_SIZE;                               break;
      case dtAngle34:      size = REAL34_SIZE;                                  break;

      default:
        sprintf(errorMessage, "In function copySourceRegisterToDestRegister: data type %s is unknown!", getDataTypeName(getRegisterDataType(sourceRegister), true, false));
        displayBugScreen(errorMessage);
        size = 0;
    }

    reallocateRegister(destRegister, getRegisterDataType(sourceRegister), size, getRegisterTag(sourceRegister));
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
void fnStoreAdd(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);

    addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
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
void fnStoreSub(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);

    subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
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
void fnStoreMult(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);

    multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
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
void fnStoreDiv(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);

    division[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
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
void fnRecall(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    if(REGISTER_X <= regist && regist <= getStackTop()) {
      copySourceRegisterToDestRegister(regist, TEMP_REGISTER);
      liftStack();
      copySourceRegisterToDestRegister(TEMP_REGISTER, REGISTER_X);
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
void fnRecallAdd(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_Y);
    copySourceRegisterToDestRegister(regist, REGISTER_X);

    addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
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
void fnRecallSub(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_Y);
    copySourceRegisterToDestRegister(regist, REGISTER_X);

    subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
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
void fnRecallMult(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_Y);
    copySourceRegisterToDestRegister(regist, REGISTER_X);

    multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
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
void fnRecallDiv(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_Y);
    copySourceRegisterToDestRegister(regist, REGISTER_X);

    division[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
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
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #ifdef PC_BUILD
      sprintf(errorMessage, "local indirection register .%02d", regist - FIRST_LOCAL_REGISTER);
      showInfoDialog("In function indirectAddressing:", errorMessage, "is not defined!", NULL);
    #endif
    return 9999;
  }

  else if(getRegisterDataType(regist) == dtReal16) {
    if(real16CompareLessThan(REGISTER_REAL16_DATA(regist), const16_0) || real16CompareGreaterEqual(REGISTER_REAL16_DATA(regist), const16_1000)) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
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
    if(real34CompareLessThan(REGISTER_REAL34_DATA(regist), const34_0) || real34CompareGreaterEqual(REGISTER_REAL34_DATA(regist), const34_1000)) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
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
    longInteger_t lgInt;

    convertLongIntegerRegisterToLongInteger(regist, lgInt);
    if(longIntegerIsNegative(lgInt) || longIntegerCompareUInt(lgInt, 180) > 0) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #ifdef PC_BUILD
        longIntegerToAllocatedString(lgInt, errorMessage + 200);
        sprintf(errorMessage, "register %" FMT16S " = %s:", regist, errorMessage + 200);
        showInfoDialog("In function indirectAddressing:", errorMessage, "this value is negative or too big!", NULL);
      #endif
      longIntegerFree(lgInt);
      return 9999;
    }
    value = longIntegerToUInt(lgInt);
    longIntegerFree(lgInt);
  }

  else if(getRegisterDataType(regist) == dtShortInteger) {
    uint64_t val;
    int16_t sign;

    convertShortIntegerRegisterToUInt64(regist, &sign, &val);
    if(sign == 1 || val > 180) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
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
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
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
#ifdef DMCP_BUILD
void printRegisterToConsole(calcRegister_t regist, int16_t line) {
#else
void printRegisterToConsole(calcRegister_t regist) {
#endif
  char str[1000];

  if(getRegisterDataType(regist) == dtReal16) {
    real16ToString(REGISTER_REAL16_DATA(regist), str);
    #if defined(PC_BUILD) || defined(TESTSUITE_BUILD)
      printf("real16 %s", str);
    #endif

    #ifdef DMCP_BUILD
      sprintf(tmpStr3000, "real16 %s", str);
      lcd_putsAt(t20, line, tmpStr3000);
    #endif
  }

  else if(getRegisterDataType(regist) == dtAngle16) {
    real16ToString(REGISTER_REAL16_DATA(regist), str);
    #if defined(PC_BUILD) || defined(TESTSUITE_BUILD)
      printf("angle16 %s %s", str, getAngularModeName(getRegisterAngularMode(regist)));
    #endif

    #ifdef DMCP_BUILD
      sprintf(tmpStr3000, "angle16 %s %s", str, getAngularModeName(getRegisterAngularMode(regist)));
      lcd_putsAt(t20, line, tmpStr3000);
    #endif
  }

  else if(getRegisterDataType(regist) == dtReal34) {
    real34ToString(REGISTER_REAL34_DATA(regist), str);
    #if defined(PC_BUILD) || defined(TESTSUITE_BUILD)
      printf("real34 %s", str);
    #endif

    #ifdef DMCP_BUILD
      sprintf(tmpStr3000, "real34 %s", str);
      lcd_putsAt(t20, line, tmpStr3000);
    #endif
  }

  else if(getRegisterDataType(regist) == dtAngle34) {
    real34ToString(REGISTER_REAL34_DATA(regist), str);
    #if defined(PC_BUILD) || defined(TESTSUITE_BUILD)
      printf("real34 %s %s", str, getAngularModeName(getRegisterAngularMode(regist)));
    #endif

    #ifdef DMCP_BUILD
      sprintf(tmpStr3000, "real34 %s %s", str, getAngularModeName(getRegisterAngularMode(regist)));
      lcd_putsAt(t20, line, tmpStr3000);
    #endif
  }

  else if(getRegisterDataType(regist) == dtComplex16) {
    real16ToString(REGISTER_REAL16_DATA(regist), str);
    #if defined(PC_BUILD) || defined(TESTSUITE_BUILD)
      printf("complex16 %s ", str);
    #endif

    #ifdef DMCP_BUILD
      sprintf(tmpStr3000, "complex16 %s", str);
      lcd_putsAt(t20, line, tmpStr3000);
    #endif

    real16ToString(REGISTER_IMAG16_DATA(regist), str);
    #if defined(PC_BUILD) || defined(TESTSUITE_BUILD)
      if(real16IsNegative(REGISTER_IMAG16_DATA(regist))) {
        printf("- ix%s", str + 1);
      }
      else {
        printf("+ ix%s", str);
      }
    #endif

    #ifdef DMCP_BUILD
      if(real16IsNegative(REGISTER_IMAG16_DATA(regist))) {
        sprintf(tmpStr3000, "-ix%s", str + 1);
      }
      else {
        sprintf(tmpStr3000, "+ix%s", str);
      }
      lcd_putsAt(t20, line, tmpStr3000);
    #endif
  }

  else if(getRegisterDataType(regist) == dtComplex34) {
    real34ToString(REGISTER_REAL34_DATA(regist), str);
    #if defined(PC_BUILD) || defined(TESTSUITE_BUILD)
      printf("complex34 %s ", str);
    #endif

    #ifdef DMCP_BUILD
      sprintf(tmpStr3000, "complex34 %s", str);
      lcd_putsAt(t20, line, tmpStr3000);
    #endif

    real34ToString(REGISTER_IMAG34_DATA(regist), str);
    #if defined(PC_BUILD) || defined(TESTSUITE_BUILD)
      if(real34IsNegative(REGISTER_IMAG34_DATA(regist))) {
        printf("- ix%s", str + 1);
      }
      else {
        printf("+ ix%s", str);
      }
    #endif

    #ifdef DMCP_BUILD
      if(real34IsNegative(REGISTER_IMAG34_DATA(regist))) {
        sprintf(tmpStr3000, "-ix%s", str + 1);
      }
      else {
        sprintf(tmpStr3000, "+ix%s", str);
      }
      lcd_putsAt(t20, line, tmpStr3000);
    #endif
  }

  else if(getRegisterDataType(regist) == dtString) {
    stringToUtf8(REGISTER_STRING_DATA(regist), (uint8_t *)str);
    #if defined(PC_BUILD) || defined(TESTSUITE_BUILD)
      printf("string (%" FMT32U " bytes) |%s|", (uint32_t)*(REGISTER_DATA_MAX_LEN(regist)), str);
    #endif

    #ifdef DMCP_BUILD
      sprintf(tmpStr3000, "STR (%" FMT32U ") %s", (uint32_t)*(REGISTER_DATA_MAX_LEN(regist)), str);
      lcd_putsAt(t20, line, tmpStr3000);
    #endif
  }

  else if(getRegisterDataType(regist) == dtShortInteger) {
    uint64_t value = *(REGISTER_SHORT_INTEGER_DATA(regist));

    #if defined(PC_BUILD) || defined(TESTSUITE_BUILD)
      printf("short integer %08x-%08x (base %" FMT32U ")", (unsigned int)(value>>32), (unsigned int)(value&0xffffffff), getRegisterTag(regist));
    #endif

    #ifdef DMCP_BUILD
      sprintf(tmpStr3000, "SI %08x-%08x (base %" FMT32U ")", (unsigned int)(value>>32), (unsigned int)(value&0xffffffff), getRegisterTag(regist));
      lcd_putsAt(t20, line, tmpStr3000);
    #endif
  }

  else if(getRegisterDataType(regist) == dtLongInteger) {
    longInteger_t lgInt;

    convertLongIntegerRegisterToLongInteger(regist, lgInt);
    longIntegerToAllocatedString(lgInt, str);
    longIntegerFree(lgInt);
    #if defined(PC_BUILD) || defined(TESTSUITE_BUILD)
      printf("long integer (%" FMT32U " bytes) %s", (uint32_t)*(REGISTER_DATA_MAX_LEN(regist)), str);
    #endif

    #ifdef DMCP_BUILD
      sprintf(tmpStr3000, "BI (%" FMT32U ") %s", (uint32_t)*(REGISTER_DATA_MAX_LEN(regist)), str);
      lcd_putsAt(t20, line, tmpStr3000);
    #endif
  }

  else {
    sprintf(errorMessage, "In printRegisterToConsole: data type %s not supported", getRegisterDataTypeName(regist ,false, false));
    displayBugScreen(errorMessage);
  }
}



#ifdef TESTSUITE_BUILD
/********************************************//**
 * \brief Prints the content of a register to a string
 *
 * \param r calcRegister_t Register number
 * \return void
 ***********************************************/
void printRegisterToString(calcRegister_t regist, char *registerContent) {
  char str[1000];

  if(getRegisterDataType(regist) == dtReal16) {
    real16ToString(REGISTER_REAL16_DATA(regist), str);
    sprintf(registerContent, "real16 %s", str);
  }

  else if(getRegisterDataType(regist) == dtAngle16) {
    real16ToString(REGISTER_REAL16_DATA(regist), str);
    sprintf(registerContent, "real16 %s %s", str, getAngularModeName(getRegisterAngularMode(regist)));
  }

  else if(getRegisterDataType(regist) == dtReal34) {
    real34ToString(REGISTER_REAL34_DATA(regist), str);
    sprintf(registerContent, "real34 %s", str);
  }

  else if(getRegisterDataType(regist) == dtAngle34) {
    real34ToString(REGISTER_REAL34_DATA(regist), str);
    sprintf(registerContent, "real34 %s %s", str, getAngularModeName(getRegisterAngularMode(regist)));
  }

  else if(getRegisterDataType(regist) == dtComplex16) {
    real16ToString(REGISTER_REAL16_DATA(regist), str);
    sprintf(registerContent, "complex16 %s ", str);

    real16ToString(REGISTER_IMAG16_DATA(regist), str);
    if(real16IsNegative(REGISTER_IMAG16_DATA(regist))) {
      strcat(registerContent, "- ix");
      strcat(registerContent, str + 1);
    }
    else {
      strcat(registerContent, "+ ix");
      strcat(registerContent, str);
    }
  }

  else if(getRegisterDataType(regist) == dtComplex34) {
    real34ToString(REGISTER_REAL34_DATA(regist), str);
    sprintf(registerContent, "complex34 %s ", str);

    real34ToString(REGISTER_IMAG34_DATA(regist), str);
    if(real34IsNegative(REGISTER_IMAG34_DATA(regist))) {
      strcat(registerContent, "- ix");
      strcat(registerContent, str + 1);
    }
    else {
      strcat(registerContent, "+ ix");
      strcat(registerContent, str);
    }
  }

  else if(getRegisterDataType(regist) == dtString) {
    stringToUtf8(REGISTER_STRING_DATA(regist), (uint8_t *)str);
    sprintf(registerContent, "string (%" FMT32U " bytes) |%s|", *(REGISTER_DATA_MAX_LEN(regist)), str);
  }

  else if(getRegisterDataType(regist) == dtShortInteger) {
    uint64_t value = *(REGISTER_SHORT_INTEGER_DATA(regist));

    sprintf(registerContent, "short integer %08x-%08x (base %u)", (unsigned int)(value>>32), (unsigned int)(value&0xffffffff), getRegisterTag(regist));
  }

  else if(getRegisterDataType(regist) == dtLongInteger) {
    longInteger_t lgInt;
    char *lgIntStr;

    convertLongIntegerRegisterToLongInteger(regist, lgInt);
    lgIntStr = longIntegerToString(lgInt, 10);
    longIntegerFree(lgInt);
    sprintf(registerContent, "long integer (%" FMT32U " bytes) %s", *(REGISTER_DATA_MAX_LEN(regist)), lgIntStr);

    freeGmp(lgIntStr, strlen(lgIntStr) + 1);
  }

  else {
    sprintf(registerContent, "In printRegisterToString: data type %s not supported", getRegisterDataTypeName(regist ,false, false));
  }
}
#endif // TESTSUITE_BUILD



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



void printRealIcToConsole(const realIc_t *value) {
  char str[1000];

  realIcToString(value, str);
  printf("realIc %s", str);
}



void printComplexIcToConsole(const complexIc_t *value) {
  char str[100];

  realIcToString(&value->real, str);
  printf("complexIc %s + ", str);
  realIcToString(&value->imag, str);
  printf("%si", str);
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

  realIcToString(value, str);
  printf("real51 %s", str);
}



void printReal451ToConsole(const real451_t *value) {
  char str[1000];

  realIcToString(value, str);
  printf("real451 %s", str);
}



void printRegisterDescriptorToConsole(calcRegister_t regist) {
  registerDescriptor_t descriptor;

  descriptor.descriptor = 0xFFFFFFFF;

  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    descriptor = reg[regist];
  }
  else if(regist < 1000) { // Local register
    if(numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < numberOfLocalRegisters) {
        descriptor = *POINTER_TO_LOCAL_REGISTER(regist);
      }
    }
  }
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= 1000u;
      if(regist < numberOfNamedVariables) {
        descriptor = *POINTER_TO_NAMED_VARIABLE(regist);
      }
    }
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      descriptor = savedStackRegister[regist - SAVED_REGISTER_X];
    }
  }

  printf("Header informations of register %d\n", regist);
  printf("    reg ptr   = %u\n", descriptor.dataPointer);
  printf("    data type = %u = %s\n", descriptor.dataType, getDataTypeName(descriptor.dataType, false, false));
  if(descriptor.dataType == dtLongInteger || descriptor.dataType == dtString) {
    printf("    data ptr  = %u\n", descriptor.dataPointer + 1);
    printf("    data size = %" FMT32U "\n", *(dataSize_t *)RAMPTR_TO_MEMPTR(reg[regist].dataPointer));
  }
  printf("    tag       = %u\n", descriptor.tag);
  printf("    name size = %u\n", descriptor.variableNameLen);
}



/********************************************//**
 * \brief Prints the content of a long integer to the console
 *
 * \param r int16_t Register number
 * \return void
 ***********************************************/
void printLongIntegerToConsole(longInteger_t value) {
  char str[3000];

  longIntegerToAllocatedString(value, str);
  printf("LI (%" FMT64U ") %s", (uint64_t)longIntegerSizeInBytes(value), str);

  freeGmp(str, strlen(str) + 1);
}



void reallocateRegister(calcRegister_t regist, uint32_t dataType, uint32_t dataSizeWithoutDataLen, uint32_t tag) { // dataSize without trailing 0 and without data length
  uint32_t dataSizeWithDataLen = dataSizeWithoutDataLen;

//printf("reallocateRegister: %d to %s tag=%u (%u bytes including maxSize) begin\n", regist, getDataTypeName(dataType, false, false), tag, dataSizeWithoutDataLen);
  if((dataType == dtReal16 || dataType == dtAngle16) && dataSizeWithoutDataLen != REAL16_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT32U " is an unexpected numByte value for a real16 or an angle16! It should be REAL16_SIZE=%" FMT32U "!", dataSizeWithoutDataLen, (uint32_t)REAL16_SIZE);
    displayBugScreen(errorMessage);
  }
  else if((dataType == dtReal34 || dataType == dtAngle34) && dataSizeWithoutDataLen != REAL34_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT32U " is an unexpected numByte value for a real34 or an angle34! It should be REAL34_SIZE=%" FMT32U "!", dataSizeWithoutDataLen, (uint32_t)REAL34_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtComplex16 && dataSizeWithoutDataLen != COMPLEX16_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT32U " is an unexpected numByte value for a complex16! It should be COMPLEX16_SIZE=%" FMT32U "!", dataSizeWithoutDataLen, (uint32_t)COMPLEX16_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtComplex34 && dataSizeWithoutDataLen != COMPLEX34_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT32U " is an unexpected numByte value for a complex34! It should be COMPLEX34_SIZE=%" FMT32U "!", dataSizeWithoutDataLen, (uint32_t)COMPLEX34_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtShortInteger && dataSizeWithoutDataLen != SHORT_INTEGER_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT32U " is an unexpected numByte value for an integer! It should be SHORT_INTEGER_SIZE=%" FMT32U "!", dataSizeWithoutDataLen, (uint32_t)SHORT_INTEGER_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtString) {
    dataSizeWithDataLen = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(dataSizeWithoutDataLen + sizeof(dataSize_t) + 1)); // +sizeof(dataSize_t) for the length of the string and +1 for the trailing 0
  }
  else if(dataType == dtLongInteger) {
    if(dataSizeWithoutDataLen % LIMB_SIZE != 0) {
      sprintf(errorMessage, "In function reallocateRegister: the value of dataSizeWithoutDataLen (%" FMT32U ") for a long integer must be a multiple of %" FMT64U "!", dataSizeWithoutDataLen, (uint64_t)LIMB_SIZE);
      displayBugScreen(errorMessage);
      dataSizeWithoutDataLen = ((dataSizeWithoutDataLen / LIMB_SIZE) + 1) * LIMB_SIZE;
    }
    dataSizeWithDataLen = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(dataSizeWithoutDataLen + sizeof(dataSize_t))); // +sizeof(dataSize_t) for the length of the data
  }

  dataSizeWithoutDataLen = dataSizeWithDataLen - sizeof(dataSize_t);
  if(getRegisterDataType(regist) != dataType || ((getRegisterDataType(regist) == dtString || getRegisterDataType(regist) == dtLongInteger) && getRegisterMaxDataLength(regist) != dataSizeWithoutDataLen)) {
    freeRegisterData(regist);
    setRegisterDataPointer(regist, allocWp43s(dataSizeWithDataLen));
    setRegisterDataType(regist, dataType, tag);
    if(dataType == dtString || dataType == dtLongInteger) {
      setRegisterMaxDataLength(regist, dataSizeWithoutDataLen);
    }
  }
  else {
    setRegisterTag(regist, tag);
  }
//sprintf(tmpStr3000, "reallocateRegister %d to %s tag=%u (%u bytes including dataLen) done", regist, getDataTypeName(dataType, false, false), tag, dataSizeWithDataLen);
//memoryDump(tmpStr3000);
}
