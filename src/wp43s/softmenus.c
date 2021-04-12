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

/*      Menu name                           <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
TO_QSPI const int16_t menu_ADV[]         = { ITM_SOLVE,                     ITM_SLVQ,                   ITM_FQX,                  ITM_PIn,               ITM_SIGMAn,                  -MNU_Sfdx,
                                             ITM_PGMSLV,                    ITM_NULL,                   ITM_FDQX,                 ITM_NULL,              ITM_NULL,                    ITM_PGMINT                    };

TO_QSPI const int16_t menu_Sfdx[]        = { ITM_NULL,                      ITM_NULL,                   VAR_ACC,                  VAR_LLIM,              VAR_ULIM,                    ITM_INTEGRAL                  };

TO_QSPI const int16_t menu_BITS[]        = { ITM_LOGICALAND,                ITM_LOGICALOR,              ITM_LOGICALXOR,           ITM_LOGICALNOT,        ITM_MASKL,                   ITM_MASKR,
                                             ITM_LOGICALNAND,               ITM_LOGICALNOR,             ITM_LOGICALXNOR,          ITM_MIRROR,            ITM_NULL,                    ITM_ASR,
                                             ITM_SB,                        ITM_BS,                     ITM_NUMB,                 ITM_FB,                ITM_BC,                      ITM_CB,
                                             ITM_A,                         ITM_B,                      ITM_C,                    ITM_D,                 ITM_E,                       ITM_F,
                                             ITM_SL,                        ITM_RL,                     ITM_RLC,                  ITM_RRC,               ITM_RR,                      ITM_SR,
                                             ITM_LJ,                        ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_RJ,
                                             ITM_1COMPL,                    ITM_2COMPL,                 ITM_UNSIGN,               ITM_SIGNMT,            ITM_NULL,                    ITM_WSIZE                     };

TO_QSPI const int16_t menu_CLK[]         = { ITM_DATE,                      ITM_DATEto,                 ITM_toDATE,               ITM_DAY,               ITM_MONTH,                   ITM_YEAR,
                                             ITM_TIME,                      ITM_TIMEto,                 ITM_toTIME,               ITM_SECOND,            ITM_MINUTE,                  ITM_HR_DEG,
                                             ITM_toHMS,                     ITM_NULL,                   ITM_XtoDATE,              ITM_WDAY,              ITM_JtoD,                    ITM_DtoJ,                 
#if DMCP_BUILD
                                             ITM_SETTIM,                    ITM_TDISP,                  ITM_SETDAT,               ITM_DMY,               ITM_YMD,                     ITM_MDY,
#else // !DMCP_BUILD
                                             ITM_RESERVE,                   ITM_TDISP,                  ITM_RESERVE,              ITM_DMY,               ITM_YMD,                     ITM_MDY,
#endif // DMCP_BUILD
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_JUL_GREG                  };

TO_QSPI const int16_t menu_CLR[]         = { ITM_CLSIGMA,                   ITM_CLP,                    ITM_CF,                   ITM_CLMENU,            ITM_CLCVAR,                  ITM_CLX,
                                             ITM_CLREGS,                    ITM_CLPALL,                 ITM_CLFALL,               ITM_CLGRF,             ITM_CLLCD,                   ITM_CLSTK,
                                             ITM_CLALL,                     ITM_NULL,                   ITM_CLAIM,                ITM_DELITM,            ITM_CLA,                     ITM_RESET                     };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
TO_QSPI const int16_t menu_CPX[]         = { ITM_RE,                        ITM_IM,                     ITM_MAGNITUDE,            ITM_ANGLE,             ITM_REexIM,                  ITM_CC,                             //JM re-arranged menu. CPX menu
                                             KEY_COMPLEX,                   ITM_CONJ,                   ITM_DOT_PROD,             ITM_CROSS_PROD,        ITM_SIGN,                    ITM_UNITV,                                          //JM re-arranged menu. CPX menu
                                             ITM_CPXI,                      ITM_CPXJ,                   ITM_CXtoRE,               ITM_REtoCX,            ITM_RECT,                    ITM_POLAR                     };    //JM re-arranged menu
                        /*KEY_CC1 removed this one*/

TO_QSPI const int16_t menu_DISP[]        = { ITM_FIX,                       ITM_SCI,                    ITM_ENG,                  ITM_ALL,               ITM_SIGFIG,                  ITM_UNIT,
                                             ITM_SDL,                       ITM_SDR,                    ITM_ROUND2,               ITM_ROUNDI2,           ITM_RDP,                     ITM_RSD,                            //JM 
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_RECT,                    ITM_POLAR,                           //JM 


                                             ITM_SCIOVR,                    ITM_ENGOVR,                 ITM_DSTACK,               ITM_CB_LEADING_ZERO,   ITM_MULTCR,                  ITM_MULTDOT,                        //JM 
                                             ITM_NULL,                      ITM_NULL,                   ITM_SHOIREP,              ITM_LARGELI,           ITM_CPXI,                    ITM_CPXJ,                           //JM 
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM 

                                             ITM_SETCHN,                    ITM_SETEUR,                 ITM_SETIND,               ITM_SETJPN,            ITM_SETUK,                   ITM_SETUSA,                    
                                             ITM_CLK12,                     ITM_CLK24,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                             ITM_RDXPER,                    ITM_RDXCOM,                 ITM_GAP,                  ITM_DMY,               ITM_YMD,                     ITM_MDY                       };


TO_QSPI const int16_t menu_EQN[]         = { ITM_EQ_NEW,                    ITM_EQ_EDI,                 -MNU_2NDDERIV,            -MNU_1STDERIV,         -MNU_Sf,                     -MNU_Solver,
                                             ITM_EQ_DEL,                    ITM_NULL,                   ITM_NULL,                 ITM_XDEMO,             ITM_XPLOT,                   ITM_XSOLV                      };

TO_QSPI const int16_t menu_EXP[]         = { ITM_CUBE,                      ITM_YX,                     ITM_SQUARE,               ITM_LOG2,              ITM_LN1X,                    ITM_LOGXY,                          //JM re-arranged menu. logxy and square to follow DM42 keyboard. Re-aligned with 42S keys.
                                             ITM_CUBEROOT,                  ITM_XTHROOT,                ITM_SQUAREROOTX,          ITM_2X,                ITM_EX1,                     ITM_EXP,                           //JM re-arranged menu. Added YˆX to follow DM42 keyboard. Swapped YˆX and Yˆ(1/X). Re-aligned with 42S keys.
                                             ITM_sinh,                      ITM_cosh,                   ITM_tanh,                 ITM_arsinh,            ITM_arcosh,                  ITM_artanh                    };

TO_QSPI const int16_t menu_TRI[]         = { ITM_sin,                       ITM_cos,                    ITM_tan,                  ITM_arcsin,            ITM_arccos,                  ITM_arctan,                         //JM re-arranged menu TRIG menu
                                             ITM_sinc,                      ITM_sincpi,                 ITM_DEG,                  ITM_RAD,               ITM_MULPI,                   ITM_DMS,                                         //  ITM_toREC,                   ITM_toPOL,                          //JM re-arranged menu TRIG menu
                                             ITM_sinh,                      ITM_cosh,                   ITM_tanh,                 ITM_arsinh,            ITM_arcosh,                  ITM_artanh                    };    //JM re-arranged menu TRIG menu

//TO_QSPI const int16_t menu_FIN[]         = { ITM_PC,                        ITM_PCMRR,                  ITM_PCT,                  ITM_PCSIGMA,           ITM_PCPMG,                   -MNU_TVM,                     
//                                             ITM_DELTAPC,                   ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };    //JM Added D%
/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
//TVM
TO_QSPI const int16_t menu_FIN[]         = { VAR_NPER,                      VAR_IPonA,                  VAR_PERonA,               VAR_PV,                VAR_PMT,                     VAR_FV,
                                             ITM_BEGINP,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_ENDP,
                                             ITM_DELTAPC,                   ITM_PC,                     ITM_PCMRR,                ITM_PCT,               ITM_PCSIGMA,                 ITM_PCPMG                      };    //JM Added D%

TO_QSPI const int16_t menu_FLAGS[]       = { ITM_SF,                        ITM_FS,                     ITM_FF,                   ITM_STATUS,            ITM_FC,                      ITM_CF,
                                             ITM_FSS,                       ITM_FSC,                    ITM_FSF,                  ITM_FCF,               ITM_FCS,                     ITM_FCC,
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_CLFALL                    };

TO_QSPI const int16_t menu_INFO[]        = { ITM_SSIZE,                     ITM_MEM,                    ITM_RMQ,                  ITM_ISM,               ITM_WSIZEQ,                  ITM_KTYP,
                                             ITM_LocRQ,                     ITM_FLASH,                  ITM_ULP,                  ITM_NEIGHB,            ITM_SDIGS,                   ITM_BATT,
                                             ITM_WHO,                       ITM_VERS,                   ITM_M_DIMQ,               ITM_PMINFINITY,        ITM_ALPHAPOS,                ITM_ALPHALENG,                 
                                             ITM_GETRANGE,                  ITM_GET_JUL_GREG,           ITM_SH_INP_DEF,           ITM_SH_ERPN,           ITM_NULL,                    ITM_BESTFQ                      };    //JM INFO
              

TO_QSPI const int16_t menu_INTS[]        = { ITM_A,                         ITM_B,                      ITM_C,                    ITM_D,                 ITM_E,                       ITM_F,
                                             ITM_IDIV,                      ITM_RMD,                    ITM_MOD,                  ITM_XMOD,              ITM_FLOOR,                   ITM_LCM,
                                             ITM_DBLDIV,                    ITM_DBLR,                   ITM_DBLMULT,              ITM_PMOD,              ITM_CEIL,                    ITM_GCD,
                                             ITM_1COMPL,                    ITM_2COMPL,                 ITM_UNSIGN,               ITM_SIGNMT,            ITM_NULL,                    ITM_WSIZE                     };    //JM

TO_QSPI const int16_t menu_LOOP[]        = { ITM_DSE,                       ITM_DSZ,                    ITM_DSL,                  ITM_ISE,               ITM_ISZ,                     ITM_ISG,
                                             ITM_DEC,                       ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_INC                       };

