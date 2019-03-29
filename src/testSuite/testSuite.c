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
 * \file testSuite.c
 ***********************************************/

#include "wp43s.h"
#include "testSuite.h"



const funcTest_t funcTestNoParam[] = {
  {"fn10Pow",                fn10Pow               },
  {"fn2Pow",                 fn2Pow                },
  {"fnAim",                  fnAim                 },
  {"fnArccos",               fnArccos              },
  {"fnArcsin",               fnArcsin              },
  {"fnArctan",               fnArctan              },
  {"fnBatteryVoltage",       fnBatteryVoltage      },
  {"fnClearRegisters",       fnClearRegisters      },
  {"fnClearStack",           fnClearStack          },
  {"fnClFAll",               fnClFAll              },
  {"fnClSigma",              fnClSigma             },
  {"fnClX",                  fnClX                 },
  {"fnComplexCCCC",          fnComplexCCCC         },
  {"fnConfigChina",          fnConfigChina         },
  {"fnConfigEurope",         fnConfigEurope        },
  {"fnConfigIndia",          fnConfigIndia         },
  {"fnConfigJapan",          fnConfigJapan         },
  {"fnConfigUk",             fnConfigUk            },
  {"fnConfigUsa",            fnConfigUsa           },
  {"fnConjugate",            fnConjugate           },
  {"fnConvertXToReal16",     fnConvertXToReal16    },
  {"fnConvertXToReal34",     fnConvertXToReal34    },
  {"fnCos",                  fnCos                 },
  {"fnCube",                 fnCube                },
  {"fnCvtCToF",              fnCvtCToF             },
  {"fnCvtDegToDms",          fnCvtDegToDms         },
  {"fnCvtDegToRad",          fnCvtDegToRad         },
  {"fnCvtDmsToDeg",          fnCvtDmsToDeg         },
  {"fnCvtFToC",              fnCvtFToC             },
  {"fnCvtRadToDeg",          fnCvtRadToDeg         },
  {"fnDenMax",               fnDenMax              },
  {"fnDrop",                 fnDrop                },
  {"fnDropY",                fnDropY               },
  {"fnExp",                  fnExp                 },
  {"fnFactorial",            fnFactorial           },
  {"fnFillStack",            fnFillStack           },
  {"fnFp",                   fnFp                  },
  {"fnFreeFlashMemory",      fnFreeFlashMemory     },
  {"fnFreeMemory",           fnFreeMemory          },
  {"fnGamma",                fnGamma               },
  {"fnGcd",                  fnGcd                 },
  {"fnGetIntegerSignMode",   fnGetIntegerSignMode  },
  {"fnGetLocR",              fnGetLocR             },
  {"fnGetRoundingMode",      fnGetRoundingMode     },
  {"fnGetSignificantDigits", fnGetSignificantDigits},
  {"fnGetStackSize",         fnGetStackSize        },
  {"fnGetWordSize",          fnGetWordSize         },
  {"fnImaginaryPart",        fnImaginaryPart       },
  {"fnInvert",               fnInvert              },
  {"fnIp",                   fnIp                  },
  {"fnIsPrime",              fnIsPrime             },
  {"fnLcm",                  fnLcm                 },
  {"fnLn",                   fnLn                  },
  {"fnLnGamma",              fnLnGamma             },
  {"fnLog10",                fnLog10               },
  {"fnLog2",                 fnLog2                },
  {"fnLogicalNot",           fnLogicalNot          },
  {"fnM1Pow",                fnM1Pow               },
  {"fnMagnitude",            fnMagnitude           },
  {"fnMirror",               fnMirror              },
  {"fnPi",                   fnPi                  },
  {"fnPower",                fnPower               },
  {"fnRealPart",             fnRealPart            },
  {"fnRollDown",             fnRollDown            },
  {"fnRollUp",               fnRollUp              },
  {"fnSign",                 fnSign                },
  {"fnSin",                  fnSin                 },
  {"fnSquare",               fnSquare              },
  {"fnSquareRoot",           fnSquareRoot          },
  {"fnSwapRealImaginary",    fnSwapRealImaginary   },
  {"fnSwapXY",               fnSwapXY              },
  {"fnTan",                  fnTan                 },
  {"fnToPolar",              fnToPolar             },
  {"fnToRect",               fnToRect              },
  {"fnUnitVector",           fnUnitVector          },
  {"",                       NULL                  }
};

const funcTest_t funcTestCvt[] = {
  {"fnCvtAcreM2",     fnCvtAcreM2    },
  {"fnCvtAcreusM2",   fnCvtAcreusM2  },
  {"fnCvtAtmPa",      fnCvtAtmPa     },
  {"fnCvtAuM",        fnCvtAuM       },
  {"fnCvtBarPa",      fnCvtBarPa     },
  {"fnCvtBtuJ",       fnCvtBtuJ      },
  {"fnCvtCalJ",       fnCvtCalJ      },
  {"fnCvtCwtKg",      fnCvtCwtKg     },
  {"fnCvtFlozukM3",   fnCvtFlozukM3  },
  {"fnCvtFlozusM3",   fnCvtFlozusM3  },
  {"fnCvtFtM",        fnCvtFtM       },
  {"fnCvtGalukM3",    fnCvtGalukM3   },
  {"fnCvtGalusM3",    fnCvtGalusM3   },
  {"fnCvtHpeW",       fnCvtHpeW      },
  {"fnCvtHpmW",       fnCvtHpmW      },
  {"fnCvtHpukW",      fnCvtHpukW     },
  {"fnCvtInchM",      fnCvtInchM     },
  {"fnCvtInhgPa",     fnCvtInhgPa    },
  {"fnCvtLbfN",       fnCvtLbfN      },
  {"fnCvtLbKg",       fnCvtLbKg      },
  {"fnCvtLyM",        fnCvtLyM       },
  {"fnCvtMiM",        fnCvtMiM       },
  {"fnCvtNmiM",       fnCvtNmiM      },
  {"fnCvtOzKg",       fnCvtOzKg      },
  {"fnCvtPcM",        fnCvtPcM       },
  {"fnCvtPointM",     fnCvtPointM    },
  {"fnCvtPsiPa",      fnCvtPsiPa     },
  {"fnCvtQuartM3",    fnCvtQuartM3   },
  {"fnCvtSfeetM",     fnCvtSfeetM    },
  {"fnCvtShortcwtKg", fnCvtShortcwtKg},
  {"fnCvtShorttonKg", fnCvtShorttonKg},
  {"fnCvtStoneKg",    fnCvtStoneKg   },
  {"fnCvtTonKg",      fnCvtTonKg     },
  {"fnCvtTorrPa",     fnCvtTorrPa    },
  {"fnCvtTrozKg",     fnCvtTrozKg    },
  {"fnCvtWhJ",        fnCvtWhJ       },
  {"fnCvtYardM",      fnCvtYardM     },
  {"fnCvtYearS",      fnCvtYearS     },
  {"",                NULL           }
};

int32_t lineNumber;



