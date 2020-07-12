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

/********************************************//** //JM
 * \file jm.c JM_TEST
 ***********************************************/

/* ADDITIONAL C43 functions and routines */



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
      #ifdef DMCP_BUILD
        lcd_forced_refresh(); // Just redraw from LCD buffer    
      #endif
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
                      
                      if(!(gotoinprogress != 11 || (gotoinprogress == 11 && (temporaryInformation == TI_FALSE)))) {                   //It is now the command that may or may not be skipped
                          go = (temporaryInformation == TI_FALSE); //As per GTO_SZ
                          gotoinprogress = 1;                      //As per GTO_SZ
                          commandnumber[0]=0;                      //As per GTO_SZ
                      } else //NOT gotoinprogress == 11

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
                      if (strcompare(commandnumber,"a" )) {strcpy(commandnumber, "11");} else
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

                      if (strcompare(commandnumber,"GTO"   ))    {
                        if(exec) {
                          gotoinprogress = 1;
                          /*if(gotoinprogress == 11) {go = (temporaryInformation == TI_FALSE);}
                          else                   */  {go = true;}
                        }
                      } else
                      if (strcompare(commandnumber,"XEQ"   ))    {if(exec) {go = true; gotoinprogress = 1; ix_m = ix;}} else
                      if (strcompare(commandnumber,"RTN"   ))    {if(exec) {ix = ix_m; ix_m = 0;}}                      else
                      if (strcompare(commandnumber,"GTO_SZ"))    {if(exec) {go = (temporaryInformation == TI_FALSE); gotoinprogress = 1; }}

                      if (strcompare(commandnumber,"END"))       {ix = stringByteLength(inputstring)-2;}
                      if (strcompare(commandnumber,"RETURN"))    if(exec) {ix = stringByteLength(inputstring)-2;} 
                      
                      switch(gotoinprogress) {
                        case 1:                  //GOTO IN PROGRESS: got command GOTO
                          gotoinprogress = 2;
                          commandnumber[0]=0;
                        break;
                        case 2:                  //GOTO IN PROGRESS: get softkeynumber 01 - 04
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
                            strcpy(indexOfItemsXEQM + (no-1)*12, commandnumber);
                            xeqlblinprogress = 0;
                            commandnumber[0]=0;   //Processed
                            #ifndef TESTSUITE_BUILD
                            showSoftmenuCurrentPart();
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

                        default:                 //NOT IN PROGRESS
                          no = atoi(commandnumber);       //Will force invalid commands and RETURN MARK etc. to 0
                          //printf("$$$ case default %s EXEC=%d no=%d\n",commandnumber,exec,no);
                          if(no > LAST_ITEM-1) {no = 0;}
                          if(no!=0 && exec) {
                            invertPixel(indic_x++, indic_y);
                            if(indic_x==SCREEN_WIDTH) {indic_x=0;indic_y--;indic_y--;}

                            if(exec) runkey(no); 
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
          if (exec) sendkeys(aa); //else printf("Skip sending |%s|",aa);
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
}



//Fixed test program, dispatching commands
void testprogram2(uint16_t unusedParamButMandatory){

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
void testprogram_mem(uint16_t unusedParamButMandatory){
char line1[TMP_STR_LENGTH];
   strcpy(line1,
 //   "BTN P1 TPRIME //TESTPRIME PROGRAM//"
/*
    "TICKS "
    "SWAP "
    "\"2\" EXIT "
    "684 "         //SWAP  "
    "Y^X "
    "\"1\" 1523  " //EXIT
    "-   "
    "STO \"00\"  "
    "PRIME?      "
    "X<>Y        "
    "622  "        //TICKS
    "X<>Y "
    "780  "        //-

    "RCL \"00\"  "
    "MARK4 "
    "\"1\" EXIT + "
    "PRIME?       "
    "GTO_M4_IF_0  "
*/
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



void XEQMENU_Selection(uint16_t selection, char *line1, bool_t exec) {
#ifndef TESTSUITE_BUILD
  switch(selection) {
    case  1:import_string_from_filename(line1,"XEQM01","XEQLBL 01 ~3^-4 \"3\" ENTER \"4\" CHS Y^X "                                                 ); displaywords(line1); execute_string(line1,exec); break;
    case  2:import_string_from_filename(line1,"XEQM02","XEQLBL 02 ~1/81 \"1\" ENTER \"81\" /   "                                                    ); displaywords(line1); execute_string(line1,exec); break;
    case  3:import_string_from_filename(line1,"XEQM03","XEQLBL 03 ~MP2203 TICKS \"2\" EXIT \"2203\" Y^X \"1\" - PRIME? X<>Y TICKS X<>Y - \"10\" / " ); displaywords(line1); execute_string(line1,exec); break;
    case  4:import_string_from_filename(line1,"XEQM04","XEQLBL 04 ~MP2281 TICKS \"2\" EXIT \"2281\" Y^X \"1\" - PRIME? X<>Y TICKS X<>Y - \"10\" / " ); displaywords(line1); execute_string(line1,exec); break;
    case  5:import_string_from_filename(line1,"XEQM05","XEQLBL 05 ~MP3217 TICKS \"2\" EXIT \"3217\" Y^X \"1\" - PRIME? X<>Y TICKS X<>Y - \"10\" / " ); displaywords(line1); execute_string(line1,exec); break;
    case  6:import_string_from_filename(line1,"XEQM06","XEQLBL 06 ~CUBE3 \"569936821221962380720\" EXIT \"3\" Y^X \"569936821113563493509\" CHS EXIT \"3\" Y^X \"472715493453327032\" CHS EXIT \"3\" Y^X + + "); displaywords(line1); execute_string(line1,exec); break;
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




void reset_jm_defaults(void) {
    SHOWregis = 9999;                                          //JMSHOW

    //JM defaults vv: CONFIG STO/RCL
    SigFigMode = 0;                                              //JM SIGFIG Default 0.
    eRPN = true;                 //TEMPORARY FOR TESTING         //JM eRPN Default. Create a flag to enable or disable eRPN. See bufferize.c
    HOME3 = true;                                                //JM HOME Default. Create a flag to enable or disable triple shift HOME3.
    ShiftTimoutMode = true;                                      //JM SHIFT Default. Create a flag to enable or disable SHIFT TIMER CANCEL.
    Home3TimerMode = true;                                       //JM SHIFT Default. Create a flag to enable or disable SHIFT TIMER MODE FOR HOME.
    UNITDisplay = false;                                         //JM HOME Default. Create a flag to enable or disable UNIT display
    SH_BASE_HOME   = true;                                       //JM
    SH_BASE_AHOME  = true;                                       //JM
    Norm_Key_00_VAR  = ITM_SIGMAPLUS;                            //JM
    Input_Default =  ID_43S;                                     //JM Input Default
    jm_FG_LINE = true;                                           //JM Screen / keyboard operation setup
    jm_FG_DOTS = false;                                          //JM Screen / keyboard operation setup
    jm_G_DOUBLETAP = false;                                      //JM Screen / keyboard operation setup
    graph_xmin = -3*3.14159265;                                //JM GRAPH
    graph_xmax = +3*3.14159265;                                //JM GRAPH
    graph_ymin = -2;                                           //JM GRAPH
    graph_ymax = +2;                                           //JM GRAPH
    graph_dx   = 0;                                            //JM GRAPH
    graph_dy   = 0;                                            //JM GRAPH
    jm_VECT = false;                                           //JM Screen / keyboard operation setup
    jm_HOME_SUM = false;                                       //JMHOME
    jm_HOME_MIR = true;                                        //JMHOME
    jm_HOME_FIX = false;                                       //JMHOME
    #if defined(DMCP_BUILD) || defined(JM_LAYOUT_2_DM42_STRICT)//JM LAYOUT 2. DM42 STRICT.
    jm_HOME_SUM = false;                                       //JMHOME
    jm_HOME_MIR = true;                                        //JMHOME
    jm_HOME_FIX = false;                                       //JMHOME
    #endif
    jm_LARGELI=true;
    setSystemFlag(FLAG_SPCRES);                                //JM default infinity etc.
    clearSystemFlag(FLAG_DENFIX);                              //JM default
    denMax = 64;                                               //JM default
     //JM defaults ^^


    fnXEQMENUpos = 0;
    while(indexOfItems[fnXEQMENUpos].func != fnXEQMENU) {
       fnXEQMENUpos++;
    }

    uint16_t ix;
    ix = 0;
    while(ix<18) {
      indexOfItemsXEQM[+12*ix]=0;
      strcpy(indexOfItemsXEQM +12*ix, indexOfItems[fnXEQMENUpos+ix].itemSoftmenuName);
      ix++;    
    }

    #ifndef TESTSUITE_BUILD
      clearScreen();
      char line1[TMP_STR_LENGTH];
      XEQMENU_Selection( 1, line1, false);
      XEQMENU_Selection( 2, line1, false);
      XEQMENU_Selection( 3, line1, false);
      XEQMENU_Selection( 4, line1, false);
      XEQMENU_Selection( 5, line1, false);
      XEQMENU_Selection( 6, line1, false);
      XEQMENU_Selection( 7, line1, false);
      XEQMENU_Selection( 8, line1, false);
      XEQMENU_Selection( 9, line1, false);
      XEQMENU_Selection(10, line1, false);
      XEQMENU_Selection(11, line1, false);
      XEQMENU_Selection(12, line1, false);
      XEQMENU_Selection(13, line1, false);
      XEQMENU_Selection(14, line1, false);
      XEQMENU_Selection(15, line1, false);
      XEQMENU_Selection(16, line1, false);
      XEQMENU_Selection(17, line1, false);
      XEQMENU_Selection(18, line1, false);
//      clearScreen(false, true, true);
    #endif
}

    int16_t fnXEQMENUpos = 0;




/********************************************//**
 * SEEMS TO BE WORKING. CALLED FROM ## in BASE
 *
 * FROM keyboard.c
 ***********************************************/
void fnBASE_Hash(uint16_t unusedParamButMandatory) {  //DONE
  int16_t lastChar;
  lastChar = strlen(nimBuffer) - 1;
  if(lastChar >= 1) {
    calcMode = CM_NIM;
#ifndef TESTSUITE_BUILD
    addItemToNimBuffer(ITM_toINT);
  }
  else {
    runFunction(ITM_toINT);
#endif
  }
}


/* NOTNEEDED
void fnKeyDotDjm(uint16_t unusedParamButMandatory) {      //FOR   HARDWIRED 
  userModeEnabledMEM = userModeEnabled;
  userModeEnabled = false;
  shiftF = false;                  //JM
  shiftG = true;                 //JM
#ifdef PC_BUILD
  btnClicked(NULL, "02");         //JM changed from 02
#endif
#ifdef DMCP_BUILD
  btnClicked(NULL, "02");         //JM changed from 02
#endif
  userModeEnabled = userModeEnabledMEM;
}
#endif
*/



/********************************************//**
 * \brief Sets/resets flag
 *
 * \param[in] jmConfig uint16_t
 * \return void
 ***********************************************/
void fnSetSetJM(uint16_t jmConfig) {                //DONE        //JM Set/Reset setting
  switch(jmConfig) {
  case JC_ERPN:                                             //JM eRPN
    eRPN = !eRPN;
    //fnInfo(eRPN);
    fnRefreshComboxState(CB_JC, JC_ERPN, eRPN);                                 //dr
    break;

  case JC_FG_LINE:                                          //JM
    jm_FG_LINE = !jm_FG_LINE;
    fnRefreshComboxState(CB_JC, JC_FG_LINE, jm_FG_LINE);                        //jm
    break;

  case JC_FG_DOTS:                                          //JM
    jm_FG_DOTS = !jm_FG_DOTS;
    fnRefreshComboxState(CB_JC, JC_FG_DOTS, jm_FG_DOTS);                        //jm
    break;

  case JC_G_DOUBLETAP:                                      //JM
    jm_G_DOUBLETAP = !jm_G_DOUBLETAP;
    fnRefreshComboxState(CB_JC, JC_G_DOUBLETAP, jm_G_DOUBLETAP);                //jm
    break;

  case JC_HOME_TRIPLE:                                      //JM HOME.3
    HOME3 = !HOME3;
    //fnInfo(HOME3);
    fnRefreshComboxState(CB_JC, JC_HOME_TRIPLE, HOME3);                         //dr
    break;

  case JC_SHFT_4s:                                          //JM SH_4s
    ShiftTimoutMode = !ShiftTimoutMode;
    //fnInfo(ShiftTimoutMode);
    fnRefreshComboxState(CB_JC, JC_SHFT_4s, ShiftTimoutMode);                   //dr
    break;

  case JC_BASE_HOME:                                        //JM HOME
    SH_BASE_HOME = !SH_BASE_HOME;
    //fnInfo(SH_BASE_HOME);
    fnRefreshComboxState(CB_JC, JC_BASE_HOME, SH_BASE_HOME);                    //dr
    break;

  case JC_BASE_AHOME:                                       //JM aHOME
    SH_BASE_AHOME = !SH_BASE_AHOME;
    //fnInfo(SH_BASE_AHOME);
    fnRefreshComboxState(CB_JC, JC_BASE_AHOME, SH_BASE_AHOME);                  //dr
    break;

  case JC_SH_3T:                                            //JM SH.3T
    Home3TimerMode = !Home3TimerMode;
    //fnInfo(Home3TimerMode);
    fnRefreshComboxState(CB_JC, JC_SH_3T, Home3TimerMode);                      //dr
    break;

  case JC_VECT:                                      //JM
    jm_VECT = !jm_VECT;
    fnRefreshComboxState(CB_JC, JC_VECT, jm_VECT);                //jm
    break;

  case JC_H_SUM:                                      //JM
    jm_HOME_SUM = !jm_HOME_SUM;
    fnRefreshComboxState(CB_JC, JC_H_SUM, jm_HOME_SUM);                //jm
    break;

  case JC_H_MIR:                                      //JM
    jm_HOME_MIR = !jm_HOME_MIR;
    fnRefreshComboxState(CB_JC, JC_H_MIR, jm_HOME_MIR);                //jm
    break;

  case JC_H_FIX:                                      //JM
    jm_HOME_FIX = !jm_HOME_FIX;
    fnRefreshComboxState(CB_JC, JC_H_FIX, jm_HOME_FIX);                //jm
    break;

  case JC_LARGELI:                                      //JM
    jm_LARGELI = !jm_LARGELI;
    fnRefreshComboxState(CB_JC, JC_LARGELI, jm_LARGELI);                //jm
    break;



   case RX_COMMA:               //DONR
     fnClearFlag(FLAG_DECIMP);
     break;

   case RX_PERIOD:               //DONR
     fnSetFlag(FLAG_DECIMP);
     break;

   case TF_H12:               //DONR
     fnClearFlag(FLAG_TDM24);
     break;

   case TF_H24:               //DONR
     fnSetFlag(FLAG_TDM24);
     break;

   case CU_I:               //DONR
     fnClearFlag(FLAG_CPXj);
     break;

   case CU_J:               //DONR
     fnSetFlag(FLAG_CPXj);
     break;

   case PS_DOT:
     fnClearFlag(FLAG_MULTx);
     break;

   case PS_CROSS:               //DONR
     fnSetFlag(FLAG_MULTx);
     break;

   case SS_4:                 //DONE
     fnClearFlag(FLAG_SSIZE8);
     break;

   case SS_8:          //DONE
     fnSetFlag(FLAG_SSIZE8);
     break;

   case CM_RECTANGULAR:          //DONE
     fnClearFlag(FLAG_POLAR);
     break;

   case CM_POLAR:                 //DONE
     fnSetFlag(FLAG_POLAR);
     break;

   case DO_SCI:
     fnClearFlag(FLAG_ALLENG);
     break;

   case DO_ENG:
     fnSetFlag(FLAG_ALLENG);
     break;



   case JC_BLZ:                                              //JM bit LeadingZeros
     fnFlipFlag(FLAG_LEAD0);                                       //dr
     break;

   case JC_BCR:                                              //JM bit ComplexResult
     fnFlipFlag(FLAG_CPXRES);                                     //dr
     break;

   case JC_BSR:                                              //JM bit ComplexResult
     fnFlipFlag(FLAG_SPCRES);                                     //dr
     break;


  default:
    break;
  }
}



/********************************************//**
 * \brief Set Input_Default
 *
 * \param[in] inputDefault uint16_t
 * \return void
 ***********************************************/
void fnInDefault(uint16_t inputDefault) {              //DONE
  Input_Default = inputDefault;

  if(Input_Default == ID_SI) {
    lastIntegerBase = 10;
  }
  else {
    lastIntegerBase = 0;
  }

  fnRefreshRadioState(RB_ID, inputDefault);
}



/********************************************//** XXX
 * \brief Set Norm_Key_00_VAR
 *
 * \param[in] sigmaAssign uint16_t
 * \return void
 ***********************************************/
void fnSigmaAssign(uint16_t sigmaAssign) {             //DONE
  Norm_Key_00_VAR = sigmaAssign;
  fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
  fnClearFlag(FLAG_USER);
}



/********************************************//**
 * \brief Displays TRUE/FALSE information
 *
 * \param[in] f bool_t
 * \return void
 ***********************************************/
//void fnInfo(bool_t f) {
//  temporaryInformation = f ? TI_TRUE : TI_FALSE;
//  refreshRegisterLine(TAM_REGISTER_LINE);
//  refreshRegisterLine(REGISTER_X);
//}



/********************************************//**
 * \brief Show flag value
 * \param[in] jmConfig to display uint16_t
 * \return void
 ***********************************************/
void fnShowJM(uint16_t jmConfig) {                               //DONE
  longInteger_t mem;
  longIntegerInit(mem);
  liftStack();

  switch(jmConfig) {
  case JC_ERPN:
    if(eRPN) { stringToLongInteger("1",10,mem); }
    else { stringToLongInteger("0",10,mem); }
    break;

  case JM_INP_DFLT:
    if(Input_Default == ID_43S) { stringToLongInteger("0",10,mem); }
    else if(Input_Default == ID_DP) { stringToLongInteger("2",10,mem); }
    else if(Input_Default == ID_CPXDP) { stringToLongInteger("4",10,mem); }
    else if(Input_Default == ID_SI) { stringToLongInteger("6",10,mem); }
    else if(Input_Default == ID_LI) { stringToLongInteger("7",10,mem); }
    break;

  default:
    break;
  }

  convertLongIntegerToLongIntegerRegister(mem, REGISTER_X);
  longIntegerFree(mem);

}





/** integer to string
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char* itoa(int value, char* result, int base) {      //DONE
    // check that the base if valid
    if (base < 2 || base > 16) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}



/********************************************//**
 * \brief Get item-value of assigned key to X
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetSigmaAssignToX(uint16_t unusedParamButMandatory) {       //DONE
  char snum[10];
  longInteger_t mem;
  longIntegerInit(mem);
  liftStack();

  itoa(Norm_Key_00_VAR, snum, 10);
  stringToLongInteger(snum,10,mem);

  convertLongIntegerToLongIntegerRegister(mem, REGISTER_X);
  longIntegerFree(mem);

}



//JM CONFIGURE USER MODE - ASSIGN KEYS

/********************************************//**
 * \brief
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnJM_GetXToNORMmode(uint16_t unusedParamButMandatory) {      //DONE
  int16_t X_REG;
  longInteger_t lgInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
    longIntegerToInt(lgInt,X_REG);
    longIntegerFree(lgInt);
    //printf("Xreg %d\n", X_REG);
    Norm_Key_00_VAR = X_REG;
    fnClearFlag(FLAG_USER);
  }
}



/********************************************//**
 * \brief
 *
 * \param[in] JM_KEY uint16_t
 * \return void
 ***********************************************/
void fnJMUSERmode(uint16_t JM_KEY) {      //DONE
  int16_t X_REG;
  longInteger_t lgInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
    longIntegerToInt(lgInt,X_REG);
    longIntegerFree(lgInt);
  //printf("Xreg %d\n", X_REG);
    if(JM_KEY >= 256) {
      kbd_usr[JM_KEY - 256].primary = X_REG;
    //printf(".primary %d\n", kbd_usr[JM_KEY - 256].primary);
      fnSetFlag(FLAG_USER);
    }
  }
}



/********************************************//**
 * \brief
 *
 * \param[in] JM_KEY uint16_t
 * \return void
 ***********************************************/
void fnJMUSERmode_f(uint16_t JM_KEY) {      //DONE
  int16_t X_REG;
  longInteger_t lgInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
    longIntegerToInt(lgInt,X_REG);
    longIntegerFree(lgInt);
  //printf("Xreg %d\n", X_REG);
    if(JM_KEY >= 256) {
      kbd_usr[JM_KEY - 256].fShifted = X_REG;
    //printf(".fShifted %d\n", kbd_usr[JM_KEY - 256].fShifted);
      fnSetFlag(FLAG_USER);
    }
  }
}



/********************************************//**
 * \brief
 *
 * \param[in] JM_KEY uint16_t
 * \return void
 ***********************************************/
void fnJMUSERmode_g(uint16_t JM_KEY) {      //DONE
  int16_t X_REG;
  longInteger_t lgInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
    longIntegerToInt(lgInt,X_REG);
    longIntegerFree(lgInt);
  //printf("Xreg %d\n", X_REG);
    if(JM_KEY >= 256) {
      kbd_usr[JM_KEY - 256].gShifted = X_REG;
      //printf(".gShifted %d\n", kbd_usr[JM_KEY - 256].gShifted);
      fnSetFlag(FLAG_USER);
    }
  }
}



//---------------------------------------------


void fnStrtoX(char aimBuffer[]) {      //DONE
  setSystemFlag(FLAG_ASLIFT);   // 5
  liftStack();
  int16_t mem = stringByteLength(aimBuffer);
  reallocateRegister(REGISTER_X, dtString, mem, AM_NONE);
  xcopy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, mem + 1);
}



void fnStrInputReal34(char inp1[]) {  // CONVERT STRING to REAL IN X      //DONE
  tmpStr3000[0] = 0;
  strcat(tmpStr3000, inp1);
  setSystemFlag(FLAG_ASLIFT);   // 5
  liftStack();
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  stringToReal34(tmpStr3000, REGISTER_REAL34_DATA(REGISTER_X));
}



void fnStrInputLongint(char inp1[]) {  // CONVERT STRING to Longint X      //DONE
  tmpStr3000[0]=0;
  strcat(tmpStr3000, inp1);
  setSystemFlag(FLAG_ASLIFT);   // 5
  liftStack();

  longInteger_t lgInt;
  longIntegerInit(lgInt);
  stringToLongInteger(tmpStr3000 + (tmpStr3000[0] == '+' ? 1 : 0), 10, lgInt);
  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
  longIntegerFree(lgInt);
}




void fnRCL(int16_t inp) {      //DONE
  setSystemFlag(FLAG_ASLIFT);
  if(inp == TEMP_REGISTER) {
    liftStack();
    copySourceRegisterToDestRegister(inp, REGISTER_X);
  } else {
  fnRecall(inp);
  }
}





uint16_t nprimes = 0;
/********************************************//**
 * RPN PROGRAM.
 *
 * \param[in] JM_OPCODE uint16_t
 * \return void
 ***********************************************/
void fnJM(uint16_t JM_OPCODE) {
#define JMTEMP TEMP_REGISTER

  if(JM_OPCODE == 6) {                                          //Delta to Star   ZYX to ZYX; destroys IJKL & JMTEMP
    saveStack();
    setSystemFlag(FLAG_ASLIFT);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);   // STO I
    copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);   // STO J
    copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_K);   // STO K
    fnAdd(0);                                                   // +
    fnSwapXY(0);                                                // X<>Y

    fnAdd(0);                                                   // +
    copySourceRegisterToDestRegister(REGISTER_X, JMTEMP);       // STO JMTEMP
    fnRCL(REGISTER_K);                                          // RCL I
    fnRCL(REGISTER_J);                                          // RCL J     // z = (zx yz) / (x+y+z)
    fnMultiply(0);                                              // *
    fnSwapXY(0);                                                // X<>Y
    fnDivide(0);                                                // /

    fnRCL(JMTEMP);                                              // RCL JMTEMP
    fnRCL(REGISTER_I);                                          // RCL J
    fnRCL(REGISTER_J);                                          // RCL K     // y = (xy yz) / (x+y+z)
    fnMultiply(0);                                              // *
    fnSwapXY(0);                                                // X<>Y
    fnDivide(0);                                                // /

    fnRCL(JMTEMP);                                              // RCL JMTEMP
    fnRCL(REGISTER_I);                                          // RCL I
    fnRCL(REGISTER_K);                                          // RCL K     // z = (xy zx) / (x+y+z)
    fnMultiply(0);                                              // *
    fnSwapXY(0);                                                // X<>Y
    fnDivide(0);                                                // /

    copySourceRegisterToDestRegister(REGISTER_I, REGISTER_L);   // STO

    temporaryInformation = TI_ABC;

    adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
    adjustResult(REGISTER_Y, false, true, REGISTER_Y, -1, -1);
    adjustResult(REGISTER_Z, false, true, REGISTER_Z, -1, -1);
  }
  else

  if(JM_OPCODE == 7) {                                          //Star to Delta ZYX to ZYX; destroys IJKL & JMTEMP
    saveStack();
    setSystemFlag(FLAG_ASLIFT);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);   // STO I
    copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);   // STO J
    copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_K);   // STO K

    fnMultiply(0);                          //IJ                // *
    fnSwapXY(0);
    fnRCL(REGISTER_I);                                          // RCL J
    fnMultiply(0);                          //IK                // *
    fnAdd(0);
    fnRCL(REGISTER_J);                                          // RCL J
    fnRCL(REGISTER_K);                                          // RCL K
    fnMultiply(0);                          //JK                // *
    fnAdd(0);
    copySourceRegisterToDestRegister(REGISTER_X, JMTEMP);       // STO JMTEMP
                                                                //
    fnRCL(REGISTER_J);                                          //      zx = () / y
    fnDivide(0);                                                // 

    fnRCL(JMTEMP);                                              // RCL JMTEMP    
    fnRCL(REGISTER_I);                                          //      yz = () / x
    fnDivide(0);                                                //

    fnRCL(JMTEMP);                                              // RCL JMTEMP    
    fnRCL(REGISTER_K);                                          //      xy = () / z
    fnDivide(0);                                                //

    copySourceRegisterToDestRegister(REGISTER_I, REGISTER_L);   // STO

    temporaryInformation = TI_ABBCCA;
    adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
    adjustResult(REGISTER_Y, false, true, REGISTER_Y, -1, -1);
    adjustResult(REGISTER_Z, false, true, REGISTER_Z, -1, -1);
  }
  else

  if(JM_OPCODE == 8) {                                          //SYMMETRICAL COMP to ABC   ZYX to ZYX; destroys IJKL
    saveStack();
    setSystemFlag(FLAG_ASLIFT);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);   // STO I  //A2
    copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);   // STO J  //A1
    copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_K);   // STO K  //A0
    fnAdd(0);                                                   // +
    fnAdd(0);                                                   // + Va = Vao + Va1 +Va2

    setSystemFlag(FLAG_ASLIFT);
    liftStack();
    fn_cnst_op_a(0);
    fnRCL(REGISTER_I);                                       // A2
    fnMultiply(0);                                              // * a
    setSystemFlag(FLAG_ASLIFT);
    liftStack();
    fn_cnst_op_aa(0);
    fnRCL(REGISTER_J);                                       // A1
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    fnRCL(REGISTER_K);                                       // A0
    fnAdd(0);                                                   // + Vb = Vao + aaVa1 +aVa2

    setSystemFlag(FLAG_ASLIFT);
    liftStack();
    fn_cnst_op_aa(0);
    fnRCL(REGISTER_I);                                       // A2
    fnMultiply(0);                                              // * a
    setSystemFlag(FLAG_ASLIFT);
    liftStack();
    fn_cnst_op_a(0);
    fnRCL(REGISTER_J);                                       // A1
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    fnRCL(REGISTER_K);                                       // A0
    fnAdd(0);                                                   // + Vb = Vao + aaVa1 +aVa2

    copySourceRegisterToDestRegister(REGISTER_I, REGISTER_L);   // STO

    temporaryInformation = TI_ABC;
  }
  else

  if(JM_OPCODE == 9) {                                          //ABC to SYMMETRICAL COMP   ZYX to ZYX; destroys IJKL & JMTEMP
    saveStack();
    setSystemFlag(FLAG_ASLIFT);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);  // STO I  //c
    copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);  // STO J  //b
    copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_K);  // STO K  //a
    fnAdd(0);                                                  // +
    fnAdd(0);                                                  // + Va0 = (Va + Vb +Vc)/3
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
    stringToReal34("3", REGISTER_REAL34_DATA(REGISTER_X));
    stringToReal34("0", REGISTER_IMAG34_DATA(REGISTER_X));      //
    copySourceRegisterToDestRegister(REGISTER_X, JMTEMP);       // STO
    fnDivide(0);


    setSystemFlag(FLAG_ASLIFT);
    liftStack();
    fn_cnst_op_a(0);
    fnRCL(REGISTER_J);                                       // VB
    fnMultiply(0);                                              // * a
    setSystemFlag(FLAG_ASLIFT);
    liftStack();
    fn_cnst_op_aa(0);
    fnRCL(REGISTER_I);                                       // VC
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    fnRCL(REGISTER_K);                                       // VA
    fnAdd(0);                                                   // + V1 = (VA +aVb +aaVc) /3
    fnRCL(JMTEMP);                                              // 3
    fnDivide(0);                                                // /


    setSystemFlag(FLAG_ASLIFT);
    liftStack();
    fn_cnst_op_aa(0);
    fnRCL(REGISTER_J);                                       // VB
    fnMultiply(0);                                              // * a
    setSystemFlag(FLAG_ASLIFT);
    liftStack();
    fn_cnst_op_a(0);
    fnRCL(REGISTER_I);                                       // VC
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    fnRCL(REGISTER_K);                                       // VA
    fnAdd(0);                                                   // + V1 = (VA +aVb +aaVc) /3
    fnRCL(JMTEMP);                                              // 3
    fnDivide(0);                                                // /

    copySourceRegisterToDestRegister(REGISTER_I, REGISTER_L);   // STO


    temporaryInformation = TI_012;
  }
  else

  if(JM_OPCODE == 10) {                                         //e^theta.j j
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);   // STO TMP
    fn_cnst_op_j(0);
    fnMultiply(0);                                              // * aa
    fnExp(0);
  }
  else

  if(JM_OPCODE == 11) {                                         //STO Z
    saveStack();
    setSystemFlag(FLAG_ASLIFT);                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    copySourceRegisterToDestRegister(REGISTER_X, 90);
    copySourceRegisterToDestRegister(REGISTER_Y, 91);
    copySourceRegisterToDestRegister(REGISTER_Z, 92);
  }
  else

  if(JM_OPCODE == 13) {                                         //STO V
    saveStack();
    setSystemFlag(FLAG_ASLIFT);                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    copySourceRegisterToDestRegister(REGISTER_X, 93);
    copySourceRegisterToDestRegister(REGISTER_Y, 94);
    copySourceRegisterToDestRegister(REGISTER_Z, 95);
  }
  else

  if(JM_OPCODE == 15) {                                         //STO I
    saveStack();
    setSystemFlag(FLAG_ASLIFT);                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    copySourceRegisterToDestRegister(REGISTER_X, 96);
    copySourceRegisterToDestRegister(REGISTER_Y, 97);
    copySourceRegisterToDestRegister(REGISTER_Z, 98);
  }
  else

  if(JM_OPCODE == 12) {                                         //RCL Z
    saveStack();
    fnRCL(92);
    fnRCL(91);
    fnRCL(90);
  }
  else

  if(JM_OPCODE == 14) {                                         //RCL V
    saveStack();
    fnRCL(95);
    fnRCL(94);
    fnRCL(93);
  }
  else

  if(JM_OPCODE == 16) {                                         //RCL I
    saveStack();
    fnRCL(98);
    fnRCL(97);
    fnRCL(96);
  }
  else

  if(JM_OPCODE == 17) {                                         // V/I
    saveStack();
    fnRCL(95);
    fnRCL(98);
    fnDivide(0);
    fnRCL(94);
    fnRCL(97);
    fnDivide(0);
    fnRCL(93);
    fnRCL(96);
    fnDivide(0);
  }
  else

  if(JM_OPCODE == 18) {                                         // IZ
    saveStack();
    fnRCL(98);
    fnRCL(92);
    fnMultiply(0);
    fnRCL(97);
    fnRCL(91);
    fnMultiply(0);
    fnRCL(96);
    fnRCL(91);
    fnMultiply(0);
  }
  else

  if(JM_OPCODE == 19) {                                         // V/Z
    saveStack();
    fnRCL(95);
    fnRCL(92);
    fnDivide(0);
    fnRCL(94);
    fnRCL(91);
    fnDivide(0);
    fnRCL(93);
    fnRCL(90);
    fnDivide(0);
  }
  else

  if(JM_OPCODE == 20) {                                         //Copy Create X>ABC
    saveStack();
    setSystemFlag(FLAG_ASLIFT);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);

    fnRCL(REGISTER_I);                                          //
    setSystemFlag(FLAG_ASLIFT);
    liftStack();
    fn_cnst_op_a(0);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_J);
    fnMultiply(0);

    fnRCL(REGISTER_I);                                          //
    setSystemFlag(FLAG_ASLIFT);
    liftStack();
    fn_cnst_op_aa(0);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_J);
    fnMultiply(0);
  }
  else

  if(JM_OPCODE == 21) {                                         //Graph
    saveStack();
    //Convert from X register to float
    real_t tmpy;
    fnToReal(0);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
    realToString(&tmpy, tmpStr3000);
    graph_xmin = strtof(tmpStr3000, NULL);
    //printf("%s %f\n",tmpStr3000,graph_xmin);
    fnDrop(0);
  }
  else
  if(JM_OPCODE == 22) {                                         //Graph
    saveStack();
    //Convert from X register to float
    real_t tmpy;
    fnToReal(0);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
    realToString(&tmpy, tmpStr3000);
    graph_xmax = strtof(tmpStr3000, NULL);
    //printf("%s %f\n",tmpStr3000,graph_xmax);
    fnDrop(0);
  }
  else
  if(JM_OPCODE == 23) {                                         //Graph
    saveStack();
    //Convert from X register to float
    real_t tmpy;
    fnToReal(0);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
    realToString(&tmpy, tmpStr3000);
    graph_ymin = strtof(tmpStr3000, NULL);
    //printf("%s %f\n",tmpStr3000,graph_ymin);
    fnDrop(0);
  }
  else
  if(JM_OPCODE == 24) {                                         //Graph
    saveStack();
    //Convert from X register to float
    real_t tmpy;
    fnToReal(0);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
    realToString(&tmpy, tmpStr3000);
    graph_ymax = strtof(tmpStr3000, NULL);
    //printf("%s %f\n",tmpStr3000,graph_ymax);
    fnDrop(0);
  }
  else
  if(JM_OPCODE == 25) {                                         //Graph
    saveStack();
    //Convert from X register to float
    real_t tmpy;
    fnToReal(0);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
    realToString(&tmpy, tmpStr3000);
    graph_dx = strtof(tmpStr3000, NULL);
    //printf("%s %f\n",tmpStr3000,graph_dx);
    fnDrop(0);
  }
  else
  if(JM_OPCODE == 26) {                                         //Graph
    saveStack();
    //Convert from X register to float
    real_t tmpy;
    fnToReal(0);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
    realToString(&tmpy, tmpStr3000);
    graph_dy = strtof(tmpStr3000, NULL);
    //printf("%s %f\n",tmpStr3000,graph_dy);
    fnDrop(0);
  }
  else
  if(JM_OPCODE == 27) {                                         //Graph
    saveStack();
    fnStrtoX("Type limits into X Register and press");
    fnStrtoX("[Xmin], [Xmax], [Ymin], [Ymax], [dX], [dY]");
    tmpStr3000[0]=0;
    char tmp[12];
    snprintf(tmp, 12, "%.5f, ", graph_xmin);
    strcat(tmpStr3000,tmp);
    snprintf(tmp, 12, "%.5f, ", graph_xmax);
    strcat(tmpStr3000,tmp);
    snprintf(tmp, 12, "%.5f, ", graph_ymin);
    strcat(tmpStr3000,tmp);
    snprintf(tmp, 12, "%.5f, ", graph_ymax);
    strcat(tmpStr3000,tmp);
    snprintf(tmp, 12, "%.3f, ", graph_dx);
    strcat(tmpStr3000,tmp);
    snprintf(tmp, 12, "%.3f", graph_dy);
    strcat(tmpStr3000,tmp);
    fnStrtoX(tmpStr3000);
    fnStrtoX("[PLOT] graphs, [SNAP] saves screen");
  }
  else



  if(JM_OPCODE == 45) {                                         //PRIME stats
    #ifndef TESTSUITE_BUILD

    longInteger_t xx3;
    longIntegerInit(xx3);
    runFunction(ITM_CLSIGMA);

    //Get 'from' loop value from Z: 1 to 399 (default 0), for starting number exponents 10^(4*1) through 10^(4*399)
    uint16_t ix1, ixx1;
    if(getRegisterDataType(REGISTER_Z) != dtLongInteger) {
      convertReal34ToLongIntegerRegister(REGISTER_REAL34_DATA(REGISTER_Z), REGISTER_Z, DEC_ROUND_DOWN);
    }
    if(getRegisterDataType(REGISTER_Z) == dtLongInteger) {
      convertLongIntegerRegisterToLongInteger(REGISTER_Z, xx3);
      longIntegerToAllocatedString(xx3, tmpStr3000, TMP_STR_LENGTH);
      longIntegerToInt(xx3,ix1);
    } else {ix1 = 0;}
    if(ix1 > 399) {ix1 = 0;}
    ixx1 = ix1;

    //Get 'to' (maximum) loops from Y: ix1 to 399  (default 0 or ix1), for starting number exponents 10^ix1 through 10^(4*399)
    uint16_t ix2;
    if(getRegisterDataType(REGISTER_Y) != dtLongInteger) {
      convertReal34ToLongIntegerRegister(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_Y, DEC_ROUND_DOWN);
    }
    if(getRegisterDataType(REGISTER_Y) == dtLongInteger) {
      convertLongIntegerRegisterToLongInteger(REGISTER_Y, xx3);
      longIntegerToAllocatedString(xx3, tmpStr3000, TMP_STR_LENGTH);
      longIntegerToInt(xx3,ix2);
    } else {ix2 = 0;}
    if(ix2 < ix1) {ix2 = ix1;}
    if(ix2 > 399) {ix2 = ix1;}

    //Get number of repeated nextprimes from X from 1 to 100, default 10
    uint16_t ix3 = 0;
    if(getRegisterDataType(REGISTER_X) != dtLongInteger) {
      convertReal34ToLongIntegerRegister(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_X, DEC_ROUND_DOWN);
    }
    if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
      convertLongIntegerRegisterToLongInteger(REGISTER_X, xx3);
      longIntegerToAllocatedString(xx3, tmpStr3000, TMP_STR_LENGTH);
      longIntegerToInt(xx3,ix3);
    } else {ix3 = 10;}
    if(ix3 < 1 || ix3 > 100) {ix3 = 10;}
    nprimes = ix3;



    //Outer loop start
    while (ix1 <= ix2 ) {
      //printf("--%lld--%lld--\n",ix1,ix1*4);
      uIntToLongInteger(ix1*4, xx3);
      convertLongIntegerToLongIntegerRegister(xx3, REGISTER_X);
      adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
      tenPowLonI();

//      clearScreen(false,true,true);
      sprintf(tmpStr3000,"i=10^4Z to i=10^4Y, n primes>i:ZYX %d %d %d|",ix1, ix2,ix3);
      #ifdef PC_BUILD
        printf(tmpStr3000);
      #endif
      print_linestr(tmpStr3000,true);

      fnJM(46);

      setSystemFlag(FLAG_ASLIFT);
      liftStack();
      uIntToLongInteger(ix1*4, xx3);
      convertLongIntegerToLongIntegerRegister(xx3, REGISTER_X);
      adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);

      runFunction(ITM_SIGMAPLUS);
      setSystemFlag(FLAG_ASLIFT);

      ix1++;
    }
    longIntegerFree(xx3);
    runFunction(ITM_DROP);

    if(ixx1!=ix2) {runFunction(ITM_PLOT);}
    #endif
  }
  else





 if(JM_OPCODE == 46) {                                         // NEXT |PRIME ROUTINE
    #ifndef TESTSUITE_BUILD
    uint32_t getUptimeMs0 = getUptimeMs();
    int16_t ix;

    calcMode = CM_BUG_ON_SCREEN;              //Hack to prevent calculator to restart operation. Used to view graph

    longInteger_t lgInt;
    longIntegerInit(lgInt);

    ix = 0;
    while (ix < nprimes ) {
      runFunction(ITM_NEXTP);
      fnStore(ix);

      char tmpstr[200];
      char tmpstr2[300];
      sprintf(tmpstr2,"[%d]:",ix);
      longIntegerRegisterToDisplayString(REGISTER_X, tmpstr, 200, 399-32, 50, STD_SPACE_4_PER_EM, false);  //JM added last parameter: Allow LARGELI
      strcat(tmpstr2,tmpstr);

      print_linestr(tmpstr2,false);

      ix++;

    }
    getUptimeMs0 = getUptimeMs() - getUptimeMs0;

    setSystemFlag(FLAG_ASLIFT);
    liftStack();
    uIntToLongInteger(getUptimeMs0, lgInt);
    convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
    longIntegerFree(lgInt);
    #endif

 } else

 if(JM_OPCODE == 47) {                                         // CALL SETTINGS
    #ifndef TESTSUITE_BUILD
    runFunction(ITM_FF);

/*    int16_t m;
    m = 0;
    while(softmenu[m].menuId != 0) {
      if(softmenu[m].menuId == ITM_CFG) {
       break;
      }
      m++;
    }
    pushSoftmenu(m);
*/
    showSoftmenu(NULL, -MNU_SYSFL, true);
    #endif
  }



