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
 * \file gui.c
 ***********************************************/

#include "wp43s.h"

#ifdef PC_BUILD
GtkWidget *grid;
GtkWidget *backgroundImage;
GtkWidget *lblFSoftkeyArea, *lblGSoftkeyArea;
GtkWidget *lblBehindScreen;

GtkWidget *btn11,   *btn12,   *btn13,   *btn14,   *btn15,   *btn16;

GtkWidget *btn21,   *btn22,   *btn23,   *btn24,   *btn25,   *btn26;
GtkWidget *lbl21F,  *lbl22F,  *lbl23F,  *lbl24F,  *lbl25F,  *lbl26F;
GtkWidget *lbl21G,  *lbl22G,  *lbl23G,  *lbl24G,  *lbl25G,  *lbl26G;
GtkWidget *lbl21L,  *lbl22L,  *lbl23L,  *lbl24L,  *lbl25L,  *lbl26L;
GtkWidget *lbl21Gr, *lbl22Gr, *lbl23Gr, *lbl24Gr, *lbl25Gr, *lbl26Gr;

GtkWidget *btn31,   *btn32,   *btn33,   *btn34,   *btn35,   *btn36;
GtkWidget *lbl31F,  *lbl32F,  *lbl33F,  *lbl34F,  *lbl35F,  *lbl36F;
GtkWidget *lbl31G,  *lbl32G,  *lbl33G,  *lbl34G,  *lbl35G,  *lbl36G;
GtkWidget *lbl31L,  *lbl32L,  *lbl33L,  *lbl34L,  *lbl35L,  *lbl36L;
GtkWidget                     *lbl33H,  *lbl34H;
GtkWidget *lbl31Gr, *lbl32Gr, *lbl33Gr, *lbl34Gr, *lbl35Gr, *lbl36Gr;

GtkWidget *btn41,   *btn42,   *btn43,   *btn44,   *btn45;
GtkWidget *lbl41F,  *lbl42F,  *lbl43F,  *lbl44F,  *lbl45F;
GtkWidget *lbl41G,  *lbl42G,  *lbl43G,  *lbl44G,  *lbl45G;
GtkWidget *lbl41L,  *lbl42L,  *lbl43L,  *lbl44L,  *lbl45L;
GtkWidget           *lbl42H,  *lbl43P,  *lbl44P;
GtkWidget *lbl41Gr, *lbl42Gr, *lbl43Gr, *lbl44Gr, *lbl45Gr;

GtkWidget *btn51,   *btn52,   *btn53,   *btn54,   *btn55;
GtkWidget *lbl51F,  *lbl52F,  *lbl53F,  *lbl54F,  *lbl55F;
GtkWidget *lbl51G,  *lbl52G,  *lbl53G,  *lbl54G,  *lbl55G;
GtkWidget *lbl51L,  *lbl52L,  *lbl53L,  *lbl54L,  *lbl55L;
GtkWidget *lbl51Gr, *lbl52Gr, *lbl53Gr, *lbl54Gr, *lbl55Gr;

GtkWidget *btn61,   *btn62,   *btn63,   *btn64,   *btn65;
GtkWidget *lbl61F,  *lbl62F,  *lbl63F,  *lbl64F,  *lbl65F;
GtkWidget *lbl61G,  *lbl62G,  *lbl63G,  *lbl64G,  *lbl65G;
GtkWidget *lbl61L,  *lbl62L,  *lbl63L,  *lbl64L,  *lbl65L;
GtkWidget                                         *lbl65H;  //JM
GtkWidget *lbl61Gr, *lbl62Gr, *lbl63Gr, *lbl64Gr, *lbl65Gr;

GtkWidget *btn71,   *btn72,   *btn73,   *btn74,   *btn75;
GtkWidget *lbl71F,  *lbl72F,  *lbl73F,  *lbl74F,  *lbl75F;
GtkWidget *lbl71G,  *lbl72G,  *lbl73G,  *lbl74G,  *lbl75G;
GtkWidget *lbl71L,  *lbl72L,  *lbl73L,  *lbl74L,  *lbl75L;
GtkWidget           *lbl72H,  *lbl73H;                      //JM
GtkWidget *lbl71Gr, *lbl72Gr, *lbl73Gr, *lbl74Gr, *lbl75Gr;

GtkWidget *btn81,   *btn82,   *btn83,   *btn84,   *btn85;
GtkWidget *lbl81F,  *lbl82F,  *lbl83F,  *lbl84F,  *lbl85F;
GtkWidget *lbl81G,  *lbl82G,  *lbl83G,  *lbl84G,  *lbl85G;
GtkWidget *lbl81L,  *lbl82L,  *lbl83L,  *lbl84L,  *lbl85L;
GtkWidget           *lbl82H,  *lbl83H,  *lbl84H,  *lbl85H;  //JM
GtkWidget *lbl81Gr, *lbl82Gr, *lbl83Gr, *lbl84Gr, *lbl85Gr;
GtkWidget *lblOn; //JM
//JM7 GtkWidget  *lblConfirmY; //JM for Y/N
//JM7 GtkWidget  *lblConfirmN; //JM for Y/N

#if (DEBUG_PANEL == 1)
  GtkWidget *lbl1[DEBUG_LINES], *lbl2[DEBUG_LINES];
  GtkWidget *btnBitFields, *btnFlags, *btnRegisters, *btnLocalRegisters, *btnStatisticalSums, *btnNamedVariables, *btnTmpAndSavedStackRegisters;
  GtkWidget *chkHexaString;
  int16_t debugWidgetDx, debugWidgetDy;
#endif

char *cssData;



static gint destroyCalc(GtkWidget* w, GdkEventAny* e, gpointer data) {
  saveCalc();
  gtk_main_quit();

  return 0;
}





//JM Lower case alpha letters from PC --> produce letters in the current case.
//JM Upper case alpha letters from PC --> change case and produce letter. Restore case.


//JM ALPHA SECTION FOR ALPHAMODE - LOWER CASE PC LETTER INPUT. USE LETTER
void btnClicked_LC(GtkWidget *w, gpointer data) {
  btnPressed(w, data);
  btnReleased(w, data);
}


//JM ALPHA SECTION FOR ALPHAMODE -  UPPER CASE PC LETTER INPUT. INVERT 43C CASE. USE LETTER.
void btnClicked_UC(GtkWidget *w, gpointer data) {
  uint8_t alphaCase_MEM;
  alphaCase_MEM = alphaCase;
  if(alphaCase == AC_UPPER) {alphaCase = AC_LOWER;} 
  else if (alphaCase == AC_LOWER) {alphaCase = AC_UPPER;}
  btnPressed(w, data);
  btnReleased(w, data);
  alphaCase = alphaCase_MEM;
}


//JM NUMERIC SECTION FOR ALPHAMODE - FORCE Numeral - Numbers from PC --> produce numbers.  
void btnClicked_NU(GtkWidget *w, gpointer data) {
  shiftF = true;       //JM
  shiftG = false;      //JM
  Reset_Shift_Mem();   //JM
  btnPressed(w, data);
  btnReleased(w, data);
}

//Shifted numbers !@#$%^&*() from PC --> activate shift and use numnber 1234567890. Restore case.
void btnClicked_SNU(GtkWidget *w, gpointer data) {
  shiftF = false;       //JM
  shiftG = true;        //JM
  Reset_Shift_Mem();    //JM
  btnClicked(NULL, "34");     //Alphadot
  //Only : is working at this point
}



gboolean keyPressed(GtkWidget *w, GdkEventKey *event, gpointer data) {
  //printf("%d\n", event->keyval);


//JM ALPHA SECTION FOR ALPHAMODE - TAKE OVER ALPHA KEYBOARD
if (calcMode == CM_AIM) {
switch (event->keyval) {
//ROW 1
    case 65470: // F1                                                    //**************-- FUNCTION KEYS --***************//
      btnFnClicked(w, "1");
      break;
    case 65471: // F2
      btnFnClicked(w, "2");
      break;
    case 65472: // F3
      btnFnClicked(w, "3");
      break;
    case 65473: // F4
      btnFnClicked(w, "4");
      break;
    case 65474: // F5
      btnFnClicked(w, "5");
      break;
    case 65475: // F6
      btnFnClicked(w, "6");
      break;
//ROW 2
    case 65:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM.    //**************-- ALPHA KEYS UPPER CASE --***************//
      btnClicked_UC(w, "00");                                          //UPPER CASE PC LETTER INPUT. INVERT 43C CASE. USE LETTER.
      break;
    case 66:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "01");
      break;
    case 67:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "02");
      break;
    case 68:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "03");
      break;
    case 69:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "04");
      break;
    case 70:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "05");
      break;
//ROW 3
    case 71:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "06");
      break;
    case 72:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "07");
      break;
    case 73:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "08");
      break;
    case 74:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "09");
      break;
    case 75:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "10");
      break;
    case 76:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "11");
      break;
//ROW 4
    case 65421:                                               //JM    // Enter
    case 65293:                                               //JM    // Enter
      btnClicked(w, "12");
      break;
    case 77:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "13");
      break;
    case 78:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "14");
      break;
    case 79:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "15");
      break;
    case 65288: // Backspace
      btnClicked(w, "16");
      break;

//ROW 5
    case 65362:                                               //JM     // CursorUp //JM
      btnClicked(w, "17");
      break;
    case 80:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "18");
      break;
    case 81:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "19");
      break;
    case 82:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "20");
      break;
    case 83:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "21");
      break;
//ROW 6
    case 65364:                                               //JM     // CursorDown //JM
      btnClicked(w, "22");
      break;
    case 84:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "23");
      break;
    case 85:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "24");
      break;
    case 86:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "25");
      break;
    case 87:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "26");
      break;
//ROW 7
    case 65505:                                               //JM     // left shift  //JM
  //case 65506: // right shift //JM
      btnClicked(w, "27");
      break;
    case 88:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "28");
      break;
    case 89:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "29");
      break;
    case 90:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_UC(w, "30");
      break;







//JM ALPHA LOWER CASE SECTION FOR ALPHAMODE - TAKE OVER ALPHA KEYBOARD
//ROW 2
    case 65+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM     //**************-- ALPHA KEYS LOWER CASE --***************//
      btnClicked_LC(w, "00");                                             //LOWER CASE PC LETTER INPUT. USE LETTER IN THE CURRENT 43C CASE.
      break;
    case 66+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "01");
      break;
    case 67+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "02");
      break;
    case 68+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "03");
      break;
    case 69+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "04");
      break;
    case 70+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "05");
      break;
//ROW 3
    case 71+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "06");
      break;
    case 72+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "07");
      break;
    case 73+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "08");
      break;
    case 74+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "09");
      break;
    case 75+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "10");
      break;
    case 76+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "11");
      break;
//ROW 4
    case 77+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "13");
      break;
    case 78+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "14");
      break;
    case 79+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "15");
      break;
//ROW 5
    case 80+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "18");
      break;
    case 81+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "19");
      break;
    case 82+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "20");
      break;
    case 83+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "21");
      break;
//ROW 6
    case 84+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "23");
      break;
    case 85+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "24");
      break;
    case 86+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "25");
      break;
    case 87+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "26");
      break;
//ROW 7
    case 88+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "28");
      break;
    case 89+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "29");
      break;
    case 90+32:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_LC(w, "30");
      break;






//JM  NUMERALS FOR ALPHAMODE - TAKE OVER ALPHA KEYBOARD
//ROW 5
    case 48+7:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM     //**************-- NUM KEYS  --***************//
      btnClicked_NU(w, "18");                                            // Numbers from PC --> produce numbers.
      break;
    case 48+8:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "19");
      break;
    case 48+9:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "20");
      break;
//ROW 6
    case 48+4:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "23");
      break;
    case 48+5:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "24");
      break;
    case 48+6:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "25");
      break;
//ROW 7
    case 48+1:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "28");
      break;
    case 48+2:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "29");
      break;
    case 48+3:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "30");
      break;
//ROW 8
    case 48+0:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "33");
      break;



//JM  CHARACTERS FOR ALPHAMODE - TAKE OVER ALPHA KEYBOARD -                //*****. USE MENUS TO GET DEEP DOWN CHARACTERS *******//
//Only colon is working
// The issue is that the international letters are not working as yet.
//   Once working, all letters available on the PC keyboard must be mapped here 
    case 158:                   //JM changed from 58 to 158, to avoid duplicasting the Colon. Colon exists above.
      btnClicked_SNU(w, "2");   //Switch to ALphadot; press FN2
      break;



//ROW 8
    case 65307:              // Esc EXIT //JM                   //JM     //**************-- OTHER DIRECT ALPHA MODE KEYBOARD KEYS  --***************//
      btnClicked(w, "32");
      break;

    case 58:                 // COLON.   //JM
      btnClicked(w, "33");
      break;

    case 44:                 // ,        //JM
    case 46:                 // .        //JM
    case 65454:              // .        //JM
      btnClicked(w, "34");
      break;

    case 63:                 // ?        //JM
      btnClicked(w, "35");
      break;

    case 32:                //JM SPACE   //JM
      btnClicked(w, "36");
      break;


  }
return FALSE;
}




