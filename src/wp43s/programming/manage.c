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

// Structure of the program memory.
// In this example the RAM is 16384 blocks (from 0 to 16383) of 4 bytes = 65536 bytes.
// The program memory occupies the end of the RAM area.
//
//  +-----+----+----+----------+
//  |Block|Step|Code|    OP    |
//  +-----+----+----+----------+
//  |16374|  0 |   1| LBL 'P1' | <-- beginOfProgramMemory
//  |     |    | 253|          |
//  |     |    |   2|          |
//  |     |    | 'P'|          |
//  |16375|    | '1'|          |  ^
//  |     |  1 | 114| 1.       |  | 1 block = 4 bytes
//  |     |    |   6|          |  |
//  |     |    |   1|          |  v
//  |16376|    | '1'|          |
//  |     |  2 |  95| +        | <-- firstDisplayedStep
//  |     |  3 | 133| END      |
//  |     |    | 168|          |
//  |16377|  4 |   1| LBL 'P2' | <-- beginOfCurrentProgram
//  |     |    | 253|          |
//  |     |    |   2|          |
//  |     |    | 'P'|          |
//  |16378|    | '2'|          |
//  |     |  5 | 114| 22.      | <-- currentStep       this inequality is always true: beginOfCurrentProgram ≤ currentStep < endOfCurrentProgram
//  |     |    |   6|          |
//  |     |    |   2|          |
//  |16379|    | '2'|          |
//  |     |    | '2'|          |
//  |     |  6 |  95| +        |
//  |     |  7 | 133| END      |
//  |16380|    | 168|          |
//  |     |  8 |   2| LBL 'P3' | <-- endOfCurrentProgram
//  |     |    | 253|          |
//  |     |    |   2|          |
//  |16381|    | 'P'|          |
//  |     |    | '3'|          |
//  |     |  9 | 114| 3.       |
//  |     |    |   6|          |
//  |16382|    |   1|          |
//  |     |    | '3'|          |
//  |     | 10 |  95| +        |
//  |     | 11 | 133| END      |
//  |16383|    | 168|          |
//  |     |    | 255| .END.    | <-- firstFreeProgramByte
//  |     |    | 255|          |
//  |     |    |   ?|          | free byte     This byte is the end of the RAM area
//  +-----+----+----+----------+
//
//  freeProgramBytes = 1



void scanLabelsAndPrograms(void) {
  uint32_t stepNumber = 0;
  uint8_t *nextStep, *step = beginOfProgramMemory;

  numberOfLabels = 0;
  numberOfPrograms = 1;
  while(*step != 255 || *(step + 1) != 255) { // .END.
    if(*step == ITM_LBL) { // LBL
      numberOfLabels++;
    }
    if((*step & 0x7f) == (ITM_END >> 8) && *(step + 1) == (ITM_END & 0xff)) { // END
      numberOfPrograms++;
    }
    step = findNextStep(step);
  }

  free(labelList);
  free(programList);

  labelList = malloc(sizeof(labelList_t) * numberOfLabels);
  if(labelList == NULL) {
    //printf("\n");
  }

  programList = malloc(sizeof(programList_t) * numberOfPrograms);
  if(programList == NULL) {
    //printf("\n");
  }

  numberOfLabels = 0;
  step = beginOfProgramMemory;
  programList[0].instructionPointer = step;
  programList[0].step = (0 + 1);
  numberOfPrograms = 1;
  stepNumber = 0;
  while(*step != 255 || *(step + 1) != 255) { // .END.
    nextStep = findNextStep(step);
    if(*step == 1) { // LBL
      labelList[numberOfLabels].program = numberOfPrograms;
      if(*(step + 1) <= 109) { // Local label
        labelList[numberOfLabels].followingStep = -(stepNumber + 1);
        labelList[numberOfLabels].labelPointer = step + 1;
      }
      else { // Global label
        labelList[numberOfLabels].followingStep = stepNumber + 1;
        labelList[numberOfLabels].labelPointer = step + 2;
      }

      labelList[numberOfLabels].instructionPointer = nextStep;
      numberOfLabels++;
    }

    if((*step & 0x7f) == (ITM_END >> 8) && *(step + 1) == (ITM_END & 0xff)) { // END
      programList[numberOfPrograms].instructionPointer = step + 2;
      programList[numberOfPrograms].step = stepNumber + 2;
      numberOfPrograms++;
    }

    step = nextStep;
    stepNumber++;
  }

  defineCurrentProgram();
}



void deleteStepsFromTo(uint8_t *from, uint8_t *to) {
  uint16_t opSize = to - from;

  xcopy(from, to, (firstFreeProgramByte - to) + 2);
  firstFreeProgramByte -= opSize;
  freeProgramBytes += opSize;
  scanLabelsAndPrograms();
}



