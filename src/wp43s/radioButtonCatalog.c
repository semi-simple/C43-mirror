/* This file is part of WP 43S.
 *
 * WP 43S is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WP 43S is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WP 43S.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ADDITIONAL C43 functions and routines */


/********************************************//**
 * \file radioButtonCatalog.c
 ***********************************************/

#include "wp43s.h"


const radiocb_eeprom_t indexOfRadioCbEepromItems[] = {
//  itemNr    item                  parameter                         function
/*  117 */  { ITM_DEG,              AM_DEGREE,              RB_AM },  //fnAngularMode
/*  134 */  { ITM_DMS,              AM_DMS,                 RB_AM },  //fnAngularMode
/*  229 */  { ITM_GRAD,             AM_GRAD,                RB_AM },  //fnAngularMode
/*  375 */  { ITM_MULPI,            AM_MULTPI,              RB_AM },  //fnAngularMode
/*  483 */  { ITM_RAD,              AM_RADIAN,              RB_AM },  //fnAngularMode
/* 1965 */  { ITM_POLAR,            CM_POLAR,               RB_CM },  //fnSetSetJM          /*  464 */ //fnComplexMode
/* 1969 */  { ITM_RECT,             CM_RECTANGULAR,         RB_CM },  //fnSetSetJM          /*  507 */ //fnComplexMode
/* 1960 */  { ITM_CPXI,             CU_I,                   RB_CU },  //fnSetSetJM          /*   96 */ //fnComplexUnit
/* 1961 */  { ITM_CPXJ,             CU_J,                   RB_CU },  //fnSetSetJM          /*   97 */ //fnComplexUnit

#ifdef XXXXX
/*  308 */  { ITM_LINF,             CF_LINEAR_FITTING       RB_CF },  //fnCurveFitting
/*  161 */  { ITM_EXPF,             CF_EXPONENTIAL_FITTING  RB_CF },  //fnCurveFitting
/*  325 */  { ITM_LOGF,             CF_LOGARITHMIC_FITTING  RB_CF },  //fnCurveFitting
/*  466 */  { ITM_POWERF,           CF_POWER_FITTING        RB_CF },  //fnCurveFitting
/* 1566 */  { ITM_ROOTF,            CF_ROOT_FITTING         RB_CF },  //fnCurveFitting
/* 1565 */  { ITM_HYPF,             CF_HYPERBOLIC_FITTING   RB_CF },  //fnCurveFitting
/* 1564 */  { ITM_PARABF,           CF_PARABOLIC_FITTING    RB_CF },  //fnCurveFitting
/* 1563 */  { ITM_CAUCHF,           CF_CAUCHY_FITTING       RB_CF },  //fnCurveFitting
/* 1562 */  { ITM_GAUSSF,           CF_GAUSS_FITTING        RB_CF },  //fnCurveFitting
/*  438 */  { ITM_ORTHOF,           CF_ORTHOGONAL_FITTING   RB_CF },  //fnCurveFitting
#endif


/*  136 */  { ITM_DMY,              DF_DMY,                 RB_DF },  //fnSetDateFormat
/*  383 */  { ITM_MDY,              DF_MDY,                 RB_DF },  //fnSetDateFormat
/*  700 */  { ITM_YMD,              DF_YMD,                 RB_DF },  //fnSetDateFormat
/*  119 */ // { ITM_DENANY,           DM_ANY,                 RB_DM },  //  --fnDenMode
/*  120 */ // { ITM_DENFAC,           DM_FAC,                 RB_DM },  //  --fnDenMode
/*  121 */ // { ITM_DENFIX,           DM_FIX,                 RB_DM },  //  --fnDenMode
/*   20 */  { ITM_ALL,              DF_ALL,                 RB_DI },  //fnDisplayFormatAll
/*  145 */  { ITM_ENG,              DF_ENG,                 RB_DI },  //fnDisplayFormatEng
/*  185 */  { ITM_FIX,              DF_FIX,                 RB_DI },  //fnDisplayFormatFix
/*  545 */  { ITM_SCI,              DF_SCI,                 RB_DI },  //fnDisplayFormatSci
/* 1682 */  { ITM_SIGFIG,           DF_SF,                  RB_DI },  //fnDisplayFormatSigFig
/* 1693 */  { ITM_UNIT,             DF_UN,                  RB_DI },  //fnDisplayFormatUnit
/* 1972 */  { ITM_ENGOVR,           DO_ENG,                 RB_DO },  //fnSetSetJM          /*  146 */ //fnDisplayOvr
/* 1971 */  { ITM_SCIOVR,           DO_SCI,                 RB_DO },  //fnSetSetJM          /*  547 */ //fnDisplayOvr
/*  251 */ // { ITM_IMPFRC,           JC_IMPROPER,            CB_JC },  //  --fnToggleFractionType
/*  471 */ // { ITM_PROFRC,           JC_PROPER,              CB_JC },  //  --fnToggleFractionType
/* 1905 */  { ITM_INP_DEF_43S,      ID_43S,                 RB_ID },  //fnInDefault
/* 1910 */  { ITM_INP_DEF_CPXDP,    ID_CPXDP,               RB_ID },  //fnInDefault
/* 1907 */  { ITM_INP_DEF_DP,       ID_DP,                  RB_ID },  //fnInDefault
/* 1913 */  { ITM_INP_DEF_LI,       ID_LI,                  RB_ID },  //fnInDefault
/* 1912 */  { ITM_INP_DEF_SI,       ID_SI,                  RB_ID },  //fnInDefault
/*    4 */  { ITM_1COMPL,           SIM_1COMPL,             RB_IM },  //fnIntegerMode
/*    7 */  { ITM_2COMPL,           SIM_2COMPL,             RB_IM },  //fnIntegerMode
/*  567 */  { ITM_SIGNMT,           SIM_SIGNMT,             RB_IM },  //fnIntegerMode
/*  645 */  { ITM_UNSIGN,           SIM_UNSIGN,             RB_IM },  //fnIntegerMode
/* 1962 */  { ITM_MULTCR,           PS_CROSS,               RB_PS },  //fnSetSetJM          /*  373 */ //fnProductSign
/* 1963 */  { ITM_MULTDOT,          PS_DOT,                 RB_PS },  //fnSetSetJM          /*  374 */ //fnProductSign
/* 1966 */  { ITM_RDXCOM,           RX_COMMA,               RB_RX },  //fnSetSetJM          /*  500 */ //fnRadixMark
/* 1967 */  { ITM_RDXPER,           RX_PERIOD,              RB_RX },  //fnSetSetJM          /*  501 */ //fnRadixMark
/* 1689 */  { ITM_WS8,              8,                      RB_WS },  //fnSetWordSize
/* 1690 */  { ITM_WS16,             16,                     RB_WS },  //fnSetWordSize
/* 1691 */  { ITM_WS32,             32,                     RB_WS },  //fnSetWordSize
/* 1692 */  { ITM_WS64,             64,                     RB_WS },  //fnSetWordSize
/* 1964 */  { ITM_SSIZE4,           SS_4,                   RB_SS },  //fnSetSetJM          /*  583 */ //fnStackSize
/* 1968 */  { ITM_SSIZE8,           SS_8,                   RB_SS },  //fnSetSetJM          /*  584 */ //fnStackSize
/* 1958 */  { ITM_CLK12,            TF_H12,                 RB_TF },  //fnSetSetJM          /*   75 */ //fnTimeFormat
/* 1959 */  { ITM_CLK24,            TF_H24,                 RB_TF },  //fnSetSetJM          /*   76 */ //fnTimeFormat
/* 1899 */  { ITM_U_KEY_ALPHA,      ITM_AIM,                RB_SA },  //fnSigmaAssign
/* 1897 */  { ITM_U_KEY_PRGM,       ITM_PR,                 RB_SA },  //fnSigmaAssign
/* 1895 */  { ITM_U_KEY_SIGMA,      ITM_SIGMAPLUS,          RB_SA },  //fnSigmaAssign
/* 1891 */  { ITM_U_KEY_CC,         ITM_CC,                 RB_SA },  //fnSigmaAssign
/* 1889 */  { ITM_U_KEY_USER,       ITM_USERMODE,           RB_SA },  //fnSigmaAssign
/* 1893 */  { ITM_U_KEY_MM,         -MNU_MyMenu,            RB_SA },  //fnSigmaAssign
/* 1744 */  { ITM_BASE_AHOME,       JC_BASE_AHOME,          CB_JC },  //fnSetSetJM
/* 1742 */  { ITM_BASE_HOME,        JC_BASE_HOME,           CB_JC },  //fnSetSetJM
/* 1695 */  { ITM_CB_CPXRES,        JC_BCR,                 CB_JC },  //fnSetSetJM
/* 1748 */  { ITM_CB_SPCRES,        JC_BSR,                 CB_JC },  //fnSetSetJM
/* 1696 */  { ITM_CB_LEADING_ZERO,  JC_BLZ,                 CB_JC },  //fnSetSetJM
/* 1678 */  { ITM_ERPN,             JC_ERPN,                CB_JC },  //fnSetSetJM
/* 1909 */  { ITM_NO_BASE_SCREEN,          JC_NO_BASE_SCREEN,             CB_JC },  //fnSetSetJM
/* 1906 */  { ITM_FG_LINE,          JC_FG_LINE,             CB_JC },  //fnSetSetJM
/* 1911 */  { ITM_G_DOUBLETAP,      JC_G_DOUBLETAP,         CB_JC },  //fnSetSetJM
/* 1679 */  { ITM_HOMEx3,           JC_HOME_TRIPLE,         CB_JC },  //fnSetSetJM
/* 1770 */  { ITM_HOMEx3T,          JC_SH_3T,               CB_JC },  //fnSetSetJM
/* 1680 */  { ITM_SHTIM,            JC_SHFT_4s,             CB_JC },  //fnSetSetJM
/* 1943 */  { ITM_VECT,             JC_VECT,                CB_JC },  //fnSetSetJM
/* 1943 */  { ITM_NVECT,            JC_NVECT,               CB_JC },  //fnSetSetJM
/*      */  { ITM_SCALE,            JC_SCALE,               CB_JC },  //fnSetSetJM
/* 1744 */  { ITM_H_SUMRY,          JC_H_SUM,               CB_JC },  //fnSetSetJM
/* 1745 */  { ITM_H_REPLCA,         JC_H_MIR,               CB_JC },  //fnSetSetJM
/* 1746 */  { ITM_H_FIXED,          JC_H_FIX,               CB_JC },  //fnSetSetJM
/*      */  { ITM_LARGELI,          JC_LARGELI,             CB_JC },  //fnSetSetJM
/*      */  { ITM_EXTX,             JC_EXTENTX,             CB_JC },  //fnSetSetJM
/*      */  { ITM_EXTY,             JC_EXTENTY,             CB_JC },  //fnSetSetJM
/* 1892 */  { ITM_TEST,             JC_ITM_TST,             CB_JC },  //fnSetInlineTest
/*      */  { ITM_PLINE,            JC_PLINE,               CB_JC },  //
/*      */  { ITM_PCROS,            JC_PCROS,               CB_JC },  //
/*      */  { ITM_PBOX ,            JC_PBOX,                CB_JC },  //
/*      */  { ITM_INTG,             JC_INTG,                CB_JC },  //
/*      */  { ITM_DIFF,             JC_DIFF,                CB_JC },  //
/*      */  { ITM_RMS ,             JC_RMS ,                CB_JC },  //
/*      */  { ITM_SHADE ,           JC_SHADE ,              CB_JC },  //

/*        { ITM_DEG2,             AM_DEGREE,              RB_AM2 },  //fnAngularModeJM  */
/*        { ITM_DMS2,             AM_DMS,                 RB_AM2 },  //fnAngularModeJM  */
/*        { ITM_GRAD2,            AM_GRAD,                RB_AM2 },  //fnAngularModeJM  */
/*        { ITM_MULPI2,           AM_MULTPI,              RB_AM2 },  //fnAngularModeJM  */
/*        { ITM_RAD2,             AM_RADIAN,              RB_AM2 },  //fnAngularModeJM  */
/*        { ITM_HMS2,             AM_HMS,                 RB_AM2 },  //fnAngularModeJM  */

/* 1685 */  { ITM_2BIN,               2,                     RB_HX  },  //fnChangeBaseJM
/* 1686 */  { ITM_2OCT,               8,                     RB_HX  },  //fnChangeBaseJM
/* 1687 */  { ITM_2DEC,              10,                     RB_HX  },  //fnChangeBaseJM
/* 1688 */  { ITM_2HEX,              16,                     RB_HX  }   //fnChangeBaseJM


};