//ORIGINAL MODIFEIED KEYBOARD DETECTION

  switch (event->keyval) {
//ROW 1
    case 65470: // F1
      //printf("key pressed: F1\n");
      btnFnClicked(w, "1");
      break;

    case 65471: // F2
      //printf("key pressed: F2\n");
      btnFnClicked(w, "2");
      break;

    case 65472: // F3
      //printf("key pressed: F3\n");
      btnFnClicked(w, "3");
      break;

    case 65473: // F4
      //printf("key pressed: F4\n");
      btnFnClicked(w, "4");
      break;

    case 65474: // F5
      //printf("key pressed: F5\n");
      btnFnClicked(w, "5");
      break;

    case 65475: // F6
      //printf("key pressed: F6\n");
      btnFnClicked(w, "6");
      break;
//ROW 2
//dr    case 73:  // I
//JM-    case 65:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 97:  // a  //dr
      //printf("key pressed: a Sigma+\n"); //dr
      btnClicked(w, "00");
      break;

//dr    case 89:  // Y
//JM-    case 66:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 118: // v //dr
      //printf("key pressed: v 1/X\n"); //dr
      btnClicked(w, "01");
      break;

//dr    case 84:  // T
//JM-    case 67:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 113: // q //dr
      //printf("key pressed: q SQRT\n"); //dr
      btnClicked(w, "02");
      break;

//dr    case 76:  // L
//JM-    case 68:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 111: // o //dr
      //printf("key pressed: o LOG\n"); //dr
      btnClicked(w, "03");
      break;

//JM-    case 69:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 108: // l //dr
      //printf("key pressed: l LN\n"); //dr
      btnClicked(w, "04");
      break;

//dr    case 81:  // Q
//JM-    case 70:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 120: // x //dr
      //printf("key pressed: x XEQ\n"); //dr
      btnClicked(w, "05");
      break;
//ROW 3
//dr    case 83:  // S
//JM-   case 71:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 109: // m //dr
      //printf("key pressed: m STO\n"); //dr
      btnClicked(w, "06");
      break;

//dr    case 82:  // R
//JM-    case 72:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 114: // r
      //printf("key pressed: r RCL\n");
      btnClicked(w, "07");
      break;

//dr    case 65366: // PgDn
//JM-    case 73:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 100: // d //dr
      //printf("key pressed: d Rdown\n"); //dr
      btnClicked(w, "08");
      break;

    case 112: // p         //dr                //JM Special case: p = pi
      //printf("key pressed: p pi\n"); //dr
      shiftF = true;       //dr
      shiftG = false;      //JM
      Reset_Shift_Mem();   //JM
      btnClicked(w, "08"); //dr
      break;               //dr

//dr    case 67:  // C
//JM-    case 74:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 115: // s //dr
      //printf("key pressed: s SIN\n"); //dr
      btnClicked(w, "09");
      break;

//dr    case 70:  // F
//JM-    case 75:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 99:  // c //dr
      //printf("key pressed: c COS\n"); //dr
      btnClicked(w, "10");
      break;

//dr    case 71:  // G
//JM-    case 76:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 116: // t //dr
      //printf("key pressed: t TAN\n"); //dr
      btnClicked(w, "11");
      break;
//ROW 4
    case 65421: // Enter
    case 65293: // Enter
      //printf("key pressed: ENTER\n");
      btnClicked(w, "12");
      break;

//dr    case 65289: // Tab
//JM-    case 77:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 119: // w //dr
      //printf("key pressed: w x<>y\n"); //dr
      btnClicked(w, "13");
      break;

//JM-    case 78:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 110: // n //dr
      //printf("key pressed: n +/-\n"); //dr
      btnClicked(w, "14");
      break;

//dr    case 69:  // E
//JM-    case 79:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 101: // e //dr
    //printf("key pressed: e EXP\n"); //dr
      btnClicked(w, "15");
      break;

    case 65288: // Backspace
      //printf("key pressed: Backspace\n");
      btnClicked(w, "16");
      break;
//ROW 5
    case 65362: // CursorUp //JM
                            //JM
      //printf("key pressed: <Up>\n"); //dr
      btnClicked(w, "17");
      break;

//JM-    case 80:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 55:    // 7
    case 65463: // 7
      //printf("key pressed: 7\n");
      btnClicked(w, "18");
      break;

//JM-    case 38:  //JM SHIFTED NUMERAL  //JM
      shiftF = true;       //JM
      shiftG = false;      //JM
      Reset_Shift_Mem();   //JM
      btnClicked(w, "18"); //JM
      break;               //JM



//JM-    case 81:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 56:    // 8
    case 65464: // 8
      //printf("key pressed: 8\n");
      btnClicked(w, "19");
      break;


//JM-    case 82:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 57:    // 9
    case 65465: // 9
      //printf("key pressed: 9\n");
      btnClicked(w, "20");
      break;

/*    case 40:  //JM SHIFTED NUMERAL  //JM
      shiftF = true;       //JM
      shiftG = false;      //JM
      Reset_Shift_Mem();   //JM
      btnClicked(w, "20"); //JM
      break;               //JM
*/



//    case 83:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 47:    // / //JM
    case 65455: // / //JM
      //printf("key pressed: divide\n"); //dr
      btnClicked(w, "21");
      break;
//ROW 6
    case 65364: // CursorDown //JM
                              //JM
      //printf("key pressed: <Down>\n"); //dr
      btnClicked(w, "22");
      break;

//    case 84:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 52:    // 4
    case 65460: // 4
      //printf("key pressed: 4\n");
      btnClicked(w, "23");
      break;

/*    case 36:  //JM SHIFTED NUMERAL  //JM
      shiftF = true;       //JM
      shiftG = false;      //JM
      Reset_Shift_Mem();   //JM
      btnClicked(w, "23"); //JM
      break;               //JM
*/


//    case 85:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 53:    // 5
    case 65461: // 5
      //printf("key pressed: 5\n");
      btnClicked(w, "24");
      break;

/*    case 37:  //JM SHIFTED NUMERAL  //JM
      shiftF = true;       //JM
      shiftG = false;      //JM
      Reset_Shift_Mem();   //JM
      btnClicked(w, "24"); //JM
      break;               //JM
*/


//    case 86:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 54:    // 6
    case 65462: // 6
      //printf("key pressed: 6\n");
      btnClicked(w, "25");
      break;

/*    case 94:  //JM SHIFTED NUMERAL  //JM
      shiftF = true;       //JM
      shiftG = false;      //JM
      Reset_Shift_Mem();   //JM
      btnClicked(w, "25"); //JM
      break;               //JM
*/


//    case 87:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 42:    // * //JM
    case 65450: // * //JM
      //printf("key pressed: multiply\n"); //dr
      btnClicked(w, "26");
      break;

//ROW 7
    case 65505: // left shift  //JM
//JM- Reinstated:
//    case 65506: //JM right shift. 65453: // -  //JM Remove Right Shift, to allow * & +
      //printf("key pressed: Shift\n"); //dr
      btnClicked(w, "27");
      break;

//JM-    case 88:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 49:    // 1
    case 65457: // 1
      //printf("key pressed: 1\n");
      btnClicked(w, "28");
      break;

/*    case 33:  //JM SHIFTED NUMERAL  //JM
      shiftF = true;       //JM
      shiftG = false;      //JM
      Reset_Shift_Mem();   //JM
      btnClicked(w, "28"); //JM
      break;               //JM
*/


//    case 89:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 50:    // 2
    case 65458: // 2
      //printf("key pressed: 2\n");
      btnClicked(w, "29");
      break;

/*    case 64:  //JM SHIFTED NUMERAL  //JM
      shiftF = true;       //JM
      shiftG = false;      //JM
      btnClicked(w, "29"); //JM
      break;               //JM
*/



//    case 90:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 51:    // 3
    case 65459: // 3
      //printf("key pressed: 3\n");
      btnClicked(w, "30");
      break;

/*    case 35:  //JM SHIFTED NUMERAL  //JM
      shiftF = true;       //JM
      shiftG = false;      //JM
      Reset_Shift_Mem();   //JM
      btnClicked(w, "30"); //JM
      break;               //JM
*/


//    case 95:  //JM UNDERSCORE FOR SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 45:    // - //JM
    case 65453: // - //JM
      //printf("key pressed: subtract\n"); //dr
      btnClicked(w, "31");
      break;
//ROW 8
    case 65307: // Esc //JM
                       //JM
      //printf("key pressed: EXIT\n"); //dr
      btnClicked(w, "32");
      break;

//    case 58:  //JM COLON FOR SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 48:    // 0
    case 65456: // 0
      //printf("key pressed: 0\n");
      btnClicked(w, "33");
      break;

/*    case 41:  //JM SHIFTED NUMERAL  //JM
      shiftF = true;       //JM
      shiftG = false;      //JM
      Reset_Shift_Mem();   //JM
      btnClicked(w, "33"); //JM
      break;               //JM
*/


    case 44:    // ,
    case 46:    // .
    case 65454: // .
      //printf("key pressed: .\n");
      btnClicked(w, "34");
      break;

//    case 65507: // left Ctrl
//    case 65508: // right Ctrl
//JM-    case 63:  //JM ? for SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 92: // \                                //JM R/S changed as on Mac CTRL is something else.
      //printf("key pressed: Ctrl R/S\n");
      btnClicked(w, "35");
      break;

//JM-    case 32:  //JM SPACE for SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
    case 43:    // + //JM
    case 65451: // + //JM
      //printf("key pressed: add\n"); //dr
      btnClicked(w, "36");
      break;

//JM- Reinstated
    case 72:  // H    //JM REMOVE CAP H. ONLY lower case wil print
    case 104: // h
      //printf("key pressed: h Hardcopy\n");
      copyScreenToClipboard();
      break;

    default:
      break;
  }
  return FALSE;

}
#endif


#ifndef DMCP_BUILD
void strReplace(char *haystack, const char *needle, const char *newNeedle) {
  ////////////////////////////////////////////////////////
  // There MUST be enough memory allocated to *haystack //
  // when strlen(newNeedle) > strlen(needle)            //
  ////////////////////////////////////////////////////////
  char *tmpString, *needleLocation;
  int  needleLg;

  while(strstr(haystack, needle) != NULL) {
    needleLg = strlen(needle);
    needleLocation = strstr(haystack, needle);
    tmpString = malloc(strlen(needleLocation + needleLg) + 1);
    #ifdef PC_BUILD
      if(tmpString == NULL) {
        showInfoDialog("In function strReplace:", "error allocating memory for tmpString!", NULL, NULL);
        exit(1);
      }
    #endif

    strcpy(tmpString, needleLocation + needleLg);
    *strstr(haystack, needle) = 0;
    strcat(haystack, newNeedle);
    strcat(haystack, tmpString);
    free(tmpString);
  }
}
#endif


#ifdef PC_BUILD
/********************************************//**
 * \brief Reads the CSS file to configure the calc's GUI style
 *
 * \param void
 * \return void
 ***********************************************/
void prepareCssData(void) {
  FILE *cssFile;
  char *toReplace, *replaceWith, needle[100], newNeedle[100];
  int  i, fileLg;


#ifdef JM_LAYOUT_2_DM42_STRICT                    //JM LAYOUT 2
  #define CSSFILE "wp43s_pre_L2.css"              //JM L
#endif //JM L
#ifdef JM_LAYOUT_1A                               //JM LAYOUT 1
  #define CSSFILE "wp43s_pre.css"
#endif //JM L

  // Convert the pre-CSS data to CSS data
  cssFile = fopen(CSSFILE, "rb");
  if(cssFile == NULL) {
    showInfoDialog("In function prepareCssData:", "error opening file " CSSFILE "!", NULL, NULL);
    exit(1);
  }

  // Get the file length
  fseek(cssFile, 0L, SEEK_END);
  fileLg = ftell(cssFile);
  fseek(cssFile, 0L, SEEK_SET);

  cssData = malloc(2*fileLg); // To be sure there is enough space
  if(cssData == NULL) {
    showInfoDialog("In function prepareCssData:", "error allocating 10000 bytes for CSS data", NULL, NULL);
    exit(1);
  }

  fread(cssData, 1, fileLg, cssFile);
  fclose(cssFile);
  cssData[fileLg] = 0;

  toReplace = strstr(cssData, "/* Replace $");
  while(toReplace != NULL) {
    toReplace += 11;
    i = -1;
    while(toReplace[++i] != ' ') {
      needle[i] = toReplace[i];
    }
    needle[i] = 0;

    *toReplace = ' ';

    replaceWith = strstr(toReplace, " with ");
    if(replaceWith == NULL) {
      showInfoDialog("In function prepareCssData:", "Can't find \" with \" after \"/* Replace $\" in CSS file " CSSFILE, NULL, NULL);
      exit(1);
    }

    replaceWith[1] = ' ';
    replaceWith += 6;
    i = -1;
    while(replaceWith[++i] != ' ') {
      newNeedle[i] = replaceWith[i];
    }
    newNeedle[i] = 0;

    strReplace(toReplace, needle, newNeedle);

    toReplace = strstr(cssData, "/* Replace $");
  }

  if(strstr(cssData, "$") != NULL) {
    showInfoDialog("In function prepareCssData:", "There is still an unreplaced $ in the CSS file!\nPlease check file " CSSFILE, NULL, NULL);
    printf("%s\n", cssData);
    exit(1);
  }
}



/********************************************//**
 * \brief Hides all the widgets on the calc GUI
 *
 * \param void
 * \return void
 ***********************************************/
