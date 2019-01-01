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
 * \file debug.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief Returns the name of a data type
 *
 * \param[in] dt uint16_t Data type
 * \return char*          Name of the data type
 ***********************************************/
char * getDataTypeName(uint16_t dt, bool_t article, bool_t padWithBlanks) {
  if(article && padWithBlanks) {
    if(dt == dtBigInteger      ) return "a big integer        ";
    if(dt == dtReal16          ) return "a real16             ";
    if(dt == dtComplex16       ) return "a complex16          ";
    if(dt == dtTime            ) return "a time               ";
    if(dt == dtDate            ) return "a date               ";
    if(dt == dtString          ) return "a string             ";
    if(dt == dtReal16Matrix    ) return "a real16 matrix      ";
    if(dt == dtComplex16Matrix ) return "a complex16 matrix   ";
    if(dt == dtSmallInteger    ) return "a small integer      ";
    if(dt == dtReal34          ) return "a real34             ";
    if(dt == dtComplex34       ) return "a complex34          ";
    //if(dt == dtLabel           ) return "a label              ";
    //if(dt == dtSystemInteger   ) return "a system integer     ";
    //if(dt == dtFlags           ) return "a flags              ";
    //if(dt == dtConfig          ) return "a config             ";
    //if(dt == dtPgmStep         ) return "a pgm step           ";
    //if(dt == dtDirectory       ) return "a directory          ";
    return                              "a ???                ";
  }
  else if(article && !padWithBlanks) {
    if(dt == dtBigInteger      ) return "a big integer";
    if(dt == dtReal16          ) return "a real16";
    if(dt == dtComplex16       ) return "a complex16";
    if(dt == dtTime            ) return "a time";
    if(dt == dtDate            ) return "a date";
    if(dt == dtString          ) return "a string";
    if(dt == dtReal16Matrix    ) return "a real16 matrix";
    if(dt == dtComplex16Matrix ) return "a complex16 matrix";
    if(dt == dtSmallInteger    ) return "a small integer";
    if(dt == dtReal34          ) return "a real34";
    if(dt == dtComplex34       ) return "a complex34";
    //if(dt == dtLabel           ) return "a label";
    //if(dt == dtSystemInteger   ) return "a system integer";
    //if(dt == dtFlags           ) return "a flags";
    //if(dt == dtConfig          ) return "a config";
    //if(dt == dtPgmStep         ) return "a pgm step";
    //if(dt == dtDirectory       ) return "a directory";
    return                              "a ???";
  }
  else if(!article && padWithBlanks) {
    if(dt == dtBigInteger      ) return "big integer          ";
    if(dt == dtReal16          ) return "real16               ";
    if(dt == dtComplex16       ) return "complex16            ";
    if(dt == dtTime            ) return "time                 ";
    if(dt == dtDate            ) return "date                 ";
    if(dt == dtString          ) return "string               ";
    if(dt == dtReal16Matrix    ) return "real16 matrix        ";
    if(dt == dtComplex16Matrix ) return "complex16 matrix     ";
    if(dt == dtSmallInteger    ) return "small integer        ";
    if(dt == dtReal34          ) return "real34               ";
    if(dt == dtComplex34       ) return "complex34            ";
    //if(dt == dtLabel           ) return "label                ";
    //if(dt == dtSystemInteger   ) return "system integer       ";
    //if(dt == dtFlags           ) return "flags                ";
    //if(dt == dtConfig          ) return "config               ";
    //if(dt == dtPgmStep         ) return "pgm step             ";
    //if(dt == dtDirectory       ) return "directory            ";
    return                              "???                  ";
  }
  else if(!article && !padWithBlanks) {
    if(dt == dtBigInteger      ) return "big integer";
    if(dt == dtReal16          ) return "real16";
    if(dt == dtComplex16       ) return "complex16";
    if(dt == dtTime            ) return "time";
    if(dt == dtDate            ) return "date";
    if(dt == dtString          ) return "string";
    if(dt == dtReal16Matrix    ) return "real16 matrix";
    if(dt == dtComplex16Matrix ) return "complex16 matrix";
    if(dt == dtSmallInteger    ) return "small integer";
    if(dt == dtReal34          ) return "real34";
    if(dt == dtComplex34       ) return "complex34";
    //if(dt == dtLabel           ) return "label";
    //if(dt == dtSystemInteger   ) return "system integer";
    //if(dt == dtFlags           ) return "flags";
    //if(dt == dtConfig          ) return "config";
    //if(dt == dtPgmStep         ) return "pgm step";
    //if(dt == dtDirectory       ) return "directory";
    return                              "???";
  }
  else {
    return                              "???";
  }
}



/********************************************//**
 * \brief Returns the name of a data type of a register
 *
 * \param[in] dt calcRegister_t register
 * \return char* Name of the data type
 ***********************************************/