int8_t fnCbIsSet(int16_t item) {
  int8_t result = -1;
  uint16_t itemNr = max(item, -item);
  size_t n = nbrOfElements(indexOfRadioCbEepromItems);
  for(uint8_t i=0; i < n; i++) {
    if(indexOfRadioCbEepromItems[i].itemNr == itemNr) {
      //printf("^^^^**** item found %d\n",itemNr);
      bool_t is_cb = false;
      uint16_t rb_param = 0;
      bool_t cb_param =  false;

      switch (indexOfRadioCbEepromItems[i].radioButton)
      {
      case RB_AM:
        {
          rb_param = currentAngularMode;
        }
        break;

      case RB_CM:
        {
          if(getSystemFlag(FLAG_POLAR)) { rb_param = CM_POLAR; }
          else { rb_param = CM_RECTANGULAR; }
        }
        break;

      case RB_CU:
        {
          if(getSystemFlag(FLAG_CPXj)) { rb_param = CU_J; }
          else { rb_param = CU_I; }
        }
        break;
#ifdef XXXXX
      case RB_CF:
        {
          rb_param = curveFitting;
        }
        break;
#endif

      case RB_DF:
        {
          if     (getSystemFlag(FLAG_YMD)) { rb_param = DF_YMD; }
          else if(getSystemFlag(FLAG_DMY)) { rb_param = DF_DMY; }
          else                             { rb_param = DF_MDY; }
        }
        break;

      case RB_DI:
        {
          rb_param = displayFormat;
          if(rb_param == DF_FIX && SigFigMode != 0) { rb_param = DF_SF; }
          if(rb_param == DF_ENG && UNITDisplay)     { rb_param = DF_UN; }
        }
        break;

      case RB_DO:
        {
          if(getSystemFlag(FLAG_ALLENG)) { rb_param = DO_ENG; }
          else { rb_param = DO_SCI; }
        }
        break;

      case RB_ID:
        {
          rb_param = Input_Default;
        }
        break;

      case RB_IM:
        {
          rb_param = shortIntegerMode;
        }
        break;

      case RB_PS:
        {
          if(getSystemFlag(FLAG_MULTx)) { rb_param = PS_CROSS; }
          else { rb_param = PS_DOT; }
        }
        break;

      case RB_RX:
        {
          if(getSystemFlag(FLAG_DECIMP)) { rb_param = RX_PERIOD; }
          else { rb_param = RX_COMMA; }
        }
        break;

      case RB_WS:
        {
          rb_param = shortIntegerWordSize;
        }
        break;

      case RB_SS:
        {
          if(getSystemFlag(FLAG_SSIZE8)) { rb_param = SS_8; }
          else { rb_param = SS_4; }
        }
        break;

      case RB_TF:
        {
          if(getSystemFlag(FLAG_TDM24)) { rb_param = TF_H24; }
          else { rb_param = TF_H12; }
        }
        break;

      case RB_SA:
        {
          rb_param = Norm_Key_00_VAR;
          //printf("^^^^*** activated %d\n",rb_param);
        }
        break;

      case RB_HX:
        {
          if(lastIntegerBase !=0) {rb_param = lastIntegerBase;} else return result;
        }
        break;

      case CB_JC:
        {
          is_cb = true;

          switch (indexOfRadioCbEepromItems[i].param)
          {
          case JC_BASE_AHOME:
            {
              cb_param = SH_BASE_AHOME;
            }
            break;

          case JC_BASE_HOME:
            {
              cb_param = SH_BASE_HOME;
            }
            break;

          case JC_BCR:
            {
              cb_param = getSystemFlag(FLAG_CPXRES);
            }
            break;

          case JC_BSR:                                     //JMvv
            {
              cb_param = getSystemFlag(FLAG_SPCRES);
            }
            break;                                         //JM^^

          case JC_BLZ:
            {
              cb_param = getSystemFlag(FLAG_LEAD0);
            }
            break;

          case JC_ERPN:
            {
              cb_param = eRPN;
            }
            break;

          case JC_NO_BASE_SCREEN:
            {
              cb_param = jm_NO_BASE_SCREEN;
            }
            break;

          case JC_FG_LINE:
            {
              cb_param = jm_FG_LINE;
            }
            break;

          case JC_G_DOUBLETAP:
            {
              cb_param = jm_G_DOUBLETAP;
            }
            break;

          case JC_HOME_TRIPLE:
            {
              cb_param = HOME3;
            }
            break;

          case JC_SH_3T:
            {
              cb_param = Home3TimerMode;
            }
            break;

          case JC_SHFT_4s:
            {
              cb_param = ShiftTimoutMode;
            }
            break;

          case JC_VECT:
            {
              cb_param = jm_VECT;
            }
            break;

          case JC_NVECT:
            {
              cb_param = jm_NVECT;
            }
            break;

          case JC_SCALE:
            {
              cb_param = jm_SCALE;
            }
            break;

          case JC_H_SUM:
            {
              cb_param = jm_HOME_SUM;
            }
            break;

          case JC_H_MIR:
            {
              cb_param = jm_HOME_MIR;
            }
            break;

          case JC_H_FIX:
            {
              cb_param = jm_HOME_FIX;
            }
            break;

          case JC_LARGELI:
            {
              cb_param = jm_LARGELI;
            }
            break;

          case JC_EXTENTX:
            {
              cb_param = extentx;
            }
            break;

          case JC_EXTENTY:
            {
              cb_param = extenty;
            }
            break;

          case JC_PLINE:
            {
              cb_param = PLOT_LINE;
            }
            break;

          case JC_PCROS:
            {
              cb_param = PLOT_CROSS;
            }
            break;

          case JC_PBOX:
            {
              cb_param = PLOT_BOX;
            }
            break;

          case JC_DIFF:
            {
              cb_param = PLOT_DIFF;
            }
            break;

          case JC_INTG:
            {
              cb_param = PLOT_INTG;
            }
            break;

          case JC_RMS:
            {
              cb_param = PLOT_RMS;
            }
            break;

          case JC_SHADE:
            {
              cb_param = PLOT_SHADE;
            }
            break;

#ifdef INLINE_TEST
          case JC_ITM_TST:
            {
              cb_param = testEnabled;
            }
            break;
#endif

          default:
            break;
          }
        }
        break;

      default:
        break;
      }

      if(is_cb) {
        result = cb_param ? CB_TRUE : CB_FALSE;
      }
      else {
        //printf("^^^^*** %d %d\n",indexOfRadioCbEepromItems[i].param, rb_param);
        result = (indexOfRadioCbEepromItems[i].param == rb_param) ? RB_TRUE : RB_FALSE; 
      }
    }
  }

  return result;
}



