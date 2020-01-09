/* This file is part of 43C.
 *
 * 43C is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 43C is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 43C.  If not, see <http://www.gnu.org/licenses/>.
 */

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
/*  464 */  { ITM_POLAR,            CM_POLAR,               RB_CM },  //fnComplexMode
/*  507 */  { ITM_RECT,             CM_RECTANGULAR,         RB_CM },  //fnComplexMode
/*   96 */  { ITM_CPXI,             CU_I,                   RB_CU },  //fnComplexUnit
/*   97 */  { ITM_CPXJ,             CU_J,                   RB_CU },  //fnComplexUnit
/*   44 */  { ITM_BESTF,            CF_BEST_FITTING,        RB_CF },  //fnCurveFitting
/*  161 */  { ITM_EXPF,             CF_EXPONENTIAL_FITTING, RB_CF },  //fnCurveFitting
/*  308 */  { ITM_LINF,             CF_LINEAR_FITTING,      RB_CF },  //fnCurveFitting
/*  325 */  { ITM_LOGF,             CF_LOGARITHMIC_FITTING, RB_CF },  //fnCurveFitting
/*  466 */  { ITM_POWERF,           CF_POWER_FITTING,       RB_CF },  //fnCurveFitting
/*  136 */  { ITM_DMY,              DF_DMY,                 RB_DF },  //fnDateFormat
/*  383 */  { ITM_MDY,              DF_MDY,                 RB_DF },  //fnDateFormat
/*  700 */  { ITM_YMD,              DF_YMD,                 RB_DF },  //fnDateFormat
/*  119 */  { ITM_DENANY,           DM_ANY,                 RB_DM },  //fnDenMode
/*  120 */  { ITM_DENFAC,           DM_FAC,                 RB_DM },  //fnDenMode
/*  121 */  { ITM_DENFIX,           DM_FIX,                 RB_DM },  //fnDenMode
/*   20 */  { ITM_ALL,              DF_ALL,                 RB_DI },  //fnDisplayFormatAll
/*  145 */  { ITM_ENG,              DF_ENG,                 RB_DI },  //fnDisplayFormatEng
/*  185 */  { ITM_FIX,              DF_FIX,                 RB_DI },  //fnDisplayFormatFix
/*  545 */  { ITM_SCI,              DF_SCI,                 RB_DI },  //fnDisplayFormatSci
/* 1682 */  { ITM_SIGFIG,           DF_SF,                  RB_DI },  //fnDisplayFormatSigFig
/* 1693 */  { ITM_UNIT,             DF_UN,                  RB_DI },  //fnDisplayFormatUnit
/*  146 */  { ITM_ENGOVR,           DO_ENG,                 RB_DO },  //fnDisplayOvr
/*  547 */  { ITM_SCIOVR,           DO_SCI,                 RB_DO },  //fnDisplayOvr
/*  251 */  { ITM_IMPFRC,           FT_IMPROPER,            RB_FT },  //fnFractionType
/*  471 */  { ITM_PROFRC,           FT_PROPER,              RB_FT },  //fnFractionType
/* 1905 */  { ITM_INP_DEF_43S,      ID_43S,                 RB_ID },  //fnInDefault
/* 1910 */  { ITM_INP_DEF_CPXDP,    ID_CPXDP,               RB_ID },  //fnInDefault
/* 1907 */  { ITM_INP_DEF_DP,       ID_DP,                  RB_ID },  //fnInDefault
/* 1913 */  { ITM_INP_DEF_LI,       ID_LI,                  RB_ID },  //fnInDefault
/* 1912 */  { ITM_INP_DEF_SI,       ID_SI,                  RB_ID },  //fnInDefault
/*    4 */  { ITM_1COMPL,           SIM_1COMPL,             RB_IM },  //fnIntegerMode
/*    7 */  { ITM_2COMPL,           SIM_2COMPL,             RB_IM },  //fnIntegerMode
/*  567 */  { ITM_SIGNMT,           SIM_SIGNMT,             RB_IM },  //fnIntegerMode
/*  645 */  { ITM_UNSIGN,           SIM_UNSIGN,             RB_IM },  //fnIntegerMode
/*  373 */  { ITM_MULTCR,           PS_CROSS,               RB_PS },  //fnProductSign
/*  374 */  { ITM_MULTDOT,          PS_DOT,                 RB_PS },  //fnProductSign
/*  500 */  { ITM_RDXCOM,           RM_COMMA,               RB_RM },  //fnRadixMark
/*  501 */  { ITM_RDXPER,           RM_PERIOD,              RB_RM },  //fnRadixMark
/* 1689 */  { ITM_WS8,              8,                      RB_WS },  //fnSetWordSize
/* 1690 */  { ITM_WS16,             16,                     RB_WS },  //fnSetWordSize
/* 1691 */  { ITM_WS32,             32,                     RB_WS },  //fnSetWordSize
/* 1692 */  { ITM_WS64,             64,                     RB_WS },  //fnSetWordSize
/*  583 */  { ITM_SSIZE4,           SS_4,                   RB_SS },  //fnStackSize
/*  584 */  { ITM_SSIZE8,           SS_8,                   RB_SS },  //fnStackSize
/*   75 */  { ITM_CLK12,            TF_H12,                 RB_TF },  //fnTimeFormat
/*   76 */  { ITM_CLK24,            TF_H24,                 RB_TF },  //fnTimeFormat
/* 1899 */  { ITM_U_KEY_ALPHA,      ITM_AIM,                RB_SA },  //fnSigmaAssign
/* 1897 */  { ITM_U_KEY_PRGM,       ITM_PR,                 RB_SA },  //fnSigmaAssign
/* 1895 */  { ITM_U_KEY_SIGMA,      ITM_SIGMAPLUS,          RB_SA },  //fnSigmaAssign
/* 1891 */  { ITM_U_KEY_CC,         KEY_CC,                 RB_SA },  //fnSigmaAssign
/* 1889 */  { ITM_U_KEY_USER,       KEY_USERMODE,           RB_SA },  //fnSigmaAssign
/* 1893 */  { ITM_U_KEY_MM,         -MNU_MyMenu,            RB_SA },  //fnSigmaAssign
/* 1744 */  { ITM_BASE_AHOME,       JC_BASE_AHOME,          CB_JC },  //fnSetSetJM
/* 1742 */  { ITM_BASE_HOME,        JC_BASE_HOME,           CB_JC },  //fnSetSetJM
/* 1695 */  { ITM_CB_CPXRES,        JC_BCR,                 CB_JC },  //fnSetSetJM
/* 1696 */  { ITM_CB_LEADING_ZERO,  JC_BLZ,                 CB_JC },  //fnSetSetJM
/* 1678 */  { ITM_ERPN,             JC_ERPN,                CB_JC },  //fnSetSetJM
/* 1909 */  { ITM_FG_DOTS,          JC_FG_DOTS,             CB_JC },  //fnSetSetJM
/* 1906 */  { ITM_FG_LINE,          JC_FG_LINE,             CB_JC },  //fnSetSetJM
/* 1911 */  { ITM_G_DOUBLETAP,      JC_G_DOUBLETAP,         CB_JC },  //fnSetSetJM
/* 1679 */  { ITM_HOMEx3,           JC_HOME_TRIPLE,         CB_JC },  //fnSetSetJM
/* 1770 */  { ITM_HOMEx3T,          JC_SH_3T,               CB_JC },  //fnSetSetJM
/* 1680 */  { ITM_SHTIM,            JC_SHFT_4s,             CB_JC }   //fnSetSetJM
#ifdef INLINE_TEST
/* 1892 */, { ITM_TEST,             DR_ITM_TST,             CB_JC }   //fnSetInlineTest
#endif
};



