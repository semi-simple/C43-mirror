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
  if(dynamicMenuItem >= 0) {
    if(dynamicMenuItem >= 6 * *dynamicSoftmenu[softmenuStack[softmenuStackPointer].softmenu].menuContent) {
      return;
    }

    uint8_t *labelName = dynamicSoftmenu[softmenuStack[softmenuStackPointer].softmenu].menuContent + 1;
    while(dynamicMenuItem > 0) {
      labelName += stringByteLength((char *)labelName) + 1;
      dynamicMenuItem--;
    }

    if(*labelName == 0) {
      return;
    }

    int16_t c, len = stringByteLength((char *)labelName);
    for(uint16_t lbl=0; lbl<numberOfLabels; lbl++) {
      if(labelList[lbl].followingStep > 0 && labelList[lbl].labelPointer[c] == len) { // It's a global label and the length is OK
        for(c=0; c<len; c++) {
          if(labelName[c] != labelList[lbl].labelPointer[c + 1]) {
            break;
          }
        }
        if(c == len) {
          stepNumber = labelList[lbl].followingStep - 1;
          break;
        }
      }
    }
  }

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