/*      Menu name                           <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
TO_QSPI const int16_t menu_MATX[]        = { ITM_M_NEW,                     ITM_M_INV,                  ITM_M_DET,                ITM_M_TRANSP,          ITM_SIM_EQ,                  ITM_M_EDI,
                                             ITM_DOT_PROD,                  ITM_CROSS_PROD,             ITM_UNITV,                ITM_M_DIM,             ITM_INDEX,                   ITM_M_EDIN,
                                             ITM_ENORM,                     ITM_NULL,                   ITM_STOEL,                ITM_RCLEL,             ITM_M_PUT,                   ITM_M_GET,
                                             ITM_IPLUS,                     ITM_IMINUS,                 ITM_STOIJ,                ITM_RCLIJ,             ITM_JMINUS,                  ITM_JPLUS,
                                             ITM_RSUM,                      ITM_RNORM,                  ITM_M_LU,                 ITM_M_DIMQ,            ITM_NULL,                    ITM_M_RR,
                                             ITM_EIGVAL,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_EIGVEC                    };

TO_QSPI const int16_t menu_M_SIM_Q[]     = { VAR_MATA,                      VAR_MATB,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_MATX                      }; // Should VAR_MATA and VAR_MATB be reclaced by ITM_MATA (to be created) and ITM_MATB (to be created) here?

TO_QSPI const int16_t menu_M_EDIT[]      = { ITM_LEFT_ARROW,                ITM_UP_ARROW,               ITM_M_OLD,                ITM_M_GOTO,            ITM_DOWN_ARROW,              ITM_RIGHT_ARROW,
                                             ITM_M_INSR,                    ITM_NULL,                   ITM_M_DELR,               ITM_NULL,              ITM_M_WRAP,                  ITM_M_GROW                    };


#if defined (INLINE_TEST)
  #define ITM_TST -MNU_INL_TST
#else
  #define ITM_TST ITM_RESERVE
#endif

#ifdef DMCP_BUILD
  #define ITM_SYS ITM_SYSTEM
#else
  #define ITM_SYS ITM_RESERVE
#endif

TO_QSPI const int16_t menu_MODE[]        = { ITM_DEG,                       ITM_RAD,                    ITM_GRAD,                 ITM_MULPI,             ITM_DMS,                     ITM_DRG,                       
                                             ITM_DEG2,                      ITM_RAD2,                   ITM_GRAD2,                ITM_MULPI2,            ITM_DMS2,                    ITM_CFG,                         //JM
                                             ITM_SYS,                       ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_RECT,                    ITM_POLAR,                         //JM

                                             ITM_SSIZE4,                    ITM_SSIZE8,                 ITM_CB_CPXRES,            ITM_CB_SPCRES,         ITM_DENMAX,                  ITM_CFG,                           //JM
                                             ITM_ERPN,                      ITM_RNG,                    ITM_SETSIG,               ITM_RM,                ITM_DENANY,                  ITM_DENFIX,                          //JM
                                             ITM_INP_DEF_43S,               ITM_INP_DEF_DP,             ITM_INP_DEF_CPXDP,        ITM_INP_DEF_LI,        ITM_INP_DEF_SI,              ITM_NULL,                          //JM

                                             ITM_NO_BASE_SCREEN,            ITM_BASE_HOME,             ITM_BASE_AHOME,            ITM_HOMEx3T,           ITM_SHTIM,                   ITM_HOMEx3,                    //JM
                                             ITM_G_DOUBLETAP,               ITM_TST,                   ITM_NULL,                  ITM_NULL,              ITM_GGREEK,                  ITM_FG_LINE,                        //JM
                                             ITM_NULL,                      ITM_NULL,                  ITM_NULL,                  ITM_NULL,              ITM_NULL,                    ITM_NULL                        };


TO_QSPI const int16_t menu_PARTS[]       = { ITM_IP,                        ITM_FP,                     ITM_MANT,                 ITM_EXPT,              ITM_SIGN,                    ITM_DECOMP,
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_MAGNITUDE,               ITM_ANGLE,
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_RE,                      ITM_IM                        };

/*      Menu name                           <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
TO_QSPI const int16_t menu_PROB[]        = { -MNU_NORML,                    -MNU_T,                     ITM_COMB,                 ITM_PERM,              -MNU_F,                      -MNU_CHI2,
                                             -MNU_LGNRM,                    -MNU_CAUCH,                 ITM_XFACT,                -MNU_EXPON,            -MNU_LOGIS,                  -MNU_WEIBL,                         //JM Added x! where it belongs, directly above Cyx
                                             ITM_NULL,                      -MNU_NBIN,                  -MNU_GEOM,                -MNU_HYPER,            -MNU_BINOM,                  -MNU_POISS,
                                             ITM_RAN,                       ITM_SEED,                   ITM_RANI,                 ITM_NULL,              ITM_LNGAMMA,                 ITM_GAMMAX                    };

TO_QSPI const int16_t menu_t[]           = { ITM_TPX,                       ITM_NULL,                   ITM_TX,                   ITM_TUX,               ITM_NULL,                    ITM_TM1P                      };
TO_QSPI const int16_t menu_F[]           = { ITM_FPX,                       ITM_NULL,                   ITM_FX,                   ITM_FUX,               ITM_NULL,                    ITM_FM1P                      };
TO_QSPI const int16_t menu_chi2[]        = { ITM_chi2Px,                    ITM_NULL,                   ITM_chi2x,                ITM_chi2ux,            ITM_NULL,                    ITM_chi2M1                    };
TO_QSPI const int16_t menu_Norml[]       = { ITM_NORMLP,                    ITM_NULL,                   ITM_NORML,                ITM_NORMLU,            ITM_NULL,                    ITM_NORMLM1                   };
TO_QSPI const int16_t menu_LgNrm[]       = { ITM_LGNRMP,                    ITM_NULL,                   ITM_LGNRM,                ITM_LGNRMU,            ITM_NULL,                    ITM_LGNRMM1                   };
TO_QSPI const int16_t menu_Cauch[]       = { ITM_CAUCHP,                    ITM_NULL,                   ITM_CAUCH,                ITM_CAUCHU,            ITM_NULL,                    ITM_CAUCHM1                   };
TO_QSPI const int16_t menu_Expon[]       = { ITM_EXPONP,                    ITM_NULL,                   ITM_EXPON,                ITM_EXPONU,            ITM_NULL,                    ITM_EXPONM1                   };
TO_QSPI const int16_t menu_Logis[]       = { ITM_LOGISP,                    ITM_NULL,                   ITM_LOGIS,                ITM_LOGISU,            ITM_NULL,                    ITM_LOGISM1                   };
TO_QSPI const int16_t menu_Weibl[]       = { ITM_WEIBLP,                    ITM_NULL,                   ITM_WEIBL,                ITM_WEIBLU,            ITM_NULL,                    ITM_WEIBLM1                   };
TO_QSPI const int16_t menu_Binom[]       = { ITM_BINOMP,                    ITM_NULL,                   ITM_BINOM,                ITM_BINOMU,            ITM_NULL,                    ITM_BINOMM1                   };
TO_QSPI const int16_t menu_Geom[]        = { ITM_GEOMP,                     ITM_NULL,                   ITM_GEOM,                 ITM_GEOMU,             ITM_NULL,                    ITM_GEOMM1                    };
TO_QSPI const int16_t menu_Hyper[]       = { ITM_HYPERP,                    ITM_NULL,                   ITM_HYPER,                ITM_HYPERU,            ITM_NULL,                    ITM_HYPERM1                   };
TO_QSPI const int16_t menu_Nbin[]        = { ITM_NBINP,                     ITM_NULL,                   ITM_NBIN,                 ITM_NBINU,             ITM_NULL,                    ITM_NBINM1                    };
TO_QSPI const int16_t menu_Poiss[]       = { ITM_POISSP,                    ITM_NULL,                   ITM_POISS,                ITM_POISSU,            ITM_NULL,                    ITM_POISSM1                   };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
TO_QSPI const int16_t menu_PFN[]         = { ITM_LBL,                       ITM_RTN,                    ITM_END,                  ITM_GTO,               ITM_XEQ,                     -MNU_PFN2,          //JM ADDED LBL RTN ...
                                             ITM_INPUT,                     ITM_END,                    ITM_ERR,                  ITM_TICKS,             ITM_PAUSE,                   ITM_NULL,
                                             ITM_PSTO,                      ITM_PRCL,                   ITM_VARMNU,               ITM_MVAR,              ITM_CNST,                    ITM_PUTK,
                                             ITM_R_CLR,                     ITM_R_COPY,                 ITM_R_SORT,               ITM_R_SWAP,            ITM_LocR,                    ITM_POPLR                     };

TO_QSPI const int16_t menu_PFN2[]        = { ITM_MENU,                      ITM_KEYG,                   ITM_KEYX,                 ITM_CLMENU,            ITM_EXITALL,                 ITM_RTNP1,
                                             ITM_SDL,                       ITM_SDR,                    ITM_MSG,                  ITM_NOP,               ITM_VARMNU,                  ITM_MVAR,
                                             ITM_BACK,                      ITM_CASE,                   ITM_SKIP,                 ITM_AGRAPH,            ITM_PIXEL,                   ITM_POINT                     };

TO_QSPI const int16_t menu_STAT[]        = { ITM_SIGMAPLUS,                 ITM_XBAR,                   ITM_STDDEVWEIGHTED,       ITM_STDDEV,            ITM_SM,                      ITM_SUM,
                                             ITM_SIGMAMINUS,                ITM_XW,                     ITM_SW,                   ITM_STDDEVPOP,         ITM_SMW,                     ITM_NSIGMA,                         //JM added ITM_NSIGMA
                                             ITM_CLSIGMA,                   ITM_XG,                     ITM_SCATTFACT,            ITM_SCATTFACTp,        ITM_SCATTFACTm,              ITM_PLOT,
                                             ITM_LR,                        ITM_CORR,                   ITM_SXY,                  ITM_COV,               ITM_XCIRC,                   ITM_YCIRC,
                                             ITM_PLOT_LR,                   ITM_XH,                     ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                             ITM_SA,                        ITM_XRMS,                   ITM_XMAX,                 ITM_XMIN,              ITM_NULL,                    ITM_ORTHOF,
                                             ITM_LINF,                      ITM_EXPF,                   ITM_LOGF,                 ITM_POWERF,            ITM_NULL,                    ITM_BESTF,
                                             ITM_GAUSSF,                    ITM_CAUCHF,                 ITM_PARABF,               ITM_HYPF,              ITM_ROOTF,                   ITM_NULL,
                                             ITM_STATDEMO0,                 ITM_STATDEMO1,              ITM_STATDEMO2,            ITM_STATDEM105,        ITM_STATDEM107,              ITM_STATDEM109                      };

TO_QSPI const int16_t menu_SUMS[]        = { ITM_NSIGMA,                    ITM_SIGMAx,                 ITM_SIGMAx2,              ITM_SIGMAxy,           ITM_SIGMAy2,                 ITM_SIGMAy,
                                             ITM_NULL,                      ITM_SIGMAlnx,               ITM_SIGMAln2x,            ITM_SIGMAlnxy,         ITM_SIGMAln2y,               ITM_SIGMAlny,
                                             ITM_SIGMAx2y,                  ITM_SIGMAxlny,              ITM_NULL,                 ITM_SIGMAlnyonx,       ITM_NULL,                    ITM_SIGMAylnx,
                                             ITM_SIGMAx2ony,                ITM_SIGMA1onx,              ITM_SIGMA1onx2,           ITM_SIGMAxony,         ITM_SIGMA1ony2,              ITM_SIGMA1ony,
                                             ITM_SIGMAx3,                   ITM_SIGMAx4,                ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

TO_QSPI const int16_t menu_PLOT_STAT[]   = {
                                             ITM_PLOT_REGLINE,              ITM_SMI,                    ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

TO_QSPI const int16_t menu_PLOT_LR[]   = {
                                             ITM_PLOT_SELLINE,              ITM_PLOTZOOM,               ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };


TO_QSPI const int16_t menu_STK[]         = { ITM_Xex,                       ITM_Yex,                    ITM_Zex,                  ITM_Tex,               ITM_SHUFFLE,                      ITM_CLSTK,                          //JM Re-arranged
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_DROPY,                          //JM Added CLRSTK
                                             ITM_FILL,                      ITM_LASTX,                  ITM_NULL,                 ITM_Rdown,             ITM_Rup,                     ITM_DROP                      };    //JM Added missing STACK items

/*      Menu name                           <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
TO_QSPI const int16_t menu_TEST[]        = { ITM_XLT,                       ITM_XLE,                    ITM_XEQU,                 ITM_XNE,               ITM_XGE,                     ITM_XGT,
                                             ITM_INT,                       ITM_EVEN,                   ITM_ODD,                  ITM_PRIME,             ITM_LEAP,                    ITM_FPQ,
                                             ITM_ENTRY,                     ITM_KEYQ,                   ITM_LBLQ,                 ITM_STRI,              ITM_CONVG,                   ITM_TOP,
                                             ITM_XEQUP0,                    ITM_XEQUM0,                 ITM_XAEQU,                ITM_MATR,              ITM_CPX,                     ITM_REAL,
                                             ITM_SPEC,                      ITM_NAN,                    ITM_NULL,                 ITM_M_SQR,             ITM_NULL,                    ITM_NULL                      };

TO_QSPI const int16_t menu_XFN[]         = { ITM_AGM,                       ITM_BN,                     ITM_BNS,                  ITM_ERF,               ITM_ERFC,                   -MNU_ORTHOG,
                                             ITM_FIB,                       ITM_GD,                     ITM_GDM1,                 ITM_IXYZ,              ITM_IGAMMAP,                 ITM_IGAMMAQ,
                                             ITM_JYX,                       ITM_LNBETA,                 ITM_LNGAMMA,              ITM_MAX,               ITM_MIN,                     ITM_NEXTP,
                                             ITM_WM,                        ITM_WP,                     ITM_WM1,                  ITM_BETAXY,            ITM_gammaXY,                 ITM_GAMMAXY,
                                             ITM_zetaX,                     ITM_M1X,                    ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_PARALLEL,
                                             ITM_NULL,                      ITM_DMPMNU,                 ITM_PGMTST,               -MNU_XXEQ,             ITM_PLOT_J,                  -MNU_EE                       };    //JM Added

TO_QSPI const int16_t menu_Orthog[]      = { ITM_HN,                        ITM_Lm,                     ITM_LmALPHA,              ITM_Pn,                ITM_Tn,                      ITM_Un,
                                             ITM_HNP,                       ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

/*      Menu name                           <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
TO_QSPI const int16_t menu_CATALOG[]     = { -MNU_FCNS,                     ITM_NULL,                   -MNU_CHARS,               -MNU_PROGS,            -MNU_VARS,                   -MNU_MENUS                    };

TO_QSPI const int16_t menu_CHARS[]       = { -MNU_ALPHAINTL,               -MNU_ALPHA_OMEGA,            ITM_NULL,                -MNU_ALPHAMATH,        -MNU_MyAlpha,                -MNU_ALPHADOT                  };

TO_QSPI const int16_t menu_PROGS[]       = { -MNU_RAM,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    -MNU_FLASH                    };

TO_QSPI const int16_t menu_VARS[]        = { -MNU_LINTS,                    -MNU_SINTS,                 -MNU_REALS,               -MNU_CPXS,             -MNU_STRINGS,                -MNU_MATRS,
                                             -MNU_DATES,                    -MNU_TIMES,                 -MNU_ANGLES,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

/*      Menu name                           <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
// Following menu is UPPER CASE for lower case: +36
TO_QSPI const int16_t menu_ALPHA_OMEGA[] = { ITM_ALPHA,                     ITM_BETA,                   ITM_GAMMA,                ITM_DELTA,             ITM_EPSILON,                 ITM_DIGAMMA,
                                             ITM_ZETA,                      ITM_ETA,                    ITM_THETA,                ITM_IOTA,              ITM_KAPPA,                   ITM_LAMBDA,
                                             ITM_MU,                        ITM_NU,                     ITM_XI,                   ITM_OMICRON,           ITM_PI,                      ITM_QOPPA,
                                             ITM_RHO,                       ITM_SIGMA,                  ITM_TAU,                  ITM_UPSILON,           ITM_PHI,                     ITM_CHI,
                                             ITM_PSI,                       ITM_OMEGA,                  ITM_SAMPI,                ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM modified greek sequence
                                             ITM_IOTA_DIALYTIKA,            ITM_NULL,                   ITM_NULL,                 ITM_UPSILON_DIALYTIKA, ITM_NULL,                    ITM_NULL                      };

TO_QSPI const int16_t menu_alpha_omega[] = { ITM_alpha,                     ITM_beta,                   ITM_gamma,                ITM_delta,             ITM_epsilon,                 ITM_digamma,
                                             ITM_zeta,                      ITM_eta,                    ITM_theta,                ITM_iota,              ITM_kappa,                   ITM_lambda,
                                             ITM_mu,                        ITM_nu,                     ITM_xi,                   ITM_omicron,           ITM_pi,                      ITM_qoppa,
                                             ITM_rho,                       ITM_sigma,                  ITM_tau,                  ITM_upsilon,           ITM_phi,                     ITM_chi,
                                             ITM_psi,                       ITM_omega,                  ITM_sampi,                ITM_NULL,              ITM_NULL,                    ITM_NULL,                        
                                             ITM_alpha_TONOS,               ITM_epsilon_TONOS,          ITM_eta_TONOS,            ITM_iotaTON,           ITM_iota_DIALYTIKA_TONOS,    ITM_NULL,                           //JM modified greek sequence
                                             ITM_iota_DIALYTIKA,            ITM_omicron_TONOS,          ITM_upsilon_TONOS,        ITM_upsilon_DIALYTIKA, ITM_upsilon_DIALYTIKA_TONOS, ITM_omega_TONOS               };

TO_QSPI const int16_t menu_AngleConv[]   = { ITM_DtoR,                      ITM_RtoD,                   ITM_DtoDMS,               ITM_DMStoD,            ITM_toDMS,                  ITM_toHMS,
                                             ITM_toDEG,                     ITM_toRAD,                  ITM_toGRAD,               ITM_toMULpi,           ITM_dotD,                   ITM_ms,                      //JM mod
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                   ITM_NULL                 };    //JM removed and Added H.MS

/*      Menu name                           <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
TO_QSPI const int16_t menu_UnitConv[]    = { -MNU_CONVE,                    -MNU_CONVP,                 ITM_YEARtoS,              -MNU_CONVFP,           -MNU_CONVM,                  -MNU_CONVX,
                                             ITM_CtoF,                      ITM_FtoC,                   ITM_StoYEAR,              ITM_NULL,              -MNU_CONVV,                  -MNU_CONVA,
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                             10000+ITM_PRtoDBc,             10000+ITM_DBtoPRc,          10000+ITM_NMtoLBFFTb,     10000+ITM_LBFFTtoNMb,  10000+ITM_FRtoDBc,           10000+ITM_DBtoFRc,
                                             30000+ITM_PRtoDBb,             30000+ITM_DBtoPRb,          20000+ITM_NMtoLBFFT,      20000+ITM_LBFFTtoNM,   30000+ITM_FRtoDBb,           30000+ITM_DBtoFRb,
                                             20000+ITM_PRtoDB,              20000+ITM_DBtoPR,           ITM_NULL,                 ITM_NULL,              20000+ITM_FRtoDB,            20000+ITM_DBtoFR              };

TO_QSPI const int16_t menu_ConvE[]       = { ITM_CALtoJ,                    ITM_JtoCAL,                 ITM_BTUtoJ,               ITM_JtoBTU,            ITM_WHtoJ,                   ITM_JtoWH                     };

TO_QSPI const int16_t menu_ConvP[]       = { ITM_HPEtoW,                    ITM_WtoHPE,                 ITM_HPUKtoW,              ITM_WtoHPUK,           ITM_HPMtoW,                  ITM_WtoHPM                    };

TO_QSPI const int16_t menu_ConvFP[]      = { ITM_LBFtoN,                    ITM_NtoLBF,                 ITM_PAtoBAR,              ITM_BARtoPA,           ITM_PSItoPA,                 ITM_PAtoPSI,
                                             10000+ITM_INCHHGtoPAb,         10000+ITM_PAtoINCHHGb,      10000+ITM_TORtoPAb,       10000+ITM_PAtoTORb,    10000+ITM_ATMtoPA,           10000+ITM_PAtoATM,
                                             20000+ITM_INCHHGtoPA,          20000+ITM_PAtoINCHHG,       20000+ITM_TORtoPA,        20000+ITM_PAtoTOR,     20000+ITM_ATMtoPAb,          20000+ITM_PAtoATMb,
                                             ITM_NULL,                      ITM_NULL,                   10000+ITM_MMHGtoPAb,      10000+ITM_PAtoMMHGb,   ITM_NULL,                    ITM_NULL,
                                             ITM_NULL,                      ITM_NULL,                   20000+ITM_MMHGtoPA,       20000+ITM_PAtoMMHG,    ITM_NULL,                    ITM_NULL                      };

TO_QSPI const int16_t menu_ConvM[]       = { ITM_LBStoKG,                   ITM_KGtoLBS,                ITM_CWTtoKG,              ITM_KGtoCWT,           ITM_OZtoG,                   ITM_GtoOZ,
                                             10000+ITM_STOtoKGb,            10000+ITM_KGtoSTOb,         10000+ITM_SCWtoKGb,       10000+ITM_KGtoSCWb,    10000+ITM_TRZtoGb,           10000+ITM_GtoTRZb,
                                             20000+ITM_STOtoKG,             20000+ITM_KGtoSTO,          20000+ITM_SCWtoKG,        20000+ITM_KGtoSCW,     20000+ITM_TRZtoG,            20000+ITM_GtoTRZ,
                                             ITM_TONtoKG,                   ITM_KGtoTON,                10000+ITM_STtoKGc,        10000+ITM_KGtoSTc,     10000+ITM_CARATtoGb,         10000+ITM_GtoCARATb,
                                             10000+ITM_LIANGtoKGb,          10000+ITM_KGtoLIANGb,       30000+ITM_STtoKGb,        30000+ITM_KGtoSTb,     20000+ITM_CARATtoG,          20000+ITM_GtoCARAT,
                                             20000+ITM_LIANGtoKG,           20000+ITM_KGtoLIANG,        20000+ITM_STtoKG,         20000+ITM_KGtoST,      ITM_JINtoKG,                 ITM_KGtoJIN                   };

TO_QSPI const int16_t menu_ConvX[]       = { ITM_AUtoM,                     ITM_MtoAU,                  ITM_LYtoM,                ITM_MtoLY,             ITM_PCtoM,                   ITM_MtoPC,
                                             ITM_MItoKM,                    ITM_KMtoMI,                 ITM_NMItoKM,              ITM_KMtoNMI,           ITM_FTtoM,                   ITM_MtoFT,
                                             ITM_INCHtoMM,                  ITM_MMtoINCH,               ITM_NULL,                 ITM_NULL,              ITM_YDtoM,                   ITM_MtoYD,
                                             ITM_LItoM,                     ITM_MtoLI,                  ITM_YINtoM,               ITM_MtoYIN,            10000+ITM_ZHANGtoMb,         10000+ITM_MtoZHANGb,
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              20000+ITM_ZHANGtoM,          20000+ITM_MtoZHANG,
                                             ITM_CHItoM,                    ITM_MtoCHI,                 ITM_CUNtoM,               ITM_MtoCUN,            ITM_FENtoM,                  ITM_MtoFEN,
                                             10000+ITM_FATHOMtoMb,          10000+ITM_MtoFATHOMb,       10000+ITM_POINTtoMMb,     10000+ITM_MMtoPOINTb,  10000+ITM_FTUStoMc,          10000+ITM_MtoFTUSc,
                                             30000+ITM_FATHOMtoM,           30000+ITM_MtoFATHOM,        30000+ITM_POINTtoMM,      30000+ITM_MMtoPOINT,   30000+ITM_FTUStoMb,          30000+ITM_MtoFTUSb,
                                             20000+ITM_FATHOMtoMc,          20000+ITM_MtoFATHOMc,       20000+ITM_POINTtoMMc,     20000+ITM_MMtoPOINTc,  20000+ITM_FTUStoM,           20000+ITM_MtoFTUS             };

TO_QSPI const int16_t menu_ConvA[]       = { 10000+ITM_ACtoHAb,             10000+ITM_HAtoACb,          ITM_HECTAREtoM2,          ITM_M2toHECTARE,       10000+ITM_ACUStoHAb,         10000+ITM_HAtoACUSb,
                                             20000+ITM_ACtoHA,              20000+ITM_HAtoAC,           ITM_NULL,                 ITM_NULL,              20000+ITM_ACUStoHA,          20000+ITM_HAtoACUS,
                                             ITM_MUtoM2,                    ITM_M2toMU,                 ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

/*      Menu name                           <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
TO_QSPI const int16_t menu_ConvV[]       = { ITM_GLUKtoL,                   ITM_LtoGLUK,                ITM_QTtoL,                ITM_LtoQT,             ITM_GLUStoL,                 ITM_LtoGLUS,
                                             10000+ITM_FZUKtoMLb,           10000+ITM_MLtoFZUKb,        10000+ITM_BARRELtoM3b,    10000+ITM_M3toBARRELb, 10000+ITM_FZUStoMLb,         10000+ITM_MLtoFZUSb,
                                             20000+ITM_FZUKtoML,            20000+ITM_MLtoFZUK,         20000+ITM_BARRELtoM3,     20000+ITM_M3toBARREL,  20000+ITM_FZUStoML,          20000+ITM_MLtoFZUS            };

TO_QSPI const int16_t menu_alphaFN[]     = { ITM_XtoALPHA,                  ITM_ALPHARL,                ITM_ALPHARR,              ITM_ALPHASL,           ITM_ALPHASR,                 ITM_ALPHAtoX,
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_ALPHALENG,               ITM_ALPHAPOS,
                                             ITM_FBR,                       ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

/*      Menu name                           <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
TO_QSPI const int16_t menu_alphaMATH[]   = { ITM_LESS_THAN,                 ITM_LESS_EQUAL,             ITM_EQUAL,                ITM_ALMOST_EQUAL,      ITM_GREATER_EQUAL,           ITM_GREATER_THAN,
                                             ITM_LEFT_CURLY_BRACKET,        ITM_LEFT_SQUARE_BRACKET,    ITM_LEFT_PARENTHESIS,     ITM_RIGHT_PARENTHESIS, ITM_RIGHT_SQUARE_BRACKET,    ITM_RIGHT_CURLY_BRACKET,
                                             ITM_PROD_SIGN,                 ITM_OBELUS,                 ITM_INTEGRAL_SIGN,        ITM_SUB_INFINITY,      ITM_SUP_INFINITY,            ITM_INFINITY,
                                             ITM_NOT,                       ITM_AND,                    ITM_OR,                   ITM_NOT_EQUAL,         ITM_PIPE,                    ITM_AMPERSAND,
                                             ITM_MEASURED_ANGLE,            ITM_RIGHT_ANGLE,            ITM_PERPENDICULAR,        ITM_CUBE_ROOT,         ITM_SQUARE_ROOT,             ITM_xTH_ROOT,
                                             ITM_x_BAR,                     ITM_y_BAR,                  ITM_x_CIRC,               ITM_y_CIRC,            ITM_x_UNDER_ROOT,            ITM_y_UNDER_ROOT,
                                             ITM_COLON_EQUALS,              ITM_ESTIMATES,              ITM_CORRESPONDS_TO,       ITM_SUB_E_OUTLINE,     ITM_COMPLEX_C,               ITM_REAL_R,
                                             ITM_SUN,                       ITM_SUB_SUN,                ITM_SUB_EARTH,            ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                             ITM_PLUS_MINUS,                ITM_CIRCUMFLEX,             ITM_SUP_T,                ITM_SUP_MINUS_1,       ITM_PLANCK_2PI,              ITM_NULL                      };

/*      Menu name                           <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                          <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
TO_QSPI const int16_t menu_alphaDot[]    = { ITM_EXCLAMATION_MARK,          ITM_SEMICOLON,              ITM_UNDERSCORE,           ITM_QUOTE,             ITM_DOUBLE_QUOTE,            ITM_AT,
                                             ITM_INVERTED_EXCLAMATION_MARK, ITM_INVERTED_QUESTION_MARK, ITM_SECTION,              ITM_NULL,              ITM_TILDE,                   ITM_BACK_SLASH,
                                             ITM_DOLLAR,                    ITM_EURO,                   ITM_PERCENT,              ITM_AMPERSAND,         ITM_POUND,                   ITM_YEN,
                                             ITM_LEFT_ARROW,                ITM_UP_ARROW,               ITM_SERIAL_IO,            ITM_DOWN_ARROW,        ITM_RIGHT_ARROW,             ITM_PGM_BEGIN,
                                             ITM_LEFT_DOUBLE_ANGLE,         ITM_RIGHT_DOUBLE_ANGLE,     ITM_EMPTY_SET,            ITM_WATCH,             ITM_BULLET,                  ITM_ASTERISK,
                                             ITM_HOURGLASS,                 ITM_NEG_EXCLAMATION_MARK,   ITM_USER_MODE,            ITM_BATTERY,           ITM_NULL,                    ITM_SUP_ASTERISK              };

TO_QSPI const int16_t menu_1stDeriv[]    = { ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
TO_QSPI const int16_t menu_2ndDeriv[]    = { ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
TO_QSPI const int16_t menu_Sf[]          = { ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
TO_QSPI const int16_t menu_Solver[]      = { ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
TO_QSPI const int16_t menu_IO[]          = { ITM_LOAD,                      ITM_LOADP,                  ITM_LOADR,                ITM_LOADSS,            ITM_LOADV,                   ITM_LOADSIGMA,
                                             ITM_BEEP,                      ITM_TONE,                   ITM_SAVE,                 -MNU_PRINT,            ITM_RECV,                    ITM_SEND                      };
TO_QSPI const int16_t menu_PRINT[]       = { ITM_PRINTERX,                  ITM_PRINTERR,               ITM_PRINTERSIGMA,         ITM_PRINTERADV,        ITM_PRINTERLCD,              ITM_PRINTERPROG,
                                             ITM_PRINTERSTK,                ITM_PRINTERREGS,            ITM_PRINTERUSER,          ITM_PRINTERTAB,        ITM_PRINTERHASH,             ITM_PRINTERCHAR,
                                             ITM_NULL,                      ITM_P_ALLREGS,              ITM_NULL,                 ITM_PRINTERWIDTH,      ITM_PRINTERDLAY,             ITM_PRINTERMODE               };
TO_QSPI const int16_t menu_Tam[]         = { ITM_INDIRECTION,               -MNU_VAR,                   ITM_REG_X,                ITM_REG_Y,             ITM_REG_Z,                   ITM_REG_T                     };
TO_QSPI const int16_t menu_TamCmp[]      = { ITM_INDIRECTION,               -MNU_VAR,                   ITM_REG_X,                ITM_REG_Y,             ITM_REG_Z,                   ITM_REG_T,
                                             ITM_0P,                        ITM_1P,                     ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
TO_QSPI const int16_t menu_TamFlag[]     = { ITM_INDIRECTION,               -MNU_SYSFL,                 ITM_REG_X,                ITM_REG_Y,             ITM_REG_Z,                   ITM_REG_T                     };
TO_QSPI const int16_t menu_TamStoRcl[]   = { ITM_INDIRECTION,               -MNU_VAR,                   ITM_REG_X,                ITM_REG_Y,             ITM_REG_Z,                   ITM_REG_T,
                                             ITM_Config,                    ITM_Stack,                  ITM_NULL,                 ITM_NULL,              ITM_Max,                     ITM_Min,
                                             ITM_dddEL,                     ITM_dddIJ,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
TO_QSPI const int16_t menu_TamShuffle[]  = { ITM_REG_X,                     ITM_REG_Y,                  ITM_REG_Z,                ITM_REG_T,             ITM_NULL,                    ITM_NULL,                     };
TO_QSPI const int16_t menu_TamLabel[]    = { ITM_INDIRECTION,               -MNU_PROG,                  ITM_REG_X,                ITM_REG_Y,             ITM_REG_Z,                   ITM_REG_T                     };

TO_QSPI const int16_t menu_BASE[]        = { ITM_IP,                        ITM_HASH_JM,                ITM_2HEX,                 ITM_2DEC,              ITM_2OCT,                    ITM_2BIN,                           //JM BASE MENU ADDED
                                             -MNU_BITS,                     -MNU_INTS,                  ITM_LOGICALAND,           ITM_LOGICALOR,         ITM_LOGICALXOR,              ITM_LOGICALNOT,    
                                             ITM_CB_LEADING_ZERO,           ITM_WSIZE,                  ITM_WS64,                 ITM_WS32,              ITM_WS16,                    ITM_WS8,

                                             ITM_LOGICALNOT            , ITM_UNSIGN, ITM_FB00+ 3, ITM_FB00+ 2, ITM_FB00+ 1, ITM_FB00+0,
                                             ITM_1COMPL                , ITM_2COMPL, ITM_FB00+ 7, ITM_FB00+ 6, ITM_FB00+ 5, ITM_FB00+4,
                                             ITM_CB_LEADING_ZERO       , ITM_SIGNMT, ITM_FB00+11, ITM_FB00+10, ITM_FB00+ 9, ITM_FB00+8,
                                             ITM_LOGICALNOT            , ITM_UNSIGN, ITM_FB00+15, ITM_FB00+14, ITM_FB00+13, ITM_FB00+12,
                                             ITM_1COMPL                , ITM_2COMPL, ITM_FB00+19, ITM_FB00+18, ITM_FB00+17, ITM_FB00+16,
                                             ITM_CB_LEADING_ZERO       , ITM_SIGNMT, ITM_FB00+23, ITM_FB00+22, ITM_FB00+21, ITM_FB00+20,
                                             ITM_LOGICALNOT            , ITM_UNSIGN, ITM_FB00+27, ITM_FB00+26, ITM_FB00+25, ITM_FB00+24,
                                             ITM_1COMPL                , ITM_2COMPL, ITM_FB00+31, ITM_FB00+30, ITM_FB00+29, ITM_FB00+28,
                                             ITM_CB_LEADING_ZERO       , ITM_SIGNMT, ITM_FB00+35, ITM_FB00+34, ITM_FB00+33, ITM_FB00+32,
                                             ITM_LOGICALNOT            , ITM_UNSIGN, ITM_FB00+39, ITM_FB00+38, ITM_FB00+37, ITM_FB00+36,
                                             ITM_1COMPL                , ITM_2COMPL, ITM_FB00+43, ITM_FB00+42, ITM_FB00+41, ITM_FB00+40,
                                             ITM_CB_LEADING_ZERO       , ITM_SIGNMT, ITM_FB00+47, ITM_FB00+46, ITM_FB00+45, ITM_FB00+44,
                                             ITM_LOGICALNOT            , ITM_UNSIGN, ITM_FB00+51, ITM_FB00+50, ITM_FB00+49, ITM_FB00+48,
                                             ITM_1COMPL                , ITM_2COMPL, ITM_FB00+55, ITM_FB00+54, ITM_FB00+53, ITM_FB00+52,
                                             ITM_CB_LEADING_ZERO       , ITM_SIGNMT, ITM_FB00+59, ITM_FB00+58, ITM_FB00+57, ITM_FB00+56,
                                             ITM_LOGICALNOT            , ITM_UNSIGN, ITM_FB00+63, ITM_FB00+62, ITM_FB00+61, ITM_FB00+60,
                                             ITM_1COMPL                , ITM_2COMPL, ITM_NULL   , ITM_NULL   ,ITM_NULL    , ITM_NULL  ,
                                             ITM_CB_LEADING_ZERO       , ITM_SIGNMT, ITM_NULL   , ITM_NULL   ,ITM_NULL    , ITM_NULL  ,
                                             
                                             ITM_SL1,     ITM_SR1,     ITM_RL1,      ITM_RR1,      ITM_RL,     ITM_RR,                 //JM POC BASE2 vv
                                             ITM_S06,     ITM_S08,     ITM_S16,      ITM_S32,      ITM_S64,    ITM_FWORD,     
                                             ITM_U06,     ITM_U08,     ITM_U16,      ITM_U32,      ITM_U64,    ITM_FBYTE               //JM POC BASE2 ^^

                                                            };    //JM BASE MENU ADDED



TO_QSPI const int16_t menu_EE[]          = { ITM_CONSTpi,                   ITM_op_j,                   ITM_SQUARE,               ITM_op_a,              ITM_op_a2,                   ITM_CLSTK,                          //JM EE
                                             ITM_EE_D2Y,                    ITM_EE_Y2D,                 ITM_EE_A2S,               ITM_EE_S2A,            ITM_PARALLEL,                -MNU_CPX,                           //JM EE
                                             ITM_DEG,                       ITM_DEG2,                   ITM_RAD,                  ITM_RAD2,              ITM_RECT,                    ITM_POLAR,                          //JM EE

                                             ITM_EE_D2Y,                    ITM_EE_Y2D,                 ITM_EE_A2S,               ITM_EE_S2A,            ITM_EE_EXP_TH,                ITM_CLSTK,                         //JM EE
                                             ITM_EE_STO_V_I,                ITM_EE_STO_IR,              ITM_EE_STO_V_Z,           ITM_EE_X2BAL,          ITM_PARALLEL,                -MNU_CPX,                           //JM EE
                                             ITM_EE_STO_Z,                  ITM_EE_RCL_Z,               ITM_EE_STO_V,             ITM_EE_RCL_V,          ITM_EE_STO_I,                ITM_EE_RCL_I                  };    //JM EE

#ifdef INLINE_TEST
TO_QSPI const int16_t menu_Inl_Tst[]     = { ITM_TEST,                      ITM_NULL,                   ITM_NULL,                 ITM_SYS_FREE_RAM,      ITM_GET_TEST_BS,             ITM_SET_TEST_BS               };    //dr
#endif


TO_QSPI const int16_t menu_ASN_N[]       = { ITM_U_KEY_USER,                ITM_U_KEY_SIGMA,            ITM_U_KEY_CC,             ITM_U_KEY_PRGM,        ITM_U_KEY_MM,                ITM_U_KEY_ALPHA,                    //JM USER NAORMAL MODE
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM USER NAORMAL MODE
                                             ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_SH_NORM_E,               ITM_GET_NORM_E                };    //JM USER NAORMAL MODE

TO_QSPI const int16_t menu_ASN_U[]       = { ITM_USER_ALPHA,                ITM_USER_CC,                ITM_USER_GSHFT,           ITM_USER_MYM,          ITM_USER_DRG,                ITM_NULL,
                                             ITM_USER_PRGM,                 ITM_USER_USER,              ITM_USER_HOME,            ITM_USER_SIGMAPLUS,    ITM_NULL,                    ITM_NULL                     };


#ifdef SAVE_SPACE_DM42
  #define CC_C43 ITM_NULL
  #define CC_V43 ITM_NULL
  #define CC_V43M ITM_NULL
#else
  #define CC_C43 ITM_USER_C43
  #define CC_V43 ITM_USER_V43
  #define CC_V43M ITM_USER_V43MIN
#endif                                             

TO_QSPI const int16_t menu_ASN[]         = { ITM_USER_DEFAULTS,             ITM_USER_COMPLEX,           CC_C43,                   CC_V43,                ITM_USER_DM42,              ITM_USER_RESET,         /*ITM_JM_ASN*/
                                             ITM_NULL,                      ITM_NULL,                   ITM_USER_SHIFTS,          CC_V43M,               ITM_USER_WP43S,             ITM_JM_SEEK,
                                             -MNU_ASN_U,                    ITM_ASSIGN,                 ITM_NULL,                 ITM_NULL,              ITM_NULL,                   -MNU_ASN_N,
                                                                          
                                             USER_PRIM00U,                         USER_PRIM01U,                      USER_PRIM02U,                    USER_PRIM03U,                 USER_PRIM04U,                       USER_PRIM05U,  //JM USER
                                             USER_SFTf00U,                         USER_SFTf01U,                      USER_SFTf02U,                    USER_SFTf03U,                 USER_SFTf04U,                       USER_SFTf05U,  //JM USER
                                             USER_SFTg00U,                         USER_SFTg01U,                      USER_SFTg02U,                    USER_SFTg03U,                 USER_SFTg04U,                       USER_SFTg05U,  //JM USER
                                                                                                                                   
                                             USER_PRIM06U,                         USER_PRIM07U,                      USER_PRIM08U,                    USER_PRIM09U,                 USER_PRIM10U,                       USER_PRIM11U,  //JM USER
                                             USER_SFTf06U,                         USER_SFTf07U,                      USER_SFTf08U,                    USER_SFTf09U,                 USER_SFTf10U,                       USER_SFTf11U,  //JM USER
                                             USER_SFTg06U,                         USER_SFTg07U,                      USER_SFTg08U,                    USER_SFTg09U,                 USER_SFTg10U,                       USER_SFTg11U,  //JM USER
                                                                                                                                   
                                             USER_PRIM12U,                         USER_PRIM13U,                      USER_PRIM14U,                    USER_PRIM15U,                 USER_PRIM16U,                       ITM_NULL,  //JM USER
                                             USER_SFTf12U,                         USER_SFTf13U,                      USER_SFTf14U,                    USER_SFTf15U,                 USER_SFTf16U,                       ITM_NULL,  //JM USER
                                             USER_SFTg12U,                         USER_SFTg13U,                      USER_SFTg14U,                    USER_SFTg15U,                 USER_SFTg16U,                       ITM_NULL,  //JM USER