void hideAllWidgets(void) {
  gtk_widget_hide(btn11);
  gtk_widget_hide(btn12);
  gtk_widget_hide(btn13);
  gtk_widget_hide(btn14);
  gtk_widget_hide(btn15);
  gtk_widget_hide(btn16);

  gtk_widget_hide(btn21);
  gtk_widget_hide(btn22);
  gtk_widget_hide(btn23);
  gtk_widget_hide(btn24);
  gtk_widget_hide(btn25);
  gtk_widget_hide(btn26);

  gtk_widget_hide(lbl21F);
  gtk_widget_hide(lbl21G);
  gtk_widget_hide(lbl21L);
  gtk_widget_hide(lbl22F);
  gtk_widget_hide(lbl22G);
  gtk_widget_hide(lbl22L);
  gtk_widget_hide(lbl23F);
  gtk_widget_hide(lbl23G);
  gtk_widget_hide(lbl23L);
  gtk_widget_hide(lbl24F);
  gtk_widget_hide(lbl24G);
  gtk_widget_hide(lbl24L);
  gtk_widget_hide(lbl25F);
  gtk_widget_hide(lbl25G);
  gtk_widget_hide(lbl25L);
  gtk_widget_hide(lbl26F);
  gtk_widget_hide(lbl26G);
  gtk_widget_hide(lbl26L);
  gtk_widget_hide(lbl21Gr);
  gtk_widget_hide(lbl22Gr);
  gtk_widget_hide(lbl23Gr);
  gtk_widget_hide(lbl24Gr);
  gtk_widget_hide(lbl25Gr);
  gtk_widget_hide(lbl26Gr);

  gtk_widget_hide(btn31);
  gtk_widget_hide(btn32);
  gtk_widget_hide(btn33);
  gtk_widget_hide(btn34);
  gtk_widget_hide(btn35);
  gtk_widget_hide(btn36);

  gtk_widget_hide(lbl31F);
  gtk_widget_hide(lbl31G);
  gtk_widget_hide(lbl31L);
  gtk_widget_hide(lbl32F);
  gtk_widget_hide(lbl32G);
  gtk_widget_hide(lbl32L);
  gtk_widget_hide(lbl33F);
  gtk_widget_hide(lbl33G);
  gtk_widget_hide(lbl33H);
  gtk_widget_hide(lbl33L);
  gtk_widget_hide(lbl34F);
  gtk_widget_hide(lbl34G);
  gtk_widget_hide(lbl34H);  //JM CAPS
  gtk_widget_hide(lbl34L);
  gtk_widget_hide(lbl35F);
  gtk_widget_hide(lbl35G);
  gtk_widget_hide(lbl35L);
  gtk_widget_hide(lbl36F);
  gtk_widget_hide(lbl36G);
  gtk_widget_hide(lbl36L);
  gtk_widget_hide(lbl31Gr);
  gtk_widget_hide(lbl32Gr);
  gtk_widget_hide(lbl33Gr);
  gtk_widget_hide(lbl34Gr);
  gtk_widget_hide(lbl35Gr);
  gtk_widget_hide(lbl36Gr);

  gtk_widget_hide(btn41);
  gtk_widget_hide(btn42);
  gtk_widget_hide(btn43);
  gtk_widget_hide(btn44);
  gtk_widget_hide(btn45);

  gtk_widget_hide(lbl41F);
  gtk_widget_hide(lbl41G);
  gtk_widget_hide(lbl41L);
  gtk_widget_hide(lbl42F);
  gtk_widget_hide(lbl42G);
  gtk_widget_hide(lbl42H);
  gtk_widget_hide(lbl42L);
  gtk_widget_hide(lbl43F);
  gtk_widget_hide(lbl43G);
  gtk_widget_hide(lbl43L);
  gtk_widget_hide(lbl43P);
  gtk_widget_hide(lbl44F);
  gtk_widget_hide(lbl44G);
  gtk_widget_hide(lbl44L);
  gtk_widget_hide(lbl44P);
  gtk_widget_hide(lbl45F);
  gtk_widget_hide(lbl45G);
  gtk_widget_hide(lbl45L);
  gtk_widget_hide(lbl41Gr);
  gtk_widget_hide(lbl42Gr);
  gtk_widget_hide(lbl43Gr);
  gtk_widget_hide(lbl44Gr);
  gtk_widget_hide(lbl45Gr);

  gtk_widget_hide(btn51);
  gtk_widget_hide(btn52);
  gtk_widget_hide(btn53);
  gtk_widget_hide(btn54);
  gtk_widget_hide(btn55);

  gtk_widget_hide(lbl51F);
  gtk_widget_hide(lbl51G);
  gtk_widget_hide(lbl51L);
  gtk_widget_hide(lbl52F);
  gtk_widget_hide(lbl52G);
  gtk_widget_hide(lbl52L);
  gtk_widget_hide(lbl53F);
  gtk_widget_hide(lbl53G);
  gtk_widget_hide(lbl53L);
  gtk_widget_hide(lbl54F);
  gtk_widget_hide(lbl54G);
  gtk_widget_hide(lbl54L);
  gtk_widget_hide(lbl55F);
  gtk_widget_hide(lbl55G);
  gtk_widget_hide(lbl55L);
  gtk_widget_hide(lbl51Gr);
  gtk_widget_hide(lbl52Gr);
  gtk_widget_hide(lbl53Gr);
  gtk_widget_hide(lbl54Gr);
  gtk_widget_hide(lbl55Gr);

  gtk_widget_hide(btn61);
  gtk_widget_hide(btn62);
  gtk_widget_hide(btn63);
  gtk_widget_hide(btn64);
  gtk_widget_hide(btn65);

  gtk_widget_hide(lbl61F);
  gtk_widget_hide(lbl61G);
  gtk_widget_hide(lbl61L);
  gtk_widget_hide(lbl62F);
  gtk_widget_hide(lbl62G);
  gtk_widget_hide(lbl62L);
  gtk_widget_hide(lbl63F);
  gtk_widget_hide(lbl63G);
  gtk_widget_hide(lbl63L);
  gtk_widget_hide(lbl64F);
  gtk_widget_hide(lbl64G);
  gtk_widget_hide(lbl64L);
  gtk_widget_hide(lbl65H);  //JM
  gtk_widget_hide(lbl65F);
  gtk_widget_hide(lbl65G);
  gtk_widget_hide(lbl65L);
  gtk_widget_hide(lbl61Gr);
  gtk_widget_hide(lbl62Gr);
  gtk_widget_hide(lbl63Gr);
  gtk_widget_hide(lbl64Gr);
  gtk_widget_hide(lbl65Gr);

  gtk_widget_hide(btn71);
  gtk_widget_hide(btn72);
  gtk_widget_hide(btn73);
  gtk_widget_hide(btn74);
  gtk_widget_hide(btn75);

  gtk_widget_hide(lbl71F);
  gtk_widget_hide(lbl71G);
  gtk_widget_hide(lbl71L);
  gtk_widget_hide(lbl73H);  //JM
  gtk_widget_hide(lbl72F);
  gtk_widget_hide(lbl72G);
  gtk_widget_hide(lbl72L);
  gtk_widget_hide(lbl72H);
  gtk_widget_hide(lbl73F);
  gtk_widget_hide(lbl73G);
  gtk_widget_hide(lbl73L);
  gtk_widget_hide(lbl74F);
  gtk_widget_hide(lbl74G);
  gtk_widget_hide(lbl74L);
  gtk_widget_hide(lbl75F);
  gtk_widget_hide(lbl75G);
  gtk_widget_hide(lbl75L);
  gtk_widget_hide(lbl71Gr);
  gtk_widget_hide(lbl72Gr);
  gtk_widget_hide(lbl73Gr);
  gtk_widget_hide(lbl74Gr);
  gtk_widget_hide(lbl75Gr);

  gtk_widget_hide(btn81);
  gtk_widget_hide(btn82);
  gtk_widget_hide(btn83);
  gtk_widget_hide(btn84);
  gtk_widget_hide(btn85);

  gtk_widget_hide(lbl81F);
  gtk_widget_hide(lbl81G);
  gtk_widget_hide(lbl81L);
  gtk_widget_hide(lbl82F);
  gtk_widget_hide(lbl82G);
  gtk_widget_hide(lbl82H);  //JM Keep menu appreviation on AIM to identify with menu name
  gtk_widget_hide(lbl82L);
  gtk_widget_hide(lbl83F);
  gtk_widget_hide(lbl83G);
  gtk_widget_hide(lbl83L);
  gtk_widget_hide(lbl83H);  //JM Keep menu appreviation on AIM to identify with menu name
  gtk_widget_hide(lbl84F);
  gtk_widget_hide(lbl84G);
  gtk_widget_hide(lbl84L);
  gtk_widget_hide(lbl84H);  //JM Keep menu appreviation on AIM to identify with menu name
  gtk_widget_hide(lbl85F);
  gtk_widget_hide(lbl85G);
  gtk_widget_hide(lbl85H);  //JM Keep menu appreviation on AIM to identify with menu name
  gtk_widget_hide(lbl85L);
  gtk_widget_hide(lbl81Gr);
  gtk_widget_hide(lbl82Gr);
  gtk_widget_hide(lbl83Gr);
  gtk_widget_hide(lbl84Gr);
  gtk_widget_hide(lbl85Gr);

  gtk_widget_hide(lblOn);
  //JM7 gtk_widget_hide(lblConfirmY);  //JM Y/N
  //JM7 gtk_widget_hide(lblConfirmN);  //JM Y/N

}



