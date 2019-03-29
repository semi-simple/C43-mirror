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
 * \file softmenus.c List of softmenus and related functions.
 * The numbers refer to the index of items in items.c
 * *         item <     0  ==>  sub menu
 * *     0 < item <  9999  ==>  item with top and bottom line
 * * 10000 < item < 19999  ==>  item without top line
 * * 20000 < item < 29999  ==>  item without bottom line
 * * 30000 < item < 39999  ==>  item without top and bottom line
 ***********************************************/

#include "wp43s.h"

/*      Menu name                  <--------------------------------------------------------------------------- 6 functions -------------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 f shifted functions ----------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 g shifted functions ----------------------------------------------------------------------->  */
const int16_t menu_MyAlpha[]     = { ITM_MA11,                      ITM_MA12,                   ITM_MA13,                 ITM_MA14,              ITM_MA15,                 ITM_MA16,
                                     ITM_MA21,                      ITM_MA22,                   ITM_MA23,                 ITM_MA24,              ITM_MA25,                 ITM_MA26,
                                     ITM_MA31,                      ITM_MA32,                   ITM_MA33,                 ITM_MA34,              ITM_MA35,                 ITM_MA36                      };

const int16_t menu_ADV[]         = { ITM_SOLVE,                     ITM_SLVQ,                   ITM_FQX,                  ITM_PI,                ITM_SIGMA,                -MNU_Sfdx,
                                     ITM_NULL,                      ITM_NULL,                   ITM_FDQX,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };

const int16_t menu_Sfdx[]        = { ITM_NULL,                      ITM_NULL,                   ITM_ACC,                  ITM_DLIM,              ITM_ULIM,                 ITM_INTEGRAL                  };

const int16_t menu_BITS[]        = { ITM_AND,                       ITM_OR,                     ITM_XOR,                  ITM_NOT,               ITM_MASKL,                ITM_MASKR,
                                     ITM_NAND,                      ITM_NOR,                    ITM_XNOR,                 ITM_NULL,              ITM_MIRROR,               ITM_ASR,
                                     ITM_SB,                        ITM_BS,                     ITM_NUMB,                 ITM_FB,                ITM_BC,                   ITM_CB,
                                     ITM_SL,                        ITM_RL,                     ITM_RLC,                  ITM_RRC,               ITM_RR,                   ITM_SR,
                                     ITM_LJ,                        ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_RJ,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL,
                                     ITM_1COMPL,                    ITM_2COMPL,                 ITM_UNSIGN,               ITM_SIGNMT,            ITM_LZON,                 ITM_WSIZE,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_LZOFF,                ITM_NULL                      };

const int16_t menu_CLK[]         = { ITM_DATE,                      ITM_toDATE,                 ITM_DATEto,               ITM_WDAY,              ITM_TIME,                 ITM_XtoDATE,
                                     ITM_JtoD,                      ITM_DtoJ,                   ITM_NULL,                 ITM_DAY,               ITM_MONTH,                ITM_YEAR,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL,
                                     ITM_SETTIM,                    ITM_TDISP,                  ITM_SETDAT,               ITM_DMY,               ITM_YMD,                  ITM_MDY,
                                     ITM_CLK12,                     ITM_CLK24,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_JonG                      };

const int16_t menu_CLR[]         = { ITM_CLSIGMA,                   ITM_CLP,                    ITM_CF,                   ITM_CLMENU,            ITM_CLSTK,                ITM_CLX,
                                     ITM_CLREGS,                    ITM_CLPALL,                 ITM_CLFALL,               ITM_CLCVAR,            ITM_CLLCD,                ITM_CLALL,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_RESET                     };

/*      Menu name                  <--------------------------------------------------------------------------- 6 functions -------------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 f shifted functions ----------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 g shifted functions ----------------------------------------------------------------------->  */
const int16_t menu_CPX[]         = { ITM_DOT,                       ITM_CROSS,                  ITM_UNITV,                ITM_MAGNITUDE,         ITM_CONJ,                 ITM_REexIM,
                                     ITM_NULL,                      ITM_NULL,                   ITM_SIGN,                 ITM_ANGLE,             ITM_RE,                   ITM_IM,
                                     ITM_CPXI,                      ITM_CPXJ,                   ITM_NULL,                 ITM_NULL,              ITM_POLAR,                ITM_RECT                      };

const int16_t menu_DISP[]        = { ITM_FIX,                       ITM_SCI,                    ITM_ENG,                  ITM_ALL,               ITM_ROUNDI,               ITM_ROUND,
                                     ITM_SDL,                       ITM_SDR,                    ITM_NULL,                 ITM_NULL,              ITM_RDP,                  ITM_RSD,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL,
                                     ITM_SCIOVR,                    ITM_ENGOVR,                 ITM_MULTCR,               ITM_MULTDOT,           ITM_RDXPER,               ITM_RDXCOM,
                                     ITM_GAP,                       ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_DSTACK,
                                     ITM_SETCHN,                    ITM_SETEUR,                 ITM_SETIND,               ITM_SETJPN,            ITM_SETUK,                ITM_SETUSA                    };

const int16_t menu_EQN[]         = { ITM_EQ_EDI,                    ITM_EQ_NEW,                 -MNU_2NDDERIV,            -MNU_1STDERIV,         -MNU_Sf,                  -MNU_Solver,
                                     ITM_EQ_DEL,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };

const int16_t menu_EXP[]         = { ITM_CUBE,                      ITM_XTHROOT,                ITM_LOGXY,                ITM_LOG2,              ITM_2X,                   ITM_SQUARE,
                                     ITM_CUBEROOT,                  ITM_NULL,                   ITM_NULL,                 ITM_LN1X,              ITM_EX1,                  ITM_NULL,
                                     ITM_sinh,                      ITM_arsinh,                 ITM_cosh,                 ITM_arcosh,            ITM_tanh,                 ITM_artanh                    };

const int16_t menu_TRI[]         = { ITM_sin,                       ITM_arcsin,                 ITM_cos,                  ITM_arccos,            ITM_tan,                  ITM_arctan                    };

const int16_t menu_FIN[]         = { ITM_PC,                        ITM_PCMRR,                  ITM_PCT,                  ITM_PCSIGMA,           ITM_PCPMG,                -MNU_TVM                      };

/*      Menu name                  <--------------------------------------------------------------------------- 6 functions -------------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 f shifted functions ----------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 g shifted functions ----------------------------------------------------------------------->  */
const int16_t menu_TVM[]         = { ITM_NPER,                      ITM_IPCA,                   ITM_PERonA,               ITM_PV,                ITM_PMT,                  ITM_FV,
                                     ITM_BEGINP,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_ENDP                      };

const int16_t menu_FLAGS[]       = { ITM_SF,                        ITM_FS,                     ITM_FF,                   ITM_STATUS,            ITM_FC,                   ITM_CF,
                                     ITM_FSS,                       ITM_FSC,                    ITM_FSF,                  ITM_FCF,               ITM_FCS,                  ITM_FCC,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_CLFALL                    };

const int16_t menu_INFO[]        = { ITM_SSIZE,                     ITM_MEM,                    ITM_RMQ,                  ITM_SMODE,             ITM_WSIZEQ,               ITM_KTYP,
                                     ITM_LocRQ,                     ITM_FLASH,                  ITM_ULP,                  ITM_NEIGHB,            ITM_SDIGS,                ITM_BATT,
                                     ITM_WHO,                       ITM_VERS,                   ITM_M_DIMQ,               ITM_PMINFINITY,        ITM_ALPHAPOS,             ITM_ALPHALENG                 };

const int16_t menu_INTS[]        = { CHR_A,                         CHR_B,                      CHR_C,                    CHR_D,                 CHR_E,                    CHR_F,
                                     ITM_IDIV,                      ITM_RMDR,                   ITM_MOD,                  ITM_XMOD,              ITM_FLOOR,                ITM_LCM,
                                     ITM_DBLSLASH,                  ITM_DBLR,                   ITM_DBLCROSS,             ITM_PMOD,              ITM_CEIL,                 ITM_GCD                       };

const int16_t menu_LOOP[]        = { ITM_DSE,                       ITM_DSZ,                    ITM_DSL,                  ITM_ISE,               ITM_ISZ,                  ITM_ISG,
                                     ITM_DEC,                       ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_INC                       };

/*      Menu name                  <--------------------------------------------------------------------------- 6 functions -------------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 f shifted functions ----------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 g shifted functions ----------------------------------------------------------------------->  */
const int16_t menu_MATX[]        = { ITM_M_NEW,                     ITM_M_INV,                  ITM_M_DET,                ITM_M_TRANSP,          -MNU_SIM_EQ,              -MNU_M_EDIT,
                                     ITM_DOT,                       ITM_CROSS,                  ITM_UNITV,                ITM_M_DIM,             ITM_INDEX,                -MNU_M_EDITN,
                                     ITM_ENORM,                     ITM_VANGLE,                 ITM_STOEL,                ITM_RCLEL,             ITM_M_PUT,                ITM_M_GET,
                                     ITM_IPLUS,                     ITM_IMINUS,                 ITM_STOIJ,                ITM_RCLIJ,             ITM_JMINUS,               ITM_JPLUS,
                                     ITM_RSUM,                      ITM_RNORM,                  ITM_M_LU,                 ITM_M_DIMQ,            ITM_NULL,                 ITM_M_R,
                                     ITM_EIGVAL,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_EIGVEC                    };

