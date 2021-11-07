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
 * \file sumprod.c
 ***********************************************/

#include "solver/sumprod.h"

#include "constantPointers.h"
#include "defines.h"
#include "error.h"
#include "items.h"
#include "mathematics/integerPart.h"
#include "mathematics/iteration.h"
#include "programming/manage.h"
#include "realType.h"
#include "registers.h"
#include "solver/solve.h"
#include "stack.h"
#include "wp43s.h"

#ifndef TESTSUITE_BUILD
static void _sumProdIteration(uint16_t label) {
  //
  //  NOT A COMPLETE ENGINE: TESTING PURPOSE ONLY!!
  //  The following decoder is minimally implemented ad hoc engine for testing of SOLVE feature.
  //  Replace with the complete programming system when ready.
  //
  uint8_t *step = labelList[label - FIRST_LABEL].instructionPointer;
  lastErrorCode = ERROR_NONE;
  while(executeStep_test(&step)) {}
}

static void _programmableSumProd(uint16_t label, bool_t prod) {
  real34_t counter, result;
  bool_t finished = false;

  real34Copy(prod ? const34_1 : const34_0, &result);

  while(1) {
    fnToReal(NOPARAM);
    if(lastErrorCode != ERROR_NONE) break;
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &counter);
    fnIp(NOPARAM);
    fnFillStack(NOPARAM);

    _sumProdIteration(label);
    if(lastErrorCode != ERROR_NONE) break;

    fnToReal(NOPARAM);
    if(lastErrorCode != ERROR_NONE) break;
    if(prod) {
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), &result, &result);
    }
    else {
      real34Add(REGISTER_REAL34_DATA(REGISTER_X), &result, &result);
    }
    real34Copy(&counter, REGISTER_REAL34_DATA(REGISTER_X));

    if(finished) break;
    fnDse(REGISTER_X);
    finished = (temporaryInformation != TI_TRUE);
  }

  if(lastErrorCode == ERROR_NONE) {
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&result, REGISTER_REAL34_DATA(REGISTER_X));
  }

  temporaryInformation = TI_NO_INFO;
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}

void _checkArgument(uint16_t label, bool_t prod) {
  if(label >= FIRST_LABEL && label <= LAST_LABEL) {
    _programmableSumProd(label, prod);
  }
  else if(label >= REGISTER_X && label <= REGISTER_T) {
    // Interactive mode
    char buf[4];
    switch(label) {
      case REGISTER_X:        buf[0] = 'X'; break;
      case REGISTER_Y:        buf[0] = 'Y'; break;
      case REGISTER_Z:        buf[0] = 'Z'; break;
      case REGISTER_T:        buf[0] = 'T'; break;
      default: /* unlikely */ buf[0] = 0;
    }
    buf[1] = 0;
    label = findNamedLabel(buf);
    if(label == INVALID_VARIABLE) {
      displayCalcErrorMessage(ERROR_LABEL_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "string '%s' is not a named label", buf);
        moreInfoOnError("In function fnPgmSlv:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      _programmableSumProd(label, prod);
    }
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "unexpected parameter %u", label);
      moreInfoOnError("In function fnPgmSlv:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}

#endif // TESTSUITE_BUILD

void fnProgrammableSum(uint16_t label) {
#ifndef TESTSUITE_BUILD
  _checkArgument(label, false);
#endif // TESTSUITE_BUILD
}

void fnProgrammableProduct(uint16_t label) {
#ifndef TESTSUITE_BUILD
  _checkArgument(label, true);
#endif // TESTSUITE_BUILD
}