void moveLabels(void) {
  int            xPos, yPos;
  GtkRequisition lblF, lblG;

  if(calcLandscape) {
    xPos = X_LEFT_LANDSCAPE;
    yPos = Y_TOP_LANDSCAPE;
  }
  else {
    xPos = X_LEFT_PORTRAIT;
    yPos = Y_TOP_PORTRAIT + DELTA_KEYS_Y;
  }

  #if defined(__MINGW64__)
   yPos += 5;
  #endif

  gtk_widget_get_preferred_size(  lbl21F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl21G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl21F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl21G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl21Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl21Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl22F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl22G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl22F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl22G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl22Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl22Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl23F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl23G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl23F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl23G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl23Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl23Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl24F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl24G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl24F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl24G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl24Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl24Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl25F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl25G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl25F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl25G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl25Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl25Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl26F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl26G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl26F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl26G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl26Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl26Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y;
  gtk_widget_get_preferred_size(  lbl31F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl31G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl31F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl31G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl31Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl31Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl32F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl32G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl32F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl32G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl32Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl32Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl33F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl33G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl33F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl33G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl33Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl33Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl34F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl34G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl34F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl34G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl34Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl34Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl35F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl35G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl35F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl35G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl35Gr, NULL, &lblG);                                                               //JM !! GR
  gtk_fixed_move(GTK_FIXED(grid), lbl35Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);  //JM !! GR

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl36F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl36G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl36F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl36G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl36Gr, NULL, &lblG);                                                               //JM !! GR
  gtk_fixed_move(GTK_FIXED(grid), lbl36Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);  //JM !! GR

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y;
  gtk_widget_get_preferred_size(  lbl41F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl41G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl41F, (2*xPos+KEY_WIDTH_1+DELTA_KEYS_X-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl41G, (2*xPos+KEY_WIDTH_1+DELTA_KEYS_X+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);

  xPos += 2*DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl42F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl42G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl42F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl42G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl42Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl42Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl43F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl43G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl43F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl43G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl43Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl43Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl44F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl44G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl44F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl44G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl44Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl44Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl45F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl45G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl45F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl45G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_widget_get_preferred_size(  lbl51F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl51G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl51F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width *0-40 +2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);//JM align [f] arrowUp (*0-40)
  gtk_fixed_move(GTK_FIXED(grid), lbl51G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width *0-40 +2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);//JM align [f] arrowUp (*0-40)
  //gtk_widget_get_preferred_size(  lbl51Gr, NULL, &lblG);
  //gtk_fixed_move(GTK_FIXED(grid), lbl51Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 18;
  gtk_widget_get_preferred_size(  lbl52F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl52G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl52F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl52G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl52Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl52Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl53F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl53G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl53F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl53G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl53Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl53Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl54F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl54G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl54F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl54G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl54Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl54Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl55F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl55G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl55F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl55G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl55Gr, NULL, &lblG);    //JM GREEK
  gtk_fixed_move(GTK_FIXED(grid), lbl55Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);   //JM GREEK

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_widget_get_preferred_size(  lbl61F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl61G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl61F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width *0-40 +2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);   //JM align [f] arrowDn (*0-40)
  gtk_fixed_move(GTK_FIXED(grid), lbl61G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width *0-40 +2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);   //JM align [f] arrowDn (*0-40)
  //gtk_widget_get_preferred_size(  lbl61Gr, NULL, &lblG);                                                                       //JM10
  //gtk_fixed_move(GTK_FIXED(grid), lbl61Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);          //JM10

  xPos += DELTA_KEYS_X + 18;
  gtk_widget_get_preferred_size(  lbl62F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl62G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl62F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl62G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl62Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl62Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl63F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl63G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl63F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl63G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl63Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl63Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl64F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl64G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl64F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl64G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl64Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl64Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl65F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl65G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl65F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl65G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl65Gr, NULL, &lblG);  //JM
  gtk_fixed_move(GTK_FIXED(grid), lbl65Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK); //JM

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
/*  gtk_widget_get_preferred_size(  lbl71F, NULL, &lblF); //JM REMOVE SHIFT LABELS
  gtk_widget_get_preferred_size(  lbl71G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl71F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl71G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl71Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl71Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);
*/
  xPos += DELTA_KEYS_X + 18;
  gtk_widget_get_preferred_size(  lbl72F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl72G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl72F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl72G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl72Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl72Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl73F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl73G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl73F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl73G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl73Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl73Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl74F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl74G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl74F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl74G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl74Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl74Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl75F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl75G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl75F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl75G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl75Gr, NULL, &lblG); //JM added
  gtk_fixed_move(GTK_FIXED(grid), lbl75Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK); //JMadded

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_widget_get_preferred_size(  lbl81F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl81G, NULL, &lblG);

//#ifdef JM_LAYOUT_2_DM42_STRICT
  gtk_fixed_move(GTK_FIXED(grid), lbl81F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
//  gtk_fixed_move(GTK_FIXED(grid), lbl81F, (2*xPos+KEY_WIDTH_1-lblF.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //JM
  gtk_fixed_move(GTK_FIXED(grid), lbl81G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
//#endif
//#ifdef JM_LAYOUT_1A
//  gtk_fixed_move(GTK_FIXED(grid), lbl81F, (2*xPos+KEY_WIDTH_1-lblF.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //JM
//  gtk_fixed_move(GTK_FIXED(grid), lbl81G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos + 38); //JM
//#endif

gtk_widget_get_preferred_size(  lblOn,  NULL, &lblF);  //JM
gtk_fixed_move(GTK_FIXED(grid), lblOn,  (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2 + 1, yPos + 38); //JM
 //gtk_widget_get_preferred_size(  lbl81Gr, NULL, &lblG);  JM++_
 // gtk_fixed_move(GTK_FIXED(grid), lbl81Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK); //JM ++

  xPos += DELTA_KEYS_X + 18;
  gtk_widget_get_preferred_size(  lbl82F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl82G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl82F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl82G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl82Gr, NULL, &lblG);                                                               //JM ALPHA BLUE MENU LABELS
  gtk_fixed_move(GTK_FIXED(grid), lbl82Gr, (2*xPos+KEY_WIDTH_2-GAP-lblG.width+2)/2,                              yPos + GAP - Y_OFFSET_SHIFTED_LABEL);  //JM ALPHA BLUE MENU LABELS

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl83F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl83G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl83F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl83G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl83Gr, NULL, &lblG);                                                               //JM ALPHA BLUE MENU LABELS
  gtk_fixed_move(GTK_FIXED(grid), lbl83Gr, (2*xPos+KEY_WIDTH_2-GAP-lblG.width+2)/2,                              yPos + GAP - Y_OFFSET_SHIFTED_LABEL);  //JM ALPHA BLUE MENU LABELS

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl84F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl84G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl84F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl84G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl84Gr, NULL, &lblG);                                                                //JM ALPHA BLUE MENU LABELS
  gtk_fixed_move(GTK_FIXED(grid), lbl84Gr, (2*xPos+KEY_WIDTH_2-GAP-lblG.width+2)/2,                              yPos + GAP - Y_OFFSET_SHIFTED_LABEL);   //JM ALPHA BLUE MENU LABELS

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl85F, NULL, &lblF);
  //gtk_fixed_move(GTK_FIXED(grid), lbl85F, (2*xPos+KEY_WIDTH_2-lblF.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL); //JM

  //gtk_widget_get_preferred_size(  lblOn,  NULL, &lblF); //JM
  gtk_widget_get_preferred_size(  lbl85G, NULL, &lblG);
 
#ifdef JM_LAYOUT_2_DM42_STRICT //JM LAYOUT 2. DM42 STRICT.
  gtk_fixed_move(GTK_FIXED(grid), lbl85F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP*1.4-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL); //JM align provision for CAT
  gtk_fixed_move(GTK_FIXED(grid), lbl85G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL); //JM align provision for CAT
#endif //JM END OF LAYOUT 2 DM42 STRICT.
#ifdef JM_LAYOUT_1A                                                                                                             //JM LAYOUT 1
  gtk_fixed_move(GTK_FIXED(grid), lbl85F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL); //JM
  gtk_fixed_move(GTK_FIXED(grid), lbl85G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL); //JM
#endif //JM END OF LAYOUT 1
  gtk_widget_get_preferred_size(  lbl85Gr, NULL, &lblG);                                                                //JM ALPHA BLUE MENU LABELS
  gtk_fixed_move(GTK_FIXED(grid), lbl85Gr, (2*xPos+KEY_WIDTH_2-GAP-lblG.width+2)/2,                              yPos + GAP - Y_OFFSET_SHIFTED_LABEL);   //JM ALPHA BLUE MENU LABELS
 
}



void labelCaptionNormal(const calcKey_t *key, GtkWidget *button, GtkWidget *lblF, GtkWidget *lblG, GtkWidget *lblL) {
  uint8_t lbl[22];

  if(key->primary == 0) {
    lbl[0] = 0;
  }
  else {
    stringToUtf8(indexOfItems[max(key->primary, -key->primary)].itemPrinted, lbl);
  }

  if(strcmp((char *)lbl, "CAT") == 0 && key->keyId != 85) {    //JM wqs 85  //JM Changed CATALOG to CAT
    lbl[3] = 0;
  }

  gtk_button_set_label(GTK_BUTTON(button), (gchar *)lbl);

  if(strcmp((char *)lbl, "/") == 0 && key->keyId == 55) {    //JM if "/", re-do to "÷". Presumed easier than to fix the UTf8 conversion above.
  gtk_button_set_label(GTK_BUTTON(button), "÷");             //JM DIV
  }                                                          //JM

  if(key->primary == KEY_f) {
    gtk_widget_set_name(button, "calcKeyF");
  }
  else if(key->primary == KEY_g) {
    gtk_widget_set_name(button, "calcKeyG");
  }
  else if((key->primary >= CHR_0 && key->primary <= CHR_9) || key->primary == CHR_PERIOD) {
    gtk_widget_set_name(button, "calcNumericKey");
  }
  else if (strcmp((char *)lbl, "/") == 0 && key->keyId == 55) {     //JM increase the font size of the operators to the numeric key size
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if (strcmp((char *)lbl, "×") == 0 && key->keyId == 65) {     //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if (strcmp((char *)lbl, "-") == 0 && key->keyId == 75) {     //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if (strcmp((char *)lbl, "+") == 0 && key->keyId == 85) {     //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else {
    gtk_widget_set_name(button, "calcKey");
  }

  stringToUtf8(indexOfItems[max(key->fShifted, -key->fShifted)].itemPrinted, lbl);

  if(key->fShifted == 0) {
    lbl[0] = 0;
  }
  else if(strcmp((char *)lbl, "CAT") == 0 && key->keyId != 85) {   //JM was 85  //JM Changed CATALOG to CAT
    lbl[3] = 0;
  }

  gtk_label_set_label(GTK_LABEL(lblF), (gchar *)lbl);
  if(key->fShifted < 0) gtk_widget_set_name(lblF, "fShiftedUnderline"); else  gtk_widget_set_name(lblF, "fShifted");

  stringToUtf8(indexOfItems[max(key->gShifted, -key->gShifted)].itemPrinted, lbl);
  if(key->gShifted == 0) {
    lbl[0] = 0;
  }
  else if(strcmp((char *)lbl, "CAT") == 0 && key->keyId != 85) {  //JM was 85 //JM Changed CATALOG to CAT
    lbl[3] = 0;
  }

  gtk_label_set_label(GTK_LABEL(lblG), (gchar *)lbl);
  if(key->gShifted < 0) gtk_widget_set_name(lblG, "gShiftedUnderline"); else  gtk_widget_set_name(lblG, "gShifted");

  stringToUtf8(indexOfItems[key->primaryAim].itemPrinted, lbl);
  if(key->primaryAim == 0) {
    lbl[0] = 0;
  }

//  if(lbl[0] == ' ') {    //JM Space replace with lines below
//    lbl[0] = '_';
//  }

  if(lbl[0] == ' ') {     //JM SPACE |  OPEN BOX 9251,  0xE2 0x90 0xA3  |  0xE2 0x90 0xA0 for SP.
    lbl[0]=0xC2;          //JM SPACE the space character is not in the font. \rather use . . for space.
    lbl[1]=0xB7;          //JM SPACE          
    lbl[1]=0xB7;          //JM SPACE          
    lbl[2]=' ';           //JM SPACE
    lbl[3]=0xc2;          //JM SPACE
    lbl[4]=0xb7;          //JM SPACE
    lbl[5]=0;             //JM SPACE
  }                       //JM SPACE

  if(lbl[0] == 'Y') {                                         //JM Brighten up Y and N
    //lbl[0] = '#';                                             //JM Brighten up Y and N
    gtk_label_set_label(GTK_LABEL(lblL), (gchar *)lbl);//JM Brighten up Y and N
    gtk_widget_set_name(lblL, "On");                      //JM Brighten up Y and N
  } else                                                           //JM Brighten up Y and N
  if(lbl[0] == 'N') {                                         //JM Brighten up Y and N
    //lbl[0] = '#';                                             //JM Brighten up Y and N
    gtk_label_set_label(GTK_LABEL(lblL), (gchar *)lbl);//JM Brighten up Y and N
    gtk_widget_set_name(lblL, "On");                      //JM Brighten up Y and N
  } else                                                         //JM Brighten up Y and N
  {                                                              //JM Brighten up Y and N
  gtk_label_set_label(GTK_LABEL(lblL), (gchar *)lbl);
  gtk_widget_set_name(lblL, "letter");
  }                                                            //JM Brighten up Y and N

}



void labelCaptionAim(const calcKey_t *key, GtkWidget *button, GtkWidget *lblGreek, GtkWidget *lblL) {
  uint8_t lbl[22];

  if(key->keyLblAim == ITM_NULL) {
    lbl[0] = 0;
  }
  else {
    stringToUtf8(indexOfItems[max(key->keyLblAim, -key->keyLblAim)].itemPrinted, lbl);
  }

//JM remove this because CAT is not on keyboard anymore, but in menu
//JM  if(strcmp((char *)lbl, "CAT") == 0 && key->keyId != 85) {    //JM was 85  //JM Changed CATALOG to CAT. Actually not needed, as the WP43C already has a shortened CAT to start with
//JM    lbl[3] = 0;
//JM  }

  gtk_button_set_label(GTK_BUTTON(button), (gchar *)lbl);

//JM Removed this previously added part, as the items.c exchange in chr_divide made works here/.
//  if(strcmp((char *)lbl, "/") == 0 && key->keyId == 55) {    //JM if "/", re-do to "÷". Presumed easier than to fix the UTf8 conversion above.
//  gtk_button_set_label(GTK_BUTTON(button), "÷");             //JM DIV
//  }                                                          //JM

//Specify the different categories of coloured zones
  if(key->keyLblAim == KEY_f) {
    gtk_widget_set_name(button, "calcKeyF");
  }
  else if(key->keyLblAim == KEY_g) {
    gtk_widget_set_name(button, "calcKeyG");
  }
  else {
    if((key->fShiftedAim == key->keyLblAim || key->fShiftedAim == CHR_PROD_SIGN) && key->keyLblAim != ITM_NULL) {
      gtk_widget_set_name(button, "calcKeyGoldenBorder");           //JM note because of this golden box, the upsizing is not effective.
    }

  else if (strcmp((char *)lbl, "/") == 0 && key->keyId == 55) {     //JM increase the font size of the operators to the numeric key size
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if (strcmp((char *)lbl, "×") == 0 && key->keyId == 65) {     //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if (strcmp((char *)lbl, "-") == 0 && key->keyId == 75) {     //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if (strcmp((char *)lbl, "+") == 0 && key->keyId == 85) {     //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  
    else {
      gtk_widget_set_name(button, "calcKey");
    }
  }


//Convert Greek CAPITAL and LOWER case to UTF !
  if( (CHR_ALPHA <= key->gShiftedAim && key->gShiftedAim <= CHR_OMEGA) || (CHR_QOPPA <= key->gShiftedAim && key->gShiftedAim <= CHR_SAMPI) ) {   //JM GREEK. Add extra 36 char greek range
    stringToUtf8(indexOfItems[key->gShiftedAim].itemPrinted, lbl);
    lbl[2] = ' ';
    lbl[3] = 0;
    stringToUtf8(indexOfItems[key->gShiftedAim + 36].itemPrinted, lbl + 3);
  }
  else {
    stringToUtf8(indexOfItems[max(key->gShiftedAim, -key->gShiftedAim)].itemPrinted, lbl);
  }

//GShift set label
  if(key->gShiftedAim == 0) {
    lbl[0] = 0;
  }

//JM Removed as CAT is not Aim keyboard anymore
//  else if(strcmp((char *)lbl, "CAT") == 0 && key->keyId != 85) {    //JM was 85   //JM Changed CATALOG to CAT
//    lbl[3] = 0;
//  }

/* JM TEST PROCEDURE TO TEST DISPLAY
else if(key->gShiftedAim == CHR_DIGAMMA ) {   
    lbl[0] = 0xCF;
    lbl[1] = 0x9C;
    lbl[2] = 32;
    lbl[3] = 0xCF;
    lbl[4] = 0x9D;
    lbl[5] = 0;
  }
*/

  gtk_label_set_label(GTK_LABEL(lblGreek), (gchar *)lbl);

//GShift colours
  if(key->gShiftedAim < 0) {
    gtk_widget_set_name(lblGreek, "greekUnderline");
  }
  else {
    gtk_widget_set_name(lblGreek, "greek");
  }

//Primaries, convert to UTF
  if(key->primaryAim == 0) {
    lbl[0] = 0;
  }
  else {
   stringToUtf8(indexOfItems[key->primaryAim].itemPrinted, lbl);  //Convert to UTF !
  }

//  if(lbl[0] == ' ') {    //JM Space replace with lines below
//    lbl[0] = '_';
//  }

  if(lbl[0] == ' ') {     //JM SPACE |  OPEN BOX 9251,  0xE2 0x90 0xA3  |  0xE2 0x90 0xA0 for SP.
    lbl[0]=0xC2;          //JM SPACE the space character is not in the font. \rather use . . for space.
    lbl[1]=0xB7;          //JM SPACE          
    lbl[1]=0xB7;          //JM SPACE          
    lbl[2]=' ';           //JM SPACE
    lbl[3]=0xc2;          //JM SPACE
    lbl[4]=0xb7;          //JM SPACE
    lbl[5]=0;             //JM SPACE
  }                       //JM SPACE

  if(lbl[0] == 'Y') {                                         //JM Brighten up Y and N
    //lbl[0] = '#';                                             //JM Brighten up Y and N
    gtk_label_set_label(GTK_LABEL(lblL), (gchar *)lbl);//JM Brighten up Y and N
    gtk_widget_set_name(lblL, "On");                      //JM Brighten up Y and N
  } else                                                           //JM Brighten up Y and N
  if(lbl[0] == 'N') {                                         //JM Brighten up Y and N
    //lbl[0] = '#';                                             //JM Brighten up Y and N
    gtk_label_set_label(GTK_LABEL(lblL), (gchar *)lbl);//JM Brighten up Y and N
    gtk_widget_set_name(lblL, "On");                      //JM Brighten up Y and N
  } else                                                          //JM Brighten up Y and N
  {                                                              //JM Brighten up Y and N
  gtk_label_set_label(GTK_LABEL(lblL), (gchar *)lbl);
  gtk_widget_set_name(lblL, "letter");
  }                                                            //JM Brighten up Y and N
}



void labelCaptionTam(const calcKey_t *key, GtkWidget *button) {
  uint8_t lbl[22];

  if(key->primaryTam == 0 || (key->keyId == 32 && tamMode != TM_VALUE_CHB)) { // Display H for changing base to hexadecimal
    lbl[0] = 0;
  }
  else {
    stringToUtf8(indexOfItems[key->primaryTam].itemPrinted, lbl);
  }

  gtk_button_set_label(GTK_BUTTON(button), (gchar *)lbl);

  if(strcmp((char *)lbl, "/") == 0 && key->keyId == 55) {    //JM if "/", re-do to "÷". Presumed easier than to fix the UTf8 conversion above.
  gtk_button_set_label(GTK_BUTTON(button), "÷");             //JM DIV
  }                                                          //JM

  if(key->primaryTam == KEY_f) {
    gtk_widget_set_name(button, "calcKeyF");
  }
  else if(key->primaryTam == KEY_g) {
    gtk_widget_set_name(button, "calcKeyG");
  }

  else if (strcmp((char *)lbl, "/") == 0 && key->keyId == 55) {     //JM increase the font size of the operators to the numeric key size
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if (strcmp((char *)lbl, "×") == 0 && key->keyId == 65) {     //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if (strcmp((char *)lbl, "-") == 0 && key->keyId == 75) {     //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if (strcmp((char *)lbl, "+") == 0 && key->keyId == 85) {     //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  
  else {
    gtk_widget_set_name(button, "calcKey");
  }
}



void calcModeNormalGui(void) {
  const calcKey_t *keys;

  keys = userModeEnabled ? kbd_usr : kbd_std;

  hideAllWidgets();

  labelCaptionNormal(keys++, btn21, lbl21F, lbl21G, lbl21L);
  labelCaptionNormal(keys++, btn22, lbl22F, lbl22G, lbl22L);
  labelCaptionNormal(keys++, btn23, lbl23F, lbl23G, lbl23L);
  labelCaptionNormal(keys++, btn24, lbl24F, lbl24G, lbl24L);
  labelCaptionNormal(keys++, btn25, lbl25F, lbl25G, lbl25L);
  labelCaptionNormal(keys++, btn26, lbl26F, lbl26G, lbl26L);

  labelCaptionNormal(keys++, btn31, lbl31F, lbl31G, lbl31L);
  labelCaptionNormal(keys++, btn32, lbl32F, lbl32G, lbl32L);
  labelCaptionNormal(keys++, btn33, lbl33F, lbl33G, lbl33L);
  labelCaptionNormal(keys++, btn34, lbl34F, lbl34G, lbl34L);
  labelCaptionNormal(keys++, btn35, lbl35F, lbl35G, lbl35L);
  labelCaptionNormal(keys++, btn36, lbl36F, lbl36G, lbl36L);

  labelCaptionNormal(keys++, btn41, lbl41F, lbl41G, lbl41L);
  labelCaptionNormal(keys++, btn42, lbl42F, lbl42G, lbl42L);
  labelCaptionNormal(keys++, btn43, lbl43F, lbl43G, lbl43L);
  labelCaptionNormal(keys++, btn44, lbl44F, lbl44G, lbl44L);
  labelCaptionNormal(keys++, btn45, lbl45F, lbl45G, lbl45L);

  labelCaptionNormal(keys++, btn51, lbl51F, lbl51G, lbl51L);
  labelCaptionNormal(keys++, btn52, lbl52F, lbl52G, lbl52L);
  labelCaptionNormal(keys++, btn53, lbl53F, lbl53G, lbl53L);
  labelCaptionNormal(keys++, btn54, lbl54F, lbl54G, lbl54L);
  labelCaptionNormal(keys++, btn55, lbl55F, lbl55G, lbl55L);

  labelCaptionNormal(keys++, btn61, lbl61F, lbl61G, lbl61L);
  labelCaptionNormal(keys++, btn62, lbl62F, lbl62G, lbl62L);
  labelCaptionNormal(keys++, btn63, lbl63F, lbl63G, lbl63L);
  labelCaptionNormal(keys++, btn64, lbl64F, lbl64G, lbl64L);
  labelCaptionNormal(keys++, btn65, lbl65F, lbl65G, lbl65L);

  labelCaptionNormal(keys++, btn71, lbl71F, lbl71G, lbl71L);
  labelCaptionNormal(keys++, btn72, lbl72F, lbl72G, lbl72L);
  labelCaptionNormal(keys++, btn73, lbl73F, lbl73G, lbl73L);
  labelCaptionNormal(keys++, btn74, lbl74F, lbl74G, lbl74L);
  labelCaptionNormal(keys++, btn75, lbl75F, lbl75G, lbl75L);

  labelCaptionNormal(keys++, btn81, lbl81F, lbl81G, lbl81L);
  labelCaptionNormal(keys++, btn82, lbl82F, lbl82G, lbl82L);
  labelCaptionNormal(keys++, btn83, lbl83F, lbl83G, lbl83L);
  labelCaptionNormal(keys++, btn84, lbl84F, lbl84G, lbl84L);
  labelCaptionNormal(keys++, btn85, lbl85F, lbl85G, lbl85L);

  gtk_widget_show(btn11);
  gtk_widget_show(btn12);
  gtk_widget_show(btn13);
  gtk_widget_show(btn14);
  gtk_widget_show(btn15);
  gtk_widget_show(btn16);

  gtk_widget_show(btn21);
  gtk_widget_show(btn22);
  gtk_widget_show(btn23);
  gtk_widget_show(btn24);
  gtk_widget_show(btn25);
  gtk_widget_show(btn26);

  gtk_widget_show(lbl21F);
  gtk_widget_show(lbl21G);
  gtk_widget_show(lbl21L);
  gtk_widget_show(lbl22F);
  gtk_widget_show(lbl22G);
  gtk_widget_show(lbl22L);
  gtk_widget_show(lbl23F);
  gtk_widget_show(lbl23G);
  gtk_widget_show(lbl23L);
  gtk_widget_show(lbl24F);
  gtk_widget_show(lbl24G);
  gtk_widget_show(lbl24L);
  gtk_widget_show(lbl25F);
  gtk_widget_show(lbl25G);
  gtk_widget_show(lbl25L);
  gtk_widget_show(lbl26L);
  gtk_widget_show(lbl26F);
  gtk_widget_show(lbl26G);

  gtk_widget_show(btn31);
  gtk_widget_show(btn32);
  gtk_widget_show(btn33);
  gtk_widget_show(btn34);
  gtk_widget_show(btn35);
  gtk_widget_show(btn36);

  gtk_widget_show(lbl31F);
  gtk_widget_show(lbl31G);
  gtk_widget_show(lbl31L);
  gtk_widget_show(lbl32F);
  gtk_widget_show(lbl32G);
  gtk_widget_show(lbl32L);
  gtk_widget_show(lbl33F);
  gtk_widget_show(lbl33G);
  gtk_widget_show(lbl33H);
  gtk_widget_show(lbl33L);
  gtk_widget_show(lbl34F);
  gtk_widget_show(lbl34G);
  gtk_widget_show(lbl34H);
  gtk_widget_show(lbl34L);
//#ifdef JM_LAYOUT_2_DM42_STRICT //JM LAYOUT 2. DM42 STRICT.
  gtk_widget_show(lbl35L); // JM !!
  gtk_widget_show(lbl36L); // JM !!
//#endif //JM END OF LAYOUT 2 DM42 STRICT.

  gtk_widget_show(lbl35F); //JM3 SIN/COS/TAN to DM42
  gtk_widget_show(lbl35G); //JM3 SIN/COS/TAN to DM42
  gtk_widget_show(lbl36F); //JM3 SIN/COS/TAN to DM42
  gtk_widget_show(lbl36G); //JM3 SIN/COS/TAN to DM42

  gtk_widget_show(btn41);
  gtk_widget_show(btn42);
  gtk_widget_show(btn43);
  gtk_widget_show(btn44);
  gtk_widget_show(btn45);

  gtk_widget_show(lbl41F);
  gtk_widget_show(lbl41G);
  gtk_widget_show(lbl42F);
  gtk_widget_show(lbl42G);
  gtk_widget_show(lbl42H);
  gtk_widget_show(lbl42L);
  gtk_widget_show(lbl43F);
  gtk_widget_show(lbl43G);
  gtk_widget_show(lbl43L);
  gtk_widget_show(lbl43P);
  gtk_widget_show(lbl44F);
  gtk_widget_show(lbl44G);
  gtk_widget_show(lbl44L);
  gtk_widget_show(lbl44P);
  gtk_widget_show(lbl45F);
  gtk_widget_show(lbl45G);

  gtk_widget_show(btn51);
  gtk_widget_show(btn52);
  gtk_widget_show(btn53);
  gtk_widget_show(btn54);
  gtk_widget_show(btn55);

  gtk_widget_show(lbl51F);
  gtk_widget_show(lbl51G);
  gtk_widget_show(lbl51L);
  gtk_widget_show(lbl52F);
  gtk_widget_show(lbl52G);
  gtk_widget_show(lbl52L);
  gtk_widget_show(lbl53F);
  gtk_widget_show(lbl53G);
  gtk_widget_show(lbl53L);
  gtk_widget_show(lbl54F);
  gtk_widget_show(lbl54G);
  gtk_widget_show(lbl54L);
  gtk_widget_show(lbl55F);
  gtk_widget_show(lbl55G);
  gtk_widget_show(lbl55L);

  gtk_widget_show(btn61);
  gtk_widget_show(btn62);
  gtk_widget_show(btn63);
  gtk_widget_show(btn64);
  gtk_widget_show(btn65);

  gtk_widget_show(lbl61F);
  gtk_widget_show(lbl61G);
  gtk_widget_show(lbl61L);
  gtk_widget_show(lbl62F);
  gtk_widget_show(lbl62G);
  gtk_widget_show(lbl62L);
  gtk_widget_show(lbl63F);
  gtk_widget_show(lbl63G);
  gtk_widget_show(lbl63L);
  gtk_widget_show(lbl64F);
  gtk_widget_show(lbl64G);
  gtk_widget_show(lbl64L);
  gtk_widget_show(lbl65H); //JM
  gtk_widget_show(lbl65F);
  gtk_widget_show(lbl65G);
  gtk_widget_show(lbl65L); //JM added

  gtk_widget_show(btn71);
  gtk_widget_show(btn72);
  gtk_widget_show(btn73);
  gtk_widget_show(btn74);
  gtk_widget_show(btn75);

//  gtk_widget_show(lbl71F); //JM REMOVE SHIFT LABEL
//  gtk_widget_show(lbl71G); //JM REMOVE SHIFT LABEL
  gtk_widget_show(lbl71L);
  gtk_widget_show(lbl72H); //JM
  gtk_widget_show(lbl72F);
  gtk_widget_show(lbl72G);
  gtk_widget_show(lbl72L);
  gtk_widget_show(lbl73H); //JM
  gtk_widget_show(lbl73F);
  gtk_widget_show(lbl73G);
  gtk_widget_show(lbl73L);
  gtk_widget_show(lbl74F);
  gtk_widget_show(lbl74G);
  gtk_widget_show(lbl74L);
  gtk_widget_show(lbl75F);
  gtk_widget_show(lbl75G);
   gtk_widget_show(lbl75L); //JM asdded

 
  gtk_widget_show(btn81);
  gtk_widget_show(btn82);
  gtk_widget_show(btn83);
  gtk_widget_show(btn84);
  gtk_widget_show(btn85);

  gtk_widget_show(lbl81F);
  gtk_widget_show(lbl81G);
  gtk_widget_show(lbl81L);
  gtk_widget_show(lbl82F);
  gtk_widget_show(lbl82G);
  gtk_widget_show(lbl82H); //JM
  gtk_widget_show(lbl82L);
  gtk_widget_show(lbl83F);
  gtk_widget_show(lbl83G);
  gtk_widget_show(lbl83L);
  gtk_widget_show(lbl83H);
  gtk_widget_show(lbl84F);
  gtk_widget_show(lbl84G);
  gtk_widget_show(lbl84H);
  gtk_widget_show(lbl84L);
  gtk_widget_show(lbl85F);
  gtk_widget_show(lbl85G);
  gtk_widget_show(lbl85H); //JM
  gtk_widget_show(lbl85L);  //JM add ?

  gtk_widget_show(lblOn);
 //JM7  gtk_widget_show(lblConfirmY);  //JM Y/N
 //JM7  gtk_widget_show(lblConfirmN);  //JM Y/N

  moveLabels();
}



void calcModeAimGui(void) {
  const calcKey_t *keys;

  keys = userModeEnabled ? kbd_usr : kbd_std;

  hideAllWidgets();

  labelCaptionAim(keys++, btn21, lbl21Gr, lbl21L);
  labelCaptionAim(keys++, btn22, lbl22Gr, lbl22L);
  labelCaptionAim(keys++, btn23, lbl23Gr, lbl23L);
  labelCaptionAim(keys++, btn24, lbl24Gr, lbl24L);
  labelCaptionAim(keys++, btn25, lbl25Gr, lbl25L);
  labelCaptionAim(keys++, btn26, lbl26Gr, lbl26L);

  labelCaptionAim(keys++, btn31, lbl31Gr, lbl31L);
  labelCaptionAim(keys++, btn32, lbl32Gr, lbl32L);
  labelCaptionAim(keys++, btn33, lbl33Gr, lbl33L);
  labelCaptionAim(keys++, btn34, lbl34Gr, lbl34L);
  labelCaptionAim(keys++, btn35, lbl35Gr, lbl35L);
  labelCaptionAim(keys++, btn36, lbl36Gr, lbl36L);

  labelCaptionAim(keys++, btn41, lbl41Gr, lbl41L);
  labelCaptionAim(keys++, btn42, lbl42Gr, lbl42L);
  labelCaptionAim(keys++, btn43, lbl43Gr, lbl43L);
  labelCaptionAim(keys++, btn44, lbl44Gr, lbl44L);
  labelCaptionAim(keys++, btn45, lbl45Gr, lbl45L);

  labelCaptionAim(keys++, btn51, lbl51Gr, lbl51L);
  labelCaptionAim(keys++, btn52, lbl52Gr, lbl52L);
  labelCaptionAim(keys++, btn53, lbl53Gr, lbl53L);
  labelCaptionAim(keys++, btn54, lbl54Gr, lbl54L);
  labelCaptionAim(keys++, btn55, lbl55Gr, lbl55L);

  labelCaptionAim(keys++, btn61, lbl61Gr, lbl61L);
  labelCaptionAim(keys++, btn62, lbl62Gr, lbl62L);
  labelCaptionAim(keys++, btn63, lbl63Gr, lbl63L);
  labelCaptionAim(keys++, btn64, lbl64Gr, lbl64L);
  labelCaptionAim(keys++, btn65, lbl65Gr, lbl65L);

  labelCaptionAim(keys++, btn71, lbl71Gr, lbl71L);
  labelCaptionAim(keys++, btn72, lbl72Gr, lbl72L);
  labelCaptionAim(keys++, btn73, lbl73Gr, lbl73L);
  labelCaptionAim(keys++, btn74, lbl74Gr, lbl74L);
  labelCaptionAim(keys++, btn75, lbl75Gr, lbl75L);

  labelCaptionAim(keys++, btn81, lbl81Gr, lbl81L);
  labelCaptionAim(keys++, btn82, lbl82Gr, lbl82L);
  labelCaptionAim(keys++, btn83, lbl83Gr, lbl83L);
  labelCaptionAim(keys++, btn84, lbl84Gr, lbl84L);
  labelCaptionAim(keys++, btn85, lbl85Gr, lbl85L);

  gtk_widget_show(lblOn);
  //JM7 gtk_widget_show(lblConfirmY);  //JM Y/N
  //JM7 gtk_widget_show(lblConfirmN);  //JM Y/N


  gtk_widget_show(btn11);
  gtk_widget_show(btn12);
  gtk_widget_show(btn13);
  gtk_widget_show(btn14);
  gtk_widget_show(btn15);
  gtk_widget_show(btn16);

  gtk_widget_show(btn21);
  gtk_widget_show(btn22);
  gtk_widget_show(btn23);
  gtk_widget_show(btn24);
  gtk_widget_show(btn25);
  gtk_widget_show(btn26);

  gtk_widget_show(lbl21L);
  gtk_widget_show(lbl22L);
  gtk_widget_show(lbl23L);
  gtk_widget_show(lbl24L);
  gtk_widget_show(lbl25L);
  gtk_widget_show(lbl26L);
  //gtk_widget_show(lbl26F);
  //gtk_widget_show(lbl26G);
  gtk_widget_show(lbl21Gr);
  gtk_widget_show(lbl22Gr);
  gtk_widget_show(lbl23Gr);
  gtk_widget_show(lbl24Gr);
  gtk_widget_show(lbl25Gr);
  gtk_widget_show(lbl26Gr);

  gtk_widget_show(btn31);
  gtk_widget_show(btn32);
  gtk_widget_show(btn33);
  gtk_widget_show(btn34);
  gtk_widget_show(btn35);
  gtk_widget_show(btn36);

  //gtk_widget_show(lbl31F);  JM
  gtk_widget_show(lbl31L);
  gtk_widget_show(lbl32L);
  gtk_widget_show(lbl33H);
  gtk_widget_show(lbl33L);
  gtk_widget_show(lbl34L);
//#ifdef JM_LAYOUT_2_DM42_STRICT //JM LAYOUT 2. DM42 STRICT.
  gtk_widget_show(lbl35L); // JM !!
  gtk_widget_show(lbl36L); // JM !!
//#endif //JM END OF LAYOUT 2 DM42 STRICT.
  gtk_widget_show(lbl34H);  //JM reinstate CAPS
  gtk_widget_show(lbl31Gr);
  gtk_widget_show(lbl32Gr);
  gtk_widget_show(lbl33Gr);
  gtk_widget_show(lbl34Gr);
  gtk_widget_show(lbl35Gr); //JM !! GR
  gtk_widget_show(lbl36Gr); //JM !! GR

  gtk_widget_show(btn41);
  gtk_widget_show(btn42);
  gtk_widget_show(btn43);
  gtk_widget_show(btn44);
  gtk_widget_show(btn45);

  //gtk_widget_show(lbl41F);
  //gtk_widget_show(lbl41G);
  gtk_widget_show(lbl42L);
  gtk_widget_show(lbl43L);
  gtk_widget_show(lbl43P);
  gtk_widget_show(lbl44L);
  gtk_widget_show(lbl44P);
  gtk_widget_show(lbl45F);
  gtk_widget_show(lbl45G);
  gtk_widget_show(lbl42Gr);
  gtk_widget_show(lbl43Gr);
  gtk_widget_show(lbl44Gr);

  gtk_widget_show(btn51);
  gtk_widget_show(btn52);
  gtk_widget_show(btn53);
  gtk_widget_show(btn54);
  gtk_widget_show(btn55);

  gtk_widget_show(lbl51L);
  gtk_widget_show(lbl51F);
  //gtk_widget_show(lbl51G); //JM__
  //gtk_widget_show(lbl55F); //JM__
  //gtk_widget_show(lbl55G); //JM__
  gtk_widget_show(lbl52L);
  gtk_widget_show(lbl53L);
  gtk_widget_show(lbl54L);
  gtk_widget_show(lbl55L);
  //gtk_widget_show(lbl51Gr);
  gtk_widget_show(lbl52Gr);
  gtk_widget_show(lbl53Gr);
  gtk_widget_show(lbl54Gr);
  gtk_widget_show(lbl55Gr);   //JM GREEK

  gtk_widget_show(btn61);
  gtk_widget_show(btn62);
  gtk_widget_show(btn63);
  gtk_widget_show(btn64);
  gtk_widget_show(btn65);

  gtk_widget_show(lbl61L);
  gtk_widget_show(lbl62L);
  gtk_widget_show(lbl63L);
  gtk_widget_show(lbl64L);
  gtk_widget_show(lbl65L); //JM added

  gtk_widget_show(lbl61F); //JM_
  //gtk_widget_show(lbl61G); //JM_
  //gtk_widget_show(lbl65F); //JM
  //gtk_widget_show(lbl65G);
  gtk_widget_show(lbl61Gr);
  gtk_widget_show(lbl62Gr);
  gtk_widget_show(lbl63Gr);
  gtk_widget_show(lbl64Gr);
  gtk_widget_show(lbl65Gr); //JM ==

  gtk_widget_show(btn71);
  gtk_widget_show(btn72);
  gtk_widget_show(btn73);
  gtk_widget_show(btn74);
  gtk_widget_show(btn75);

  gtk_widget_show(lbl71L);
  gtk_widget_show(lbl72L);
  gtk_widget_show(lbl73L);
  gtk_widget_show(lbl74L);
  gtk_widget_show(lbl75L); //JM added
  //gtk_widget_show(lbl71F);  //JM_          //JM REMOVE SHIFT LABEL
  //gtk_widget_show(lbl75G); //JM_
  gtk_widget_show(lbl71Gr);
  gtk_widget_show(lbl72Gr);
  gtk_widget_show(lbl73Gr);
  gtk_widget_show(lbl74Gr);
  gtk_widget_show(lbl75Gr); //JM ==

  gtk_widget_show(btn81);
  gtk_widget_show(btn82);
  gtk_widget_show(btn83);
  gtk_widget_show(btn84);
  gtk_widget_show(btn85);

  gtk_widget_show(lbl81L);
  gtk_widget_show(lbl81F); //JM added OFF with Layout2
  gtk_widget_show(lbl82L);
  gtk_widget_show(lbl83L);
  gtk_widget_show(lbl84L);
  gtk_widget_show(lbl82H);  //JM AIM MENU
  gtk_widget_show(lbl83H);  //JM AIM MENU
  gtk_widget_show(lbl84H);  //JM AIM MENU
  gtk_widget_show(lbl85H);  //JM AIM MENU
  //gtk_widget_show(lbl85F); //JM
  //gtk_widget_show(lbl85G); //JM
  gtk_widget_show(lbl85L);  //JM add ?
  
  gtk_widget_show(lbl82Gr);
  gtk_widget_show(lbl83Gr);
  gtk_widget_show(lbl84Gr); //JM TT
  gtk_widget_show(lbl85Gr); //JM
  
  gtk_widget_show(lblOn);
  //JM7 gtk_widget_show(lblConfirmY);  //JM Y/N
  //JM7 gtk_widget_show(lblConfirmN);  //JM Y/N

//  gtk_widget_show(lbl31G);   //JM Remnanty from 43S. Not sure why. Removed.
  moveLabels();
//  gtk_widget_hide(lbl31G);   //JM Remnanty from 43S. Not sure why. Removed.
}



void calcModeTamGui(void) {
  const calcKey_t *keys;

  keys = userModeEnabled ? kbd_usr : kbd_std;

  hideAllWidgets();

  labelCaptionTam(keys++, btn21);
  labelCaptionTam(keys++, btn22);
  labelCaptionTam(keys++, btn23);
  labelCaptionTam(keys++, btn24);
  labelCaptionTam(keys++, btn25);
  labelCaptionTam(keys++, btn26);

  labelCaptionTam(keys++, btn31);
  labelCaptionTam(keys++, btn32);
  labelCaptionTam(keys++, btn33);
  labelCaptionTam(keys++, btn34);
  labelCaptionTam(keys++, btn35);
  labelCaptionTam(keys++, btn36);

  labelCaptionTam(keys++, btn41);
  labelCaptionTam(keys++, btn42);
  labelCaptionTam(keys++, btn43);
  labelCaptionTam(keys++, btn44);
  labelCaptionTam(keys++, btn45);

  labelCaptionTam(keys++, btn51);
  labelCaptionTam(keys++, btn52);
  labelCaptionTam(keys++, btn53);
  labelCaptionTam(keys++, btn54);
  labelCaptionTam(keys++, btn55);

  labelCaptionTam(keys++, btn61);
  labelCaptionTam(keys++, btn62);
  labelCaptionTam(keys++, btn63);
  labelCaptionTam(keys++, btn64);
  labelCaptionTam(keys++, btn65);

  labelCaptionTam(keys++, btn71);
  labelCaptionTam(keys++, btn72);
  labelCaptionTam(keys++, btn73);
  labelCaptionTam(keys++, btn74);
  labelCaptionTam(keys++, btn75);

  labelCaptionTam(keys++, btn81);
  labelCaptionTam(keys++, btn82);
  labelCaptionTam(keys++, btn83);
  labelCaptionTam(keys++, btn84);
  labelCaptionTam(keys++, btn85);

  hideAllWidgets();

  gtk_widget_show(btn11);
  gtk_widget_show(btn12);
  gtk_widget_show(btn13);
  gtk_widget_show(btn14);
  gtk_widget_show(btn15);
  gtk_widget_show(btn16);

  gtk_widget_show(btn21);
  gtk_widget_show(btn22);
  gtk_widget_show(btn23);
  gtk_widget_show(btn24);
  gtk_widget_show(btn25);
  gtk_widget_show(btn26);

  gtk_widget_show(btn31);
  gtk_widget_show(btn32);
  gtk_widget_show(btn33);
  gtk_widget_show(btn34);
  gtk_widget_show(btn35);
  gtk_widget_show(btn36);

  gtk_widget_show(btn41);
  gtk_widget_show(btn42);
  gtk_widget_show(btn43);
  gtk_widget_show(btn44);
  gtk_widget_show(btn45);

  gtk_widget_show(btn51);
  gtk_widget_show(btn52);
  gtk_widget_show(btn53);
  gtk_widget_show(btn54);
  gtk_widget_show(btn55);

  gtk_widget_show(btn61);
  gtk_widget_show(btn62);
  gtk_widget_show(btn63);
  gtk_widget_show(btn64);
  gtk_widget_show(btn65);

  gtk_widget_show(btn71);
  gtk_widget_show(btn72);
  gtk_widget_show(btn73);
  gtk_widget_show(btn74);
  gtk_widget_show(btn75);

  gtk_widget_show(btn81);
  gtk_widget_show(btn82);
  gtk_widget_show(btn83);
  gtk_widget_show(btn84);
  gtk_widget_show(btn85);

  moveLabels();
}



void configureCallback(GtkWindow *window, GdkEvent *event, gpointer data) {
  allowScreenUpdate = false;
  //printf("x=%d y=%d\n", event->configure.x, event->configure.y);
 }



/********************************************//**
 * \brief Creates the calc's GUI window with all the widgets
 *
 * \param void
 * \return void
 ***********************************************/
void setupUI(void) {
  int            numBytes, xPos, yPos;
  GError         *error;
  GtkCssProvider *cssProvider;
  GdkDisplay     *cssDisplay;
  GdkScreen      *cssScreen;

  prepareCssData();

  cssProvider = gtk_css_provider_new();
  cssDisplay  = gdk_display_get_default();
  cssScreen   = gdk_display_get_default_screen(cssDisplay);
  gtk_style_context_add_provider_for_screen(cssScreen, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

  error = NULL;
  gtk_css_provider_load_from_data(cssProvider, cssData, -1, &error);
  if(error != NULL) {
    showInfoDialog("In function setupUI:", "error while loading CSS style sheet " CSSFILE, NULL, NULL);
    exit(1);
  }
  g_object_unref(cssProvider);
  free(cssData);

  // Get the monitor geometry to determine whether the calc is portrait or landscape
  GdkRectangle monitor;
  gdk_monitor_get_geometry(gdk_display_get_monitor(gdk_display_get_default(), 0), &monitor);
  //gdk_screen_get_monitor_geometry(gdk_screen_get_default(), 0, &monitor);

  if(calcAutoLandscapePortrait) {
    calcLandscape = (monitor.height <  1025);
  }

  // The main window
  frmCalc = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  if(calcLandscape) {
    #if (DEBUG_PANEL == 1)
      gtk_window_set_default_size(GTK_WINDOW(frmCalc), 1000, 540);
      debugWidgetDx = 0;
      debugWidgetDy = 545;
    #else
      gtk_window_set_default_size(GTK_WINDOW(frmCalc), 1000, 540);
    #endif
  }
  else {
    #if (DEBUG_PANEL == 1)
      gtk_window_set_default_size(GTK_WINDOW(frmCalc),  1530, 980);
      debugWidgetDx = 531;
      debugWidgetDy = 0;
    #else
      gtk_window_set_default_size(GTK_WINDOW(frmCalc),  526, 980);
    #endif
  }

  gtk_widget_set_name(frmCalc, "mainWindow");
  gtk_window_set_resizable (GTK_WINDOW(frmCalc), FALSE);
  gtk_window_set_title(GTK_WINDOW(frmCalc), "WP 43C");                   //JM NAME
  g_signal_connect(frmCalc, "destroy", G_CALLBACK(destroyCalc), NULL);
  g_signal_connect(frmCalc, "key_press_event", G_CALLBACK(keyPressed), NULL);

  gtk_widget_add_events(GTK_WIDGET(frmCalc), GDK_CONFIGURE);
  g_signal_connect(G_OBJECT(frmCalc), "configure-event", G_CALLBACK(configureCallback), NULL);

  // Fixed grid to freely put widgets on it
  grid = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(frmCalc), grid);



  // Backround image
  if(calcLandscape) {
    backgroundImage = gtk_image_new_from_file("dm42lshort.png");
  }
  else {

#ifdef JM_LAYOUT_2_DM42_STRICT //JM L2
    backgroundImage = gtk_image_new_from_file("dm42l_L2.png");
#endif //JM Layout
#if defined(JM_LAYOUT_1A) //JM LAYOUT 1
    backgroundImage = gtk_image_new_from_file("dm42l.png");
#endif //JM Layout

  }

  gtk_fixed_put(GTK_FIXED(grid), backgroundImage,  0, 0);

#if defined(JM_LAYOUT_1A)  //JM LAYOUT 1. FINAL. Show colour band next to LCD
  // Areas for the g shifted softkeys
  lblGSoftkeyArea = gtk_label_new("");
  gtk_widget_set_name(lblGSoftkeyArea, "gSoftkeyArea");
  gtk_widget_set_size_request(lblGSoftkeyArea, 438, 24);
  gtk_fixed_put(GTK_FIXED(grid), lblGSoftkeyArea, 44, 72+170);

  // Area for the f shifted softkeys
  lblFSoftkeyArea = gtk_label_new("");
  gtk_widget_set_name(lblFSoftkeyArea, "fSoftkeyArea");
  gtk_widget_set_size_request(lblFSoftkeyArea, 438, 24);
  gtk_fixed_put(GTK_FIXED(grid), lblFSoftkeyArea, 44, 72+170+24);
#endif //JM


  // LCD screen 400x240
  screen = gtk_drawing_area_new();
  gtk_widget_set_size_request(screen, SCREEN_WIDTH, SCREEN_HEIGHT);
  gtk_widget_set_tooltip_text(GTK_WIDGET(screen), "h(ardcopy) copies screen image to clipboard");
  gtk_fixed_put(GTK_FIXED(grid), screen, 63, 72);
  screenStride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, SCREEN_WIDTH)/4;
  numBytes = screenStride * SCREEN_HEIGHT * 4;
  screenData = malloc(numBytes);
  if(screenData == NULL) {
    sprintf(errorMessage, "error allocating %d x %d = %d bytes for screenData", screenStride*4, SCREEN_HEIGHT, numBytes);
    showInfoDialog("In function setupUI:", errorMessage, NULL, NULL);
    exit(1);
  }

  clearScreen(true, true, true);
  g_signal_connect(screen, "draw", G_CALLBACK(drawScreen), NULL);


  #if (DEBUG_REGISTER_L == 1)
    lblRegisterL1 = gtk_label_new("");
    lblRegisterL2 = gtk_label_new("");
    gtk_widget_set_name(lblRegisterL1, "registerL");
    gtk_widget_set_name(lblRegisterL2, "registerL");
    gtk_fixed_put(GTK_FIXED(grid), lblRegisterL1, 30, 28);
    gtk_fixed_put(GTK_FIXED(grid), lblRegisterL2, 30, 46);
  #endif

  #if (SHOW_MEMORY_STATUS == 1)
    lblMemoryStatus = gtk_label_new("");
    gtk_widget_set_name(lblMemoryStatus, "memoryStatus");
    gtk_fixed_put(GTK_FIXED(grid), lblMemoryStatus, 5, 5);
  #endif

  // 1st row: F1 to F6 buttons
  btn11 = gtk_button_new_with_label(" ");        //JM I prefer blank to "^"
  btn12 = gtk_button_new_with_label(" ");        //JM I prefer blank to "^"
  btn13 = gtk_button_new_with_label(" ");        //JM I prefer blank to "^"
  btn14 = gtk_button_new_with_label(" ");        //JM I prefer blank to "^"
  btn15 = gtk_button_new_with_label(" ");        //JM I prefer blank to "^"
  btn16 = gtk_button_new_with_label(" ");        //JM I prefer blank to "^"

  gtk_widget_set_tooltip_text(GTK_WIDGET(btn11), "F1");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn12), "F2");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn13), "F3");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn14), "F4");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn15), "F5");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn16), "F6");

  gtk_widget_set_size_request(btn11, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn12, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn13, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn14, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn15, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn16, KEY_WIDTH_1, 0);

  gtk_widget_set_name(btn11, "calcKey");
  gtk_widget_set_name(btn12, "calcKey");
  gtk_widget_set_name(btn13, "calcKey");
  gtk_widget_set_name(btn14, "calcKey");
  gtk_widget_set_name(btn15, "calcKey");
  gtk_widget_set_name(btn16, "calcKey");

  g_signal_connect(btn11, "clicked", G_CALLBACK(btnFnClicked), "1");
  g_signal_connect(btn12, "clicked", G_CALLBACK(btnFnClicked), "2");
  g_signal_connect(btn13, "clicked", G_CALLBACK(btnFnClicked), "3");
  g_signal_connect(btn14, "clicked", G_CALLBACK(btnFnClicked), "4");
  g_signal_connect(btn15, "clicked", G_CALLBACK(btnFnClicked), "5");
  g_signal_connect(btn16, "clicked", G_CALLBACK(btnFnClicked), "6");

  xPos = X_LEFT_PORTRAIT;
  yPos = Y_TOP_PORTRAIT;
  gtk_fixed_put(GTK_FIXED(grid), btn11, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn12, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn13, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn14, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn15, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn16, xPos, yPos);


  // 2nd row
  btn21   = gtk_button_new();
  btn22   = gtk_button_new();
  btn23   = gtk_button_new();
  btn24   = gtk_button_new();
  btn25   = gtk_button_new();
  btn26   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn21), "a"); //dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn22), "v"); //dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn23), "q"); //dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn24), "o"); //dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn25), "l"); //dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn26), "x"); //dr
  lbl21F  = gtk_label_new("");
  lbl22F  = gtk_label_new("");
  lbl23F  = gtk_label_new("");
  lbl24F  = gtk_label_new("");
  lbl25F  = gtk_label_new("");
  lbl26F  = gtk_label_new("");
  lbl21G  = gtk_label_new("");
  lbl22G  = gtk_label_new("");
  lbl23G  = gtk_label_new("");
  lbl24G  = gtk_label_new("");
  lbl25G  = gtk_label_new("");
  lbl26G  = gtk_label_new("");
  lbl21L  = gtk_label_new("");
  lbl22L  = gtk_label_new("");
  lbl23L  = gtk_label_new("");
  lbl24L  = gtk_label_new("");
  lbl25L  = gtk_label_new("");
  lbl26L  = gtk_label_new("");
  lbl21Gr = gtk_label_new("");
  lbl22Gr = gtk_label_new("");
  lbl23Gr = gtk_label_new("");
  lbl24Gr = gtk_label_new("");
  lbl25Gr = gtk_label_new("");
  lbl26Gr = gtk_label_new("");

  gtk_widget_set_size_request(btn21, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn22, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn23, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn24, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn25, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn26, KEY_WIDTH_1, 0);

  g_signal_connect(btn21, "pressed", G_CALLBACK(btnPressed), "00");
  g_signal_connect(btn22, "pressed", G_CALLBACK(btnPressed), "01");
  g_signal_connect(btn23, "pressed", G_CALLBACK(btnPressed), "02");
  g_signal_connect(btn24, "pressed", G_CALLBACK(btnPressed), "03");
  g_signal_connect(btn25, "pressed", G_CALLBACK(btnPressed), "04");
  g_signal_connect(btn26, "pressed", G_CALLBACK(btnPressed), "05");
  g_signal_connect(btn21, "released", G_CALLBACK(btnReleased), "00");
  g_signal_connect(btn22, "released", G_CALLBACK(btnReleased), "01");
  g_signal_connect(btn23, "released", G_CALLBACK(btnReleased), "02");
  g_signal_connect(btn24, "released", G_CALLBACK(btnReleased), "03");
  g_signal_connect(btn25, "released", G_CALLBACK(btnReleased), "04");
  g_signal_connect(btn26, "released", G_CALLBACK(btnReleased), "05");

  gtk_fixed_put(GTK_FIXED(grid), lbl21F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl22F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl23F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl24F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl25F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl26F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl21G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl22G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl23G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl24G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl25G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl26G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl21Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl22Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl23Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl24Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl25Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl26Gr, 0, 0);

  if(calcLandscape) {
    xPos = X_LEFT_LANDSCAPE;
    yPos = Y_TOP_LANDSCAPE;
  }
  else {
    xPos = X_LEFT_PORTRAIT;
    yPos += DELTA_KEYS_Y;
  }

  gtk_fixed_put(GTK_FIXED(grid), btn21,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl21L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn22,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl22L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn23,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl23L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn24,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl24L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn25,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl25L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn26,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl26L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);



  // 3rd row
  btn31   = gtk_button_new();
  btn32   = gtk_button_new();
  btn33   = gtk_button_new();
  btn34   = gtk_button_new();
  btn35   = gtk_button_new();
  btn36   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn31), "m"); //dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn32), "r"); //dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn33), "d"); //dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn34), "s"); //dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn35), "c"); //dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn36), "t"); //dr
  lbl31F  = gtk_label_new("");
  lbl32F  = gtk_label_new("");
  lbl33F  = gtk_label_new("");
  lbl34F  = gtk_label_new("");
  lbl35F  = gtk_label_new("");
  lbl36F  = gtk_label_new("");
  lbl31G  = gtk_label_new("");
  lbl32G  = gtk_label_new("");
  lbl33G  = gtk_label_new("");
  lbl34G  = gtk_label_new("");
  lbl35G  = gtk_label_new("");
  lbl36G  = gtk_label_new("");
  lbl33H  = gtk_label_new("\u21e9"); // Hollow down
  lbl34H  = gtk_label_new("\u2102"); //JM CAPS LOCK                          // ("\u03b7"); // eta          //JM removed1