const int16_t menu_M_SIM_Q[]     = { ITM_MATA,                      ITM_MATB,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_MATX                      };

const int16_t menu_M_EDIT[]      = { CHR_LEFT_ARROW,                CHR_UP_ARROW,               ITM_M_OLD,                ITM_M_GOTO,            CHR_DOWN_ARROW,           CHR_RIGHT_ARROW,
                                     ITM_M_INSR,                    ITM_NULL,                   ITM_M_DELR,               ITM_NULL,              ITM_M_WRAP,               ITM_M_GROW                    };

const int16_t menu_M_EDITN[]     = { CHR_LEFT_ARROW,                CHR_UP_ARROW,               ITM_M_OLD,                ITM_M_GOTO,            CHR_DOWN_ARROW,           CHR_RIGHT_ARROW,
                                     ITM_M_INSR,                    ITM_NULL,                   ITM_M_DELR,               ITM_NULL,              ITM_M_WRAP,               ITM_M_GROW                    };

const int16_t menu_MODE[]        = { ITM_DEG,                       ITM_RAD,                    ITM_GRAD,                 ITM_MULPI,             ITM_RECT,                 ITM_POLAR,
                                     ITM_FAST,                      ITM_SLOW,                   ITM_RM,                   ITM_QUIET,             ITM_REALRES,              ITM_CPXRES,
                                     ITM_DENMAX,                    ITM_DENANY,                 ITM_DENFAC,               ITM_DENFIX,            ITM_SSIZE4,               ITM_SSIZE8,
                                     ITM_1COMPL,                    ITM_2COMPL,                 ITM_UNSIGN,               ITM_SIGNMT,            ITM_LZON,                 ITM_WSIZE,
                                     ITM_SETSIG,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_LZOFF,                ITM_NULL,
#if DMCP_BUILD
                                     ITM_SYSTEM,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL
#endif
                                                                                                                                                                                                         };

const int16_t menu_PARTS[]       = { ITM_IP,                        ITM_FP,                     ITM_MANT,                 ITM_EXPT,              ITM_SIGN,                 ITM_MAGNITUDE,
                                     ITM_ROUNDI,                    ITM_ROUND,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_DECOMP,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_RE,                ITM_IM,                   ITM_ANGLE                     };

/*      Menu name                  <--------------------------------------------------------------------------- 6 functions -------------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 f shifted functions ----------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 g shifted functions ----------------------------------------------------------------------->  */
const int16_t menu_PROB[]        = { -MNU_T,                        -MNU_PHI,                   ITM_COMB,                 ITM_PERM,              -MNU_F,                   -MNU_CHI2,
                                     -MNU_NORML,                    -MNU_LGNRM,                 -MNU_CAUCH,               -MNU_EXPON,            -MNU_LOGIS,               -MNU_WEIBL,
                                     -MNU_BINOM,                    -MNU_GEOM,                  -MNU_HYPER,               -MNU_NBIN,             ITM_NULL,                 -MNU_POISS,
                                     ITM_RAN,                       ITM_SEED,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_GAMMAX                    };

const int16_t menu_t[]           = { ITM_TPX,                       ITM_TX,                     ITM_NULL,                 ITM_NULL,              ITM_TUX,                  ITM_TM1P                      };
const int16_t menu_PHI[]         = { ITM_phix,                      ITM_PHIx,                   ITM_NULL,                 ITM_NULL,              ITM_PHIux,                ITM_PHIM1p                    };
const int16_t menu_F[]           = { ITM_FPX,                       ITM_FX,                     ITM_NULL,                 ITM_NULL,              ITM_FUX,                  ITM_FM1P                      };
const int16_t menu_chi2[]        = { ITM_chi2Px,                    ITM_chi2x,                  ITM_NULL,                 ITM_NULL,              ITM_chi2ux,               ITM_chi2M1                    };
const int16_t menu_Norml[]       = { ITM_NORMLP,                    ITM_NORML,                  ITM_NULL,                 ITM_NULL,              ITM_NORMLU,               ITM_NORMLM1                   };
const int16_t menu_LgNrm[]       = { ITM_LGNRMP,                    ITM_LGNRM,                  ITM_NULL,                 ITM_NULL,              ITM_LGNRMU,               ITM_LGNRMM1                   };
const int16_t menu_Cauch[]       = { ITM_CAUCHP,                    ITM_CAUCH,                  ITM_NULL,                 ITM_NULL,              ITM_CAUCHU,               ITM_CAUCHM1                   };
const int16_t menu_Expon[]       = { ITM_EXPONP,                    ITM_EXPON,                  ITM_NULL,                 ITM_NULL,              ITM_EXPONU,               ITM_EXPONM1                   };
const int16_t menu_Logis[]       = { ITM_LOGISP,                    ITM_LOGIS,                  ITM_NULL,                 ITM_NULL,              ITM_LOGISU,               ITM_LOGISM1                   };
const int16_t menu_Weibl[]       = { ITM_WEIBLP,                    ITM_WEIBL,                  ITM_NULL,                 ITM_NULL,              ITM_WEIBLU,               ITM_WEIBLM1                   };
const int16_t menu_Binom[]       = { ITM_BINOMP,                    ITM_BINOM,                  ITM_NULL,                 ITM_NULL,              ITM_BINOMU,               ITM_BINOMM1                   };
const int16_t menu_Geom[]        = { ITM_GEOMP,                     ITM_GEOM,                   ITM_NULL,                 ITM_NULL,              ITM_GEOMU,                ITM_GEOMM1                    };
const int16_t menu_Hyper[]       = { ITM_HYPERP,                    ITM_HYPER,                  ITM_NULL,                 ITM_NULL,              ITM_HYPERU,               ITM_HYPERM1                   };
const int16_t menu_Nbin[]        = { ITM_NBINP,                     ITM_NBIN,                   ITM_NULL,                 ITM_NULL,              ITM_NBINU,                ITM_NBINM1                    };
const int16_t menu_Poiss[]       = { ITM_POISSP,                    ITM_POISS,                  ITM_NULL,                 ITM_NULL,              ITM_POISSU,               ITM_POISSM1                   };

/*      Menu name                  <--------------------------------------------------------------------------- 6 functions -------------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 f shifted functions ----------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 g shifted functions ----------------------------------------------------------------------->  */
const int16_t menu_PFN[]         = { ITM_INPUT,                     ITM_END,                    ITM_ERR,                  ITM_TICKS,             ITM_PAUSE,                -MNU_PFN2,
                                     ITM_PSTO,                      ITM_PRCL,                   ITM_ALPHAOFF,             ITM_ALPHAON,           ITM_CONST,                ITM_PUTK,
                                     ITM_R_CLR,                     ITM_R_COPY,                 ITM_R_SORT,               ITM_R_SWAP,            ITM_LocR,                 ITM_POPLR                     };

const int16_t menu_PFN2[]        = { ITM_MENU,                      ITM_KEYG,                   ITM_KEYX,                 ITM_CLMENU,            ITM_EXITALL,              ITM_RTNP1,
                                     ITM_SDL,                       ITM_SDR,                    ITM_MSG,                  ITM_NOP,               ITM_VARMNU,               ITM_MVAR,
                                     ITM_BACK,                      ITM_CASE,                   ITM_SKIP,                 ITM_AGRAPH,            ITM_PIXEL,                ITM_POINT                     };

const int16_t menu_STAT[]        = { ITM_SIGMAPLUS,                 ITM_XBAR,                   ITM_S,                    ITM_sigma,             ITM_SM,                   ITM_SQUARE,
                                     ITM_SIGMAMINUS,                ITM_XW,                     ITM_SW,                   ITM_sigmaw,            ITM_SMW,                  ITM_SUM,
                                     ITM_CLSIGMA,                   ITM_XG,                     ITM_epsilon,              ITM_epsilonP,          ITM_epsilonM,             ITM_PLOT,
                                     ITM_LR,                        ITM_CORR,                   ITM_SXY,                  ITM_XCIRC,             ITM_YCIRC,                ITM_SQUARE,
                                     ITM_ORTHOF,                    ITM_NULL,                   ITM_COV,                  ITM_NULL,              ITM_NULL,                 ITM_NULL,
                                     ITM_LINF,                      ITM_EXPF,                   ITM_LOGF,                 ITM_POWERF,            ITM_NULL,                 ITM_BESTF,
                                     ITM_NSIGMA,                    ITM_SIGMAx,                 ITM_SIGMAx2,              ITM_SIGMAxy,           ITM_SIGMAy2,              ITM_SIGMAy,
                                     ITM_NULL,                      ITM_SIGMAlnx,               ITM_SIGMAln2x,            ITM_SIGMAlnxy,         ITM_SIGMAln2y,            ITM_SIGMAlny,
                                     ITM_SIGMAx2y,                  ITM_SIGMAxlny,              ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_SIGMAylnx                 };

const int16_t menu_STK[]         = { ITM_Xex,                       ITM_Yex,                    ITM_Zex,                  ITM_Tex,               ITM_ex,                   ITM_DROP,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_DROPY,
                                     ITM_SSIZE4,                    ITM_SSIZE8,                 ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };

