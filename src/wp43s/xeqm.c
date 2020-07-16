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


#include "wp43s.h"


void capture_sequence(char *origin, uint16_t item) {
   char line1[TMP_STR_LENGTH];
   char ll[20];
   uint16_t ix;
#ifdef PC_BUILD
   //printf("Captured: %4d   //%10s//  (%s)\n",item,indexOfItems[item].itemSoftmenuName, origin);
#endif

    ll[0]=0; ll[1]=0;
    switch (item) {
      case  684: strcpy(ll,"X<>Y"); break;
      case  698: strcpy(ll,"Y^X" ); break;
      case  784: strcpy(ll,"/"   ); break;
      case  890: ll[0]=48; strcpy(line1," \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  891: ll[0]=49; strcpy(line1," \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  892: ll[0]=50; strcpy(line1," \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  893: ll[0]=51; strcpy(line1," \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  894: ll[0]=52; strcpy(line1," \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  895: ll[0]=53; strcpy(line1," \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  896: ll[0]=54; strcpy(line1," \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  897: ll[0]=55; strcpy(line1," \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  898: ll[0]=56; strcpy(line1," \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  899: ll[0]=57; strcpy(line1," \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case 1310: ll[0]=46; strcpy(line1," \""); strcat(line1,ll); strcat(line1,"\" "); break; //.
      case 1487: ll[0]=69; strcpy(line1," \""); strcat(line1,ll); strcat(line1,"\" "); break; //E
      default: { strcpy(ll,indexOfItems[item].itemSoftmenuName);
               }  
    }
    ix = 0;
    while (ll[ix] != 0) {
      if (( (ll[ix] & 128) == 128) || ll[ix] < 32) {ll[ix] = 35;}
      ix++;
    }
    sprintf(line1, " %4d //%10s//\n",item,ll);

    #ifndef TESTSUITE_BUILD
    export_string_to_file(line1);
    #endif
}


//############################ SEND KEY TO 43S ENGINE ####################
void runkey(uint16_t item){
  #ifndef TESTSUITE_BUILD
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
  #endif
}

//############################ DECODE NUMBERS AND THEN SEND KEY TO 43S ENGINE ####################
void sendkeys(const char aa[]) {
  int16_t ix = 0;
  while (aa[ix]!=0) {
    if(aa[ix]>=65 && aa[ix]<=90){runkey(900+aa[ix]-65);} else //A..Z
    if(aa[ix]>=48 && aa[ix]<=57){runkey(890+aa[ix]-48);} else //0..9
    switch (aa[ix]) {
      case 46: runkey(1310); break; //.
      case 69: runkey(1487); break; //E
      case 101: runkey(1487); break; //e
      case 45: runkey(780); break; //-
      case 43: runkey(778); break; //+
      case 32: runkey(1295); break; //space
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
uint16_t indic_x = 0;
uint16_t indic_y = SCREEN_HEIGHT-1;


void execute_string(const char *inputstring, bool_t exec1) {
#ifndef TESTSUITE_BUILD
      uint16_t ix, ix_m;
      uint16_t ix_m1 = 0;
      uint16_t ix_m2 = 0;
      uint16_t ix_m3 = 0;
      uint16_t ix_m4 = 0;
      uint16_t no;
      char     commandnumber[20];
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


    while(!gotlabels || (gotlabels && exec) ){   //scheme to use for label scouting and name processing in "false", and to do a two parse exec 
      //printf("Indexes: M1:%d M2:%d M3:%d M4:%d   EXEC:%d\n",ix_m1, ix_m2, ix_m3, ix_m4, exec);
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
      while (inputstring[ix]!=0) {
        strcpy(bb,aa);
        aa[0] = inputstring[ix];
        aa[1] = 0;

        switch(bb[0]) {//COMMAND can start after any of these: space, tab, cr, lf, comma, beginning of file
          case 32:
          case 8 :
          case 13:
          case 10:
          case 44:
          case 0 :  if( //COMMAND WORD START DETECTION +-*/ 0-9; A-Z 
                    (      (aa[0]==42 // *
                        ||  aa[0]==43 // +
                        ||  aa[0]==45 // -
                        ||  aa[0]==47 // /
                        ||  aa[0]==126) //~
                        || (aa[0]>=48 && aa[0]<=57) //0-9
                        || (aa[0]>=65 && aa[0]<=90)  ) //A-Z
                    && !state_comments                 //If not inside comments
                    && !state_quotes                   //if not inside quotes
                    && !state_commands                 //Don't re-check until done
                    ) {
                        state_commands = true;         // Waiting to open command number or name: nnn
                      }
          default:;
        }
        
        if(state_comments && (aa[0] == 13 || aa[0] == 10)) {state_comments=!state_comments;} else
        switch(aa[0]) {
          case 47: if(bb[0] == 47 && state_comments == false) {//ADDED  STATE, SO //always switches on the comment, but not off. CR/LF cancels it
                      state_comments = !state_comments;        // Toggle comment state
                      state_commands = false;
                      state_quotes   = false;
                      commandnumber[0]=0;
                    }
                     break;
          case 34: if(!state_comments && !state_commands) {   // Toggle quote state
                       state_quotes   = !state_quotes;
                     }
                     break;

          case 13: //cr
          case 10: //lf
          case 8 : //tab
          case 44: //,
          case 32: if(state_commands){
                      state_commands = false;                // Waiting for delimiter to close off and send command number: nnn<                 
                      //printf("&%s&",commandnumber);
                      
                      //DSZ:
                      if(!(gotoinprogress != 11 || (gotoinprogress == 11 && (temporaryInformation == TI_FALSE)))) {     //If DEC results in 0, then 'true'.    It is now the command that may or may not be skipped
                          //......IS NOT DSZ.... OR               DSZ    with REG NOT ZERO
                          go = (temporaryInformation == TI_FALSE); //As per GTO_SZ ---- REGISTER<>0, then go
                          gotoinprogress = 1;                      //As per GTO_SZ
                          commandnumber[0]=0;                      //As per GTO_SZ
                      } else // (gotoinprogress <> 11)  AND NOT (==11 AND reg>0)

// FAST TRACKED FOR QUICKER RESPONSE DUPLICATED ************************************************************************
                      
                      if (strcompare(commandnumber,"TICKS" )) {strcpy(commandnumber, "622");} else
                      if (strcompare(commandnumber,"X<>Y"  )) {strcpy(commandnumber, "684");} else
                      if (strcompare(commandnumber,"EXIT"  )) {strcpy(commandnumber,"1523");} else
                      if (strcompare(commandnumber,"ENTER" )) {strcpy(commandnumber, "148");} else
                      if (strcompare(commandnumber,"DROP"  )) {strcpy(commandnumber, "127");} else
                      if (strcompare(commandnumber,"Y^X"   )) {strcpy(commandnumber, "698");} else
                      if (strcompare(commandnumber,"10^X"  )) {strcpy(commandnumber,   "3");} else
                      if (strcompare(commandnumber,"SQRT"  )) {strcpy(commandnumber, "808");} else
                      if (strcompare(commandnumber,"-"     )) {strcpy(commandnumber, "780");} else
                      if (strcompare(commandnumber,"+"     )) {strcpy(commandnumber, "778");} else
                      if (strcompare(commandnumber,"/"     )) {strcpy(commandnumber, "784");} else
                      if (strcompare(commandnumber,"*"     )) {strcpy(commandnumber, "782");} else
                      if (strcompare(commandnumber,"CHS"   )) {strcpy(commandnumber, "779");} else
                      if (strcompare(commandnumber,"SUM+"  )) {strcpy(commandnumber, "762");} else
                      if (strcompare(commandnumber,"CLSUM" )) {strcpy(commandnumber,  "85");} else
                      if (strcompare(commandnumber,"PRIME?")) {strcpy(commandnumber, "469");} else
                      if (strcompare(commandnumber,"NEXTP" )) {strcpy(commandnumber, "422");} else
                      if (strcompare(commandnumber,"RAN#"  )) {strcpy(commandnumber, "486");} else
                      if (strcompare(commandnumber,"PHI"   )) {strcpy(commandnumber, "764");} else
                      if (strcompare(commandnumber,"e"     )) {strcpy(commandnumber, "139");} else
                      if (strcompare(commandnumber,"ABS"   )) {strcpy(commandnumber, "815");} else
                      if (strcompare(commandnumber,"ARG"   )) {strcpy(commandnumber, "819");} else
                      if (strcompare(commandnumber,"RE"    )) {strcpy(commandnumber, "503");} else
                      if (strcompare(commandnumber,"IM"    )) {strcpy(commandnumber, "250");} else
                      if (strcompare(commandnumber,"STO"   )) {strcpy(commandnumber, "589");} else    //EXPECTING FOLLOWING OPERAND "nn". NOT CHECKING "nn", just sending it if in ""
                      if (strcompare(commandnumber,"RCL"   )) {strcpy(commandnumber, "488");} else    //EXPECTING FOLLOWING OPERAND "nn". NOT CHECKING "nn", just sending it if in ""
//                    if (strcompare(commandnumber,"DEC"   )) {strcpy(commandnumber, "115");} else    //EXPECTING FOLLOWING OPERAND "nn". NOT CHECKING "nn", just sending it if in ""
//                    if (strcompare(commandnumber,"INC"   )) {strcpy(commandnumber, "252");} else    //EXPECTING FOLLOWING OPERAND "nn". NOT CHECKING "nn", just sending it if in ""

// FROM SPREADSHEET vvv ****************************************************************************************************
                      if (strcompare(commandnumber,"10^X" )) {strcpy(commandnumber, "3");} else
                      if (strcompare(commandnumber,"SNAP" )) {strcpy(commandnumber, "5");} else
                      if (strcompare(commandnumber,"1/X" )) {strcpy(commandnumber, "6");} else
                      if (strcompare(commandnumber,"2^X" )) {strcpy(commandnumber, "8");} else
                      if (strcompare(commandnumber,"CUBERT" )) {strcpy(commandnumber, "9");} else
                      if (strcompare(commandnumber,"AGM" )) {strcpy(commandnumber, "18");} else
                      if (strcompare(commandnumber,"AND" )) {strcpy(commandnumber, "22");} else
                      if (strcompare(commandnumber,"ARCCOS" )) {strcpy(commandnumber, "24");} else
                      if (strcompare(commandnumber,"ARCOSH" )) {strcpy(commandnumber, "25");} else
                      if (strcompare(commandnumber,"ARCSIN" )) {strcpy(commandnumber, "26");} else
                      if (strcompare(commandnumber,"ARCTAN" )) {strcpy(commandnumber, "27");} else
                      if (strcompare(commandnumber,"ARSINH" )) {strcpy(commandnumber, "28");} else
                      if (strcompare(commandnumber,"ARTANH" )) {strcpy(commandnumber, "29");} else
                      if (strcompare(commandnumber,"ASR" )) {strcpy(commandnumber, "30");} else
                      if (strcompare(commandnumber,"BC?" )) {strcpy(commandnumber, "41");} else
                      if (strcompare(commandnumber,"BS?" )) {strcpy(commandnumber, "53");} else
                      if (strcompare(commandnumber,"c" )) {strcpy(commandnumber, "56");} else
                      if (strcompare(commandnumber,"CB" )) {strcpy(commandnumber, "67");} else
                      if (strcompare(commandnumber,"CEIL" )) {strcpy(commandnumber, "68");} else
                      if (strcompare(commandnumber,"CLFALL" )) {strcpy(commandnumber, "73");} else
                      if (strcompare(commandnumber,"CLLCD" )) {strcpy(commandnumber, "77");} else
                      if (strcompare(commandnumber,"CLMENU" )) {strcpy(commandnumber, "78");} else
                      if (strcompare(commandnumber,"CLREGS" )) {strcpy(commandnumber, "82");} else
                      if (strcompare(commandnumber,"CLSTK" )) {strcpy(commandnumber, "83");} else
                      if (strcompare(commandnumber,"CLX" )) {strcpy(commandnumber, "84");} else
                      if (strcompare(commandnumber,"CLSUM" )) {strcpy(commandnumber, "85");} else
                      if (strcompare(commandnumber,"COMB" )) {strcpy(commandnumber, "87");} else
                      if (strcompare(commandnumber,"CONJ" )) {strcpy(commandnumber, "88");} else
                      if (strcompare(commandnumber,"CNST" )) {strcpy(commandnumber, "89");} else
                      if (strcompare(commandnumber,"COS" )) {strcpy(commandnumber, "92");} else
                      if (strcompare(commandnumber,"COSH" )) {strcpy(commandnumber, "93");} else
                      if (strcompare(commandnumber,"CROSS" )) {strcpy(commandnumber, "101");} else
                      if (strcompare(commandnumber,"CX>RE" )) {strcpy(commandnumber, "103");} else
                      if (strcompare(commandnumber,"DEC" )) {strcpy(commandnumber, "115");} else
                      if (strcompare(commandnumber,"DECOMP" )) {strcpy(commandnumber, "116");} else
                      if (strcompare(commandnumber,"DEG" )) {strcpy(commandnumber, "117");} else
                      if (strcompare(commandnumber,"DEG>" )) {strcpy(commandnumber, "118");} else
                      if (strcompare(commandnumber,"DOT" )) {strcpy(commandnumber, "125");} else
                      if (strcompare(commandnumber,"DROP" )) {strcpy(commandnumber, "127");} else
                      if (strcompare(commandnumber,"DROPY" )) {strcpy(commandnumber, "128");} else
                      if (strcompare(commandnumber,"D.MS" )) {strcpy(commandnumber, "134");} else
                      if (strcompare(commandnumber,"D.MS>" )) {strcpy(commandnumber, "135");} else
                      if (strcompare(commandnumber,"D>R" )) {strcpy(commandnumber, "138");} else
                      if (strcompare(commandnumber,"e" )) {strcpy(commandnumber, "139");} else
                      if (strcompare(commandnumber,"ENG" )) {strcpy(commandnumber, "145");} else
                      if (strcompare(commandnumber,"ENTER" )) {strcpy(commandnumber, "148");} else
                      if (strcompare(commandnumber,"E^X" )) {strcpy(commandnumber, "158");} else
                      if (strcompare(commandnumber,"EXPT" )) {strcpy(commandnumber, "167");} else
                      if (strcompare(commandnumber,"E^X-1" )) {strcpy(commandnumber, "168");} else
                      if (strcompare(commandnumber,"FB" )) {strcpy(commandnumber, "173");} else
                      if (strcompare(commandnumber,"FF" )) {strcpy(commandnumber, "180");} else
                      if (strcompare(commandnumber,"FILL" )) {strcpy(commandnumber, "182");} else
                      if (strcompare(commandnumber,"FIX" )) {strcpy(commandnumber, "185");} else
                      if (strcompare(commandnumber,"FLASH?" )) {strcpy(commandnumber, "188");} else
                      if (strcompare(commandnumber,"FLOOR" )) {strcpy(commandnumber, "189");} else
                      if (strcompare(commandnumber,"FP" )) {strcpy(commandnumber, "190");} else
                      if (strcompare(commandnumber,"FR>DB" )) {strcpy(commandnumber, "196");} else
                      if (strcompare(commandnumber,"GCD" )) {strcpy(commandnumber, "217");} else
                      if (strcompare(commandnumber,"ge" )) {strcpy(commandnumber, "220");} else
                      if (strcompare(commandnumber,"GRAD>" )) {strcpy(commandnumber, "230");} else
                      if (strcompare(commandnumber,"gEARTH" )) {strcpy(commandnumber, "233");} else
                      if (strcompare(commandnumber,"IDIV" )) {strcpy(commandnumber, "247");} else
                      if (strcompare(commandnumber,"IM" )) {strcpy(commandnumber, "250");} else
                      if (strcompare(commandnumber,"INC" )) {strcpy(commandnumber, "252");} else
                      if (strcompare(commandnumber,"IP" )) {strcpy(commandnumber, "259");} else
                      if (strcompare(commandnumber,"LASTX" )) {strcpy(commandnumber, "296");} else
                      if (strcompare(commandnumber,"LCM" )) {strcpy(commandnumber, "301");} else
                      if (strcompare(commandnumber,"LJ" )) {strcpy(commandnumber, "309");} else
                      if (strcompare(commandnumber,"LN" )) {strcpy(commandnumber, "310");} else
                      if (strcompare(commandnumber,"LN(1+X)" )) {strcpy(commandnumber, "312");} else
                      if (strcompare(commandnumber,"LNBETA" )) {strcpy(commandnumber, "314");} else
                      if (strcompare(commandnumber,"LNGAMMA" )) {strcpy(commandnumber, "315");} else
                      if (strcompare(commandnumber,"LOCR?" )) {strcpy(commandnumber, "322");} else
                      if (strcompare(commandnumber,"LOG10" )) {strcpy(commandnumber, "323");} else
                      if (strcompare(commandnumber,"LOG2" )) {strcpy(commandnumber, "324");} else
                      if (strcompare(commandnumber,"LOGXY" )) {strcpy(commandnumber, "331");} else
                      if (strcompare(commandnumber,"MANT" )) {strcpy(commandnumber, "344");} else
                      if (strcompare(commandnumber,"MASKL" )) {strcpy(commandnumber, "345");} else
                      if (strcompare(commandnumber,"MASKR" )) {strcpy(commandnumber, "346");} else
                      if (strcompare(commandnumber,"MAX" )) {strcpy(commandnumber, "353");} else
                      if (strcompare(commandnumber,"MEM?" )) {strcpy(commandnumber, "355");} else
                      if (strcompare(commandnumber,"MIN" )) {strcpy(commandnumber, "358");} else
                      if (strcompare(commandnumber,"MIRROR" )) {strcpy(commandnumber, "359");} else
                      if (strcompare(commandnumber,"MOD" )) {strcpy(commandnumber, "364");} else
                      if (strcompare(commandnumber,"NAND" )) {strcpy(commandnumber, "414");} else
                      if (strcompare(commandnumber,"NEIGHB" )) {strcpy(commandnumber, "421");} else
                      if (strcompare(commandnumber,"NEXTP" )) {strcpy(commandnumber, "422");} else
                      if (strcompare(commandnumber,"NOR" )) {strcpy(commandnumber, "425");} else
                      if (strcompare(commandnumber,"NOT" )) {strcpy(commandnumber, "431");} else
                      if (strcompare(commandnumber,"NSUM" )) {strcpy(commandnumber, "433");} else
                      if (strcompare(commandnumber,"OR" )) {strcpy(commandnumber, "437");} else
                      if (strcompare(commandnumber,"PERM" )) {strcpy(commandnumber, "450");} else
                      if (strcompare(commandnumber,"PLOT" )) {strcpy(commandnumber, "455");} else
                      if (strcompare(commandnumber,"PR>DB" )) {strcpy(commandnumber, "467");} else
                      if (strcompare(commandnumber,"PRIME?" )) {strcpy(commandnumber, "469");} else
                      if (strcompare(commandnumber,"RAD" )) {strcpy(commandnumber, "483");} else
                      if (strcompare(commandnumber,"RAD>" )) {strcpy(commandnumber, "484");} else
                      if (strcompare(commandnumber,"RAN#" )) {strcpy(commandnumber, "486");} else
                      if (strcompare(commandnumber,"RCL" )) {strcpy(commandnumber, "488");} else
                      if (strcompare(commandnumber,"RCLEL" )) {strcpy(commandnumber, "490");} else
                      if (strcompare(commandnumber,"RCLIJ" )) {strcpy(commandnumber, "491");} else
                      if (strcompare(commandnumber,"RCLS" )) {strcpy(commandnumber, "492");} else
                      if (strcompare(commandnumber,"RCL+" )) {strcpy(commandnumber, "493");} else
                      if (strcompare(commandnumber,"RCL-" )) {strcpy(commandnumber, "494");} else
                      if (strcompare(commandnumber,"RCLx" )) {strcpy(commandnumber, "495");} else
                      if (strcompare(commandnumber,"RCL/" )) {strcpy(commandnumber, "496");} else
                      if (strcompare(commandnumber,"RCLMAX" )) {strcpy(commandnumber, "497");} else
                      if (strcompare(commandnumber,"RCLMIN" )) {strcpy(commandnumber, "498");} else
                      if (strcompare(commandnumber,"RE" )) {strcpy(commandnumber, "503");} else
                      if (strcompare(commandnumber,"RE>CX" )) {strcpy(commandnumber, "511");} else
                      if (strcompare(commandnumber,"RE<>IM" )) {strcpy(commandnumber, "512");} else
                      if (strcompare(commandnumber,"RJ" )) {strcpy(commandnumber, "513");} else
                      if (strcompare(commandnumber,"RL" )) {strcpy(commandnumber, "515");} else
                      if (strcompare(commandnumber,"RLC" )) {strcpy(commandnumber, "516");} else
                      if (strcompare(commandnumber,"RMODE?" )) {strcpy(commandnumber, "519");} else
                      if (strcompare(commandnumber,"RMD" )) {strcpy(commandnumber, "520");} else
                      if (strcompare(commandnumber,"RR" )) {strcpy(commandnumber, "524");} else
                      if (strcompare(commandnumber,"RRC" )) {strcpy(commandnumber, "525");} else
                      if (strcompare(commandnumber,"R>D" )) {strcpy(commandnumber, "534");} else
                      if (strcompare(commandnumber,"SB" )) {strcpy(commandnumber, "543");} else
                      if (strcompare(commandnumber,"SCI" )) {strcpy(commandnumber, "545");} else
                      if (strcompare(commandnumber,"SDIGS?" )) {strcpy(commandnumber, "548");} else
                      if (strcompare(commandnumber,"SDL" )) {strcpy(commandnumber, "549");} else
                      if (strcompare(commandnumber,"SDR" )) {strcpy(commandnumber, "550");} else
                      if (strcompare(commandnumber,"SEED" )) {strcpy(commandnumber, "552");} else
                      if (strcompare(commandnumber,"SIGN" )) {strcpy(commandnumber, "566");} else
                      if (strcompare(commandnumber,"SIN" )) {strcpy(commandnumber, "569");} else
                      if (strcompare(commandnumber,"SINC" )) {strcpy(commandnumber, "570");} else
                      if (strcompare(commandnumber,"SINH" )) {strcpy(commandnumber, "571");} else
                      if (strcompare(commandnumber,"SL" )) {strcpy(commandnumber, "573");} else
                      if (strcompare(commandnumber,"SLVQ" )) {strcpy(commandnumber, "575");} else
                      if (strcompare(commandnumber,"SMODE?" )) {strcpy(commandnumber, "577");} else
                      if (strcompare(commandnumber,"SR" )) {strcpy(commandnumber, "582");} else
                      if (strcompare(commandnumber,"SSIZE?" )) {strcpy(commandnumber, "585");} else
                      if (strcompare(commandnumber,"STO" )) {strcpy(commandnumber, "589");} else
                      if (strcompare(commandnumber,"STOEL" )) {strcpy(commandnumber, "591");} else
                      if (strcompare(commandnumber,"STOIJ" )) {strcpy(commandnumber, "592");} else
                      if (strcompare(commandnumber,"STOS" )) {strcpy(commandnumber, "594");} else
                      if (strcompare(commandnumber,"ST.A" )) {strcpy(commandnumber, "604");} else
                      if (strcompare(commandnumber,"ST.B" )) {strcpy(commandnumber, "605");} else
                      if (strcompare(commandnumber,"ST.C" )) {strcpy(commandnumber, "606");} else
                      if (strcompare(commandnumber,"ST.D" )) {strcpy(commandnumber, "607");} else
                      if (strcompare(commandnumber,"ST.T" )) {strcpy(commandnumber, "608");} else
                      if (strcompare(commandnumber,"ST.X" )) {strcpy(commandnumber, "609");} else
                      if (strcompare(commandnumber,"ST.Y" )) {strcpy(commandnumber, "610");} else
                      if (strcompare(commandnumber,"ST.Z" )) {strcpy(commandnumber, "611");} else
                      if (strcompare(commandnumber,"SUM" )) {strcpy(commandnumber, "612");} else
                      if (strcompare(commandnumber,"TAN" )) {strcpy(commandnumber, "618");} else
                      if (strcompare(commandnumber,"TANH" )) {strcpy(commandnumber, "619");} else
                      if (strcompare(commandnumber,"TICKS" )) {strcpy(commandnumber, "622");} else
                      if (strcompare(commandnumber,"T<>" )) {strcpy(commandnumber, "641");} else
                      if (strcompare(commandnumber,"ULP?" )) {strcpy(commandnumber, "642");} else
                      if (strcompare(commandnumber,"UNITV" )) {strcpy(commandnumber, "644");} else
                      if (strcompare(commandnumber,"WSIZE" )) {strcpy(commandnumber, "664");} else
                      if (strcompare(commandnumber,"WSIZE?" )) {strcpy(commandnumber, "665");} else
                      if (strcompare(commandnumber,"X^2" )) {strcpy(commandnumber, "669");} else
                      if (strcompare(commandnumber,"X^3" )) {strcpy(commandnumber, "670");} else
                      if (strcompare(commandnumber,"XNOR" )) {strcpy(commandnumber, "672");} else
                      if (strcompare(commandnumber,"XOR" )) {strcpy(commandnumber, "673");} else
                      if (strcompare(commandnumber,"X_MEAN" )) {strcpy(commandnumber, "674");} else
                      if (strcompare(commandnumber,"X_GEO" )) {strcpy(commandnumber, "675");} else
                      if (strcompare(commandnumber,"X_WEIGHTD" )) {strcpy(commandnumber, "676");} else
                      if (strcompare(commandnumber,"X!" )) {strcpy(commandnumber, "679");} else
                      if (strcompare(commandnumber,"X>ALPHA" )) {strcpy(commandnumber, "682");} else
                      if (strcompare(commandnumber,"X<>" )) {strcpy(commandnumber, "683");} else
                      if (strcompare(commandnumber,"X<>Y" )) {strcpy(commandnumber, "684");} else
                      if (strcompare(commandnumber,"XRTY" )) {strcpy(commandnumber, "694");} else
                      if (strcompare(commandnumber,"Y^X" )) {strcpy(commandnumber, "698");} else
                      if (strcompare(commandnumber,"Y<>" )) {strcpy(commandnumber, "701");} else
                      if (strcompare(commandnumber,"Z<>" )) {strcpy(commandnumber, "703");} else
                      if (strcompare(commandnumber,"XMAX" )) {strcpy(commandnumber, "708");} else
                      if (strcompare(commandnumber,"XMIN" )) {strcpy(commandnumber, "709");} else
                      if (strcompare(commandnumber,"GAMMA(X)" )) {strcpy(commandnumber, "724");} else
                      if (strcompare(commandnumber,"DELTA%" )) {strcpy(commandnumber, "726");} else
                      if (strcompare(commandnumber,"mu0" )) {strcpy(commandnumber, "735");} else
                      if (strcompare(commandnumber,"PI" )) {strcpy(commandnumber, "744");} else
                      if (strcompare(commandnumber,"SUMLN^2X" )) {strcpy(commandnumber, "748");} else
                      if (strcompare(commandnumber,"SUMLN^2Y" )) {strcpy(commandnumber, "749");} else
                      if (strcompare(commandnumber,"SUMLNX" )) {strcpy(commandnumber, "750");} else
                      if (strcompare(commandnumber,"SUMLNXY" )) {strcpy(commandnumber, "751");} else
                      if (strcompare(commandnumber,"SUMLNY" )) {strcpy(commandnumber, "752");} else
                      if (strcompare(commandnumber,"SUMX" )) {strcpy(commandnumber, "754");} else
                      if (strcompare(commandnumber,"SUMX^2" )) {strcpy(commandnumber, "755");} else
                      if (strcompare(commandnumber,"SUMX^2Y" )) {strcpy(commandnumber, "756");} else
                      if (strcompare(commandnumber,"SUMXLNY" )) {strcpy(commandnumber, "757");} else
                      if (strcompare(commandnumber,"SUMXY" )) {strcpy(commandnumber, "758");} else
                      if (strcompare(commandnumber,"SUMY" )) {strcpy(commandnumber, "759");} else
                      if (strcompare(commandnumber,"SUMY^2" )) {strcpy(commandnumber, "760");} else
                      if (strcompare(commandnumber,"SUMYLNX" )) {strcpy(commandnumber, "761");} else
                      if (strcompare(commandnumber,"SUM+" )) {strcpy(commandnumber, "762");} else
                      if (strcompare(commandnumber,"PHI" )) {strcpy(commandnumber, "764");} else
                      if (strcompare(commandnumber,"RANI#" )) {strcpy(commandnumber, "766");} else
                      if (strcompare(commandnumber,"RANGE" )) {strcpy(commandnumber, "769");} else
                      if (strcompare(commandnumber,"RANGE?" )) {strcpy(commandnumber, "770");} else
                      if (strcompare(commandnumber,"(-1)^X" )) {strcpy(commandnumber, "777");} else
                      if (strcompare(commandnumber,"+" )) {strcpy(commandnumber, "778");} else
                      if (strcompare(commandnumber,"CHS" )) {strcpy(commandnumber, "779");} else
                      if (strcompare(commandnumber,"-" )) {strcpy(commandnumber, "780");} else
                      if (strcompare(commandnumber,"-INFINITY" )) {strcpy(commandnumber, "781");} else
                      if (strcompare(commandnumber,"*" )) {strcpy(commandnumber, "782");} else
                      if (strcompare(commandnumber,"/" )) {strcpy(commandnumber, "784");} else
                      if (strcompare(commandnumber,">DEG" )) {strcpy(commandnumber, "788");} else
                      if (strcompare(commandnumber,">D.MS" )) {strcpy(commandnumber, "789");} else
                      if (strcompare(commandnumber,">GRAD" )) {strcpy(commandnumber, "790");} else
                      if (strcompare(commandnumber,">HR" )) {strcpy(commandnumber, "791");} else
                      if (strcompare(commandnumber,">H.MS" )) {strcpy(commandnumber, "792");} else
                      if (strcompare(commandnumber,">INT" )) {strcpy(commandnumber, "793");} else
                      if (strcompare(commandnumber,">MULPI" )) {strcpy(commandnumber, "794");} else
                      if (strcompare(commandnumber,">RAD" )) {strcpy(commandnumber, "796");} else
                      if (strcompare(commandnumber,">REAL" )) {strcpy(commandnumber, "797");} else
                      if (strcompare(commandnumber,"D>D.MS" )) {strcpy(commandnumber, "799");} else
                      if (strcompare(commandnumber,"<>" )) {strcpy(commandnumber, "802");} else
                      if (strcompare(commandnumber,"%" )) {strcpy(commandnumber, "803");} else
                      if (strcompare(commandnumber,"%MRR" )) {strcpy(commandnumber, "804");} else
                      if (strcompare(commandnumber,"%T" )) {strcpy(commandnumber, "805");} else
                      if (strcompare(commandnumber,"%SUM" )) {strcpy(commandnumber, "806");} else
                      if (strcompare(commandnumber,"%+MG" )) {strcpy(commandnumber, "807");} else
                      if (strcompare(commandnumber,"SQRT" )) {strcpy(commandnumber, "808");} else
                      if (strcompare(commandnumber,"INFINITY" )) {strcpy(commandnumber, "812");} else
                      if (strcompare(commandnumber,"ABS" )) {strcpy(commandnumber, "815");} else
                      if (strcompare(commandnumber,"PARL" )) {strcpy(commandnumber, "816");} else
                      if (strcompare(commandnumber,"ARG" )) {strcpy(commandnumber, "819");} else
                      if (strcompare(commandnumber,"MULPI>" )) {strcpy(commandnumber, "820");} else
                      if (strcompare(commandnumber,"#B" )) {strcpy(commandnumber, "837");} else
                      if (strcompare(commandnumber,"REG_A" )) {strcpy(commandnumber, "882");} else
                      if (strcompare(commandnumber,"REG_B" )) {strcpy(commandnumber, "883");} else
                      if (strcompare(commandnumber,"REG_C" )) {strcpy(commandnumber, "884");} else
                      if (strcompare(commandnumber,"REG_D" )) {strcpy(commandnumber, "885");} else
                      if (strcompare(commandnumber,"REG_L" )) {strcpy(commandnumber, "886");} else
                      if (strcompare(commandnumber,"REG_I" )) {strcpy(commandnumber, "887");} else
                      if (strcompare(commandnumber,"REG_J" )) {strcpy(commandnumber, "888");} else
                      if (strcompare(commandnumber,"REG_K" )) {strcpy(commandnumber, "889");} else
                      if (strcompare(commandnumber,"CC" )) {strcpy(commandnumber, "1516");} else
                      if (strcompare(commandnumber,"EXIT" )) {strcpy(commandnumber, "1523");} else
                      if (strcompare(commandnumber,"ALPHA" )) {strcpy(commandnumber, "1526");} else
                      if (strcompare(commandnumber,".D" )) {strcpy(commandnumber, "1527");} else
                      if (strcompare(commandnumber,"D.MS>D" )) {strcpy(commandnumber, "1533");} else
                      if (strcompare(commandnumber,"X_HARM" )) {strcpy(commandnumber, "1559");} else
                      if (strcompare(commandnumber,"X_RMS" )) {strcpy(commandnumber, "1560");} else
                      if (strcompare(commandnumber,"SUMLNY/X" )) {strcpy(commandnumber, "1567");} else
                      if (strcompare(commandnumber,"SUMX^2/Y" )) {strcpy(commandnumber, "1568");} else
                      if (strcompare(commandnumber,"SUM^1/X" )) {strcpy(commandnumber, "1569");} else
                      if (strcompare(commandnumber,"SUM^1/X^2" )) {strcpy(commandnumber, "1570");} else
                      if (strcompare(commandnumber,"SUMX/Y" )) {strcpy(commandnumber, "1571");} else
                      if (strcompare(commandnumber,"SUM^1/Y" )) {strcpy(commandnumber, "1572");} else
                      if (strcompare(commandnumber,"SUM^1/Y^2" )) {strcpy(commandnumber, "1573");} else
                      if (strcompare(commandnumber,"SUMX^3" )) {strcpy(commandnumber, "1574");} else
                      if (strcompare(commandnumber,"SUMX^4" )) {strcpy(commandnumber, "1575");} else
                      if (strcompare(commandnumber,"IDIVR" )) {strcpy(commandnumber, "1577");} else
                      if (strcompare(commandnumber,"DET" )) {strcpy(commandnumber, "1581");} else
                      if (strcompare(commandnumber,"INVRT" )) {strcpy(commandnumber, "1582");} else
                      if (strcompare(commandnumber,"TRANS" )) {strcpy(commandnumber, "1583");} else
                      if (strcompare(commandnumber,"ERPN" )) {strcpy(commandnumber, "1678");} else
                      if (strcompare(commandnumber,"SIG" )) {strcpy(commandnumber, "1682");} else
                      if (strcompare(commandnumber,"UNIT" )) {strcpy(commandnumber, "1693");} else
                      if (strcompare(commandnumber,"ERPN?" )) {strcpy(commandnumber, "1694");} else
                      if (strcompare(commandnumber,"CASE" )) {strcpy(commandnumber, "1736");} else
                      if (strcompare(commandnumber,"##>INT" )) {strcpy(commandnumber, "1737");} else
                      if (strcompare(commandnumber,"OP_A" )) {strcpy(commandnumber, "1739");} else
                      if (strcompare(commandnumber,"OP_A^2" )) {strcpy(commandnumber, "1740");} else
                      if (strcompare(commandnumber,"OP_J" )) {strcpy(commandnumber, "1741");} else
                      if (strcompare(commandnumber,"D>Y" )) {strcpy(commandnumber, "1750");} else
                      if (strcompare(commandnumber,"Y>D" )) {strcpy(commandnumber, "1751");} else
                      if (strcompare(commandnumber,"ATOSYM" )) {strcpy(commandnumber, "1752");} else
                      if (strcompare(commandnumber,"SYMTOA" )) {strcpy(commandnumber, "1753");} else
                      if (strcompare(commandnumber,"E^THETAJ" )) {strcpy(commandnumber, "1755");} else
                      if (strcompare(commandnumber,"STO3Z" )) {strcpy(commandnumber, "1756");} else
                      if (strcompare(commandnumber,"RCL3Z" )) {strcpy(commandnumber, "1757");} else
                      if (strcompare(commandnumber,"STO3V" )) {strcpy(commandnumber, "1758");} else
                      if (strcompare(commandnumber,"RCL3V" )) {strcpy(commandnumber, "1759");} else
                      if (strcompare(commandnumber,"STO3I" )) {strcpy(commandnumber, "1760");} else
                      if (strcompare(commandnumber,"RCL3I" )) {strcpy(commandnumber, "1761");} else
                      if (strcompare(commandnumber,"3V/3I" )) {strcpy(commandnumber, "1762");} else
                      if (strcompare(commandnumber,"3Ix3Z" )) {strcpy(commandnumber, "1763");} else
                      if (strcompare(commandnumber,"3V/3Z" )) {strcpy(commandnumber, "1764");} else
                      if (strcompare(commandnumber,"X>BAL" )) {strcpy(commandnumber, "1765");} else
                      if (strcompare(commandnumber,"COMPLEX" )) {strcpy(commandnumber, "1766");} else
                      if (strcompare(commandnumber,"CONVUP" )) {strcpy(commandnumber, "1768");} else
                      if (strcompare(commandnumber,"CONVDN" )) {strcpy(commandnumber, "1769");} else
                      if (strcompare(commandnumber,">LI<>SI" )) {strcpy(commandnumber, "1925");} else
                      if (strcompare(commandnumber,".MS" )) {strcpy(commandnumber, "1926");} else
                      if (strcompare(commandnumber,">POLAR" )) {strcpy(commandnumber, "1955");} else
                      if (strcompare(commandnumber,">RECT" )) {strcpy(commandnumber, "1956");} else
                      if (strcompare(commandnumber,"CPXI" )) {strcpy(commandnumber, "1960");} else
                      if (strcompare(commandnumber,"CPXJ" )) {strcpy(commandnumber, "1961");} else
                      if (strcompare(commandnumber,"SSIZE4" )) {strcpy(commandnumber, "1964");} else
                      if (strcompare(commandnumber,"SSIZE8" )) {strcpy(commandnumber, "1968");} else
                      if (strcompare(commandnumber,"XEQM01" ) && exec) {strcpy(commandnumber, "1979");} else
                      if (strcompare(commandnumber,"XEQM02" ) && exec) {strcpy(commandnumber, "1980");} else
                      if (strcompare(commandnumber,"XEQM03" ) && exec) {strcpy(commandnumber, "1981");} else
                      if (strcompare(commandnumber,"XEQM04" ) && exec) {strcpy(commandnumber, "1982");} else
                      if (strcompare(commandnumber,"XEQM05" ) && exec) {strcpy(commandnumber, "1983");} else
                      if (strcompare(commandnumber,"XEQM06" ) && exec) {strcpy(commandnumber, "1984");} else
                      if (strcompare(commandnumber,"XEQM07" ) && exec) {strcpy(commandnumber, "1985");} else
                      if (strcompare(commandnumber,"XEQM08" ) && exec) {strcpy(commandnumber, "1986");} else
                      if (strcompare(commandnumber,"XEQM09" ) && exec) {strcpy(commandnumber, "1987");} else
                      if (strcompare(commandnumber,"XEQM10" ) && exec) {strcpy(commandnumber, "1988");} else
                      if (strcompare(commandnumber,"XEQM11" ) && exec) {strcpy(commandnumber, "1989");} else
                      if (strcompare(commandnumber,"XEQM12" ) && exec) {strcpy(commandnumber, "1990");} else
                      if (strcompare(commandnumber,"XEQM13" ) && exec) {strcpy(commandnumber, "1991");} else
                      if (strcompare(commandnumber,"XEQM14" ) && exec) {strcpy(commandnumber, "1992");} else
                      if (strcompare(commandnumber,"XEQM15" ) && exec) {strcpy(commandnumber, "1993");} else
                      if (strcompare(commandnumber,"XEQM16" ) && exec) {strcpy(commandnumber, "1994");} else
                      if (strcompare(commandnumber,"XEQM17" ) && exec) {strcpy(commandnumber, "1995");} else
                      if (strcompare(commandnumber,"XEQM18" ) && exec) {strcpy(commandnumber, "1996");} else
                      if (strcompare(commandnumber,"ROUND" )) {strcpy(commandnumber, "1997");} else
                      if (strcompare(commandnumber,"ROUNDI" )) {strcpy(commandnumber, "1998");} else
// FROM SPREADSHEET ^^^ ****************************************************************************************************




                      if (strcompare(commandnumber,"DSZ"   )) {strcpy(commandnumber, "115"); gotoinprogress = 10;}      else //EXPECTING FOLLOWING OPERAND "nn"
                       if (strcompare(commandnumber,"ISZ"   )) {strcpy(commandnumber, "252"); gotoinprogress = 10;}      else //EXPECTING FOLLOWING OPERAND "nn"
                        if (strcompare(commandnumber,"LBL"))       {xeqlblinprogress = 10; }                              else //EXPECTING FOLLOWING OPERAND Mn
                         if (strcompare(commandnumber,"XEQLBL"))    {xeqlblinprogress =  1; }                                   //EXPECTING 2 OPERANDS nn XXXXXX
//         END ELSE
                      if (strcompare(commandnumber,"GTO"   ))    {
                        if(exec) {
                          gotoinprogress = 1;
                          /*if(gotoinprogress == 11) {go = (temporaryInformation == TI_FALSE);}
                          else                   */  {go = true;}
                          if(go) {
                            force_refresh();
                            #ifdef PC_BUILD
                            printf(">>> Loop:go\n");
                            #endif
                          }
                        }
                      } else
                      if (strcompare(commandnumber,"XEQ"   ))    {if(exec) {go = true; gotoinprogress = 1; ix_m = ix;}} else
                       if (strcompare(commandnumber,"RTN"   ))    {if(exec) {ix = ix_m; ix_m = 0;}}                      else
                        if (strcompare(commandnumber,"GTO_SZ"))    {if(exec) {go = (temporaryInformation == TI_FALSE); gotoinprogress = 1; }}
//         END ELSE
                      if (strcompare(commandnumber,"END"))       {ix = stringByteLength(inputstring)-2;}
                      if (strcompare(commandnumber,"RETURN"))    if(exec) {ix = stringByteLength(inputstring)-2;} 
                      


                      temporaryInformation = TI_NO_INFO;   //Cancel after go was determined.
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
                            strcpy(indexOfItemsXEQM + (no-1)*10, commandnumber);
                            xeqlblinprogress = 0;
                            commandnumber[0]=0;   //Processed
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
                          invertPixel(indic_x, indic_y-1);
                        break;
//HERE DEFAULT !!
                        default:                 //NOT IN PROGRESS
                          no = atoi(commandnumber);       //Will force invalid commands and RETURN MARK etc. to 0
                          //printf("$$$ case default %s EXEC=%d no=%d\n",commandnumber,exec,no);
                          if(no > LAST_ITEM-1) {no = 0;}
                          if(no!=0 && exec) {
                            invertPixel(indic_x++, indic_y);
                            if(indic_x==SCREEN_WIDTH) {indic_x=0;indic_y--;indic_y--;}

                            if(exec) {
                              runkey(no); 
                            }
                            //printf(">>> %d\n",temporaryInformation);
                            if(gotoinprogress==10) {gotoinprogress = 11;}
                          }
                          else {
                            //printf("Skip execution |%s|",commandnumber);
                          }
                          commandnumber[0]=0;   //Processed
                        break;
                      }
                    }
                    break;
          default:;           //ignore all other characters
        }
        if(state_quotes) {
          if (exec) {
            sendkeys(aa); //else printf("Skip sending |%s|",aa);
          }
        } 
        else { 
          if(state_commands && stringByteLength(commandnumber) < 20-1) {
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



void XEQMENU_Selection(uint16_t selection, char *line1, bool_t exec) {
#ifndef TESTSUITE_BUILD
  switch(selection) {
    case  1:import_string_from_filename(line1,"XEQM01","XEQLBL 01 ~3^-4   \"3\" ENTER \"4\" CHS Y^X "                                               ); displaywords(line1); execute_string(line1,exec); break;
    case  2:import_string_from_filename(line1,"XEQM02","XEQLBL 02 ~1/81   \"1\" ENTER \"81\" /   "                                                  ); displaywords(line1); execute_string(line1,exec); break;
    case  3:import_string_from_filename(line1,"XEQM03","XEQLBL 03 ~MP2203 TICKS \"2\" EXIT \"2203\" Y^X \"1\" - PRIME? X<>Y TICKS X<>Y - \"10\" / " ); displaywords(line1); execute_string(line1,exec); break;
    case  4:import_string_from_filename(line1,"XEQM04","XEQLBL 04 ~MP2281 TICKS \"2\" EXIT \"2281\" Y^X \"1\" - PRIME? X<>Y TICKS X<>Y - \"10\" / " ); displaywords(line1); execute_string(line1,exec); break;
    case  5:import_string_from_filename(line1,"XEQM05","XEQLBL 05 ~MP3217 TICKS \"2\" EXIT \"3217\" Y^X \"1\" - PRIME? X<>Y TICKS X<>Y - \"10\" / " ); displaywords(line1); execute_string(line1,exec); break;
    case  6:import_string_from_filename(line1,"XEQM06","XEQLBL 06 ~CUBE3  \"569936821221962380720\" EXIT \"3\" Y^X \"569936821113563493509\" CHS EXIT \"3\" Y^X \"472715493453327032\" CHS EXIT \"3\" Y^X + + "); displaywords(line1); execute_string(line1,exec); break;
    case  7:import_string_from_filename(line1,"XEQM07","XEQLBL 07 XEQM07 "); displaywords(line1); execute_string(line1,exec); break;
    case  8:import_string_from_filename(line1,"XEQM08","XEQLBL 08 XEQM08 "); displaywords(line1); execute_string(line1,exec); break;
    case  9:import_string_from_filename(line1,"XEQM09","XEQLBL 09 XEQM09 "); displaywords(line1); execute_string(line1,exec); break;
    case 10:import_string_from_filename(line1,"XEQM10","XEQLBL 10 XEQM10 "); displaywords(line1); execute_string(line1,exec); break;
    case 11:import_string_from_filename(line1,"XEQM11","XEQLBL 11 XEQM11 "); displaywords(line1); execute_string(line1,exec); break;
    case 12:import_string_from_filename(line1,"XEQM12","XEQLBL 12 XEQM12 "); displaywords(line1); execute_string(line1,exec); break;
    case 13:import_string_from_filename(line1,"XEQM13","XEQLBL 13 XEQM13 "); displaywords(line1); execute_string(line1,exec); break;
    case 14:import_string_from_filename(line1,"XEQM14","XEQLBL 14 XEQM14 "); displaywords(line1); execute_string(line1,exec); break;
    case 15:import_string_from_filename(line1,"XEQM15","XEQLBL 15 XEQM15 "); displaywords(line1); execute_string(line1,exec); break;
    case 16:import_string_from_filename(line1,"XEQM16","XEQLBL 16 XEQM16 "); displaywords(line1); execute_string(line1,exec); break;
    case 17:import_string_from_filename(line1,"XEQM17","XEQLBL 17 XEQM17 "); displaywords(line1); execute_string(line1,exec); break;
    case 18:import_string_from_filename(line1,"XEQM18","XEQLBL 18 XEQM18 "); displaywords(line1); execute_string(line1,exec); break;
    default:;
  }
#endif
}

void fnXEQMENU(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
  char line1[TMP_STR_LENGTH];
  XEQMENU_Selection( unusedParamButMandatory, line1, true);
  //calcMode = CM_BUG_ON_SCREEN;
  //temporaryInformation = TI_NO_INFO;
  #endif
}




/*
//Fixed test program, dispatching commands
void testprogram_12(uint16_t unusedParamButMandatory){

    runkey(ITM_TICKS); //622
    runkey(684);       //X<>Y
    sendkeys("2"); runkey(KEY_EXIT1); //EXIT
    runkey(684);   //X<>Y
    runkey(698);   //Y^X
    sendkeys("1"); runkey(KEY_EXIT1); //EXIT
    runkey(780);   //-
    runkey(589);   sendkeys("00"); //STO 00
    runkey(469);   //PRIME?
    runkey(684);   //X<>Y
    runkey(ITM_TICKS);
    runkey(684);   //X<>Y
    runkey(ITM_SUB);
}


//Fixed test program, dispatching commands from text string
void testprogram_1(uint16_t unusedParamButMandatory){
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