/*                                           USER_PRIM17U,                         USER_PRIM18U,                      USER_PRIM19U,                    USER_PRIM20U,                 USER_PRIM21U,                       ITM_NULL,  //JM USER
                                             USER_SFTf17U,                         USER_SFTf18U,                      USER_SFTf19U,                    USER_SFTf20U,                 USER_SFTf21U,                       ITM_NULL,  //JM USER
                                             USER_SFTg17U,                         USER_SFTg18U,                      USER_SFTg19U,                    USER_SFTg20U,                 USER_SFTg21U,                       ITM_NULL,  //JM USER
                                             USER_PRIM22U,                         USER_PRIM23U,                      USER_PRIM24U,                    USER_PRIM25U,                 USER_PRIM26U,                       ITM_NULL,  //JM USER
                                             USER_SFTf22U,                         USER_SFTf23U,                      USER_SFTf24U,                    USER_SFTf25U,                 USER_SFTf26U,                       ITM_NULL,  //JM USER
                                             USER_SFTg22U,                         USER_SFTg23U,                      USER_SFTg24U,                    USER_SFTg25U,                 USER_SFTg26U,                       ITM_NULL,  //JM USER
                                             USER_PRIM27U,                         USER_PRIM28U,                      USER_PRIM29U,                    USER_PRIM30U,                 USER_PRIM31U,                       ITM_NULL,  //JM USER
                                             USER_SFTf27U,                         USER_SFTf28U,                      USER_SFTf29U,                    USER_SFTf30U,                 USER_SFTf31U,                       ITM_NULL,  //JM USER
                                             USER_SFTg27U,                         USER_SFTg28U,                      USER_SFTg29U,                    USER_SFTg30U,                 USER_SFTg31U,                       ITM_NULL,  //JM USER
                                             USER_PRIM32U,                         USER_PRIM33U,                      USER_PRIM34U,                    USER_PRIM35U,                 USER_PRIM36U,                       ITM_NULL,  //JM USER
                                             USER_SFTf32U,                         USER_SFTf33U,                      USER_SFTf34U,                    USER_SFTf35U,                 USER_SFTf36U,                       ITM_NULL,  //JM USER
                                             USER_SFTg32U,                         USER_SFTg33U,                      USER_SFTg34U,                    USER_SFTg35U,                 USER_SFTg36U,                       ITM_NULL   //JM USER
 */
      };

TO_QSPI const int16_t menu_XEQ[]         = { ITM_X_P1,                      ITM_X_P2,                   ITM_X_P3,                 ITM_X_P4,              ITM_X_P5,                    ITM_X_P6,
                                             ITM_X_f1,                      ITM_X_f2,                   ITM_X_f3,                 ITM_X_f4,              ITM_X_f5,                    ITM_X_f6,
                                             ITM_X_g1,                      ITM_X_g2,                   ITM_X_g3,                 ITM_X_g4,              ITM_X_g5,                    ITM_X_g6   };