char * getRegisterDataTypeName(calcRegister_t regist, bool_t article, bool_t padWithBlanks) {
  uint16_t dt;
  static char str[22];

  dt = getRegisterDataType(regist);

  if(article && padWithBlanks) {
    if(dt == dtBigInteger      ) return "a big integer        ";
    if(dt == dtReal16          ) return "a real16             ";
    if(dt == dtComplex16       ) return "a complex16          ";
    if(dt == dtTime            ) return "a time               ";
    if(dt == dtDate            ) return "a date               ";
    if(dt == dtString          ) return "a string             ";
    if(dt == dtReal16Matrix    ) return "a real16 matrix      ";
    if(dt == dtComplex16Matrix ) return "a complex16 matrix   ";
    if(dt == dtSmallInteger    ) {
      sprintf(str, "a small integer (%2" FMT32U ") ", getRegisterBase(regist));
      return str;
    }
    if(dt == dtReal34          ) return "a real34             ";
    if(dt == dtComplex34       ) return "a complex34          ";
    //if(dt == dtLabel           ) return "a label              ";
    //if(dt == dtSystemInteger   ) return "a system integer     ";
    //if(dt == dtFlags           ) return "a flags              ";
    //if(dt == dtConfig          ) return "a config             ";
    //if(dt == dtPgmStep         ) return "a pgm step           ";
    //if(dt == dtDirectory       ) return "a directory          ";
    return                              "a ???                ";
  }
  else if(article && !padWithBlanks) {
    if(dt == dtBigInteger      ) return "a big integer";
    if(dt == dtReal16          ) return "a real16";
    if(dt == dtComplex16       ) return "a complex16";
    if(dt == dtTime            ) return "a time";
    if(dt == dtDate            ) return "a date";
    if(dt == dtString          ) return "a string";
    if(dt == dtReal16Matrix    ) return "a real16 matrix";
    if(dt == dtComplex16Matrix ) return "a complex16 matrix";
    if(dt == dtSmallInteger    ) {
      sprintf(str, "a small integer (%2" FMT32U ")", getRegisterBase(regist));
      return str;
    }
    if(dt == dtReal34          ) return "a real34";
    if(dt == dtComplex34       ) return "a complex34";
    //if(dt == dtLabel           ) return "a label";
    //if(dt == dtSystemInteger   ) return "a system integer";
    //if(dt == dtFlags           ) return "a flags";
    //if(dt == dtConfig          ) return "a config";
    //if(dt == dtPgmStep         ) return "a pgm step";
    //if(dt == dtDirectory       ) return "a directory";
    return                              "a ???";
  }
  else if(!article && padWithBlanks) {
    if(dt == dtBigInteger      ) return "big integer          ";
    if(dt == dtReal16          ) return "real16               ";
    if(dt == dtComplex16       ) return "complex16            ";
    if(dt == dtTime            ) return "time                 ";
    if(dt == dtDate            ) return "date                 ";
    if(dt == dtString          ) return "string               ";
    if(dt == dtReal16Matrix    ) return "real16 matrix        ";
    if(dt == dtComplex16Matrix ) return "complex16 matrix     ";
    if(dt == dtSmallInteger    ) {
      sprintf(str, "small integer (%2" FMT32U ")   ", getRegisterBase(regist));
      return str;
    }
    if(dt == dtReal34          ) return "real34               ";
    if(dt == dtComplex34       ) return "complex34            ";
    //if(dt == dtLabel           ) return "label                ";
    //if(dt == dtSystemInteger   ) return "system integer       ";
    //if(dt == dtFlags           ) return "flags                ";
    //if(dt == dtConfig          ) return "config               ";
    //if(dt == dtPgmStep         ) return "pgm step             ";
    //if(dt == dtDirectory       ) return "directory            ";
    return                              "???                  ";
  }
  else if(!article && !padWithBlanks) {
    if(dt == dtBigInteger      ) return "big integer";
    if(dt == dtReal16          ) return "real16";
    if(dt == dtComplex16       ) return "complex16";
    if(dt == dtTime            ) return "time";
    if(dt == dtDate            ) return "date";
    if(dt == dtString          ) return "string";
    if(dt == dtReal16Matrix    ) return "real16 matrix";
    if(dt == dtComplex16Matrix ) return "complex16 matrix";
    if(dt == dtSmallInteger    ) {
      sprintf(str, "small integer (%" FMT32U ")", getRegisterBase(regist));
      return str;
    }
    if(dt == dtReal34          ) return "real34";
    if(dt == dtComplex34       ) return "complex34";
    //if(dt == dtLabel           ) return "label";
    //if(dt == dtSystemInteger   ) return "system integer";
    //if(dt == dtFlags           ) return "flags";
    //if(dt == dtConfig          ) return "config";
    //if(dt == dtPgmStep         ) return "pgm step";
    //if(dt == dtDirectory       ) return "directory";
    return                              "???";
  }
  else {
    return                              "???";
  }
}


/********************************************//**
 * \brief Returns the name of an angukar mode
 *
 * \param[in] am uint16_t Angular mode
 * \return char*          Name of the angular mode
 ***********************************************/
char * getAngularModeName(uint16_t am) {
  if(am == AM_DEGREE) return "degree";
  if(am == AM_DMS   ) return "d.ms  ";
  if(am == AM_GRAD  ) return "grad  ";
  if(am == AM_MULTPI) return "multPi";
  if(am == AM_RADIAN) return "radian";

  return "???   ";
}


/********************************************//**
 * \brief Returns the name of an integer mode
 *
 * \param[in] im uint16_t Integer mode
 * \return char*          Name of the integer mode
 ***********************************************/
char * getSmallIntegerModeName(uint16_t im) {
  if(im == SIM_1COMPL) return "1compl";
  if(im == SIM_2COMPL) return "2compl";
  if(im == SIM_SIGNMT) return "signmt";
  if(im == SIM_UNSIGN) return "unsign";

  return "???   ";
}



void debugNIM(void) {
  if     (nimNumberPart == NP_EMPTY               ) printf("nimNumberPart = NP_EMPTY               \n");
  else if(nimNumberPart == NP_INT_10              ) printf("nimNumberPart = NP_INT_10              \n");
  else if(nimNumberPart == NP_INT_16              ) printf("nimNumberPart = NP_INT_16              \n");
  else if(nimNumberPart == NP_INT_BASE            ) printf("nimNumberPart = NP_INT_BASE            \n");
  else if(nimNumberPart == NP_REAL_FLOAT_PART     ) printf("nimNumberPart = NP_REAL_FLOAT_PART     \n");
  else if(nimNumberPart == NP_REAL_EXPONENT       ) printf("nimNumberPart = NP_REAL_EXPONENT       \n");
  else if(nimNumberPart == NP_FRACTION_DENOMINATOR) printf("nimNumberPart = NP_FRACTION_DENOMINATOR\n");
  else if(nimNumberPart == NP_COMPLEX_INT_PART    ) printf("nimNumberPart = NP_COMPLEX_INT_PART    \n");
  else if(nimNumberPart == NP_COMPLEX_FLOAT_PART  ) printf("nimNumberPart = NP_COMPLEX_FLOAT_PART  \n");
  else if(nimNumberPart == NP_COMPLEX_EXPONENT    ) printf("nimNumberPart = NP_COMPLEX_EXPONENT    \n");
  else                                              printf("nimNumberPart = NP_???                 \n");
}



