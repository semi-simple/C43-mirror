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
 * \file stack.c Stack managenent
 ***********************************************/

#include "wp43s.h"

/********************************************//**
 * \brief Clears X and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnClX(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnClX");
  #endif

  clearRegister(REGISTER_X);
  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnClX");
  #endif
}



/********************************************//**
 * \brief Clears the stack and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 *
 ***********************************************/
void fnClearStack(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnClearStack");
  #endif

  for(calcRegister_t regist=REGISTER_X; regist<=getStackTop(); regist++) {
    clearRegister(regist);
  }

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnClearStack");
  #endif
}



/********************************************//**
 * \brief Drops X from the stack and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnDrop(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnDrop");
  #endif

  freeRegisterData(REGISTER_X);
  for(calcRegister_t regist=REGISTER_X; regist<getStackTop(); regist++) {
    reg[regist] = reg[regist + 1];
  }

  setRegisterDataPointer(getStackTop() - 1, allocateMemory(getRegisterDataSize(getStackTop())));
  memcpy(POINTER_TO_REGISTER_DATA(getStackTop()-1), POINTER_TO_REGISTER_DATA(getStackTop()), getRegisterDataSize(getStackTop()));
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnDrop");
  #endif
}



/********************************************//**
 * \brief Lifts the stack if allowed and reallocates
 * the X register
 *
 * \param[in] dataType uint32_t Data type of the new X register
 * \param[in] numBytes uint32_t Number of bytes allocated to the new X register
 * \return void
 ***********************************************/
void liftStack(uint32_t dataType, uint32_t numBytes) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("liftStack");
  #endif

  if(stackLiftEnabled) {
    freeRegisterData(getStackTop());
    for(uint16_t i=getStackTop(); i>REGISTER_X; i--) {
      reg[i] = reg[i-1];
    }
  }
  else {
    freeRegisterData(REGISTER_X);
  }

  setRegisterDataType(REGISTER_X, dataType);
  setRegisterDataPointer(REGISTER_X, allocateMemory(numBytes));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("liftStack");
  #endif
}



/********************************************//**
 * \brief Drops Y from the stack and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnDropY(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnDropY");
  #endif

  freeRegisterData(REGISTER_Y);
  for(uint16_t i=REGISTER_Y; i<getStackTop(); i++) {
    reg[i] = reg[i+1];
  }

  setRegisterDataPointer(getStackTop() - 1, allocateMemory(getRegisterDataSize(getStackTop())));
  memcpy(POINTER_TO_REGISTER_DATA(getStackTop()-1), POINTER_TO_REGISTER_DATA(getStackTop()), getRegisterDataSize(getStackTop()));
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnDropY");
  #endif
}



/********************************************//**
 * \brief Rolls the stack up and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnRollUp(uint16_t unusedParamButMandatory) {
  uint32_t savedRegister = reg[stackSize==SS_4 ? REGISTER_T : REGISTER_D];

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnRollUp");
  #endif

  for(uint16_t i=(stackSize==SS_4 ? REGISTER_T : REGISTER_D); i>REGISTER_X; i--) {
    reg[i] = reg[i-1];
  }
  reg[REGISTER_X] = savedRegister;
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnRollUp");
  #endif
}



/********************************************//**
 * \brief Rolls the stack down and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnRollDown(uint16_t unusedParamButMandatory) {
  uint32_t savedRegister = reg[REGISTER_X];

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnRollDown");
  #endif

  for(uint16_t i=REGISTER_X; i<(stackSize==SS_4 ? REGISTER_T : REGISTER_D); i++) {
    reg[i] = reg[i+1];
  }
  reg[stackSize==SS_4 ? REGISTER_T : REGISTER_D] = savedRegister;
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnRollDown");
  #endif
}



/********************************************//**
 * \brief Sets the stack size
 *
 * \param[in] stackSize uint16_t
 * \return void
 ***********************************************/
void fnStackSize(uint16_t ss) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnStackSize");
  #endif

  stackSize = ss;

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnStackSize");
  #endif
}



/********************************************//**
 * \brief Sets the number of stack registers displayed
 *
 * \param[in] numberOfStackLines uint16_t
 * \return void
 ***********************************************/
void fnDisplayStack(uint16_t numberOfStackLines) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnDisplayStack");
  #endif

  displayStack = numberOfStackLines;
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnDisplayStack");
  #endif
}