//##################################################################################################################################################################################################################################
//#################################  -1     : softmenu contents not dynamically changing 
//#################################   0- 36 : softmenu contents copied from key number Primary 0-36
//################################# 100-136 : softmenu contents copied from key number f 0-36
//################################# 200-236 : softmenu contents copied from key number g 0-36
TO_QSPI const int16_t menu_A_HOME[360]   = { -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1,               //HOME  0
                                             -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1,               //HOME -1  (Dn = -)
                                             -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1,               //HOME -2
        
                                             -1,32,33,34,35,36,        -1 ,132,133,134,135,136,  -1 ,232,233,234,235,236,         //REPLICA ROW 7
                                             -1,27,28,29,30,31,        -1 ,127,128,129,130,131,  -1 ,227,228,229,230,231,         //REPLICA ROW 6
                                             -1,22,23,24,25,26,        -1 ,122,123,124,125,126,  -1 ,222,223,224,225,226,         //REPLICA ROW 5
                                             -1,17,18,19,20,21,        -1 ,117,118,119,120,121,  -1 ,217,218,219,220,221,         //REPLICA ROW 4
                                             -1,12,13,14,15,16,        -1 ,112,113,114,115,116,  -1 ,212,213,214,215,216,         //REPLICA ROW 3
                                             6 , 7, 8, 9,10,11,        106,107,108,109,110,111,  206,207,208,209,210,211,         //REPLICA ROW 2
                                             0 , 1, 2, 3, 4, 5,        100,101,102,103,104,105,  200,201,202,203,204,205,         //REPLICA ROW 1
        
                                             233,217,218,219,220,221,  234,222,223,224,225,226,  235,236,228,229,230,231,         //SUMMARY 2    Change all to -1 to have the fixed system
                                             200,201,202,203,204,205,  206,207,208,209,210,211,  232,212,213,214,215,216,         //SUMMARY 1    Change all to -1 to have the fixed system
        
                                             -1,-1,-1,-1,-1,-1,        -1 ,132,133,134,135,136,  -1 ,232,233,234,235,136,         //FIXED ROW 7
                                             -1,-1,-1,-1,-1,-1,        -1 ,127,128,129,130,131,  -1 ,227,228,229,230,231,         //FIXED ROW 6
                                             -1,-1,-1,-1,-1,-1,        -1 ,122,123,124,125,126,  -1 ,222,223,224,225,226,         //FIXED ROW 5
                                             -1,-1,-1,-1,-1,-1,        -1 ,117,118,119,120,121,  -1 ,217,218,219,220,221,         //FIXED ROW 4
                                             -1,-1,-1,-1,-1,-1,        -1 ,112,113,114,115,116,  -1 ,212,213,214,215,216,         //FIXED ROW 3
                                             -1,-1,-1,-1,-1,-1,        106,107,108,109,110,111,  206,207,208,209,210,211,         //FIXED ROW 2
                                             -1,-1,-1,-1,-1,-1,        100,101,102,103,104,105,  200,201,202,203,204,205,         //FIXED ROW 1
        
                                             -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1        };      //HOME  +1  (Up = +)
        

TO_QSPI const int16_t menu_HOME[360]     = { //JMHOMEDEMO: NOTE REMOVE CONST TO MAKE JMHOME DEMO WORK
/*HOME0 */           
/*0x18*/                                     ITM_SIGMAMINUS,      ITM_YX,          ITM_SQUARE,        ITM_10x,          ITM_EXP,          ITM_CC,                        //JM HOME
                                             ITM_MAGNITUDE,       ITM_ANGLE,       ITM_PARALLEL,      ITM_XFACT,        ITM_toREC2,       ITM_toPOL2,                        //JM HOME
                                             -MNU_TRI,             ITM_DEG,         ITM_RAD,           ITM_op_j,         ITM_RECT,         ITM_POLAR,                          //JM HOME
/*HOME-1*/                                                                                                                                                                     //JM HOME
/*1*18*/                                     ITM_SIGMAMINUS,      ITM_YX,          ITM_SQUARE,        ITM_10x,          ITM_EXP,          ITM_CC,                        //JM HOME
                                             ITM_RI,              ITM_IP,          ITM_NULL,          ITM_XFACT,        -MNU_BASE,        -MNU_INTS,                          //JM HOME
                                             ITM_SF,              ITM_CF,          ITM_RL,            ITM_RR,           ITM_RMD,          ITM_IDIV,                           //JM HOME
/*HOME-2*/                                                                                                                                                                     //JM HOME
/*2*18*/                                     ITM_SIGMAMINUS,      ITM_YX,          ITM_SQUARE,        ITM_10x,          ITM_EXP,          ITM_CC,                        //JM HOME
                                             -MNU_TRI,            -MNU_CPX,        -MNU_EXP,          -MNU_XEQ,         -MNU_MyMenu,      -MNU_EE,                            //JM HOME
                                             -MNU_CONST,          ITM_RBR,         ITM_FLGSV,         ITM_H_SUMRY,      ITM_H_REPLCA,     ITM_H_FIXED,                        //JM HOME
                                       
// 03 -- 09 HAAKON SUGGESTIONS OF FULL REPLICA
#ifdef JM_LAYOUT_1A  //JM UPDATED TO LAYOUT 1C. OPTIMAL. SIMULATOR.
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
                                             ITM_NULL,                 ITM_EXIT1,                ITM_0,                    ITM_PERIOD,               ITM_RS,                   ITM_ADD,                  
                                             ITM_NULL,                 ITM_OFF,                  ITM_VIEW,                 ITM_SHOW,                 ITM_PR,                   -MNU_CATALOG,             
                                             ITM_NULL,                 -MNU_PRINT,               ITM_TIMER,                -MNU_INFO,                -MNU_PFN,                 -MNU_IO,                  

                                             ITM_NULL,                 KEY_fg,                   ITM_1,                    ITM_2,                    ITM_3,                    ITM_SUB,                  
                                             ITM_NULL,                 ITM_NULL,                 ITM_ASSIGN,               ITM_USERMODE,             -MNU_PARTS,               -MNU_FIN,                 
                                             ITM_NULL,                 ITM_NULL,                 -MNU_ASN,                 -MNU_LOOP,                -MNU_TEST,                -MNU_ALPHAFN,             

                                             ITM_NULL,                 ITM_DOWN1,                ITM_4,                    ITM_5,                    ITM_6,                    ITM_MULT,                 
                                             ITM_NULL,                 ITM_SST,                  -MNU_BASE,                -MNU_ANGLECONV,           -MNU_FLAGS,               -MNU_PROB,                
                                             ITM_NULL,                 ITM_FLGSV,                -MNU_CLK,                 -MNU_UNITCONV,            -MNU_BITS,                -MNU_INTS,                

                                             ITM_NULL,                 ITM_UP1,                  ITM_7,                    ITM_8,                    ITM_9,                    ITM_DIV,                  
                                             ITM_NULL,                 ITM_BST,                  -MNU_EQN,                 -MNU_ADV,                 -MNU_MATX,                -MNU_STAT,                
                                             ITM_NULL,                 ITM_RBR,                  -MNU_HOME,                -MNU_CONST,               -MNU_XFN,                 -MNU_SUMS,                

                                             ITM_NULL,                 ITM_ENTER,                ITM_XexY,                 ITM_CHS,                  ITM_EXPONENT,             ITM_BACKSPACE,            
                                             ITM_NULL,                 KEY_COMPLEX,              ITM_LASTX,                -MNU_MODE,                -MNU_DSP,                 ITM_UNDO,                 
                                             ITM_NULL,                 -MNU_CPX,                 ITM_Rup,                  -MNU_STK,                 -MNU_EXP,                 -MNU_CLR,                 

                                             ITM_STO,                  ITM_RCL,                  ITM_Rdown,                ITM_sin,                  ITM_cos,                  ITM_tan,                  
                                             ITM_MAGNITUDE,            ITM_PC,                   ITM_CONSTpi,              ITM_arcsin,               ITM_arccos,               ITM_arctan,               
                                             ITM_ANGLE,                ITM_DELTAPC,              ITM_XTHROOT,              ITM_GTO,                  ITM_LBL,                  ITM_RTN,                  

                                             ITM_SIGMAPLUS,            ITM_1ONX,                 ITM_SQUAREROOTX,          ITM_LOG10,                ITM_LN,                   ITM_XEQ,                  
                                             ITM_RI/*ITM_RND*/,        ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_AIM,                  
                                             ITM_TGLFRT,               ITM_HASH_JM/*ITM_toINT*/, ITM_ms,                   ITM_dotD,                 ITM_toREC2,               ITM_toPOL2,               
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
#endif //JM END OF LAYOUT 1a.


#ifdef JM_LAYOUT_2_DM42_STRICT //JM DM42 STRICT. NAME CHANGE FROM LAYOUT 2 TO LAYOUT 42C
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
                                             ITM_NULL,                 ITM_EXIT1,                ITM_0,                    ITM_PERIOD,               ITM_RS,                   ITM_ADD,                  
                                             ITM_NULL,                 ITM_OFF,                  -MNU_HOME,                ITM_SHOW,                 ITM_PR,                   -MNU_CATALOG,             
                                             ITM_NULL,                 -MNU_PRINT,               ITM_TIMER,                ITM_VIEW,                 -MNU_CLK,                 -MNU_PARTS,               

                                             ITM_NULL,                 KEY_fg,                   ITM_1,                    ITM_2,                    ITM_3,                    ITM_SUB,                  
                                             ITM_NULL,                 ITM_NULL,                 ITM_ASSIGN,               ITM_USERMODE,             -MNU_PFN,                 -MNU_IO,                  
                                             ITM_NULL,                 ITM_NULL,                 -MNU_ASN,                 -MNU_LOOP,                -MNU_TEST,                -MNU_ALPHAFN,             

                                             ITM_NULL,                 ITM_DOWN1,                ITM_4,                    ITM_5,                    ITM_6,                    ITM_MULT,                 
                                             ITM_NULL,                 ITM_SST,                  -MNU_BASE,                -MNU_ANGLECONV,           -MNU_FLAGS,               -MNU_PROB,                
                                             ITM_NULL,                 ITM_FLGSV,                -MNU_FIN,                 -MNU_UNITCONV,            -MNU_BITS,                -MNU_INTS,                

                                             ITM_NULL,                 ITM_UP1,                  ITM_7,                    ITM_8,                    ITM_9,                    ITM_DIV,                  
                                             ITM_NULL,                 ITM_BST,                  -MNU_EQN,                 -MNU_ADV,                 -MNU_MATX,                -MNU_STAT,                
                                             ITM_NULL,                 ITM_RBR,                  -MNU_INFO,                -MNU_CONST,               -MNU_XFN,                 -MNU_SUMS,                

                                             ITM_NULL,                 ITM_ENTER,                ITM_XexY,                 ITM_CHS,                  ITM_EXPONENT,             ITM_BACKSPACE,            
                                             ITM_NULL,                 ITM_AIM,                  ITM_LASTX,                -MNU_MODE,                -MNU_DSP,                 -MNU_CLR,                 
                                             ITM_NULL,                 -MNU_CPX,                 ITM_Rup,                  -MNU_STK,                 -MNU_EXP,                 ITM_UNDO,                 

                                             ITM_STO,                  ITM_RCL,                  ITM_Rdown,                ITM_sin,                  ITM_cos,                  ITM_tan,                  
                                             KEY_COMPLEX,              ITM_PC,                   ITM_CONSTpi,              ITM_arcsin,               ITM_arccos,               ITM_arctan,               
                                             ITM_MAGNITUDE,            ITM_ANGLE,                ITM_XTHROOT,              ITM_CC,                   ITM_LBL,                  ITM_RTN,                  

                                             ITM_SIGMAPLUS,            ITM_1ONX,                 ITM_SQUAREROOTX,          ITM_LOG10,                ITM_LN,                   ITM_XEQ,                  
                                             ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_GTO,                  
                                             ITM_TGLFRT,               ITM_HASH_JM,              ITM_ms,                   ITM_dotD,                 ITM_toREC2,               ITM_toPOL2,               
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
#endif //JM END OF LAYOUT 2 DM42 STRICT.



// 10 - 11 NIGEL SUGGESTED 36 CELLS OF ALL THE BLUE ONES                                                                                                                                                                         //JM HOME
/* g       */                                ITM_TIMER,            ITM_RBR,          -MNU_INFO,        -MNU_CONST,       -MNU_XFN,         -MNU_SUMS,     
/* g       */                                ITM_VIEW,             ITM_STATUS,       -MNU_FIN,         -MNU_UNITCONV,    -MNU_BITS,        -MNU_INTS,    
/* g       */                                -MNU_CLK,             -MNU_PARTS,       -MNU_ASN,         -MNU_LOOP,        -MNU_TEST,        -MNU_ALPHAFN,  
         
/* g       */                                ITM_TGLFRT,           ITM_HASH_JM,      ITM_ms,           ITM_dotD,         ITM_toREC2,       ITM_toPOL2,    
/* g       */                                ITM_MAGNITUDE,        ITM_ANGLE,        ITM_XTHROOT,      ITM_CC,           ITM_LBL,          ITM_RTN,      
/* g       */                                -MNU_PRINT,           -MNU_CPX,         ITM_Rup,          -MNU_STK,         -MNU_EXP,         ITM_UNDO,       


// 12 - 18 JACO PROPOSAL WITH FIXED BUTTONS RUNG #1, ALL F-functions on RUNG #2, ALL G-functions on RUNG #3
//
//################################################################################################
 #ifdef JM_LAYOUT_1A  //JM UPDATED TO LAYOUT 1C. OPTIMAL. SIMULATOR.
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
                                            ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_XFACT, 
                                            ITM_EXIT1,                ITM_OFF,                  ITM_VIEW,                 ITM_SHOW,                 ITM_PR,                   -MNU_CATALOG,             
                                            ITM_EXIT1,                -MNU_PRINT,               ITM_TIMER,                -MNU_INFO,                -MNU_PFN,                 -MNU_IO,                  
                
                                            ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_XFACT, 
                                            KEY_fg,                   ITM_NULL,                 ITM_ASSIGN,               ITM_USERMODE,             -MNU_PARTS,               -MNU_FIN,                 
                                            KEY_fg,                   ITM_NULL,                 -MNU_ASN,                 -MNU_LOOP,                -MNU_TEST,                -MNU_ALPHAFN,             
                
                                            ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_XFACT, 
                                            ITM_DOWN1,                ITM_SST,                  -MNU_BASE,                -MNU_ANGLECONV,           -MNU_FLAGS,               -MNU_PROB,                
                                            ITM_DOWN1,                ITM_FLGSV,                -MNU_CLK,                 -MNU_UNITCONV,            -MNU_BITS,                -MNU_INTS,                
                
                                            ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_XFACT, 
                                            ITM_UP1,                  ITM_BST,                  -MNU_EQN,                 -MNU_ADV,                 -MNU_MATX,                -MNU_STAT,                
                                            ITM_UP1,                  ITM_RBR,                  -MNU_HOME,                -MNU_CONST,               -MNU_XFN,                 -MNU_SUMS,                
                
                                            ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_XFACT, 
                                            ITM_ENTER,                KEY_COMPLEX,              ITM_LASTX,                -MNU_MODE,                -MNU_DSP,                 ITM_UNDO,                 
                                            ITM_ENTER,                -MNU_CPX,                 ITM_Rup,                  -MNU_STK,                 -MNU_EXP,                 -MNU_CLR,                 
                
                                            ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_XFACT, 
                                            KEY_COMPLEX,              ITM_MAGNITUDE,            ITM_PC,                   ITM_CONSTpi,              ITM_arcsin,               ITM_arccos,               
                                            ITM_MAGNITUDE,            ITM_ANGLE,                ITM_DELTAPC,              ITM_XTHROOT,              ITM_GTO,                  ITM_LBL,                  
                
                                            ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_XFACT, 
//                                            ITM_NULLITM_RI/*ITM_RND*/,        ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  
                                            ITM_NULL,                 ITM_NULL,                 ITM_NULL,                 ITM_NULL,                 ITM_NULL,                 ITM_GTO,  
                                            ITM_TGLFRT,               ITM_TGLFRT,               ITM_HASH_JM/*ITM_toINT*/, ITM_ms,                   ITM_dotD,                 ITM_toREC2,
#endif //JM END OF LAYOUT 1a.


#ifdef JM_LAYOUT_2_DM42_STRICT //JM DM42 STRICT. NAME CHANGE FROM LAYOUT 2 TO LAYOUT 42C
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
                                           ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_XFACT, 
                                           ITM_EXIT1,                ITM_OFF,                  -MNU_HOME,                ITM_SHOW,                 ITM_PR,                   -MNU_CATALOG,             
                                           ITM_EXIT1,                -MNU_PRINT,               ITM_TIMER,                ITM_VIEW,                 -MNU_CLK,                 -MNU_PARTS,               

                                           ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_XFACT, 
                                           KEY_fg,                   ITM_NULL,                 ITM_ASSIGN,               ITM_USERMODE,             -MNU_PFN,                 -MNU_IO,                  
                                           KEY_fg,                   ITM_NULL,                 -MNU_ASN,                 -MNU_LOOP,                -MNU_TEST,                -MNU_ALPHAFN,             

                                           ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_XFACT, 
                                           ITM_DOWN1,                ITM_SST,                  -MNU_BASE,                -MNU_ANGLECONV,           -MNU_FLAGS,               -MNU_PROB,                
                                           ITM_DOWN1,                ITM_FLGSV,                -MNU_FIN,                 -MNU_UNITCONV,            -MNU_BITS,                -MNU_INTS,                

                                           ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_XFACT, 
                                           ITM_UP1,                  ITM_BST,                  -MNU_EQN,                 -MNU_ADV,                 -MNU_MATX,                -MNU_STAT,                
                                           ITM_UP1,                  ITM_RBR,                  -MNU_INFO,                -MNU_CONST,               -MNU_XFN,                 -MNU_SUMS,                

                                           ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_XFACT, 
                                           ITM_ENTER,                ITM_AIM,                  ITM_LASTX,                -MNU_MODE,                -MNU_DSP,                 -MNU_CLR,                 
                                           ITM_ENTER,                -MNU_CPX,                 ITM_Rup,                  -MNU_STK,                 -MNU_EXP,                 ITM_UNDO,                 

                                           ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_XFACT, 
                                           KEY_COMPLEX,              KEY_COMPLEX,              ITM_PC,                   ITM_CONSTpi,              ITM_arcsin,               ITM_arccos,               
                                           ITM_MAGNITUDE,            ITM_MAGNITUDE,            ITM_ANGLE,                ITM_XTHROOT,              ITM_CC,                   ITM_LBL,                  

                                           ITM_SIGMAMINUS,           ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  ITM_XFACT, 
//                                         ITM_NULLITM_SIGMAMINUS,   ITM_YX,                   ITM_SQUARE,               ITM_10x,                  ITM_EXP,                  
                                           ITM_NULL,                 ITM_NULL,                 ITM_NULL,                 ITM_NULL,                 ITM_NULL,                 ITM_GTO,  
                                           ITM_TGLFRT,               ITM_TGLFRT,               ITM_HASH_JM,              ITM_ms,                   ITM_dotD,                 ITM_toREC2,
#endif //JM END OF LAYOUT 2 DM42 STRICT.


/*HOME1*/
                                           ITM_SIGMAMINUS,                ITM_YX,                     ITM_SQUARE,               ITM_10x,               ITM_EXP,                     ITM_CC,                           //JM HOME
                                           ITM_SI_f,                      ITM_SI_p,                   ITM_PARALLEL,             ITM_XFACT,             ITM_MAGNITUDE,               ITM_SI_T,                             //JM HOME                                    
                                           ITM_SI_n,                      ITM_SI_u,                   ITM_SI_m,                 ITM_SI_k,              ITM_SI_M,                    ITM_SI_G                              //JM UNITS SI

//##################################################################################################################################################################################################################################

                                     };                                                                                                                                                                                   //JM HOME

