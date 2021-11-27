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
 * \file assign.c
 ***********************************************/

#include "assign.h"
#include "charString.h"
#include "error.h"
#include "fonts.h"
#include "items.h"
#include "memory.h"
#include "registers.h"
#include "screen.h"
#include "softmenus.h"
#include "sort.h"
#include "wp43s.h"
#include <string.h>

//This variable is to store in flash memory
TO_QSPI const calcKey_t kbd_std[37] = {
//keyId primary          fShifted         gShifted         keyLblAim        primaryAim         fShiftedAim            gShiftedAim    primaryTam
/*
 {21,   ITM_1ONX,        ITM_TGLFRT,     -MNU_ALPHAFN,     ITM_NULL,        ITM_A,            -MNU_ALPHAINTL,         ITM_ALPHA,     ITM_REG_A    },
 {22,  -MNU_EXP,         ITM_toINT,      -MNU_BITS,        ITM_NUMBER_SIGN, ITM_B,             ITM_NUMBER_SIGN,       ITM_BETA,      ITM_REG_B    },
 {23,  -MNU_TRI,         ITM_DMS,        -MNU_ANGLECONV,   ITM_NULL,        ITM_C,             ITM_LEFT_PARENTHESIS,  ITM_GAMMA,     ITM_REG_C    },
 {24,   ITM_LN,          ITM_dotD,        ITM_LOG10,       ITM_NULL,        ITM_D,             ITM_RIGHT_PARENTHESIS, ITM_DELTA,     ITM_REG_D    }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case D for decimal base
 {25,   ITM_EXP,         ITM_toHMS,       ITM_10x,         ITM_ALOG_SYMBOL, ITM_E,             ITM_ALOG_SYMBOL,       ITM_EPSILON,   ITM_NULL     },
 {26,   ITM_SQUAREROOTX, ITM_AIM,        -MNU_CPX,         ITM_ROOT_SIGN,   ITM_F,             ITM_ROOT_SIGN,         ITM_PHI,       ITM_alpha    },


 {31,   ITM_STO,         ITM_ASSIGN,      ITM_SAVE,        ITM_NULL,        ITM_G,             ITM_ASSIGN,            ITM_GAMMA,     ITM_NULL     },
 {32,   ITM_RCL,         ITM_RBR,         ITM_VIEW,        ITM_NULL,        ITM_H,             ITM_RBR,               ITM_CHI,       ITM_HEX      }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case H for hexadecimal base
 {33,   ITM_Rdown,       ITM_Rup,        -MNU_STK,         ITM_NULL,        ITM_I,             ITM_DOWN_ARROW,        ITM_IOTA,      ITM_REG_I    },
 {34,   ITM_CC,          ITM_MAGNITUDE,   ITM_ANGLE,       ITM_NULL,        ITM_J,             ITM_VERTICAL_BAR,      ITM_ETA,       ITM_REG_J    },
 {35,   ITM_SHIFTf,      ITM_NULL,        ITM_SNAP,        ITM_SHIFTf,      ITM_SHIFTf,        ITM_NULL,              ITM_SNAP,      ITM_SHIFTf   },
 {36,   ITM_SHIFTg,      ITM_USERMODE,    ITM_NULL,        ITM_SHIFTg,      ITM_SHIFTg,        ITM_USERMODE,          ITM_NULL,      ITM_SHIFTg   },

 {41,   ITM_ENTER,       ITM_FILL,        ITM_DROP,        ITM_ENTER,       ITM_ENTER,         ITM_NULL,              ITM_NULL,      ITM_ENTER    },
 {42,   ITM_XexY,        ITM_Xex,        -MNU_UNITCONV,    ITM_ex,          ITM_K,             ITM_ex,                ITM_KAPPA,     ITM_REG_K    },
 {43,   ITM_CHS,         ITM_DELTAPC,    -MNU_FIN,         ITM_PLUS_MINUS,  ITM_L,             ITM_PLUS_MINUS,        ITM_LAMBDA,    ITM_REG_L    },
 {44,   ITM_EXPONENT,    ITM_DSP,        -MNU_DISP,        ITM_NULL,        ITM_M,             ITM_UP_ARROW,          ITM_MU,        ITM_NULL     },
 {45,   ITM_BACKSPACE,   ITM_UNDO,       -MNU_CLR,         ITM_BACKSPACE,   ITM_BACKSPACE,     ITM_UNDO,             -MNU_CLR,       ITM_BACKSPACE},

 {51,   ITM_DIV,         ITM_PARALLEL,    ITM_MOD,         ITM_SLASH,       ITM_N,             ITM_SLASH,             ITM_NU,        ITM_DIV      },
 {52,   ITM_7,           ITM_NULL,        ITM_NULL,        ITM_7,           ITM_O,             ITM_7,                 ITM_OMEGA,     ITM_7        },
 {53,   ITM_8,           ITM_NULL,        ITM_NULL,        ITM_8,           ITM_P,             ITM_8,                 ITM_PI,        ITM_8        },
 {54,   ITM_9,           ITM_NULL,        ITM_RTN,         ITM_9,           ITM_Q,             ITM_9,                 ITM_NULL,      ITM_9        },
 {55,   ITM_XEQ,         ITM_GTO,         ITM_LBL,         ITM_NULL,        ITM_NULL,          ITM_NULL,              ITM_NULL,      ITM_NULL     },

 {61,   ITM_MULT,        ITM_XFACT,      -MNU_PROB,        ITM_CROSS,       ITM_R,             ITM_PROD_SIGN,         ITM_RHO,       ITM_MULT     },
 {62,   ITM_4,          -MNU_SUMS,       -MNU_STAT,        ITM_4,           ITM_S,             ITM_4,                 ITM_SIGMA,     ITM_4        },
 {63,   ITM_5,           ITM_toREC,       ITM_toPOL,       ITM_5,           ITM_T,             ITM_5,                 ITM_TAU,       ITM_5        },
 {64,   ITM_6,           ITM_TIMER,      -MNU_CLK,         ITM_6,           ITM_U,             ITM_6,                 ITM_THETA,     ITM_6        },
 {65,   ITM_UP,          ITM_BST,        -MNU_FLAGS,       ITM_UP,          ITM_UP,            ITM_BST,              -MNU_FLAGS,     ITM_UP       },

 {71,   ITM_SUB,        -MNU_INTS,       -MNU_PARTS,       ITM_MINUS,       ITM_V,             ITM_MINUS,            -MNU_ALPHAMATH, ITM_SUB      },
 {72,   ITM_1,          -MNU_ADV,        -MNU_EQN,         ITM_1,           ITM_W,             ITM_1,                 ITM_PSI,       ITM_1        },
 {73,   ITM_2,          -MNU_MATX,       -MNU_XFN,         ITM_2,           ITM_X,             ITM_2,                 ITM_XI,        ITM_2        },
 {74,   ITM_3,           ITM_CONSTpi,    -MNU_CONST,       ITM_3,           ITM_Y,             ITM_3,                 ITM_UPSILON,   ITM_3        },
 {75,   ITM_DOWN,        ITM_SST,        -MNU_MODE,        ITM_DOWN,        ITM_DOWN,          ITM_SST,               ITM_NULL,      ITM_DOWN     },

 {81,   ITM_ADD,        -MNU_LOOP,       -MNU_TEST,        ITM_PLUS,        ITM_Z,             ITM_PLUS,              ITM_ZETA,      ITM_ADD      },
 {82,   ITM_0,          -MNU_IO,         -MNU_PRINT,       ITM_0,           ITM_QUESTION_MARK, ITM_0,                 ITM_PRINTER,   ITM_0        },
 {83,   ITM_PERIOD,      ITM_SHOW,       -MNU_INFO,        ITM_PERIOD,      ITM_COMMA,         ITM_PERIOD,           -MNU_ALPHADOT,  ITM_PERIOD   },
 {84,   ITM_RS,          ITM_PR,         -MNU_PFN,         ITM_NULL,        ITM_SPACE,         ITM_NULL,              ITM_NULL,      ITM_NULL     },
 {85,   ITM_EXIT,       -MNU_CATALOG,     ITM_OFF,         ITM_EXIT,        ITM_EXIT,         -MNU_CATALOG,           ITM_OFF,       ITM_EXIT     }
*/



#ifdef JM_LAYOUT_1A  //JM UPDATED TO LAYOUT 1C. OPTIMAL. SIMULATOR.
  //keyID,   primary,           fShifted,          gShifted,          keyLblAim,         primaryAim,        fShiftedAim,       gShiftedAim,       primaryTam         
      {21,   ITM_SIGMAPLUS,     ITM_RI,            ITM_TGLFRT,        ITM_NULL,          ITM_A,             ITM_a,             ITM_ALPHA,         ITM_REG_A          },
      {22,   ITM_1ONX,          ITM_YX,            ITM_HASH_JM,       ITM_NUMBER_SIGN,   ITM_B,             ITM_b,             ITM_BETA,          ITM_REG_B          },
      {23,   ITM_SQUAREROOTX,   ITM_SQUARE,        ITM_ms,            ITM_ROOT_SIGN,     ITM_C,             ITM_c,             ITM_CHI,           ITM_REG_C          },
      {24,   ITM_LOG10,         ITM_10x,           ITM_dotD,          ITM_NULL,          ITM_D,             ITM_d,             ITM_DELTA,         ITM_REG_D          },
      {25,   ITM_LN,            ITM_EXP,           ITM_toREC2,        ITM_NULL,          ITM_E,             ITM_e,             ITM_EPSILON,       ITM_NULL           },
      {26,   ITM_XEQ,           ITM_AIM,           ITM_toPOL2,        ITM_NULL,          ITM_F,             ITM_f,             ITM_DIGAMMA,       ITM_alpha          },

      {31,   ITM_STO,           ITM_MAGNITUDE,     ITM_ANGLE,         ITM_NULL,          ITM_G,             ITM_g,             ITM_GAMMA,         ITM_NULL           },
      {32,   ITM_RCL,           ITM_PC,            ITM_DELTAPC,       ITM_NULL,          ITM_H,             ITM_h,             ITM_ETA,           ITM_HEX            },
      {33,   ITM_Rdown,         ITM_CONSTpi,       ITM_XTHROOT,       ITM_NULL,          ITM_I,             ITM_i,             ITM_IOTA,          ITM_REG_I          },
      {34,   ITM_sin,           ITM_arcsin,        ITM_GTO,           ITM_NULL,          ITM_J,             ITM_j,             ITM_THETA,         ITM_REG_J          },
      {35,   ITM_cos,           ITM_arccos,        ITM_LBL,           ITM_NULL,          ITM_K,             ITM_k,             ITM_KAPPA,         ITM_REG_K          },
      {36,   ITM_tan,           ITM_arctan,        ITM_RTN,           ITM_NULL,          ITM_L,             ITM_l,             ITM_LAMBDA,        ITM_REG_L          },

      {41,   ITM_ENTER,         KEY_COMPLEX,       -MNU_CPX,          ITM_ENTER,         ITM_ENTER,         ITM_XEDIT,         ITM_NULL,          ITM_ENTER          },
      {42,   ITM_XexY,          ITM_LASTX,         ITM_Rup,           ITM_ex,            ITM_M,             ITM_m,             ITM_MU,            ITM_NULL           },
      {43,   ITM_CHS,           -MNU_MODE,         -MNU_STK,          ITM_PLUS_MINUS,    ITM_N,             ITM_n,             ITM_NU,            ITM_NULL           },
      {44,   ITM_EXPONENT,      -MNU_DISP,         -MNU_EXP,          ITM_NULL,          ITM_O,             ITM_o,             ITM_OMICRON,       ITM_NULL           },
      {45,   ITM_BACKSPACE,     ITM_UNDO,          -MNU_CLR,          ITM_BACKSPACE,     ITM_BACKSPACE,     ITM_CLA,           ITM_NULL,          ITM_BACKSPACE      },

      {51,   ITM_UP1,           ITM_BST,           ITM_RBR,           ITM_UP1,           ITM_UP1,           CHR_caseUP,        ITM_UP_ARROW,      ITM_UP1            },
      {52,   ITM_7,             -MNU_EQN,          -MNU_HOME,         ITM_7,             ITM_P,             ITM_p,             ITM_PI,            ITM_7              },
      {53,   ITM_8,             -MNU_ADV,          -MNU_CONST,        ITM_8,             ITM_Q,             ITM_q,             ITM_QOPPA,         ITM_8              },
      {54,   ITM_9,             -MNU_MATX,         -MNU_XFN,          ITM_9,             ITM_R,             ITM_r,             ITM_RHO,           ITM_9              },
      {55,   ITM_DIV,           -MNU_STAT,         -MNU_SUMS,         ITM_OBELUS,        ITM_S,             ITM_s,             ITM_SIGMA,         ITM_DIV            },

      {61,   ITM_DOWN1,         ITM_SST,           ITM_FLGSV,         ITM_DOWN1,         ITM_DOWN1,         CHR_caseDN,        ITM_DOWN_ARROW,    ITM_DOWN1          },
      {62,   ITM_4,             -MNU_BASE,         -MNU_CLK,          ITM_4,             ITM_T,             ITM_t,             ITM_TAU,           ITM_4              },
      {63,   ITM_5,             -MNU_ANGLECONV,    -MNU_UNITCONV,     ITM_5,             ITM_U,             ITM_u,             ITM_PHI,           ITM_5              },
      {64,   ITM_6,             -MNU_FLAGS,        -MNU_BITS,         ITM_6,             ITM_V,             ITM_v,             ITM_PSI,           ITM_6              },
      {65,   ITM_MULT,          -MNU_PROB,         -MNU_INTS,         ITM_CROSS,         ITM_W,             ITM_w,             ITM_OMEGA,         ITM_MULT           },

      {71,   KEY_fg,            ITM_NULL,          ITM_NULL,          KEY_fg,            KEY_fg,            ITM_NULL,          ITM_NULL,          KEY_fg             },
      {72,   ITM_1,             ITM_ASSIGN,        -MNU_ASN,          ITM_1,             ITM_X,             ITM_x,             ITM_XI,            ITM_1              },
      {73,   ITM_2,             ITM_USERMODE,      -MNU_LOOP,         ITM_2,             ITM_Y,             ITM_y,             ITM_UPSILON,       ITM_2              },
      {74,   ITM_3,             -MNU_PARTS,        -MNU_TEST,         ITM_3,             ITM_Z,             ITM_z,             ITM_ZETA,          ITM_3              },
      {75,   ITM_SUB,           -MNU_FIN,          -MNU_ALPHAFN,      ITM_MINUS,         ITM_UNDERSCORE,    ITM_MINUS,         ITM_SAMPI,         ITM_SUB            },

      {81,   ITM_EXIT1,         ITM_OFF,           ITM_PRN,           ITM_EXIT1,         ITM_EXIT1,         ITM_OFF,           ITM_PRINTER,       ITM_EXIT1          },
      {82,   ITM_0,             ITM_VIEW,          ITM_TIMER,         ITM_0,             ITM_COLON,         ITM_0,             -MNU_ALPHA,        ITM_0              },
      {83,   ITM_PERIOD,        ITM_SHOW,          -MNU_INFO,         ITM_PERIOD,        ITM_COMMA,         ITM_PERIOD,        -MNU_ALPHADOT,     ITM_PERIOD         },
      {84,   ITM_RS,            ITM_PR,            -MNU_PFN,          ITM_NULL,          ITM_QUESTION_MARK, ITM_SLASH,         -MNU_ALPHAMATH,    ITM_NULL           },
      {85,   ITM_ADD,           -MNU_CATALOG,      -MNU_IO,           ITM_PLUS,          ITM_SPACE,         ITM_PLUS,          -MNU_ALPHAINTL,    ITM_ADD            }
  //keyID,   primary,           fShifted,          gShifted,          keyLblAim,         primaryAim,        fShiftedAim,       gShiftedAim,       primaryTam         
#endif //JM END OF LAYOUT 1a.



#ifdef JM_LAYOUT_2_DM42_STRICT //JM DM42 STRICT. NAME CHANGE FROM LAYOUT 2 TO LAYOUT 42C
  //keyID,   primary,           fShifted,          gShifted,          keyLblAim,         primaryAim,        fShiftedAim,       gShiftedAim,       primaryTam         
      {21,   ITM_SIGMAPLUS,     ITM_SIGMAMINUS,    ITM_TGLFRT,        ITM_NULL,          ITM_A,             ITM_a,             ITM_ALPHA,         ITM_REG_A          },
      {22,   ITM_1ONX,          ITM_YX,            ITM_HASH_JM,       ITM_NUMBER_SIGN,   ITM_B,             ITM_b,             ITM_BETA,          ITM_REG_B          },
      {23,   ITM_SQUAREROOTX,   ITM_SQUARE,        ITM_ms,            ITM_ROOT_SIGN,     ITM_C,             ITM_c,             ITM_CHI,           ITM_REG_C          },
      {24,   ITM_LOG10,         ITM_10x,           ITM_dotD,          ITM_NULL,          ITM_D,             ITM_d,             ITM_DELTA,         ITM_REG_D          },
      {25,   ITM_LN,            ITM_EXP,           ITM_toREC2,        ITM_NULL,          ITM_E,             ITM_e,             ITM_EPSILON,       ITM_NULL           },
      {26,   ITM_XEQ,           ITM_GTO,           ITM_toPOL2,        ITM_NULL,          ITM_F,             ITM_f,             ITM_DIGAMMA,       ITM_alpha          },

      {31,   ITM_STO,           KEY_COMPLEX,       ITM_MAGNITUDE,     ITM_NULL,          ITM_G,             ITM_g,             ITM_GAMMA,         ITM_NULL           },
      {32,   ITM_RCL,           ITM_PC,            ITM_ANGLE,         ITM_NULL,          ITM_H,             ITM_h,             ITM_ETA,           ITM_HEX            },
      {33,   ITM_Rdown,         ITM_CONSTpi,       ITM_XTHROOT,       ITM_NULL,          ITM_I,             ITM_i,             ITM_IOTA,          ITM_REG_I          },
      {34,   ITM_sin,           ITM_arcsin,        ITM_CC,            ITM_NULL,          ITM_J,             ITM_j,             ITM_THETA,         ITM_REG_J          },
      {35,   ITM_cos,           ITM_arccos,        ITM_LBL,           ITM_NULL,          ITM_K,             ITM_k,             ITM_KAPPA,         ITM_REG_K          },
      {36,   ITM_tan,           ITM_arctan,        ITM_RTN,           ITM_NULL,          ITM_L,             ITM_l,             ITM_LAMBDA,        ITM_REG_L          },

      {41,   ITM_ENTER,         ITM_AIM,           -MNU_CPX,          ITM_ENTER,         ITM_ENTER,         ITM_XEDIT,         ITM_NULL,          ITM_ENTER          },
      {42,   ITM_XexY,          ITM_LASTX,         ITM_Rup,           ITM_ex,            ITM_M,             ITM_m,             ITM_MU,            ITM_NULL           },
      {43,   ITM_CHS,           -MNU_MODE,         -MNU_STK,          ITM_PLUS_MINUS,    ITM_N,             ITM_n,             ITM_NU,            ITM_NULL           },
      {44,   ITM_EXPONENT,      -MNU_DISP,         -MNU_EXP,          ITM_NULL,          ITM_O,             ITM_o,             ITM_OMICRON,       ITM_NULL           },
      {45,   ITM_BACKSPACE,     -MNU_CLR,          ITM_UNDO,          ITM_BACKSPACE,     ITM_BACKSPACE,     ITM_CLA,           ITM_NULL,          ITM_BACKSPACE      },

      {51,   ITM_UP1,           ITM_BST,           ITM_RBR,           ITM_UP1,           ITM_UP1,           CHR_caseUP,        ITM_UP_ARROW,      ITM_UP1            },
      {52,   ITM_7,             -MNU_EQN,          -MNU_INFO,         ITM_7,             ITM_P,             ITM_p,             ITM_PI,            ITM_7              },
      {53,   ITM_8,             -MNU_ADV,          -MNU_CONST,        ITM_8,             ITM_Q,             ITM_q,             ITM_QOPPA,         ITM_8              },
      {54,   ITM_9,             -MNU_MATX,         -MNU_XFN,          ITM_9,             ITM_R,             ITM_r,             ITM_RHO,           ITM_9              },
      {55,   ITM_DIV,           -MNU_STAT,         -MNU_SUMS,         ITM_OBELUS,        ITM_S,             ITM_s,             ITM_SIGMA,         ITM_DIV            },

      {61,   ITM_DOWN1,         ITM_SST,           ITM_FLGSV,         ITM_DOWN1,         ITM_DOWN1,         CHR_caseDN,        ITM_DOWN_ARROW,    ITM_DOWN1          },
      {62,   ITM_4,             -MNU_BASE,         -MNU_FIN,          ITM_4,             ITM_T,             ITM_t,             ITM_TAU,           ITM_4              },
      {63,   ITM_5,             -MNU_ANGLECONV,    -MNU_UNITCONV,     ITM_5,             ITM_U,             ITM_u,             ITM_PHI,           ITM_5              },
      {64,   ITM_6,             -MNU_FLAGS,        -MNU_BITS,         ITM_6,             ITM_V,             ITM_v,             ITM_PSI,           ITM_6              },
      {65,   ITM_MULT,          -MNU_PROB,         -MNU_INTS,         ITM_CROSS,         ITM_W,             ITM_w,             ITM_OMEGA,         ITM_MULT           },

      {71,   KEY_fg,            ITM_NULL,          ITM_NULL,          KEY_fg,            KEY_fg,            ITM_NULL,          ITM_NULL,          KEY_fg             },
      {72,   ITM_1,             ITM_ASSIGN,        -MNU_ASN,          ITM_1,             ITM_X,             ITM_x,             ITM_XI,            ITM_1              },
      {73,   ITM_2,             ITM_USERMODE,      -MNU_LOOP,         ITM_2,             ITM_Y,             ITM_y,             ITM_UPSILON,       ITM_2              },
      {74,   ITM_3,             -MNU_PFN,          -MNU_TEST,         ITM_3,             ITM_Z,             ITM_z,             ITM_ZETA,          ITM_3              },
      {75,   ITM_SUB,           -MNU_IO,           -MNU_ALPHAFN,      ITM_MINUS,         ITM_UNDERSCORE,    ITM_MINUS,         ITM_SAMPI,         ITM_SUB            },

      {81,   ITM_EXIT1,         ITM_OFF,           ITM_PRN,           ITM_EXIT1,         ITM_EXIT1,         ITM_OFF,           ITM_PRINTER,       ITM_EXIT1          },
      {82,   ITM_0,             -MNU_HOME,         ITM_TIMER,         ITM_0,             ITM_COLON,         ITM_0,             -MNU_ALPHA,        ITM_0              },
      {83,   ITM_PERIOD,        ITM_SHOW,          ITM_VIEW,          ITM_PERIOD,        ITM_COMMA,         ITM_PERIOD,        -MNU_ALPHADOT,     ITM_PERIOD         },
      {84,   ITM_RS,            ITM_PR,            -MNU_CLK,          ITM_NULL,          ITM_QUESTION_MARK, ITM_SLASH,         -MNU_ALPHAMATH,    ITM_NULL           },
      {85,   ITM_ADD,           -MNU_CATALOG,      -MNU_PARTS,        ITM_PLUS,          ITM_SPACE,         ITM_PLUS,          -MNU_ALPHAINTL,    ITM_ADD            }
  //keyID,   primary,           fShifted,          gShifted,          keyLblAim,         primaryAim,        fShiftedAim,       gShiftedAim,       primaryTam         
#endif //JM END OF LAYOUT 2 DM42 STRICT.

};