// Item 255 is NOP

}






/********************************************//**
 * \brief Sets/resets USER
 *
 * \param[in] jmConfig uint16_t
 * \return void
 ***********************************************/
void fnUserJM(uint16_t jmUser) {
  switch(jmUser) {
  case USER_DEFAULTS:                                           //USER_DEFAULTS FOR USER: E+ CC
    kbd_usr[0].primary     = KEY_CC;
    kbd_usr[0].gShifted    = KEY_TYPCON_UP;
    kbd_usr[0].fShifted    = KEY_TYPCON_DN;
    Norm_Key_00_VAR        = KEY_CC;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;


#if defined (JM_LAYOUT_1A)  && !defined (TESTSUITE_BUILD)                          //JM LAYOUT 1A.
  case USER_COMPLEX:                                            //USER_COMPLEX FOR USER: U^ ENTER^ CC
    kbd_usr[12].fShifted   = KEY_CC;                            //JM Changed CPX menu therefore USER MODE changes
    kbd_usr[0].primary     = -MNU_MyMenu;
    kbd_usr[0].gShifted    = KEY_TYPCON_UP;
    kbd_usr[0].fShifted    = KEY_TYPCON_DN;
    Norm_Key_00_VAR        = -MNU_MyMenu;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_CC:                                            //USER_COMPLEX FOR USER: U^ ENTER^ CC
    kbd_usr[0].primary     = KEY_CC;
    Norm_Key_00_VAR        = KEY_CC;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;
#endif

#if defined (JM_LAYOUT_2_DM42_STRICT) && !defined (TESTSUITE_BUILD)              //JM LAYOUT 42
  case USER_COMPLEX:                                            //USER_COMPLEX FOR USER: U^ ENTER^ CC
    //kbd_usr[12].gShifted   = KEY_CC;
    kbd_usr[0].primary     = -MNU_MyMenu;
    kbd_usr[0].gShifted    = KEY_TYPCON_UP;
    kbd_usr[0].fShifted    = KEY_TYPCON_DN;
    Norm_Key_00_VAR        = -MNU_MyMenu;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_CC:                                            //USER_COMPLEX FOR USER: U^ ENTER^ CC
    kbd_usr[0].primary     = KEY_CC;
    Norm_Key_00_VAR        = KEY_CC;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;
#endif


  case USER_V43LT:

    fnUserJM(USER_V43);

    kbd_usr[6].primary      = KEY_f;
    kbd_usr[6].fShifted     = ITM_NULL;
    kbd_usr[6].gShifted     = ITM_NULL;
    kbd_usr[6].keyLblAim     = KEY_f;
    kbd_usr[6].primaryAim     = KEY_f;
     kbd_usr[6].fShiftedAim     = ITM_NULL;
     kbd_usr[6].gShiftedAim     = ITM_NULL;
    kbd_usr[6].primaryTam     = KEY_f;

    kbd_usr[7].primary      = KEY_g;
    kbd_usr[7].fShifted     = ITM_NULL;
    kbd_usr[7].gShifted     = ITM_NULL;
    kbd_usr[7].keyLblAim     = KEY_g;
    kbd_usr[7].primaryAim     = KEY_g;
     kbd_usr[7].fShiftedAim     = ITM_NULL;
     kbd_usr[7].gShiftedAim     = ITM_NULL;
    kbd_usr[7].primaryTam     = KEY_g;


    kbd_usr[4].primary    = ITM_STO;
    kbd_usr[4].fShifted   = ITM_MAGNITUDE;
    kbd_usr[4].gShifted   = ITM_ANGLE;
    kbd_usr[4].primaryAim     = CHR_E;

    kbd_usr[5].primary    = ITM_RCL;
    kbd_usr[5].fShifted   = ITM_PC;
    kbd_usr[5].gShifted   = ITM_DELTAPC;
    kbd_usr[5].primaryAim     = CHR_F;

    Norm_Key_00_VAR        = KEY_USERMODE;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;


  case USER_V43:          //USER

    fnUserJM(USER_RESET);

    kbd_usr[0].primary      = ITM_1ONX;
    kbd_usr[0].fShifted     = ITM_YX;
    kbd_usr[0].gShifted     = ITM_toINT;

    kbd_usr[1].primary      = ITM_SQUAREROOTX;
    kbd_usr[1].fShifted     = ITM_SQUARE;
    kbd_usr[1].gShifted     = ITM_ms;

    kbd_usr[2].primary      = ITM_LOG10;
    kbd_usr[2].fShifted     = ITM_10x;
    kbd_usr[2].gShifted     = KEY_dotD;

    kbd_usr[3].primary      = ITM_LN;
    kbd_usr[3].fShifted     = ITM_EX;
    kbd_usr[3].gShifted     = ITM_TGLFRT;

    kbd_usr[4].primary      = KEY_f;
    kbd_usr[4].fShifted     = ITM_NULL;
    kbd_usr[4].gShifted     = ITM_NULL;
    kbd_usr[4].keyLblAim     = KEY_f;
    kbd_usr[4].primaryAim     = KEY_f;
     kbd_usr[4].fShiftedAim     = ITM_NULL;
     kbd_usr[4].gShiftedAim     = ITM_NULL;
    kbd_usr[4].primaryTam     = KEY_f;

    kbd_usr[5].primary      = KEY_g;
    kbd_usr[5].fShifted     = ITM_NULL;
    kbd_usr[5].gShifted     = ITM_NULL;
    kbd_usr[5].keyLblAim     = KEY_g;
    kbd_usr[5].primaryAim     = KEY_g;
     kbd_usr[5].fShiftedAim     = ITM_NULL;
     kbd_usr[5].gShiftedAim     = ITM_NULL;
    kbd_usr[5].primaryTam     = KEY_g;

    kbd_usr[6].primaryAim     = CHR_E;
    kbd_usr[7].primaryAim     = CHR_F;
    kbd_usr[8].primaryAim     = CHR_G;
    kbd_usr[9].primaryAim     = CHR_H;
    kbd_usr[10].primaryAim     = CHR_I;
    kbd_usr[11].primaryAim     = CHR_J;

    kbd_usr[13].primaryAim     = CHR_K;
    kbd_usr[14].primaryAim     = CHR_L;
    kbd_usr[15].primaryAim     = CHR_M;

    kbd_usr[17].primary     = ITM_SUB;
    kbd_usr[17].fShifted    = -MNU_STAT;
    kbd_usr[17].gShifted    = -MNU_SUMS;
    kbd_usr[17].keyLblAim     = CHR_MINUS;
    kbd_usr[17].primaryAim     = CHR_N;
    kbd_usr[17].fShiftedAim     = CHR_MINUS;
    kbd_usr[17].primaryTam     = ITM_SUB;

    kbd_usr[18].primaryAim     = CHR_O;
    kbd_usr[19].primaryAim     = CHR_P;
    kbd_usr[20].primaryAim     = CHR_Q;

    kbd_usr[21].primary     = ITM_XEQ;
    kbd_usr[21].fShifted    = ITM_AIM;
    kbd_usr[21].gShifted    = KEY_USERMODE;
    kbd_usr[21].keyLblAim     = ITM_NULL;
    kbd_usr[21].primaryAim     = CHR_R;
    kbd_usr[21].fShiftedAim     = ITM_NULL;
    kbd_usr[21].gShiftedAim     = ITM_NULL;
    kbd_usr[21].primaryTam     = CHR_alpha;

    kbd_usr[22].primary     = ITM_ADD;
    kbd_usr[22].fShifted    = -MNU_PROB;
    kbd_usr[22].gShifted    = -MNU_INTS;
    kbd_usr[22].keyLblAim     = CHR_PLUS;
    kbd_usr[22].primaryAim     = CHR_S;
    kbd_usr[22].fShiftedAim     = CHR_PLUS;
    kbd_usr[22].primaryTam     = ITM_ADD;

    kbd_usr[26].primary     = KEY_UP1;
    kbd_usr[26].fShifted    = KEY_BST;
    kbd_usr[26].gShifted    = ITM_RBR;
    kbd_usr[26].keyLblAim     = KEY_UP1;
    kbd_usr[26].primaryAim     = KEY_UP1;  //arrow
    kbd_usr[26].fShiftedAim     = KEY_BST;
    kbd_usr[26].gShiftedAim     = ITM_NULL;
    kbd_usr[26].primaryTam     = KEY_UP1;

    kbd_usr[27].primary     = ITM_MULT;       //ex-f/g
    kbd_usr[27].fShifted    = -MNU_STK;
    kbd_usr[27].gShifted    = -MNU_ALPHAFN;
    kbd_usr[27].keyLblAim     = CHR_CROSS;
    kbd_usr[27].primaryAim     = CHR_W;
    kbd_usr[27].fShiftedAim     = CHR_CROSS;
    kbd_usr[27].gShiftedAim     = ITM_NULL;
    kbd_usr[27].primaryTam     = ITM_MULT;

    kbd_usr[31].primary     = KEY_DOWN1;
    kbd_usr[31].fShifted    = KEY_SST;
    kbd_usr[31].gShifted    = ITM_FLGSV;
    kbd_usr[31].keyLblAim     = KEY_DOWN1;
    kbd_usr[31].primaryAim     = KEY_DOWN1;  //arrow
    kbd_usr[31].fShiftedAim     = KEY_SST;
    kbd_usr[31].gShiftedAim     = ITM_NULL;
    kbd_usr[31].primaryTam     = KEY_DOWN1;

    kbd_usr[32].primary     = ITM_DIV;   //ex-exit
    kbd_usr[32].fShifted    = -MNU_CATALOG;
    kbd_usr[32].gShifted    = -MNU_PRINT;
    kbd_usr[32].keyLblAim     = CHR_DIVIDE;
    kbd_usr[32].primaryAim     = CHR_SPACE;
    kbd_usr[32].fShiftedAim     = CHR_DIVIDE;
    kbd_usr[32].gShiftedAim     = CHR_PRINTER;
    kbd_usr[32].primaryTam     = ITM_DIV;

    kbd_usr[36].primary     = KEY_EXIT1;
    kbd_usr[36].fShifted    = ITM_OFF;
    kbd_usr[36].gShifted    = -MNU_IO;
    kbd_usr[36].keyLblAim     = KEY_EXIT1;
    kbd_usr[36].primaryAim     = KEY_EXIT1;  //
    kbd_usr[36].fShiftedAim     = ITM_OFF;
    kbd_usr[36].primaryTam     = KEY_EXIT1;

    Norm_Key_00_VAR         = ITM_1ONX;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;


  case USER_DM42:
    fnUserJM(USER_RESET);
    kbd_usr[0].primary=ITM_SIGMAPLUS;kbd_usr[0].fShifted=ITM_SIGMAMINUS;kbd_usr[0].gShifted=ITM_TGLFRT;kbd_usr[0].keyLblAim=ITM_NULL;kbd_usr[0].primaryAim=CHR_A;kbd_usr[0].fShiftedAim=-MNU_ALPHAINTL;kbd_usr[0].gShiftedAim=CHR_ALPHA;kbd_usr[0].primaryTam=ITM_ST_A;
    kbd_usr[1].primary=ITM_1ONX;kbd_usr[1].fShifted=ITM_YX;kbd_usr[1].gShifted=ITM_toINT;kbd_usr[1].keyLblAim=CHR_NUMBER_SIGN;kbd_usr[1].primaryAim=CHR_B;kbd_usr[1].fShiftedAim=CHR_NUMBER_SIGN;kbd_usr[1].gShiftedAim=CHR_BETA;kbd_usr[1].primaryTam=ITM_ST_B;
    kbd_usr[2].primary=ITM_SQUAREROOTX;kbd_usr[2].fShifted=ITM_SQUARE;kbd_usr[2].gShifted=ITM_ms;kbd_usr[2].keyLblAim=CHR_CHECK_MARK;kbd_usr[2].primaryAim=CHR_C;kbd_usr[2].fShiftedAim=CHR_CHECK_MARK;kbd_usr[2].gShiftedAim=CHR_CHI;kbd_usr[2].primaryTam=ITM_ST_C;
    kbd_usr[3].primary=ITM_LOG10;kbd_usr[3].fShifted=ITM_10x;kbd_usr[3].gShifted=KEY_dotD;kbd_usr[3].keyLblAim=ITM_NULL;kbd_usr[3].primaryAim=CHR_D;kbd_usr[3].fShiftedAim=ITM_NULL;kbd_usr[3].gShiftedAim=CHR_DELTA;kbd_usr[3].primaryTam=ITM_ST_D;
    kbd_usr[4].primary=ITM_LN;kbd_usr[4].fShifted=ITM_EX;kbd_usr[4].gShifted=ITM_toREC2;kbd_usr[4].keyLblAim=ITM_NULL;kbd_usr[4].primaryAim=CHR_E;kbd_usr[4].fShiftedAim=ITM_NULL;kbd_usr[4].gShiftedAim=CHR_EPSILON;kbd_usr[4].primaryTam=ITM_NULL;
    kbd_usr[5].primary=ITM_XEQ;kbd_usr[5].fShifted=ITM_GTO;kbd_usr[5].gShifted=ITM_toPOL2;kbd_usr[5].keyLblAim=ITM_NULL;kbd_usr[5].primaryAim=CHR_F;kbd_usr[5].fShiftedAim=ITM_NULL;kbd_usr[5].gShiftedAim=CHR_DIGAMMA;kbd_usr[5].primaryTam=CHR_alpha;
    kbd_usr[6].primary=ITM_STO;kbd_usr[6].fShifted=KEY_COMPLEX;kbd_usr[6].gShifted=ITM_MAGNITUDE;kbd_usr[6].keyLblAim=ITM_NULL;kbd_usr[6].primaryAim=CHR_G;kbd_usr[6].fShiftedAim=ITM_NULL;kbd_usr[6].gShiftedAim=CHR_GAMMA;kbd_usr[6].primaryTam=ITM_NULL;
    kbd_usr[7].primary=ITM_RCL;kbd_usr[7].fShifted=ITM_PC;kbd_usr[7].gShifted=ITM_DELTAPC;kbd_usr[7].keyLblAim=ITM_NULL;kbd_usr[7].primaryAim=CHR_H;kbd_usr[7].fShiftedAim=ITM_NULL;kbd_usr[7].gShiftedAim=CHR_ETA;kbd_usr[7].primaryTam=ITM_HEX;
    kbd_usr[8].primary=ITM_Rdown;kbd_usr[8].fShifted=ITM_pi;kbd_usr[8].gShifted=ITM_ANGLE;kbd_usr[8].keyLblAim=ITM_NULL;kbd_usr[8].primaryAim=CHR_I;kbd_usr[8].fShiftedAim=CHR_DOWN_ARROW;kbd_usr[8].gShiftedAim=CHR_IOTA;kbd_usr[8].primaryTam=ITM_REGI;
    kbd_usr[9].primary=ITM_sin;kbd_usr[9].fShifted=ITM_arcsin;kbd_usr[9].gShifted=ITM_XTHROOT;kbd_usr[9].keyLblAim=ITM_NULL;kbd_usr[9].primaryAim=CHR_J;kbd_usr[9].fShiftedAim=CHR_case;kbd_usr[9].gShiftedAim=CHR_THETA;kbd_usr[9].primaryTam=ITM_REGJ;
    kbd_usr[10].primary=ITM_cos;kbd_usr[10].fShifted=ITM_arccos;kbd_usr[10].gShifted=ITM_LBL;kbd_usr[10].keyLblAim=ITM_NULL;kbd_usr[10].primaryAim=CHR_K;kbd_usr[10].fShiftedAim=ITM_NULL;kbd_usr[10].gShiftedAim=CHR_KAPPA;kbd_usr[10].primaryTam=ITM_REGK;
    kbd_usr[11].primary=ITM_tan;kbd_usr[11].fShifted=ITM_arctan;kbd_usr[11].gShifted=ITM_RTN;kbd_usr[11].keyLblAim=ITM_NULL;kbd_usr[11].primaryAim=CHR_L;kbd_usr[11].fShiftedAim=ITM_NULL;kbd_usr[11].gShiftedAim=CHR_LAMBDA;kbd_usr[11].primaryTam=ITM_REGL;
    kbd_usr[12].primary=ITM_ENTER;kbd_usr[12].fShifted=ITM_AIM;kbd_usr[12].gShifted=-MNU_CPX;kbd_usr[12].keyLblAim=ITM_ENTER;kbd_usr[12].primaryAim=ITM_ENTER;kbd_usr[12].fShiftedAim=ITM_NULL;kbd_usr[12].gShiftedAim=ITM_NULL;kbd_usr[12].primaryTam=ITM_ENTER;
    kbd_usr[13].primary=ITM_XexY;kbd_usr[13].fShifted=ITM_LASTX;kbd_usr[13].gShifted=-MNU_EXP;kbd_usr[13].keyLblAim=CHR_ex;kbd_usr[13].primaryAim=CHR_M;kbd_usr[13].fShiftedAim=CHR_ex;kbd_usr[13].gShiftedAim=CHR_MU;kbd_usr[13].primaryTam=ITM_NULL;
    kbd_usr[14].primary=ITM_CHS;kbd_usr[14].fShifted=-MNU_MODE;kbd_usr[14].gShifted=KEY_CC;kbd_usr[14].keyLblAim=CHR_PLUS_MINUS;kbd_usr[14].primaryAim=CHR_N;kbd_usr[14].fShiftedAim=CHR_PLUS_MINUS;kbd_usr[14].gShiftedAim=CHR_NU;kbd_usr[14].primaryTam=ITM_NULL;
    kbd_usr[15].primary=ITM_EXPONENT;kbd_usr[15].fShifted=-MNU_DSP;kbd_usr[15].gShifted=-MNU_FIN;kbd_usr[15].keyLblAim=ITM_NULL;kbd_usr[15].primaryAim=CHR_O;kbd_usr[15].fShiftedAim=CHR_UP_ARROW;kbd_usr[15].gShiftedAim=CHR_OMICRON;kbd_usr[15].primaryTam=ITM_NULL;
    kbd_usr[16].primary=KEY_BACKSPACE;kbd_usr[16].fShifted=-MNU_CLR;kbd_usr[16].gShifted=KEY_UNDO;kbd_usr[16].keyLblAim=KEY_BACKSPACE;kbd_usr[16].primaryAim=KEY_BACKSPACE;kbd_usr[16].fShiftedAim=-MNU_CLR;kbd_usr[16].gShiftedAim=KEY_UNDO;kbd_usr[16].primaryTam=KEY_BACKSPACE;
    kbd_usr[17].primary=KEY_UP1;kbd_usr[17].fShifted=KEY_BST;kbd_usr[17].gShifted=ITM_RBR;kbd_usr[17].keyLblAim=KEY_UP1;kbd_usr[17].primaryAim=KEY_UP1;kbd_usr[17].fShiftedAim=KEY_BST;kbd_usr[17].gShiftedAim=-MNU_ALPHA;kbd_usr[17].primaryTam=KEY_UP1;
    kbd_usr[18].primary=CHR_7;kbd_usr[18].fShifted=-MNU_EQN;kbd_usr[18].gShifted=-MNU_INFO;kbd_usr[18].keyLblAim=CHR_7;kbd_usr[18].primaryAim=CHR_P;kbd_usr[18].fShiftedAim=CHR_7;kbd_usr[18].gShiftedAim=CHR_PI;kbd_usr[18].primaryTam=CHR_7;
    kbd_usr[19].primary=CHR_8;kbd_usr[19].fShifted=-MNU_ADV;kbd_usr[19].gShifted=-MNU_CONST;kbd_usr[19].keyLblAim=CHR_8;kbd_usr[19].primaryAim=CHR_Q;kbd_usr[19].fShiftedAim=CHR_8;kbd_usr[19].gShiftedAim=CHR_QOPPA;kbd_usr[19].primaryTam=CHR_8;
    kbd_usr[20].primary=CHR_9;kbd_usr[20].fShifted=-MNU_MATX;kbd_usr[20].gShifted=-MNU_XFN;kbd_usr[20].keyLblAim=CHR_9;kbd_usr[20].primaryAim=CHR_R;kbd_usr[20].fShiftedAim=CHR_9;kbd_usr[20].gShiftedAim=CHR_RHO;kbd_usr[20].primaryTam=CHR_9;
    kbd_usr[21].primary=ITM_DIV;kbd_usr[21].fShifted=-MNU_STAT;kbd_usr[21].gShifted=-MNU_SUMS;kbd_usr[21].keyLblAim=CHR_DIVIDE;kbd_usr[21].primaryAim=CHR_S;kbd_usr[21].fShiftedAim=CHR_DIVIDE;kbd_usr[21].gShiftedAim=CHR_SIGMA;kbd_usr[21].primaryTam=ITM_DIV;
    kbd_usr[22].primary=KEY_DOWN1;kbd_usr[22].fShifted=KEY_SST;kbd_usr[22].gShifted=ITM_FLGSV;kbd_usr[22].keyLblAim=KEY_DOWN1;kbd_usr[22].primaryAim=KEY_DOWN1;kbd_usr[22].fShiftedAim=KEY_SST;kbd_usr[22].gShiftedAim=CHR_case;kbd_usr[22].primaryTam=KEY_DOWN1;
    kbd_usr[23].primary=CHR_4;kbd_usr[23].fShifted=-MNU_BASE;kbd_usr[23].gShifted=-MNU_CLK;kbd_usr[23].keyLblAim=CHR_4;kbd_usr[23].primaryAim=CHR_T;kbd_usr[23].fShiftedAim=CHR_4;kbd_usr[23].gShiftedAim=CHR_TAU;kbd_usr[23].primaryTam=CHR_4;
    kbd_usr[24].primary=CHR_5;kbd_usr[24].fShifted=-MNU_ANGLECONV;kbd_usr[24].gShifted=-MNU_UNITCONV;kbd_usr[24].keyLblAim=CHR_5;kbd_usr[24].primaryAim=CHR_U;kbd_usr[24].fShiftedAim=CHR_5;kbd_usr[24].gShiftedAim=CHR_PHI;kbd_usr[24].primaryTam=CHR_5;
    kbd_usr[25].primary=CHR_6;kbd_usr[25].fShifted=-MNU_FLAGS;kbd_usr[25].gShifted=-MNU_BITS;kbd_usr[25].keyLblAim=CHR_6;kbd_usr[25].primaryAim=CHR_V;kbd_usr[25].fShiftedAim=CHR_6;kbd_usr[25].gShiftedAim=CHR_PSI;kbd_usr[25].primaryTam=CHR_6;
    kbd_usr[26].primary=ITM_MULT;kbd_usr[26].fShifted=-MNU_PROB;kbd_usr[26].gShifted=-MNU_INTS;kbd_usr[26].keyLblAim=CHR_CROSS;kbd_usr[26].primaryAim=CHR_W;kbd_usr[26].fShiftedAim=CHR_CROSS;kbd_usr[26].gShiftedAim=CHR_OMEGA;kbd_usr[26].primaryTam=ITM_MULT;
    kbd_usr[27].primary=KEY_fg;kbd_usr[27].fShifted=ITM_NULL;kbd_usr[27].gShifted=ITM_NULL;kbd_usr[27].keyLblAim=KEY_fg;kbd_usr[27].primaryAim=KEY_fg;kbd_usr[27].fShiftedAim=ITM_NULL;kbd_usr[27].gShiftedAim=ITM_NULL;kbd_usr[27].primaryTam=KEY_fg;
    kbd_usr[28].primary=CHR_1;kbd_usr[28].fShifted=ITM_ASSIGN;kbd_usr[28].gShifted=-MNU_ASN;kbd_usr[28].keyLblAim=CHR_1;kbd_usr[28].primaryAim=CHR_X;kbd_usr[28].fShiftedAim=CHR_1;kbd_usr[28].gShiftedAim=CHR_XI;kbd_usr[28].primaryTam=CHR_1;
    kbd_usr[29].primary=CHR_2;kbd_usr[29].fShifted=KEY_USERMODE;kbd_usr[29].gShifted=-MNU_LOOP;kbd_usr[29].keyLblAim=CHR_2;kbd_usr[29].primaryAim=CHR_Y;kbd_usr[29].fShiftedAim=CHR_2;kbd_usr[29].gShiftedAim=CHR_UPSILON;kbd_usr[29].primaryTam=CHR_2;
    kbd_usr[30].primary=CHR_3;kbd_usr[30].fShifted=-MNU_PFN;kbd_usr[30].gShifted=-MNU_TEST;kbd_usr[30].keyLblAim=CHR_3;kbd_usr[30].primaryAim=CHR_Z;kbd_usr[30].fShiftedAim=CHR_3;kbd_usr[30].gShiftedAim=CHR_ZETA;kbd_usr[30].primaryTam=CHR_3;
    kbd_usr[31].primary=ITM_SUB;kbd_usr[31].fShifted=-MNU_IO;kbd_usr[31].gShifted=-MNU_ALPHAFN;kbd_usr[31].keyLblAim=CHR_MINUS;kbd_usr[31].primaryAim=CHR_UNDERSCORE;kbd_usr[31].fShiftedAim=CHR_MINUS;kbd_usr[31].gShiftedAim=CHR_SAMPI;kbd_usr[31].primaryTam=ITM_SUB;
    kbd_usr[32].primary=KEY_EXIT1;kbd_usr[32].fShifted=ITM_OFF;kbd_usr[32].gShifted=-MNU_PRINT;kbd_usr[32].keyLblAim=KEY_EXIT1;kbd_usr[32].primaryAim=KEY_EXIT1;kbd_usr[32].fShiftedAim=ITM_OFF;kbd_usr[32].gShiftedAim=CHR_PRINTER;kbd_usr[32].primaryTam=KEY_EXIT1;
    kbd_usr[33].primary=CHR_0;kbd_usr[33].fShifted=-MNU_HOME;kbd_usr[33].gShifted=ITM_TIMER;kbd_usr[33].keyLblAim=CHR_0;kbd_usr[33].primaryAim=CHR_COLON;kbd_usr[33].fShiftedAim=CHR_0;kbd_usr[33].gShiftedAim=-MNU_ALPHA;kbd_usr[33].primaryTam=CHR_0;
    kbd_usr[34].primary=CHR_PERIOD;kbd_usr[34].fShifted=ITM_SHOW;kbd_usr[34].gShifted=ITM_VIEW;kbd_usr[34].keyLblAim=CHR_PERIOD;kbd_usr[34].primaryAim=CHR_COMMA;kbd_usr[34].fShiftedAim=CHR_PERIOD;kbd_usr[34].gShiftedAim=-MNU_ALPHADOT;kbd_usr[34].primaryTam=CHR_PERIOD;
    kbd_usr[35].primary=ITM_RS;kbd_usr[35].fShifted=ITM_PR;kbd_usr[35].gShifted=-MNU_STK;kbd_usr[35].keyLblAim=ITM_NULL;kbd_usr[35].primaryAim=CHR_QUESTION_MARK;kbd_usr[35].fShiftedAim=CHR_SLASH;kbd_usr[35].gShiftedAim=-MNU_ALPHAMATH;kbd_usr[35].primaryTam=ITM_NULL;
    kbd_usr[36].primary=ITM_ADD;kbd_usr[36].fShifted=-MNU_CATALOG;kbd_usr[36].gShifted=-MNU_PARTS;kbd_usr[36].keyLblAim=CHR_PLUS;kbd_usr[36].primaryAim=CHR_SPACE;kbd_usr[36].fShiftedAim=CHR_PLUS;kbd_usr[36].gShiftedAim=-MNU_ALPHAINTL;kbd_usr[36].primaryTam=ITM_ADD;
    fnSetFlag(FLAG_USER);
    break;



  case USER_43S:          //USER
    fnUserJM(USER_RESET);
    kbd_usr[0].primary=ITM_1ONX;  kbd_usr[0].fShifted=ITM_TGLFRT;   kbd_usr[0].gShifted=-MNU_ALPHAFN;   kbd_usr[0].keyLblAim=ITM_NULL;  kbd_usr[0].primaryAim=CHR_A;  kbd_usr[0].fShiftedAim=-MNU_ALPHAINTL;  kbd_usr[0].gShiftedAim=CHR_ALPHA;   kbd_usr[0].primaryTam=ITM_ST_A; 
    kbd_usr[1].primary=ITM_YX;  kbd_usr[1].fShifted=ITM_toINT;  kbd_usr[1].gShifted=-MNU_EXP;   kbd_usr[1].keyLblAim=CHR_NUMBER_SIGN;   kbd_usr[1].primaryAim=CHR_B;  kbd_usr[1].fShiftedAim=CHR_NUMBER_SIGN;   kbd_usr[1].gShiftedAim=CHR_BETA;  kbd_usr[1].primaryTam=ITM_ST_B; 
    kbd_usr[2].primary=-MNU_TRI;  kbd_usr[2].fShifted=ITM_DMS;  kbd_usr[2].gShifted=ITM_pi;   kbd_usr[2].keyLblAim=ITM_NULL;  kbd_usr[2].primaryAim=CHR_C;  kbd_usr[2].fShiftedAim=ITM_NULL;  kbd_usr[2].gShiftedAim=CHR_GAMMA;   kbd_usr[2].primaryTam=ITM_ST_C; 
    kbd_usr[3].primary=ITM_LN;  kbd_usr[3].fShifted=KEY_dotD;   kbd_usr[3].gShifted=ITM_LOG10;  kbd_usr[3].keyLblAim=ITM_NULL;  kbd_usr[3].primaryAim=CHR_D;  kbd_usr[3].fShiftedAim=ITM_NULL;  kbd_usr[3].gShiftedAim=CHR_DELTA;   kbd_usr[3].primaryTam=ITM_ST_D; 
    kbd_usr[4].primary=ITM_EX;  kbd_usr[4].fShifted=ITM_toHMS;  kbd_usr[4].gShifted=ITM_10x;  kbd_usr[4].keyLblAim=ITM_NULL;  kbd_usr[4].primaryAim=CHR_E;  kbd_usr[4].fShiftedAim=ITM_NULL;  kbd_usr[4].gShiftedAim=CHR_EPSILON;   kbd_usr[4].primaryTam=ITM_NULL; 
    kbd_usr[5].primary=ITM_SQUARE;  kbd_usr[5].fShifted=ITM_AIM;  kbd_usr[5].gShifted=ITM_SQUAREROOTX;  kbd_usr[5].keyLblAim=CHR_CHECK_MARK;  kbd_usr[5].primaryAim=CHR_F;  kbd_usr[5].fShiftedAim=CHR_CHECK_MARK;  kbd_usr[5].gShiftedAim=CHR_PHI;   kbd_usr[5].primaryTam=CHR_alpha; 
    kbd_usr[6].primary=ITM_STO;   kbd_usr[6].fShifted=ITM_ASSIGN;   kbd_usr[6].gShifted=ITM_SAVE;   kbd_usr[6].keyLblAim=ITM_NULL;  kbd_usr[6].primaryAim=CHR_G;  kbd_usr[6].fShiftedAim=ITM_ASSIGN;  kbd_usr[6].gShiftedAim=CHR_GAMMA;   kbd_usr[6].primaryTam=ITM_NULL; 
    kbd_usr[7].primary=ITM_RCL;   kbd_usr[7].fShifted=ITM_RBR;  kbd_usr[7].gShifted=ITM_VIEW;   kbd_usr[7].keyLblAim=ITM_NULL;  kbd_usr[7].primaryAim=CHR_H;  kbd_usr[7].fShiftedAim=ITM_RBR;   kbd_usr[7].gShiftedAim=CHR_CHI;   kbd_usr[7].primaryTam=ITM_HEX; 
    kbd_usr[8].primary=ITM_Rdown;   kbd_usr[8].fShifted=ITM_Rup;  kbd_usr[8].gShifted=-MNU_CPX;   kbd_usr[8].keyLblAim=ITM_NULL;  kbd_usr[8].primaryAim=CHR_I;  kbd_usr[8].fShiftedAim=CHR_DOWN_ARROW;  kbd_usr[8].gShiftedAim=CHR_IOTA;  kbd_usr[8].primaryTam=ITM_REGI; 
    kbd_usr[9].primary=KEY_CC;  kbd_usr[9].fShifted=ITM_MAGNITUDE;  kbd_usr[9].gShifted=ITM_ANGLE;  kbd_usr[9].keyLblAim=ITM_NULL;  kbd_usr[9].primaryAim=CHR_J;  kbd_usr[9].fShiftedAim=ITM_NULL;  kbd_usr[9].gShiftedAim=CHR_ETA;   kbd_usr[9].primaryTam=ITM_REGJ; 
    kbd_usr[10].primary=KEY_f;  kbd_usr[10].fShifted=ITM_NULL;  kbd_usr[10].gShifted=ITM_NULL;  kbd_usr[10].keyLblAim=KEY_f;  kbd_usr[10].primaryAim=KEY_f;   kbd_usr[10].fShiftedAim=ITM_NULL;   kbd_usr[10].gShiftedAim=ITM_SCRDMP;   kbd_usr[10].primaryTam=KEY_f; 
    kbd_usr[11].primary=KEY_g;  kbd_usr[11].fShifted=ITM_NULL;  kbd_usr[11].gShifted=ITM_NULL;  kbd_usr[11].keyLblAim=KEY_g;  kbd_usr[11].primaryAim=KEY_g;   kbd_usr[11].fShiftedAim=KEY_USERMODE;   kbd_usr[11].gShiftedAim=ITM_NULL;   kbd_usr[11].primaryTam=KEY_g; 
    kbd_usr[12].primary=ITM_ENTER;  kbd_usr[12].fShifted=ITM_STATUS;  kbd_usr[12].gShifted=ITM_DROP;  kbd_usr[12].keyLblAim=ITM_ENTER;  kbd_usr[12].primaryAim=ITM_ENTER;   kbd_usr[12].fShiftedAim=ITM_STATUS;   kbd_usr[12].gShiftedAim=ITM_DROP;   kbd_usr[12].primaryTam=ITM_ENTER; 
    kbd_usr[13].primary=ITM_XexY;   kbd_usr[13].fShifted=ITM_FILL;  kbd_usr[13].gShifted=-MNU_STK;  kbd_usr[13].keyLblAim=CHR_ex;   kbd_usr[13].primaryAim=CHR_K;   kbd_usr[13].fShiftedAim=CHR_ex;   kbd_usr[13].gShiftedAim=CHR_KAPPA;  kbd_usr[13].primaryTam=ITM_REGK; 
    kbd_usr[14].primary=ITM_CHS;  kbd_usr[14].fShifted=ITM_DELTAPC;   kbd_usr[14].gShifted=-MNU_FIN;  kbd_usr[14].keyLblAim=CHR_PLUS_MINUS;   kbd_usr[14].primaryAim=CHR_L;   kbd_usr[14].fShiftedAim=CHR_PLUS_MINUS;   kbd_usr[14].gShiftedAim=CHR_LAMBDA;   kbd_usr[14].primaryTam=ITM_REGL; 
    kbd_usr[15].primary=ITM_EXPONENT;   kbd_usr[15].fShifted=ITM_SHOW;  kbd_usr[15].gShifted=-MNU_DSP;  kbd_usr[15].keyLblAim=ITM_NULL;   kbd_usr[15].primaryAim=CHR_M;   kbd_usr[15].fShiftedAim=CHR_UP_ARROW;   kbd_usr[15].gShiftedAim=CHR_MU;   kbd_usr[15].primaryTam=ITM_NULL; 
    kbd_usr[16].primary=KEY_BACKSPACE;  kbd_usr[16].fShifted= KEY_UNDO;   kbd_usr[16].gShifted=-MNU_CLR;  kbd_usr[16].keyLblAim=KEY_BACKSPACE;  kbd_usr[16].primaryAim=KEY_BACKSPACE;   kbd_usr[16].fShiftedAim=KEY_UNDO;   kbd_usr[16].gShiftedAim=-MNU_CLR;   kbd_usr[16].primaryTam=KEY_BACKSPACE; 
    kbd_usr[17].primary=ITM_DIV;  kbd_usr[17].fShifted=ITM_RMD;   kbd_usr[17].gShifted=ITM_MOD;   kbd_usr[17].keyLblAim=CHR_SLASH;  kbd_usr[17].primaryAim=CHR_N;   kbd_usr[17].fShiftedAim=CHR_SLASH;  kbd_usr[17].gShiftedAim=CHR_NU;   kbd_usr[17].primaryTam=ITM_DIV; 
    kbd_usr[18].primary=CHR_7;  kbd_usr[18].fShifted=-MNU_CONST;  kbd_usr[18].gShifted=-MNU_ASN;  kbd_usr[18].keyLblAim=CHR_7;  kbd_usr[18].primaryAim=CHR_O;   kbd_usr[18].fShiftedAim=CHR_7;  kbd_usr[18].gShiftedAim=CHR_OMEGA;  kbd_usr[18].primaryTam=CHR_7; 
    kbd_usr[19].primary=CHR_8;  kbd_usr[19].fShifted=KEY_USERMODE;  kbd_usr[19].gShifted=ITM_SCRDMP;  kbd_usr[19].keyLblAim=CHR_8;  kbd_usr[19].primaryAim=CHR_P;   kbd_usr[19].fShiftedAim=CHR_8;  kbd_usr[19].gShiftedAim=CHR_PI;   kbd_usr[19].primaryTam=CHR_8; 
    kbd_usr[20].primary=CHR_9;  kbd_usr[20].fShifted=-MNU_HOME;   kbd_usr[20].gShifted=ITM_RTN;   kbd_usr[20].keyLblAim=CHR_9;  kbd_usr[20].primaryAim=CHR_Q;   kbd_usr[20].fShiftedAim=CHR_9;  kbd_usr[20].gShiftedAim=ITM_RTN;  kbd_usr[20].primaryTam=CHR_9; 
    kbd_usr[21].primary=ITM_XEQ;  kbd_usr[21].fShifted=ITM_GTO;   kbd_usr[21].gShifted=ITM_LBL;   kbd_usr[21].keyLblAim=ITM_NULL;   kbd_usr[21].primaryAim=ITM_NULL;  kbd_usr[21].fShiftedAim=ITM_NULL;   kbd_usr[21].gShiftedAim=ITM_NULL;   kbd_usr[21].primaryTam=ITM_NULL; 
    kbd_usr[22].primary=ITM_MULT;   kbd_usr[22].fShifted=ITM_XFACT;   kbd_usr[22].gShifted=-MNU_PROB;   kbd_usr[22].keyLblAim=CHR_CROSS;  kbd_usr[22].primaryAim=CHR_R;   kbd_usr[22].fShiftedAim=CHR_PROD_SIGN;  kbd_usr[22].gShiftedAim=CHR_RHO;  kbd_usr[22].primaryTam=ITM_MULT; 
    kbd_usr[23].primary=CHR_4;  kbd_usr[23].fShifted=-MNU_STAT;   kbd_usr[23].gShifted=-MNU_SUMS;   kbd_usr[23].keyLblAim=CHR_4;  kbd_usr[23].primaryAim=CHR_S;   kbd_usr[23].fShiftedAim=CHR_4;  kbd_usr[23].gShiftedAim=CHR_SIGMA;  kbd_usr[23].primaryTam=CHR_4; 
    kbd_usr[24].primary=CHR_5;  kbd_usr[24].fShifted=ITM_toREC;   kbd_usr[24].gShifted=ITM_toPOL;   kbd_usr[24].keyLblAim=CHR_5;  kbd_usr[24].primaryAim=CHR_T;   kbd_usr[24].fShiftedAim=CHR_5;  kbd_usr[24].gShiftedAim=CHR_TAU;  kbd_usr[24].primaryTam=CHR_5; 
    kbd_usr[25].primary=CHR_6;  kbd_usr[25].fShifted=-MNU_UNITCONV;   kbd_usr[25].gShifted=-MNU_ANGLECONV;  kbd_usr[25].keyLblAim=CHR_6;  kbd_usr[25].primaryAim=CHR_U;   kbd_usr[25].fShiftedAim=CHR_6;  kbd_usr[25].gShiftedAim=CHR_THETA;  kbd_usr[25].primaryTam=CHR_6; 
    kbd_usr[26].primary=KEY_UP1;  kbd_usr[26].fShifted=KEY_BST;   kbd_usr[26].gShifted=-MNU_FLAGS;  kbd_usr[26].keyLblAim=KEY_UP1;  kbd_usr[26].primaryAim=KEY_UP1;   kbd_usr[26].fShiftedAim=KEY_BST;  kbd_usr[26].gShiftedAim=-MNU_FLAGS;   kbd_usr[26].primaryTam=KEY_UP1; 
    kbd_usr[27].primary=ITM_SUB;  kbd_usr[27].fShifted=-MNU_BITS;   kbd_usr[27].gShifted=-MNU_INTS;   kbd_usr[27].keyLblAim=CHR_MINUS;  kbd_usr[27].primaryAim=CHR_V;   kbd_usr[27].fShiftedAim=CHR_MINUS;  kbd_usr[27].gShiftedAim=-MNU_ALPHAMATH;   kbd_usr[27].primaryTam=ITM_SUB; 
    kbd_usr[28].primary=CHR_1;  kbd_usr[28].fShifted=-MNU_ADV;  kbd_usr[28].gShifted=-MNU_EQN;  kbd_usr[28].keyLblAim=CHR_1;  kbd_usr[28].primaryAim=CHR_W;   kbd_usr[28].fShiftedAim=CHR_1;  kbd_usr[28].gShiftedAim=CHR_PSI;  kbd_usr[28].primaryTam=CHR_1; 
    kbd_usr[29].primary=CHR_2;  kbd_usr[29].fShifted=-MNU_MATX;   kbd_usr[29].gShifted=-MNU_XFN;  kbd_usr[29].keyLblAim=CHR_2;  kbd_usr[29].primaryAim=CHR_X;   kbd_usr[29].fShiftedAim=CHR_2;  kbd_usr[29].gShiftedAim=CHR_XI;   kbd_usr[29].primaryTam=CHR_2; 
    kbd_usr[30].primary=CHR_3;  kbd_usr[30].fShifted=ITM_TIMER;   kbd_usr[30].gShifted=-MNU_CLK;  kbd_usr[30].keyLblAim=CHR_3;  kbd_usr[30].primaryAim=CHR_Y;   kbd_usr[30].fShiftedAim=CHR_3;  kbd_usr[30].gShiftedAim=CHR_UPSILON;  kbd_usr[30].primaryTam=CHR_3; 
    kbd_usr[31].primary=KEY_DOWN1;  kbd_usr[31].fShifted=KEY_SST;   kbd_usr[31].gShifted=-MNU_MODE;   kbd_usr[31].keyLblAim=KEY_DOWN1;  kbd_usr[31].primaryAim=KEY_DOWN1;   kbd_usr[31].fShiftedAim=KEY_SST;  kbd_usr[31].gShiftedAim=-MNU_MODE;  kbd_usr[31].primaryTam=KEY_DOWN1; 
    kbd_usr[32].primary=ITM_ADD;  kbd_usr[32].fShifted=-MNU_IO;   kbd_usr[32].gShifted=-MNU_PRINT;  kbd_usr[32].keyLblAim=CHR_PLUS;   kbd_usr[32].primaryAim=CHR_Z;   kbd_usr[32].fShiftedAim=CHR_PLUS;   kbd_usr[32].gShiftedAim=CHR_ZETA;   kbd_usr[32].primaryTam=ITM_ADD; 
    kbd_usr[33].primary=CHR_0;  kbd_usr[33].fShifted=-MNU_LOOP;   kbd_usr[33].gShifted=-MNU_TEST;   kbd_usr[33].keyLblAim=CHR_0;  kbd_usr[33].primaryAim=ITM_NULL;  kbd_usr[33].fShiftedAim=CHR_0;  kbd_usr[33].gShiftedAim=CHR_PRINTER;  kbd_usr[33].primaryTam=CHR_0; 
    kbd_usr[34].primary=CHR_PERIOD;   kbd_usr[34].fShifted=-MNU_PARTS;  kbd_usr[34].gShifted=-MNU_INFO;   kbd_usr[34].keyLblAim=CHR_PERIOD;   kbd_usr[34].primaryAim=CHR_COMMA;   kbd_usr[34].fShiftedAim=CHR_PERIOD;   kbd_usr[34].gShiftedAim=-MNU_ALPHADOT;  kbd_usr[34].primaryTam=CHR_PERIOD; 
    kbd_usr[35].primary=ITM_RS;   kbd_usr[35].fShifted=ITM_PR;  kbd_usr[35].gShifted=-MNU_PFN;  kbd_usr[35].keyLblAim=ITM_NULL;   kbd_usr[35].primaryAim=CHR_SPACE;   kbd_usr[35].fShiftedAim=ITM_NULL;   kbd_usr[35].gShiftedAim=ITM_NULL;   kbd_usr[35].primaryTam=ITM_NULL; 
    kbd_usr[36].primary=KEY_EXIT1;  kbd_usr[36].fShifted=-MNU_CATALOG;  kbd_usr[36].gShifted=ITM_OFF;   kbd_usr[36].keyLblAim=KEY_EXIT1;  kbd_usr[36].primaryAim=KEY_EXIT1;   kbd_usr[36].fShiftedAim=-MNU_CATALOG;   kbd_usr[36].gShiftedAim=ITM_OFF;  kbd_usr[36].primaryTam=KEY_EXIT1; 
    fnSetFlag(FLAG_USER);
    break;


  case USER_C43:          //USER
    fnUserJM(USER_RESET);
    kbd_usr[0].primary=ITM_SIGMAPLUS;   kbd_usr[0].fShifted=ITM_RI;   kbd_usr[0].gShifted=ITM_TGLFRT;   kbd_usr[0].keyLblAim=ITM_NULL;  kbd_usr[0].primaryAim=CHR_A;  kbd_usr[0].fShiftedAim=-MNU_ALPHAINTL;  kbd_usr[0].gShiftedAim=CHR_ALPHA;   kbd_usr[0].primaryTam=ITM_ST_A; 
    kbd_usr[1].primary=ITM_1ONX;  kbd_usr[1].fShifted=ITM_YX;   kbd_usr[1].gShifted=ITM_toINT;  kbd_usr[1].keyLblAim=CHR_NUMBER_SIGN;   kbd_usr[1].primaryAim=CHR_B;  kbd_usr[1].fShiftedAim=CHR_NUMBER_SIGN;   kbd_usr[1].gShiftedAim=CHR_BETA;  kbd_usr[1].primaryTam=ITM_ST_B; 
    kbd_usr[2].primary=ITM_SQUAREROOTX;   kbd_usr[2].fShifted=ITM_SQUARE;   kbd_usr[2].gShifted=ITM_ms;   kbd_usr[2].keyLblAim=CHR_CHECK_MARK;  kbd_usr[2].primaryAim=CHR_C;  kbd_usr[2].fShiftedAim=CHR_CHECK_MARK;  kbd_usr[2].gShiftedAim=CHR_CHI;   kbd_usr[2].primaryTam=ITM_ST_C; 
    kbd_usr[3].primary=ITM_LOG10;   kbd_usr[3].fShifted=ITM_10x;  kbd_usr[3].gShifted=KEY_dotD;   kbd_usr[3].keyLblAim=ITM_NULL;  kbd_usr[3].primaryAim=CHR_D;  kbd_usr[3].fShiftedAim=ITM_NULL;  kbd_usr[3].gShiftedAim=CHR_DELTA;   kbd_usr[3].primaryTam=ITM_ST_D; 
    kbd_usr[4].primary=ITM_LN;  kbd_usr[4].fShifted=ITM_EX;   kbd_usr[4].gShifted=ITM_toREC2;   kbd_usr[4].keyLblAim=ITM_NULL;  kbd_usr[4].primaryAim=CHR_E;  kbd_usr[4].fShiftedAim=ITM_NULL;  kbd_usr[4].gShiftedAim=CHR_EPSILON;   kbd_usr[4].primaryTam=ITM_NULL; 
    kbd_usr[5].primary=ITM_XEQ;   kbd_usr[5].fShifted=ITM_XTHROOT;  kbd_usr[5].gShifted=ITM_toPOL2;   kbd_usr[5].keyLblAim=ITM_NULL;  kbd_usr[5].primaryAim=CHR_F;  kbd_usr[5].fShiftedAim=ITM_NULL;  kbd_usr[5].gShiftedAim=CHR_DIGAMMA;   kbd_usr[5].primaryTam=CHR_alpha; 
    kbd_usr[6].primary=ITM_STO;   kbd_usr[6].fShifted=ITM_MAGNITUDE;  kbd_usr[6].gShifted=ITM_ANGLE;  kbd_usr[6].keyLblAim=ITM_NULL;  kbd_usr[6].primaryAim=CHR_G;  kbd_usr[6].fShiftedAim=ITM_NULL;  kbd_usr[6].gShiftedAim=CHR_GAMMA;   kbd_usr[6].primaryTam=ITM_NULL; 
    kbd_usr[7].primary=ITM_RCL;   kbd_usr[7].fShifted=ITM_PC;   kbd_usr[7].gShifted=ITM_DELTAPC;  kbd_usr[7].keyLblAim=ITM_NULL;  kbd_usr[7].primaryAim=CHR_H;  kbd_usr[7].fShiftedAim=ITM_NULL;  kbd_usr[7].gShiftedAim=CHR_ETA;   kbd_usr[7].primaryTam=ITM_HEX; 
    kbd_usr[8].primary=ITM_Rdown;   kbd_usr[8].fShifted=ITM_pi;   kbd_usr[8].gShifted=ITM_Rup;  kbd_usr[8].keyLblAim=ITM_NULL;  kbd_usr[8].primaryAim=CHR_I;  kbd_usr[8].fShiftedAim=CHR_DOWN_ARROW;  kbd_usr[8].gShiftedAim=CHR_IOTA;  kbd_usr[8].primaryTam=ITM_REGI; 
    kbd_usr[9].primary=ITM_sin;   kbd_usr[9].fShifted=ITM_arcsin;   kbd_usr[9].gShifted=ITM_GTO;  kbd_usr[9].keyLblAim=ITM_NULL;  kbd_usr[9].primaryAim=CHR_J;  kbd_usr[9].fShiftedAim=CHR_case;  kbd_usr[9].gShiftedAim=CHR_THETA;   kbd_usr[9].primaryTam=ITM_REGJ; 
    kbd_usr[10].primary=ITM_cos;  kbd_usr[10].fShifted=ITM_arccos;  kbd_usr[10].gShifted=ITM_LBL;   kbd_usr[10].keyLblAim=ITM_NULL;   kbd_usr[10].primaryAim=CHR_K;   kbd_usr[10].fShiftedAim=ITM_NULL;   kbd_usr[10].gShiftedAim=CHR_KAPPA;  kbd_usr[10].primaryTam=ITM_REGK; 
    kbd_usr[11].primary=ITM_tan;  kbd_usr[11].fShifted=ITM_arctan;  kbd_usr[11].gShifted=ITM_RTN;   kbd_usr[11].keyLblAim=ITM_NULL;   kbd_usr[11].primaryAim=CHR_L;   kbd_usr[11].fShiftedAim=ITM_NULL;   kbd_usr[11].gShiftedAim=CHR_LAMBDA;   kbd_usr[11].primaryTam=ITM_REGL; 
    kbd_usr[12].primary=ITM_ENTER;  kbd_usr[12].fShifted=KEY_COMPLEX;   kbd_usr[12].gShifted=-MNU_CPX;  kbd_usr[12].keyLblAim=ITM_ENTER;  kbd_usr[12].primaryAim=ITM_ENTER;   kbd_usr[12].fShiftedAim=ITM_NULL;   kbd_usr[12].gShiftedAim=ITM_NULL;   kbd_usr[12].primaryTam=ITM_ENTER; 
    kbd_usr[13].primary=ITM_XexY;   kbd_usr[13].fShifted=ITM_LASTX;   kbd_usr[13].gShifted=-MNU_EXP;  kbd_usr[13].keyLblAim=CHR_ex;   kbd_usr[13].primaryAim=CHR_M;   kbd_usr[13].fShiftedAim=CHR_ex;   kbd_usr[13].gShiftedAim=CHR_MU;   kbd_usr[13].primaryTam=ITM_NULL; 
    kbd_usr[14].primary=ITM_CHS;  kbd_usr[14].fShifted=-MNU_MODE;   kbd_usr[14].gShifted=-MNU_BASE;   kbd_usr[14].keyLblAim=CHR_PLUS_MINUS;   kbd_usr[14].primaryAim=CHR_N;   kbd_usr[14].fShiftedAim=CHR_PLUS_MINUS;   kbd_usr[14].gShiftedAim=CHR_NU;   kbd_usr[14].primaryTam=ITM_NULL; 
    kbd_usr[15].primary=ITM_EXPONENT;   kbd_usr[15].fShifted=-MNU_DSP;  kbd_usr[15].gShifted=-MNU_FIN;  kbd_usr[15].keyLblAim=ITM_NULL;   kbd_usr[15].primaryAim=CHR_O;   kbd_usr[15].fShiftedAim=CHR_UP_ARROW;   kbd_usr[15].gShiftedAim=CHR_OMICRON;  kbd_usr[15].primaryTam=ITM_NULL; 
    kbd_usr[16].primary=KEY_BACKSPACE;  kbd_usr[16].fShifted=KEY_UNDO;  kbd_usr[16].gShifted=-MNU_CLR;  kbd_usr[16].keyLblAim=KEY_BACKSPACE;  kbd_usr[16].primaryAim=KEY_BACKSPACE;   kbd_usr[16].fShiftedAim=KEY_UNDO;   kbd_usr[16].gShiftedAim=-MNU_CLR;   kbd_usr[16].primaryTam=KEY_BACKSPACE; 
    kbd_usr[17].primary=KEY_UP1;  kbd_usr[17].fShifted=KEY_BST;   kbd_usr[17].gShifted=ITM_RBR;   kbd_usr[17].keyLblAim=KEY_UP1;  kbd_usr[17].primaryAim=KEY_UP1;   kbd_usr[17].fShiftedAim=KEY_BST;  kbd_usr[17].gShiftedAim=-MNU_ALPHA;   kbd_usr[17].primaryTam=KEY_UP1; 
    kbd_usr[18].primary=CHR_7;  kbd_usr[18].fShifted=-MNU_EQN;  kbd_usr[18].gShifted=-MNU_HOME;   kbd_usr[18].keyLblAim=CHR_7;  kbd_usr[18].primaryAim=CHR_P;   kbd_usr[18].fShiftedAim=CHR_7;  kbd_usr[18].gShiftedAim=CHR_PI;   kbd_usr[18].primaryTam=CHR_7; 
    kbd_usr[19].primary=CHR_8;  kbd_usr[19].fShifted=-MNU_ADV;  kbd_usr[19].gShifted=-MNU_CONST;  kbd_usr[19].keyLblAim=CHR_8;  kbd_usr[19].primaryAim=CHR_Q;   kbd_usr[19].fShiftedAim=CHR_8;  kbd_usr[19].gShiftedAim=CHR_QOPPA;  kbd_usr[19].primaryTam=CHR_8; 
    kbd_usr[20].primary=CHR_9;  kbd_usr[20].fShifted=-MNU_MATX;   kbd_usr[20].gShifted=-MNU_XFN;  kbd_usr[20].keyLblAim=CHR_9;  kbd_usr[20].primaryAim=CHR_R;   kbd_usr[20].fShiftedAim=CHR_9;  kbd_usr[20].gShiftedAim=CHR_RHO;  kbd_usr[20].primaryTam=CHR_9; 
    kbd_usr[21].primary=ITM_DIV;  kbd_usr[21].fShifted=-MNU_STAT;   kbd_usr[21].gShifted=-MNU_SUMS;   kbd_usr[21].keyLblAim=CHR_DIVIDE;   kbd_usr[21].primaryAim=CHR_S;   kbd_usr[21].fShiftedAim=CHR_DIVIDE;   kbd_usr[21].gShiftedAim=CHR_SIGMA;  kbd_usr[21].primaryTam=ITM_DIV; 
    kbd_usr[22].primary=KEY_DOWN1;  kbd_usr[22].fShifted=KEY_SST;   kbd_usr[22].gShifted=ITM_FLGSV;   kbd_usr[22].keyLblAim=KEY_DOWN1;  kbd_usr[22].primaryAim=KEY_DOWN1;   kbd_usr[22].fShiftedAim=KEY_SST;  kbd_usr[22].gShiftedAim=CHR_case;   kbd_usr[22].primaryTam=KEY_DOWN1; 
    kbd_usr[23].primary=CHR_4;  kbd_usr[23].fShifted=ITM_AIM;   kbd_usr[23].gShifted=-MNU_CLK;  kbd_usr[23].keyLblAim=CHR_4;  kbd_usr[23].primaryAim=CHR_T;   kbd_usr[23].fShiftedAim=CHR_4;  kbd_usr[23].gShiftedAim=CHR_TAU;  kbd_usr[23].primaryTam=CHR_4; 
    kbd_usr[24].primary=CHR_5;  kbd_usr[24].fShifted=-MNU_ANGLECONV;  kbd_usr[24].gShifted=-MNU_UNITCONV;   kbd_usr[24].keyLblAim=CHR_5;  kbd_usr[24].primaryAim=CHR_U;   kbd_usr[24].fShiftedAim=CHR_5;  kbd_usr[24].gShiftedAim=CHR_PHI;  kbd_usr[24].primaryTam=CHR_5; 
    kbd_usr[25].primary=CHR_6;  kbd_usr[25].fShifted=-MNU_FLAGS;  kbd_usr[25].gShifted=-MNU_BITS;   kbd_usr[25].keyLblAim=CHR_6;  kbd_usr[25].primaryAim=CHR_V;   kbd_usr[25].fShiftedAim=CHR_6;  kbd_usr[25].gShiftedAim=CHR_PSI;  kbd_usr[25].primaryTam=CHR_6; 
    kbd_usr[26].primary=ITM_MULT;   kbd_usr[26].fShifted=-MNU_PROB;   kbd_usr[26].gShifted=-MNU_INTS;   kbd_usr[26].keyLblAim=CHR_CROSS;  kbd_usr[26].primaryAim=CHR_W;   kbd_usr[26].fShiftedAim=CHR_CROSS;  kbd_usr[26].gShiftedAim=CHR_OMEGA;  kbd_usr[26].primaryTam=ITM_MULT; 
    kbd_usr[27].primary=KEY_fg;   kbd_usr[27].fShifted=ITM_NULL;  kbd_usr[27].gShifted=ITM_NULL;  kbd_usr[27].keyLblAim=KEY_fg;   kbd_usr[27].primaryAim=KEY_fg;  kbd_usr[27].fShiftedAim=ITM_NULL;   kbd_usr[27].gShiftedAim=ITM_NULL;   kbd_usr[27].primaryTam=KEY_fg; 
    kbd_usr[28].primary=CHR_1;  kbd_usr[28].fShifted=ITM_ASSIGN;  kbd_usr[28].gShifted=-MNU_ASN;  kbd_usr[28].keyLblAim=CHR_1;  kbd_usr[28].primaryAim=CHR_X;   kbd_usr[28].fShiftedAim=CHR_1;  kbd_usr[28].gShiftedAim=CHR_XI;   kbd_usr[28].primaryTam=CHR_1; 
    kbd_usr[29].primary=CHR_2;  kbd_usr[29].fShifted=KEY_USERMODE;  kbd_usr[29].gShifted=-MNU_LOOP;   kbd_usr[29].keyLblAim=CHR_2;  kbd_usr[29].primaryAim=CHR_Y;   kbd_usr[29].fShiftedAim=CHR_2;  kbd_usr[29].gShiftedAim=CHR_UPSILON;  kbd_usr[29].primaryTam=CHR_2; 
    kbd_usr[30].primary=CHR_3;  kbd_usr[30].fShifted=-MNU_PARTS;  kbd_usr[30].gShifted=-MNU_TEST;   kbd_usr[30].keyLblAim=CHR_3;  kbd_usr[30].primaryAim=CHR_Z;   kbd_usr[30].fShiftedAim=CHR_3;  kbd_usr[30].gShiftedAim=CHR_ZETA;   kbd_usr[30].primaryTam=CHR_3; 
    kbd_usr[31].primary=ITM_SUB;  kbd_usr[31].fShifted=-MNU_STK;  kbd_usr[31].gShifted=-MNU_ALPHAFN;  kbd_usr[31].keyLblAim=CHR_MINUS;  kbd_usr[31].primaryAim=CHR_UNDERSCORE;  kbd_usr[31].fShiftedAim=CHR_MINUS;  kbd_usr[31].gShiftedAim=CHR_SAMPI;  kbd_usr[31].primaryTam=ITM_SUB; 
    kbd_usr[32].primary=KEY_EXIT1;  kbd_usr[32].fShifted=ITM_OFF;   kbd_usr[32].gShifted=-MNU_PRINT;  kbd_usr[32].keyLblAim=KEY_EXIT1;  kbd_usr[32].primaryAim=KEY_EXIT1;   kbd_usr[32].fShiftedAim=ITM_OFF;  kbd_usr[32].gShiftedAim=CHR_PRINTER;  kbd_usr[32].primaryTam=KEY_EXIT1; 
    kbd_usr[33].primary=CHR_0;  kbd_usr[33].fShifted=ITM_VIEW;  kbd_usr[33].gShifted=ITM_TIMER;   kbd_usr[33].keyLblAim=CHR_0;  kbd_usr[33].primaryAim=CHR_COLON;   kbd_usr[33].fShiftedAim=CHR_0;  kbd_usr[33].gShiftedAim=-MNU_ALPHA;   kbd_usr[33].primaryTam=CHR_0; 
    kbd_usr[34].primary=CHR_PERIOD;   kbd_usr[34].fShifted=ITM_SHOW;  kbd_usr[34].gShifted=-MNU_INFO;   kbd_usr[34].keyLblAim=CHR_PERIOD;   kbd_usr[34].primaryAim=CHR_COMMA;   kbd_usr[34].fShiftedAim=CHR_PERIOD;   kbd_usr[34].gShiftedAim=-MNU_ALPHADOT;  kbd_usr[34].primaryTam=CHR_PERIOD; 
    kbd_usr[35].primary=ITM_RS;   kbd_usr[35].fShifted=ITM_PR;  kbd_usr[35].gShifted=-MNU_PFN;  kbd_usr[35].keyLblAim=ITM_NULL;   kbd_usr[35].primaryAim=CHR_QUESTION_MARK;   kbd_usr[35].fShiftedAim=CHR_SLASH;  kbd_usr[35].gShiftedAim=-MNU_ALPHAMATH;   kbd_usr[35].primaryTam=ITM_NULL; 
    kbd_usr[36].primary=ITM_ADD;  kbd_usr[36].fShifted=-MNU_CATALOG;  kbd_usr[36].gShifted=-MNU_IO;   kbd_usr[36].keyLblAim=CHR_PLUS;   kbd_usr[36].primaryAim=CHR_SPACE;   kbd_usr[36].fShiftedAim=CHR_PLUS;   kbd_usr[36].gShiftedAim=-MNU_ALPHAINTL;   kbd_usr[36].primaryTam=ITM_ADD; 
    fnSetFlag(FLAG_USER);
    break;



  case USER_SHIFTS:                                             //USER_SHIFTS 25          //JM Sectioon to be put on a menu
  
    fnUserJM(USER_C43);
  
    kbd_usr[0].primary     = KEY_USERMODE;
    kbd_usr[9].primary     = -MNU_TRI;
    kbd_usr[9].fShifted    = KEY_USERMODE;
       //kbd_usr[9].gShifted    = ITM_RTN;
       //kbd_usr[9].fShifted    = ITM_NULL;
    kbd_usr[10].primary    = KEY_f;
    kbd_usr[10].fShifted   = ITM_NULL;
    kbd_usr[10].gShifted   = ITM_NULL;
    kbd_usr[11].primary    = KEY_g;
    kbd_usr[11].fShifted   = ITM_NULL;
    kbd_usr[11].gShifted   = ITM_NULL;
    Norm_Key_00_VAR        = KEY_USERMODE;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  //LEFT SHIFTS Not used, and disabled from KEYS menu
  case USER_SHIFTS2:                                             //USER_SHIFTS 25          //JM Sectioon to be put on a menu
    kbd_usr[0].primary     = KEY_USERMODE;
    kbd_usr[9].primary     = -MNU_TRI;
    kbd_usr[9].fShifted    = KEY_USERMODE;

    kbd_usr[10].primary    = ITM_STO;
    kbd_usr[10].fShifted   = ITM_MAGNITUDE;
    kbd_usr[10].gShifted   = ITM_ANGLE;
    kbd_usr[11].primary    = ITM_RCL;
    kbd_usr[11].fShifted   = ITM_PC;
    kbd_usr[11].gShifted   = ITM_DELTAPC;

    kbd_usr[6].primary    = KEY_f;
    kbd_usr[6].fShifted   = ITM_NULL;
    kbd_usr[6].gShifted   = ITM_NULL;
    kbd_usr[7].primary    = KEY_g;
    kbd_usr[7].fShifted   = ITM_NULL;
    kbd_usr[7].gShifted   = ITM_NULL;
    Norm_Key_00_VAR        = KEY_USERMODE;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;



  case USER_SIGMAPLUS:                                              //USER_DEFAULTS FOR USER: E+ E+
    kbd_usr[0].primary     = ITM_SIGMAPLUS;
    Norm_Key_00_VAR        = ITM_SIGMAPLUS;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_PRGM:                                              //USER_DEFAULTS FOR USER: E+ PRGM
    kbd_usr[0].primary     = ITM_PR;
    Norm_Key_00_VAR        = ITM_PR;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_USER:                                              //USER_DEFAULTS FOR USER: E+ USER
    kbd_usr[0].primary     = KEY_USERMODE;
    Norm_Key_00_VAR        = KEY_USERMODE;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_HOME:                                              //USER_DEFAULTS FOR USER: E+ USER
    kbd_usr[0].primary     = -MNU_HOME;
    Norm_Key_00_VAR        = -MNU_HOME;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_ALPHA:                                              //USER_DEFAULTS FOR USER: E+ ALPHA
    kbd_usr[0].primary     = ITM_AIM;
    Norm_Key_00_VAR        = ITM_AIM;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_MYM:                                              //USER_DEFAULTS FOR USER: E+ MYM
    kbd_usr[0].primary     = -MNU_MyMenu;
    Norm_Key_00_VAR        = -MNU_MyMenu;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_GSHFT:                                           //USER_DEFAULTS FOR USER: E+ SHIFT G
    kbd_usr[0].primary     = KEY_g;
    kbd_usr[0].gShifted    = ITM_NULL;
    kbd_usr[0].fShifted    = ITM_NULL;
    Norm_Key_00_VAR        = KEY_g;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_RESET:                                              //USER_RESET 26
    xcopy(kbd_usr, kbd_std, sizeof(kbd_std));
    Norm_Key_00_VAR        = ITM_SIGMAPLUS;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnClearFlag(FLAG_USER); //userModeEnabled = false;
    break;

/*
  case JM_ASSIGN:                 //A non 0 and non 32766 value means the FN NUMBER is in JM_ASSIGN, AND KEYBOARD.C will wait for a key to be assigned to                                     //USER_RESET 27
    JM_ASN_MODE = KEY_CC;         //TEMPORARY TEST FUNCTION
    break;
*/

  case JM_SEEK_FN:      //32766 in KEYBOARD.C will wait for a key. SEEK FUNCTION,         //USER_RESET 27
    JM_ASN_MODE = 32766;
#ifndef TESTSUITE_BUILD
//    clearScreen(false,true,false);
    showString("Select function from keys: EXIT Aborts", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X), vmNormal, true, true);
#endif
    break;

  default:
    break;
  }
}




