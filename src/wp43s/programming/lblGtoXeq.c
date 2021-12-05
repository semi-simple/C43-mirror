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

#include "programming/lblGtoXeq.h"

#include "charString.h"
#include "items.h"
#include "programming/manage.h"
#include "programming/nextStep.h"
#include "softmenus.h"

#include "wp43s.h"



void fnGoto(uint16_t label) {
  if(tam.mode) {
    if(dynamicMenuItem >= 0) {
      fnGotoDot(label);
      return;
    }

    // Local Label 00 to 99 and A, B, C, D, I, and J
    if(label < REGISTER_X || (label != REGISTER_L && REGISTER_A <= label && label <= REGISTER_J)) {
      switch(label) {
        case REGISTER_A: label = 100 - 'A' + 'A'; break;
        case REGISTER_B: label = 100 - 'A' + 'B'; break;
        case REGISTER_C: label = 100 - 'A' + 'C'; break;
        case REGISTER_D: label = 100 - 'A' + 'D'; break;
        case REGISTER_I: label = 100 - 'A' + 'I'; break;
        case REGISTER_J: label = 100 - 'A' + 'J'; break;
        default: {}
      }

      // Search for local label
      for(uint16_t lbl=0; lbl<numberOfLabels; lbl++) {
        if(labelList[lbl].program == currentProgramNumber && labelList[lbl].step < 0 && *(labelList[lbl].labelPointer) == label) { // Is in the current program and is a local label and is the searched label
          fnGotoDot(-labelList[lbl].step);
          return;
        }
      }

      #ifndef DMCP_BUILD
        if(label < REGISTER_X) {
          printf("Error in function fnGoto: there is no local label %02u in current program\n", label);
        }
        else {
          printf("Error in function fnGoto: there is no local label %c in current program\n", 'A' + (label - 100));
        }
      #endif // DMCP_BUILD
    }
    else if(label >= FIRST_LABEL && label <= LAST_LABEL) { // Global named label
      if((label - FIRST_LABEL) < numberOfLabels) {
        fnGotoDot(labelList[label - FIRST_LABEL].step);
        return;
      }
      #ifndef DMCP_BUILD
      else {
        printf("Error in function fnGoto: label ID %u out of range\n", label - FIRST_LABEL);
      }
      #endif // DMCP_BUILD
    }
    else { // Global label X, Y, Z, T, K, or L
      switch(label) {
        case REGISTER_X: label = 'X'; break;
        case REGISTER_Y: label = 'Y'; break;
        case REGISTER_Z: label = 'Z'; break;
        case REGISTER_T: label = 'T'; break;
        case REGISTER_K: label = 'K'; break;
        case REGISTER_L: label = 'L'; break;
        default: {}
      }

      for(uint16_t lbl=0; lbl<numberOfLabels; lbl++) {
        if(labelList[lbl].step > 0 && *(labelList[lbl].labelPointer) == 1 && *(labelList[lbl].labelPointer + 1) == label) { // Is a global label and is the searched label
          fnGotoDot(labelList[lbl].step);
          return;
        }
      }

      #ifndef DMCP_BUILD
        printf("Error in function fnGoto: there is no global label %c\n", label);
      #endif // DMCP_BUILD
    }
  }
  else {
    insertStepInProgram(ITM_GTO);
  }
}



void fnGotoDot(uint16_t globalStepNumber) {
  if(dynamicMenuItem >= 0) {
    uint8_t *labelName = (uint8_t *)dynmenuGetLabel(dynamicMenuItem);

    if(*labelName == 0) {
      return;
    }

    int16_t c, len = stringByteLength((char *)labelName);
    for(uint16_t lbl=0; lbl<numberOfLabels; lbl++) {
      if(labelList[lbl].step > 0 && *labelList[lbl].labelPointer == len) { // It's a global label and the length is OK
        for(c=0; c<len; c++) {
          if(labelName[c] != labelList[lbl].labelPointer[c + 1]) {
            break;
          }
        }
        if(c == len) {
          globalStepNumber = labelList[lbl].step;
          break;
        }
      }
    }
  }

  defineCurrentProgramFromGlobalStepNumber(globalStepNumber);
  currentLocalStepNumber = globalStepNumber - programList[currentProgramNumber - 1].step + 1;

  uint8_t *stepPointer = beginOfCurrentProgram;
  globalStepNumber = 1;
  while(true) {
    if(globalStepNumber == currentLocalStepNumber) {
      currentStep = stepPointer;
      break;
    }

    stepPointer = findNextStep(stepPointer);
    globalStepNumber++;
  }

  if(currentLocalStepNumber >= 3) {
    firstDisplayedLocalStepNumber = currentLocalStepNumber - 3;
    firstDisplayedStep = findPreviousStep(findPreviousStep(currentStep));
    if(firstDisplayedLocalStepNumber != 0) {
      firstDisplayedStep = findPreviousStep(firstDisplayedStep);
    }
    uint16_t numberOfSteps = getNumberOfSteps();
    if(firstDisplayedLocalStepNumber + 6 > numberOfSteps) {
      for(int i=3+currentLocalStepNumber-numberOfSteps; i>0; i--) {
        if(firstDisplayedLocalStepNumber > 0) {
          firstDisplayedLocalStepNumber--;
        }
        if(firstDisplayedStep > programList[currentProgramNumber - 1].instructionPointer) {
          firstDisplayedStep = findPreviousStep(firstDisplayedStep);
        }
      }
    }
  }
  else {
    firstDisplayedLocalStepNumber = 0;
    firstDisplayedStep = beginOfCurrentProgram;
  }
}
