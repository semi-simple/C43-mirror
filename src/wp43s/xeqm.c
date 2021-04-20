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
 * \file xeqm.c
 ***********************************************/

#include "items.h"
#include <string.h>
#include "wp43s.h"

#define commandnumberl NIM_BUFFER_LENGTH


void press_key(void) {
    #ifdef DMCP_BUILD
      print_inlinestr("Press key",true);
      wait_for_key_press();
    #endif
}


void capture_sequence(char *origin, uint16_t item) {
   char line1[TMP_STR_LENGTH];
   char ll[commandnumberl];
#ifdef PC_BUILD
   //printf("Captured: %4d   //%10s//  (%s)\n",item,indexOfItems[item].itemSoftmenuName, origin);
#endif

    line1[0]=0;
    ll[0]=0; ll[1]=0;
    switch (item) {
      case  ITM_XexY: strcpy(ll,"X<>Y"); break;
      case  ITM_YX:   strcpy(ll,"Y^X" ); break;
      case  ITM_DIV:  strcpy(ll,"/"   ); break;

      case  ITM_0:
      case  ITM_1:
      case  ITM_2:
      case  ITM_3:
      case  ITM_4:
      case  ITM_5:
      case  ITM_6:
      case  ITM_7:
      case  ITM_8:
      case  ITM_9:
              ll[0]=item - ITM_0 + 48; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
/*       
      case  ITM_0:        ll[0]=48; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_1:        ll[0]=49; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_2:        ll[0]=50; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_3:        ll[0]=51; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_4:        ll[0]=52; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_5:        ll[0]=53; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_6:        ll[0]=54; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_7:        ll[0]=55; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_8:        ll[0]=56; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_9:        ll[0]=57; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
*/
      case  ITM_PERIOD:   ll[0]=46; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break; //.
      case  ITM_EXPONENT: ll[0]=69; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break; //E
      default: { strcpy(ll,indexOfItems[item].itemSoftmenuName);
               }  
    }

//    uint16_t ix;
//    ix = 0;                  //CONVERT OUTPUT ## for >128 characters
//    while (ll[ix] != 0) {
//    if (( (ll[ix] & 128) == 128) || ll[ix] < 32) {ll[ix] = 35;}
//    ix++;
//    }

    if(line1[0]==0) sprintf(line1, " %4d //%10s",item,ll);

    #ifndef TESTSUITE_BUILD
    stringToUtf8(line1, (uint8_t *)tmpString);
    export_string_to_file(tmpString);
    #endif
}


//############################ SEND KEY TO 43S ENGINE ####################
void runkey(int16_t item){
  #ifndef TESTSUITE_BUILD

/*
char tmp[2];
tmp[0]=0;
executeFunction(tmp, item);
*/  


  if(item < 0) {
    showSoftmenu(item);
  } else {

    //printf("§%d§ ",item);
    hideFunctionName();           //Clear in case active
    processKeyAction(item);
    if (!keyActionProcessed){
      hideFunctionName();         //Clear in case activated during process
      runFunction(item);
//      #ifdef DMCP_BUILD
//        lcd_forced_refresh(); // Just redraw from LCD buffer    
//      #endif
    } 
  }

  #endif
}

//############################ DECODE NUMBERS AND THEN SEND KEY TO 43S ENGINE ####################
void sendkeys(const char aa[]) {
  int16_t ix = 0;
  while (aa[ix]!=0) {
    if(aa[ix]>=65 && aa[ix]<= 90){runkey(ITM_A+aa[ix]-65);} else //A..Z
    if(aa[ix]>=97 && aa[ix]<=122){runkey(ITM_A+aa[ix]-65-32);} else //a..z converted to A..Z
    if(aa[ix]>=48 && aa[ix]<= 57){runkey(ITM_0+aa[ix]-48);} else //0..9
    switch (aa[ix]) {
      case '.':  runkey(ITM_PERIOD); break; //.
      case 'E':  runkey(ITM_EXPONENT); break; //E
      case 'e': runkey(ITM_EXPONENT); break; //e
      case '-':  runkey(ITM_SUB); break; //-
      case '+':  runkey(ITM_ADD); break; //+
      case ' ':  runkey(ITM_SPACE); break; //space
      case '#':  runkey(ITM_toINT); break; //#
      case ':':  runkey(ITM_COLON        ); break; //#
      case ';':  runkey(ITM_SEMICOLON    ); break; //#
      case '\'': runkey(ITM_QUOTE        ); break; //#
      case '?':  runkey(ITM_QUESTION_MARK); break; //#
      default:;
    }
  ix++;
  }
}


bool_t strcompare( char *in1, char *in2) {
  if (stringByteLength(in1) == stringByteLength(in2)) {
    int16_t i = 0;
    bool_t areEqual = true;
    while (areEqual && in1[i] != 0) {
      if(in1[i] != in2[i]) {areEqual = false; return false;}
      i++;
    }
    return areEqual;
  } else return false;
}

// COMMAND name or number: command to be located in between any of CR, LF, comma, space, tab, i.e. X<>Y, PRIME?, ...
// COMMENT: comment to be located in between / and /, i.e. /Comment/
// NUMBER to be located in between quotes: "123.123" or STO "00"
// Example: "200" EXIT PRIME?
// Ignores all other ASCII control and white space.


bool_t running_program_jm = false;
uint32_t indic_x = 0;
uint32_t indic_y = SCREEN_HEIGHT-1;




