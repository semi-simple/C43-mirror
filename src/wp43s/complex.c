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
 * \file complex.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief Unit vector of a complex or a matrix
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnUnitVector(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnUnitVector");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    real16_t temp;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real16Multiply(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X), &temp);
    real16FMA(POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL16_SIZE, &temp, &temp);
    real16SquareRoot(&temp, &temp);
    real16Divide(POINTER_TO_REGISTER_DATA(REGISTER_X), &temp, POINTER_TO_REGISTER_DATA(REGISTER_X));
    real16Divide(POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL16_SIZE, &temp, POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL16_SIZE);

    refreshStack();
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    real34_t temp;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real34Multiply(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X), &temp);
    real34FMA(POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL34_SIZE, &temp, &temp);
    real34SquareRoot(&temp, &temp);
    real34Divide(POINTER_TO_REGISTER_DATA(REGISTER_X), &temp, POINTER_TO_REGISTER_DATA(REGISTER_X));
    real34Divide(POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL34_SIZE, &temp, POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL34_SIZE);

    refreshStack();
  }
  #ifdef PC_BUILD
  else if(getRegisterDataType(REGISTER_X) == dtReal16Matrix || getRegisterDataType(REGISTER_X) == dtComplex16Matrix) {
    showInfoDialog("In function fnUnitVector:", "UNITV for matrix:", "to be coded", NULL);
  }
  #endif
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "UNITV doesn't work an %s!", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnUnitVector:", errorMessage, NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnUnitVector");
  #endif
}



/********************************************//**
 * \brief Conjugate of a complex
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConjugate(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnConjugate");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real16ChangeSign(POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL16_SIZE);
    refreshStack();
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real34ChangeSign(POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL34_SIZE);
    refreshStack();
  }
  #ifdef PC_BUILD
  else if(getRegisterDataType(REGISTER_X) == dtComplex16Matrix) {
    showInfoDialog("In function fnConjugate:", "conj for a complex matrix:", "to be coded", NULL);
  }
  #endif
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "conj doesn't work an %s!", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnConjugate:", errorMessage, NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnConjugate");
  #endif
}



/********************************************//**
 * \brief Swaps the real part and the imaginary part of a complex
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSwapRealImaginary(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnSwapRealImaginary");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real16Copy(POINTER_TO_REGISTER_DATA(REGISTER_L) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_X)               );
    real16Copy(POINTER_TO_REGISTER_DATA(REGISTER_L)               , POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL16_SIZE);
    refreshStack();
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_L) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_X)               );
    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_L)               , POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL34_SIZE);
    refreshStack();
  }
  #ifdef PC_BUILD
  else if(getRegisterDataType(REGISTER_X) == dtComplex16Matrix) {
    showInfoDialog("In function fnSwapRealImaginary:", "Re" STD_LEFT_RIGHT_ARROWS "Im for a complex matrix:", "to be coded", NULL);
  }
  #endif
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Re" STD_LEFT_RIGHT_ARROWS "Im doesn't work on %s!", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnSwapRealImaginary:", errorMessage, NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnSwapRealImaginary");
  #endif
}



/********************************************//**
 * \brief Returns the real part of a complex or a complex matrix
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnRealPart(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnRealPart");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, 0);
    real16Copy(POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_X));
    refreshStack();
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, 0);
    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_X));
    refreshStack();
  }
  #ifdef PC_BUILD
  else if(getRegisterDataType(REGISTER_X) == dtComplex16Matrix) {
    showInfoDialog("In function fnRealPart:", "Re for a complex matrix:", "to be coded", NULL);
  }
  #endif
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Re doesn't work on %s!", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnRealPart:", errorMessage, NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnRealPart");
  #endif
}



/********************************************//**
 * \brief Returns the imaginary part of a complex or a complex matrix
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnImaginaryPart(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnImaginaryPart");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, 0);
    real16Copy(POINTER_TO_REGISTER_DATA(REGISTER_L) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_X));
    refreshStack();
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, 0);
    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_L) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_X));
    refreshStack();
  }
  #ifdef PC_BUILD
  else if(getRegisterDataType(REGISTER_X) == dtComplex16Matrix) {
    showInfoDialog("In function fnImaginaryPart:", "Im for a complex matrix:", "to be coded", NULL);
  }
  #endif
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Im doesn't work on %s!", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnImaginaryPart:", errorMessage, NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnImaginaryPart");
  #endif
}