TO_QSPI const int16_t menu_PLOT[]        = { ITM_PLOT_J,                    ITM_PLOTLS,                 ITM_SCALE,                ITM_PLINE,             ITM_SIGMAPLUS,               ITM_SNAP,                           //JM GRAPH
                                             ITM_NVECT,                     ITM_VECT,                   ITM_EXTX,                 ITM_PCROS,             ITM_LISTXY,                  ITM_RMS,                            //JM GRAPH
                                             ITM_DIFF,                      ITM_INTG,                   ITM_EXTY,                 ITM_PBOX,              ITM_CLSIGMA,                 ITM_SHADE,                          //JM GRAPH

                                             ITM_SCALE,                     ITM_PLINE,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM GRAPH
                                             ITM_EXTX,                      ITM_PCROS,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM GRAPH
                                             ITM_EXTY,                      ITM_PBOX,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM GRAPH

                                             ITM_SIGMAPLUS,                 ITM_SNAP,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM GRAPH
                                             ITM_LISTXY,                    ITM_RMS,                    ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM GRAPH
                                             ITM_CLSIGMA,                   ITM_SHADE,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM GRAPH

                                             ITM_PLOT_J,                    ITM_PLOTLS,                 ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM GRAPH
                                             ITM_DEMO4,                     ITM_DEMO5,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM GRAPH
                                             ITM_DEMO6,                     ITM_P_ALLREGS,              ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM GRAPH

                                             ITM_PLOT_J,                    ITM_PLOTLS,                 ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM GRAPH
                                             ITM_DEMO1,                     ITM_DEMO2,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM GRAPH
                                             ITM_DEMO3,                     ITM_P_ALLREGS,              ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM GRAPH

                                             ITM_DEMO2,                     ITM_SLV2,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM GRAPH
                                             ITM_DEMO3,                     ITM_SLV3,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM GRAPH
                                             ITM_PZOOMX,                    ITM_PZOOMY,                 ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                        };  //JM GRAPH


TO_QSPI const int16_t menu_ALPHA[]       = { ITM_T_UP_ARROW,                ITM_T_DOWN_ARROW,           ITM_T_LLEFT_ARROW,        ITM_T_RRIGHT_ARROW,    ITM_T_LEFT_ARROW,            ITM_T_RIGHT_ARROW, 
                                             -MNU_MyAlpha,                  -MNU_T_EDIT,                -MNU_ALPHA_OMEGA,         -MNU_ALPHADOT,         -MNU_ALPHAMATH,              -MNU_ALPHAINTL,                     //JM
                                             ITM_XEDIT,                     ITM_USERMODE,               -MNU_ASN,                 -MNU_CATALOG,          CHR_num,                     CHR_case                        };    //JM


TO_QSPI const int16_t menu_T_EDIT[]      = { ITM_T_UP_ARROW,                ITM_T_DOWN_ARROW,           ITM_T_LLEFT_ARROW,       ITM_T_RRIGHT_ARROW,     ITM_T_LEFT_ARROW,            ITM_T_RIGHT_ARROW,                    //JM TEXTEDIT
                                             ITM_DOUBLE_QUOTE,              -MNU_ALPHA,                 -MNU_ALPHA_OMEGA,        -MNU_ALPHADOT,         -MNU_ALPHAMATH,              -MNU_ALPHAINTL,               
                                             ITM_QUOTE,                     ITM_ASTERISK,               ITM_CIRCUMFLEX,          ITM_SLASH,              ITM_LESS_THAN,               ITM_GREATER_THAN                }; 

TO_QSPI const int16_t menu_XXEQ[]        = { ITM_XSAVE,                     ITM_XLOAD,                  ITM_XEDIT,               ITM_XNEW,              ITM_XXEQ,                     -MNU_XEQ                        };   //JM


#include "softmenuCatalogs.h"

TO_QSPI const softmenu_t softmenu[] = {
/*   0 */  {.menuItem = -MNU_MyMenu,      .numItems = 0,                                        .softkeyItem = NULL             }, // MyMenu must be the 1st
/*   1 */  {.menuItem = -MNU_MyAlpha,     .numItems = 0,                                        .softkeyItem = NULL             }, // Myalpha must be the 2nd
/*   2 */  {.menuItem = -MNU_RAM,         .numItems = 0,                                        .softkeyItem = NULL             },
/*   3 */  {.menuItem = -MNU_FLASH,       .numItems = 0,                                        .softkeyItem = NULL             }, // The 15 first menus are
/*   4 */  {.menuItem = -MNU_VAR,         .numItems = 0,                                        .softkeyItem = NULL             }, // variable softmenus and
/*   5 */  {.menuItem = -MNU_PROG,        .numItems = 0,                                        .softkeyItem = NULL             }, // MUST be in the same
/*   6 */  {.menuItem = -MNU_MATRS,       .numItems = 0,                                        .softkeyItem = NULL             }, // order as the
/*   7 */  {.menuItem = -MNU_STRINGS,     .numItems = 0,                                        .softkeyItem = NULL             }, // dynamicSoftmenu area.
/*   8 */  {.menuItem = -MNU_DATES,       .numItems = 0,                                        .softkeyItem = NULL             }, //
/*   9 */  {.menuItem = -MNU_TIMES,       .numItems = 0,                                        .softkeyItem = NULL             }, // If you add or remove one:
/*  10 */  {.menuItem = -MNU_ANGLES,      .numItems = 0,                                        .softkeyItem = NULL             }, // don't forget to adjust
/*  11 */  {.menuItem = -MNU_SINTS,       .numItems = 0,                                        .softkeyItem = NULL             }, // NUMBER_OF_DYNAMIC_SOFTMENUS
/*  12 */  {.menuItem = -MNU_LINTS,       .numItems = 0,                                        .softkeyItem = NULL             }, // in defines.h
/*  13 */  {.menuItem = -MNU_REALS,       .numItems = 0,                                        .softkeyItem = NULL             },
/*  14 */  {.menuItem = -MNU_CPXS,        .numItems = 0,                                        .softkeyItem = NULL             },
/*  15 */  {.menuItem = -MNU_TAMFLAG,     .numItems = sizeof(menu_TamFlag    )/sizeof(int16_t), .softkeyItem = menu_TamFlag     },
/*  16 */  {.menuItem = -MNU_SYSFL,       .numItems = sizeof(menu_SYSFL      )/sizeof(int16_t), .softkeyItem = menu_SYSFL       },
/*  17 */  {.menuItem = -MNU_ALPHAINTL,   .numItems = sizeof(menu_alpha_INTL )/sizeof(int16_t), .softkeyItem = menu_alpha_INTL  },
/*  18 */  {.menuItem = -MNU_ALPHAintl,   .numItems = sizeof(menu_alpha_intl )/sizeof(int16_t), .softkeyItem = menu_alpha_intl  },
/*  19 */  {.menuItem = -MNU_ADV,         .numItems = sizeof(menu_ADV        )/sizeof(int16_t), .softkeyItem = menu_ADV         },
/*  20 */  {.menuItem = -MNU_Sfdx,        .numItems = sizeof(menu_Sfdx       )/sizeof(int16_t), .softkeyItem = menu_Sfdx        },
/*  21 */  {.menuItem = -MNU_BITS,        .numItems = sizeof(menu_BITS       )/sizeof(int16_t), .softkeyItem = menu_BITS        },
/*  22 */  {.menuItem = -MNU_CLK,         .numItems = sizeof(menu_CLK        )/sizeof(int16_t), .softkeyItem = menu_CLK         },
/*  23 */  {.menuItem = -MNU_CLR,         .numItems = sizeof(menu_CLR        )/sizeof(int16_t), .softkeyItem = menu_CLR         },
/*  24 */  {.menuItem = -MNU_CPX,         .numItems = sizeof(menu_CPX        )/sizeof(int16_t), .softkeyItem = menu_CPX         },
/*  25 */  {.menuItem = -MNU_DSP,         .numItems = sizeof(menu_DISP       )/sizeof(int16_t), .softkeyItem = menu_DISP        },
/*  26 */  {.menuItem = -MNU_EQN,         .numItems = sizeof(menu_EQN        )/sizeof(int16_t), .softkeyItem = menu_EQN         },
/*  27 */  {.menuItem = -MNU_1STDERIV,    .numItems = sizeof(menu_1stDeriv   )/sizeof(int16_t), .softkeyItem = menu_1stDeriv    },
/*  28 */  {.menuItem = -MNU_2NDDERIV,    .numItems = sizeof(menu_2ndDeriv   )/sizeof(int16_t), .softkeyItem = menu_2ndDeriv    },
/*  29 */  {.menuItem = -MNU_Sf,          .numItems = sizeof(menu_Sf         )/sizeof(int16_t), .softkeyItem = menu_Sf          },
/*  30 */  {.menuItem = -MNU_Solver,      .numItems = sizeof(menu_Solver     )/sizeof(int16_t), .softkeyItem = menu_Solver      },
/*  31 */  {.menuItem = -MNU_EXP,         .numItems = sizeof(menu_EXP        )/sizeof(int16_t), .softkeyItem = menu_EXP         },
/*  32 */  {.menuItem = -MNU_TRI,         .numItems = sizeof(menu_TRI        )/sizeof(int16_t), .softkeyItem = menu_TRI         },
/*  33 */  {.menuItem = -MNU_FIN,         .numItems = sizeof(menu_FIN        )/sizeof(int16_t), .softkeyItem = menu_FIN         },
// /*  34 */  {.menuItem = -MNU_TVM,         .numItems = sizeof(menu_TVM        )/sizeof(int16_t), .softkeyItem = menu_TVM         },
/*  35 */  {.menuItem = -MNU_FLAGS,       .numItems = sizeof(menu_FLAGS      )/sizeof(int16_t), .softkeyItem = menu_FLAGS       },
/*  36 */  {.menuItem = -MNU_INFO,        .numItems = sizeof(menu_INFO       )/sizeof(int16_t), .softkeyItem = menu_INFO        },
/*  37 */  {.menuItem = -MNU_INTS,        .numItems = sizeof(menu_INTS       )/sizeof(int16_t), .softkeyItem = menu_INTS        },
/*  38 */  {.menuItem = -MNU_LOOP,        .numItems = sizeof(menu_LOOP       )/sizeof(int16_t), .softkeyItem = menu_LOOP        },
/*  39 */  {.menuItem = -MNU_MATX,        .numItems = sizeof(menu_MATX       )/sizeof(int16_t), .softkeyItem = menu_MATX        },
/*  40 */  {.menuItem = -MNU_SIMQ,        .numItems = sizeof(menu_M_SIM_Q    )/sizeof(int16_t), .softkeyItem = menu_M_SIM_Q     },
/*  41 */  {.menuItem = -MNU_M_EDIT,      .numItems = sizeof(menu_M_EDIT     )/sizeof(int16_t), .softkeyItem = menu_M_EDIT      },
/*  42 */  {.menuItem = -MNU_MODE,        .numItems = sizeof(menu_MODE       )/sizeof(int16_t), .softkeyItem = menu_MODE        },
/*  43 */  {.menuItem = -MNU_PARTS,       .numItems = sizeof(menu_PARTS      )/sizeof(int16_t), .softkeyItem = menu_PARTS       },
/*  44 */  {.menuItem = -MNU_PROB,        .numItems = sizeof(menu_PROB       )/sizeof(int16_t), .softkeyItem = menu_PROB        },
/*  45 */  {.menuItem = -MNU_T,           .numItems = sizeof(menu_t          )/sizeof(int16_t), .softkeyItem = menu_t           },
/*  46 */  {.menuItem = -MNU_F,           .numItems = sizeof(menu_F          )/sizeof(int16_t), .softkeyItem = menu_F           },
/*  47 */  {.menuItem = -MNU_CHI2,        .numItems = sizeof(menu_chi2       )/sizeof(int16_t), .softkeyItem = menu_chi2        },
/*  48 */  {.menuItem = -MNU_NORML,       .numItems = sizeof(menu_Norml      )/sizeof(int16_t), .softkeyItem = menu_Norml       },
/*  49 */  {.menuItem = -MNU_LGNRM,       .numItems = sizeof(menu_LgNrm      )/sizeof(int16_t), .softkeyItem = menu_LgNrm       },
/*  50 */  {.menuItem = -MNU_CAUCH,       .numItems = sizeof(menu_Cauch      )/sizeof(int16_t), .softkeyItem = menu_Cauch       },
/*  51 */  {.menuItem = -MNU_EXPON,       .numItems = sizeof(menu_Expon      )/sizeof(int16_t), .softkeyItem = menu_Expon       },
/*  52 */  {.menuItem = -MNU_LOGIS,       .numItems = sizeof(menu_Logis      )/sizeof(int16_t), .softkeyItem = menu_Logis       },
/*  53 */  {.menuItem = -MNU_WEIBL,       .numItems = sizeof(menu_Weibl      )/sizeof(int16_t), .softkeyItem = menu_Weibl       },
/*  54 */  {.menuItem = -MNU_BINOM,       .numItems = sizeof(menu_Binom      )/sizeof(int16_t), .softkeyItem = menu_Binom       },
/*  55 */  {.menuItem = -MNU_GEOM,        .numItems = sizeof(menu_Geom       )/sizeof(int16_t), .softkeyItem = menu_Geom        },
/*  56 */  {.menuItem = -MNU_HYPER,       .numItems = sizeof(menu_Hyper      )/sizeof(int16_t), .softkeyItem = menu_Hyper       },
/*  57 */  {.menuItem = -MNU_NBIN,        .numItems = sizeof(menu_Nbin       )/sizeof(int16_t), .softkeyItem = menu_Nbin        },
/*  58 */  {.menuItem = -MNU_POISS,       .numItems = sizeof(menu_Poiss      )/sizeof(int16_t), .softkeyItem = menu_Poiss       },
/*  59 */  {.menuItem = -MNU_PFN,         .numItems = sizeof(menu_PFN        )/sizeof(int16_t), .softkeyItem = menu_PFN         },
/*  60 */  {.menuItem = -MNU_PFN2,        .numItems = sizeof(menu_PFN2       )/sizeof(int16_t), .softkeyItem = menu_PFN2        },
/*  61 */  {.menuItem = -MNU_STAT,        .numItems = sizeof(menu_STAT       )/sizeof(int16_t), .softkeyItem = menu_STAT        },
/*  62 */  {.menuItem = -MNU_SUMS,        .numItems = sizeof(menu_SUMS       )/sizeof(int16_t), .softkeyItem = menu_SUMS        },
/*  63 */  {.menuItem = -MNU_PLOT_STAT,   .numItems = sizeof(menu_PLOT_STAT  )/sizeof(int16_t), .softkeyItem = menu_PLOT_STAT   },
/*  64 */  {.menuItem = -MNU_PLOT_LR,     .numItems = sizeof(menu_PLOT_LR    )/sizeof(int16_t), .softkeyItem = menu_PLOT_LR     },
/*  65 */  {.menuItem = -MNU_STK,         .numItems = sizeof(menu_STK        )/sizeof(int16_t), .softkeyItem = menu_STK         },
/*  66 */  {.menuItem = -MNU_TEST,        .numItems = sizeof(menu_TEST       )/sizeof(int16_t), .softkeyItem = menu_TEST        },
/*  67 */  {.menuItem = -MNU_XFN,         .numItems = sizeof(menu_XFN        )/sizeof(int16_t), .softkeyItem = menu_XFN         },
/*  68 */  {.menuItem = -MNU_ORTHOG,      .numItems = sizeof(menu_Orthog     )/sizeof(int16_t), .softkeyItem = menu_Orthog      },
/*  69 */  {.menuItem = -MNU_CATALOG,     .numItems = sizeof(menu_CATALOG    )/sizeof(int16_t), .softkeyItem = menu_CATALOG     },
/*  70 */  {.menuItem = -MNU_CHARS,       .numItems = sizeof(menu_CHARS      )/sizeof(int16_t), .softkeyItem = menu_CHARS       },
/*  71 */  {.menuItem = -MNU_PROGS,       .numItems = sizeof(menu_PROGS      )/sizeof(int16_t), .softkeyItem = menu_PROGS       },
/*  72 */  {.menuItem = -MNU_VARS,        .numItems = sizeof(menu_VARS       )/sizeof(int16_t), .softkeyItem = menu_VARS        },
/*  73 */  {.menuItem = -MNU_ALPHA_OMEGA, .numItems = sizeof(menu_ALPHA_OMEGA)/sizeof(int16_t), .softkeyItem = menu_ALPHA_OMEGA },
/*  74 */  {.menuItem = -MNU_alpha_omega, .numItems = sizeof(menu_alpha_omega)/sizeof(int16_t), .softkeyItem = menu_alpha_omega },
/*  75 */  {.menuItem = -MNU_FCNS,        .numItems = sizeof(menu_FCNS       )/sizeof(int16_t), .softkeyItem = menu_FCNS        },
/*  76 */  {.menuItem = -MNU_MENUS,       .numItems = sizeof(menu_MENUS      )/sizeof(int16_t), .softkeyItem = menu_MENUS       },
/*  77 */  {.menuItem = -MNU_ALPHAMATH,   .numItems = sizeof(menu_alphaMATH  )/sizeof(int16_t), .softkeyItem = menu_alphaMATH   },
/*  78 */  {.menuItem = -MNU_ALPHADOT,    .numItems = sizeof(menu_alphaDot   )/sizeof(int16_t), .softkeyItem = menu_alphaDot    },
/*  79 */  {.menuItem = -MNU_ALPHAFN,     .numItems = sizeof(menu_alphaFN    )/sizeof(int16_t), .softkeyItem = menu_alphaFN     },
/*  80 */  {.menuItem = -MNU_ANGLECONV,   .numItems = sizeof(menu_AngleConv  )/sizeof(int16_t), .softkeyItem = menu_AngleConv   },
/*  81 */  {.menuItem = -MNU_UNITCONV,    .numItems = sizeof(menu_UnitConv   )/sizeof(int16_t), .softkeyItem = menu_UnitConv    },
/*  82 */  {.menuItem = -MNU_CONVE,       .numItems = sizeof(menu_ConvE      )/sizeof(int16_t), .softkeyItem = menu_ConvE       },
/*  83 */  {.menuItem = -MNU_CONVP,       .numItems = sizeof(menu_ConvP      )/sizeof(int16_t), .softkeyItem = menu_ConvP       },
/*  84 */  {.menuItem = -MNU_CONVFP,      .numItems = sizeof(menu_ConvFP     )/sizeof(int16_t), .softkeyItem = menu_ConvFP      },
/*  85 */  {.menuItem = -MNU_CONVM,       .numItems = sizeof(menu_ConvM      )/sizeof(int16_t), .softkeyItem = menu_ConvM       },
/*  86 */  {.menuItem = -MNU_CONVX,       .numItems = sizeof(menu_ConvX      )/sizeof(int16_t), .softkeyItem = menu_ConvX       },
/*  87 */  {.menuItem = -MNU_CONVV,       .numItems = sizeof(menu_ConvV      )/sizeof(int16_t), .softkeyItem = menu_ConvV       },
/*  88 */  {.menuItem = -MNU_CONVA,       .numItems = sizeof(menu_ConvA      )/sizeof(int16_t), .softkeyItem = menu_ConvA       },
/*  89 */  {.menuItem = -MNU_CONST,       .numItems = sizeof(menu_CONST      )/sizeof(int16_t), .softkeyItem = menu_CONST       },
/*  90 */  {.menuItem = -MNU_IO,          .numItems = sizeof(menu_IO         )/sizeof(int16_t), .softkeyItem = menu_IO          },
/*  91 */  {.menuItem = -MNU_PRINT,       .numItems = sizeof(menu_PRINT      )/sizeof(int16_t), .softkeyItem = menu_PRINT       },
/*  92 */  {.menuItem = -MNU_TAM,         .numItems = sizeof(menu_Tam        )/sizeof(int16_t), .softkeyItem = menu_Tam         },
/*  93 */  {.menuItem = -MNU_TAMCMP,      .numItems = sizeof(menu_TamCmp     )/sizeof(int16_t), .softkeyItem = menu_TamCmp      },
/*  94 */  {.menuItem = -MNU_TAMSTORCL,   .numItems = sizeof(menu_TamStoRcl  )/sizeof(int16_t), .softkeyItem = menu_TamStoRcl   },
/*  95 */  {.menuItem = -MNU_TAMSHUFFLE,  .numItems = sizeof(menu_TamShuffle )/sizeof(int16_t), .softkeyItem = menu_TamShuffle  },
/*  96 */  {.menuItem = -MNU_TAMLABEL,    .numItems = sizeof(menu_TamLabel   )/sizeof(int16_t), .softkeyItem = menu_TamLabel    },

/*  99 */  {.menuItem = -MNU_ASN_N,       .numItems = sizeof(menu_ASN_N      )/sizeof(int16_t), .softkeyItem = menu_ASN_N       },  //JM USER NORMAL
/* 100 */  {.menuItem = -MNU_ASN_U,       .numItems = sizeof(menu_ASN_U      )/sizeof(int16_t), .softkeyItem = menu_ASN_U       },  //JM USER 
/* 101 */  {.menuItem = -MNU_ASN,         .numItems = sizeof(menu_ASN        )/sizeof(int16_t), .softkeyItem = menu_ASN         },  //JM USER
/* 102 */  {.menuItem = -MNU_XEQ,         .numItems = sizeof(menu_XEQ        )/sizeof(int16_t), .softkeyItem = menu_XEQ         },  //JM EXEC
/* 103 */  {.menuItem = -MNU_HOME,        .numItems = sizeof(menu_HOME       )/sizeof(int16_t), .softkeyItem = menu_HOME        },  //JM HOME
/* 104 */  {.menuItem = -MNU_PLOT,        .numItems = sizeof(menu_PLOT       )/sizeof(int16_t), .softkeyItem = menu_PLOT        },  //JM GRAPH
/* 105 */  {.menuItem = -MNU_ALPHA,       .numItems = sizeof(menu_ALPHA      )/sizeof(int16_t), .softkeyItem = menu_ALPHA       },  //JM ALPHA
/* 106 */  {.menuItem = -MNU_BASE,        .numItems = sizeof(menu_BASE       )/sizeof(int16_t), .softkeyItem = menu_BASE        },  //JM BASE
/* 107 */  {.menuItem = -MNU_EE,          .numItems = sizeof(menu_EE         )/sizeof(int16_t), .softkeyItem = menu_EE          },  //JM EE
/* 108 */  {.menuItem = -MNU_T_EDIT,      .numItems = sizeof(menu_T_EDIT     )/sizeof(int16_t), .softkeyItem = menu_T_EDIT      },  //JM TEXTEDIT
/* 109 */  {.menuItem = -MNU_XXEQ,        .numItems = sizeof(menu_XXEQ       )/sizeof(int16_t), .softkeyItem = menu_XXEQ        },  //JM TEXTEDIT
#ifdef INLINE_TEST                                                              //vv dr
/* 110 */  {.menuItem= -MNU_INL_TST,     .numItems = sizeof(menu_Inl_Tst    )/sizeof(int16_t), .softkeyItem = menu_Inl_Tst     },
#endif                                                                          //^^
/* 110 */  {.menuItem =  0,               .numItems = 0,                                        .softkeyItem = NULL             }
};


