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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("unitConversion");
  #endif

  real34_t *pointerToRegisterXData;
  bool_t real16;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    real16 = true;
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real16 = false;
  }
  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real16 = true;
  }
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function unitConversion:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("unitConversion");
    #endif

    return;
  }

  pointerToRegisterXData = REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X));

  if(multiplyDivide == multiply) {
    real34Multiply(pointerToRegisterXData, coefficient, pointerToRegisterXData);
  }
  else {
    real34Divide(pointerToRegisterXData, coefficient, pointerToRegisterXData);
  }

  if(real16) {
    convertRegister34To16(REGISTER_X);
  }

  roundRegister(REGISTER_X);
  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("unitConversion");
  #endif
}



/********************************************//**
 * \brief Converts °Celcius to °Fahrenheit: (°Celcius * 1,8) + 32.
 * Refreshes the stack. This is the exact formula.
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCvtCToF(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtCToF");
  #endif

  unitConversion(const34_1_8, multiply);

  uint8_t *pointerToRegisterXData = POINTER_TO_REGISTER_DATA(REGISTER_X);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    real16Add(pointerToRegisterXData, const16_32, pointerToRegisterXData);
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34Add(pointerToRegisterXData, const34_32, pointerToRegisterXData);
  }

  roundRegister(REGISTER_X);
  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtCToF");
  #endif
}



/********************************************//**
 * \brief Converts °Fahrenheit to °Celcius: (°Fahrenheit - 32) / 1,8.
 * Refreshes the stack. This is the exact formula.
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCvtFToC(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtFToC");
  #endif

  calcRegister_t tempReg;

  tempReg = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, tempReg);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    real16Subtract(POINTER_TO_REGISTER_DATA(REGISTER_X), const16_32, POINTER_TO_REGISTER_DATA(REGISTER_X));
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34Subtract(POINTER_TO_REGISTER_DATA(REGISTER_X), const34_32, POINTER_TO_REGISTER_DATA(REGISTER_X));
  }
  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    bigInteger_t bigInteger;
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &bigInteger);
    bigIntegerSubtractUInt(&bigInteger, 32, &bigInteger);
    convertBigIntegerToBigIntegerRegister(&bigInteger, REGISTER_X);
  }

  unitConversion(const34_1_8, divide);

  copySourceRegisterToDestRegister(tempReg, REGISTER_L);
  freeTemporaryRegister(tempReg);

  roundRegister(REGISTER_X);
  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtFToC");
  #endif
}


void fnCvtYearS(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtYearS");
  #endif

  unitConversion(const34_YearToS, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtYearS");
  #endif
}


void fnCvtCalJ(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtCalJ");
  #endif

  unitConversion(const34_CalToJ, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtCalJ");
  #endif
}


void fnCvtBtuJ(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtBtuJ");
  #endif

  unitConversion(const34_BtuToJ, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtBtuJ");
  #endif
}


void fnCvtWhJ(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtWhJ");
  #endif

  unitConversion(const34_WhToJ, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtWhJ");
  #endif
}


void fnCvtHpeW(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtHpeW");
  #endif

  unitConversion(const34_HpeToW, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtHpeW");
  #endif
}


void fnCvtHpmW(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtHpmW");
  #endif

  unitConversion(const34_HpmToW, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtHpmW");
  #endif
}


void fnCvtHpukW(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtHpukW");
  #endif

  unitConversion(const34_HpukToW, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtHpukW");
  #endif
}


void fnCvtLbfN(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtLbfN");
  #endif

  unitConversion(const34_LbfToN, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtLbfN");
  #endif
}


void fnCvtBarPa(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtBarPa");
  #endif

  unitConversion(const34_BarToPa, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtBarPa");
  #endif
}


void fnCvtPsiPa(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtPsiPa");
  #endif

  unitConversion(const34_PsiToPa, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtPsiPa");
  #endif
}


void fnCvtInhgPa(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtInhgPa");
  #endif

  unitConversion(const34_InhgToPa, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtInhgPa");
  #endif
}


void fnCvtTorrPa(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtTorrPa");
  #endif

  unitConversion(const34_TorrToPa, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtTorrPa");
  #endif
}


void fnCvtAtmPa(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtAtmPa");
  #endif

  unitConversion(const34_AtmToPa, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtAtmPa");
  #endif
}


void fnCvtLbsKg(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtLbsKg");
  #endif

  unitConversion(const34_LbsToKg, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtLbsKg");
  #endif
}


void fnCvtCwtKg(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtCwtKg");
  #endif

  unitConversion(const34_CwtToKg, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtCwtKg");
  #endif
}


void fnCvtOzKg(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtOzKg");
  #endif

  unitConversion(const34_OzToKg, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtOzKg");
  #endif
}


void fnCvtStonesKg(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtStonesKg");
  #endif

  unitConversion(const34_StonesToKg, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtStonesKg");
  #endif
}


void fnCvtShortcwtKg(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtShortcwtKg");
  #endif

  unitConversion(const34_ShortcwtToKg, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtShortcwtKg");
  #endif
}


void fnCvtTrozKg(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtTrozKg");
  #endif

  unitConversion(const34_TrozToKg, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtTrozKg");
  #endif
}


void fnCvtTonsKg(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtTonsKg");
  #endif

  unitConversion(const34_TonsToKg, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtTonsKg");
  #endif
}


void fnCvtShorttonsKg(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtShorttonsKg");
  #endif

  unitConversion(const34_ShorttonsToKg, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtShorttonsKg");
  #endif
}


void fnCvtAuM(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtAuM");
  #endif

  unitConversion(const34_AuToM, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtAuM");
  #endif
}


void fnCvtMiM(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtMiM");
  #endif

  unitConversion(const34_MiToM, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtMiM");
  #endif
}


void fnCvtLyM(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtLyM");
  #endif

  unitConversion(const34_LyToM, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtLyM");
  #endif
}


void fnCvtNmiM(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtNmiM");
  #endif

  unitConversion(const34_NmiToM, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtNmiM");
  #endif
}


void fnCvtFtM(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtFtM");
  #endif

  unitConversion(const34_FtToM, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtFtM");
  #endif
}


void fnCvtPcM(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtPcM");
  #endif

  unitConversion(const34_PcToM, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtPcM");
  #endif
}


void fnCvtInchesM(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtInchesM");
  #endif

  unitConversion(const34_InchesToM, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtInchesM");
  #endif
}


void fnCvtSfeetM(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtSfeetM");
  #endif

  unitConversion(const34_SfeetToM, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtSfeetM");
  #endif
}


void fnCvtYardsM(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtYardsM");
  #endif

  unitConversion(const34_YardsToM, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtYardsM");
  #endif
}


void fnCvtPointsM(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtPointsM");
  #endif

  unitConversion(const34_PointsToM, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtPointsM");
  #endif
}


void fnCvtGalukM3(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtGalukM3");
  #endif

  unitConversion(const34_GalukToM3, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtGalukM3");
  #endif
}


void fnCvtGalusM3(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtGalusM3");
  #endif

  unitConversion(const34_GalusToM3, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtGalusM3");
  #endif
}


void fnCvtFlozukM3(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtFlozukM3");
  #endif

  unitConversion(const34_FlozukToM3, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtFlozukM3");
  #endif
}


void fnCvtFlozusM3(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtFlozusM3");
  #endif

  unitConversion(const34_FlozusToM3, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtFlozusM3");
  #endif
}


void fnCvtQuartM3(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtQuartM3");
  #endif

  unitConversion(const34_QuartToM3, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtQuartM3");
  #endif
}


void fnCvtAcresM2(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtAcresM2");
  #endif

  unitConversion(const34_AccresToM2, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtAcresM2");
  #endif
}


void fnCvtAcresusM2(uint16_t multiplyDivide) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtAcresusM2");
  #endif

  unitConversion(const34_AccresusToM2, multiplyDivide);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtAcresusM2");
  #endif
}



/********************************************//**
 * \brief Converts power or field ratio to dB
 * dB = (10 or 20) * log10((power or field) ratio) this is the exact formula
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCvtRatioDb(uint16_t tenOrTwenty) { // ten: power ratio   twenty: field ratio
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtRatioDb");
  #endif

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    real16Log10(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
    if(tenOrTwenty == 10) {
      real16Multiply(POINTER_TO_REGISTER_DATA(REGISTER_X), const16_10, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else {
      real16Multiply(POINTER_TO_REGISTER_DATA(REGISTER_X), const16_20, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34Log10(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
    if(tenOrTwenty == 10) {
      real34Multiply(POINTER_TO_REGISTER_DATA(REGISTER_X), const34_10, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else {
      real34Multiply(POINTER_TO_REGISTER_DATA(REGISTER_X), const34_20, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
  }
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtRatioDb:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif
  }

  roundRegister(REGISTER_X);
  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtRatioDb");
  #endif
}



/********************************************//**
 * \brief Converts dB to power or field ratio
 * (power or field) ratio = 10^(dB / 10 or 20) this is the exact formula
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCvtDbRatio(uint16_t tenOrTwenty) { // ten: power ratio   twenty: field ratio
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtDbRatio");
  #endif

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    if(tenOrTwenty == 10) {
      real16Divide(POINTER_TO_REGISTER_DATA(REGISTER_X), const16_10, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else {
      real16Divide(POINTER_TO_REGISTER_DATA(REGISTER_X), const16_20, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }

    real16Power(const16_10, POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    if(tenOrTwenty == 10) {
      real34Divide(POINTER_TO_REGISTER_DATA(REGISTER_X), const34_10, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else {
      real34Divide(POINTER_TO_REGISTER_DATA(REGISTER_X), const34_20, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }

    real34Power(const34_10, POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtDbRatio:", getRegisterDataTypeName(REGISTER_X, true, false), "cannot be converted!", NULL);
    #endif
  }

  roundRegister(REGISTER_X);
  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtDbRatio");
  #endif
}
