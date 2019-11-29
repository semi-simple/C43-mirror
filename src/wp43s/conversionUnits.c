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
 * \file conversionUnits.c
 ***********************************************/

#include "wp43s.h"



static void unitConversion(const realIc_t * const coefficient, uint16_t multiplyDivide) {
  realIc_t reX;
  bool_t real16;

  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &reX);
    real16 = true;
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &reX);
    real16 = false;
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToRealIc(REGISTER_X, &reX);
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
    real16 = true;
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function unitConversion:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif
    restoreStack();
    refreshStack();
    return;
  }

  if(realIcIsNaN(&reX)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function unitConversion:", "cannot use NaN as X input of unitConversion", NULL, NULL);
    #endif
    restoreStack();
    refreshStack();
    return;
  }

  if(multiplyDivide == multiply) {
    realIcMultiply(&reX, coefficient, &reX);
  }
  else {
    realIcDivide(&reX, coefficient, &reX);
  }

  if(real16) {
    realIcToReal16(&reX, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    realIcToReal34(&reX, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, -1, -1, -1);
}



/********************************************//**
 * \brief Converts °Celcius to °Fahrenheit: (°Celcius * 1,8) + 32.
 * Refreshes the stack. This is the exact formula.
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCvtCToF(uint16_t unusedParamButMandatory) {
  realIc_t reX;
  bool_t real16;

  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &reX);
    real16 = true;
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &reX);
    real16 = false;
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToRealIc(REGISTER_X, &reX);
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
    real16 = true;
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtCToF:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif
    restoreStack();
    refreshStack();
    return;
  }

  if(realIcIsNaN(&reX)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtCToF:", "cannot use NaN as X input of fnCvtCToF", NULL, NULL);
    #endif
    restoreStack();
    refreshStack();
    return;
  }

  realIcFMA(&reX, const_9on5, const_32, &reX);

  if(real16) {
    realIcToReal16(&reX, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    realIcToReal34(&reX, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, -1, -1, -1);
}



/********************************************//**
 * \brief Converts °Fahrenheit to °Celcius: (°Fahrenheit - 32) / 1,8.
 * Refreshes the stack. This is the exact formula.
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCvtFToC(uint16_t unusedParamButMandatory) {
  realIc_t reX;
  bool_t real16;

  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &reX);
    real16 = true;
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &reX);
    real16 = false;
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToRealIc(REGISTER_X, &reX);
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
    real16 = true;
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtFToC:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif
    restoreStack();
    refreshStack();
    return;
  }

  if(realIcIsNaN(&reX)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtFToC:", "cannot use NaN as X input of fnCvtFToC", NULL, NULL);
    #endif
    restoreStack();
    refreshStack();
    return;
  }

  realIcSubtract(&reX, const_32, &reX);
  realIcDivide(&reX, const_9on5, &reX);

  if(real16) {
    realIcToReal16(&reX, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    realIcToReal34(&reX, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, -1, -1, -1);
}


void fnCvtYearS(uint16_t multiplyDivide) {
  unitConversion(const_YearToS, multiplyDivide);
}


void fnCvtCalJ(uint16_t multiplyDivide) {
  unitConversion(const_CalToJ, multiplyDivide);
}


void fnCvtBtuJ(uint16_t multiplyDivide) {
  unitConversion(const_BtuToJ, multiplyDivide);
}


void fnCvtWhJ(uint16_t multiplyDivide) {
  unitConversion(const_WhToJ, multiplyDivide);
}


void fnCvtHpeW(uint16_t multiplyDivide) {
  unitConversion(const_HpeToW, multiplyDivide);
}


void fnCvtHpmW(uint16_t multiplyDivide) {
  unitConversion(const_HpmToW, multiplyDivide);
}


void fnCvtHpukW(uint16_t multiplyDivide) {
  unitConversion(const_HpukToW, multiplyDivide);
}


void fnCvtLbfN(uint16_t multiplyDivide) {
  unitConversion(const_LbfToN, multiplyDivide);
}


void fnCvtBarPa(uint16_t multiplyDivide) {
  unitConversion(const_BarToPa, multiplyDivide);
}


void fnCvtPsiPa(uint16_t multiplyDivide) {
  unitConversion(const_PsiToPa, multiplyDivide);
}


void fnCvtInhgPa(uint16_t multiplyDivide) {
  unitConversion(const_InhgToPa, multiplyDivide);
}

void fnCvtmmhgPa(uint16_t multiplyDivide) {          //JM mm.Hg
  unitConversion(const_InhgToPa, multiplyDivide);
  if (multiplyDivide == divide) {
    unitConversion(const_InchToM, multiply);
    liftStack();
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
    stringToReal16("1000", REGISTER_REAL16_DATA(REGISTER_X));
    fnMultiply(0);    
  }
  else {
    unitConversion(const_InchToM, divide);
    liftStack();
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
    stringToReal16("1000", REGISTER_REAL16_DATA(REGISTER_X));
    fnDivide(0);    
  }

}

void fnCvtTorrPa(uint16_t multiplyDivide) {
  unitConversion(const_TorrToPa, multiplyDivide);
}


void fnCvtAtmPa(uint16_t multiplyDivide) {
  unitConversion(const_AtmToPa, multiplyDivide);
}


void fnCvtLbKg(uint16_t multiplyDivide) {
  unitConversion(const_LbToKg, multiplyDivide);
}


void fnCvtCwtKg(uint16_t multiplyDivide) {
  unitConversion(const_CwtToKg, multiplyDivide);
}


void fnCvtOzKg(uint16_t multiplyDivide) {
  unitConversion(const_OzToKg, multiplyDivide);
}


void fnCvtStoneKg(uint16_t multiplyDivide) {
  unitConversion(const_StoneToKg, multiplyDivide);
}


void fnCvtShortcwtKg(uint16_t multiplyDivide) {
  unitConversion(const_ShortcwtToKg, multiplyDivide);
}


void fnCvtTrozKg(uint16_t multiplyDivide) {
  unitConversion(const_TrozToKg, multiplyDivide);
}


void fnCvtTonKg(uint16_t multiplyDivide) {
  unitConversion(const_TonToKg, multiplyDivide);
}


void fnCvtShorttonKg(uint16_t multiplyDivide) {
  unitConversion(const_ShorttonToKg, multiplyDivide);
}


void fnCvtCaratKg(uint16_t multiplyDivide) {
  unitConversion(const_CaratToKg, multiplyDivide);
}


void fnCvtAuM(uint16_t multiplyDivide) {
  unitConversion(const_AuToM, multiplyDivide);
}


void fnCvtMiM(uint16_t multiplyDivide) {
  unitConversion(const_MiToM, multiplyDivide);
}


void fnCvtLyM(uint16_t multiplyDivide) {
  unitConversion(const_LyToM, multiplyDivide);
}


void fnCvtNmiM(uint16_t multiplyDivide) {
  unitConversion(const_NmiToM, multiplyDivide);
}


void fnCvtFtM(uint16_t multiplyDivide) {
  unitConversion(const_FtToM, multiplyDivide);
}


void fnCvtPcM(uint16_t multiplyDivide) {
  unitConversion(const_PcToM, multiplyDivide);
}


void fnCvtInchM(uint16_t multiplyDivide) {
  unitConversion(const_InchToM, multiplyDivide);
}


void fnCvtSfeetM(uint16_t multiplyDivide) {
  unitConversion(const_SfeetToM, multiplyDivide);
}


void fnCvtYardM(uint16_t multiplyDivide) {
  unitConversion(const_YardToM, multiplyDivide);
}


void fnCvtPointM(uint16_t multiplyDivide) {
  unitConversion(const_PointToM, multiplyDivide);
}


void fnCvtFathomM(uint16_t multiplyDivide) {
  unitConversion(const_FathomToM, multiplyDivide);
}


void fnCvtGalukM3(uint16_t multiplyDivide) {
  unitConversion(const_GalukToM3, multiplyDivide);
}


void fnCvtGalusM3(uint16_t multiplyDivide) {
  unitConversion(const_GalusToM3, multiplyDivide);
}


void fnCvtFlozukM3(uint16_t multiplyDivide) {
  unitConversion(const_FlozukToM3, multiplyDivide);
}


void fnCvtFlozusM3(uint16_t multiplyDivide) {
  unitConversion(const_FlozusToM3, multiplyDivide);
}


void fnCvtBarrelM3(uint16_t multiplyDivide) {
  unitConversion(const_BarrelToM3, multiplyDivide);
}


void fnCvtQuartM3(uint16_t multiplyDivide) {
  unitConversion(const_QuartToM3, multiplyDivide);
}


void fnCvtAcreM2(uint16_t multiplyDivide) {
  unitConversion(const_AccreToM2, multiplyDivide);
}


void fnCvtAcreusM2(uint16_t multiplyDivide) {
  unitConversion(const_AccreusToM2, multiplyDivide);
}


void fnCvtHectareM2(uint16_t multiplyDivide) {
  unitConversion(const_10000, multiplyDivide);
}



/********************************************//**
 * \brief Converts power or field ratio to dB
 * dB = (10 or 20) * log10((power or field) ratio) this is the exact formula
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCvtRatioDb(uint16_t tenOrTwenty) { // ten: power ratio   twenty: field ratio
  realIc_t reX;
  bool_t real16;

  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &reX);
    real16 = true;
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &reX);
    real16 = false;
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToRealIc(REGISTER_X, &reX);
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
    real16 = true;
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtRatioDb:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif
    restoreStack();
    refreshStack();
    return;
  }

  if(realIcIsNaN(&reX)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtRatioDb:", "cannot use NaN as X input of fnCvtRatioDb", NULL, NULL);
    #endif
    restoreStack();
    refreshStack();
    return;
  }

  WP34S_Log10(&reX, &reX);
  realIcMultiply(&reX, (tenOrTwenty == 10 ? const_10 : const_20), &reX);

  if(real16) {
    realIcToReal16(&reX, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    realIcToReal34(&reX, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, -1, -1, -1);
}



/********************************************//**
 * \brief Converts dB to power or field ratio
 * (power or field) ratio = 10^(dB / 10 or 20) this is the exact formula
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCvtDbRatio(uint16_t tenOrTwenty) { // ten: power ratio   twenty: field ratio
  realIc_t reX;
  bool_t real16;

  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &reX);
    real16 = true;
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &reX);
    real16 = false;
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToRealIc(REGISTER_X, &reX);
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
    real16 = true;
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtRatioDb:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif
    restoreStack();
    refreshStack();
    return;
  }

  if(realIcIsNaN(&reX)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtRatioDb:", "cannot use NaN as X input of fnCvtRatioDb", NULL, NULL);
    #endif
    restoreStack();
    refreshStack();
    return;
  }

  realIcDivide(&reX, (tenOrTwenty == 10 ? const_10 : const_20), &reX);
  realIcPower(const_10, &reX, &reX);

  if(real16) {
    realIcToReal16(&reX, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    realIcToReal34(&reX, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, -1, -1, -1);
}
