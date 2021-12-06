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

#include "charString.h"
#include "constantPointers.h"
#include "defines.h"
#include "error.h"
#include "flags.h"
#include "items.h"
#include "longIntegerType.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/wp34s.h"
#include "programming/manage.h"
#include "programming/nextStep.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "softmenus.h"
#include "solver/equation.h"
#include "solver/tvm.h"
#include "stack.h"
#include "wp43s.h"

void fnPgmSlv(uint16_t label) {
  if(label >= FIRST_LABEL && label <= LAST_LABEL) {
    currentSolverProgram = label - FIRST_LABEL;
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
      currentSolverProgram = label - FIRST_LABEL;
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

static bool_t _realSolverFirstGuesses(calcRegister_t regist, real34_t *val) {
  if(getRegisterDataType(regist) == dtReal34 && getRegisterAngularMode(regist) == amNone) {
    real34Copy(REGISTER_REAL34_DATA(regist), val);
    return true;
  }
  else if(getRegisterDataType(regist) == dtLongInteger) {
    convertLongIntegerRegisterToReal34(regist, val);
    return true;
  }
  return false;
}

void fnSolve(uint16_t labelOrVariable) {
  if((labelOrVariable >= FIRST_LABEL && labelOrVariable <= LAST_LABEL) || (labelOrVariable >= REGISTER_X && labelOrVariable <= REGISTER_T)) {
    // Interactive mode
    fnPgmSlv(labelOrVariable);
    if(lastErrorCode == ERROR_NONE)
      currentSolverStatus = SOLVER_STATUS_INTERACTIVE;
    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }
  else if(labelOrVariable >= FIRST_NAMED_VARIABLE && labelOrVariable <= LAST_NAMED_VARIABLE) {
    // Execute
    real34_t z, y, x;
    real_t tmp;
    int resultCode = 0;
    if(_realSolverFirstGuesses(REGISTER_Y, &y) && _realSolverFirstGuesses(REGISTER_X, &x)) {
      currentSolverVariable = labelOrVariable;
      resultCode = solver(labelOrVariable, &y, &x, &z, &y, &x);
      fnClearStack(NOPARAM); // reset stack to 0.
      real34ToReal(&z, &tmp), convertRealToReal34ResultRegister(&tmp, REGISTER_Z);
      real34ToReal(&y, &tmp), convertRealToReal34ResultRegister(&tmp, REGISTER_Y);
      real34ToReal(&x, &tmp), convertRealToReal34ResultRegister(&tmp, REGISTER_X);
      int32ToReal34(resultCode, REGISTER_REAL34_DATA(REGISTER_T));
      switch(resultCode) {
        case SOLVER_RESULT_NORMAL:
          temporaryInformation = TI_SOLVER_VARIABLE;
          lastErrorCode = ERROR_NONE;
          break;
        case SOLVER_RESULT_SIGN_REVERSAL:
          temporaryInformation = TI_SOLVER_FAILED;
          displayCalcErrorMessage(ERROR_LARGE_DELTA_AND_OPPOSITE_SIGN, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
          break;
        case SOLVER_RESULT_EXTREMUM:
          temporaryInformation = TI_SOLVER_FAILED;
          displayCalcErrorMessage(ERROR_SOLVER_REACHED_LOCAL_EXTREMUM, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
          break;
        case SOLVER_RESULT_BAD_GUESS:
          temporaryInformation = TI_SOLVER_FAILED;
          displayCalcErrorMessage(ERROR_INITIAL_GUESS_OUT_OF_DOMAIN, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
          break;
        case SOLVER_RESULT_CONSTANT:
          temporaryInformation = TI_SOLVER_FAILED;
          displayCalcErrorMessage(ERROR_FUNCTION_VALUES_LOOK_CONSTANT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
          break;
      }
      saveForUndo();
      adjustResult(REGISTER_X, false, false, REGISTER_X, REGISTER_Y, -1);
    }
    else {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
      #ifdef PC_BUILD
        sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
        moreInfoOnError("In function fnSolve:", errorMessage, "is not a real number.", "");
      #endif
      adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
    }
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "unexpected parameter %u", labelOrVariable);
      moreInfoOnError("In function fnPgmSlv:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }
}

void fnSolveVar(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  const char *var = (char *)getNthString(dynamicSoftmenu[softmenuStack[0].softmenuId].menuContent, dynamicMenuItem);
  const uint16_t regist = findOrAllocateNamedVariable(var);
  if(currentSolverStatus & SOLVER_STATUS_READY_TO_EXECUTE) {
    reallyRunFunction(ITM_SOLVE, regist);
  }
  else {
    currentSolverVariable = regist;
    reallyRunFunction(ITM_STO, regist);
    currentSolverStatus |= SOLVER_STATUS_READY_TO_EXECUTE;
    temporaryInformation = TI_SOLVER_VARIABLE;
  }
#endif /* TESTSUITE_BUILD */
}

#ifndef TESTSUITE_BUILD
bool_t executeStep_test(uint8_t **step) {
  //
  //  NOT A COMPLETE ENGINE: TESTING PURPOSE ONLY!!
  //  The following decoder is minimally implemented ad hoc engine for testing of SOLVE feature.
  //  Replace with the complete programming system when ready.
  //
  uint16_t op = **step;
  if(op & 0x80) op = ((op << 8) | *(*step + 1)) & 0x7fff;
  switch(op) {
    case ITM_MVAR:
    case ITM_RTN:
      *step = findNextStep(*step);
      break;
    case ITM_END:
    case 0x7fff: // .END.
      return false;
    case CST_18:
    case ITM_ADD:
    case ITM_DIV:
    case ITM_SQUAREROOTX:
    case ITM_1ONX:
      runFunction(op);
      *step = findNextStep(*step);
      break;
    case ITM_LITERAL:
      if(*(*step + 1) == STRING_LONG_INTEGER) {
        longInteger_t val;
        setSystemFlag(FLAG_ASLIFT);
        longIntegerInit(val);
        xcopy(tmpString, *step + 3, *(*step + 2));
        tmpString[*(*step + 2)] = 0;
        stringToLongInteger(tmpString, 10, val);
        liftStack();
        convertLongIntegerToLongIntegerRegister(val, REGISTER_X);
        longIntegerFree(val);
      }
      #ifdef PC_BUILD
      else {
        printf("***Unimplemented type %u!\n", *(*step + 1));
        fflush(stdout);
      }
      #endif /* PC_BUILD */
      *step = findNextStep(*step);
      break;
    case ITM_RCL:
    case ITM_RCLMULT:
    case ITM_RCLADD:
    case ITM_RCLSUB:
      if(*(*step + 1) == STRING_LABEL_VARIABLE) {
        xcopy(tmpString, *step + 3, *(*step + 2));
        tmpString[*(*step + 2)] = 0;
        reallyRunFunction(op, findNamedVariable(tmpString));
      }
      #ifdef PC_BUILD
      else {
        printf("***Not a named variable %u!\n", *(*step + 1));
        fflush(stdout);
      }
      #endif /* PC_BUILD */
      *step = findNextStep(*step);
      break;
    #ifdef PC_BUILD
    default:
      printf("***Unimplemented opcode %u!\n", op);
      fflush(stdout);
      *step = findNextStep(*step);
    #endif /* PC_BUILD */
  }
  return lastErrorCode == ERROR_NONE;
}
static void _solverIteration(real34_t *res) {
  if(currentSolverStatus & SOLVER_STATUS_TVM_APPLICATION) {
    tvmEquation();
  }
  else if(currentSolverStatus & SOLVER_STATUS_USES_FORMULA) {
    parseEquation(currentFormula, EQUATION_PARSER_XEQ, tmpString, tmpString + AIM_BUFFER_LENGTH);
  }
  else {
    //
    //  NOT A COMPLETE ENGINE: TESTING PURPOSE ONLY!!
    //  The following decoder is minimally implemented ad hoc engine for testing of SOLVE feature.
    //  Replace with the complete programming system when ready.
    //
    uint8_t *step = labelList[currentSolverProgram].instructionPointer;
    lastErrorCode = ERROR_NONE;
    while(executeStep_test(&step)) {}
  }
  if(lastErrorCode == ERROR_OVERFLOW_PLUS_INF) {
    realToReal34(const_plusInfinity, res);
    lastErrorCode = ERROR_NONE;
  }
  else if(lastErrorCode == ERROR_OVERFLOW_MINUS_INF) {
    realToReal34(const_minusInfinity, res);
    lastErrorCode = ERROR_NONE;
  }
  else if(lastErrorCode != ERROR_NONE) {
    realToReal34(const_NaN, res);
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34 && getRegisterAngularMode(REGISTER_X) == amNone) {
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), res);
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal34(REGISTER_X, res);
  }
  else {
    realToReal34(const_NaN, res);
  }
}

static void _executeSolver(calcRegister_t variable, const real34_t *val, real34_t *res) {
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  real34Copy(val, REGISTER_REAL34_DATA(REGISTER_X));
  if(currentSolverStatus & SOLVER_STATUS_TVM_APPLICATION) {
    copySourceRegisterToDestRegister(REGISTER_X, variable);
  }
  else {
    reallyRunFunction(ITM_STO, variable);
    fnFillStack(NOPARAM);
  }
  _solverIteration(res);
}

static void _linearInterpolation(const real_t *a, const real_t *b, const real_t *fa, const real_t *fb, real_t *res, real_t *slope, realContext_t *realContext) {
  real_t amb, famfb;
  realSubtract(a, b, &amb, realContext);
  realSubtract(fa, fb, &famfb, realContext);
  if(slope) realDivide(&famfb, &amb, slope, realContext);
  if(res) {
    realDivide(&amb, &famfb, &amb, realContext);
    realMultiply(&amb, fb, &amb, realContext);
    realSubtract(b, &amb, res, realContext);
  }
}

static void _inverseQuadraticInterpolation(const real_t *a, const real_t *b, const real_t *c, const real_t *fa, const real_t *fb, const real_t *fc, real_t *res, realContext_t *realContext) {
  real_t val, num, den, tmp;

  realMultiply(fb, fc, &num, realContext);
  realMultiply(&num, a, &num, realContext);
  realSubtract(fa, fb, &den, realContext);
  realSubtract(fa, fc, &tmp, realContext);
  realMultiply(&den, &tmp, &den, realContext);
  realDivide(&num, &den, &val, realContext);

  realMultiply(fa, fc, &num, realContext);
  realMultiply(&num, b, &num, realContext);
  realSubtract(fb, fa, &den, realContext);
  realSubtract(fb, fc, &tmp, realContext);
  realMultiply(&den, &tmp, &den, realContext);
  realDivide(const_1, &den, &den, realContext);
  realFMA(&num, &den, &val, &val, realContext);

  realMultiply(fa, fb, &num, realContext);
  realMultiply(&num, c, &num, realContext);
  realSubtract(fc, fa, &den, realContext);
  realSubtract(fc, fb, &tmp, realContext);
  realMultiply(&den, &tmp, &den, realContext);
  realDivide(const_1, &den, &den, realContext);
  realFMA(&num, &den, &val, res, realContext);
}
#endif /* TESTSUITE_BUILD */

int solver(calcRegister_t variable, const real34_t *y, const real34_t *x, real34_t *resZ, real34_t *resY, real34_t *resX) {
#ifndef TESTSUITE_BUILD
  real34_t a, b, b1, b2, fa, fb, fb1, m, s, *bp1, fbp1, tmp;
  real_t aa, bb, bb1, bb2, faa, fbb, fbb1, mm, ss, secantSlopeA, secantSlopeB, delta, deltaB, smb, tol;
  bool_t extendRange = false;
  bool_t originallyLevel = false;
  bool_t extremum = false;
  int result = SOLVER_RESULT_NORMAL;

  realCopy(const_1, &tol);
  tol.exponent -= (significantDigits == 0 || significantDigits >= 32) ? 32 : significantDigits;

  ++currentSolverNestingDepth;
  setSystemFlag(FLAG_SOLVING);

  realCopy(const_0, &delta);

  real34Copy(y, &a);
  real34Copy(y, &b1);
  real34Copy(x, &b);
  realToReal34(const_NaN, &b2);

  real34Subtract(&b, &a, &s);
  if(real34CompareAbsLessThan(&s, const34_1e_32)) {
    real34Copy(const34_1e_32, &s);
    if(real34CompareLessThan(&b, &a)) real34SetNegativeSign(&s);
    real34Subtract(&a, &s, &a);
    real34Add(&b, &s, &b);
  }

  _executeSolver(variable, &b1, &fb1);
  if(lastErrorCode != ERROR_NONE) {
    result = SOLVER_RESULT_BAD_GUESS;
  }
  real34Copy(&fb1, &fa);

  // calculation
  _executeSolver(variable, &b, &fb);
  if(lastErrorCode != ERROR_NONE) {
    result = SOLVER_RESULT_BAD_GUESS;
  }

  if(real34IsSpecial(&fb) || real34IsSpecial(&fb1)) {
    result = SOLVER_RESULT_BAD_GUESS;
  }
  else if(real34IsNegative(&fb) == real34IsNegative(&fb1)) {
    extendRange = true;
  }
  if(real34CompareEqual(&fb, &fb1)) {
    originallyLevel = true;
  }
  if(real34CompareAbsLessThan(&fa, &fb)) {
    real34Copy(&b, &tmp); real34Copy(&a, &b); real34Copy(&tmp, &a); real34Copy(&tmp, &b1);
    real34Copy(&fb, &tmp); real34Copy(&fa, &fb); real34Copy(&tmp, &fa); real34Copy(&tmp, &fb1);
  }

  do {
    // convert real34 to real
    real34ToReal(&a, &aa);
    real34ToReal(&b, &bb);
    real34ToReal(&b1, &bb1);
    real34ToReal(&b2, &bb2);
    real34ToReal(&fa, &faa);
    real34ToReal(&fb, &fbb);
    real34ToReal(&fb1, &fbb1);

    // pre-calculation
    if(realIsSpecial(&bb2)) {
      realSubtract(&bb, &bb1, &deltaB, &ctxtReal39);
    }
    else {
      realSubtract(&bb1, &bb2, &deltaB, &ctxtReal39);
    }
    realSetPositiveSign(&deltaB);

    _linearInterpolation(&aa, &bb, &faa, &fbb, NULL, &secantSlopeA, &ctxtReal39);

    // interpolation
    if(!(realCompareEqual(&faa, &fbb) || realCompareEqual(&faa, &fbb1) || realCompareEqual(&fbb, &fbb1))) { // inverse quadratic interpolation
      _linearInterpolation(&bb, &bb1, &fbb, &fbb1, NULL, &secantSlopeB, &ctxtReal39);
      _inverseQuadraticInterpolation(&aa, &bb, &bb1, &faa, &fbb, &fbb1, &ss, &ctxtReal39);
    }
    else { // linear interpolation
      _linearInterpolation(&bb, &bb1, &fbb, &fbb1, &ss, &secantSlopeB, &ctxtReal39);
    }
    realToReal34(&ss, &s);

    realSubtract(&ss, &bb, &smb, &ctxtReal39);
    realMultiply(&smb, const_2, &smb, &ctxtReal39);
    realSetPositiveSign(&smb);

    // bisection
    realAdd(&aa, &bb, &mm, &ctxtReal39);
    realMultiply(&mm, const_1on2, &mm, &ctxtReal39);
    realToReal34(&mm, &m);

    // next point
    if(extendRange) {
      if(real34CompareEqual(&fb, &fa)) {
        real34Subtract(&b, &a, &s);
        if(real34CompareAbsLessThan(&s, const34_1e_32)) {
          real34Copy(const34_1e_32, &s);
          if(real34CompareLessThan(&b, &a)) real34SetNegativeSign(&s);
        }
        real34Subtract(&a, &s, &a);
        _executeSolver(variable, &a, &fa);
        real34Add(&b, &s, &s);
      }
      else if(!real34CompareEqual(&b, &a)) {
        real34Subtract(&b, &a, &s);
        real34Add(&s, &b, &s);
      }
      else if(real34IsNegative(&fb)) {
        real34Multiply(&b, const34_1e_32, &s);
        real34Subtract(&b, &s, &s);
      }
      else {
        real34Multiply(&b, const34_1e_32, &s);
        if(real34CompareAbsLessThan(&s, const34_1e_32)) {
          real34Copy(const34_1e_32, &s);
          if(real34CompareLessThan(&b, &a)) real34SetNegativeSign(&s);
        }
        if(real34IsNegative(&fb)) {
          real34Subtract(&b, &s, &s);
        }
        else {
          real34Add(&b, &s, &s);
        }
      }
      bp1 = &s;
    }
    else if(real34CompareEqual(&b, &s)) {
      bp1 = &m;
    }
    else if(!real34IsSpecial(&s) && ((real34CompareLessThan(&b, &s) && real34CompareLessThan(&s, &m)) || (real34CompareLessThan(&m, &s) && real34CompareLessThan(&s, &m)))) {
      if(realCompareLessThan(&delta, &deltaB) && realCompareLessThan(&smb, &deltaB))
        bp1 = &s;
      else
        bp1 = &m;
    }
    else {
      bp1 = &m;
    }

    // calculation
    _executeSolver(variable, bp1, &fbp1);
    if(extendRange && (realIsNegative(&secantSlopeA) != realIsNegative(&secantSlopeB)) && !realIsZero(&secantSlopeA) && !realIsZero(&secantSlopeB)) {
      extendRange = false;
      extremum = (real34IsNegative(&fb) == real34IsNegative(&fbp1));
    }

    if(!real34IsSpecial(bp1) && !real34IsSpecial(&fbp1)) {
      if(extendRange) {
        if((real34IsNegative(&fb) != real34IsNegative(&fbp1)) || (real34IsNegative(&fb) != real34IsNegative(&fa))) {
          extendRange = false;
          originallyLevel = false;
        }
        if((real34IsNegative(&fa) == real34IsNegative(&fbp1)) && real34CompareAbsLessThan(&fb, &fbp1)) {
          extendRange = false;
          originallyLevel = false;
          extremum = true;
        }
      }
      else if(real34IsNegative(&fa) == real34IsNegative(&fbp1)) {
        real34Copy(&b, &a);
        real34Copy(&fb, &fa);
      }
      else {
        extendRange = false;
        extremum = false;
        if(!real34CompareEqual(&fb, &fa)) {
          originallyLevel = false;
        }
      }

      if(real34CompareAbsLessThan(&fa, &fbp1)) {
        real34Copy(bp1, &tmp); real34Copy(&a, bp1); real34Copy(&tmp, &a);
        real34Copy(&fbp1, &tmp); real34Copy(&fa, &fbp1); real34Copy(&tmp, &fa);
        real34Copy(&a, &b); real34Copy(&fa, &fb);
      }

      if(bp1 == &s) {
        realToReal34(const_NaN, &b2);
      }
      else {
        real34Copy(&b1, &b2);
      }
      real34Copy(&b, &b1);
      real34Copy(&fb, &fb1);
      real34Copy(bp1, &b);
      real34Copy(&fbp1, &fb);
    }

    else if(originallyLevel && (real34IsInfinite(&b) || real34IsInfinite(&a))) {
      result = SOLVER_RESULT_CONSTANT;
    }
    else if(extendRange) {
      extendRange = false;
      originallyLevel = false;
      extremum = true;
    }
    else if(real34IsNegative(&fa) != real34IsNegative(&fb)) {
      result = SOLVER_RESULT_SIGN_REVERSAL;
    }
    else {
      result = SOLVER_RESULT_EXTREMUM;
    }

    real34ToReal(&b, &bb);
    real34ToReal(&b1, &bb1);

  } while(result == SOLVER_RESULT_NORMAL &&
          (real34IsSpecial(&b2) || !real34CompareEqual(&b1, &b2) || !(extendRange || extremum || WP34S_RelativeError(&bb, &bb1, &tol, &ctxtReal39))) &&
          (originallyLevel || !((!extendRange && WP34S_RelativeError(&bb, &bb1, &tol, &ctxtReal39)) || real34CompareEqual(&b, &b1) || real34CompareEqual(&fb, const34_0)))
         );

  if((extendRange && !originallyLevel) || extremum) {
    result = SOLVER_RESULT_EXTREMUM;
  }

  if((--currentSolverNestingDepth) == 0)
    clearSystemFlag(FLAG_SOLVING);

  real34Copy(&fb, resZ);
  real34Copy(&b1, resY);
  real34Copy(&b, resX);
  return result;
#else /* TESTSUITE_BUILD */
  return SOLVER_RESULT_NORMAL;
#endif /* TESTSUITE_BUILD */
}