//                     ****************************************************************************************************
bool_t checkindexes(int16_t *com,  char *str, bool_t exec) {
                      *com = 0;
//FROM SPREADSHEET vvv ****************************************************************************************************
//XEQM TYPE1 ORIGINAL FULL SELECTED LIST

//                      if (strcompare(str,"LBL" )) { *com = ITM_LBL;} else
//                      if (strcompare(str,"GTO" )) { *com = ITM_GTO;} else
//                      if (strcompare(str,"XEQ" )) { *com = ITM_XEQ;} else
//                      if (strcompare(str,"RTN" )) { *com = ITM_RTN;} else
//                      if (strcompare(str,"X=?" )) { *com = ITM_XEQU;} else
//                      if (strcompare(str,"XNOT_EQUAL?" )) { *com = ITM_XNE;} else
//                      if (strcompare(str,"X=+0?" )) { *com = ITM_XEQUP0;} else
//                      if (strcompare(str,"X=-0?" )) { *com = ITM_XEQUM0;} else
//                      if (strcompare(str,"XALMOST_EQUAL?" )) { *com = ITM_XAEQU;} else
//                      if (strcompare(str,"X<?" )) { *com = ITM_XLT;} else
//                      if (strcompare(str,"XLESS_EQUAL?" )) { *com = ITM_XLE;} else
//                      if (strcompare(str,"XGREATER_EQUAL?" )) { *com = ITM_XGE;} else
//                      if (strcompare(str,"X>?" )) { *com = ITM_XGT;} else
//                      if (strcompare(str,"FC?" )) { *com = ITM_FC;} else
//                      if (strcompare(str,"FS?" )) { *com = ITM_FS;} else
//                      if (strcompare(str,"EVEN?" )) { *com = ITM_EVEN;} else
//                      if (strcompare(str,"ODD?" )) { *com = ITM_ODD;} else
//                      if (strcompare(str,"FP?" )) { *com = ITM_FPQ;} else
//                      if (strcompare(str,"INT?" )) { *com = ITM_INT;} else
//                      if (strcompare(str,"CPX?" )) { *com = ITM_CPX;} else
//                      if (strcompare(str,"MATR?" )) { *com = ITM_MATR;} else
//                      if (strcompare(str,"NAN?" )) { *com = ITM_NAN;} else
//                      if (strcompare(str,"REAL?" )) { *com = ITM_REAL;} else
//                      if (strcompare(str,"SPEC?" )) { *com = ITM_SPEC;} else
//                      if (strcompare(str,"STRI?" )) { *com = ITM_STRI;} else
//                      if (strcompare(str,"PLUS_MINUSINFINITY?" )) { *com = ITM_PMINFINITY;} else
                      if (strcompare(str,"PRIME?" )) { *com = ITM_PRIME;} else
//                      if (strcompare(str,"TOP?" )) { *com = ITM_TOP;} else
                      if (strcompare(str,"ENTER" )) { *com = ITM_ENTER;} else
                      if (strcompare(str,"X<>Y" )) { *com = ITM_XexY;} else
                      if (strcompare(str,"DROP" )) { *com = ITM_DROP;} else
//                      if (strcompare(str,"PAUSE" )) { *com = ITM_PAUSE;} else
//                      if (strcompare(str,"R" )) { *com = ITM_Rup;} else
//                      if (strcompare(str,"RDOWN_ARROW" )) { *com = ITM_Rdown;} else
                      if (strcompare(str,"CLX" )) { *com = ITM_CLX;} else
                      if (strcompare(str,"FILL" )) { *com = ITM_FILL;} else
//                      if (strcompare(str,"INPUT" )) { *com = ITM_INPUT;} else
                      if (strcompare(str,"STO" )) { *com = ITM_STO;} else
                      if (strcompare(str,"COMB" )) { *com = ITM_COMB;} else
                      if (strcompare(str,"PERM" )) { *com = ITM_PERM;} else
                      if (strcompare(str,"RCL" )) { *com = ITM_RCL;} else
//                      if (strcompare(str,"CONVG?" )) { *com = ITM_CONVG;} else
//                      if (strcompare(str,"ENTRY?" )) { *com = ITM_ENTRY;} else
                      if (strcompare(str,"X^2" )) { *com = ITM_SQUARE;} else
                      if (strcompare(str,"X^3" )) { *com = ITM_CUBE;} else
                      if (strcompare(str,"Y^X" )) { *com = ITM_YX;} else
                      if (strcompare(str,"SQRT" )) { *com = ITM_SQUAREROOTX;} else
                      if (strcompare(str,"CUBRT" )) { *com = ITM_CUBEROOT;} else
                      if (strcompare(str,"XRTY" )) { *com = ITM_XTHROOT;} else
                      if (strcompare(str,"2^X" )) { *com = ITM_2X;} else
                      if (strcompare(str,"E^X" )) { *com = ITM_EXP;} else
                      if (strcompare(str,"10^X" )) { *com = ITM_10x;} else
                      if (strcompare(str,"LOG2" )) { *com = ITM_LOG2;} else
                      if (strcompare(str,"LN" )) { *com = ITM_LN;} else
//                      if (strcompare(str,"STOP" )) { *com = ITM_STOP;} else
                      if (strcompare(str,"LOG10" )) { *com = ITM_LOG10;} else
                      if (strcompare(str,"LOGXY" )) { *com = ITM_LOGXY;} else
                      if (strcompare(str,"1/X" )) { *com = ITM_1ONX;} else
                      if (strcompare(str,"COS" )) { *com = ITM_cos;} else
                      if (strcompare(str,"COSH" )) { *com = ITM_cosh;} else
                      if (strcompare(str,"SIN" )) { *com = ITM_sin;} else
//                      if (strcompare(str,"KEY?" )) { *com = ITM_KEYQ;} else
                      if (strcompare(str,"SINH" )) { *com = ITM_sinh;} else
                      if (strcompare(str,"TAN" )) { *com = ITM_tan;} else
                      if (strcompare(str,"TANH" )) { *com = ITM_tanh;} else
                      if (strcompare(str,"ARCCOS" )) { *com = ITM_arccos;} else
                      if (strcompare(str,"ARCCOSH" )) { *com = ITM_arcosh;} else
                      if (strcompare(str,"ARCSIN" )) { *com = ITM_arcsin;} else
                      if (strcompare(str,"ARCSINH" )) { *com = ITM_arsinh;} else
                      if (strcompare(str,"ARCTAN" )) { *com = ITM_arctan;} else
                      if (strcompare(str,"ARCTANH" )) { *com = ITM_artanh;} else
//                      if (strcompare(str,"CEIL" )) { *com = ITM_CEIL;} else
//                      if (strcompare(str,"FLOOR" )) { *com = ITM_FLOOR;} else
                      if (strcompare(str,"GCD" )) { *com = ITM_GCD;} else
                      if (strcompare(str,"LCM" )) { *com = ITM_LCM;} else
                      if (strcompare(str,"DEC" )) { *com = ITM_DEC;} else
                      if (strcompare(str,"INC" )) { *com = ITM_INC;} else
                      if (strcompare(str,"IP" )) { *com = ITM_IP;} else
                      if (strcompare(str,"FP" )) { *com = ITM_FP;} else
                      if (strcompare(str,"+" )) { *com = ITM_ADD;} else
                      if (strcompare(str,"-" )) { *com = ITM_SUB;} else
                      if (strcompare(str,"CHS" )) { *com = ITM_CHS;} else
                      if (strcompare(str,"*" )) { *com = ITM_MULT;} else
                      if (strcompare(str,"/" )) { *com = ITM_DIV;} else
                      if (strcompare(str,"IDIV" )) { *com = ITM_IDIV;} else
//                      if (strcompare(str,"VIEW" )) { *com = ITM_VIEW;} else
                      if (strcompare(str,"MOD" )) { *com = ITM_MOD;} else
                      if (strcompare(str,"MAX" )) { *com = ITM_MAX;} else
                      if (strcompare(str,"MIN" )) { *com = ITM_MIN;} else
                      if (strcompare(str,"ABS" )) { *com = ITM_MAGNITUDE;} else
//                      if (strcompare(str,"NEIGHB" )) { *com = ITM_NEIGHB;} else
                      if (strcompare(str,"NEXTP" )) { *com = ITM_NEXTP;} else
//                      if (strcompare(str,"X!" )) { *com = ITM_XFACT;} else
                      if (strcompare(str,"PI" )) { *com = ITM_CONSTpi;} else
//                      if (strcompare(str,"CF" )) { *com = ITM_CF;} else
//                      if (strcompare(str,"SF" )) { *com = ITM_SF;} else
//                      if (strcompare(str,"FF" )) { *com = ITM_FF;} else
//                      if (strcompare(str,"M.SQR?" )) { *com = ITM_M_SQR;} else
                      if (strcompare(str,">DEG" )) { *com = ITM_toDEG;} else
                      if (strcompare(str,">D.MS" )) { *com = ITM_toDMS;} else
                      if (strcompare(str,">GRAD" )) { *com = ITM_toGRAD;} else
                      if (strcompare(str,">MULPI" )) { *com = ITM_toMULpi;} else
                      if (strcompare(str,">RAD" )) { *com = ITM_toRAD;} else
                      if (strcompare(str,"D>R" )) { *com = ITM_DtoR;} else
                      if (strcompare(str,"R>D" )) { *com = ITM_RtoD;} else
//                      if (strcompare(str,"RMD" )) { *com = ITM_RMD;} else
//                      if (strcompare(str,"NOT" )) { *com = ITM_LOGICALNOT;} else
//                      if (strcompare(str,"AND" )) { *com = ITM_LOGICALAND;} else
//                      if (strcompare(str,"OR" )) { *com = ITM_LOGICALOR;} else
//                      if (strcompare(str,"XOR" )) { *com = ITM_LOGICALXOR;} else
//                      if (strcompare(str,"X<>" )) { *com = ITM_Xex;} else
                      if (strcompare(str,"c" )) { *com = CST_05;} else
//                      if (strcompare(str,"e" )) { *com = CST_08;} else
//                      if (strcompare(str,"ge" )) { *com = CST_16;} else
//                      if (strcompare(str,"gEARTH" )) { *com = CST_18;} else
//                      if (strcompare(str,"mu0" )) { *com = CST_65;} else
                      if (strcompare(str,"PHI" )) { *com = CST_74;} else
//                      if (strcompare(str,"-INF" )) { *com = CST_77;} else
//                      if (strcompare(str,"INF" )) { *com = CST_78;} else
//                      if (strcompare(str,"FC?C" )) { *com = ITM_FCC;} else
//                      if (strcompare(str,"FC?S" )) { *com = ITM_FCS;} else
//                      if (strcompare(str,"FC?F" )) { *com = ITM_FCF;} else
//                      if (strcompare(str,"FS?C" )) { *com = ITM_FSC;} else
//                      if (strcompare(str,"FS?S" )) { *com = ITM_FSS;} else
//                      if (strcompare(str,"FS?F" )) { *com = ITM_FSF;} else
//                      if (strcompare(str,"NAND" )) { *com = ITM_LOGICALNAND;} else
//                      if (strcompare(str,"NOR" )) { *com = ITM_LOGICALNOR;} else
//                      if (strcompare(str,"XNOR" )) { *com = ITM_LOGICALXNOR;} else
//                      if (strcompare(str,"BS?" )) { *com = ITM_BS;} else
//                      if (strcompare(str,"BC?" )) { *com = ITM_BC;} else
//                      if (strcompare(str,"CB" )) { *com = ITM_CB;} else
//                      if (strcompare(str,"SB" )) { *com = ITM_SB;} else
//                      if (strcompare(str,"FB" )) { *com = ITM_FB;} else
//                      if (strcompare(str,"RL" )) { *com = ITM_RL;} else
//                      if (strcompare(str,"RLC" )) { *com = ITM_RLC;} else
//                      if (strcompare(str,"RR" )) { *com = ITM_RR;} else
//                      if (strcompare(str,"RRC" )) { *com = ITM_RRC;} else
//                      if (strcompare(str,"SL" )) { *com = ITM_SL;} else
//                      if (strcompare(str,"SR" )) { *com = ITM_SR;} else
//                      if (strcompare(str,"ASR" )) { *com = ITM_ASR;} else
//                      if (strcompare(str,"LJ" )) { *com = ITM_LJ;} else
//                      if (strcompare(str,"RJ" )) { *com = ITM_RJ;} else
//                      if (strcompare(str,"MASKL" )) { *com = ITM_MASKL;} else
//                      if (strcompare(str,"MASKR" )) { *com = ITM_MASKR;} else
//                      if (strcompare(str,"MIRROR" )) { *com = ITM_MIRROR;} else
//                      if (strcompare(str,"#B" )) { *com = ITM_NUMB;} else
//                      if (strcompare(str,"SDL" )) { *com = ITM_SDL;} else
//                      if (strcompare(str,"SDR" )) { *com = ITM_SDR;} else
                      if (strcompare(str,"SUM+" )) { *com = ITM_SIGMAPLUS;} else
                      if (strcompare(str,"NSUM" )) { *com = ITM_NSIGMA;} else
                      if (strcompare(str,"SUMX" )) { *com = ITM_SIGMAx;} else
                      if (strcompare(str,"SUMY" )) { *com = ITM_SIGMAy;} else
//                      if (strcompare(str,"SMX^2" )) { *com = ITM_SIGMAx2;} else
//                      if (strcompare(str,"SMX^2Y" )) { *com = ITM_SIGMAx2y;} else
//                      if (strcompare(str,"SMY^2" )) { *com = ITM_SIGMAy2;} else
//                      if (strcompare(str,"SMXY" )) { *com = ITM_SIGMAxy;} else
//                      if (strcompare(str,"SMLNXY" )) { *com = ITM_SIGMAlnxy;} else
//                      if (strcompare(str,"SMLNX" )) { *com = ITM_SIGMAlnx;} else
//                      if (strcompare(str,"SMLN^2X" )) { *com = ITM_SIGMAln2x;} else
//                      if (strcompare(str,"SMYLNX" )) { *com = ITM_SIGMAylnx;} else
//                      if (strcompare(str,"SMLNY" )) { *com = ITM_SIGMAlny;} else
//                      if (strcompare(str,"SMLN^2Y" )) { *com = ITM_SIGMAln2y;} else
//                      if (strcompare(str,"SMXLNY" )) { *com = ITM_SIGMAxlny;} else
//                      if (strcompare(str,"SMX^2LNY" )) { *com = ITM_SIGMAx2lny;} else
//                      if (strcompare(str,"SMLNY/X" )) { *com = ITM_SIGMAlnyonx;} else
//                      if (strcompare(str,"SMX^2/Y" )) { *com = ITM_SIGMAx2ony;} else
//                      if (strcompare(str,"SM^1/X" )) { *com = ITM_SIGMA1onx;} else
//                      if (strcompare(str,"SM^1/X^2" )) { *com = ITM_SIGMA1onx2;} else
//                      if (strcompare(str,"SMX/Y" )) { *com = ITM_SIGMAxony;} else
//                      if (strcompare(str,"SM^1/Y" )) { *com = ITM_SIGMA1ony;} else
//                      if (strcompare(str,"SM^1/Y^2" )) { *com = ITM_SIGMA1ony2;} else
//                      if (strcompare(str,"SMX^3" )) { *com = ITM_SIGMAx3;} else
//                      if (strcompare(str,"SMX^4" )) { *com = ITM_SIGMAx4;} else
//                      if (strcompare(str,"FRACT" )) { *com = SFL_FRACT;} else
//                      if (strcompare(str,"PROPFR" )) { *com = SFL_PROPFR;} else
//                      if (strcompare(str,"DENANY" )) { *com = SFL_DENANY;} else
//                      if (strcompare(str,"DENFIX" )) { *com = SFL_DENFIX;} else
                      if (strcompare(str,"X" )) { *com = ITM_REG_X;} else
                      if (strcompare(str,"Y" )) { *com = ITM_REG_Y;} else
//                      if (strcompare(str,"Z" )) { *com = ITM_REG_Z;} else
//                      if (strcompare(str,"T" )) { *com = ITM_REG_T;} else
//                      if (strcompare(str,"A" )) { *com = ITM_REG_A;} else
//                      if (strcompare(str,"B" )) { *com = ITM_REG_B;} else
                      if (strcompare(str,"C" )) { *com = ITM_REG_C;} else
//                      if (strcompare(str,"D" )) { *com = ITM_REG_D;} else
//                      if (strcompare(str,"L" )) { *com = ITM_REG_L;} else
//                      if (strcompare(str,"I" )) { *com = ITM_REG_I;} else
//                      if (strcompare(str,"J" )) { *com = ITM_REG_J;} else
//                      if (strcompare(str,"K" )) { *com = ITM_REG_K;} else
                      if (strcompare(str,"IND>" )) { *com = ITM_INDIRECTION;} else
                      if (strcompare(str,"MAX" )) { *com = ITM_Max;} else
                      if (strcompare(str,"MIN" )) { *com = ITM_Min;} else
                      if (strcompare(str,"EEX" )) { *com = ITM_EXPONENT;} else
//                      if (strcompare(str,"BINOMP" )) { *com = ITM_BINOMP;} else
//                      if (strcompare(str,"BINOMGAUSS_BLACK_LGAUSS_WHITE_R" )) { *com = ITM_BINOM;} else
//                      if (strcompare(str,"BINOMGAUSS_WHITE_LGAUSS_BLACK_R" )) { *com = ITM_BINOMU;} else
//                      if (strcompare(str,"BINOM^MINUS_1" )) { *com = ITM_BINOMM1;} else
//                      if (strcompare(str,"CAUCHP" )) { *com = ITM_CAUCHP;} else
//                      if (strcompare(str,"CAUCHGAUSS_BLACK_LGAUSS_WHITE_R" )) { *com = ITM_CAUCH;} else
//                      if (strcompare(str,"CAUCHGAUSS_WHITE_LGAUSS_BLACK_R" )) { *com = ITM_CAUCHU;} else
//                      if (strcompare(str,"CAUCH^MINUS_1" )) { *com = ITM_CAUCHM1;} else
//                      if (strcompare(str,"EXPONP" )) { *com = ITM_EXPONP;} else
//                      if (strcompare(str,"EXPONGAUSS_BLACK_LGAUSS_WHITE_R" )) { *com = ITM_EXPON;} else
//                      if (strcompare(str,"EXPONGAUSS_WHITE_LGAUSS_BLACK_R" )) { *com = ITM_EXPONU;} else
//                      if (strcompare(str,"EXPON^MINUS_1" )) { *com = ITM_EXPONM1;} else
//                      if (strcompare(str,"FP(X)" )) { *com = ITM_FPX;} else
//                      if (strcompare(str,"FGAUSS_BLACK_LGAUSS_WHITE_R(X)" )) { *com = ITM_FX;} else
//                      if (strcompare(str,"FGAUSS_WHITE_LGAUSS_BLACK_R(X)" )) { *com = ITM_FUX;} else
//                      if (strcompare(str,"F^MINUS_1(P)" )) { *com = ITM_FM1P;} else
//                      if (strcompare(str,"GEOMP" )) { *com = ITM_GEOMP;} else
//                      if (strcompare(str,"GEOMGAUSS_BLACK_LGAUSS_WHITE_R" )) { *com = ITM_GEOM;} else
//                      if (strcompare(str,"GEOMGAUSS_WHITE_LGAUSS_BLACK_R" )) { *com = ITM_GEOMU;} else
//                      if (strcompare(str,"GEOM^MINUS_1" )) { *com = ITM_GEOMM1;} else
//                      if (strcompare(str,"HYPERP" )) { *com = ITM_HYPERP;} else
//                      if (strcompare(str,"HYPERGAUSS_BLACK_LGAUSS_WHITE_R" )) { *com = ITM_HYPER;} else
//                      if (strcompare(str,"HYPERGAUSS_WHITE_LGAUSS_BLACK_R" )) { *com = ITM_HYPERU;} else
//                      if (strcompare(str,"HYPER^MINUS_1" )) { *com = ITM_HYPERM1;} else
//                      if (strcompare(str,"LGNRMP" )) { *com = ITM_LGNRMP;} else
//                      if (strcompare(str,"LGNRMGAUSS_BLACK_LGAUSS_WHITE_R" )) { *com = ITM_LGNRM;} else
//                      if (strcompare(str,"LGNRMGAUSS_WHITE_LGAUSS_BLACK_R" )) { *com = ITM_LGNRMU;} else
//                      if (strcompare(str,"LGNRM^MINUS_1" )) { *com = ITM_LGNRMM1;} else
//                      if (strcompare(str,"LOGISP" )) { *com = ITM_LOGISP;} else
//                      if (strcompare(str,"LOGISGAUSS_BLACK_LGAUSS_WHITE_R" )) { *com = ITM_LOGIS;} else
//                      if (strcompare(str,"LOGISGAUSS_WHITE_LGAUSS_BLACK_R" )) { *com = ITM_LOGISU;} else
//                      if (strcompare(str,"LOGIS^MINUS_1" )) { *com = ITM_LOGISM1;} else
//                      if (strcompare(str,"NBINP" )) { *com = ITM_NBINP;} else
//                      if (strcompare(str,"NBINGAUSS_BLACK_LGAUSS_WHITE_R" )) { *com = ITM_NBIN;} else
//                      if (strcompare(str,"NBINGAUSS_WHITE_LGAUSS_BLACK_R" )) { *com = ITM_NBINU;} else
//                      if (strcompare(str,"NBIN^MINUS_1" )) { *com = ITM_NBINM1;} else
//                      if (strcompare(str,"NORMLP" )) { *com = ITM_NORMLP;} else
//                      if (strcompare(str,"NORMLGAUSS_BLACK_LGAUSS_WHITE_R" )) { *com = ITM_NORML;} else
//                      if (strcompare(str,"NORMLGAUSS_WHITE_LGAUSS_BLACK_R" )) { *com = ITM_NORMLU;} else
//                      if (strcompare(str,"NORML^MINUS_1" )) { *com = ITM_NORMLM1;} else
//                      if (strcompare(str,"POISSP" )) { *com = ITM_POISSP;} else
//                      if (strcompare(str,"POISSGAUSS_BLACK_LGAUSS_WHITE_R" )) { *com = ITM_POISS;} else
//                      if (strcompare(str,"POISSGAUSS_WHITE_LGAUSS_BLACK_R" )) { *com = ITM_POISSU;} else
//                      if (strcompare(str,"POISS^MINUS_1" )) { *com = ITM_POISSM1;} else
//                      if (strcompare(str,"TP(X)" )) { *com = ITM_TPX;} else
//                      if (strcompare(str,"TGAUSS_BLACK_LGAUSS_WHITE_R(X)" )) { *com = ITM_TX;} else
//                      if (strcompare(str,"TGAUSS_WHITE_LGAUSS_BLACK_R(X)" )) { *com = ITM_TUX;} else
//                      if (strcompare(str,"T^MINUS_1(P)" )) { *com = ITM_TM1P;} else
//                      if (strcompare(str,"WEIBLP" )) { *com = ITM_WEIBLP;} else
//                      if (strcompare(str,"WEIBLGAUSS_BLACK_LGAUSS_WHITE_R" )) { *com = ITM_WEIBL;} else
//                      if (strcompare(str,"WEIBLGAUSS_WHITE_LGAUSS_BLACK_R" )) { *com = ITM_WEIBLU;} else
//                      if (strcompare(str,"WEIBL^MINUS_1" )) { *com = ITM_WEIBLM1;} else
//                      if (strcompare(str,"CHI^2P(X)" )) { *com = ITM_chi2Px;} else
//                      if (strcompare(str,"CHI^2GAUSS_BLACK_LGAUSS_WHITE_R(X)" )) { *com = ITM_chi2x;} else
//                      if (strcompare(str,"CHI^2GAUSS_WHITE_LGAUSS_BLACK_R(X)" )) { *com = ITM_chi2ux;} else
//                      if (strcompare(str,"(CHI^2)^MINUS_1" )) { *com = ITM_chi2M1;} else
//                      if (strcompare(str,"BESTF" )) { *com = ITM_BESTF;} else
//                      if (strcompare(str,"EXPF" )) { *com = ITM_EXPF;} else
//                      if (strcompare(str,"LINF" )) { *com = ITM_LINF;} else
//                      if (strcompare(str,"LOGF" )) { *com = ITM_LOGF;} else
//                      if (strcompare(str,"ORTHOF" )) { *com = ITM_ORTHOF;} else
//                      if (strcompare(str,"POWERF" )) { *com = ITM_POWERF;} else
//                      if (strcompare(str,"GAUSSF" )) { *com = ITM_GAUSSF;} else
//                      if (strcompare(str,"CAUCHF" )) { *com = ITM_CAUCHF;} else
//                      if (strcompare(str,"PARABF" )) { *com = ITM_PARABF;} else
//                      if (strcompare(str,"HYPF" )) { *com = ITM_HYPF;} else
//                      if (strcompare(str,"ROOTF" )) { *com = ITM_ROOTF;} else
//                      if (strcompare(str,"1COMPL" )) { *com = ITM_1COMPL;} else
                      if (strcompare(str,"SNAP" )) { *com = ITM_SNAP;} else
//                      if (strcompare(str,"2COMPL" )) { *com = ITM_2COMPL;} else
                      if (strcompare(str,"ABS" )) { *com = ITM_ABS;} else
//                      if (strcompare(str,"AGM" )) { *com = ITM_AGM;} else
//                      if (strcompare(str,"AGRAPH" )) { *com = ITM_AGRAPH;} else
                      if (strcompare(str,"ALL" )) { *com = ITM_ALL;} else
//                      if (strcompare(str,"ASN" )) { *com = ITM_ASSIGN;} else
//                      if (strcompare(str,"BACK" )) { *com = ITM_BACK;} else
                      if (strcompare(str,"BATT?" )) { *com = ITM_BATT;} else
//                      if (strcompare(str,"BEEP" )) { *com = ITM_BEEP;} else
//                      if (strcompare(str,"BEGINP" )) { *com = ITM_BEGINP;} else
//                      if (strcompare(str,"BN" )) { *com = ITM_BN;} else
//                      if (strcompare(str,"BN^ASTERISK" )) { *com = ITM_BNS;} else
                      if (strcompare(str,"CASE" )) { *com = ITM_CASE;} else
//                      if (strcompare(str,"CLALL" )) { *com = ITM_CLALL;} else
//                      if (strcompare(str,"CLCVAR" )) { *com = ITM_CLCVAR;} else
//                      if (strcompare(str,"CLFALL" )) { *com = ITM_CLFALL;} else
//                      if (strcompare(str,"CLLCD" )) { *com = ITM_CLLCD;} else
//                      if (strcompare(str,"CLMENU" )) { *com = ITM_CLMENU;} else
//                      if (strcompare(str,"CLP" )) { *com = ITM_CLP;} else
//                      if (strcompare(str,"CLPALL" )) { *com = ITM_CLPALL;} else
//                      if (strcompare(str,"CLREGS" )) { *com = ITM_CLREGS;} else
                      if (strcompare(str,"CLSTK" )) { *com = ITM_CLSTK;} else
                      if (strcompare(str,"CLSUM" )) { *com = ITM_CLSIGMA;} else
//                      if (strcompare(str,"CONJ" )) { *com = ITM_CONJ;} else
//                      if (strcompare(str,"CORR" )) { *com = ITM_CORR;} else
//                      if (strcompare(str,"COV" )) { *com = ITM_COV;} else
//                      if (strcompare(str,"BESTF?" )) { *com = ITM_BESTFQ;} else
//                      if (strcompare(str,"CROSS" )) { *com = ITM_CROSS_PROD;} else
//                      if (strcompare(str,"CX>RE" )) { *com = ITM_CXtoRE;} else
//                      if (strcompare(str,"DATE" )) { *com = ITM_DATE;} else
//                      if (strcompare(str,"DATE>" )) { *com = ITM_DATEto;} else
//                      if (strcompare(str,"DAY" )) { *com = ITM_DAY;} else
//                      if (strcompare(str,"DBLR" )) { *com = ITM_DBLR;} else
//                      if (strcompare(str,"DBLCROSS" )) { *com = ITM_DBLMULT;} else
//                      if (strcompare(str,"DBL/" )) { *com = ITM_DBLDIV;} else
//                      if (strcompare(str,"DECOMP" )) { *com = ITM_DECOMP;} else
                      if (strcompare(str,"DEG" )) { *com = ITM_DEG;} else
//                      if (strcompare(str,"DEG>" )) { *com = ITM_DEGto;} else
//                      if (strcompare(str,"S(A)" )) { *com = ITM_SA;} else
//                      if (strcompare(str,"DENMAX" )) { *com = ITM_DENMAX;} else
//                      if (strcompare(str,"DOT" )) { *com = ITM_DOT_PROD;} else
//                      if (strcompare(str,"DSTACK" )) { *com = ITM_DSTACK;} else
//                      if (strcompare(str,"D.MS" )) { *com = ITM_DMS;} else
//                      if (strcompare(str,"D.MS>" )) { *com = ITM_DMSto;} else
//                      if (strcompare(str,"D.MY" )) { *com = ITM_DMY;} else
//                      if (strcompare(str,"D>J" )) { *com = ITM_DtoJ;} else
//                      if (strcompare(str,"DELITM" )) { *com = ITM_DELITM;} else
//                      if (strcompare(str,"EIGVAL" )) { *com = ITM_EIGVAL;} else
//                      if (strcompare(str,"EIGVEC" )) { *com = ITM_EIGVEC;} else
//                      if (strcompare(str,"END" )) { *com = ITM_END;} else
//                      if (strcompare(str,"ENDP" )) { *com = ITM_ENDP;} else
                      if (strcompare(str,"ENG" )) { *com = ITM_ENG;} else
//                      if (strcompare(str,"ENORM" )) { *com = ITM_ENORM;} else
//                      if (strcompare(str,"EQ.DEL" )) { *com = ITM_EQ_DEL;} else
//                      if (strcompare(str,"EQ.EDI" )) { *com = ITM_EQ_EDI;} else
//                      if (strcompare(str,"EQ.NEW" )) { *com = ITM_EQ_NEW;} else
//                      if (strcompare(str,"ERF" )) { *com = ITM_ERF;} else
//                      if (strcompare(str,"ERFC" )) { *com = ITM_ERFC;} else
//                      if (strcompare(str,"ERR" )) { *com = ITM_ERR;} else
//                      if (strcompare(str,"EXITALL" )) { *com = ITM_EXITALL;} else
                      if (strcompare(str,"EXPT" )) { *com = ITM_EXPT;} else
//                      if (strcompare(str,"J/G?" )) { *com = ITM_GET_JUL_GREG;} else
                      if (strcompare(str,"FIB" )) { *com = ITM_FIB;} else
                      if (strcompare(str,"FIX" )) { *com = ITM_FIX;} else
//                      if (strcompare(str,"FLASH?" )) { *com = ITM_FLASH;} else
//                      if (strcompare(str,"F'(X)" )) { *com = ITM_FQX;} else
//                      if (strcompare(str,"F\(X)" )) { *com = ITM_FDQX;} else
//                      if (strcompare(str,"GAP" )) { *com = ITM_GAP;} else
                      if (strcompare(str,"GD" )) { *com = ITM_GD;} else
                      if (strcompare(str,"GD^-1" )) { *com = ITM_GDM1;} else
//                      if (strcompare(str,"GRAD" )) { *com = ITM_GRAD;} else
//                      if (strcompare(str,"GRAD>" )) { *com = ITM_GRADto;} else
//                      if (strcompare(str,"GTO." )) { *com = ITM_GTOP;} else
//                      if (strcompare(str,"HN" )) { *com = ITM_HN;} else
//                      if (strcompare(str,"HNP" )) { *com = ITM_HNP;} else
                      if (strcompare(str,"IM" )) { *com = ITM_IM;} else
//                      if (strcompare(str,"INDEX" )) { *com = ITM_INDEX;} else
//                      if (strcompare(str,"IXYZ" )) { *com = ITM_IXYZ;} else
//                      if (strcompare(str,"IGAMMAP" )) { *com = ITM_IGAMMAP;} else
//                      if (strcompare(str,"IGAMMAQ" )) { *com = ITM_IGAMMAQ;} else
//                      if (strcompare(str,"I+" )) { *com = ITM_IPLUS;} else
//                      if (strcompare(str,"I-" )) { *com = ITM_IMINUS;} else
//                      if (strcompare(str,"JY(X)" )) { *com = ITM_JYX;} else
//                      if (strcompare(str,"J+" )) { *com = ITM_JPLUS;} else
//                      if (strcompare(str,"J-" )) { *com = ITM_JMINUS;} else
//                      if (strcompare(str,"J/G" )) { *com = ITM_JUL_GREG;} else
//                      if (strcompare(str,"J>D" )) { *com = ITM_JtoD;} else
//                      if (strcompare(str,"KEY" )) { *com = ITM_KEY;} else
//                      if (strcompare(str,"KEYG" )) { *com = ITM_KEYG;} else
//                      if (strcompare(str,"KEYX" )) { *com = ITM_KEYX;} else
                      if (strcompare(str,"SINC" )) { *com = ITM_sinc;} else
//                      if (strcompare(str,"KTYP?" )) { *com = ITM_KTYP;} else
//                      if (strcompare(str,"LASTX" )) { *com = ITM_LASTX;} else
//                      if (strcompare(str,"LBL?" )) { *com = ITM_LBLQ;} else
//                      if (strcompare(str,"LEAP?" )) { *com = ITM_LEAP;} else
//                      if (strcompare(str,"LM" )) { *com = ITM_Lm;} else
//                      if (strcompare(str,"LMALPHA" )) { *com = ITM_LmALPHA;} else
//                      if (strcompare(str,"LNBETA" )) { *com = ITM_LNBETA;} else
//                      if (strcompare(str,"LNGAMMA" )) { *com = ITM_LNGAMMA;} else
//                      if (strcompare(str,"LOAD" )) { *com = ITM_LOAD;} else
//                      if (strcompare(str,"LOADP" )) { *com = ITM_LOADP;} else
//                      if (strcompare(str,"LOADR" )) { *com = ITM_LOADR;} else
//                      if (strcompare(str,"LOADSS" )) { *com = ITM_LOADSS;} else
//                      if (strcompare(str,"LOADSUM" )) { *com = ITM_LOADSIGMA;} else
//                      if (strcompare(str,"LOCR" )) { *com = ITM_LocR;} else
//                      if (strcompare(str,"LOCR?" )) { *com = ITM_LocRQ;} else
//                      if (strcompare(str,"L.R." )) { *com = ITM_LR;} else
//                      if (strcompare(str,"MANT" )) { *com = ITM_MANT;} else
//                      if (strcompare(str,"MAT_X" )) { *com = ITM_MATX;} else
//                      if (strcompare(str,"MEM?" )) { *com = ITM_MEM;} else
//                      if (strcompare(str,"MENU" )) { *com = ITM_MENU;} else
//                      if (strcompare(str,"MONTH" )) { *com = ITM_MONTH;} else
//                      if (strcompare(str,"MSG" )) { *com = ITM_MSG;} else
                      if (strcompare(str,"MULPI" )) { *com = ITM_MULPI;} else
//                      if (strcompare(str,"MVAR" )) { *com = ITM_MVAR;} else
//                      if (strcompare(str,"M.DELR" )) { *com = ITM_M_DELR;} else
//                      if (strcompare(str,"M.DIM" )) { *com = ITM_M_DIM;} else
//                      if (strcompare(str,"M.DIM?" )) { *com = ITM_M_DIMQ;} else
//                      if (strcompare(str,"M.DY" )) { *com = ITM_MDY;} else
//                      if (strcompare(str,"M.EDI" )) { *com = ITM_M_EDI;} else
//                      if (strcompare(str,"M.EDIN" )) { *com = ITM_M_EDIN;} else
//                      if (strcompare(str,"M.GET" )) { *com = ITM_M_GET;} else
//                      if (strcompare(str,"M.GOTO" )) { *com = ITM_M_GOTO;} else
//                      if (strcompare(str,"M.GROW" )) { *com = ITM_M_GROW;} else
//                      if (strcompare(str,"M.INSR" )) { *com = ITM_M_INSR;} else
//                      if (strcompare(str,"M.LU" )) { *com = ITM_M_LU;} else
//                      if (strcompare(str,"M.NEW" )) { *com = ITM_M_NEW;} else
//                      if (strcompare(str,"M.OLD" )) { *com = ITM_M_OLD;} else
//                      if (strcompare(str,"M.PUT" )) { *com = ITM_M_PUT;} else
//                      if (strcompare(str,"M.R<>R" )) { *com = ITM_M_RR;} else
                      if (strcompare(str,"SINCPI" )) { *com = ITM_sincpi;} else
//                      if (strcompare(str,"M.WRAP" )) { *com = ITM_M_WRAP;} else
//                      if (strcompare(str,"NOP" )) { *com = ITM_NOP;} else
//                      if (strcompare(str,"OFF" )) { *com = ITM_OFF;} else
//                      if (strcompare(str,"DROPY" )) { *com = ITM_DROPY;} else
//                      if (strcompare(str,"PGMINT" )) { *com = ITM_PGMINT;} else
//                      if (strcompare(str,"PGMSLV" )) { *com = ITM_PGMSLV;} else
//                      if (strcompare(str,"PIXEL" )) { *com = ITM_PIXEL;} else
                      if (strcompare(str,"PLOT" )) { *com = ITM_PLOT;} else
//                      if (strcompare(str,"PN" )) { *com = ITM_Pn;} else
//                      if (strcompare(str,"POINT" )) { *com = ITM_POINT;} else
//                      if (strcompare(str,"LOADV" )) { *com = ITM_LOADV;} else
//                      if (strcompare(str,"POPLR" )) { *com = ITM_POPLR;} else
//                      if (strcompare(str,"PRCL" )) { *com = ITM_PRCL;} else
//                      if (strcompare(str,"PSTO" )) { *com = ITM_PSTO;} else
//                      if (strcompare(str,"PUTK" )) { *com = ITM_PUTK;} else
                      if (strcompare(str,"RAD" )) { *com = ITM_RAD;} else
//                      if (strcompare(str,"RAD>" )) { *com = ITM_RADto;} else
                      if (strcompare(str,"RAN#" )) { *com = ITM_RAN;} else
//                      if (strcompare(str,"REGS.V" )) { *com = ITM_RBR;} else
//                      if (strcompare(str,"RCLCFG" )) { *com = ITM_RCLCFG;} else
//                      if (strcompare(str,"RCLEL" )) { *com = ITM_RCLEL;} else
//                      if (strcompare(str,"RCLIJ" )) { *com = ITM_RCLIJ;} else
//                      if (strcompare(str,"RCLS" )) { *com = ITM_RCLS;} else
//                      if (strcompare(str,"RDP" )) { *com = ITM_RDP;} else
                      if (strcompare(str,"RE" )) { *com = ITM_RE;} else
//                      if (strcompare(str,"RECV" )) { *com = ITM_RECV;} else
//                      if (strcompare(str,"RESET" )) { *com = ITM_RESET;} else
//                      if (strcompare(str,"RE>CX" )) { *com = ITM_REtoCX;} else
                      if (strcompare(str,"RE<>IM" )) { *com = ITM_REexIM;} else
//                      if (strcompare(str,"RMODE" )) { *com = ITM_RM;} else
//                      if (strcompare(str,"RMODE?" )) { *com = ITM_RMQ;} else
//                      if (strcompare(str,"RNORM" )) { *com = ITM_RNORM;} else
                      if (strcompare(str,"E^X-1" )) { *com = ITM_EX1;} else
//                      if (strcompare(str,"RSD" )) { *com = ITM_RSD;} else
//                      if (strcompare(str,"RSUM" )) { *com = ITM_RSUM;} else
//                      if (strcompare(str,"RTN+1" )) { *com = ITM_RTNP1;} else
//                      if (strcompare(str,"R-CLR" )) { *com = ITM_R_CLR;} else
//                      if (strcompare(str,"R-COPY" )) { *com = ITM_R_COPY;} else
//                      if (strcompare(str,"R-SORT" )) { *com = ITM_R_SORT;} else
//                      if (strcompare(str,"R-SWAP" )) { *com = ITM_R_SWAP;} else
//                      if (strcompare(str,"S" )) { *com = ITM_STDDEVWEIGHTED;} else
//                      if (strcompare(str,"SAVE" )) { *com = ITM_SAVE;} else
                      if (strcompare(str,"SCI" )) { *com = ITM_SCI;} else
//                      if (strcompare(str,"SDIGS?" )) { *com = ITM_SDIGS;} else
//                      if (strcompare(str,"SEED" )) { *com = ITM_SEED;} else
//                      if (strcompare(str,"SEND" )) { *com = ITM_SEND;} else
//                      if (strcompare(str,"SETCHN" )) { *com = ITM_SETCHN;} else
//                      if (strcompare(str,"SETDAT" )) { *com = ITM_SETDAT;} else
//                      if (strcompare(str,"SETEUR" )) { *com = ITM_SETEUR;} else
//                      if (strcompare(str,"SETIND" )) { *com = ITM_SETIND;} else
//                      if (strcompare(str,"SETJPN" )) { *com = ITM_SETJPN;} else
//                      if (strcompare(str,"SETSIG" )) { *com = ITM_SETSIG;} else
//                      if (strcompare(str,"SETTIM" )) { *com = ITM_SETTIM;} else
//                      if (strcompare(str,"SETUK" )) { *com = ITM_SETUK;} else
//                      if (strcompare(str,"SETUSA" )) { *com = ITM_SETUSA;} else
//                      if (strcompare(str,"SIGN" )) { *com = ITM_SIGN;} else
//                      if (strcompare(str,"SIGNMT" )) { *com = ITM_SIGNMT;} else
//                      if (strcompare(str,"SIM_EQ" )) { *com = ITM_SIM_EQ;} else
//                      if (strcompare(str,"SKIP" )) { *com = ITM_SKIP;} else
//                      if (strcompare(str,"SLVQ" )) { *com = ITM_SLVQ;} else
//                      if (strcompare(str,"SM" )) { *com = ITM_SM;} else
//                      if (strcompare(str,"ISM?" )) { *com = ITM_ISM;} else
//                      if (strcompare(str,"SMW" )) { *com = ITM_SMW;} else
//                      if (strcompare(str,"SOLVE" )) { *com = ITM_SOLVE;} else
//                      if (strcompare(str,"SSIZE?" )) { *com = ITM_SSIZE;} else
//                      if (strcompare(str,"STATUS" )) { *com = ITM_STATUS;} else
//                      if (strcompare(str,"STOCFG" )) { *com = ITM_STOCFG;} else
//                      if (strcompare(str,"STOEL" )) { *com = ITM_STOEL;} else
//                      if (strcompare(str,"STOIJ" )) { *com = ITM_STOIJ;} else
                      if (strcompare(str,"LN(1+X)" )) { *com = ITM_LN1X;} else
//                      if (strcompare(str,"STOS" )) { *com = ITM_STOS;} else
//                      if (strcompare(str,"SUM" )) { *com = ITM_SUM;} else
//                      if (strcompare(str,"SW" )) { *com = ITM_SW;} else
//                      if (strcompare(str,"SXY" )) { *com = ITM_SXY;} else
//                      if (strcompare(str,"TDISP" )) { *com = ITM_TDISP;} else
                      if (strcompare(str,"TICKS" )) { *com = ITM_TICKS;} else
//                      if (strcompare(str,"TIME" )) { *com = ITM_TIME;} else
//                      if (strcompare(str,"TIMER" )) { *com = ITM_TIMER;} else
//                      if (strcompare(str,"TN" )) { *com = ITM_Tn;} else
//                      if (strcompare(str,"TONE" )) { *com = ITM_TONE;} else
//                      if (strcompare(str,"T<>" )) { *com = ITM_Tex;} else
//                      if (strcompare(str,"ULP?" )) { *com = ITM_ULP;} else
//                      if (strcompare(str,"UN" )) { *com = ITM_Un;} else
//                      if (strcompare(str,"UNITV" )) { *com = ITM_UNITV;} else
//                      if (strcompare(str,"UNSIGN" )) { *com = ITM_UNSIGN;} else
//                      if (strcompare(str,"VARMNU" )) { *com = ITM_VARMNU;} else
//                      if (strcompare(str,"VERS?" )) { *com = ITM_VERS;} else
//                      if (strcompare(str,"IDIVR" )) { *com = ITM_IDIVR;} else
//                      if (strcompare(str,"WDAY" )) { *com = ITM_WDAY;} else
//                      if (strcompare(str,"WHO?" )) { *com = ITM_WHO;} else
//                      if (strcompare(str,"WM" )) { *com = ITM_WM;} else
//                      if (strcompare(str,"WP" )) { *com = ITM_WP;} else
//                      if (strcompare(str,"W^MINUS_1" )) { *com = ITM_WM1;} else
//                      if (strcompare(str,"WSIZE" )) { *com = ITM_WSIZE;} else
//                      if (strcompare(str,"WSIZE?" )) { *com = ITM_WSIZEQ;} else
//                      if (strcompare(str,"X_MEAN" )) { *com = ITM_XBAR;} else
//                      if (strcompare(str,"X_GEO" )) { *com = ITM_XG;} else
//                      if (strcompare(str,"X_WTD" )) { *com = ITM_XW;} else
//                      if (strcompare(str,"X_CIRC" )) { *com = ITM_XCIRC;} else
//                      if (strcompare(str,"X>DATE" )) { *com = ITM_XtoDATE;} else
//                      if (strcompare(str,"X>ALPHA" )) { *com = ITM_XtoALPHA;} else
//                      if (strcompare(str,"YEAR" )) { *com = ITM_YEAR;} else
//                      if (strcompare(str,"Y_CIRC" )) { *com = ITM_YCIRC;} else
//                      if (strcompare(str,"Y.MD" )) { *com = ITM_YMD;} else
//                      if (strcompare(str,"Y<>" )) { *com = ITM_Yex;} else
//                      if (strcompare(str,"Z<>" )) { *com = ITM_Zex;} else
//                      if (strcompare(str,"XMAX" )) { *com = ITM_XMAX;} else
//                      if (strcompare(str,"XMIN" )) { *com = ITM_XMIN;} else
//                      if (strcompare(str,"BETA" )) { *com = ITM_BETAXY;} else
//                      if (strcompare(str,"GAMMAXY" )) { *com = ITM_gammaXY;} else
//                      if (strcompare(str,"GAMMAXY" )) { *com = ITM_GAMMAXY;} else
//                      if (strcompare(str,"GAMMA" )) { *com = ITM_GAMMAX;} else
//                      if (strcompare(str,"DELTA%" )) { *com = ITM_DELTAPC;} else
//                      if (strcompare(str,"EPSILON" )) { *com = ITM_SCATTFACT;} else
//                      if (strcompare(str,"EPSILONM" )) { *com = ITM_SCATTFACTm;} else
//                      if (strcompare(str,"EPSILONP" )) { *com = ITM_SCATTFACTp;} else
//                      if (strcompare(str,"ZETA(X)" )) { *com = ITM_zetaX;} else
//                      if (strcompare(str,"PIN" )) { *com = ITM_PIn;} else
//                      if (strcompare(str,"SUMN" )) { *com = ITM_SIGMAn;} else
//                      if (strcompare(str,"SUM" )) { *com = ITM_STDDEV;} else
//                      if (strcompare(str,"SUMW" )) { *com = ITM_STDDEVPOP;} else
//                      if (strcompare(str,"RANI#" )) { *com = ITM_RANI;} else
//                      if (strcompare(str,"PRINTERX" )) { *com = ITM_PRINTERX;} else
//                      if (strcompare(str,"RANGE" )) { *com = ITM_RANGE;} else
//                      if (strcompare(str,"RANGE?" )) { *com = ITM_GETRANGE;} else
                      if (strcompare(str,"(-1)^X" )) { *com = ITM_M1X;} else
//                      if (strcompare(str,"CROSSMOD" )) { *com = ITM_XMOD;} else
//                      if (strcompare(str,">DATE" )) { *com = ITM_toDATE;} else
//                      if (strcompare(str,">HR" )) { *com = ITM_toHR;} else
//                      if (strcompare(str,">H.MS" )) { *com = ITM_toHMS;} else
//                      if (strcompare(str,">INT" )) { *com = ITM_toINT;} else
                      if (strcompare(str,">REAL" )) { *com = ITM_toREAL;} else
//                      if (strcompare(str,"D>D.MS" )) { *com = ITM_DtoDMS;} else
//                      if (strcompare(str,"<>" )) { *com = ITM_SHUFFLE;} else
//                      if (strcompare(str,"%" )) { *com = ITM_PC;} else
//                      if (strcompare(str,"%MRR" )) { *com = ITM_PCMRR;} else
//                      if (strcompare(str,"%T" )) { *com = ITM_PCT;} else
//                      if (strcompare(str,"%SUM" )) { *com = ITM_PCSIGMA;} else
//                      if (strcompare(str,"%+MG" )) { *com = ITM_PCPMG;} else
//                      if (strcompare(str,"INTEGRAL" )) { *com = ITM_INTEGRAL;} else
//                      if (strcompare(str,"^MOD" )) { *com = ITM_PMOD;} else
//                      if (strcompare(str,"|M|" )) { *com = ITM_M_DET;} else
//                      if (strcompare(str,"PARL" )) { *com = ITM_PARALLEL;} else
//                      if (strcompare(str,"[M]^T" )) { *com = ITM_M_TRANSP;} else
//                      if (strcompare(str,"[M]^MINUS_1" )) { *com = ITM_M_INV;} else
                      if (strcompare(str,"ARG" )) { *com = ITM_ANGLE;} else
//                      if (strcompare(str,"MULPI>" )) { *com = ITM_MULPIto;} else
//                      if (strcompare(str,"PRINTERADV" )) { *com = ITM_PRINTERADV;} else
//                      if (strcompare(str,"PRINTERCHAR" )) { *com = ITM_PRINTERCHAR;} else
//                      if (strcompare(str,"PRINTERDLAY" )) { *com = ITM_PRINTERDLAY;} else
//                      if (strcompare(str,"PRINTERLCD" )) { *com = ITM_PRINTERLCD;} else
//                      if (strcompare(str,"PRINTERMODE" )) { *com = ITM_PRINTERMODE;} else
//                      if (strcompare(str,"PRINTERPROG" )) { *com = ITM_PRINTERPROG;} else
//                      if (strcompare(str,"PRINTERR" )) { *com = ITM_PRINTERR;} else
//                      if (strcompare(str,"PRINTERREGS" )) { *com = ITM_PRINTERREGS;} else
//                      if (strcompare(str,"PRINTERSTK" )) { *com = ITM_PRINTERSTK;} else
//                      if (strcompare(str,"PRINTERTAB" )) { *com = ITM_PRINTERTAB;} else
//                      if (strcompare(str,"PRINTERUSER" )) { *com = ITM_PRINTERUSER;} else
//                      if (strcompare(str,"PRINTERWIDTH" )) { *com = ITM_PRINTERWIDTH;} else
//                      if (strcompare(str,"PRINTERSUM" )) { *com = ITM_PRINTERSIGMA;} else
//                      if (strcompare(str,"PRINTER#" )) { *com = ITM_PRINTERHASH;} else
//                      if (strcompare(str,"FBR" )) { *com = ITM_FBR;} else
                      if (strcompare(str,"EXIT" )) { *com = ITM_EXIT1;} else
                      if (strcompare(str,"ALPHA" )) { *com = ITM_AIM;} else
                      if (strcompare(str,"DOTD" )) { *com = ITM_dotD;} else
//                      if (strcompare(str,"SHOW" )) { *com = ITM_SHOW;} else
//                      if (strcompare(str,"SYSTEM" )) { *com = ITM_SYSTEM;} else
//                      if (strcompare(str,"D.MS>D" )) { *com = ITM_DMStoD;} else
//                      if (strcompare(str,"X_HARM" )) { *com = ITM_XH;} else
//                      if (strcompare(str,"X_RMS" )) { *com = ITM_XRMS;} else
//                      if (strcompare(str,"DET" )) { *com = ITM_DET;} else
//                      if (strcompare(str,"INVRT" )) { *com = ITM_INVRT;} else
//                      if (strcompare(str,"TRANS" )) { *com = ITM_TRANS;} else
//                      if (strcompare(str,"D>Y" )) { *com = ITM_EE_D2Y;} else
//                      if (strcompare(str,"Y>D" )) { *com = ITM_EE_Y2D;} else
//                      if (strcompare(str,"ATOSYM" )) { *com = ITM_EE_A2S;} else
//                      if (strcompare(str,"SYMTOA" )) { *com = ITM_EE_S2A;} else
//                      if (strcompare(str,"E^THETAJ" )) { *com = ITM_EE_EXP_TH;} else
//                      if (strcompare(str,"STO3Z" )) { *com = ITM_EE_STO_Z;} else
//                      if (strcompare(str,"RCL3Z" )) { *com = ITM_EE_RCL_Z;} else
//                      if (strcompare(str,"STO3V" )) { *com = ITM_EE_STO_V;} else
//                      if (strcompare(str,"RCL3V" )) { *com = ITM_EE_RCL_V;} else
//                      if (strcompare(str,"STO3I" )) { *com = ITM_EE_STO_I;} else
//                      if (strcompare(str,"RCL3I" )) { *com = ITM_EE_RCL_I;} else
//                      if (strcompare(str,"3V/3I" )) { *com = ITM_EE_STO_V_I;} else
//                      if (strcompare(str,"3Ix3Z" )) { *com = ITM_EE_STO_IR;} else
//                      if (strcompare(str,"3V/3Z" )) { *com = ITM_EE_STO_V_Z;} else
//                      if (strcompare(str,"X>BAL" )) { *com = ITM_EE_X2BAL;} else
                      if (strcompare(str,"TEST_45" )) { *com = ITM_PGMTST;} else
//                      if (strcompare(str,"OP_A" )) { *com = ITM_op_a;} else
//                      if (strcompare(str,"OP_A^2" )) { *com = ITM_op_a2;} else
//                      if (strcompare(str,"OP_J" )) { *com = ITM_op_j;} else
//                      if (strcompare(str,">BIN" )) { *com = ITM_2BIN;} else
//                      if (strcompare(str,">OCT" )) { *com = ITM_2OCT;} else
//                      if (strcompare(str,">DEC" )) { *com = ITM_2DEC;} else
//                      if (strcompare(str,">HEX" )) { *com = ITM_2HEX;} else
//                      if (strcompare(str,"HOUR" )) { *com = ITM_HR_DEG;} else
                      if (strcompare(str,"MIN" )) { *com = ITM_MINUTE;} else
//                      if (strcompare(str,"SEC" )) { *com = ITM_SECOND;} else
//                      if (strcompare(str,">TIME" )) { *com = ITM_toTIME;} else
//                      if (strcompare(str,"TIME>" )) { *com = ITM_TIMEto;} else
                      if (strcompare(str,"COMPLEX" )) { *com = KEY_COMPLEX;} else
                      if (strcompare(str,">POLAR" )) { *com = ITM_toPOL2;} else
                      if (strcompare(str,">RECT" )) { *com = ITM_toREC2;} else
                      if (strcompare(str,"ERPN" )) { *com = ITM_eRPN_ON;} else
                      if (strcompare(str,"RPN" )) { *com = ITM_eRPN_OFF;} else
                      if (strcompare(str,"SIG" )) { *com = ITM_SIGFIG;} else
//                      if (strcompare(str,"UNIT" )) { *com = ITM_UNIT;} else
                      if (strcompare(str,"ROUND" )) { *com = ITM_ROUND2;} else
                      if (strcompare(str,"ROUNDI" )) { *com = ITM_ROUNDI2;} else
//                      if (strcompare(str,">I" )) { *com = ITM_RI;} else
//                      if (strcompare(str,"DRG" )) { *com = ITM_DRG;} else
                      if (strcompare(str,"CASEUP" )) { *com = CHR_caseUP;} else
                      if (strcompare(str,"CASEDN" )) { *com = CHR_caseDN;} else
                      if (strcompare(str,"LISTXY" )) { *com = ITM_LISTXY;} else
//                      if (strcompare(str,"ERPN?" )) { *com = ITM_SH_ERPN;} else
//                      if (strcompare(str,"X.XEQ" )) { *com = ITM_XXEQ;} else
//                      if (strcompare(str,"FLAGS.V" )) { *com = ITM_FLGSV;} else
//                      if (strcompare(str,"CPXI" )) { *com = ITM_CPXI;} else
//                      if (strcompare(str,"CPXJ" )) { *com = ITM_CPXJ;} else
//                      if (strcompare(str,"SSIZE4" )) { *com = ITM_SSIZE4;} else
//                      if (strcompare(str,"SSIZE8" )) { *com = ITM_SSIZE8;} else
//                      if (strcompare(str,".MS" )) { *com = ITM_ms;} else
//                      if (strcompare(str,">>DEG" )) { *com = ITM_DEG2;} else
//                      if (strcompare(str,">>D.MS" )) { *com = ITM_DMS2;} else
//                      if (strcompare(str,">>GRAD" )) { *com = ITM_GRAD2;} else
//                      if (strcompare(str,">>MULPI" )) { *com = ITM_MULPI2;} else
//                      if (strcompare(str,">>RAD" )) { *com = ITM_RAD2;} else
//                      if (strcompare(str,">>H.MS" )) { *com = ITM_HMS2;} else
//                      if (strcompare(str,"XEQM01" ) && exec) { *com = ITM_X_P1;} else
//                      if (strcompare(str,"XEQM02" ) && exec) { *com = ITM_X_P2;} else
//                      if (strcompare(str,"XEQM03" ) && exec) { *com = ITM_X_P3;} else
//                      if (strcompare(str,"XEQM04" ) && exec) { *com = ITM_X_P4;} else
//                      if (strcompare(str,"XEQM05" ) && exec) { *com = ITM_X_P5;} else
//                      if (strcompare(str,"XEQM06" ) && exec) { *com = ITM_X_P6;} else
//                      if (strcompare(str,"XEQM07" ) && exec) { *com = ITM_X_f1;} else
//                      if (strcompare(str,"XEQM08" ) && exec) { *com = ITM_X_f2;} else
//                      if (strcompare(str,"XEQM09" ) && exec) { *com = ITM_X_f3;} else
//                      if (strcompare(str,"XEQM10" ) && exec) { *com = ITM_X_f4;} else
//                      if (strcompare(str,"XEQM11" ) && exec) { *com = ITM_X_f5;} else
//                      if (strcompare(str,"XEQM12" ) && exec) { *com = ITM_X_f6;} else
//                      if (strcompare(str,"XEQM13" ) && exec) { *com = ITM_X_g1;} else
//                      if (strcompare(str,"XEQM14" ) && exec) { *com = ITM_X_g2;} else
//                      if (strcompare(str,"XEQM15" ) && exec) { *com = ITM_X_g3;} else
//                      if (strcompare(str,"XEQM16" ) && exec) { *com = ITM_X_g4;} else
//                      if (strcompare(str,"XEQM17" ) && exec) { *com = ITM_X_g5;} else
//                      if (strcompare(str,"XEQM18" ) && exec) { *com = ITM_X_g6;} else
//                      if (strcompare(str,"X.SAVE" )) { *com = ITM_XSAVE;} else
//                      if (strcompare(str,"X.LOAD" )) { *com = ITM_XLOAD;} else
//                      if (strcompare(str,"EXITCLR" )) { *com = ITM_CLAIM;} else
                      if (strcompare(str,"PLOTLS" )) { *com = ITM_PLOTLS;} else
                      if (strcompare(str,"P_INT" )) { *com = ITM_INTG;} else
                      if (strcompare(str,"P_DIFF" )) { *com = ITM_DIFF;} else
                      if (strcompare(str,"P_RMS" )) { *com = ITM_RMS;} else
                      if (strcompare(str,"P_SHADE" )) { *com = ITM_SHADE;} else
                      if (strcompare(str,"CLGRF" )) { *com = ITM_CLGRF;} else
                      if (strcompare(str,"PLOTXY" )) { *com = ITM_PLOT_XY;} else
                      if (strcompare(str,"PLTRST" )) { *com = ITM_PLOTRST;} else


//FROM SPREADSHEET ^^^ ****************************************************************************************************
                        {*com = 0;}
                      if(*com != 0) return true;
                      else          return false;
                    }




