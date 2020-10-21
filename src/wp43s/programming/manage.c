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
 * \file manage.c
 ***********************************************/

#include "wp43s.h"



void fnClPAll(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnClPAll);
  }
  else {
    *programMemoryPointer       = 255; // .END.
    *(programMemoryPointer + 1) = 255; // .END.
    resizeProgramMemory(1); // 1 block for an empty program
    programCounter = programMemoryPointer;
    currentProgramMemoryPointer = programMemoryPointer;
    freeProgramBytes = 2;
    temporaryInformation = TI_NO_INFO;
  }
}



void fnClP(uint16_t unusedParamButMandatory) {
  printf("CLP\n");
}



void fnPEM(uint16_t unusedParamButMandatory) {
  if(calcMode != CM_PEM) {
    calcMode = CM_PEM;
    return;
  }

  showString(" Ligne 0 ", &standardFont, 5, Y_POSITION_OF_REGISTER_T_LINE + 21*0, vmNormal,  true, true);
  showString(" Ligne 1 ", &standardFont, 5, Y_POSITION_OF_REGISTER_T_LINE + 21*1, vmNormal,  true, true);
  showString(" Ligne 2 ", &standardFont, 5, Y_POSITION_OF_REGISTER_T_LINE + 21*2, vmNormal,  true, true);
  showString(" Ligne 3 ", &standardFont, 5, Y_POSITION_OF_REGISTER_T_LINE + 21*3, vmReverse, true, true);
  showString(" Ligne 4 ", &standardFont, 5, Y_POSITION_OF_REGISTER_T_LINE + 21*4, vmNormal,  true, true);
  showString(" Ligne 5 ", &standardFont, 5, Y_POSITION_OF_REGISTER_T_LINE + 21*5, vmNormal,  true, true);
  showString(" Ligne 6 ", &standardFont, 5, Y_POSITION_OF_REGISTER_T_LINE + 21*6, vmNormal,  true, true);
}