#if (LOG_FUNCTIONS == 1)
  int16_t logIndent = 0;

  void enteringFunction(char *func) {
    for(int16_t i=0; i<logIndent; i++) {
      putchar(' ');
    }
    printf("Entering %s\n", func);
    logIndent += 1;
  }



  void leavingFunction(char *func) {
    logIndent -= 1;
    for(int16_t i=0; i<logIndent; i++) {
      putchar(' ');
    }
    printf("Leaving  %s\n", func);
  }
#endif



#if (DEBUG_PANEL == 1)
  /********************************************//**
   * \brief Returns the name of a denominator mode
   *
   * \param[in] dm uint16_t Denominator mode
   * \return char*          Name of the denominator mode
   ***********************************************/
  char * getDenModeName(uint16_t dm) {
    if(dm == DM_ANY) return "any";
    if(dm == DM_FAC) return "fac";
    if(dm == DM_FIX) return "fix";

    return "???";
  }


  /********************************************//**
   * \brief Returns the name of a display format
   *
   * \param[in] df uint16_t Display format
   * \return char*          Name of the display format
   ***********************************************/
  char * getDisplayFormatName(uint16_t df) {
    if(df == DF_ALL) return "all";
    if(df == DF_FIX) return "fix";
    if(df == DF_SCI) return "sci";
    if(df == DF_ENG) return "eng";

    return "???";
  }


  /********************************************//**
   * \brief Returns the name of a time format
   *
   * \param[in] tf uint16_t Time format
   * \return char*          Name of the time format
   ***********************************************/
  char * getTimeFormatName(uint16_t tf) {
    if(tf == TF_H12) return "H12";
    if(tf == TF_H24) return "H24";

    return "???";
  }


  /********************************************//**
   * \brief Returns the name of a date format
   *
   * \param[in] df uint16_t Date format
   * \return char*          Name of the date format
   *
   ***********************************************/
  char * getDateFormatName(uint16_t df) {
    if(df == DF_DMY) return "D.MY";
    if(df == DF_MDY) return "M.DY";
    if(df == DF_YMD) return "Y.MD";

    return "??? ";
  }


  /********************************************//**
   * \brief Returns the name of a boolean
   *
   * \param[in] b uint16_t Boolean
   * \return char*         Name of the boolean
   ***********************************************/
  char * getBooleanName(uint16_t b) {
    if(b) {
      return "true";
    }
    else {
      return "false";
    }
  }


  /********************************************//**
   * \brief Returns the name of a RBR mode
   *
   * \param[in] mode uint16_t
   * \return char*         Name of the mode
   ***********************************************/
  char * getRbrModeName(uint16_t mode) {
    if(mode == RBR_GLOBAL) {
      return "RBR_GLOBAL";
    }
    else if(mode == RBR_LOCAL) {
      return "RBR_LOCAL";
    }
    else if(mode == RBR_NAMED) {
      return "RBR_NAMED";
    }
   else {
     return "RBR_???";
   }
  }


  /********************************************//**
   * \brief Returns the name of a curve fitting mode
   *
   * \param[in] cf uint16_t Curve fitting mode
   * \return char*          Name of the curve fitting mode
   ***********************************************/
  char * getCurveFittingName(uint16_t cf) {
    if(cf == CF_BEST_FITTING)        return "best fit ";
    if(cf == CF_EXPONENTIAL_FITTING) return "exp fit  ";
    if(cf == CF_LINEAR_FITTING)      return "lin fit  ";
    if(cf == CF_LOGARITHMIC_FITTING) return "log fit  ";
    if(cf == CF_POWER_FITTING)       return "power fit";

    return "???      ";
  }


  /********************************************//**
   * \brief Returns the name of a rounding mode
   *
   * \param[in] rm uint16_t Rounding mode
   * \return char*          Name of the rounding mode
   ***********************************************/
  char * getRoundingModeName(uint16_t rm) {
    if(rm == RM_CEIL)      return "ceil     ";
    if(rm == RM_DOWN)      return "down     ";
    if(rm == RM_FLOOR)     return "floor    ";
    if(rm == RM_HALF_DOWN) return "half down";
    if(rm == RM_HALF_EVEN) return "half even";
    if(rm == RM_HALF_UP)   return "half up  ";
    if(rm == RM_UP)        return "up       ";

    return "???      ";
  }


  /********************************************//**
   * \brief Returns the name of a calc mode
   *
   * \param[in] cm uint16_t Calc mode
   * \return char*          Name of the calc mode
   ***********************************************/
  char * getCalcModeName(uint16_t cm) {
    if(cm == CM_AIM)    return "AIM   ";
    if(cm == CM_NORMAL) return "normal";
    if(cm == CM_TAM)    return "TAM   ";
    if(cm == CM_NIM)    return "NIM   ";

    return "???   ";
  }


  /********************************************//**
   * \brief Returns the name of a TAM mode
   *
   * \param[in] cm uint16_t TAM
   * \return char*          Name of the TAM mode
   ***********************************************/
  char * getTamModeName(uint16_t cm) {
    if(cm == TM_NORMAL)  return "normal";
    if(cm == TM_CMP)     return "CMP   ";
    if(cm == TM_STO_RCL) return "STORCL";

    return "???   ";
  }


  /********************************************//**
   * \brief Returns the name of a "next char indicator"
   *
   * \param[in] nc uint16_t Next char indicator
   * \return char*          Name of the "next char indicator"
   ***********************************************/
  char * getNextCharName(uint16_t nc) {
    if(nc == NC_NORMAL)      return "normal     ";
    if(nc == NC_SUBSCRIPT)   return "subscript  ";
    if(nc == NC_SUPERSCRIPT) return "superscript";

    return "???        ";
  }


  /********************************************//**
   * \brief Returns the name of a complex unit
   *
   * \param[in] cu uint16_t Complex unit
   * \return char*          Name of the complex unit
   ***********************************************/
  char * getComplexUnitName(uint16_t cu) {
    if(cu == CU_I) return "i";
    if(cu == CU_J) return "j";

    return "?";
  }


  /********************************************//**
   * \brief Returns the name of a Product sign
   *
   * \param[in] ps uint16_t Product sign
   * \return char*          Name of the product sign
   ***********************************************/
  char * getProductSignName(uint16_t ps) {
    if(ps == PS_CROSS) return "cross";
    if(ps == PS_DOT)   return "dot  ";

    return "???  ";
  }


  /********************************************//**
   * \brief returns the name of a fraction type
   *
   * \param[in] ft uint16_t Fraction type
   * \return char*          Name of the fraction type
   ***********************************************/
  char * getFractionTypeName(uint16_t ft) {
    if(ft == FT_IMPROPER) return "improper d/c";
    if(ft == FT_PROPER)   return "proper a b/c";

    return "???         ";
  }


  /********************************************//**
   * \brief returns the name of a radix mark
   *
   * \param[in] rm uint16_t Radix mark
   * \return char*          Name of the radix mark
   ***********************************************/
  char * getRadixMarkName(uint16_t rm) {
    if(rm == RM_COMMA)  return "comma ";
    if(rm == RM_PERIOD) return "period";

    return "???   ";
  }


  /********************************************//**
   * \brief returns the name of a display override mode
   *
   * \param[in] dio uint16_t Display override mode
   * \return char*           Name of the override mode
   ***********************************************/
  char * getDisplayOvrName(uint16_t dio) {
    if(dio == DO_ENG) return "engOvr";
    if(dio == DO_SCI) return "sciOvr";

    return "???   ";
  }


  /********************************************//**
   * \brief returns the name of a stack size
   *
   * \param[in] ss uint16_t Stack size
   * \return char*          Name of the stack size
   ***********************************************/
  char * getStackSizeName(uint16_t ss) {
    if(ss == SS_4) return "4 levels";
    if(ss == SS_8) return "8 levels";

    return "? levels";
  }


  /********************************************//**
   * \brief returns the name of a complex mode
   *
   * \param[in] cm uint16_t Complex mode
   * \return char*          Name of the complex mode
   ***********************************************/
  char * getComplexModeName(uint16_t cm) {
    if(cm == CM_POLAR)       return "polar      ";
    if(cm == CM_RECTANGULAR) return "rectangular";

    return "???        ";
  }


  /********************************************//**
   * \brief returns the name of an alpha case mode
   *
   * \param[in] ac uint16_t Alphacase mode
   * \return char*          Name of the alphacase mode
   ***********************************************/
  char * getAlphaCaseName(uint16_t ac) {
    if(ac == AC_LOWER) return "lower";
    if(ac == AC_UPPER) return "upper";

    return "???  ";
  }


  /********************************************//**
   * \brief returns the name of a cursor font
   *
   * \param[in] cf uint16_t Cursor font
   * \return char*          Name of the cursor font
   ***********************************************/
  char * getCursorFontName(uint16_t cf) {
    if(cf == CF_NUMERIC)  return "numeric ";
    if(cf == CF_STANDARD) return "standard";

    return "???     ";
  }



  /********************************************//**
   * \brief Fills the lth line of the debug window
   * with the value of register r
   *
   * \param[in] r int   Register number
   * \param[in] row int Row number
   * \return void
   *
   ***********************************************/
  void debugRegisterValue(calcRegister_t regist, int row) {
    #if (LOG_FUNCTIONS == 1)
      enteringFunction("debugRegisterValue");
    #endif

    char     string[1000], *p;
    uint16_t i, k, n=0;

    if(1000 <= regist && regist < 1000+numberOfNamedRegisters) { // Named register
      n = stringByteLength(POINTER_TO_REGISTER_NAME(regist));
      for(i=0, p=POINTER_TO_REGISTER_NAME(regist); i<=n; i++, p++) {
        string[i] = *p;
      }

      strcat(string, STD_SPACE_3_PER_EM "=" STD_SPACE_3_PER_EM);
      n = stringByteLength(string);
    }

    if(getRegisterDataType(regist) == dtReal16) {
      formatReal16Debug(string + n, getRegisterDataPointer(regist));
    }

    else if(getRegisterDataType(regist) == dtReal34) {
      formatReal34Debug(string + n, getRegisterDataPointer(regist));
    }

    else if(getRegisterDataType(regist) == dtComplex16) {
      formatComplex16Debug(string + n, getRegisterDataPointer(regist));
    }

    else if(getRegisterDataType(regist) == dtComplex34) {
      formatComplex34Debug(string + n, getRegisterDataPointer(regist));
    }

    else if(getRegisterDataType(regist) == dtString) {
      if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chkHexaString))) {
        sprintf(string + n, "%04x", *(uint16_t *)(POINTER_TO_REGISTER_DATA(regist)));
        k = 4;
        for(i=2; i<getRegisterDataSize(regist); i++) {
          sprintf(string + n + k, STD_SPACE_3_PER_EM "%02x", (unsigned char)*(POINTER_TO_REGISTER_DATA(regist) + i));
          k += 4;
        }
      }
      else {
        for(i=0, p=POINTER_TO_REGISTER_STRING(regist); i<=stringByteLength(POINTER_TO_REGISTER_STRING(regist)); i++, p++) {
          string[n + i] = *p;
        }
      }
    }

    else if(getRegisterDataType(regist) == dtSmallInteger) {
      const font_t *font = &standardFont;

      smallIntegerToDisplayString(regist, string + n, &font);
      strcat(string + n, STD_SPACE_3_PER_EM);
      strcat(string + n, getSmallIntegerModeName(smallIntegerMode));
    }

    else if(getRegisterDataType(regist) == dtBigInteger) {
      bigIntegerToDisplayString(regist, string + n);
    }

    else {
      sprintf(string + n, "data type %s not supported for now!", getRegisterDataTypeName(regist, false, false));
    }

    while(stringWidth(string, &standardFont, true, true) > 961) {
      string[stringLastGlyph(string)] = 0;
    }

    stringToUtf8(string, (uint8_t *)tmpStr3000);

    gtk_label_set_label(GTK_LABEL(lbl2[row]), tmpStr3000);
    gtk_widget_show(lbl2[row]);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("debugRegisterValue");
    #endif
  }



  /********************************************//**
   * \brief refreshes the debug window
   *
   * \param void
   * \return void
   ***********************************************/
  void refreshDebugPanel(void) {
    #if (LOG_FUNCTIONS == 1)
      enteringFunction("refreshDebugPanel");
    #endif

    char string[100];
    int row;

    if(debugWindow == DBG_BIT_FIELDS) {
      for(int i=0; i<DEBUG_LINES; i++) {
        gtk_widget_hide(lbl1[i]);
        gtk_widget_hide(lbl2[i]);
      }

      row = 0;

      if(row < DEBUG_LINES) {
        sprintf(string, "1stFreeByte                  = %6u",        firstFreeByte);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "lastFreeByte                 = %6u",        lastFreeByte);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "numberOfLocalRegisters       = %6u",        numberOfLocalRegisters);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "allLocalRegisterPointer      = %6u",        allLocalRegisterPointer);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "statisticalSumsPointer       = %6u",        statisticalSumsPointer);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "numberOfNamedRegisters       = %6u",        numberOfNamedRegisters);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "allNamedRegisterPointer      = %6u",        allNamedRegisterPointer);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "stackSize                    = %6u = %s",   stackSize,            getStackSizeName(stackSize));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "softmenuStackPointer         = %6u\n",      softmenuStackPointer);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "fractionType                 = %6u = %s",   fractionType,         getFractionTypeName(fractionType));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "denominatorMode              = %6u = %s",   denominatorMode,      getDenModeName(denominatorMode));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "denMax                       = %6u\n",      denMax);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "PC                           = %6u",        programCounter);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "smallIntegerMode             = %6u = %s",   smallIntegerMode,     getSmallIntegerModeName(smallIntegerMode));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "smallIntegerWordSize         = %6u",        smallIntegerWordSize);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "displatFormat                = %6u = %s",   displayFormat,        getDisplayFormatName(displayFormat));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "displatFormatDigits          = %6u",        displayFormatDigits);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "displayModeOverride          = %6u = %s",   displayModeOverride,  getDisplayOvrName(displayModeOverride));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "groupingGap                  = %6u",        groupingGap);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "significantDigits            = %6u",        significantDigits);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "roundingMode                 = %6u = %s",   roundingMode,         getRoundingModeName(roundingMode));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "angularMode                  = %6u = %s",   angularMode,          getAngularModeName(angularMode));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "timeFormat                   = %6u = %s",   timeFormat,           getTimeFormatName(timeFormat));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "dateFormat                   = %6u = %s",   dateFormat,           getDateFormatName(dateFormat));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "firstGregorianDay            = %6u",        firstGregorianDay);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      //if(row < DEBUG_LINES) {
      //  sprintf(string, "displayDateTime              = %6u = %s",   displayDateTime,      getBooleanName(displayDateTime));
      //  gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
      //  gtk_widget_show(lbl1[row++]);
      //}

      if(row < DEBUG_LINES) {
        sprintf(string, "complexUnit                  = %6u = %s",   complexUnit,          getComplexUnitName(complexUnit));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "displayLeadingZeros          = %6u = %s",   displayLeadingZeros,  getBooleanName(displayLeadingZeros));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "productSign                  = %6u = %s",   productSign,          getProductSignName(productSign));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "radixMark                    = %6u = %s",   radixMark,            getRadixMarkName(radixMark));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "complexMode                  = %6u = %s",   complexMode,          getComplexModeName(complexMode));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "alphaCase                    = %6u = %s",   alphaCase,            getAlphaCaseName(alphaCase));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "hourGlassIconEnabled         = %6u = %s",   hourGlassIconEnabled, getBooleanName(hourGlassIconEnabled));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "watchIconEnabled             = %6u = %s",   watchIconEnabled,     getBooleanName(watchIconEnabled));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "userModeEnabled              = %6u = %s",   userModeEnabled,      getBooleanName(userModeEnabled));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "serialIOIconEnabled          = %6u = %s",   serialIOIconEnabled,  getBooleanName(serialIOIconEnabled));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "printerIconEnabled           = %6u = %s",   printerIconEnabled,   getBooleanName(printerIconEnabled));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "batteryIconEnabled           = %6u = %s",   batteryIconEnabled,   getBooleanName(batteryIconEnabled));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "stackLiftEnabled             = %6u = %s",   stackLiftEnabled,     getBooleanName(stackLiftEnabled));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "curveFitting                 = %6u = %s\n", curveFitting,         getCurveFittingName(curveFitting));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "calcMode                     = %6u = %s",   calcMode,             getCalcModeName(calcMode));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "nextChar                     = %6u = %s",   nextChar,             getNextCharName(nextChar));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "TAM mode                     = %6u = %s",   tamMode,              getTamModeName(tamMode));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "transSystStatus              = %6u",        transitionSystemStatus);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "shiftF                       = %6u = %s",   shiftF,               getBooleanName(shiftF));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "shiftG                       = %6u = %s\n", shiftG,               getBooleanName(shiftG));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "xCursor                      = %6u",        xCursor);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "yCursor                      = %6u",        yCursor);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "cursorEnabled                = %6u = %s",   cursorEnabled,        getBooleanName(cursorEnabled));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "cursorFont                   = %6u = %s",   cursorFont,           getCursorFontName(cursorFont));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "currentFntScr                = %6u",        currentFntScr);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "cursorBlinkCounter           = %6u\n",      cursorBlinkCounter);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "currentRegisterBrowserScreen = %6u",        currentRegisterBrowserScreen);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "currentFlgScr                = %6u",        currentFlgScr);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "rbrMode                      = %6u = %s",   rbrMode,              getRbrModeName(rbrMode));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "showContent                  = %6u = %s",   showContent,          getBooleanName(showContent));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }
    }

    else if(debugWindow == DBG_FLAGS) {
      for(int i=0; i<DEBUG_LINES; i++) {
        gtk_widget_hide(lbl1[i]);
        gtk_widget_hide(lbl2[i]);
      }

      row = 0;

      gtk_label_set_label(GTK_LABEL(lbl1[row]), "Flags:");
      gtk_widget_show(lbl1[row++]);

      gtk_label_set_label(GTK_LABEL(lbl1[row]), "      0 1 2 3 4 5 6 7 8 9");
      gtk_widget_show(lbl1[row++]);

      for(int i=0; i<=10; i++) {
        if(i == 10) {
          row++;
          gtk_label_set_label(GTK_LABEL(lbl1[row]), "      X Y Z T A B C D L I");
          gtk_widget_show(lbl1[row++]);
        }

        sprintf(string, "%4d ", 10*i);

        for(int col=0; col<10; col++) {
          strcat(string, getFlag(10*i+col) ? " 1" : " .");
        }

        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      gtk_label_set_label(GTK_LABEL(lbl1[row]), "      J K");
      gtk_widget_show(lbl1[row++]);

      sprintf(string, " 110 ");
      strcat(string, getFlag(110) ? " 1" : " .");
      strcat(string, getFlag(111) ? " 1" : " .");
      gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
      gtk_widget_show(lbl1[row++]);

      row++;
      if(numberOfLocalRegisters > 0) {
        gtk_label_set_label(GTK_LABEL(lbl1[row]), "Local flags:");
        gtk_widget_show(lbl1[row++]);

        gtk_label_set_label(GTK_LABEL(lbl1[row]), "      0 1 2 3 4 5 6 7 8 9");
        gtk_widget_show(lbl1[row++]);

        sprintf(string, "   0 ");
        for(int col=0; col<10; col++) {
          strcat(string, getFlag(NUMBER_OF_LOCAL_FLAGS + col) ? " 1" : " .");
        }
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);

        sprintf(string, "  10 ");
        for(int col=10; col<16; col++) {
         strcat(string, getFlag(NUMBER_OF_LOCAL_FLAGS + col) ? " 1" : " .");
        }
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }
    }

    else if(debugWindow == DBG_REGISTERS) {
      for(int i=0; i<DEBUG_LINES; i++) {
        gtk_widget_hide(lbl1[i]);
        gtk_widget_hide(lbl2[i]);
      }

      row = 0;
      gtk_label_set_label(GTK_LABEL(lbl1[row]), "Regis Type                  Address    Size");
      gtk_label_set_label(GTK_LABEL(lbl2[row]), "Content");
      gtk_widget_show(lbl1[row]);
      gtk_widget_show(lbl2[row++]);

      for(int i=REGISTER_K; i>=REGISTER_I; i--) {
        sprintf(string, "%3d %c %s %7d %7d", i, i-REGISTER_I+'I', getRegisterDataTypeName(i, false, true), getRegisterDataPointer(i), getRegisterDataSize(i));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row]);
        debugRegisterValue(i, row++);
      }

      for(int i=REGISTER_D; i>=REGISTER_A; i--) {
        sprintf(string, "%3d %c %s %7d %7d", i, i-REGISTER_A+'A', getRegisterDataTypeName(i, false, true), getRegisterDataPointer(i), getRegisterDataSize(i));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row]);
        debugRegisterValue(i, row++);
      }

      sprintf(string, "103 T %s %7d %7d", getRegisterDataTypeName(REGISTER_T, false, true), getRegisterDataPointer(REGISTER_T), getRegisterDataSize(REGISTER_T));
      gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
      gtk_widget_show(lbl1[row]);
      debugRegisterValue(REGISTER_T, row++);

      for(int i=REGISTER_Z; i>=REGISTER_X; i--) {
        sprintf(string, "%3d %c %s %7d %7d", i, i-REGISTER_X+'X', getRegisterDataTypeName(i, false, true), getRegisterDataPointer(i), getRegisterDataSize(i));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row]);
        debugRegisterValue(i, row++);
      }

      sprintf(string, "108 L %s %7d %7d", getRegisterDataTypeName(REGISTER_L, false, true), getRegisterDataPointer(REGISTER_L), getRegisterDataSize(REGISTER_L));
      gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
      gtk_widget_show(lbl1[row]);
      debugRegisterValue(REGISTER_L, row++);

      row++;
      for(int i=0; i<100; i++) {
        if(row < DEBUG_LINES) {
          sprintf(string, "  %02d  %s %7d %7d", i, getRegisterDataTypeName(i, false, true), getRegisterDataPointer(i), getRegisterDataSize(i));
          gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
          gtk_widget_show(lbl1[row]);
          debugRegisterValue(i, row);
          row++;
        }
      }
    }

    else if(debugWindow == DBG_LOCAL_REGISTERS) {
      for(int i=0; i<DEBUG_LINES; i++) {
        gtk_widget_hide(lbl1[i]);
        gtk_widget_hide(lbl2[i]);
      }

  //showInfoDialog(tmpStr3000, NULL, NULL, NULL);
      row = 0;
      gtk_label_set_label(GTK_LABEL(lbl1[row]), "Regis Type                  Address    Size");
      sprintf(string, "Content of the %" FMT16U " local registers", numberOfLocalRegisters);
      gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
      gtk_widget_show(lbl1[row]);
      gtk_widget_show(lbl2[row++]);

      for(uint16_t i=FIRST_LOCAL_REGISTER; i<FIRST_LOCAL_REGISTER+numberOfLocalRegisters; i++) {
        if(row < DEBUG_LINES) {
          sprintf(string, ".%02d   %s %7d %7d", i-FIRST_LOCAL_REGISTER, getRegisterDataTypeName(i, false, true), getRegisterDataPointer(i), getRegisterDataSize(i));
          gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
          gtk_widget_show(lbl1[row]);
          debugRegisterValue(i, row++);
        }
      }
    }

    else if(debugWindow == DBG_STATISTICAL_SUMS) {
      for(int i=0; i<DEBUG_LINES; i++) {
        gtk_widget_hide(lbl1[i]);
        gtk_widget_hide(lbl2[i]);
      }

      row = 0;
      gtk_label_set_label(GTK_LABEL(lbl1[row]), "Sum");
      sprintf(string, "Content of the statistical sums (%d)", statisticalSumsPointer);
      gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
      gtk_widget_show(lbl1[row]);
      gtk_widget_show(lbl2[row++]);

      if(statisticalSumsPointer != 0) {
        gtk_label_set_label(GTK_LABEL(lbl1[row]), "n");
        gtk_widget_show(lbl1[row]);
        formatReal16Debug(string, statisticalSumsPointer + REAL16_SIZE*0);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(x)");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatReal16Debug(string, statisticalSumsPointer + REAL16_SIZE*1);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(y)");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatReal16Debug(string, statisticalSumsPointer + REAL16_SIZE*2);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(x" STD_SUP_2 ")");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatReal16Debug(string, statisticalSumsPointer + REAL16_SIZE*3);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(y" STD_SUP_2 ")");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatReal16Debug(string, statisticalSumsPointer + REAL16_SIZE*5);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(xy)");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatReal16Debug(string, statisticalSumsPointer + REAL16_SIZE*6);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(ln(x))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatReal16Debug(string, statisticalSumsPointer + REAL16_SIZE*8);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(ln(y))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatReal16Debug(string, statisticalSumsPointer + REAL16_SIZE*11);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(ln(x) ln(y))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatReal16Debug(string, statisticalSumsPointer + REAL16_SIZE*7);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(ln" STD_SUP_2 "(x))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatReal16Debug(string, statisticalSumsPointer + REAL16_SIZE*9);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(ln" STD_SUP_2 "(y))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatReal16Debug(string, statisticalSumsPointer + REAL16_SIZE*12);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(x ln(y))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatReal16Debug(string, statisticalSumsPointer + REAL16_SIZE*13);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(y ln(x))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatReal16Debug(string, statisticalSumsPointer + REAL16_SIZE*10);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(x" STD_SUP_2 "y)");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatReal16Debug(string, statisticalSumsPointer + REAL16_SIZE*4);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);
      }
    }

    else if(debugWindow == DBG_NAMED_REGISTERS) {
      for(int i=0; i<DEBUG_LINES; i++) {
        gtk_widget_hide(lbl1[i]);
        gtk_widget_hide(lbl2[i]);
      }

      row = 0;
      gtk_label_set_label(GTK_LABEL(lbl1[row]), "Regis Type                  Address    Size");
      sprintf(string, "Content of the %" FMT16U " named registers", numberOfNamedRegisters);
      gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
      gtk_widget_show(lbl1[row]);
      gtk_widget_show(lbl2[row++]);

      for(uint16_t i=1000; i<1000+numberOfNamedRegisters; i++) {
        if(row < DEBUG_LINES) {
          sprintf(string, "%03d   %s %7d %7d", i-1000, getRegisterDataTypeName(i, false, true), getRegisterDataPointer(i), getRegisterDataSize(i));
          gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
          gtk_widget_show(lbl1[row]);
          debugRegisterValue(i, row++);
        }
      }
    }

    else if(debugWindow == DBG_TMP_SAVED_STACK_REGISTERS) {
      for(int i=0; i<DEBUG_LINES; i++) {
        gtk_widget_hide(lbl1[i]);
        gtk_widget_hide(lbl2[i]);
      }

      row = 0;
      gtk_label_set_label(GTK_LABEL(lbl1[row]), "Regis Type                  Address    Size");
      sprintf(string, "Content of the %d temporary and 8 saved stack registers", NUMBER_OF_TEMPORARY_REGISTERS);
      gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
      gtk_widget_show(lbl1[row]);
      gtk_widget_show(lbl2[row++]);

      for(uint16_t i=FIRST_TEMPORARY_REGISTER; i<FIRST_TEMPORARY_REGISTER+NUMBER_OF_TEMPORARY_REGISTERS; i++) {
        sprintf(string, "%3d   %s %7d %7d", i-FIRST_TEMPORARY_REGISTER, getRegisterDataTypeName(i, false, true), getRegisterDataPointer(i), getRegisterDataSize(i));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row]);
        debugRegisterValue(i, row++);
      }

      row ++;

      for(uint16_t i=SAVED_REGISTER_X; i<=SAVED_REGISTER_L; i++) {
        sprintf(string, "%3d   %s %7d %7d", i-SAVED_REGISTER_X, getRegisterDataTypeName(i, false, true), getRegisterDataPointer(i), getRegisterDataSize(i));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row]);
        debugRegisterValue(i, row++);
      }
    }

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("refreshDebugPanel");
    #endif
  }

  void btnBitFieldsClicked(GtkWidget* w, gpointer data) {
    #if (LOG_FUNCTIONS == 1)
      enteringFunction("btnBitFieldsClicked");
    #endif

    allowScreenUpdate = true;
    debugWindow = DBG_BIT_FIELDS;
    refreshDebugPanel();

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("btnBitFieldsClicked");
    #endif
  }

  void btnFlagsClicked(GtkWidget* w, gpointer data) {
    #if (LOG_FUNCTIONS == 1)
      enteringFunction("btnFlagsClicked");
    #endif

    allowScreenUpdate = true;
    debugWindow = DBG_FLAGS;
    //gtk_label_set_label(GTK_LABEL(lbl1[0]), "Flags:");
    //gtk_widget_show(frmCalc);
    refreshDebugPanel();

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("btnFlagsClicked");
    #endif
  }

  void btnRegistersClicked(GtkWidget* w, gpointer data) {
    #if (LOG_FUNCTIONS == 1)
      enteringFunction("btnRegistersClicked");
    #endif

    allowScreenUpdate = true;
    debugWindow = DBG_REGISTERS;
    //gtk_label_set_label(GTK_LABEL(lbl1[0]), "Regis Addres   Type  Size Content");
    refreshDebugPanel();

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("btnRegistersClicked");
    #endif
  }

  void btnLocalRegistersClicked(GtkWidget* w, gpointer data) {
    #if (LOG_FUNCTIONS == 1)
      enteringFunction("btnLocalRegistersClicked");
    #endif

    allowScreenUpdate = true;
    debugWindow = DBG_LOCAL_REGISTERS;
    //gtk_label_set_label(GTK_LABEL(lbl1[0]), "Regis Addres   Type  Size Content");
    refreshDebugPanel();

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("btnLocalRegistersClicked");
    #endif
  }

  void btnStatisticalSumsClicked(GtkWidget* w, gpointer data) {
    #if (LOG_FUNCTIONS == 1)
      enteringFunction("btnStatisticalSumsClicked");
    #endif

    allowScreenUpdate = true;
    debugWindow = DBG_STATISTICAL_SUMS;
    //gtk_label_set_label(GTK_LABEL(lbl1[0]), "Regis Addres   Type  Size Content");
    refreshDebugPanel();

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("btnStatisticalSumsClicked");
    #endif
  }

  void btnNamedRegistersClicked(GtkWidget* w, gpointer data) {
    #if (LOG_FUNCTIONS == 1)
      enteringFunction("btnNamedRegistersClicked");
    #endif

    allowScreenUpdate = true;
    debugWindow = DBG_NAMED_REGISTERS;
    //gtk_label_set_label(GTK_LABEL(lbl1[0]), "Regis Addres   Type  Size Content");
    refreshDebugPanel();

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("btnNamedRegistersClicked");
    #endif
  }

  void btnTmpAndSavedStackRegistersClicked(GtkWidget* w, gpointer data) {
    #if (LOG_FUNCTIONS == 1)
      enteringFunction("btnTmpAndSavedStackRegistersClicked");
    #endif

    allowScreenUpdate = true;
    debugWindow = DBG_TMP_SAVED_STACK_REGISTERS;
    //gtk_label_set_label(GTK_LABEL(lbl1[0]), "Regis Addres   Type  Size Content");
    refreshDebugPanel();

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("btnTmpAndSavedStackRegistersClicked");
    #endif
  }

  void chkHexaStringClicked(GtkWidget* w, gpointer data) {
    #if (LOG_FUNCTIONS == 1)
      enteringFunction("chkHexaStringClicked");
    #endif

    allowScreenUpdate = true;
    refreshDebugPanel();

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("chkHexaStringClicked");
    #endif
  }