char *endOfString(char *string) { // string must point on the 1st "
  string++;
  while(*string != '"' && *string != 0) {
    if(*string == '\\') {
      string++;
    }
    string++;
  }

  if(*string == '"') {
    string++;
  }
  else {
    printf("Unterminated string\n");
    exit(-1);
  }

  return string; // pointer to the 1st char after the ending "
}



void strToShortInteger(char *nimBuffer, calcRegister_t regist) {
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Any change in this function is to be reported in the function closeNim from file bufferize.c after the line: else if(nimNumberPart == NP_INT_BASE) {
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  longInteger_t minVal, value, maxVal;
  int16_t posHash, i, lg;
  int32_t base;

  lg = strlen(nimBuffer);
  posHash = 0;
  for(i=1; i<lg; i++) {
    if(nimBuffer[i] == '#') {
      posHash = i;
      break;
    }
  }

  for(i=posHash+1; i<lg; i++) {
    if(nimBuffer[i]<'0' || nimBuffer[i]>'9') {
      printf("Error while initializing a short integer: there is a non numeric character in the base of the integer!\n");
      exit(-1);
    }
  }

  base = atoi(nimBuffer + posHash + 1);
  if(base < 2 || base > 16) {
    printf("Error while initializing a short integer: the base of the integer must be from 2 to 16!\n");
    exit(-1);
  }

  for(i=nimBuffer[0] == '-' ? 1 : 0; i<posHash; i++) {
    if((nimBuffer[i] > '9' ? nimBuffer[i] - 'A' + 10 : nimBuffer[i] - '0') >= base) {
      printf("Error while initializing a short integer: digit %c is not allowed in base %d!\n", nimBuffer[i], base);
      exit(-1);
    }
  }

  stringToLongInteger(nimBuffer + (nimBuffer[0] == '+' ? 1 : 0), base, &value);

  // maxVal = 2^shortIntegerWordSize
  if(shortIntegerWordSize >= 1 && shortIntegerWordSize <= 64) {
    longInteger2Exp(shortIntegerWordSize, &maxVal);
  }
  else {
    printf("Error while initializing a short integer: shortIntegerWordSize must be fom 1 to 64\n");
    exit(-1);
  }

  // minVal = -maxVal/2
  longIntegerSetZero(&minVal); // Mandatory! Else segmentation fault at next instruction
  longIntegerDivideUInt(&maxVal, 2, &minVal); // minVal = maxVal / 2
  longIntegerSetNegativeSign(&minVal); // minVal = -minVal

  if(shortIntegerMode != SIM_UNSIGN) {
    longIntegerDivideUInt(&maxVal, 2, &maxVal); // maxVal /= 2
  }

  longIntegerSubtractUInt(&maxVal, 1, &maxVal); // maxVal--

  if(shortIntegerMode == SIM_UNSIGN) {
    longIntegerSetZero(&minVal); // minVal = 0
  }

  if(shortIntegerMode == SIM_1COMPL || shortIntegerMode == SIM_SIGNMT) {
    longIntegerAddUInt(&minVal, 1, &minVal); // minVal++
  }

  if(longIntegerCompare(&value, &minVal) == LONG_INTEGER_LESS_THAN || longIntegerCompare(&value, &maxVal) == LONG_INTEGER_GREATER_THAN) {
    char strMin[22], strMax[22];
    longIntegerToString(&minVal, strMin, 10);
    longIntegerToString(&maxVal, strMax, 10);
    printf("Error while initializing a short integer: for a word size of %d bit%s and integer mode %s, the entered number must be from %s to %s!\n", shortIntegerWordSize, shortIntegerWordSize>1 ? "s" : "", getShortIntegerModeName(shortIntegerMode), strMin, strMax);
    exit(-1);
  }

  reallocateRegister(regist, dtShortInteger, SHORT_INTEGER_SIZE, base);

  char strValue[22];
  longIntegerToString(&value, strValue, 10);

  uint64_t val;
  if(value.sign) {
    val = atoll(strValue + 1); // value is negative: discard the minus sign
  }
  else {
    val = atoll(strValue); // value is positive
  }

  if(shortIntegerMode == SIM_UNSIGN) {
    *(REGISTER_SHORT_INTEGER_DATA(regist)) = val;
  }
  else if(shortIntegerMode == SIM_2COMPL) {
    if(value.sign) {
      val = (~val + 1) & shortIntegerMask;
    }

    *(REGISTER_SHORT_INTEGER_DATA(regist)) = val;
  }
  else if(shortIntegerMode == SIM_1COMPL) {
    if(value.sign) {
      val = ~val & shortIntegerMask;
    }

    *(REGISTER_SHORT_INTEGER_DATA(regist)) = val;
  }
  else if(shortIntegerMode == SIM_SIGNMT) {
    if(value.sign) {
      val += shortIntegerMask;
    }

    *(REGISTER_SHORT_INTEGER_DATA(regist)) = val;
  }
  else {
    *(REGISTER_SHORT_INTEGER_DATA(regist)) = 0;
    printf("Bad integer mode while initializing a short integer\n");
    exit(-1);
  }
}



void setParameter(char *p) {
  char l[100], r[100], real[100], imag[100], angMod[100], letter;
  int32_t i, am;
  longInteger_t temp;

  //printf("  setting %s\n", p);

  i = 0;
  while(p[i] != '=' && p[i] != 0) i++;
  if(p[i] == 0) {
    printf("Missformed parameter setting. Missing equal sign, remember that no space is allowed around the equal sign.\n");
    exit(-1);
  }

  p[i] = 0;
  strcpy(l, p);
  strcpy(r, p + i + 1);

  if(r[0] == 0) {
    printf("Missformed parameter setting. Missing value after equal sign, remember that no space is allowed around the equal sign.\n");
    exit(-1);
  }

  //Setting a flag
  if(l[0] == 'F') {
    if(r[0] != '0' && r[0] != '1' && r[1] != 0) {
      printf("Missformed flag setting. The rvalue must be 0 or 1\n");
      exit(-1);
    }

    //Lettered flag
    if(l[1] >= 'A' && l[2] == 0) {
      if(strstr("XYZTABCDLIJK", l + 1) != NULL) {
        uint16_t flg;

        flg = l[1] == 'T' ? 103 :
              l[1] == 'L' ? 108 :
              l[1] <= 'D' ? l[1] + 39 :
              l[1] <= 'K' ? l[1] + 36 :
                            l[1] + 12;

        if(r[0] == '1') {
          fnSetFlag(flg);
          printf("  Flag %c set\n", l[1]);
        }
        else {
          fnClearFlag(flg);
          printf("  Flag %c cleared\n", l[1]);
        }
      }
      else {
        printf("Missformed flag setting. After F there shall be a number from 0 to 111 or a lettered flag\n");
        exit(-1);
      }
    }

    //Numbered flag
    else if(   (l[1] >= '0' && l[1] <= '9' && l[2] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] >= '0' && l[3] <= '9' && l[4] == 0)) {
      uint16_t flg = atoi(l + 1);
      if(flg <= 111) {
        if(r[0] == '1') {
          fnSetFlag(flg);
          printf("  Flag %d set\n", flg);
        }
        else {
          fnClearFlag(flg);
          printf("  Flag %d cleared\n", flg);
        }
      }
      else {
        printf("Missformed flag setting. After the after F shall be a number from 0 to 111\n");
        exit(-1);
      }
    }

    else {
      printf("Missformed numbered flag setting. After F there shall be a number from 0 to 111\n");
      exit(-1);
    }
  }

  //Setting Stack Lift
  else if(strcmp(l, "SL") == 0) {
    if(r[0] != '0' && r[0] != '1' && r[1] != 0) {
      printf("Missformed stack lift setting. The rvalue must be 0 or 1");
      exit(-1);
    }

    if(r[0] == '1') {
      STACK_LIFT_ENABLE;
      printf("  Stack lift enabled\n");
    }
    else {
      STACK_LIFT_DISABLE;
      printf("  Stack lift disabled\n");
    }
  }

  //Setting integer mode
  else if(strcmp(l, "IM") == 0) {
    if(strcmp(r, "1COMPL") == 0) {
      shortIntegerMode = SIM_1COMPL;
      printf("  Set integer mode to 1COMPL\n");
    }
    else if(strcmp(r, "2COMPL") == 0) {
      shortIntegerMode = SIM_2COMPL;
      printf("  Set integer mode to 2COMPL\n");
    }
    else if(strcmp(r, "UNSIGN") == 0) {
      shortIntegerMode = SIM_UNSIGN;
      printf("  Set integer mode to UNSIGN\n");
    }
    else if(strcmp(r, "SIGNMT") == 0) {
      shortIntegerMode = SIM_SIGNMT;
      printf("  Set integer mode to SIGNMT\n");
    }
    else {
      printf("Missformed integer mode setting. The rvalue must be 1COMPL, 2COMPL, UNSIGN or SIGNMT.\n");
      exit(-1);
    }
  }

  //Setting angular mode
  else if(strcmp(l, "AM") == 0) {
    if(strcmp(r, "DEG") == 0) {
      angularMode = AM_DEGREE;
      printf("  Set angular mode to DEG\n");
    }
    else if(strcmp(r, "DMS") == 0) {
      angularMode = AM_DMS;
      printf("  Set angular mode to DMS\n");
    }
    else if(strcmp(r, "GRAD") == 0) {
      angularMode = AM_GRAD;
      printf("  Set angular mode to GRAD\n");
    }
    else if(strcmp(r, "RAD") == 0) {
      angularMode = AM_RADIAN;
      printf("  Set angular mode to RAD\n");
    }
    else if(strcmp(r, "MULTPI") == 0) {
      angularMode = AM_MULTPI;
      printf("  Set angular mode to MULTPI\n");
    }
    else {
      printf("Missformed angular mode setting. The rvalue must be DEG, DMS, GRAD, RAD or MULTPI.\n");
      exit(-1);
    }
  }

  //Setting stack size
  else if(strcmp(l, "SS") == 0) {
    if(strcmp(r, "4") == 0) {
      stackSize = SS_4;
      printf("  Set stack size to 4\n");
    }
    else if(strcmp(r, "8") == 0) {
      stackSize = SS_8;
      printf("  Set stack size to 8\n");
    }
    else {
      printf("Missformed stack size setting. The rvalue must be 4 or 8.\n");
      exit(-1);
    }
  }

  //Setting word size
  else if(strcmp(l, "WS") == 0) {
    if(   (r[0] >= '0' && r[0] <= '9' && r[1] == 0)
       || (r[0] >= '0' && r[0] <= '9' && r[1] >= '0' && r[1] <= '9' && r[2] == 0)) {
      uint16_t ws = atoi(r);

      if(ws == 0) {
        ws = 64;
      }
      if(ws <= 64) {
        fnSetWordSize(ws);
        printf("  Set word size to %d bit\n", ws);
      }
      else {
        printf("Missformed word size setting. The rvalue must be from 0 to 64 (0 is the same as 64).\n");
        exit(-1);
      }
    }
    else {
      printf("Missformed word size setting. The rvalue must be a number from 0 to 64 (0 is the same as 64).\n");
      exit(-1);
    }
  }

  //Setting significant digits
  else if(strcmp(l, "SD") == 0) {
    if(   (r[0] >= '0' && r[0] <= '9' && r[1] == 0)
       || (r[0] >= '0' && r[0] <= '9' && r[1] >= '0' && r[1] <= '9' && r[2] == 0)) {
      uint16_t sd = atoi(r);

      if(sd <= 34) {
        significantDigits = sd;
        printf("  Set significant digits to %d\n", sd);
      }
      else {
        printf("Missformed significant digits setting. The rvalue must be from 0 to 34 (0 is the same as 34).\n");
        exit(-1);
      }
    }
    else {
      printf("Missformed significant digits setting. The rvalue must be a number from 0 to 34 (0 is the same as 34).\n");
      exit(-1);
    }
  }

  //Setting rounding mode
  else if(strcmp(l, "RM") == 0) {
    if(r[0] >= '0' && r[0] <= '9' && r[1] == 0) {
      uint16_t rm = atoi(r);

      if(rm <= 6) {
        fnRoundingMode(rm);
        //printf("  Set rounding mode to %d (%s)\n", rm, getRoundingModeName(rm));
        printf("  Set rounding mode to %d\n", rm);
      }
      else {
        printf("Missformed rounding mode setting. The rvalue must be a number from 0 to 6.\n");
        exit(-1);
      }
    }
    else {
      printf("Missformed rounding mode setting. The rvalue must be a number from 0 to 6.\n");
      exit(-1);
    }
  }

  //Setting a register
  else if(l[0] == 'R') {
    calcRegister_t regist;

    //Lettered register
    if(l[1] >= 'A' && l[2] == 0) {
      if(strstr("XYZTABCDLIJK", l + 1) != NULL) {
        letter = l[1];
        regist = l[1] == 'T' ? 103 :
                 l[1] == 'L' ? 108 :
                 l[1] <= 'D' ? l[1] + 39 :
                 l[1] <= 'K' ? l[1] + 36 :
                               l[1] + 12;
      }
      else {
        printf("Missformed lettered register setting. The letter after R is not a lettered register.\n");
        exit(-1);
      }
    }

    //Numbered register
    else if(   (l[1] >= '0' && l[1] <= '9' && l[2] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] >= '0' && l[3] <= '9' && l[4] == 0)) {
      regist = atoi(l + 1);
      if(regist > 111) {
        printf("Missformed numbered register setting. Th number after R shall be a number from 0 to 111.\n");
        exit(-1);
      }
      letter = 0;
    }

    else {
      printf("Missformed register setting. After R there shall be a number from 0 to 111 or a lettered register.\n");
      exit(-1);
    }

    // find the : separating the data type and the value
    i = 0;
    while(r[i] != ':' && r[i] != 0) i++;
    if(r[i] == 0) {
      printf("Missformed register value. Missing colon between data type and value.\n");
      exit(-1);
    }

    // separating the data type and the value
    r[i] = 0;
    strcpy(l, r);
    strcpy(r, r + i + 1);

    if(strcmp(l, "LONI") == 0) {
      // remove beginning and ending " and removing leading spaces
      strcpy(r, r + 1);
      while(r[0] == ' ') strcpy(r, r + 1);
      r[strlen(r) - 1] = 0;

      stringToLongInteger(r, 10, &temp);
      convertLongIntegerToLongIntegerRegister(&temp, regist);
    }
    else if(strcmp(l, "RE16") == 0) {
      // remove beginning and ending " and removing leading spaces
      strcpy(r, r + 1);
      while(r[0] == ' ') strcpy(r, r + 1);
      r[strlen(r) - 1] = 0;

      // removing trailing spaces
      while(r[strlen(r) - 1] == ' ') r[strlen(r) - 1] = 0;

      // replace , with .
      for(int i=0; i<(int)strlen(r); i++) {
        if(r[i] == ',') r[i] = '.';
      }

      reallocateRegister(regist, dtReal16, REAL16_SIZE, 0);
      stringToReal16(r, REGISTER_REAL16_DATA(regist));
    }
    else if(strcmp(l, "CO16") == 0) {
      // remove beginning and ending " and removing leading spaces
      strcpy(r, r + 1);
      while(r[0] == ' ') strcpy(r, r + 1);
      r[strlen(r) - 1] = 0;

      // find the i separating the real and imagynary part
      i = 0;
      while(r[i] != 'i' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("Missformed register complex value. Missing i between real and imaginary part.\n");
        exit(-1);
      }

      // separate real and imaginary part
      r[i] = 0;
      strcpy(real, r);
      strcpy(imag, r + i + 1);

      // remove leading spaces
      while(imag[0] == ' ') strcpy(imag, imag + 1);

      // removing trailing spaces from real part
      while(real[strlen(real) - 1] == ' ') real[strlen(real) - 1] = 0;

      // removing trailing spaces from imaginary part
      while(imag[strlen(imag) - 1] == ' ') imag[strlen(imag) - 1] = 0;

      // replace , with . in the real part
      for(int i=0; i<(int)strlen(real); i++) {
        if(real[i] == ',') real[i] = '.';
      }

      // replace , with . in the imaginary part
      for(int i=0; i<(int)strlen(imag); i++) {
        if(imag[i] == ',') imag[i] = '.';
      }

      reallocateRegister(regist, dtComplex16, COMPLEX16_SIZE, 0);
      stringToReal16(real, REGISTER_REAL16_DATA(regist));
      stringToReal16(imag, REGISTER_IMAG16_DATA(regist));
    }
    else if(strcmp(l, "ANGL") == 0) {
      // find the : separating the angle value from the angular mode
      i = 0;
      while(r[i] != ':' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("Missformed register angle value. Missing colon between angle value and angular mode.\n");
        exit(-1);
      }

      // separate angle value and angular mode
      r[i] = 0;
      strcpy(angMod, r + i + 1);

           if(strcmp(angMod, "DEG"   ) == 0) am = AM_DEGREE;
      else if(strcmp(angMod, "DMS"   ) == 0) am = AM_DMS;
      else if(strcmp(angMod, "GRAD"  ) == 0) am = AM_GRAD;
      else if(strcmp(angMod, "RAD"   ) == 0) am = AM_RADIAN;
      else if(strcmp(angMod, "MULTPI") == 0) am = AM_MULTPI;
      else {
        printf("Missformed register angle unit. Unknown unit after angle value.\n");
        exit(-1);
      }

      // remove beginning and ending " and removing leading spaces
      strcpy(r, r + 1);
      while(r[0] == ' ') strcpy(r, r + 1);
      r[strlen(r) - 1] = 0;

      // replace , with .
      for(int i=0; i<(int)strlen(r); i++) {
        if(r[i] == ',') r[i] = '.';
      }

      reallocateRegister(regist, dtAngle, ANGLE_SIZE, am);
      #if (ANGLE16 == 1)
        stringToReal16(r, REGISTER_REAL16_DATA(regist));
      #else
        stringToReal34(r, REGISTER_REAL34_DATA(regist));
      #endif

      convertAngleToInternal(REGISTER_ANGLE_DATA(regist), am);
      setRegisterDataType(result, dtAngle);
      setRegisterAngularMode(regist, am);
    }
    else if(strcmp(l, "STRI") == 0) {
      r[strlen(r) - 1] = 0;

      for(int i=strlen(r); i>0; i--) {
        if(r[i - 1] == '\\') {
          strcpy(r + i - 1, r + i);
          i--;
        }
      }

      reallocateRegister(regist, dtString, strlen(r + 1), 0);
      strcpy(REGISTER_STRING_DATA(regist), r + 1);
    }
    else if(strcmp(l, "SHOI") == 0) {
      // find the # separating the value from the base
      i = 0;
      while(r[i] != '#' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("Missformed register short integer value. Missing # between value and base.\n");
        exit(-1);
      }

      // remove beginning and ending " and removing leading spaces
      strcpy(r, r + 1);
      while(r[0] == ' ') strcpy(r, r + 1);
      r[strlen(r) - 1] = 0;

      strToShortInteger(r, regist);
    }
    else if(strcmp(l, "RE34") == 0) {
      strcpy(r, r + 1);
      while(r[0] == ' ') strcpy(r, r + 1);
      r[strlen(r) - 1] = 0;

      while(r[strlen(r) - 1] == ' ') r[strlen(r) - 1] = 0;
      for(int i=0; i<(int)strlen(r); i++) {
        if(r[i] == ',') r[i] = '.';
      }

      reallocateRegister(regist, dtReal34, REAL34_SIZE, 0);
      stringToReal34(r, REGISTER_REAL34_DATA(regist));
    }
    else if(strcmp(l, "CO34") == 0) {
      // remove beginning and ending " and removing leading spaces
      strcpy(r, r + 1);
      while(r[0] == ' ') strcpy(r, r + 1);
      r[strlen(r) - 1] = 0;

      // find the i separating the real and imagynary part
      i = 0;
      while(r[i] != 'i' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("Missformed register complex value. Missing i between real and imaginary part.\n");
        exit(-1);
      }

      // separate real and imaginary part
      r[i] = 0;
      strcpy(real, r);
      strcpy(imag, r + i + 1);

      // remove leading spaces
      while(imag[0] == ' ') strcpy(imag, imag + 1);

      // removing trailing spaces from real part
      while(real[strlen(real) - 1] == ' ') real[strlen(real) - 1] = 0;

      // removing trailing spaces from imaginary part
      while(imag[strlen(imag) - 1] == ' ') imag[strlen(imag) - 1] = 0;

      // replace , with . in the real part
      for(int i=0; i<(int)strlen(real); i++) {
        if(real[i] == ',') real[i] = '.';
      }

      // replace , with . in the imaginary part
      for(int i=0; i<(int)strlen(imag); i++) {
        if(imag[i] == ',') imag[i] = '.';
      }

      reallocateRegister(regist, dtComplex34, COMPLEX34_SIZE, 0);
      stringToReal34(real, REGISTER_REAL34_DATA(regist));
      stringToReal34(imag, REGISTER_IMAG34_DATA(regist));
    }
    else {
      printf("Missformed register value. Unknown data type %s for register %s\n", l, p+1);
      exit(-1);
    }

    if(letter == 0) {
      printf("  R%d = ", regist);
    }
    else {
      printf("  R%c = ", letter);
    }

    printRegisterToConsole(regist, 0);
    printf("\n");
  }

  else {
    printf("Unknown setting %s.\n", l);
    exit(-1);
  }
}