//JM  lbl35H  = gtk_label_new("");                                               // ("\u03b7"); // eta          //JM removed1

  lbl31L  = gtk_label_new(""); 
  lbl32L  = gtk_label_new("");
  lbl33L  = gtk_label_new("");
  lbl34L  = gtk_label_new("");
  lbl35L  = gtk_label_new("");
  lbl36L  = gtk_label_new("");

  lbl31Gr = gtk_label_new("");
  lbl32Gr = gtk_label_new("");
  lbl33Gr = gtk_label_new("");
  lbl34Gr = gtk_label_new("");
  lbl35Gr = gtk_label_new("");
  lbl36Gr = gtk_label_new("");

  gtk_widget_set_size_request(btn31, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn32, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn33, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn34, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn35, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn36, KEY_WIDTH_1, 0);

  gtk_widget_set_name(lbl33H,  "fShifted");
  gtk_widget_set_name(lbl34H,  "fShifted");  //JM CAPS
  // gtk_widget_set_name(lbl34H,  "gShifted");   //JM removed1

  g_signal_connect(btn31, "pressed", G_CALLBACK(btnPressed), "06");
  g_signal_connect(btn32, "pressed", G_CALLBACK(btnPressed), "07");
  g_signal_connect(btn33, "pressed", G_CALLBACK(btnPressed), "08");
  g_signal_connect(btn34, "pressed", G_CALLBACK(btnPressed), "09");
  g_signal_connect(btn35, "pressed", G_CALLBACK(btnPressed), "10");
  g_signal_connect(btn36, "pressed", G_CALLBACK(btnPressed), "11");
  g_signal_connect(btn31, "released", G_CALLBACK(btnReleased), "06");
  g_signal_connect(btn32, "released", G_CALLBACK(btnReleased), "07");
  g_signal_connect(btn33, "released", G_CALLBACK(btnReleased), "08");
  g_signal_connect(btn34, "released", G_CALLBACK(btnReleased), "09");
  g_signal_connect(btn35, "released", G_CALLBACK(btnReleased), "10");
  g_signal_connect(btn36, "released", G_CALLBACK(btnReleased), "11");

  gtk_fixed_put(GTK_FIXED(grid), lbl31F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl32F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl33F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl34F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl35F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl36F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl31G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl32G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl33G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl34G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl35G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl36G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl31Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl32Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl33Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl34Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl35Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl36Gr, 0, 0);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y;
  gtk_fixed_put(GTK_FIXED(grid), btn31,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl31L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn32,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl32L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn33,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl33L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl33H, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn34,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl34L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl34H, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);               //JM CAPS

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn35,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl35L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn36,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl36L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);



  // 4th row
  btn41   = gtk_button_new();
  btn42   = gtk_button_new();
  btn43   = gtk_button_new();
  btn44   = gtk_button_new();
  btn45   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn41), "Enter");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn42), "w"); //dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn43), "n"); //dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn44), "e"); //dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn45), "Backspace");
  lbl41F  = gtk_label_new("");
  lbl42F  = gtk_label_new("");
  lbl43F  = gtk_label_new("");
  lbl44F  = gtk_label_new("");
  lbl45F  = gtk_label_new("");
  lbl41G  = gtk_label_new("");
  lbl42G  = gtk_label_new("");
  lbl43G  = gtk_label_new("");
  lbl44G  = gtk_label_new("");
  lbl45G  = gtk_label_new("");
  lbl42H  = gtk_label_new("");
  lbl43P  = gtk_label_new("");
  lbl44P  = gtk_label_new("\u21e7"); // Hollow up
  lbl41L  = gtk_label_new("");
  lbl42L  = gtk_label_new("");
  lbl43L  = gtk_label_new("");
  lbl44L  = gtk_label_new("");
  lbl45L  = gtk_label_new("");
  lbl41Gr = gtk_label_new("");
  lbl42Gr = gtk_label_new("");
  lbl43Gr = gtk_label_new("");
  lbl44Gr = gtk_label_new("");
  lbl45Gr = gtk_label_new("");

  gtk_widget_set_size_request(btn41, KEY_WIDTH_1 + DELTA_KEYS_X, 0);
  gtk_widget_set_size_request(btn42, KEY_WIDTH_1,              0);
  gtk_widget_set_size_request(btn43, KEY_WIDTH_1,              0);
  gtk_widget_set_size_request(btn44, KEY_WIDTH_1,              0);
  gtk_widget_set_size_request(btn45, KEY_WIDTH_1,              0);

  gtk_widget_set_name(lbl43P,  "fShifted");
  gtk_widget_set_name(lbl44P,  "fShifted");

  g_signal_connect(btn41, "pressed", G_CALLBACK(btnPressed), "12");
  g_signal_connect(btn42, "pressed", G_CALLBACK(btnPressed), "13");
  g_signal_connect(btn43, "pressed", G_CALLBACK(btnPressed), "14");
  g_signal_connect(btn44, "pressed", G_CALLBACK(btnPressed), "15");
  g_signal_connect(btn45, "pressed", G_CALLBACK(btnPressed), "16");
  g_signal_connect(btn41, "released", G_CALLBACK(btnReleased), "12");
  g_signal_connect(btn42, "released", G_CALLBACK(btnReleased), "13");
  g_signal_connect(btn43, "released", G_CALLBACK(btnReleased), "14");
  g_signal_connect(btn44, "released", G_CALLBACK(btnReleased), "15");
  g_signal_connect(btn45, "released", G_CALLBACK(btnReleased), "16");

  gtk_fixed_put(GTK_FIXED(grid), lbl41F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl42F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl43F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl44F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl45F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl41G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl42G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl43G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl44G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl45G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl41Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl42Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl43Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl44Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl45Gr, 0, 0);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y;
  gtk_fixed_put(GTK_FIXED(grid), btn41,  xPos,                          yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl41L, xPos + KEY_WIDTH_1 + DELTA_KEYS_X + 4, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X*2;
  gtk_fixed_put(GTK_FIXED(grid), btn42,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl42L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl42H, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn43,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl43L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl43P, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn44,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl44L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl44P, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn45,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl45L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);



  // 5th row
  btn51   = gtk_button_new();
  btn52   = gtk_button_new();
  btn53   = gtk_button_new();
  btn54   = gtk_button_new();
  btn55   = gtk_button_new(); 
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn51), "Up"); //JM
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn52), "7");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn53), "8");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn54), "9");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn55), "/"); //JM
  lbl51F  = gtk_label_new("");
  lbl52F  = gtk_label_new("");
  lbl53F  = gtk_label_new("");
  lbl54F  = gtk_label_new("");
  lbl55F  = gtk_label_new("");
  lbl51G  = gtk_label_new("");
  lbl52G  = gtk_label_new("");
  lbl53G  = gtk_label_new("");
  lbl54G  = gtk_label_new("");
  lbl55G  = gtk_label_new("");
  lbl51L  = gtk_label_new("");
  lbl52L  = gtk_label_new("");
  lbl53L  = gtk_label_new("");
  lbl54L  = gtk_label_new("");
  lbl55L  = gtk_label_new("");
  lbl51Gr = gtk_label_new("");
  lbl52Gr = gtk_label_new("");
  lbl53Gr = gtk_label_new("");
  lbl54Gr = gtk_label_new("");
  lbl55Gr = gtk_label_new("");

  gtk_widget_set_size_request(btn51, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn52, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn53, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn54, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn55, KEY_WIDTH_2, 0);

  g_signal_connect(btn51, "pressed", G_CALLBACK(btnPressed), "17");
  g_signal_connect(btn52, "pressed", G_CALLBACK(btnPressed), "18");
  g_signal_connect(btn53, "pressed", G_CALLBACK(btnPressed), "19");
  g_signal_connect(btn54, "pressed", G_CALLBACK(btnPressed), "20");
  g_signal_connect(btn55, "pressed", G_CALLBACK(btnPressed), "21");
  g_signal_connect(btn51, "released", G_CALLBACK(btnReleased), "17");
  g_signal_connect(btn52, "released", G_CALLBACK(btnReleased), "18");
  g_signal_connect(btn53, "released", G_CALLBACK(btnReleased), "19");
  g_signal_connect(btn54, "released", G_CALLBACK(btnReleased), "20");
  g_signal_connect(btn55, "released", G_CALLBACK(btnReleased), "21");

  gtk_fixed_put(GTK_FIXED(grid), lbl51F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl52F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl53F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl54F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl55F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl51G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl52G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl53G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl54G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl55G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl51Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl52Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl53Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl54Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl55Gr, 0, 0);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_fixed_put(GTK_FIXED(grid), btn51,  xPos,                         yPos);
  //gtk_fixed_put(GTK_FIXED(grid), lbl51L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);  //JM remove arrow in text

  xPos += DELTA_KEYS_X + 18;
  gtk_fixed_put(GTK_FIXED(grid), btn52,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl52L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn53,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl53L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn54,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl54L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn55,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl55L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);



  // 6th row
  btn61   = gtk_button_new();
  btn62   = gtk_button_new();
  btn63   = gtk_button_new();
  btn64   = gtk_button_new();
  btn65   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn61), "Down"); //JM
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn62), "4");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn63), "5");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn64), "6");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn65), "*"); //JM
  lbl61F  = gtk_label_new("");
  lbl62F  = gtk_label_new("");
  lbl63F  = gtk_label_new("");
  lbl64F  = gtk_label_new("");
  lbl65F  = gtk_label_new("");
  lbl61G  = gtk_label_new("");
  lbl62G  = gtk_label_new("");
  lbl63G  = gtk_label_new("");
  lbl64G  = gtk_label_new("");
  lbl65G  = gtk_label_new("");
  lbl65H  = gtk_label_new("");          // "\u03b8");  //JM.   //JM Removed1
  lbl61L  = gtk_label_new("");
  lbl62L  = gtk_label_new("");
  lbl63L  = gtk_label_new("");
  lbl64L  = gtk_label_new("");
  lbl65L  = gtk_label_new("");
  lbl61Gr = gtk_label_new("");
  lbl62Gr = gtk_label_new("");
  lbl63Gr = gtk_label_new("");
  lbl64Gr = gtk_label_new("");
  lbl65Gr = gtk_label_new("");

  gtk_widget_set_size_request(btn61, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn62, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn63, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn64, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn65, KEY_WIDTH_2, 0);

  gtk_widget_set_name(lbl65H,  "gShifted");  //JM

  g_signal_connect(btn61, "pressed", G_CALLBACK(btnPressed), "22");
  g_signal_connect(btn62, "pressed", G_CALLBACK(btnPressed), "23");
  g_signal_connect(btn63, "pressed", G_CALLBACK(btnPressed), "24");
  g_signal_connect(btn64, "pressed", G_CALLBACK(btnPressed), "25");
  g_signal_connect(btn65, "pressed", G_CALLBACK(btnPressed), "26");
  g_signal_connect(btn61, "released", G_CALLBACK(btnReleased), "22");
  g_signal_connect(btn62, "released", G_CALLBACK(btnReleased), "23");
  g_signal_connect(btn63, "released", G_CALLBACK(btnReleased), "24");
  g_signal_connect(btn64, "released", G_CALLBACK(btnReleased), "25");
  g_signal_connect(btn65, "released", G_CALLBACK(btnReleased), "26");

  gtk_fixed_put(GTK_FIXED(grid), lbl61F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl62F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl63F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl64F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl65F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl61G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl62G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl63G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl64G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl65G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl61Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl62Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl63Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl64Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl65Gr, 0, 0);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_fixed_put(GTK_FIXED(grid), btn61,  xPos,                         yPos);
  //gtk_fixed_put(GTK_FIXED(grid), lbl61L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);  //JM

  xPos += DELTA_KEYS_X + 18;
  gtk_fixed_put(GTK_FIXED(grid), btn62,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl62L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn63,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl63L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn64,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl64L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
 // gtk_fixed_put(GTK_FIXED(grid), lbl64H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1);  //JM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn65,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl65L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl65H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1); //JM



  // 7th row
  btn71   = gtk_button_new();
  btn72   = gtk_button_new();
  btn73   = gtk_button_new();
  btn74   = gtk_button_new();
  btn75   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn71), "Shift"); //JM //jm shortcut
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn72), "1");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn73), "2");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn74), "3");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn75), "-"); //JM
  lbl71F  = gtk_label_new("");
  lbl72F  = gtk_label_new("");
  lbl73F  = gtk_label_new("");
  lbl74F  = gtk_label_new("");
  lbl75F  = gtk_label_new("");
  lbl71G  = gtk_label_new("");
  lbl72G  = gtk_label_new("");
  lbl73G  = gtk_label_new("");
  lbl74G  = gtk_label_new("");
  lbl75G  = gtk_label_new("");
  lbl72H  = gtk_label_new("");      //M Menu alphaMATH for AIM //JM REMOVED
  lbl73H  = gtk_label_new("");  //"\u03c8"); // psi  //JM REMOVED
  lbl71L  = gtk_label_new("");
  lbl72L  = gtk_label_new("");
  lbl73L  = gtk_label_new("");
  lbl74L  = gtk_label_new("");
  lbl75L  = gtk_label_new("");
  lbl71Gr = gtk_label_new("");
  lbl72Gr = gtk_label_new("");
  lbl73Gr = gtk_label_new("");
  lbl74Gr = gtk_label_new("");
  lbl75Gr = gtk_label_new("");

  gtk_widget_set_size_request(btn71, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn72, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn73, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn74, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn75, KEY_WIDTH_2, 0);

  //JM Removed1 gtk_widget_set_name(lbl72H,  "gShiftedUnderline"); //JM
  //JM Removed1   gtk_widget_set_name(lbl73H,  "gShifted");  //JM

  g_signal_connect(btn71, "pressed", G_CALLBACK(btnPressed), "27");
  g_signal_connect(btn72, "pressed", G_CALLBACK(btnPressed), "28");
  g_signal_connect(btn73, "pressed", G_CALLBACK(btnPressed), "29");
  g_signal_connect(btn74, "pressed", G_CALLBACK(btnPressed), "30");
  g_signal_connect(btn75, "pressed", G_CALLBACK(btnPressed), "31");
  g_signal_connect(btn71, "released", G_CALLBACK(btnReleased), "27");
  g_signal_connect(btn72, "released", G_CALLBACK(btnReleased), "28");
  g_signal_connect(btn73, "released", G_CALLBACK(btnReleased), "29");
  g_signal_connect(btn74, "released", G_CALLBACK(btnReleased), "30");
  g_signal_connect(btn75, "released", G_CALLBACK(btnReleased), "31");

  gtk_fixed_put(GTK_FIXED(grid), lbl71F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl72F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl73F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl74F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl75F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl71G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl72G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl73G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl74G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl75G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl71Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl72Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl73Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl74Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl75Gr, 0, 0);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_fixed_put(GTK_FIXED(grid), btn71,  xPos,                         yPos);
  //gtk_fixed_put(GTK_FIXED(grid), lbl71L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER); //JM
  //gtk_fixed_put(GTK_FIXED(grid), lbl71H, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1); //JM

  xPos += DELTA_KEYS_X + 18;
  gtk_fixed_put(GTK_FIXED(grid), btn72,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl72L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  //gtk_fixed_put(GTK_FIXED(grid), lbl72H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1); //JM
  gtk_fixed_put(GTK_FIXED(grid), lbl72H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1); //JM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn73,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl73L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl73H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1); //JM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn74,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl74L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn75,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl75L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);



  // 8th row
  btn81   = gtk_button_new();
  btn82   = gtk_button_new();
  btn83   = gtk_button_new();
  btn84   = gtk_button_new();
  btn85   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn81), "Esc");  //JM
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn82), "0");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn83), ". ,");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn84), "\\");   //JM Changed from Ctrl to backslash 92
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn85), "+");    //JM
  lbl81F  = gtk_label_new("");
  lbl82F  = gtk_label_new("");
  lbl83F  = gtk_label_new("");
  lbl84F  = gtk_label_new("");
  lbl85F  = gtk_label_new("");
  lbl81G  = gtk_label_new("");
  lbl82G  = gtk_label_new("");
  lbl83G  = gtk_label_new("");
  lbl84G  = gtk_label_new("");
  lbl85G  = gtk_label_new("");
  lbl82H  = gtk_label_new("\u221D");        //JM ALPHA
  lbl83H  = gtk_label_new("\u221D\u2218");  //JM Alphadot -  Menu alphaDOT for AIM
  lbl84H  = gtk_label_new("\u221D\u222E");  //JM Alpha MATH - also considered pi \u03C0
  lbl85H  = gtk_label_new("\u221D\u00f1");  //JM Alpha Intnl - also considered \u2139
  lbl81L  = gtk_label_new("");
  lbl82L  = gtk_label_new("");
  lbl83L  = gtk_label_new("");
  lbl84L  = gtk_label_new("");
  lbl85L  = gtk_label_new("");
  lbl81Gr = gtk_label_new("");
  lbl82Gr = gtk_label_new("");
  lbl83Gr = gtk_label_new("");
  lbl84Gr = gtk_label_new("");
  lbl85Gr = gtk_label_new("");
  lblOn   = gtk_label_new("ON");

  gtk_widget_set_size_request(btn81, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn82, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn83, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn84, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn85, KEY_WIDTH_2, 0);

  gtk_widget_set_name(lbl82H, "greekUnderline");    //JM was gShiftedUnderline, changed to greekUnderline, x4
  gtk_widget_set_name(lbl83H, "greekUnderline");
  gtk_widget_set_name(lbl84H, "greekUnderline");
  gtk_widget_set_name(lbl85H, "greekUnderline");    //JM
  gtk_widget_set_name(lblOn,  "On");

  g_signal_connect(btn81, "pressed", G_CALLBACK(btnPressed), "32");
  g_signal_connect(btn82, "pressed", G_CALLBACK(btnPressed), "33");
  g_signal_connect(btn83, "pressed", G_CALLBACK(btnPressed), "34");
  g_signal_connect(btn84, "pressed", G_CALLBACK(btnPressed), "35");
  g_signal_connect(btn85, "pressed", G_CALLBACK(btnPressed), "36");
  g_signal_connect(btn81, "released", G_CALLBACK(btnReleased), "32");
  g_signal_connect(btn82, "released", G_CALLBACK(btnReleased), "33");
  g_signal_connect(btn83, "released", G_CALLBACK(btnReleased), "34");
  g_signal_connect(btn84, "released", G_CALLBACK(btnReleased), "35");
  g_signal_connect(btn85, "released", G_CALLBACK(btnReleased), "36");

  gtk_fixed_put(GTK_FIXED(grid), lbl81F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl82F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl83F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl84F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl85F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl81G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl82G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl83G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl84G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl85G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl81Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl82Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl83Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl84Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl85Gr, 0, 0);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_fixed_put(GTK_FIXED(grid), btn81,  xPos,                         yPos);
  //gtk_fixed_put(GTK_FIXED(grid), lbl81L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);   //JM REMOVED Superfluous EXIT in Gr
  //gtk_fixed_put(GTK_FIXED(grid), lbl81H, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);  //JM
  //gtk_fixed_move(GTK_FIXED(grid), lbl81G, xPos+KEY_WIDTH_1+ X_OFFSET_LETTER, yPos + 38); //JM+++ REMOVED AGAIN. OFF IS MANUALLY INSERTED SOMEHOW
  gtk_fixed_put(GTK_FIXED(grid), lblOn,   0, 0);     //JM Removed ON to 81

  xPos += DELTA_KEYS_X + 18;
  gtk_fixed_put(GTK_FIXED(grid), btn82,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl82L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl82H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1);                //JM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn83,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl83L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl83H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn84,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl84L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl84H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn85,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl85L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl85H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1);  //JM


  // gtk_fixed_put(GTK_FIXED(grid), lblOn,   0, 0);     //JM Removed ON to 81

  // The debug window
  #if (DEBUG_PANEL == 1)
    for(int i=0; i<DEBUG_LINES; i++) {
      lbl1[i] = gtk_label_new("");
      gtk_widget_set_name(lbl1[i], "debugDejaVu");
      gtk_fixed_put(GTK_FIXED(grid), lbl1[i], 1 + debugWidgetDx, 26 + i*14 + debugWidgetDy);
      lbl2[i] = gtk_label_new("");
      gtk_widget_set_name(lbl2[i], "debugWP43S");
      gtk_fixed_put(GTK_FIXED(grid), lbl2[i], 270 + debugWidgetDx, 25 + i*14 + debugWidgetDy);
    }

    btnBitFields                 = gtk_button_new_with_label("Bitfields");
    btnFlags                     = gtk_button_new_with_label("Flags");
    btnRegisters                 = gtk_button_new_with_label("Registers");
    btnLocalRegisters            = gtk_button_new_with_label("Local registers");
    btnStatisticalSums           = gtk_button_new_with_label("Statistical sums");
    btnNamedVariables            = gtk_button_new_with_label("Named registers");
    btnTmpAndSavedStackRegisters = gtk_button_new_with_label("Tmp & saved stack registers");
    chkHexaString                = gtk_check_button_new_with_label("Strings in hexadecimal form");

    gtk_widget_set_name(btnBitFields,                 "debugButton");
    gtk_widget_set_name(btnFlags,                     "debugButton");
    gtk_widget_set_name(btnRegisters,                 "debugButton");
    gtk_widget_set_name(btnLocalRegisters,            "debugButton");
    gtk_widget_set_name(btnStatisticalSums,           "debugButton");
    gtk_widget_set_name(btnNamedVariables,            "debugButton");
    gtk_widget_set_name(btnTmpAndSavedStackRegisters, "debugButton");
    gtk_widget_set_name(chkHexaString,                "debugCheckbox");

    g_signal_connect(btnBitFields,                 "clicked", G_CALLBACK(btnBitFieldsClicked),                 NULL);
    g_signal_connect(btnFlags,                     "clicked", G_CALLBACK(btnFlagsClicked),                     NULL);
    g_signal_connect(btnRegisters,                 "clicked", G_CALLBACK(btnRegistersClicked),                 NULL);
    g_signal_connect(btnLocalRegisters,            "clicked", G_CALLBACK(btnLocalRegistersClicked),            NULL);
    g_signal_connect(btnStatisticalSums,           "clicked", G_CALLBACK(btnStatisticalSumsClicked),           NULL);
    g_signal_connect(btnNamedVariables,            "clicked", G_CALLBACK(btnNamedVariablesClicked),            NULL);
    g_signal_connect(btnTmpAndSavedStackRegisters, "clicked", G_CALLBACK(btnTmpAndSavedStackRegistersClicked), NULL);
    g_signal_connect(chkHexaString,                "clicked", G_CALLBACK(chkHexaStringClicked),                NULL);

    gtk_fixed_put(GTK_FIXED(grid), btnBitFields,                   1 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnFlags,                      60 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnRegisters,                 101 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnLocalRegisters,            166 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnStatisticalSums,           260 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnNamedVariables,            360 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnTmpAndSavedStackRegisters, 465 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), chkHexaString,                630 + debugWidgetDx, 1 + debugWidgetDy);

    gtk_widget_show(btnBitFields);
    gtk_widget_show(btnFlags);
    gtk_widget_show(btnRegisters);
    gtk_widget_show(btnLocalRegisters);
    gtk_widget_show(btnStatisticalSums);
    gtk_widget_show(btnNamedVariables);
    gtk_widget_show(btnTmpAndSavedStackRegisters);
    gtk_widget_show(chkHexaString);

    debugWindow = DBG_REGISTERS;
  #endif

  gtk_widget_show_all(frmCalc);
}
#endif



