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
 * \file lblGtoXeq.c
 ***********************************************/

#include "wp43s.h"



void fnGoto(uint16_t label) {
  #ifndef DMCP_BUILD
    printf("fnGoto: GTO %02u\n", label);
  #endif // DMCP_BUILD
}



void fnGotoDot(uint16_t stepNumber) {
  printf("\nGTO.%4u", stepNumber);
  currentStepNumber = stepNumber;

  defineCurrentProgramFromCurrentStepNumber();

  uint8_t *stepPointer = beginOfCurrentProgram;
  stepNumber = programList[currentProgramNumber].step - 1;
  while(true) {
    if(stepNumber == currentStepNumber) {
      currentStep = stepPointer;
      break;
    }

    stepPointer = findNextStep(stepPointer);
    stepNumber++;
  }

  if(currentStepNumber >= 3) {
    firstDisplayedStepNumber = currentStepNumber - 3;
    firstDisplayedStep = findPreviousStep(findPreviousStep(findPreviousStep(currentStep)));
  }
  else {
    firstDisplayedStepNumber = 0;
    firstDisplayedStep = beginOfProgramMemory;
  }
}
