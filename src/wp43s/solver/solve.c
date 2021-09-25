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
 * \file solve.c
 ***********************************************/

#include "solver/solve.h"

#include "defines.h"
#include "items.h"
#include "registers.h"
#include "softmenus.h"
#include "wp43s.h"

void fnPgmSlv(uint16_t label) {
  currentSolverProgram = label - FIRST_LABEL;
}

void fnSolve(uint16_t labelOrVariable) {
  if(labelOrVariable >= FIRST_LABEL && labelOrVariable <= LAST_LABEL) {
    // Interactive mode
    currentSolverProgram = labelOrVariable - FIRST_LABEL;
    currentSolverStatus = SOLVER_STATUS_INTERACTIVE;
  }
  else if(labelOrVariable >= FIRST_NAMED_VARIABLE && labelOrVariable <= LAST_NAMED_VARIABLE) {
    // Execute
    currentSolverVariable = labelOrVariable;
    printf("Solver to be coded\n");
    fflush(stdout);
    temporaryInformation = TI_SOLVER_VARIABLE;
  }
  else {
    /* mockup */
  }
}

void fnSolveVar(uint16_t unusedButMandatoryParameter) {
  const char *var = (char *)getNthString(dynamicSoftmenu[softmenuStack[0].softmenuId].menuContent, dynamicMenuItem);
  const uint16_t regist = findOrAllocateNamedVariable(var);
  printf("fnSolveVar:\n       softmenuId = %d\n  dynamicMenuItem = %d\n", softmenuStack[0].softmenuId, dynamicMenuItem);
  printf("%d %s\n", regist, var);
  fflush(stdout);
  if(currentSolverStatus & SOLVER_STATUS_READY_TO_EXECUTE) {
    reallyRunFunction(ITM_SOLVE, regist);
  }
  else {
    currentSolverVariable = regist;
    reallyRunFunction(ITM_STO, regist);
    currentSolverStatus |= SOLVER_STATUS_READY_TO_EXECUTE;
    temporaryInformation = TI_SOLVER_VARIABLE;
  }
}