/*      Menu name                  <--------------------------------------------------------------------------- 6 functions -------------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 f shifted functions ----------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 g shifted functions ----------------------------------------------------------------------->  */
const int16_t menu_TEST[]        = { ITM_XLT,                       ITM_XLE,                    ITM_XEQU,                 ITM_XNE,               ITM_XGE,                  ITM_XGT,
                                     ITM_INT,                       ITM_EVEN,                   ITM_ODD,                  ITM_PRIME,             ITM_LEAP,                 ITM_FPQ,
                                     ITM_ENTRY,                     ITM_KEYQ,                   ITM_LBLQ,                 ITM_STRI,              ITM_CONVG,                ITM_TOP,
                                     ITM_XEQUP0,                    ITM_XEQUM0,                 ITM_XAEQU,                ITM_MATR,              ITM_CPX,                  ITM_REAL,
                                     ITM_SPEC,                      ITM_NAN,                    ITM_NULL,                 ITM_M_SQR,             ITM_NULL,                 ITM_DBL                       };

const int16_t menu_XFN[]         = { ITM_AGM,                       ITM_BN,                     ITM_BNS,                  ITM_ERF,               ITM_ERFC,                -MNU_ORTHOG,
                                     ITM_FIB,                       ITM_GD,                     ITM_GDM1,                 ITM_IXYZ,              ITM_IGAMMAP,              ITM_IGAMMAQ,
                                     ITM_JYX,                       ITM_LNBETA,                 ITM_LNGAMMA,              ITM_MAX,               ITM_MIN,                  ITM_NEXTP,
                                     ITM_SHOW,                      ITM_sinc,                   ITM_WM,                   ITM_WP,                ITM_WM1,                  ITM_BETAXY,
                                     ITM_gammaXY,                   ITM_GAMMAXY,                ITM_zetaX,                ITM_M1X,               ITM_toDP,                 ITM_toSP,
                                     ITM_PARALLEL,                  ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };

const int16_t menu_Orthog[]      = { ITM_HN,                        ITM_Ln,                     ITM_LNALPHA,              ITM_PN,                ITM_TN,                   ITM_UN,
                                     ITM_HNP,                       ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };

// Following menu is UPPER CASE for lower case: +80

const int16_t menu_alpha_INTL[]  = { CHR_A_MACRON,                  CHR_A_ACUTE,                CHR_A_BREVE,              CHR_A_GRAVE,           CHR_A_DIARESIS,           CHR_A_TILDE,
                                     CHR_A_CIRC,                    CHR_A_RING,                 CHR_AE,                   CHR_A_OGONEK,          CHR_C_ACUTE,              CHR_C_CARON,
                                     CHR_C_CEDILLA,                 CHR_D_STROKE,               CHR_D_CARON,              CHR_E_MACRON,          CHR_E_ACUTE,              CHR_E_BREVE,
                                     CHR_E_GRAVE,                   CHR_E_DIARESIS,             CHR_E_CIRC,               CHR_E_OGONEK,          CHR_G_BREVE,              ITM_NULL,
                                     CHR_I_MACRON,                  CHR_I_ACUTE,                CHR_I_BREVE,              CHR_I_GRAVE,           CHR_I_DIARESIS,           CHR_I_CIRC,
                                     CHR_I_OGONEK,                  CHR_I_DOT,                  CHR_I_DOTLESS,            CHR_L_STROKE,          CHR_L_ACUTE,              CHR_L_APOSTROPHE,
                                     CHR_N_ACUTE,                   CHR_N_CARON,                CHR_N_TILDE,              CHR_O_MACRON,          CHR_O_ACUTE,              CHR_O_BREVE,
                                     CHR_O_GRAVE,                   CHR_O_DIARESIS,             CHR_O_TILDE,              CHR_O_CIRC,            CHR_O_STROKE,             CHR_OE,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 CHR_S_ACUTE,           CHR_S_CARON,              CHR_S_CEDILLA,
                                     CHR_T_CARON,                   CHR_T_CEDILLA,              CHR_U_MACRON,             CHR_U_ACUTE,           CHR_U_BREVE,              CHR_U_GRAVE,
                                     CHR_U_DIARESIS,                CHR_U_TILDE,                CHR_U_CIRC,               CHR_U_RING,            CHR_W_CIRC,               ITM_NULL,
                                     ITM_NULL,                      ITM_NULL,                   CHR_Y_CIRC,               CHR_Y_ACUTE,           CHR_Y_DIARESIS,           CHR_Z_ACUTE,
                                     CHR_Z_CARON,                   CHR_Z_DOT,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };

/*      Menu name                  <--------------------------------------------------------------------------- 6 functions -------------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 f shifted functions ----------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 g shifted functions ----------------------------------------------------------------------->  */
const int16_t menu_alpha_intl[]  = { CHR_a_MACRON,                  CHR_a_ACUTE,                CHR_a_BREVE,              CHR_a_GRAVE,           CHR_a_DIARESIS,           CHR_a_TILDE,
                                     CHR_a_CIRC,                    CHR_a_RING,                 CHR_ae,                   CHR_a_OGONEK,          CHR_c_ACUTE,              CHR_c_CARON,
                                     CHR_c_CEDILLA,                 CHR_d_STROKE,               CHR_d_APOSTROPHE,         CHR_e_MACRON,          CHR_e_ACUTE,              CHR_e_BREVE,
                                     CHR_e_GRAVE,                   CHR_e_DIARESIS,             CHR_e_CIRC,               CHR_e_OGONEK,          CHR_g_BREVE,              CHR_h_STROKE,
                                     CHR_i_MACRON,                  CHR_i_ACUTE,                CHR_i_BREVE,              CHR_i_GRAVE,           CHR_i_DIARESIS,           CHR_i_CIRC,
                                     CHR_i_OGONEK,                  CHR_i_DOT,                  CHR_i_DOTLESS,            CHR_l_STROKE,          CHR_l_ACUTE,              CHR_l_APOSTROPHE,
                                     CHR_n_ACUTE,                   CHR_n_CARON,                CHR_n_TILDE,              CHR_o_MACRON,          CHR_o_ACUTE,              CHR_o_BREVE,
                                     CHR_o_GRAVE,                   CHR_o_DIARESIS,             CHR_o_TILDE,              CHR_o_CIRC,            CHR_o_STROKE,             CHR_oe,
                                     CHR_r_CARON,                   CHR_r_ACUTE,                CHR_s_SHARP,              CHR_s_ACUTE,           CHR_s_CARON,              CHR_s_CEDILLA,
                                     CHR_t_APOSTROPHE,              CHR_t_CEDILLA,              CHR_u_MACRON,             CHR_u_ACUTE,           CHR_u_BREVE,              CHR_u_GRAVE,
                                     CHR_u_DIARESIS,                CHR_u_TILDE,                CHR_u_CIRC,               CHR_u_RING,            CHR_w_CIRC,               CHR_x_BAR,
                                     CHR_x_CIRC,                    CHR_y_BAR,                  CHR_y_CIRC,               CHR_y_ACUTE,           CHR_y_DIARESIS,           CHR_z_ACUTE,
                                     CHR_z_CARON,                   CHR_z_DOT,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };

const int16_t menu_CATALOG[]     = { -MNU_FCNS,                     -MNU_DIGITS,                -MNU_CHARS,               -MNU_PROGS,            -MNU_VARS,                -MNU_MENUS                    };

const int16_t menu_DIGITS[]      = { CHR_0,                         CHR_1,                      CHR_2,                    CHR_3,                 CHR_4,                    CHR_5,
                                     CHR_6,                         CHR_7,                      CHR_8,                    CHR_9,                 CHR_A,                    CHR_B,
                                     CHR_C,                         CHR_D,                      CHR_E,                    CHR_F,                 CHR_i,                    ITM_NULL                      };

const int16_t menu_CHARS[]       = { -MNU_A_Z,                      -MNU_ALPHA_OMEGA,           -MNU_ALPHAINTL,           -MNU_ALPHAMATH,        -MNU_MyAlpha,             -MNU_ALPHADOT                 };

const int16_t menu_PROGS[]       = { -MNU_RAM,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 -MNU_FLASH                    };

const int16_t menu_VARS[]        = { -MNU_IINTS,                    -MNU_FINTS,                 -MNU_REALS,               -MNU_CPXS,             -MNU_STRING,              -MNU_MATRS,
                                     -MNU_DATES,                    -MNU_TIMES,                 ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };

/*      Menu name                  <--------------------------------------------------------------------------- 6 functions -------------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 f shifted functions ----------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 g shifted functions ----------------------------------------------------------------------->  */
// Following menu is UPPER CASE for lower case: +26
const int16_t menu_A_Z[]         = { CHR_A,                         CHR_B,                      CHR_C,                    CHR_D,                 CHR_E,                    CHR_F,
                                     CHR_G,                         CHR_H,                      CHR_I,                    CHR_J,                 CHR_K,                    CHR_L,
                                     CHR_M,                         CHR_N,                      CHR_O,                    CHR_P,                 CHR_Q,                    CHR_R,
                                     CHR_S,                         CHR_T,                      CHR_U,                    CHR_V,                 CHR_W,                    CHR_X,
                                     CHR_Y,                         CHR_Z,                      ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };

const int16_t menu_a_z[]         = { CHR_a,                         CHR_b,                      CHR_c,                    CHR_d,                 CHR_e,                    CHR_f,
                                     CHR_g,                         CHR_h,                      CHR_i,                    CHR_j,                 CHR_k,                    CHR_l,
                                     CHR_m,                         CHR_n,                      CHR_o,                    CHR_p,                 CHR_q,                    CHR_r,
                                     CHR_s,                         CHR_t,                      CHR_u,                    CHR_v,                 CHR_w,                    CHR_x,
                                     CHR_y,                         CHR_z,                      ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };

// Following menu is UPPER CASE for lower case: +36
const int16_t menu_ALPHA_OMEGA[] = { CHR_ALPHA,                     ITM_NULL,                   CHR_BETA,                 CHR_GAMMA,             CHR_DELTA,                CHR_EPSILON,
                                     ITM_NULL,                      CHR_ZETA,                   CHR_ETA,                  ITM_NULL,              CHR_THETA,                CHR_IOTA,
                                     ITM_NULL,                      ITM_NULL,                   CHR_IOTA_DIALYTIKA,       CHR_KAPPA,             CHR_LAMBDA,               CHR_MU,
                                     CHR_NU,                        CHR_XI,                     CHR_OMICRON,              ITM_NULL,              CHR_PI,                   CHR_RHO,
                                     CHR_SIGMA,                     ITM_NULL,                   CHR_TAU,                  CHR_UPSILON,           ITM_NULL,                 CHR_UPSILON_DIALYTIKA,
                                     ITM_NULL,                      CHR_PHI,                    CHR_CHI,                  CHR_PSI,               CHR_OMEGA,                ITM_NULL                      };

const int16_t menu_alpha_omega[] = { CHR_alpha,                     CHR_alpha_TONOS,            CHR_beta,                 CHR_gamma,             CHR_delta,                CHR_epsilon,
                                     CHR_epsilon_TONOS,             CHR_zeta,                   CHR_eta,                  CHR_eta_TONOS,         CHR_theta,                CHR_iota,
                                     CHR_iotaTON,                   CHR_iota_DIALYTIKA_TONOS,   CHR_iota_DIALYTIKA,       CHR_kappa,             CHR_lambda,               CHR_mu,
                                     CHR_nu,                        CHR_xi,                     CHR_omicron,              CHR_omicron_TONOS,     CHR_pi,                   CHR_rho,
                                     CHR_sigma,                     CHR_sigma_end,              CHR_tau,                  CHR_upsilon,           CHR_upsilon_TONOS,        CHR_upsilon_DIALYTIKA,
                                     CHR_upsilon_DIALYTIKA_TONOS,   CHR_phi,                    CHR_chi,                  CHR_psi,               CHR_omega,                CHR_omega_TONOS               };

const int16_t menu_AngleConv[]   = { ITM_toDEG,                     ITM_toRAD,                  ITM_toGRAD,               ITM_NULL,              ITM_toDMS,                ITM_toMULpi,
                                     ITM_DEGto,                     ITM_RADto,                  ITM_GRADto,               ITM_NULL,              ITM_DMSto,                ITM_MULPIto,
                                     ITM_DtoR,                      ITM_RtoD,                   ITM_NULL,                 ITM_DtoDMS,            ITM_DMStoD,               ITM_NULL,                     };

/*      Menu name                  <--------------------------------------------------------------------------- 6 functions -------------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 f shifted functions ----------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 g shifted functions ----------------------------------------------------------------------->  */
const int16_t menu_UnitConv[]    = { -MNU_CONVE,                    -MNU_CONVP,                 ITM_YEARtoS,              -MNU_CONVFP,           -MNU_CONVM,               -MNU_CONVX,
                                     ITM_CtoF,                      ITM_FtoC,                   ITM_StoYEAR,              ITM_NULL,              -MNU_CONVV,               -MNU_CONVA,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL,
                                     10000+ITM_PRtoDBc,             10000+ITM_DBtoPRc,          ITM_NULL,                 ITM_NULL,              10000+ITM_FRtoDBc,        10000+ITM_DBtoFRc,
                                     30000+ITM_PRtoDBb,             30000+ITM_DBtoPRb,          ITM_NULL,                 ITM_NULL,              30000+ITM_FRtoDBb,        30000+ITM_DBtoFRb,
                                     20000+ITM_PRtoDB,              20000+ITM_DBtoPR,           ITM_NULL,                 ITM_NULL,              20000+ITM_FRtoDB,         20000+ITM_DBtoFR              };

const int16_t menu_ConvE[]       = { ITM_CALtoJ,                    ITM_JtoCAL,                 ITM_BTUtoJ,               ITM_JtoBTU,            ITM_WHtoJ,                ITM_JtoWH                     };

const int16_t menu_ConvP[]       = { ITM_HPEtoW,                    ITM_WtoHPE,                 ITM_HPUKtoW,              ITM_WtoHPUK,           ITM_HPMtoW,               ITM_WtoHPM                    };

const int16_t menu_ConvFP[]      = { ITM_LBFtoN,                    ITM_NtoLBF,                 ITM_BARtoPA,              ITM_PAtoBAR,           ITM_PSItoPA,              ITM_PAtoPSI,
                                     10000+ITM_IHGtoPAb,            10000+ITM_PAtoIHGb,         10000+ITM_TORtoPAb,       10000+ITM_PAtoTORb,    10000+ITM_ATMtoPA,        10000+ITM_PAtoATM,
                                     20000+ITM_IHGtoPA,             20000+ITM_PAtoIHG,          20000+ITM_TORtoPA,        20000+ITM_PAtoTOR,     20000+ITM_ATMtoPAb,       20000+ITM_PAtoATMb            };

const int16_t menu_ConvM[]       = { ITM_LBStoKG,                   ITM_KGtoLBS,                ITM_CWTtoKG,              ITM_KGtoCWT,           ITM_OZtoKG,               ITM_KGtoOZ,
                                     10000+ITM_STOtoKGb,            10000+ITM_KGtoSTOb,         10000+ITM_SCWtoKGb,       10000+ITM_KGtoSCWb,    10000+ITM_TRZtoKGb,       10000+ITM_KGtoTRZb,
                                     20000+ITM_STOtoKG,             20000+ITM_KGtoSTO,          20000+ITM_SCWtoKG,        20000+ITM_KGtoSCW,     20000+ITM_TRZtoKG,        20000+ITM_KGtoTRZ,
                                     10000+ITM_TONtoKG,             10000+ITM_KGtoTON,          10000+ITM_STtoKGc,        10000+ITM_KGtoSTc,     10000+ITM_CARATtoKGb,     10000+ITM_KGtoCARATb,
                                     30000+ITM_TONtoKGb,            30000+ITM_KGtoTONb,         30000+ITM_STtoKGb,        30000+ITM_KGtoSTb,     30000+ITM_CARATtoKG,      30000+ITM_KGtoCARAT,
                                     20000+ITM_TONtoKGc,            20000+ITM_KGtoTONc,         20000+ITM_STtoKG,         20000+ITM_KGtoST,      20000+ITM_CARATtoKGc,     20000+ITM_KGtoCARATc          };

const int16_t menu_ConvX[]       = { ITM_AUtoM,                     ITM_MtoAU,                  ITM_LYtoM,                ITM_MtoLY,             ITM_PCtoM,                ITM_MtoPC,
                                     ITM_MItoM,                     ITM_MtoMI,                  ITM_NMItoM,               ITM_MtoNMI,            ITM_FTtoM,                ITM_MtoFT,
                                     ITM_INtoM,                     ITM_MtoINCH,                ITM_NULL,                 ITM_NULL,              ITM_YDtoM,                ITM_MtoYD,
                                     10000+ITM_FATHOMtoMb,          10000+ITM_MtoFATHOMb,       10000+ITM_POINTtoMb,      10000+ITM_MtoPOINT,    10000+ITM_FTUStoMc,       10000+ITM_MtoFTUSc,
                                     30000+ITM_FATHOMtoM,           30000+ITM_MtoFATHOM,        30000+ITM_POINTtoM,       30000+ITM_MtoPOINTb,   30000+ITM_FTUStoMb,       30000+ITM_MtoFTUSb,
                                     20000+ITM_FATHOMtoMc,          20000+ITM_MtoFATHOMc,       20000+ITM_POINTtoMc,      20000+ITM_MtoPOINTc,   20000+ITM_FTUStoM,        20000+ITM_MtoFTUS             };

const int16_t menu_ConvA[]       = { 10000+ITM_ACtoM2b,             10000+ITM_M2toACb,          ITM_NULL,                 ITM_NULL,              10000+ITM_ACUStoM2b,      10000+ITM_M2toACUSb,
                                     20000+ITM_ACtoM2,              20000+ITM_M2toAC,           ITM_NULL,                 ITM_NULL,              20000+ITM_ACUStoM2,       20000+ITM_M2toACUS            };

/*      Menu name                  <--------------------------------------------------------------------------- 6 functions -------------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 f shifted functions ----------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 g shifted functions ----------------------------------------------------------------------->  */
const int16_t menu_ConvV[]       = { ITM_GLUKtoM3,                  ITM_M3toGLUK,               ITM_QTtoM3,               ITM_M3toQT,            ITM_GLUStoM3,             ITM_M3toGLUS,
                                     10000+ITM_FZUKtoM3b,           10000+ITM_M3toFZUKb,        10000+ITM_BARRELtoM3b,    10000+ITM_M3toBARRELb, 10000+ITM_FZUStoM3b,      10000+ITM_M3toFZUSb,
                                     20000+ITM_FZUKtoM3,            20000+ITM_M3toFZUK,         20000+ITM_BARRELtoM3,     20000+ITM_M3toBARREL,  20000+ITM_FZUStoM3,       20000+ITM_M3toFZUS            };

