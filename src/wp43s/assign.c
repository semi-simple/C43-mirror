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

#include "wp43s.h"

//This variable is to store in flash memory
const calcKey_t kbd_std[37] = {
//keyId primary        fShifted         gShifted         keyLblAim        primaryAim         fShiftedAim      gShiftedAim    primaryTam
 {21,   ITM_1ONX,      ITM_TGLFRT,     -MNU_ALPHAFN,     ITM_NULL,        CHR_A,            -MNU_ALPHAINTL,   CHR_ALPHA,     ITM_ST_A     },
 {22,   ITM_YX,        ITM_toINT,      -MNU_EXP,         ITM_NUMBER_SIGN, CHR_B,             ITM_NUMBER_SIGN, CHR_BETA,      ITM_ST_B     },
 {23,  -MNU_TRI,       ITM_DMS,         ITM_pi,          ITM_NULL,        CHR_C,             ITM_NULL,        CHR_GAMMA,     ITM_ST_C     },
 {24,   ITM_LN,        ITM_dotD,        ITM_LOG10,       ITM_NULL,        CHR_D,             ITM_NULL,        CHR_DELTA,     ITM_ST_D     }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case D for decimal base
 {25,   ITM_EXP,       ITM_toHMS,       ITM_10x,         ITM_NULL,        CHR_E,             ITM_NULL,        CHR_EPSILON,   ITM_NULL     },
 {26,   ITM_SQUARE,    ITM_AIM,         ITM_SQUAREROOTX, ITM_CHECK_MARK,  CHR_F,             ITM_CHECK_MARK,  CHR_PHI,       CHR_alpha    },

 {31,   ITM_STO,       ITM_ASSIGN,      ITM_SAVE,        ITM_NULL,        CHR_G,             ITM_ASSIGN,      CHR_GAMMA,     ITM_NULL     },
 {32,   ITM_RCL,       ITM_RBR,         ITM_VIEW,        ITM_NULL,        CHR_H,             ITM_RBR,         CHR_CHI,       ITM_HEX      }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case H for hexadecimal base
 {33,   ITM_Rdown,     ITM_Rup,        -MNU_CPX,         ITM_NULL,        CHR_I,             CHR_DOWN_ARROW,  CHR_IOTA,      ITM_REGI     },
 {34,   ITM_CC,        ITM_MAGNITUDE,   ITM_ANGLE,       ITM_NULL,        CHR_J,             ITM_NULL,        CHR_ETA,       ITM_REGJ     },
 {35,   ITM_f,         ITM_NULL,        ITM_SCRDMP,      ITM_f,           ITM_f,             ITM_NULL,        ITM_SCRDMP,    ITM_f        },
 {36,   ITM_g,         ITM_USERMODE,    ITM_NULL,        ITM_g,           ITM_g,             ITM_USERMODE,    ITM_NULL,      ITM_g        },

 {41,   ITM_ENTER,     ITM_STATUS,      ITM_DROP,        ITM_ENTER,       ITM_ENTER,         ITM_STATUS,      ITM_DROP,      ITM_ENTER    },
 {42,   ITM_XexY,      ITM_FILL,       -MNU_STK,         ITM_ex,          CHR_K,             ITM_ex,          CHR_KAPPA,     ITM_REGK     },
 {43,   ITM_CHS,       ITM_DELTAPC,    -MNU_FIN,         ITM_PLUS_MINUS,  CHR_L,             ITM_PLUS_MINUS,  CHR_LAMBDA,    ITM_REGL     },
 {44,   ITM_EXPONENT,  ITM_SHOW,       -MNU_DSP,         ITM_NULL,        CHR_M,             CHR_UP_ARROW,    CHR_MU,        ITM_NULL     },
 {45,   ITM_BACKSPACE, ITM_UNDO,       -MNU_CLR,         ITM_BACKSPACE,   ITM_BACKSPACE,     ITM_UNDO,       -MNU_CLR,       ITM_BACKSPACE},

 {51,   ITM_DIV,       ITM_RMD,         ITM_MOD,         ITM_SLASH,       CHR_N,             ITM_SLASH,       CHR_NU,        ITM_DIV      },
 {52,   ITM_7,        -MNU_CONST,       ITM_NULL,        ITM_7,           CHR_O,             ITM_7,           CHR_OMEGA,     ITM_7        },
 {53,   ITM_8,         ITM_NULL,        ITM_NULL,        ITM_8,           CHR_P,             ITM_8,           CHR_PI,        ITM_8        },
 {54,   ITM_9,         ITM_NULL,        ITM_RTN,         ITM_9,           CHR_Q,             ITM_9,           ITM_RTN,       ITM_9        },
 {55,   ITM_XEQ,       ITM_GTO,         ITM_LBL,         ITM_NULL,        ITM_NULL,          ITM_NULL,        ITM_NULL,      ITM_NULL     },

 {61,   ITM_MULT,      ITM_XFACT,      -MNU_PROB,        ITM_CROSS,       CHR_R,             ITM_PROD_SIGN,   CHR_RHO,       ITM_MULT     },
 {62,   ITM_4,        -MNU_STAT,       -MNU_SUMS,        ITM_4,           CHR_S,             ITM_4,           CHR_SIGMA,     ITM_4        },
 {63,   ITM_5,         ITM_toREC,       ITM_toPOL,       ITM_5,           CHR_T,             ITM_5,           CHR_TAU,       ITM_5        },
 {64,   ITM_6,        -MNU_UNITCONV,   -MNU_ANGLECONV,   ITM_6,           CHR_U,             ITM_6,           CHR_THETA,     ITM_6        },
 {65,   ITM_UP,        ITM_BST,        -MNU_FLAGS,       ITM_UP,          ITM_UP,            ITM_BST,        -MNU_FLAGS,     ITM_UP       },

 {71,   ITM_SUB,      -MNU_BITS,       -MNU_INTS,        ITM_MINUS,       CHR_V,             ITM_MINUS,      -MNU_ALPHAMATH, ITM_SUB      },
 {72,   ITM_1,        -MNU_ADV,        -MNU_EQN,         ITM_1,           CHR_W,             ITM_1,           CHR_PSI,       ITM_1        },
 {73,   ITM_2,        -MNU_MATX,       -MNU_XFN,         ITM_2,           CHR_X,             ITM_2,           CHR_XI,        ITM_2        },
 {74,   ITM_3,         ITM_TIMER,      -MNU_CLK,         ITM_3,           CHR_Y,             ITM_3,           CHR_UPSILON,   ITM_3        },
 {75,   ITM_DOWN,      ITM_SST,        -MNU_MODE,        ITM_DOWN,        ITM_DOWN,          ITM_SST,        -MNU_MODE,      ITM_DOWN     },

 {81,   ITM_ADD,      -MNU_IO,         -MNU_PRINT,       ITM_PLUS,        CHR_Z,             ITM_PLUS,        CHR_ZETA,      ITM_ADD      },
 {82,   ITM_0,        -MNU_LOOP,       -MNU_TEST,        ITM_0,           CHR_QUESTION_MARK, ITM_0,           CHR_PRINTER,   ITM_0        },
 {83,   ITM_PERIOD,   -MNU_PARTS,      -MNU_INFO,        ITM_PERIOD,      CHR_COMMA,         ITM_PERIOD,     -MNU_ALPHADOT,  ITM_PERIOD   },
 {84,   ITM_RS,        ITM_PR,         -MNU_PFN,         ITM_NULL,        CHR_SPACE,         ITM_NULL,        ITM_NULL,      ITM_NULL     },
 {85,   ITM_EXIT,     -MNU_CATALOG,     ITM_OFF,         ITM_EXIT,        ITM_EXIT,         -MNU_CATALOG,     ITM_OFF,       ITM_EXIT     }
//keyId primary        fShifted         gShifted         keyLblAim        primaryAim         fShiftedAim      gShiftedAim    primaryTam
};