void inParameters(char *token) {
  char parameter[200];
  int32_t index, lg;

  while(*token == ' ') token++;
  while(*token != 0) {
    index = 0;
    while(*token != ' ' && *token != 0) {
      if(*token == '"') { // Inside a string
        lg = endOfString(token) - token;
        strncpy(parameter + index, token, lg--);
        index += lg;
        token += lg;
      }
      parameter[index++] = *(token++);
    }
    parameter[index] = 0;

    setParameter(parameter);

    while(*token == ' ') token++;
  }
}



void checkRegisterType(calcRegister_t regist, char letter, uint32_t expectedDataType) {
  if(getRegisterDataType(regist) != expectedDataType) {
    if(letter == 0) {
      printf("Register %u should be %s but it is %s!\n", regist, getDataTypeName(expectedDataType, true, false), getDataTypeName(getRegisterDataType(regist), true, false));
      printf("R%u = ", regist);
    }
    else {
      printf("Register %c should be %s but it is %s!\n", letter, getDataTypeName(expectedDataType, true, false), getDataTypeName(getRegisterDataType(regist), true, false));
      printf("R%c = ", letter);
    }
    printRegisterToConsole(regist, 0);
    printf("\n");
    exit(-1);
  }
}



void wrongRegisterValue(calcRegister_t regist, char letter, char *expectedValue) {
  if(letter == 0) {
    printf("Register %u value should be ", regist);
  }
  else {
    printf("Register %c value should be ", letter);
  }
  printf("%s\nbut it is ", expectedValue);
  printRegisterToConsole(regist, 0);
  printf("\n");
  exit(-1);
}