void fnKEYSELECT(void) {                                        //JM ASSIGN - REMEMBER NEXT KEYBOARD FUNCTION
  if(JM_ASN_MODE == KEY_EXIT1 || JM_ASN_MODE == KEY_BACKSPACE) {
    JM_ASN_MODE = 0;
#ifndef TESTSUITE_BUILD
    showString("Abandoned or illegal function", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X), vmNormal, true, true);
#endif
  }
  else {
#ifndef TESTSUITE_BUILD
    showString("Select key: top 4 lines excl. FN1-6 & [<-],", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X), vmNormal, true, true);
    showString("incl. [/] [*] [-] [+] [R/S].   EXIT aborts.", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_X - REGISTER_X), vmNormal, true, true);
#endif

      fnFlipFlag(FLAG_USER);
      fnFlipFlag(FLAG_USER);

//    userModeEnabled = true;                                     //JM Get out ouf USER MODE to select key in next step
  //  toggleUserMode();
  }
}



//JM Check if JM ASSIGN IS IN PROGRESS AND CAPTURE THE FUNCTION AND KEY TO BE ASSIGNED
//gets here only after valid function and any key is selected
void fnASSIGN(int16_t JM_ASN_MODE, int16_t tempkey) {           //JM ASSIGN - REMEMBER NEXT KEYBOARD FUNCTION
  switch(tempkey) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 21:
    case 26:
    case 31:
    case 35:
    case 36:

      //JM_convertIntegerToShortIntegerRegister(tempkey, 10, REGISTER_X);
      //JM_convertIntegerToShortIntegerRegister(JM_ASN_MODE, 10, REGISTER_X);
      if(shiftF) {
        (kbd_usr + tempkey)->fShifted = JM_ASN_MODE;  //Assign function into keyboard array
      }
      else if(shiftG) {
        (kbd_usr + tempkey)->gShifted = JM_ASN_MODE;  //Assign function into keyboard array
      }
      else {
        (kbd_usr + tempkey)->primary = JM_ASN_MODE;   //Assign function into keyboard array
      }

      fnSetFlag(FLAG_USER);
      break;
    default:
#ifndef TESTSUITE_BUILD
//        clearScreen(false,true,false);
        showString("Invalid key", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X), vmNormal, true, true);
