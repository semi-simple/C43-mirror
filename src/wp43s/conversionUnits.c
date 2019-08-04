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



static void unitConversion(const real34_t * const coefficient, uint16_t multiplyDivide) {
  real34_t *pointerToRegisterXData;
  bool_t real16;

  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    real16 = true;
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real16 = false;
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real16 = true;
  }
  else {
    displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function unitConversion:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function unitConversion:", "cannot use NaN as an input of unitConversion", NULL, NULL);
    #endif
    restoreStack();
    refreshStack();
    return;
  }

  pointerToRegisterXData = REGISTER_REAL34_DATA(REGISTER_X);

  if(multiplyDivide == multiply) {
    real34Multiply(pointerToRegisterXData, coefficient, pointerToRegisterXData);
  }
  else {
    real34Divide(pointerToRegisterXData, coefficient, pointerToRegisterXData);
  }

  if(real16) {
    convertRegister34To16(REGISTER_X);
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
  unitConversion(const34_1_8, multiply);

  if(lastErrorCode == 0) {
    if(getRegisterDataType(REGISTER_X) == dtReal16) {
      real16Add(REGISTER_REAL16_DATA(REGISTER_X), const16_32, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else if(getRegisterDataType(REGISTER_X) == dtReal34) {
      real34Add(REGISTER_REAL34_DATA(REGISTER_X), const34_32, REGISTER_REAL34_DATA(REGISTER_X));
    }

    adjustResult(REGISTER_X, false, false, -1, -1, -1);
  }
}



/********************************************//**
 * \brief Converts °Fahrenheit to °Celcius: (°Fahrenheit - 32) / 1,8.
 * Refreshes the stack. This is the exact formula.
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCvtFToC(uint16_t unusedParamButMandatory) {
  calcRegister_t tempReg;

  tempReg = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, tempReg);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    real16Subtract(REGISTER_REAL16_DATA(REGISTER_X), const16_32, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34Subtract(REGISTER_REAL34_DATA(REGISTER_X), const34_32, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    longInteger_t lgInt;

    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerSubtractUInt(lgInt, 32, lgInt);
    convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
    longIntegerFree(lgInt);
  }

  unitConversion(const34_1_8, divide);

  if(lastErrorCode == 0) {
    copySourceRegisterToDestRegister(tempReg, REGISTER_L);
    freeTemporaryRegister(tempReg);

    adjustResult(REGISTER_X, false, false, -1, -1, -1);
  }
}


void fnCvtYearS(uint16_t multiplyDivide) {
  unitConversion(const34_YearToS, multiplyDivide);
}


void fnCvtCalJ(uint16_t multiplyDivide) {
  unitConversion(const34_CalToJ, multiplyDivide);
}


void fnCvtBtuJ(uint16_t multiplyDivide) {
  unitConversion(const34_BtuToJ, multiplyDivide);
}


void fnCvtWhJ(uint16_t multiplyDivide) {
  unitConversion(const34_WhToJ, multiplyDivide);
}


void fnCvtHpeW(uint16_t multiplyDivide) {
  unitConversion(const34_HpeToW, multiplyDivide);
}


void fnCvtHpmW(uint16_t multiplyDivide) {
  unitConversion(const34_HpmToW, multiplyDivide);
}


void fnCvtHpukW(uint16_t multiplyDivide) {
  unitConversion(const34_HpukToW, multiplyDivide);
}


void fnCvtLbfN(uint16_t multiplyDivide) {
  unitConversion(const34_LbfToN, multiplyDivide);
}


void fnCvtBarPa(uint16_t multiplyDivide) {
  unitConversion(const34_BarToPa, multiplyDivide);
}


void fnCvtPsiPa(uint16_t multiplyDivide) {
  unitConversion(const34_PsiToPa, multiplyDivide);
}


void fnCvtInhgPa(uint16_t multiplyDivide) {
  unitConversion(const34_InhgToPa, multiplyDivide);
}


void fnCvtTorrPa(uint16_t multiplyDivide) {
  unitConversion(const34_TorrToPa, multiplyDivide);
}


void fnCvtAtmPa(uint16_t multiplyDivide) {
  unitConversion(const34_AtmToPa, multiplyDivide);
}


void fnCvtLbKg(uint16_t multiplyDivide) {
  unitConversion(const34_LbToKg, multiplyDivide);
}


void fnCvtCwtKg(uint16_t multiplyDivide) {
  unitConversion(const34_CwtToKg, multiplyDivide);
}


void fnCvtOzKg(uint16_t multiplyDivide) {
  unitConversion(const34_OzToKg, multiplyDivide);
}


void fnCvtStoneKg(uint16_t multiplyDivide) {
  unitConversion(const34_StoneToKg, multiplyDivide);
}


void fnCvtShortcwtKg(uint16_t multiplyDivide) {
  unitConversion(const34_ShortcwtToKg, multiplyDivide);
}


void fnCvtTrozKg(uint16_t multiplyDivide) {
  unitConversion(const34_TrozToKg, multiplyDivide);
}


void fnCvtTonKg(uint16_t multiplyDivide) {
  unitConversion(const34_TonToKg, multiplyDivide);
}


void fnCvtShorttonKg(uint16_t multiplyDivide) {
  unitConversion(const34_ShorttonToKg, multiplyDivide);
}


void fnCvtCaratKg(uint16_t multiplyDivide) {
  unitConversion(const34_CaratToKg, multiplyDivide);
}


void fnCvtAuM(uint16_t multiplyDivide) {
  unitConversion(const34_AuToM, multiplyDivide);
}


void fnCvtMiM(uint16_t multiplyDivide) {
  unitConversion(const34_MiToM, multiplyDivide);
}


void fnCvtLyM(uint16_t multiplyDivide) {
  unitConversion(const34_LyToM, multiplyDivide);
}


void fnCvtNmiM(uint16_t multiplyDivide) {
  unitConversion(const34_NmiToM, multiplyDivide);
}


void fnCvtFtM(uint16_t multiplyDivide) {
  unitConversion(const34_FtToM, multiplyDivide);
}


void fnCvtPcM(uint16_t multiplyDivide) {
  unitConversion(const34_PcToM, multiplyDivide);
}


void fnCvtInchM(uint16_t multiplyDivide) {
  unitConversion(const34_InchToM, multiplyDivide);
}


void fnCvtSfeetM(uint16_t multiplyDivide) {
  unitConversion(const34_SfeetToM, multiplyDivide);
}


void fnCvtYardM(uint16_t multiplyDivide) {
  unitConversion(const34_YardToM, multiplyDivide);
}


void fnCvtPointM(uint16_t multiplyDivide) {
  unitConversion(const34_PointToM, multiplyDivide);
}


void fnCvtFathomM(uint16_t multiplyDivide) {
  unitConversion(const34_FathomToM, multiplyDivide);
}


void fnCvtGalukM3(uint16_t multiplyDivide) {
  unitConversion(const34_GalukToM3, multiplyDivide);
}


void fnCvtGalusM3(uint16_t multiplyDivide) {
  unitConversion(const34_GalusToM3, multiplyDivide);
}


void fnCvtFlozukM3(uint16_t multiplyDivide) {
  unitConversion(const34_FlozukToM3, multiplyDivide);
}


void fnCvtFlozusM3(uint16_t multiplyDivide) {
  unitConversion(const34_FlozusToM3, multiplyDivide);
}


void fnCvtBarrelM3(uint16_t multiplyDivide) {
  unitConversion(const34_BarrelToM3, multiplyDivide);
}


void fnCvtQuartM3(uint16_t multiplyDivide) {
  unitConversion(const34_QuartToM3, multiplyDivide);
}


void fnCvtAcreM2(uint16_t multiplyDivide) {
  unitConversion(const34_AccreToM2, multiplyDivide);
}


void fnCvtAcreusM2(uint16_t multiplyDivide) {
  unitConversion(const34_AccreusToM2, multiplyDivide);
}



/********************************************//**
 * \brief Converts power or field ratio to dB
 * dB = (10 or 20) * log10((power or field) ratio) this is the exact formula
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCvtRatioDb(uint16_t tenOrTwenty) { // ten: power ratio   twenty: field ratio
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnCvtRatioDb:", "cannot use NaN as an input of fnCvtRatioDb", NULL, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
    }

    real16Log10(REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
    if(tenOrTwenty == 10) {
      real16Multiply(REGISTER_REAL16_DATA(REGISTER_X), const16_10, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      real16Multiply(REGISTER_REAL16_DATA(REGISTER_X), const16_20, REGISTER_REAL16_DATA(REGISTER_X));
    }
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnCvtRatioDb:", "cannot use NaN as an input of fnCvtRatioDb", NULL, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
    }

    real34Log10(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
    if(tenOrTwenty == 10) {
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), const34_10, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), const34_20, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }
  else {
    displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtRatioDb:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif
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
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnCvtDbRatio:", "cannot use NaN as an input of fnCvtDbRatio", NULL, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
    }

    if(tenOrTwenty == 10) {
      real16Divide(REGISTER_REAL16_DATA(REGISTER_X), const16_10, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      real16Divide(REGISTER_REAL16_DATA(REGISTER_X), const16_20, REGISTER_REAL16_DATA(REGISTER_X));
    }

    real16Power(const16_10, REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnCvtDbRatio:", "cannot use NaN as an input of fnCvtDbRatio", NULL, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
    }

    if(tenOrTwenty == 10) {
      real34Divide(REGISTER_REAL34_DATA(REGISTER_X), const34_10, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      real34Divide(REGISTER_REAL34_DATA(REGISTER_X), const34_20, REGISTER_REAL34_DATA(REGISTER_X));
    }

    real34Power(const34_10, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtDbRatio:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif
  }

  adjustResult(REGISTER_X, false, false, -1, -1, -1);
}