dynamicSoftmenu_t dynamicSoftmenu[NUMBER_OF_DYNAMIC_SOFTMENUS] = {
/*   0 */  {.menuItem = -MNU_MyMenu,  .numItems = 0, .menuContent = NULL},
/*   1 */  {.menuItem = -MNU_MyAlpha, .numItems = 0, .menuContent = NULL},
/*   2 */  {.menuItem = -MNU_RAM,     .numItems = 0, .menuContent = NULL},
/*   3 */  {.menuItem = -MNU_FLASH,   .numItems = 0, .menuContent = NULL},
/*   4 */  {.menuItem = -MNU_VAR,     .numItems = 0, .menuContent = NULL},
/*   5 */  {.menuItem = -MNU_PROG,    .numItems = 0, .menuContent = NULL},
/*   6 */  {.menuItem = -MNU_MATRS,   .numItems = 0, .menuContent = NULL},
/*   7 */  {.menuItem = -MNU_STRINGS, .numItems = 0, .menuContent = NULL},
/*   8 */  {.menuItem = -MNU_DATES,   .numItems = 0, .menuContent = NULL},
/*   9 */  {.menuItem = -MNU_TIMES,   .numItems = 0, .menuContent = NULL},
/*  10 */  {.menuItem = -MNU_ANGLES,  .numItems = 0, .menuContent = NULL},
/*  11 */  {.menuItem = -MNU_SINTS,   .numItems = 0, .menuContent = NULL},
/*  12 */  {.menuItem = -MNU_LINTS,   .numItems = 0, .menuContent = NULL},
/*  13 */  {.menuItem = -MNU_REALS,   .numItems = 0, .menuContent = NULL},
/*  14 */  {.menuItem = -MNU_CPXS,    .numItems = 0, .menuContent = NULL},
};



uint8_t *getNthString(uint8_t *ptr, int16_t n) { // Starting with string 0 (the 1st string is returned for n=0)
  while(n) {
    ptr += stringByteLength((char *)ptr) + 1;
    n--;
  }

  return ptr;
}



void fnDynamicMenu(uint16_t unusedButMandatoryParameter) {
  printf("fnDynamicMenu:\n       softmenuId = %d\n  dynamicMenuItem = %d\n", softmenuStack[0].softmenuId, dynamicMenuItem);
}



#ifndef TESTSUITE_BUILD
  static int sortMenu(void const *a, void const *b) {
    return compareString(a, b, CMP_EXTENSIVE);
  }



  static void _dynmenuConstructVars(int16_t menu, bool_t applyFilter, dataType_t typeFilter, bool_t isAngular) {
    uint16_t numberOfBytes, numberOfVars;
    uint8_t *ptr;
    numberOfBytes = 1;
    numberOfVars = 0;
    memset(tmpString, 0, TMP_STR_LENGTH);
    for(int i=0; i<numberOfNamedVariables; i++) {
      calcRegister_t regist = i+FIRST_NAMED_VARIABLE;
      dataType_t dt = getRegisterDataType(regist);
      if(!applyFilter || (dt != dtReal34 && dt == typeFilter) ||
          (typeFilter == dtReal34Matrix && dt == dtComplex34Matrix) ||
          (typeFilter == dtReal34 && dt == dtReal34 &&
            ((isAngular && getRegisterAngularMode(regist) != amNone) ||
             (!isAngular && getRegisterAngularMode(regist) == amNone)))) {
        xcopy(tmpString + 15 * numberOfVars, allNamedVariables[i].variableName + 1, allNamedVariables[i].variableName[0]);
        numberOfVars++;
        numberOfBytes += 1 + allNamedVariables[i].variableName[0];
      }
    }

    if(numberOfVars != 0) {
      qsort(tmpString, numberOfVars, 15, sortMenu);
    }

    ptr = malloc(numberOfBytes);
    dynamicSoftmenu[menu].menuContent = ptr;
    for(int i=0; i<numberOfVars; i++) {
      int16_t len = stringByteLength(tmpString + 15*i) + 1;
      xcopy(ptr, tmpString + 15*i, len);
      ptr += len;
    }

    dynamicSoftmenu[menu].numItems = numberOfVars;
  }



  static void initVariableSoftmenu(int16_t menu) {
    int16_t i, numberOfBytes, numberOfGlobalLabels;
    uint8_t *ptr;

    free(dynamicSoftmenu[menu].menuContent);

    switch(-dynamicSoftmenu[menu].menuItem) {
      case MNU_MyAlpha: dynamicSoftmenu[menu].menuContent = malloc(17);
                        //xcopy(dynamicSoftmenu[menu].menuContent, "My" STD_alpha "\000to\000be\000coded", 17);
                        xcopy(dynamicSoftmenu[menu].menuContent, "", 1);
                        dynamicSoftmenu[menu].numItems = 0;
                        break;

      case MNU_FLASH:   dynamicSoftmenu[menu].menuContent = malloc(18);
                        xcopy(dynamicSoftmenu[menu].menuContent, "FLASH\000to\000be\000coded", 18);
                        dynamicSoftmenu[menu].numItems = 4;
                        break;

      case MNU_RAM:     numberOfBytes = 0;
                        numberOfGlobalLabels = 0;
                        memset(tmpString, 0, TMP_STR_LENGTH);
                        for(i=0; i<numberOfLabels; i++) {
                          if(labelList[i].program > 0 && labelList[i].step > 0) { // RAM and Global label
                            xcopy(tmpString + 15 * numberOfGlobalLabels, labelList[i].labelPointer + 1, labelList[i].labelPointer[0]);
                            numberOfGlobalLabels++;
                            numberOfBytes += 1 + labelList[i].labelPointer[0];
                          }
                        }

                        if(numberOfGlobalLabels != 0) {
                          qsort(tmpString, numberOfGlobalLabels, 15, sortMenu);
                        }

                        ptr = malloc(numberOfBytes);
                        dynamicSoftmenu[menu].menuContent = ptr;
                        for(i=0; i<numberOfGlobalLabels; i++) {
                          int16_t len = stringByteLength(tmpString + 15*i) + 1;
                          xcopy(ptr, tmpString + 15*i, len);
                          ptr += len;
                        }

                        dynamicSoftmenu[menu].numItems = numberOfGlobalLabels;
                        break;

      case MNU_MyMenu:  dynamicSoftmenu[menu].menuContent = malloc(19);
                        //xcopy(dynamicSoftmenu[menu].menuContent, "MyMenu\000to\000be\000coded", 19);
                        xcopy(dynamicSoftmenu[menu].menuContent, "", 1);
                        dynamicSoftmenu[menu].numItems = 0;
                        break;

      case MNU_VAR:     _dynmenuConstructVars(menu, false, 0, false);
                        break;

      case MNU_PROG:    numberOfBytes = 1;
                        numberOfGlobalLabels = 0;
                        memset(tmpString, 0, TMP_STR_LENGTH);
                        for(i=0; i<numberOfLabels; i++) {
                          if(labelList[i].program > 0 && labelList[i].step > 0) { // RAM and Global label
                            xcopy(tmpString + 15 * numberOfGlobalLabels, labelList[i].labelPointer + 1, labelList[i].labelPointer[0]);
                            numberOfGlobalLabels++;
                            numberOfBytes += 1 + labelList[i].labelPointer[0];
                          }
                        }

                        if(numberOfGlobalLabels != 0) {
                          qsort(tmpString, numberOfGlobalLabels, 15, sortMenu);
                        }

                        ptr = malloc(numberOfBytes);
                        dynamicSoftmenu[menu].menuContent = ptr;
                        for(i=0; i<numberOfGlobalLabels; i++) {
                          int16_t len = stringByteLength(tmpString + 15*i) + 1;
                          xcopy(ptr, tmpString + 15*i, len);
                          ptr += len;
                        }

                        dynamicSoftmenu[menu].numItems = numberOfGlobalLabels;
                        break;

      case MNU_MATRS:   _dynmenuConstructVars(menu, true, dtReal34Matrix, false);
                        break;

      case MNU_STRINGS: _dynmenuConstructVars(menu, true, dtString, false);
                        break;

      case MNU_DATES:   _dynmenuConstructVars(menu, true, dtDate, false);
                        break;

      case MNU_TIMES:   _dynmenuConstructVars(menu, true, dtTime, false);
                        break;

      case MNU_ANGLES:  _dynmenuConstructVars(menu, true, dtReal34, true);
                        break;

      case MNU_SINTS:   _dynmenuConstructVars(menu, true, dtShortInteger, false);
                        break;

      case MNU_LINTS:   _dynmenuConstructVars(menu, true, dtLongInteger, false);
                        break;

      case MNU_REALS:   _dynmenuConstructVars(menu, true, dtReal34, false);
                        break;

      case MNU_CPXS:    _dynmenuConstructVars(menu, true, dtComplex34, false);
                        break;

      default:          sprintf(errorMessage, "In function initVariableSoftmenu: unexpected variable softmenu %" PRId16 "!", (int16_t)(-dynamicSoftmenu[menu].menuItem));
                        displayBugScreen(errorMessage);
    }
  }



//JM To determine the menu number for a given menuId          //JMvv
int16_t mm(int16_t id) {
  int16_t m;
  m = 0;
  if(id != 0) { // Search by ID
    while(softmenu[m].menuItem != 0) {
      printf(">>> mm %d %d %d %s \n",id, m, softmenu[m].menuItem, indexOfItems[-softmenu[m].menuItem].itemSoftmenuName);
      if(softmenu[m].menuItem == id) {
       break;
      }
      m++;
    }
  }
  return m;
}                                                             //JM^^



//vv EXTRA DRAWINGS FOR RADIO_BUTTON AND CHECK_BOX
#define JM_LINE2_DRAW
#undef JM_LINE2_DRAW
#ifdef JM_LINE2_DRAW
void JM_LINE2(uint32_t xx, uint32_t yy) {                          // To draw the lines for radiobutton on screen
  uint32_t x, y;
  y = yy-3-1;
  for(x=xx-66+1; x<min(xx-1,SCREEN_WIDTH); x++) {
    if(mod(x, 2) == 0) {
      setBlackPixel(x, y);
      setBlackPixel(x, y+2);
    }
    else {
      setBlackPixel(x, y+1);
    }
  }
}
#endif


#define RB_EXTRA_BORDER
//#undef RB_EXTRA_BORDER
#define RB_CLEAR_CENTER
#undef RB_CLEAR_CENTER
#ifdef RB_EXTRA_BORDER
void rbColumnCcccccc(uint32_t xx, uint32_t yy) {
  lcd_fill_rect(xx,yy+2,1,7,  0);
}
#endif



void rbColumnCcSssssCc(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+8,1,2,  0);
#endif
  lcd_fill_rect(xx,yy+3,1,5,  0xFF);
#ifdef RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+1,1,1,  0);
#endif
}



void rbColumnCcSssssssCc(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+9,1,2,  0);
#endif
  lcd_fill_rect(xx,yy+2,1,7,  0xFF);
#ifdef RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy,1,2,  0);
#endif
}



void rbColumnCSssCccSssC(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+10);
#endif
  lcd_fill_rect(xx,yy+7,1,3,  0xFF);
  lcd_fill_rect(xx,yy+4,1,3,  0);
  lcd_fill_rect(xx,yy+1,1,3,  0xFF);
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+0);
#endif
}



void rbColumnCSsCSssCSsC(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+10);
#endif
  lcd_fill_rect(xx,yy+8,1,2,  0xFF);
  setWhitePixel (xx,yy+7);
  lcd_fill_rect(xx,yy+4,1,3,  0xFF);
  setWhitePixel (xx,yy+3);
  lcd_fill_rect(xx,yy+1,1,2,  0xFF);
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+0);
#endif
}



void rbColumnCcSsNnnSsCc(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+9,1,2,  0);
#endif
  lcd_fill_rect(xx,yy+7,1,2,  0xFF);
