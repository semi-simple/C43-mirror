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
 * \file fractions.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief Sets the denominator mode
 *
 * \param[in] denMode uint16_t Denominator mode
 * \return void
 ***********************************************/
void fnDenMode(uint16_t denMode) {
  denominatorMode = denMode;
  showFracMode();
  refreshStack();
}



void fnDenMax(uint16_t unusedParamButMandatory) {
  realIc_t reX;

  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &reX);
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &reX);
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToRealIc(REGISTER_X, &reX);
  }
  else if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    convertShortIntegerRegisterToRealIc(REGISTER_X, &reX);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnDenMax:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif
    restoreStack();
    refreshStack();
    return;
  }

  if(realIcIsNaN(&reX)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnDenMax:", "cannot use NaN as X input of fnDenMax", NULL, NULL);
    #endif
    restoreStack();
    refreshStack();
    return;
  }

  if(realIcIsSpecial(&reX) || realIcCompareLessThan(&reX, const_1) || realIcCompareGreaterEqual(&reX, const_9999)) {
    denMax = DM_DENMAX;
  }
  else {
    int32_t den;

    realIcToInt32(&reX, den);

    if(den == 1) {
      longInteger_t lgInt;

      longIntegerInit(lgInt);
      uIntToLongInteger(denMax, lgInt);
      convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
      longIntegerFree(lgInt);
    }
    else {
      denMax = den;
    }
  }

  showFracMode();
  refreshStack();
}



/********************************************//**
 * \brief Sets the fraction type
 *
 * \param[in] fractionType uint16_t Fraction type
 * \return void
 ***********************************************/
void fnFractionType(uint16_t ft) {
  fractionType = ft;
  displayRealAsFraction = true;
  refreshStack();
}