void checkExpectedOutParameter(char *p) {
  char l[100], r[100], real[100], imag[100], angMod[100], letter;
  int32_t i, am;

  //printf("  Checking %s\n", p);

  i = 0;
  while(p[i] != '=' && p[i] != 0) i++;
  if(p[i] == 0) {
    printf("Missformed out parameter. Missing equal sign, remember that no space is allowed around the equal sign.\n");
    exit(-1);
  }

  p[i] = 0;
  strcpy(l, p);
  strcpy(r, p + i + 1);

  if(r[0] == 0) {
    printf("Missformed out parameter. Missing value after equal sign, remember that no space is allowed around the equal sign.\n");
    exit(-1);
  }

  //Checking a flag
  if(l[0] == 'F') {
    if(r[0] != '0' && r[0] != '1' && r[1] != 0) {
      printf("Missformed flag checking. The rvalue must be 0 or 1.\n");
      exit(-1);
    }

    //Lettered flag
    if(l[1] >= 'A' && l[2] == 0) {
      if(strstr("XYZTABCDLIJK", l + 1) != NULL) {
        uint16_t flg;

        flg = l[1] == 'T' ? 103 :
              l[1] == 'L' ? 108 :
              l[1] <= 'D' ? l[1] + 39 :
              l[1] <= 'K' ? l[1] + 36 :
                            l[1] + 12;

        if(r[0] == '1') {
          if(!getFlag(flg)) {
            printf("Flag %c should be set but it is clear!\n", l[1]);
            exit(-1);
          }
        }
        else {
          if(getFlag(flg)) {
            printf("Flag %c should be clear but it is set!\n", l[1]);
            exit(-1);
          }
        }
      }
      else {
        printf("Missformed flag checking. After F there shall be a number from 0 to 111 or a lettered flag.\n");
        exit(-1);
      }
    }

    //Numbered flag
    else if(   (l[1] >= '0' && l[1] <= '9' && l[2] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] >= '0' && l[3] <= '9' && l[4] == 0)) {
      uint16_t flg = atoi(l + 1);
      if(flg <= 111) {
        if(r[0] == '1') {
          printf("Flag %d should be set but it is clear!\n", flg);
          exit(-1);
        }
        else {
          printf("Flag %d should be clear but it is set!\n", flg);
          exit(-1);
        }
      }
      else {
        printf("Missformed flag checking in line. After the after F shall be a number from 0 to 111.\n");
        exit(-1);
      }
    }

    else {
      printf("Missformed numbered flag checking. After F there shall be a number from 0 to 111.\n");
      exit(-1);
    }
  }

  //Checking Stack Lift
  else if(strcmp(l, "SL") == 0) {
    if(r[0] != '0' && r[0] != '1' && r[1] != 0) {
      printf("Missformed stack lift checking. The rvalue must be 0 or 1.\n");
      exit(-1);
    }

    if(r[0] == '1') {
      if(!stackLiftEnabled) {
        printf("Stack lift should be enabled but it is disabled!\n");
        exit(-1);
      }
    }
    else {
      if(stackLiftEnabled) {
        printf("Stack lift should be disabled but it is enabled!\n");
        exit(-1);
      }
    }
  }

  //Checking integer mode
  else if(strcmp(l, "IM") == 0) {
    if(strcmp(r, "1COMPL") == 0) {
      if(shortIntegerMode != SIM_1COMPL) {
        printf("Integer mode should be 1COMPL but it is not!\n");
        exit(-1);
      }
    }
    else if(strcmp(r, "2COMPL") == 0) {
      if(shortIntegerMode != SIM_2COMPL) {
        printf("Integer mode should be 2COMPL but it is not!\n");
        exit(-1);
      }
    }
    else if(strcmp(r, "UNSIGN") == 0) {
      if(shortIntegerMode != SIM_UNSIGN) {
        printf("Integer mode should be UNSIGN but it is not!\n");
        exit(-1);
      }
    }
    else if(strcmp(r, "SIGNMT") == 0) {
      if(shortIntegerMode != SIM_SIGNMT) {
        printf("Integer mode should be SIGNMT but it is not!\n");
        exit(-1);
      }
    }
    else {
      printf("Missformed integer mode checking. The rvalue must be 1COMPL, 2COMPL, UNSIGN or SIGNMT.\n");
      exit(-1);
    }
  }

  //Checking angular mode
  else if(strcmp(l, "AM") == 0) {
    if(strcmp(r, "DEG") == 0) {
      if(angularMode != AM_DEGREE) {
        printf("Angular mode should be DEGREE but it is not!\n");
        exit(-1);
      }
    }
    else if(strcmp(r, "DMS") == 0) {
      if(angularMode != AM_DMS) {
        printf("Angular mode should be DMS but it is not!\n");
        exit(-1);
      }
    }
    else if(strcmp(r, "GRAD") == 0) {
      if(angularMode != AM_GRAD) {
        printf("Angular mode should be GRAD but it is not!\n");
        exit(-1);
      }
    }
    else if(strcmp(r, "RAD") == 0) {
      if(angularMode != AM_RADIAN) {
        printf("Angular mode should be RAD but it is not!\n");
        exit(-1);
      }
    }
    else if(strcmp(r, "MULTPI") == 0) {
      if(angularMode != AM_MULTPI) {
        printf("Angular mode should be MULTPI but it is not!\n");
        exit(-1);
      }
    }
    else {
      printf("Missformed angular mode checking. The rvalue must be DEG, DMS, GRAD, RAD or MULTPI.\n");
      exit(-1);
    }
  }

  //Checking stack size
  else if(strcmp(l, "SS") == 0) {
    if(strcmp(r, "4") == 0) {
      if(stackSize != SS_4) {
        printf("Stack size should be 4 but it is not!\n");
        exit(-1);
      }
    }
    else if(strcmp(r, "8") == 0) {
      if(stackSize != SS_8) {
        printf("Stack size should be 8 but it is not!\n");
        exit(-1);
      }
    }
    else {
      printf("Missformed stack size checking. The rvalue must be 4 or 8.\n");
      exit(-1);
    }
  }

  //Checking word size
  else if(strcmp(l, "WS") == 0) {
    if(   (r[0] >= '0' && r[0] <= '9' && r[1] == 0)
       || (r[0] >= '0' && r[0] <= '9' && r[1] >= '0' && r[1] <= '9' && r[2] == 0)) {
      uint16_t ws = atoi(r);

      if(ws == 0) {
        ws = 64;
      }
      if(ws <= 64) {
        if(shortIntegerWordSize != ws) {
          printf("Short integer word size should be %u but it is %u!\n", ws, shortIntegerWordSize);
          exit(-1);
        }
      }
      else {
        printf("Missformed word size checking. The rvalue must be from 0 to 64 (0 is the same as 64).\n");
        exit(-1);
      }
    }
    else {
      printf("Missformed word size checking. The rvalue must be a number from 0 to 64 (0 is the same as 64).\n");
      exit(-1);
    }
  }

  //Checking significant digits
  else if(strcmp(l, "SD") == 0) {
    if(   (r[0] >= '0' && r[0] <= '9' && r[1] == 0)
       || (r[0] >= '0' && r[0] <= '9' && r[1] >= '0' && r[1] <= '9' && r[2] == 0)) {
      uint16_t sd = atoi(r);

      if(sd <= 34) {
        if(significantDigits != sd) {
          printf("Number of significant digits should be %u but it is %u!\n", sd, significantDigits);
          exit(-1);
        }
      }
      else {
        printf("Missformed significant digits checking. The rvalue must be from 0 to 34 (0 is the same as 34).\n");
        exit(-1);
      }
    }
    else {
      printf("Missformed significant digits checking. The rvalue must be a number from 0 to 34 (0 is the same as 34).\n");
      exit(-1);
    }
  }

  //Checking rounding mode
  else if(strcmp(l, "RM") == 0) {
    if(r[0] >= '0' && r[0] <= '9' && r[1] == 0) {
      uint16_t rm = atoi(r);

      if(rm <= 6) {
        if(roundingMode != rm) {
          printf("Rounding mode should be %u but it is %u!\n", rm, roundingMode);
          exit(-1);
        }
      }
      else {
        printf("Missformed rounding mode checking. The rvalue must be a number from 0 to 6.\n");
        exit(-1);
      }
    }
    else {
      printf("Missformed rounding mode checking. The rvalue must be a number from 0 to 6.\n");
      exit(-1);
    }
  }

  //Checking error code
  else if(strcmp(l, "EC") == 0) {
    if(   (r[0] >= '0' && r[0] <= '9' && r[1] == 0)
       || (r[0] >= '0' && r[0] <= '9' && r[1] >= '0' && r[1] <= '9' && r[2] == 0)) {
      uint16_t ec = atoi(r);

      if(ec <= 28) {
        if(lastErrorCode != ec) {
          printf("Last error code should be %u (%s) but it is %u (%s)!\n", ec, errorMessages[ec], lastErrorCode, errorMessages[lastErrorCode]);
          exit(-1);
        }
      }
      else {
        printf("Missformed error code checking. The rvalue must be a number from 0 to 28.\n");
        exit(-1);
      }
    }
    else {
      printf("Missformed error code checking. The rvalue must be a number from 0 to 28.\n");
      exit(-1);
    }
  }

  //Checking a register
  else if(l[0] == 'R') {
    calcRegister_t regist;

    //Lettered register
    if(l[1] >= 'A' && l[2] == 0) {
      if(strstr("XYZTABCDLIJK", l + 1) != NULL) {
        letter = l[1];
        regist = l[1] == 'T' ? 103 :
                 l[1] == 'L' ? 108 :
                 l[1] <= 'D' ? l[1] + 39 :
                 l[1] <= 'K' ? l[1] + 36 :
                               l[1] + 12;
      }
      else {
        printf("Missformed lettered register checking. The letter after R is not a lettered register.\n");
        exit(-1);
      }
    }

    //Numbered register
    else if(   (l[1] >= '0' && l[1] <= '9' && l[2] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] >= '0' && l[3] <= '9' && l[4] == 0)) {
      regist = atoi(l + 1);
      if(regist > 111) {
        printf("Missformed numbered register checking. Th number after R shall be a number from 0 to 111.\n");
        exit(-1);
      }
      letter = 0;
    }

    else {
      printf("Missformed register checking. After R there shall be a number from 0 to 111 or a lettered register.\n");
      exit(-1);
    }

    // find the : separating the data type and the value
    i = 0;
    while(r[i] != ':' && r[i] != 0) i++;
    if(r[i] == 0) {
      printf("Missformed register value. Missing colon between data type and value.\n");
      exit(-1);
    }

    // separating the data type and the value
    r[i] = 0;
    strcpy(l, r);
    strcpy(r, r + i + 1);

    if(strcmp(l, "LONI") == 0) {
      longInteger_t expectedLongInteger, registerLongInteger;

      checkRegisterType(regist, letter, dtLongInteger);

      // remove beginning and ending " and removing leading spaces
      strcpy(r, r + 1);
      while(r[0] == ' ') strcpy(r, r + 1);
      r[strlen(r) - 1] = 0;

      stringToLongInteger(r, 10, &expectedLongInteger);
      convertLongIntegerRegisterToLongInteger(regist, &registerLongInteger);
      if(longIntegerCompare(&expectedLongInteger, &registerLongInteger) != LONG_INTEGER_EQUAL) {
        wrongRegisterValue(regist, letter, r);
      }
    }
    else if(strcmp(l, "RE16") == 0) {
      real16_t expectedReal16;

      checkRegisterType(regist, letter, dtReal16);

      // remove beginning and ending " and removing leading spaces
      strcpy(r, r + 1);
      while(r[0] == ' ') strcpy(r, r + 1);
      r[strlen(r) - 1] = 0;

      // removing trailing spaces
      while(r[strlen(r) - 1] == ' ') r[strlen(r) - 1] = 0;

      // replace , with .
      for(int i=0; i<(int)strlen(r); i++) {
        if(r[i] == ',') r[i] = '.';
      }

      stringToReal16(r, &expectedReal16);
      if(!real16CompareEqual(&expectedReal16, REGISTER_REAL16_DATA(regist))) {
        wrongRegisterValue(regist, letter, r);
      }
    }
    else if(strcmp(l, "CO16") == 0) {
      real16_t expectedReal16, expectedImag16;

      checkRegisterType(regist, letter, dtComplex16);

      // remove beginning and ending " and removing leading spaces
      strcpy(r, r + 1);
      while(r[0] == ' ') strcpy(r, r + 1);
      r[strlen(r) - 1] = 0;

      // find the i separating the real and imagynary part
      i = 0;
      while(r[i] != 'i' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("Missformed register complex value. Missing i between real and imaginary part.\n");
        exit(-1);
      }

      // separate real and imaginary part
      r[i] = 0;
      strcpy(real, r);
      strcpy(imag, r + i + 1);

      // remove leading spaces
      while(imag[0] == ' ') strcpy(imag, imag + 1);

      // removing trailing spaces from real part
      while(real[strlen(real) - 1] == ' ') real[strlen(real) - 1] = 0;

      // removing trailing spaces from imaginary part
      while(imag[strlen(imag) - 1] == ' ') imag[strlen(imag) - 1] = 0;

      // replace , with . in the real part
      for(int i=0; i<(int)strlen(real); i++) {
        if(real[i] == ',') real[i] = '.';
      }

      // replace , with . in the imaginary part
      for(int i=0; i<(int)strlen(imag); i++) {
        if(imag[i] == ',') imag[i] = '.';
      }

      stringToReal16(real, &expectedReal16);
      stringToReal16(imag, &expectedImag16);
      if(!real16CompareEqual(&expectedReal16, REGISTER_REAL16_DATA(regist)) || !real16CompareEqual(&expectedImag16, REGISTER_IMAG16_DATA(regist))) {
        wrongRegisterValue(regist, letter, r);
      }
    }
    else if(strcmp(l, "ANGL") == 0) {
      #if (ANGLE16 == 1)
        real16_t expectedReal16;
      #else
        real34_t expectedReal34;
      #endif

      checkRegisterType(regist, letter, dtAngle);

      // find the : separating the angle value from the angular mode
      i = 0;
      while(r[i] != ':' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("Missformed register angle value. Missing colon between angle value and angular mode.\n");
        exit(-1);
      }

      // separate angle value and angular mode
      r[i] = 0;
      strcpy(angMod, r + i + 1);

           if(strcmp(angMod, "DEG"   ) == 0) am = AM_DEGREE;
      else if(strcmp(angMod, "DMS"   ) == 0) am = AM_DMS;
      else if(strcmp(angMod, "GRAD"  ) == 0) am = AM_GRAD;
      else if(strcmp(angMod, "RAD"   ) == 0) am = AM_RADIAN;
      else if(strcmp(angMod, "MULTPI") == 0) am = AM_MULTPI;
      else {
        printf("Missformed register angle unit. Unknown unit after angle value.\n");
        exit(-1);
      }

      if(getRegisterAngularMode(regist) != (uint32_t)am) {
        if(letter == 0) {
          printf("The expected angular mode for R%d is %s but it is %s\n", regist, angMod, getAngularModeName(getRegisterAngularMode(regist)));
        }
        else {
          printf("The expected angular mode for R%c is %s but it is %s\n", letter, angMod, getAngularModeName(getRegisterAngularMode(regist)));
        }
        exit(-1);
      }

      // remove beginning and ending " and removing leading spaces
      strcpy(r, r + 1);
      while(r[0] == ' ') strcpy(r, r + 1);
      r[strlen(r) - 1] = 0;

      // replace , with .
      for(int i=0; i<(int)strlen(r); i++) {
        if(r[i] == ',') r[i] = '.';
      }

      #if (ANGLE16 == 1)
        real16_t isValue;

        stringToReal16(r, &expectedReal16);
        real16Copy(REGISTER_ANGLE_DATA(regist), &isValue);
        convertAngle16FromInternal(&isValue, am);
        if(!real16CompareEqual(&expectedReal16, &isValue)) {
          wrongRegisterValue(regist, letter, r);
        }
      #else
        real34_t isValue;

        stringToReal34(r, &expectedReal34);
        real34Copy(REGISTER_ANGLE_DATA(regist), &isValue);
        convertAngle34FromInternal(&isValue, am);
        if(!real34CompareEqual(&expectedReal34, &isValue)) {
          wrongRegisterValue(regist, letter, r);
      #endif
    }
    else if(strcmp(l, "STRI") == 0) {
      checkRegisterType(regist, letter, dtString);

      r[strlen(r) - 1] = 0;

      for(int i=strlen(r); i>0; i--) {
        if(r[i - 1] == '\\') {
          strcpy(r + i - 1, r + i);
          i--;
        }
      }

      if(stringByteLength(r + 1) != stringByteLength(REGISTER_STRING_DATA(regist))) {
        printf("The 2 strings are not of the same size.\n");
        printf("Expected string: |%s|\n", r + 1);
        printf("Register string: |%s|\n", REGISTER_STRING_DATA(regist));
        exit(-1);
      }

      int i;
      char *expected, *is;
      for(i=stringByteLength(r + 1), expected=r + 1, is=REGISTER_STRING_DATA(regist);
          i>0;
          i--, expected++, is++) {
        if(*expected != *is) {
          printf("The 2 strings are different.\n");
          printf("Expected string: |%s|\n", r + 1);
          printf("Register string: |%s|\n", REGISTER_STRING_DATA(regist));
          exit(-1);
        }
      }
    }
    else if(strcmp(l, "SHOI") == 0) {
      checkRegisterType(regist, letter, dtShortInteger);

      // find the # separating the value from the base
      i = 0;
      while(r[i] != '#' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("Missformed register short integer value. Missing # between value and base.\n");
        exit(-1);
      }

      // remove beginning and ending " and removing leading spaces
      strcpy(r, r + 1);
      while(r[0] == ' ') strcpy(r, r + 1);
      r[strlen(r) - 1] = 0;

      opX = allocateTemporaryRegister();
      strToShortInteger(r, opX);
      if(*REGISTER_SHORT_INTEGER_DATA(opX) != *REGISTER_SHORT_INTEGER_DATA(regist)) {
        wrongRegisterValue(regist, letter, r);
        freeTemporaryRegister(opX);
        exit(-1);
      }
      freeTemporaryRegister(opX);
    }
    else if(strcmp(l, "RE34") == 0) {
      real34_t expectedReal34;

      checkRegisterType(regist, letter, dtReal34);

      strcpy(r, r + 1);
      while(r[0] == ' ') strcpy(r, r + 1);
      r[strlen(r) - 1] = 0;

      while(r[strlen(r) - 1] == ' ') r[strlen(r) - 1] = 0;
      for(int i=0; i<(int)strlen(r); i++) {
        if(r[i] == ',') r[i] = '.';
      }

      stringToReal34(r, &expectedReal34);
      if(!real34CompareEqual(&expectedReal34, REGISTER_REAL34_DATA(regist))) {
        wrongRegisterValue(regist, letter, r);
      }
    }
    else if(strcmp(l, "CO34") == 0) {
      real34_t expectedReal34, expectedImag34;

      checkRegisterType(regist, letter, dtComplex34);

      // remove beginning and ending " and removing leading spaces
      strcpy(r, r + 1);
      while(r[0] == ' ') strcpy(r, r + 1);
      r[strlen(r) - 1] = 0;

      // find the i separating the real and imagynary part
      i = 0;
      while(r[i] != 'i' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("Missformed register complex value. Missing i between real and imaginary part.\n");
        exit(-1);
      }

      // separate real and imaginary part
      r[i] = 0;
      strcpy(real, r);
      strcpy(imag, r + i + 1);

      // remove leading spaces
      while(imag[0] == ' ') strcpy(imag, imag + 1);

      // removing trailing spaces from real part
      while(real[strlen(real) - 1] == ' ') real[strlen(real) - 1] = 0;

      // removing trailing spaces from imaginary part
      while(imag[strlen(imag) - 1] == ' ') imag[strlen(imag) - 1] = 0;

      // replace , with . in the real part
      for(int i=0; i<(int)strlen(real); i++) {
        if(real[i] == ',') real[i] = '.';
      }

      // replace , with . in the imaginary part
      for(int i=0; i<(int)strlen(imag); i++) {
        if(imag[i] == ',') imag[i] = '.';
      }

      stringToReal34(real, &expectedReal34);
      stringToReal34(imag, &expectedImag34);
      if(!real34CompareEqual(&expectedReal34, REGISTER_REAL34_DATA(regist)) || !real34CompareEqual(&expectedImag34, REGISTER_IMAG34_DATA(regist))) {
        wrongRegisterValue(regist, letter, r);
      }
    }
    else {
      printf("Missformed register value. Unknown data type %s for register %s\n", l, p+1);
      exit(-1);
    }
  }

  else {
    printf("Unknown checking %s\n", l);
    exit(-1);
  }
}