void execute_string(const char *inputstring, bool_t exec1) {
#ifndef TESTSUITE_BUILD
      int16_t commno;
      uint16_t ix, ix_m;
      uint16_t ix_m1 = 0;
      uint16_t ix_m2 = 0;
      uint16_t ix_m3 = 0;
      uint16_t ix_m4 = 0;
      int16_t no, ii;
      char     commandnumber[commandnumberl];
      char     aa[2], bb[2];
      bool_t   state_comments, state_commands, state_quotes;
      uint16_t xeqlblinprogress;
      uint16_t gotoinprogress;
      bool_t gotlabels = false;
      bool_t exec = false;
      bool_t go;
      running_program_jm = true;
      indic_x = 0;
      indic_y = SCREEN_HEIGHT-1;
      uint8_t starttoken = 0;
      #ifdef PC_BUILD
        uint16_t loopnumber = 0;
      #endif


      hourGlassIconEnabled = true;
      cancelFilename = true;
      showHideHourGlass();
      #ifdef DMCP_BUILD
        lcd_refresh();
      #else // !DMCP_BUILD
        refreshLcd(NULL);
      #endif // DMCP_BUILD

                                                 //If !gotlabels, means it is a scouting pass/parse to find and mark the goto labels M1-M4
    while(!gotlabels || (gotlabels && exec) ){   //scheme to use for label scouting and name processing in "false", and to do a two parse exec 
    
      #ifdef PC_BUILD_VERBOSE1
        #ifdef PC_BUILD
          printf("------Starting parse ------- Indexes: M1:%d M2:%d M3:%d M4:%d   EXEC:%d\n",ix_m1, ix_m2, ix_m3, ix_m4, exec);
          printf("|%s|\n",inputstring);
        #endif
      #endif
      xeqlblinprogress = 0;
      gotoinprogress   = 0;
      go = false;
      ix = 0;
      ix_m = 0;
      no = 0;
      state_comments = false;
      state_commands = false;
      state_quotes = false;
      commandnumber[0]=0;
      aa[0]=0;
#ifdef DMCP_BUILD
      while (inputstring[ix]!=0 && key_empty()) {
#else
      while (inputstring[ix]!=0) {
#endif
        strcpy(bb,aa);
        aa[0] = inputstring[ix];
        aa[1] = 0;

        switch(bb[0]) {//COMMAND can start after any of these: space, tab, cr, lf, comma, beginning of file
          case 32:
          case 8 :
          case 13:
          case 10:
          case 44:
          case 0 :  if( //COMMAND WORD START DETECTION +-*/ 0-9; A-Z; %; >; (; a-z; . 
                    (       aa[0]=='*' //42 // *
                        ||  aa[0]=='+' //43 // +
                        ||  aa[0]=='-' //45 // -
                        ||  aa[0]=='/' //47 // /
                        ||  aa[0]=='~' //126) //~
                        || (aa[0]>=48 && aa[0]<=57) //0-9
                        || (aa[0]>=65 && aa[0]<=90) //A-Z
                        || (aa[0]>=65+32 && aa[0]<=90+32) //a-z
                        || aa[0]=='%'
                        || aa[0]=='>'
                        || aa[0]=='('
                        || aa[0]=='.'
                        )
                    && !state_comments                 //If not inside comments
                    && !state_quotes                   //if not inside quotes
                    && !state_commands                 //Don't re-check until done
                    ) {
                        state_commands = true;         // Waiting to open command number or name: nnn
                      }
          default:;
        }
        
        if(state_comments && (aa[0] == 13 || aa[0] == 10)) {
          state_comments=false;
        } else
        switch(aa[0]) {
          case 47: if(bb[0] == 47 && state_comments == false) {//ADDED  STATE, SO //always switches on the comment, but not off. CR/LF cancels it
                      state_comments = true;           // Switch comment state on
                      state_commands = false;
                      state_quotes   = false;
                      commandnumber[0]=0;
                    }
                     break;
          case 34: if(!state_comments && !state_commands) {   // " Toggle quote state
                       state_quotes   = !state_quotes;
                     }
                     break;

          case 13: //cr
          case 10: //lf
          case 8 : //tab
          case 44: //,
          case 32: 
                   //printf("@@@ %s\n",commandnumber);
                   if(state_commands){
                      state_commands = false;                // Waiting for delimiter to close off and send command number: nnn<                 
                      #ifdef PC_BUILD_VERBOSE1
                        #ifdef PC_BUILD
                          printf("Command/number detected:(tempjm=%d)(gotoinprogress=%d) %45s \n",temporaryInformation,gotoinprogress,commandnumber);
                        #endif
                      #endif
                      
                      //DSZ:
                      if(!(gotoinprogress != 11 || (gotoinprogress == 11 && (temporaryInformation == TI_FALSE)))) {     //If DEC results in 0, then 'true'.    It is now the command that may or may not be skipped
                          //......IS NOT DSZ.... OR               DSZ    with REG NOT ZERO
                          go = (temporaryInformation == TI_FALSE); //As per GTO_SZ ---- REGISTER<>0, then go
                          //printf("   DSZ/ISZ temporaryInformation = %5d\n",temporaryInformation);
                          gotoinprogress = 1;                      //As per GTO_SZ
                          commandnumber[0]=0;                      //As per GTO_SZ
                      } else


// Unlimited GTO                                      GTO M1
// 4 Labels M1, M2, M3 & M4                           XEQLBL M1
// Unlimited DSZ and ISZ                              DSZ 00
// Non-nested subroutine GSB M1..M4; RTN              GSB M01     RTN
// END reacts to labelling parse and execution parse  END
// RETURN reacts to execution parse only              RETURN

                      if(checkindexes(&commno, commandnumber, exec)) {sprintf(commandnumber,"%d", commno);} else
                   
                      if (strcompare(commandnumber,"DSZ"    )) {sprintf(commandnumber,"%d", ITM_DEC); gotoinprogress = 9;}      else //EXPECTING FOLLOWING OPERAND "nn"
                       if (strcompare(commandnumber,"ISZ"   )) {sprintf(commandnumber,"%d", ITM_INC); gotoinprogress = 9;}       else //EXPECTING FOLLOWING OPERAND "nn"
                        if (strcompare(commandnumber,"LBL"))       {xeqlblinprogress = 10; }                                      else //EXPECTING FOLLOWING OPERAND Mn
                          if (strcompare(commandnumber,"XEQC43"))   {starttoken = 1; }                                             else //EXPECTING FOLLOWING OPERAND Mn
                           if (strcompare(commandnumber,"XEQLBL"))    {xeqlblinprogress =  1; starttoken = 1;}                      else //EXPECTING 2 OPERANDS nn XXXXXX
                            if (strcompare(commandnumber,"GTO"   ))    {
                                  if(exec) {go = true; gotoinprogress = 1;
                                  force_refresh();
                                  #ifdef PC_BUILD
                                    printf("   >>> Loop GTO Jump %d:go\n",loopnumber++);
                                  #endif
                              }
                            } else
                             if (strcompare(commandnumber,"GSB"   ))    {
                                  if(exec) {go = true; gotoinprogress = 1; 
                                  ix_m = ix;
                                  force_refresh();
                                  #ifdef PC_BUILD
                                    printf("   >>> Sub  GSB Jump %d:go storing return address %d\n",loopnumber++, ix_m);
                                  #endif
                               }
                             } else
                              if (strcompare(commandnumber,"RTN"))       {
                                  if(exec) {ix = ix_m+2; ix_m = 0;
                                  force_refresh();
                                  #ifdef PC_BUILD
                                    printf("   >>> Sub  RTN to return address %d\n", ix);
                                  #endif
                               }
                             } else
                               if (strcompare(commandnumber,"GTO_SZ"))    {if(exec) {go = (temporaryInformation == TI_FALSE); gotoinprogress = 1; }} else
                                if (strcompare(commandnumber,"END"))       {ix = stringByteLength(inputstring)-2;}                      else
                                 if (strcompare(commandnumber,"RETURN"))    {if(exec) {ix = stringByteLength(inputstring)-2;}}           else
//         END ELSE
                                   { 
                                     ii = 0;
                                     while(commandnumber[ii]!=0 && ((commandnumber[ii]<='9' && commandnumber[ii]>='0') || commandnumber[ii]>='.' || commandnumber[ii]>='E' ) ) {
                                       ii++;
                                     }
                                     if(commandnumber[ii]==0 && (gotoinprogress == 0 || gotoinprogress == 10) && xeqlblinprogress == 0 ) {
                                       //printf("   Fell thru, i.e. number %s, gotoinprogress %d\n",commandnumber,gotoinprogress);   // at this stage there SHOULD be a number (not checked) coming out at this point, from the ELSE, it is a number from the text file, therefore literal mnumbers
                                                                 // prepare to break out of this state
                                                                 // and set flags as if in direct character (quotes) state
                                       if (exec) {
                                         sendkeys(commandnumber);
                                         //printf("   sent-->a|%s|\n",commandnumber);
                                       }
                                       commandnumber[0]=0;
                                       if(gotoinprogress == 10) {gotoinprogress = 11;} //if the digits of STO or DSZ
                                       break;
                                     }
                                   }
        //v

                      if(starttoken == 0) {                  //if not started with XEQLBL or XEQC43, immediately abandon program mode
                        running_program_jm = false;
                        return;
                      }
                      
                      //printf("   gotoinprogress = %5d; xeqlblinprogress = %5d; commandnumber = %10s\n",gotoinprogress,xeqlblinprogress,commandnumber);


                      temporaryInformation   = TI_NO_INFO;   //Cancel after go was determined.
                      switch(gotoinprogress) {
                        case 1:                  //GOTO IN PROGRESS: got command GOTO. If arriving from DSZ with no label, just pass around and skip to 2
                          gotoinprogress = 2;
                          commandnumber[0]=0;
                        break;

                        case 2:                  //GOTO IN PROGRESS: if arriving from DSZ / 1, witho9ut M1-M4, waste the commandd, zero, and pass around
                          if(strcompare(commandnumber,"M1") && exec && go && (ix_m1 !=0)) ix = ix_m1; else
                          if(strcompare(commandnumber,"M2") && exec && go && (ix_m2 !=0)) ix = ix_m2; else 
                          if(strcompare(commandnumber,"M3") && exec && go && (ix_m3 !=0)) ix = ix_m3; else
                          if(strcompare(commandnumber,"M4") && exec && go && (ix_m4 !=0)) ix = ix_m4;
                          gotoinprogress = 0;
                          commandnumber[0]=0;   //Processed
                          go = false;
                        break;

                        case 13:                  //GOTO IN PROGRESS: eat one word
                          gotoinprogress = 0;
                          commandnumber[0]=0;
                        break;


                        default:;
                      }



                      switch(xeqlblinprogress) {
                        case 1:                  //XEQMLABEL IN PROGRESS: got command XEQLBL
                          xeqlblinprogress = 2;
                          commandnumber[0]=0;
                        break;

                        case 2:                  //XEQMLABEL IN PROGRESS: get softkeynumber 01 - 18
                          no = atoi(commandnumber);
                          if(no>=1 && no <=18) {
                            xeqlblinprogress = 3;
                            commandnumber[0]=0;   //Processed
                          } 
                          else {
                            xeqlblinprogress = 0;
                            commandnumber[0]=0;   //Processed
                          }
                        break;

                        case 3:                  //XEQMLABEL IN PROGRESS: get label
                          if (no>=1 && no<=18) {
                            char tmpp[commandnumberl];
                            strcpy(tmpp,commandnumber);
                            tmpp[8-1]=0;         //Limit to length of indexOfItemsXEQM
                            //printf(">>> Exec:%d no:%d ComndNo:%s tmpp:%s>>XEQM:%s\n",exec, no,commandnumber, tmpp,indexOfItemsXEQM + (no-1)*8);
                            if(!exec) strcpy(indexOfItemsXEQM + (no-1)*8, tmpp);        // At Exec time, the XEQM label is changed to the command number. So the re-allocation of the name can only happen in the !exec state
                            xeqlblinprogress = 0;
                            commandnumber[0]=0;  //Processed
                            #ifndef TESTSUITE_BUILD
//JMXX                            showSoftmenuCurrentPart(); //Redisplay because softkey content changed
                            #endif
                          }
                        break;

                        case 10:                  //LABEL IN PROGRESS: got command XEQLBL
                          xeqlblinprogress = 11;
                          commandnumber[0]=0;
                        break;

                        case 11:                  //LABEL IN PROGRESS: get label M1-M4
                          //printf("LABEL %s\n",commandnumber);
                          if(strcompare(commandnumber,"M1")) ix_m1 = ix; else
                          if(strcompare(commandnumber,"M2")) ix_m2 = ix; else
                          if(strcompare(commandnumber,"M3")) ix_m3 = ix; else
                          if(strcompare(commandnumber,"M4")) ix_m4 = ix;
                          xeqlblinprogress = 0;
                          commandnumber[0]=0;   //Processed
                          invert_Pixel(indic_x, indic_y-1);
                        break;
//HERE DEFAULT !!
//NOT IN PROGRESS                        
                        default:                 //NOT IN PROGRESS
                          no = atoi(commandnumber);       //Will force all unknown commands to have no number, and invalid command and RETURN MARK etc. to 0
                          //printf("   Command send %s EXEC=%d no=%d ",commandnumber,exec,no);
                          if(no > LAST_ITEM-1) {no = 0;}
                          if(no!=0 && exec) {
                            invert_Pixel(indic_x++, indic_y);
                            if(indic_x==SCREEN_WIDTH) {indic_x=0;indic_y--;indic_y--;}

                            if(exec) {
                              runkey(no); 
                              //printf("   -->%d sent ",no);
                            } else {
                              //printf("   -->%d not sent ",no);
                            }
                            //printf(">>> %d\n",temporaryInformation);
                            if(gotoinprogress == 9 ) {gotoinprogress = 10;}
                          }
                          else {
                            //printf("Skip execution |%s|",commandnumber);
                          }
                          //printf("#\n");
                          commandnumber[0]=0;   //Processed
                        break;
                      }
                    }
                    break;
          default:;           //ignore all other characters
        }
        if(state_quotes) {
          if (exec) {
            //printf("   sent-->b|%s|\n",aa);
            sendkeys(aa);} //else printf("Skip sending |%s|",aa);
        }
        else {
          if(state_commands && stringByteLength(commandnumber) < commandnumberl-1) {
            strcat(commandnumber,aa);
          }   // accumulate string
        }
        ix++;
      }

      gotlabels = true;                              //allow to run only once, unless
      if(!exec) exec = exec1; else exec = false;     //exec must run, and ensure it runs only once.
    }
running_program_jm = false;
#endif
}


 
void replaceFF(char* FF, char* line2) {
  int16_t ix =0;
  if(FF[0]>=48 && FF[0]<=57 && FF[1]>=48 && FF[1]<=57 && FF[2]==0) {
    while(line2[ix] != 0 && ix+10<stringByteLength(line2)) {
      if(line2[ix]==88 /*X*/ && line2[ix+1]==69 /*E*/ && line2[ix+2]==81 /*Q*/ && line2[ix+3]==76 /*L*/ && line2[ix+4]==66 /*B*/ && line2[ix+5]==76 /*L*/ && line2[ix+6]==32 && line2[ix+7]==70 /*F*/ && line2[ix+8]==70 /*F*/) {
        line2[ix+7]=FF[0];
        line2[ix+8]=FF[1];
      }
      ix++;
    }
  }
}

void XEQMENU_Selection(uint16_t selection, char *line1, bool_t exec) {
#ifndef TESTSUITE_BUILD
                                            //Read in XEQMINDEX.TXT file, with default XEQMnn file name replacements
  line1[0]=0;                               //Clear incoming/outgoing string data
  char nn[6];
  nn[0]=0;
  char fallback[130];     //Fallback text
  char fn_long[200];      //Long file name
  char fn_short[16];      //standard file name

  #if (VERBOSE_LEVEL >= 1) 
    char tmp[400];          //Messages
  #endif


  #define pgmpath "PROGRAMS"
  strcpy(fn_short,"XEQMINDEX.TXT");
  strcpy(fn_long, "");
  strcpy(fallback,"XEQM01:HELP;");

  #if (VERBOSE_LEVEL >= 1) 
    strcpy(tmp,fn_short); strcat(tmp," A: Loading XEQMENU mapping"); print_linestr(tmp,false);
  #endif

  import_string_from_filename(line1,pgmpath,fn_short,fn_long,fallback);

  #if (VERBOSE_LEVEL >= 1) 
   print_inlinestr(" B: Loaded. ",false);
  #endif

  #if (VERBOSE_LEVEL >= 2)
    #ifdef DMCP_BUILD
      press_key();
    #endif
  #endif

  #if (VERBOSE_LEVEL >= 1) 
    clearScreen_old(false, true, true);
  #endif

  int16_t ix = 0;
  int16_t iy = 0;
  sprintf(nn,"%2d",selection);                   //Create string value for 00
  if(nn[0]==32) {nn[0]=48;}
  if(nn[1]==32) {nn[1]=48;}
  strcpy(fn_short,"XEQM");                        //Build default short file name XEQMnn
  strcat(fn_short,nn);
  strcpy(fn_long,fn_short);

                                            //Find XEQMnn in the replacement token file         
  while(line1[ix] != 0 && ix+6<stringByteLength(line1)) {
     if(line1[ix]==88 /*X*/ && line1[ix+1]==69 /*E*/ && line1[ix+2]==81 /*Q*/ && line1[ix+3]==77 /*M*/ && line1[ix+4]==nn[0] && line1[ix+5]==nn[1] && line1[ix+6]==58 /*:*/) {
       ix = ix + 7;
       iy = ix;                             //If found, find the replacement text after the colon until before the semi-colon
       while(line1[ix] != 0 && ix<stringByteLength(line1)) {
          if(line1[ix] == 59) {line1[ix]=0; strcpy(fn_long,line1 + iy); break;}     //Replace file name with content from replacement string
          ix++;
       }
     } 
     ix++;
  }
  strcat(fn_short,".TXT");                        //Add .TXT
  strcat(fn_long,".TXT");                         //Add .TXT

  #if (VERBOSE_LEVEL >= 1) 
    sprintf(tmp,"C: Trying %s then %s.",fn_short,fn_long);
    print_linestr(tmp,true);
  #endif

  line1[0]=0;                                     //Clear incoming/outgoing string data

  //printf(">>> original name:|%s|, replacement file name:|%s|\n",fn_short,fn_long);           
  if(selection==1) {
    sprintf(fallback,"XEQLBL 01 HELP ALPHA \"I\" CASE \"n directory \" CASE \"PROGRAMS\" CASEDN \" create \" CASEUP \"XEQM\" CASEDN \"NN\" CASEUP \".TXT\" EXIT ");
  } 
  else {
    sprintf(fallback,"XEQLBL %s X%s ",nn,nn);
  }

  #if (VERBOSE_LEVEL >= 2) 
    sprintf(tmp,"  Fallback:%s",fallback); print_inlinestr(tmp,false);
  #endif

  import_string_from_filename(line1,pgmpath,fn_short,fn_long,fallback); 
  replaceFF(nn,line1); 
  #if (VERBOSE_LEVEL >= 1) 
    print_inlinestr(line1,true);
  #endif

  #if (VERBOSE_LEVEL >= 2) 
    #ifdef DMCP_BUILD
      press_key();
    #endif
  #endif


  #if (VERBOSE_LEVEL >= 1) 
    clearScreen_old(false, true, true);
  #endif

  displaywords(line1);

  #if (VERBOSE_LEVEL >= 2) 
    #ifdef DMCP_BUILD
      press_key();
    #endif
  #endif

  #if (VERBOSE_LEVEL >= 1) 
    clearScreen_old(false, true, true);
  #endif

  execute_string(line1,exec);

  #if (VERBOSE_LEVEL >= 2) 
    #ifdef DMCP_BUILD
      press_key();
      clearScreen_old(false, true, true);
    #endif
  #endif

#endif
}



void fnXEQMENU(uint16_t XEQM_no) {
  #ifndef TESTSUITE_BUILD
  clearScreen_old(false, true, true);
  print_linestr("Loading XEQM program file:",true);

  char line[TMP_STR_LENGTH];
  XEQMENU_Selection( XEQM_no, line, true);

  //calcMode = CM_BUG_ON_SCREEN;
  //temporaryInformation = TI_NO_INFO;
  #endif
}



void XEQMENU_loadAllfromdisk(void) {
#ifndef TESTSUITE_BUILD
      clearScreen_old(false, true, true);
      print_inlinestr("Loading XEQM:",true);

      char line[TMP_STR_LENGTH];
      
      char tmp[10];
      uint8_t ix = 1;
      while(ix<=18) {
        sprintf(tmp,"%2d",ix);
        print_inlinestr(tmp,false);
        XEQMENU_Selection( ix, line, false);
        ix++;
      }
    #endif
}



/*
//Fixed test program, dispatching commands
void testprogram_12(uint16_t unusedButMandatoryParameter){

    runkey(ITM_TICKS); //622
    runkey(684);       //X<>Y
    sendkeys("2"); runkey(ITM_EXIT1); //EXIT
    runkey(684);   //X<>Y
    runkey(698);   //Y^X
    sendkeys("1"); runkey(ITM_EXIT1); //EXIT
    runkey(780);   //-
    runkey(589);   sendkeys("00"); //STO 00
    runkey(469);   //PRIME?
    runkey(684);   //X<>Y
    runkey(ITM_TICKS);
    runkey(684);   //X<>Y
    runkey(ITM_SUB);
}


//Fixed test program, dispatching commands from text string
void testprogram_1(uint16_t unusedButMandatoryParameter){
char line1[TMP_STR_LENGTH];
   strcpy(line1,
    "TICKS //RPN Program to demostrate PRIME// "
    "\"2\" EXIT "
    "\"2203\" "
    "Y^X "
    "\"1\" - "
    "PRIME?  "
    "X<>Y "
    "TICKS "
    "X<>Y - "
    "\"10.0\" / "    
    "RETURN "
    "ABCDEFGHIJKLMNOPQ!@#$%^&*()\n"
    );
    displaywords(line1);
    execute_string(line1,true);
}
*/




void fnXEQMSAVE (uint16_t XEQM_no) {                                  //X-REGISTER TO DISK
  char tt[40]; 
  if(getRegisterDataType(REGISTER_X) == dtString) {
    xcopy(tmpString + TMP_STR_LENGTH/2, REGISTER_STRING_DATA(REGISTER_X), stringByteLength(REGISTER_STRING_DATA(REGISTER_X))+1);
    tt[0]=0;

    sprintf(tt,"XEQM%02u.TXT",XEQM_no);
    //printf("^^^^^^^^^^^^^%s\n",tt);

/*
    switch(XEQM_no) {
      case  1:strcpy(tt, "XEQM01.TXT"); break;
      case  2:strcpy(tt, "XEQM02.TXT"); break;
      case  3:strcpy(tt, "XEQM03.TXT"); break;
      case  4:strcpy(tt, "XEQM04.TXT"); break;
      case  5:strcpy(tt, "XEQM05.TXT"); break;
      case  6:strcpy(tt, "XEQM06.TXT"); break;
      case  7:strcpy(tt, "XEQM07.TXT"); break;
      case  8:strcpy(tt, "XEQM08.TXT"); break;
      case  9:strcpy(tt, "XEQM09.TXT"); break;
      case 10:strcpy(tt, "XEQM10.TXT"); break;
      case 11:strcpy(tt, "XEQM11.TXT"); break;
      case 12:strcpy(tt, "XEQM12.TXT"); break;
      case 13:strcpy(tt, "XEQM13.TXT"); break;
      case 14:strcpy(tt, "XEQM14.TXT"); break;
      case 15:strcpy(tt, "XEQM15.TXT"); break;
      case 16:strcpy(tt, "XEQM16.TXT"); break;
      case 17:strcpy(tt, "XEQM17.TXT"); break;
      case 18:strcpy(tt, "XEQM18.TXT"); break;
      default:;
    }
*/

    #ifdef PC_BUILD
    printf(">>> string ready  ## %s:%s\n",tt,tmpString + TMP_STR_LENGTH/2);
    //uint16_t ix = 0;while (ix!=20) {printf("%d:%d=\n",ix,tmpString[ix]);ix++;}
    stringToUtf8(tmpString + TMP_STR_LENGTH/2, (uint8_t *)tmpString);
    printf(">>> string in utf ## %s:%s\n",tt, tmpString);
    //ix = 0;while (ix!=20) {printf("%d:%d=\n",ix,ll[ix]);ix++;}
    #endif

    #ifndef TESTSUITE_BUILD
      stringToUtf8(tmpString + TMP_STR_LENGTH/2, (uint8_t *)tmpString);
      if(tt[0]!=0) export_string_to_filename(tmpString, overwrite, "PROGRAMS", tt);
    #endif
  }
}


void fnXEQMLOAD (uint16_t XEQM_no) {                                  //DISK to X-REGISTER
  #ifdef PC_BUILD
    printf("LOAD %d\n",XEQM_no);
  #endif
  char line1[TMP_STR_LENGTH];
  line1[0]=0;
  XEQMENU_Selection(XEQM_no, line1, false);
  uint16_t ix = 0;while (ix!=20) {
    #ifdef PC_BUILD
      printf("%d ",line1[ix]);
    #endif
    ix++;
  }
  //printf(">>> loaded: utf:%s\n",line1);  
  utf8ToString((uint8_t *)line1,line1 + TMP_STR_LENGTH/2);
  //ix = 0;while (ix!=20) {printf("%d ",line1[ix]);ix++;}  printf(">>> loaded: str:%s\n",line1 + TMP_STR_LENGTH/2);  
  int16_t len = stringByteLength(line1 + TMP_STR_LENGTH/2);
  liftStack();
  reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), amNone);
  strcpy(REGISTER_STRING_DATA(REGISTER_X),line1 + TMP_STR_LENGTH/2);

}