int8_t fnCbIsSet(int16_t item) {
  int8_t result = -1;
  uint16_t itemNr = max(item, -item);

/*for(uint8_t i=0; i < cntOfRadioCbItems; i++) {
    if(indexOfRadioCbItems[i].itemNr == itemNr) {
      result = indexOfRadioCbItems[i].state;
    }
  }*/
  size_t n = min(nbrOfElements(indexOfRadioCbEepromItems), MAX_RADIO_CB_ITEMS);
  for(uint8_t i=0; i < n; i++) {
    if(indexOfRadioCbEepromItems[i].itemNr == itemNr) {
      result = indexOfRadioCbItems[i].state;
    }
  }

  return result;
}



void fnRefreshRadioState(char rb, uint16_t mode) {
/*for(uint8_t i=0; i < cntOfRadioCbItems; i++) {
    if(indexOfRadioCbItems[i].radioButton == rb) {
      uint8_t cb = (indexOfRadioCbItems[i].param == mode) ? 1 : 0;
      if(indexOfRadioCbItems[i].state != cb) {
        indexOfRadioCbItems[i].state = cb;
#ifndef TESTSUITE_BUILD
        showSoftmenuCurrentPart();
#endif
      }
      //indexOfRadioCbItems[i].state = indexOfRadioCbItems[i].param == mode;
    }
  }*/
  size_t n = min(nbrOfElements(indexOfRadioCbEepromItems), MAX_RADIO_CB_ITEMS);
  for(uint8_t i=0; i < n; i++) {
    if(indexOfRadioCbEepromItems[i].radioButton == rb) {
      uint8_t cb = (indexOfRadioCbEepromItems[i].param == mode) ? RB_TRUE : RB_FALSE;
      if(indexOfRadioCbItems[i].state != cb) {
        indexOfRadioCbItems[i].state = cb;
#ifndef TESTSUITE_BUILD
        showSoftmenuCurrentPart();
#endif
      }
    }
  }
}