void fnRefreshRadioState(char rb, uint16_t mode) {
#ifndef TESTSUITE_BUILD
  doRefreshSoftMenu = true;
#endif
}



void fnRefreshComboxState(char rb, uint16_t param, bool_t state) {
#ifndef TESTSUITE_BUILD
  doRefreshSoftMenu = true;
#endif
}



int16_t fnItemShowValue(int16_t item) {
  int16_t result = -1;
  uint16_t itemNr = max(item, -item);

  switch(itemNr)
  {
  case ITM_DSTACK:  //  132
    result = displayStack;
    break;

  case ITM_SHOIREP:
    result = displayStackSHOIDISP;
    break;

  case ITM_GAP:     //  215
    result = groupingGap;
    break;

  case ITM_FIX:     //  185
    if(displayFormat == DF_FIX && SigFigMode == 0) {
      result = displayFormatDigits;
    }
    break;

  case ITM_SIGFIG:  // 1682
    if(displayFormat == DF_FIX && SigFigMode != 0) {
      result = displayFormatDigits;
    }
    break;

  case ITM_ENG:     //  145
    if(displayFormat == DF_ENG && !UNITDisplay) {
      result = displayFormatDigits;
    }
    break;

  case ITM_UNIT:    // 1693
    if(displayFormat == DF_ENG && UNITDisplay) {
      result = displayFormatDigits;
    }
    break;

  case ITM_SCI:     //  545
    if(displayFormat == DF_SCI) {
      result = displayFormatDigits;
    }
    break;

  case ITM_ALL:     //   20
    if(displayFormat == DF_ALL) {
      result = displayFormatDigits;
    }
    break;

  case ITM_WSIZE:   //  664
    result = shortIntegerWordSize;
    break;

  case ITM_RNG:   //  
    result = exponentLimit;
    break;

  case ITM_HASH_JM:   //  
    if(lastIntegerBase!=0) result = lastIntegerBase; 
    else result = -99;
    break;

  default:
    if(indexOfItems[itemNr].func == itemToBeCoded) {
      result = ITEM_NOT_CODED;
    }
    break;
  }

  return result;
}



