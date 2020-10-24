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
    firstDisplayedStepPointer   = programMemoryPointer;
    firstDisplayedStep          = 0;
    freeProgramBytes = 2;
    temporaryInformation = TI_NO_INFO;
  }
}



void fnClP(uint16_t unusedParamButMandatory) {
  printf("CLP\n");
}



void fnPem(uint16_t unusedParamButMandatory) {
  uint16_t line;
  uint8_t *stepPointer;
  bool_t lblOrEnd;

  if(calcMode != CM_PEM) {
    calcMode = CM_PEM;
    return;
  }

  stepPointer = firstDisplayedStepPointer;
  for(line=0; line<7; line++) {
    sprintf(tmpStr3000, "%04u:", firstDisplayedStep + line);
    showString(tmpStr3000, &standardFont, 1, Y_POSITION_OF_REGISTER_T_LINE + 21*line, vmNormal,  false, false);
    lblOrEnd = (*stepPointer == ITM_LBL) || ((*stepPointer == ((ITM_END >> 8) | 0x80)) && (*(stepPointer + 1) == (ITM_END & 0xff)));
    decodeOneStep(stepPointer);
    showString(tmpStr3000, &standardFont, lblOrEnd ? 45 : 75, Y_POSITION_OF_REGISTER_T_LINE + 21*line, vmNormal,  false, false);
    stepPointer = nextStep(stepPointer);
  }
}