void fnAssign(uint16_t mode) {
  if(mode) {
    createMenu(aimBuffer);
    aimBuffer[0] = 0;
  }
  else {
    previousCalcMode = calcMode;
    calcMode = CM_ASSIGN;
    itemToBeAssigned = 0;
    updateAssignTamBuffer();
  }
}



void updateAssignTamBuffer(void) {
  char *tbPtr = tamBuffer;
  tbPtr = stpcpy(tbPtr, "ASSIGN ");

  if(itemToBeAssigned == 0) {
    tbPtr = stpcpy(tbPtr, "_");
  }
  else if(itemToBeAssigned >= ASSIGN_LABELS) {
    uint8_t *lblPtr = labelList[itemToBeAssigned - ASSIGN_LABELS].labelPointer;
    uint32_t count = *(lblPtr++);
    for(uint32_t i = 0; i < count; ++i) {
      *(tbPtr++) = *(lblPtr++);
    }
  }
  else if(itemToBeAssigned >= ASSIGN_RESERVED_VARIABLES) {
    tbPtr = stpcpy(tbPtr, (char *)allReservedVariables[itemToBeAssigned - ASSIGN_RESERVED_VARIABLES].reservedVariableName + 1);
  }
  else if(itemToBeAssigned >= ASSIGN_NAMED_VARIABLES) {
    tbPtr = stpcpy(tbPtr, (char *)allNamedVariables[itemToBeAssigned - ASSIGN_NAMED_VARIABLES].variableName + 1);
  }
  else if(itemToBeAssigned <= ASSIGN_USER_MENU) {
    tbPtr = stpcpy(tbPtr, userMenus[-(itemToBeAssigned - ASSIGN_USER_MENU)].menuName);
  }
  else if(itemToBeAssigned < 0) {
    tbPtr = stpcpy(tbPtr, indexOfItems[-itemToBeAssigned].itemCatalogName);
  }
  else if(indexOfItems[itemToBeAssigned].itemCatalogName[0] == 0) {
    tbPtr = stpcpy(tbPtr, indexOfItems[itemToBeAssigned].itemSoftmenuName);
  }
  else if(itemToBeAssigned == ITM_ENTER) {
    tbPtr = stpcpy(tbPtr, "NULL");
  }
  else {
    tbPtr = stpcpy(tbPtr, indexOfItems[itemToBeAssigned].itemCatalogName);
  }

  tbPtr = stpcpy(tbPtr, " ");
  if(itemToBeAssigned != 0 && shiftF) {
    tbPtr = stpcpy(tbPtr, STD_SUP_f STD_CURSOR);
  }
  else if(itemToBeAssigned != 0 && shiftG) {
    tbPtr = stpcpy(tbPtr, STD_SUP_g STD_CURSOR);
  }
  else {
    tbPtr = stpcpy(tbPtr, "_");
  }
}