#ifdef RB_CLEAR_CENTER
  lcd_fill_rect(xx,yy+4,1,3,  0);
#endif
  lcd_fill_rect(xx,yy+2,1,2,  0xFF);
#ifdef RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+0,1,2,  0);
#endif
}



void rbColumnCSsNnnnnSsC(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+10);
#endif
  lcd_fill_rect(xx,yy+8,1,2,  0xFF);
#ifdef RB_CLEAR_CENTER
  lcd_fill_rect(xx,yy+3,1,5,  0);
#endif
  lcd_fill_rect(xx,yy+1,1,2,  0xFF);
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+0);
#endif
}



void rbColumnCSNnnnnnnSC(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+10);
#endif
  setBlackPixel (xx,yy+9);
#ifdef RB_CLEAR_CENTER
  lcd_fill_rect(xx,yy+2,1,7,  0);
#endif
  setBlackPixel (xx,yy+1);
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+0);
#endif
}



#ifdef RB_EXTRA_BORDER
void cbColumnCcccccccccc(uint32_t xx, uint32_t yy) {
  lcd_fill_rect(xx,yy+0,1,11,  0);
}
#endif



void cbColumnCSssssssssC(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+10);
#endif
  lcd_fill_rect(xx,yy+1,1,9,  0xFF);
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+0);
#endif
}



void cbColumnCSsCccccSsC(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+10);
#endif
  lcd_fill_rect(xx,yy+8,1,2,  0xFF);
  lcd_fill_rect(xx,yy+3,1,5,  0);
  lcd_fill_rect(xx,yy+1,1,2,  0xFF);
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+0);
#endif
}



void cbColumnCSNnnnnnnSC(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+10);
#endif
  setBlackPixel (xx,yy+9);
#ifdef RB_CLEAR_CENTER
  lcd_fill_rect(xx,yy+2,1,7,  0);
#endif
  setBlackPixel (xx,yy+1);
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+0);
#endif
}



void RB_CHECKED(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  rbColumnCcccccc(xx+0, yy);
#endif
  rbColumnCcSssssCc(xx+1, yy);
  rbColumnCcSssssssCc(xx+2, yy);
  rbColumnCSssCccSssC(xx+3, yy);
  rbColumnCSsCSssCSsC(xx+4, yy);
  rbColumnCSsCSssCSsC(xx+5, yy);
  rbColumnCSsCSssCSsC(xx+6, yy);
  rbColumnCSssCccSssC(xx+7, yy);
  rbColumnCcSssssssCc(xx+8, yy);
  rbColumnCcSssssCc(xx+9, yy);
//#ifdef RB_EXTRA_BORDER
//  rbColumnCcccccc(xx+10, yy);
//#endif
}



void RB_UNCHECKED(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  rbColumnCcccccc(xx+0, yy);
#endif
  rbColumnCcSssssCc(xx+1, yy);
  rbColumnCcSsNnnSsCc(xx+2, yy);
  rbColumnCSsNnnnnSsC(xx+3, yy);
  rbColumnCSNnnnnnnSC(xx+4, yy);
  rbColumnCSNnnnnnnSC(xx+5, yy);
  rbColumnCSNnnnnnnSC(xx+6, yy);
  rbColumnCSsNnnnnSsC(xx+7, yy);
  rbColumnCcSsNnnSsCc(xx+8, yy);
  rbColumnCcSssssCc(xx+9, yy);
//#ifdef RB_EXTRA_BORDER
//  rbColumnCcccccc(xx+10, yy);
//#endif
}



void CB_CHECKED(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  cbColumnCcccccccccc(xx+0, yy);
#endif
  cbColumnCSssssssssC(xx+1, yy);
  cbColumnCSssssssssC(xx+2, yy);
  cbColumnCSsCccccSsC(xx+3, yy);
  rbColumnCSsCSssCSsC(xx+4, yy);
  rbColumnCSsCSssCSsC(xx+5, yy);
  rbColumnCSsCSssCSsC(xx+6, yy);
  cbColumnCSsCccccSsC(xx+7, yy);
  cbColumnCSssssssssC(xx+8, yy);
  cbColumnCSssssssssC(xx+9, yy);
//#ifdef RB_EXTRA_BORDER
//  cbColumnCcccccccccc(xx+10, yy);
//#endif
}



void CB_UNCHECKED(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  cbColumnCcccccccccc(xx+0, yy);
#endif
  cbColumnCSssssssssC(xx+1, yy);
  cbColumnCSNnnnnnnSC(xx+2, yy);
  cbColumnCSNnnnnnnSC(xx+3, yy);
  cbColumnCSNnnnnnnSC(xx+4, yy);
  cbColumnCSNnnnnnnSC(xx+5, yy);
  cbColumnCSNnnnnnnSC(xx+6, yy);
  cbColumnCSNnnnnnnSC(xx+7, yy);
  cbColumnCSNnnnnnnSC(xx+8, yy);
  cbColumnCSssssssssC(xx+9, yy);
//#ifdef RB_EXTRA_BORDER
//  cbColumnCcccccccccc(xx+10, yy);
//#endif
}
//^^


  /********************************************//**
   * \brief Displays one softkey
   *
   * \param[in] l const char*         Text to display
   * \param[in] xSoftkey int16_t      x location of softkey: from 0 (left) to 5 (right)
   * \param[in] ySoftKey int16_t      y location of softkey: from 0 (bottom) to 2 (top)
   * \param[in] videoMode videoMode_t Video mode normal or reverse
   * \param[in] topLineDotted bool_t  Is the item's top line dotted
   * \param[in] topLine bool_t        Draw a top line
   * \param[in] bottomLine bool_t     Draw a bottom line
   * \return void
   ***********************************************/
  void showSoftkey(const char *label, int16_t xSoftkey, int16_t ySoftKey, videoMode_t videoMode, bool_t topLine, bool_t bottomLine, int8_t showCb, int16_t showValue) {     //dr
    int16_t x1, y1, x2, y2;
    int16_t w;
    char l[15];

  if((calcMode == CM_GRAPH || calcMode == CM_PLOT_STAT) && xSoftkey >= 2) {           //JM prevent softkeys columns 3-6 from displaying over the graph
      return;
  }

    if(0 <= xSoftkey && xSoftkey <= 5) {
      x1 = 67 * xSoftkey - 1;
      x2 = x1 + 67;
    }
    else {
      sprintf(errorMessage, "In function showSoftkey: xSoftkey=%" PRId16 " must be from 0 to 5" , xSoftkey);
      displayBugScreen(errorMessage);
      return;
    }

    if(0 <= ySoftKey && ySoftKey <= 2) {
      y1 = 217 - SOFTMENU_HEIGHT * ySoftKey;
      y2 = y1 + SOFTMENU_HEIGHT;
    }
    else {
      sprintf(errorMessage, "In function showSoftkey: ySoftKey=%" PRId16 " but must be from 0 to 2!" , ySoftKey);
      displayBugScreen(errorMessage);
      return;
    }

    // Draw the frame
    //   Top line
    if(topLine) {
      lcd_fill_rect(max(0, x1), y1, min(x2, SCREEN_WIDTH) - max(0, x1), 1, (videoMode == vmNormal ? LCD_EMPTY_VALUE : LCD_SET_VALUE));
    }

    //   Bottom line
    if(y1 + SOFTMENU_HEIGHT <= min(y2, 239) && bottomLine) {
      lcd_fill_rect(max(0, x1), y1 + SOFTMENU_HEIGHT, min(x2, SCREEN_WIDTH) - max(0, x1), 1, (videoMode == vmNormal ? LCD_EMPTY_VALUE : LCD_SET_VALUE));
    }

    //   Left line
    if(x1 >= 0) {
      lcd_fill_rect(x1, y1, 1, min(y2, SCREEN_HEIGHT - 1) + 1 - y1, (videoMode == vmNormal ? LCD_EMPTY_VALUE : LCD_SET_VALUE));
    }

    //   Right line
    if(x2 < SCREEN_WIDTH) {
      lcd_fill_rect(x2, y1, 1, min(y2, SCREEN_HEIGHT - 1) + 1 - y1, (videoMode == vmNormal ? LCD_EMPTY_VALUE : LCD_SET_VALUE));
    }

    // Clear inside the frame
    lcd_fill_rect(x1 + 1, y1 + 1, min(x2, SCREEN_WIDTH) - x1 - 1, min(y2, SCREEN_HEIGHT) - y1 - 1, (videoMode == vmNormal ? LCD_SET_VALUE : LCD_EMPTY_VALUE));

    xcopy(l, label, stringByteLength(label) + 1);
    w = stringWidth(l, &standardFont, false, false);
    while(w > (xSoftkey == 5 ? 65 : 66)) {
      l[stringLastGlyph(l)] = 0;
      w = stringWidth(l, &standardFont, false, false);
    }

//continue with trimmed label
  w = stringWidth(figlabel(l, showValue), &standardFont, false, false);                      //JM & dr vv
  if((showCb >= 0) || (w >= 50)) {
    w = stringWidth(figlabel(l, showValue), &standardFont, false, false);
    if(showCb >= 0) { w = w + 8; }
    compressString = 1;       //JM compressString
    showString(figlabel(l, showValue), &standardFont, compressString + x1 + (xSoftkey == 5 ? 33 : 34) - w/2, y1 + 2, videoMode, false, false);
//  showString(l, &standardFont, x1 + (xSoftkey == 5 ? 33 : 34) - w/2, y1 + 2, videoMode, false, false);
    compressString = 0;       //JM compressString
  }
  else {
//  w = stringWidth(l, &standardFont, false, false);
     showString(figlabel(l, showValue), &standardFont, x1 + (xSoftkey == 5 ? 33 : 34) - w/2, y1 + 2, videoMode, false, false);
  }                                                                                              //JM & dr ^^

//  w = stringWidth(l, &standardFont, false, false);
//  if(showCb >= 0) { compressString = 1; w = w +2; }         //JM compressString
//  showString(l, &standardFont, x1 + 33 - w/2, y1 + 2, videoMode, false, false);
//  if(showCb >= 0) { compressString = 0; }                   //JM unCompressString

#ifdef JM_LINE2_DRAW
  if(showCb >= 0) {
    if(videoMode == vmNormal) {
      JM_LINE2(x2, y2);
    }
  }
#endif

  //vv EXTRA DRAWINGS FOR RADIO_BUTTON AND CHECK_BOX
  if(showCb >= 0) {
    if(videoMode == vmNormal) {
      if(showCb == RB_FALSE) {
        RB_UNCHECKED(x2-11, y2-16);
      }
      else if(showCb == RB_TRUE) {
        RB_CHECKED(x2-11, y2-16);
      }
      else if(showCb == CB_TRUE) {
        CB_CHECKED(x2-11, y2-16);
      }
      else {
        CB_UNCHECKED(x2-11, y2-16);
      }
    }
  }
  //^^
}



  /********************************************//**
   * \brief Displays the current part of the displayed softmenu
   *
   * \param void
   * \return void
   ***********************************************/
  void showSoftmenuCurrentPart(void) {

//JMTOCHECK: Removed exceptions for underline removal. 
    int16_t x, y, yDotted=0, currentFirstItem, item, numberOfItems, m = softmenuStack[0].softmenuId;
    bool_t dottedTopLine;
    #ifdef PC_BUILD
      char tmp[200]; sprintf(tmp,"^^^^showSoftmenuCurrentPart: Showing Softmenu id=%d\n",m); jm_show_comment(tmp);
    #endif //PC_BUILD
    if(!(m==0 && jm_NO_BASE_SCREEN) && calcMode != CM_FLAG_BROWSER && calcMode != CM_FONT_BROWSER && calcMode != CM_REGISTER_BROWSER && calcMode != CM_BUG_ON_SCREEN) {           //JM: Added exclusions, as this procedure is not only called from refreshScreen, but from various places due to underline
    clearScreen_old(false, false, true); //JM, added to ensure the f/g underlines are deleted

    if(m < NUMBER_OF_DYNAMIC_SOFTMENUS) { // Dynamic softmenu
      initVariableSoftmenu(m);
      numberOfItems = dynamicSoftmenu[m].numItems;
    }
    else { // Static softmenu
      numberOfItems = softmenu[m].numItems;
    }
    currentFirstItem = softmenuStack[0].firstItem;

//JMTOCHECK
/*
    //JMvv Temporary method to ensure AIM is active if the 3 ALPHA menus are shown //JM TOCHECK
    if((softmenuStackPointer > 0) && (calcMode != CM_AIM && (softmenu[m].menuId == -MNU_ALPHA || softmenu[m].menuId == -MNU_T_EDIT || softmenu[m].menuId == -MNU_MyAlpha))) {
      calcMode = CM_AIM;
      cursorFont = &standardFont;
      cursorEnabled = true;
      setSystemFlag(FLAG_ALPHA);
      refreshRegisterLine(AIM_REGISTER_LINE);
    } //JM ^^
*/
    ULFL = false;                                   //JM Underline
    ULGL = false;                                   //JM Underline


    if(numberOfItems <= 18) {
      dottedTopLine = false;
    }
    else {
      dottedTopLine = true;
      yDotted = min(3, (numberOfItems + modulo(currentFirstItem - numberOfItems, 6))/6 - currentFirstItem/6) - 1;

      if(m >= NUMBER_OF_DYNAMIC_SOFTMENUS) { // Static softmenu
        item = 6 * (currentFirstItem / 6 + yDotted);
        if(                softmenu[m].softkeyItem[item]==0 && softmenu[m].softkeyItem[item+1]==0 && softmenu[m].softkeyItem[item+2]==0 && softmenu[m].softkeyItem[item+3]==0 && softmenu[m].softkeyItem[item+4]==0 && softmenu[m].softkeyItem[item+5]==0) {
          yDotted--;
        }

        item = 6 * (currentFirstItem / 6 + yDotted);
        if(yDotted >= 0 && softmenu[m].softkeyItem[item]==0 && softmenu[m].softkeyItem[item+1]==0 && softmenu[m].softkeyItem[item+2]==0 && softmenu[m].softkeyItem[item+3]==0 && softmenu[m].softkeyItem[item+4]==0 && softmenu[m].softkeyItem[item+5]==0) {
          yDotted--;
        }

        item = 6 * (currentFirstItem / 6 + yDotted);
        if(yDotted >= 0 && softmenu[m].softkeyItem[item]==0 && softmenu[m].softkeyItem[item+1]==0 && softmenu[m].softkeyItem[item+2]==0 && softmenu[m].softkeyItem[item+3]==0 && softmenu[m].softkeyItem[item+4]==0 && softmenu[m].softkeyItem[item+5]==0) {
          yDotted--;
        }
      }
    }

    if(m < NUMBER_OF_DYNAMIC_SOFTMENUS) { // Dynamic softmenu
      if(numberOfItems == 0) {
        for(x=0; x<6; x++) {
          showSoftkey("", x, 0, vmNormal, true, true, NOVAL, NOVAL);
        }
      }
      else {
        uint8_t *ptr = getNthString(dynamicSoftmenu[m].menuContent, currentFirstItem);
        for(y=0; y<3; y++) {
          for(x=0; x<6; x++) {
            if(x + 6*y + currentFirstItem < numberOfItems) {
              showSoftkey((char *)ptr, x, y, vmNormal, true, true, NOVAL, NOVAL);
              ptr += stringByteLength((char *)ptr) + 1;
            }
          }
        }
      }
    }
    else {
      const int16_t *softkeyItem = softmenu[m].softkeyItem + currentFirstItem;
      for(y=currentFirstItem/6; y<=min(currentFirstItem/6+2, numberOfItems/6); y++, softkeyItem+=6) {
        for(x=0; x<6; x++) {
          if(softkeyItem + x >= softmenu[m].softkeyItem + numberOfItems) {
            item = ITM_NULL;
          }
          else {

              int16_t xx = x + y*6;
              //printf("x:%d y:%d 6y:%d xx:%d menu_A_HOME[xx]=%d menuId=%d currentFirstItem=%d/18=%d --> ",x,y,6*y,xx,menu_A_HOME[xx],softmenu[softmenuStack[0].softmenu].menuId,currentFirstItem,currentFirstItem/18);  //JMHOME
              if(  menu_A_HOME[xx] >= 0  &&  softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_HOME)
              {                                          //JMHOME
                  if(menu_A_HOME[xx] < 100) {item = !getSystemFlag(FLAG_USER) ? (kbd_std[menu_A_HOME[xx]    ].primary ) : (kbd_usr[menu_A_HOME[xx]    ].primary );}             else
                  if(menu_A_HOME[xx] < 200) {item = !getSystemFlag(FLAG_USER) ? (kbd_std[menu_A_HOME[xx]-100].fShifted) : (kbd_usr[menu_A_HOME[xx]-100].fShifted);}             else
                  if(menu_A_HOME[xx]>= 200) {item = !getSystemFlag(FLAG_USER) ? (kbd_std[menu_A_HOME[xx]-200].gShifted) : (kbd_usr[menu_A_HOME[xx]-200].gShifted);}
                  //printf("item (std/usr)=%d \n",item);                              //JMHOME
                  
                  if(!getSystemFlag(FLAG_USER) && menu_A_HOME[xx] == 0 && (calcMode == CM_NORMAL || calcMode == CM_NIM) && (Norm_Key_00_VAR != kbd_std[0].primary)){
                      item = Norm_Key_00_VAR;
                  }
                 
              }
              else {                                                                     //JMHOME vv

            item = softkeyItem[x];
                //printf("item (-1)=%d \n",item);                 
              }
            }
            int8_t showCb = fnCbIsSet(item%10000);                                  //dr
            int16_t showValue = fnItemShowValue(item%10000);                        //dr

          if(item < 0) { // softmenu
            int16_t menu = 0;
            while(softmenu[menu].menuItem != 0) {
              if(softmenu[menu].menuItem == item) {
                break;
              }
              menu++;
            }

            if(softmenu[menu].menuItem == 0) {
              sprintf(errorMessage, "In function showSoftmenuCurrentPart: softmenu ID %" PRId16 " not found!", item);
              displayBugScreen(errorMessage);
            }
//                showSoftkey(indexOfItems[-softmenu[menu].menuId].itemSoftmenuName, x, y-currentFirstItem/6, vmReverse, true, true);
            else if(softmenu[menu].menuItem == -MNU_ALPHA_OMEGA && alphaCase == AC_UPPER) { //JMvv
                showSoftkey(indexOfItems[MNU_ALPHA_OMEGA].itemSoftmenuName, x, y-currentFirstItem/6, vmReverse, true, true, NOVAL, NOVAL);
            }     //JM ^^
            else if(softmenu[menu].menuItem == -MNU_ALPHA_OMEGA && alphaCase == AC_LOWER) { //JMvv
                showSoftkey(indexOfItems[MNU_alpha_omega].itemSoftmenuName, x, y-currentFirstItem/6, vmReverse, true, true, NOVAL, NOVAL);
            }
            else {
                  //JMCHECKvv
#ifdef INLINE_TEST                                                              //vv dr
              if(softmenu[menu].menuItem == -MNU_INL_TST) {
                showSoftkey(/*STD_omicron*/STD_SPACE_3_PER_EM, x, y-currentFirstItem/6, vmNormal, false, false, NOVAL, NOVAL);
              }
              else {
                  //JMCHECKvv

#endif                                                                          //^^
              showSoftkey(indexOfItems[-softmenu[menu].menuItem].itemSoftmenuName, x, y-currentFirstItem/6, vmReverse, true, true, NOVAL, NOVAL);
#ifdef INLINE_TEST                                                              //vv dr
              }
#endif                                                                          //^^

              }
            }


        else                                                                   //JMXEQvv
        if(softmenu[m].menuItem == -MNU_XEQ) {
          if(indexOfItems[item%10000].func == fnXEQMENU) {
            showSoftkey(indexOfItemsXEQM + 8*(item%10000-fnXEQMENUpos),  x, y-currentFirstItem/6, vmNormal, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1, showCb, showValue);
          } else
            showSoftkey(indexOfItems    [item%10000].itemSoftmenuName,  x, y-currentFirstItem/6, vmNormal, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1, showCb, showValue);
        }                                                                      //JMXEQ^^
        else                                                                   //JMvv add radiobuttons
        if(softmenu[m].menuItem == -MNU_SYSFL) {
          if(indexOfItems[item%10000].itemCatalogName[0] != 0) {
            if(isSystemFlagWriteProtected(indexOfItems[item%10000].param)) {
              showSoftkey(indexOfItems[item%10000].itemCatalogName,  x, y-currentFirstItem/6, vmNormal, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1, showCb, getSystemFlag(indexOfItems[item%10000].param) ?  1 : 0);
            }
            else {
              showSoftkey(indexOfItems[item%10000].itemCatalogName,  x, y-currentFirstItem/6, vmNormal, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1, getSystemFlag(indexOfItems[item%10000].param) ?  CB_TRUE : CB_FALSE, NOVAL);
            }
          }
        }                                                                      //JM^^

            else if(item == 9999) {
              showSoftkey(indexOfItems[getSystemFlag(FLAG_MULTx) ? ITM_DOT : ITM_CROSS].itemSoftmenuName, x, y-currentFirstItem/6, vmNormal, true, true, NOVAL, NOVAL);
            }
            else if(item > 0 && indexOfItems[item%10000].itemSoftmenuName[0] != 0) { // softkey
              // item : +10000 -> no top line
              //        +20000 -> no bottom line
              //        +30000 -> neither top nor bottom line
              if(softmenu[m].menuItem == -MNU_FCNS) {
                showSoftkey(indexOfItems[item%10000].itemCatalogName,  x, y-currentFirstItem/6, vmNormal, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1, showCb, showValue);
              }

              else                                                                            //JM vv display i or j properly on display
              if(item%10000 == ITM_op_j && getSystemFlag(FLAG_CPXj)) {
                showSoftkey(STD_j, x, y-currentFirstItem/6, vmNormal, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1, showCb, showValue);
              }
              else if(item%10000 == ITM_op_j && !getSystemFlag(FLAG_CPXj)) {
                showSoftkey(STD_i, x, y-currentFirstItem/6, vmNormal, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1, showCb, showValue);
              }                                                                                //JM ^^
              else if((item == ITM_CFG) || (item ==ITM_PLOT_LR) || (item == ITM_PLOT_J) || (item == ITM_PLOT) || (item == ITM_PLOTLS)) {       //JMvv colour PLOT in reverse font to appear to be menus
                showSoftkey(indexOfItems[item%10000].itemSoftmenuName, x, y-currentFirstItem/6, vmReverse, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1, showCb, showValue);
              }                                                                                //JM^^

              else {
                showSoftkey(indexOfItems[item%10000].itemSoftmenuName, x, y-currentFirstItem/6, vmNormal, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1, showCb, showValue);
              }

              if(jm_G_DOUBLETAP && softmenu[m].menuItem == -MNU_ALPHA && y == 0 ){  // Indicate disabled double tap
                int16_t yStrokeA = SCREEN_HEIGHT - (y-currentFirstItem/6)*23 - 1;
                int16_t xStrokeA=x*67 + 66 -12;                                     //JM mod Show double press which does not work anymore
//                showString(STD_EMPTY_SET, &standardFont, xStrokeA +2, yStrokeA -16-3, vmNormal, false, false);
                plotline(xStrokeA +2+4, yStrokeA -16-3, xStrokeA +2+4+5, yStrokeA -16-3+5);
              }


              if(indexOfItems[item%10000].func == itemToBeCoded) {
                // Strike out non coded functions
                int16_t yStroke = SCREEN_HEIGHT - (y-currentFirstItem/6)*23 - 1;
                for(int16_t xStroke=x*67 + 1 +9 ; xStroke<x*67 + 66 -10; xStroke++) {      //JM mod stroke slash cross out
                  if(xStroke%3 == 0) yStroke--;
                  setBlackPixel(xStroke, yStroke -3);                                      //JM mod
                }
              }
            }
          }
        }
      }

    if(0 <= yDotted && yDotted <= 2) {
      yDotted = 217 - SOFTMENU_HEIGHT * yDotted;

      if(dottedTopLine) {
        for(x=0; x<SCREEN_WIDTH; x++) {
          if(x%8 < 4) {
            setBlackPixel(x, yDotted);
          }
          else {
            setWhitePixel(x, yDotted);
          }
        }


                                         //JMvv    //triangle centre point  // Triangles indicating more menus
        #define t 5
        #define t_o 1.6*t                                                             //offset
        #define tt_o 2                                                             //total offset
        lcd_fill_rect(0,(uint32_t)(yDotted-t), 20,t+1, 0);
        uint32_t xx;
        for(xx=0; xx<=t; xx++) {
          lcd_fill_rect(xx,       (uint32_t)(tt_o-t + yDotted-xx+t),   2*(t-xx), 1 ,true );
          lcd_fill_rect(xx + t_o, (uint32_t)(tt_o-t + yDotted-t+xx+t), 2*(t-xx), 1 ,true );
                                                                            //JM ^^
        }
      }
    }
    showShiftState(); //JM
  }
}