void outParameters(char *token) {
  char parameter[200];
  int32_t index, lg;

  while(*token == ' ') token++;
  while(*token != 0) {
    index = 0;
    while(*token != ' ' && *token != 0) {
      if(*token == '"') { // Inside a string
        lg = endOfString(token) - token;
        strncpy(parameter + index, token, lg--);
        index += lg;
        token += lg;
      }
      parameter[index++] = *(token++);
    }
    parameter[index] = 0;

    printf("  Check %s\n", parameter);
    checkExpectedOutParameter(parameter);

    while(*token == ' ') token++;
  }
}



void functionToCall(char *functionName) {
  int32_t function;
  void (*func)(uint16_t);

  lastErrorCode = 0;
  function = 0;
  while(funcTestNoParam[function].name[0] != 0 && strcmp(funcTestNoParam[function].name, functionName) != 0) {
    function++;
  }
  if(funcTestNoParam[function].name[0] != 0) {
    int32_t index;

    //ici il faut tester la présence de fromUnitToISO ou fromISOtoUnit

    func = funcTestNoParam[function].func;
    func(NOPARAM);

    for(index=1; index<=LAST_ITEM; index++) {
      if(indexOfItems[index].func == func) {
        break;
      }
    }

    if(index < LAST_ITEM) {
      if(indexOfItems[index].stackLiftStatus == SLS_DISABLED) {
        stackLiftEnabled = false;
      }
      else if(indexOfItems[index].stackLiftStatus == SLS_ENABLED) {
        stackLiftEnabled = true;
      }
    }
    return;
  }
  else {
    function = 0;
    while(funcTestCvt[function].name[0] != 0 && strcmp(funcTestCvt[function].name, functionName) != 0) {
      function++;
    }
    if(funcTestCvt[function].name[0] != 0) {
      int32_t index;

      func = funcTestCvt[function].func;
      func(NOPARAM);

      for(index=1; index<=LAST_ITEM; index++) {
        if(indexOfItems[index].func == func) {
          break;
        }
      }

      if(index < LAST_ITEM) {
        if(indexOfItems[index].stackLiftStatus == SLS_DISABLED) {
          stackLiftEnabled = false;
        }
        else if(indexOfItems[index].stackLiftStatus == SLS_ENABLED) {
          stackLiftEnabled = true;
        }
      }
      return;
    }
    else {
    }
  }

  printf("Cannot find the function to test: check spelling of the function name and remember the name is case sensitive\n");
  exit(-1);
}