static void _assignItem(userMenuItem_t *menuItem) {
  const uint8_t *lblPtr = NULL;
  uint32_t l = 0;
  if(itemToBeAssigned >= ASSIGN_LABELS) {
    lblPtr                    = labelList[itemToBeAssigned - ASSIGN_LABELS].labelPointer;
    menuItem->item            = ITM_XEQ;
  }
  else if(itemToBeAssigned >= ASSIGN_RESERVED_VARIABLES) {
    lblPtr                    = allReservedVariables[itemToBeAssigned - ASSIGN_RESERVED_VARIABLES].reservedVariableName;
    menuItem->item            = ITM_RCL;
  }
  else if(itemToBeAssigned >= ASSIGN_NAMED_VARIABLES) {
    lblPtr                    = allNamedVariables[itemToBeAssigned - ASSIGN_NAMED_VARIABLES].variableName;
    menuItem->item            = ITM_RCL;
  }
  else if(itemToBeAssigned <= ASSIGN_USER_MENU) {
    lblPtr                    = (uint8_t *)userMenus[-(itemToBeAssigned - ASSIGN_USER_MENU)].menuName;
    menuItem->item            = -MNU_DYNAMIC;
    xcopy(menuItem->argumentName, (char *)lblPtr, stringByteLength((char *)lblPtr));
    lblPtr                    = NULL;
  }
  else if(itemToBeAssigned == ITM_ENTER) {
    menuItem->item            = ITM_NULL;
    menuItem->argumentName[0] = 0;
  }
  else {
    menuItem->item            = itemToBeAssigned;
    menuItem->argumentName[0] = 0;
  }
  if(lblPtr) {
    l = (uint32_t)(*(lblPtr++));
    xcopy(menuItem->argumentName, (char *)lblPtr, l);
    menuItem->argumentName[l] = 0;
  }
}

