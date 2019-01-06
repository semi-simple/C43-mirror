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
 * \file testMathematics.h
 ***********************************************/

#include "wp43s.h"

#ifdef PC_BUILD
/********************************************//**
 * \brief Tests with 1 or 2 examples each function
 *
 * \param void
 * \return void
 ***********************************************/
void testFunctions(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("testFunctions");
  #endif

  #ifdef PC_BUILD
    printf("  indexOfItems\n");
    if(strcmp(indexOfItems[ITM_CtoF                     ].itemPrinted, STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F"                                )) {printf("item %" FMT16S " is not ITM_CtoF                     \n", ITM_CtoF                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_FtoC                     ].itemPrinted, STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C"                                )) {printf("item %" FMT16S " is not ITM_FtoC                     \n", ITM_FtoC                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_10x                      ].itemPrinted, "10" STD_SUP_x                                                               )) {printf("item %" FMT16S " is not ITM_10x                      \n", ITM_10x                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_1COMPL                   ].itemPrinted, "1COMPL"                                                                     )) {printf("item %" FMT16S " is not ITM_1COMPL                   \n", ITM_1COMPL                   ); exit(0);}
    if(strcmp(indexOfItems[CST_00                       ].itemPrinted, STD_ONE_HALF                                                                 )) {printf("item %" FMT16S " is not CST_00                       \n", CST_00                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_1X                       ].itemPrinted, "1/x"                                                                        )) {printf("item %" FMT16S " is not ITM_1X                       \n", ITM_1X                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_2COMPL                   ].itemPrinted, "2COMPL"                                                                     )) {printf("item %" FMT16S " is not ITM_2COMPL                   \n", ITM_2COMPL                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_2X                       ].itemPrinted, "2" STD_SUP_x                                                                )) {printf("item %" FMT16S " is not ITM_2X                       \n", ITM_2X                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_CUBEROOT                 ].itemPrinted, STD_CUBIC_ROOT STD_x_UNDER_ROOT                                              )) {printf("item %" FMT16S " is not ITM_CUBEROOT                 \n", ITM_CUBEROOT                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_REGA                     ].itemPrinted, "A"                                                                          )) {printf("item %" FMT16S " is not ITM_REGA                     \n", ITM_REGA                     ); exit(0);}
    if(strcmp(indexOfItems[CST_01                       ].itemPrinted, "a"                                                                          )) {printf("item %" FMT16S " is not CST_01                       \n", CST_01                       ); exit(0);}
    if(strcmp(indexOfItems[CST_02                       ].itemPrinted, "a" STD_SUB_0                                                                )) {printf("item %" FMT16S " is not CST_02                       \n", CST_02                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_ABS                      ].itemPrinted, "ABS"                                                                        )) {printf("item %" FMT16S " is not ITM_ABS                      \n", ITM_ABS                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_ACC                      ].itemPrinted, "ACC"                                                                        )) {printf("item %" FMT16S " is not ITM_ACC                      \n", ITM_ACC                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_ACtoM2                   ].itemPrinted, "acres"                                                                      )) {printf("item %" FMT16S " is not ITM_ACtoM2                   \n", ITM_ACtoM2                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_ACUStoM2                 ].itemPrinted, "acres" STD_US                                                               )) {printf("item %" FMT16S " is not ITM_ACUStoM2                 \n", ITM_ACUStoM2                 ); exit(0);}
    if(strcmp(indexOfItems[MNU_ADV                      ].itemPrinted, "ADV"                                                                        )) {printf("item %" FMT16S " is not MNU_ADV                      \n", MNU_ADV                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_AGM                      ].itemPrinted, "AGM"                                                                        )) {printf("item %" FMT16S " is not ITM_AGM                      \n", ITM_AGM                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_AGRAPH                   ].itemPrinted, "AGRAPH"                                                                     )) {printf("item %" FMT16S " is not ITM_AGRAPH                   \n", ITM_AGRAPH                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_ALL                      ].itemPrinted, "ALL"                                                                        )) {printf("item %" FMT16S " is not ITM_ALL                      \n", ITM_ALL                      ); exit(0);}
    if(strcmp(indexOfItems[CST_03                       ].itemPrinted, "a" STD_SUB_M                                                                )) {printf("item %" FMT16S " is not CST_03                       \n", CST_03                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_AND                      ].itemPrinted, "AND"                                                                        )) {printf("item %" FMT16S " is not ITM_AND                      \n", ITM_AND                      ); exit(0);}
    if(strcmp(indexOfItems[MNU_ANGLES                   ].itemPrinted, "ANGLES"                                                                     )) {printf("item %" FMT16S " is not MNU_ANGLES                   \n", MNU_ANGLES                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_arccos                   ].itemPrinted, "arccos"                                                                     )) {printf("item %" FMT16S " is not ITM_arccos                   \n", ITM_arccos                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_arcosh                   ].itemPrinted, "arcosh"                                                                     )) {printf("item %" FMT16S " is not ITM_arcosh                   \n", ITM_arcosh                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_arcsin                   ].itemPrinted, "arcsin"                                                                     )) {printf("item %" FMT16S " is not ITM_arcsin                   \n", ITM_arcsin                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_arctan                   ].itemPrinted, "arctan"                                                                     )) {printf("item %" FMT16S " is not ITM_arctan                   \n", ITM_arctan                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_arsinh                   ].itemPrinted, "arsinh"                                                                     )) {printf("item %" FMT16S " is not ITM_arsinh                   \n", ITM_arsinh                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_artanh                   ].itemPrinted, "artanh"                                                                     )) {printf("item %" FMT16S " is not ITM_artanh                   \n", ITM_artanh                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_ASR                      ].itemPrinted, "ASR"                                                                        )) {printf("item %" FMT16S " is not ITM_ASR                      \n", ITM_ASR                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_ASSIGN                   ].itemPrinted, "ASN"                                                                        )) {printf("item %" FMT16S " is not ITM_ASSIGN                   \n", ITM_ASSIGN                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_ATMtoPA                  ].itemPrinted, "atm"                                                                        )) {printf("item %" FMT16S " is not ITM_ATMtoPA                  \n", ITM_ATMtoPA                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_AUtoM                    ].itemPrinted, "au" STD_RIGHT_ARROW "m"                                                     )) {printf("item %" FMT16S " is not ITM_AUtoM                    \n", ITM_AUtoM                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_A_Z                      ].itemPrinted, "A" STD_ELLIPSIS "Z"                                                         )) {printf("item %" FMT16S " is not MNU_A_Z                      \n", MNU_A_Z                      ); exit(0);}
    if(strcmp(indexOfItems[MNU_CONVA                    ].itemPrinted, "A:"                                                                         )) {printf("item %" FMT16S " is not MNU_CONVA                    \n", MNU_CONVA                    ); exit(0);}
    if(strcmp(indexOfItems[CST_04                       ].itemPrinted, "a" STD_SUB_EARTH                                                            )) {printf("item %" FMT16S " is not CST_04                       \n", CST_04                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_REGB                     ].itemPrinted, "B"                                                                          )) {printf("item %" FMT16S " is not ITM_REGB                     \n", ITM_REGB                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_BACK                     ].itemPrinted, "BACK"                                                                       )) {printf("item %" FMT16S " is not ITM_BACK                     \n", ITM_BACK                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_BARtoPA                  ].itemPrinted, "bar" STD_RIGHT_ARROW "Pa"                                                   )) {printf("item %" FMT16S " is not ITM_BARtoPA                  \n", ITM_BARtoPA                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_BATT                     ].itemPrinted, "BATT?"                                                                      )) {printf("item %" FMT16S " is not ITM_BATT                     \n", ITM_BATT                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_BC                       ].itemPrinted, "BC?"                                                                        )) {printf("item %" FMT16S " is not ITM_BC                       \n", ITM_BC                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_BEEP                     ].itemPrinted, "BEEP"                                                                       )) {printf("item %" FMT16S " is not ITM_BEEP                     \n", ITM_BEEP                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_BEGINP                   ].itemPrinted, "Begin"                                                                      )) {printf("item %" FMT16S " is not ITM_BEGINP                   \n", ITM_BEGINP                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_BESTF                    ].itemPrinted, "BestF"                                                                      )) {printf("item %" FMT16S " is not ITM_BESTF                    \n", ITM_BESTF                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_BINOM                    ].itemPrinted, "Binom"                                                                      )) {printf("item %" FMT16S " is not ITM_BINOM                    \n", ITM_BINOM                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_BINOMP                   ].itemPrinted, "Binom" STD_SUB_p                                                            )) {printf("item %" FMT16S " is not ITM_BINOMP                   \n", ITM_BINOMP                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_BINOMU                   ].itemPrinted, "Binom" STD_SUB_u                                                            )) {printf("item %" FMT16S " is not ITM_BINOMU                   \n", ITM_BINOMU                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_BINOMM1                  ].itemPrinted, "Binom" STD_SUP_MINUS_1                                                      )) {printf("item %" FMT16S " is not ITM_BINOMM1                  \n", ITM_BINOMM1                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_BINOM                    ].itemPrinted, "Binom:"                                                                     )) {printf("item %" FMT16S " is not MNU_BINOM                    \n", MNU_BINOM                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_BITS                     ].itemPrinted, "BITS"                                                                       )) {printf("item %" FMT16S " is not MNU_BITS                     \n", MNU_BITS                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_BN                       ].itemPrinted, "B" STD_SUB_n                                                                )) {printf("item %" FMT16S " is not ITM_BN                       \n", ITM_BN                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_BNS                      ].itemPrinted, "B" STD_SUB_n STD_SUP_ASTERISK                                               )) {printf("item %" FMT16S " is not ITM_BNS                      \n", ITM_BNS                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_BS                       ].itemPrinted, "BS?"                                                                        )) {printf("item %" FMT16S " is not ITM_BS                       \n", ITM_BS                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_BTUtoJ                   ].itemPrinted, "Btu" STD_RIGHT_ARROW "J"                                                    )) {printf("item %" FMT16S " is not ITM_BTUtoJ                   \n", ITM_BTUtoJ                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_REGC                     ].itemPrinted, "C"                                                                          )) {printf("item %" FMT16S " is not ITM_REGC                     \n", ITM_REGC                     ); exit(0);}
    if(strcmp(indexOfItems[CST_05                       ].itemPrinted, "c"                                                                          )) {printf("item %" FMT16S " is not CST_05                       \n", CST_05                       ); exit(0);}
    if(strcmp(indexOfItems[CST_06                       ].itemPrinted, "c" STD_SUB_1                                                                )) {printf("item %" FMT16S " is not CST_06                       \n", CST_06                       ); exit(0);}
    if(strcmp(indexOfItems[CST_07                       ].itemPrinted, "c" STD_SUB_2                                                                )) {printf("item %" FMT16S " is not CST_07                       \n", CST_07                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_CALtoJ                   ].itemPrinted, "cal" STD_RIGHT_ARROW "J"                                                    )) {printf("item %" FMT16S " is not ITM_CALtoJ                   \n", ITM_CALtoJ                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_CASE                     ].itemPrinted, "CASE"                                                                       )) {printf("item %" FMT16S " is not ITM_CASE                     \n", ITM_CASE                     ); exit(0);}
    if(strcmp(indexOfItems[MNU_CATALOG                  ].itemPrinted, "CATALOG"                                                                    )) {printf("item %" FMT16S " is not MNU_CATALOG                  \n", MNU_CATALOG                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_CAUCH                    ].itemPrinted, "Cauch"                                                                      )) {printf("item %" FMT16S " is not ITM_CAUCH                    \n", ITM_CAUCH                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_CAUCHP                   ].itemPrinted, "Cauch" STD_SUB_p                                                            )) {printf("item %" FMT16S " is not ITM_CAUCHP                   \n", ITM_CAUCHP                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_CAUCHU                   ].itemPrinted, "Cauch" STD_SUB_u                                                            )) {printf("item %" FMT16S " is not ITM_CAUCHU                   \n", ITM_CAUCHU                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_CAUCHM1                  ].itemPrinted, "Cauch" STD_SUP_MINUS_1                                                      )) {printf("item %" FMT16S " is not ITM_CAUCHM1                  \n", ITM_CAUCHM1                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_CAUCH                    ].itemPrinted, "Cauch:"                                                                     )) {printf("item %" FMT16S " is not MNU_CAUCH                    \n", MNU_CAUCH                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_CB                       ].itemPrinted, "CB"                                                                         )) {printf("item %" FMT16S " is not ITM_CB                       \n", ITM_CB                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_CEIL                     ].itemPrinted, "CEIL"                                                                       )) {printf("item %" FMT16S " is not ITM_CEIL                     \n", ITM_CEIL                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_CF                       ].itemPrinted, "CF"                                                                         )) {printf("item %" FMT16S " is not ITM_CF                       \n", ITM_CF                       ); exit(0);}
    if(strcmp(indexOfItems[MNU_CHARS                    ].itemPrinted, "CHARS"                                                                      )) {printf("item %" FMT16S " is not MNU_CHARS                    \n", MNU_CHARS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_CLALL                    ].itemPrinted, "CLall"                                                                      )) {printf("item %" FMT16S " is not ITM_CLALL                    \n", ITM_CLALL                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_CLCVAR                   ].itemPrinted, "CLCVAR"                                                                     )) {printf("item %" FMT16S " is not ITM_CLCVAR                   \n", ITM_CLCVAR                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_CLFALL                   ].itemPrinted, "CLFall"                                                                     )) {printf("item %" FMT16S " is not ITM_CLFALL                   \n", ITM_CLFALL                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_CLK                      ].itemPrinted, "CLK"                                                                        )) {printf("item %" FMT16S " is not MNU_CLK                      \n", MNU_CLK                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_CLK12                    ].itemPrinted, "CLK12"                                                                      )) {printf("item %" FMT16S " is not ITM_CLK12                    \n", ITM_CLK12                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_CLK24                    ].itemPrinted, "CLK24"                                                                      )) {printf("item %" FMT16S " is not ITM_CLK24                    \n", ITM_CLK24                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_CLLCD                    ].itemPrinted, "CLLCD"                                                                      )) {printf("item %" FMT16S " is not ITM_CLLCD                    \n", ITM_CLLCD                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_CLMENU                   ].itemPrinted, "CLMENU"                                                                     )) {printf("item %" FMT16S " is not ITM_CLMENU                   \n", ITM_CLMENU                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_CLP                      ].itemPrinted, "CLP"                                                                        )) {printf("item %" FMT16S " is not ITM_CLP                      \n", ITM_CLP                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_CLPALL                   ].itemPrinted, "CLPall"                                                                     )) {printf("item %" FMT16S " is not ITM_CLPALL                   \n", ITM_CLPALL                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_CLR                      ].itemPrinted, "CLR"                                                                        )) {printf("item %" FMT16S " is not MNU_CLR                      \n", MNU_CLR                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_CLREGS                   ].itemPrinted, "CLREGS"                                                                     )) {printf("item %" FMT16S " is not ITM_CLREGS                   \n", ITM_CLREGS                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_CLSTK                    ].itemPrinted, "CLSTK"                                                                      )) {printf("item %" FMT16S " is not ITM_CLSTK                    \n", ITM_CLSTK                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_CLX                      ].itemPrinted, "CLX"                                                                        )) {printf("item %" FMT16S " is not ITM_CLX                      \n", ITM_CLX                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_CLSIGMA                  ].itemPrinted, "CL" STD_SIGMA                                                               )) {printf("item %" FMT16S " is not ITM_CLSIGMA                  \n", ITM_CLSIGMA                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_CNST                     ].itemPrinted, "CNST"                                                                       )) {printf("item %" FMT16S " is not MNU_CNST                     \n", MNU_CNST                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_COMB                     ].itemPrinted, "C" STD_SUB_y STD_SUB_x                                                      )) {printf("item %" FMT16S " is not ITM_COMB                     \n", ITM_COMB                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_CONJ                     ].itemPrinted, "conj"                                                                       )) {printf("item %" FMT16S " is not ITM_CONJ                     \n", ITM_CONJ                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_CONST                    ].itemPrinted, "CONST"                                                                      )) {printf("item %" FMT16S " is not ITM_CONST                    \n", ITM_CONST                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_CONVG                    ].itemPrinted, "CONVG?"                                                                     )) {printf("item %" FMT16S " is not ITM_CONVG                    \n", ITM_CONVG                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_CORR                     ].itemPrinted, "r"                                                                          )) {printf("item %" FMT16S " is not ITM_CORR                     \n", ITM_CORR                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_cos                      ].itemPrinted, "cos"                                                                        )) {printf("item %" FMT16S " is not ITM_cos                      \n", ITM_cos                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_cosh                     ].itemPrinted, "cosh"                                                                       )) {printf("item %" FMT16S " is not ITM_cosh                     \n", ITM_cosh                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_COV                      ].itemPrinted, "cov"                                                                        )) {printf("item %" FMT16S " is not ITM_COV                      \n", ITM_COV                      ); exit(0);}
    if(strcmp(indexOfItems[MNU_CPX                      ].itemPrinted, "CPX"                                                                        )) {printf("item %" FMT16S " is not MNU_CPX                      \n", MNU_CPX                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_CPXI                     ].itemPrinted, "CPXi"                                                                       )) {printf("item %" FMT16S " is not ITM_CPXI                     \n", ITM_CPXI                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_CPXJ                     ].itemPrinted, "CPXj"                                                                       )) {printf("item %" FMT16S " is not ITM_CPXJ                     \n", ITM_CPXJ                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_CPXRES                   ].itemPrinted, "CPXRES"                                                                     )) {printf("item %" FMT16S " is not ITM_CPXRES                   \n", ITM_CPXRES                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_CPXS                     ].itemPrinted, "CPXS"                                                                       )) {printf("item %" FMT16S " is not MNU_CPXS                     \n", MNU_CPXS                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_CPX                      ].itemPrinted, "CPX?"                                                                       )) {printf("item %" FMT16S " is not ITM_CPX                      \n", ITM_CPX                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_CROSS                    ].itemPrinted, "cross"                                                                      )) {printf("item %" FMT16S " is not ITM_CROSS                    \n", ITM_CROSS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_CWTtoKG                  ].itemPrinted, "cwt" STD_RIGHT_ARROW "kg"                                                   )) {printf("item %" FMT16S " is not ITM_CWTtoKG                  \n", ITM_CWTtoKG                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_CXtoRE                   ].itemPrinted, "CX" STD_RIGHT_ARROW "RE"                                                    )) {printf("item %" FMT16S " is not ITM_CXtoRE                   \n", ITM_CXtoRE                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_REGD                     ].itemPrinted, "D"                                                                          )) {printf("item %" FMT16S " is not ITM_REGD                     \n", ITM_REGD                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_DATE                     ].itemPrinted, "DATE"                                                                       )) {printf("item %" FMT16S " is not ITM_DATE                     \n", ITM_DATE                     ); exit(0);}
    if(strcmp(indexOfItems[MNU_DATES                    ].itemPrinted, "DATES"                                                                      )) {printf("item %" FMT16S " is not MNU_DATES                    \n", MNU_DATES                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_DATEto                   ].itemPrinted, "DATE" STD_RIGHT_ARROW                                                       )) {printf("item %" FMT16S " is not ITM_DATEto                   \n", ITM_DATEto                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_DAY                      ].itemPrinted, "DAY"                                                                        )) {printf("item %" FMT16S " is not ITM_DAY                      \n", ITM_DAY                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_DBL                      ].itemPrinted, "DBL?"                                                                       )) {printf("item %" FMT16S " is not ITM_DBL                      \n", ITM_DBL                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_DBLR                     ].itemPrinted, "DBLR"                                                                       )) {printf("item %" FMT16S " is not ITM_DBLR                     \n", ITM_DBLR                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_DBLCROSS                 ].itemPrinted, "DBL" STD_CROSS                                                              )) {printf("item %" FMT16S " is not ITM_DBLCROSS                 \n", ITM_DBLCROSS                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_DBLSLASH                 ].itemPrinted, "DBL/"                                                                       )) {printf("item %" FMT16S " is not ITM_DBLSLASH                 \n", ITM_DBLSLASH                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_DBtoFR                   ].itemPrinted, "dB " STD_RIGHT_ARROW                                                        )) {printf("item %" FMT16S " is not ITM_DBtoFR                   \n", ITM_DBtoFR                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_DBtoPR                   ].itemPrinted, "dB " STD_RIGHT_ARROW                                                        )) {printf("item %" FMT16S " is not ITM_DBtoPR                   \n", ITM_DBtoPR                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_DEC                      ].itemPrinted, "DEC"                                                                        )) {printf("item %" FMT16S " is not ITM_DEC                      \n", ITM_DEC                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_DECOMP                   ].itemPrinted, "DECOMP"                                                                     )) {printf("item %" FMT16S " is not ITM_DECOMP                   \n", ITM_DECOMP                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_DEG                      ].itemPrinted, "DEG"                                                                        )) {printf("item %" FMT16S " is not ITM_DEG                      \n", ITM_DEG                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_DEGto                    ].itemPrinted, STD_MEASURED_ANGLE STD_DEGREE STD_RIGHT_ARROW STD_MEASURED_ANGLE             )) {printf("item %" FMT16S " is not ITM_DEGto                    \n", ITM_DEGto                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_DENANY                   ].itemPrinted, "DENANY"                                                                     )) {printf("item %" FMT16S " is not ITM_DENANY                   \n", ITM_DENANY                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_DENFAC                   ].itemPrinted, "DENFAC"                                                                     )) {printf("item %" FMT16S " is not ITM_DENFAC                   \n", ITM_DENFAC                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_DENFIX                   ].itemPrinted, "DENFIX"                                                                     )) {printf("item %" FMT16S " is not ITM_DENFIX                   \n", ITM_DENFIX                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_DENMAX                   ].itemPrinted, "DENMAX"                                                                     )) {printf("item %" FMT16S " is not ITM_DENMAX                   \n", ITM_DENMAX                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_DIGITS                   ].itemPrinted, "DIGITS"                                                                     )) {printf("item %" FMT16S " is not MNU_DIGITS                   \n", MNU_DIGITS                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_DISP                     ].itemPrinted, "DISP"                                                                       )) {printf("item %" FMT16S " is not MNU_DISP                     \n", MNU_DISP                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_DOT                      ].itemPrinted, "dot"                                                                        )) {printf("item %" FMT16S " is not ITM_DOT                      \n", ITM_DOT                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_toDP                     ].itemPrinted, STD_RIGHT_ARROW "DP"                                                         )) {printf("item %" FMT16S " is not ITM_toDP                     \n", ITM_toDP                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_DROP                     ].itemPrinted, "DROP"                                                                       )) {printf("item %" FMT16S " is not ITM_DROP                     \n", ITM_DROP                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_DROPY                    ].itemPrinted, "DROPy"                                                                      )) {printf("item %" FMT16S " is not ITM_DROPY                    \n", ITM_DROPY                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_DSE                      ].itemPrinted, "DSE"                                                                        )) {printf("item %" FMT16S " is not ITM_DSE                      \n", ITM_DSE                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_DSL                      ].itemPrinted, "DSL"                                                                        )) {printf("item %" FMT16S " is not ITM_DSL                      \n", ITM_DSL                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_DSP                      ].itemPrinted, "DSP"                                                                        )) {printf("item %" FMT16S " is not ITM_DSP                      \n", ITM_DSP                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_DSTACK                   ].itemPrinted, "DSTACK"                                                                     )) {printf("item %" FMT16S " is not ITM_DSTACK                   \n", ITM_DSTACK                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_DSZ                      ].itemPrinted, "DSZ"                                                                        )) {printf("item %" FMT16S " is not ITM_DSZ                      \n", ITM_DSZ                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_DMS                      ].itemPrinted, "d.ms"                                                                       )) {printf("item %" FMT16S " is not ITM_DMS                      \n", ITM_DMS                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_DMSto                    ].itemPrinted, STD_MEASURED_ANGLE STD_DOUBLE_HIGH_QUOTE STD_RIGHT_ARROW STD_MEASURED_ANGLE  )) {printf("item %" FMT16S " is not ITM_DMSto                    \n", ITM_DMSto                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_DMY                      ].itemPrinted, "D.MY"                                                                       )) {printf("item %" FMT16S " is not ITM_DMY                      \n", ITM_DMY                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_DtoJ                     ].itemPrinted, "D" STD_RIGHT_ARROW "J"                                                      )) {printf("item %" FMT16S " is not ITM_DtoJ                     \n", ITM_DtoJ                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_DtoR                     ].itemPrinted, STD_MEASURED_ANGLE STD_DEGREE STD_RIGHT_ARROW STD_MEASURED_ANGLE STD_SUP_r   )) {printf("item %" FMT16S " is not ITM_DtoR                     \n", ITM_DtoR                     ); exit(0);}
    if(strcmp(indexOfItems[CST_08                       ].itemPrinted, "e"                                                                          )) {printf("item %" FMT16S " is not CST_08                       \n", CST_08                       ); exit(0);}
    if(strcmp(indexOfItems[CST_09                       ].itemPrinted, "e" STD_SUB_E                                                                )) {printf("item %" FMT16S " is not CST_09                       \n", CST_09                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_EIGVAL                   ].itemPrinted, "EIGVAL"                                                                     )) {printf("item %" FMT16S " is not ITM_EIGVAL                   \n", ITM_EIGVAL                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_EIGVEC                   ].itemPrinted, "EIGVEC"                                                                     )) {printf("item %" FMT16S " is not ITM_EIGVEC                   \n", ITM_EIGVEC                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_END                      ].itemPrinted, "END"                                                                        )) {printf("item %" FMT16S " is not ITM_END                      \n", ITM_END                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_ENDP                     ].itemPrinted, "End"                                                                        )) {printf("item %" FMT16S " is not ITM_ENDP                     \n", ITM_ENDP                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_ENG                      ].itemPrinted, "ENG"                                                                        )) {printf("item %" FMT16S " is not ITM_ENG                      \n", ITM_ENG                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_ENGOVR                   ].itemPrinted, "ENGOVR"                                                                     )) {printf("item %" FMT16S " is not ITM_ENGOVR                   \n", ITM_ENGOVR                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_ENORM                    ].itemPrinted, "ENORM"                                                                      )) {printf("item %" FMT16S " is not ITM_ENORM                    \n", ITM_ENORM                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_ENTER                    ].itemPrinted, "ENTER" STD_UP_ARROW                                                         )) {printf("item %" FMT16S " is not ITM_ENTER                    \n", ITM_ENTER                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_ENTRY                    ].itemPrinted, "ENTRY?"                                                                     )) {printf("item %" FMT16S " is not ITM_ENTRY                    \n", ITM_ENTRY                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_EQN                      ].itemPrinted, "EQN"                                                                        )) {printf("item %" FMT16S " is not MNU_EQN                      \n", MNU_EQN                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_EQ_DEL                   ].itemPrinted, "DELETE"                                                                     )) {printf("item %" FMT16S " is not ITM_EQ_DEL                   \n", ITM_EQ_DEL                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_EQ_EDI                   ].itemPrinted, "EDIT"                                                                       )) {printf("item %" FMT16S " is not ITM_EQ_EDI                   \n", ITM_EQ_EDI                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_EQ_NEW                   ].itemPrinted, "NEW"                                                                        )) {printf("item %" FMT16S " is not ITM_EQ_NEW                   \n", ITM_EQ_NEW                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_ERF                      ].itemPrinted, "erf"                                                                        )) {printf("item %" FMT16S " is not ITM_ERF                      \n", ITM_ERF                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_ERFC                     ].itemPrinted, "erfc"                                                                       )) {printf("item %" FMT16S " is not ITM_ERFC                     \n", ITM_ERFC                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_ERR                      ].itemPrinted, "ERR"                                                                        )) {printf("item %" FMT16S " is not ITM_ERR                      \n", ITM_ERR                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_EVEN                     ].itemPrinted, "EVEN?"                                                                      )) {printf("item %" FMT16S " is not ITM_EVEN                     \n", ITM_EVEN                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_EX                       ].itemPrinted, "e" STD_SUP_x                                                                )) {printf("item %" FMT16S " is not ITM_EX                       \n", ITM_EX                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_EXITALL                  ].itemPrinted, "EXITall"                                                                    )) {printf("item %" FMT16S " is not ITM_EXITALL                  \n", ITM_EXITALL                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_EXP                      ].itemPrinted, "EXP"                                                                        )) {printf("item %" FMT16S " is not MNU_EXP                      \n", MNU_EXP                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_EXPF                     ].itemPrinted, "ExpF"                                                                       )) {printf("item %" FMT16S " is not ITM_EXPF                     \n", ITM_EXPF                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_EXPON                    ].itemPrinted, "Expon"                                                                      )) {printf("item %" FMT16S " is not ITM_EXPON                    \n", ITM_EXPON                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_EXPONP                   ].itemPrinted, "Expon" STD_SUB_p                                                            )) {printf("item %" FMT16S " is not ITM_EXPONP                   \n", ITM_EXPONP                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_EXPONU                   ].itemPrinted, "Expon" STD_SUB_u                                                            )) {printf("item %" FMT16S " is not ITM_EXPONU                   \n", ITM_EXPONU                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_EXPONM1                  ].itemPrinted, "Expon" STD_SUP_MINUS_1                                                      )) {printf("item %" FMT16S " is not ITM_EXPONM1                  \n", ITM_EXPONM1                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_EXPON                    ].itemPrinted, "Expon:"                                                                     )) {printf("item %" FMT16S " is not MNU_EXPON                    \n", MNU_EXPON                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_EXPT                     ].itemPrinted, "EXPT"                                                                       )) {printf("item %" FMT16S " is not ITM_EXPT                     \n", ITM_EXPT                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_EX1                      ].itemPrinted, "e" STD_SUP_x "-1"                                                           )) {printf("item %" FMT16S " is not ITM_EX1                      \n", ITM_EX1                      ); exit(0);}
    if(strcmp(indexOfItems[CST_10                       ].itemPrinted, "e/m" STD_SUB_e                                                              )) {printf("item %" FMT16S " is not CST_10                       \n", CST_10                       ); exit(0);}
    if(strcmp(indexOfItems[MNU_CONVE                    ].itemPrinted, "E:"                                                                         )) {printf("item %" FMT16S " is not MNU_CONVE                    \n", MNU_CONVE                    ); exit(0);}
    if(strcmp(indexOfItems[CST_11                       ].itemPrinted, "F"                                                                          )) {printf("item %" FMT16S " is not CST_11                       \n", CST_11                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_FAST                     ].itemPrinted, "FAST"                                                                       )) {printf("item %" FMT16S " is not ITM_FAST                     \n", ITM_FAST                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_FB                       ].itemPrinted, "FB"                                                                         )) {printf("item %" FMT16S " is not ITM_FB                       \n", ITM_FB                       ); exit(0);}
    if(strcmp(indexOfItems[MNU_FCNS                     ].itemPrinted, "FCNS"                                                                       )) {printf("item %" FMT16S " is not MNU_FCNS                     \n", MNU_FCNS                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_FC                       ].itemPrinted, "FC?"                                                                        )) {printf("item %" FMT16S " is not ITM_FC                       \n", ITM_FC                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_FCC                      ].itemPrinted, "FC?C"                                                                       )) {printf("item %" FMT16S " is not ITM_FCC                      \n", ITM_FCC                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_FCF                      ].itemPrinted, "FC?F"                                                                       )) {printf("item %" FMT16S " is not ITM_FCF                      \n", ITM_FCF                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_FCS                      ].itemPrinted, "FC?S"                                                                       )) {printf("item %" FMT16S " is not ITM_FCS                      \n", ITM_FCS                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_FTtoM                    ].itemPrinted, "ft." STD_RIGHT_ARROW "m"                                                    )) {printf("item %" FMT16S " is not ITM_FTtoM                    \n", ITM_FTtoM                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_FF                       ].itemPrinted, "FF"                                                                         )) {printf("item %" FMT16S " is not ITM_FF                       \n", ITM_FF                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_FIB                      ].itemPrinted, "FIB"                                                                        )) {printf("item %" FMT16S " is not ITM_FIB                      \n", ITM_FIB                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_FILL                     ].itemPrinted, "FILL"                                                                       )) {printf("item %" FMT16S " is not ITM_FILL                     \n", ITM_FILL                     ); exit(0);}
    if(strcmp(indexOfItems[MNU_FIN                      ].itemPrinted, "FIN"                                                                        )) {printf("item %" FMT16S " is not MNU_FIN                      \n", MNU_FIN                      ); exit(0);}
    if(strcmp(indexOfItems[MNU_FINTS                    ].itemPrinted, "FINTS"                                                                      )) {printf("item %" FMT16S " is not MNU_FINTS                    \n", MNU_FINTS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_FIX                      ].itemPrinted, "FIX"                                                                        )) {printf("item %" FMT16S " is not ITM_FIX                      \n", ITM_FIX                      ); exit(0);}
    if(strcmp(indexOfItems[MNU_FLAGS                    ].itemPrinted, "FLAGS"                                                                      )) {printf("item %" FMT16S " is not MNU_FLAGS                    \n", MNU_FLAGS                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_FLASH                    ].itemPrinted, "FLASH"                                                                      )) {printf("item %" FMT16S " is not MNU_FLASH                    \n", MNU_FLASH                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_FLASH                    ].itemPrinted, "FLASH?"                                                                     )) {printf("item %" FMT16S " is not ITM_FLASH                    \n", ITM_FLASH                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_FLOOR                    ].itemPrinted, "FLOOR"                                                                      )) {printf("item %" FMT16S " is not ITM_FLOOR                    \n", ITM_FLOOR                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_FP                       ].itemPrinted, "FP"                                                                         )) {printf("item %" FMT16S " is not ITM_FP                       \n", ITM_FP                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_FPQ                      ].itemPrinted, "FP?"                                                                        )) {printf("item %" FMT16S " is not ITM_FPQ                      \n", ITM_FPQ                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_FPX                      ].itemPrinted, "F" STD_SUB_p "(x)"                                                          )) {printf("item %" FMT16S " is not ITM_FPX                      \n", ITM_FPX                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_FUX                      ].itemPrinted, "F" STD_SUB_u "(x)"                                                          )) {printf("item %" FMT16S " is not ITM_FUX                      \n", ITM_FUX                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_FX                       ].itemPrinted, "F(x)"                                                                       )) {printf("item %" FMT16S " is not ITM_FX                       \n", ITM_FX                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_FM1P                     ].itemPrinted, "F" STD_SUP_MINUS_1 "(p)"                                                    )) {printf("item %" FMT16S " is not ITM_FM1P                     \n", ITM_FM1P                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_FRtoDB                   ].itemPrinted, "field"                                                                      )) {printf("item %" FMT16S " is not ITM_FRtoDB                   \n", ITM_FRtoDB                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_FS                       ].itemPrinted, "FS?"                                                                        )) {printf("item %" FMT16S " is not ITM_FS                       \n", ITM_FS                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_FSC                      ].itemPrinted, "FS?C"                                                                       )) {printf("item %" FMT16S " is not ITM_FSC                      \n", ITM_FSC                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_FSF                      ].itemPrinted, "FS?F"                                                                       )) {printf("item %" FMT16S " is not ITM_FSF                      \n", ITM_FSF                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_FSS                      ].itemPrinted, "FS?S"                                                                       )) {printf("item %" FMT16S " is not ITM_FSS                      \n", ITM_FSS                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_FTUStoM                  ].itemPrinted, "s:feet" STD_US                                                              )) {printf("item %" FMT16S " is not ITM_FTUStoM                  \n", ITM_FTUStoM                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_FV                       ].itemPrinted, "FV"                                                                         )) {printf("item %" FMT16S " is not ITM_FV                       \n", ITM_FV                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_FZUKtoM3                 ].itemPrinted, "floz" STD_UK                                                                )) {printf("item %" FMT16S " is not ITM_FZUKtoM3                 \n", ITM_FZUKtoM3                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_FZUStoM3                 ].itemPrinted, "floz" STD_US                                                                )) {printf("item %" FMT16S " is not ITM_FZUStoM3                 \n", ITM_FZUStoM3                 ); exit(0);}
    if(strcmp(indexOfItems[CST_12                       ].itemPrinted, "F" STD_SUB_alpha                                                            )) {printf("item %" FMT16S " is not CST_12                       \n", CST_12                       ); exit(0);}
    if(strcmp(indexOfItems[CST_13                       ].itemPrinted, "F" STD_SUB_delta                                                            )) {printf("item %" FMT16S " is not CST_13                       \n", CST_13                       ); exit(0);}
    if(strcmp(indexOfItems[MNU_F                        ].itemPrinted, "F:"                                                                         )) {printf("item %" FMT16S " is not MNU_F                        \n", MNU_F                        ); exit(0);}
    if(strcmp(indexOfItems[MNU_1STDERIV                 ].itemPrinted, "f" STD_RIGHT_SINGLE_QUOTE                                                   )) {printf("item %" FMT16S " is not MNU_1STDERIV                 \n", MNU_1STDERIV                 ); exit(0);}
    if(strcmp(indexOfItems[MNU_2NDDERIV                 ].itemPrinted, "f" STD_RIGHT_DOUBLE_QUOTE                                                   )) {printf("item %" FMT16S " is not MNU_2NDDERIV                 \n", MNU_2NDDERIV                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_FQX                      ].itemPrinted, "f" STD_RIGHT_SINGLE_QUOTE "(x)"                                             )) {printf("item %" FMT16S " is not ITM_FQX                      \n", ITM_FQX                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_FDQX                     ].itemPrinted, "f" STD_RIGHT_DOUBLE_QUOTE "(x)"                                             )) {printf("item %" FMT16S " is not ITM_FDQX                     \n", ITM_FDQX                     ); exit(0);}
    if(strcmp(indexOfItems[MNU_CONVFP                   ].itemPrinted, "F&p:"                                                                       )) {printf("item %" FMT16S " is not MNU_CONVFP                   \n", MNU_CONVFP                   ); exit(0);}
    if(strcmp(indexOfItems[CST_14                       ].itemPrinted, "G"                                                                          )) {printf("item %" FMT16S " is not CST_14                       \n", CST_14                       ); exit(0);}
    if(strcmp(indexOfItems[CST_15                       ].itemPrinted, "G" STD_SUB_0                                                                )) {printf("item %" FMT16S " is not CST_15                       \n", CST_15                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_GAP                      ].itemPrinted, "GAP"                                                                        )) {printf("item %" FMT16S " is not ITM_GAP                      \n", ITM_GAP                      ); exit(0);}
    if(strcmp(indexOfItems[CST_16                       ].itemPrinted, "G" STD_SUB_C                                                                )) {printf("item %" FMT16S " is not CST_16                       \n", CST_16                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_GCD                      ].itemPrinted, "GCD"                                                                        )) {printf("item %" FMT16S " is not ITM_GCD                      \n", ITM_GCD                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_GD                       ].itemPrinted, "g" STD_SUB_d                                                                )) {printf("item %" FMT16S " is not ITM_GD                       \n", ITM_GD                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_GDM1                     ].itemPrinted, "g" STD_SUB_d STD_SUP_MINUS_1                                                )) {printf("item %" FMT16S " is not ITM_GDM1                     \n", ITM_GDM1                     ); exit(0);}
    if(strcmp(indexOfItems[CST_17                       ].itemPrinted, "g" STD_SUB_e                                                                )) {printf("item %" FMT16S " is not CST_17                       \n", CST_17                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_GEOM                     ].itemPrinted, "Geom"                                                                       )) {printf("item %" FMT16S " is not ITM_GEOM                     \n", ITM_GEOM                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_GEOMP                    ].itemPrinted, "Geom" STD_SUB_p                                                             )) {printf("item %" FMT16S " is not ITM_GEOMP                    \n", ITM_GEOMP                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_GEOMU                    ].itemPrinted, "Geom" STD_SUB_u                                                             )) {printf("item %" FMT16S " is not ITM_GEOMU                    \n", ITM_GEOMU                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_GEOMM1                   ].itemPrinted, "Geom" STD_SUP_MINUS_1                                                       )) {printf("item %" FMT16S " is not ITM_GEOMM1                   \n", ITM_GEOMM1                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_GEOM                     ].itemPrinted, "Geom:"                                                                      )) {printf("item %" FMT16S " is not MNU_GEOM                     \n", MNU_GEOM                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_GLUKtoM3                 ].itemPrinted, "gl" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3                                    )) {printf("item %" FMT16S " is not ITM_GLUKtoM3                 \n", ITM_GLUKtoM3                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_GLUStoM3                 ].itemPrinted, "gl" STD_US STD_RIGHT_ARROW "m" STD_SUP_3                                    )) {printf("item %" FMT16S " is not ITM_GLUStoM3                 \n", ITM_GLUStoM3                 ); exit(0);}
    if(strcmp(indexOfItems[CST_18                       ].itemPrinted, "GM" STD_SUB_EARTH                                                           )) {printf("item %" FMT16S " is not CST_18                       \n", CST_18                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_GRAD                     ].itemPrinted, "GRAD"                                                                       )) {printf("item %" FMT16S " is not ITM_GRAD                     \n", ITM_GRAD                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_GRADto                   ].itemPrinted, STD_MEASURED_ANGLE STD_SUP_g STD_RIGHT_ARROW STD_MEASURED_ANGLE              )) {printf("item %" FMT16S " is not ITM_GRADto                   \n", ITM_GRADto                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_GTO                      ].itemPrinted, "GTO"                                                                        )) {printf("item %" FMT16S " is not ITM_GTO                      \n", ITM_GTO                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_GTOP                     ].itemPrinted, "GTO."                                                                       )) {printf("item %" FMT16S " is not ITM_GTOP                     \n", ITM_GTOP                     ); exit(0);}
    if(strcmp(indexOfItems[CST_19                       ].itemPrinted, "g" STD_SUB_EARTH                                                            )) {printf("item %" FMT16S " is not CST_19                       \n", CST_19                       ); exit(0);}
    if(strcmp(indexOfItems[CST_20                       ].itemPrinted, STD_PLANCK                                                                   )) {printf("item %" FMT16S " is not CST_20                       \n", CST_20                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_HN                       ].itemPrinted, "H" STD_SUB_n                                                                )) {printf("item %" FMT16S " is not ITM_HN                       \n", ITM_HN                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_HNP                      ].itemPrinted, "H" STD_SUB_n STD_SUB_p                                                      )) {printf("item %" FMT16S " is not ITM_HNP                      \n", ITM_HNP                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_HPEtoW                   ].itemPrinted, "hp" STD_SUB_E STD_RIGHT_ARROW "W"                                           )) {printf("item %" FMT16S " is not ITM_HPEtoW                   \n", ITM_HPEtoW                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_HPMtoW                   ].itemPrinted, "hp" STD_SUB_M STD_RIGHT_ARROW "W"                                           )) {printf("item %" FMT16S " is not ITM_HPMtoW                   \n", ITM_HPMtoW                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_HPUKtoW                  ].itemPrinted, "hp" STD_UK STD_RIGHT_ARROW "W"                                              )) {printf("item %" FMT16S " is not ITM_HPUKtoW                  \n", ITM_HPUKtoW                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_HYPER                    ].itemPrinted, "Hyper"                                                                      )) {printf("item %" FMT16S " is not ITM_HYPER                    \n", ITM_HYPER                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_HYPERP                   ].itemPrinted, "Hyper" STD_SUB_p                                                            )) {printf("item %" FMT16S " is not ITM_HYPERP                   \n", ITM_HYPERP                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_HYPERU                   ].itemPrinted, "Hyper" STD_SUB_u                                                            )) {printf("item %" FMT16S " is not ITM_HYPERU                   \n", ITM_HYPERU                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_HYPERM1                  ].itemPrinted, "Hyper" STD_SUP_MINUS_1                                                      )) {printf("item %" FMT16S " is not ITM_HYPERM1                  \n", ITM_HYPERM1                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_HYPER                    ].itemPrinted, "Hyper:"                                                                     )) {printf("item %" FMT16S " is not MNU_HYPER                    \n", MNU_HYPER                    ); exit(0);}
    if(strcmp(indexOfItems[CST_21                       ].itemPrinted, STD_PLANCK_2PI                                                               )) {printf("item %" FMT16S " is not CST_21                       \n", CST_21                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_REGI                     ].itemPrinted, "I"                                                                          )) {printf("item %" FMT16S " is not ITM_REGI                     \n", ITM_REGI                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_IDIV                     ].itemPrinted, "IDIV"                                                                       )) {printf("item %" FMT16S " is not ITM_IDIV                     \n", ITM_IDIV                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_IHGtoPA                  ].itemPrinted, "in.Hg"                                                                      )) {printf("item %" FMT16S " is not ITM_IHGtoPA                  \n", ITM_IHGtoPA                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_IINTS                    ].itemPrinted, "IINTS"                                                                      )) {printf("item %" FMT16S " is not MNU_IINTS                    \n", MNU_IINTS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_IM                       ].itemPrinted, "Im"                                                                         )) {printf("item %" FMT16S " is not ITM_IM                       \n", ITM_IM                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_IMPFRC                   ].itemPrinted, "d/c"                                                                        )) {printf("item %" FMT16S " is not ITM_IMPFRC                   \n", ITM_IMPFRC                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_INC                      ].itemPrinted, "INC"                                                                        )) {printf("item %" FMT16S " is not ITM_INC                      \n", ITM_INC                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_INDEX                    ].itemPrinted, "INDEX"                                                                      )) {printf("item %" FMT16S " is not ITM_INDEX                    \n", ITM_INDEX                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_INFO                     ].itemPrinted, "INFO"                                                                       )) {printf("item %" FMT16S " is not MNU_INFO                     \n", MNU_INFO                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_INPUT                    ].itemPrinted, "INPUT"                                                                      )) {printf("item %" FMT16S " is not ITM_INPUT                    \n", ITM_INPUT                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_INTS                     ].itemPrinted, "INTS"                                                                       )) {printf("item %" FMT16S " is not MNU_INTS                     \n", MNU_INTS                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_INT                      ].itemPrinted, "INT?"                                                                       )) {printf("item %" FMT16S " is not ITM_INT                      \n", ITM_INT                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_INtoM                    ].itemPrinted, "in." STD_RIGHT_ARROW "m"                                                    )) {printf("item %" FMT16S " is not ITM_INtoM                    \n", ITM_INtoM                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_IP                       ].itemPrinted, "IP"                                                                         )) {printf("item %" FMT16S " is not ITM_IP                       \n", ITM_IP                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_ISE                      ].itemPrinted, "ISE"                                                                        )) {printf("item %" FMT16S " is not ITM_ISE                      \n", ITM_ISE                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_ISG                      ].itemPrinted, "ISG"                                                                        )) {printf("item %" FMT16S " is not ITM_ISG                      \n", ITM_ISG                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_ISZ                      ].itemPrinted, "ISZ"                                                                        )) {printf("item %" FMT16S " is not ITM_ISZ                      \n", ITM_ISZ                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_IXYZ                     ].itemPrinted, "I" STD_SUB_x STD_SUB_y STD_SUB_z                                            )) {printf("item %" FMT16S " is not ITM_IXYZ                     \n", ITM_IXYZ                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_IGAMMAP                  ].itemPrinted, "I" STD_GAMMA STD_SUB_p                                                      )) {printf("item %" FMT16S " is not ITM_IGAMMAP                  \n", ITM_IGAMMAP                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_IGAMMAQ                  ].itemPrinted, "I" STD_GAMMA STD_SUB_q                                                      )) {printf("item %" FMT16S " is not ITM_IGAMMAQ                  \n", ITM_IGAMMAQ                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_IPLUS                    ].itemPrinted, "I+"                                                                         )) {printf("item %" FMT16S " is not ITM_IPLUS                    \n", ITM_IPLUS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_IMINUS                   ].itemPrinted, "I-"                                                                         )) {printf("item %" FMT16S " is not ITM_IMINUS                   \n", ITM_IMINUS                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_IO                       ].itemPrinted, "I/O"                                                                        )) {printf("item %" FMT16S " is not MNU_IO                       \n", MNU_IO                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_IPCA                     ].itemPrinted, "i%/a"                                                                       )) {printf("item %" FMT16S " is not ITM_IPCA                     \n", ITM_IPCA                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_REGJ                     ].itemPrinted, "J"                                                                          )) {printf("item %" FMT16S " is not ITM_REGJ                     \n", ITM_REGJ                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_JYX                      ].itemPrinted, "J" STD_SUB_y "(x)"                                                          )) {printf("item %" FMT16S " is not ITM_JYX                      \n", ITM_JYX                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_JPLUS                    ].itemPrinted, "J+"                                                                         )) {printf("item %" FMT16S " is not ITM_JPLUS                    \n", ITM_JPLUS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_JMINUS                   ].itemPrinted, "J-"                                                                         )) {printf("item %" FMT16S " is not ITM_JMINUS                   \n", ITM_JMINUS                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_JonG                     ].itemPrinted, "J/G"                                                                        )) {printf("item %" FMT16S " is not ITM_JonG                     \n", ITM_JonG                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_JtoBTU                   ].itemPrinted, "J" STD_RIGHT_ARROW "Btu"                                                    )) {printf("item %" FMT16S " is not ITM_JtoBTU                   \n", ITM_JtoBTU                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_JtoCAL                   ].itemPrinted, "J" STD_RIGHT_ARROW "cal"                                                    )) {printf("item %" FMT16S " is not ITM_JtoCAL                   \n", ITM_JtoCAL                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_JtoD                     ].itemPrinted, "J" STD_RIGHT_ARROW "D"                                                      )) {printf("item %" FMT16S " is not ITM_JtoD                     \n", ITM_JtoD                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_JtoWH                    ].itemPrinted, "J" STD_RIGHT_ARROW "Wh"                                                     )) {printf("item %" FMT16S " is not ITM_JtoWH                    \n", ITM_JtoWH                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_REGK                     ].itemPrinted, "K"                                                                          )) {printf("item %" FMT16S " is not ITM_REGK                     \n", ITM_REGK                     ); exit(0);}
    if(strcmp(indexOfItems[CST_22                       ].itemPrinted, "k"                                                                          )) {printf("item %" FMT16S " is not CST_22                       \n", CST_22                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_KEY                      ].itemPrinted, "KEY"                                                                        )) {printf("item %" FMT16S " is not ITM_KEY                      \n", ITM_KEY                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_KEYG                     ].itemPrinted, "KEYG"                                                                       )) {printf("item %" FMT16S " is not ITM_KEYG                     \n", ITM_KEYG                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_KEYX                     ].itemPrinted, "KEYX"                                                                       )) {printf("item %" FMT16S " is not ITM_KEYX                     \n", ITM_KEYX                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_KEYQ                     ].itemPrinted, "KEY?"                                                                       )) {printf("item %" FMT16S " is not ITM_KEYQ                     \n", ITM_KEYQ                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_KGtoCWT                  ].itemPrinted, "kg" STD_RIGHT_ARROW "cwt"                                                   )) {printf("item %" FMT16S " is not ITM_KGtoCWT                  \n", ITM_KGtoCWT                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_KGtoLBS                  ].itemPrinted, "kg" STD_RIGHT_ARROW "lbs"                                                   )) {printf("item %" FMT16S " is not ITM_KGtoLBS                  \n", ITM_KGtoLBS                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_KGtoOZ                   ].itemPrinted, "kg" STD_RIGHT_ARROW "oz"                                                    )) {printf("item %" FMT16S " is not ITM_KGtoOZ                   \n", ITM_KGtoOZ                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_KGtoSCW                  ].itemPrinted, "kg " STD_RIGHT_ARROW                                                        )) {printf("item %" FMT16S " is not ITM_KGtoSCW                  \n", ITM_KGtoSCW                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_KGtoSTO                  ].itemPrinted, "kg " STD_RIGHT_ARROW                                                        )) {printf("item %" FMT16S " is not ITM_KGtoSTO                  \n", ITM_KGtoSTO                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_KGtoST                   ].itemPrinted, "kg " STD_RIGHT_ARROW                                                        )) {printf("item %" FMT16S " is not ITM_KGtoST                   \n", ITM_KGtoST                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_KGtoTON                  ].itemPrinted, "kg " STD_RIGHT_ARROW                                                        )) {printf("item %" FMT16S " is not ITM_KGtoTON                  \n", ITM_KGtoTON                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_KGtoTRZ                  ].itemPrinted, "kg " STD_RIGHT_ARROW                                                        )) {printf("item %" FMT16S " is not ITM_KGtoTRZ                  \n", ITM_KGtoTRZ                  ); exit(0);}
    if(strcmp(indexOfItems[CST_23                       ].itemPrinted, "K" STD_SUB_J                                                                )) {printf("item %" FMT16S " is not CST_23                       \n", CST_23                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_KTYP                     ].itemPrinted, "KTYP?"                                                                      )) {printf("item %" FMT16S " is not ITM_KTYP                     \n", ITM_KTYP                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_REGL                     ].itemPrinted, "L"                                                                          )) {printf("item %" FMT16S " is not ITM_REGL                     \n", ITM_REGL                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_LASTX                    ].itemPrinted, "LASTx"                                                                      )) {printf("item %" FMT16S " is not ITM_LASTX                    \n", ITM_LASTX                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_LBFtoN                   ].itemPrinted, "lbf" STD_RIGHT_ARROW "N"                                                    )) {printf("item %" FMT16S " is not ITM_LBFtoN                   \n", ITM_LBFtoN                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_LBL                      ].itemPrinted, "LBL"                                                                        )) {printf("item %" FMT16S " is not ITM_LBL                      \n", ITM_LBL                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_LBLQ                     ].itemPrinted, "LBL?"                                                                       )) {printf("item %" FMT16S " is not ITM_LBLQ                     \n", ITM_LBLQ                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_LBStoKG                  ].itemPrinted, "lbs" STD_RIGHT_ARROW "kg"                                                   )) {printf("item %" FMT16S " is not ITM_LBStoKG                  \n", ITM_LBStoKG                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_LCM                      ].itemPrinted, "LCM"                                                                        )) {printf("item %" FMT16S " is not ITM_LCM                      \n", ITM_LCM                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_LEAP                     ].itemPrinted, "LEAP?"                                                                      )) {printf("item %" FMT16S " is not ITM_LEAP                     \n", ITM_LEAP                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_LGNRM                    ].itemPrinted, "LgNrm"                                                                      )) {printf("item %" FMT16S " is not ITM_LGNRM                    \n", ITM_LGNRM                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_LGNRMP                   ].itemPrinted, "LgNrm" STD_SUB_p                                                            )) {printf("item %" FMT16S " is not ITM_LGNRMP                   \n", ITM_LGNRMP                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_LGNRMU                   ].itemPrinted, "LgNrm" STD_SUB_u                                                            )) {printf("item %" FMT16S " is not ITM_LGNRMU                   \n", ITM_LGNRMU                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_LGNRMM1                  ].itemPrinted, "LgNrm" STD_SUP_MINUS_1                                                      )) {printf("item %" FMT16S " is not ITM_LGNRMM1                  \n", ITM_LGNRMM1                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_LGNRM                    ].itemPrinted, "LgNrm:"                                                                     )) {printf("item %" FMT16S " is not MNU_LGNRM                    \n", MNU_LGNRM                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_LINF                     ].itemPrinted, "LinF"                                                                       )) {printf("item %" FMT16S " is not ITM_LINF                     \n", ITM_LINF                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_LJ                       ].itemPrinted, "LJ"                                                                         )) {printf("item %" FMT16S " is not ITM_LJ                       \n", ITM_LJ                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_LN                       ].itemPrinted, "ln"                                                                         )) {printf("item %" FMT16S " is not ITM_LN                       \n", ITM_LN                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_Ln                       ].itemPrinted, "L" STD_SUB_n                                                                )) {printf("item %" FMT16S " is not ITM_Ln                       \n", ITM_Ln                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_LN1X                     ].itemPrinted, "ln 1+x"                                                                     )) {printf("item %" FMT16S " is not ITM_LN1X                     \n", ITM_LN1X                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_LNALPHA                  ].itemPrinted, "L" STD_SUB_n STD_SUB_alpha                                                  )) {printf("item %" FMT16S " is not ITM_LNALPHA                  \n", ITM_LNALPHA                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_LNBETA                   ].itemPrinted, "ln" STD_beta                                                                )) {printf("item %" FMT16S " is not ITM_LNBETA                   \n", ITM_LNBETA                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_LNGAMMA                  ].itemPrinted, "ln" STD_GAMMA                                                               )) {printf("item %" FMT16S " is not ITM_LNGAMMA                  \n", ITM_LNGAMMA                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_LOAD                     ].itemPrinted, "LOAD"                                                                       )) {printf("item %" FMT16S " is not ITM_LOAD                     \n", ITM_LOAD                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_LOADP                    ].itemPrinted, "LOADP"                                                                      )) {printf("item %" FMT16S " is not ITM_LOADP                    \n", ITM_LOADP                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_LOADR                    ].itemPrinted, "LOADR"                                                                      )) {printf("item %" FMT16S " is not ITM_LOADR                    \n", ITM_LOADR                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_LOADSS                   ].itemPrinted, "LOADSS"                                                                     )) {printf("item %" FMT16S " is not ITM_LOADSS                   \n", ITM_LOADSS                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_LOADSIGMA                ].itemPrinted, "LOAD" STD_SIGMA                                                             )) {printf("item %" FMT16S " is not ITM_LOADSIGMA                \n", ITM_LOADSIGMA                ); exit(0);}
    if(strcmp(indexOfItems[ITM_LocR                     ].itemPrinted, "LocR"                                                                       )) {printf("item %" FMT16S " is not ITM_LocR                     \n", ITM_LocR                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_LocRQ                    ].itemPrinted, "LocR?"                                                                      )) {printf("item %" FMT16S " is not ITM_LocRQ                    \n", ITM_LocRQ                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_LOG10                    ].itemPrinted, "lg"                                                                         )) {printf("item %" FMT16S " is not ITM_LOG10                    \n", ITM_LOG10                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_LOG2                     ].itemPrinted, "lb x"                                                                       )) {printf("item %" FMT16S " is not ITM_LOG2                     \n", ITM_LOG2                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_LOGF                     ].itemPrinted, "LogF"                                                                       )) {printf("item %" FMT16S " is not ITM_LOGF                     \n", ITM_LOGF                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_LOGIS                    ].itemPrinted, "Logis"                                                                      )) {printf("item %" FMT16S " is not ITM_LOGIS                    \n", ITM_LOGIS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_LOGISP                   ].itemPrinted, "Logis" STD_SUB_p                                                            )) {printf("item %" FMT16S " is not ITM_LOGISP                   \n", ITM_LOGISP                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_LOGISU                   ].itemPrinted, "Logis" STD_SUB_u                                                            )) {printf("item %" FMT16S " is not ITM_LOGISU                   \n", ITM_LOGISU                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_LOGISM1                  ].itemPrinted, "Logis" STD_SUP_MINUS_1                                                      )) {printf("item %" FMT16S " is not ITM_LOGISM1                  \n", ITM_LOGISM1                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_LOGIS                    ].itemPrinted, "Logis:"                                                                     )) {printf("item %" FMT16S " is not MNU_LOGIS                    \n", MNU_LOGIS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_LOGXY                    ].itemPrinted, "log" STD_SUB_x "y"                                                          )) {printf("item %" FMT16S " is not ITM_LOGXY                    \n", ITM_LOGXY                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_LOOP                     ].itemPrinted, "LOOP"                                                                       )) {printf("item %" FMT16S " is not MNU_LOOP                     \n", MNU_LOOP                     ); exit(0);}
    if(strcmp(indexOfItems[CST_24                       ].itemPrinted, "l" STD_SUB_p                                                                )) {printf("item %" FMT16S " is not CST_24                       \n", CST_24                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_LYtoM                    ].itemPrinted, "ly" STD_RIGHT_ARROW "m"                                                     )) {printf("item %" FMT16S " is not ITM_LYtoM                    \n", ITM_LYtoM                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_LZOFF                    ].itemPrinted, "LZOFF"                                                                      )) {printf("item %" FMT16S " is not ITM_LZOFF                    \n", ITM_LZOFF                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_LZON                     ].itemPrinted, "LZON"                                                                       )) {printf("item %" FMT16S " is not ITM_LZON                     \n", ITM_LZON                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_LR                       ].itemPrinted, "L.R."                                                                       )) {printf("item %" FMT16S " is not ITM_LR                       \n", ITM_LR                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_M2toAC                   ].itemPrinted, "m" STD_SUP_2 " " STD_RIGHT_ARROW                                            )) {printf("item %" FMT16S " is not ITM_M2toAC                   \n", ITM_M2toAC                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_M2toACUS                 ].itemPrinted, "m" STD_SUP_2 " " STD_RIGHT_ARROW                                            )) {printf("item %" FMT16S " is not ITM_M2toACUS                 \n", ITM_M2toACUS                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_M3toFZUK                 ].itemPrinted, "m" STD_SUP_3 " " STD_RIGHT_ARROW                                            )) {printf("item %" FMT16S " is not ITM_M3toFZUK                 \n", ITM_M3toFZUK                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_M3toFZUS                 ].itemPrinted, "m" STD_SUP_3 " " STD_RIGHT_ARROW                                            )) {printf("item %" FMT16S " is not ITM_M3toFZUS                 \n", ITM_M3toFZUS                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_M3toGLUK                 ].itemPrinted, "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_UK                                    )) {printf("item %" FMT16S " is not ITM_M3toGLUK                 \n", ITM_M3toGLUK                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_M3toGLUS                 ].itemPrinted, "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_US                                    )) {printf("item %" FMT16S " is not ITM_M3toGLUS                 \n", ITM_M3toGLUS                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_MANT                     ].itemPrinted, "MANT"                                                                       )) {printf("item %" FMT16S " is not ITM_MANT                     \n", ITM_MANT                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MASKL                    ].itemPrinted, "MASKL"                                                                      )) {printf("item %" FMT16S " is not ITM_MASKL                    \n", ITM_MASKL                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_MASKR                    ].itemPrinted, "MASKR"                                                                      )) {printf("item %" FMT16S " is not ITM_MASKR                    \n", ITM_MASKR                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_MATRS                    ].itemPrinted, "MATRS"                                                                      )) {printf("item %" FMT16S " is not MNU_MATRS                    \n", MNU_MATRS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_MATR                     ].itemPrinted, "MATR?"                                                                      )) {printf("item %" FMT16S " is not ITM_MATR                     \n", ITM_MATR                     ); exit(0);}
    if(strcmp(indexOfItems[MNU_MATX                     ].itemPrinted, "MATX"                                                                       )) {printf("item %" FMT16S " is not MNU_MATX                     \n", MNU_MATX                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MATA                     ].itemPrinted, "Mat A"                                                                      )) {printf("item %" FMT16S " is not ITM_MATA                     \n", ITM_MATA                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MATB                     ].itemPrinted, "Mat B"                                                                      )) {printf("item %" FMT16S " is not ITM_MATB                     \n", ITM_MATB                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MATX                     ].itemPrinted, "Mat X"                                                                      )) {printf("item %" FMT16S " is not ITM_MATX                     \n", ITM_MATX                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MAX                      ].itemPrinted, "max"                                                                        )) {printf("item %" FMT16S " is not ITM_MAX                      \n", ITM_MAX                      ); exit(0);}
    if(strcmp(indexOfItems[CST_25                       ].itemPrinted, "m" STD_SUB_e                                                                )) {printf("item %" FMT16S " is not CST_25                       \n", CST_25                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_MEM                      ].itemPrinted, "MEM?"                                                                       )) {printf("item %" FMT16S " is not ITM_MEM                      \n", ITM_MEM                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_MENU                     ].itemPrinted, "MENU"                                                                       )) {printf("item %" FMT16S " is not ITM_MENU                     \n", ITM_MENU                     ); exit(0);}
    if(strcmp(indexOfItems[MNU_MENUS                    ].itemPrinted, "MENUS"                                                                      )) {printf("item %" FMT16S " is not MNU_MENUS                    \n", MNU_MENUS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_MIN                      ].itemPrinted, "min"                                                                        )) {printf("item %" FMT16S " is not ITM_MIN                      \n", ITM_MIN                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_MIRROR                   ].itemPrinted, "MIRROR"                                                                     )) {printf("item %" FMT16S " is not ITM_MIRROR                   \n", ITM_MIRROR                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_MItoM                    ].itemPrinted, "mi." STD_RIGHT_ARROW "m"                                                    )) {printf("item %" FMT16S " is not ITM_MItoM                    \n", ITM_MItoM                    ); exit(0);}
    if(strcmp(indexOfItems[CST_26                       ].itemPrinted, "M" STD_SUB_M                                                                )) {printf("item %" FMT16S " is not CST_26                       \n", CST_26                       ); exit(0);}
    if(strcmp(indexOfItems[CST_27                       ].itemPrinted, "m" STD_SUB_n                                                                )) {printf("item %" FMT16S " is not CST_27                       \n", CST_27                       ); exit(0);}
    if(strcmp(indexOfItems[CST_28                       ].itemPrinted, "m" STD_SUB_n "/m" STD_SUB_p                                                 )) {printf("item %" FMT16S " is not CST_28                       \n", CST_28                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_MOD                      ].itemPrinted, "MOD"                                                                        )) {printf("item %" FMT16S " is not ITM_MOD                      \n", ITM_MOD                      ); exit(0);}
    if(strcmp(indexOfItems[MNU_MODE                     ].itemPrinted, "MODE"                                                                       )) {printf("item %" FMT16S " is not MNU_MODE                     \n", MNU_MODE                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MONTH                    ].itemPrinted, "MONTH"                                                                      )) {printf("item %" FMT16S " is not ITM_MONTH                    \n", ITM_MONTH                    ); exit(0);}
    if(strcmp(indexOfItems[CST_29                       ].itemPrinted, "M" STD_SUB_p                                                                )) {printf("item %" FMT16S " is not CST_29                       \n", CST_29                       ); exit(0);}
    if(strcmp(indexOfItems[CST_30                       ].itemPrinted, "m" STD_SUB_p                                                                )) {printf("item %" FMT16S " is not CST_30                       \n", CST_30                       ); exit(0);}
    if(strcmp(indexOfItems[CST_31                       ].itemPrinted, "m" STD_SUB_p "/m" STD_SUB_e                                                 )) {printf("item %" FMT16S " is not CST_31                       \n", CST_31                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_MSG                      ].itemPrinted, "MSG"                                                                        )) {printf("item %" FMT16S " is not ITM_MSG                      \n", ITM_MSG                      ); exit(0);}
    if(strcmp(indexOfItems[CST_32                       ].itemPrinted, "m" STD_SUB_u                                                                )) {printf("item %" FMT16S " is not CST_32                       \n", CST_32                       ); exit(0);}
    if(strcmp(indexOfItems[CST_33                       ].itemPrinted, "m" STD_SUB_u "c" STD_SUP_2                                                  )) {printf("item %" FMT16S " is not CST_33                       \n", CST_33                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_MULTCR                   ].itemPrinted, "MULT" STD_CROSS                                                             )) {printf("item %" FMT16S " is not ITM_MULTCR                   \n", ITM_MULTCR                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_MULTDOT                  ].itemPrinted, "MULT" STD_DOT                                                               )) {printf("item %" FMT16S " is not ITM_MULTDOT                  \n", ITM_MULTDOT                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_MULPI                    ].itemPrinted, "MUL" STD_pi                                                                 )) {printf("item %" FMT16S " is not ITM_MULPI                    \n", ITM_MULPI                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_MVAR                     ].itemPrinted, "MVAR"                                                                       )) {printf("item %" FMT16S " is not ITM_MVAR                     \n", ITM_MVAR                     ); exit(0);}
    if(strcmp(indexOfItems[MNU_MYMENU                   ].itemPrinted, "MyMenu"                                                                     )) {printf("item %" FMT16S " is not MNU_MYMENU                   \n", MNU_MYMENU                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_MyAlpha                  ].itemPrinted, "My" STD_alpha                                                               )) {printf("item %" FMT16S " is not MNU_MyAlpha                  \n", MNU_MyAlpha                  ); exit(0);}
    if(strcmp(indexOfItems[CST_34                       ].itemPrinted, "m" STD_SUB_mu                                                               )) {printf("item %" FMT16S " is not CST_34                       \n", CST_34                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_DELR                   ].itemPrinted, "DELR"                                                                       )) {printf("item %" FMT16S " is not ITM_M_DELR                   \n", ITM_M_DELR                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_DIM                    ].itemPrinted, "DIM"                                                                        )) {printf("item %" FMT16S " is not ITM_M_DIM                    \n", ITM_M_DIM                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_DIMQ                   ].itemPrinted, "DIM?"                                                                       )) {printf("item %" FMT16S " is not ITM_M_DIMQ                   \n", ITM_M_DIMQ                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_MDY                      ].itemPrinted, "M.DY"                                                                       )) {printf("item %" FMT16S " is not ITM_MDY                      \n", ITM_MDY                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_EDI                    ].itemPrinted, "EDIT"                                                                       )) {printf("item %" FMT16S " is not ITM_M_EDI                    \n", ITM_M_EDI                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_M_EDITN                  ].itemPrinted, "EDITN"                                                                      )) {printf("item %" FMT16S " is not MNU_M_EDITN                  \n", MNU_M_EDITN                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_M_EDIT                   ].itemPrinted, "EDIT"                                                                       )) {printf("item %" FMT16S " is not MNU_M_EDIT                   \n", MNU_M_EDIT                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_GET                    ].itemPrinted, "GETM"                                                                       )) {printf("item %" FMT16S " is not ITM_M_GET                    \n", ITM_M_GET                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_GOTO                   ].itemPrinted, "GOTO"                                                                       )) {printf("item %" FMT16S " is not ITM_M_GOTO                   \n", ITM_M_GOTO                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_GROW                   ].itemPrinted, "GROW"                                                                       )) {printf("item %" FMT16S " is not ITM_M_GROW                   \n", ITM_M_GROW                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_INSR                   ].itemPrinted, "INSR"                                                                       )) {printf("item %" FMT16S " is not ITM_M_INSR                   \n", ITM_M_INSR                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_LU                     ].itemPrinted, "M.LU"                                                                       )) {printf("item %" FMT16S " is not ITM_M_LU                     \n", ITM_M_LU                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_NEW                    ].itemPrinted, "NEW"                                                                        )) {printf("item %" FMT16S " is not ITM_M_NEW                    \n", ITM_M_NEW                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_OLD                    ].itemPrinted, "OLD"                                                                        )) {printf("item %" FMT16S " is not ITM_M_OLD                    \n", ITM_M_OLD                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_PUT                    ].itemPrinted, "PUTM"                                                                       )) {printf("item %" FMT16S " is not ITM_M_PUT                    \n", ITM_M_PUT                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_R                      ].itemPrinted, "R" STD_LEFT_RIGHT_ARROWS "R"                                                )) {printf("item %" FMT16S " is not ITM_M_R                      \n", ITM_M_R                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_SIMQ                   ].itemPrinted, "M.SIMQ"                                                                     )) {printf("item %" FMT16S " is not ITM_M_SIMQ                   \n", ITM_M_SIMQ                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_SQR                    ].itemPrinted, "M.SQR?"                                                                     )) {printf("item %" FMT16S " is not ITM_M_SQR                    \n", ITM_M_SQR                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_WRAP                   ].itemPrinted, "WRAP"                                                                       )) {printf("item %" FMT16S " is not ITM_M_WRAP                   \n", ITM_M_WRAP                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_CONVM                    ].itemPrinted, "m:"                                                                         )) {printf("item %" FMT16S " is not MNU_CONVM                    \n", MNU_CONVM                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_MtoAU                    ].itemPrinted, "m" STD_RIGHT_ARROW "au"                                                     )) {printf("item %" FMT16S " is not ITM_MtoAU                    \n", ITM_MtoAU                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_MtoFT                    ].itemPrinted, "m" STD_RIGHT_ARROW "ft."                                                    )) {printf("item %" FMT16S " is not ITM_MtoFT                    \n", ITM_MtoFT                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_MtoFTUS                  ].itemPrinted, "m " STD_RIGHT_ARROW                                                         )) {printf("item %" FMT16S " is not ITM_MtoFTUS                  \n", ITM_MtoFTUS                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_MtoINCH                  ].itemPrinted, "m" STD_RIGHT_ARROW "in."                                                    )) {printf("item %" FMT16S " is not ITM_MtoINCH                  \n", ITM_MtoINCH                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_MtoLY                    ].itemPrinted, "m" STD_RIGHT_ARROW "ly"                                                     )) {printf("item %" FMT16S " is not ITM_MtoLY                    \n", ITM_MtoLY                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_MtoMI                    ].itemPrinted, "m" STD_RIGHT_ARROW "mi."                                                    )) {printf("item %" FMT16S " is not ITM_MtoMI                    \n", ITM_MtoMI                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_MtoNMI                   ].itemPrinted, "m" STD_RIGHT_ARROW "nmi."                                                   )) {printf("item %" FMT16S " is not ITM_MtoNMI                   \n", ITM_MtoNMI                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_MtoPC                    ].itemPrinted, "m" STD_RIGHT_ARROW "pc"                                                     )) {printf("item %" FMT16S " is not ITM_MtoPC                    \n", ITM_MtoPC                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_MtoPT                    ].itemPrinted, "m" STD_RIGHT_ARROW "pt."                                                    )) {printf("item %" FMT16S " is not ITM_MtoPT                    \n", ITM_MtoPT                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_MtoYD                    ].itemPrinted, "m" STD_RIGHT_ARROW "yd."                                                    )) {printf("item %" FMT16S " is not ITM_MtoYD                    \n", ITM_MtoYD                    ); exit(0);}
    if(strcmp(indexOfItems[CST_35                       ].itemPrinted, "m" STD_SUB_SUN                                                              )) {printf("item %" FMT16S " is not CST_35                       \n", CST_35                       ); exit(0);}
    if(strcmp(indexOfItems[CST_36                       ].itemPrinted, "m" STD_SUB_EARTH                                                            )) {printf("item %" FMT16S " is not CST_36                       \n", CST_36                       ); exit(0);}
    if(strcmp(indexOfItems[CST_37                       ].itemPrinted, "N" STD_SUB_A                                                                )) {printf("item %" FMT16S " is not CST_37                       \n", CST_37                       ); exit(0);}
    if(strcmp(indexOfItems[CST_38                       ].itemPrinted, "NaN"                                                                        )) {printf("item %" FMT16S " is not CST_38                       \n", CST_38                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_NAND                     ].itemPrinted, "NAND"                                                                       )) {printf("item %" FMT16S " is not ITM_NAND                     \n", ITM_NAND                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_NAN                      ].itemPrinted, "NaN?"                                                                       )) {printf("item %" FMT16S " is not ITM_NAN                      \n", ITM_NAN                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_NBIN                     ].itemPrinted, "NBin"                                                                       )) {printf("item %" FMT16S " is not ITM_NBIN                     \n", ITM_NBIN                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_NBINP                    ].itemPrinted, "NBin" STD_SUB_p                                                             )) {printf("item %" FMT16S " is not ITM_NBINP                    \n", ITM_NBINP                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_NBINU                    ].itemPrinted, "NBin" STD_SUB_u                                                             )) {printf("item %" FMT16S " is not ITM_NBINU                    \n", ITM_NBINU                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_NBINM1                   ].itemPrinted, "NBin" STD_SUP_MINUS_1                                                       )) {printf("item %" FMT16S " is not ITM_NBINM1                   \n", ITM_NBINM1                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_NBIN                     ].itemPrinted, "NBin:"                                                                      )) {printf("item %" FMT16S " is not MNU_NBIN                     \n", MNU_NBIN                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_NEIGHB                   ].itemPrinted, "NEIGHB"                                                                     )) {printf("item %" FMT16S " is not ITM_NEIGHB                   \n", ITM_NEIGHB                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_NEXTP                    ].itemPrinted, "NEXTP"                                                                      )) {printf("item %" FMT16S " is not ITM_NEXTP                    \n", ITM_NEXTP                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_NMItoM                   ].itemPrinted, "nmi." STD_RIGHT_ARROW "m"                                                   )) {printf("item %" FMT16S " is not ITM_NMItoM                   \n", ITM_NMItoM                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_NOP                      ].itemPrinted, "NOP"                                                                        )) {printf("item %" FMT16S " is not ITM_NOP                      \n", ITM_NOP                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_NOR                      ].itemPrinted, "NOR"                                                                        )) {printf("item %" FMT16S " is not ITM_NOR                      \n", ITM_NOR                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_NORML                    ].itemPrinted, "Norml"                                                                      )) {printf("item %" FMT16S " is not ITM_NORML                    \n", ITM_NORML                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_NORMLP                   ].itemPrinted, "Norml" STD_SUB_p                                                            )) {printf("item %" FMT16S " is not ITM_NORMLP                   \n", ITM_NORMLP                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_NORMLU                   ].itemPrinted, "Norml" STD_SUB_u                                                            )) {printf("item %" FMT16S " is not ITM_NORMLU                   \n", ITM_NORMLU                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_NORMLM1                  ].itemPrinted, "Norml" STD_SUP_MINUS_1                                                      )) {printf("item %" FMT16S " is not ITM_NORMLM1                  \n", ITM_NORMLM1                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_NORML                    ].itemPrinted, "Norml:"                                                                     )) {printf("item %" FMT16S " is not MNU_NORML                    \n", MNU_NORML                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_NOT                      ].itemPrinted, "NOT"                                                                        )) {printf("item %" FMT16S " is not ITM_NOT                      \n", ITM_NOT                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_NPER                     ].itemPrinted, "n" STD_SUB_P STD_SUB_E STD_SUB_R                                            )) {printf("item %" FMT16S " is not ITM_NPER                     \n", ITM_NPER                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_NSIGMA                   ].itemPrinted, "n"                                                                          )) {printf("item %" FMT16S " is not ITM_NSIGMA                   \n", ITM_NSIGMA                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_NtoLBF                   ].itemPrinted, "N" STD_RIGHT_ARROW "lbf"                                                    )) {printf("item %" FMT16S " is not ITM_NtoLBF                   \n", ITM_NtoLBF                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_ODD                      ].itemPrinted, "ODD?"                                                                       )) {printf("item %" FMT16S " is not ITM_ODD                      \n", ITM_ODD                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_OFF                      ].itemPrinted, "OFF"                                                                        )) {printf("item %" FMT16S " is not ITM_OFF                      \n", ITM_OFF                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_OR                       ].itemPrinted, "OR"                                                                         )) {printf("item %" FMT16S " is not ITM_OR                       \n", ITM_OR                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_ORTHOF                   ].itemPrinted, "OrthoF"                                                                     )) {printf("item %" FMT16S " is not ITM_ORTHOF                   \n", ITM_ORTHOF                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_ORTHOG                   ].itemPrinted, "Orthog"                                                                     )) {printf("item %" FMT16S " is not MNU_ORTHOG                   \n", MNU_ORTHOG                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_OZtoKG                   ].itemPrinted, "oz" STD_RIGHT_ARROW "kg"                                                    )) {printf("item %" FMT16S " is not ITM_OZtoKG                   \n", ITM_OZtoKG                   ); exit(0);}
    if(strcmp(indexOfItems[CST_39                       ].itemPrinted, "P" STD_SUB_0                                                                )) {printf("item %" FMT16S " is not CST_39                       \n", CST_39                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_PAUSE                    ].itemPrinted, "PAUSE"                                                                      )) {printf("item %" FMT16S " is not ITM_PAUSE                    \n", ITM_PAUSE                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_PAtoATM                  ].itemPrinted, "Pa " STD_RIGHT_ARROW                                                        )) {printf("item %" FMT16S " is not ITM_PAtoATM                  \n", ITM_PAtoATM                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_PAtoBAR                  ].itemPrinted, "Pa" STD_RIGHT_ARROW "bar"                                                   )) {printf("item %" FMT16S " is not ITM_PAtoBAR                  \n", ITM_PAtoBAR                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_PAtoIHG                  ].itemPrinted, "Pa" STD_RIGHT_ARROW                                                         )) {printf("item %" FMT16S " is not ITM_PAtoIHG                  \n", ITM_PAtoIHG                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_PAtoPSI                  ].itemPrinted, "Pa" STD_RIGHT_ARROW "psi"                                                   )) {printf("item %" FMT16S " is not ITM_PAtoPSI                  \n", ITM_PAtoPSI                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_PAtoTOR                  ].itemPrinted, "Pa " STD_RIGHT_ARROW                                                        )) {printf("item %" FMT16S " is not ITM_PAtoTOR                  \n", ITM_PAtoTOR                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_PARTS                    ].itemPrinted, "PARTS"                                                                      )) {printf("item %" FMT16S " is not MNU_PARTS                    \n", MNU_PARTS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_PCtoM                    ].itemPrinted, "pc" STD_RIGHT_ARROW "m"                                                     )) {printf("item %" FMT16S " is not ITM_PCtoM                    \n", ITM_PCtoM                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_PERM                     ].itemPrinted, "P" STD_SUB_y STD_SUB_x                                                      )) {printf("item %" FMT16S " is not ITM_PERM                     \n", ITM_PERM                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_PERonA                   ].itemPrinted, "per/a"                                                                      )) {printf("item %" FMT16S " is not ITM_PERonA                   \n", ITM_PERonA                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_PGMINT                   ].itemPrinted, "PGMINT"                                                                     )) {printf("item %" FMT16S " is not ITM_PGMINT                   \n", ITM_PGMINT                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_PGMSLV                   ].itemPrinted, "PGMSLV"                                                                     )) {printf("item %" FMT16S " is not ITM_PGMSLV                   \n", ITM_PGMSLV                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_PIXEL                    ].itemPrinted, "PIXEL"                                                                      )) {printf("item %" FMT16S " is not ITM_PIXEL                    \n", ITM_PIXEL                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_PLOT                     ].itemPrinted, "PLOT"                                                                       )) {printf("item %" FMT16S " is not ITM_PLOT                     \n", ITM_PLOT                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_PMT                      ].itemPrinted, "PMT"                                                                        )) {printf("item %" FMT16S " is not ITM_PMT                      \n", ITM_PMT                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_PN                       ].itemPrinted, "P" STD_SUB_n                                                                )) {printf("item %" FMT16S " is not ITM_PN                       \n", ITM_PN                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_POINT                    ].itemPrinted, "POINT"                                                                      )) {printf("item %" FMT16S " is not ITM_POINT                    \n", ITM_POINT                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_POISS                    ].itemPrinted, "Poiss"                                                                      )) {printf("item %" FMT16S " is not ITM_POISS                    \n", ITM_POISS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_POISSP                   ].itemPrinted, "Poiss" STD_SUB_p                                                            )) {printf("item %" FMT16S " is not ITM_POISSP                   \n", ITM_POISSP                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_POISSU                   ].itemPrinted, "Poiss" STD_SUB_u                                                            )) {printf("item %" FMT16S " is not ITM_POISSU                   \n", ITM_POISSU                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_POISSM1                  ].itemPrinted, "Poiss" STD_SUP_MINUS_1                                                      )) {printf("item %" FMT16S " is not ITM_POISSM1                  \n", ITM_POISSM1                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_POISS                    ].itemPrinted, "Poiss:"                                                                     )) {printf("item %" FMT16S " is not MNU_POISS                    \n", MNU_POISS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_POLAR                    ].itemPrinted, "POLAR"                                                                      )) {printf("item %" FMT16S " is not ITM_POLAR                    \n", ITM_POLAR                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_POPLR                    ].itemPrinted, "PopLR"                                                                      )) {printf("item %" FMT16S " is not ITM_POPLR                    \n", ITM_POPLR                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_POWERF                   ].itemPrinted, "PowerF"                                                                     )) {printf("item %" FMT16S " is not ITM_POWERF                   \n", ITM_POWERF                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRtoDB                   ].itemPrinted, "power"                                                                      )) {printf("item %" FMT16S " is not ITM_PRtoDB                   \n", ITM_PRtoDB                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRCL                     ].itemPrinted, "PRCL"                                                                       )) {printf("item %" FMT16S " is not ITM_PRCL                     \n", ITM_PRCL                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRIME                    ].itemPrinted, "PRIME?"                                                                     )) {printf("item %" FMT16S " is not ITM_PRIME                    \n", ITM_PRIME                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_PROB                     ].itemPrinted, "PROB"                                                                       )) {printf("item %" FMT16S " is not MNU_PROB                     \n", MNU_PROB                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_PROFRC                   ].itemPrinted, "a" STD_SPACE_3_PER_EM "b/c"                                                 )) {printf("item %" FMT16S " is not ITM_PROFRC                   \n", ITM_PROFRC                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_PROGS                    ].itemPrinted, "PROGS"                                                                      )) {printf("item %" FMT16S " is not MNU_PROGS                    \n", MNU_PROGS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_PSItoPA                  ].itemPrinted, "psi" STD_RIGHT_ARROW "Pa"                                                   )) {printf("item %" FMT16S " is not ITM_PSItoPA                  \n", ITM_PSItoPA                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_PSTO                     ].itemPrinted, "PSTO"                                                                       )) {printf("item %" FMT16S " is not ITM_PSTO                     \n", ITM_PSTO                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_PTtoM                    ].itemPrinted, "pt." STD_RIGHT_ARROW "m"                                                    )) {printf("item %" FMT16S " is not ITM_PTtoM                    \n", ITM_PTtoM                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_PUTK                     ].itemPrinted, "PUTK"                                                                       )) {printf("item %" FMT16S " is not ITM_PUTK                     \n", ITM_PUTK                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_PV                       ].itemPrinted, "PV"                                                                         )) {printf("item %" FMT16S " is not ITM_PV                       \n", ITM_PV                       ); exit(0);}
    if(strcmp(indexOfItems[MNU_PFN                      ].itemPrinted, "P.FN"                                                                       )) {printf("item %" FMT16S " is not MNU_PFN                      \n", MNU_PFN                      ); exit(0);}
    if(strcmp(indexOfItems[MNU_PFN2                     ].itemPrinted, "P.FN2"                                                                      )) {printf("item %" FMT16S " is not MNU_PFN2                     \n", MNU_PFN2                     ); exit(0);}
    if(strcmp(indexOfItems[MNU_CONVP                    ].itemPrinted, "P:"                                                                         )) {printf("item %" FMT16S " is not MNU_CONVP                    \n", MNU_CONVP                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_QUIET                    ].itemPrinted, "QUIET"                                                                      )) {printf("item %" FMT16S " is not ITM_QUIET                    \n", ITM_QUIET                    ); exit(0);}
    if(strcmp(indexOfItems[CST_40                       ].itemPrinted, "R"                                                                          )) {printf("item %" FMT16S " is not CST_40                       \n", CST_40                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_RAD                      ].itemPrinted, "RAD"                                                                        )) {printf("item %" FMT16S " is not ITM_RAD                      \n", ITM_RAD                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_RADto                    ].itemPrinted, STD_MEASURED_ANGLE STD_SUP_r STD_RIGHT_ARROW STD_MEASURED_ANGLE              )) {printf("item %" FMT16S " is not ITM_RADto                    \n", ITM_RADto                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_RAM                      ].itemPrinted, "RAM"                                                                        )) {printf("item %" FMT16S " is not MNU_RAM                      \n", MNU_RAM                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_RAN                      ].itemPrinted, "RAN#"                                                                       )) {printf("item %" FMT16S " is not ITM_RAN                      \n", ITM_RAN                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_RBR                      ].itemPrinted, "RBR"                                                                        )) {printf("item %" FMT16S " is not ITM_RBR                      \n", ITM_RBR                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_RCL                      ].itemPrinted, "RCL"                                                                        )) {printf("item %" FMT16S " is not ITM_RCL                      \n", ITM_RCL                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_RCLCFG                   ].itemPrinted, "Config"                                                                     )) {printf("item %" FMT16S " is not ITM_RCLCFG                   \n", ITM_RCLCFG                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_RCLEL                    ].itemPrinted, "RCLEL"                                                                      )) {printf("item %" FMT16S " is not ITM_RCLEL                    \n", ITM_RCLEL                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_RCLIJ                    ].itemPrinted, "RCLIJ"                                                                      )) {printf("item %" FMT16S " is not ITM_RCLIJ                    \n", ITM_RCLIJ                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_RCLS                     ].itemPrinted, "Stack"                                                                      )) {printf("item %" FMT16S " is not ITM_RCLS                     \n", ITM_RCLS                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_RCLPLUS                  ].itemPrinted, "RCL+"                                                                       )) {printf("item %" FMT16S " is not ITM_RCLPLUS                  \n", ITM_RCLPLUS                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_RCLMINUS                 ].itemPrinted, "RCL-"                                                                       )) {printf("item %" FMT16S " is not ITM_RCLMINUS                 \n", ITM_RCLMINUS                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_RCLMULT                  ].itemPrinted, "RCL" STD_CROSS                                                              )) {printf("item %" FMT16S " is not ITM_RCLMULT                  \n", ITM_RCLMULT                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_RCLDIV                   ].itemPrinted, "RCL/"                                                                       )) {printf("item %" FMT16S " is not ITM_RCLDIV                   \n", ITM_RCLDIV                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_RCLMAX                   ].itemPrinted, "Max"                                                                        )) {printf("item %" FMT16S " is not ITM_RCLMAX                   \n", ITM_RCLMAX                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_RCLMIN                   ].itemPrinted, "Min"                                                                        )) {printf("item %" FMT16S " is not ITM_RCLMIN                   \n", ITM_RCLMIN                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_RDP                      ].itemPrinted, "RDP"                                                                        )) {printf("item %" FMT16S " is not ITM_RDP                      \n", ITM_RDP                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_RDXCOM                   ].itemPrinted, "RDX,"                                                                       )) {printf("item %" FMT16S " is not ITM_RDXCOM                   \n", ITM_RDXCOM                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_RDXPER                   ].itemPrinted, "RDX."                                                                       )) {printf("item %" FMT16S " is not ITM_RDXPER                   \n", ITM_RDXPER                   ); exit(0);}
    if(strcmp(indexOfItems[CST_41                       ].itemPrinted, "r" STD_SUB_e                                                                )) {printf("item %" FMT16S " is not CST_41                       \n", CST_41                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_RE                       ].itemPrinted, "Re"                                                                         )) {printf("item %" FMT16S " is not ITM_RE                       \n", ITM_RE                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_REALRES                  ].itemPrinted, "REALRE"                                                                     )) {printf("item %" FMT16S " is not ITM_REALRES                  \n", ITM_REALRES                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_REALS                    ].itemPrinted, "REALS"                                                                      )) {printf("item %" FMT16S " is not MNU_REALS                    \n", MNU_REALS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_REAL                     ].itemPrinted, "REAL?"                                                                      )) {printf("item %" FMT16S " is not ITM_REAL                     \n", ITM_REAL                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_RECT                     ].itemPrinted, "RECT"                                                                       )) {printf("item %" FMT16S " is not ITM_RECT                     \n", ITM_RECT                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_REGS                     ].itemPrinted, "REGS"                                                                       )) {printf("item %" FMT16S " is not ITM_REGS                     \n", ITM_REGS                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_RECV                     ].itemPrinted, "RECV"                                                                       )) {printf("item %" FMT16S " is not ITM_RECV                     \n", ITM_RECV                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_RESET                    ].itemPrinted, "RESET"                                                                      )) {printf("item %" FMT16S " is not ITM_RESET                    \n", ITM_RESET                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_REtoCX                   ].itemPrinted, "CC"                                                                         )) {printf("item %" FMT16S " is not ITM_REtoCX                   \n", ITM_REtoCX                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_REexIM                   ].itemPrinted, "Re" STD_LEFT_RIGHT_ARROWS "Im"                                              )) {printf("item %" FMT16S " is not ITM_REexIM                   \n", ITM_REexIM                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_RJ                       ].itemPrinted, "RJ"                                                                         )) {printf("item %" FMT16S " is not ITM_RJ                       \n", ITM_RJ                       ); exit(0);}
    if(strcmp(indexOfItems[CST_42                       ].itemPrinted, "R" STD_SUB_K                                                                )) {printf("item %" FMT16S " is not CST_42                       \n", CST_42                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_RL                       ].itemPrinted, "RL"                                                                         )) {printf("item %" FMT16S " is not ITM_RL                       \n", ITM_RL                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_RLC                      ].itemPrinted, "RLC"                                                                        )) {printf("item %" FMT16S " is not ITM_RLC                      \n", ITM_RLC                      ); exit(0);}
    if(strcmp(indexOfItems[CST_43                       ].itemPrinted, "R" STD_SUB_M                                                                )) {printf("item %" FMT16S " is not CST_43                       \n", CST_43                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_RM                       ].itemPrinted, "RM"                                                                         )) {printf("item %" FMT16S " is not ITM_RM                       \n", ITM_RM                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_RMQ                      ].itemPrinted, "RM?"                                                                        )) {printf("item %" FMT16S " is not ITM_RMQ                      \n", ITM_RMQ                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_RMDR                     ].itemPrinted, "RMDR"                                                                       )) {printf("item %" FMT16S " is not ITM_RMDR                     \n", ITM_RMDR                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_RNORM                    ].itemPrinted, "RNORM"                                                                      )) {printf("item %" FMT16S " is not ITM_RNORM                    \n", ITM_RNORM                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_ROUND                    ].itemPrinted, "ROUND"                                                                      )) {printf("item %" FMT16S " is not ITM_ROUND                    \n", ITM_ROUND                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_ROUNDI                   ].itemPrinted, "ROUNDI"                                                                     )) {printf("item %" FMT16S " is not ITM_ROUNDI                   \n", ITM_ROUNDI                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_RR                       ].itemPrinted, "RR"                                                                         )) {printf("item %" FMT16S " is not ITM_RR                       \n", ITM_RR                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_RRC                      ].itemPrinted, "RRC"                                                                        )) {printf("item %" FMT16S " is not ITM_RRC                      \n", ITM_RRC                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_RSD                      ].itemPrinted, "RSD"                                                                        )) {printf("item %" FMT16S " is not ITM_RSD                      \n", ITM_RSD                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_RSUM                     ].itemPrinted, "RSUM"                                                                       )) {printf("item %" FMT16S " is not ITM_RSUM                     \n", ITM_RSUM                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_RTN                      ].itemPrinted, "RTN"                                                                        )) {printf("item %" FMT16S " is not ITM_RTN                      \n", ITM_RTN                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_RTNP1                    ].itemPrinted, "RTN+1"                                                                      )) {printf("item %" FMT16S " is not ITM_RTNP1                    \n", ITM_RTNP1                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_R_CLR                    ].itemPrinted, "R-CLR"                                                                      )) {printf("item %" FMT16S " is not ITM_R_CLR                    \n", ITM_R_CLR                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_R_COPY                   ].itemPrinted, "R-COPY"                                                                     )) {printf("item %" FMT16S " is not ITM_R_COPY                   \n", ITM_R_COPY                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_R_SORT                   ].itemPrinted, "R-SORT"                                                                     )) {printf("item %" FMT16S " is not ITM_R_SORT                   \n", ITM_R_SORT                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_R_SWAP                   ].itemPrinted, "R-SWAP"                                                                     )) {printf("item %" FMT16S " is not ITM_R_SWAP                   \n", ITM_R_SWAP                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_RtoD                     ].itemPrinted, STD_MEASURED_ANGLE STD_SUP_r STD_RIGHT_ARROW STD_MEASURED_ANGLE STD_DEGREE   )) {printf("item %" FMT16S " is not ITM_RtoD                     \n", ITM_RtoD                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_Rup                      ].itemPrinted, "R" STD_UP_ARROW                                                             )) {printf("item %" FMT16S " is not ITM_Rup                      \n", ITM_Rup                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_Rdown                    ].itemPrinted, "R" STD_DOWN_ARROW                                                           )) {printf("item %" FMT16S " is not ITM_Rdown                    \n", ITM_Rdown                    ); exit(0);}
    if(strcmp(indexOfItems[CST_44                       ].itemPrinted, "R" STD_SUB_INFINITY                                                         )) {printf("item %" FMT16S " is not CST_44                       \n", CST_44                       ); exit(0);}
    if(strcmp(indexOfItems[CST_45                       ].itemPrinted, "R" STD_SUB_SUN                                                              )) {printf("item %" FMT16S " is not CST_45                       \n", CST_45                       ); exit(0);}
    if(strcmp(indexOfItems[CST_46                       ].itemPrinted, "R" STD_SUB_EARTH                                                            )) {printf("item %" FMT16S " is not CST_46                       \n", CST_46                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_S                        ].itemPrinted, "s"                                                                          )) {printf("item %" FMT16S " is not ITM_S                        \n", ITM_S                        ); exit(0);}
    if(strcmp(indexOfItems[CST_47                       ].itemPrinted, "Sa"                                                                         )) {printf("item %" FMT16S " is not CST_47                       \n", CST_47                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_SAVE                     ].itemPrinted, "SAVE"                                                                       )) {printf("item %" FMT16S " is not ITM_SAVE                     \n", ITM_SAVE                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_SB                       ].itemPrinted, "SB"                                                                         )) {printf("item %" FMT16S " is not ITM_SB                       \n", ITM_SB                       ); exit(0);}
    if(strcmp(indexOfItems[CST_48                       ].itemPrinted, "Sb"                                                                         )) {printf("item %" FMT16S " is not CST_48                       \n", CST_48                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_SCI                      ].itemPrinted, "SCI"                                                                        )) {printf("item %" FMT16S " is not ITM_SCI                      \n", ITM_SCI                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_SCWtoKG                  ].itemPrinted, "short"                                                                      )) {printf("item %" FMT16S " is not ITM_SCWtoKG                  \n", ITM_SCWtoKG                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_SCIOVR                   ].itemPrinted, "SCIOVR"                                                                     )) {printf("item %" FMT16S " is not ITM_SCIOVR                   \n", ITM_SCIOVR                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_SDIGS                    ].itemPrinted, "SDIGS?"                                                                     )) {printf("item %" FMT16S " is not ITM_SDIGS                    \n", ITM_SDIGS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_SDL                      ].itemPrinted, "SDL"                                                                        )) {printf("item %" FMT16S " is not ITM_SDL                      \n", ITM_SDL                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_SDR                      ].itemPrinted, "SDR"                                                                        )) {printf("item %" FMT16S " is not ITM_SDR                      \n", ITM_SDR                      ); exit(0);}
    if(strcmp(indexOfItems[CST_49                       ].itemPrinted, "Se" STD_SUP_2                                                               )) {printf("item %" FMT16S " is not CST_49                       \n", CST_49                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_SEED                     ].itemPrinted, "SEED"                                                                       )) {printf("item %" FMT16S " is not ITM_SEED                     \n", ITM_SEED                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_SEND                     ].itemPrinted, "SEND"                                                                       )) {printf("item %" FMT16S " is not ITM_SEND                     \n", ITM_SEND                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_SETCHN                   ].itemPrinted, "CHINA"                                                                      )) {printf("item %" FMT16S " is not ITM_SETCHN                   \n", ITM_SETCHN                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_SETDAT                   ].itemPrinted, "SETDAT"                                                                     )) {printf("item %" FMT16S " is not ITM_SETDAT                   \n", ITM_SETDAT                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_SETEUR                   ].itemPrinted, "EUROPE"                                                                     )) {printf("item %" FMT16S " is not ITM_SETEUR                   \n", ITM_SETEUR                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_SETIND                   ].itemPrinted, "INDIA"                                                                      )) {printf("item %" FMT16S " is not ITM_SETIND                   \n", ITM_SETIND                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_SETJPN                   ].itemPrinted, "JAPAN"                                                                      )) {printf("item %" FMT16S " is not ITM_SETJPN                   \n", ITM_SETJPN                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_SETSIG                   ].itemPrinted, "SETSIG"                                                                     )) {printf("item %" FMT16S " is not ITM_SETSIG                   \n", ITM_SETSIG                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_SETTIM                   ].itemPrinted, "SETTIM"                                                                     )) {printf("item %" FMT16S " is not ITM_SETTIM                   \n", ITM_SETTIM                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_SETUK                    ].itemPrinted, "UK"                                                                         )) {printf("item %" FMT16S " is not ITM_SETUK                    \n", ITM_SETUK                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_SETUSA                   ].itemPrinted, "USA"                                                                        )) {printf("item %" FMT16S " is not ITM_SETUSA                   \n", ITM_SETUSA                   ); exit(0);}
    if(strcmp(indexOfItems[CST_50                       ].itemPrinted, "Se'" STD_SUP_2                                                              )) {printf("item %" FMT16S " is not CST_50                       \n", CST_50                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_SF                       ].itemPrinted, "SF"                                                                         )) {printf("item %" FMT16S " is not ITM_SF                       \n", ITM_SF                       ); exit(0);}
    if(strcmp(indexOfItems[CST_51                       ].itemPrinted, "Sf" STD_SUP_MINUS_1                                                         )) {printf("item %" FMT16S " is not CST_51                       \n", CST_51                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGN                     ].itemPrinted, "sign"                                                                       )) {printf("item %" FMT16S " is not ITM_SIGN                     \n", ITM_SIGN                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGNMT                   ].itemPrinted, "SIGNMT"                                                                     )) {printf("item %" FMT16S " is not ITM_SIGNMT                   \n", ITM_SIGNMT                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_SIM_EQ                   ].itemPrinted, "SIM EQ"                                                                     )) {printf("item %" FMT16S " is not MNU_SIM_EQ                   \n", MNU_SIM_EQ                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_sin                      ].itemPrinted, "sin"                                                                        )) {printf("item %" FMT16S " is not ITM_sin                      \n", ITM_sin                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_sinc                     ].itemPrinted, "sinc"                                                                       )) {printf("item %" FMT16S " is not ITM_sinc                     \n", ITM_sinc                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_sinh                     ].itemPrinted, "sinh"                                                                       )) {printf("item %" FMT16S " is not ITM_sinh                     \n", ITM_sinh                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_SKIP                     ].itemPrinted, "SKIP"                                                                       )) {printf("item %" FMT16S " is not ITM_SKIP                     \n", ITM_SKIP                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_SL                       ].itemPrinted, "SL"                                                                         )) {printf("item %" FMT16S " is not ITM_SL                       \n", ITM_SL                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_SLOW                     ].itemPrinted, "SLOW"                                                                       )) {printf("item %" FMT16S " is not ITM_SLOW                     \n", ITM_SLOW                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_SLVQ                     ].itemPrinted, "SLVQ"                                                                       )) {printf("item %" FMT16S " is not ITM_SLVQ                     \n", ITM_SLVQ                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_SM                       ].itemPrinted, "s" STD_SUB_m                                                                )) {printf("item %" FMT16S " is not ITM_SM                       \n", ITM_SM                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_SMODE                    ].itemPrinted, "SMODE?"                                                                     )) {printf("item %" FMT16S " is not ITM_SMODE                    \n", ITM_SMODE                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_SMW                      ].itemPrinted, "s" STD_SUB_m STD_SUB_w                                                      )) {printf("item %" FMT16S " is not ITM_SMW                      \n", ITM_SMW                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_SOLVE                    ].itemPrinted, "SOLVE"                                                                      )) {printf("item %" FMT16S " is not ITM_SOLVE                    \n", ITM_SOLVE                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_Solver                   ].itemPrinted, "Solver"                                                                     )) {printf("item %" FMT16S " is not MNU_Solver                   \n", MNU_Solver                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_SPEC                     ].itemPrinted, "SPEC?"                                                                      )) {printf("item %" FMT16S " is not ITM_SPEC                     \n", ITM_SPEC                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_SR                       ].itemPrinted, "SR"                                                                         )) {printf("item %" FMT16S " is not ITM_SR                       \n", ITM_SR                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_SSIZE4                   ].itemPrinted, "SSIZE4"                                                                     )) {printf("item %" FMT16S " is not ITM_SSIZE4                   \n", ITM_SSIZE4                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_SSIZE8                   ].itemPrinted, "SSIZE8"                                                                     )) {printf("item %" FMT16S " is not ITM_SSIZE8                   \n", ITM_SSIZE8                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_SSIZE                    ].itemPrinted, "SSIZE?"                                                                     )) {printf("item %" FMT16S " is not ITM_SSIZE                    \n", ITM_SSIZE                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_STAT                     ].itemPrinted, "STAT"                                                                       )) {printf("item %" FMT16S " is not MNU_STAT                     \n", MNU_STAT                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_STATUS                   ].itemPrinted, "STATUS"                                                                     )) {printf("item %" FMT16S " is not ITM_STATUS                   \n", ITM_STATUS                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_STK                      ].itemPrinted, "STK"                                                                        )) {printf("item %" FMT16S " is not MNU_STK                      \n", MNU_STK                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_STO                      ].itemPrinted, "STO"                                                                        )) {printf("item %" FMT16S " is not ITM_STO                      \n", ITM_STO                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_STOCFG                   ].itemPrinted, "Config"                                                                     )) {printf("item %" FMT16S " is not ITM_STOCFG                   \n", ITM_STOCFG                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_STOEL                    ].itemPrinted, "STOEL"                                                                      )) {printf("item %" FMT16S " is not ITM_STOEL                    \n", ITM_STOEL                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_STOIJ                    ].itemPrinted, "STOIJ"                                                                      )) {printf("item %" FMT16S " is not ITM_STOIJ                    \n", ITM_STOIJ                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_STOP                     ].itemPrinted, "R/S"                                                                        )) {printf("item %" FMT16S " is not ITM_STOP                     \n", ITM_STOP                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_STOS                     ].itemPrinted, "Stack"                                                                      )) {printf("item %" FMT16S " is not ITM_STOS                     \n", ITM_STOS                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_STOPLUS                  ].itemPrinted, "STO+"                                                                       )) {printf("item %" FMT16S " is not ITM_STOPLUS                  \n", ITM_STOPLUS                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_STOMINUS                 ].itemPrinted, "STO-"                                                                       )) {printf("item %" FMT16S " is not ITM_STOMINUS                 \n", ITM_STOMINUS                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_STOMULT                  ].itemPrinted, "STO" STD_CROSS                                                              )) {printf("item %" FMT16S " is not ITM_STOMULT                  \n", ITM_STOMULT                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_STODIV                   ].itemPrinted, "STO/"                                                                       )) {printf("item %" FMT16S " is not ITM_STODIV                   \n", ITM_STODIV                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_STOMAX                   ].itemPrinted, "Max"                                                                        )) {printf("item %" FMT16S " is not ITM_STOMAx                   \n", ITM_STOMAX                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_STOMIN                   ].itemPrinted, "Min"                                                                        )) {printf("item %" FMT16S " is not ITM_STOMIN                   \n", ITM_STOMIN                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_STOtoKG                  ].itemPrinted, "stones"                                                                     )) {printf("item %" FMT16S " is not ITM_STOtoKG                  \n", ITM_STOtoKG                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_STRI                     ].itemPrinted, "STRI?"                                                                      )) {printf("item %" FMT16S " is not ITM_STRI                     \n", ITM_STRI                     ); exit(0);}
    if(strcmp(indexOfItems[MNU_STRING                   ].itemPrinted, "STRING"                                                                     )) {printf("item %" FMT16S " is not MNU_STRING                   \n", MNU_STRING                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_ST_A                     ].itemPrinted, "ST.A"                                                                       )) {printf("item %" FMT16S " is not ITM_ST_A                     \n", ITM_ST_A                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_ST_B                     ].itemPrinted, "ST.B"                                                                       )) {printf("item %" FMT16S " is not ITM_ST_B                     \n", ITM_ST_B                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_ST_C                     ].itemPrinted, "ST.C"                                                                       )) {printf("item %" FMT16S " is not ITM_ST_C                     \n", ITM_ST_C                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_ST_D                     ].itemPrinted, "ST.D"                                                                       )) {printf("item %" FMT16S " is not ITM_ST_D                     \n", ITM_ST_D                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_ST_T                     ].itemPrinted, "ST.T"                                                                       )) {printf("item %" FMT16S " is not ITM_ST_T                     \n", ITM_ST_T                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_ST_X                     ].itemPrinted, "ST.X"                                                                       )) {printf("item %" FMT16S " is not ITM_ST_X                     \n", ITM_ST_X                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_ST_Y                     ].itemPrinted, "ST.Y"                                                                       )) {printf("item %" FMT16S " is not ITM_ST_Y                     \n", ITM_ST_Y                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_ST_Z                     ].itemPrinted, "ST.Z"                                                                       )) {printf("item %" FMT16S " is not ITM_ST_Z                     \n", ITM_ST_Z                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_SUM                      ].itemPrinted, "SUM"                                                                        )) {printf("item %" FMT16S " is not ITM_SUM                      \n", ITM_SUM                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_SW                       ].itemPrinted, "s" STD_SUB_w                                                                )) {printf("item %" FMT16S " is not ITM_SW                       \n", ITM_SW                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_SXY                      ].itemPrinted, "s" STD_SUB_x STD_SUB_y                                                      )) {printf("item %" FMT16S " is not ITM_SXY                      \n", ITM_SXY                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_STtoKG                   ].itemPrinted, "short"                                                                      )) {printf("item %" FMT16S " is not ITM_STtoKG                   \n", ITM_STtoKG                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_StoYEAR                  ].itemPrinted, "s" STD_RIGHT_ARROW "year"                                                   )) {printf("item %" FMT16S " is not ITM_StoYEAR                  \n", ITM_StoYEAR                  ); exit(0);}
    if(strcmp(indexOfItems[CST_52                       ].itemPrinted, "T" STD_SUB_0                                                                )) {printf("item %" FMT16S " is not CST_52                       \n", CST_52                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_tan                      ].itemPrinted, "tan"                                                                        )) {printf("item %" FMT16S " is not ITM_tan                      \n", ITM_tan                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_tanh                     ].itemPrinted, "tanh"                                                                       )) {printf("item %" FMT16S " is not ITM_tanh                     \n", ITM_tanh                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_TDISP                    ].itemPrinted, "TDISP"                                                                      )) {printf("item %" FMT16S " is not ITM_TDISP                    \n", ITM_TDISP                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_TEST                     ].itemPrinted, "TEST"                                                                       )) {printf("item %" FMT16S " is not MNU_TEST                     \n", MNU_TEST                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_TICKS                    ].itemPrinted, "TICKS"                                                                      )) {printf("item %" FMT16S " is not ITM_TICKS                    \n", ITM_TICKS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_TIME                     ].itemPrinted, "TIME"                                                                       )) {printf("item %" FMT16S " is not ITM_TIME                     \n", ITM_TIME                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_TIMER                    ].itemPrinted, "TIMER"                                                                      )) {printf("item %" FMT16S " is not ITM_TIMER                    \n", ITM_TIMER                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_TIMES                    ].itemPrinted, "TIMES"                                                                      )) {printf("item %" FMT16S " is not MNU_TIMES                    \n", MNU_TIMES                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_TN                       ].itemPrinted, "T" STD_SUB_n                                                                )) {printf("item %" FMT16S " is not ITM_TN                       \n", ITM_TN                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_TONE                     ].itemPrinted, "TONE"                                                                       )) {printf("item %" FMT16S " is not ITM_TONE                     \n", ITM_TONE                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_TONtoKG                  ].itemPrinted, "tons"                                                                       )) {printf("item %" FMT16S " is not ITM_TONtoKG                  \n", ITM_TONtoKG                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_TOP                      ].itemPrinted, "TOP?"                                                                       )) {printf("item %" FMT16S " is not ITM_TOP                      \n", ITM_TOP                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_TORtoPA                  ].itemPrinted, "torr"                                                                       )) {printf("item %" FMT16S " is not ITM_TORtoPA                  \n", ITM_TORtoPA                  ); exit(0);}
    if(strcmp(indexOfItems[CST_53                       ].itemPrinted, "T" STD_SUB_P                                                                )) {printf("item %" FMT16S " is not CST_53                       \n", CST_53                       ); exit(0);}
    if(strcmp(indexOfItems[CST_54                       ].itemPrinted, "t" STD_SUB_p                                                                )) {printf("item %" FMT16S " is not CST_54                       \n", CST_54                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_TPX                      ].itemPrinted, "t" STD_SUB_p "(x)"                                                          )) {printf("item %" FMT16S " is not ITM_TPX                      \n", ITM_TPX                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_TUX                      ].itemPrinted, "t" STD_SUB_u "(x)"                                                          )) {printf("item %" FMT16S " is not ITM_TUX                      \n", ITM_TUX                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_TX                       ].itemPrinted, "t(x)"                                                                       )) {printf("item %" FMT16S " is not ITM_TX                       \n", ITM_TX                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_TM1P                     ].itemPrinted, "t" STD_SUP_MINUS_1 "(p)"                                                    )) {printf("item %" FMT16S " is not ITM_TM1P                     \n", ITM_TM1P                     ); exit(0);}
    if(strcmp(indexOfItems[MNU_TRI                      ].itemPrinted, "TRI"                                                                        )) {printf("item %" FMT16S " is not MNU_TRI                      \n", MNU_TRI                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_TRZtoKG                  ].itemPrinted, "tr.oz"                                                                      )) {printf("item %" FMT16S " is not ITM_TRZtoKG                  \n", ITM_TRZtoKG                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_TVM                      ].itemPrinted, "TVM"                                                                        )) {printf("item %" FMT16S " is not MNU_TVM                      \n", MNU_TVM                      ); exit(0);}
    if(strcmp(indexOfItems[MNU_T                        ].itemPrinted, "t:"                                                                         )) {printf("item %" FMT16S " is not MNU_T                        \n", MNU_T                        ); exit(0);}
    if(strcmp(indexOfItems[ITM_Tex                      ].itemPrinted, "t" STD_LEFT_RIGHT_ARROWS                                                    )) {printf("item %" FMT16S " is not ITM_Tex                      \n", ITM_Tex                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_ULP                      ].itemPrinted, "ULP?"                                                                       )) {printf("item %" FMT16S " is not ITM_ULP                      \n", ITM_ULP                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_UN                       ].itemPrinted, "U" STD_SUB_n                                                                )) {printf("item %" FMT16S " is not ITM_UN                       \n", ITM_UN                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_UNITV                    ].itemPrinted, "UNITV"                                                                      )) {printf("item %" FMT16S " is not ITM_UNITV                    \n", ITM_UNITV                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_UNSIGN                   ].itemPrinted, "UNSIGN"                                                                     )) {printf("item %" FMT16S " is not ITM_UNSIGN                   \n", ITM_UNSIGN                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_UNITCONV                 ].itemPrinted, "U" STD_RIGHT_ARROW                                                          )) {printf("item %" FMT16S " is not MNU_UNITCONV                 \n", MNU_UNITCONV                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_VARMNU                   ].itemPrinted, "VARMNU"                                                                     )) {printf("item %" FMT16S " is not ITM_VARMNU                   \n", ITM_VARMNU                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_VARS                     ].itemPrinted, "VARS"                                                                       )) {printf("item %" FMT16S " is not MNU_VARS                     \n", MNU_VARS                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_VERS                     ].itemPrinted, "VERS?"                                                                      )) {printf("item %" FMT16S " is not ITM_VERS                     \n", ITM_VERS                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_VIEW                     ].itemPrinted, "VIEW"                                                                       )) {printf("item %" FMT16S " is not ITM_VIEW                     \n", ITM_VIEW                     ); exit(0);}
    if(strcmp(indexOfItems[CST_55                       ].itemPrinted, "V" STD_SUB_m                                                                )) {printf("item %" FMT16S " is not CST_55                       \n", CST_55                       ); exit(0);}
    if(strcmp(indexOfItems[MNU_CONVV                    ].itemPrinted, "V:"                                                                         )) {printf("item %" FMT16S " is not MNU_CONVV                    \n", MNU_CONVV                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_WDAY                     ].itemPrinted, "WDAY"                                                                       )) {printf("item %" FMT16S " is not ITM_WDAY                     \n", ITM_WDAY                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_WEIBL                    ].itemPrinted, "Weibl"                                                                      )) {printf("item %" FMT16S " is not ITM_WEIBL                    \n", ITM_WEIBL                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_WEIBLP                   ].itemPrinted, "Weibl" STD_SUB_p                                                            )) {printf("item %" FMT16S " is not ITM_WEIBLP                   \n", ITM_WEIBLP                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_WEIBLU                   ].itemPrinted, "Weibl" STD_SUB_u                                                            )) {printf("item %" FMT16S " is not ITM_WEIBLU                   \n", ITM_WEIBLU                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_WEIBLM1                  ].itemPrinted, "Weibl" STD_SUP_MINUS_1                                                      )) {printf("item %" FMT16S " is not ITM_WEIBLM1                  \n", ITM_WEIBLM1                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_WEIBL                    ].itemPrinted, "Weibl:"                                                                     )) {printf("item %" FMT16S " is not MNU_WEIBL                    \n", MNU_WEIBL                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_WHO                      ].itemPrinted, "WHO?"                                                                       )) {printf("item %" FMT16S " is not ITM_WHO                      \n", ITM_WHO                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_WHtoJ                    ].itemPrinted, "Wh" STD_RIGHT_ARROW "J"                                                     )) {printf("item %" FMT16S " is not ITM_WHtoJ                    \n", ITM_WHtoJ                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_WM                       ].itemPrinted, "W" STD_SUB_m                                                                )) {printf("item %" FMT16S " is not ITM_WM                       \n", ITM_WM                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_WP                       ].itemPrinted, "W" STD_SUB_p                                                                )) {printf("item %" FMT16S " is not ITM_WP                       \n", ITM_WP                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_WM1                      ].itemPrinted, "W" STD_SUP_MINUS_1                                                          )) {printf("item %" FMT16S " is not ITM_WM1                      \n", ITM_WM1                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_WSIZE                    ].itemPrinted, "WSIZE"                                                                      )) {printf("item %" FMT16S " is not ITM_WSIZE                    \n", ITM_WSIZE                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_WSIZEQ                   ].itemPrinted, "WSIZE?"                                                                     )) {printf("item %" FMT16S " is not ITM_WSIZEQ                   \n", ITM_WSIZEQ                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_WtoHPE                   ].itemPrinted, "W" STD_RIGHT_ARROW "hp" STD_SUB_E                                           )) {printf("item %" FMT16S " is not ITM_WtoHPE                   \n", ITM_WtoHPE                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_WtoHPM                   ].itemPrinted, "W" STD_RIGHT_ARROW "hp" STD_SUB_M                                           )) {printf("item %" FMT16S " is not ITM_WtoHPM                   \n", ITM_WtoHPM                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_WtoHPUK                  ].itemPrinted, "W" STD_RIGHT_ARROW "hp" STD_UK                                              )) {printf("item %" FMT16S " is not ITM_WtoHPUK                  \n", ITM_WtoHPUK                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_SQUARE                   ].itemPrinted, "x" STD_SUP_2                                                                )) {printf("item %" FMT16S " is not ITM_SQUARE                   \n", ITM_SQUARE                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_CUBE                     ].itemPrinted, "x" STD_SUP_3                                                                )) {printf("item %" FMT16S " is not ITM_CUBE                     \n", ITM_CUBE                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_XEQ                      ].itemPrinted, "XEQ"                                                                        )) {printf("item %" FMT16S " is not ITM_XEQ                      \n", ITM_XEQ                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_XNOR                     ].itemPrinted, "XNOR"                                                                       )) {printf("item %" FMT16S " is not ITM_XNOR                     \n", ITM_XNOR                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_XOR                      ].itemPrinted, "XOR"                                                                        )) {printf("item %" FMT16S " is not ITM_XOR                      \n", ITM_XOR                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_XBAR                     ].itemPrinted, STD_x_BAR                                                                    )) {printf("item %" FMT16S " is not ITM_XBAR                     \n", ITM_XBAR                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_XG                       ].itemPrinted, STD_x_BAR STD_SUB_G                                                          )) {printf("item %" FMT16S " is not ITM_XG                       \n", ITM_XG                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_XW                       ].itemPrinted, STD_x_BAR STD_SUB_w                                                          )) {printf("item %" FMT16S " is not ITM_XW                       \n", ITM_XW                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_XCIRC                    ].itemPrinted, STD_x_CIRC                                                                   )) {printf("item %" FMT16S " is not ITM_XCIRC                    \n", ITM_XCIRC                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_XFN                      ].itemPrinted, "X.FN"                                                                       )) {printf("item %" FMT16S " is not MNU_XFN                      \n", MNU_XFN                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_XFACT                    ].itemPrinted, "x!"                                                                         )) {printf("item %" FMT16S " is not ITM_XFACT                    \n", ITM_XFACT                    ); exit(0);}
    if(strcmp(indexOfItems[MNU_CONVX                    ].itemPrinted, "x:"                                                                         )) {printf("item %" FMT16S " is not MNU_CONVX                    \n", MNU_CONVX                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_XtoDATE                  ].itemPrinted, "x" STD_RIGHT_ARROW "DATE"                                                   )) {printf("item %" FMT16S " is not ITM_XtoDATE                  \n", ITM_XtoDATE                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_XtoALPHA                 ].itemPrinted, "x" STD_RIGHT_ARROW STD_alpha                                                )) {printf("item %" FMT16S " is not ITM_XtoALPHA                 \n", ITM_XtoALPHA                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_Xex                      ].itemPrinted, "x" STD_LEFT_RIGHT_ARROWS                                                    )) {printf("item %" FMT16S " is not ITM_Xex                      \n", ITM_Xex                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_XexY                     ].itemPrinted, "x" STD_LEFT_RIGHT_ARROWS "y"                                                )) {printf("item %" FMT16S " is not ITM_XexY                     \n", ITM_XexY                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_XEQU                     ].itemPrinted, "x= ?"                                                                       )) {printf("item %" FMT16S " is not ITM_XEQU                     \n", ITM_XEQU                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_XNE                      ].itemPrinted, "x" STD_NOT_EQUAL " ?"                                                       )) {printf("item %" FMT16S " is not ITM_XNE                      \n", ITM_XNE                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_XEQUP0                   ].itemPrinted, "x=+0?"                                                                      )) {printf("item %" FMT16S " is not ITM_XEQUP0                   \n", ITM_XEQUP0                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_XEQUM0                   ].itemPrinted, "x=-0?"                                                                      )) {printf("item %" FMT16S " is not ITM_XEQUM0                   \n", ITM_XEQUM0                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_XAEQU                    ].itemPrinted, "x" STD_ALMOST_EQUAL " ?"                                                    )) {printf("item %" FMT16S " is not ITM_XAEQU                    \n", ITM_XAEQU                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_XLT                      ].itemPrinted, "x< ?"                                                                       )) {printf("item %" FMT16S " is not ITM_XLT                      \n", ITM_XLT                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_XLE                      ].itemPrinted, "x" STD_LESS_EQUAL " ?"                                                      )) {printf("item %" FMT16S " is not ITM_XLE                      \n", ITM_XLE                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_XGE                      ].itemPrinted, "x" STD_GREATER_EQUAL " ?"                                                   )) {printf("item %" FMT16S " is not ITM_XGE                      \n", ITM_XGE                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_XGT                      ].itemPrinted, "x> ?"                                                                       )) {printf("item %" FMT16S " is not ITM_XGT                      \n", ITM_XGT                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_XTHROOT                  ].itemPrinted, STD_xTH_ROOT STD_y_UNDER_ROOT                                                )) {printf("item %" FMT16S " is not ITM_XTHROOT                  \n", ITM_XTHROOT                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_YDtoM                    ].itemPrinted, "yd." STD_RIGHT_ARROW "m"                                                    )) {printf("item %" FMT16S " is not ITM_YDtoM                    \n", ITM_YDtoM                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_YEAR                     ].itemPrinted, "YEAR"                                                                       )) {printf("item %" FMT16S " is not ITM_YEAR                     \n", ITM_YEAR                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_YEARtoS                  ].itemPrinted, "year" STD_RIGHT_ARROW "s"                                                   )) {printf("item %" FMT16S " is not ITM_YEARtoS                  \n", ITM_YEARtoS                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_YX                       ].itemPrinted, "y" STD_SUP_x                                                                )) {printf("item %" FMT16S " is not ITM_YX                       \n", ITM_YX                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_YCIRC                    ].itemPrinted, STD_y_CIRC                                                                   )) {printf("item %" FMT16S " is not ITM_YCIRC                    \n", ITM_YCIRC                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_YMD                      ].itemPrinted, "Y.MD"                                                                       )) {printf("item %" FMT16S " is not ITM_YMD                      \n", ITM_YMD                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_Yex                      ].itemPrinted, "y" STD_LEFT_RIGHT_ARROWS                                                    )) {printf("item %" FMT16S " is not ITM_Yex                      \n", ITM_Yex                      ); exit(0);}
    if(strcmp(indexOfItems[CST_56                       ].itemPrinted, "Z" STD_SUB_0                                                                )) {printf("item %" FMT16S " is not CST_56                       \n", CST_56                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_Zex                      ].itemPrinted, "z" STD_LEFT_RIGHT_ARROWS                                                    )) {printf("item %" FMT16S " is not ITM_Zex                      \n", ITM_Zex                      ); exit(0);}
    if(strcmp(indexOfItems[CST_57                       ].itemPrinted, STD_alpha                                                                    )) {printf("item %" FMT16S " is not CST_57                       \n", CST_57                       ); exit(0);}
    if(strcmp(indexOfItems[MNU_ALPHAINTL                ].itemPrinted, STD_alpha "INTL"                                                             )) {printf("item %" FMT16S " is not MNU_ALPHAINTL                \n", MNU_ALPHAINTL                ); exit(0);}
    if(strcmp(indexOfItems[ITM_ALPHALENG                ].itemPrinted, STD_alpha "LENG?"                                                            )) {printf("item %" FMT16S " is not ITM_ALPHALENG                \n", ITM_ALPHALENG                ); exit(0);}
    if(strcmp(indexOfItems[MNU_ALPHAMATH                ].itemPrinted, STD_alpha "MATH"                                                             )) {printf("item %" FMT16S " is not MNU_ALPHAMATH                \n", MNU_ALPHAMATH                ); exit(0);}
    if(strcmp(indexOfItems[ITM_ALPHAOFF                 ].itemPrinted, STD_alpha "OFF"                                                              )) {printf("item %" FMT16S " is not ITM_ALPHAOFF                 \n", ITM_ALPHAOFF                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_ALPHAON                  ].itemPrinted, STD_alpha "ON"                                                               )) {printf("item %" FMT16S " is not ITM_ALPHAON                  \n", ITM_ALPHAON                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_ALPHAPOS                 ].itemPrinted, STD_alpha "POS?"                                                             )) {printf("item %" FMT16S " is not ITM_ALPHAPOS                 \n", ITM_ALPHAPOS                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_ALPHARL                  ].itemPrinted, STD_alpha "RL"                                                               )) {printf("item %" FMT16S " is not ITM_ALPHARL                  \n", ITM_ALPHARL                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_ALPHARR                  ].itemPrinted, STD_alpha "RR"                                                               )) {printf("item %" FMT16S " is not ITM_ALPHARR                  \n", ITM_ALPHARR                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_ALPHASL                  ].itemPrinted, STD_alpha "SL"                                                               )) {printf("item %" FMT16S " is not ITM_ALPHASL                  \n", ITM_ALPHASL                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_ALPHAFN                  ].itemPrinted, STD_alpha ".FN"                                                              )) {printf("item %" FMT16S " is not MNU_ALPHAFN                  \n", MNU_ALPHAFN                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_ALPHA_OMEGA              ].itemPrinted, STD_ALPHA STD_ELLIPSIS STD_OMEGA                                             )) {printf("item %" FMT16S " is not MNU_ALPHA_OMEGA              \n", MNU_ALPHA_OMEGA              ); exit(0);}
    if(strcmp(indexOfItems[MNU_ALPHADOT                 ].itemPrinted, STD_alpha STD_DOT                                                            )) {printf("item %" FMT16S " is not MNU_ALPHADOT                 \n", MNU_ALPHADOT                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_ALPHAtoX                 ].itemPrinted, STD_alpha STD_RIGHT_ARROW "x"                                                )) {printf("item %" FMT16S " is not ITM_ALPHAtoX                 \n", ITM_ALPHAtoX                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_BETAXY                   ].itemPrinted, STD_beta "(x,y)"                                                             )) {printf("item %" FMT16S " is not ITM_BETAXY                   \n", ITM_BETAXY                   ); exit(0);}
    if(strcmp(indexOfItems[CST_58                       ].itemPrinted, STD_gamma                                                                    )) {printf("item %" FMT16S " is not CST_58                       \n", CST_58                       ); exit(0);}
    if(strcmp(indexOfItems[CST_59                       ].itemPrinted, STD_gamma STD_SUB_E STD_SUB_M                                                )) {printf("item %" FMT16S " is not CST_59                       \n", CST_59                       ); exit(0);}
    if(strcmp(indexOfItems[CST_60                       ].itemPrinted, STD_gamma STD_SUB_p                                                          )) {printf("item %" FMT16S " is not CST_60                       \n", CST_60                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_gammaXY                  ].itemPrinted, STD_gamma STD_SUB_x STD_SUB_y                                                )) {printf("item %" FMT16S " is not ITM_gammaXY                  \n", ITM_gammaXY                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_GAMMAXY                  ].itemPrinted, STD_GAMMA STD_SUB_x STD_SUB_y                                                )) {printf("item %" FMT16S " is not ITM_GAMMAXY                  \n", ITM_GAMMAXY                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_GAMMAX                   ].itemPrinted, STD_GAMMA "(x)"                                                              )) {printf("item %" FMT16S " is not ITM_GAMMAX                   \n", ITM_GAMMAX                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_deltaX                   ].itemPrinted, STD_delta "x"                                                                )) {printf("item %" FMT16S " is not ITM_deltaX                   \n", ITM_deltaX                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_DELTAPC                  ].itemPrinted, STD_DELTA "%"                                                                )) {printf("item %" FMT16S " is not ITM_DELTAPC                  \n", ITM_DELTAPC                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_epsilon                  ].itemPrinted, STD_epsilon                                                                  )) {printf("item %" FMT16S " is not ITM_epsilon                  \n", ITM_epsilon                  ); exit(0);}
    if(strcmp(indexOfItems[CST_61                       ].itemPrinted, STD_epsilon STD_SUB_0                                                        )) {printf("item %" FMT16S " is not CST_61                       \n", CST_61                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_epsilonM                 ].itemPrinted, STD_epsilon STD_SUB_m                                                        )) {printf("item %" FMT16S " is not ITM_epsilonM                 \n", ITM_epsilonM                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_epsilonP                 ].itemPrinted, STD_epsilon STD_SUB_p                                                        )) {printf("item %" FMT16S " is not ITM_epsilonP                 \n", ITM_epsilonP                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_zetaX                    ].itemPrinted, STD_zeta "(x)"                                                               )) {printf("item %" FMT16S " is not ITM_zetaX                    \n", ITM_zetaX                    ); exit(0);}
    if(strcmp(indexOfItems[CST_62                       ].itemPrinted, STD_lambda STD_SUB_C                                                         )) {printf("item %" FMT16S " is not CST_62                       \n", CST_62                       ); exit(0);}
    if(strcmp(indexOfItems[CST_63                       ].itemPrinted, STD_lambda STD_SUB_C STD_SUB_n                                               )) {printf("item %" FMT16S " is not CST_63                       \n", CST_63                       ); exit(0);}
    if(strcmp(indexOfItems[CST_64                       ].itemPrinted, STD_lambda STD_SUB_C STD_SUB_p                                               )) {printf("item %" FMT16S " is not CST_64                       \n", CST_64                       ); exit(0);}
    if(strcmp(indexOfItems[CST_65                       ].itemPrinted, STD_mu STD_SUB_0                                                             )) {printf("item %" FMT16S " is not CST_65                       \n", CST_65                       ); exit(0);}
    if(strcmp(indexOfItems[CST_66                       ].itemPrinted, STD_mu STD_SUB_B                                                             )) {printf("item %" FMT16S " is not CST_66                       \n", CST_66                       ); exit(0);}
    if(strcmp(indexOfItems[CST_67                       ].itemPrinted, STD_mu STD_SUB_e                                                             )) {printf("item %" FMT16S " is not CST_67                       \n", CST_67                       ); exit(0);}
    if(strcmp(indexOfItems[CST_68                       ].itemPrinted, STD_mu STD_SUB_e "/" STD_mu STD_SUB_B                                        )) {printf("item %" FMT16S " is not CST_68                       \n", CST_68                       ); exit(0);}
    if(strcmp(indexOfItems[CST_69                       ].itemPrinted, STD_mu STD_SUB_n                                                             )) {printf("item %" FMT16S " is not CST_69                       \n", CST_69                       ); exit(0);}
    if(strcmp(indexOfItems[CST_70                       ].itemPrinted, STD_mu STD_SUB_p                                                             )) {printf("item %" FMT16S " is not CST_70                       \n", CST_70                       ); exit(0);}
    if(strcmp(indexOfItems[CST_71                       ].itemPrinted, STD_mu STD_SUB_u                                                             )) {printf("item %" FMT16S " is not CST_71                       \n", CST_71                       ); exit(0);}
    if(strcmp(indexOfItems[CST_72                       ].itemPrinted, STD_mu STD_SUB_mu                                                            )) {printf("item %" FMT16S " is not CST_72                       \n", CST_72                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_PI                       ].itemPrinted, STD_PI                                                                       )) {printf("item %" FMT16S " is not ITM_PI                       \n", ITM_PI                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_pi                       ].itemPrinted, STD_pi                                                                       )) {printf("item %" FMT16S " is not ITM_pi                       \n", ITM_pi                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMA                    ].itemPrinted, STD_SIGMA                                                                    )) {printf("item %" FMT16S " is not ITM_SIGMA                    \n", ITM_SIGMA                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_sigma                    ].itemPrinted, STD_sigma                                                                    )) {printf("item %" FMT16S " is not ITM_sigma                    \n", ITM_sigma                    ); exit(0);}
    if(strcmp(indexOfItems[CST_73                       ].itemPrinted, STD_sigma STD_SUB_B                                                          )) {printf("item %" FMT16S " is not CST_73                       \n", CST_73                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAln2x                ].itemPrinted, STD_SIGMA "ln" STD_SUP_2 "x"                                                 )) {printf("item %" FMT16S " is not ITM_SIGMAln2x                \n", ITM_SIGMAln2x                ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAln2y                ].itemPrinted, STD_SIGMA "ln" STD_SUP_2 "y"                                                 )) {printf("item %" FMT16S " is not ITM_SIGMAln2y                \n", ITM_SIGMAln2y                ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAlnx                 ].itemPrinted, STD_SIGMA "lnx"                                                              )) {printf("item %" FMT16S " is not ITM_SIGMAlnx                 \n", ITM_SIGMAlnx                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAlnxy                ].itemPrinted, STD_SIGMA "lnxy"                                                             )) {printf("item %" FMT16S " is not ITM_SIGMAlnxy                \n", ITM_SIGMAlnxy                ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAlny                 ].itemPrinted, STD_SIGMA "lny"                                                              )) {printf("item %" FMT16S " is not ITM_SIGMAlny                 \n", ITM_SIGMAlny                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_sigmaw                   ].itemPrinted, STD_sigma STD_SUB_w                                                          )) {printf("item %" FMT16S " is not ITM_sigmaw                   \n", ITM_sigmaw                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAx                   ].itemPrinted, STD_SIGMA "x"                                                                )) {printf("item %" FMT16S " is not ITM_SIGMAx                   \n", ITM_SIGMAx                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAx2                  ].itemPrinted, STD_SIGMA "x" STD_SUP_2                                                      )) {printf("item %" FMT16S " is not ITM_SIGMAx2                  \n", ITM_SIGMAx2                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAx2y                 ].itemPrinted, STD_SIGMA "x" STD_SUP_2 "y"                                                  )) {printf("item %" FMT16S " is not ITM_SIGMAx2y                 \n", ITM_SIGMAx2y                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAxlny                ].itemPrinted, STD_SIGMA "xlny"                                                             )) {printf("item %" FMT16S " is not ITM_SIGMAxlny                \n", ITM_SIGMAxlny                ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAxy                  ].itemPrinted, STD_SIGMA "xy"                                                               )) {printf("item %" FMT16S " is not ITM_SIGMAxy                  \n", ITM_SIGMAxy                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAy                   ].itemPrinted, STD_SIGMA "y"                                                                )) {printf("item %" FMT16S " is not ITM_SIGMAy                   \n", ITM_SIGMAy                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAy2                  ].itemPrinted, STD_SIGMA "y" STD_SUP_2                                                      )) {printf("item %" FMT16S " is not ITM_SIGMAy2                  \n", ITM_SIGMAy2                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAylnx                ].itemPrinted, STD_SIGMA "ylnx"                                                             )) {printf("item %" FMT16S " is not ITM_SIGMAylnx                \n", ITM_SIGMAylnx                ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAPLUS                ].itemPrinted, STD_SIGMA "+"                                                                )) {printf("item %" FMT16S " is not ITM_SIGMAPLUS                \n", ITM_SIGMAPLUS                ); exit(0);}
    if(strcmp(indexOfItems[ITM_SIGMAMINUS               ].itemPrinted, STD_SIGMA "-"                                                                )) {printf("item %" FMT16S " is not ITM_SIGMAMINUS               \n", ITM_SIGMAMINUS               ); exit(0);}
    if(strcmp(indexOfItems[CST_74                       ].itemPrinted, STD_PHI                                                                      )) {printf("item %" FMT16S " is not CST_74                       \n", CST_74                       ); exit(0);}
    if(strcmp(indexOfItems[CST_75                       ].itemPrinted, STD_PHI STD_SUB_0                                                            )) {printf("item %" FMT16S " is not CST_75                       \n", CST_75                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_PHIux                    ].itemPrinted, STD_PHI STD_SUB_u "(x)"                                                      )) {printf("item %" FMT16S " is not ITM_PHIux                    \n", ITM_PHIux                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_PHIx                     ].itemPrinted, STD_PHI "(x)"                                                                )) {printf("item %" FMT16S " is not ITM_PHIx                     \n", ITM_PHIx                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_phix                     ].itemPrinted, STD_phi "(x)"                                                                )) {printf("item %" FMT16S " is not ITM_phix                     \n", ITM_phix                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_PHIM1p                   ].itemPrinted, STD_PHI STD_SUP_MINUS_1 "(p)"                                                )) {printf("item %" FMT16S " is not ITM_PHIM1p                   \n", ITM_PHIM1p                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_PHI                      ].itemPrinted, STD_PHI ":"                                                                  )) {printf("item %" FMT16S " is not MNU_PHI                      \n", MNU_PHI                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_chi2x                    ].itemPrinted, STD_chi STD_SUP_2 "(x)"                                                      )) {printf("item %" FMT16S " is not ITM_chi2x                    \n", ITM_chi2x                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_chi2Px                   ].itemPrinted, STD_chi STD_SUP_2 STD_SUB_P "(x)"                                            )) {printf("item %" FMT16S " is not ITM_chi2Px                   \n", ITM_chi2Px                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_chi2ux                   ].itemPrinted, STD_chi STD_SUP_2 STD_SUB_u "(x)"                                            )) {printf("item %" FMT16S " is not ITM_chi2ux                   \n", ITM_chi2ux                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_chi2M1                   ].itemPrinted, "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1                                    )) {printf("item %" FMT16S " is not ITM_chi2M1                   \n", ITM_chi2M1                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_CHI2                     ].itemPrinted, STD_chi STD_SUP_2 ":"                                                        )) {printf("item %" FMT16S " is not MNU_CHI2                     \n", MNU_CHI2                     ); exit(0);}
    if(strcmp(indexOfItems[CST_76                       ].itemPrinted, STD_omega                                                                    )) {printf("item %" FMT16S " is not CST_76                       \n", CST_76                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_M1X                      ].itemPrinted, "(-1)" STD_SUP_x                                                             )) {printf("item %" FMT16S " is not ITM_M1X                      \n", ITM_M1X                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_ADD                      ].itemPrinted, "+"                                                                          )) {printf("item %" FMT16S " is not ITM_ADD                      \n", ITM_ADD                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_CHS                      ].itemPrinted, "+/-"                                                                        )) {printf("item %" FMT16S " is not ITM_CHS                      \n", ITM_CHS                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_SUB                      ].itemPrinted, "-"                                                                          )) {printf("item %" FMT16S " is not ITM_SUB                      \n", ITM_SUB                      ); exit(0);}
    if(strcmp(indexOfItems[CST_77                       ].itemPrinted, "-" STD_INFINITY                                                             )) {printf("item %" FMT16S " is not CST_77                       \n", CST_77                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_MULT                     ].itemPrinted, STD_CROSS                                                                    )) {printf("item %" FMT16S " is not ITM_MULT                     \n", ITM_MULT                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_XMOD                     ].itemPrinted, STD_CROSS "MOD"                                                              )) {printf("item %" FMT16S " is not ITM_XMOD                     \n", ITM_XMOD                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_DIV                      ].itemPrinted, "/"                                                                          )) {printf("item %" FMT16S " is not ITM_DIV                      \n", ITM_DIV                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_PMINFINITY               ].itemPrinted, STD_PLUS_MINUS STD_INFINITY "?"                                              )) {printf("item %" FMT16S " is not ITM_PMINFINITY               \n", ITM_PMINFINITY               ); exit(0);}
    if(strcmp(indexOfItems[ITM_INDIRECTION              ].itemPrinted, STD_RIGHT_ARROW                                                              )) {printf("item %" FMT16S " is not ITM_INDIRECTION              \n", ITM_INDIRECTION              ); exit(0);}
    if(strcmp(indexOfItems[ITM_toDATE                   ].itemPrinted, STD_RIGHT_ARROW "DATE"                                                       )) {printf("item %" FMT16S " is not ITM_toDATE                   \n", ITM_toDATE                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_toDEG                    ].itemPrinted, STD_MEASURED_ANGLE STD_RIGHT_ARROW STD_MEASURED_ANGLE STD_DEGREE             )) {printf("item %" FMT16S " is not ITM_toDEG                    \n", ITM_toDEG                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_toDMS                    ].itemPrinted, STD_MEASURED_ANGLE STD_RIGHT_ARROW STD_MEASURED_ANGLE STD_RIGHT_DOUBLE_QUOTE )) {printf("item %" FMT16S " is not ITM_toDMS                    \n", ITM_toDMS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_toGRAD                   ].itemPrinted, STD_MEASURED_ANGLE STD_RIGHT_ARROW STD_MEASURED_ANGLE STD_SUP_g              )) {printf("item %" FMT16S " is not ITM_toGRAD                   \n", ITM_toGRAD                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_toHR                     ].itemPrinted, ".d"                                                                         )) {printf("item %" FMT16S " is not ITM_toHR                     \n", ITM_toHR                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_toHMS                    ].itemPrinted, "h.ms"                                                                       )) {printf("item %" FMT16S " is not ITM_toHMS                    \n", ITM_toHMS                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_toINT                    ].itemPrinted, "#"                                                                          )) {printf("item %" FMT16S " is not ITM_toINT                    \n", ITM_toINT                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_toMULpi                  ].itemPrinted, STD_MEASURED_ANGLE STD_RIGHT_ARROW STD_MEASURED_ANGLE STD_pi                 )) {printf("item %" FMT16S " is not ITM_toMULpi                  \n", ITM_toMULpi                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_toPOL                    ].itemPrinted, STD_RIGHT_ARROW "P"                                                          )) {printf("item %" FMT16S " is not ITM_toPOL                    \n", ITM_toPOL                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_toRAD                    ].itemPrinted, STD_MEASURED_ANGLE STD_RIGHT_ARROW STD_MEASURED_ANGLE STD_SUP_r              )) {printf("item %" FMT16S " is not ITM_toRAD                    \n", ITM_toRAD                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_toREAL                   ].itemPrinted, ".d"                                                                         )) {printf("item %" FMT16S " is not ITM_toREAL                   \n", ITM_toREAL                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_toREC                    ].itemPrinted, "R" STD_LEFT_ARROW                                                           )) {printf("item %" FMT16S " is not ITM_toREC                    \n", ITM_toREC                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_DtoDMS                   ].itemPrinted, STD_MEASURED_ANGLE STD_DEGREE STD_RIGHT_ARROW STD_MEASURED_ANGLE STD_RIGHT_DOUBLE_QUOTE)) {printf("item %" FMT16S " is not ITM_DtoDMS         \n", ITM_DtoDMS                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_ULIM                     ].itemPrinted, STD_UP_ARROW "Lim"                                                           )) {printf("item %" FMT16S " is not ITM_ULIM                     \n", ITM_ULIM                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_DLIM                     ].itemPrinted, STD_DOWN_ARROW "Lim"                                                         )) {printf("item %" FMT16S " is not ITM_DLIM                     \n", ITM_DLIM                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_ex                       ].itemPrinted, STD_LEFT_RIGHT_ARROWS                                                        )) {printf("item %" FMT16S " is not ITM_ex                       \n", ITM_ex                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_PC                       ].itemPrinted, "%"                                                                          )) {printf("item %" FMT16S " is not ITM_PC                       \n", ITM_PC                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_PCMRR                    ].itemPrinted, "%MRR"                                                                       )) {printf("item %" FMT16S " is not ITM_PCMRR                    \n", ITM_PCMRR                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_PCT                      ].itemPrinted, "%T"                                                                         )) {printf("item %" FMT16S " is not ITM_PCT                      \n", ITM_PCT                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_PCSIGMA                  ].itemPrinted, "%" STD_SIGMA                                                                )) {printf("item %" FMT16S " is not ITM_PCSIGMA                  \n", ITM_PCSIGMA                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_PCPMG                    ].itemPrinted, "%+MG"                                                                       )) {printf("item %" FMT16S " is not ITM_PCPMG                    \n", ITM_PCPMG                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_SQUAREROOTX              ].itemPrinted, STD_SQUARE_ROOT STD_x_UNDER_ROOT                                             )) {printf("item %" FMT16S " is not ITM_SQUAREROOTX              \n", ITM_SQUAREROOTX              ); exit(0);}
    if(strcmp(indexOfItems[ITM_INTEGRAL                 ].itemPrinted, STD_INTEGRAL                                                                 )) {printf("item %" FMT16S " is not ITM_INTEGRAL                 \n", ITM_INTEGRAL                 ); exit(0);}
    if(strcmp(indexOfItems[MNU_Sf                       ].itemPrinted, STD_INTEGRAL "f"                                                             )) {printf("item %" FMT16S " is not MNU_Sf                       \n", MNU_Sf                       ); exit(0);}
    if(strcmp(indexOfItems[MNU_Sfdx                     ].itemPrinted, STD_INTEGRAL "fdx"                                                           )) {printf("item %" FMT16S " is not MNU_Sfdx                     \n", MNU_Sfdx                     ); exit(0);}
    if(strcmp(indexOfItems[CST_78                       ].itemPrinted, STD_INFINITY                                                                 )) {printf("item %" FMT16S " is not CST_78                       \n", CST_78                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_PMOD                     ].itemPrinted, "^MOD"                                                                       )) {printf("item %" FMT16S " is not ITM_PMOD                     \n", ITM_PMOD                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_DET                    ].itemPrinted, "|M|"                                                                        )) {printf("item %" FMT16S " is not ITM_M_DET                    \n", ITM_M_DET                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_MAGNITUDE                ].itemPrinted, "|x|"                                                                        )) {printf("item %" FMT16S " is not ITM_MAGNITUDE                \n", ITM_MAGNITUDE                ); exit(0);}
    if(strcmp(indexOfItems[ITM_PARALLEL                 ].itemPrinted, "|" STD_SPACE_3_PER_EM "|"                                                   )) {printf("item %" FMT16S " is not ITM_PARALLEL                 \n", ITM_PARALLEL                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_TRANSP                 ].itemPrinted, "[M]" STD_SUP_T                                                              )) {printf("item %" FMT16S " is not ITM_M_TRANSP                 \n", ITM_M_TRANSP                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_M_INV                    ].itemPrinted, "[M]" STD_SUP_MINUS_1                                                        )) {printf("item %" FMT16S " is not ITM_M_INV                    \n", ITM_M_INV                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_ANGLE                    ].itemPrinted, STD_MEASURED_ANGLE                                                           )) {printf("item %" FMT16S " is not ITM_ANGLE                    \n", ITM_ANGLE                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_ANGLEDEGto               ].itemPrinted, STD_MEASURED_ANGLE STD_DEGREE STD_RIGHT_ARROW STD_MEASURED_ANGLE             )) {printf("item %" FMT16S " is not ITM_ANGLEDEGto               \n", ITM_ANGLEDEGto               ); exit(0);}
    if(strcmp(indexOfItems[ITM_ANGLEGRAGto              ].itemPrinted, STD_MEASURED_ANGLE STD_SUP_g STD_RIGHT_ARROW STD_MEASURED_ANGLE              )) {printf("item %" FMT16S " is not ITM_ANGLEGRAGto              \n", ITM_ANGLEGRAGto              ); exit(0);}
    if(strcmp(indexOfItems[ITM_ANGLERADto               ].itemPrinted, STD_MEASURED_ANGLE STD_SUP_r STD_RIGHT_ARROW STD_MEASURED_ANGLE              )) {printf("item %" FMT16S " is not ITM_ANGLERADto               \n", ITM_ANGLERADto               ); exit(0);}
    if(strcmp(indexOfItems[ITM_ANGLEPIto                ].itemPrinted, STD_MEASURED_ANGLE STD_pi STD_RIGHT_ARROW STD_MEASURED_ANGLE                 )) {printf("item %" FMT16S " is not ITM_ANGLEPIto                \n", ITM_ANGLEPIto                ); exit(0);}
    if(strcmp(indexOfItems[ITM_ANGLEDMSto               ].itemPrinted, STD_MEASURED_ANGLE STD_RIGHT_DOUBLE_QUOTE STD_RIGHT_ARROW STD_MEASURED_ANGLE )) {printf("item %" FMT16S " is not ITM_ANGLEDMSto               \n", ITM_ANGLEDMSto               ); exit(0);}
    if(strcmp(indexOfItems[MNU_ANGLECONV                ].itemPrinted, STD_MEASURED_ANGLE STD_RIGHT_ARROW                                           )) {printf("item %" FMT16S " is not MNU_ANGLECONV                \n", MNU_ANGLECONV                ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRINTERADV               ].itemPrinted, STD_PRINTER "ADV"                                                            )) {printf("item %" FMT16S " is not ITM_PRINTERADV               \n", ITM_PRINTERADV               ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRINTERCHAR              ].itemPrinted, STD_PRINTER "CHAR"                                                           )) {printf("item %" FMT16S " is not ITM_PRINTERCHAR              \n", ITM_PRINTERCHAR              ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRINTERDLAY              ].itemPrinted, STD_PRINTER "DLAY"                                                           )) {printf("item %" FMT16S " is not ITM_PRINTERDLAY              \n", ITM_PRINTERDLAY              ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRINTERLCD               ].itemPrinted, STD_PRINTER "LCD"                                                            )) {printf("item %" FMT16S " is not ITM_PRINTERLCD               \n", ITM_PRINTERLCD               ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRINTERMODE              ].itemPrinted, STD_PRINTER "MODE"                                                           )) {printf("item %" FMT16S " is not ITM_PRINTERMODE              \n", ITM_PRINTERMODE              ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRINTERPROG              ].itemPrinted, STD_PRINTER "PROG"                                                           )) {printf("item %" FMT16S " is not ITM_PRINTERPROG              \n", ITM_PRINTERPROG              ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRINTERR                 ].itemPrinted, STD_PRINTER "r"                                                              )) {printf("item %" FMT16S " is not ITM_PRINTERR                 \n", ITM_PRINTERR                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRINTERREGS              ].itemPrinted, STD_PRINTER "REGS"                                                           )) {printf("item %" FMT16S " is not ITM_PRINTERREGS              \n", ITM_PRINTERREGS              ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRINTERSTK               ].itemPrinted, STD_PRINTER "STK"                                                            )) {printf("item %" FMT16S " is not ITM_PRINTERSTK               \n", ITM_PRINTERSTK               ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRINTERTAB               ].itemPrinted, STD_PRINTER "TAB"                                                            )) {printf("item %" FMT16S " is not ITM_PRINTERTAB               \n", ITM_PRINTERTAB               ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRINTERUSER              ].itemPrinted, STD_PRINTER "USER"                                                           )) {printf("item %" FMT16S " is not ITM_PRINTERUSER              \n", ITM_PRINTERUSER              ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRINTERWIDTH             ].itemPrinted, STD_PRINTER "WIDTH"                                                          )) {printf("item %" FMT16S " is not ITM_PRINTERWIDTH             \n", ITM_PRINTERWIDTH             ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRINTERSIGMA             ].itemPrinted, STD_PRINTER STD_SIGMA                                                        )) {printf("item %" FMT16S " is not ITM_PRINTERSIGMA             \n", ITM_PRINTERSIGMA             ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRINTERHASH              ].itemPrinted, STD_PRINTER "#"                                                              )) {printf("item %" FMT16S " is not ITM_PRINTERHASH              \n", ITM_PRINTERHASH              ); exit(0);}
    if(strcmp(indexOfItems[ITM_NUMB                     ].itemPrinted, "#B"                                                                         )) {printf("item %" FMT16S " is not ITM_NUMB                     \n", ITM_NUMB                     ); exit(0);}

    if(strcmp(indexOfItems[ITM_ACtoM2b                  ].itemPrinted, STD_RIGHT_ARROW " m" STD_SUP_2                                               )) {printf("item %" FMT16S " is not ITM_ACtoM2b                  \n", ITM_ACtoM2b                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_ACUStoM2b                ].itemPrinted, STD_RIGHT_ARROW " m" STD_SUP_2                                               )) {printf("item %" FMT16S " is not ITM_ACUStoM2b                \n", ITM_ACUStoM2b                ); exit(0);}
    if(strcmp(indexOfItems[ITM_ATMtoPAb                 ].itemPrinted, STD_RIGHT_ARROW " Pa"                                                        )) {printf("item %" FMT16S " is not ITM_ATMtoPAb                 \n", ITM_ATMtoPAb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_DBtoFRb                  ].itemPrinted, "field"                                                                      )) {printf("item %" FMT16S " is not ITM_DBtoFRb                  \n", ITM_DBtoFRb                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_DBtoFRc                  ].itemPrinted, "ratio"                                                                      )) {printf("item %" FMT16S " is not ITM_DBtoFRc                  \n", ITM_DBtoFRc                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_DBtoPRb                  ].itemPrinted, "power"                                                                      )) {printf("item %" FMT16S " is not ITM_DBtoPRb                  \n", ITM_DBtoPRb                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_DBtoPRc                  ].itemPrinted, "ratio"                                                                      )) {printf("item %" FMT16S " is not ITM_DBtoPRc                  \n", ITM_DBtoPRc                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_FRtoDBb                  ].itemPrinted, "ratio"                                                                      )) {printf("item %" FMT16S " is not ITM_FRtoDBb                  \n", ITM_FRtoDBb                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_FRtoDBc                  ].itemPrinted, STD_RIGHT_ARROW " dB"                                                        )) {printf("item %" FMT16S " is not ITM_FRtoDBc                  \n", ITM_FRtoDBc                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_FTUStoMb                 ].itemPrinted, STD_RIGHT_ARROW " m"                                                         )) {printf("item %" FMT16S " is not ITM_FTUStoMb                 \n", ITM_FTUStoMb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_FZUKtoM3b                ].itemPrinted, STD_RIGHT_ARROW " m" STD_SUP_3                                               )) {printf("item %" FMT16S " is not ITM_FZUKtoM3b                \n", ITM_FZUKtoM3b                ); exit(0);}
    if(strcmp(indexOfItems[ITM_FZUStoM3b                ].itemPrinted, STD_RIGHT_ARROW " m" STD_SUP_3                                               )) {printf("item %" FMT16S " is not ITM_FZUStoM3b                \n", ITM_FZUStoM3b                ); exit(0);}
    if(strcmp(indexOfItems[ITM_IHGtoPAb                 ].itemPrinted, STD_RIGHT_ARROW " Pa"                                                        )) {printf("item %" FMT16S " is not ITM_IHGtoPAb                 \n", ITM_IHGtoPAb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_KGtoSCWb                 ].itemPrinted, "sh.cwt"                                                                     )) {printf("item %" FMT16S " is not ITM_KGtoSCWb                 \n", ITM_KGtoSCWb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_KGtoSTOb                 ].itemPrinted, "stones"                                                                     )) {printf("item %" FMT16S " is not ITM_KGtoSTOb                 \n", ITM_KGtoSTOb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_KGtoSTb                  ].itemPrinted, "short"                                                                      )) {printf("item %" FMT16S " is not ITM_KGtoSTb                  \n", ITM_KGtoSTb                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_KGtoSTc                  ].itemPrinted, "tons"                                                                       )) {printf("item %" FMT16S " is not ITM_KGtoSTc                  \n", ITM_KGtoSTc                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_KGtoTONb                 ].itemPrinted, "tons"                                                                       )) {printf("item %" FMT16S " is not ITM_KGtoTONb                 \n", ITM_KGtoTONb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_KGtoTRZb                 ].itemPrinted, "tr.oz"                                                                      )) {printf("item %" FMT16S " is not ITM_KGtoTRZb                 \n", ITM_KGtoTRZb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_M2toACb                  ].itemPrinted, "acres"                                                                      )) {printf("item %" FMT16S " is not ITM_M2toACb                  \n", ITM_M2toACb                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_M2toACUSb                ].itemPrinted, "acres" STD_US                                                               )) {printf("item %" FMT16S " is not ITM_M2toACUSb                \n", ITM_M2toACUSb                ); exit(0);}
    if(strcmp(indexOfItems[ITM_M3toFZUKb                ].itemPrinted, "floz" STD_UK                                                                )) {printf("item %" FMT16S " is not ITM_M3toFZUKb                \n", ITM_M3toFZUKb                ); exit(0);}
    if(strcmp(indexOfItems[ITM_M3toFZUSb                ].itemPrinted, "floz" STD_US                                                                )) {printf("item %" FMT16S " is not ITM_M3toFZUSb                \n", ITM_M3toFZUSb                ); exit(0);}
    if(strcmp(indexOfItems[ITM_MtoFTUSb                 ].itemPrinted, "s:feet" STD_US                                                              )) {printf("item %" FMT16S " is not ITM_MtoFTUSb                 \n", ITM_MtoFTUSb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_PAtoATMb                 ].itemPrinted, "atm"                                                                        )) {printf("item %" FMT16S " is not ITM_PAtoATMb                 \n", ITM_PAtoATMb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_PAtoIHGb                 ].itemPrinted, "in.Hg"                                                                      )) {printf("item %" FMT16S " is not ITM_PAtoIHGb                 \n", ITM_PAtoIHGb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_PAtoTORb                 ].itemPrinted, "torr"                                                                       )) {printf("item %" FMT16S " is not ITM_PAtoTORb                 \n", ITM_PAtoTORb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRtoDBb                  ].itemPrinted, "ratio"                                                                      )) {printf("item %" FMT16S " is not ITM_PRtoDBb                  \n", ITM_PRtoDBb                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_PRtoDBc                  ].itemPrinted, STD_RIGHT_ARROW " dB"                                                        )) {printf("item %" FMT16S " is not ITM_PRtoDBc                  \n", ITM_PRtoDBc                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_SCWtoKGb                 ].itemPrinted, "cwt" STD_RIGHT_ARROW "kg"                                                   )) {printf("item %" FMT16S " is not ITM_SCWtoKGb                 \n", ITM_SCWtoKGb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_STOtoKGb                 ].itemPrinted, STD_RIGHT_ARROW " kg"                                                        )) {printf("item %" FMT16S " is not ITM_STOtoKGb                 \n", ITM_STOtoKGb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_STtoKGb                  ].itemPrinted, "tons"                                                                       )) {printf("item %" FMT16S " is not ITM_STtoKGb                  \n", ITM_STtoKGb                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_STtoKGc                  ].itemPrinted, STD_RIGHT_ARROW " kg"                                                        )) {printf("item %" FMT16S " is not ITM_STtoKGc                  \n", ITM_STtoKGc                  ); exit(0);}
    if(strcmp(indexOfItems[ITM_TONtoKGb                 ].itemPrinted, STD_RIGHT_ARROW " kg"                                                        )) {printf("item %" FMT16S " is not ITM_TONtoKGb                 \n", ITM_TONtoKGb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_TORtoPAb                 ].itemPrinted, STD_RIGHT_ARROW " Pa"                                                        )) {printf("item %" FMT16S " is not ITM_TORtoPAb                 \n", ITM_TORtoPAb                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_TRZtoKGb                 ].itemPrinted, STD_RIGHT_ARROW " kg"                                                        )) {printf("item %" FMT16S " is not ITM_TRZtoKGb                 \n", ITM_TRZtoKGb                 ); exit(0);}

    if(strcmp(indexOfItems[ITM_FBR                      ].itemPrinted, "FBR"                                                                        )) {printf("item %" FMT16S " is not ITM_FBR                      \n", ITM_FBR                      ); exit(0);}
    if(strcmp(indexOfItems[MNU_a_z                      ].itemPrinted, "a" STD_ELLIPSIS "z"                                                         )) {printf("item %" FMT16S " is not MNU_a_z                      \n", MNU_a_z                      ); exit(0);}
    if(strcmp(indexOfItems[MNU_alpha_omega              ].itemPrinted, STD_alpha STD_ELLIPSIS STD_omega                                             )) {printf("item %" FMT16S " is not MNU_alpha_omega              \n", MNU_alpha_omega              ); exit(0);}
    if(strcmp(indexOfItems[MNU_ALPHAintl                ].itemPrinted, STD_alpha "intl"                                                             )) {printf("item %" FMT16S " is not MNU_ALPHAintl                \n", MNU_ALPHAintl                ); exit(0);}
    if(strcmp(indexOfItems[ITM_REG_X                    ].itemPrinted, "X"                                                                          )) {printf("item %" FMT16S " is not ITM_REG_X                    \n", ITM_REG_X                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_REG_Y                    ].itemPrinted, "Y"                                                                          )) {printf("item %" FMT16S " is not ITM_REG_Y                    \n", ITM_REG_Y                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_REG_Z                    ].itemPrinted, "Z"                                                                          )) {printf("item %" FMT16S " is not ITM_REG_Z                    \n", ITM_REG_Z                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_REG_T                    ].itemPrinted, "T"                                                                          )) {printf("item %" FMT16S " is not ITM_REG_T                    \n", ITM_REG_T                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_REG_A                    ].itemPrinted, "A"                                                                          )) {printf("item %" FMT16S " is not ITM_REG_A                    \n", ITM_REG_A                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_REG_B                    ].itemPrinted, "B"                                                                          )) {printf("item %" FMT16S " is not ITM_REG_B                    \n", ITM_REG_B                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_REG_C                    ].itemPrinted, "C"                                                                          )) {printf("item %" FMT16S " is not ITM_REG_C                    \n", ITM_REG_C                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_REG_D                    ].itemPrinted, "D"                                                                          )) {printf("item %" FMT16S " is not ITM_REG_D                    \n", ITM_REG_D                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_REG_L                    ].itemPrinted, "L"                                                                          )) {printf("item %" FMT16S " is not ITM_REG_L                    \n", ITM_REG_L                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_REG_I                    ].itemPrinted, "I"                                                                          )) {printf("item %" FMT16S " is not ITM_REG_I                    \n", ITM_REG_I                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_REG_J                    ].itemPrinted, "J"                                                                          )) {printf("item %" FMT16S " is not ITM_REG_J                    \n", ITM_REG_J                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_REG_K                    ].itemPrinted, "K"                                                                          )) {printf("item %" FMT16S " is not ITM_REG_K                    \n", ITM_REG_K                    ); exit(0);}

    if(strcmp(indexOfItems[CHR_0                        ].itemPrinted, "0"                                                                          )) {printf("char %" FMT16S " is not CHR_0                        \n", CHR_0                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_1                        ].itemPrinted, "1"                                                                          )) {printf("char %" FMT16S " is not CHR_1                        \n", CHR_1                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_2                        ].itemPrinted, "2"                                                                          )) {printf("char %" FMT16S " is not CHR_2                        \n", CHR_2                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_3                        ].itemPrinted, "3"                                                                          )) {printf("char %" FMT16S " is not CHR_3                        \n", CHR_3                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_4                        ].itemPrinted, "4"                                                                          )) {printf("char %" FMT16S " is not CHR_4                        \n", CHR_4                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_5                        ].itemPrinted, "5"                                                                          )) {printf("char %" FMT16S " is not CHR_5                        \n", CHR_5                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_6                        ].itemPrinted, "6"                                                                          )) {printf("char %" FMT16S " is not CHR_6                        \n", CHR_6                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_7                        ].itemPrinted, "7"                                                                          )) {printf("char %" FMT16S " is not CHR_7                        \n", CHR_7                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_8                        ].itemPrinted, "8"                                                                          )) {printf("char %" FMT16S " is not CHR_8                        \n", CHR_8                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_9                        ].itemPrinted, "9"                                                                          )) {printf("char %" FMT16S " is not CHR_9                        \n", CHR_9                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_A                        ].itemPrinted, "A"                                                                          )) {printf("char %" FMT16S " is not CHR_A                        \n", CHR_A                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_B                        ].itemPrinted, "B"                                                                          )) {printf("char %" FMT16S " is not CHR_B                        \n", CHR_B                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_C                        ].itemPrinted, "C"                                                                          )) {printf("char %" FMT16S " is not CHR_C                        \n", CHR_C                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_D                        ].itemPrinted, "D"                                                                          )) {printf("char %" FMT16S " is not CHR_D                        \n", CHR_D                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_E                        ].itemPrinted, "E"                                                                          )) {printf("char %" FMT16S " is not CHR_E                        \n", CHR_E                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_F                        ].itemPrinted, "F"                                                                          )) {printf("char %" FMT16S " is not CHR_F                        \n", CHR_F                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_G                        ].itemPrinted, "G"                                                                          )) {printf("char %" FMT16S " is not CHR_G                        \n", CHR_G                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_H                        ].itemPrinted, "H"                                                                          )) {printf("char %" FMT16S " is not CHR_H                        \n", CHR_H                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_I                        ].itemPrinted, "I"                                                                          )) {printf("char %" FMT16S " is not CHR_I                        \n", CHR_I                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_J                        ].itemPrinted, "J"                                                                          )) {printf("char %" FMT16S " is not CHR_J                        \n", CHR_J                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_K                        ].itemPrinted, "K"                                                                          )) {printf("char %" FMT16S " is not CHR_K                        \n", CHR_K                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_L                        ].itemPrinted, "L"                                                                          )) {printf("char %" FMT16S " is not CHR_L                        \n", CHR_L                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_M                        ].itemPrinted, "M"                                                                          )) {printf("char %" FMT16S " is not CHR_M                        \n", CHR_M                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_N                        ].itemPrinted, "N"                                                                          )) {printf("char %" FMT16S " is not CHR_N                        \n", CHR_N                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_O                        ].itemPrinted, "O"                                                                          )) {printf("char %" FMT16S " is not CHR_O                        \n", CHR_O                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_P                        ].itemPrinted, "P"                                                                          )) {printf("char %" FMT16S " is not CHR_P                        \n", CHR_P                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_Q                        ].itemPrinted, "Q"                                                                          )) {printf("char %" FMT16S " is not CHR_Q                        \n", CHR_Q                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_R                        ].itemPrinted, "R"                                                                          )) {printf("char %" FMT16S " is not CHR_R                        \n", CHR_R                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_S                        ].itemPrinted, "S"                                                                          )) {printf("char %" FMT16S " is not CHR_S                        \n", CHR_S                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_T                        ].itemPrinted, "T"                                                                          )) {printf("char %" FMT16S " is not CHR_T                        \n", CHR_T                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_U                        ].itemPrinted, "U"                                                                          )) {printf("char %" FMT16S " is not CHR_U                        \n", CHR_U                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_V                        ].itemPrinted, "V"                                                                          )) {printf("char %" FMT16S " is not CHR_V                        \n", CHR_V                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_W                        ].itemPrinted, "W"                                                                          )) {printf("char %" FMT16S " is not CHR_W                        \n", CHR_W                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_X                        ].itemPrinted, "X"                                                                          )) {printf("char %" FMT16S " is not CHR_X                        \n", CHR_X                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_Y                        ].itemPrinted, "Y"                                                                          )) {printf("char %" FMT16S " is not CHR_Y                        \n", CHR_Y                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_Z                        ].itemPrinted, "Z"                                                                          )) {printf("char %" FMT16S " is not CHR_Z                        \n", CHR_Z                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_a                        ].itemPrinted, "a"                                                                          )) {printf("char %" FMT16S " is not CHR_a                        \n", CHR_a                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_b                        ].itemPrinted, "b"                                                                          )) {printf("char %" FMT16S " is not CHR_b                        \n", CHR_b                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_c                        ].itemPrinted, "c"                                                                          )) {printf("char %" FMT16S " is not CHR_c                        \n", CHR_c                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_d                        ].itemPrinted, "d"                                                                          )) {printf("char %" FMT16S " is not CHR_d                        \n", CHR_d                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_e                        ].itemPrinted, "e"                                                                          )) {printf("char %" FMT16S " is not CHR_e                        \n", CHR_e                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_f                        ].itemPrinted, "f"                                                                          )) {printf("char %" FMT16S " is not CHR_f                        \n", CHR_f                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_g                        ].itemPrinted, "g"                                                                          )) {printf("char %" FMT16S " is not CHR_g                        \n", CHR_g                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_h                        ].itemPrinted, "h"                                                                          )) {printf("char %" FMT16S " is not CHR_h                        \n", CHR_h                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_i                        ].itemPrinted, "i"                                                                          )) {printf("char %" FMT16S " is not CHR_i                        \n", CHR_i                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_j                        ].itemPrinted, "j"                                                                          )) {printf("char %" FMT16S " is not CHR_j                        \n", CHR_j                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_k                        ].itemPrinted, "k"                                                                          )) {printf("char %" FMT16S " is not CHR_k                        \n", CHR_k                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_l                        ].itemPrinted, "l"                                                                          )) {printf("char %" FMT16S " is not CHR_l                        \n", CHR_l                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_m                        ].itemPrinted, "m"                                                                          )) {printf("char %" FMT16S " is not CHR_m                        \n", CHR_m                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_n                        ].itemPrinted, "n"                                                                          )) {printf("char %" FMT16S " is not CHR_n                        \n", CHR_n                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_o                        ].itemPrinted, "o"                                                                          )) {printf("char %" FMT16S " is not CHR_o                        \n", CHR_o                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_p                        ].itemPrinted, "p"                                                                          )) {printf("char %" FMT16S " is not CHR_p                        \n", CHR_p                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_q                        ].itemPrinted, "q"                                                                          )) {printf("char %" FMT16S " is not CHR_q                        \n", CHR_q                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_r                        ].itemPrinted, "r"                                                                          )) {printf("char %" FMT16S " is not CHR_r                        \n", CHR_r                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_s                        ].itemPrinted, "s"                                                                          )) {printf("char %" FMT16S " is not CHR_s                        \n", CHR_s                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_t                        ].itemPrinted, "t"                                                                          )) {printf("char %" FMT16S " is not CHR_t                        \n", CHR_t                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_u                        ].itemPrinted, "u"                                                                          )) {printf("char %" FMT16S " is not CHR_u                        \n", CHR_u                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_v                        ].itemPrinted, "v"                                                                          )) {printf("char %" FMT16S " is not CHR_v                        \n", CHR_v                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_w                        ].itemPrinted, "w"                                                                          )) {printf("char %" FMT16S " is not CHR_w                        \n", CHR_w                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_x                        ].itemPrinted, "x"                                                                          )) {printf("char %" FMT16S " is not CHR_x                        \n", CHR_x                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_y                        ].itemPrinted, "y"                                                                          )) {printf("char %" FMT16S " is not CHR_y                        \n", CHR_y                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_z                        ].itemPrinted, "z"                                                                          )) {printf("char %" FMT16S " is not CHR_z                        \n", CHR_z                        ); exit(0);}
    if(strcmp(indexOfItems[CHR_ALPHA                    ].itemPrinted, STD_ALPHA                                                                    )) {printf("char %" FMT16S " is not CHR_ALPHA                    \n", CHR_ALPHA                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_0960                     ].itemPrinted, "0960"                                                                       )) {printf("char %" FMT16S " is not CHR_0960                     \n", CHR_0960                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_BETA                     ].itemPrinted, STD_BETA                                                                     )) {printf("char %" FMT16S " is not CHR_BETA                     \n", CHR_BETA                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_GAMMA                    ].itemPrinted, STD_GAMMA                                                                    )) {printf("char %" FMT16S " is not CHR_GAMMA                    \n", CHR_GAMMA                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_DELTA                    ].itemPrinted, STD_DELTA                                                                    )) {printf("char %" FMT16S " is not CHR_DELTA                    \n", CHR_DELTA                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_EPSILON                  ].itemPrinted, STD_EPSILON                                                                  )) {printf("char %" FMT16S " is not CHR_EPSILON                  \n", CHR_EPSILON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_0960                     ].itemPrinted, "0960"                                                                       )) {printf("char %" FMT16S " is not CHR_0960                     \n", CHR_0960                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_ZETA                     ].itemPrinted, STD_ZETA                                                                     )) {printf("char %" FMT16S " is not CHR_ZETA                     \n", CHR_ZETA                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_ETA                      ].itemPrinted, STD_ETA                                                                      )) {printf("char %" FMT16S " is not CHR_ETA                      \n", CHR_ETA                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_0963                     ].itemPrinted, "0963"                                                                       )) {printf("char %" FMT16S " is not CHR_0963                     \n", CHR_0963                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_THETA                    ].itemPrinted, STD_THETA                                                                    )) {printf("char %" FMT16S " is not CHR_THETA                    \n", CHR_THETA                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_IOTA                     ].itemPrinted, STD_IOTA                                                                     )) {printf("char %" FMT16S " is not CHR_IOTA                     \n", CHR_IOTA                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_0966                     ].itemPrinted, "0966"                                                                       )) {printf("char %" FMT16S " is not CHR_0966                     \n", CHR_0966                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_0967                     ].itemPrinted, "0967"                                                                       )) {printf("char %" FMT16S " is not CHR_0967                     \n", CHR_0967                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_IOTA_DIALYTIKA           ].itemPrinted, STD_IOTA_DIALYTIKA                                                           )) {printf("char %" FMT16S " is not CHR_IOTA_DIALYTIKA           \n", CHR_IOTA_DIALYTIKA           ); exit(0);}
    if(strcmp(indexOfItems[CHR_KAPPA                    ].itemPrinted, STD_KAPPA                                                                    )) {printf("char %" FMT16S " is not CHR_KAPPA                    \n", CHR_KAPPA                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_LAMBDA                   ].itemPrinted, STD_LAMBDA                                                                   )) {printf("char %" FMT16S " is not CHR_LAMBDA                   \n", CHR_LAMBDA                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_MU                       ].itemPrinted, STD_MU                                                                       )) {printf("char %" FMT16S " is not CHR_MU                       \n", CHR_MU                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_NU                       ].itemPrinted, STD_NU                                                                       )) {printf("char %" FMT16S " is not CHR_NU                       \n", CHR_NU                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_XI                       ].itemPrinted, STD_XI                                                                       )) {printf("char %" FMT16S " is not CHR_XI                       \n", CHR_XI                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_OMICRON                  ].itemPrinted, STD_OMICRON                                                                  )) {printf("char %" FMT16S " is not CHR_OMICRON                  \n", CHR_OMICRON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_0975                     ].itemPrinted, "0975"                                                                       )) {printf("char %" FMT16S " is not CHR_0975                     \n", CHR_0975                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_PI                       ].itemPrinted, STD_PI                                                                       )) {printf("char %" FMT16S " is not CHR_PI                       \n", CHR_PI                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_RHO                      ].itemPrinted, STD_RHO                                                                      )) {printf("char %" FMT16S " is not CHR_RHO                      \n", CHR_RHO                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_SIGMA                    ].itemPrinted, STD_SIGMA                                                                    )) {printf("char %" FMT16S " is not CHR_SIGMA                    \n", CHR_SIGMA                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_0979                     ].itemPrinted, "0979"                                                                       )) {printf("char %" FMT16S " is not CHR_0679                     \n", CHR_0979                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_TAU                      ].itemPrinted, STD_TAU                                                                      )) {printf("char %" FMT16S " is not CHR_TAU                      \n", CHR_TAU                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_UPSILON                  ].itemPrinted, STD_UPSILON                                                                  )) {printf("char %" FMT16S " is not CHR_UPSILON                  \n", CHR_UPSILON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_0982                     ].itemPrinted, "0982"                                                                       )) {printf("char %" FMT16S " is not CHR_0982                     \n", CHR_0982                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_UPSILON_DIALYTIKA        ].itemPrinted, STD_UPSILON_DIALYTIKA                                                        )) {printf("char %" FMT16S " is not CHR_UPSILON_DIALYTIKA        \n", CHR_UPSILON_DIALYTIKA        ); exit(0);}
    if(strcmp(indexOfItems[CHR_0984                     ].itemPrinted, "0984"                                                                       )) {printf("char %" FMT16S " is not CHR_0984                     \n", CHR_0984                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_PHI                      ].itemPrinted, STD_PHI                                                                      )) {printf("char %" FMT16S " is not CHR_PHI                      \n", CHR_PHI                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_CHI                      ].itemPrinted, STD_CHI                                                                      )) {printf("char %" FMT16S " is not CHR_CHI                      \n", CHR_CHI                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_PSI                      ].itemPrinted, STD_PSI                                                                      )) {printf("char %" FMT16S " is not CHR_PSI                      \n", CHR_PSI                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_OMEGA                    ].itemPrinted, STD_OMEGA                                                                    )) {printf("char %" FMT16S " is not CHR_OMEGA                    \n", CHR_OMEGA                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_0989                     ].itemPrinted, "0989"                                                                       )) {printf("char %" FMT16S " is not CHR_0989                     \n", CHR_0989                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_alpha                    ].itemPrinted, STD_alpha                                                                    )) {printf("char %" FMT16S " is not CHR_alpha                    \n", CHR_alpha                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_alpha_TONOS              ].itemPrinted, STD_alpha_TONOS                                                              )) {printf("char %" FMT16S " is not CHR_alpha_TONOS              \n", CHR_alpha_TONOS              ); exit(0);}
    if(strcmp(indexOfItems[CHR_beta                     ].itemPrinted, STD_beta                                                                     )) {printf("char %" FMT16S " is not CHR_beta                     \n", CHR_beta                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_gamma                    ].itemPrinted, STD_gamma                                                                    )) {printf("char %" FMT16S " is not CHR_gamma                    \n", CHR_gamma                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_delta                    ].itemPrinted, STD_delta                                                                    )) {printf("char %" FMT16S " is not CHR_delta                    \n", CHR_delta                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_epsilon                  ].itemPrinted, STD_epsilon                                                                  )) {printf("char %" FMT16S " is not CHR_epsilon                  \n", CHR_epsilon                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_epsilon_TONOS            ].itemPrinted, STD_epsilon_TONOS                                                            )) {printf("char %" FMT16S " is not CHR_epsilon_TONOS            \n", CHR_epsilon_TONOS            ); exit(0);}
    if(strcmp(indexOfItems[CHR_zeta                     ].itemPrinted, STD_zeta                                                                     )) {printf("char %" FMT16S " is not CHR_zeta                     \n", CHR_zeta                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_eta                      ].itemPrinted, STD_eta                                                                      )) {printf("char %" FMT16S " is not CHR_eta                      \n", CHR_eta                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_eta_TONOS                ].itemPrinted, STD_eta_TONOS                                                                )) {printf("char %" FMT16S " is not CHR_eta_TONOS                \n", CHR_eta_TONOS                ); exit(0);}
    if(strcmp(indexOfItems[CHR_theta                    ].itemPrinted, STD_theta                                                                    )) {printf("char %" FMT16S " is not CHR_theta                    \n", CHR_theta                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_iota                     ].itemPrinted, STD_iota                                                                     )) {printf("char %" FMT16S " is not CHR_iota                     \n", CHR_iota                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_iotaTON                  ].itemPrinted, STD_iota_TONOS                                                               )) {printf("char %" FMT16S " is not CHR_iotaTON                  \n", CHR_iotaTON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_iota_DIALYTIKA_TONOS     ].itemPrinted, STD_iota_DIALYTIKA_TONOS                                                     )) {printf("char %" FMT16S " is not CHR_iota_DIALYTIKA_TONOS     \n", CHR_iota_DIALYTIKA_TONOS     ); exit(0);}
    if(strcmp(indexOfItems[CHR_iota_DIALYTIKA           ].itemPrinted, STD_iota_DIALYTIKA                                                           )) {printf("char %" FMT16S " is not CHR_iota_DIALYTIKA           \n", CHR_iota_DIALYTIKA           ); exit(0);}
    if(strcmp(indexOfItems[CHR_kappa                    ].itemPrinted, STD_kappa                                                                    )) {printf("char %" FMT16S " is not CHR_kappa                    \n", CHR_kappa                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_lambda                   ].itemPrinted, STD_lambda                                                                   )) {printf("char %" FMT16S " is not CHR_lambda                   \n", CHR_lambda                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_mu                       ].itemPrinted, STD_mu                                                                       )) {printf("char %" FMT16S " is not CHR_mu                       \n", CHR_mu                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_nu                       ].itemPrinted, STD_nu                                                                       )) {printf("char %" FMT16S " is not CHR_nu                       \n", CHR_nu                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_xi                       ].itemPrinted, STD_xi                                                                       )) {printf("char %" FMT16S " is not CHR_xi                       \n", CHR_xi                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_omicron                  ].itemPrinted, STD_omicron                                                                  )) {printf("char %" FMT16S " is not CHR_omicron                  \n", CHR_omicron                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_omicron_TONOS            ].itemPrinted, STD_omicron_TONOS                                                            )) {printf("char %" FMT16S " is not CHR_omicron_TONOS            \n", CHR_omicron_TONOS            ); exit(0);}
    if(strcmp(indexOfItems[CHR_pi                       ].itemPrinted, STD_pi                                                                       )) {printf("char %" FMT16S " is not CHR_pi                       \n", CHR_pi                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_rho                      ].itemPrinted, STD_rho                                                                      )) {printf("char %" FMT16S " is not CHR_rho                      \n", CHR_rho                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_sigma_end                ].itemPrinted, STD_sigma_end                                                                )) {printf("char %" FMT16S " is not CHR_sigma_end                \n", CHR_sigma_end                ); exit(0);}
    if(strcmp(indexOfItems[CHR_sigma                    ].itemPrinted, STD_sigma                                                                    )) {printf("char %" FMT16S " is not CHR_sigma                    \n", CHR_sigma                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_tau                      ].itemPrinted, STD_tau                                                                      )) {printf("char %" FMT16S " is not CHR_tau                      \n", CHR_tau                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_upsilon                  ].itemPrinted, STD_upsilon                                                                  )) {printf("char %" FMT16S " is not CHR_upsilon                  \n", CHR_upsilon                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_upsilon_TONOS            ].itemPrinted, STD_upsilon_TONOS                                                            )) {printf("char %" FMT16S " is not CHR_upsilon_TONOS            \n", CHR_upsilon_TONOS            ); exit(0);}
    if(strcmp(indexOfItems[CHR_upsilon_DIALYTIKA        ].itemPrinted, STD_upsilon_DIALYTIKA                                                        )) {printf("char %" FMT16S " is not CHR_upsilon_DIALYTIKA        \n", CHR_upsilon_DIALYTIKA        ); exit(0);}
    if(strcmp(indexOfItems[CHR_upsilon_DIALYTIKA_TONOS  ].itemPrinted, STD_upsilon_DIALYTIKA_TONOS                                                  )) {printf("char %" FMT16S " is not CHR_upsilon_DIALYTIKA_TONOS  \n", CHR_upsilon_DIALYTIKA_TONOS  ); exit(0);}
    if(strcmp(indexOfItems[CHR_phi                      ].itemPrinted, STD_phi                                                                      )) {printf("char %" FMT16S " is not CHR_phi                      \n", CHR_phi                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_chi                      ].itemPrinted, STD_chi                                                                      )) {printf("char %" FMT16S " is not CHR_chi                      \n", CHR_chi                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_psi                      ].itemPrinted, STD_psi                                                                      )) {printf("char %" FMT16S " is not CHR_psi                      \n", CHR_psi                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_omega                    ].itemPrinted, STD_omega                                                                    )) {printf("char %" FMT16S " is not CHR_omega                    \n", CHR_omega                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_omega_TONOS              ].itemPrinted, STD_omega_TONOS                                                              )) {printf("char %" FMT16S " is not CHR_omega_TONOS              \n", CHR_omega_TONOS              ); exit(0);}
    if(strcmp(indexOfItems[CHR_1026                     ].itemPrinted, "1026"                                                                       )) {printf("char %" FMT16S " is not CHR_1026                     \n", CHR_1026                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1027                     ].itemPrinted, "1027"                                                                       )) {printf("char %" FMT16S " is not CHR_1027                     \n", CHR_1027                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1028                     ].itemPrinted, "1028"                                                                       )) {printf("char %" FMT16S " is not CHR_1028                     \n", CHR_1028                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1029                     ].itemPrinted, "1029"                                                                       )) {printf("char %" FMT16S " is not CHR_1029                     \n", CHR_1029                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1030                     ].itemPrinted, "1030"                                                                       )) {printf("char %" FMT16S " is not CHR_1030                     \n", CHR_1030                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1031                     ].itemPrinted, "1031"                                                                       )) {printf("char %" FMT16S " is not CHR_1031                     \n", CHR_1031                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_A_MACRON                 ].itemPrinted, STD_A_MACRON                                                                 )) {printf("char %" FMT16S " is not CHR_A_MACRON                 \n", CHR_A_MACRON                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_A_ACUTE                  ].itemPrinted, STD_A_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_A_ACUTE                  \n", CHR_A_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_A_BREVE                  ].itemPrinted, STD_A_BREVE                                                                  )) {printf("char %" FMT16S " is not CHR_A_BREVE                  \n", CHR_A_BREVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_A_GRAVE                  ].itemPrinted, STD_A_GRAVE                                                                  )) {printf("char %" FMT16S " is not CHR_A_GRAVE                  \n", CHR_A_GRAVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_A_DIARESIS               ].itemPrinted, STD_A_DIARESIS                                                               )) {printf("char %" FMT16S " is not CHR_A_DIARESIS               \n", CHR_A_DIARESIS               ); exit(0);}
    if(strcmp(indexOfItems[CHR_A_TILDE                  ].itemPrinted, STD_A_TILDE                                                                  )) {printf("char %" FMT16S " is not CHR_A_TILDE                  \n", CHR_A_TILDE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_A_CIRC                   ].itemPrinted, STD_A_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_A_CIRC                   \n", CHR_A_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_A_RING                   ].itemPrinted, STD_A_RING                                                                   )) {printf("char %" FMT16S " is not CHR_A_RING                   \n", CHR_A_RING                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_AE                       ].itemPrinted, STD_AE                                                                       )) {printf("char %" FMT16S " is not CHR_AE                       \n", CHR_AE                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_A_OGONEK                 ].itemPrinted, STD_A_OGONEK                                                                 )) {printf("char %" FMT16S " is not CHR_A_OGONEK                 \n", CHR_A_OGONEK                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_C_ACUTE                  ].itemPrinted, STD_C_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_C_ACUTE                  \n", CHR_C_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_C_CARON                  ].itemPrinted, STD_C_CARON                                                                  )) {printf("char %" FMT16S " is not CHR_C_CARON                  \n", CHR_C_CARON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_C_CEDILLA                ].itemPrinted, STD_C_CEDILLA                                                                )) {printf("char %" FMT16S " is not CHR_C_CEDILLA                \n", CHR_C_CEDILLA                ); exit(0);}
    if(strcmp(indexOfItems[CHR_D_STROKE                 ].itemPrinted, STD_D_STROKE                                                                 )) {printf("char %" FMT16S " is not CHR_D_STROKE                 \n", CHR_D_STROKE                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_D_CARON                  ].itemPrinted, STD_D_CARON                                                                  )) {printf("char %" FMT16S " is not CHR_D_CARON                  \n", CHR_D_CARON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_E_MACRON                 ].itemPrinted, STD_E_MACRON                                                                 )) {printf("char %" FMT16S " is not CHR_E_MACRON                 \n", CHR_E_MACRON                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_E_ACUTE                  ].itemPrinted, STD_E_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_E_ACUTE                  \n", CHR_E_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_E_BREVE                  ].itemPrinted, STD_E_BREVE                                                                  )) {printf("char %" FMT16S " is not CHR_E_BREVE                  \n", CHR_E_BREVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_E_GRAVE                  ].itemPrinted, STD_E_GRAVE                                                                  )) {printf("char %" FMT16S " is not CHR_E_GRAVE                  \n", CHR_E_GRAVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_E_DIARESIS               ].itemPrinted, STD_E_DIARESIS                                                               )) {printf("char %" FMT16S " is not CHR_E_DIARESIS               \n", CHR_E_DIARESIS               ); exit(0);}
    if(strcmp(indexOfItems[CHR_E_CIRC                   ].itemPrinted, STD_E_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_E_CIRC                   \n", CHR_E_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_E_OGONEK                 ].itemPrinted, STD_E_OGONEK                                                                 )) {printf("char %" FMT16S " is not CHR_E_OGONEK                 \n", CHR_E_OGONEK                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_G_BREVE                  ].itemPrinted, STD_G_BREVE                                                                  )) {printf("char %" FMT16S " is not CHR_G_BREVE                  \n", CHR_G_BREVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_1055                     ].itemPrinted, "1055"                                                                       )) {printf("char %" FMT16S " is not CHR_1055                     \n", CHR_1055                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_I_MACRON                 ].itemPrinted, STD_I_MACRON                                                                 )) {printf("char %" FMT16S " is not CHR_I_MACRON                 \n", CHR_I_MACRON                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_I_ACUTE                  ].itemPrinted, STD_I_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_I_ACUTE                  \n", CHR_I_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_I_BREVE                  ].itemPrinted, STD_I_BREVE                                                                  )) {printf("char %" FMT16S " is not CHR_I_BREVE                  \n", CHR_I_BREVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_I_GRAVE                  ].itemPrinted, STD_I_GRAVE                                                                  )) {printf("char %" FMT16S " is not CHR_I_GRAVE                  \n", CHR_I_GRAVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_I_DIARESIS               ].itemPrinted, STD_I_DIARESIS                                                               )) {printf("char %" FMT16S " is not CHR_I_DIARESIS               \n", CHR_I_DIARESIS               ); exit(0);}
    if(strcmp(indexOfItems[CHR_I_CIRC                   ].itemPrinted, STD_I_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_I_CIRC                   \n", CHR_I_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_I_OGONEK                 ].itemPrinted, STD_I_OGONEK                                                                 )) {printf("char %" FMT16S " is not CHR_I_OGONEK                 \n", CHR_I_OGONEK                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_I_DOT                    ].itemPrinted, STD_I_DOT                                                                    )) {printf("char %" FMT16S " is not CHR_I_DOT                    \n", CHR_I_DOT                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_I_DOTLESS                ].itemPrinted, STD_I                                                                        )) {printf("char %" FMT16S " is not CHR_I_DOTLESS                \n", CHR_I_DOTLESS                ); exit(0);}
    if(strcmp(indexOfItems[CHR_L_STROKE                 ].itemPrinted, STD_L_STROKE                                                                 )) {printf("char %" FMT16S " is not CHR_L_STROKE                 \n", CHR_L_STROKE                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_L_ACUTE                  ].itemPrinted, STD_L_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_L_ACUTE                  \n", CHR_L_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_L_APOSTROPHE             ].itemPrinted, STD_L_APOSTROPHE                                                             )) {printf("char %" FMT16S " is not CHR_L_APOSTROPHE             \n", CHR_L_APOSTROPHE             ); exit(0);}
    if(strcmp(indexOfItems[CHR_N_ACUTE                  ].itemPrinted, STD_N_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_N_ACUTE                  \n", CHR_N_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_N_CARON                  ].itemPrinted, STD_N_CARON                                                                  )) {printf("char %" FMT16S " is not CHR_N_CARON                  \n", CHR_N_CARON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_N_TILDE                  ].itemPrinted, STD_N_TILDE                                                                  )) {printf("char %" FMT16S " is not CHR_N_TILDE                  \n", CHR_N_TILDE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_O_MACRON                 ].itemPrinted, STD_O_MACRON                                                                 )) {printf("char %" FMT16S " is not CHR_O_MACRON                 \n", CHR_O_MACRON                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_O_ACUTE                  ].itemPrinted, STD_O_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_O_ACUTE                  \n", CHR_O_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_O_BREVE                  ].itemPrinted, STD_O_BREVE                                                                  )) {printf("char %" FMT16S " is not CHR_O_BREVE                  \n", CHR_O_BREVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_O_GRAVE                  ].itemPrinted, STD_O_GRAVE                                                                  )) {printf("char %" FMT16S " is not CHR_O_GRAVE                  \n", CHR_O_GRAVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_O_DIARESIS               ].itemPrinted, STD_O_DIARESIS                                                               )) {printf("char %" FMT16S " is not CHR_O_DIARESIS               \n", CHR_O_DIARESIS               ); exit(0);}
    if(strcmp(indexOfItems[CHR_O_TILDE                  ].itemPrinted, STD_O_TILDE                                                                  )) {printf("char %" FMT16S " is not CHR_O_TILDE                  \n", CHR_O_TILDE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_O_CIRC                   ].itemPrinted, STD_O_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_O_CIRC                   \n", CHR_O_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_O_STROKE                 ].itemPrinted, STD_O_STROKE                                                                 )) {printf("char %" FMT16S " is not CHR_O_STROKE                 \n", CHR_O_STROKE                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_OE                       ].itemPrinted, STD_OE                                                                       )) {printf("char %" FMT16S " is not CHR_OE                       \n", CHR_OE                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_1080                     ].itemPrinted, "1080"                                                                       )) {printf("char %" FMT16S " is not CHR_1080                     \n", CHR_1080                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1081                     ].itemPrinted, "1081"                                                                       )) {printf("char %" FMT16S " is not CHR_1081                     \n", CHR_1081                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1082                     ].itemPrinted, "1082"                                                                       )) {printf("char %" FMT16S " is not CHR_1082                     \n", CHR_1082                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_S_ACUTE                  ].itemPrinted, STD_S_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_S_ACUTE                  \n", CHR_S_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_S_CARON                  ].itemPrinted, STD_S_CARON                                                                  )) {printf("char %" FMT16S " is not CHR_S_CARON                  \n", CHR_S_CARON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_S_CEDILLA                ].itemPrinted, STD_S_CEDILLA                                                                )) {printf("char %" FMT16S " is not CHR_S_CEDILLA                \n", CHR_S_CEDILLA                ); exit(0);}
    if(strcmp(indexOfItems[CHR_T_CARON                  ].itemPrinted, STD_T_CARON                                                                  )) {printf("char %" FMT16S " is not CHR_T_CARON                  \n", CHR_T_CARON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_T_CEDILLA                ].itemPrinted, STD_T_CEDILLA                                                                )) {printf("char %" FMT16S " is not CHR_T_CEDILLA                \n", CHR_T_CEDILLA                ); exit(0);}
    if(strcmp(indexOfItems[CHR_U_MACRON                 ].itemPrinted, STD_U_MACRON                                                                 )) {printf("char %" FMT16S " is not CHR_U_MACRON                 \n", CHR_U_MACRON                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_U_ACUTE                  ].itemPrinted, STD_U_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_U_ACUTE                  \n", CHR_U_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_U_BREVE                  ].itemPrinted, STD_U_BREVE                                                                  )) {printf("char %" FMT16S " is not CHR_U_BREVE                  \n", CHR_U_BREVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_U_GRAVE                  ].itemPrinted, STD_U_GRAVE                                                                  )) {printf("char %" FMT16S " is not CHR_U_GRAVE                  \n", CHR_U_GRAVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_U_DIARESIS               ].itemPrinted, STD_U_DIARESIS                                                               )) {printf("char %" FMT16S " is not CHR_U_DIARESIS               \n", CHR_U_DIARESIS               ); exit(0);}
    if(strcmp(indexOfItems[CHR_U_TILDE                  ].itemPrinted, STD_U_TILDE                                                                  )) {printf("char %" FMT16S " is not CHR_U_TILDE                  \n", CHR_U_TILDE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_U_CIRC                   ].itemPrinted, STD_U_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_U_CIRC                   \n", CHR_U_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_U_RING                   ].itemPrinted, STD_U_RING                                                                   )) {printf("char %" FMT16S " is not CHR_U_RING                   \n", CHR_U_RING                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_W_CIRC                   ].itemPrinted, STD_W_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_W_CIRC                   \n", CHR_W_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_1097                     ].itemPrinted, "1097"                                                                       )) {printf("char %" FMT16S " is not CHR_1097                     \n", CHR_1097                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1098                     ].itemPrinted, "1098"                                                                       )) {printf("char %" FMT16S " is not CHR_1098                     \n", CHR_1098                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1099                     ].itemPrinted, "1099"                                                                       )) {printf("char %" FMT16S " is not CHR_1099                     \n", CHR_1099                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_Y_CIRC                   ].itemPrinted, STD_Y_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_Y_CIRC                   \n", CHR_Y_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_Y_ACUTE                  ].itemPrinted, STD_Y_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_Y_ACUTE                  \n", CHR_Y_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_Y_DIARESIS               ].itemPrinted, STD_Y_DIARESIS                                                               )) {printf("char %" FMT16S " is not CHR_Y_DIARESIS               \n", CHR_Y_DIARESIS               ); exit(0);}
    if(strcmp(indexOfItems[CHR_Z_ACUTE                  ].itemPrinted, STD_Z_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_Z_ACUTE                  \n", CHR_Z_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_Z_CARON                  ].itemPrinted, STD_Z_CARON                                                                  )) {printf("char %" FMT16S " is not CHR_Z_CARON                  \n", CHR_Z_CARON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_Z_DOT                    ].itemPrinted, STD_Z_DOT                                                                    )) {printf("char %" FMT16S " is not CHR_Z_DOT                    \n", CHR_Z_DOT                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_1106                     ].itemPrinted, "1106"                                                                       )) {printf("char %" FMT16S " is not CHR_1106                     \n", CHR_1106                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1107                     ].itemPrinted, "1107"                                                                       )) {printf("char %" FMT16S " is not CHR_1107                     \n", CHR_1107                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1108                     ].itemPrinted, "1108"                                                                       )) {printf("char %" FMT16S " is not CHR_1108                     \n", CHR_1108                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1109                     ].itemPrinted, "1109"                                                                       )) {printf("char %" FMT16S " is not CHR_1109                     \n", CHR_1109                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1110                     ].itemPrinted, "1110"                                                                       )) {printf("char %" FMT16S " is not CHR_1110                     \n", CHR_1110                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1111                     ].itemPrinted, "1111"                                                                       )) {printf("char %" FMT16S " is not CHR_1111                     \n", CHR_1111                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_a_MACRON                 ].itemPrinted, STD_a_MACRON                                                                 )) {printf("char %" FMT16S " is not CHR_a_MACRON                 \n", CHR_a_MACRON                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_a_ACUTE                  ].itemPrinted, STD_a_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_a_ACUTE                  \n", CHR_a_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_a_BREVE                  ].itemPrinted, STD_a_BREVE                                                                  )) {printf("char %" FMT16S " is not CHR_a_BREVE                  \n", CHR_a_BREVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_a_GRAVE                  ].itemPrinted, STD_a_GRAVE                                                                  )) {printf("char %" FMT16S " is not CHR_a_GRAVE                  \n", CHR_a_GRAVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_a_DIARESIS               ].itemPrinted, STD_a_DIARESIS                                                               )) {printf("char %" FMT16S " is not CHR_a_DIARESIS               \n", CHR_a_DIARESIS               ); exit(0);}
    if(strcmp(indexOfItems[CHR_a_TILDE                  ].itemPrinted, STD_a_TILDE                                                                  )) {printf("char %" FMT16S " is not CHR_a_TILDE                  \n", CHR_a_TILDE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_a_CIRC                   ].itemPrinted, STD_a_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_a_CIRC                   \n", CHR_a_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_a_RING                   ].itemPrinted, STD_a_RING                                                                   )) {printf("char %" FMT16S " is not CHR_a_RING                   \n", CHR_a_RING                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_ae                       ].itemPrinted, STD_ae                                                                       )) {printf("char %" FMT16S " is not CHR_ae                       \n", CHR_ae                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_a_OGONEK                 ].itemPrinted, STD_a_OGONEK                                                                 )) {printf("char %" FMT16S " is not CHR_a_OGONEK                 \n", CHR_a_OGONEK                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_c_ACUTE                  ].itemPrinted, STD_c_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_c_ACUTE                  \n", CHR_c_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_c_CARON                  ].itemPrinted, STD_c_CARON                                                                  )) {printf("char %" FMT16S " is not CHR_c_CARON                  \n", CHR_c_CARON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_c_CEDILLA                ].itemPrinted, STD_c_CEDILLA                                                                )) {printf("char %" FMT16S " is not CHR_c_CEDILLA                \n", CHR_c_CEDILLA                ); exit(0);}
    if(strcmp(indexOfItems[CHR_d_STROKE                 ].itemPrinted, STD_d_STROKE                                                                 )) {printf("char %" FMT16S " is not CHR_d_STROKE                 \n", CHR_d_STROKE                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_d_APOSTROPHE             ].itemPrinted, STD_d_APOSTROPHE                                                             )) {printf("char %" FMT16S " is not CHR_d_APOSTROPHE             \n", CHR_d_APOSTROPHE             ); exit(0);}
    if(strcmp(indexOfItems[CHR_e_MACRON                 ].itemPrinted, STD_e_MACRON                                                                 )) {printf("char %" FMT16S " is not CHR_e_MACRON                 \n", CHR_e_MACRON                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_e_ACUTE                  ].itemPrinted, STD_e_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_e_ACUTE                  \n", CHR_e_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_e_BREVE                  ].itemPrinted, STD_e_BREVE                                                                  )) {printf("char %" FMT16S " is not CHR_e_BREVE                  \n", CHR_e_BREVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_e_GRAVE                  ].itemPrinted, STD_e_GRAVE                                                                  )) {printf("char %" FMT16S " is not CHR_e_GRAVE                  \n", CHR_e_GRAVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_e_DIARESIS               ].itemPrinted, STD_e_DIARESIS                                                               )) {printf("char %" FMT16S " is not CHR_e_DIARESIS               \n", CHR_e_DIARESIS               ); exit(0);}
    if(strcmp(indexOfItems[CHR_e_CIRC                   ].itemPrinted, STD_e_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_e_CIRC                   \n", CHR_e_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_e_OGONEK                 ].itemPrinted, STD_e_OGONEK                                                                 )) {printf("char %" FMT16S " is not CHR_e_OGONEK                 \n", CHR_e_OGONEK                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_g_BREVE                  ].itemPrinted, STD_g_BREVE                                                                  )) {printf("char %" FMT16S " is not CHR_g_BREVE                  \n", CHR_g_BREVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_h_STROKE                 ].itemPrinted, STD_h_STROKE                                                                 )) {printf("char %" FMT16S " is not CHR_h_STROKE                 \n", CHR_h_STROKE                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_i_MACRON                 ].itemPrinted, STD_i_MACRON                                                                 )) {printf("char %" FMT16S " is not CHR_i_MACRON                 \n", CHR_i_MACRON                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_i_ACUTE                  ].itemPrinted, STD_i_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_i_ACUTE                  \n", CHR_i_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_i_BREVE                  ].itemPrinted, STD_i_BREVE                                                                  )) {printf("char %" FMT16S " is not CHR_i_BREVE                  \n", CHR_i_BREVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_i_GRAVE                  ].itemPrinted, STD_i_GRAVE                                                                  )) {printf("char %" FMT16S " is not CHR_i_GRAVE                  \n", CHR_i_GRAVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_i_DIARESIS               ].itemPrinted, STD_i_DIARESIS                                                               )) {printf("char %" FMT16S " is not CHR_i_DIARESIS               \n", CHR_i_DIARESIS               ); exit(0);}
    if(strcmp(indexOfItems[CHR_i_CIRC                   ].itemPrinted, STD_i_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_i_CIRC                   \n", CHR_i_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_i_OGONEK                 ].itemPrinted, STD_i_OGONEK                                                                 )) {printf("char %" FMT16S " is not CHR_i_OGONEK                 \n", CHR_i_OGONEK                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_i_DOT                    ].itemPrinted, STD_i                                                                        )) {printf("char %" FMT16S " is not CHR_i_DOT                    \n", CHR_i_DOT                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_i_DOTLESS                ].itemPrinted, STD_i_DOTLESS                                                                )) {printf("char %" FMT16S " is not CHR_i_DOTLESS                \n", CHR_i_DOTLESS                ); exit(0);}
    if(strcmp(indexOfItems[CHR_l_STROKE                 ].itemPrinted, STD_l_STROKE                                                                 )) {printf("char %" FMT16S " is not CHR_l_STROKE                 \n", CHR_l_STROKE                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_l_ACUTE                  ].itemPrinted, STD_l_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_l_ACUTE                  \n", CHR_l_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_l_APOSTROPHE             ].itemPrinted, STD_l_APOSTROPHE                                                             )) {printf("char %" FMT16S " is not CHR_l_APOSTROPHE             \n", CHR_l_APOSTROPHE             ); exit(0);}
    if(strcmp(indexOfItems[CHR_n_ACUTE                  ].itemPrinted, STD_n_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_n_ACUTE                  \n", CHR_n_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_n_CARON                  ].itemPrinted, STD_n_CARON                                                                  )) {printf("char %" FMT16S " is not CHR_n_CARON                  \n", CHR_n_CARON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_n_TILDE                  ].itemPrinted, STD_n_TILDE                                                                  )) {printf("char %" FMT16S " is not CHR_n_TILDE                  \n", CHR_n_TILDE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_o_MACRON                 ].itemPrinted, STD_o_MACRON                                                                 )) {printf("char %" FMT16S " is not CHR_o_MACRON                 \n", CHR_o_MACRON                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_o_ACUTE                  ].itemPrinted, STD_o_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_o_ACUTE                  \n", CHR_o_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_o_BREVE                  ].itemPrinted, STD_o_BREVE                                                                  )) {printf("char %" FMT16S " is not CHR_o_BREVE                  \n", CHR_o_BREVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_o_GRAVE                  ].itemPrinted, STD_o_GRAVE                                                                  )) {printf("char %" FMT16S " is not CHR_o_GRAVE                  \n", CHR_o_GRAVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_o_DIARESIS               ].itemPrinted, STD_o_DIARESIS                                                               )) {printf("char %" FMT16S " is not CHR_o_DIARESIS               \n", CHR_o_DIARESIS               ); exit(0);}
    if(strcmp(indexOfItems[CHR_o_TILDE                  ].itemPrinted, STD_o_TILDE                                                                  )) {printf("char %" FMT16S " is not CHR_o_TILDE                  \n", CHR_o_TILDE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_o_CIRC                   ].itemPrinted, STD_o_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_o_CIRC                   \n", CHR_o_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_o_STROKE                 ].itemPrinted, STD_o_STROKE                                                                 )) {printf("char %" FMT16S " is not CHR_o_STROKE                 \n", CHR_o_STROKE                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_oe                       ].itemPrinted, STD_oe                                                                       )) {printf("char %" FMT16S " is not CHR_oe                       \n", CHR_oe                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_r_CARON                  ].itemPrinted, STD_r_CARON                                                                  )) {printf("char %" FMT16S " is not CHR_r_CARON                  \n", CHR_r_CARON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_r_ACUTE                  ].itemPrinted, STD_r_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_r_ACUTE                  \n", CHR_r_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_s_SHARP                  ].itemPrinted, STD_s_SHARP                                                                  )) {printf("char %" FMT16S " is not CHR_s_SHARP                  \n", CHR_s_SHARP                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_s_ACUTE                  ].itemPrinted, STD_s_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_s_ACUTE                  \n", CHR_s_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_s_CARON                  ].itemPrinted, STD_s_CARON                                                                  )) {printf("char %" FMT16S " is not CHR_s_CARON                  \n", CHR_s_CARON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_s_CEDILLA                ].itemPrinted, STD_s_CEDILLA                                                                )) {printf("char %" FMT16S " is not CHR_s_CEDILLA                \n", CHR_s_CEDILLA                ); exit(0);}
    if(strcmp(indexOfItems[CHR_t_APOSTROPHE             ].itemPrinted, STD_t_APOSTROPHE                                                             )) {printf("char %" FMT16S " is not CHR_t_APOSTROPHE             \n", CHR_t_APOSTROPHE             ); exit(0);}
    if(strcmp(indexOfItems[CHR_t_CEDILLA                ].itemPrinted, STD_t_CEDILLA                                                                )) {printf("char %" FMT16S " is not CHR_t_CEDILLA                \n", CHR_t_CEDILLA                ); exit(0);}
    if(strcmp(indexOfItems[CHR_u_MACRON                 ].itemPrinted, STD_u_MACRON                                                                 )) {printf("char %" FMT16S " is not CHR_u_MACRON                 \n", CHR_u_MACRON                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_u_ACUTE                  ].itemPrinted, STD_u_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_u_ACUTE                  \n", CHR_u_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_u_BREVE                  ].itemPrinted, STD_u_BREVE                                                                  )) {printf("char %" FMT16S " is not CHR_u_BREVE                  \n", CHR_u_BREVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_u_GRAVE                  ].itemPrinted, STD_u_GRAVE                                                                  )) {printf("char %" FMT16S " is not CHR_u_GRAVE                  \n", CHR_u_GRAVE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_u_DIARESIS               ].itemPrinted, STD_u_DIARESIS                                                               )) {printf("char %" FMT16S " is not CHR_u_DIARESIS               \n", CHR_u_DIARESIS               ); exit(0);}
    if(strcmp(indexOfItems[CHR_u_TILDE                  ].itemPrinted, STD_u_TILDE                                                                  )) {printf("char %" FMT16S " is not CHR_u_TILDE                  \n", CHR_u_TILDE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_u_CIRC                   ].itemPrinted, STD_u_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_u_CIRC                   \n", CHR_u_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_u_RING                   ].itemPrinted, STD_u_RING                                                                   )) {printf("char %" FMT16S " is not CHR_u_RING                   \n", CHR_u_RING                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_w_CIRC                   ].itemPrinted, STD_w_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_w_CIRC                   \n", CHR_w_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_x_BAR                    ].itemPrinted, STD_x_BAR                                                                    )) {printf("char %" FMT16S " is not CHR_x_BAR                    \n", CHR_x_BAR                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_x_CIRC                   ].itemPrinted, STD_x_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_x_CIRC                   \n", CHR_x_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_y_BAR                    ].itemPrinted, STD_y_BAR                                                                    )) {printf("char %" FMT16S " is not CHR_y_BAR                    \n", CHR_y_BAR                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_y_CIRC                   ].itemPrinted, STD_y_CIRC                                                                   )) {printf("char %" FMT16S " is not CHR_y_CIRC                   \n", CHR_y_CIRC                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_y_ACUTE                  ].itemPrinted, STD_y_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_y_ACUTE                  \n", CHR_y_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_y_DIARESIS               ].itemPrinted, STD_y_DIARESIS                                                               )) {printf("char %" FMT16S " is not CHR_y_DIARESIS               \n", CHR_y_DIARESIS               ); exit(0);}
    if(strcmp(indexOfItems[CHR_z_ACUTE                  ].itemPrinted, STD_z_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_z_ACUTE                  \n", CHR_z_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_z_CARON                  ].itemPrinted, STD_z_CARON                                                                  )) {printf("char %" FMT16S " is not CHR_z_CARON                  \n", CHR_z_CARON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_z_DOT                    ].itemPrinted, STD_z_DOT                                                                    )) {printf("char %" FMT16S " is not CHR_z_DOT                    \n", CHR_z_DOT                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_1186                     ].itemPrinted, "1186"                                                                       )) {printf("char %" FMT16S " is not CHR_1186                     \n", CHR_1186                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1187                     ].itemPrinted, "1187"                                                                       )) {printf("char %" FMT16S " is not CHR_1187                     \n", CHR_1187                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1188                     ].itemPrinted, "1188"                                                                       )) {printf("char %" FMT16S " is not CHR_1188                     \n", CHR_1188                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1189                     ].itemPrinted, "1189"                                                                       )) {printf("char %" FMT16S " is not CHR_1189                     \n", CHR_1189                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1190                     ].itemPrinted, "1190"                                                                       )) {printf("char %" FMT16S " is not CHR_1190                     \n", CHR_1190                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_1191                     ].itemPrinted, "1191"                                                                       )) {printf("char %" FMT16S " is not CHR_1191                     \n", CHR_1191                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_alpha                ].itemPrinted, STD_SUB_alpha                                                                )) {printf("char %" FMT16S " is not CHR_SUB_alpha                \n", CHR_SUB_alpha                ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_delta                ].itemPrinted, STD_SUB_delta                                                                )) {printf("char %" FMT16S " is not CHR_SUB_delta                \n", CHR_SUB_delta                ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_mu                   ].itemPrinted, STD_SUB_mu                                                                   )) {printf("char %" FMT16S " is not CHR_SUB_mu                   \n", CHR_SUB_mu                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_SUN                  ].itemPrinted, STD_SUB_SUN                                                                  )) {printf("char %" FMT16S " is not CHR_SUB_SUN                  \n", CHR_SUB_SUN                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_SUN_b                ].itemPrinted, STD_SUB_SUN_b                                                                )) {printf("char %" FMT16S " is not CHR_SUB_SUN_b                \n", CHR_SUB_SUN_b                ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_EARTH                ].itemPrinted, STD_SUB_EARTH                                                                )) {printf("char %" FMT16S " is not CHR_SUB_EARTH                \n", CHR_SUB_EARTH                ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_EARTH_b              ].itemPrinted, STD_SUB_EARTH_b                                                              )) {printf("char %" FMT16S " is not CHR_SUB_EARTH_b              \n", CHR_SUB_EARTH_b              ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_PLUS                 ].itemPrinted, STD_SUB_PLUS                                                                 )) {printf("char %" FMT16S " is not CHR_SUB_PLUS                 \n", CHR_SUB_PLUS                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_MINUS                ].itemPrinted, STD_SUB_MINUS                                                                )) {printf("char %" FMT16S " is not CHR_SUB_MINUS                \n", CHR_SUB_MINUS                ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_INFINITY             ].itemPrinted, STD_SUB_INFINITY                                                             )) {printf("char %" FMT16S " is not CHR_SUB_INFINITY             \n", CHR_SUB_INFINITY             ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_0                    ].itemPrinted, STD_SUB_0                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_0                    \n", CHR_SUB_0                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_1                    ].itemPrinted, STD_SUB_1                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_1                    \n", CHR_SUB_1                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_2                    ].itemPrinted, STD_SUB_2                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_2                    \n", CHR_SUB_2                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_3                    ].itemPrinted, STD_SUB_3                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_3                    \n", CHR_SUB_3                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_4                    ].itemPrinted, STD_SUB_4                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_4                    \n", CHR_SUB_4                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_5                    ].itemPrinted, STD_SUB_5                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_5                    \n", CHR_SUB_5                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_6                    ].itemPrinted, STD_SUB_6                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_6                    \n", CHR_SUB_6                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_7                    ].itemPrinted, STD_SUB_7                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_7                    \n", CHR_SUB_7                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_8                    ].itemPrinted, STD_SUB_8                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_8                    \n", CHR_SUB_8                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_9                    ].itemPrinted, STD_SUB_9                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_9                    \n", CHR_SUB_9                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_10                   ].itemPrinted, STD_SUB_10                                                                   )) {printf("char %" FMT16S " is not CHR_SUB_10                   \n", CHR_SUB_10                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_A                    ].itemPrinted, STD_SUB_A                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_A                    \n", CHR_SUB_A                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_B                    ].itemPrinted, STD_SUB_B                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_B                    \n", CHR_SUB_B                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_C                    ].itemPrinted, STD_SUB_C                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_C                    \n", CHR_SUB_C                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_D                    ].itemPrinted, STD_SUB_D                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_D                    \n", CHR_SUB_D                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_E                    ].itemPrinted, STD_SUB_E                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_E                    \n", CHR_SUB_E                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_F                    ].itemPrinted, STD_SUB_F                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_F                    \n", CHR_SUB_F                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_G                    ].itemPrinted, STD_SUB_G                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_G                    \n", CHR_SUB_G                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_H                    ].itemPrinted, STD_SUB_H                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_H                    \n", CHR_SUB_H                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_I                    ].itemPrinted, STD_SUB_I                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_I                    \n", CHR_SUB_I                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_J                    ].itemPrinted, STD_SUB_J                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_J                    \n", CHR_SUB_J                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_K                    ].itemPrinted, STD_SUB_K                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_K                    \n", CHR_SUB_K                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_L                    ].itemPrinted, STD_SUB_L                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_L                    \n", CHR_SUB_L                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_M                    ].itemPrinted, STD_SUB_M                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_M                    \n", CHR_SUB_M                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_N                    ].itemPrinted, STD_SUB_N                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_N                    \n", CHR_SUB_N                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_O                    ].itemPrinted, STD_SUB_O                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_O                    \n", CHR_SUB_O                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_P                    ].itemPrinted, STD_SUB_P                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_P                    \n", CHR_SUB_P                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_Q                    ].itemPrinted, STD_SUB_Q                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_Q                    \n", CHR_SUB_Q                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_R                    ].itemPrinted, STD_SUB_R                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_R                    \n", CHR_SUB_R                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_S                    ].itemPrinted, STD_SUB_S                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_S                    \n", CHR_SUB_S                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_T                    ].itemPrinted, STD_SUB_T                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_T                    \n", CHR_SUB_T                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_U                    ].itemPrinted, STD_SUB_U                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_U                    \n", CHR_SUB_U                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_V                    ].itemPrinted, STD_SUB_V                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_V                    \n", CHR_SUB_V                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_W                    ].itemPrinted, STD_SUB_W                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_W                    \n", CHR_SUB_W                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_X                    ].itemPrinted, STD_SUB_X                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_X                    \n", CHR_SUB_X                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_Y                    ].itemPrinted, STD_SUB_Y                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_Y                    \n", CHR_SUB_Y                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_Z                    ].itemPrinted, STD_SUB_Z                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_Z                    \n", CHR_SUB_Z                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_E_OUTLINE            ].itemPrinted, STD_SUB_E_OUTLINE                                                            )) {printf("char %" FMT16S " is not CHR_SUB_E_OUTLINE            \n", CHR_SUB_E_OUTLINE            ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_a                    ].itemPrinted, STD_SUB_a                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_a                    \n", CHR_SUB_a                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_b                    ].itemPrinted, STD_SUB_b                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_b                    \n", CHR_SUB_b                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_c                    ].itemPrinted, STD_SUB_c                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_c                    \n", CHR_SUB_c                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_d                    ].itemPrinted, STD_SUB_d                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_d                    \n", CHR_SUB_d                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_e                    ].itemPrinted, STD_SUB_e                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_e                    \n", CHR_SUB_e                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_h                    ].itemPrinted, STD_SUB_h                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_h                    \n", CHR_SUB_h                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_i                    ].itemPrinted, STD_SUB_i                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_i                    \n", CHR_SUB_i                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_j                    ].itemPrinted, STD_SUB_j                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_j                    \n", CHR_SUB_j                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_k                    ].itemPrinted, STD_SUB_k                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_k                    \n", CHR_SUB_k                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_l                    ].itemPrinted, STD_SUB_l                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_l                    \n", CHR_SUB_l                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_m                    ].itemPrinted, STD_SUB_m                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_m                    \n", CHR_SUB_m                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_n                    ].itemPrinted, STD_SUB_n                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_n                    \n", CHR_SUB_n                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_o                    ].itemPrinted, STD_SUB_o                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_o                    \n", CHR_SUB_o                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_p                    ].itemPrinted, STD_SUB_p                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_p                    \n", CHR_SUB_p                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_q                    ].itemPrinted, STD_SUB_q                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_q                    \n", CHR_SUB_q                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_s                    ].itemPrinted, STD_SUB_s                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_s                    \n", CHR_SUB_s                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_t                    ].itemPrinted, STD_SUB_t                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_t                    \n", CHR_SUB_t                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_u                    ].itemPrinted, STD_SUB_u                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_u                    \n", CHR_SUB_u                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_v                    ].itemPrinted, STD_SUB_v                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_v                    \n", CHR_SUB_v                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_w                    ].itemPrinted, STD_SUB_w                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_w                    \n", CHR_SUB_w                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_x                    ].itemPrinted, STD_SUB_x                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_x                    \n", CHR_SUB_x                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_y                    ].itemPrinted, STD_SUB_y                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_y                    \n", CHR_SUB_y                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_z                    ].itemPrinted, STD_SUB_z                                                                    )) {printf("char %" FMT16S " is not CHR_SUB_z                    \n", CHR_SUB_z                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_a_b                  ].itemPrinted, STD_SUB_a_b                                                                  )) {printf("char %" FMT16S " is not CHR_SUB_a_b                  \n", CHR_SUB_a_b                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_e_b                  ].itemPrinted, STD_SUB_e_b                                                                  )) {printf("char %" FMT16S " is not CHR_SUB_e_b                  \n", CHR_SUB_e_b                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_k_b                  ].itemPrinted, STD_SUB_k_b                                                                  )) {printf("char %" FMT16S " is not CHR_SUB_k_b                  \n", CHR_SUB_k_b                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_l_b                  ].itemPrinted, STD_SUB_l_b                                                                  )) {printf("char %" FMT16S " is not CHR_SUB_l_b                  \n", CHR_SUB_l_b                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_m_b                  ].itemPrinted, STD_SUB_m_b                                                                  )) {printf("char %" FMT16S " is not CHR_SUB_m_b                  \n", CHR_SUB_m_b                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_n_b                  ].itemPrinted, STD_SUB_n_b                                                                  )) {printf("char %" FMT16S " is not CHR_SUB_n_b                  \n", CHR_SUB_n_b                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_o_b                  ].itemPrinted, STD_SUB_o_b                                                                  )) {printf("char %" FMT16S " is not CHR_SUB_o_b                  \n", CHR_SUB_o_b                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_p_b                  ].itemPrinted, STD_SUB_p_b                                                                  )) {printf("char %" FMT16S " is not CHR_SUB_p_b                  \n", CHR_SUB_p_b                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_s_b                  ].itemPrinted, STD_SUB_s_b                                                                  )) {printf("char %" FMT16S " is not CHR_SUB_s_b                  \n", CHR_SUB_s_b                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_u_b                  ].itemPrinted, STD_SUB_u_b                                                                  )) {printf("char %" FMT16S " is not CHR_SUB_u_b                  \n", CHR_SUB_u_b                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUB_x_b                  ].itemPrinted, STD_SUB_x_b                                                                  )) {printf("char %" FMT16S " is not CHR_SUB_x_b                  \n", CHR_SUB_x_b                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_PLUS                 ].itemPrinted, STD_SUP_PLUS                                                                 )) {printf("char %" FMT16S " is not CHR_SUP_PLUS                 \n", CHR_SUP_PLUS                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_MINUS                ].itemPrinted, STD_SUP_MINUS                                                                )) {printf("char %" FMT16S " is not CHR_SUP_MINUS                \n", CHR_SUP_MINUS                ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_MINUS_1              ].itemPrinted, STD_SUP_MINUS_1                                                              )) {printf("char %" FMT16S " is not CHR_SUP_MINUS_1              \n", CHR_SUP_MINUS_1              ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_INFINITY             ].itemPrinted, STD_SUP_INFINITY                                                             )) {printf("char %" FMT16S " is not CHR_SUP_INFINITY             \n", CHR_SUP_INFINITY             ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_ASTERISK             ].itemPrinted, STD_SUP_ASTERISK                                                             )) {printf("char %" FMT16S " is not CHR_SUP_ASTERISK             \n", CHR_SUP_ASTERISK             ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_0                    ].itemPrinted, STD_SUP_0                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_0                    \n", CHR_SUP_0                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_1                    ].itemPrinted, STD_SUP_1                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_1                    \n", CHR_SUP_1                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_2                    ].itemPrinted, STD_SUP_2                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_2                    \n", CHR_SUP_2                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_3                    ].itemPrinted, STD_SUP_3                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_3                    \n", CHR_SUP_3                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_4                    ].itemPrinted, STD_SUP_4                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_4                    \n", CHR_SUP_4                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_5                    ].itemPrinted, STD_SUP_5                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_5                    \n", CHR_SUP_5                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_6                    ].itemPrinted, STD_SUP_6                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_6                    \n", CHR_SUP_6                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_7                    ].itemPrinted, STD_SUP_7                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_7                    \n", CHR_SUP_7                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_8                    ].itemPrinted, STD_SUP_8                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_8                    \n", CHR_SUP_8                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_9                    ].itemPrinted, STD_SUP_9                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_9                    \n", CHR_SUP_9                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_1_b                  ].itemPrinted, STD_SUP_1_b                                                                  )) {printf("char %" FMT16S " is not CHR_SUP_1_b                  \n", CHR_SUP_1_b                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_T                    ].itemPrinted, STD_SUP_T                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_T                    \n", CHR_SUP_T                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_a                    ].itemPrinted, STD_SUP_a                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_a                    \n", CHR_SUP_a                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_f                    ].itemPrinted, STD_SUP_f                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_f                    \n", CHR_SUP_f                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_g                    ].itemPrinted, STD_SUP_g                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_g                    \n", CHR_SUP_g                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_h                    ].itemPrinted, STD_SUP_h                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_h                    \n", CHR_SUP_h                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_r                    ].itemPrinted, STD_SUP_r                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_r                    \n", CHR_SUP_r                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUP_x                    ].itemPrinted, STD_SUP_x                                                                    )) {printf("char %" FMT16S " is not CHR_SUP_x                    \n", CHR_SUP_x                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SPACE                    ].itemPrinted, STD_SPACE                                                                    )) {printf("char %" FMT16S " is not CHR_SPACE                    \n", CHR_SPACE                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_EXCLAMATION_MARK         ].itemPrinted, STD_EXCLAMATION_MARK                                                         )) {printf("char %" FMT16S " is not CHR_EXCLAMATION_MARK         \n", CHR_EXCLAMATION_MARK         ); exit(0);}
    if(strcmp(indexOfItems[CHR_DOUBLE_QUOTE             ].itemPrinted, STD_DOUBLE_QUOTE                                                             )) {printf("char %" FMT16S " is not CHR_DOUBLE_QUOTE             \n", CHR_DOUBLE_QUOTE             ); exit(0);}
    if(strcmp(indexOfItems[CHR_NUMBER_SIGN              ].itemPrinted, STD_NUMBER_SIGN                                                              )) {printf("char %" FMT16S " is not CHR_NUMBER_SIGN              \n", CHR_NUMBER_SIGN              ); exit(0);}
    if(strcmp(indexOfItems[CHR_DOLLAR                   ].itemPrinted, STD_DOLLAR                                                                   )) {printf("char %" FMT16S " is not CHR_DOLLAR                   \n", CHR_DOLLAR                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_PERCENT                  ].itemPrinted, STD_PERCENT                                                                  )) {printf("char %" FMT16S " is not CHR_PERCENT                  \n", CHR_PERCENT                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_AMPERSAND                ].itemPrinted, STD_AMPERSAND                                                                )) {printf("char %" FMT16S " is not CHR_AMPERSAND                \n", CHR_AMPERSAND                ); exit(0);}
    if(strcmp(indexOfItems[CHR_QUOTE                    ].itemPrinted, STD_QUOTE                                                                    )) {printf("char %" FMT16S " is not CHR_QUOTE                    \n", CHR_QUOTE                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_LEFT_PARENTHESIS         ].itemPrinted, STD_LEFT_PARENTHESIS                                                         )) {printf("char %" FMT16S " is not CHR_LEFT_PARENTHESIS         \n", CHR_LEFT_PARENTHESIS         ); exit(0);}
    if(strcmp(indexOfItems[CHR_RIGHT_PARENTHESIS        ].itemPrinted, STD_RIGHT_PARENTHESIS                                                        )) {printf("char %" FMT16S " is not CHR_RIGHT_PARENTHESIS        \n", CHR_RIGHT_PARENTHESIS        ); exit(0);}
    if(strcmp(indexOfItems[CHR_ASTERISK                 ].itemPrinted, STD_ASTERISK                                                                 )) {printf("char %" FMT16S " is not CHR_ASTERISK                 \n", CHR_ASTERISK                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_ASTERISK_b               ].itemPrinted, STD_ASTERISK_b                                                               )) {printf("char %" FMT16S " is not CHR_ASTERISK_b               \n", CHR_ASTERISK_b               ); exit(0);}
    if(strcmp(indexOfItems[CHR_PLUS                     ].itemPrinted, STD_PLUS                                                                     )) {printf("char %" FMT16S " is not CHR_PLUS                     \n", CHR_PLUS                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_COMMA                    ].itemPrinted, STD_COMMA                                                                    )) {printf("char %" FMT16S " is not CHR_COMMA                    \n", CHR_COMMA                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_MINUS                    ].itemPrinted, STD_MINUS                                                                    )) {printf("char %" FMT16S " is not CHR_MINUS                    \n", CHR_MINUS                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_PERIOD                   ].itemPrinted, STD_PERIOD                                                                   )) {printf("char %" FMT16S " is not CHR_PERIOD                   \n", CHR_PERIOD                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_SLASH                    ].itemPrinted, STD_SLASH                                                                    )) {printf("char %" FMT16S " is not CHR_SLASH                    \n", CHR_SLASH                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_COLON                    ].itemPrinted, STD_COLON                                                                    )) {printf("char %" FMT16S " is not CHR_COLON                    \n", CHR_COLON                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_SEMICOLON                ].itemPrinted, STD_SEMICOLON                                                                )) {printf("char %" FMT16S " is not CHR_SEMICOLON                \n", CHR_SEMICOLON                ); exit(0);}
    if(strcmp(indexOfItems[CHR_LESS_THAN                ].itemPrinted, STD_LESS_THAN                                                                )) {printf("char %" FMT16S " is not CHR_LESS_THAN                \n", CHR_LESS_THAN                ); exit(0);}
    if(strcmp(indexOfItems[CHR_EQUAL                    ].itemPrinted, STD_EQUAL                                                                    )) {printf("char %" FMT16S " is not CHR_EQUAL                    \n", CHR_EQUAL                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_GREATER_THAN             ].itemPrinted, STD_GREATER_THAN                                                             )) {printf("char %" FMT16S " is not CHR_GREATER_THAN             \n", CHR_GREATER_THAN             ); exit(0);}
    if(strcmp(indexOfItems[CHR_QUESTION_MARK            ].itemPrinted, STD_QUESTION_MARK                                                            )) {printf("char %" FMT16S " is not CHR_QUESTION_MARK            \n", CHR_QUESTION_MARK            ); exit(0);}
    if(strcmp(indexOfItems[CHR_AT                       ].itemPrinted, STD_AT                                                                       )) {printf("char %" FMT16S " is not CHR_AT                       \n", CHR_AT                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_LEFT_SQUARE_BRACKET      ].itemPrinted, STD_LEFT_SQUARE_BRACKET                                                      )) {printf("char %" FMT16S " is not CHR_LEFT_SQUARE_BRACKET      \n", CHR_LEFT_SQUARE_BRACKET      ); exit(0);}
    if(strcmp(indexOfItems[CHR_BACK_SLASH               ].itemPrinted, STD_BACK_SLASH                                                               )) {printf("char %" FMT16S " is not CHR_BACK_SLASH               \n", CHR_BACK_SLASH               ); exit(0);}
    if(strcmp(indexOfItems[CHR_RIGHT_SQUARE_BRACKET     ].itemPrinted, STD_RIGHT_SQUARE_BRACKET                                                     )) {printf("char %" FMT16S " is not CHR_RIGHT_SQUARE_BRACKET     \n", CHR_RIGHT_SQUARE_BRACKET     ); exit(0);}
    if(strcmp(indexOfItems[CHR_CIRCUMFLEX               ].itemPrinted, STD_CIRCUMFLEX                                                               )) {printf("char %" FMT16S " is not CHR_CIRCUMFLEX               \n", CHR_CIRCUMFLEX               ); exit(0);}
    if(strcmp(indexOfItems[CHR_UNDERSCORE               ].itemPrinted, STD_UNDERSCORE                                                               )) {printf("char %" FMT16S " is not CHR_UNDERSCORE               \n", CHR_UNDERSCORE               ); exit(0);}
    if(strcmp(indexOfItems[CHR_LEFT_CURLY_BRACKET       ].itemPrinted, STD_LEFT_CURLY_BRACKET                                                       )) {printf("char %" FMT16S " is not CHR_LEFT_CURLY_BRACKET       \n", CHR_LEFT_CURLY_BRACKET       ); exit(0);}
    if(strcmp(indexOfItems[CHR_PIPE                     ].itemPrinted, STD_PIPE                                                                     )) {printf("char %" FMT16S " is not CHR_PIPE                     \n", CHR_PIPE                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_RIGHT_CURLY_BRACKET      ].itemPrinted, STD_RIGHT_CURLY_BRACKET                                                      )) {printf("char %" FMT16S " is not CHR_RIGHT_CURLY_BBRACKET     \n", CHR_RIGHT_CURLY_BRACKET      ); exit(0);}
    if(strcmp(indexOfItems[CHR_TILDE                    ].itemPrinted, STD_TILDE                                                                    )) {printf("char %" FMT16S " is not CHR_TILDE                    \n", CHR_TILDE                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_INVERTED_EXCLAMATION_MARK].itemPrinted, STD_INVERTED_EXCLAMATION_MARK                                                )) {printf("char %" FMT16S " is not CHR_INVERTED_EXCLAMATION_MARK\n", CHR_INVERTED_EXCLAMATION_MARK); exit(0);}
    if(strcmp(indexOfItems[CHR_CENT                     ].itemPrinted, STD_CENT                                                                     )) {printf("char %" FMT16S " is not CHR_CENT                     \n", CHR_CENT                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_POUND                    ].itemPrinted, STD_POUND                                                                    )) {printf("char %" FMT16S " is not CHR_POUND                    \n", CHR_POUND                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_YEN                      ].itemPrinted, STD_YEN                                                                      )) {printf("char %" FMT16S " is not CHR_YEN                      \n", CHR_YEN                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_SECTION                  ].itemPrinted, STD_SECTION                                                                  )) {printf("char %" FMT16S " is not CHR_SECTION                  \n", CHR_SECTION                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_OVERFLOW_CARRY           ].itemPrinted, STD_OVERFLOW_CARRY                                                           )) {printf("char %" FMT16S " is not CHR_OVERFLOW_CARRY           \n", CHR_OVERFLOW_CARRY           ); exit(0);}
    if(strcmp(indexOfItems[CHR_LEFT_DOUBLE_ANGLE        ].itemPrinted, STD_LEFT_DOUBLE_ANGLE                                                        )) {printf("char %" FMT16S " is not CHR_LEFT_DOUBLE_ANGLE        \n", CHR_LEFT_DOUBLE_ANGLE        ); exit(0);}
    if(strcmp(indexOfItems[CHR_NOT                      ].itemPrinted, STD_NOT                                                                      )) {printf("char %" FMT16S " is not CHR_NOT                      \n", CHR_NOT                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_DEGREE                   ].itemPrinted, STD_DEGREE                                                                   )) {printf("char %" FMT16S " is not CHR_DEGREE                   \n", CHR_DEGREE                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_PLUS_MINUS               ].itemPrinted, STD_PLUS_MINUS                                                               )) {printf("char %" FMT16S " is not CHR_PLUS_MINUS               \n", CHR_PLUS_MINUS               ); exit(0);}
    if(strcmp(indexOfItems[CHR_mu_b                     ].itemPrinted, STD_mu_b                                                                     )) {printf("char %" FMT16S " is not CHR_mu_b                     \n", CHR_mu_b                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_DOT                      ].itemPrinted, STD_DOT                                                                      )) {printf("char %" FMT16S " is not CHR_DOT                      \n", CHR_DOT                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_ORDINAL                  ].itemPrinted, STD_ORDINAL                                                                  )) {printf("char %" FMT16S " is not CHR_ORDINAL                  \n", CHR_ORDINAL                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_RIGHT_DOUBLE_ANGLE       ].itemPrinted, STD_RIGHT_DOUBLE_ANGLE                                                       )) {printf("char %" FMT16S " is not CHR_RIGHT_DOUBLE_ANGLE       \n", CHR_RIGHT_DOUBLE_ANGLE       ); exit(0);}
    if(strcmp(indexOfItems[CHR_ONE_HALF                 ].itemPrinted, STD_ONE_HALF                                                                 )) {printf("char %" FMT16S " is not CHR_ONE_HALF                 \n", CHR_ONE_HALF                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_ONE_QUARTER              ].itemPrinted, STD_ONE_QUARTER                                                              )) {printf("char %" FMT16S " is not CHR_ONE_QUARTER              \n", CHR_ONE_QUARTER              ); exit(0);}
    if(strcmp(indexOfItems[CHR_INVERTED_QUESTION_MARK   ].itemPrinted, STD_INVERTED_QUESTION_MARK                                                   )) {printf("char %" FMT16S " is not CHR_INVERTED_QUESTION_MARK   \n", CHR_INVERTED_QUESTION_MARK   ); exit(0);}
    if(strcmp(indexOfItems[CHR_ETH                      ].itemPrinted, STD_ETH                                                                      )) {printf("char %" FMT16S " is not CHR_ETH                      \n", CHR_ETH                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_CROSS                    ].itemPrinted, STD_CROSS                                                                    )) {printf("char %" FMT16S " is not CHR_CROSS                    \n", CHR_CROSS                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_eth                      ].itemPrinted, STD_eth                                                                      )) {printf("char %" FMT16S " is not CHR_eth                      \n", CHR_eth                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_DIVIDE                   ].itemPrinted, STD_DIVIDE                                                                   )) {printf("char %" FMT16S " is not CHR_DIVIDE                   \n", CHR_DIVIDE                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_E_DOT                    ].itemPrinted, STD_E_DOT                                                                    )) {printf("char %" FMT16S " is not CHR_E_DOT                    \n", CHR_E_DOT                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_e_DOT                    ].itemPrinted, STD_e_DOT                                                                    )) {printf("char %" FMT16S " is not CHR_e_DOT                    \n", CHR_e_DOT                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_E_CARON                  ].itemPrinted, STD_E_CARON                                                                  )) {printf("char %" FMT16S " is not CHR_E_CARON                  \n", CHR_E_CARON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_e_CARON                  ].itemPrinted, STD_e_CARON                                                                  )) {printf("char %" FMT16S " is not CHR_e_CARON                  \n", CHR_e_CARON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_h_STROKE                 ].itemPrinted, STD_h_STROKE                                                                 )) {printf("char %" FMT16S " is not CHR_h_STROKE                 \n", CHR_h_STROKE                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_R_ACUTE                  ].itemPrinted, STD_R_ACUTE                                                                  )) {printf("char %" FMT16S " is not CHR_R_ACUTE                  \n", CHR_R_ACUTE                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_R_CARON                  ].itemPrinted, STD_R_CARON                                                                  )) {printf("char %" FMT16S " is not CHR_R_CARON                  \n", CHR_R_CARON                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_U_OGONEK                 ].itemPrinted, STD_U_OGONEK                                                                 )) {printf("char %" FMT16S " is not CHR_U_OGONEK                 \n", CHR_U_OGONEK                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_u_OGONEK                 ].itemPrinted, STD_u_OGONEK                                                                 )) {printf("char %" FMT16S " is not CHR_u_OGONEK                 \n", CHR_u_OGONEK                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_y_UNDER_ROOT             ].itemPrinted, STD_y_UNDER_ROOT                                                             )) {printf("char %" FMT16S " is not CHR_y_UNDER_ROOT             \n", CHR_y_UNDER_ROOT             ); exit(0);}
    if(strcmp(indexOfItems[CHR_x_UNDER_ROOT             ].itemPrinted, STD_x_UNDER_ROOT                                                             )) {printf("char %" FMT16S " is not CHR_x_UNDER_ROOT             \n", CHR_x_UNDER_ROOT             ); exit(0);}
    if(strcmp(indexOfItems[CHR_SPACE_EM                 ].itemPrinted, STD_SPACE_EM                                                                 )) {printf("char %" FMT16S " is not CHR_SPACE_EM                 \n", CHR_SPACE_EM                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_SPACE_3_PER_EM           ].itemPrinted, STD_SPACE_3_PER_EM                                                           )) {printf("char %" FMT16S " is not CHR_SPACE_3_PER_EM           \n", CHR_SPACE_3_PER_EM           ); exit(0);}
    if(strcmp(indexOfItems[CHR_SPACE_4_PER_EM           ].itemPrinted, STD_SPACE_4_PER_EM                                                           )) {printf("char %" FMT16S " is not CHR_SPACE_4_PER_EM           \n", CHR_SPACE_4_PER_EM           ); exit(0);}
    if(strcmp(indexOfItems[CHR_SPACE_6_PER_EM           ].itemPrinted, STD_SPACE_6_PER_EM                                                           )) {printf("char %" FMT16S " is not CHR_SPACE_6_PER_EM           \n", CHR_SPACE_6_PER_EM           ); exit(0);}
    if(strcmp(indexOfItems[CHR_SPACE_FIGURE             ].itemPrinted, STD_SPACE_FIGURE                                                             )) {printf("char %" FMT16S " is not CHR_SPACE_FIGURE             \n", CHR_SPACE_FIGURE             ); exit(0);}
    if(strcmp(indexOfItems[CHR_SPACE_PUNCTUATION        ].itemPrinted, STD_SPACE_PUNCTUATION                                                        )) {printf("char %" FMT16S " is not CHR_SPACE_PUNCTUATION        \n", CHR_SPACE_PUNCTUATION        ); exit(0);}
    if(strcmp(indexOfItems[CHR_SPACE_HAIR               ].itemPrinted, STD_SPACE_HAIR                                                               )) {printf("char %" FMT16S " is not CHR_SPACE_HAIR               \n", CHR_SPACE_HAIR               ); exit(0);}
    if(strcmp(indexOfItems[CHR_LEFT_SINGLE_QUOTE        ].itemPrinted, STD_LEFT_SINGLE_QUOTE                                                        )) {printf("char %" FMT16S " is not CHR_LEFT_SINGLE_QUOTE        \n", CHR_LEFT_SINGLE_QUOTE        ); exit(0);}
    if(strcmp(indexOfItems[CHR_RIGHT_SINGLE_QUOTE       ].itemPrinted, STD_RIGHT_SINGLE_QUOTE                                                       )) {printf("char %" FMT16S " is not CHR_RIGHT_SINGLE_QUOTE       \n", CHR_RIGHT_SINGLE_QUOTE       ); exit(0);}
    if(strcmp(indexOfItems[CHR_SINGLE_LOW_QUOTE         ].itemPrinted, STD_SINGLE_LOW_QUOTE                                                         )) {printf("char %" FMT16S " is not CHR_SINGLE_LOW_QUOTE         \n", CHR_SINGLE_LOW_QUOTE         ); exit(0);}
    if(strcmp(indexOfItems[CHR_SINGLE_HIGH_QUOTE        ].itemPrinted, STD_SINGLE_HIGH_QUOTE                                                        )) {printf("char %" FMT16S " is not CHR_SINGLE_HIGH_QUOTE        \n", CHR_SINGLE_HIGH_QUOTE        ); exit(0);}
    if(strcmp(indexOfItems[CHR_LEFT_DOUBLE_QUOTE        ].itemPrinted, STD_LEFT_DOUBLE_QUOTE                                                        )) {printf("char %" FMT16S " is not CHR_LEFT_DOUBLE_QUOTE        \n", CHR_LEFT_DOUBLE_QUOTE        ); exit(0);}
    if(strcmp(indexOfItems[CHR_RIGHT_DOUBLE_QUOTE       ].itemPrinted, STD_RIGHT_DOUBLE_QUOTE                                                       )) {printf("char %" FMT16S " is not CHR_RIGHT_DOUBLE_QUOTE       \n", CHR_RIGHT_DOUBLE_QUOTE       ); exit(0);}
    if(strcmp(indexOfItems[CHR_DOUBLE_LOW_QUOTE         ].itemPrinted, STD_DOUBLE_LOW_QUOTE                                                         )) {printf("char %" FMT16S " is not CHR_DOUBLE_LOW_QUOTE         \n", CHR_DOUBLE_LOW_QUOTE         ); exit(0);}
    if(strcmp(indexOfItems[CHR_DOUBLE_HIGH_QUOTE        ].itemPrinted, STD_DOUBLE_HIGH_QUOTE                                                        )) {printf("char %" FMT16S " is not CHR_DOUBLE_HIGH_QUOTE        \n", CHR_DOUBLE_HIGH_QUOTE        ); exit(0);}
    if(strcmp(indexOfItems[CHR_ELLIPSIS                 ].itemPrinted, STD_ELLIPSIS                                                                 )) {printf("char %" FMT16S " is not CHR_ELLIPSIS                 \n", CHR_ELLIPSIS                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_ONE                      ].itemPrinted, STD_ONE                                                                      )) {printf("char %" FMT16S " is not CHR_ONE                      \n", CHR_ONE                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_EURO                     ].itemPrinted, STD_EURO                                                                     )) {printf("char %" FMT16S " is not CHR_EURO                     \n", CHR_EURO                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_COMPLEX_C                ].itemPrinted, STD_COMPLEX_C                                                                )) {printf("char %" FMT16S " is not CHR_COMPLEX_C                \n", CHR_COMPLEX_C                ); exit(0);}
    if(strcmp(indexOfItems[CHR_PLANCK                   ].itemPrinted, STD_PLANCK                                                                   )) {printf("char %" FMT16S " is not CHR_PLANCK                   \n", CHR_PLANCK                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_PLANCK_2PI               ].itemPrinted, STD_PLANCK_2PI                                                               )) {printf("char %" FMT16S " is not CHR_PLANCK_2PI               \n", CHR_PLANCK_2PI               ); exit(0);}
    if(strcmp(indexOfItems[CHR_NATURAL_N                ].itemPrinted, STD_NATURAL_N                                                                )) {printf("char %" FMT16S " is not CHR_NATURAL_N                \n", CHR_NATURAL_N                ); exit(0);}
    if(strcmp(indexOfItems[CHR_RATIONAL_Q               ].itemPrinted, STD_RATIONAL_Q                                                               )) {printf("char %" FMT16S " is not CHR_RATIONAL_Q               \n", CHR_RATIONAL_Q               ); exit(0);}
    if(strcmp(indexOfItems[CHR_REAL_R                   ].itemPrinted, STD_REAL_R                                                                   )) {printf("char %" FMT16S " is not CHR_REAL_R                   \n", CHR_REAL_R                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_LEFT_ARROW               ].itemPrinted, STD_LEFT_ARROW                                                               )) {printf("char %" FMT16S " is not CHR_LEFT_ARROW               \n", CHR_LEFT_ARROW               ); exit(0);}
    if(strcmp(indexOfItems[CHR_UP_ARROW                 ].itemPrinted, STD_UP_ARROW                                                                 )) {printf("char %" FMT16S " is not CHR_UP_ARROW                 \n", CHR_UP_ARROW                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_UP_ARROW_b               ].itemPrinted, STD_UP_ARROW_b                                                               )) {printf("char %" FMT16S " is not CHR_UP_ARROW_b               \n", CHR_UP_ARROW_b               ); exit(0);}
    if(strcmp(indexOfItems[CHR_RIGHT_ARROW              ].itemPrinted, STD_RIGHT_ARROW                                                              )) {printf("char %" FMT16S " is not CHR_RIGHT_ARROW              \n", CHR_RIGHT_ARROW              ); exit(0);}
    if(strcmp(indexOfItems[CHR_DOWN_ARROW               ].itemPrinted, STD_DOWN_ARROW                                                               )) {printf("char %" FMT16S " is not CHR_DOWN_ARROW               \n", CHR_DOWN_ARROW               ); exit(0);}
    if(strcmp(indexOfItems[CHR_DOWN_ARROW_b             ].itemPrinted, STD_DOWN_ARROW_b                                                             )) {printf("char %" FMT16S " is not CHR_DOWN_ARROW_b             \n", CHR_DOWN_ARROW_b             ); exit(0);}
    if(strcmp(indexOfItems[CHR_SERIAL_IO                ].itemPrinted, STD_SERIAL_IO                                                                )) {printf("char %" FMT16S " is not CHR_SERIAL_IO                \n", CHR_SERIAL_IO                ); exit(0);}
    if(strcmp(indexOfItems[CHR_RIGHT_SHORT_ARROW        ].itemPrinted, STD_RIGHT_SHORT_ARROW                                                        )) {printf("char %" FMT16S " is not CHR_RIGHT_SHORT_ARROW        \n", CHR_RIGHT_SHORT_ARROW        ); exit(0);}
    if(strcmp(indexOfItems[CHR_LEFT_RIGHT_ARROWS        ].itemPrinted, STD_LEFT_RIGHT_ARROWS                                                        )) {printf("char %" FMT16S " is not CHR_LEFT_RIGHT_ARROWS        \n", CHR_LEFT_RIGHT_ARROWS        ); exit(0);}
    if(strcmp(indexOfItems[CHR_BST                      ].itemPrinted, STD_BST                                                                      )) {printf("char %" FMT16S " is not CHR_BST                      \n", CHR_BST                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_SST                      ].itemPrinted, STD_SST                                                                      )) {printf("char %" FMT16S " is not CHR_SST                      \n", CHR_SST                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_HAMBURGER                ].itemPrinted, STD_HAMBURGER                                                                )) {printf("char %" FMT16S " is not CHR_HAMBURGER                \n", CHR_HAMBURGER                ); exit(0);}
    if(strcmp(indexOfItems[CHR_UNDO                     ].itemPrinted, STD_UNDO                                                                     )) {printf("char %" FMT16S " is not CHR_UNDO                     \n", CHR_UNDO                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_FOR_ALL                  ].itemPrinted, STD_FOR_ALL                                                                  )) {printf("char %" FMT16S " is not CHR_FOR_ALL                  \n", CHR_FOR_ALL                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_COMPLEMENT               ].itemPrinted, STD_COMPLEMENT                                                               )) {printf("char %" FMT16S " is not CHR_COMPLEMENT               \n", CHR_COMPLEMENT               ); exit(0);}
    if(strcmp(indexOfItems[CHR_PARTIAL_DIFF             ].itemPrinted, STD_PARTIAL_DIFF                                                             )) {printf("char %" FMT16S " is not CHR_PARTIAL_DIFF             \n", CHR_PARTIAL_DIFF             ); exit(0);}
    if(strcmp(indexOfItems[CHR_THERE_EXISTS             ].itemPrinted, STD_THERE_EXISTS                                                             )) {printf("char %" FMT16S " is not CHR_THERE_EXISTS             \n", CHR_THERE_EXISTS             ); exit(0);}
    if(strcmp(indexOfItems[CHR_THERE_DOES_NOT_EXIST     ].itemPrinted, STD_THERE_DOES_NOT_EXIST                                                     )) {printf("char %" FMT16S " is not CHR_THERE_DOES_NOT_EXIST     \n", CHR_THERE_DOES_NOT_EXIST     ); exit(0);}
    if(strcmp(indexOfItems[CHR_EMPTY_SET                ].itemPrinted, STD_EMPTY_SET                                                                )) {printf("char %" FMT16S " is not CHR_EMPTY_SET                \n", CHR_EMPTY_SET                ); exit(0);}
    if(strcmp(indexOfItems[CHR_INCREMENT                ].itemPrinted, STD_INCREMENT                                                                )) {printf("char %" FMT16S " is not CHR_INCREMENT                \n", CHR_INCREMENT                ); exit(0);}
    if(strcmp(indexOfItems[CHR_NABLA                    ].itemPrinted, STD_NABLA                                                                    )) {printf("char %" FMT16S " is not CHR_NABLA                    \n", CHR_NABLA                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_ELEMENT_OF               ].itemPrinted, STD_ELEMENT_OF                                                               )) {printf("char %" FMT16S " is not CHR_ELEMENT_OF               \n", CHR_ELEMENT_OF               ); exit(0);}
    if(strcmp(indexOfItems[CHR_NOT_ELEMENT_OF           ].itemPrinted, STD_NOT_ELEMENT_OF                                                           )) {printf("char %" FMT16S " is not CHR_NOT_ELEMENT_OF           \n", CHR_NOT_ELEMENT_OF           ); exit(0);}
    if(strcmp(indexOfItems[CHR_CONTAINS                 ].itemPrinted, STD_CONTAINS                                                                 )) {printf("char %" FMT16S " is not CHR_CONTAINS                 \n", CHR_CONTAINS                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_DOES_NOT_CONTAIN         ].itemPrinted, STD_DOES_NOT_CONTAIN                                                         )) {printf("char %" FMT16S " is not CHR_DOES_NOT_CONTAIN         \n", CHR_DOES_NOT_CONTAIN         ); exit(0);}
    if(strcmp(indexOfItems[CHR_ZERO                     ].itemPrinted, STD_ZERO                                                                     )) {printf("char %" FMT16S " is not CHR_ZERO                     \n", CHR_ZERO                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_PRODUCT                  ].itemPrinted, STD_PRODUCT                                                                  )) {printf("char %" FMT16S " is not CHR_PRODUCT                  \n", CHR_PRODUCT                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUM                      ].itemPrinted, STD_SUM                                                                      )) {printf("char %" FMT16S " is not CHR_SUM                      \n", CHR_SUM                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_MINUS_SIGN               ].itemPrinted, STD_MINUS_SIGN                                                               )) {printf("char %" FMT16S " is not CHR_MINUS_SIGN               \n", CHR_MINUS_SIGN               ); exit(0);}
    if(strcmp(indexOfItems[CHR_MINUS_PLUS               ].itemPrinted, STD_MINUS_PLUS                                                               )) {printf("char %" FMT16S " is not CHR_MINUS_PLUS               \n", CHR_MINUS_PLUS               ); exit(0);}
    if(strcmp(indexOfItems[CHR_DIVISION                 ].itemPrinted, STD_DIVISION                                                                 )) {printf("char %" FMT16S " is not CHR_DIVISION                 \n", CHR_DIVISION                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_SET_MINUS                ].itemPrinted, STD_SET_MINUS                                                                )) {printf("char %" FMT16S " is not CHR_SET_MINUS                \n", CHR_SET_MINUS                ); exit(0);}
    if(strcmp(indexOfItems[CHR_RING                     ].itemPrinted, STD_RING                                                                     )) {printf("char %" FMT16S " is not CHR_RING                     \n", CHR_RING                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_BULLET                   ].itemPrinted, STD_BULLET                                                                   )) {printf("char %" FMT16S " is not CHR_BULLET                   \n", CHR_BULLET                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_SQUARE_ROOT              ].itemPrinted, STD_SQUARE_ROOT                                                              )) {printf("char %" FMT16S " is not CHR_SQUARE_ROOT              \n", CHR_SQUARE_ROOT              ); exit(0);}
    if(strcmp(indexOfItems[CHR_CUBIC_ROOT               ].itemPrinted, STD_CUBIC_ROOT                                                               )) {printf("char %" FMT16S " is not CHR_CUBIC_ROOT               \n", CHR_CUBIC_ROOT               ); exit(0);}
    if(strcmp(indexOfItems[CHR_xTH_ROOT                 ].itemPrinted, STD_xTH_ROOT                                                                 )) {printf("char %" FMT16S " is not CHR_xTH_ROOT                 \n", CHR_xTH_ROOT                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_PROPORTIONAL             ].itemPrinted, STD_PROPORTIONAL                                                             )) {printf("char %" FMT16S " is not CHR_PROPORTIONAL             \n", CHR_PROPORTIONAL             ); exit(0);}
    if(strcmp(indexOfItems[CHR_INFINITY                 ].itemPrinted, STD_INFINITY                                                                 )) {printf("char %" FMT16S " is not CHR_INFINITY                 \n", CHR_INFINITY                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_RIGHT_ANGLE              ].itemPrinted, STD_RIGHT_ANGLE                                                              )) {printf("char %" FMT16S " is not CHR_RIGHT_ANGLE              \n", CHR_RIGHT_ANGLE              ); exit(0);}
    if(strcmp(indexOfItems[CHR_ANGLE                    ].itemPrinted, STD_ANGLE                                                                    )) {printf("char %" FMT16S " is not CHR_ANGLE                    \n", CHR_ANGLE                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_MEASURED_ANGLE           ].itemPrinted, STD_MEASURED_ANGLE                                                           )) {printf("char %" FMT16S " is not CHR_MEASURED_ANGLE           \n", CHR_MEASURED_ANGLE           ); exit(0);}
    if(strcmp(indexOfItems[CHR_DIVIDES                  ].itemPrinted, STD_DIVIDES                                                                  )) {printf("char %" FMT16S " is not CHR_DIVIDES                  \n", CHR_DIVIDES                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_DOES_NOT_DIVIDE          ].itemPrinted, STD_DOES_NOT_DIVIDE                                                          )) {printf("char %" FMT16S " is not CHR_DOES_NOT_DIVIDE          \n", CHR_DOES_NOT_DIVIDE          ); exit(0);}
    if(strcmp(indexOfItems[CHR_PARALLEL                 ].itemPrinted, STD_PARALLEL                                                                 )) {printf("char %" FMT16S " is not CHR_PARALLEL                 \n", CHR_PARALLEL                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_NOT_PARALLEL             ].itemPrinted, STD_NOT_PARALLEL                                                             )) {printf("char %" FMT16S " is not CHR_NOT_PARALLEL             \n", CHR_NOT_PARALLEL             ); exit(0);}
    if(strcmp(indexOfItems[CHR_AND                      ].itemPrinted, STD_AND                                                                      )) {printf("char %" FMT16S " is not CHR_AND                      \n", CHR_AND                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_OR                       ].itemPrinted, STD_OR                                                                       )) {printf("char %" FMT16S " is not CHR_OR                       \n", CHR_OR                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_INTERSECTION             ].itemPrinted, STD_INTERSECTION                                                             )) {printf("char %" FMT16S " is not CHR_INTERSECTION             \n", CHR_INTERSECTION             ); exit(0);}
    if(strcmp(indexOfItems[CHR_UNION                    ].itemPrinted, STD_UNION                                                                    )) {printf("char %" FMT16S " is not CHR_UNION                    \n", CHR_UNION                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_INTEGRAL                 ].itemPrinted, STD_INTEGRAL                                                                 )) {printf("char %" FMT16S " is not CHR_INTEGRAL                 \n", CHR_INTEGRAL                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_DOUBLE_INTEGRAL          ].itemPrinted, STD_DOUBLE_INTEGRAL                                                          )) {printf("char %" FMT16S " is not CHR_DOUBLE_INTEGRAL          \n", CHR_DOUBLE_INTEGRAL          ); exit(0);}
    if(strcmp(indexOfItems[CHR_CONTOUR_INTEGRAL         ].itemPrinted, STD_CONTOUR_INTEGRAL                                                         )) {printf("char %" FMT16S " is not CHR_CONTOUR_INTEGRAL         \n", CHR_CONTOUR_INTEGRAL         ); exit(0);}
    if(strcmp(indexOfItems[CHR_SURFACE_INTEGRAL         ].itemPrinted, STD_SURFACE_INTEGRAL                                                         )) {printf("char %" FMT16S " is not CHR_SURFACE_INTEGRAL         \n", CHR_SURFACE_INTEGRAL         ); exit(0);}
    if(strcmp(indexOfItems[CHR_RATIO                    ].itemPrinted, STD_RATIO                                                                    )) {printf("char %" FMT16S " is not CHR_RATIO                    \n", CHR_RATIO                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_TILDE_b                  ].itemPrinted, STD_TILDE_b                                                                  )) {printf("char %" FMT16S " is not CHR_TILDE_b                  \n", CHR_TILDE_b                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_ASYMPOTICALLY_EQUAL      ].itemPrinted, STD_ASYMPOTICALLY_EQUAL                                                      )) {printf("char %" FMT16S " is not CHR_ASYMPOTICALLY_EQUAL      \n", CHR_ASYMPOTICALLY_EQUAL      ); exit(0);}
    if(strcmp(indexOfItems[CHR_ALMOST_EQUAL             ].itemPrinted, STD_ALMOST_EQUAL                                                             )) {printf("char %" FMT16S " is not CHR_ALMOST_EQUAL             \n", CHR_ALMOST_EQUAL             ); exit(0);}
    if(strcmp(indexOfItems[CHR_COLON_EQUALS             ].itemPrinted, STD_COLON_EQUALS                                                             )) {printf("char %" FMT16S " is not CHR_COLON_EQUALS             \n", CHR_COLON_EQUALS             ); exit(0);}
    if(strcmp(indexOfItems[CHR_CORRESPONDS_TO           ].itemPrinted, STD_CORRESPONDS_TO                                                           )) {printf("char %" FMT16S " is not CHR_CORRESPONDS_TO           \n", CHR_CORRESPONDS_TO           ); exit(0);}
    if(strcmp(indexOfItems[CHR_ESTIMATES                ].itemPrinted, STD_ESTIMATES                                                                )) {printf("char %" FMT16S " is not CHR_ESTIMATES                \n", CHR_ESTIMATES                ); exit(0);}
    if(strcmp(indexOfItems[CHR_NOT_EQUAL                ].itemPrinted, STD_NOT_EQUAL                                                                )) {printf("char %" FMT16S " is not CHR_NOT_EQUAL                \n", CHR_NOT_EQUAL                ); exit(0);}
    if(strcmp(indexOfItems[CHR_IDENTICAL_TO             ].itemPrinted, STD_IDENTICAL_TO                                                             )) {printf("char %" FMT16S " is not CHR_IDENTICAL_TO             \n", CHR_IDENTICAL_TO             ); exit(0);}
    if(strcmp(indexOfItems[CHR_LESS_EQUAL               ].itemPrinted, STD_LESS_EQUAL                                                               )) {printf("char %" FMT16S " is not CHR_LESS_EQUAL               \n", CHR_LESS_EQUAL               ); exit(0);}
    if(strcmp(indexOfItems[CHR_GREATER_EQUAL            ].itemPrinted, STD_GREATER_EQUAL                                                            )) {printf("char %" FMT16S " is not CHR_GREATER_EQUAL            \n", CHR_GREATER_EQUAL            ); exit(0);}
    if(strcmp(indexOfItems[CHR_MUCH_LESS                ].itemPrinted, STD_MUCH_LESS                                                                )) {printf("char %" FMT16S " is not CHR_MUCH_LESS                \n", CHR_MUCH_LESS                ); exit(0);}
    if(strcmp(indexOfItems[CHR_MUCH_GREATER             ].itemPrinted, STD_MUCH_GREATER                                                             )) {printf("char %" FMT16S " is not CHR_MUCH_GREATER             \n", CHR_MUCH_GREATER             ); exit(0);}
    if(strcmp(indexOfItems[CHR_SUN                      ].itemPrinted, STD_SUN                                                                      )) {printf("char %" FMT16S " is not CHR_SUN                      \n", CHR_SUN                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_DOWN_TACK                ].itemPrinted, STD_DOWN_TACK                                                                )) {printf("char %" FMT16S " is not CHR_DOWN_TACK                \n", CHR_DOWN_TACK                ); exit(0);}
    if(strcmp(indexOfItems[CHR_UP_TACK                  ].itemPrinted, STD_UP_TACK                                                                  )) {printf("char %" FMT16S " is not CHR_UP_TACK                  \n", CHR_UP_TACK                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_XOR                      ].itemPrinted, STD_XOR                                                                      )) {printf("char %" FMT16S " is not CHR_XOR                      \n", CHR_XOR                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_NAND                     ].itemPrinted, STD_NAND                                                                     )) {printf("char %" FMT16S " is not CHR_NAND                     \n", CHR_NAND                     ); exit(0);}
    if(strcmp(indexOfItems[CHR_NOR                      ].itemPrinted, STD_NOR                                                                      )) {printf("char %" FMT16S " is not CHR_NOR                      \n", CHR_NOR                      ); exit(0);}
    if(strcmp(indexOfItems[CHR_WATCH                    ].itemPrinted, STD_WATCH                                                                    )) {printf("char %" FMT16S " is not CHR_WATCH                    \n", CHR_WATCH                    ); exit(0);}
    if(strcmp(indexOfItems[CHR_HOURGLASS                ].itemPrinted, STD_HOURGLASS                                                                )) {printf("char %" FMT16S " is not CHR_HOURGLASS                \n", CHR_HOURGLASS                ); exit(0);}
    if(strcmp(indexOfItems[CHR_PRINTER                  ].itemPrinted, STD_PRINTER                                                                  )) {printf("char %" FMT16S " is not CHR_PRINTER                  \n", CHR_PRINTER                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_MAT_TL                   ].itemPrinted, STD_MAT_TL                                                                   )) {printf("char %" FMT16S " is not CHR_MAT_TL                   \n", CHR_MAT_TL                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_MAT_ML                   ].itemPrinted, STD_MAT_ML                                                                   )) {printf("char %" FMT16S " is not CHR_MAT_ML                   \n", CHR_MAT_ML                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_MAT_BL                   ].itemPrinted, STD_MAT_BL                                                                   )) {printf("char %" FMT16S " is not CHR_MAT_BL                   \n", CHR_MAT_BL                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_MAT_TR                   ].itemPrinted, STD_MAT_TR                                                                   )) {printf("char %" FMT16S " is not CHR_MAT_TR                   \n", CHR_MAT_TR                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_MAT_MR                   ].itemPrinted, STD_MAT_MR                                                                   )) {printf("char %" FMT16S " is not CHR_MAT_MR                   \n", CHR_MAT_MR                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_MAT_BR                   ].itemPrinted, STD_MAT_BR                                                                   )) {printf("char %" FMT16S " is not CHR_MAT_BR                   \n", CHR_MAT_BR                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_OBLIQUE1                 ].itemPrinted, STD_OBLIQUE1                                                                 )) {printf("char %" FMT16S " is not CHR_OBLIQUE1                 \n", CHR_OBLIQUE1                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_OBLIQUE2                 ].itemPrinted, STD_OBLIQUE2                                                                 )) {printf("char %" FMT16S " is not CHR_OBLIQUE2                 \n", CHR_OBLIQUE2                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_OBLIQUE3                 ].itemPrinted, STD_OBLIQUE3                                                                 )) {printf("char %" FMT16S " is not CHR_OBLIQUE3                 \n", CHR_OBLIQUE3                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_OBLIQUE4                 ].itemPrinted, STD_OBLIQUE4                                                                 )) {printf("char %" FMT16S " is not CHR_OBLIQUE4                 \n", CHR_OBLIQUE4                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_CURSOR                   ].itemPrinted, STD_CURSOR                                                                   )) {printf("char %" FMT16S " is not CHR_CURSOR                   \n", CHR_CURSOR                   ); exit(0);}
    if(strcmp(indexOfItems[CHR_PERIOD34                 ].itemPrinted, STD_PERIOD34                                                                 )) {printf("char %" FMT16S " is not CHR_PERIOD34                 \n", CHR_PERIOD34                 ); exit(0);}
    if(strcmp(indexOfItems[CHR_COMMA34                  ].itemPrinted, STD_COMMA34                                                                  )) {printf("char %" FMT16S " is not CHR_COMMA34                  \n", CHR_COMMA34                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_BATTERY                  ].itemPrinted, STD_BATTERY                                                                  )) {printf("char %" FMT16S " is not CHR_BATTERY                  \n", CHR_BATTERY                  ); exit(0);}
    if(strcmp(indexOfItems[CHR_PGM_BEGIN                ].itemPrinted, STD_PGM_BEGIN                                                                )) {printf("char %" FMT16S " is not CHR_PGM_BEGIN                \n", CHR_PGM_BEGIN                ); exit(0);}
    if(strcmp(indexOfItems[CHR_USER_MODE                ].itemPrinted, STD_USER_MODE                                                                )) {printf("char %" FMT16S " is not CHR_USER_MODE                \n", CHR_USER_MODE                ); exit(0);}
    if(strcmp(indexOfItems[CHR_UK                       ].itemPrinted, STD_UK                                                                       )) {printf("char %" FMT16S " is not CHR_UK                       \n", CHR_UK                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_US                       ].itemPrinted, STD_US                                                                       )) {printf("char %" FMT16S " is not CHR_US                       \n", CHR_US                       ); exit(0);}
    if(strcmp(indexOfItems[CHR_NEG_EXCLAMATION_MARK     ].itemPrinted, STD_NEG_EXCLAMATION_MARK                                                     )) {printf("char %" FMT16S " is not CHR_NEG_EXCLAMATION_MARK     \n", CHR_NEG_EXCLAMATION_MARK     ); exit(0);}
    if(strcmp(indexOfItems[ITM_Max                      ].itemPrinted, "Max"                                                                        )) {printf("item %" FMT16S " is not ITM_Max                      \n", ITM_Max                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_Min                      ].itemPrinted, "Min"                                                                        )) {printf("item %" FMT16S " is not ITM_Min                      \n", ITM_Min                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_Config                   ].itemPrinted, "Config"                                                                     )) {printf("item %" FMT16S " is not ITM_Config                   \n", ITM_Config                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_Stack                    ].itemPrinted, "Stack"                                                                      )) {printf("item %" FMT16S " is not ITM_Stack                    \n", ITM_Stack                    ); exit(0);}
    if(strcmp(indexOfItems[ITM_0P                       ].itemPrinted, "0."                                                                         )) {printf("item %" FMT16S " is not ITM_0P                       \n", ITM_0P                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_1P                       ].itemPrinted, "1."                                                                         )) {printf("item %" FMT16S " is not ITM_1P                       \n", ITM_1P                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_EXPONENT                 ].itemPrinted, STD_SUB_E_OUTLINE                                                            )) {printf("item %" FMT16S " is not ITM_EXPONENT                 \n", ITM_EXPONENT                 ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA11                     ].itemPrinted, STD_a_DIARESIS                                                               )) {printf("item %" FMT16S " is not ITM_MA11                     \n", ITM_MA11                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA12                     ].itemPrinted, STD_o_DIARESIS                                                               )) {printf("item %" FMT16S " is not ITM_MA12                     \n", ITM_MA12                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA13                     ].itemPrinted, STD_u_DIARESIS                                                               )) {printf("item %" FMT16S " is not ITM_MA13                     \n", ITM_MA13                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA14                     ].itemPrinted, ""                                                                           )) {printf("item %" FMT16S " is not ITM_MA14                     \n", ITM_MA14                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA15                     ].itemPrinted, "sch"                                                                        )) {printf("item %" FMT16S " is not ITM_MA15                     \n", ITM_MA15                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA16                     ].itemPrinted, STD_s_SHARP                                                                  )) {printf("item %" FMT16S " is not ITM_MA16                     \n", ITM_MA16                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA21                     ].itemPrinted, "not"                                                                        )) {printf("item %" FMT16S " is not ITM_MA21                     \n", ITM_MA21                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA22                     ].itemPrinted, "edi-"                                                                       )) {printf("item %" FMT16S " is not ITM_MA22                     \n", ITM_MA22                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA23                     ].itemPrinted, "table"                                                                      )) {printf("item %" FMT16S " is not ITM_MA23                     \n", ITM_MA23                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA24                     ].itemPrinted, "for"                                                                        )) {printf("item %" FMT16S " is not ITM_MA24                     \n", ITM_MA24                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA25                     ].itemPrinted, "now"                                                                        )) {printf("item %" FMT16S " is not ITM_MA25                     \n", ITM_MA25                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA26                     ].itemPrinted, ""                                                                           )) {printf("item %" FMT16S " is not ITM_MA26                     \n", ITM_MA26                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA31                     ].itemPrinted, ""                                                                           )) {printf("item %" FMT16S " is not ITM_MA31                     \n", ITM_MA31                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA32                     ].itemPrinted, ""                                                                           )) {printf("item %" FMT16S " is not ITM_MA32                     \n", ITM_MA32                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA33                     ].itemPrinted, ""                                                                           )) {printf("item %" FMT16S " is not ITM_MA33                     \n", ITM_MA33                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA34                     ].itemPrinted, ""                                                                           )) {printf("item %" FMT16S " is not ITM_MA34                     \n", ITM_MA34                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA35                     ].itemPrinted, ""                                                                           )) {printf("item %" FMT16S " is not ITM_MA35                     \n", ITM_MA35                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_MA36                     ].itemPrinted, ""                                                                           )) {printf("item %" FMT16S " is not ITM_MA36                     \n", ITM_MA36                     ); exit(0);}
    if(strcmp(indexOfItems[KEY_UNDO                     ].itemPrinted, STD_UNDO                                                                     )) {printf("item %" FMT16S " is not KEY_UNDO                     \n", KEY_UNDO                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_PR                       ].itemPrinted, "P/R"                                                                        )) {printf("item %" FMT16S " is not ITM_PR                       \n", ITM_PR                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_RS                       ].itemPrinted, "R/S"                                                                        )) {printf("item %" FMT16S " is not ITM_RS                       \n", ITM_RS                       ); exit(0);}
    if(strcmp(indexOfItems[ITM_Not                      ].itemPrinted, "Not"                                                                        )) {printf("item %" FMT16S " is not ITM_Not                      \n", ITM_Not                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_yet                      ].itemPrinted, "yet"                                                                        )) {printf("item %" FMT16S " is not ITM_yet                      \n", ITM_yet                      ); exit(0);}
    if(strcmp(indexOfItems[ITM_defined                  ].itemPrinted, "defined"                                                                    )) {printf("item %" FMT16S " is not ITM_defined                  \n", ITM_defined                  ); exit(0);}
    if(strcmp(indexOfItems[MNU_TAM                      ].itemPrinted, "Tam"                                                                        )) {printf("menu %" FMT16S " is not MNU_TAM                      \n", MNU_TAM                      ); exit(0);}
    if(strcmp(indexOfItems[MNU_TAMCMP                   ].itemPrinted, "TamCmp"                                                                     )) {printf("menu %" FMT16S " is not MNU_TAMCMP                   \n", MNU_TAMCMP                   ); exit(0);}
    if(strcmp(indexOfItems[MNU_TAMSTORCL                ].itemPrinted, "TamStoRcl"                                                                  )) {printf("menu %" FMT16S " is not MNU_TAMSTORCL                \n", MNU_TAMSTORCL                ); exit(0);}
    if(strcmp(indexOfItems[ITM_QTtoM3                   ].itemPrinted, "qt." STD_RIGHT_ARROW "m" STD_SUP_3                                          )) {printf("menu %" FMT16S " is not ITM_QTtoM3                   \n", ITM_QTtoM3                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_M3toQT                   ].itemPrinted, "m" STD_SUP_3 STD_RIGHT_ARROW "qt."                                          )) {printf("menu %" FMT16S " is not ITM_M3toQT                   \n", ITM_M3toQT                   ); exit(0);}
    if(strcmp(indexOfItems[ITM_toSP                     ].itemPrinted, STD_RIGHT_ARROW "SP"                                                         )) {printf("item %" FMT16S " is not ITM_toSP                     \n", ITM_toSP                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_SHOW                     ].itemPrinted, "SHOW"                                                                       )) {printf("item %" FMT16S " is not ITM_SHOW                     \n", ITM_SHOW                     ); exit(0);}
    if(strcmp(indexOfItems[ITM_SYSTEM                   ].itemPrinted, "SYSTEM"                                                                     )) {printf("item %" FMT16S " is not ITM_SYSTEM                   \n", ITM_SYSTEM                   ); exit(0);}
    if(strcmp(indexOfItems[LAST_ITEM                    ].itemPrinted, "Last item"                                                                  )) {printf("item %" FMT16S " is not LAST_ITEM                    \n", LAST_ITEM                    ); exit(0);}
  #endif




  //**********************************
  //* addition.c                     *
  //**********************************
  #ifdef PC_BUILD
    printf("  addition.c\n");
  #endif

  initializeRegister(REGISTER_Y, dtReal16,    "-10", "",   0);
  initializeRegister(REGISTER_X, dtComplex34, "-2",  "-4", 0);
  fnAdd(NOPARAM);
  registerShouldBe(REGISTER_X, dtComplex34, "addRe16Co34()", "-12", "-4", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "addRe16Co34()", "-2",  "-4", 0);

  op1    = allocateTemporaryRegister();
  op2    = allocateTemporaryRegister();
  result = allocateTemporaryRegister();

  initializeRegister(op1, dtBigInteger,  "546854254695442521456456845658456584654565455256455456545652564554556455425545", "", 0);
  initializeRegister(op2, dtBigInteger, "-231053122121212121212121212121212121212121002102102102121331344321032103312220", "", 0);
  addBigIBigI();
  registerShouldBe(result, dtBigInteger, "addBigIBigI()", "315801132574230400244335633537244463442444453154353354424321220233524352113325", "", 0);

  initializeRegister(op1, dtBigInteger,  "546854254695442521456456845658456584654565455256455456545652564554556455425545", "", 0);
  initializeRegister(op2, dtReal16,      "5e61",                                                                           "", 0);
  addBigIRe16();
  registerShouldBe(result, dtReal16,     "addBigIRe16()", "5468542546954426e62", "", 0);

  //addBigICo16();
  //addBigITime();
  //addBigIDate();
  //addBigISmaI();
  //addBigIRe34();
  //addBigICo34();
  //addRe16BigI();
  //addRe16Re16();
  //addRe16Co16();
  //addRe16Time();
  //addRe16Date();
  //addRe16SmaI();
  //addRe16Re34();
  //addRe16Co34();
  //addCo16BigI();
  //addCo16Re16();
  //addCo16Co16();
  //addCo16SmaI();
  //addCo16Re34();
  //addCo16Co34();
  //addTimeBigI();
  //addTimeRe16();
  //addTimeTime();
  //addTimeRe34();
  //addDateBigI();
  //addDateRe16();
  //addDateRe34();
  //addStriBigI();
  //addStriRe16();
  //addStriCo16();
  //addStriTime();
  //addStriDate();
  //addStriStri();
  //addStriRm16();
  //addStriCm16();
  //addStriSmaI();
  //addStriRe34();
  //addStriCo34();
  //addRm16Rm16();
  //addRm16Cm16();
  //addCm16Rm16();
  //addCm16Cm16();
  //addSmaIBigI();
  //addSmaIRe16();
  //addSmaICo16();
  //addSmaISmaI();
  //addSmaIRe34();
  //addSmaICo34();
  //addRe34BigI();
  //addRe34Re16();
  //addRe34Co16();
  //addRe34Time();
  //addRe34Date();
  //addRe34SmaI();
  //addRe34Re34();
  //addRe34Co34();
  //addCo34BigI();
  //addCo34Re16();
  //addCo34Co16();
  //addCo34SmaI();
  //addCo34Re34();
  //addCo34Co34();

  freeTemporaryRegister(op1);
  freeTemporaryRegister(op2);
  freeTemporaryRegister(result);

  //////////////////////////////
  // pi
  #ifdef PC_BUILD
    printf("  Pi\n");
  #endif

  fnPi(NOPARAM);
  registerShouldBe(REGISTER_X, dtReal16, "fnPi()", "3.141592653589793", "", 0);


  //////////////////////////////
  // Constants
  #ifdef PC_BUILD
    printf("  Constants\n");
  #endif

  fnConstant(36);

  registerShouldBe(REGISTER_X, dtReal16, "fnConstant(36)", "5.9736e24", "", 0); // Mass of Earth


  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, 0);

  real16Copy(const16_minusInfinity, POINTER_TO_REGISTER_DATA(REGISTER_X));
  registerShouldBe(REGISTER_X, dtReal16, "const16_minusInfinity", "-9999e9999", "", 0);

  real16Copy(const16_32, POINTER_TO_REGISTER_DATA(REGISTER_X));
  registerShouldBe(REGISTER_X, dtReal16, "const16_32", "32", "", 0);

  real16Copy(const16_plusInfinity, POINTER_TO_REGISTER_DATA(REGISTER_X));
  registerShouldBe(REGISTER_X, dtReal16, "const16_plusInfinity", "9999e9999", "", 0);


  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, 0);

  real34Copy(const34_10000, POINTER_TO_REGISTER_DATA(REGISTER_X));
  registerShouldBe(REGISTER_X, dtReal34, "const34_10000", "10000", "", 0);

  real34Copy(const34_36, POINTER_TO_REGISTER_DATA(REGISTER_X));
  registerShouldBe(REGISTER_X, dtReal34, "const34_36", "36", "", 0);

  real34Copy(const34_60, POINTER_TO_REGISTER_DATA(REGISTER_X));
  registerShouldBe(REGISTER_X, dtReal34, "const34_60", "60", "", 0);

  real34Copy(const34_0_5, POINTER_TO_REGISTER_DATA(REGISTER_X));
  registerShouldBe(REGISTER_X, dtReal34, "const34_0_5", "0.5", "", 0);

  real34Copy(const34_1_8, POINTER_TO_REGISTER_DATA(REGISTER_X));
  registerShouldBe(REGISTER_X, dtReal34, "const34_1_8", "1.8", "", 0);

  real34Copy(const34_pi, POINTER_TO_REGISTER_DATA(REGISTER_X));
  registerShouldBe(REGISTER_X, dtReal34, "const34_pi", "3.141592653589793238462643383279502884", "", 0);

  real34Copy(const34_32, POINTER_TO_REGISTER_DATA(REGISTER_X));
  registerShouldBe(REGISTER_X, dtReal34, "const34_32", "32", "", 0);


  //////////////////////////////
  // Clear register X
  #ifdef PC_BUILD
    printf("  Clear X\n");
  #endif

  initializeRegister(REGISTER_X, dtComplex16, "-1", "-2", 0);

  fnClX(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16, "fnClX()", "0", "", 0);


  //////////////////////////////
  // Clear stack 4 levels
  #ifdef PC_BUILD
    printf("  Clear stack\n");
  #endif

  fnStackSize(SS_4);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  fnClearStack(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16,    "fnClearStack(4)", "0",     "",        0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnClearStack(4)", "0",     "",        0);
  registerShouldBe(REGISTER_Z, dtReal16,    "fnClearStack(4)", "0",     "",        0);
  registerShouldBe(REGISTER_T, dtReal16,    "fnClearStack(4)", "0",     "",        0);
  registerShouldBe(REGISTER_A, dtComplex16, "fnClearStack(4)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_B, dtComplex34, "fnClearStack(4)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_C, dtReal16,    "fnClearStack(4)", "43",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "fnClearStack(4)", "44",    "",        0);


  //////////////////////////////
  // Clear stack 8 levels
  fnStackSize(SS_8);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  fnClearStack(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16, "fnClearStack(8)", "0", "", 0);
  registerShouldBe(REGISTER_Y, dtReal16, "fnClearStack(8)", "0", "", 0);
  registerShouldBe(REGISTER_Z, dtReal16, "fnClearStack(8)", "0", "", 0);
  registerShouldBe(REGISTER_T, dtReal16, "fnClearStack(8)", "0", "", 0);
  registerShouldBe(REGISTER_A, dtReal16, "fnClearStack(8)", "0", "", 0);
  registerShouldBe(REGISTER_B, dtReal16, "fnClearStack(8)", "0", "", 0);
  registerShouldBe(REGISTER_C, dtReal16, "fnClearStack(8)", "0", "", 0);
  registerShouldBe(REGISTER_D, dtReal16, "fnClearStack(8)", "0", "", 0);


  //////////////////////////////
  // Drop 4 stack levels
  #ifdef PC_BUILD
    printf("  Drop stack\n");
  #endif

  fnStackSize(SS_4);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  fnDrop(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "fnDrop(4)", "-10",   "-20",     0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnDrop(4)", "1.1",   "",        0);
  registerShouldBe(REGISTER_Z, dtReal34,    "fnDrop(4)", "2.2",   "",        0);
  registerShouldBe(REGISTER_T, dtReal34,    "fnDrop(4)", "2.2",   "",        0);
  registerShouldBe(REGISTER_A, dtComplex16, "fnDrop(4)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_B, dtComplex34, "fnDrop(4)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_C, dtReal16,    "fnDrop(4)", "43",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "fnDrop(4)", "44",    "",        0);


  //////////////////////////////
  // Drop 8 stack levels
  fnStackSize(SS_8);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  fnDrop(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "fnDrop(8)", "-10",   "-20",     0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnDrop(8)", "1.1",   "",        0);
  registerShouldBe(REGISTER_Z, dtReal34,    "fnDrop(8)", "2.2",   "",        0);
  registerShouldBe(REGISTER_T, dtComplex16, "fnDrop(8)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_A, dtComplex34, "fnDrop(8)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_B, dtReal16,    "fnDrop(8)", "43",    "",        0);
  registerShouldBe(REGISTER_C, dtReal34,    "fnDrop(8)", "44",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "fnDrop(8)", "44",    "",        0);


  //////////////////////////////
  // Lift 4 stack levels
  #ifdef PC_BUILD
    printf("  Lift stack\n");
  #endif

  fnStackSize(SS_4);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  #if (STACK_LIFT_DEBUG == 1)
    stackLiftEnable();
  #else
    stackLiftEnabled = true;
  #endif

  liftStack(dtReal16, REAL16_SIZE);
  real16Zero(POINTER_TO_REGISTER_DATA(REGISTER_X));

  registerShouldBe(REGISTER_X, dtReal16,    "liftStack(4)", "0",     "",        0);
  registerShouldBe(REGISTER_Y, dtComplex16, "liftStack(4)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_Z, dtComplex34, "liftStack(4)", "-10",   "-20",     0);
  registerShouldBe(REGISTER_T, dtReal16,    "liftStack(4)", "1.1",   "",        0);
  registerShouldBe(REGISTER_A, dtComplex16, "liftStack(4)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_B, dtComplex34, "liftStack(4)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_C, dtReal16,    "liftStack(4)", "43",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "liftStack(4)", "44",    "",        0);


  //////////////////////////////
  // Lift 8 stack levels
  fnStackSize(SS_8);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  #if (STACK_LIFT_DEBUG == 1)
    stackLiftEnable();
  #else
    stackLiftEnabled = true;
  #endif

  liftStack(dtReal16, REAL16_SIZE);
  real16Zero(POINTER_TO_REGISTER_DATA(REGISTER_X));

  registerShouldBe(REGISTER_X, dtReal16,    "liftStack(8)", "0",     "",        0);
  registerShouldBe(REGISTER_Y, dtComplex16, "liftStack(8)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_Z, dtComplex34, "liftStack(8)", "-10",   "-20",     0);
  registerShouldBe(REGISTER_T, dtReal16,    "liftStack(8)", "1.1",   "",        0);
  registerShouldBe(REGISTER_A, dtReal34,    "liftStack(8)", "2.2",   "",        0);
  registerShouldBe(REGISTER_B, dtComplex16, "liftStack(8)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_C, dtComplex34, "liftStack(8)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_D, dtReal16,    "liftStack(8)", "43",    "",        0);


  //////////////////////////////
  // Drop Y 4 stack levels
  #ifdef PC_BUILD
    printf("  Drop Y\n");
  #endif

  fnStackSize(SS_4);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  fnDropY(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "fnDropY(4)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnDropY(4)", "1.1",   "",        0);
  registerShouldBe(REGISTER_Z, dtReal34,    "fnDropY(4)", "2.2",   "",        0);
  registerShouldBe(REGISTER_T, dtReal34,    "fnDropY(4)", "2.2",   "",        0);
  registerShouldBe(REGISTER_A, dtComplex16, "fnDropY(4)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_B, dtComplex34, "fnDropY(4)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_C, dtReal16,    "fnDropY(4)", "43",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "fnDropY(4)", "44",    "",        0);


  //////////////////////////////
  // Drop Y 8 stack levels
  fnStackSize(SS_8);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  fnDropY(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "fnDropY(8)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnDropY(8)", "1.1",   "",        0);
  registerShouldBe(REGISTER_Z, dtReal34,    "fnDropY(8)", "2.2",   "",        0);
  registerShouldBe(REGISTER_T, dtComplex16, "fnDropY(8)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_A, dtComplex34, "fnDropY(8)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_B, dtReal16,    "fnDropY(8)", "43",    "",        0);
  registerShouldBe(REGISTER_C, dtReal34,    "fnDropY(8)", "44",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "fnDropY(8)", "44",    "",        0);


  //////////////////////////////
  // Roll down 4 stack levels
  #ifdef PC_BUILD
    printf("  Roll down stack\n");
  #endif

  fnStackSize(SS_4);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  fnRollDown(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "fnRollDown(4)", "-10",   "-20",     0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnRollDown(4)", "1.1",   "",        0);
  registerShouldBe(REGISTER_Z, dtReal34,    "fnRollDown(4)", "2.2",   "",        0);
  registerShouldBe(REGISTER_T, dtComplex16, "fnRollDown(4)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_A, dtComplex16, "fnRollDown(4)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_B, dtComplex34, "fnRollDown(4)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_C, dtReal16,    "fnRollDown(4)", "43",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "fnRollDown(4)", "44",    "",        0);


  //////////////////////////////
  // Roll down 8 stack levels
  fnStackSize(SS_8);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  fnRollDown(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "fnRollDown(8)", "-10",   "-20",     0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnRollDown(8)", "1.1",   "",        0);
  registerShouldBe(REGISTER_Z, dtReal34,    "fnRollDown(8)", "2.2",   "",        0);
  registerShouldBe(REGISTER_T, dtComplex16, "fnRollDown(8)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_A, dtComplex34, "fnRollDown(8)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_B, dtReal16,    "fnRollDown(8)", "43",    "",        0);
  registerShouldBe(REGISTER_C, dtReal34,    "fnRollDown(8)", "44",    "",        0);
  registerShouldBe(REGISTER_D, dtComplex16, "fnRollDown(8)", "-1",    "-2",      0);


  //////////////////////////////
  // Roll up 4 stack levels
  #ifdef PC_BUILD
    printf("  Roll up stack\n");
  #endif

  fnStackSize(SS_4);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  fnRollUp(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34,    "fnRollUp(4)", "2.2",   "",        0);
  registerShouldBe(REGISTER_Y, dtComplex16, "fnRollUp(4)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_Z, dtComplex34, "fnRollUp(4)", "-10",   "-20",     0);
  registerShouldBe(REGISTER_T, dtReal16,    "fnRollUp(4)", "1.1",   "",        0);
  registerShouldBe(REGISTER_A, dtComplex16, "fnRollUp(4)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_B, dtComplex34, "fnRollUp(4)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_C, dtReal16,    "fnRollUp(4)", "43",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "fnRollUp(4)", "44",    "",        0);


  //////////////////////////////
  // Roll up 8 stack levels
  fnStackSize(SS_8);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  fnRollUp(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34,    "fnRollUp(8)", "44",    "",        0);
  registerShouldBe(REGISTER_Y, dtComplex16, "fnRollUp(8)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_Z, dtComplex34, "fnRollUp(8)", "-10",   "-20",     0);
  registerShouldBe(REGISTER_T, dtReal16,    "fnRollUp(8)", "1.1",   "",        0);
  registerShouldBe(REGISTER_A, dtReal34,    "fnRollUp(8)", "2.2",   "",        0);
  registerShouldBe(REGISTER_B, dtComplex16, "fnRollUp(8)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_C, dtComplex34, "fnRollUp(8)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_D, dtReal16,    "fnRollUp(8)", "43",    "",        0);


  //////////////////////////////
  // Swap X and Y
  #ifdef PC_BUILD
    printf("  Swap X and Y\n");
  #endif

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",  0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20", 0);

  fnSwapXY(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "fnSwapXY()", "-10", "-20", 0);
  registerShouldBe(REGISTER_Y, dtComplex16, "fnSwapXY()", "-1",  "-2",  0);


  //////////////////////////////
  // Fill 4 stack levels
  #ifdef PC_BUILD
    printf("  Fill stack stack\n");
  #endif

  fnStackSize(SS_4);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  fnFillStack(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "fnFillStack(4)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_Y, dtComplex16, "fnFillStack(4)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_Z, dtComplex16, "fnFillStack(4)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_T, dtComplex16, "fnFillStack(4)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_A, dtComplex16, "fnFillStack(4)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_B, dtComplex34, "fnFillStack(4)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_C, dtReal16,    "fnFillStack(4)", "43",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "fnFillStack(4)", "44",    "",        0);


  //////////////////////////////
  // Fill 8 stack levels
  fnStackSize(SS_8);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  fnFillStack(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "fnFillStack(8)", "-1", "-2", 0);
  registerShouldBe(REGISTER_Y, dtComplex16, "fnFillStack(8)", "-1", "-2", 0);
  registerShouldBe(REGISTER_Z, dtComplex16, "fnFillStack(8)", "-1", "-2", 0);
  registerShouldBe(REGISTER_T, dtComplex16, "fnFillStack(8)", "-1", "-2", 0);
  registerShouldBe(REGISTER_A, dtComplex16, "fnFillStack(8)", "-1", "-2", 0);
  registerShouldBe(REGISTER_B, dtComplex16, "fnFillStack(8)", "-1", "-2", 0);
  registerShouldBe(REGISTER_C, dtComplex16, "fnFillStack(8)", "-1", "-2", 0);
  registerShouldBe(REGISTER_D, dtComplex16, "fnFillStack(8)", "-1", "-2", 0);


  //////////////////////////////
  // Swap real16 and imaginary part
  #ifdef PC_BUILD
    printf("  Swap real and imaginary part\n");
  #endif

  initializeRegister(REGISTER_X, dtComplex16, "41.41", "41.4141", 0);

  fnSwapRealImaginary(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "fnSwapReIm(16d)", "41.4141", "41.41",   0);
  registerShouldBe(REGISTER_L, dtComplex16, "fnSwapReIm(16d)", "41.41",   "41.4141", 0);


  //////////////////////////////
  // Swap real34 and imaginary part
  initializeRegister(REGISTER_X, dtComplex34, "42.42", "42.4242", 0);

  fnSwapRealImaginary(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "fnSwapReIm(34d)", "42.4242", "42.42",   0);
  registerShouldBe(REGISTER_L, dtComplex34, "fnSwapReIm(34d)", "42.42",   "42.4242", 0);


  //////////////////////////////
  // real16 complex real part
  #ifdef PC_BUILD
    printf("  Real part\n");
  #endif

  initializeRegister(REGISTER_X, dtComplex16, "41.41", "41.4141", 0);

  fnRealPart(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16,    "fnRealPart(16d)", "41.41", "",        0);
  registerShouldBe(REGISTER_L, dtComplex16, "fnRealPart(16d)", "41.41", "41.4141", 0);


  //////////////////////////////
  // real34 complex real part
  initializeRegister(REGISTER_X, dtComplex34, "42.42", "42.4242", 0);

  fnRealPart(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34,    "fnRealPart(34d)", "42.42", "",        0);
  registerShouldBe(REGISTER_L, dtComplex34, "fnRealPart(34d)", "42.42", "42.4242", 0);


  //////////////////////////////
  // real16 complex imaginary part
  #ifdef PC_BUILD
    printf("  Imaginary part\n");
  #endif

  initializeRegister(REGISTER_X, dtComplex16, "41.41", "41.4141", 0);

  fnImaginaryPart(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16,    "fnImaginaryPart(16d)", "41.4141", "",        0);
  registerShouldBe(REGISTER_L, dtComplex16, "fnImaginaryPart(16d)", "41.41",   "41.4141", 0);


  //////////////////////////////
  // real34 complex imaginary part
  initializeRegister(REGISTER_X, dtComplex34, "42.42", "42.4242", 0);

  fnImaginaryPart(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34,    "fnImaginaryPart(34d)", "42.4242", "",        0);
  registerShouldBe(REGISTER_L, dtComplex34, "fnImaginaryPart(34d)", "42.42",   "42.4242", 0);


  //////////////////////////////
  // complex16 conjugate
  #ifdef PC_BUILD
    printf("  Conjugate\n");
  #endif

  initializeRegister(REGISTER_X, dtComplex16, "41.41", "41.4141", 0);

  fnConjugate(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "fnConj(16d)", "41.41", "-41.4141", 0);
  registerShouldBe(REGISTER_L, dtComplex16, "fnConj(16d)", "41.41", "41.4141",  0);


  //////////////////////////////
  // complex34 conjugate
  initializeRegister(REGISTER_X, dtComplex34, "42.42", "42.4242", 0);

  fnConjugate(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "fnConj(34d)", "42.42", "-42.4242", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "fnConj(34d)", "42.42", "42.4242" , 0);


  //////////////////////////////
  // Magnitude complex16
  #ifdef PC_BUILD
    printf("  Magnitude\n");
  #endif

  initializeRegister(REGISTER_X, dtComplex16, "41.41", "11.11", 0);

  fnMagnitude(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16,    "fnMagnitude(16d)", "42.87447025911807", "",      0);
  registerShouldBe(REGISTER_L, dtComplex16, "fnMagnitude(16d)", "41.41",             "11.11", 0);


  //////////////////////////////
  // Magnitude complex34
  initializeRegister(REGISTER_X, dtComplex34, "42.42", "22.22", 0);

  fnMagnitude(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34,    "fnMagnitude(34d)", "47.887209148163980260860838154706820", "",      0);
  registerShouldBe(REGISTER_L, dtComplex34, "fnMagnitude(34d)", "42.42",                                "22.22", 0);


  //////////////////////////////
  // UNITV complex16
  #ifdef PC_BUILD
    printf("  UnitV\n");
  #endif

  initializeRegister(REGISTER_X, dtComplex16, "41.41", "11.11", 0);

  fnUnitVector(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "fnUnitV(16d)", "0.9658428372346683", "0.2591285660873500", 0);
  registerShouldBe(REGISTER_L, dtComplex16, "fnUnitV(16d)", "41.41",              "11.11",              0);


  //////////////////////////////
  // UNITV complex34
  initializeRegister(REGISTER_X, dtComplex34, "42.42", "22.22", 0);

  fnUnitVector(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "fnUnitV(34d)", "0.8858315352801553728533857168958488", "0.4640069946705575762565353755168732", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "fnUnitV(34d)", "42.42",                                "22.22",                                0);


  //////////////////////////////
  // Convert X from real16 to real34
  #ifdef PC_BUILD
    printf("  Convert 16d to 34d\n");
  #endif

  initializeRegister(REGISTER_X, dtReal16, "12.34", "", 0);

  fnConvertXToReal34(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "fnConvertXToReal34()", "12.34", "", 0);


  //////////////////////////////
  // Convert X from complex16 to complex34
  initializeRegister(REGISTER_X, dtComplex16, "12.34", "56.78", 0);

  fnConvertXToReal34(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "fnConvertXToReal34()", "12.34", "56.78", 0);


  //////////////////////////////
  // Invert real16
  #ifdef PC_BUILD
    printf("  1/x\n");
  #endif

  initializeRegister(REGISTER_X, dtReal16, "8", "", 0);

  fnInvert(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16, "fnInvert(16d)", "0.125", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnInvert(16d)", "8",     "", 0);


  //////////////////////////////
  // Invert real34
  initializeRegister(REGISTER_X, dtReal34, "8", "", 0);

  fnInvert(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "fnInvert(34d)", "0.125", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnInvert(34d)", "8",     "", 0);


  //////////////////////////////
  // Invert complex16
  initializeRegister(REGISTER_X, dtComplex16, "1", "2", 0);

  fnInvert(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "fnInvert(16d)", "0.2", "-0.4", 0);
  registerShouldBe(REGISTER_L, dtComplex16, "fnInvert(16d)", "1",   "2",    0);


  //////////////////////////////
  // Invert complex34
  initializeRegister(REGISTER_X, dtComplex34, "1", "2", 0);

  fnInvert(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "fnInvert(34d)", "0.2", "-0.4", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "fnInvert(34d)", "1",   "2",    0);


  //////////////////////////////
  // Square real16
  #ifdef PC_BUILD
    printf("  X squared\n");
  #endif

  initializeRegister(REGISTER_X, dtReal16, "12.3", "", 0);

  fnSquare(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16, "fnSquare(real16)", "151.29", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnSquare(real16)", "12.3",   "", 0);


  //////////////////////////////
  // Square real34
  initializeRegister(REGISTER_X, dtReal34, "23.4", "", 0);

  fnSquare(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "fnSquare(real34)", "547.56", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnSquare(real34)", "23.4",   "", 0);


  //////////////////////////////
  // Square complex16
  initializeRegister(REGISTER_X, dtComplex16, "12.3", "-5.7", 0);

  fnSquare(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "fnSquare(complex16)", "118.8", "-140.22", 0);
  registerShouldBe(REGISTER_L, dtComplex16, "fnSquare(complex16)", "12.3",  "-5.7",    0);


  //////////////////////////////
  // Square complex34
  initializeRegister(REGISTER_X, dtComplex34, "23.4", "-9.3", 0);

  fnSquare(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "fnSquare(complex34)", "461.07", "-435.24", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "fnSquare(complex34)", "23.4",   "-9.3",    0);


  //////////////////////////////
  // Square big integer
  initializeRegister(REGISTER_X, dtBigInteger, "12345678901234567890", "", 0);

  fnSquare(NOPARAM);
  //fnSquare(NOPARAM);
  //fnSquare(NOPARAM);
  //fnSquare(NOPARAM);
  //fnSquare(NOPARAM);

  registerShouldBe(REGISTER_X, dtBigInteger, "fnSquare(big integer)", "152415787532388367501905199875019052100", "", 0);
  registerShouldBe(REGISTER_L, dtBigInteger, "fnSquare(big integer)", "12345678901234567890",                    "", 0);


  //////////////////////////////
  // Cube real16
  #ifdef PC_BUILD
    printf("  X cubed\n");
  #endif

  initializeRegister(REGISTER_X, dtReal16, "12.3", "", 0);

  fnCube(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16, "fnCube(real16)", "1860.867", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCube(real16)", "12.3",     "", 0);


  //////////////////////////////
  // Cube real34
  initializeRegister(REGISTER_X, dtReal34, "23.4", "", 0);

  fnCube(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "fnCube(real34)", "12812.904", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCube(real34)", "23.4",      "", 0);


  //////////////////////////////
  // Cube complex16
  initializeRegister(REGISTER_X, dtComplex16, "12.3", "-5.7", 0);

  fnCube(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "fnCube(complex16)", "661.986", "-2401.866", 0);
  registerShouldBe(REGISTER_L, dtComplex16, "fnCube(complex16)", "12.3",    "-5.7",      0);


  //////////////////////////////
  // Cube complex34
  initializeRegister(REGISTER_X, dtComplex34, "23.4", "-9.3", 0);

  fnCube(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "fnCube(complex34)", "6741.306", "-14472.567", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "fnCube(complex34)", "23.4",     "-9.3",       0);


  //////////////////////////////
  // Cube big integer
  initializeRegister(REGISTER_X, dtBigInteger, "1234567890", "", 0);

  fnCube(NOPARAM);

  registerShouldBe(REGISTER_X, dtBigInteger, "fnCube(big integer)", "1881676371789154860897069000", "", 0);
  registerShouldBe(REGISTER_L, dtBigInteger, "fnCube(big integer)", "1234567890",   "",    0);



  //////////////////////////////
  // String tests
  #ifdef PC_BUILD
    printf("  String\n");
  #endif

  #ifdef PC_BUILD
    initializeRegister(REGISTER_X, dtString, "50", "Test string " STD_OMEGA STD_SUB_2 "\x81\x00\xa2\x00", 0); //20+1 bytes and 16 glyphs

    if(stringByteLength( POINTER_TO_REGISTER_STRING(REGISTER_X))     != 20) {printf("stringByteLength( POINTER_TO_REGISTER_STRING(REGISTER_X))     should be 20 but it's %" FMT32S "\n", stringByteLength( POINTER_TO_REGISTER_STRING(REGISTER_X)));     exit(0);}
    if(stringGlyphLength(POINTER_TO_REGISTER_STRING(REGISTER_X))     != 16) {printf("stringGlyphLength(POINTER_TO_REGISTER_STRING(REGISTER_X))     should be 20 but it's %" FMT32S "\n", stringGlyphLength(POINTER_TO_REGISTER_STRING(REGISTER_X)));     exit(0);}
    if(stringNextGlyph(  POINTER_TO_REGISTER_STRING(REGISTER_X),  8) !=  9) {printf("stringNextGlypg(  POINTER_TO_REGISTER_STRING(REGISTER_X),  8) should be  9 but it's %" FMT16S "\n", stringNextGlyph(  POINTER_TO_REGISTER_STRING(REGISTER_X),  8)); exit(0);}
    if(stringNextGlyph(  POINTER_TO_REGISTER_STRING(REGISTER_X), 12) != 14) {printf("stringNextGlypg(  POINTER_TO_REGISTER_STRING(REGISTER_X), 12) should be 14 but it's %" FMT16S "\n", stringNextGlyph(  POINTER_TO_REGISTER_STRING(REGISTER_X), 12)); exit(0);}
    if(stringLastGlyph(  POINTER_TO_REGISTER_STRING(REGISTER_X))     != 18) {printf("stringLastGlypg(  POINTER_TO_REGISTER_STRING(REGISTER_X))     should be 18 but it's %" FMT16S "\n", stringLastGlyph(  POINTER_TO_REGISTER_STRING(REGISTER_X)));     exit(0);}
    if(stringWidth(POINTER_TO_REGISTER_STRING(REGISTER_X), &standardFont, true, true) != 155) {printf("stringWidth(POINTER_TO_REGISTER_STRING(REGISTER_X), ...) should be 155 but it's %" FMT16S "\n", stringWidth(POINTER_TO_REGISTER_STRING(REGISTER_X), &standardFont, true, true)); exit(0);}
  #endif


  //////////////////////////////
  // Change sign real16
  #ifdef PC_BUILD
    printf("  Change sign\n");
  #endif

  initializeRegister(REGISTER_X, dtReal16, "125.85", "", 0);
  fnChangeSign(NOPARAM);
  registerShouldBe(REGISTER_X, dtReal16, "fnChangeSign(real16)", "-125.85",  "", 0);
  //registerShouldBe(REGISTER_L, dtReal16, "fnChangeSign(real16)", "125.85",   "", 0);
  fnChangeSign(NOPARAM);
  registerShouldBe(REGISTER_X, dtReal16, "fnChangeSign(real16)", "125.85",    "", 0);
  //registerShouldBe(REGISTER_L, dtReal16, "fnChangeSign(real16)", "-125.85",   "", 0);


  //////////////////////////////
  // Change sign real34
  initializeRegister(REGISTER_X, dtReal34, "425.85", "", 0);
  fnChangeSign(NOPARAM);
  registerShouldBe(REGISTER_X, dtReal34, "fnChangeSign(real34)", "-425.85",  "", 0);
  //registerShouldBe(REGISTER_L, dtReal34, "fnChangeSign(real34)", "425.85",   "", 0);
  fnChangeSign(NOPARAM);
  registerShouldBe(REGISTER_X, dtReal34, "fnChangeSign(real34)", "425.85",    "", 0);
  //registerShouldBe(REGISTER_L, dtReal34, "fnChangeSign(real34)", "-425.85",   "", 0);


  //////////////////////////////
  // Change sign complex16
  initializeRegister(REGISTER_X, dtReal16, "175.85", "-547.564", 0);
  fnChangeSign(NOPARAM);
  registerShouldBe(REGISTER_X, dtReal16, "fnChangeSign(complex16)", "-175.85", "547.564", 0);
  //registerShouldBe(REGISTER_L, dtReal16, "fnChangeSign(complex16)", "175.85",  "-547.564", 0);
  fnChangeSign(NOPARAM);
  registerShouldBe(REGISTER_X, dtReal16, "fnChangeSign(complex16)", "175.85",  "-547.564", 0);
  //registerShouldBe(REGISTER_L, dtReal16, "fnChangeSign(complex16)", "-175.85", "547.564", 0);


  //////////////////////////////
  // Change sign complex34
  initializeRegister(REGISTER_X, dtReal34, "-895.541", "5297.2", 0);
  fnChangeSign(NOPARAM);
  registerShouldBe(REGISTER_X, dtReal34, "fnChangeSign(complex34)", "895.541",  "-5297.2", 0);
  //registerShouldBe(REGISTER_L, dtReal34, "fnChangeSign(complex34)", "-895.541", "5297.2", 0);
  fnChangeSign(NOPARAM);
  registerShouldBe(REGISTER_X, dtReal34, "fnChangeSign(complex34)", "-895.541", "5297.2", 0);
  //registerShouldBe(REGISTER_L, dtReal34, "fnChangeSign(complex34)", "895.541",  "-5297.2", 0);


  //////////////////////////////
  // CC complex16 ==> real16s 4 stack levels
  #ifdef PC_BUILD
    printf("  Split complex\n");
  #endif

  fnStackSize(SS_4);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  userModeEnabled = false;
  shiftF = true;
  btnClicked(NULL, "02"); // KEY CC

  registerShouldBe(REGISTER_L, dtComplex16, "fnComplexCC(complex16 ==> real16s 4)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_X, dtReal16, "   fnComplexCC(complex16 ==> real16s 4)", "-1",    "",        0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnComplexCC(complex16 ==> real16s 4)", "-2",    "",        0);
  registerShouldBe(REGISTER_Z, dtComplex34, "fnComplexCC(complex16 ==> real16s 4)", "-10",   "-20",     0);
  registerShouldBe(REGISTER_T, dtReal16,    "fnComplexCC(complex16 ==> real16s 4)", "1.1",   "",        0);
  registerShouldBe(REGISTER_A, dtComplex16, "fnComplexCC(complex16 ==> real16s 4)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_B, dtComplex34, "fnComplexCC(complex16 ==> real16s 4)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_C, dtReal16,    "fnComplexCC(complex16 ==> real16s 4)", "43",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "fnComplexCC(complex16 ==> real16s 4)", "44",    "",        0);


  //////////////////////////////
  // CC complex16 ==> real16s 8 stack levels
  fnStackSize(SS_8);

  initializeRegister(REGISTER_X, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Y, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  userModeEnabled = false;
  shiftF = true;
  btnClicked(NULL, "02"); // KEY CC

  registerShouldBe(REGISTER_L, dtComplex16, "fnComplexCC(complex16 ==> real16s 8)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_X, dtReal16,    "fnComplexCC(complex16 ==> real16s 8)", "-1",    "",        0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnComplexCC(complex16 ==> real16s 8)", "-2",    "",        0);
  registerShouldBe(REGISTER_Z, dtComplex34, "fnComplexCC(complex16 ==> real16s 8)", "-10",   "-20",     0);
  registerShouldBe(REGISTER_T, dtReal16,    "fnComplexCC(complex16 ==> real16s 8)", "1.1",   "",        0);
  registerShouldBe(REGISTER_A, dtReal34,    "fnComplexCC(complex16 ==> real16s 8)", "2.2",   "",        0);
  registerShouldBe(REGISTER_B, dtComplex16, "fnComplexCC(complex16 ==> real16s 8)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_C, dtComplex34, "fnComplexCC(complex16 ==> real16s 8)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_D, dtReal16,    "fnComplexCC(complex16 ==> real16s 8)", "43",    "",        0);


  //////////////////////////////
  // CC complex34 ==> real34s 4 stack levels
  fnStackSize(SS_4);

  initializeRegister(REGISTER_X, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Y, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  userModeEnabled = false;
  shiftF = true;
  btnClicked(NULL, "02"); // KEY CC

  registerShouldBe(REGISTER_L, dtComplex34, "fnComplexCC(complex34 ==> real34s 4)", "-10",   "-20",     0);
  registerShouldBe(REGISTER_X, dtReal34, "   fnComplexCC(complex34 ==> real34s 4)", "-10",   "",        0);
  registerShouldBe(REGISTER_Y, dtReal34,    "fnComplexCC(complex34 ==> real34s 4)", "-20",   "",        0);
  registerShouldBe(REGISTER_Z, dtComplex16, "fnComplexCC(complex34 ==> real34s 4)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_T, dtReal16,    "fnComplexCC(complex34 ==> real34s 4)", "1.1",   "",        0);
  registerShouldBe(REGISTER_A, dtComplex16, "fnComplexCC(complex34 ==> real34s 4)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_B, dtComplex34, "fnComplexCC(complex34 ==> real34s 4)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_C, dtReal16,    "fnComplexCC(complex34 ==> real34s 4)", "43",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "fnComplexCC(complex34 ==> real34s 4)", "44",    "",        0);


  //////////////////////////////
  // CC complex34 ==> real34s 8 stack levels
  fnStackSize(SS_8);

  initializeRegister(REGISTER_X, dtComplex34, "-10",   "-20",     0);
  initializeRegister(REGISTER_Y, dtComplex16, "-1",    "-2",      0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  userModeEnabled = false;
  shiftF = true;
  btnClicked(NULL, "02"); // KEY CC

  registerShouldBe(REGISTER_L, dtComplex34, "fnComplexCC(complex34 ==> real34s 8)", "-10",   "-20",     0);
  registerShouldBe(REGISTER_X, dtReal34, "   fnComplexCC(complex34 ==> real34s 8)", "-10",   "",        0);
  registerShouldBe(REGISTER_Y, dtReal34,    "fnComplexCC(complex34 ==> real34s 8)", "-20",   "",        0);
  registerShouldBe(REGISTER_Z, dtComplex16, "fnComplexCC(complex34 ==> real34s 8)", "-1",    "-2",      0);
  registerShouldBe(REGISTER_T, dtReal16,    "fnComplexCC(complex34 ==> real34s 8)", "1.1",   "",        0);
  registerShouldBe(REGISTER_A, dtReal34,    "fnComplexCC(complex34 ==> real34s 8)", "2.2",   "",        0);
  registerShouldBe(REGISTER_B, dtComplex16, "fnComplexCC(complex34 ==> real34s 8)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_C, dtComplex34, "fnComplexCC(complex34 ==> real34s 8)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_D, dtReal16,    "fnComplexCC(complex34 ==> real34s 8)", "43",    "",        0);


  //////////////////////////////
  // CC real16s ==> complex16 4 stack levels
  #ifdef PC_BUILD
    printf("  Compose complex\n");
  #endif

  fnStackSize(SS_4);

  initializeRegister(REGISTER_X, dtReal16,    "-4",    "",        0);
  initializeRegister(REGISTER_Y, dtReal16,    "-8",    "",        0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  userModeEnabled = false;
  shiftF = true;
  btnClicked(NULL, "02"); // KEY CC

  registerShouldBe(REGISTER_L, dtReal16,    "fnComplexCC(real16s ==> complex16 4)", "-4",    "",        0);
  registerShouldBe(REGISTER_X, dtComplex16, "fnComplexCC(real16s ==> complex16 4)", "-4",    "-8",      0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnComplexCC(real16s ==> complex16 4)", "1.1",   "",        0);
  registerShouldBe(REGISTER_Z, dtReal34,    "fnComplexCC(real16s ==> complex16 4)", "2.2",   "",        0);
  registerShouldBe(REGISTER_T, dtReal34,    "fnComplexCC(real16s ==> complex16 4)", "2.2",   "",        0);
  registerShouldBe(REGISTER_A, dtComplex16, "fnComplexCC(real16s ==> complex16 4)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_B, dtComplex34, "fnComplexCC(real16s ==> complex16 4)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_C, dtReal16,    "fnComplexCC(real16s ==> complex16 4)", "43",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "fnComplexCC(real16s ==> complex16 4)", "44",    "",        0);


  //////////////////////////////
  // CC real16s ==> complex16 8 stack levels
  fnStackSize(SS_8);

  initializeRegister(REGISTER_X, dtReal16,    "-4",    "",        0);
  initializeRegister(REGISTER_Y, dtReal16,    "-8",    "",        0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  userModeEnabled = false;
  shiftF = true;
  btnClicked(NULL, "02"); // KEY CC

  registerShouldBe(REGISTER_L, dtReal16,    "fnComplexCC(real16s ==> complex16 8)", "-4",    "",        0);
  registerShouldBe(REGISTER_X, dtComplex16, "fnComplexCC(real16s ==> complex16 8)", "-4",    "-8",      0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnComplexCC(real16s ==> complex16 8)", "1.1",   "",        0);
  registerShouldBe(REGISTER_Z, dtReal34,    "fnComplexCC(real16s ==> complex16 8)", "2.2",   "",        0);
  registerShouldBe(REGISTER_T, dtComplex16, "fnComplexCC(real16s ==> complex16 8)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_A, dtComplex34, "fnComplexCC(real16s ==> complex16 8)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_B, dtReal16,    "fnComplexCC(real16s ==> complex16 8)", "43",    "",        0);
  registerShouldBe(REGISTER_C, dtReal34,    "fnComplexCC(real16s ==> complex16 8)", "44",    "",        0);
  registerShouldBe(REGISTER_C, dtReal34,    "fnComplexCC(real16s ==> complex16 8)", "44",    "",        0);


  //////////////////////////////
  // CC real34s ==> complex34 4 stack levels
  fnStackSize(SS_4);

  initializeRegister(REGISTER_X, dtReal34,    "-3",    "",        0);
  initializeRegister(REGISTER_Y, dtReal34,    "-6",    "",        0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  userModeEnabled = false;
  shiftF = true;
  btnClicked(NULL, "02"); // KEY CC

  registerShouldBe(REGISTER_L, dtReal34,    "fnComplexCC(real34s ==> complex34 4)", "-3",    "",        0);
  registerShouldBe(REGISTER_X, dtComplex34, "fnComplexCC(real34s ==> complex34 4)", "-3",    "-6",      0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnComplexCC(real34s ==> complex34 4)", "1.1",   "",        0);
  registerShouldBe(REGISTER_Z, dtReal34,    "fnComplexCC(real34s ==> complex34 4)", "2.2",   "",        0);
  registerShouldBe(REGISTER_T, dtReal34,    "fnComplexCC(real34s ==> complex34 4)", "2.2",   "",        0);
  registerShouldBe(REGISTER_A, dtComplex16, "fnComplexCC(real34s ==> complex34 4)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_B, dtComplex34, "fnComplexCC(real34s ==> complex34 4)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_C, dtReal16,    "fnComplexCC(real34s ==> complex34 4)", "43",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "fnComplexCC(real34s ==> complex34 4)", "44",    "",        0);


  //////////////////////////////
  // CC real34s ==> complex34 8 stack levels
  fnStackSize(SS_8);

  initializeRegister(REGISTER_X, dtReal34,    "-3",    "",        0);
  initializeRegister(REGISTER_Y, dtReal34,    "-6",    "",        0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  userModeEnabled = false;
  shiftF = true;
  btnClicked(NULL, "02"); // KEY CC

  registerShouldBe(REGISTER_L, dtReal34,    "fnComplexCC(real34s ==> complex34 8)", "-3",    "",        0);
  registerShouldBe(REGISTER_X, dtComplex34, "fnComplexCC(real34s ==> complex34 8)", "-3",    "-6",      0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnComplexCC(real34s ==> complex34 8)", "1.1",   "",        0);
  registerShouldBe(REGISTER_Z, dtReal34,    "fnComplexCC(real34s ==> complex34 8)", "2.2",   "",        0);
  registerShouldBe(REGISTER_T, dtComplex16, "fnComplexCC(real34s ==> complex34 8)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_A, dtComplex34, "fnComplexCC(real34s ==> complex34 8)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_B, dtReal16,    "fnComplexCC(real34s ==> complex34 8)", "43",    "",        0);
  registerShouldBe(REGISTER_C, dtReal34,    "fnComplexCC(real34s ==> complex34 8)", "44",    "",        0);
  registerShouldBe(REGISTER_C, dtReal34,    "fnComplexCC(real34s ==> complex34 8)", "44",    "",        0);


  //////////////////////////////
  // CC real16 + real34 ==> complex34 4 stack levels
  fnStackSize(SS_4);

  initializeRegister(REGISTER_X, dtReal16,    "3",     "",        0);
  initializeRegister(REGISTER_Y, dtReal34,    "6",     "",        0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  userModeEnabled = false;
  shiftF = true;
  btnClicked(NULL, "02"); // KEY CC

  registerShouldBe(REGISTER_L, dtReal16,    "fnComplexCC(real16 + real34 ==> complex34 4)", "3",     "",        0);
  registerShouldBe(REGISTER_X, dtComplex34, "fnComplexCC(real16 + real34 ==> complex34 4)", "3",     "6",       0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnComplexCC(real16 + real34 ==> complex34 4)", "1.1",   "",        0);
  registerShouldBe(REGISTER_Z, dtReal34,    "fnComplexCC(real16 + real34 ==> complex34 4)", "2.2",   "",        0);
  registerShouldBe(REGISTER_T, dtReal34,    "fnComplexCC(real16 + real34 ==> complex34 4)", "2.2",   "",        0);
  registerShouldBe(REGISTER_A, dtComplex16, "fnComplexCC(real16 + real34 ==> complex34 4)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_B, dtComplex34, "fnComplexCC(real16 + real34 ==> complex34 4)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_C, dtReal16,    "fnComplexCC(real16 + real34 ==> complex34 4)", "43",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "fnComplexCC(real16 + real34 ==> complex34 4)", "44",    "",        0);


  //////////////////////////////
  // CC real16 + real34 ==> complex34 8 stack levels
  fnStackSize(SS_8);

  initializeRegister(REGISTER_X, dtReal16,    "3",     "",        0);
  initializeRegister(REGISTER_Y, dtReal34,    "6",     "",        0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  userModeEnabled = false;
  shiftF = true;
  btnClicked(NULL, "02"); // KEY CC

  registerShouldBe(REGISTER_L, dtReal16,    "fnComplexCC(real16 + real34 ==> complex34 8)", "3",     "",        0);
  registerShouldBe(REGISTER_X, dtComplex34, "fnComplexCC(real16 + real34 ==> complex34 8)", "3",     "6",       0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnComplexCC(real16 + real34 ==> complex34 8)", "1.1",   "",        0);
  registerShouldBe(REGISTER_Z, dtReal34,    "fnComplexCC(real16 + real34 ==> complex34 8)", "2.2",   "",        0);
  registerShouldBe(REGISTER_T, dtComplex16, "fnComplexCC(real16 + real34 ==> complex34 8)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_A, dtComplex34, "fnComplexCC(real16 + real34 ==> complex34 8)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_B, dtReal16,    "fnComplexCC(real16 + real34 ==> complex34 8)", "43",    "",        0);
  registerShouldBe(REGISTER_C, dtReal34,    "fnComplexCC(real16 + real34 ==> complex34 8)", "44",    "",        0);
  registerShouldBe(REGISTER_C, dtReal34,    "fnComplexCC(real16 + real34 ==> complex34 8)", "44",    "",        0);


  //////////////////////////////
  // CC real34 + real16 ==> complex34 4 stack levels
  fnStackSize(SS_4);

  initializeRegister(REGISTER_X, dtReal34,    "6",     "",        0);
  initializeRegister(REGISTER_Y, dtReal16,    "3",     "",        0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  userModeEnabled = false;
  shiftF = true;
  btnClicked(NULL, "02"); // KEY CC

  registerShouldBe(REGISTER_L, dtReal34,    "fnComplexCC(real34 + real16 ==> complex34 4)", "6",     "",        0);
  registerShouldBe(REGISTER_X, dtComplex34, "fnComplexCC(real34 + real16 ==> complex34 4)", "6",     "3",       0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnComplexCC(real34 + real16 ==> complex34 4)", "1.1",   "",        0);
  registerShouldBe(REGISTER_Z, dtReal34,    "fnComplexCC(real34 + real16 ==> complex34 4)", "2.2",   "",        0);
  registerShouldBe(REGISTER_T, dtReal34,    "fnComplexCC(real34 + real16 ==> complex34 4)", "2.2",   "",        0);
  registerShouldBe(REGISTER_A, dtComplex16, "fnComplexCC(real34 + real16 ==> complex34 4)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_B, dtComplex34, "fnComplexCC(real34 + real16 ==> complex34 4)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_C, dtReal16,    "fnComplexCC(real34 + real16 ==> complex34 4)", "43",    "",        0);
  registerShouldBe(REGISTER_D, dtReal34,    "fnComplexCC(real34 + real16 ==> complex34 4)", "44",    "",        0);


  //////////////////////////////
  // CC real34 + real16 ==> complex34 8 stack levels
  fnStackSize(SS_8);

  initializeRegister(REGISTER_X, dtReal34,    "6",     "",        0);
  initializeRegister(REGISTER_Y, dtReal16,    "3",     "",        0);
  initializeRegister(REGISTER_Z, dtReal16,    "1.1",   "",        0);
  initializeRegister(REGISTER_T, dtReal34,    "2.2",   "",        0);
  initializeRegister(REGISTER_A, dtComplex16, "41.41", "41.4141", 0);
  initializeRegister(REGISTER_B, dtComplex34, "42.42", "42.4242", 0);
  initializeRegister(REGISTER_C, dtReal16,    "43",    "",        0);
  initializeRegister(REGISTER_D, dtReal34,    "44",    "",        0);

  userModeEnabled = false;
  shiftF = true;
  btnClicked(NULL, "02"); // KEY CC

  registerShouldBe(REGISTER_L, dtReal34,    "fnComplexCC(real34 + real16 ==> complex34 8)", "6",     "",        0);
  registerShouldBe(REGISTER_X, dtComplex34, "fnComplexCC(real34 + real16 ==> complex34 8)", "6",     "3",       0);
  registerShouldBe(REGISTER_Y, dtReal16,    "fnComplexCC(real34 + real16 ==> complex34 8)", "1.1",   "",        0);
  registerShouldBe(REGISTER_Z, dtReal34,    "fnComplexCC(real34 + real16 ==> complex34 8)", "2.2",   "",        0);
  registerShouldBe(REGISTER_T, dtComplex16, "fnComplexCC(real34 + real16 ==> complex34 8)", "41.41", "41.4141", 0);
  registerShouldBe(REGISTER_A, dtComplex34, "fnComplexCC(real34 + real16 ==> complex34 8)", "42.42", "42.4242", 0);
  registerShouldBe(REGISTER_B, dtReal16,    "fnComplexCC(real34 + real16 ==> complex34 8)", "43",    "",        0);
  registerShouldBe(REGISTER_C, dtReal34,    "fnComplexCC(real34 + real16 ==> complex34 8)", "44",    "",        0);
  registerShouldBe(REGISTER_C, dtReal34,    "fnComplexCC(real34 + real16 ==> complex34 8)", "44",    "",        0);


//real16ToDisplayString
//real34ToDisplayString
//roundRegister
//fnStore
//fnRecall
//fnSetFlag
//fnClearFlag
//fnFlipFlag






  //////////////////////////////
  // Convert real16 °C to °F
  #ifdef PC_BUILD
    printf("  Unit conversions\n");
  #endif

  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtCToF(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtCtoF()", "33.8", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtCtoF()", "1", "", 0);


  //////////////////////////////
  // Convert real34 °C to °F
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtCToF(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtCtoF()", "33.8", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtCtoF()", "1", "", 0);


  //////////////////////////////
  // Convert real16 °F to °C
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtFToC(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtFToC()", "-17.22222222222222", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtFToC()", "1", "", 0);


  //////////////////////////////
  // Convert real34 °F to °C
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtFToC(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtFToC()", "-17.22222222222222222222222222222222", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtFToC()", "1", "", 0);


  //////////////////////////////
  // Convert real16 year to second
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtYearS(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtYearS()", "31556952", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtYearS()", "1", "", 0);


  //////////////////////////////
  // Convert real34 year to second
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtYearS(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtYearS()", "31556952", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtYearS()", "1", "", 0);


  //////////////////////////////
  // Convert real16 second to year
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtYearS(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtYearS()", "3.168873850681143e-8", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtYearS()", "1", "", 0);


  //////////////////////////////
  // Convert real34 to second to year
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtYearS(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtYearS()", "3.168873850681143096456210346297070e-8", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtYearS()", "1", "", 0);


  //////////////////////////////
  // Convert real16 calorie to joule
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtCalJ(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtCalJ()", "4.1868", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtCalJ()", "1", "", 0);


  //////////////////////////////
  // Convert real34 calorie to joule
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtCalJ(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtCalJ()", "4.1868", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtCalJ()", "1", "", 0);


  //////////////////////////////
  // Convert real16 joule to calorie
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtCalJ(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtCalJ()", "0.2388458966274959", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtCalJ()", "1", "", 0);


  //////////////////////////////
  // Convert real34 joule to calorie
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtCalJ(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtCalJ()", "0.2388458966274959396197573325690265", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtCalJ()", "1", "", 0);


  //////////////////////////////
  // Convert real16 BTU to joule
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtBtuJ(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtBtuJ()", "1055.05585262", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtBtuJ()", "1", "", 0);


  //////////////////////////////
  // Convert real34 BTU to joule
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtBtuJ(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtBtuJ()", "1055.05585262", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtBtuJ()", "1", "", 0);


  //////////////////////////////
  // Convert real16 joule to BTU
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtBtuJ(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtBtuJ()", "9.478171203133172e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtBtuJ()", "1", "", 0);


  //////////////////////////////
  // Convert real34 joule to BTU
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtBtuJ(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtBtuJ()", "9.478171203133172000127850444756106e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtBtuJ()", "1", "", 0);


  //////////////////////////////
  // Convert real16 watt hour to joule
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtWhJ(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtWhJ()", "3600", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtWhJ()", "1", "", 0);


  //////////////////////////////
  // Convert real34 watt hour to joule
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtWhJ(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtWhJ()", "3600", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtWhJ()", "1", "", 0);


  //////////////////////////////
  // Convert real16 joule to watt hour
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtWhJ(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtWhJ()", "2.777777777777778e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtWhJ()", "1", "", 0);


  //////////////////////////////
  // Convert real34 joule to watt hour
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtWhJ(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtWhJ()", "2.777777777777777777777777777777778e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtWhJ()", "1", "", 0);


  //////////////////////////////
  // Convert real16 hpe to watt
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtHpeW(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtHpeW()", "746", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtHpeW()", "1", "", 0);


  //////////////////////////////
  // Convert real34 hpe to watt
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtHpeW(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtHpeW()", "746", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtHpeW()", "1", "", 0);


  //////////////////////////////
  // Convert real16 watt to hpe
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtHpeW(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtHpeW()", "1.340482573726542e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtHpeW()", "1", "", 0);


  //////////////////////////////
  // Convert real34 watt to hpe
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtHpeW(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtHpeW()", "1.340482573726541554959785522788204e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtHpeW()", "1", "", 0);


  //////////////////////////////
  // Convert real16 hpm to watt
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtHpmW(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtHpmW()", "735.49875", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtHpmW()", "1", "", 0);


  //////////////////////////////
  // Convert real34 hpm to watt
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtHpmW(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtHpmW()", "735.49875", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtHpmW()", "1", "", 0);


  //////////////////////////////
  // Convert real16 watt to hpm
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtHpmW(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtHpmW()", "1.359621617303904e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtHpmW()", "1", "", 0);


  //////////////////////////////
  // Convert real34 watt to hpm
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtHpmW(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtHpmW()", "1.35962161730390432342679032425276e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtHpmW()", "1", "", 0);


  //////////////////////////////
  // Convert real16 hpuk to watt
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtHpukW(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtHpukW()", "745.69987158227022", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtHpukW()", "1", "", 0);


  //////////////////////////////
  // Convert real34 hpuk to watt
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtHpukW(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtHpukW()", "745.69987158227022", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtHpukW()", "1", "", 0);


  //////////////////////////////
  // Convert real16 watt to hpuk
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtHpukW(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtHpukW()", "1.341022089595028e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtHpukW()", "1", "", 0);


  //////////////////////////////
  // Convert real34 watt to hpuk
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtHpukW(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtHpukW()", "1.341022089595027934323785572674645e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtHpukW()", "1", "", 0);


  //////////////////////////////
  // Convert real16 lbf to newton
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtLbfN(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtLbfN()", "4.4482216152605", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtLbfN()", "1", "", 0);


  //////////////////////////////
  // Convert real34 lbf to newton
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtLbfN(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtLbfN()", "4.4482216152605", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtLbfN()", "1", "", 0);


  //////////////////////////////
  // Convert real16 newton to lbf
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtLbfN(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtLbfN()", "0.2248089430997105", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtLbfN()", "1", "", 0);


  //////////////////////////////
  // Convert real34 newton to lbf
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtLbfN(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtLbfN()", "0.2248089430997104829100394134031775", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtLbfN()", "1", "", 0);


  //////////////////////////////
  // Convert real16 bar to pascal
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtBarPa(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtBarPa()", "100000", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtBarPa()", "1", "", 0);


  //////////////////////////////
  // Convert real34 bar to pascal
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtBarPa(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtBarPa()", "100000", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtBarPa()", "1", "", 0);


  //////////////////////////////
  // Convert real16 pascal to bar
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtBarPa(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtBarPa()", "1e-5", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtBarPa()", "1", "", 0);


  //////////////////////////////
  // Convert real34 pascal to bar
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtBarPa(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtBarPa()", "1e-5", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtBarPa()", "1", "", 0);


  //////////////////////////////
  // Convert real16 psi to pascal
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtPsiPa(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtPsiPa()", "6894.757293168361", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtPsiPa()", "1", "", 0);


  //////////////////////////////
  // Convert real34 psi to pascal
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtPsiPa(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtPsiPa()", "6894.757293168361336722673445346891", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtPsiPa()", "1", "", 0);


  //////////////////////////////
  // Convert real16 pascal to psi
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtPsiPa(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtPsiPa()", "1.450377377302092e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtPsiPa()", "1", "", 0);


  //////////////////////////////
  // Convert real34 pascal to psi
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtPsiPa(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtPsiPa()", "1.45037737730209215154241027951194e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtPsiPa()", "1", "", 0);


  //////////////////////////////
  // Convert real16 inhg to pascal
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtInhgPa(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtInhgPa()", "3386.388157894737", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtInhgPa()", "1", "", 0);


  //////////////////////////////
  // Convert real34 inhg to pascal
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtInhgPa(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtInhgPa()", "3386.388157894736842105263157894737", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtInhgPa()", "1", "", 0);


  //////////////////////////////
  // Convert real16 pascal to inhg
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtInhgPa(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtInhgPa()", "2.952998750803818e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtInhgPa()", "1", "", 0);


  //////////////////////////////
  // Convert real34 pascal to inhg
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtInhgPa(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtInhgPa()", "2.952998750803817916542815567743151e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtInhgPa()", "1", "", 0);


  //////////////////////////////
  // Convert real16 torr to pascal
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtTorrPa(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtTorrPa()", "133.3223684210526", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtTorrPa()", "1", "", 0);


  //////////////////////////////
  // Convert real34 torr to pascal
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtTorrPa(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtTorrPa()", "133.3223684210526315789473684210526", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtTorrPa()", "1", "", 0);


  //////////////////////////////
  // Convert real16 pascal to torr
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtTorrPa(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtTorrPa()", "7.500616827041698e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtTorrPa()", "1", "", 0);


  //////////////////////////////
  // Convert real34 pascal to torr
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtTorrPa(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtTorrPa()", "7.500616827041697508018751542067606e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtTorrPa()", "1", "", 0);


  //////////////////////////////
  // Convert real16 atm to pascal
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtAtmPa(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtAtmPa()", "101325", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtAtmPa()", "1", "", 0);


  //////////////////////////////
  // Convert real34 atm to pascal
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtAtmPa(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtAtmPa()", "101325", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtAtmPa()", "1", "", 0);


  //////////////////////////////
  // Convert real16 pascal to atm
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtAtmPa(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtAtmPa()", "9.869232667160128e-6", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtAtmPa()", "1", "", 0);


  //////////////////////////////
  // Convert real34 pascal to atm
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtAtmPa(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtAtmPa()", "9.8692326671601283000246730816679e-6", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtAtmPa()", "1", "", 0);


  //////////////////////////////
  // Convert real16 lbs to kg
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtLbsKg(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtLbsKg()", "0.45359237", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtLbsKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 lbs to kg
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtLbsKg(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtLbsKg()", "0.45359237", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtLbsKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 kg to lbs
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtLbsKg(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtLbsKg()", "2.204622621848776", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtLbsKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 kg to lbs
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtLbsKg(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtLbsKg()", "2.20462262184877580722973801345027", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtLbsKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 cwt to kg
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtCwtKg(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtCwtKg()", "50.80234544", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtCwtKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 cwt to kg
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtCwtKg(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtCwtKg()", "50.80234544", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtCwtKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 kg to cwt
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtCwtKg(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtCwtKg()", "1.968413055222121e-2", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtCwtKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 kg to cwt
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtCwtKg(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtCwtKg()", "1.968413055222121256455123226294884e-2", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtCwtKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 oz to kg
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtOzKg(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtOzKg()", "0.028349523125", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtOzKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 oz to kg
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtOzKg(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtOzKg()", "0.028349523125", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtOzKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 kg to oz
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtOzKg(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtOzKg()", "35.27396194958041", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtOzKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 kg to oz
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtOzKg(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtOzKg()", "35.27396194958041291567580821520433", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtOzKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 stone to kg
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtStonesKg(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtStonesKg()", "6.35029318", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtStonesKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 stone to kg
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtStonesKg(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtStonesKg()", "6.35029318", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtStonesKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 kg to stone
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtStonesKg(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtStonesKg()", "0.1574730444177697", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtStonesKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 kg to stone
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtStonesKg(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtStonesKg()", "0.1574730444177697005164098581035907", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtStonesKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 short cwt to kg
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtShortcwtKg(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtShortcwtKg()", "45.359237", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtShortcwtKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 short cwt to kg
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtShortcwtKg(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtShortcwtKg()", "45.359237", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtShortcwtKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 kg to short cwt
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtShortcwtKg(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtShortcwtKg()", "0.02204622621848776", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtShortcwtKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 kg to short cwt
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtShortcwtKg(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtShortcwtKg()", "0.0220462262184877580722973801345027", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtShortcwtKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 troz to kg
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtTrozKg(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtTrozKg()", "0.0311034768", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtTrozKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 troz to kg
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtTrozKg(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtTrozKg()", "0.0311034768", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtTrozKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 kg to troz
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtTrozKg(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtTrozKg()", "32.15074656862798", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtTrozKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 kg to troz
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtTrozKg(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtTrozKg()", "32.15074656862798052210034602948311", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtTrozKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 tons to kg
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtTonsKg(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtTonsKg()", "1016.0469088", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtTonsKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 tons to kg
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtTonsKg(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtTonsKg()", "1016.0469088", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtTonsKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 kg to tons
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtTonsKg(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtTonsKg()", "9.842065276110606e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtTonsKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 kg to tons
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtTonsKg(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtTonsKg()", "9.842065276110606282275616131474421e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtTonsKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 short tons to kg
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtShorttonsKg(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtShorttonsKg()", "907.18474", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtShorttonsKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 short tons to kg
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtShorttonsKg(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtShorttonsKg()", "907.18474", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtShorttonsKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 kg to short tons
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtShorttonsKg(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtShorttonsKg()", "1.102311310924388e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtShorttonsKg()", "1", "", 0);


  //////////////////////////////
  // Convert real34 kg to short tons
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtShorttonsKg(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtShorttonsKg()", "1.102311310924387903614869006725135e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtShorttonsKg()", "1", "", 0);


  //////////////////////////////
  // Convert real16 au to m
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtAuM(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtAuM()", "149597870700", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtAuM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 au to m
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtAuM(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtAuM()", "149597870700", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtAuM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m to au
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtAuM(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtAuM()", "6.684587122268445e-12", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtAuM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m to au
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtAuM(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtAuM()", "6.684587122268445495995953370210636e-12", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtAuM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 miles to m
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtMiM(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtMiM()", "1609.344", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtMiM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 miles to m
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtMiM(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtMiM()", "1609.344", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtMiM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m to miles
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtMiM(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtMiM()", "6.213711922373340e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtMiM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m to miles
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtMiM(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtMiM()", "6.213711922373339696174341843633182e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtMiM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 ly to m
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtLyM(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtLyM()", "9460730472580800", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtLyM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 ly to m
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtLyM(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtLyM()", "9460730472580800", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtLyM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m to ly
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtLyM(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtLyM()", "1.057000834024615e-16", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtLyM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m to ly
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtLyM(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtLyM()", "1.057000834024615463709460524485127e-16", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtLyM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 nmi to m
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtNmiM(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtNmiM()", "1852", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtNmiM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 nmi to m
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtNmiM(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtNmiM()", "1852", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtNmiM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m to nmi
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtNmiM(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtNmiM()", "5.399568034557235e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtNmiM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m to nmi
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtNmiM(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtNmiM()", "5.399568034557235421166306695464363e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtNmiM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 ft to m
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtFtM(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtFtM()", "0.3048", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtFtM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 ft to m
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtFtM(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtFtM()", "0.3048", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtFtM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m to ft
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtFtM(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtFtM()", "3.280839895013123", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtFtM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m to ft
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtFtM(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtFtM()", "3.28083989501312335958005249343832", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtFtM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 pc to m
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtPcM(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtPcM()", "30856775814913670", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtPcM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 pc to m
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtPcM(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtPcM()", "30856775814913672.78913937957796472", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtPcM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m to pc
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtPcM(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtPcM()", "3.240779289444365e-17", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtPcM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m to pc
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtPcM(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtPcM()", "3.240779289444365023237687716352957e-17", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtPcM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 inches to m
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtInchesM(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtInchesM()", "0.0254", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtInchesM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 inches to m
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtInchesM(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtInchesM()", "0.0254", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtInchesM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m to inches
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtInchesM(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtInchesM()", "39.37007874015748", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtInchesM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m to inches
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtInchesM(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtInchesM()", "39.37007874015748031496062992125984", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtInchesM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 sfeetus to m
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtSfeetM(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtSfeetM()", "0.3048006096012192", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtSfeetM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 sfeetus to m
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtSfeetM(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtSfeetM()", "0.3048006096012192024384048768097536", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtSfeetM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m to sfeetus
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtSfeetM(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtSfeetM()", "3.280833333333333", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtSfeetM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m to sfeetus
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtSfeetM(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtSfeetM()", "3.280833333333333333333333333333334", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtSfeetM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 yards to m
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtYardsM(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtYardsM()", "0.9144", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtYardsM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 yards to m
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtYardsM(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtYardsM()", "0.9144", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtYardsM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m to yards
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtYardsM(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtYardsM()", "1.093613298337708", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtYardsM()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m to yards
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtYardsM(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtYardsM()", "1.09361329833770778652668416447944", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtYardsM()", "1", "", 0);


  ////////////////////////////////
  // Convert real16 points to m
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtPointsM(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtPointsM()", "352.7777777777778e-6", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtPointsM()", "1", "", 0);


  ////////////////////////////////
  // Convert real34 points to m
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtPointsM(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtPointsM()", "352.7777777777777777777777777777778e-6", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtPointsM()", "1", "", 0);


  ////////////////////////////////
  // Convert real16 m to points
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtPointsM(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtPointsM()", "2834.645669291339", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtPointsM()", "1", "", 0);


  ////////////////////////////////
  // Convert real34 m to points
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtPointsM(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtPointsM()", "2834.645669291338582677165354330708", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtPointsM()", "1", "", 0);


  //////////////////////////////
  // Convert real16 gluk to m3
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtGalukM3(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtGalukM3()", "4.54609e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtGalukM3()", "1", "", 0);


  //////////////////////////////
  // Convert real34 gluk to m3
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtGalukM3(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtGalukM3()", "4.54609e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtGalukM3()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m3 to gluk
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtGalukM3(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtGalukM3()", "219.9692482990878", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtGalukM3()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m3 to gluk
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtGalukM3(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtGalukM3()", "219.9692482990877875273036829451243", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtGalukM3()", "1", "", 0);


  //////////////////////////////
  // Convert real16 glus to m3
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtGalusM3(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtGalusM3()", "3.785411784e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtGalusM3()", "1", "", 0);


  //////////////////////////////
  // Convert real34 glus to m3
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtGalusM3(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtGalusM3()", "3.785411784e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtGalusM3()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m3 to glus
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtGalusM3(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtGalusM3()", "264.1720523581484", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtGalusM3()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m3 to glus
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtGalusM3(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtGalusM3()", "264.1720523581484153798999216091625", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtGalusM3()", "1", "", 0);


  //////////////////////////////
  // Convert real16 flozuk to m3
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtFlozukM3(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtFlozukM3()", "28.4130625e-6", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtFlozukM3()", "1", "", 0);


  //////////////////////////////
  // Convert real34 flozuk to m3
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtFlozukM3(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtFlozukM3()", "28.4130625e-6", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtFlozukM3()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m3 to flozuk
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtFlozukM3(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtFlozukM3()", "35195.07972785405", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtFlozukM3()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m3 to flozuk
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtFlozukM3(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtFlozukM3()", "35195.07972785404600436858927121988", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtFlozukM3()", "1", "", 0);


  //////////////////////////////
  // Convert real16 flozus to m3
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtFlozusM3(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtFlozusM3()", "29.5735295625e-6", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtFlozusM3()", "1", "", 0);


  //////////////////////////////
  // Convert real34 flozus to m3
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtFlozusM3(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtFlozusM3()", "29.5735295625e-6", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtFlozusM3()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m3 to flozus
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtFlozusM3(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtFlozusM3()", "33814.02270184300", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtFlozusM3()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m3 to flozus
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtFlozusM3(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtFlozusM3()", "33814.0227018429971686271899659728", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtFlozusM3()", "1", "", 0);


  //////////////////////////////
  // Convert real16 quart to m3
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtQuartM3(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtQuartM3()", "1.1365225e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtQuartM3()", "1", "", 0);


  //////////////////////////////
  // Convert real34 quart to m3
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtQuartM3(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtQuartM3()", "1.1365225e-3", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtQuartM3()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m3 to quart
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtQuartM3(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtQuartM3()", "879.8769931963512", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtQuartM3()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m3 to quart
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtQuartM3(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtQuartM3()", "879.8769931963511501092147317804971", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtQuartM3()", "1", "", 0);


  //////////////////////////////
  // Convert real16 acres to m2
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtAcresM2(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtAcresM2()", "4046.8564224", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtAcresM2()", "1", "", 0);


  //////////////////////////////
  // Convert real34 acres to m2
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtAcresM2(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtAcresM2()", "4046.8564224", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtAcresM2()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m2 to acres
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtAcresM2(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtAcresM2()", "2.471053814671653e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtAcresM2()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m2 to acres
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtAcresM2(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtAcresM2()", "2.471053814671653422482439291988063e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtAcresM2()", "1", "", 0);


  //////////////////////////////
  // Convert real16 acresus to m2
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtAcresusM2(multiply);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtAcresusM2()", "4046.872609874252", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtAcresusM2()", "1", "", 0);


  //////////////////////////////
  // Convert real34 acresus to m2
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtAcresusM2(multiply);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtAcresusM2()", "4046.87260987425200656852926609079", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtAcresusM2()", "1", "", 0);


  //////////////////////////////
  // Convert real16 m2 to acresus
  initializeRegister(REGISTER_X, dtReal16, "1", "", 0);

  fnCvtAcresusM2(divide);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtAcresusM2()", "2.471043930466279e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtAcresusM2()", "1", "", 0);


  //////////////////////////////
  // Convert real34 m2 to acresus
  initializeRegister(REGISTER_X, dtReal34, "1", "", 0);

  fnCvtAcresusM2(divide);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtAcresusM2()", "2.471043930466278951127435975920825e-4", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtAcresusM2()", "1", "", 0);


  //////////////////////////////
  // Convert real16 power ratio to dB
  initializeRegister(REGISTER_X, dtReal16, "2", "", 0);

  fnCvtRatioDb(10);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtRatioDb(10)", "3.010299956639812", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtRatioDb(10)", "2", "", 0);


  //////////////////////////////
  // Convert real34 power ratio to dB
  initializeRegister(REGISTER_X, dtReal34, "2", "", 0);

  fnCvtRatioDb(10);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtRatioDb(10)", "3.01029995663981195213738894724493", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtRatioDb(10)", "2", "", 0);


  //////////////////////////////
  // Convert real16 field ratio to dB
  initializeRegister(REGISTER_X, dtReal16, "2", "", 0);

  fnCvtRatioDb(20);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtRatioDb(20)", "6.020599913279624", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtRatioDb(20)", "2", "", 0);


  //////////////////////////////
  // Convert real34 field ratio to dB
  initializeRegister(REGISTER_X, dtReal34, "2", "", 0);

  fnCvtRatioDb(20);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtRatioDb(20)", "6.02059991327962390427477789448986", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtRatioDb(20)", "2", "", 0);


  //////////////////////////////
  // Convert real16 dB to power ratio
  initializeRegister(REGISTER_X, dtReal16, "2", "", 0);

  fnCvtDbRatio(10);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtDbRatio(10)", "1.584893192461113", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtDbRatio(10)", "2", "", 0);


  //////////////////////////////
  // Convert real34 dB to power ratio
  initializeRegister(REGISTER_X, dtReal34, "2", "", 0);

  fnCvtDbRatio(10);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtDbRatio(10)", "1.584893192461113485202101373391507", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtDbRatio(10)", "2", "", 0);


  //////////////////////////////
  // Convert real16 dB to field ratio
  initializeRegister(REGISTER_X, dtReal16, "2", "", 0);

  fnCvtDbRatio(20);

  registerShouldBe(REGISTER_X, dtReal16, "fnCvtDbRatio(20)", "1.258925411794167", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "fnCvtDbRatio(20)", "2", "", 0);


  //////////////////////////////
  // Convert real34 dB to field ratio
  initializeRegister(REGISTER_X, dtReal34, "2", "", 0);

  fnCvtDbRatio(20);

  registerShouldBe(REGISTER_X, dtReal34, "fnCvtDbRatio(20)", "1.258925411794167210423954106395801", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "fnCvtDbRatio(20)", "2", "", 0);


  /////////////////////////////
  // Convert Degrees to ...
  #ifdef PC_BUILD
    printf("  Angle conversions\n");
  #endif

  angularMode = AM_DEGREE;

  initializeRegister(REGISTER_X, dtReal16, "77.77", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DEGREE);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(degree -> degree)", "77.77", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "77.77", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DMS);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(degree -> d.ms)", "77.4612", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "77.77", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_GRAD);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(degree -> grad)", "86.41111111111111", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "77.77", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_MULTPI);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(degree -> multpi)", "0.4320555555555556", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "77.77", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_RADIAN);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(degree -> radian)", "1.357342559275990", "", 0);


  initializeRegister(REGISTER_X, dtReal34, "77.77", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DEGREE);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(degree -> degree)", "77.77", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "77.77", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DMS);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(degree -> d.ms)", "77.4612", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "77.77", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_GRAD);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(degree -> grad)", "86.41111111111111111111111111111111", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "77.77", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_MULTPI);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(degree -> multpi)", "0.4320555555555555555555555555555556", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "77.77", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_RADIAN);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(degree -> radian)", "1.357342559275990111973554310653594", "", 0);


  /////////////////////////////
  // Convert Dms to ...
  angularMode = AM_DMS;

  initializeRegister(REGISTER_X, dtReal16, "55.5555555", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DEGREE);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(d.ms -> degree)", "55.93209861111111", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "55.5555555", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DMS);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(d.ms -> d.ms)", "55.5555555", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "55.5555555", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_GRAD);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(d.ms -> grad)", "62.14677623456790", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "55.5555555", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_MULTPI);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(d.ms -> multpi)", "0.3107338811728395", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "55.5555555", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_RADIAN);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(d.ms -> radian)", "0.9761992783140364", "", 0);


  initializeRegister(REGISTER_X, dtReal34, "55.5555555", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DEGREE);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(d.ms -> degree)", "55.93209861111111111111111111111111", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "55.5555555", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DMS);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(d.ms -> d.ms)", "55.5555555", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "55.5555555", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_GRAD);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(d.ms -> grad)", "62.1467762345679012345679012345679", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "55.5555555", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_MULTPI);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(d.ms -> multpi)", "0.3107338811728395061728395061728395", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "55.5555555", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_RADIAN);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(d.ms -> radian)", "0.9761992783140363578177359648261144", "", 0);


  /////////////////////////////
  // Convert Grad to ...
  angularMode = AM_GRAD;

  initializeRegister(REGISTER_X, dtReal16, "99.99", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DEGREE);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(grad -> degree)", "89.991", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "99.99", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DMS);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(grad -> d.ms)", "89.59276", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "99.99", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_GRAD);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(grad -> grad)", "99.99", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "99.99", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_MULTPI);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(grad -> multpi)", "0.49995", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "99.99", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_RADIAN);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(grad -> radian)", "1.570639247162217", "", 0);


  initializeRegister(REGISTER_X, dtReal34, "99.99", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DEGREE);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(grad -> degree)", "89.991", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "99.99", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DMS);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(grad -> d.ms)", "89.59276", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "99.99", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_GRAD);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(grad -> grad)", "99.99", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "99.99", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_MULTPI);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(grad -> multpi)", "0.49995", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "99.99", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_RADIAN);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(grad -> radian)", "1.570639247162217129569398559470587", "", 0);


  /////////////////////////////
  // Convert MultPi to ...
  angularMode = AM_MULTPI;

  initializeRegister(REGISTER_X, dtReal16, "1.2345", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DEGREE);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(multpi -> degree)", "222.21", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "1.2345", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DMS);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(multpi -> d.ms)", "222.1236", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "1.2345", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_GRAD);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(multpi -> grad)", "246.9", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "1.2345", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_MULTPI);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(multpi -> multpi)", "1.2345", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "1.2345", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_RADIAN);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(multpi -> radian)", "3.878296130856600", "", 0);


  initializeRegister(REGISTER_X, dtReal34, "1.2345", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DEGREE);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(multpi -> degree)", "222.21", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "1.2345", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DMS);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(multpi -> d.ms)", "222.1236", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "1.2345", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_GRAD);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(multpi -> grad)", "246.9", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "1.2345", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_MULTPI);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(multpi -> multpi)", "1.2345", "", 0);

  initializeRegister(REGISTER_X, dtReal34, "1.2345", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_RADIAN);
  registerShouldBe(REGISTER_X, dtReal34, "convertAngleFromTo(multpi -> radian)", "3.878296130856599752882133256658546", "", 0);


  /////////////////////////////
  // Convert Radian to ...
  angularMode = AM_RADIAN;

  initializeRegister(REGISTER_X, dtReal16, "0.54321", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DEGREE);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(radian -> degree)", "31.12364038930145", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "0.54321", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DMS);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(radian -> d.ms)", "31.07251054014852", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "0.54321", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_GRAD);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(radian -> grad)", "34.58182265477939", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "0.54321", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_MULTPI);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(radian -> multpi)", "0.1729091132738969", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "0.54321", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_RADIAN);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(radian -> radian)", "0.54321", "", 0);


  initializeRegister(REGISTER_X, dtReal16, "0.54321", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DEGREE);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(radian -> degree)", "31.12364038930144752348552567657006", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "0.54321", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_DMS);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(radian -> d.ms)", "31.07251054014852110845478924356522", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "0.54321", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_GRAD);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(radian -> grad)", "34.58182265477938613720613964063342", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "0.54321", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_MULTPI);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(radian -> multpi)", "0.1729091132738969306860306982031670", "", 0);

  initializeRegister(REGISTER_X, dtReal16, "0.54321", "", 0);
  convertRegisterAngleFromTo(REGISTER_X, angularMode, AM_RADIAN);
  registerShouldBe(REGISTER_X, dtReal16, "convertAngleFromTo(radian -> radian)", "0.54321", "", 0);

  angularMode = AM_DEGREE;

  /////////////////////////////////////////
  // Convert big integer to real16
  #ifdef PC_BUILD
    printf("  Convert big integer to real16 and real34\n");
  #endif

  initializeRegister(REGISTER_X, dtBigInteger, "-1254789565424599", "", 0);
  convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  registerShouldBe(REGISTER_X, dtReal16, "convertBigIntegerRegisterToReal16Register", "-1254789565424599.0", "", 0);


  /////////////////////////////////////////
  // Convert big integer to real34
  initializeRegister(REGISTER_X, dtBigInteger, "-12547895548542665424599", "", 0);
  convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  registerShouldBe(REGISTER_X, dtReal34, "convertBigIntegerRegisterToReal34Register", "-12547895548542665424599.0", "", 0);


  /////////////////////////////////////////
  // Convert integer to real16
  #ifdef PC_BUILD
    printf("  Convert integer to real16 and real34\n");
  #endif

  fnIntegerMode(SIM_UNSIGN);
  fnSetWordSize(12);
  initializeRegister(REGISTER_X, dtSmallInteger, "147", "", 10);
  registerShouldBe(REGISTER_X, dtSmallInteger, "convertSmallIntegerRegisterToRea16lRegister (SIM_UNSIGN)", "147", "", 10);
  convertSmallIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  registerShouldBe(REGISTER_X, dtReal16, "convertSmallIntegerRegisterToRea16lRegister (SIM_UNSIGN)", "147", "", 0);

  fnIntegerMode(SIM_2COMPL);
  fnSetWordSize(12);
  initializeRegister(REGISTER_X, dtSmallInteger, "3949", "", 10);
  registerShouldBe(REGISTER_X, dtSmallInteger, "convertSmallIntegerRegisterToRea16lRegister (SIM_2COMPL)", "3949", "", 10);
  convertSmallIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  registerShouldBe(REGISTER_X, dtReal16, "convertSmallIntegerRegisterToRea16lRegister (SIM_2COMPL)", "-147", "", 0);

  fnIntegerMode(SIM_1COMPL);
  fnSetWordSize(12);
  initializeRegister(REGISTER_X, dtSmallInteger, "3948", "", 10);
  registerShouldBe(REGISTER_X, dtSmallInteger, "convertSmallIntegerRegisterToRea16lRegister (SIM_1COMPL)", "3948", "", 10);
  convertSmallIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  registerShouldBe(REGISTER_X, dtReal16, "convertSmallIntegerRegisterToRea16lRegister (SIM_1COMPL)", "-147", "", 0);

  fnIntegerMode(SIM_SIGNMT);
  fnSetWordSize(12);
  initializeRegister(REGISTER_X, dtSmallInteger, "2195", "", 10);
  registerShouldBe(REGISTER_X, dtSmallInteger, "convertSmallIntegerRegisterToRea16lRegister (SIM_SIGNMT)", "2195", "", 10);
  convertSmallIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  registerShouldBe(REGISTER_X, dtReal16, "convertSmallIntegerRegisterToRea16lRegister (SIM_SIGNMT)", "-147", "", 0);

  fnIntegerMode(SIM_UNSIGN);
  fnSetWordSize(24);
  initializeRegister(REGISTER_X, dtSmallInteger, "3316774", "", 10);
  registerShouldBe(REGISTER_X, dtSmallInteger, "convertSmallIntegerRegisterToReal34Register (SIM_UNSIGN)", "3316774", "", 10);
  convertSmallIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  registerShouldBe(REGISTER_X, dtReal34, "convertSmallIntegerRegisterToReal34Register (SIM_UNSIGN)", "3316774", "", 0);

  fnIntegerMode(SIM_2COMPL);
  fnSetWordSize(24);
  initializeRegister(REGISTER_X, dtSmallInteger, "13460442", "", 10);
  registerShouldBe(REGISTER_X, dtSmallInteger, "convertSmallIntegerRegisterToReal34Register (SIM_2COMPL)", "13460442", "", 10);
  convertSmallIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  registerShouldBe(REGISTER_X, dtReal34, "convertSmallIntegerRegisterToReal34Register (SIM_2COMPL)", "-3316774", "", 0);

  fnIntegerMode(SIM_1COMPL);
  fnSetWordSize(24);
  initializeRegister(REGISTER_X, dtSmallInteger, "13460441", "", 10);
  registerShouldBe(REGISTER_X, dtSmallInteger, "convertSmallIntegerRegisterToReal34Register (SIM_1COMPL)", "13460441", "", 10);
  convertSmallIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  registerShouldBe(REGISTER_X, dtReal34, "convertSmallIntegerRegisterToReal34Register (SIM_1COMPL)", "-3316774", "", 0);

  fnIntegerMode(SIM_SIGNMT);
  fnSetWordSize(24);
  initializeRegister(REGISTER_X, dtSmallInteger, "11705382", "", 10);
  registerShouldBe(REGISTER_X, dtSmallInteger, "convertSmallIntegerRegisterToReal34Register (SIM_SIGNMT)", "11705382", "", 10);
  convertSmallIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  registerShouldBe(REGISTER_X, dtReal34, "convertSmallIntegerRegisterToReal34Register (SIM_SIGNMT)", "-3316774", "", 0);



  ////////////////////////////
  // addRe16Re16 -10 + -10
  #ifdef PC_BUILD
    printf("  4 operations real16 real16\n");
  #endif

  initializeRegister(REGISTER_Y, dtReal16, "-10", "", 0);
  initializeRegister(REGISTER_X, dtReal16, "-10", "", 0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16, "addRe16Re16()", "-20", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "addRe16Re16()", "-10", "", 0);



  ////////////////////////////
  // subRe16Re16 -10 - -10
  initializeRegister(REGISTER_Y, dtReal16, "-10", "", 0);
  initializeRegister(REGISTER_X, dtReal16, "-10", "", 0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16, "subRe16Re16()", "0",   "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "subRe16Re16()", "-10", "", 0);



  ////////////////////////////
  // mulRe16Re16 -10 × -10
  initializeRegister(REGISTER_Y, dtReal16, "-10", "", 0);
  initializeRegister(REGISTER_X, dtReal16, "-10", "", 0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16, "mulRe16Re16()", "100", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "mulRe16Re16()", "-10", "", 0);



  ////////////////////////////
  // divRe16Re16 -10 ÷ -10
  initializeRegister(REGISTER_Y, dtReal16, "-10", "", 0);
  initializeRegister(REGISTER_X, dtReal16, "-10", "", 0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16, "divRe16Re16()", "1",   "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "divRe16Re16()", "-10", "", 0);



  ////////////////////////////
  // addRe16Co16 -10 + -1-2i
  #ifdef PC_BUILD
    printf("  4 operations real16 complex16\n");
  #endif

  initializeRegister(REGISTER_Y, dtReal16,    "-10", "",   0);
  initializeRegister(REGISTER_X, dtComplex16, "-1",  "-2", 0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "addRe16Co16()", "-11", "-2", 0);
  registerShouldBe(REGISTER_L, dtComplex16, "addRe16Co16()", "-1",  "-2", 0);



  ////////////////////////////
  // subRe16Co16 -10 - -1-2i
  initializeRegister(REGISTER_Y, dtReal16,    "-10", "",   0);
  initializeRegister(REGISTER_X, dtComplex16, "-1",  "-2", 0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "subRe16Co16()", "-9", "2",  0);
  registerShouldBe(REGISTER_L, dtComplex16, "subRe16Co16()", "-1", "-2", 0);



  ////////////////////////////
  // mulRe16Co16 -10 × -1-2i
  initializeRegister(REGISTER_Y, dtReal16,    "-10", "",   0);
  initializeRegister(REGISTER_X, dtComplex16, "-1",  "-2", 0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "mulRe16Co16()", "10", "20", 0);
  registerShouldBe(REGISTER_L, dtComplex16, "mulRe16Co16()", "-1", "-2", 0);



  ////////////////////////////
  // divRe16Co16 -10 ÷ -1-2i
  initializeRegister(REGISTER_Y, dtReal16,    "-10", "",   0);
  initializeRegister(REGISTER_X, dtComplex16, "-1",  "-2", 0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "divRe16Co16()", "2", "-4", 0);
  registerShouldBe(REGISTER_L, dtComplex16, "divRe16Co16()", "-1","-2", 0);



  ////////////////////////////
  // addRe16Re34 -10 + -20
  #ifdef PC_BUILD
    printf("  4 operations real16 real34\n");
  #endif

  initializeRegister(REGISTER_Y, dtReal16, "-10", "", 0);
  initializeRegister(REGISTER_X, dtReal34, "-20", "", 0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "addRe16Re34()", "-30", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "addRe16Re34()", "-20", "", 0);



  ////////////////////////////
  // subRe16Re34 -10 - -20
  initializeRegister(REGISTER_Y, dtReal16, "-10", "", 0);
  initializeRegister(REGISTER_X, dtReal34, "-20", "", 0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "subRe16Re34()", "10",  "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "subRe16Re34()", "-20", "", 0);



  ////////////////////////////
  // mulRe16Re34 -10 × -20
  initializeRegister(REGISTER_Y, dtReal16, "-10", "", 0);
  initializeRegister(REGISTER_X, dtReal34, "-20", "", 0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "mulRe16Re34()", "200", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "mulRe16Re34()", "-20", "", 0);



  ////////////////////////////
  // divRe16Re34 -10 ÷ -20
  initializeRegister(REGISTER_Y, dtReal16, "-10", "", 0);
  initializeRegister(REGISTER_X, dtReal34, "-20", "", 0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "divRe16Re34()", "0.5", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "divRe16Re34()", "-20", "", 0);



  ////////////////////////////
  // addRe16Co34 -10 + -2-4i
  #ifdef PC_BUILD
    printf("  4 operations real16 complex34\n");
  #endif

  initializeRegister(REGISTER_Y, dtReal16,    "-10", "",   0);
  initializeRegister(REGISTER_X, dtComplex34, "-2",  "-4", 0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "addRe16Co34()", "-12", "-4", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "addRe16Co34()", "-2",  "-4", 0);



  ////////////////////////////
  // subRe16Co34 -10 - -2-4i
  initializeRegister(REGISTER_Y, dtReal16,    "-10", "",   0);
  initializeRegister(REGISTER_X, dtComplex34, "-2",  "-4", 0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "subRe16Co34()", "-8", "4",  0);
  registerShouldBe(REGISTER_L, dtComplex34, "subRe16Co34()", "-2", "-4", 0);



  ////////////////////////////
  // mulRe16Co34 -10 × -2-4i
  initializeRegister(REGISTER_Y, dtReal16,    "-10", "",   0);
  initializeRegister(REGISTER_X, dtComplex34, "-2",  "-4", 0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "mulRe16Co34()", "20", "40", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "mulRe16Co34()", "-2", "-4", 0);



  ////////////////////////////
  // divRe16Co34 -10 ÷ -2-4i
  initializeRegister(REGISTER_Y, dtReal16,    "-10", "",   0);
  initializeRegister(REGISTER_X, dtComplex34, "-2",  "-4", 0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "divRe16Co34()", "1",  "-2", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "divRe16Co34()", "-2", "-4", 0);



  ////////////////////////////
  // addCo16Re16 -1-2i + -10
  #ifdef PC_BUILD
    printf("  4 operations complex16 real16\n");
  #endif

  initializeRegister(REGISTER_Y, dtComplex16, "-1",  "-2", 0);
  initializeRegister(REGISTER_X, dtReal16,    "-10", "",   0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "addCo16Re16()", "-11", "-2", 0);
  registerShouldBe(REGISTER_L, dtReal16,    "addCo16Re16()", "-10", "",   0);



  ////////////////////////////
  // subCo16Re16 -1-2i - -10
  initializeRegister(REGISTER_Y, dtComplex16, "-1",  "-2", 0);
  initializeRegister(REGISTER_X, dtReal16,    "-10", "",   0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "subCo16Re16()", "9",   "-2", 0);
  registerShouldBe(REGISTER_L, dtReal16,    "subCo16Re16()", "-10", "",   0);



  ////////////////////////////
  // mulCo16Re16 -1-2i × -10
  initializeRegister(REGISTER_Y, dtComplex16, "-1",  "-2", 0);
  initializeRegister(REGISTER_X, dtReal16,    "-10", "",   0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "mulCo16Re16()", "10",  "20", 0);
  registerShouldBe(REGISTER_L, dtReal16,    "mulCo16Re16()", "-10", "",   0);



  ////////////////////////////
  // divCo16Re16 -1-2i ÷ -10
  initializeRegister(REGISTER_Y, dtComplex16, "-1",  "-2", 0);
  initializeRegister(REGISTER_X, dtReal16,    "-10", "",   0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "divCo16Re16()", "0.1", "0.2", 0);
  registerShouldBe(REGISTER_L, dtReal16,    "divCo16Re16()", "-10", "",    0);



  ////////////////////////////
  // addCo16Co16 -1-2i + -1-2i
  #ifdef PC_BUILD
    printf("  4 operations complex16 complex16\n");
  #endif

  initializeRegister(REGISTER_Y, dtComplex16, "-1", "-2", 0);
  initializeRegister(REGISTER_X, dtComplex16, "-1", "-2", 0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "addCo16Co16()", "-2", "-4", 0);
  registerShouldBe(REGISTER_L, dtComplex16, "addCo16Co16()", "-1", "-2", 0);



  ////////////////////////////
  // subCo16Co16 -1-2i - -1-2i
  initializeRegister(REGISTER_Y, dtComplex16, "-1", "-2", 0);
  initializeRegister(REGISTER_X, dtComplex16, "-1", "-2", 0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "subCo16Co16()", "0",  "0",  0);
  registerShouldBe(REGISTER_L, dtComplex16, "subCo16Co16()", "-1", "-2", 0);



  ////////////////////////////
  // mulCo16Co16 -1-2i × -1-2i
  initializeRegister(REGISTER_Y, dtComplex16, "-1", "-2", 0);
  initializeRegister(REGISTER_X, dtComplex16, "-1", "-2", 0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "mulCo16Co16()", "-3", "4",  0);
  registerShouldBe(REGISTER_L, dtComplex16, "mulCo16Co16()", "-1", "-2", 0);



  ////////////////////////////
  // divCo16Co16 -1-2i ÷ -1-2i
  initializeRegister(REGISTER_Y, dtComplex16, "-1", "-2", 0);
  initializeRegister(REGISTER_X, dtComplex16, "-1", "-2", 0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex16, "divCo16Co16()", "1",  "0",  0);
  registerShouldBe(REGISTER_L, dtComplex16, "divCo16Co16()", "-1", "-2", 0);



  ////////////////////////////
  // addCo16Re34 -1-2i + -20
  #ifdef PC_BUILD
    printf("  4 operations complex16 real34\n");
  #endif

  initializeRegister(REGISTER_Y, dtComplex16, "-1",  "-2", 0);
  initializeRegister(REGISTER_X, dtReal34,    "-20", "",   0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "addCo16Re34()", "-21", "-2", 0);
  registerShouldBe(REGISTER_L, dtReal34,    "addCo16Re34()", "-20", "",   0);



  ////////////////////////////
  // subCo16Re34 -1-2i - -20
  initializeRegister(REGISTER_Y, dtComplex16, "-1",  "-2", 0);
  initializeRegister(REGISTER_X, dtReal34,    "-20", "",   0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "subCo16Re34()", "19",  "-2", 0);
  registerShouldBe(REGISTER_L, dtReal34,    "subCo16Re34()", "-20", "",   0);



  ////////////////////////////
  // mulCo16Re34 -1-2i × -20
  initializeRegister(REGISTER_Y, dtComplex16, "-1",  "-2", 0);
  initializeRegister(REGISTER_X, dtReal34,    "-20", "",   0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "mulCo16Re34()", "20",  "40", 0);
  registerShouldBe(REGISTER_L, dtReal34,    "mulCo16Re34()", "-20", "",   0);



  ////////////////////////////
  // divCo16Re34 -1-2i ÷ -20
  initializeRegister(REGISTER_Y, dtComplex16, "-1",  "-2", 0);
  initializeRegister(REGISTER_X, dtReal34,    "-20", "",   0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "divCo16Re34()", "0.05", "0.1", 0);
  registerShouldBe(REGISTER_L, dtReal34,    "divCo16Re34()", "-20",  "",    0);



  ////////////////////////////
  // addCo16Co34 -1-2i + -2-4i
  #ifdef PC_BUILD
    printf("  4 operations complex16 complex34\n");
  #endif

  initializeRegister(REGISTER_Y, dtComplex16, "-1", "-2", 0);
  initializeRegister(REGISTER_X, dtComplex34, "-2", "-4", 0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "addCo16Co34()", "-3", "-6", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "addCo16Co34()", "-2", "-4", 0);



  ////////////////////////////
  // subCo16Co34 -1-2i - -2-4i
  initializeRegister(REGISTER_Y, dtComplex16, "-1", "-2", 0);
  initializeRegister(REGISTER_X, dtComplex34, "-2", "-4", 0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "subCo16Co34()", "1",  "2",  0);
  registerShouldBe(REGISTER_L, dtComplex34, "subCo16Co34()", "-2", "-4", 0);



  ////////////////////////////
  // mulCo16Co34 -1-2i × -2-4i
  initializeRegister(REGISTER_Y, dtComplex16, "-1", "-2", 0);
  initializeRegister(REGISTER_X, dtComplex34, "-2", "-4", 0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "mulCo16Co34()", "-6", "8",  0);
  registerShouldBe(REGISTER_L, dtComplex34, "mulCo16Co34()", "-2", "-4", 0);



  ////////////////////////////
  // divCo16Co34 -1-2i ÷ -2-4i
  initializeRegister(REGISTER_Y, dtComplex16, "-1", "-2", 0);
  initializeRegister(REGISTER_X, dtComplex34, "-2", "-4", 0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "divCo16Co34()", "0.5", "0",  0);
  registerShouldBe(REGISTER_L, dtComplex34, "divCo16Co34()", "-2",  "-4", 0);



  ////////////////////////////
  // addRe34Re16 -20 + -10
  #ifdef PC_BUILD
    printf("  4 operations real34 real16\n");
  #endif

  initializeRegister(REGISTER_Y, dtReal34, "-20", "", 0);
  initializeRegister(REGISTER_X, dtReal16, "-10", "", 0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "addRe34Re16()", "-30", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "addRe34Re16()", "-10", "", 0);



  ////////////////////////////
  // subRe34Re16 -20 - -10
  initializeRegister(REGISTER_Y, dtReal34, "-20", "", 0);
  initializeRegister(REGISTER_X, dtReal16, "-10", "", 0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "subRe34Re16()", "-10", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "subRe34Re16()", "-10", "", 0);



  ////////////////////////////
  // mulRe34Re16 -20 × -10
  initializeRegister(REGISTER_Y, dtReal34, "-20", "", 0);
  initializeRegister(REGISTER_X, dtReal16, "-10", "", 0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "mulRe34Re16()", "200", "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "mulRe34Re16()", "-10", "", 0);



  ////////////////////////////
  // divRe34Re16 -20 ÷ -10
  initializeRegister(REGISTER_Y, dtReal34, "-20", "", 0);
  initializeRegister(REGISTER_X, dtReal16, "-10", "", 0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "divRe34Re16()", "2",   "", 0);
  registerShouldBe(REGISTER_L, dtReal16, "divRe34Re16()", "-10", "", 0);



  ////////////////////////////
  // addRe34Co16 -20 + -1-2i
  #ifdef PC_BUILD
    printf("  4 operations real34 complex16\n");
  #endif

  initializeRegister(REGISTER_Y, dtReal34,    "-20", "",   0);
  initializeRegister(REGISTER_X, dtComplex16, "-1",  "-2", 0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "addRe34Co16()", "-21", "-2", 0);
  registerShouldBe(REGISTER_L, dtComplex16, "addRe34Co16()", "-1",  "-2", 0);



  ////////////////////////////
  // subRe34Co16 -20 - -1-2i
  initializeRegister(REGISTER_Y, dtReal34,    "-20", "",   0);
  initializeRegister(REGISTER_X, dtComplex16, "-1",  "-2", 0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "subRe34Co16()", "-19", "2",  0);
  registerShouldBe(REGISTER_L, dtComplex16, "subRe34Co16()", "-1",  "-2", 0);



  ////////////////////////////
  // mulRe34Co16 -20 × -1-2i
  initializeRegister(REGISTER_Y, dtReal34,    "-20", "",   0);
  initializeRegister(REGISTER_X, dtComplex16, "-1",  "-2", 0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "mulRe34Co16()", "20", "40", 0);
  registerShouldBe(REGISTER_L, dtComplex16, "mulRe34Co16()", "-1", "-2", 0);



  ////////////////////////////
  // divRe34Co16 -20 ÷ -1-2i
  initializeRegister(REGISTER_Y, dtReal34,    "-20", "",   0);
  initializeRegister(REGISTER_X, dtComplex16, "-1",  "-2", 0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "divRe34Co16()", "4",  "-8", 0);
  registerShouldBe(REGISTER_L, dtComplex16, "divRe34Co16()", "-1", "-2", 0);



  ////////////////////////////
  // addRe34Re34 -20 + -20
  #ifdef PC_BUILD
    printf("  4 operations real34 real34\n");
  #endif

  initializeRegister(REGISTER_Y, dtReal34, "-20", "", 0);
  initializeRegister(REGISTER_X, dtReal34, "-20", "", 0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "addRe34Re34()", "-40", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "addRe34Re34()", "-20", "", 0);



  ////////////////////////////
  // subRe34Re34 -20 + -20
  initializeRegister(REGISTER_Y, dtReal34, "-20", "", 0);
  initializeRegister(REGISTER_X, dtReal34, "-20", "", 0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "subRe34Re34()", "0",   "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "subRe34Re34()", "-20", "", 0);



  ////////////////////////////
  // mulRe34Re34 -20 + -20
  initializeRegister(REGISTER_Y, dtReal34, "-20", "", 0);
  initializeRegister(REGISTER_X, dtReal34, "-20", "", 0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "mulRe34Re34()", "400", "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "mulRe34Re34()", "-20", "", 0);



  ////////////////////////////
  // divRe34Re34 -20 + -20
  initializeRegister(REGISTER_Y, dtReal34, "-20", "", 0);
  initializeRegister(REGISTER_X, dtReal34, "-20", "", 0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal34, "divRe34Re34()", "1",   "", 0);
  registerShouldBe(REGISTER_L, dtReal34, "divRe34Re34()", "-20", "", 0);



  ////////////////////////////
  // addRe34Co34 -20 + -2-4i
  #ifdef PC_BUILD
    printf("  4 operations real34 complex34\n");
  #endif

  initializeRegister(REGISTER_Y, dtReal34,    "-20", "",   0);
  initializeRegister(REGISTER_X, dtComplex34, "-2",  "-4", 0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "addRe34Co34()", "-22", "-4", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "addRe34Co34()", "-2", "-4",  0);



  ////////////////////////////
  // subRe34Co34 -20 - -2-4i
  initializeRegister(REGISTER_Y, dtReal34,    "-20", "",   0);
  initializeRegister(REGISTER_X, dtComplex34, "-2",  "-4", 0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "subRe34Co34()", "-18", "4", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "subRe34Co34()", "-2", "-4", 0);



  ////////////////////////////
  // mulRe34Co34 -20 - -2-4i
  initializeRegister(REGISTER_Y, dtReal34,    "-20", "",   0);
  initializeRegister(REGISTER_X, dtComplex34, "-2",  "-4", 0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "mulRe34Co34()", "40", "80", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "mulRe34Co34()", "-2", "-4", 0);



  ////////////////////////////
  // divRe34Co34 -20 - -2-4i
  initializeRegister(REGISTER_Y, dtReal34,    "-20", "",   0);
  initializeRegister(REGISTER_X, dtComplex34, "-2",  "-4", 0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "divRe34Co34()", "2",  "-4", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "divRe34Co34()", "-2", "-4", 0);



  ////////////////////////////
  // addCo34Re16 -2-4i + -10
  #ifdef PC_BUILD
    printf("  4 operations complex34 real16\n");
  #endif

  initializeRegister(REGISTER_Y, dtComplex34, "-2",  "-4", 0);
  initializeRegister(REGISTER_X, dtReal16,    "-10", "",   0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "addCo34Re16()", "-12", "-4", 0);
  registerShouldBe(REGISTER_L, dtReal16,    "addCo34Re16()", "-10", "",   0);



  ////////////////////////////
  // subCo34Re16 -2-4i - -10
  initializeRegister(REGISTER_Y, dtComplex34, "-2",  "-4", 0);
  initializeRegister(REGISTER_X, dtReal16,    "-10", "",   0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "subCo34Re16()", "8",   "-4", 0);
  registerShouldBe(REGISTER_L, dtReal16,    "subCo34Re16()", "-10", "",   0);



  ////////////////////////////
  // mulCo34Re16 -2-4i × -10
  initializeRegister(REGISTER_Y, dtComplex34, "-2",  "-4", 0);
  initializeRegister(REGISTER_X, dtReal16,    "-10", "",   0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "mulCo34Re16()", "20",  "40", 0);
  registerShouldBe(REGISTER_L, dtReal16,    "mulCo34Re16()", "-10", "",   0);



  ////////////////////////////
  // divCo34Re16 -2-4i ÷ -10
  initializeRegister(REGISTER_Y, dtComplex34, "-2",  "-4", 0);
  initializeRegister(REGISTER_X, dtReal16,    "-10", "",   0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "divCo34Re16()", "0.2", "0.4", 0);
  registerShouldBe(REGISTER_L, dtReal16,    "divCo34Re16()", "-10", "",    0);



  ////////////////////////////
  // addCo34Co16 -2-4i + -1-2i
  #ifdef PC_BUILD
    printf("  4 operations complex34 complex16\n");
  #endif

  initializeRegister(REGISTER_Y, dtComplex34, "-2", "-4", 0);
  initializeRegister(REGISTER_X, dtComplex16, "-1", "-2", 0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "addCo34Co16()", "-3", "-6", 0);
  registerShouldBe(REGISTER_L, dtComplex16, "addCo34Co16()", "-1", "-2", 0);



  ////////////////////////////
  // subCo34Co16 -2-4i - -1-2i
  initializeRegister(REGISTER_Y, dtComplex34, "-2", "-4", 0);
  initializeRegister(REGISTER_X, dtComplex16, "-1", "-2", 0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "subCo34Co16()", "-1", "-2", 0);
  registerShouldBe(REGISTER_L, dtComplex16, "subCo34Co16()", "-1", "-2", 0);



  ////////////////////////////
  // mulCo34Co16 -2-4i × -1-2i
  initializeRegister(REGISTER_Y, dtComplex34, "-2", "-4", 0);
  initializeRegister(REGISTER_X, dtComplex16, "-1", "-2", 0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "mulCo34Co16()", "-6", "8",  0);
  registerShouldBe(REGISTER_L, dtComplex16, "mulCo34Co16()", "-1", "-2", 0);



  ////////////////////////////
  // divCo34Co16 -2-4i ÷ -1-2i
  initializeRegister(REGISTER_Y, dtComplex34, "-2", "-4", 0);
  initializeRegister(REGISTER_X, dtComplex16, "-1", "-2", 0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "divCo34Co16()", "2",  "0",  0);
  registerShouldBe(REGISTER_L, dtComplex16, "divCo34Co16()", "-1", "-2", 0);



  ////////////////////////////
  // addCo34Re34 -2-4i + -20
  #ifdef PC_BUILD
    printf("  4 operations complex34 real34\n");
  #endif

  initializeRegister(REGISTER_Y, dtComplex34, "-2",  "-4", 0);
  initializeRegister(REGISTER_X, dtReal34,    "-20", "",   0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "addCo34Re34()", "-22", "-4", 0);
  registerShouldBe(REGISTER_L, dtReal34,    "addCo34Re34()", "-20", "",   0);



  ////////////////////////////
  // subCo34Re34 -2-4i - -20
  initializeRegister(REGISTER_Y, dtComplex34, "-2",  "-4", 0);
  initializeRegister(REGISTER_X, dtReal34,    "-20", "",   0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "subCo34Re34()", "18",  "-4", 0);
  registerShouldBe(REGISTER_L, dtReal34,    "subCo34Re34()", "-20", "",   0);



  ////////////////////////////
  // mulCo34Re34 -2-4i × -20
  initializeRegister(REGISTER_Y, dtComplex34, "-2",  "-4", 0);
  initializeRegister(REGISTER_X, dtReal34,    "-20", "",   0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "mulCo34Re34()", "40",  "80", 0);
  registerShouldBe(REGISTER_L, dtReal34,    "mulCo34Re34()", "-20", "",   0);



  ////////////////////////////
  // divCo34Re34 -2-4i ÷ -20
  initializeRegister(REGISTER_Y, dtComplex34, "-2",  "-4", 0);
  initializeRegister(REGISTER_X, dtReal34,    "-20", "",   0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "divCo34Re34()", "0.1", "0.2", 0);
  registerShouldBe(REGISTER_L, dtReal34,    "divCo34Re34()", "-20", "",    0);



  ////////////////////////////
  // addCo34Co34 -2-4i + -2-4i
  #ifdef PC_BUILD
    printf("  4 operations complex34 complex34\n");
  #endif

  initializeRegister(REGISTER_Y, dtComplex34, "-2", "-4", 0);
  initializeRegister(REGISTER_X, dtComplex34, "-2", "-4", 0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "addCo34Co34()", "-4", "-8", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "addCo34Co34()", "-2", "-4", 0);



  ////////////////////////////
  // subCo34Co34 -2-4i - -2-4i
  initializeRegister(REGISTER_Y, dtComplex34, "-2", "-4", 0);
  initializeRegister(REGISTER_X, dtComplex34, "-2", "-4", 0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "subCo34Co34()", "0",  "0",  0);
  registerShouldBe(REGISTER_L, dtComplex34, "subCo34Co34()", "-2", "-4", 0);



  ////////////////////////////
  // mulCo34Co34 -2-4i × -2-4i
  initializeRegister(REGISTER_Y, dtComplex34, "-2", "-4", 0);
  initializeRegister(REGISTER_X, dtComplex34, "-2", "-4", 0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "mulCo34Co34()", "-12", "16", 0);
  registerShouldBe(REGISTER_L, dtComplex34, "mulCo34Co34()", "-2",  "-4", 0);



  ////////////////////////////
  // divCo34Co34 -2-4i ÷ -2-4i
  initializeRegister(REGISTER_Y, dtComplex34, "-2", "-4", 0);
  initializeRegister(REGISTER_X, dtComplex34, "-2", "-4", 0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtComplex34, "divCo34Co34()", "1",  "0",  0);
  registerShouldBe(REGISTER_L, dtComplex34, "divCo34Co34()", "-2", "-4", 0);


  ////////////////////////////
  // real16 to fraction
  #ifdef PC_BUILD
    printf("  Fractions\n");
  #endif

  initializeRegister(REGISTER_X, dtReal16, "2.125",  "", 0);
  fractionType = FT_PROPER; denominatorMode = DM_ANY; denMax = 9999;
  fractionToDisplayString(REGISTER_X, tmpStr3000);
  tmpStr1390ShouldBe(REGISTER_X, "2" NUM_SPACE_PUNCTUATION NUM_SUP_1 "/" NUM_SUB_8 NUM_SPACE_PUNCTUATION "=");

  initializeRegister(REGISTER_X, dtReal16, "-1.4",  "", 0);
  fractionType = FT_PROPER; denominatorMode = DM_ANY; denMax = 9999;
  fractionToDisplayString(REGISTER_X, tmpStr3000);
  tmpStr1390ShouldBe(REGISTER_X, "-1" NUM_SPACE_PUNCTUATION NUM_SUP_2 "/" NUM_SUB_5 NUM_SPACE_PUNCTUATION "=");

  initializeRegister(REGISTER_X, dtReal16, "3.7",  "", 0);
  fractionType = FT_PROPER; denominatorMode = DM_ANY; denMax = 9999;
  fractionToDisplayString(REGISTER_X, tmpStr3000);
  tmpStr1390ShouldBe(REGISTER_X, "3" NUM_SPACE_PUNCTUATION NUM_SUP_7 "/" NUM_SUB_1 NUM_SUB_0 NUM_SPACE_PUNCTUATION "=");

  initializeRegister(REGISTER_X, dtReal16, "-0.3333333333333333",  "", 0);
  fractionType = FT_PROPER; denominatorMode = DM_ANY; denMax = 9999;
  fractionToDisplayString(REGISTER_X, tmpStr3000);
  tmpStr1390ShouldBe(REGISTER_X, "-0" NUM_SPACE_PUNCTUATION NUM_SUP_1 "/" NUM_SUB_3 NUM_SPACE_PUNCTUATION "<");

  initializeRegister(REGISTER_X, dtReal16, "3.333333333333333",  "", 0);
  fractionType = FT_PROPER; denominatorMode = DM_ANY; denMax = 9999;
  fractionToDisplayString(REGISTER_X, tmpStr3000);
  tmpStr1390ShouldBe(REGISTER_X, "3" NUM_SPACE_PUNCTUATION NUM_SUP_1 "/" NUM_SUB_3 NUM_SPACE_PUNCTUATION ">");

  initializeRegister(REGISTER_X, dtReal16, "3.1415926535",  "", 0);
  fractionType = FT_PROPER; denominatorMode = DM_ANY; denMax = 9999;
  fractionToDisplayString(REGISTER_X, tmpStr3000);
  tmpStr1390ShouldBe(REGISTER_X, "3" NUM_SPACE_PUNCTUATION NUM_SUP_1 NUM_SUP_6 "/" NUM_SUB_1 NUM_SUB_1 NUM_SUB_3 NUM_SPACE_PUNCTUATION ">");

  initializeRegister(REGISTER_X, dtReal16, "2.125",  "", 0);
  fractionType = FT_PROPER; denominatorMode = DM_FIX; denMax = 9999;
  fractionToDisplayString(REGISTER_X, tmpStr3000);
  tmpStr1390ShouldBe(REGISTER_X, "2" NUM_SPACE_PUNCTUATION NUM_SUP_1 NUM_SPACE_PUNCTUATION NUM_SUP_2 NUM_SUP_5 NUM_SUP_0 "/" NUM_SUB_9 NUM_SPACE_PUNCTUATION NUM_SUB_9 NUM_SUB_9 NUM_SUB_9 NUM_SPACE_PUNCTUATION ">");

  initializeRegister(REGISTER_X, dtReal16, "-1.4",  "", 0);
  fractionType = FT_PROPER; denominatorMode = DM_FIX; denMax = 8888;
  fractionToDisplayString(REGISTER_X, tmpStr3000);
  tmpStr1390ShouldBe(REGISTER_X, "-1" NUM_SPACE_PUNCTUATION NUM_SUP_3 NUM_SPACE_PUNCTUATION NUM_SUP_5 NUM_SUP_5 NUM_SUP_5 "/" NUM_SUB_8 NUM_SPACE_PUNCTUATION NUM_SUB_8 NUM_SUB_8 NUM_SUB_8 NUM_SPACE_PUNCTUATION ">");

  initializeRegister(REGISTER_X, dtReal16, "3.7",  "", 0);
  fractionType = FT_PROPER; denominatorMode = DM_FIX; denMax = 7777;
  fractionToDisplayString(REGISTER_X, tmpStr3000);
  tmpStr1390ShouldBe(REGISTER_X, "3" NUM_SPACE_PUNCTUATION NUM_SUP_5 NUM_SPACE_PUNCTUATION NUM_SUP_4 NUM_SUP_4 NUM_SUP_4 "/" NUM_SUB_7 NUM_SPACE_PUNCTUATION NUM_SUB_7 NUM_SUB_7 NUM_SUB_7 NUM_SPACE_PUNCTUATION ">");

  initializeRegister(REGISTER_X, dtReal16, "-0.3333333333333333",  "", 0);
  fractionType = FT_PROPER; denominatorMode = DM_FIX; denMax = 6666;
  fractionToDisplayString(REGISTER_X, tmpStr3000);
  tmpStr1390ShouldBe(REGISTER_X, "-0" NUM_SPACE_PUNCTUATION NUM_SUP_2 NUM_SPACE_PUNCTUATION NUM_SUP_2 NUM_SUP_2 NUM_SUP_2 "/" NUM_SUB_6 NUM_SPACE_PUNCTUATION NUM_SUB_6 NUM_SUB_6 NUM_SUB_6 NUM_SPACE_PUNCTUATION "<");

  initializeRegister(REGISTER_X, dtReal16, "3.333333333333333",  "", 0);
  fractionType = FT_PROPER; denominatorMode = DM_FIX; denMax = 5555;
  fractionToDisplayString(REGISTER_X, tmpStr3000);
  tmpStr1390ShouldBe(REGISTER_X, "3" NUM_SPACE_PUNCTUATION NUM_SUP_1 NUM_SPACE_PUNCTUATION NUM_SUP_8 NUM_SUP_5 NUM_SUP_2 "/" NUM_SUB_5 NUM_SPACE_PUNCTUATION NUM_SUB_5 NUM_SUB_5 NUM_SUB_5 NUM_SPACE_PUNCTUATION ">");

  initializeRegister(REGISTER_X, dtReal16, "3.1415926535",  "", 0);
  fractionType = FT_PROPER; denominatorMode = DM_FIX; denMax = 4444;
  fractionToDisplayString(REGISTER_X, tmpStr3000);
  tmpStr1390ShouldBe(REGISTER_X, "3" NUM_SPACE_PUNCTUATION NUM_SUP_6 NUM_SUP_2 NUM_SUP_9 "/" NUM_SUB_4 NUM_SPACE_PUNCTUATION NUM_SUB_4 NUM_SUB_4 NUM_SUB_4 NUM_SPACE_PUNCTUATION "<");

  initializeRegister(REGISTER_X, dtReal16, "7.125",  "", 0);
  fractionType = FT_PROPER; denominatorMode = DM_FAC; denMax = 360;
  fractionToDisplayString(REGISTER_X, tmpStr3000);
  tmpStr1390ShouldBe(REGISTER_X, "7" NUM_SPACE_PUNCTUATION NUM_SUP_1 "/" NUM_SUB_8 NUM_SPACE_PUNCTUATION "=");

  initializeRegister(REGISTER_X, dtReal16, "1.017857143",  "", 0);
  fractionType = FT_PROPER; denominatorMode = DM_FAC; denMax = 360;
  fractionToDisplayString(REGISTER_X, tmpStr3000);
  tmpStr1390ShouldBe(REGISTER_X, "1" NUM_SPACE_PUNCTUATION NUM_SUP_1 "/" NUM_SUB_6 NUM_SUB_0 NUM_SPACE_PUNCTUATION "<");


  ////////////////////////////
  //Convert from .d to d.ms
  #ifdef PC_BUILD
    printf("  DMS conversion\n");
  #endif

  initializeRegister(REGISTER_X, dtReal34, "11.99572",  "", 0);
  convertRegisterToDms(REGISTER_X);
  registerShouldBe(REGISTER_X, dtReal34, "convertToDms()", "11.5944592",  "", 0);

  initializeRegister(REGISTER_X, dtReal34, "-46.68452156",  "", 0);
  convertRegisterToDms(REGISTER_X);
  registerShouldBe(REGISTER_X, dtReal34, "convertToDms()", "-46.4104277616",  "", 0);


  ////////////////////////////
  //Convert from d.ms to .d
  initializeRegister(REGISTER_X, dtReal34, "11.5944592",  "", 0);
  convertRegisterFromDms(REGISTER_X);
  registerShouldBe(REGISTER_X, dtReal34, "convertFromDms()", "11.99572",  "", 0);

  initializeRegister(REGISTER_X, dtReal34, "-46.4104277616",  "", 0);
  convertRegisterFromDms(REGISTER_X);
  registerShouldBe(REGISTER_X, dtReal34, "convertFromDms()", "-46.68452156",  "", 0);


  ///////////////////////////////////
  // addBigIBigI
  #ifdef PC_BUILD
    printf("  4 operations Big integer Big integer\n");
  #endif

  initializeRegister(REGISTER_Y, dtBigInteger, "1111111111111111111111111111111111111111", "", 0);
  initializeRegister(REGISTER_X, dtBigInteger, "2222222222222222222222222222222222222222", "", 0);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtBigInteger, "addBigIBigI()", "3333333333333333333333333333333333333333", "", 0);
  registerShouldBe(REGISTER_L, dtBigInteger, "addBigIBigI()", "2222222222222222222222222222222222222222", "", 0);


  ///////////////////////////////////
  // subBigIBigI
  initializeRegister(REGISTER_Y, dtBigInteger, "7777777777777777777777777777777777777777", "", 0);
  initializeRegister(REGISTER_X, dtBigInteger, "3333333333333333333333333333333333333333", "", 0);

  fnSubtract(NOPARAM);

  registerShouldBe(REGISTER_X, dtBigInteger, "subBigIBigI()", "4444444444444444444444444444444444444444", "", 0);
  registerShouldBe(REGISTER_L, dtBigInteger, "subBigIBigI()", "3333333333333333333333333333333333333333", "", 0);


  ///////////////////////////////////
  // mulBigIBigI
  initializeRegister(REGISTER_Y, dtBigInteger, "111111111111111111111", "", 0);
  initializeRegister(REGISTER_X, dtBigInteger, "999999999999999999999", "", 0);

  fnMultiply(NOPARAM);

  registerShouldBe(REGISTER_X, dtBigInteger, "mulBigIBigI()", "111111111111111111110888888888888888888889", "", 0);
  registerShouldBe(REGISTER_L, dtBigInteger, "mulBigIBigI()", "999999999999999999999", "", 0);


  ///////////////////////////////////
  // divBigIBigI
  initializeRegister(REGISTER_Y, dtBigInteger, "111111111111111111110888888888888888888889", "", 0);
  initializeRegister(REGISTER_X, dtBigInteger, "999999999999999999999", "", 0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtBigInteger, "divBigIBigI()", "111111111111111111111", "", 0);
  registerShouldBe(REGISTER_L, dtBigInteger, "divBigIBigI()", "999999999999999999999", "", 0);


  ///////////////////////////////////
  // divBigIBigI
  initializeRegister(REGISTER_Y, dtBigInteger, "2546845215987456314564901325026140025545", "", 0);
  initializeRegister(REGISTER_X, dtBigInteger, "20278546482302162448505693725283484", "", 0);

  fnDivide(NOPARAM);

  registerShouldBe(REGISTER_X, dtReal16, "divBigIBigI()", "125593.0851952423", "", 0);
  registerShouldBe(REGISTER_L, dtBigInteger, "divBigIBigI()", "20278546482302162448505693725283484", "", 0);


  ///////////////////////////////////
  // addSmaISmaI
  #ifdef PC_BUILD
    printf("  4 operations Small integer Small integer\n");
  #endif

  fnSetWordSize(24);
  fnIntegerMode(SIM_UNSIGN);
  initializeRegister(REGISTER_Y, dtSmallInteger, "16000000", "", 10);
  initializeRegister(REGISTER_X, dtSmallInteger,  "3000000", "",  8);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtSmallInteger, "addSmaISmaI(SIM_UNSIGN)", "2222784", "", 10); // 19000000 - 2^24
  registerShouldBe(REGISTER_L, dtSmallInteger, "addSmaISmaI(SIM_UNSIGN)", "3000000", "", 8);


  fnSetWordSize(24);
  fnIntegerMode(SIM_2COMPL);
  initializeRegister(REGISTER_Y, dtSmallInteger, "8000000", "",  7);
  initializeRegister(REGISTER_X, dtSmallInteger, "3000000", "", 16);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtSmallInteger, "addSmaISmaI(SIM_2COMPL)", "11000000", "",  7); // 8000000+3000000 = 11000000 --> -5777216
  registerShouldBe(REGISTER_L, dtSmallInteger, "addSmaISmaI(SIM_2COMPL)",  "3000000", "", 16);


  fnSetWordSize(24);
  fnIntegerMode(SIM_1COMPL);
  initializeRegister(REGISTER_Y, dtSmallInteger, "7000000", "",  2);
  initializeRegister(REGISTER_X, dtSmallInteger, "2000000", "", 12);

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtSmallInteger, "addSmaISmaI(SIM_1COMPL)", "9000000", "",  2); // 7000000+2000000 = 9000000 --> -7777215
  registerShouldBe(REGISTER_L, dtSmallInteger, "addSmaISmaI(SIM_1COMPL)", "2000000", "", 12);


  fnSetWordSize(24);
  fnIntegerMode(SIM_SIGNMT);
  initializeRegister(REGISTER_Y, dtSmallInteger,  "5000000", "", 3);
  initializeRegister(REGISTER_X, dtSmallInteger, "10000000", "", 5); // -1611392

  fnAdd(NOPARAM);

  registerShouldBe(REGISTER_X, dtSmallInteger, "addSmaISmaI(SIM_SIGNMT)",  "3388608", "", 3); // 5000000+-1611392 = 3388608
  registerShouldBe(REGISTER_L, dtSmallInteger, "addSmaISmaI(SIM_SIGNMT)", "10000000", "", 5);



  /////////////////////////////////
  // Initialize integer defaults
  fnIntegerMode(SIM_2COMPL);
  fnSetWordSize(64);


  ////////////////////////////
  // Initialize stack
  fnStackSize(SS_8);
  initializeRegister(REGISTER_X, dtReal16, "0", "", 0);
  fnFillStack(NOPARAM);
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
  fnStackSize(SS_4);


  /////////////////////////////////
  // Initialize fraction defaults
  angularMode = AM_DEGREE;
  fractionType = FT_PROPER;
  denominatorMode = DM_ANY;
  denMax = 9999;

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("testFunctions");
  #endif
}


void registerShouldBe(calcRegister_t regist, uint32_t dataType, const char *function, const char *real, const char *imag, uint32_t dataInfo) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("registerShouldBe");
  #endif

  if(getRegisterDataType(regist) != dataType || getRegisterDataInfo(regist) != dataInfo) {
    #ifdef PC_BUILD
      printf("After %s register %" FMT16S " should be %s (%" FMT32U ") but it's %s\n", function, regist, getDataTypeName(dataType, true, false), dataInfo, getRegisterDataTypeName(regist, true, false));
      exit(0);
    #endif

    #ifdef DMCP_BUILD
      sprintf(errorMessage, "Self test:");
      lcd_putsAt(t20, 1, errorMessage);

      sprintf(errorMessage, "after %s", function);
      lcd_putsAt(t20, 2, errorMessage);

      sprintf(errorMessage, "register %" FMT16S, regist);
      lcd_putsAt(t20, 3, errorMessage);

      sprintf(errorMessage, "should be %s (%" FMT32U ")", getDataTypeName(dataType, true, false), dataInfo);
      lcd_putsAt(t20, 4, errorMessage);

      sprintf(errorMessage, "but it's %s!", getRegisterDataTypeName(regist, true, false));
      lcd_putsAt(t20, 4, errorMessage);

      lcd_refresh();
      wait_for_key_press();
    #endif
  }

  if(dataType == dtReal16) {
    real16_t temp;

    stringToReal16(real, &temp);

    if(real16IsNaN(&temp) && real16IsNaN(POINTER_TO_REGISTER_DATA(regist))) {
      return;
    }
    else if(real16IsNaN(&temp) && !real16IsNaN(POINTER_TO_REGISTER_DATA(regist))) {
      #ifdef PC_BUILD
        printf("After %s register %" FMT16S " should be\nreal16 NaN\nbut it is\n", function, regist);
        printRegisterToConsole(regist, 0);
        printf("\n");
	exit(0);
      #endif

      #ifdef DMCP_BUILD
        sprintf(errorMessage, "Self test:");
        lcd_putsAt(t20, 1, errorMessage);

        sprintf(errorMessage, "after %s", function);
        lcd_putsAt(t20, 2, errorMessage);

        sprintf(errorMessage, "register %" FMT16S " should be real16 NaN but it's", regist);
        lcd_putsAt(t20, 3, errorMessage);

        printRegisterToConsole(regist, 4);

        lcd_refresh();
        wait_for_key_press();
      #endif
    }

    if(real16IsInfinite(&temp) && real16IsPositive(&temp) && (real16IsInfinite(POINTER_TO_REGISTER_DATA(regist)) && real16IsPositive(POINTER_TO_REGISTER_DATA(regist)))) {
      return;
    }
    else if(real16IsInfinite(&temp) && real16IsPositive(&temp) && !(real16IsInfinite(POINTER_TO_REGISTER_DATA(regist)) && real16IsPositive(POINTER_TO_REGISTER_DATA(regist)))) {
      #ifdef PC_BUILD
        printf("After %s register %" FMT16S " should be\nreal16 Infinity\nbut it is\n", function, regist);
        printRegisterToConsole(regist, 0);
        printf("\n");
	exit(0);
      #endif

      #ifdef DMCP_BUILD
        sprintf(errorMessage, "Self test:");
        lcd_putsAt(t20, 1, errorMessage);

        sprintf(errorMessage, "after %s", function);
        lcd_putsAt(t20, 2, errorMessage);

        sprintf(errorMessage, "register %" FMT16S " should be real16 Infinity but it's", regist);
        lcd_putsAt(t20, 3, errorMessage);

        printRegisterToConsole(regist, 4);

        lcd_refresh();
        wait_for_key_press();
      #endif
    }

    if(real16IsInfinite(&temp) && real16IsNegative(&temp) && (real16IsInfinite(POINTER_TO_REGISTER_DATA(regist)) && real16IsNegative(POINTER_TO_REGISTER_DATA(regist)))) {
      return;
    }
    else if(real16IsInfinite(&temp) && real16IsNegative(&temp) && !(real16IsInfinite(POINTER_TO_REGISTER_DATA(regist)) && real16IsNegative(POINTER_TO_REGISTER_DATA(regist)))) {
      #ifdef PC_BUILD
        printf("After %s register %" FMT16S " should be\nreal16 -Infinity\nbut it is\n", function, regist);
        printRegisterToConsole(regist, 0);
        printf("\n");
        exit(0);
      #endif

      #ifdef DMCP_BUILD
        sprintf(errorMessage, "Self test:");
        lcd_putsAt(t20, 1, errorMessage);

        sprintf(errorMessage, "after %s", function);
        lcd_putsAt(t20, 2, errorMessage);

        sprintf(errorMessage, "register %" FMT16S " should be real16 -Infinity but it's", regist);
        lcd_putsAt(t20, 3, errorMessage);

        printRegisterToConsole(regist, 4);

        lcd_refresh();
        wait_for_key_press();
      #endif
    }

    real16Subtract(&temp, POINTER_TO_REGISTER_DATA(regist), &temp);
    if(!real16IsZero(&temp)) {
      #ifdef PC_BUILD
        printf("After %s register %" FMT16S " should be\nreal16 %s\nbut it is\n", function, regist, real);
        printRegisterToConsole(regist, 0);
        printf("\n");
        exit(0);
      #endif

      #ifdef DMCP_BUILD
        sprintf(errorMessage, "Self test:");
        lcd_putsAt(t20, 1, errorMessage);

        sprintf(errorMessage, "after %s", function);
        lcd_putsAt(t20, 2, errorMessage);

        sprintf(errorMessage, "register %" FMT16S " should be real16 %s Infinity but it's", regist, real);
        lcd_putsAt(t20, 3, errorMessage);

        printRegisterToConsole(regist, 4);

        lcd_refresh();
        wait_for_key_press();
      #endif
    }
  }

  else if(dataType == dtComplex16) {
    real16_t temp, temp2;

    stringToReal16(real, &temp);
    stringToReal16(imag, &temp2);
    real16Subtract(&temp, POINTER_TO_REGISTER_DATA(regist), &temp);
    real16Subtract(&temp2, POINTER_TO_REGISTER_DATA(regist) + REAL16_SIZE, &temp2);
    if(!real16IsZero(&temp) || !real16IsZero(&temp2)) {
      #ifdef PC_BUILD
        printf("After %s register %" FMT16S " should be\nreal16 %s + %si\nbut it is\n", function, regist, real, imag);
        printRegisterToConsole(regist, 0);
        printf("\n");
        exit(0);
      #endif

      #ifdef DMCP_BUILD
        sprintf(errorMessage, "Self test:");
        lcd_putsAt(t20, 1, errorMessage);

        sprintf(errorMessage, "after %s", function);
        lcd_putsAt(t20, 2, errorMessage);

        sprintf(errorMessage, "register %" FMT16S " should be real16", regist);
        lcd_putsAt(t20, 3, errorMessage);

        sprintf(errorMessage, "%s + %si but it is", real, imag);
        lcd_putsAt(t20, 4, errorMessage);

        printRegisterToConsole(regist, 5);

        lcd_refresh();
        wait_for_key_press();
      #endif
    }
  }

  else if(dataType == dtReal34) {
    real34_t temp;

    stringToReal34(real, &temp);
    real34Subtract(&temp, POINTER_TO_REGISTER_DATA(regist), &temp);
    if(!real34IsZero(&temp)) {
      #ifdef PC_BUILD
        printf("After %s register %" FMT16S " should be\nreal34 %s\nbut it is\n", function, regist, real);
        printRegisterToConsole(regist, 0);
        printf("\n");
        exit(0);
      #endif

      #ifdef DMCP_BUILD
        sprintf(errorMessage, "Self test:");
        lcd_putsAt(t20, 1, errorMessage);

        sprintf(errorMessage, "after %s", function);
        lcd_putsAt(t20, 2, errorMessage);

        sprintf(errorMessage, "register %" FMT16S " should be real34 %s but it's", regist, real);
        lcd_putsAt(t20, 3, errorMessage);

        printRegisterToConsole(regist, 4);

        lcd_refresh();
        wait_for_key_press();
      #endif
    }
  }

  else if(dataType == dtComplex34) {
    real34_t temp, temp2;

    stringToReal34(real, &temp);
    stringToReal34(imag, &temp2);
    real34Subtract(&temp, POINTER_TO_REGISTER_DATA(regist), &temp);
    real34Subtract(&temp2, POINTER_TO_REGISTER_DATA(regist) + REAL34_SIZE, &temp2);
    if(!real34IsZero(&temp) || !real34IsZero(&temp2)) {
      #ifdef PC_BUILD
        printf("After %s register %" FMT16S " should be\nreal34 %s + %si\nbut it is\n", function, regist, real, imag);
        printRegisterToConsole(regist, 0);
        printf("\n");
        exit(0);
      #endif

      #ifdef DMCP_BUILD
        sprintf(errorMessage, "Self test:");
        lcd_putsAt(t20, 1, errorMessage);

        sprintf(errorMessage, "after %s", function);
        lcd_putsAt(t20, 2, errorMessage);

        sprintf(errorMessage, "register %" FMT16S " should be real34", regist);
        lcd_putsAt(t20, 3, errorMessage);

        sprintf(errorMessage, "%s + %si but it is", real, imag);
        lcd_putsAt(t20, 4, errorMessage);

        printRegisterToConsole(regist, 5);

        lcd_refresh();
        wait_for_key_press();
      #endif
    }
  }

  else if(dataType == dtBigInteger) {
    bigInteger_t is;
    bigInteger_t shouldBe;

    convertBigIntegerRegisterToBigInteger(regist, &is);
    stringToBigInteger(real, 10, &shouldBe);
    if(bigIntegerCompare(&is, &shouldBe) != BIG_INTEGER_EQUAL) {
      #ifdef PC_BUILD
        printf("After %s register %" FMT16S " should be\nBig integer %s\nbut it is\n", function, regist, real);
        printRegisterToConsole(regist, 0);
        printf("\n");
        exit(0);
      #endif

      #ifdef DMCP_BUILD
        sprintf(errorMessage, "Self test:");
        lcd_putsAt(t20, 1, errorMessage);

        sprintf(errorMessage, "after %s", function);
        lcd_putsAt(t20, 2, errorMessage);

        sprintf(errorMessage, "register %" FMT16S " should be big integer %s but it's", regist, real);
        lcd_putsAt(t20, 3, errorMessage);

        printRegisterToConsole(regist, 4);

        lcd_refresh();
        wait_for_key_press();
      #endif
    }
  }

  else if(dataType == dtSmallInteger) {
    if(*(uint64_t *)(POINTER_TO_REGISTER_DATA(regist)) != (uint64_t)atoll(real)) {
      #ifdef PC_BUILD
        printf("After %s register %" FMT16S " should be\nSmall integer %s(%" FMT32U ")\nbut it is\n%" FMT64U "(%" FMT32U ")\n", function, regist, real, dataInfo, *(uint64_t *)(POINTER_TO_REGISTER_DATA(regist)), getRegisterBase(regist));
        exit(0);
      #endif

      #ifdef DMCP_BUILD
        sprintf(errorMessage, "Self test:");
        lcd_putsAt(t20, 1, errorMessage);

        sprintf(errorMessage, "after %s", function);
        lcd_putsAt(t20, 2, errorMessage);

        sprintf(errorMessage, "register %" FMT16S " should be small integer %s(%" FMT32U ") but it's", regist, real, dataInfo);
        lcd_putsAt(t20, 3, errorMessage);

        sprintf(errorMessage, "%" FMT64U "(%" FMT32U ")", *(uint64_t *)(POINTER_TO_REGISTER_DATA(regist)), getRegisterBase(regist));
        lcd_putsAt(t20, 4, errorMessage);

        lcd_refresh();
        wait_for_key_press();
      #endif
    }
  }

  else {
    #ifdef PC_BUILD
      printf("Data type %s comparison to be coded!\n", getDataTypeName(dataType, false, false));
      exit(0);
    #endif

    #ifdef DMCP_BUILD
      sprintf(errorMessage, "Self test:");
      lcd_putsAt(t20, 1, errorMessage);

      sprintf(errorMessage, "Data type %s comparison to be coded!", getDataTypeName(dataType, false, false));
      lcd_putsAt(t20, 2, errorMessage);

      lcd_refresh();
      wait_for_key_press();
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("registerShouldBe");
  #endif
}



void tmpStr1390ShouldBe(calcRegister_t regist, const char *shouldBe) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("tmpStr1390ShouldBe");
  #endif

  if(strcmp(tmpStr3000, shouldBe)) {
    #ifdef PC_BUILD
      printf("for "); printRegisterToConsole(REGISTER_X, 0);
      stringToUtf8(shouldBe,  (unsigned char *)errorMessage);
      stringToUtf8(tmpStr3000, (unsigned char *)errorMessage + ERROR_MESSAGE_LENGTH/2);
      printf(" the fraction should be '%s' but it is '%s'!\n", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2);
      exit(0);
    #endif

    #ifdef DMCP_BUILD
      sprintf(errorMessage, "Self test: for");
      lcd_putsAt(t20, 1, errorMessage);

      printRegisterToConsole(regist, 2);

      sprintf(errorMessage, "the faction is not correct!");
      lcd_putsAt(t20, 3, errorMessage);

      lcd_refresh();
      wait_for_key_press();
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("tmpStr1390ShouldBe");
  #endif
}



void initializeRegister(calcRegister_t regist, uint32_t dataType, const char *real, const char *imag, uint32_t dataInfo) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("initializeRegister");
  #endif

  if(dataType == dtReal16) {
    reallocateRegister(regist, dataType, REAL16_SIZE, dataInfo);
    stringToReal16(real, POINTER_TO_REGISTER_DATA(regist));
  }

  else if(dataType == dtComplex16) {
    reallocateRegister(regist, dtComplex16, COMPLEX16_SIZE, dataInfo);
    stringToReal16(real, POINTER_TO_REGISTER_DATA(regist));
    stringToReal16(imag, POINTER_TO_REGISTER_DATA(regist) + REAL16_SIZE);
  }

  else if(dataType == dtReal34) {
    reallocateRegister(regist, dtReal34, REAL34_SIZE, dataInfo);
    stringToReal34(real, POINTER_TO_REGISTER_DATA(regist));
  }

  else if(dataType == dtComplex34) {
    reallocateRegister(regist, dtComplex34, COMPLEX34_SIZE, dataInfo);
    stringToReal34(real, POINTER_TO_REGISTER_DATA(regist));
    stringToReal34(imag, POINTER_TO_REGISTER_DATA(regist) + REAL34_SIZE);
  }

  else if(dataType == dtString) {
    reallocateRegister(regist, dtString, atoi(real), dataInfo);
    memcpy(POINTER_TO_REGISTER_DATA(regist) + 2, imag, atoi(real)+1);
  }

  else if(dataType == dtBigInteger) {
    bigInteger_t tmp;
    stringToBigInteger(real, 10, &tmp);
    convertBigIntegerToBigIntegerRegister(&tmp, regist);
  }

  else if(dataType == dtSmallInteger) {
    reallocateRegister(regist, dtSmallInteger, SMALL_INTEGER_SIZE, dataInfo);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(regist)) = atol(real);
    setRegisterBase(regist, dataInfo);
  }

  else {
    #ifdef PC_BUILD
      printf("Cannot initialize %s! To be coded!\n", getDataTypeName(dataType, true, false));
      exit(0);
    #endif

    #ifdef DMCP_BUILD
      sprintf(errorMessage, "Self test:");
      lcd_putsAt(t20, 1, errorMessage);

      sprintf(errorMessage, "Cannot initialize %s!", getDataTypeName(dataType, true, false));
      lcd_putsAt(t20, 2, errorMessage);

      sprintf(errorMessage, "To be coded!");
      lcd_putsAt(t20, 3, errorMessage);

      lcd_refresh();
      wait_for_key_press();
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("initializeRegister");
  #endif
}
#endif
