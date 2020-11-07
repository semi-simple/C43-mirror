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
//  |16375|    | '1'| 1.       |  ^
//  |     |  1 | 114|          |  | 1 block = 4 bytes
//  |     |    |   6|          |  |
//  |     |    |   1|          |  v
//  |16376|    | '1'|          |
//  |     |  2 |  95| +        |
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
//  |     |    | '3'| 3.       |
//  |     |  9 | 114|          |
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



void scanLabels(void) {
  uint32_t stepNumber = 0;
  uint16_t program;
  uint8_t *nextStep, *step = beginOfProgramMemory;

  numberOfLabels = 0;
  while(*step != 255 || *(step + 1) != 255) { // .END.
    if(*step == 1) {
      numberOfLabels++;
    }
    step = findNextStep(step);
  }

  free(labelList);
  labelList = malloc(sizeof(labelList_t) * numberOfLabels);
  if(labelList == NULL) {
    //printf("\n");
  }

  numberOfLabels = 0;
  step = beginOfProgramMemory;
  program = 1;
  stepNumber = 0;
  while(*step != 255 || *(step + 1) != 255) { // .END.
    nextStep = findNextStep(step);
    if(*step == 1) { // LBL
      labelList[numberOfLabels].program = program;
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
      program++;
    }

    step = nextStep;
    stepNumber++;
  }
}



void deleteStepsFromTo(uint8_t *from, uint8_t *to) {
  uint16_t opSize = to - from;
  xcopy(from, to, (firstFreeProgramByte - to) + 2);
  firstFreeProgramByte -= opSize;
  freeProgramBytes += opSize;
printf("freeProgramBytes = %u\n", freeProgramBytes);
  scanLabels();
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
    beginOfCurrentProgram       = beginOfProgramMemory;
    endOfCurrentProgram         = beginOfProgramMemory + 2;
    firstDisplayedStep          = beginOfProgramMemory;
    firstDisplayedStepNumber    = 0;
    temporaryInformation        = TI_NO_INFO;
    scanLabels();
printf("freeProgramBytes = %u\n", freeProgramBytes);
  }
}



void fnClP(uint16_t unusedParamButMandatory) {
  if(beginOfCurrentProgram != beginOfProgramMemory || *endOfCurrentProgram != 255 || *(endOfCurrentProgram + 1) != 255) {
    deleteStepsFromTo(beginOfCurrentProgram, endOfCurrentProgram);
  }
}



void defineCurrentProgram(void) {
  if(currentStep < beginOfCurrentProgram || currentStep >= endOfCurrentProgram) { // currentStep in not between begin and end of the current program
    // Calculating endOfCurrentProgram
    endOfCurrentProgram = currentStep;
    while((*endOfCurrentProgram & 0x7f) != (ITM_END >> 8) || *(endOfCurrentProgram + 1) != (ITM_END & 0xff)) { // not END
      endOfCurrentProgram = findNextStep(endOfCurrentProgram);
    }
    beginOfCurrentProgram = endOfCurrentProgram;
    endOfCurrentProgram = findNextStep(endOfCurrentProgram);

    // Calculating beginOfCurrentProgram
    if(beginOfCurrentProgram > beginOfProgramMemory) { // not at the beginning of program memory
      beginOfCurrentProgram = findPreviousStep(beginOfCurrentProgram);
    }

    if((*beginOfCurrentProgram & 0x7f) == (ITM_END >> 8) && *(beginOfCurrentProgram + 1) == (ITM_END & 0xff)) { // END
      beginOfCurrentProgram = findNextStep(beginOfCurrentProgram);
      return;
    }

    while(((*beginOfCurrentProgram & 0x7f) != (ITM_END >> 8) || *(beginOfCurrentProgram + 1) != (ITM_END & 0xff)) && beginOfCurrentProgram > beginOfProgramMemory) { // not END and not at the beginning of program memory
      beginOfCurrentProgram = findPreviousStep(beginOfCurrentProgram);
    }

    if(beginOfCurrentProgram > beginOfProgramMemory) { // not END and not at the beginning of program memory
      beginOfCurrentProgram = findNextStep(beginOfCurrentProgram);
    }
  }
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
      defineCurrentProgram();
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