void assignToMyMenu(uint16_t position) {
  if(position < 18) {
    _assignItem(&userMenuItems[position]);
  }
  cachedDynamicMenu = 0;
  refreshScreen();
}

void assignToMyAlpha(uint16_t position) {
  if(position < 18) {
    _assignItem(&userAlphaItems[position]);
  }
  cachedDynamicMenu = 0;
  refreshScreen();
}

void assignToUserMenu(uint16_t position) {
  if(position < 18) {
    _assignItem(&userMenus[currentUserMenu].menuItem[position]);
  }
  cachedDynamicMenu = 0;
  refreshScreen();
}

void assignToKey(const char *data) {
  int keyCode = (*data - '0')*10 + *(data+1) - '0';
  calcKey_t *key = kbd_usr + keyCode;
  userMenuItem_t tmpMenuItem;
  int keyStateCode = ((previousCalcMode) == CM_AIM ? 3 : 0) + (shiftG ? 2 : shiftF ? 1 : 0);

  _assignItem(&tmpMenuItem);
  if(tmpMenuItem.item == ITM_NULL) {
    const calcKey_t *stdKey = kbd_std + keyCode;
    switch(keyStateCode) {
      case 5: key->gShiftedAim = stdKey->gShiftedAim; break;
      case 4: key->fShiftedAim = stdKey->fShiftedAim; break;
      case 3: key->primaryAim  = stdKey->primaryAim;  break;
      case 2: key->gShifted    = stdKey->gShifted;    break;
      case 1: key->fShifted    = stdKey->fShifted;    break;
      case 0: key->primary     = stdKey->primary;     break;
    }
  }
  else {
    switch(keyStateCode) {
      case 5: key->gShiftedAim = tmpMenuItem.item; break;
      case 4: key->fShiftedAim = tmpMenuItem.item; break;
      case 3: key->primaryAim  = tmpMenuItem.item; break;
      case 2: key->gShifted    = tmpMenuItem.item; break;
      case 1: key->fShifted    = tmpMenuItem.item; break;
      case 0: key->primary     = tmpMenuItem.item; break;
    }
  }

  setUserKeyArgument(keyCode * 6 + keyStateCode, tmpMenuItem.argumentName);
}