char tmp[12];

void add_digitglyph_to_tmp2(char* tmp2, int16_t xx) {
  tmp2[0] = 0;

  switch(xx)
  {
    case  0: strcat(tmp2, STD_SUB_0);   break;
    case  1: strcat(tmp2, STD_BASE_1);  break;
    case  2: strcat(tmp2, STD_BASE_2);  break;
    case  3: strcat(tmp2, STD_BASE_3);  break;
    case  4: strcat(tmp2, STD_BASE_4);  break;
    case  5: strcat(tmp2, STD_BASE_5);  break;
    case  6: strcat(tmp2, STD_BASE_6);  break;
    case  7: strcat(tmp2, STD_BASE_7);  break;
    case  8: strcat(tmp2, STD_BASE_8);  break;
    case  9: strcat(tmp2, STD_BASE_9);  break;
    case 10: strcat(tmp2, STD_BASE_10); break;
    case 11: strcat(tmp2, STD_BASE_11); break;
    case 12: strcat(tmp2, STD_BASE_12); break;
    case 13: strcat(tmp2, STD_BASE_13); break;
    case 14: strcat(tmp2, STD_BASE_14); break;
    case 15: strcat(tmp2, STD_BASE_15); break;
    case 16: strcat(tmp2, STD_BASE_16); break;
    default:                            break;
  }
}