void fnXEQMEDIT (uint16_t unusedButMandatoryParameter) {

          if(aimBuffer[0] != 0) {          //JM if something already in the AIMB|UFFER when X-EDIT is called, store this in the stack first
            setSystemFlag(FLAG_ASLIFT);
            liftStack();
            copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
            copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_Y);

            int16_t len = stringByteLength(aimBuffer) + 1;
            reallocateRegister(REGISTER_Z, dtString, TO_BLOCKS(len), amNone);
            xcopy(REGISTER_STRING_DATA(REGISTER_Z), aimBuffer, len);
            aimBuffer[0] = 0;

            setSystemFlag(FLAG_ASLIFT);
          }


  if(calcMode == CM_AIM && getRegisterDataType(REGISTER_Y) == dtString) {
    //printf(">>> !@# stringByteLength(REGISTER_STRING_DATA(REGISTER_Y))=%d; AIM_BUFFER_LENGTH=%d\n",stringByteLength(REGISTER_STRING_DATA(REGISTER_Y)),AIM_BUFFER_LENGTH);
    if(stringByteLength(REGISTER_STRING_DATA(REGISTER_Y)) < AIM_BUFFER_LENGTH) {
      if(eRPN) {      //JM NEWERPN 
        setSystemFlag(FLAG_ASLIFT);            //JM NEWERPN OVERRIDE SLS, AS ERPN ENTER ALWAYS HAS SLS SET
      }                                        //JM NEWERPN 
      strcpy(aimBuffer, REGISTER_STRING_DATA(REGISTER_Y)); 
      T_cursorPos = stringByteLength(aimBuffer);
      fnDrop(0);
/*      refreshRegisterLine(REGISTER_T);
      refreshRegisterLine(REGISTER_Z);
      refreshRegisterLine(REGISTER_Y);
*/      refreshRegisterLine(REGISTER_X);        //JM Execute here, to make sure that the 5/2 line check is done
      last_CM=253;

    }
  }
  else
  if(calcMode == CM_AIM && (getRegisterDataType(REGISTER_Y) == dtReal34 || getRegisterDataType(REGISTER_Y) == dtComplex34  || getRegisterDataType(REGISTER_Y) == dtLongInteger   || getRegisterDataType(REGISTER_Y) == dtShortInteger    || getRegisterDataType(REGISTER_Y) == dtTime   || getRegisterDataType(REGISTER_Y) == dtDate)) {
    if(stringByteLength(REGISTER_STRING_DATA(REGISTER_Y)) < AIM_BUFFER_LENGTH) {

      if(eRPN) {      //JM NEWERPN 
        setSystemFlag(FLAG_ASLIFT);            //JM NEWERPN OVERRIDE SLS, AS ERPN ENTER ALWAYS HAS SLS SET
      }                                        //JM NEWERPN 

      char tmp[2];
      tmp[0]=0;
      int16_t len = stringByteLength(tmp) + 1;

      reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), amNone);           //Make blank string in X
      xcopy(REGISTER_STRING_DATA(REGISTER_X), tmp, len);
      addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();//Convert Y number to string in X REGISTER
      adjustResult(REGISTER_X, false, false, -1, -1, -1);                          //Copy X string to Aimbuffer
      strcpy(aimBuffer, REGISTER_STRING_DATA(REGISTER_X)); 

      T_cursorPos = stringByteLength(aimBuffer);
      fnDrop(0);
      refreshRegisterLine(REGISTER_X);        //JM Execute here, to make sure that the 5/2 line check is done
      last_CM=253;

    }
  }


  else if (calcMode == CM_NORMAL && getRegisterDataType(REGISTER_X) == dtString) {
    if(stringByteLength(REGISTER_STRING_DATA(REGISTER_X)) < AIM_BUFFER_LENGTH) {
      if(eRPN) {      //JM NEWERPN 
        setSystemFlag(FLAG_ASLIFT);            //JM NEWERPN OVERRIDE SLS, AS ERPN ENTER ALWAYS HAS SLS SET
      }                                        //JM NEWERPN 
      strcpy(aimBuffer, REGISTER_STRING_DATA(REGISTER_X));
      T_cursorPos = stringByteLength(aimBuffer);
      fnDrop(0);
      #ifndef TESTSUITE_BUILD
        resetShiftState();
        calcModeAim(NOPARAM); // Alpha Input Mode
        showSoftmenu(-MNU_T_EDIT);
      #endif
    } 
//    else {
//    	printf(">>> !@# stringByteLength(REGISTER_STRING_DATA(REGISTER_Y))=%d; AIM_BUFFER_LENGTH=%d\n",stringByteLength(REGISTER_STRING_DATA(REGISTER_Y)),AIM_BUFFER_LENGTH);
//    }
  }
  else if (calcMode == CM_NORMAL && getRegisterDataType(REGISTER_X) != dtString) {
    char line1[TMP_STR_LENGTH];
    line1[0]=0;
    strcpy(line1," ");
    int16_t len = stringByteLength(line1);
    if(eRPN) {      //JM NEWERPN 
      setSystemFlag(FLAG_ASLIFT);            //JM NEWERPN OVERRIDE SLS, AS ERPN ENTER ALWAYS HAS SLS SET
    }                                        //JM NEWERPN 
    liftStack();
    reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), amNone);
    strcpy(REGISTER_STRING_DATA(REGISTER_X),line1);
    fnXEQMEDIT(0);
  }
  last_CM=252;
  refreshScreen();
  last_CM=251;
  refreshScreen();
}