void setUserKeyArgument(uint16_t position, const char *name) {
  char *userKeyLabelPtr1 = (char *)getNthString((uint8_t *)userKeyLabel, position);
  char *userKeyLabelPtr2 = (char *)getNthString((uint8_t *)userKeyLabel, position + 1);
  char *userKeyLabelPtr3 = (char *)getNthString((uint8_t *)userKeyLabel, 37 * 6);
  uint16_t newUserKeyLabelSize = userKeyLabelSize - stringByteLength(userKeyLabelPtr1) + stringByteLength(name);
  char *newUserKeyLabel = allocWp43s(TO_BLOCKS(newUserKeyLabelSize));
  char *newUserKeyLabelPtr = newUserKeyLabel;

  xcopy(newUserKeyLabelPtr, userKeyLabel, (int)(userKeyLabelPtr1 - userKeyLabel));
  newUserKeyLabelPtr += (int)(userKeyLabelPtr1 - userKeyLabel);
  xcopy(newUserKeyLabelPtr, name, stringByteLength(name));
  newUserKeyLabelPtr += stringByteLength(name);
  *(newUserKeyLabelPtr++) = 0;
  xcopy(newUserKeyLabelPtr, userKeyLabelPtr2, (int)(userKeyLabelPtr3 - userKeyLabelPtr2));
  newUserKeyLabelPtr += (int)(userKeyLabelPtr3 - userKeyLabelPtr2);
  *(newUserKeyLabelPtr++) = 0;

  freeWp43s(userKeyLabel, TO_BLOCKS(userKeyLabelSize));
  userKeyLabel = newUserKeyLabel;
  userKeyLabelSize = newUserKeyLabelSize;
}