void fnRefreshComboxState(char rb, uint16_t param, bool_t state) {
/*for(uint8_t i=0; i < cntOfRadioCbItems; i++) {
    if((indexOfRadioCbItems[i].radioButton == rb) && (indexOfRadioCbItems[i].param == param)) {
      uint8_t cb = state ? 3 : 2;
      if(indexOfRadioCbItems[i].state != cb) {
        indexOfRadioCbItems[i].state = cb;
#ifndef TESTSUITE_BUILD
        showSoftmenuCurrentPart();
#endif
      }
    }
  }*/
  size_t n = min(nbrOfElements(indexOfRadioCbEepromItems), MAX_RADIO_CB_ITEMS);
  for(uint8_t i=0; i < n; i++) {
    if((indexOfRadioCbEepromItems[i].radioButton == rb) && (indexOfRadioCbEepromItems[i].param == param)) {
      uint8_t cb = state ? CB_TRUE : CB_FALSE;
      if(indexOfRadioCbItems[i].state != cb) {
        indexOfRadioCbItems[i].state = cb;
#ifndef TESTSUITE_BUILD
        showSoftmenuCurrentPart();
#endif
      }
    }
  }
}



int16_t fnItemShowValue(int16_t item) {
  int16_t result = -1;
  uint16_t itemNr = max(item, -item);

  switch(itemNr)
  {
  case ITM_DSTACK:  //  132
    result = displayStack;
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

  default:
    if(indexOfItems[itemNr].func == itemToBeCoded) {
      result = ITEM_NOT_CODED;
    }
    break;
  }

  return result;
}