void fnClPAll(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnClPAll);
  }
  else {
    resizeProgramMemory(1); // 1 block for an empty program
    *(beginOfProgramMemory + 0) = (ITM_END >> 8) | 0x80;
    *(beginOfProgramMemory + 1) =  ITM_END       & 0xff;
    *(beginOfProgramMemory + 2) = 255; // .END.
    *(beginOfProgramMemory + 3) = 255; // .END.
    firstFreeProgramByte        = beginOfProgramMemory + 2;
    freeProgramBytes            = 0;
    currentStep                 = beginOfProgramMemory;
    firstDisplayedStep          = beginOfProgramMemory;
    firstDisplayedStepNumber    = 0;
    temporaryInformation        = TI_NO_INFO;
    scanLabelsAndPrograms();
  }
}



void fnClP(uint16_t unusedParamButMandatory) {
  int16_t i = 0;

  if(beginOfCurrentProgram == beginOfProgramMemory && *endOfCurrentProgram == 255 && *(endOfCurrentProgram + 1) == 255) { // There is only one program in memory
    fnClPAll(CONFIRMED);
  }
  else {
    currentStep = beginOfCurrentProgram;
    while(programList[i].instructionPointer != beginOfCurrentProgram) {
      i++;
    }

    if(*programList[i + 1].instructionPointer == 255 && *(programList[i + 1].instructionPointer + 1) == 255) { // The last program of the list will be deleted
      currentStepNumber        = programList[i - 1].step - 1;
      firstDisplayedStepNumber = currentStepNumber;
      currentStep              = programList[i - 1].instructionPointer;
    }
    else {
      currentStepNumber        = programList[i].step - 1;
      firstDisplayedStepNumber = currentStepNumber;
      currentStep              = programList[i].instructionPointer;
    }

    deleteStepsFromTo(beginOfCurrentProgram, endOfCurrentProgram);
    firstDisplayedStep = beginOfCurrentProgram;

    if(firstDisplayedStepNumber >= 3) {
      firstDisplayedStepNumber -= 3;
      firstDisplayedStep = findPreviousStep(firstDisplayedStep);
      firstDisplayedStep = findPreviousStep(firstDisplayedStep);
      firstDisplayedStep = findPreviousStep(firstDisplayedStep);
    }
    else if(firstDisplayedStepNumber >= 2) {
      firstDisplayedStepNumber -= 2;
      firstDisplayedStep = findPreviousStep(firstDisplayedStep);
      firstDisplayedStep = findPreviousStep(firstDisplayedStep);
    }
    else if(firstDisplayedStepNumber >= 1) {
      firstDisplayedStepNumber -= 1;
      firstDisplayedStep = findPreviousStep(firstDisplayedStep);
    }
  }
}



void defineCurrentProgram(void) {
  int16_t program;

  program = 0;
  while(currentStep >= programList[program].instructionPointer) {
    program++;
  }

  endOfCurrentProgram = programList[program--].instructionPointer;
  beginOfCurrentProgram = programList[program].instructionPointer;
}



void fnPem(uint16_t unusedParamButMandatory) {
  uint16_t line, stepSize;
  uint8_t *step, *nextStep;
  bool_t lblOrEnd;

  if(calcMode != CM_PEM) {
    calcMode = CM_PEM;
    return;
  }

  step = firstDisplayedStep;
  programListEnd = false;

  for(line=0; line<7; line++) {
    nextStep = findNextStep(step);
    stepSize = (uint16_t)(nextStep - step);
    sprintf(tmpString, "%04u:" STD_SPACE_4_PER_EM "%s%u", firstDisplayedStepNumber + line, stepSize >= 10 ? "" : STD_SPACE_FIGURE, stepSize);
    if(firstDisplayedStepNumber + line == currentStepNumber) {
      showString(tmpString, &standardFont, 1, Y_POSITION_OF_REGISTER_T_LINE + 21*line, vmReverse, false, true);
      currentStep = step;
      if(currentStep < beginOfCurrentProgram || currentStep >= endOfCurrentProgram) { // currentSetep is outside the current program
        defineCurrentProgram();
      }
    }
    else {
      showString(tmpString, &standardFont, 1, Y_POSITION_OF_REGISTER_T_LINE + 21*line, vmNormal,  false, true);
    }
    lblOrEnd = (*step == ITM_LBL) || ((*step == ((ITM_END >> 8) | 0x80)) && (*(step + 1) == (ITM_END & 0xff)));
    decodeOneStep(step);
    showString(tmpString, &standardFont, lblOrEnd ? 45+20 : 75+20, Y_POSITION_OF_REGISTER_T_LINE + 21*line, vmNormal,  false, false);
    numberOfStepsOnScreen = line;
    if(((*step == ((ITM_END >> 8) | 0x80)) && (*(step + 1) == (ITM_END & 0xff))) && ((*nextStep == 255 && *(nextStep + 1) == 255))) {
      programListEnd = true;
      break;
    }
    step = nextStep;
  }
}