void createMenu(const char *name) {
  if(validateName(name)) {
    bool_t alreadyInUse = false;

    for(uint32_t i = 0; softmenu[i].menuItem < 0; ++i) {
      if(compareString(name, indexOfItems[-softmenu[i].menuItem].itemCatalogName, CMP_BINARY) == 0) {
        alreadyInUse = true;
      }
    }
    for(uint32_t i = 0; i < numberOfUserMenus; ++i) {
      if(compareString(name, userMenus[i].menuName, CMP_BINARY) == 0) {
        alreadyInUse = true;
      }
    }

    if(alreadyInUse) {
      displayCalcErrorMessage(ERROR_ENTER_NEW_NAME, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnAssign:", "the menu", name, "already exists");
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      if(numberOfUserMenus == 0) {
        userMenus = allocWp43s(TO_BLOCKS(sizeof(userMenu_t)));
      }
      else {
        userMenus = reallocWp43s(userMenus, TO_BLOCKS(sizeof(userMenu_t)) * numberOfUserMenus, TO_BLOCKS(sizeof(userMenu_t)) * (numberOfUserMenus + 1));
      }
      memset(userMenus + numberOfUserMenus, 0, sizeof(userMenu_t));
      xcopy(userMenus[numberOfUserMenus].menuName, name, stringByteLength(name));
      ++numberOfUserMenus;
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_NAME, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function fnAssign:", "the menu", name, "does not follow the naming convention");
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}
