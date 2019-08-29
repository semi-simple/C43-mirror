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
// ITM_ASSIGN  KEY_USERMODE,   -MNU_MODE,  -MNU_CATALOG  -MNU_FLAGS    -MNU_ALPHAMATH    -MNU_ALPHAINTL   // JMTEMP
//This variable is to store in flash memory
const calcKey_t kbd_std[37] = {

#ifdef JM_LAYOUT_1  //JM LAYOUT 1. FINAL.
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
 {21,   ITM_SIGMAPLUS,    ITM_PROFRC,      ITM_IMPFRC,   ITM_NULL,       CHR_A,             ITM_NULL,        CHR_ALPHA,     ITM_ST_A     }, //JM2 changed top line primary to DM42 keyboard
 {22,   ITM_1X,           ITM_YX,          ITM_toINT,    ITM_NULL,       CHR_B,             ITM_NULL,        CHR_BETA,      ITM_ST_B     }, //JM2 changed top line primary to DM42 keyboard
 {23,   ITM_SQUAREROOTX,  ITM_SQUARE,      ITM_DMS,      ITM_NULL,       CHR_C,             ITM_NULL,        CHR_GAMMA,     ITM_ST_C     }, //JM2 changed top line primary to DM42 keyboard
 {24,   ITM_LOG10,        ITM_10x,         KEY_dotD,     ITM_NULL,       CHR_D,             ITM_NULL,        CHR_DELTA,     ITM_ST_D     }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case D for decimal base; //JM2 changed top line primary to DM42 keyboard
 {25,   ITM_LN,           ITM_EX,          ITM_toHMS,    ITM_NULL,       CHR_E,             ITM_NULL,        CHR_EPSILON,   ITM_NULL     }, //JM2 changed top line primary to DM42 keyboard
 {26,   ITM_XEQ,          ITM_GTO,         ITM_LBL,      ITM_NULL,       CHR_F,             ITM_NULL,        CHR_PHI,       CHR_alpha    }, //JM2 changed top line primary to DM42 keyboard; moved GTO & LBL too

 {31,   ITM_STO,          ITM_MAGNITUDE,   ITM_ANGLE,    ITM_NULL,       CHR_G,             ITM_NULL,        CHR_GAMMA,     ITM_NULL     }, //JM
 {32,   ITM_RCL,          ITM_PC,          ITM_DELTAPC,  ITM_NULL,       CHR_H,             ITM_NULL,        CHR_CHI,       ITM_HEX      }, //JM
 {33,   ITM_Rdown,        ITM_pi,          ITM_Rup,      ITM_NULL,       CHR_I,             CHR_DOWN_ARROW,  CHR_IOTA,      ITM_REGI     }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case H for hexadecimal base
 {34,   ITM_sin,          ITM_arcsin,      ITM_toREC,    ITM_NULL,       CHR_J,             ITM_NULL,        CHR_ETA,       ITM_REGJ     }, //JM3 adding SIN/COS/TAN according to DM42
 {35,   ITM_cos,          ITM_arccos,      ITM_toPOL,    ITM_NULL,       CHR_K,             ITM_NULL,        CHR_KAPPA,     ITM_REGK     }, //JM3 adding SIN/COS/TAN according to DM42
 {36,   ITM_tan,          ITM_arctan,      ITM_RTN,      ITM_NULL,       CHR_L,             ITM_NULL,        CHR_LAMBDA,    ITM_REGL     }, //JM3 adding SIN/COS/TAN according to DM42
       
 {41,   ITM_ENTER,        KEY_CC,          ITM_DROP,     ITM_ENTER,      ITM_ENTER,         ITM_NULL,        ITM_NULL,      ITM_ENTER    }, //JM
 {42,   ITM_XexY,         ITM_LASTX,      -MNU_CPX,      CHR_ex,         CHR_M,             CHR_ex,          CHR_MU,        ITM_NULL     }, //JM
 {43,   ITM_CHS,         -MNU_MODE,       -MNU_EXP,      CHR_PLUS_MINUS, CHR_N,             CHR_PLUS_MINUS,  CHR_NU,        ITM_NULL     }, //JM
 {44,   ITM_EXPONENT,    -MNU_DISP,       -MNU_FIN,      ITM_NULL,       CHR_O,             CHR_UP_ARROW,    CHR_OMEGA,     ITM_NULL     }, //JM
 {45,   KEY_BACKSPACE,   -MNU_CLR,         KEY_UNDO,     KEY_BACKSPACE,  KEY_BACKSPACE,     KEY_UNDO,       -MNU_CLR,       KEY_BACKSPACE}, //JM
                 
 {51,   KEY_UP,           KEY_BST,         ITM_RBR,      KEY_UP,         KEY_UP,            KEY_BST,         ITM_NULL,      KEY_UP       }, //JM swap UP/MULT; Shifted up to match DM42 keyboard
 {52,   CHR_7,            ITM_VIEW,       -MNU_HOME,     CHR_7,          CHR_P,             CHR_7,           CHR_PI,        CHR_7        }, //JM2 Add TRI to g(7)
 {53,   CHR_8,           -MNU_CNST,       -MNU_INFO,     CHR_8,          CHR_Q,             CHR_8,           ITM_NULL,      CHR_8        }, //JM2 Move Alpha controls here, from BTN26
 {54,   CHR_9,           -MNU_MATX,       -MNU_XFN,      CHR_9,          CHR_R,             CHR_9,           CHR_RHO,       CHR_9        }, //JM-keyboard re-shuffle
 {55,   ITM_DIV,         -MNU_STAT,       -MNU_SUMS,     CHR_DIVIDE,     CHR_S,             CHR_DIVIDE,      CHR_SIGMA,     ITM_DIV      }, //JM swap XEQ/DIV

 {61,   KEY_DOWN,         KEY_SST,         ITM_STATUS,   KEY_DOWN,       KEY_DOWN,          KEY_SST,         ITM_NULL,      KEY_DOWN     }, //JM swap DN/MIN; Shifted up to match DM42 keyboard
 {62,   CHR_4,            ITM_AIM,        -MNU_CLK,      CHR_4,          CHR_T,             CHR_4,           CHR_TAU,       CHR_4        }, //JM  roll RSTU
 {63,   CHR_5,           -MNU_ANGLECONV,  -MNU_UNITCONV, CHR_5,          CHR_U,             CHR_5,           CHR_THETA,     CHR_5        }, //JM  roll RSTU
 {64,   CHR_6,           -MNU_FLAGS,      -MNU_BITS,     CHR_6,          CHR_V,             CHR_6,           ITM_NULL,      CHR_6        }, //JM  roll RSTU
 {65,   ITM_MULT,        -MNU_PROB,       -MNU_INTS,     CHR_CROSS,      CHR_W,             CHR_CROSS,       CHR_PSI,       ITM_MULT     }, //JM  swap UP/MULT & roll

 {71,   KEY_f,            ITM_NULL,        ITM_NULL,     KEY_f,          KEY_f,             ITM_NULL,        ITM_NULL,      KEY_f        }, //JM swap XEQ/DIV; ; Shifted down to match DM42 keyboard; Moved GTO & LBL away; DM42 f key
 {72,   CHR_1,            ITM_ASSIGN,      ITM_SAVE,     CHR_1,          CHR_X,             CHR_1,           CHR_XI,        CHR_1        }, //JM roll
 {73,   CHR_2,            KEY_USERMODE,   -MNU_LOOP,     CHR_2,          CHR_Y,             CHR_2,           CHR_UPSILON,   CHR_2        }, //JM roll
 {74,   CHR_3,           -MNU_PARTS,      -MNU_EQN,     CHR_3,          CHR_Z,             CHR_3,           CHR_ZETA,      CHR_3        }, //JM roll
 {75,   ITM_SUB,          -MNU_STK,       -MNU_ADV,      CHR_MINUS,      CHR_UNDERSCORE,    CHR_MINUS,       CHR_UNDERSCORE,ITM_SUB      }, //JM swap DN/MIN & roll

 {81,   KEY_EXIT,         ITM_OFF,         KEY_PRTX,     KEY_EXIT,       KEY_EXIT,          ITM_OFF,         ITM_NULL,      KEY_EXIT     }, //JM swap EXIT/ADD
 {82,   CHR_0,            ITM_TIMER,      -MNU_ALPHAFN,  CHR_0,          CHR_COLON,         CHR_0,           CHR_COLON,     CHR_0        }, //JM
 {83,   CHR_PERIOD,       ITM_SHOW,       -MNU_TEST,     CHR_PERIOD,     CHR_COMMA,         CHR_PERIOD,     -MNU_ALPHADOT,  CHR_PERIOD   }, //JM
 {84,   ITM_RS,           ITM_PR,         -MNU_PFN,      ITM_NULL,       CHR_QUESTION_MARK, ITM_NULL,        ITM_NULL,      ITM_NULL     }, //JM swap
 {85,   ITM_ADD,         -MNU_CATALOG,    -MNU_IO,       CHR_PLUS,       CHR_SPACE,         CHR_PLUS,        ITM_NULL,      ITM_ADD      }  //JM swap EXIT/ADD
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
#endif //JM END OF LAYOUT 1.

 #ifdef JM_LAYOUT_1A  //JM LAYOUT 1A. CHALLENGE.
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
 {21,   ITM_SIGMAPLUS,    ITM_PROFRC,      ITM_IMPFRC,   ITM_NULL,       CHR_A,             ITM_NULL,        CHR_ALPHA,     ITM_ST_A     }, //JM2 changed top line primary to DM42 keyboard
 {22,   ITM_1X,           ITM_YX,          ITM_toINT,    ITM_NULL,       CHR_B,             ITM_NULL,        CHR_BETA,      ITM_ST_B     }, //JM2 changed top line primary to DM42 keyboard
 {23,   ITM_SQUAREROOTX,  ITM_SQUARE,      ITM_DMS,      ITM_NULL,       CHR_C,             ITM_NULL,        CHR_GAMMA,     ITM_ST_C     }, //JM2 changed top line primary to DM42 keyboard
 {24,   ITM_LOG10,        ITM_10x,         KEY_dotD,     ITM_NULL,       CHR_D,             ITM_NULL,        CHR_DELTA,     ITM_ST_D     }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case D for decimal base; //JM2 changed top line primary to DM42 keyboard
 {25,   ITM_LN,           ITM_EX,          ITM_toHMS,    ITM_NULL,       CHR_E,             ITM_NULL,        CHR_EPSILON,   ITM_NULL     }, //JM2 changed top line primary to DM42 keyboard
 {26,   ITM_XEQ,          ITM_GTO,         ITM_LBL,      ITM_NULL,       CHR_F,             ITM_NULL,        CHR_PHI,       CHR_alpha    }, //JM2 changed top line primary to DM42 keyboard; moved GTO & LBL too

 {31,   ITM_STO,          ITM_MAGNITUDE,   ITM_ANGLE,    ITM_NULL,       CHR_G,             ITM_NULL,        CHR_GAMMA,     ITM_NULL     }, //JM
 {32,   ITM_RCL,          ITM_PC,          ITM_DELTAPC,  ITM_NULL,       CHR_H,             ITM_NULL,        CHR_CHI,       ITM_HEX      }, //JM
 {33,   ITM_Rdown,        ITM_pi,          ITM_Rup,      ITM_NULL,       CHR_I,             CHR_DOWN_ARROW,  CHR_IOTA,      ITM_REGI     }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case H for hexadecimal base
 {34,   ITM_sin,          ITM_arcsin,      ITM_toREC,    ITM_NULL,       CHR_J,             ITM_NULL,        CHR_ETA,       ITM_REGJ     }, //JM3 adding SIN/COS/TAN according to DM42
 {35,   ITM_cos,          ITM_arccos,      ITM_toPOL,    ITM_NULL,       CHR_K,             ITM_NULL,        CHR_KAPPA,     ITM_REGK     }, //JM3 adding SIN/COS/TAN according to DM42
 {36,   ITM_tan,          ITM_arctan,      ITM_RTN,      ITM_NULL,       CHR_L,             ITM_NULL,        CHR_LAMBDA,    ITM_REGL     }, //JM3 adding SIN/COS/TAN according to DM42
       
 {41,   ITM_ENTER,        KEY_CC,          ITM_DROP,     ITM_ENTER,      ITM_ENTER,         ITM_NULL,        ITM_NULL,      ITM_ENTER    }, //JM
 {42,   ITM_XexY,         ITM_LASTX,      -MNU_CPX,      CHR_ex,         CHR_M,             CHR_ex,          CHR_MU,        ITM_NULL     }, //JM
 {43,   ITM_CHS,         -MNU_MODE,       -MNU_EXP,      CHR_PLUS_MINUS, CHR_N,             CHR_PLUS_MINUS,  CHR_NU,        ITM_NULL     }, //JM
 {44,   ITM_EXPONENT,    -MNU_DISP,       -MNU_FIN,      ITM_NULL,       CHR_O,             CHR_UP_ARROW,    CHR_OMEGA,     ITM_NULL     }, //JM
 {45,   KEY_BACKSPACE,   -MNU_CLR,         KEY_UNDO,     KEY_BACKSPACE,  KEY_BACKSPACE,     KEY_UNDO,       -MNU_CLR,       KEY_BACKSPACE}, //JM
                  
 {51,   KEY_UP,           KEY_BST,         ITM_RBR,      KEY_UP,         KEY_UP,            KEY_BST,         ITM_NULL,      KEY_UP       }, //JM swap UP/MULT; Shifted up to match DM42 keyboard
 {52,   CHR_7,           -MNU_EQN,        -MNU_HOME,     CHR_7,          CHR_P,             CHR_7,           CHR_PI,        CHR_7        }, //JM2 Add TRI to g(7)
 {53,   CHR_8,           -MNU_ADV,        -MNU_CNST,     CHR_8,          CHR_Q,             CHR_8,           ITM_NULL,      CHR_8        }, //JM2 Move Alpha controls here, from BTN26
 {54,   CHR_9,           -MNU_MATX,       -MNU_XFN,      CHR_9,          CHR_R,             CHR_9,           CHR_RHO,       CHR_9        }, //JM-keyboard re-shuffle
 {55,   ITM_DIV,         -MNU_STAT,       -MNU_SUMS,     CHR_DIVIDE,     CHR_S,             CHR_DIVIDE,        CHR_SIGMA,     ITM_DIV      }, //JM swap XEQ/DIV

 {61,   KEY_DOWN,         KEY_SST,         ITM_STATUS,   KEY_DOWN,       KEY_DOWN,          KEY_SST,         ITM_NULL,      KEY_DOWN     }, //JM swap DN/MIN; Shifted up to match DM42 keyboard
 {62,   CHR_4,            ITM_AIM,        -MNU_CLK,      CHR_4,          CHR_T,             CHR_4,           CHR_TAU,       CHR_4        }, //JM  roll RSTU
 {63,   CHR_5,           -MNU_ANGLECONV,  -MNU_UNITCONV, CHR_5,          CHR_U,             CHR_5,           CHR_THETA,     CHR_5        }, //JM  roll RSTU
 {64,   CHR_6,           -MNU_FLAGS,      -MNU_BITS,     CHR_6,          CHR_V,             CHR_6,           ITM_NULL,      CHR_6        }, //JM  roll RSTU
 {65,   ITM_MULT,        -MNU_PROB,       -MNU_INTS,     CHR_CROSS,      CHR_W,             CHR_CROSS,       CHR_PSI,       ITM_MULT     }, //JM  swap UP/MULT & roll

 {71,   KEY_f,            ITM_NULL,        ITM_NULL,     KEY_f,          KEY_f,             ITM_NULL,        ITM_NULL,      KEY_f        }, //JM swap XEQ/DIV; ; Shifted down to match DM42 keyboard; Moved GTO & LBL away; DM42 f key
 {72,   CHR_1,            ITM_ASSIGN,      ITM_SAVE,     CHR_1,          CHR_X,             CHR_1,           CHR_XI,        CHR_1        }, //JM roll
 {73,   CHR_2,            KEY_USERMODE,   -MNU_LOOP,     CHR_2,          CHR_Y,             CHR_2,           CHR_UPSILON,   CHR_2        }, //JM roll
 {74,   CHR_3,           -MNU_PARTS,      -MNU_TEST,     CHR_3,          CHR_Z,             CHR_3,           CHR_ZETA,      CHR_3        }, //JM roll
 {75,   ITM_SUB,         -MNU_STK,        -MNU_ALPHAFN,  CHR_MINUS,      CHR_UNDERSCORE,    CHR_MINUS,       CHR_UNDERSCORE,ITM_SUB      }, //JM swap DN/MIN & roll

 {81,   KEY_EXIT,         ITM_OFF,         KEY_PRTX,     KEY_EXIT,       KEY_EXIT,          ITM_OFF,         ITM_NULL,      KEY_EXIT     }, //JM swap EXIT/ADD
 {82,   CHR_0,            ITM_TIMER,       ITM_VIEW,     CHR_0,          CHR_COLON,         CHR_0,           CHR_COLON,     CHR_0        }, //JM
 {83,   CHR_PERIOD,       ITM_SHOW,       -MNU_INFO,     CHR_PERIOD,     CHR_COMMA,         CHR_PERIOD,     -MNU_ALPHADOT,  CHR_PERIOD   }, //JM
 {84,   ITM_RS,           ITM_PR,         -MNU_PFN,      ITM_NULL,       CHR_QUESTION_MARK, ITM_NULL,        ITM_NULL,      ITM_NULL     }, //JM swap
 {85,   ITM_ADD,         -MNU_CATALOG,    -MNU_IO,       CHR_PLUS,       CHR_SPACE,         CHR_PLUS,        ITM_NULL,      ITM_ADD      }  //JM swap EXIT/ADD
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
#endif //JM END OF LAYOUT 1a.


#ifdef JM_LAYOUT_2_DM42_STRICT //JM LAYOUT 2. DM42 STRICT.
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
 {21,   ITM_SIGMAPLUS,    ITM_PROFRC,      ITM_IMPFRC,   ITM_NULL,       CHR_A,             ITM_NULL,        CHR_ALPHA,     ITM_ST_A     }, //JM2 changed top line primary to DM42 keyboard
 {22,   ITM_1X,           ITM_YX,          ITM_toINT,    ITM_NULL,       CHR_B,             ITM_NULL,        CHR_BETA,      ITM_ST_B     }, //JM2 changed top line primary to DM42 keyboard
 {23,   ITM_SQUAREROOTX,  ITM_SQUARE,      ITM_DMS,      ITM_NULL,       CHR_C,             ITM_NULL,        CHR_GAMMA,     ITM_ST_C     }, //JM2 changed top line primary to DM42 keyboard
 {24,   ITM_LOG10,        ITM_10x,         KEY_dotD,     ITM_NULL,       CHR_D,             ITM_NULL,        CHR_DELTA,     ITM_ST_D     }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case D for decimal base; //JM2 changed top line primary to DM42 keyboard
 {25,   ITM_LN,           ITM_EX,          ITM_toHMS,    ITM_NULL,       CHR_E,             ITM_NULL,        CHR_EPSILON,   ITM_NULL     }, //JM2 changed top line primary to DM42 keyboard
 {26,   ITM_XEQ,          ITM_GTO,         ITM_LBL,      ITM_NULL,       CHR_F,             ITM_NULL,        CHR_PHI,       CHR_alpha    }, //JM2 changed top line primary to DM42 keyboard; moved GTO & LBL too

 {31,   ITM_STO,          KEY_CC,          ITM_MAGNITUDE,ITM_NULL,       CHR_G,             ITM_NULL,        CHR_GAMMA,     ITM_NULL     }, //JM
 {32,   ITM_RCL,          ITM_PC,          ITM_DELTAPC,  ITM_NULL,       CHR_H,             ITM_NULL,        CHR_CHI,       ITM_HEX      }, //JM
 {33,   ITM_Rdown,        ITM_pi,          ITM_ANGLE,    ITM_NULL,       CHR_I,             CHR_DOWN_ARROW,  CHR_IOTA,      ITM_REGI     }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case H for hexadecimal base
 {34,   ITM_sin,          ITM_arcsin,      ITM_toREC,    ITM_NULL,       CHR_J,             ITM_NULL,        CHR_ETA,       ITM_REGJ     }, //JM3 adding SIN/COS/TAN according to DM42
 {35,   ITM_cos,          ITM_arccos,      ITM_toPOL,    ITM_NULL,       CHR_K,             ITM_NULL,        CHR_KAPPA,     ITM_REGK     }, //JM3 adding SIN/COS/TAN according to DM42
 {36,   ITM_tan,          ITM_arctan,      ITM_RTN,      ITM_NULL,       CHR_L,             ITM_NULL,        CHR_LAMBDA,    ITM_REGL     }, //JM3 adding SIN/COS/TAN according to DM42
       
 {41,   ITM_ENTER,        ITM_AIM,         ITM_DROP,     ITM_ENTER,      ITM_ENTER,         ITM_NULL,        ITM_NULL,      ITM_ENTER    }, //JM
 {42,   ITM_XexY,         ITM_LASTX,      -MNU_CPX,      CHR_ex,         CHR_M,             CHR_ex,          CHR_MU,        ITM_NULL     }, //JM
 {43,   ITM_CHS,         -MNU_MODE,       -MNU_EXP,      CHR_PLUS_MINUS, CHR_N,             CHR_PLUS_MINUS,  CHR_NU,        ITM_NULL     }, //JM
 {44,   ITM_EXPONENT,    -MNU_DISP,       -MNU_FIN,      ITM_NULL,       CHR_O,             CHR_UP_ARROW,    CHR_OMEGA,     ITM_NULL     }, //JM
 {45,   KEY_BACKSPACE,   -MNU_CLR,         KEY_UNDO,     KEY_BACKSPACE,  KEY_BACKSPACE,     KEY_UNDO,       -MNU_CLR,       KEY_BACKSPACE}, //JM
                 
 {51,   KEY_UP,           KEY_BST,         ITM_RBR,      KEY_UP,         KEY_UP,            KEY_BST,         ITM_NULL,      KEY_UP       }, //JM swap UP/MULT; Shifted up to match DM42 keyboard
 {52,   CHR_7,           -MNU_EQN,        -MNU_INFO,     CHR_7,          CHR_P,             CHR_7,           CHR_PI,        CHR_7        }, //JM2 Add TRI to g(7)
 {53,   CHR_8,           -MNU_ADV,        -MNU_CNST,     CHR_8,          CHR_Q,             CHR_8,           ITM_NULL,      CHR_8        }, //JM2 Move Alpha controls here, from BTN26
 {54,   CHR_9,           -MNU_MATX,       -MNU_XFN,      CHR_9,          CHR_R,             CHR_9,           CHR_RHO,       CHR_9        }, //JM-keyboard re-shuffle
 {55,   ITM_DIV,         -MNU_STAT,       -MNU_SUMS,     CHR_DIVIDE,     CHR_S,             CHR_DIVIDE,      CHR_SIGMA,     ITM_DIV      }, //JM swap XEQ/DIV

 {61,   KEY_DOWN,         KEY_SST,         ITM_STATUS,   KEY_DOWN,       KEY_DOWN,          KEY_SST,         ITM_NULL,      KEY_DOWN     }, //JM swap DN/MIN; Shifted up to match DM42 keyboard
 {62,   CHR_4,           -MNU_BASE,       -MNU_CLK,      CHR_4,          CHR_T,             CHR_4,           CHR_TAU,       CHR_4        }, //JM  roll RSTU
 {63,   CHR_5,           -MNU_ANGLECONV,  -MNU_UNITCONV, CHR_5,          CHR_U,             CHR_5,           CHR_THETA,     CHR_5        }, //JM  roll RSTU
 {64,   CHR_6,           -MNU_FLAGS,      -MNU_BITS,     CHR_6,          CHR_V,             CHR_6,           ITM_NULL,      CHR_6        }, //JM  roll RSTU
 {65,   ITM_MULT,        -MNU_PROB,       -MNU_INTS,     CHR_CROSS,      CHR_W,             CHR_CROSS,       CHR_PSI,       ITM_MULT     }, //JM  swap UP/MULT & roll

 {71,   KEY_f,            ITM_NULL,        ITM_NULL,     KEY_f,          KEY_f,             ITM_NULL,        ITM_NULL,      KEY_f        }, //JM swap XEQ/DIV; ; Shifted down to match DM42 keyboard; Moved GTO & LBL away; DM42 f key
 {72,   CHR_1,            ITM_ASSIGN,      ITM_SAVE,     CHR_1,          CHR_X,             CHR_1,           CHR_XI,        CHR_1        }, //JM roll
 {73,   CHR_2,            KEY_USERMODE,   -MNU_LOOP,     CHR_2,          CHR_Y,             CHR_2,           CHR_UPSILON,   CHR_2        }, //JM roll
 {74,   CHR_3,           -MNU_PFN,        -MNU_TEST,     CHR_3,          CHR_Z,             CHR_3,           CHR_ZETA,      CHR_3        }, //JM roll
 {75,   ITM_SUB,         -MNU_IO,         -MNU_ALPHAFN,  CHR_MINUS,      CHR_UNDERSCORE,    CHR_MINUS,       CHR_UNDERSCORE,ITM_SUB      }, //JM swap DN/MIN & roll

 {81,   KEY_EXIT,         ITM_OFF,         KEY_PRTX,     KEY_EXIT,       KEY_EXIT,          ITM_OFF,         ITM_NULL,      KEY_EXIT     }, //JM swap EXIT/ADD
 {82,   CHR_0,           -MNU_HOME,        ITM_TIMER,    CHR_0,          CHR_COLON,         CHR_0,           CHR_COLON,     CHR_0        }, //JM
 {83,   CHR_PERIOD,       ITM_SHOW,        ITM_VIEW,     CHR_PERIOD,     CHR_COMMA,         CHR_PERIOD,     -MNU_ALPHADOT,  CHR_PERIOD   }, //JM
 {84,   ITM_RS,           ITM_PR,         -MNU_STK,      ITM_NULL,       CHR_QUESTION_MARK, ITM_NULL,        ITM_NULL,      ITM_NULL     }, //JM swap
 {85,   ITM_ADD,         -MNU_CATALOG,    -MNU_PARTS,    CHR_PLUS,       CHR_SPACE,         CHR_PLUS,        ITM_NULL,      ITM_ADD      }  //JM swap EXIT/ADD
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
#endif //JM END OF LAYOUT 2 DM42 STRICT.


};


void fnUserMode(uint16_t unusedParamButMandatory) {
  toggleUserMode();
}