void fnRebuildRadioState() {
//  117     { ITM_DEG,              AM_DEGREE,              RB_AM },  //fnAngularMode
//  134     { ITM_DMS,              AM_DMS,                 RB_AM },  //fnAngularMode
//  229     { ITM_GRAD,             AM_GRAD,                RB_AM },  //fnAngularMode
//  375     { ITM_MULPI,            AM_MULTPI,              RB_AM },  //fnAngularMode
//  483     { ITM_RAD,              AM_RADIAN,              RB_AM },  //fnAngularMode
  fnRefreshRadioState(RB_AM, currentAngularMode);
//  464     { ITM_POLAR,            CM_POLAR,               RB_CM },  //fnComplexMode
//  507     { ITM_RECT,             CM_RECTANGULAR,         RB_CM },  //fnComplexMode
  fnRefreshRadioState(RB_CM, complexMode);
//   96     { ITM_CPXI,             CU_I,                   RB_CU },  //fnComplexUnit
//   97     { ITM_CPXJ,             CU_J,                   RB_CU },  //fnComplexUnit
  fnRefreshRadioState(RB_CU, complexUnit);
//   44     { ITM_BESTF,            CF_BEST_FITTING,        RB_CF },  //fnCurveFitting
//  161     { ITM_EXPF,             CF_EXPONENTIAL_FITTING, RB_CF },  //fnCurveFitting
//  308     { ITM_LINF,             CF_LINEAR_FITTING,      RB_CF },  //fnCurveFitting
//  325     { ITM_LOGF,             CF_LOGARITHMIC_FITTING, RB_CF },  //fnCurveFitting
//  466     { ITM_POWERF,           CF_POWER_FITTING,       RB_CF },  //fnCurveFitting
  fnRefreshRadioState(RB_CF, curveFitting);
//  136     { ITM_DMY,              DF_DMY,                 RB_DF },  //fnDateFormat
//  383     { ITM_MDY,              DF_MDY,                 RB_DF },  //fnDateFormat
//  700     { ITM_YMD,              DF_YMD,                 RB_DF },  //fnDateFormat
  fnRefreshRadioState(RB_DF, dateFormat);
//  119     { ITM_DENANY,           DM_ANY,                 RB_DM },  //fnDenMode
//  120     { ITM_DENFAC,           DM_FAC,                 RB_DM },  //fnDenMode
//  121     { ITM_DENFIX,           DM_FIX,                 RB_DM },  //fnDenMode
  fnRefreshRadioState(RB_DM, denominatorMode);
//   20     { ITM_ALL,              DF_ALL,                 RB_DI },  //fnDisplayFormatAll
//  145     { ITM_ENG,              DF_ENG,                 RB_DI },  //fnDisplayFormatEng
//  185     { ITM_FIX,              DF_FIX,                 RB_DI },  //fnDisplayFormatFix
//  545     { ITM_SCI,              DF_SCI,                 RB_DI },  //fnDisplayFormatSci
// 1682     { ITM_SIGFIG,           DF_SF,                  RB_DI },  //fnDisplayFormatSigFig
// 1693     { ITM_UNIT,             DF_UN,                  RB_DI },  //fnDisplayFormatUnit
  uint8_t df = displayFormat;
  if(df == DF_FIX && SigFigMode != 0) { df = DF_SF; }
  if(df == DF_ENG && UNITDisplay) { df = DF_UN; }
  fnRefreshRadioState(RB_DI, df);
//  146     { ITM_ENGOVR,           DO_ENG,                 RB_DO },  //fnDisplayOvr
//  547     { ITM_SCIOVR,           DO_SCI,                 RB_DO },  //fnDisplayOvr
  fnRefreshRadioState(RB_DO, displayModeOverride);
//  251     { ITM_IMPFRC,           FT_IMPROPER,            RB_FT },  //fnFractionType
//  471     { ITM_PROFRC,           FT_PROPER,              RB_FT },  //fnFractionType
  fnRefreshRadioState(RB_FT, fractionType);
// 1905     { ITM_INP_DEF_43S,      ID_43S,                 RB_ID },  //fnInDefault
// 1910     { ITM_INP_DEF_CPXDP,    ID_CPXDP,               RB_ID },  //fnInDefault
// 1907     { ITM_INP_DEF_DP,       ID_DP,                  RB_ID },  //fnInDefault
// 1913     { ITM_INP_DEF_LI,       ID_LI,                  RB_ID },  //fnInDefault
// 1912     { ITM_INP_DEF_SI,       ID_SI,                  RB_ID },  //fnInDefault
  fnRefreshRadioState(RB_ID, Input_Default);
//    4     { ITM_1COMPL,           SIM_1COMPL,             RB_IM },  //fnIntegerMode
//    7     { ITM_2COMPL,           SIM_2COMPL,             RB_IM },  //fnIntegerMode
//  567     { ITM_SIGNMT,           SIM_SIGNMT,             RB_IM },  //fnIntegerMode
//  645     { ITM_UNSIGN,           SIM_UNSIGN,             RB_IM },  //fnIntegerMode
  fnRefreshRadioState(RB_IM, shortIntegerMode);
//  373     { ITM_MULTCR,           PS_CROSS,               RB_PS },  //fnProductSign
//  374     { ITM_MULTDOT,          PS_DOT,                 RB_PS },  //fnProductSign
  fnRefreshRadioState(RB_PS, productSign);
//  500     { ITM_RDXCOM,           RM_COMMA,               RB_RM },  //fnRadixMark
//  501     { ITM_RDXPER,           RM_PERIOD,              RB_RM },  //fnRadixMark
  fnRefreshRadioState(RB_RM, radixMark);
// 1689     { ITM_WS8,              8,                      RB_WS },  //fnSetWordSize
// 1690     { ITM_WS16,             16,                     RB_WS },  //fnSetWordSize
// 1691     { ITM_WS32,             32,                     RB_WS },  //fnSetWordSize
// 1692     { ITM_WS64,             64,                     RB_WS },  //fnSetWordSize
  fnRefreshRadioState(RB_WS, shortIntegerWordSize);
//  583     { ITM_SSIZE4,           SS_4,                   RB_SS },  //fnStackSize
//  584     { ITM_SSIZE8,           SS_8,                   RB_SS },  //fnStackSize
  fnRefreshRadioState(RB_SS, stackSize);
//   75     { ITM_CLK12,            TF_H12,                 RB_TF },  //fnTimeFormat
//   76     { ITM_CLK24,            TF_H24,                 RB_TF },  //fnTimeFormat
  fnRefreshRadioState(RB_TF, timeFormat);
// 1899     { ITM_U_KEY_ALPHA,      ITM_AIM,                RB_SA },  //fnSigmaAssign
// 1897     { ITM_U_KEY_PRGM,       ITM_PR,                 RB_SA },  //fnSigmaAssign
// 1895     { ITM_U_KEY_SIGMA,      ITM_SIGMAPLUS,          RB_SA },  //fnSigmaAssign
// 1891     { ITM_U_KEY_CC,         KEY_CC,                 RB_SA },  //fnSigmaAssign
// 1889     { ITM_U_KEY_USER,       KEY_USERMODE,           RB_SA },  //fnSigmaAssign
// 1893     { ITM_U_KEY_MM,         -MNU_MyMenu,            RB_SA },  //fnSigmaAssign
  fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
// 1744     { ITM_BASE_AHOME,       JC_BASE_AHOME,          CB_JC },  //fnSetSetJM
  fnRefreshComboxState(CB_JC, JC_BASE_AHOME, SH_BASE_AHOME);
// 1742     { ITM_BASE_HOME,        JC_BASE_HOME,           CB_JC },  //fnSetSetJM
  fnRefreshComboxState(CB_JC, JC_BASE_HOME, SH_BASE_HOME);
// 1695 */  { ITM_CB_CPXRES,        JC_BCR,                 CB_JC },  //fnSetSetJM
  fnRefreshComboxState(CB_JC, JC_BCR, getFlag(FLAG_CPXRES));
// 1696     { ITM_CB_LEADING_ZERO,  JC_BLZ,                 CB_JC },  //fnSetSetJM
  fnRefreshComboxState(CB_JC, JC_BLZ, displayLeadingZeros);
// 1678     { ITM_ERPN,             JC_ERPN,                CB_JC },  //fnSetSetJM
  fnRefreshComboxState(CB_JC, JC_ERPN, eRPN);
// 1909     { ITM_FG_DOTS,          JC_FG_DOTS,             CB_JC },  //fnSetSetJM
  fnRefreshComboxState(CB_JC, JC_FG_DOTS, jm_FG_DOTS);
// 1906     { ITM_FG_LINE,          JC_FG_LINE,             CB_JC },  //fnSetSetJM
  fnRefreshComboxState(CB_JC, JC_FG_LINE, jm_FG_LINE);
// 1911     { ITM_G_DOUBLETAP,      JC_G_DOUBLETAP,         CB_JC },  //fnSetSetJM
  fnRefreshComboxState(CB_JC, JC_G_DOUBLETAP, jm_G_DOUBLETAP);
// 1679     { ITM_HOMEx3,           JC_HOME_TRIPLE,         CB_JC },  //fnSetSetJM
  fnRefreshComboxState(CB_JC, JC_HOME_TRIPLE, HOME3);
// 1770     { ITM_HOMEx3T,          JC_SH_3T,               CB_JC },  //fnSetSetJM
  fnRefreshComboxState(CB_JC, JC_SH_3T, Home3TimerMode);
// 1680     { ITM_SHTIM,            JC_SHFT_4s,             CB_JC }   //fnSetSetJM
  fnRefreshComboxState(CB_JC, JC_SHFT_4s, ShiftTimoutMode);
#ifdef INLINE_TEST
// 1892   , { ITM_TEST,             DR_ITM_TST,             CB_JC }   //fnSetInlineTest
  fnRefreshComboxState(CB_JC, DR_ITM_TST, false);
#endif

  size_t n = nbrOfElements(indexOfRadioCbEepromItems);
  printf("Nbr of RadioButton/Checkbox  %6I64u\n", n);
}