void use_base_glyphs(char* tmp1, int16_t xx) {                         //Needs non-local variable tmp2
  char tmp2[12];
  tmp1[0] = 0;
  
  if(xx <= 16) {
    add_digitglyph_to_tmp2(tmp2, xx);
    strcat(tmp1, tmp2);
  }
  else if(xx <= 99) {
    add_digitglyph_to_tmp2(tmp2, xx / 10);
    strcat(tmp1, tmp2);
    add_digitglyph_to_tmp2(tmp2, xx % 10);
    strcat(tmp1, tmp2);
  }
  else if(xx ==999) {
    add_digitglyph_to_tmp2(tmp2, 9);   strcat(tmp1, tmp2);
    add_digitglyph_to_tmp2(tmp2, 9);   strcat(tmp1, tmp2);
    add_digitglyph_to_tmp2(tmp2, 9);   strcat(tmp1, tmp2);
  }
  else if(xx ==6145) {
    add_digitglyph_to_tmp2(tmp2, 6);   strcat(tmp1, tmp2);
    add_digitglyph_to_tmp2(tmp2, 1);   strcat(tmp1, tmp2);
    add_digitglyph_to_tmp2(tmp2, 4);   strcat(tmp1, tmp2);
    add_digitglyph_to_tmp2(tmp2, 5);   strcat(tmp1, tmp2);
  }
  else {
    snprintf(tmp1, 12, "%d", xx);
  }
}



char* figlabel(const char* label, int16_t showValue) {      //JM
  char tmp1[12];
  tmp[0] = 0;

  if(strlen(label) <= 12) {
    strcpy(tmp, label);
  }
  if(showValue >= 0 && strlen(label) <= 8) {
    use_base_glyphs(tmp1, showValue);
    strcat(tmp, tmp1);
  }

  return tmp;
}                                                           //JM ^^