void fnXEQMXXEQ (uint16_t unusedButMandatoryParameter) {
  char line1[3000];
  if(getRegisterDataType(REGISTER_X) == dtString) {
    xcopy(line1, REGISTER_STRING_DATA(REGISTER_X), stringByteLength(REGISTER_STRING_DATA(REGISTER_X)) + 1);
    fnDrop(0);
    displaywords(line1); 
    execute_string(line1,false); 
    execute_string(line1,true);
  }

}



void fnXEQNEW (uint16_t unusedButMandatoryParameter) {
  fnStrtoX("XEQC43 XEQLBL 01 XXXXXX ");
  fnXEQMEDIT(0);
}


void fnCla(uint16_t unusedButMandatoryParameter){
  //Not using calcModeAim becose some modes are reset which should not be
  aimBuffer[0]=0;
  T_cursorPos = 0;
  nextChar = NC_NORMAL;
  xCursor = 1;
  yCursor = Y_POSITION_OF_AIM_LINE + 6;
  cursorFont = &standardFont;
  cursorEnabled = true;
  last_CM=252;
  #ifndef TESTSUITE_BUILD
    clearRegisterLine(AIM_REGISTER_LINE, true, true);
    refreshRegisterLine(AIM_REGISTER_LINE);        //JM Execute here, to make sure that the 5/2 line check is done
  #endif
  last_CM=253;
}


void fnCln(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
    calcModeNim(0);
    last_CM=252;
    refreshRegisterLine(REGISTER_X);        //JM Execute here, to make sure that the 5/2 line check is done
    last_CM=253;
    addItemToNimBuffer(ITM_0);
  #endif
}