/*******
void fnRebuildRadioState() {
  uint8_t i=0;
  for(uint16_t k=0; k<LAST_ITEM; k++) {
    if(indexOfItems[k].func == fnAngularMode) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (currentAngularMode == rb.param) ? 1 : 0;
      rb.radioButton = RB_AM;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnComplexMode) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (complexMode == rb.param) ? 1 : 0;
      rb.radioButton = RB_CM;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnComplexUnit) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (complexUnit == rb.param) ? 1 : 0;
      rb.radioButton = RB_CU;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnCurveFitting) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (curveFitting == rb.param) ? 1 : 0;
      rb.radioButton = RB_CF;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnDateFormat) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (dateFormat == rb.param) ? 1 : 0;
      rb.radioButton = RB_DF;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnDenMode) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (denominatorMode == rb.param) ? 1 : 0;
      rb.radioButton = RB_DM;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnDisplayFormatAll) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = DF_ALL;
      rb.state = (displayFormat == rb.param) ? 1 : 0;
      rb.radioButton = RB_DI;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnDisplayFormatEng) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = DF_ENG;
      rb.state = (displayFormat == rb.param) ? 1 : 0;
      rb.radioButton = RB_DI;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnDisplayFormatFix) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = DF_FIX;
      rb.state = (displayFormat == rb.param) && !(UNITDisplay || (SigFigMode != 0)) ? 1 : 0;
      rb.radioButton = RB_DI;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnDisplayFormatSigFig) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = DF_SF;
      rb.state = (SigFigMode != 0) ? 1 : 0;
      rb.radioButton = RB_DI;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }

    else if(indexOfItems[k].func == fnDisplayFormatUnit) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = DF_UN;
      rb.state = UNITDisplay ? 1 : 0;
      rb.radioButton = RB_DI;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnDisplayFormatSci) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = DF_SCI;
      rb.state = (displayFormat == rb.param) ? 1 : 0;
      rb.radioButton = RB_DI;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnDisplayOvr) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (displayModeOverride == rb.param) ? 1 : 0;
      rb.radioButton = RB_DO;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnFractionType) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (fractionType == rb.param) ? 1 : 0;
      rb.radioButton = RB_FT;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnIntegerMode) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (shortIntegerMode == rb.param) ? 1 : 0;
      rb.radioButton = RB_IM;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnProductSign) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (productSign == rb.param) ? 1 : 0;
      rb.radioButton = RB_PS;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnRadixMark) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (radixMark == rb.param) ? 1 : 0;
      rb.radioButton = RB_RM;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnStackSize) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (stackSize == rb.param) ? 1 : 0;
      rb.radioButton = RB_SS;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnTimeFormat) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (timeFormat == rb.param) ? 1 : 0;
      rb.radioButton = RB_TF;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if((indexOfItems[k].func == fnSetWordSize) && (k != ITM_WSIZE)) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (shortIntegerWordSize == rb.param) ? 1 : 0;
      rb.radioButton = RB_WS;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnInDefault) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (Input_Default == rb.param) ? 1 : 0;
      rb.radioButton = RB_ID;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
    else if(indexOfItems[k].func == fnSigmaAssign) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (Norm_Key_00_VAR == rb.param) ? 1 : 0;
      rb.radioButton = RB_SA;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
  //else if(indexOfItems[k].func == fnComplexResult) {
  //  radiocb_t rb;
  //  rb.itemNr = k;
  //  rb.param = indexOfItems[k].param;
  //  rb.state = (getFlag(FLAG_CPXRES) == rb.param) ? 1 : 0;
  //  rb.radioButton = RB_BCR;
  //  if(i<MAX_RADIO_CB_ITEMS) {
  //    indexOfRadioCbItems[i] = rb;
  //  }
  //  i++;
  //}
  //else if(indexOfItems[k].func == fnLeadingZeros) {
  //  radiocb_t rb;
  //  rb.itemNr = k;
  //  rb.param = indexOfItems[k].param;
  //  rb.state = (displayLeadingZeros == rb.param) ? 1 : 0;
  //  rb.radioButton = RB_BLZ;
  //  if(i<MAX_RADIO_CB_ITEMS) {
  //    indexOfRadioCbItems[i] = rb;
  //  }
  //  i++;
  //}
    else if(indexOfItems[k].func == fnSetSetJM) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      switch (rb.param)
      {
      case JC_ERPN:
        rb.state = eRPN? 3 : 2;
        break;

      case JC_FG_LINE:
        rb.state = jm_FG_LINE? 3 : 2;
        break;

      case JC_FG_DOTS:
        rb.state = jm_FG_DOTS? 3 : 2;
        break;

      case JC_G_DOUBLETAP:
        rb.state = jm_G_DOUBLETAP? 3 : 2;
        break;

      case JC_HOME_TRIPLE:
        rb.state = HOME3? 3 : 2;
        break;

      case JC_SHFT_4s:
        rb.state = ShiftTimoutMode? 3 : 2;
        break;

      case JC_BASE_HOME:
        rb.state = SH_BASE_HOME? 3 : 2;
        break;

      case JC_BASE_AHOME:
        rb.state = SH_BASE_AHOME? 3 : 2;
        break;

      case JC_SH_3T:
        rb.state = Home3TimerMode? 3 : 2;
        break;

      case JC_BCR:
        rb.state = getFlag(FLAG_CPXRES)? 3 : 2;
        break;

      case JC_BLZ:
        rb.state = displayLeadingZeros? 3 : 2;
        break;

      default:
        break;
      }
      rb.radioButton = CB_JC;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
#ifdef INLINE_TEST
    else if(indexOfItems[k].func == fnSetInlineTest) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      switch (rb.param)
      {
      case DR_ITM_TST:
        rb.state = testEnabled? 3 : 2;
        break;

      default:
        break;
      }
      rb.radioButton = CB_JC;
      if(i<MAX_RADIO_CB_ITEMS) {
        indexOfRadioCbItems[i] = rb;
      }
      i++;
    }
#endif

  }
  if(i<MAX_RADIO_CB_ITEMS) {
    cntOfRadioCbItems = i;
  }
  else {
    cntOfRadioCbItems = MAX_RADIO_CB_ITEMS;
  }

  printf("Nbr of RadioButton/Checkbox  %6u\n", i);
}
*******/