const int16_t menu_CNST[]        = { CST_00,                        CST_01,                     CST_02,                   CST_03,                CST_04,                   CST_05,
                                     CST_06,                        CST_07,                     CST_08,                   CST_09,                CST_10,                   CST_11,
                                     CST_12,                        CST_13,                     CST_14,                   CST_15,                CST_16,                   CST_17,
                                     CST_18,                        CST_19,                     CST_20,                   CST_21,                CST_22,                   CST_23,
                                     CST_24,                        CST_25,                     CST_26,                   CST_27,                CST_28,                   CST_29,
                                     CST_30,                        CST_31,                     CST_32,                   CST_33,                CST_34,                   CST_35,
                                     CST_36,                        CST_37,                     CST_38,                   CST_39,                CST_40,                   CST_41,
                                     CST_42,                        CST_43,                     CST_44,                   CST_45,                CST_46,                   CST_47,
                                     CST_48,                        CST_49,                     CST_50,                   CST_51,                CST_52,                   CST_53,
                                     CST_54,                        CST_55,                     CST_56,                   CST_57,                CST_58,                   CST_59,
                                     CST_60,                        CST_61,                     CST_62,                   CST_63,                CST_64,                   CST_65,
                                     CST_66,                        CST_67,                     CST_68,                   CST_69,                CST_70,                   CST_71,
                                     CST_72,                        CST_73,                     CST_74,                   CST_75,                CST_76,                   CST_77,
                                     CST_78,                        ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };

const int16_t menu_alphaFN[]     = { ITM_XtoALPHA,                  ITM_ALPHARL,                ITM_ALPHARR,              ITM_ALPHASL,           ITM_ALPHAPOS,             ITM_ALPHAtoX,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_ALPHALENG,            ITM_NULL,
                                     ITM_FBR,                       ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };

/*      Menu name                  <--------------------------------------------------------------------------- 6 functions -------------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 f shifted functions ----------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 g shifted functions ----------------------------------------------------------------------->  */
const int16_t menu_FCNS[]        = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_MENUS[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_MATRS[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_STRING[]      = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_CPXS[]        = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_RAM[]         = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_FLASH[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_DATES[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_TIMES[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_ANGLES[]      = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_FINTS[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_IINTS[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };

const int16_t menu_alphaMATH[]   = { CHR_LESS_THAN,                 CHR_LESS_EQUAL,             CHR_EQUAL,                CHR_ALMOST_EQUAL,      CHR_GREATER_EQUAL,        CHR_GREATER_THAN,
                                     CHR_LEFT_CURLY_BRACKET,        CHR_LEFT_SQUARE_BRACKET,    CHR_LEFT_PARENTHESIS,     CHR_RIGHT_PARENTHESIS, CHR_RIGHT_SQUARE_BRACKET, CHR_RIGHT_CURLY_BRACKET,
                                     CHR_PROD_SIGN,                 CHR_DIVIDE,                 CHR_INTEGRAL,             CHR_SUB_INFINITY,      CHR_SUP_INFINITY,         CHR_INFINITY,
                                     CHR_NOT,                       CHR_AND,                    CHR_OR,                   CHR_NOT_EQUAL,         CHR_PIPE,                 CHR_AMPERSAND,
                                     CHR_MEASURED_ANGLE,            CHR_RIGHT_ANGLE,            CHR_PERPENDICULAR,        CHR_CUBIC_ROOT,        CHR_SQUARE_ROOT,          CHR_xTH_ROOT,
                                     CHR_x_BAR,                     CHR_y_BAR,                  CHR_x_CIRC,               CHR_y_CIRC,            CHR_x_UNDER_ROOT,         CHR_y_UNDER_ROOT,
                                     CHR_COLON_EQUALS,              CHR_ESTIMATES,              CHR_CORRESPONDS_TO,       CHR_SUB_E_OUTLINE,     CHR_COMPLEX_C,            CHR_REAL_R,
                                     CHR_SUN,                       CHR_SUB_SUN,                CHR_SUB_EARTH,            ITM_NULL,              ITM_NULL,                 CHR_NEG_EXCLAMATION_MARK,
                                     CHR_PLUS_MINUS,                CHR_CIRCUMFLEX,             CHR_SUP_T,                CHR_SUP_MINUS_1,       CHR_PLANCK_2PI,           ITM_NULL                      };

/*      Menu name                  <--------------------------------------------------------------------------- 6 functions -------------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 f shifted functions ----------------------------------------------------------------------->  */
/*                                 <-------------------------------------------------------------------- 6 g shifted functions ----------------------------------------------------------------------->  */
const int16_t menu_alphaDot[]    = { CHR_EXCLAMATION_MARK,          CHR_COLON,                  CHR_SEMICOLON,            CHR_QUOTE,             CHR_DOUBLE_QUOTE,         CHR_AT,
                                     CHR_INVERTED_EXCLAMATION_MARK, CHR_INVERTED_QUESTION_MARK, CHR_NUMBER_SIGN,          CHR_UNDERSCORE,        CHR_TILDE,                CHR_BACK_SLASH,
                                     CHR_DOLLAR,                    CHR_EURO,                   CHR_PERCENT,              CHR_AMPERSAND,         CHR_POUND,                CHR_YEN,
                                     CHR_LEFT_ARROW,                CHR_UP_ARROW,               CHR_SERIAL_IO,            CHR_DOWN_ARROW,        CHR_LEFT_ARROW,           CHR_PGM_BEGIN,
                                     CHR_LEFT_DOUBLE_ANGLE,         CHR_RIGHT_DOUBLE_ANGLE,     CHR_EMPTY_SET,            CHR_WATCH,             CHR_BULLET,               CHR_ASTERISK,
                                     CHR_PRINTER,                   CHR_HOURGLASS,              CHR_NEG_EXCLAMATION_MARK, CHR_USER_MODE,         CHR_BATTERY,              CHR_SUP_ASTERISK              };

const int16_t menu_REALS[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_1stDeriv[]    = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_2ndDeriv[]    = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_Sf[]          = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_Solver[]      = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_IO[]          = { ITM_BEEP,                      ITM_LOAD,                   ITM_LOADP,                ITM_LOADR,             ITM_LOADSS,               ITM_LOADSIGMA,
                                     ITM_QUIET,                     ITM_TONE,                   ITM_NULL,                 ITM_NULL,              ITM_RECV,                 ITM_SEND,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL,
                                     ITM_PRINTERADV,                ITM_PRINTERCHAR,            ITM_PRINTERDLAY,          ITM_PRINTERLCD,        ITM_PRINTERMODE,          ITM_PRINTERPROG,
                                     ITM_PRINTERR,                  ITM_PRINTERREGS,            ITM_PRINTERSTK,           ITM_PRINTERTAB,        ITM_PRINTERUSER,          ITM_PRINTERWIDTH,
                                     ITM_PRINTERSIGMA,              ITM_PRINTERHASH,            ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_Tam[]         = { ITM_INDIRECTION,               -MNU_VARS,                  ITM_ST_X,                 ITM_ST_Y,              ITM_ST_Z,                 ITM_ST_T                      };
const int16_t menu_TamCmp[]      = { ITM_INDIRECTION,               -MNU_VARS,                  ITM_ST_X,                 ITM_ST_Y,              ITM_ST_Z,                 ITM_ST_T,
                                     ITM_0P,                        ITM_1P,                     ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };
const int16_t menu_TamStoRcl[]   = { ITM_INDIRECTION,               -MNU_VARS,                  ITM_ST_X,                 ITM_ST_Y,              ITM_ST_Z,                 ITM_ST_T,
                                     ITM_Config,                    ITM_Stack,                  ITM_NULL,                 ITM_NULL,              ITM_Max,                  ITM_Min,
                                     ITM_dddEL,                     ITM_dddIJ,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                 ITM_NULL                      };

const softmenu_t softmenu[] = {
  {.menuId = -MNU_MyAlpha,     .numRows = sizeof(menu_MyAlpha    )/sizeof(int16_t)/6, .softkeyRow = menu_MyAlpha     }, // This menu MUST stay the 1st in this list or change #define MY_ALPHA_MENU 0
  {.menuId = -MNU_ADV,         .numRows = sizeof(menu_ADV        )/sizeof(int16_t)/6, .softkeyRow = menu_ADV         },
  {.menuId = -MNU_Sfdx,        .numRows = sizeof(menu_Sfdx       )/sizeof(int16_t)/6, .softkeyRow = menu_Sfdx        },
  {.menuId = -MNU_BITS,        .numRows = sizeof(menu_BITS       )/sizeof(int16_t)/6, .softkeyRow = menu_BITS        },
  {.menuId = -MNU_CLK,         .numRows = sizeof(menu_CLK        )/sizeof(int16_t)/6, .softkeyRow = menu_CLK         },
  {.menuId = -MNU_CLR,         .numRows = sizeof(menu_CLR        )/sizeof(int16_t)/6, .softkeyRow = menu_CLR         },
  {.menuId = -MNU_CPX,         .numRows = sizeof(menu_CPX        )/sizeof(int16_t)/6, .softkeyRow = menu_CPX         },
  {.menuId = -MNU_DISP,        .numRows = sizeof(menu_DISP       )/sizeof(int16_t)/6, .softkeyRow = menu_DISP        },
  {.menuId = -MNU_EQN,         .numRows = sizeof(menu_EQN        )/sizeof(int16_t)/6, .softkeyRow = menu_EQN         },
  {.menuId = -MNU_1STDERIV,    .numRows = sizeof(menu_1stDeriv   )/sizeof(int16_t)/6, .softkeyRow = menu_1stDeriv    },
  {.menuId = -MNU_2NDDERIV,    .numRows = sizeof(menu_2ndDeriv   )/sizeof(int16_t)/6, .softkeyRow = menu_2ndDeriv    },
  {.menuId = -MNU_Sf,          .numRows = sizeof(menu_Sf         )/sizeof(int16_t)/6, .softkeyRow = menu_Sf          },
  {.menuId = -MNU_Solver,      .numRows = sizeof(menu_Solver     )/sizeof(int16_t)/6, .softkeyRow = menu_Solver      },
  {.menuId = -MNU_EXP,         .numRows = sizeof(menu_EXP        )/sizeof(int16_t)/6, .softkeyRow = menu_EXP         },
  {.menuId = -MNU_TRI,         .numRows = sizeof(menu_TRI        )/sizeof(int16_t)/6, .softkeyRow = menu_TRI         },
  {.menuId = -MNU_FIN,         .numRows = sizeof(menu_FIN        )/sizeof(int16_t)/6, .softkeyRow = menu_FIN         },
  {.menuId = -MNU_TVM,         .numRows = sizeof(menu_TVM        )/sizeof(int16_t)/6, .softkeyRow = menu_TVM         },
  {.menuId = -MNU_FLAGS,       .numRows = sizeof(menu_FLAGS      )/sizeof(int16_t)/6, .softkeyRow = menu_FLAGS       },
  {.menuId = -MNU_INFO,        .numRows = sizeof(menu_INFO       )/sizeof(int16_t)/6, .softkeyRow = menu_INFO        },
  {.menuId = -MNU_INTS,        .numRows = sizeof(menu_INTS       )/sizeof(int16_t)/6, .softkeyRow = menu_INTS        },
  {.menuId = -MNU_LOOP,        .numRows = sizeof(menu_LOOP       )/sizeof(int16_t)/6, .softkeyRow = menu_LOOP        },
  {.menuId = -MNU_MATX,        .numRows = sizeof(menu_MATX       )/sizeof(int16_t)/6, .softkeyRow = menu_MATX        },
  {.menuId = -MNU_SIM_EQ,      .numRows = sizeof(menu_M_SIM_Q    )/sizeof(int16_t)/6, .softkeyRow = menu_M_SIM_Q     },
  {.menuId = -MNU_M_EDIT,      .numRows = sizeof(menu_M_EDIT     )/sizeof(int16_t)/6, .softkeyRow = menu_M_EDIT      },
  {.menuId = -MNU_M_EDITN,     .numRows = sizeof(menu_M_EDITN    )/sizeof(int16_t)/6, .softkeyRow = menu_M_EDITN     },
  {.menuId = -MNU_MODE,        .numRows = sizeof(menu_MODE       )/sizeof(int16_t)/6, .softkeyRow = menu_MODE        },
  {.menuId = -MNU_PARTS,       .numRows = sizeof(menu_PARTS      )/sizeof(int16_t)/6, .softkeyRow = menu_PARTS       },
  {.menuId = -MNU_PROB,        .numRows = sizeof(menu_PROB       )/sizeof(int16_t)/6, .softkeyRow = menu_PROB        },
  {.menuId = -MNU_T,           .numRows = sizeof(menu_t          )/sizeof(int16_t)/6, .softkeyRow = menu_t           },
  {.menuId = -MNU_PHI,         .numRows = sizeof(menu_PHI        )/sizeof(int16_t)/6, .softkeyRow = menu_PHI         },
  {.menuId = -MNU_F,           .numRows = sizeof(menu_F          )/sizeof(int16_t)/6, .softkeyRow = menu_F           },
  {.menuId = -MNU_CHI2,        .numRows = sizeof(menu_chi2       )/sizeof(int16_t)/6, .softkeyRow = menu_chi2        },
  {.menuId = -MNU_NORML,       .numRows = sizeof(menu_Norml      )/sizeof(int16_t)/6, .softkeyRow = menu_Norml       },
  {.menuId = -MNU_LGNRM,       .numRows = sizeof(menu_LgNrm      )/sizeof(int16_t)/6, .softkeyRow = menu_LgNrm       },
  {.menuId = -MNU_CAUCH,       .numRows = sizeof(menu_Cauch      )/sizeof(int16_t)/6, .softkeyRow = menu_Cauch       },
  {.menuId = -MNU_EXPON,       .numRows = sizeof(menu_Expon      )/sizeof(int16_t)/6, .softkeyRow = menu_Expon       },
  {.menuId = -MNU_LOGIS,       .numRows = sizeof(menu_Logis      )/sizeof(int16_t)/6, .softkeyRow = menu_Logis       },
  {.menuId = -MNU_WEIBL,       .numRows = sizeof(menu_Weibl      )/sizeof(int16_t)/6, .softkeyRow = menu_Weibl       },
  {.menuId = -MNU_BINOM,       .numRows = sizeof(menu_Binom      )/sizeof(int16_t)/6, .softkeyRow = menu_Binom       },
  {.menuId = -MNU_GEOM,        .numRows = sizeof(menu_Geom       )/sizeof(int16_t)/6, .softkeyRow = menu_Geom        },
  {.menuId = -MNU_HYPER,       .numRows = sizeof(menu_Hyper      )/sizeof(int16_t)/6, .softkeyRow = menu_Hyper       },
  {.menuId = -MNU_NBIN,        .numRows = sizeof(menu_Nbin       )/sizeof(int16_t)/6, .softkeyRow = menu_Nbin        },
  {.menuId = -MNU_POISS,       .numRows = sizeof(menu_Poiss      )/sizeof(int16_t)/6, .softkeyRow = menu_Poiss       },
  {.menuId = -MNU_PFN,         .numRows = sizeof(menu_PFN        )/sizeof(int16_t)/6, .softkeyRow = menu_PFN         },
  {.menuId = -MNU_PFN2,        .numRows = sizeof(menu_PFN2       )/sizeof(int16_t)/6, .softkeyRow = menu_PFN2        },
  {.menuId = -MNU_STAT,        .numRows = sizeof(menu_STAT       )/sizeof(int16_t)/6, .softkeyRow = menu_STAT        },
  {.menuId = -MNU_STK,         .numRows = sizeof(menu_STK        )/sizeof(int16_t)/6, .softkeyRow = menu_STK         },
  {.menuId = -MNU_TEST,        .numRows = sizeof(menu_TEST       )/sizeof(int16_t)/6, .softkeyRow = menu_TEST        },
  {.menuId = -MNU_XFN,         .numRows = sizeof(menu_XFN        )/sizeof(int16_t)/6, .softkeyRow = menu_XFN         },
  {.menuId = -MNU_ORTHOG,      .numRows = sizeof(menu_Orthog     )/sizeof(int16_t)/6, .softkeyRow = menu_Orthog      },
  {.menuId = -MNU_ALPHAINTL,   .numRows = sizeof(menu_alpha_INTL )/sizeof(int16_t)/6, .softkeyRow = menu_alpha_INTL  },
  {.menuId = -MNU_ALPHAintl,   .numRows = sizeof(menu_alpha_intl )/sizeof(int16_t)/6, .softkeyRow = menu_alpha_intl  },
  {.menuId = -MNU_CATALOG,     .numRows = sizeof(menu_CATALOG    )/sizeof(int16_t)/6, .softkeyRow = menu_CATALOG     },
  {.menuId = -MNU_DIGITS,      .numRows = sizeof(menu_DIGITS     )/sizeof(int16_t)/6, .softkeyRow = menu_DIGITS      },
  {.menuId = -MNU_CHARS,       .numRows = sizeof(menu_CHARS      )/sizeof(int16_t)/6, .softkeyRow = menu_CHARS       },
  {.menuId = -MNU_PROGS,       .numRows = sizeof(menu_PROGS      )/sizeof(int16_t)/6, .softkeyRow = menu_PROGS       },
  {.menuId = -MNU_VARS,        .numRows = sizeof(menu_VARS       )/sizeof(int16_t)/6, .softkeyRow = menu_VARS        },
  {.menuId = -MNU_A_Z,         .numRows = sizeof(menu_A_Z        )/sizeof(int16_t)/6, .softkeyRow = menu_A_Z         },
  {.menuId = -MNU_a_z,         .numRows = sizeof(menu_a_z        )/sizeof(int16_t)/6, .softkeyRow = menu_a_z         },
  {.menuId = -MNU_ALPHA_OMEGA, .numRows = sizeof(menu_ALPHA_OMEGA)/sizeof(int16_t)/6, .softkeyRow = menu_ALPHA_OMEGA },
  {.menuId = -MNU_alpha_omega, .numRows = sizeof(menu_alpha_omega)/sizeof(int16_t)/6, .softkeyRow = menu_alpha_omega },
  {.menuId = -MNU_FCNS,        .numRows = sizeof(menu_FCNS       )/sizeof(int16_t)/6, .softkeyRow = menu_FCNS        },
  {.menuId = -MNU_MENUS,       .numRows = sizeof(menu_MENUS      )/sizeof(int16_t)/6, .softkeyRow = menu_MENUS       },
  {.menuId = -MNU_MATRS,       .numRows = sizeof(menu_MATRS      )/sizeof(int16_t)/6, .softkeyRow = menu_MATRS       },
  {.menuId = -MNU_STRING,      .numRows = sizeof(menu_STRING     )/sizeof(int16_t)/6, .softkeyRow = menu_STRING      },
  {.menuId = -MNU_RAM,         .numRows = sizeof(menu_RAM        )/sizeof(int16_t)/6, .softkeyRow = menu_RAM         },
  {.menuId = -MNU_FLASH,       .numRows = sizeof(menu_FLASH      )/sizeof(int16_t)/6, .softkeyRow = menu_FLASH       },
  {.menuId = -MNU_DATES,       .numRows = sizeof(menu_DATES      )/sizeof(int16_t)/6, .softkeyRow = menu_DATES       },
  {.menuId = -MNU_TIMES,       .numRows = sizeof(menu_TIMES      )/sizeof(int16_t)/6, .softkeyRow = menu_TIMES       },
  {.menuId = -MNU_ANGLES,      .numRows = sizeof(menu_ANGLES     )/sizeof(int16_t)/6, .softkeyRow = menu_ANGLES      },
  {.menuId = -MNU_FINTS,       .numRows = sizeof(menu_FINTS      )/sizeof(int16_t)/6, .softkeyRow = menu_FINTS       },
  {.menuId = -MNU_IINTS,       .numRows = sizeof(menu_IINTS      )/sizeof(int16_t)/6, .softkeyRow = menu_IINTS       },
  {.menuId = -MNU_ALPHAMATH,   .numRows = sizeof(menu_alphaMATH  )/sizeof(int16_t)/6, .softkeyRow = menu_alphaMATH   },
  {.menuId = -MNU_ALPHADOT,    .numRows = sizeof(menu_alphaDot   )/sizeof(int16_t)/6, .softkeyRow = menu_alphaDot    },
  {.menuId = -MNU_REALS,       .numRows = sizeof(menu_REALS      )/sizeof(int16_t)/6, .softkeyRow = menu_REALS       },
  {.menuId = -MNU_ALPHAFN,     .numRows = sizeof(menu_alphaFN    )/sizeof(int16_t)/6, .softkeyRow = menu_alphaFN     },
  {.menuId = -MNU_CPXS,        .numRows = sizeof(menu_CPXS       )/sizeof(int16_t)/6, .softkeyRow = menu_CPXS        },
  {.menuId = -MNU_ANGLECONV,   .numRows = sizeof(menu_AngleConv  )/sizeof(int16_t)/6, .softkeyRow = menu_AngleConv   },
  {.menuId = -MNU_UNITCONV,    .numRows = sizeof(menu_UnitConv   )/sizeof(int16_t)/6, .softkeyRow = menu_UnitConv    },
  {.menuId = -MNU_CONVE,       .numRows = sizeof(menu_ConvE      )/sizeof(int16_t)/6, .softkeyRow = menu_ConvE       },
  {.menuId = -MNU_CONVP,       .numRows = sizeof(menu_ConvP      )/sizeof(int16_t)/6, .softkeyRow = menu_ConvP       },
  {.menuId = -MNU_CONVFP,      .numRows = sizeof(menu_ConvFP     )/sizeof(int16_t)/6, .softkeyRow = menu_ConvFP      },
  {.menuId = -MNU_CONVM,       .numRows = sizeof(menu_ConvM      )/sizeof(int16_t)/6, .softkeyRow = menu_ConvM       },
  {.menuId = -MNU_CONVX,       .numRows = sizeof(menu_ConvX      )/sizeof(int16_t)/6, .softkeyRow = menu_ConvX       },
  {.menuId = -MNU_CONVV,       .numRows = sizeof(menu_ConvV      )/sizeof(int16_t)/6, .softkeyRow = menu_ConvV       },
  {.menuId = -MNU_CONVA,       .numRows = sizeof(menu_ConvA      )/sizeof(int16_t)/6, .softkeyRow = menu_ConvA       },
  {.menuId = -MNU_CNST,        .numRows = sizeof(menu_CNST       )/sizeof(int16_t)/6, .softkeyRow = menu_CNST        },
  {.menuId = -MNU_IO,          .numRows = sizeof(menu_IO         )/sizeof(int16_t)/6, .softkeyRow = menu_IO          },
  {.menuId = -MNU_TAM,         .numRows = sizeof(menu_Tam        )/sizeof(int16_t)/6, .softkeyRow = menu_Tam         },
  {.menuId = -MNU_TAMCMP,      .numRows = sizeof(menu_TamCmp     )/sizeof(int16_t)/6, .softkeyRow = menu_TamCmp      },
  {.menuId = -MNU_TAMSTORCL,   .numRows = sizeof(menu_TamStoRcl  )/sizeof(int16_t)/6, .softkeyRow = menu_TamStoRcl   },
  {.menuId =  0,               .numRows = 0,                                          .softkeyRow = NULL             }
};



#ifndef TESTSUITE_BUILD
/********************************************//**
 * \brief Displays one softkey
 *
 * \param[in] label const char*     Text to display
 * \param[in] xsk int16_t           x location of softkey: from 0 (left) to 5 (right)
 * \param[in] ysk int16_t           y location of softkey: from 0 (bottom) to 2 (top)
 * \param[in] videoMode videoMode_t Video mode normal or reverse
 * \param[in] topLineDotted bool_t  Is the item's top line dotted
 * \param[in] topLine bool_t        Draw a top line
 * \param[in] bottomLine bool_t     Draw a bottom line
 * \return void
 ***********************************************/
void showSoftkey(const char *label, int16_t xsk, int16_t ysk, videoMode_t videoMode, bool_t topLineDotted, bool_t topLine, bool_t bottomLine) {
  int16_t x, y, x1, y1, x2, y2;
  int16_t w;

  if(0 <= xsk && xsk <= 5) {
    x1 = 67*xsk - 1;
    x2 = x1 + 67;
  }
  else {
    sprintf(errorMessage, "In function showSoftkey: xsk=%" FMT16S " must be from 0 to 5" , xsk);
    displayBugScreen(errorMessage);
    return;
  }

  if(0 <= ysk && ysk <= 2) {
    y1 = 217 - SOFTMENU_HEIGHT*ysk;
    y2 = y1 + SOFTMENU_HEIGHT;
  }
  else {
    sprintf(errorMessage, "In function showSoftkey: ysk=%" FMT16S " but must be from 0 to 2!" , ysk);
    displayBugScreen(errorMessage);
    return;
  }

  // Draw the frame
  //   Top line
  for(x=max(0,x1); x<min(x2,SCREEN_WIDTH); x++) {
    if(topLineDotted) {
      if(x%8 < 4) {
        setPixel(x, y1);
      }
      else {
        clearPixel(x, y1);
      }
    }
    else {
      if(videoMode == vmNormal) {
        if(topLine) {
          setPixel(x, y1);
        }
        //else {
        //  clearPixel(x, y1);
        //}
      }
      else {
        clearPixel(x, y1);
      }
    }
  }

  //   Bottom line
  if(y1 + SOFTMENU_HEIGHT <= min(y2, 239)) {
    y = y1 + SOFTMENU_HEIGHT;
    for(x=max(0,x1); x<min(x2,SCREEN_WIDTH); x++) {
      if(videoMode == vmNormal) {
        if(bottomLine) {
          setPixel(x, y);
        }
        //else {
        //  clearPixel(x, y);
        //}
      }
      else {
        clearPixel(x, y);
      }
    }
  }

  //   Left line
  if(x1 >= 0) {
    for(y=y1; y<=min(y2,SCREEN_HEIGHT-1); y++) {
      if(videoMode == vmNormal) {
        setPixel(x1, y);
      }
      else {
        clearPixel(x1, y);
      }
    }
  }

  //   Right line
  if(x2 < SCREEN_WIDTH) {
    for(y=y1; y<=min(y2,SCREEN_HEIGHT-1); y++) {
      if(videoMode == vmNormal) {
        setPixel(x2, y);
      }
      else {
        clearPixel(x2, y);
      }
    }
  }

  // Clear inside the frame
  for(y=y1+1; y<min(y2,SCREEN_HEIGHT); y++) {
    for(x=x1+1; x<min(x2,SCREEN_WIDTH); x++) {
      if(videoMode == vmNormal) {
        clearPixel(x, y);
      }
      else {
        setPixel(x, y);
      }
    }
  }

  w = stringWidth(label, &standardFont, false, false);
  showString(label, &standardFont, x1 + 33 - w/2, y1 + 2, videoMode, false, false);
}



/********************************************//**
 * \brief Displays the current part of a softmenu
 *
 * \param void
 * \return void
 ***********************************************/
void showSoftmenuCurrentPart(void) {
  int16_t m, x, y, menu, yDotted=0, currentRow, item;
  const int16_t *softkeyRow;
  bool_t  dottedTopLine;

  if(softmenuStackPointer > 0) {
    clearScreen(false, false, true);

    m          = softmenuStack[softmenuStackPointer-1].softmenu;
    currentRow = softmenuStack[softmenuStackPointer-1].row;

    if(softmenu[m].numRows <= 3) {
      dottedTopLine = false;
    }
    else {
      dottedTopLine = true;
      yDotted = min(3, softmenu[m].numRows - currentRow) - 1;

      if(   softmenu[m].softkeyRow[6*(currentRow+yDotted)  ]==0 && softmenu[m].softkeyRow[6*(currentRow+yDotted)+1]==0 && softmenu[m].softkeyRow[6*(currentRow+yDotted)+2]==0
         && softmenu[m].softkeyRow[6*(currentRow+yDotted)+3]==0 && softmenu[m].softkeyRow[6*(currentRow+yDotted)+4]==0 && softmenu[m].softkeyRow[6*(currentRow+yDotted)+5]==0) {
        yDotted--;
      }

      if(   yDotted >= 0
         && softmenu[m].softkeyRow[6*(currentRow+yDotted)  ]==0 && softmenu[m].softkeyRow[6*(currentRow+yDotted)+1]==0 && softmenu[m].softkeyRow[6*(currentRow+yDotted)+2]==0
         && softmenu[m].softkeyRow[6*(currentRow+yDotted)+3]==0 && softmenu[m].softkeyRow[6*(currentRow+yDotted)+4]==0 && softmenu[m].softkeyRow[6*(currentRow+yDotted)+5]==0) {
        yDotted--;
      }

      if(   yDotted >= 0
         && softmenu[m].softkeyRow[6*(currentRow+yDotted)  ]==0 && softmenu[m].softkeyRow[6*(currentRow+yDotted)+1]==0 && softmenu[m].softkeyRow[6*(currentRow+yDotted)+2]==0
         && softmenu[m].softkeyRow[6*(currentRow+yDotted)+3]==0 && softmenu[m].softkeyRow[6*(currentRow+yDotted)+4]==0 && softmenu[m].softkeyRow[6*(currentRow+yDotted)+5]==0) {
       yDotted--;
      }
    }

    for(y=currentRow, softkeyRow=(softmenu[m].softkeyRow)+currentRow*6; y<min(currentRow+3, softmenu[m].numRows); y++, softkeyRow+=6) {
      for(x=0; x<6; x++) {
        item = softkeyRow[x];
        if(item < 0) { // softmenu
          menu = 0;
          while(softmenu[menu].menuId != 0) {
            if(softmenu[menu].menuId == item) {
              break;
            }
            menu++;
          }

          if(softmenu[menu].menuId == 0) {
            sprintf(errorMessage, "In function showSoftmenuCurrentPart: softmenu ID %" FMT16S " not found!", item);
            displayBugScreen(errorMessage);
          }
          else {
            if(softmenu[menu].softkeyRow == NULL) {
              sprintf(errorMessage, "In function showSoftmenuCurrentPart: Softmenu ID %" FMT16S " must be generated!", item);
              displayBugScreen(errorMessage);
            }
            else {
              showSoftkey(indexOfItems[-softmenu[menu].menuId].itemPrinted, x, y-currentRow, vmReverse, dottedTopLine && y-currentRow == yDotted, true, true);
            }
          }
        }
        else if(item == 9999) {
          showSoftkey(indexOfItems[productSign == PS_DOT ? CHR_CROSS : CHR_DOT].itemPrinted, x, y-currentRow, vmNormal, dottedTopLine && y-currentRow == yDotted, true, true);
        }
        else if(item > 0 && indexOfItems[item%10000].itemPrinted[0] != 0) { // softkey
          // item : +10000 -> no top line
          //        +20000 -> no bottom line
          //        +30000 -> neither top nor bottom line
          showSoftkey(indexOfItems[item%10000].itemPrinted, x, y-currentRow, vmNormal, dottedTopLine && y-currentRow == yDotted, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1);
        }
      }
    }

    // TODO may be we can omit the 5th parameter of function showSoftkey if the dotted line remains going thru the full widh of the display
    if(0 <= yDotted && yDotted <= 2) {
      yDotted = 217 - SOFTMENU_HEIGHT*yDotted;

      if(dottedTopLine) {
        for(x=0; x<SCREEN_WIDTH; x++) {
          if(x%8 < 4) {
            setPixel(x, yDotted);
          }
          else {
            clearPixel(x, yDotted);
          }
        }
      }
    }
  }
}



/********************************************//**
 * \brief Initializes the softmenu stack with a
 * softmenu and displays it's first part
 *
 * \param[in] softmenu int16_t Softmenu number
 * \return void
 ***********************************************/
void initSoftmenuStack(int16_t softmenu) {
  softmenuStack[0].softmenu = softmenu;
  softmenuStack[0].row      = 0;
  softmenuStackPointer = 1;
  showSoftmenuCurrentPart();
}



/********************************************//**
 * \brief Pushes a new softmenu on the softmenu stack
 * and displays it's first part
 *
 * \param[in] softmenu int16_t Softmenu number
 * \return void
 ***********************************************/
void pushSoftmenu(int16_t softmenu) {
  if(softmenuStackPointer < SOFTMENU_STACK_SIZE) {
    softmenuStack[softmenuStackPointer].softmenu = softmenu;
    softmenuStack[softmenuStackPointer].row      = 0;
    softmenuStackPointer++;
    showSoftmenuCurrentPart();
  }
  else {
    displayBugScreen("In function pushSoftmenu: the softmenu stack is full! Please increase the value of #define SOFTMENU_STACK_SIZE in wp43s.h");
  }
}



/********************************************//**
 * \brief Pops a softmenu from the softmenu stack
 * and display it's current part or clear the
 * softmenu area of the screen if there's nothing
 * to pop.
 *
 * \param[in] softmenu int16_t Softmenu number
 * \return void
 ***********************************************/
void popSoftmenu(void) {
  if(softmenuStackPointer > 0) {
    softmenuStackPointer--;
    if(softmenuStackPointer > 0) {
      showSoftmenuCurrentPart();
    }
    else {
      clearScreen(false, false, true);
    }
  }
  else {
    displayBugScreen("In function popSoftmenu: the softmenu stack is empty, there is no softmenu to pop!");
  }
}



/********************************************//**
 * \brief Displays a softmenu. One of the 2 first
 * parameter must be null and one must be not null
 *
 * \param[in] menu const char* Name of softmenu
 * \param[in] id int16_t       ID of softmenu
 * \param[in] push bool_t      * false: initialize the softmenu stack
 *                             * true: push the softmenu on the stack
 * \return void
 ***********************************************/
void showSoftmenu(const char *menu, int16_t id, bool_t push) {
  int16_t m;

  if((menu != NULL && id != 0) || (menu == NULL && id == 0)) {
    clearScreen(false, false, true);
    displayBugScreen("In function showSoftmenu: one parameter must be 0 and one parameter must not be 0!");
    return;
  }

  m = 0;

  if(id==-MNU_ALPHAINTL && alphaCase==AC_LOWER) { // alphaINTL
    id = -MNU_ALPHAintl;
  }
  else if(id==-MNU_A_Z && alphaCase==AC_LOWER) { // A...Z
    id = -MNU_a_z;
  }
  else if(id==-MNU_ALPHA_OMEGA && alphaCase==AC_LOWER) { // alpha...omega
    id = -MNU_alpha_omega;
  }

  if((id==0 && !strcmp(menu, STD_alpha "INTL")) && alphaCase==AC_LOWER) { // alphaINTL
    id = -MNU_ALPHAintl;
  }
  else if((id==0 && !strcmp(menu, "A" STD_ELLIPSIS "Z")) && alphaCase==AC_LOWER) { // A...Z
    id = -MNU_a_z;
  }
  else if((id==0 && !strcmp(menu, STD_ALPHA STD_ELLIPSIS STD_OMEGA)) && alphaCase==AC_LOWER) { // alpha...omega
    id = -MNU_alpha_omega;
  }

  if(id != 0) { // Search by ID
    while(softmenu[m].menuId != 0) {
      if(softmenu[m].menuId == id) {
       break;
      }
      m++;
    }
  }
  else { // Search by name
    while(softmenu[m].menuId != 0) {
      if(strcmp(indexOfItems[-softmenu[m].menuId].itemPrinted, menu) == 0) {
        break;
      }
      m++;
    }
  }
  if(softmenu[m].menuId == 0) {
    if(menu == NULL) {
      sprintf(errorMessage, "In function showSoftmenu: softmenu %" FMT16S " not found!", id);
      displayBugScreen(errorMessage);
    }
    else {
      sprintf(errorMessage, "In function showSoftmenu: softmenu %s not found!", menu);
      displayBugScreen(errorMessage);
    }
  }
  else {
    if(calcMode == CM_NORMAL || calcMode == CM_NIM) {
      if(push) {
        pushSoftmenu(m);
      }
      else {
        initSoftmenuStack(m);
      }
    }
    else if(calcMode == CM_AIM) {
      if(push) {
        pushSoftmenu(m);
      }
      else {
        while(softmenuStack[softmenuStackPointer - 1].softmenu != MY_ALPHA_MENU) {
          softmenuStackPointer--;
        }
        pushSoftmenu(m);
      }
    }
    else if(calcMode == CM_TAM) {
    }
    else {
      sprintf(errorMessage, "In fuction showSoftMenu: %" FMT8U " is an unexpected value for calcMode!", calcMode);
      displayBugScreen(errorMessage);
    }
  }
}
#endif
