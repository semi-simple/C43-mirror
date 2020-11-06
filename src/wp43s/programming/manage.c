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



void scanLabels(void) {
  uint32_t step = 0;
  uint16_t program;
  uint8_t *ns, *currentStepPointer = programMemoryPointer;

  numberOfLabels = 0;
  while(*currentStepPointer != 255 || *(currentStepPointer + 1) != 255) { // .END.
    if(*currentStepPointer == 1) {
      numberOfLabels++;
    }
    currentStepPointer = nextStep(currentStepPointer);
  }

  free(labelList);
  labelList = malloc(sizeof(labelList_t) * numberOfLabels);
  if(labelList == NULL) {
    //printf("\n");
  }

  numberOfLabels = 0;
  currentStepPointer = programMemoryPointer;
  program = 1;
  step = 0;
  while(*currentStepPointer != 255 || *(currentStepPointer + 1) != 255) { // .END.
    ns = nextStep(currentStepPointer);
    if(*currentStepPointer == 1) { // LBL
      labelList[numberOfLabels].program = program;
      if(*(currentStepPointer + 1) <= 109) { // Local label
        labelList[numberOfLabels].followingStep = -(step + 1);
        labelList[numberOfLabels].labelPointer = currentStepPointer + 1;
      }
      else { // Global label
        labelList[numberOfLabels].followingStep = step + 1;
        labelList[numberOfLabels].labelPointer = currentStepPointer + 2;
      }

      labelList[numberOfLabels].instructionPointer = ns;
      numberOfLabels++;
    }

    if((*currentStepPointer & 0x7f) == (ITM_END >> 8) && *(currentStepPointer + 1) == (ITM_END & 0xff)) { // END
      program++;
    }

    currentStepPointer = ns;
    step++;
  }
}



void deleteStepsFromTo(uint8_t *from, uint8_t *to) {
  uint16_t opSize = to - from;
  xcopy(from, to, (firstFreeProgramBytePointer - to) + 2);
  firstFreeProgramBytePointer -= opSize;
  freeProgramBytes += opSize;
printf("freeProgramBytes = %u\n", freeProgramBytes);
  scanLabels();
}



void fnClPAll(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnClPAll);
  }
  else {
    *(programMemoryPointer + 0) = (ITM_END >> 8) | 0x80;
    *(programMemoryPointer + 1) =  ITM_END       & 0xff;
    *(programMemoryPointer + 2) = 255; // .END.
    *(programMemoryPointer + 3) = 255; // .END.
    freeProgramBytes = 0;
printf("freeProgramBytes = %u\n", freeProgramBytes);
    resizeProgramMemory(1); // 1 block for an empty program
    programCounter = programMemoryPointer;
    currentProgramMemoryPointer = programMemoryPointer;
    firstDisplayedStepPointer   = programMemoryPointer;
    firstDisplayedStep          = 0;
    temporaryInformation = TI_NO_INFO;
    scanLabels();
  }
}



void fnClP(uint16_t unusedParamButMandatory) {
  uint8_t *endOfCurrentProgram;

  endOfCurrentProgram = nextStep(currentProgramMemoryPointer);
  if(currentProgramMemoryPointer == programMemoryPointer && *endOfCurrentProgram == 255 && *(endOfCurrentProgram + 1) == 255) { // There is no program to delete at all
    return;
  }

  if(*currentProgramMemoryPointer == ((ITM_END >> 8) | 0x80) && *(currentProgramMemoryPointer + 1) == (ITM_END & 0xff)) { // There is only END in the current program
    deleteStepsFromTo(currentProgramMemoryPointer, endOfCurrentProgram);
    return;
  }

  while(*endOfCurrentProgram != ((ITM_END >> 8) | 0x80) || *(endOfCurrentProgram + 1) != (ITM_END & 0xff)) { // Find the END of the current program
    endOfCurrentProgram = nextStep(endOfCurrentProgram);
  }
  endOfCurrentProgram = nextStep(endOfCurrentProgram);

  if(currentProgramMemoryPointer == programMemoryPointer && *endOfCurrentProgram == 255 && *(endOfCurrentProgram + 1) == 255) { // There is only one program in memory
    fnClPAll(CONFIRMED);
    return;
  }

  deleteStepsFromTo(currentProgramMemoryPointer, endOfCurrentProgram);
}



void fnPem(uint16_t unusedParamButMandatory) {
  uint16_t line, stepSize;
  uint8_t *stepPointer, *ns;
  bool_t lblOrEnd;

  if(calcMode != CM_PEM) {
    calcMode = CM_PEM;
    return;
  }

  stepPointer = firstDisplayedStepPointer;
  programListEnd = false;

  for(line=0; line<7; line++) {
    ns = nextStep(stepPointer);
    stepSize = (uint16_t)(ns - stepPointer);
    sprintf(tmpString, "%04u:" STD_SPACE_4_PER_EM "%s%u", firstDisplayedStep + line, stepSize >= 10 ? "" : STD_SPACE_FIGURE, stepSize);
    if(firstDisplayedStep + line == currentStep) {
      showString(tmpString, &standardFont, 1, Y_POSITION_OF_REGISTER_T_LINE + 21*line, vmReverse, false, true);
      programCounter = stepPointer;
    }
    else {
      showString(tmpString, &standardFont, 1, Y_POSITION_OF_REGISTER_T_LINE + 21*line, vmNormal,  false, true);
    }
    lblOrEnd = (*stepPointer == ITM_LBL) || ((*stepPointer == ((ITM_END >> 8) | 0x80)) && (*(stepPointer + 1) == (ITM_END & 0xff)));
    decodeOneStep(stepPointer);
    showString(tmpString, &standardFont, lblOrEnd ? 45+20 : 75+20, Y_POSITION_OF_REGISTER_T_LINE + 21*line, vmNormal,  false, false);
    numberOfStepsOnScreen = line;
    if(((*stepPointer == ((ITM_END >> 8) | 0x80)) && (*(stepPointer + 1) == (ITM_END & 0xff))) && ((*ns == 255 && *(ns + 1) == 255))) {
      programListEnd = true;
      break;
    }
    stepPointer = ns;
  }
}