#endif



#if (DEBUG_PANEL == 1) || (DEBUG_REGISTER_L == 1)
  /********************************************//**
   * \brief Formats a real16 for the debug window
   *
   * \param[in] str char*    String receiving the value
   * \param[in] x real16_t* Value
   * \return void
   ***********************************************/
  void formatReal16Debug(char *str, uint32_t addr) {
    #if (LOG_FUNCTIONS == 1)
      enteringFunction("formatReal16Debug");
    #endif

    uint8_t ch, coef, digit;
    uint8_t bcd[DECDOUBLE_Pmax];
    int32_t sign, exponent;

    if(real16IsInfinite(RAM(addr)) || real16IsNaN(RAM(addr))) {
      real16ToString(RAM(addr), str);

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("formatReal16Debug");
      #endif

      return;
    }

    if(real16IsZero(RAM(addr))) {
      strcpy(str, "+0.000000000000000e+0");

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("formatReal16Debug");
      #endif

      return;
    }

    sign = real16GetCoefficient(RAM(addr), bcd);
    exponent = real16GetExponent(RAM(addr));
    if(sign) {
      str[0] = '-';
    }
    else {
      str[0] = '+';
    }

    coef = 0;
    while(coef<16 && bcd[coef] == 0) coef++;

    str[1] = '0' + bcd[coef++];
    digit = 1;
    str[2] = '.';

    ch = 3;
    while(coef<16) {
      str[ch++] = '0' + bcd[coef++];
      digit++;
      exponent++;
    }

    while(digit<16) {
      str[ch++] = '0';
      digit++;
    }

    sprintf(str+ch, "e%+d", exponent);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("formatReal16Debug");
    #endif
  }


  /********************************************//**
   * \brief Formats a complex16 for the debug window
   *
   * \param[in] str char*    String receiving the value
   * \param[in] x real16_t* Value
   * \return void
   ***********************************************/
  void formatComplex16Debug(char *str, uint32_t addr) {
    #if (LOG_FUNCTIONS == 1)
      enteringFunction("formatComplex16Debug");
    #endif

    formatReal16Debug(str     , addr               );
    formatReal16Debug(str + 64, addr + REAL16_SIZE);

    strcat(str, " ");
    strcat(str, str + 64);
    strcat(str, "i");

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("formatComplex16Debug");
    #endif
  }


  /********************************************//**
   * \brief Formats a real34 for the debug window
   *
   * \param[in] str char*  String receiving the value
   * \param[in] x real34_t* Value
   * \return void
   ***********************************************/
  void formatReal34Debug(char *str, uint32_t addr) {
    #if (LOG_FUNCTIONS == 1)
      enteringFunction("formatReal34Debug");
    #endif

    uint8_t ch, coef, digit;
    uint8_t bcd[DECQUAD_Pmax];
    int32_t sign, exponent;

    if(real34IsInfinite(RAM(addr)) || real34IsNaN(RAM(addr))) {
      real34ToString(RAM(addr), str);

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("formatReal34Debug");
      #endif

      return;
    }

    if(real34IsZero(RAM(addr))) {
      strcpy(str, "+0.000000000000000000000000000000000e+0");

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("formatReal34Debug");
      #endif

      return;
    }

    sign = real34GetCoefficient(RAM(addr), bcd);
    exponent = real34GetExponent(RAM(addr));
    if(sign) {
      str[0] = '-';
    }
    else {
      str[0] = '+';
    }

    coef = 0;
    while(coef<34 && bcd[coef] == 0) coef++;

    str[1] = '0' + bcd[coef++];
    digit = 1;
    str[2] = '.';

    ch = 3;
    while(coef<34) {
      str[ch++] = '0' + bcd[coef++];
      digit++;
      exponent++;
    }

    while(digit<34) {
      str[ch++] = '0';
      digit++;
    }

    sprintf(str+ch, "e%+d", exponent);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("formatReal34Debug");
    #endif
  }



  /********************************************//**
   * \brief Formats a complex34 for the debug window
   *
   * \param[in] str char*  String receiving the value
   * \param[in] x real34_t* Value
   * \return void
   ***********************************************/
  void formatComplex34Debug(char *str, uint32_t addr) {
    #if (LOG_FUNCTIONS == 1)
      enteringFunction("formatComplex34Debug");
    #endif

    formatReal34Debug(str     , addr             );
    formatReal34Debug(str + 64, addr + REAL34_SIZE);

    strcat(str, " ");
    strcat(str, str + 64);
    strcat(str, "i");

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("formatComplex34Debug");
    #endif
  }
#endif