void processLine(char *line) {
  char *location;

  // trim comments
  location = strstr(line, ";");
  if(location != NULL) {
    *location = 0;
  }

  // trim ending LF
  location = strstr(line, "\n");
  if(location != NULL) {
    *location = 0;
  }

  // trim ending CR
  location = strstr(line, "\r");
  if(location != NULL) {
    *location = 0;
  }

  // Change tabs in spaces
  for(int i=strlen(line)-1; i>0; i--) {
    if(line[i] == '\t') {
      line[i] = ' ';
    }
  }

  // Trim ending spaces
  for(int i=strlen(line)-1; i>0; i--) {
    if(line[i] == ' ') {
      line[i] = 0;
    }
    else {
      break;
    }
  }

  // Trim beginning spaces
  while(line[0] == ' ') {
    strcpy(line, line + 1);
  }

  // convert to upper case
  int32_t lg = strlen(line);
  for(int i=0; i<lg; i++) {
    if(line[i] == '"') {
      i = endOfString(line + i) - line;
    }

    if('a' <= line[i] && line[i] <= 'z') {
      line[i] -= 32;
    }
    if(i >= 5 && strncmp(line, "FUNC: ", 6) == 0) {
      break;
    }
  }

  if(strncmp(line, "IN: ", 4) == 0) {
    printf("**************************************************************************************************************\nLine %d   %s\n", lineNumber, line);
    inParameters(line + 4);
  }

  else if(strncmp(line, "FUNC: ", 6) == 0) {
    printf("\nLine %d   %s\n", lineNumber, line);
    functionToCall(line + 6);
  }

  else if(strncmp(line, "OUT: ", 5) == 0) {
    printf("\nLine %d   %s\n", lineNumber, line);
    outParameters(line + 5);
  }

  else if(strlen(line) == 0) {}

  else {
    printf("\nLine %d cannot be processed\n%s\n", lineNumber, line);
    exit(-1);
  }
}