#endif
    break;
  }
}




/*
void JM_convertIntegerToShortIntegerRegister(int16_t inp, uint32_t base, calcRegister_t destination) {
  char snum[10];
  itoa(inp, snum, base);
  longInteger_t mem;
  longIntegerInit(mem);
  liftStack();
  stringToLongInteger(snum,base,mem);
  convertLongIntegerToShortIntegerRegister(mem, base, destination);

  //setRegisterShortIntegerBase(destination, base);
  if(lastIntegerBase != 0) {
    fnChangeBase(lastIntegerBase);                     //This converts shortint, longint and real to shortint!
  }
  else {
    fnChangeBase(base);                                //This converts shortint, longint and real to shortint!
  }

  longIntegerFree(mem);
}
*/










//*********************
//JM\/\/\/\/
/*
#ifdef NOTNEEDED. KEPT FOR REFERENCE

void fnComplexCCCC_CPX(uint16_t unusedParamButMandatory) {      //JM HARDWAIRED FOR f[COMPLEX]
  userModeEnabledMEM = userModeEnabled;
  userModeEnabled = false;

#ifdef JM_LAYOUT_1A               //JM LAYOUT 1A. CHALLENGE.
  shiftF = true;                  //JM
  shiftG = false;                 //JM
  #ifdef PC_BUILD
  btnClicked(NULL, "12");         //JM changed from 02
  #endif
  #ifdef DMCP_BUILD
  btnClicked(NULL, "12");         //JM changed from 02
  #endif
#endif

#ifdef JM_LAYOUT_2_DM42_STRICT    //JM LAYOUT 2. DM42 STRICT.
  shiftF = true;                  //JM
  shiftG = false;                 //JM
  #ifdef PC_BUILD
  btnClicked(NULL, "06");         //JM changed from 02
  #endif

  #ifdef DMCP_BUILD
  btnClicked(NULL, "06");         //JM changed from 02
  #endif
#endif
  userModeEnabled = userModeEnabledMEM;
}



//void fnComplexCCCC_CC1(uint16_t unusedParamButMandatory) {      //FOR CC1  HARDWIRED FOR TOP LEFT BUTTON
//#ifndef TESTSUITE_BUILD
//  userModeEnabledMEM = userModeEnabled;
//  userModeEnabled = false;
//  shiftF = false;                 //JM
//  shiftG = false;                 //JM
//  int16_t Norm_Key_00_VAR_M;
//  Norm_Key_00_VAR_M = Norm_Key_00_VAR;   //JM Use key 00 as temporary location for CC, which is not normally on the 43C keyboard.
//  Norm_Key_00_VAR        = KEY_CC1;               //JM
//  btnClicked(NULL, "00");
//  Norm_Key_00_VAR        = Norm_Key_00_VAR_M;    //JM
//  userModeEnabled = userModeEnabledMEM;
//#endif
//}



void fnComplexCCCC_CC(uint16_t unusedParamButMandatory) {       //FOR CC  HARDWIRED FOR TOP LEFT BUTTON
#ifndef TESTSUITE_BUILD
  userModeEnabledMEM = userModeEnabled;
  userModeEnabled = false;
  shiftF = false;                 //JM
  shiftG = false;                 //JM
  int16_t Norm_Key_00_VAR_M;
  Norm_Key_00_VAR_M = Norm_Key_00_VAR;   //JM Use key 00 as temporary location for CC, which is not normally on the 43C keyboard.
  Norm_Key_00_VAR        = KEY_CC;               //JM
  btnClicked(NULL, "00");
  Norm_Key_00_VAR        = Norm_Key_00_VAR_M;    //JM
  userModeEnabled = userModeEnabledMEM;
#endif
}

#endif //NOTNEEDED
*/