void fraction(calcRegister_t regist, int16_t *sign, uint64_t *intPart, uint64_t *numer, uint64_t *denom, int16_t *lessEqualGreater) {
  // temp0 = fractional_part(absolute_value(real number))
  // temp1 = continued fraction calculation --> factional_part(1 / temp1)  initialized with temp0
  // delta = difference between the best faction and the real number

  //printf("0 regist = "); printRegisterToConsole(regist, 0); printf("\n");
  real34_t temp0;

  if(getRegisterDataType(regist) == dtReal16) {
    real16ToReal34(REGISTER_REAL16_DATA(regist), &temp0);
  }
  else if(getRegisterDataType(regist) == dtReal34) {
    real34Copy(REGISTER_REAL34_DATA(regist), &temp0);
  }
  else {
    #ifdef PC_BUILD
      sprintf(errorMessage, "%s cannot be shown as a fraction!", getRegisterDataTypeName(regist, true, false));
      showInfoDialog("In function fraction:", errorMessage, NULL, NULL);
    #endif
    *sign             = 0;
    *intPart          = 0;
    *numer            = 0;
    *denom            = 0;
    *lessEqualGreater = 0;

    return;
  }

  if(real34IsZero(&temp0)) {
    *sign             = 0;
    *intPart          = 0;
    *numer            = 0;
    *denom            = 1;
    *lessEqualGreater = 0;

    return;
  }

  //printf("1 temp0 = "); printReal34ToConsole(&temp0); printf("\n");
  if(real34IsNegative(&temp0)) {
    *sign = -1;
    real34ChangeSign(&temp0);
  }
  else {
    *sign = 1;
  }

  real34_t delta, temp3;
  uInt32ToReal34(9999, &delta);
  //printf("2 delta = "); printReal34ToConsole(&delta); printf("\n");

  *intPart = real34ToUInt32(&temp0);
  uInt32ToReal34(*intPart, &temp3);
  real34Subtract(&temp0, &temp3, &temp0);
  //printf("3 partie_decimale = temp0 = "); printReal34ToConsole(&temp0); printf("\n");

  //*******************
  //* Any denominator *
  //*******************
  if(denominatorMode == DM_ANY) {
    uint64_t iPart[20], ex, bestNumer=0, bestDenom=1;
    uint32_t invalidOperation=0;
    int16_t i, j;

    real34_t temp1, temp4;

    // Calculate the continued fraction
    *denom = 1;
    i = 0;
    iPart[0] = *intPart;

    real34Copy(&temp0, &temp1);
    //printf("4 partie_decimale = temp0 = "); printReal34ToConsole(&temp0); printf("\n");

    if(real34CompareAbsLessThan(&temp1, const34_1e_6)) {
      real34Zero(&temp1);
    }

    while(*denom < denMax && !real34IsZero(&temp1) && !invalidOperation) {
      real34Divide(const34_1, &temp1, &temp1);
      //printf("  5 1/partie_decimale = temp1 = "); printReal34ToConsole(&temp1); printf("\n");
      iPart[++i] = real34ToUInt32(&temp1);
      uInt32ToReal34(iPart[i], &temp3);
      invalidOperation = decContextGetStatus(&ctxtReal34) & DEC_Invalid_operation;
      decContextClearStatus(&ctxtReal34, DEC_Invalid_operation);
      real34Subtract(&temp1, &temp3, &temp1);
      if(real34CompareAbsLessThan(&temp1, const34_1e_6)) {
        real34Zero(&temp1);
      }
      //printf("  6 partie_decimale de 1/partie_decimale = temp1 = "); printReal34ToConsole(&temp1); printf("\n");

      *numer = 1;
      *denom = iPart[i];
      //printf("  7 numer=%" FMT64U " denom=%" FMT64U "\n", *numer, *denom);
      for(j=i; j>1; j--) {
        *numer += *denom * iPart[j-1];
        ex = *numer; *numer = *denom; *denom = ex;
        //printf("    8 numer=%" FMT64U " denom=%" FMT64U "\n", *numer, *denom);
      }
      //printf("  9 numer=%" FMT64U " denom=%" FMT64U "\n", *numer, *denom);

      if(*denom <= denMax) {
        uInt32ToReal34(*numer, &temp3);
        //printf("A partie_decimale = temp3 = "); printReal34ToConsole(&temp3); printf("\n");
        uInt32ToReal34(*denom, &temp4);
        //printf("B partie_decimale = temp4 = "); printReal34ToConsole(&temp4); printf("\n");

        //printf("   C temp3 "); printReal34ToConsole(&temp3); printf(" / temp4 "); printReal34ToConsole(&temp4);
        real34Divide(&temp3, &temp4, &temp3);
        //printf(" = temp3 "); printReal34ToConsole(&temp3); printf("\n");


        //printf("   D temp3 "); printReal34ToConsole(&temp3); printf(" - temp0 "); printReal34ToConsole(&temp0);
        real34Subtract(&temp3, &temp0, &temp3);
        //printf(" = temp3 "); printReal34ToConsole(&temp3); printf("\n");

        real34SetPositiveSign(&temp3);
        //printf("   E temp3 = "); printReal34ToConsole(&temp3); printf("\n");

        //printf("   F temp3 "); printReal34ToConsole(&temp3); printf(" - delta "); printReal34ToConsole(&delta);
        real34Subtract(&temp3, &delta, &temp3);
        //printf(" = temp3 "); printReal34ToConsole(&temp3); printf("\n");

        if(real34IsNegative(&temp3)) {
          real34Add(&temp3, &delta, &delta);
          bestNumer = *numer;
          bestDenom = *denom;
          //printf("  G bestNumer=%" FMT64U " BestDenom=%" FMT64U "\n", bestNumer, bestDenom);
          //printf("  H delta = "); printReal34ToConsole(&delta); printf("\n");
        }
      }

      *numer = 1;
      *denom = iPart[i] + 1;
      //printf("  I numer=%" FMT64U " denom=%" FMT64U "\n", *numer, *denom);
      for(j=i; j>1; j--) {
        *numer += *denom * iPart[j-1];
        ex = *numer; *numer = *denom; *denom = ex;
        //printf("    J numer=%" FMT64U " denom=%" FMT64U "\n", *numer, *denom);
      }
      //printf("  K numer=%" FMT64U " denom=%" FMT64U "\n", *numer, *denom);

      if(*denom <= denMax) {
        uInt32ToReal34(*numer, &temp3);
        uInt32ToReal34(*denom, &temp4);
        real34Divide(&temp3, &temp4, &temp3);
        real34Subtract(&temp3, &temp0, &temp3);
        real34SetPositiveSign(&temp3);
        real34Subtract(&temp3, &delta, &temp3);
        if(real34IsNegative(&temp3)) {
          real34Add(&temp3, &delta, &delta);
          bestNumer = *numer;
          bestDenom = *denom;
          //printf("  L bestNumer=%" FMT64U " BestDenom=%" FMT64U "\n", bestNumer, bestDenom);
          //printf("  M delta = "); printReal34ToConsole(&delta); printf("\n");
        }
      }
    }

    *numer = bestNumer;
    *denom = bestDenom;

    if(*numer == 1 && *denom == 1) {
      *numer = 0;
      *intPart += 1;
    }
  }

  //*******************
  //* Fix denominator *
  //*******************
  else if(denominatorMode == DM_FIX) {
    *denom = denMax;

    uInt32ToReal34(denMax, &delta);
    real34FMA(&delta, &temp0, const34_1on2, &temp3);
    *numer = real34ToUInt32(&temp3);
  }

  //******************************
  //* Factors of max denominator *
  //******************************
  else if(denominatorMode == DM_FAC) {
    uint64_t bestNumer=0, bestDenom=1;

    real34_t temp4;

    // TODO: we can certainly do better here
    for(uint32_t i=1; i<=denMax; i++) {
      if(denMax % i == 0) {
        uInt32ToReal34(i, &temp4);
        real34FMA(&temp4, &temp0, const34_1on2, &temp3);
        *numer = real34ToUInt32(&temp3);

        uInt32ToReal34(*numer, &temp3);
        uInt32ToReal34(i, &temp4);
        real34Divide(&temp3, &temp4, &temp3);
        real34Subtract(&temp3, &temp0, &temp3);
        real34SetPositiveSign(&temp3);
        real34Subtract(&temp3, &delta, &temp3);
        if(real34IsNegative(&temp3)) {
          real34Add(&temp3, &delta, &delta);
          bestNumer = *numer;
          bestDenom = i;
        }
      }
    }

    *numer = bestNumer;
    *denom = bestDenom;
  }

  else {
    sprintf(errorMessage, "In function fraction: %d is an unexpected value for denominatorMode!", denominatorMode);
    displayBugScreen(errorMessage);
    *sign             = 0;
    *intPart          = 0;
    *numer            = 0;
    *denom            = 0;
    *lessEqualGreater = 0;

    return;
  }

  // The register value
  realIc_t r;
  if(getRegisterDataType(regist) == dtReal16) {
    real16ToRealIc(REGISTER_REAL16_DATA(regist), &r);
  }
  else if(getRegisterDataType(regist) == dtReal34) {
    real34ToRealIc(REGISTER_REAL34_DATA(regist), &r);
  }

  // The fraction value
  realIc_t f, d;
  uInt32ToRealIc(*intPart, &f);
  uInt32ToRealIc(*denom, &d);
  realIcMultiply(&f, &d, &f);
  uInt32ToRealIc(*numer, &d);
  realIcAdd(&f, &d, &f);
  uInt32ToRealIc(*denom, &d);
  realIcDivide(&f, &d, &f);
  if(*sign == -1) {
    realIcChangeSign(&f);
  }

  realIcSubtract(&f, &r, &f);

  if(realIcIsZero(&f)) {
    *lessEqualGreater = 0;
  }
  else if(realIcIsNegative(&f)) {
    *lessEqualGreater = -1;
  }
  else {
    *lessEqualGreater = 1;
  }

  if(fractionType == FT_IMPROPER) { // d/c
    *numer += *denom * *intPart;
    *intPart = 0;
  }
}