#ifndef TESTSUITE_BUILD
void fnOff(uint16_t unsuedParamButMandatory) {
  resetShiftState();

  #ifdef PC_BUILD
    saveCalc();
    gtk_main_quit();
  #endif

  #ifdef DMCP_BUILD
    SET_ST(STAT_PGM_END);
  #endif
}



/********************************************//**
 * \brief Sets the calc mode to normal
 *
 * \param void
 * \return void
 ***********************************************/
void calcModeNormal(void) {
  if(calcMode == CM_TAM) {
    popSoftmenu();
    refreshRegisterLine(TAM_REGISTER_LINE);
    STACK_LIFT_ENABLE;
  }

  calcMode = CM_NORMAL;
  hideCursor();
  cursorEnabled = false;

  #ifdef PC_BUILD
    calcModeNormalGui();
  #endif
}



/********************************************//**
 * \brief Sets the calc mode to alpha input mode
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void calcModeAIM(uint16_t unusedParamButMandatory) {
  saveStack();
  liftStack();
  refreshStack();
  showSoftmenu(NULL, -MNU_MyAlpha, true);
//  showSoftmenu(NULL, -MNU_ALPHA, true);       //JM ALPHA. Disabled temporarily. First need to check the stack of pop on pop offs, when not called by keyboard
  calcMode = CM_AIM;
  alphaCase = AC_UPPER;
  showAlphaMode();
  nextChar = NC_NORMAL;

  clearRegisterLine(Y_POSITION_OF_AIM_LINE - 4, REGISTER_LINE_HEIGHT);
  xCursor = 1;
  yCursor = Y_POSITION_OF_AIM_LINE + 6;
  cursorFont = CF_STANDARD;
  cursorEnabled = true;

  #ifdef PC_BUILD
    calcModeAimGui();
  #endif
}



/********************************************//**
 * \brief Sets the calc mode to number input mode
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void calcModeNIM(uint16_t unusedParamButMandatory) {
  saveStack();

  calcMode = CM_NIM;

  liftStack();
  real16Zero(REGISTER_REAL16_DATA(REGISTER_X));
  refreshStack();

  nimBuffer[0] = 0;
  hexDigits = 0;

  clearRegisterLine(Y_POSITION_OF_NIM_LINE - 4, REGISTER_LINE_HEIGHT);
  xCursor = 1;
  yCursor = Y_POSITION_OF_NIM_LINE;
  cursorFont = CF_NUMERIC;
  cursorEnabled = true;
}



/********************************************//**
 * \brief Sets the calc mode to temporary alpha mode
 *
 * \param[in] tamMode int16_t    TAM mode
 *                               TAM       = general operation code
 *                               TAMCMP    = comparison operationcode
 *                               TAMSTORCL = STO or RCL operation code
 * \param[in] opCode const char* Operation code
 * \param[in] minN int16_t       Min value in TAM mode
 * \param[in] maxN int16_t       Max value in TAM mode
 * \return void
 ***********************************************/