//JM^^^^^^^



//FUTURE
/*
void ItemBrowser(uint16_t unusedParamButMandatory) {
  int16_t registerNameWidth;

  if(currentRegisterBrowserScreen == 9999) { // Init
    currentRegisterBrowserScreen = 0;
    rbrMode = RBR_GLOBAL;
    showContent = true;
    rbr1stDigit = true;
    calcMode = CM_REGISTER_BROWSER;
  }

  if(currentRegisterBrowserScreen < 9999) {
    clearScreen(false, true, true);

    int16_t regist;
    for(int16_t row=0; row<10; row++) {
      regist = (currentRegisterBrowserScreen + row); // % FIRST_LOCAL_REGISTER;

      itoa(regist, tmpStr3000, 10);
      registerNameWidth = showString(tmpStr3000, &standardFont, 1, 219-22*row, vmNormal, false, true);

      strcpy(tmpStr3000, "'");
      strcpy(tmpStr3000, indexOfItems[regist].itemPrinted);
      strcat(tmpStr3000, "'");
      if(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
        tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
        while(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
          tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
        }
        strcat(tmpStr3000 + stringByteLength(tmpStr3000), STD_ELLIPSIS);
      }
      showString(tmpStr3000, &standardFont, SCREEN_WIDTH - stringWidth(tmpStr3000, &standardFont, false, true) - 1, 219-22*row, vmNormal, false, true);
    }
  }
}
*/