void fnMenuDump(uint16_t menu, uint16_t item) {                              //JMvv procedure to dump all menus. First page only. To mod todump all pages

  doRefreshSoftMenu = true;
  showSoftmenu(softmenu[menu].menuItem);
  softmenuStack[0].firstItem += item;
  showSoftmenuCurrentPart();

#ifdef PC_BUILD
  FILE *bmp;
  char bmpFileName[22];
  int32_t x, y;
  uint32_t uint32;
  uint16_t uint16;
  uint8_t  uint8;

  gtk_widget_queue_draw(screen);
  while(gtk_events_pending()) {
    gtk_main_iteration();
  }
 
  printf(">>> %s\n",indexOfItems[-softmenu[menu].menuItem].itemSoftmenuName);

  sprintf(bmpFileName,"Menu_%4d%4d.bmp",menu,item);
  bmp = fopen(bmpFileName, "wb");

  fwrite("BM", 1, 2, bmp);        // Offset 0x00  0  BMP header

  uint32 = (SCREEN_WIDTH/8 * (SCREEN_HEIGHT-171)) + 610;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x02  2  File size

  uint32 = 0;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x06  6  unused

  uint32 = 0x00000082;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x0a 10  Offset where the bitmap data can be found

  uint32 = 0x0000006c;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x0e 14  Number of bytes in DIB header

  uint32 = SCREEN_WIDTH;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x12 18  Bitmap width

  uint32 = SCREEN_HEIGHT-171;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x16 22  Bitmap height

  uint16 = 0x0001;
  fwrite(&uint16, 1, 2, bmp);     // Offset 0x1a 26  Number of planes

  uint16 = 0x0001;
  fwrite(&uint16, 1, 2, bmp);     // Offset 0x1c 28  Number of bits per pixel

  uint32 = 0;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x1e 30  Compression

  uint32 = 0x000030c0;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x22 34  Size of bitmap data (including padding)

  uint32 = 0x00001a7c; // 6780 pixels/m
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x26 38  Horizontal print resolution

  uint32 = 0x00001a7c; // 6780 pixels/m
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x2a 42  Vertical print resolution

  uint32 = 0x00000002;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x2e 46  Number of colors in the palette

  uint32 = 0x00000002;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x32 50  Number of important colors

  uint32 = 0x73524742;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x36  ???

  uint32 = 0;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x3a  ???
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x3e  ???
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x42  ???
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x46  ???
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x4a  ???
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x4e  ???
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x52  ???
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x56  ???
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x5a  ???
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x5e  ???
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x62  ???
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x66  ???

  uint32 = 0x00000002;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x6a  ???

  uint32 = 0;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x6e  ???
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x72  ???
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x76  ???

  uint32 = 0x00dff5cc; // light green
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x7a  RGB color for 0

  uint32 = 0;
  fwrite(&uint32, 1, 4, bmp);     // Offset 0x7e  RGB color for 1

  // Offset 0x82  bit map data
  uint16 = 0;
  uint32 = 0;
  for(y=SCREEN_HEIGHT-1; y>=171; y--) {
    for(x=0; x<SCREEN_WIDTH; x++) {
      uint8 <<= 1;
      if(*(screenData + y*screenStride + x) == ON_PIXEL) {
        uint8 |= 1;
      }

      if((x % 8) == 7) {
        fwrite(&uint8, 1, 1, bmp);
        uint8 = 0;
      }
    }
    fwrite(&uint16, 1, 2, bmp); // Padding
  }


  fclose(bmp);
  popSoftmenu();
#endif
}




                                                              //JM ^^



  /********************************************//**
   * \brief Pushes a new softmenu on the softmenu stack
   *
   * \param[in] softmenuId int16_t Softmenu ID
   * \return void
   ***********************************************/
  static void pushSoftmenu(int16_t softmenuId) {
    int i;
//    if(running_program_jm) return;                             //JM

    #ifdef PC_BUILD
      char tmp[300]; sprintf(tmp,">>> ...... pushing id:%d name:%s\n",softmenuId, indexOfItems[-softmenu[softmenuId].menuItem].itemSoftmenuName); jm_show_comment(tmp);
    #endif //PC_BUILD
    if(softmenuStack[0].softmenuId == softmenuId) { // The menu to push on the stack is already displayed
      return;
    }

    for(i=0; i<SOFTMENU_STACK_SIZE; i++) { // Searching the stack for the menu to push on the stack
      if(softmenuStack[i].softmenuId == softmenuId) { // if found, remove it
        xcopy(softmenuStack + 1, softmenuStack, i * sizeof(softmenuStack_t));
        break;
      }
    }

    if(i == SOFTMENU_STACK_SIZE) { // The menu to push was not found on the stack
      xcopy(softmenuStack + 1, softmenuStack, (SOFTMENU_STACK_SIZE - 1) * sizeof(softmenuStack_t)); // shifting the entire stack
    }


    softmenuStack[0].softmenuId = softmenuId;
    softmenuStack[0].firstItem = lastCatalogPosition[catalog];
   
      doRefreshSoftMenu = true;     //dr
  }



  /********************************************//**
   * \brief Pops a softmenu from the softmenu stack
   *
   * \param[in] softmenu int16_t Softmenu number
   * \return void
   ***********************************************/
  void popSoftmenu(void) {
//    if(running_program_jm) return;                             //JM

    xcopy(softmenuStack, softmenuStack + 1, (SOFTMENU_STACK_SIZE - 1) * sizeof(softmenuStack_t)); // shifting the entire stack
    memset(softmenuStack + SOFTMENU_STACK_SIZE - 1, 0, sizeof(softmenuStack_t)); // Put MyMenu in the last stack element

    doRefreshSoftMenu = true;     //dr

    if(softmenuStack[0].softmenuId == 0 && calcMode == CM_AIM) { // MyMenu displayed and in AIM
      softmenuStack[0].softmenuId = 1; // MyAlpha
    }
    else if(softmenuStack[0].softmenuId == 1 && calcMode != CM_AIM) { // MyAlpha displayed and not in AIM
      softmenuStack[0].softmenuId = 0; // MyMenu
    }
    else

    if(softmenuStack[0].softmenuId == 0 && SH_BASE_HOME && calcMode != CM_AIM) {                  //JM vv
      softmenuStack[0].softmenuId = mm_MNU_HOME;
    } 
//    else if(softmenuStack[0].softmenuId == 0 && jm_NO_BASE_SCREEN && calcMode != CM_AIM) {
//      softmenuStack[0].softmenuId = 0; //0=MyMenu JM for blank screen
//    } 
    else if(softmenuStack[0].softmenuId == 1 && SH_BASE_AHOME && calcMode == CM_AIM) {
      softmenuStack[0].softmenuId = mm_MNU_ALPHA;
    } 
                                                              //JM ^^
//        softmenuStack[0].firstItem = 0;

    enterAsmModeIfMenuIsACatalog(softmenu[softmenuStack[0].softmenuId].menuItem);

    #ifdef PC_BUILD
      jm_show_calc_state("popped");
      char tmp[300]; sprintf(tmp,">>> ...... popped into [0]: Id:%d Name:%s\n",softmenuStack[0].softmenuId, indexOfItems[-softmenu[softmenuStack[0].softmenuId].menuItem].itemSoftmenuName); jm_show_comment(tmp);
    #endif //PC_BUILD
  }



  /********************************************//**
   * \brief Displays a softmenu.
   *
   * \param[in] id int16_t       ID of softmenu
   * \return void
   ***********************************************/
  void showSoftmenu(int16_t id) {
//    if(running_program_jm) return;                             //JM
    int16_t m;
    #ifdef PC_BUILD
      char tmp[200]; sprintf(tmp,"^^^^showSoftmenu: Showing Softmenu id=%d\n",id); jm_show_comment(tmp);
    #endif //PC_BUILD

    enterAsmModeIfMenuIsACatalog(id);

    if(id == 0) {
      displayBugScreen("In function showSoftmenu: id must not be 0!");
      return;
    }

    if(id == -MNU_ALPHAINTL && alphaCase == AC_LOWER) { // alphaINTL
      id = -MNU_ALPHAintl;
    }
    else if(id == -MNU_ALPHA_OMEGA && alphaCase == AC_LOWER) { // alpha...omega
      id = -MNU_alpha_omega;
    }

    m = 0;
    while(softmenu[m].menuItem != 0) {
      if(softmenu[m].menuItem == id) {
       if(!tam.mode) softmenuStack[0].firstItem = lastCatalogPosition[catalog];
       break;
      }
      m++;
    }

    if(softmenu[m].menuItem == 0) {
      sprintf(errorMessage, "In function showSoftmenu: softmenu %" PRId16 " not found!", id);
      displayBugScreen(errorMessage);
    }
    else {
      if(tam.mode) {
        numberOfTamMenusToPop++;
      }
      pushSoftmenu(m);
    }
  }



  void setCatalogLastPos(void) {
    lastCatalogPosition[catalog] = (catalog ? softmenuStack[0].firstItem : 0);

    if(catalog == CATALOG_AINT) {
      lastCatalogPosition[CATALOG_aint] = softmenuStack[0].firstItem;
    }
    else if(catalog == CATALOG_aint) {
      lastCatalogPosition[CATALOG_AINT] = softmenuStack[0].firstItem;
    }
  }

  bool_t currentSoftmenuScrolls(void) {
    int16_t menuId = softmenuStack[0].softmenuId;
    return (menuId > 1 &&
      (   (menuId <  NUMBER_OF_DYNAMIC_SOFTMENUS && dynamicSoftmenu[menuId].numItems > 18)
       || (menuId >= NUMBER_OF_DYNAMIC_SOFTMENUS &&        softmenu[menuId].numItems > 18)));
  }

  bool_t isAlphabeticSoftmenu(void) {
    int16_t menuItem = softmenu[softmenuStack[0].softmenuId].menuItem;
    switch(menuItem) {
      case -MNU_ALPHAINTL:
      case -MNU_ALPHAintl:
      case -MNU_ALPHA_OMEGA:
      case -MNU_alpha_omega:
      case -MNU_ALPHAMATH:
      case -MNU_MyAlpha:
      case -MNU_ALPHADOT:
      case -MNU_T_EDIT:  //JM
      case -MNU_ALPHA:   //JM
        return true;
      default:
        return false;
    }
  }

  bool_t isJMAlphaSoftmenu(int16_t menuId) {
    int16_t menuItem = softmenu[menuId].menuItem;
    switch(menuItem) {
      case -MNU_MyAlpha:
      case -MNU_T_EDIT:  //JM
      case -MNU_ALPHA:   //JM
        return true;
      default:
        return false;
    }
  }


#endif // !TESTSUITE_BUILD

char *dynmenuGetLabel(int16_t menuitem) {
  if(menuitem < 0 || menuitem >= dynamicSoftmenu[softmenuStack[0].softmenuId].numItems) {
    return "";
  }
  char *labelName = (char *)dynamicSoftmenu[softmenuStack[0].softmenuId].menuContent;
  while(menuitem > 0) {
    labelName += stringByteLength(labelName) + 1;
    menuitem--;
  }
  return labelName;
}


void fnDumpMenus(uint16_t unusedButMandatoryParameter) {                      //JM
#ifdef PC_BUILD
  int16_t m,n;
  m = 0;
    while(softmenu[m].menuItem != 0) {
      n=0;
      while(n <= softmenu[m].numItems) {
        printf("m=%d n=%d\n",m,n );
        fnMenuDump(m, n);
        n += 18;
      }
      m++;
    }
#endif //PC_BUILD
}                                                                            //JM^^