void calcModeTAM(void) {
  if(calcMode == CM_NIM) {
    closeNim();
  }

  if(tamMode == TM_VALUE || tamMode == TM_VALUE_CHB || tamMode == TM_REGISTER || tamMode == TM_FLAG) {
    showSoftmenu(NULL, -MNU_TAM, true);
  }
  else if(tamMode == TM_CMP) {
    showSoftmenu(NULL, -MNU_TAMCMP, true);
  }
  else if(tamMode == TM_STORCL) {
    showSoftmenu(NULL, -MNU_TAMSTORCL, true);
  }
  else {
    sprintf(errorMessage, "In function calcModeTAM: %" FMT8U " is an unexpected value for tamMode!", tamMode);
    displayBugScreen(errorMessage);
    return;
  }

  calcMode = CM_TAM;

  strcat(tamBuffer, " __");
  if(stringWidth(tamBuffer, &standardFont, true, true) + 1 + lineTWidth > SCREEN_WIDTH) {
    clearRegisterLine(Y_POSITION_OF_REGISTER_T_LINE - 4, REGISTER_LINE_HEIGHT);
  }
  showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE + 6, vmNormal, true, true);

  transitionSystemState = 0;

  #ifdef PC_BUILD
    calcModeTamGui();
  #endif
}
#endif
