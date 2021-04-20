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

#include "conversionUnits.h"

#include "browsers/fontBrowser.h"
#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"



static void unitConversion(const real_t * const coefficient, uint16_t multiplyDivide) {
  real_t reX;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &reX);
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal(REGISTER_X, &reX, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function unitConversion:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  if(multiplyDivide == multiply) {
    realMultiply(&reX, coefficient, &reX, &ctxtReal39);
  }
  else {
    realDivide(&reX, coefficient, &reX, &ctxtReal39);
  }

  realToReal34(&reX, REGISTER_REAL34_DATA(REGISTER_X));

  adjustResult(REGISTER_X, false, false, -1, -1, -1);
}



/********************************************//**
 * \brief Converts °Celcius to °Fahrenheit: (°Celcius * 1,8) + 32.
 * Refreshes the stack. This is the exact formula.
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnCvtCToF(uint16_t unusedButMandatoryParameter) {
  real_t reX;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &reX);
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal(REGISTER_X, &reX, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function fnCvtCToF:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  realFMA(&reX, const_9on5, const_32, &reX, &ctxtReal39);

  realToReal34(&reX, REGISTER_REAL34_DATA(REGISTER_X));

  adjustResult(REGISTER_X, false, false, -1, -1, -1);
}



/********************************************//**
 * \brief Converts °Fahrenheit to °Celcius: (°Fahrenheit - 32) / 1,8.
 * Refreshes the stack. This is the exact formula.
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnCvtFToC(uint16_t unusedButMandatoryParameter) {
  real_t reX;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &reX);
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal(REGISTER_X, &reX, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function fnCvtFToC:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  realSubtract(&reX, const_32, &reX, &ctxtReal39);
  realDivide(&reX, const_9on5, &reX, &ctxtReal39);

  realToReal34(&reX, REGISTER_REAL34_DATA(REGISTER_X));

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


void fnCvtMmhgPa(uint16_t multiplyDivide) {
  unitConversion(const_MmhgToPa, multiplyDivide);
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


void fnCvtOzG(uint16_t multiplyDivide) {
  unitConversion(const_OzToG, multiplyDivide);
}


void fnCvtStoneKg(uint16_t multiplyDivide) {
  unitConversion(const_StoneToKg, multiplyDivide);
}


void fnCvtShortcwtKg(uint16_t multiplyDivide) {
  unitConversion(const_ShortcwtToKg, multiplyDivide);
}


void fnCvtTrozG(uint16_t multiplyDivide) {
  unitConversion(const_TrozToG, multiplyDivide);
}


void fnCvtTonKg(uint16_t multiplyDivide) {
  unitConversion(const_TonToKg, multiplyDivide);
}


void fnCvtShorttonKg(uint16_t multiplyDivide) {
  unitConversion(const_ShorttonToKg, multiplyDivide);
}


void fnCvtCaratG(uint16_t multiplyDivide) {
  unitConversion(const_CaratToG, multiplyDivide);
}


void fnCvtLiangKg(uint16_t multiplyDivide) {
  unitConversion(const_20, multiplyDivide);
}


void fnCvtJinKg(uint16_t multiplyDivide) {
  unitConversion(const_2, multiplyDivide);
}


void fnCvtAuM(uint16_t multiplyDivide) {
  unitConversion(const_AuToM, multiplyDivide);
}


void fnCvtMiKm(uint16_t multiplyDivide) {
  unitConversion(const_MiToKm, multiplyDivide);
}


void fnCvtLyM(uint16_t multiplyDivide) {
  unitConversion(const_LyToM, multiplyDivide);
}


void fnCvtNmiKm(uint16_t multiplyDivide) {
  unitConversion(const_NmiToKm, multiplyDivide);
}


void fnCvtFtM(uint16_t multiplyDivide) {
  unitConversion(const_FtToM, multiplyDivide);
}


void fnCvtPcM(uint16_t multiplyDivide) {
  unitConversion(const_PcToM, multiplyDivide);
}


void fnCvtInchMm(uint16_t multiplyDivide) {
  unitConversion(const_InchToMm, multiplyDivide);
}


void fnCvtSfeetM(uint16_t multiplyDivide) {
  unitConversion(const_SfeetToM, multiplyDivide);
}


void fnCvtYardM(uint16_t multiplyDivide) {
  unitConversion(const_YardToM, multiplyDivide);
}


void fnCvtPointMm(uint16_t multiplyDivide) {
  unitConversion(const_PointToMm, multiplyDivide);
}


void fnCvtFathomM(uint16_t multiplyDivide) {
  unitConversion(const_FathomToM, multiplyDivide);
}


void fnCvtLiM(uint16_t multiplyDivide) {
  unitConversion(const_LiToM, multiplyDivide);
}


void fnCvtChiM(uint16_t multiplyDivide) {
  unitConversion(const_3, multiplyDivide);
}


void fnCvtYinM(uint16_t multiplyDivide) {
  unitConversion(const_YinToM, multiplyDivide);
}


void fnCvtCunM(uint16_t multiplyDivide) {
  unitConversion(const_CunToM, multiplyDivide);
}


void fnCvtZhangM(uint16_t multiplyDivide) {
  unitConversion(const_ZhangToM, multiplyDivide);
}


void fnCvtFenM(uint16_t multiplyDivide) {
  unitConversion(const_FenToM, multiplyDivide);
}


void fnCvtGalukL(uint16_t multiplyDivide) {
  unitConversion(const_GalukToL, multiplyDivide);
}


void fnCvtGalusL(uint16_t multiplyDivide) {
  unitConversion(const_GalusToL, multiplyDivide);
}


void fnCvtFlozukMl(uint16_t multiplyDivide) {
  unitConversion(const_FlozukToMl, multiplyDivide);
}


void fnCvtFlozusMl(uint16_t multiplyDivide) {
  unitConversion(const_FlozusToMl, multiplyDivide);
}


void fnCvtBarrelM3(uint16_t multiplyDivide) {
  unitConversion(const_BarrelToM3, multiplyDivide);
}


void fnCvtQuartL(uint16_t multiplyDivide) {
  unitConversion(const_QuartToL, multiplyDivide);
}


void fnCvtAcreHa(uint16_t multiplyDivide) {
  unitConversion(const_AccreToHa, multiplyDivide);
}


void fnCvtAcreusHa(uint16_t multiplyDivide) {
  unitConversion(const_AccreusToHa, multiplyDivide);
}


void fnCvtHectareM2(uint16_t multiplyDivide) {
  unitConversion(const_10000, multiplyDivide);
}


void fnCvtMuM2(uint16_t multiplyDivide) {
  unitConversion(const_MuToM2, multiplyDivide);
}


void fnCvtLbfftNm(uint16_t multiplyDivide) {
  unitConversion(const_LbfftToNm, multiplyDivide);
}



/********************************************//**
 * \brief Converts power or field ratio to dB
 * dB = (10 or 20) * log10((power or field) ratio) this is the exact formula
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnCvtRatioDb(uint16_t tenOrTwenty) { // ten: power ratio   twenty: field ratio
  real_t reX;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &reX);
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal(REGISTER_X, &reX, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function fnCvtRatioDb:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  WP34S_Log10(&reX, &reX, &ctxtReal39);
  realMultiply(&reX, (tenOrTwenty == 10 ? const_10 : const_20), &reX, &ctxtReal39);

  realToReal34(&reX, REGISTER_REAL34_DATA(REGISTER_X));

  adjustResult(REGISTER_X, false, false, -1, -1, -1);
}



/********************************************//**
 * \brief Converts dB to power or field ratio
 * (power or field) ratio = 10^(dB / 10 or 20) this is the exact formula
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnCvtDbRatio(uint16_t tenOrTwenty) { // ten: power ratio   twenty: field ratio
  real_t reX;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &reX);
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal(REGISTER_X, &reX, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function fnCvtRatioDb:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  realDivide(&reX, (tenOrTwenty == 10 ? const_10 : const_20), &reX, &ctxtReal39);
  realPower(const_10, &reX, &reX, &ctxtReal39);

  realToReal34(&reX, REGISTER_REAL34_DATA(REGISTER_X));

  adjustResult(REGISTER_X, false, false, -1, -1, -1);
}