/********************************************//**
 * \brief Swaps X and Y and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSwapXY(uint16_t unusedParamButMandatory) {
  uint32_t savedRegister = reg[REGISTER_X];

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnSwapXY");
  #endif

  reg[REGISTER_X] = reg[REGISTER_Y];
  reg[REGISTER_Y] = savedRegister;
  refreshRegisterLine(REGISTER_X);
  refreshRegisterLine(REGISTER_Y);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnSwapXY");
  #endif
}



/********************************************//**
 * \brief Fills the stack with the value of X and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFillStack(uint16_t unusedParamButMandatory) {
  uint16_t dataTypeX = getRegisterDataType(REGISTER_X);
  uint16_t dataSizeX = getRegisterDataSize(REGISTER_X);
  uint16_t dataInfo  = getRegisterDataInfo(REGISTER_X);
  uint16_t newDataPointer;

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnFillStack");
  #endif

  for(uint16_t i=REGISTER_Y; i<=(stackSize==SS_4 ? REGISTER_T : REGISTER_D); i++) {
    freeRegisterData(i);
    setRegisterDataType(i, dataTypeX);
    setRegisterDataInfo(i, dataInfo);
    newDataPointer = allocateMemory(dataSizeX);
    setRegisterDataPointer(i, newDataPointer);
    memcpy(ram + newDataPointer, POINTER_TO_REGISTER_DATA(REGISTER_X), dataSizeX);
  }

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnFillStack");
  #endif
}



/********************************************//**
 * \brief Sets X to the stack size and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetStackSize(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnGetStackSize");
  #endif

  liftStack(dtReal16, REAL16_SIZE);

  int32ToReal16(stackSize==SS_4 ? 4 : 8, POINTER_TO_REGISTER_DATA(REGISTER_X));

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnGetStackSize");
  #endif
}



/********************************************//**
 * \brief Refreshes the screen
 *
 * \param void
 * \return void
 ***********************************************/
void refreshStack(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("refreshStack");
  #endif

  refreshRegisterLine(REGISTER_X);
  refreshRegisterLine(REGISTER_Y);
  refreshRegisterLine(REGISTER_Z);
  refreshRegisterLine(REGISTER_T);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("refreshStack");
  #endif
}



void saveStack(void) {
  savedStackLiftEnabled = stackLiftEnabled;
  for(calcRegister_t regist=getStackTop(); regist>=REGISTER_X; regist--) {
    //printf("save %" FMT16S " to %" FMT16S " : ", regist, SAVED_REGISTER_X - REGISTER_X + regist); printRegisterToConsole(regist, 0); printf("\n");
    copySourceRegisterToDestRegister(regist, SAVED_REGISTER_X - REGISTER_X + regist);
  }
  //printf("save %" FMT16S " to %" FMT16S " : ", REGISTER_L, SAVED_REGISTER_L); printRegisterToConsole(REGISTER_L, 0); printf("\n");
  copySourceRegisterToDestRegister(REGISTER_L, SAVED_REGISTER_L);
  //printf("\n");
  //refreshDebugPanel();
}



void restoreStack(void) {
  stackLiftEnabled = savedStackLiftEnabled;
  for(calcRegister_t regist=getStackTop(); regist>=REGISTER_X; regist--) {
    copySourceRegisterToDestRegister(SAVED_REGISTER_X - REGISTER_X + regist, regist);
    //printf("restore %" FMT16S " to %" FMT16S " : ", SAVED_REGISTER_X - REGISTER_X + regist, regist); printRegisterToConsole(regist, 0); printf("\n");
  }
  copySourceRegisterToDestRegister(SAVED_REGISTER_L, REGISTER_L);
  //printf("restore %" FMT16S " to %" FMT16S " : ", SAVED_REGISTER_L, REGISTER_L); printRegisterToConsole(REGISTER_L, 0); printf("\n");
  //printf("\n");
  refreshStack();
}



#if (STACK_LIFT_DEBUG == 1)
void stackLiftEnable(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("stackLiftEnable");
  #endif

  //printf("Stack lift enabled\n");
  stackLiftEnabled = true;
  hideBattery();

  // Draw S
  setPixel(392,  1);
  setPixel(393,  1);
  setPixel(394,  1);
  setPixel(391,  2);
  setPixel(395,  2);
  setPixel(391,  3);
  setPixel(392,  4);
  setPixel(393,  4);
  setPixel(394,  4);
  setPixel(395,  5);
  setPixel(391,  6);
  setPixel(395,  6);
  setPixel(392,  7);
  setPixel(393,  7);
  setPixel(394,  7);

  // Draw L
  setPixel(391, 10);
  setPixel(391, 11);
  setPixel(391, 12);
  setPixel(391, 13);
  setPixel(391, 14);
  setPixel(391, 15);
  setPixel(391, 16);
  setPixel(392, 16);
  setPixel(393, 16);
  setPixel(394, 16);
  setPixel(395, 16);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("stackLiftEnable");
  #endif
}



void stackLiftDisable(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("stackLiftDisable");
  #endif

  //printf("Stack lift disabled\n");
  stackLiftEnabled = false;
  hideBattery();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("stackLiftDisable");
  #endif
}
#endif
