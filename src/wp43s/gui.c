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

#include "gui.h"

#include "bufferize.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "items.h"
#include "keyboard.h"
#include "registers.h"
#include "saveRestoreCalcState.h"
#include "screen.h"
#include "stack.h"
#include <string.h>
#include "typeDefinitions.h"

#include "wp43s.h"

#ifdef PC_BUILD
  GtkWidget *grid;
  #if (SCREEN_800X480 == 0)
    GtkWidget *backgroundImage;
#ifdef S43
    GtkWidget *lblFSoftkeyArea, *lblGSoftkeyArea;
#endif
GtkWidget *lblFKey2;
GtkWidget *lblGKey2;
//GtkWidget *lblEKey;
//GtkWidget *lblEEKey;
//GtkWidget *lblSKey;
    GtkWidget *lblBehindScreen;

    GtkWidget *btn11,   *btn12,   *btn13,   *btn14,   *btn15,   *btn16;
    GtkWidget *btn21,   *btn22,   *btn23,   *btn24,   *btn25,   *btn26;
GtkWidget *lbl21F,  *lbl22F,  *lbl23F,  *lbl24F,  *lbl25F,  *lbl26F;
GtkWidget *lbl21G,  *lbl22G,  *lbl23G,  *lbl24G,  *lbl25G,  *lbl26G;
GtkWidget *lbl21L,  *lbl22L,  *lbl23L,  *lbl24L,  *lbl25L,  *lbl26L;
GtkWidget *lbl21H;
GtkWidget *lbl21Gr, *lbl22Gr, *lbl23Gr, *lbl24Gr, *lbl25Gr, *lbl26Gr;
GtkWidget *btn21A,  *btn22A,  *btn23A,  *btn24A,  *btn25A,  *btn26A;    //dr - new AIM
GtkWidget *lbl21Fa, *lbl22Fa, *lbl23Fa, *lbl24Fa, *lbl25Fa, *lbl26Fa;                                 //JM

    GtkWidget *btn31,   *btn32,   *btn33,   *btn34,   *btn35,   *btn36;
GtkWidget *lbl31F,  *lbl32F,  *lbl33F,  *lbl34F,  *lbl35F,  *lbl36F;
GtkWidget *lbl31G,  *lbl32G,  *lbl33G,  *lbl34G,  *lbl35G,  *lbl36G;
GtkWidget *lbl31L,  *lbl32L,  *lbl33L,  *lbl34L,  *lbl35L,  *lbl36L; 
//GtkWidget                     *lbl33H;                                  //JMALPHA2 Removed lbl34H, to be replaced with lbl34Fa
GtkWidget *lbl31Gr, *lbl32Gr, *lbl33Gr, *lbl34Gr, *lbl35Gr, *lbl36Gr;
GtkWidget *btn31A,  *btn32A,  *btn33A,  *btn34A,  *btn35A,  *btn36A;    //dr - new AIM
GtkWidget *lbl31Fa, *lbl32Fa, *lbl33Fa,  *lbl34Fa, *lbl35Fa, *lbl36Fa;                                 //JMALPHA2

    GtkWidget *btn41,   *btn42,   *btn43,   *btn44,   *btn45;
GtkWidget *lbl41F,  *lbl42F,  *lbl43F,  *lbl44F,  *lbl45F;
GtkWidget *lbl41G,  *lbl42G,  *lbl43G,  *lbl44G,  *lbl45G;
GtkWidget *lbl41L,  *lbl42L,  *lbl43L,  *lbl44L,  *lbl45L;
GtkWidget           *lbl42H,  *lbl43P;//,  *lbl44P;
GtkWidget *lbl41Gr, *lbl42Gr, *lbl43Gr, *lbl44Gr, *lbl45Gr;
GtkWidget           *btn42A,  *btn43A,  *btn44A;                        //vv dr - new AIM
GtkWidget *lbl41Fa, *lbl42Fa, *lbl43Fa, *lbl44Fa, *lbl45Fa;                                 //^^

    GtkWidget *btn51,   *btn52,   *btn53,   *btn54,   *btn55;
GtkWidget *lbl51F,  *lbl52F,  *lbl53F,  *lbl54F,  *lbl55F;
GtkWidget *lbl51G,  *lbl52G,  *lbl53G,  *lbl54G,  *lbl55G;
GtkWidget *lbl51L,  *lbl52L,  *lbl53L,  *lbl54L,  *lbl55L;
GtkWidget *lbl51Gr, *lbl52Gr, *lbl53Gr, *lbl54Gr, *lbl55Gr;
GtkWidget           *btn52A,  *btn53A,  *btn54A,  *btn55A;              //vv dr - new AIM
GtkWidget           *lbl52Fa, *lbl53Fa, *lbl54Fa, *lbl55Fa;             //^^

    GtkWidget *btn61,   *btn62,   *btn63,   *btn64,   *btn65;
GtkWidget *lbl61F,  *lbl62F,  *lbl63F,  *lbl64F,  *lbl65F;
GtkWidget *lbl61G,  *lbl62G,  *lbl63G,  *lbl64G,  *lbl65G;
GtkWidget *lbl61L,  *lbl62L,  *lbl63L,  *lbl64L,  *lbl65L;
GtkWidget                                         *lbl65H;  //JM
GtkWidget *lbl61Gr, *lbl62Gr, *lbl63Gr, *lbl64Gr, *lbl65Gr;
GtkWidget           *btn62A,  *btn63A,  *btn64A,  *btn65A;              //vv dr - new AIM
GtkWidget           *lbl62Fa, *lbl63Fa, *lbl64Fa, *lbl65Fa;             //^^

    GtkWidget *btn71,   *btn72,   *btn73,   *btn74,   *btn75;
GtkWidget *lbl71F,  *lbl72F,  *lbl73F,  *lbl74F,  *lbl75F;
GtkWidget *lbl71G,  *lbl72G,  *lbl73G,  *lbl74G,  *lbl75G;
GtkWidget *lbl71L,  *lbl72L,  *lbl73L,  *lbl74L,  *lbl75L;
GtkWidget           *lbl72H,  *lbl73H;                      //JM
GtkWidget *lbl71Gr, *lbl72Gr, *lbl73Gr, *lbl74Gr, *lbl75Gr;
GtkWidget           *btn72A,  *btn73A,  *btn74A,  *btn75A;              //vv dr - new AIM
GtkWidget           *lbl72Fa, *lbl73Fa, *lbl74Fa, *lbl75Fa;             //^^

    GtkWidget *btn81,   *btn82,   *btn83,   *btn84,   *btn85;
GtkWidget *lbl81F,  *lbl82F,  *lbl83F,  *lbl84F,  *lbl85F;
GtkWidget *lbl81G,  *lbl82G,  *lbl83G,  *lbl84G,  *lbl85G;
GtkWidget *lbl81L,  *lbl82L,  *lbl83L,  *lbl84L,  *lbl85L;
GtkWidget           *lbl82H,  *lbl83H,  *lbl84H,  *lbl85H;  //JM
GtkWidget *lbl81Gr, *lbl82Gr, *lbl83Gr, *lbl84Gr, *lbl85Gr;
GtkWidget           *btn82A,  *btn83A,  *btn84A,  *btn85A;              //vv dr - new AIM
GtkWidget           *lbl82Fa, *lbl83Fa, *lbl84Fa, *lbl85Fa;             //^^
//GtkWidget *lblOn; //JM
//JM7 GtkWidget  *lblConfirmY; //JM for Y/N
//JM7 GtkWidget  *lblConfirmN; //JM for Y/N

    #if (DEBUG_PANEL == 1)
      GtkWidget *lbl1[DEBUG_LINES], *lbl2[DEBUG_LINES];
      GtkWidget *btnBitFields, *btnFlags, *btnRegisters, *btnLocalRegisters, *btnStatisticalSums, *btnNamedVariables, *btnSavedStackRegisters;
      GtkWidget *chkHexaString;
      int16_t debugWidgetDx, debugWidgetDy;
    #endif // (DEBUG_PANEL == 1)

    char *cssData;
  #endif // (SCREEN_800X480 == 0)



  static gint destroyCalc(GtkWidget* w, GdkEventAny* e, gpointer data) {
    saveCalc();
    gtk_main_quit();

    return 0;
  }


 void btn_Clicked_Gen(bool_t shF, bool_t shG, char *st) {
    GtkWidget *w;
    w = NULL;
    shiftF = shF;
    shiftG = shG;
    uint8_t alphaCase_MEM = alphaCase;
    bool_t numLock_MEM;  numLock_MEM = numLock;  numLock = false;
    bool_t u_mem = getSystemFlag(FLAG_USER); clearSystemFlag(FLAG_USER);
    btnClicked(w, st);
    if(u_mem) setSystemFlag(FLAG_USER);
    numLock = numLock_MEM;
    alphaCase = alphaCase_MEM;
    refreshStatusBar();
  }



//JM Lower case alpha letters from PC --> produce letters in the current case.
//JM Upper case alpha letters from PC --> change case and produce letter. Restore case.


//JM ALPHA SECTION FOR ALPHAMODE - LOWER CASE PC LETTER INPUT. USE LETTER
void btnClicked_LC(GtkWidget *w, gpointer data) {
  bool_t numLock_MEM;
  numLock_MEM = numLock;
  numLock = false;
  btnClicked(w, data);
  numLock = numLock_MEM;
  refreshStatusBar();
}


//JM ALPHA SECTION FOR ALPHAMODE -  UPPER CASE PC LETTER INPUT. INVERT 43C CASE. USE LETTER.
void btnClicked_UC(GtkWidget *w, gpointer data) {
  uint8_t alphaCase_MEM;
  bool_t numLock_MEM;
  alphaCase_MEM = alphaCase;
  numLock_MEM = numLock;
  if(alphaCase == AC_UPPER) {alphaCase = AC_LOWER;}
  else if(alphaCase == AC_LOWER) {alphaCase = AC_UPPER;}
  numLock = false;
  btnClicked(w, data);
  alphaCase = alphaCase_MEM;
  numLock = numLock_MEM;
  refreshStatusBar();
}


//JM NUMERIC SECTION FOR ALPHAMODE - FORCE Numeral - Numbers from PC --> produce numbers.
void btnClicked_NU(GtkWidget *w, gpointer data) {
  bool_t numLock_MEM;
  bool_t jm_GGREEK_MEM;
  numLock_MEM = numLock;
  jm_GGREEK_MEM = jm_GGREEK;

  numLock = false;
  shiftF = false;       //JM
  shiftG = true;      //JM
  btnClicked(w, data);

  numLock = numLock_MEM;
  jm_GGREEK = jm_GGREEK_MEM;
  refreshStatusBar();
}

//Shifted numbers !@#$%^&*() from PC --> activate shift and use numnber 1234567890. Restore case.
void btnClicked_SNU(GtkWidget *w, gpointer data) {
  bool_t numLock_MEM;
  bool_t jm_GGREEK_MEM;
  numLock_MEM = numLock;
  jm_GGREEK_MEM = jm_GGREEK;

  numLock = false;
  shiftF = true;       //JM
  shiftG = false;        //JM
//  btnClicked(NULL, "34");     //Alphadot
  btnClicked(w, data);

  //Only : is working at this point
  numLock = numLock_MEM;
  jm_GGREEK = jm_GGREEK_MEM;
  refreshStatusBar();
}


uint32_t CTRL_State, event_keyval;


  gboolean keyReleased(GtkWidget *w, GdkEventKey *event, gpointer data) {     //JM
    //printf("Released %d\n", event->keyval);

    switch(event->keyval) {

      case 65507: // left Ctrl
      case 65508: // right Ctrl
        //printf("key depressed: CTRL De-activate\n");
        CTRL_State = 0;
        break;


      case 65470: // F1                      //JM Added this portion to be able to go to NOP on emulator
        //printf("key pressed: F1\n");
        btnFnClickedR(w, "1");
        break;

      case 65471: // F2
        //printf("key pressed: F2\n");
        btnFnClickedR(w, "2");
        break;

      case 65472: // F3
        //printf("key pressed: F3\n");
        btnFnClickedR(w, "3");
        break;

      case 65473: // F4
        //printf("key pressed: F4\n");
        btnFnClickedR(w, "4");
        break;

      case 65474: // F5
        //printf("key pressed: F5\n");
        btnFnClickedR(w, "5");
        break;

      case 65475: // F6
        //printf("key pressed: F6\n");
        btnFnClickedR(w, "6");
        break;

    case 65505: // left shift  //JM Added this portion to be able to repeat f key on emulator
      //printf("key pressed: Shift\n"); //dr
      btnClickedR(w, "27");
      break;


    default:
      break;

  }
  return FALSE;
}                                                                           //JM


gboolean keyPressed(GtkWidget *w, GdkEventKey *event, gpointer data) {
  //printf("Pressed %d\n", event->keyval);                                  //JM
  event_keyval = event->keyval + CTRL_State;
  //printf("#######%d\n",event_keyval);

//JM ALPHA SECTION FOR ALPHAMODE - TAKE OVER ALPHA KEYBOARD
if (calcMode == CM_AIM || tam.mode) {
switch (event_keyval) {

    case 65507: // left Ctrl
    case 65508: // right Ctrl
      //printf("key pressed: CTRL Activated\n");
      CTRL_State = 65536;
      break;
    case 72+65536: // Ctrl H
    case 104+65536: // Ctrl h
      printf("key pressed: CTRL+h Hardcopy\n");
      copyScreenToClipboard();
      break;

//ROW 0
    case 65362:                                               //JM     // CursorUp //JM
      btnFnClicked(w, "1");
      break;
    case 65361:                                               //JM     // CursorLt BST //JM
      btnFnClicked(w, "5");
      break;
    case 65364:                                               //JM     // CursorDown //JM
      btnFnClicked(w, "2");
      break;
    case 65363:                                               //JM     // CursorRt SST //JM
      btnFnClicked(w, "6");
      break;


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
    case 65439:
    case 65535: // Delete
      fnT_ARROW(ITM_T_RIGHT_ARROW);
      btnClicked(w, "16");
      break;
//ROW 5
    case 65360:                                               //JM     // HOME  //JM
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
    case 65367:                                               //JM     // END  //JM
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
    case 65463: // 7
    case 48+7:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM     //**************-- NUM KEYS  --***************//
      btnClicked_NU(w, "18");                                            // Numbers from PC --> produce numbers.
      break;
    case 65464: // 8
    case 48+8:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "19");
      break;
    case 65465: // 9
    case 48+9:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "20");
      break;
//ROW 6
    case 65460: // 4
    case 48+4:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "23");
      break;
    case 65461: // 5
    case 48+5:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "24");
      break;
    case 65462: // 6
    case 48+6:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "25");
      break;
//ROW 7
    case 65457: // 1
    case 48+1:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "28");
      break;
    case 65458: // 2
    case 48+2:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "29");
      break;
    case 65459: // 3
    case 48+3:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "30");
      break;
//ROW 8
    case 65456: // 0
    case 48+0:  //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM
      btnClicked_NU(w, "33");
      break;
    case 46:    //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM             // .        //JM
    case 65454: //JM SHIFTED CAPITAL ALPHA AND SHIFTED NUMERAL  //JM             // .        //JM
      btnClicked_NU(w, "34");
      break;




//OPERATORS / * - +
    case 65455: // / //JM
    case 47:              // divide                   //JM     //**************-- OTHER DIRECT ALPHA MODE KEYBOARD KEYS  --***************//
      btnClicked_NU(w, "21");
      break;
    case 65450: // * //JM
    case 42:              // mult                     //JM     //**************-- OTHER DIRECT ALPHA MODE KEYBOARD KEYS  --***************//
      btnClicked_NU(w, "26");
      break;
    case 65453: // - //JM
    case 45:              // sub                      //JM     //**************-- OTHER DIRECT ALPHA MODE KEYBOARD KEYS  --***************//
      btnClicked_NU(w, "31");
      break;
    case 65451: // + //JM
    case 43:              // plus                     //JM     //**************-- OTHER DIRECT ALPHA MODE KEYBOARD KEYS  --***************//
      btnClicked_NU(w, "36");
      break;






//ROW 7/8
    case 95:                //JM UNDERSCORE   //JM
      btnClicked(w, "31");
      break;

    case 65307:              // Esc EXIT      //JM                   //JM     //**************-- OTHER DIRECT ALPHA MODE KEYBOARD KEYS  --***************//
      btnClicked(w, "32");
      break;

    case 58:                 // COLON.        //JM
      btnClicked(w, "33");
      break;

    case 59:                 // semicolon.    //JM
      btnClicked_SNU(w, "33");
      break;

    case 44:                 // ,             //JM
      btnClicked(w, "34");
      break;

    case 63:                 // ?             //JM
      btnClicked(w, "35");
      break;

    case 32:                //JM SPACE        //JM
      btnClicked(w, "36");
      break;
/*
    case 65507: // left Ctrl
    case 65508: // right Ctrl
      //printf("key pressed: CTRL_State Hardcopy\n");
      copyScreenToClipboard();
      break;
*/
    default:
      break;

  }
return FALSE;
}



//ORIGINAL MODIFIED KEYBOARD DETECTION
//FOR NON AIM MODE. AIM HAS RETURNED AT THIS POINT SO NO IF NEEDED

  switch (event_keyval) {
//ROW 1
    case 65470: // F1                       //JM Changed these to btnFnPressed from btnFnClicked
      //printf("key pressed: F1\n");
      btnFnClickedP(w, "1");
      break;

    case 65471: // F2
      //printf("key pressed: F2\n");
      btnFnClickedP(w, "2");
      break;

    case 65472: // F3
      //printf("key pressed: F3\n");
      btnFnClickedP(w, "3");
      break;

    case 65473: // F4
      //printf("key pressed: F4\n");
      btnFnClickedP(w, "4");
      break;

    case 65474: // F5
      //printf("key pressed: F5\n");
      btnFnClickedP(w, "5");
      break;

    case 65475: // F6
      //printf("key pressed: F6\n");
      btnFnClickedP(w, "6");
      break;
//ROW 2
    case 97:  // a  //dr
      //printf("key pressed: a Sigma+\n"); //dr
      btnClicked(w, "00");
      break;

    case 118: // v //dr
      //printf("key pressed: v 1/X\n"); //dr
      btnClicked(w, "01");
      break;

    case 113: // q //dr
      //printf("key pressed: q SQRT\n"); //dr
      btnClicked(w, "02");
      break;

    case 111: // o //dr
      //printf("key pressed: o LOG\n"); //dr
      btnClicked(w, "03");
      break;

    case 108: // l //dr
      //printf("key pressed: l LN\n"); //dr
      btnClicked(w, "04");
      break;

    case 120: // x //dr
      //printf("key pressed: x XEQ\n"); //dr
      btnClicked(w, "05");
      break;

//ROW 3
    case 109: // m //dr
      //printf("key pressed: m STO\n"); //dr
      btnClicked(w, "06");
      break;

    case 114: // r
      //printf("key pressed: r RCL\n");
      btnClicked(w, "07");
      break;

//dr    case 65366: // PgDn
    case 100: // d //dr
      //printf("key pressed: d Rdown\n"); //dr
      btnClicked(w, "08");
      break;

    case 112: // p         //dr                //JM Special case: p = pi
      //printf("key pressed: p pi\n"); //dr
      shiftF = true;       //dr
      shiftG = false;      //JM
      btnClicked(w, "08"); //dr
      break;               //dr

    case 115: // s //dr
      //printf("key pressed: s SIN\n"); //dr
      btnClicked(w, "09");
      break;

    case 99:  // c //dr
      //printf("key pressed: c COS\n"); //dr
      btnClicked(w, "10");
      break;

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
    case 119: // w //dr
      //printf("key pressed: w x<>y\n"); //dr
      btnClicked(w, "13");
      break;


    case 110: // n //dr
      //printf("key pressed: n +/-\n"); //dr
      btnClicked(w, "14");
      break;

//dr    case 69:  // E
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

      case 55:    // 7
      case 65463: // 7
        //printf("key pressed: 7\n");
        btnClicked(w, "18");
        break;

      case 56:    // 8
      case 65464: // 8
        //printf("key pressed: 8\n");
        btnClicked(w, "19");
        break;

      case 57:    // 9
      case 65465: // 9
        //printf("key pressed: 9\n");
        btnClicked(w, "20");
        break;


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

      case 52:    // 4
      case 65460: // 4
        //printf("key pressed: 4\n");
        btnClicked(w, "23");
        break;

      case 53:    // 5
      case 65461: // 5
        //printf("key pressed: 5\n");
        btnClicked(w, "24");
        break;

      case 54:    // 6
      case 65462: // 6
        //printf("key pressed: 6\n");
        btnClicked(w, "25");
        break;


    case 42:    // * //JM
    case 65450: // * //JM
      //printf("key pressed: multiply\n"); //dr
      btnClicked(w, "26");
      break;

//ROW 7
    case 65505: // left shift  //JM
//    case 65506: //JM right shift. 65453: // -  //JM Remove Right Shift, to allow * & +
      //printf("key pressed: Shift\n"); //dr
      btnClickedP(w, "27");                         //JM PRESSED FOR KEYBOARD F REPEAT
      break;

      case 49:    // 1
      case 65457: // 1
        //printf("key pressed: 1\n");
        btnClicked(w, "28");
        break;

      case 50:    // 2
      case 65458: // 2
        //printf("key pressed: 2\n");
        btnClicked(w, "29");
        break;

      case 51:    // 3
      case 65459: // 3
        //printf("key pressed: 3\n");
        btnClicked(w, "30");
        break;


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

      case 48:    // 0
      case 65456: // 0
        //printf("key pressed: 0\n");
        btnClicked(w, "33");
        break;

      case 44:    // ,
      case 46:    // .
      case 65454: // .
        //printf("key pressed: .\n");
        btnClicked(w, "34");
        break;

    case 92: // \                                //JM R/S changed to \ as on Mac CTRL is something else.
      //printf("key pressed: \\ R/S\n");
      btnClicked(w, "35");
      break;


    case 43:    // + //JM
    case 65451: // + //JM
      //printf("key pressed: add\n"); //dr
      btnClicked(w, "36");
      break;

/*//JM- Reinstated
    case 72:  // H    //JM REMOVE CAP H. ONLY lower case wil print
    case 104: // h
      //printf("key pressed: h Hardcopy to clipboard\n");
      copyScreenToClipboard();
      break;
*/

    case 65507: // left Ctrl
    case 65508: // right Ctrl
      //printf("key pressed: CTRL Activated\n");
      CTRL_State = 65536;
      break;


    case 72+65536: // Ctrl H
    case 104+65536: // Ctrl h
      printf("key pressed: CTRL+h Hardcopy\n");
      copyScreenToClipboard();
      break;


    case 120+65536: // CTRL x
    case 88+65536: // CTRL X
    case 99+65536: // CTRL c
    case 67+65536: // CTRL C
      printf("key pressed: CTRL+c/x Copy x register to clipboard\n");
      copyRegisterXToClipboard();
      break;


    case 100+65536: // CTRL d
    case 68+65536: // CTRL D
      printf("key pressed: CTRL+d Copy Stack registers to clipboard\n");
      copyStackRegistersToClipboard();
      break;


    case 97+65536: // CTRL a
    case 65+65536: // CTRL A
      printf("key pressed: CTRL+d Copy All registers to clipboard\n");
      copyAllRegistersToClipboard();
      break;

      default:
        break;
    }
    return FALSE;
  }
#endif // PC_BUILD


#ifndef DMCP_BUILD
  void strReplace(char *haystack, const char *needle, const char *newNeedle) {
    ////////////////////////////////////////////////////////
    // There MUST be enough memory allocated to *haystack //
    // when strlen(newNeedle) > strlen(needle)            //
    ////////////////////////////////////////////////////////
    char *str, *needleLocation;
    int  needleLg;

    while(strstr(haystack, needle) != NULL) {
      needleLg = strlen(needle);
      needleLocation = strstr(haystack, needle);
      str = malloc(strlen(needleLocation + needleLg) + 1);
      #ifdef PC_BUILD
        if(str == NULL) {
          moreInfoOnError("In function strReplace:", "error allocating memory for str!", NULL, NULL);
          exit(1);
        }
      #endif // PC_BUILD

      strcpy(str, needleLocation + needleLg);
      *strstr(haystack, needle) = 0;
      strcat(haystack, newNeedle);
      strcat(haystack, str);
      free(str);
    }
  }
#endif // !DMCP_BUILD


#ifdef PC_BUILD
  #if (SCREEN_800X480 == 0)
    /********************************************//**
     * \brief Reads the CSS file to configure the calc's GUI style
     *
     * \param void
     * \return void
     ***********************************************/
    void prepareCssData(void) {
      FILE *cssFile;
      char *toReplace, *replaceWith, needle[100], newNeedle[100];
      int  fileLg;

      // Convert the pre-CSS data to CSS data
      cssFile = fopen(CSSFILE, "rb");
      if(cssFile == NULL) {
        moreInfoOnError("In function prepareCssData:", "error opening file " CSSFILE "!", NULL, NULL);
        exit(1);
      }

      // Get the file length
      fseek(cssFile, 0L, SEEK_END);
      fileLg = ftell(cssFile);
      fseek(cssFile, 0L, SEEK_SET);

      cssData = malloc(2*fileLg); // To be sure there is enough space
      if(cssData == NULL) {
        moreInfoOnError("In function prepareCssData:", "error allocating 10000 bytes for CSS data", NULL, NULL);
        exit(1);
      }

      ignore_result(fread(cssData, 1, fileLg, cssFile));
      fclose(cssFile);
      cssData[fileLg] = 0;

      toReplace = strstr(cssData, "/* Replace $");
      while(toReplace != NULL) {
        int i = -1;
        toReplace += 11;
        while(toReplace[++i] != ' ') {
          needle[i] = toReplace[i];
        }
        needle[i] = 0;

        *toReplace = ' ';

        replaceWith = strstr(toReplace, " with ");
        if(replaceWith == NULL) {
          moreInfoOnError("In function prepareCssData:", "Can't find \" with \" after \"/* Replace $\" in CSS file " CSSFILE, NULL, NULL);
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
        moreInfoOnError("In function prepareCssData:", "There is still an unreplaced $ in the CSS file!\nPlease check file " CSSFILE, NULL, NULL);
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
#if defined(JM_LAYOUT_1A)  //JM LAYOUT 1. FINAL. Show colour band next to LCD
  gtk_widget_hide(lblFKey2);  //JMLINES
  gtk_widget_hide(lblGKey2);  //JMLINES
//  gtk_widget_hide(lblEKey);   //JMLINES
//  gtk_widget_hide(lblEEKey);   //JMLINES
//  gtk_widget_hide(lblSKey);   //JMLINES
#endif  
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
  gtk_widget_hide(btn21A);  //vv dr - new AIM
  gtk_widget_hide(btn22A);
  gtk_widget_hide(btn23A);
  gtk_widget_hide(btn24A);
  gtk_widget_hide(btn25A);
  gtk_widget_hide(btn26A);  //^^

  gtk_widget_hide(lbl21F);
  gtk_widget_hide(lbl21G);
//  gtk_widget_hide(lbl21H);  //JMALPHA
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

  gtk_widget_hide(lbl21Fa); //JM AIM2
  gtk_widget_hide(lbl22Fa); //JM AIM2
  gtk_widget_hide(lbl23Fa); //JM AIM2
  gtk_widget_hide(lbl24Fa); //JM AIM2
  gtk_widget_hide(lbl25Fa); //JM AIM2
  gtk_widget_hide(lbl26Fa); //JM AIM2


  gtk_widget_hide(btn31);
  gtk_widget_hide(btn32);
  gtk_widget_hide(btn33);
  gtk_widget_hide(btn34);
  gtk_widget_hide(btn35);
  gtk_widget_hide(btn36);
  gtk_widget_hide(btn31A);  //vv dr - new AIM
  gtk_widget_hide(btn32A);
  gtk_widget_hide(btn33A);
  gtk_widget_hide(btn34A);
  gtk_widget_hide(btn35A);
  gtk_widget_hide(btn36A);  //^^

  gtk_widget_hide(lbl31F);
  gtk_widget_hide(lbl31G);
  gtk_widget_hide(lbl31L);
  gtk_widget_hide(lbl32F);
  gtk_widget_hide(lbl32G);
  gtk_widget_hide(lbl32L);
  gtk_widget_hide(lbl33F);
  gtk_widget_hide(lbl33G);
//  gtk_widget_hide(lbl33H);
  gtk_widget_hide(lbl33L);
  gtk_widget_hide(lbl34F);
  gtk_widget_hide(lbl34G);
//  gtk_widget_hide(lbl34H);  //JM CAPS //JMALPHA2 temporary remove A from J
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

  gtk_widget_hide(lbl31Fa); //JM AIM2
  gtk_widget_hide(lbl32Fa); //JM AIM2
  gtk_widget_hide(lbl33Fa); //JM AIM2
  gtk_widget_hide(lbl34Fa); //JM AIM2
  gtk_widget_hide(lbl35Fa); //JM AIM2
  gtk_widget_hide(lbl36Fa); //JM AIM2

//  gtk_widget_hide(lbl34Fa); //JMALPHA2
//  gtk_widget_hide(lbl35Fa); //JMALPHA2

  gtk_widget_hide(btn41);
  gtk_widget_hide(btn42);
  gtk_widget_hide(btn43);
  gtk_widget_hide(btn44);
  gtk_widget_hide(btn45);
  gtk_widget_hide(btn42A);  //vv dr - new AIM
  gtk_widget_hide(btn43A);
  gtk_widget_hide(btn44A);  //^^

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
//  gtk_widget_hide(lbl44P);
  gtk_widget_hide(lbl45F);
  gtk_widget_hide(lbl45G);
  gtk_widget_hide(lbl45L);
  gtk_widget_hide(lbl41Gr);
  gtk_widget_hide(lbl42Gr);
  gtk_widget_hide(lbl43Gr);
  gtk_widget_hide(lbl44Gr);
  gtk_widget_hide(lbl45Gr);
  gtk_widget_hide(lbl41Fa); //JM
  gtk_widget_hide(lbl42Fa); //vv dr - new AIM
  gtk_widget_hide(lbl43Fa); //^^
  gtk_widget_hide(lbl44Fa); //JM AIM2
  gtk_widget_hide(lbl45Fa); //JM

  gtk_widget_hide(btn51);
  gtk_widget_hide(btn52);
  gtk_widget_hide(btn53);
  gtk_widget_hide(btn54);
  gtk_widget_hide(btn55);
  gtk_widget_hide(btn52A);  //vv dr - new AIM
  gtk_widget_hide(btn53A);
  gtk_widget_hide(btn54A);
  gtk_widget_hide(btn55A);  //^^

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
  gtk_widget_hide(lbl52Fa); //vv dr - new AIM
  gtk_widget_hide(lbl53Fa);
  gtk_widget_hide(lbl54Fa);
  gtk_widget_hide(lbl55Fa); //^^

  gtk_widget_hide(btn61);
  gtk_widget_hide(btn62);
  gtk_widget_hide(btn63);
  gtk_widget_hide(btn64);
  gtk_widget_hide(btn65);
  gtk_widget_hide(btn62A);  //vv dr - new AIM
  gtk_widget_hide(btn63A);
  gtk_widget_hide(btn64A);
  gtk_widget_hide(btn65A);  //^^

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
  gtk_widget_hide(lbl62Fa); //vv dr - new AIM
  gtk_widget_hide(lbl63Fa);
  gtk_widget_hide(lbl64Fa);
  gtk_widget_hide(lbl65Fa); //^^

  gtk_widget_hide(btn71);
  gtk_widget_hide(btn72);
  gtk_widget_hide(btn73);
  gtk_widget_hide(btn74);
  gtk_widget_hide(btn75);
  gtk_widget_hide(btn72A);  //vv dr - new AIM
  gtk_widget_hide(btn73A);
  gtk_widget_hide(btn74A);
  gtk_widget_hide(btn75A);  //^^

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
  gtk_widget_hide(lbl72Fa); //vv dr - new AIM
  gtk_widget_hide(lbl73Fa);
  gtk_widget_hide(lbl74Fa);
  gtk_widget_hide(lbl75Fa); //^^

  gtk_widget_hide(btn81);
  gtk_widget_hide(btn82);
  gtk_widget_hide(btn83);
  gtk_widget_hide(btn84);
  gtk_widget_hide(btn85);
  gtk_widget_hide(btn82A);  //vv dr - new AIM
  gtk_widget_hide(btn83A);
  gtk_widget_hide(btn84A);
  gtk_widget_hide(btn85A);  //^^

  gtk_widget_hide(lbl81F);
  gtk_widget_hide(lbl81G);
  gtk_widget_hide(lbl81L);
//gtk_widget_hide(lbl81H);  //JM
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
  gtk_widget_hide(lbl82Fa); //vv dr - new AIM
  gtk_widget_hide(lbl83Fa);
  gtk_widget_hide(lbl84Fa);
  gtk_widget_hide(lbl85Fa); //^^

//  gtk_widget_hide(lblOn);
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
  gtk_widget_get_preferred_size(  lbl21Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl21Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl22F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl22G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl22F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl22G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl22Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl22Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl22Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl22Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl23F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl23G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl23F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl23G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl23Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl23Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl23Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl23Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^
 
  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl24F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl24G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl24F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl24G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl24Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl24Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl24Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl24Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl25F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl25G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl25F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl25G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl25Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl25Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl25Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl25Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl26F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl26G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl26F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl26G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl26Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl26Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl26Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl26Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y;
  gtk_widget_get_preferred_size(  lbl31F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl31G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl31F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl31G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl31Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl31Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl31Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl31Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl32F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl32G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl32F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl32G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl32Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl32Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl32Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl32Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl33F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl33G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl33F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl33G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl33Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl33Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl33Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl33Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl34F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl34G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl34F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl34G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl34Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl34Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl34Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl34Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl35F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl35G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl35F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl35G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl35Gr, NULL, &lblG);                                                               //JM !! GR
  gtk_fixed_move(GTK_FIXED(grid), lbl35Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);  //JM !! GR
  gtk_widget_get_preferred_size(  lbl35Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl35Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl36F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl36G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl36F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl36G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl36Gr, NULL, &lblG);                                                               //JM !! GR
  gtk_fixed_move(GTK_FIXED(grid), lbl36Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);  //JM !! GR
  gtk_widget_get_preferred_size(  lbl36Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl36Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y;
  gtk_widget_get_preferred_size(  lbl41F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl41G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl41F, (2*xPos+KEY_WIDTH_1+DELTA_KEYS_X-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl41G, (2*xPos+KEY_WIDTH_1+DELTA_KEYS_X+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl41Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl41Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += 2*DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl42F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl42G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl42F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP/2-lblG.width+2)/2-GAP/2, yPos - Y_OFFSET_SHIFTED_LABEL);           //JMWIDTH MODIFIED FOR EXP, CPX & BASE mod
  gtk_fixed_move(GTK_FIXED(grid), lbl42G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP/2-lblG.width+2)/2-GAP/2, yPos - Y_OFFSET_SHIFTED_LABEL);           //JMWIDTH MODIFIED FOR EXP, CPX & BASE mod
  gtk_widget_get_preferred_size(  lbl42Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl42Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl42Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl42Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl43F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl43G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl43F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP/2-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);                 //JMWIDTH MODIFIED FOR EXP, CPX & BASE mod
  gtk_fixed_move(GTK_FIXED(grid), lbl43G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP/2-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);                 //JMWIDTH MODIFIED FOR EXP, CPX & BASE mod
  gtk_widget_get_preferred_size(  lbl43Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl43Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl43Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl43Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl44F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl44G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl44F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP/2-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);                 //JMWIDTH MODIFIED FOR EXP, CPX & BASE mod
  gtk_fixed_move(GTK_FIXED(grid), lbl44G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP/2-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);                 //JMWIDTH MODIFIED FOR EXP, CPX & BASE mod
  gtk_widget_get_preferred_size(  lbl44Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl44Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl44Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl44Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl45F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl45G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl45F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl45G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl45Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl45Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_widget_get_preferred_size(  lbl51F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl51G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl51F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);//JM align [f] arrowUp (*0-40)
  gtk_fixed_move(GTK_FIXED(grid), lbl51G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);//JM align [f] arrowUp (*0-40)
  gtk_widget_get_preferred_size(  lbl51Gr, NULL, &lblG); //JMAHOME
//  gtk_fixed_move(GTK_FIXED(grid), lbl51Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK); //JMAHOME
  gtk_fixed_move(GTK_FIXED(grid), lbl51Gr, (2*xPos+KEY_WIDTH_2+lblF.width+GAP*6-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);      //JM JMAHOME ALPHA BLUE MENU LABELS //^^

  xPos += DELTA_KEYS_X + 18;
  gtk_widget_get_preferred_size(  lbl52F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl52G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl52F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl52G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl52Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl52Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl52Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl52Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl53F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl53G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl53F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl53G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl53Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl53Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl53Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl53Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl54F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl54G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl54F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl54G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl54Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl54Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl54Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl54Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl55F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl55G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl55F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl55G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl55Gr, NULL, &lblG);                                                                //JM GREEK
  gtk_fixed_move(GTK_FIXED(grid), lbl55Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);   //JM GREEK
  gtk_widget_get_preferred_size(  lbl55Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl55Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_widget_get_preferred_size(  lbl61F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl61G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl61F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);   //JM align [f] arrowDn (*0-40)
  gtk_fixed_move(GTK_FIXED(grid), lbl61G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);   //JM align [f] arrowDn (*0-40)
  gtk_widget_get_preferred_size(  lbl61Gr, NULL, &lblG); //JMAHOME2                                                                       //JM10
  //gtk_fixed_move(GTK_FIXED(grid), lbl61Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);          //JM10
  gtk_fixed_move(GTK_FIXED(grid), lbl61Gr, (2*xPos+KEY_WIDTH_2+lblF.width+GAP*6-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);      //JM JMAHOME2 ALPHA BLUE MENU LABELS //^^

  xPos += DELTA_KEYS_X + 18;
  gtk_widget_get_preferred_size(  lbl62F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl62G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl62F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl62G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl62Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl62Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl62Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl62Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl63F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl63G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl63F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl63G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl63Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl63Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl63Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl63Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl64F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl64G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl64F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl64G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl64Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl64Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl64Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl64Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl65F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl65G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl65F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl65G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl65Gr, NULL, &lblG);                                                                //JM
  gtk_fixed_move(GTK_FIXED(grid), lbl65Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);   //JM
  gtk_widget_get_preferred_size(  lbl65Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl65Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

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
  gtk_widget_get_preferred_size(  lbl72Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl72Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl73F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl73G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl73F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl73G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl73Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl73Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl73Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl73Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl74F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl74G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl74F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl74G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl74Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl74Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);
  gtk_widget_get_preferred_size(  lbl74Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl74Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl75F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl75G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl75F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl75G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl75Gr, NULL, &lblG);                                                                //JM added
  gtk_fixed_move(GTK_FIXED(grid), lbl75Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);   //JMadded
  gtk_widget_get_preferred_size(  lbl75Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl75Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_widget_get_preferred_size(  lbl81F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl81G, NULL, &lblG);



//-last one  gtk_fixed_move(GTK_FIXED(grid), lbl81F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
//-last one  gtk_fixed_move(GTK_FIXED(grid), lbl81G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
//JM MANUAL positioning
//  gtk_fixed_move(GTK_FIXED(grid), lbl81F, (2*xPos+KEY_WIDTH_1-22)/2, yPos - Y_OFFSET_SHIFTED_LABEL);   //JM

//JMPRT removed for template-  gtk_fixed_move(GTK_FIXED(grid), lbl81G, (2*xPos+KEY_WIDTH_1+lblF.width+2)/2 + 15, yPos + 10);                       //JM
//  gtk_widget_get_preferred_size(  lblOn, NULL, &lblF);                                                          //JM

    gtk_fixed_move(GTK_FIXED(grid), lbl81F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
    gtk_fixed_move(GTK_FIXED(grid), lbl81G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
//    gtk_fixed_move(GTK_FIXED(grid), lblOn,  (2*xPos+KEY_WIDTH_1+lblF.width+2*GAP-lblG.width+2)/2 -10, yPos - Y_OFFSET_SHIFTED_LABEL);
//  gtk_fixed_move(GTK_FIXED(grid), lblOn,  (2*xPos+KEY_WIDTH_1-20)/2, yPos + 38);    //JM

//  gtk_widget_get_preferred_size(  lbl81Gr, NULL, &lblG);         //JMPRTA                                                     //JM++_ //JMAPRT
//  gtk_fixed_move(GTK_FIXED(grid), lbl81Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK); //JM ++ //JMAPRT
//JMPRT removed for template-    gtk_fixed_move(GTK_FIXED(grid), lbl81Gr, (2*xPos+KEY_WIDTH_1+lblF.width+2)/2 + 20, yPos + 10);      //JM JMAPRT ALPHA BLUE MENU LABELS //^^
//JM^^



  xPos += DELTA_KEYS_X + 18;
  gtk_widget_get_preferred_size(  lbl82F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl82G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl82F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl82G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl82Gr, NULL, &lblG);                                                                            //JM ALPHA BLUE MENU LABELS
//gtk_fixed_move(GTK_FIXED(grid), lbl82Gr, (2*xPos+KEY_WIDTH_2-GAP-lblG.width+2)/2,           yPos + GAP - Y_OFFSET_SHIFTED_LABEL); //JM ALPHA BLUE MENU LABELS //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl82Gr, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);      //JM ALPHA BLUE MENU LABELS //^^
  gtk_widget_get_preferred_size(  lbl82Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl82Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl83F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl83G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl83F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl83G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl83Gr, NULL, &lblG);                                                                            //JM ALPHA BLUE MENU LABELS
//gtk_fixed_move(GTK_FIXED(grid), lbl83Gr, (2*xPos+KEY_WIDTH_2-GAP-lblG.width+2)/2,           yPos + GAP - Y_OFFSET_SHIFTED_LABEL); //JM ALPHA BLUE MENU LABELS //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl83Gr, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);      //JM ALPHA BLUE MENU LABELS //^^
  gtk_widget_get_preferred_size(  lbl83Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl83Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl84F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl84G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl84F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl84G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl84Gr, NULL, &lblG);                                                                            //JM ALPHA BLUE MENU LABELS
//gtk_fixed_move(GTK_FIXED(grid), lbl84Gr, (2*xPos+KEY_WIDTH_2-GAP-lblG.width+2)/2,           yPos + GAP - Y_OFFSET_SHIFTED_LABEL); //JM ALPHA BLUE MENU LABELS //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl84Gr, (2*xPos+KEY_WIDTH_2+lblF.width+GAP*4-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);      //JM ALPHA BLUE MENU LABELS //^^              //JM MANUAL GAP ADJUSTMENT TO 4x
  gtk_widget_get_preferred_size(  lbl84Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl84Fa, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

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
  gtk_widget_get_preferred_size(  lbl85Gr, NULL, &lblG);                                                                              //JM ALPHA BLUE MENU LABELS
//gtk_fixed_move(GTK_FIXED(grid), lbl85Gr, (2*xPos+KEY_WIDTH_2-GAP-lblG.width+2)/2,           yPos + GAP - Y_OFFSET_SHIFTED_LABEL);   //JM ALPHA BLUE MENU LABELS //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl85Gr, (2*xPos+KEY_WIDTH_2+lblF.width+2*GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);        //JM ALPHA BLUE MENU LABELS //^^
  gtk_widget_get_preferred_size(  lbl85Fa, NULL, &lblF);                                                                        //vv dr - new AIM
  gtk_fixed_move(GTK_FIXED(grid), lbl85Fa, (2*xPos+KEY_WIDTH_2-lblF.width-2*GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);  //^^

}



void labelCaptionNormal(const calcKey_t *key, GtkWidget *button, GtkWidget *lblF, GtkWidget *lblG, GtkWidget *lblL) {
  uint8_t lbl[22];

  if(key->primary == 0) {
    lbl[0] = 0;
  }
  else {
    stringToUtf8(indexOfItems[max(key->primary, -key->primary)].itemSoftmenuName, lbl);
  }

  if(key->primary == ITM_SIGMAPLUS && calcMode == CM_NORMAL && !getSystemFlag(FLAG_USER)) {                       //JMUSER Change the name inside the Sigma+ button
    stringToUtf8(indexOfItems[max(Norm_Key_00_VAR, -Norm_Key_00_VAR)].itemSoftmenuName, lbl);            //JMUSER
  }                                                                                                      //JM

  if(strcmp((char *)lbl, "CAT") == 0 && key->keyId != 85) {    //JM wqs 85  //JM Changed CATALOG to CAT
    lbl[3] = 0;
  }

  gtk_button_set_label(GTK_BUTTON(button), (gchar *)lbl);

//  if(strcmp((char *)lbl, "/") == 0 && key->keyId == 55) {    //JM if "/", re-do to "÷". Presumed easier than to fix the UTf8 conversion above.
//  gtk_button_set_label(GTK_BUTTON(button), "÷");             //JM DIV
//  }                                                          //JM

  if((key->primary == ITM_AIM && getSystemFlag(FLAG_USER) && calcMode == CM_NORMAL ) || (!getSystemFlag(FLAG_USER) && key->primary == ITM_SIGMAPLUS && calcMode == CM_NORMAL && Norm_Key_00_VAR == ITM_AIM)) {
    gtk_widget_set_name(button, "AlphaKey");                 //JMALPHA Colour the alpha key gold if assigned.
  }
  else if(key->primary == ITM_SHIFTf) {
    gtk_widget_set_name(button, "calcKeyF");
  }
  else if(key->primary == ITM_SHIFTg  || (key->primary == ITM_SIGMAPLUS && Norm_Key_00_VAR == ITM_SHIFTg) ) {
    gtk_widget_set_name(button, "calcKeyG");
  }
  else if(key->primary == KEY_fg) {
    gtk_widget_set_name(button, "calcKeyFG");
  }
  else if((key->primary >= ITM_0 && key->primary <= ITM_9) || key->primary == ITM_PERIOD) {
    gtk_widget_set_name(button, "calcNumericKey");
  }
  else if(strcmp((char *)lbl, "÷") == 0 && key->keyId == 55) {      //JM increase the font size of the operators to the numeric key size
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if(strcmp((char *)lbl, "×") == 0 && key->keyId == 65) {      //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if(strcmp((char *)lbl, "-") == 0 && key->keyId == 75) {      //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if(strcmp((char *)lbl, "+") == 0 && key->keyId == 85) {      //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else {
    gtk_widget_set_name(button, "calcKey");
  }

  stringToUtf8(indexOfItems[max(key->fShifted, -key->fShifted)].itemSoftmenuName, lbl);

  if(key->fShifted == 0) {
    lbl[0] = 0;
  }
  else if(strcmp((char *)lbl, "CAT") == 0 && key->keyId != 85) {   //JM was 85  //JM Changed CATALOG to CAT
    lbl[3] = 0;
  }

  gtk_label_set_label(GTK_LABEL(lblF), (gchar *)lbl);
  if(key->fShifted < 0) gtk_widget_set_name(lblF, "fShiftedUnderline"); else  gtk_widget_set_name(lblF, "fShifted");

  stringToUtf8(indexOfItems[max(key->gShifted, -key->gShifted)].itemSoftmenuName, lbl);
  if(key->gShifted == 0) {
    lbl[0] = 0;
  }
  else if(strcmp((char *)lbl, "MODE#") == 0 && key->keyId == 22) {          //JM MODE#
    strcpy((char *)lbl,"#");                                                //JM MODE#
  }

  gtk_label_set_label(GTK_LABEL(lblG), (gchar *)lbl);
  if(key->gShifted < 0) gtk_widget_set_name(lblG, "gShiftedUnderline"); else  gtk_widget_set_name(lblG, "gShifted");

  stringToUtf8(indexOfItems[key->primaryAim].itemSoftmenuName, lbl);
  if(key->primaryAim == 0) {
    lbl[0] = 0;
  }

//  if(lbl[0] == ' ') {    //JM Space replace with lines below
//    lbl[0] = '_';
//  }

  if(lbl[0] == ' ') {     //JM SPACE |  OPEN BOX 9251,  0xE2 0x90 0xA3  |  0xE2 0x90 0xA0 for SP.
    lbl[0]=0xC2;          //JM SPACE the space character is not in the font. \rather use . . for space.
    lbl[1]=0xB7;          //JM SPACE
    lbl[2]='_';           //JM SPACE
    lbl[3]=0xc2;          //JM SPACE
    lbl[4]=0xb7;          //JM SPACE
    lbl[5]=0;             //JM SPACE
  }                       //JM SPACE

  if(lbl[0] == 'Y') {                                       //JM Brighten up Y and N
    //lbl[0] = '#';                                         //JM Brighten up Y and N
    gtk_label_set_label(GTK_LABEL(lblL), (gchar *)lbl);     //JM Brighten up Y and N
    gtk_widget_set_name(lblL, "YN");                        //JM Brighten up Y and N
  }
  else if(lbl[0] == 'N') {                                  //JM Brighten up Y and N
    //lbl[0] = '#';                                         //JM Brighten up Y and N
    gtk_label_set_label(GTK_LABEL(lblL), (gchar *)lbl);     //JM Brighten up Y and N
    gtk_widget_set_name(lblL, "YN");                        //JM Brighten up Y and N
  }
  else {                                                    //JM Brighten up Y and N
    gtk_label_set_label(GTK_LABEL(lblL), (gchar *)lbl);
    gtk_widget_set_name(lblL, "letter");
  }                                                         //JM Brighten up Y and N

}


//dr
void labelCaptionAimFa(const calcKey_t* key, GtkWidget* lblF) {
  uint8_t lbl[22];

  if(key->primaryAim == ITM_NULL) {
    lbl[0] = 0;
  } 
  else {
    //printf("####### %d %d ",alphaCase,key->primaryAim);
    if( (ITM_A <= key->primaryAim && key->primaryAim <= ITM_Z)) {             //if in upper case, show lower case on yellow
      stringToUtf8(indexOfItems[((max(key->primaryAim, -key->primaryAim)) + ((alphaCase == AC_UPPER) ? 26:0 ))].itemSoftmenuName, lbl);
      //printf(" 1 %s \n",lbl);
    }
    else {                                                                                               //
      stringToUtf8(indexOfItems[numlockReplacements(1,max(key->primaryAim, -key->primaryAim),!numLock,false,false)].itemSoftmenuName, lbl);
      //printf(" 2 %s \n",lbl);
    }
  }

  if(numlockReplacements(2,max(key->primaryAim, -key->primaryAim),!numLock,false,false) == ITM_SPACE) {
      lbl[0]=0xC2;          //JM SPACE the space character is not in the font. \rather use . . for space.
      lbl[1]=0xB7;          //JM SPACE
      lbl[2]='_';           //JM SPACE
      lbl[3]=0xc2;          //JM SPACE
      lbl[4]=0xb7;          //JM SPACE
      lbl[5]=0;             //JM SPACE
  } else

  //JM Exception, to change 0 to ";", when !NL & SHFT-0
  if(numlockReplacements(3,max(key->fShiftedAim, -key->fShiftedAim),numLock,true,false) != max(key->fShiftedAim, -key->fShiftedAim)) { 
    stringToUtf8(indexOfItems[numlockReplacements(4,max(key->fShiftedAim, -key->fShiftedAim),numLock,true,false)].itemSoftmenuName, lbl);
  }

//  if(key->primary == ITM_RS) {    //JM R/S change to /
//  lbl[0]=47;                      //JM DIV
//  lbl[1]=0;                       //JM
//  }                               //JM

  gtk_label_set_label(GTK_LABEL(lblF), (gchar*)lbl);
  if(key->primary < 0) gtk_widget_set_name(lblF, "fShiftedUnderline"); else  gtk_widget_set_name(lblF, "fShifted");
}


//dr
void labelCaptionAimFaChr(const calcKey_t* key, GtkWidget* lblF, int chrF) {
  uint8_t lbl[22];

  stringToUtf8(indexOfItems[chrF].itemSoftmenuName, lbl);

  if(key->primary == 0) {
    lbl[0] = 0;
  }

  gtk_label_set_label(GTK_LABEL(lblF), (gchar*)lbl);
  if(key->fShiftedAim < 0) gtk_widget_set_name(lblF, "AimfShiftedUnderline"); else  gtk_widget_set_name(lblF, "fShifted");
}



void labelCaptionAim(const calcKey_t *key, GtkWidget *button, GtkWidget *lblGreek, GtkWidget *lblL) {
  uint8_t lbl[22];

  if(key->primaryAim == ITM_NULL) {
    lbl[0] = 0;
  } 
  else {
    if(alphaCase == AC_LOWER && (ITM_A <= key->primaryAim && key->primaryAim <= ITM_Z)) {
      stringToUtf8(indexOfItems[numlockReplacements(5,max(key->primaryAim, -key->primaryAim) + 26, numLock, shiftF, shiftG)].itemSoftmenuName, lbl);
    }
    else {
      stringToUtf8(indexOfItems[numlockReplacements(6,max(key->primaryAim, -key->primaryAim), numLock, shiftF, shiftG)].itemSoftmenuName, lbl);
    }
  }
  //printf("####B^^ %d %d %d %d\n", numLock, shiftF,shiftG,calcMode==CM_AIM);
  if(numlockReplacements(7,max(key->primaryAim, -key->primaryAim), numLock, false, false) == ITM_SPACE) {
      lbl[0]=0xC2;          //JM SPACE the space character is not in the font. \rather use . . for space.
      lbl[1]=0xB7;          //JM SPACE
      lbl[2]='_';           //JM SPACE
      lbl[3]=0xc2;          //JM SPACE
      lbl[4]=0xb7;          //JM SPACE
      lbl[5]=0;             //JM SPACE
  }

//JM remove this because CAT is not on keyboard anymore, but in menu
//JM was 85  //JM Changed CATALOG to CAT. Actually not needed, as the WP43C already has a shortened CAT to start with
//JM  if(strcmp((char *)lbl, "CAT") == 0 && key->keyId != 85) {
//JM    lbl[3] = 0;
//JM  }

  gtk_button_set_label(GTK_BUTTON(button), (gchar *)lbl);


//Specify the different categories of coloured zones
  if(key->keyLblAim == ITM_SHIFTf) {
    gtk_widget_set_name(button, "calcKeyF");
  }
  else if(key->keyLblAim == ITM_SHIFTg) {
    gtk_widget_set_name(button, "calcKeyG");
  }
  else if(key->keyLblAim == KEY_fg) {                                 //JM 
    gtk_widget_set_name(button, "calcKeyFG");                          //JM
  }
  else {
    /*        //vv dr - new AIM
    if((key->fShiftedAim == key->keyLblAim || key->fShiftedAim == ITM_PROD_SIGN) && key->keyLblAim != ITM_NULL) {
      gtk_widget_set_name(button, "calcKeyGoldenBorder");
    }
    else {*/  //^^
      gtk_widget_set_name(button, "calcKey");
  /*}*/       //dr - new AIM
  }


//Convert Greek CAPITAL and LOWER case to UTF !
  if((ITM_ALPHA <= key->gShiftedAim && key->gShiftedAim <= ITM_OMEGA) || (ITM_QOPPA <= key->gShiftedAim && key->gShiftedAim <= ITM_SAMPI)) {   //JM GREEK. Add extra 36 char greek range
  /*stringToUtf8(indexOfItems[key->gShiftedAim].itemSoftmenuName, lbl);  //vv dr - new AIM
    lbl[2] = ' ';
    lbl[3] = 0;
    stringToUtf8(indexOfItems[key->gShiftedAim + 36].itemSoftmenuName, lbl + 3);*/
    if(alphaCase == AC_LOWER) {
      stringToUtf8(indexOfItems[numlockReplacements(8,key->gShiftedAim + 36, numLock, false, !jm_GGREEK)].itemSoftmenuName, lbl);
    }
    else {
      stringToUtf8(indexOfItems[numlockReplacements(9,key->gShiftedAim, numLock, false, !jm_GGREEK)].itemSoftmenuName, lbl);
    }                                                               //^^
  }
  else {
    stringToUtf8(indexOfItems[numlockReplacements(10,key->gShiftedAim, numLock, false, !jm_GGREEK)].itemSoftmenuName, lbl);
  }

//GShift set label
  if(key->gShiftedAim == 0) {
    lbl[0] = 0;
  }

//JM Removed as CAT is not Aim keyboard anymore
//JM was 85   //JM Changed CATALOG to CAT
//JM  else if(strcmp((char *)lbl, "CAT") == 0 && key->keyId != 85) {
//JM    lbl[3] = 0;
//JM  }

/* JM TEST PROCEDURE TO TEST DISPLAY
  else if(key->gShiftedAim == ITM_DIGAMMA ) {
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
  if(key->gShiftedAim < 0 && jm_GGREEK) {
    gtk_widget_set_name(lblGreek, "gShiftedUnderline");     //dr - new AIM
  }
  else {
    gtk_widget_set_name(lblGreek, "greek");
  }

//Primaries, convert to UTF
  if(key->primaryAim == 0) {
    lbl[0] = 0;
  }
  else {
   stringToUtf8(indexOfItems[key->primaryAim].itemSoftmenuName, lbl);  //Convert to UTF !
  }

//if(lbl[0] == ' ') {     //JM Space replace with lines below
//  lbl[0] = '_';
//}

  if(lbl[0] == ' ') {     //JM SPACE |  OPEN BOX 9251,  0xE2 0x90 0xA3  |  0xE2 0x90 0xA0 for SP.
    lbl[0]=0xC2;          //JM SPACE the space character is not in the font. \rather use . . for space.
    lbl[1]=0xB7;          //JM SPACE
    lbl[1]=0xB7;          //JM SPACE
    lbl[2]=' ';           //JM SPACE
    lbl[3]=0xc2;          //JM SPACE
    lbl[4]=0xb7;          //JM SPACE
    lbl[5]=0;             //JM SPACE
  }                       //JM SPACE

  if(lbl[0] == 'Y') {                                       //JM Brighten up Y and N
    //lbl[0] = '#';                                         //JM Brighten up Y and N
    gtk_label_set_label(GTK_LABEL(lblL), (gchar *)lbl);     //JM Brighten up Y and N
    gtk_widget_set_name(lblL, "YN");                        //JM Brighten up Y and N
  }
  else if(lbl[0] == 'N') {                                  //JM Brighten up Y and N
    //lbl[0] = '#';                                         //JM Brighten up Y and N
    gtk_label_set_label(GTK_LABEL(lblL), (gchar *)lbl);     //JM Brighten up Y and N
    gtk_widget_set_name(lblL, "YN");                        //JM Brighten up Y and N
  }
  else {                                                    //JM Brighten up Y and N
    gtk_label_set_label(GTK_LABEL(lblL), (gchar *)lbl);
    gtk_widget_set_name(lblL, "letter");
  }                                                         //JM Brighten up Y and N
}



void labelCaptionTam(const calcKey_t *key, GtkWidget *button) {
  uint8_t lbl[22];

  if(key->primaryTam == 0 || (key->keyId == 32 && tam.mode != TM_VALUE_CHB)) { // Display H for changing base to hexadecimal
    lbl[0] = 0;
  }
  else {
    stringToUtf8(indexOfItems[key->primaryTam].itemSoftmenuName, lbl);
  }

  gtk_button_set_label(GTK_BUTTON(button), (gchar *)lbl);

  if(strcmp((char *)lbl, "/") == 0 && key->keyId == 55) {    //JM if "/", re-do to "÷". Presumed easier than to fix the UTf8 conversion above.
    gtk_button_set_label(GTK_BUTTON(button), "÷");           //JM DIV
  }                                                          //JM

  if(key->primaryTam == ITM_SHIFTf) {
    gtk_widget_set_name(button, "calcKeyF");
  }
  else if(key->primaryTam == ITM_SHIFTg) {
    gtk_widget_set_name(button, "calcKeyG");
  }
  else if(key->primaryTam == KEY_fg) {                              //JM
    gtk_widget_set_name(button, "calcKeyFG");                        //JM
  }

  else if(strcmp((char *)lbl, "/") == 0 && key->keyId == 55) {      //JM increase the font size of the operators to the numeric key size
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if(strcmp((char *)lbl, "×") == 0 && key->keyId == 65) {      //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if(strcmp((char *)lbl, "-") == 0 && key->keyId == 75) {      //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators
  else if(strcmp((char *)lbl, "+") == 0 && key->keyId == 85) {      //JM increase the font size of the operators
    gtk_widget_set_name(button, "calcNumericKey");                  //JM increase the font size of the operators
  }                                                                 //JM increase the font size of the operators

  else {
    gtk_widget_set_name(button, "calcKey");
  }
}



    void calcModeNormalGui(void) {
      const calcKey_t *keys;

  if(running_program_jm) return;                        //JM faster during program excution

  keys = getSystemFlag(FLAG_USER) ? kbd_usr : kbd_std;

  hideAllWidgets();

#if defined(JM_LAYOUT_1A)  //JM LAYOUT 1. FINAL. Show colour band next to LCD
  gtk_widget_show( lblFKey2);  //JMLINES
  gtk_widget_show( lblGKey2);  //JMLINES
//  gtk_widget_show( lblEKey);   //JMLINES
//  gtk_widget_show( lblEEKey);   //JMLINES
//  gtk_widget_show( lblSKey);   //JMLINES
#endif

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
  //gtk_widget_show(lbl21H); //JMALPHA temporary remove A from Sigma+
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
//  gtk_widget_show(lbl33H);
  gtk_widget_show(lbl33L);
  gtk_widget_show(lbl34F);
  gtk_widget_show(lbl34G);
  //gtk_widget_show(lbl34H);//JMALPHA2 temporary remove A from J
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
//  gtk_widget_show(lbl44P);
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
  gtk_widget_show(lbl81G); //JM
  gtk_widget_show(lbl81L);
//gtk_widget_show(lbl81H);  //JM
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

 // gtk_widget_show(lblOn);
 //JM7  gtk_widget_show(lblConfirmY);  //JM Y/N
 //JM7  gtk_widget_show(lblConfirmN);  //JM Y/N

  moveLabels();
}



    void calcModeAimGui(void) {
      const calcKey_t *keys;

  if(running_program_jm) return;                        //JM faster during program excution

  keys = getSystemFlag(FLAG_USER) ? kbd_usr : kbd_std;

  hideAllWidgets();

  labelCaptionAimFa(keys, lbl21Fa);                     //vv dr - new AIM
//  labelCaptionAimFaChr(   keys,   lbl21Fa, ITM_ALPHA);          //JM new
  labelCaptionAim(keys++, btn21A, lbl21Gr, lbl21L);     //vv dr - new AIM
  labelCaptionAimFa(keys, lbl22Fa);                     //vv dr - new AIM
//  labelCaptionAimFaChr(   keys,   lbl22Fa, ITM_NUMBER_SIGN);          //JM new
  labelCaptionAim(keys++, btn22A, lbl22Gr, lbl22L);
  labelCaptionAimFa(keys, lbl23Fa);                     //vv dr - new AIM
//  labelCaptionAimFaChr(   keys,   lbl23Fa, ITM_SQUARE_ROOT);          //JM
  labelCaptionAim(keys++, btn23A, lbl23Gr, lbl23L);
  labelCaptionAimFa(keys, lbl24Fa);                     //vv dr - new AIM //JM newest AIM
  labelCaptionAim(keys++, btn24A, lbl24Gr, lbl24L);
  labelCaptionAimFa(keys, lbl25Fa);                     //vv dr - new AIM //JM newest AIM
  labelCaptionAim(keys++, btn25A, lbl25Gr, lbl25L);
  labelCaptionAimFa(keys, lbl26Fa);                     //vv dr - new AIM //JM newest AIM
  labelCaptionAim(keys++, btn26A, lbl26Gr, lbl26L);

  labelCaptionAimFa(keys, lbl31Fa);                     //vv dr - new AIM //JM newest AIM
  labelCaptionAim(keys++, btn31A, lbl31Gr, lbl31L);
  labelCaptionAimFa(keys, lbl32Fa);                     //vv dr - new AIM //JM newest AIM
  labelCaptionAim(keys++, btn32A, lbl32Gr, lbl32L);
  labelCaptionAimFa(keys, lbl33Fa);                     //vv dr - new AIM //JM newest AIM
  labelCaptionAim(keys++, btn33A, lbl33Gr, lbl33L);
//  labelCaptionAimFaChr(   keys,   lbl34Fa, CHR_case);          //JMALPHA2 new
  labelCaptionAimFa(keys, lbl34Fa);                     //vv dr - new AIM //JM newest AIM
  labelCaptionAim(keys++, btn34A, lbl34Gr, lbl34L);
//  labelCaptionAimFaChr(   keys,   lbl35Fa, CHR_num);          //JMALPHA2 new
  labelCaptionAimFa(keys, lbl35Fa);                     //vv dr - new AIM //JM newest AIM
  labelCaptionAim(keys++, btn35A, lbl35Gr, lbl35L);
  labelCaptionAimFa(keys, lbl36Fa);                     //vv dr - new AIM //JM newest AIM
  labelCaptionAim(keys++, btn36A, lbl36Gr, lbl36L);     //^^

  labelCaptionAimFaChr(   keys,   lbl41Fa, ITM_XEDIT);  //JM
  labelCaptionAim(keys++, btn41,  lbl41Gr, lbl41L);
//  labelCaptionAimFaChr(   keys,   lbl42Fa, ITM_ex);     //vv dr - new AIM
  labelCaptionAimFa(keys, lbl42Fa);                     //vv dr - new AIM //JM newest AIM
  labelCaptionAim(keys++, btn42A, lbl42Gr, lbl42L);
//  labelCaptionAimFaChr(   keys,   lbl43Fa, ITM_PLUS_MINUS);
  labelCaptionAimFa(keys, lbl43Fa);                     //vv dr - new AIM //JM newest AIM
  labelCaptionAim(keys++, btn43A, lbl43Gr, lbl43L);
  labelCaptionAimFa(keys, lbl44Fa);                     //vv dr - new AIM //JM newest AIM
  labelCaptionAim(keys++, btn44A, lbl44Gr, lbl44L);     //^^
  labelCaptionAimFaChr(   keys,   lbl45Fa, ITM_CLA);  //JM
  labelCaptionAim(keys++, btn45,  lbl45Gr, lbl45L);

  labelCaptionAim(keys++, btn51,  lbl51Gr, lbl51L);
  labelCaptionAimFa(keys, lbl52Fa);                     //vv dr - new AIM
  labelCaptionAim(keys++, btn52A, lbl52Gr, lbl52L);
  labelCaptionAimFa(keys, lbl53Fa);
  labelCaptionAim(keys++, btn53A, lbl53Gr, lbl53L);
  labelCaptionAimFa(keys, lbl54Fa);
  labelCaptionAim(keys++, btn54A, lbl54Gr, lbl54L);
  labelCaptionAimFa(keys, lbl55Fa);
  labelCaptionAim(keys++, btn55A, lbl55Gr, lbl55L);     //^^

  labelCaptionAim(keys++, btn61,  lbl61Gr, lbl61L);
  labelCaptionAimFa(keys, lbl62Fa);                     //vv dr - new AIM
  labelCaptionAim(keys++, btn62A, lbl62Gr, lbl62L);
  labelCaptionAimFa(keys, lbl63Fa);
  labelCaptionAim(keys++, btn63A, lbl63Gr, lbl63L);
  labelCaptionAimFa(keys, lbl64Fa);
  labelCaptionAim(keys++, btn64A, lbl64Gr, lbl64L);
  labelCaptionAimFa(keys, lbl65Fa);
  labelCaptionAim(keys++, btn65A, lbl65Gr, lbl65L);     //^^

  labelCaptionAim(keys++, btn71,  lbl71Gr, lbl71L);
  labelCaptionAimFa(keys, lbl72Fa);                     //vv dr - new AIM
  labelCaptionAim(keys++, btn72A, lbl72Gr, lbl72L);
  labelCaptionAimFa(keys, lbl73Fa);
  labelCaptionAim(keys++, btn73A, lbl73Gr, lbl73L);
  labelCaptionAimFa(keys, lbl74Fa);
  labelCaptionAim(keys++, btn74A, lbl74Gr, lbl74L);
  labelCaptionAimFa(keys, lbl75Fa);
  labelCaptionAim(keys++, btn75A, lbl75Gr, lbl75L);     //^^

  labelCaptionAim(keys++, btn81,  lbl81Gr, lbl81L);
  labelCaptionAimFa(keys, lbl82Fa);                     //vv dr - new AIM
  labelCaptionAim(keys++, btn82A, lbl82Gr, lbl82L);
  labelCaptionAimFa(keys, lbl83Fa);
  labelCaptionAim(keys++, btn83A, lbl83Gr, lbl83L);
  labelCaptionAimFa(keys, lbl84Fa);
  labelCaptionAim(keys++, btn84A, lbl84Gr, lbl84L);
  labelCaptionAimFa(keys, lbl85Fa);
  labelCaptionAim(keys++, btn85A, lbl85Gr, lbl85L);     //^^

  //gtk_widget_show(lblOn);
  //JM7 gtk_widget_show(lblConfirmY);  //JM Y/N
  //JM7 gtk_widget_show(lblConfirmN);  //JM Y/N


  gtk_widget_show(btn11);
  gtk_widget_show(btn12);
  gtk_widget_show(btn13);
  gtk_widget_show(btn14);
  gtk_widget_show(btn15);
  gtk_widget_show(btn16);

  gtk_widget_show(btn21A);      //vv dr - new AIM
  gtk_widget_show(btn22A);
  gtk_widget_show(btn23A);
  gtk_widget_show(btn24A);
  gtk_widget_show(btn25A);
  gtk_widget_show(btn26A);

  gtk_widget_show(lbl21Fa);    //JM
  gtk_widget_show(lbl22Fa);    //JM
  gtk_widget_show(lbl23Fa);    //JM
  gtk_widget_show(lbl24Fa);    //JM AIM2
  gtk_widget_show(lbl25Fa);    //JM AIM2
  gtk_widget_show(lbl26Fa);    //JM AIM2
  
//  gtk_widget_show(lbl21H); //JMALPHA temporary remove A from Sigma+
/*gtk_widget_show(lbl21L);
  gtk_widget_show(lbl22L);
  gtk_widget_show(lbl23L);
  gtk_widget_show(lbl24L);
  gtk_widget_show(lbl25L);
  gtk_widget_show(lbl26L);*/    //^^
  //gtk_widget_show(lbl26F);
  //gtk_widget_show(lbl26G);
  gtk_widget_show(lbl21Gr);
  gtk_widget_show(lbl22Gr);
  gtk_widget_show(lbl23Gr);
  gtk_widget_show(lbl24Gr);
  gtk_widget_show(lbl25Gr);
  gtk_widget_show(lbl26Gr);

  gtk_widget_show(btn31A);      //vv dr - new AIM
  gtk_widget_show(btn32A);
  gtk_widget_show(btn33A);
  gtk_widget_show(btn34A);
  gtk_widget_show(btn35A);
  gtk_widget_show(btn36A);      //^^


  gtk_widget_show(lbl31Fa);    //JM AIM2
  gtk_widget_show(lbl32Fa);    //JM AIM2
  gtk_widget_show(lbl33Fa);    //JM AIM2
  gtk_widget_show(lbl34Fa);    //JM AIM2
  gtk_widget_show(lbl35Fa);    //JM AIM2
  gtk_widget_show(lbl36Fa);    //JM AIM2


//  gtk_widget_show(lbl34Fa);    //JMALPHA2
//  gtk_widget_show(lbl35Fa);    //JMALPHA2

  //gtk_widget_show(lbl31F);  JM
  //gtk_widget_show(lbl31L);    //dr - new AIM
  //gtk_widget_show(lbl32L);    //dr - new AIM
//  gtk_widget_show(lbl33H);
  //gtk_widget_show(lbl33L);    //dr - new AIM
  //gtk_widget_show(lbl34L);    //dr - new AIM
//#ifdef JM_LAYOUT_2_DM42_STRICT //JM LAYOUT 2. DM42 STRICT.
  //gtk_widget_show(lbl35L); // JM !!    //dr - new AIM
  //gtk_widget_show(lbl36L); // JM !!    //dr - new AIM
//#endif //JM END OF LAYOUT 2 DM42 STRICT.
  //gtk_widget_show(lbl34H);  //JMALPHA2 reinstate CAPS //JMALPHA2 temporary remove A from J
  gtk_widget_show(lbl31Gr);
  gtk_widget_show(lbl32Gr);
  gtk_widget_show(lbl33Gr);
  gtk_widget_show(lbl34Gr);
  gtk_widget_show(lbl35Gr); //JM !! GR
  gtk_widget_show(lbl36Gr); //JM !! GR

  gtk_widget_show(btn41);
  gtk_widget_show(btn42A);      //vv dr - new AIM
  gtk_widget_show(btn43A);
  gtk_widget_show(btn44A);      //^^
  gtk_widget_show(btn45);

  //gtk_widget_show(lbl41F);
  //gtk_widget_show(lbl41G);
  //gtk_widget_show(lbl42L);    //dr - new AIM
  //gtk_widget_show(lbl43L);
  gtk_widget_show(lbl41Fa);     //JM
  gtk_widget_show(lbl42Fa);
  gtk_widget_show(lbl43Fa);     //^^
  gtk_widget_show(lbl44Fa);    //JM AIM2
  gtk_widget_show(lbl43P);
  //gtk_widget_show(lbl44L);    //dr - new AIM
//  gtk_widget_show(lbl44P);
//  gtk_widget_show(lbl45F);
//  gtk_widget_show(lbl45G);
  gtk_widget_show(lbl42Gr);
  gtk_widget_show(lbl43Gr);
  gtk_widget_show(lbl44Gr);
  gtk_widget_show(lbl45Fa);     //^^

  gtk_widget_show(btn51);
  gtk_widget_show(btn52A);      //vv dr - new AIM
  gtk_widget_show(btn53A);
  gtk_widget_show(btn54A);
  gtk_widget_show(btn55A);      //^^

  gtk_widget_show(lbl51L);
  gtk_widget_show(lbl51F);
  //gtk_widget_show(lbl51G); //JM__
  //gtk_widget_show(lbl55F); //JM__
  //gtk_widget_show(lbl55G); //JM__
/*gtk_widget_show(lbl52L);      //vv dr - new AIM
  gtk_widget_show(lbl53L);
  gtk_widget_show(lbl54L);
  gtk_widget_show(lbl55L);*/
  gtk_widget_show(lbl52Fa);
  gtk_widget_show(lbl53Fa);
  gtk_widget_show(lbl54Fa);
  gtk_widget_show(lbl55Fa);     //^^
  gtk_widget_show(lbl51Gr);  //JMAHOME
  gtk_widget_show(lbl52Gr);
  gtk_widget_show(lbl53Gr);
  gtk_widget_show(lbl54Gr);
  gtk_widget_show(lbl55Gr);   //JM GREEK

  gtk_widget_show(btn61);
  gtk_widget_show(btn62A);      //vv dr - new AIM
  gtk_widget_show(btn63A);
  gtk_widget_show(btn64A);
  gtk_widget_show(btn65A);      //^^

  gtk_widget_show(lbl61L);
/*gtk_widget_show(lbl62L);      //vv dr - new AIM
  gtk_widget_show(lbl63L);
  gtk_widget_show(lbl64L);
  gtk_widget_show(lbl65L); //JM added*/
  gtk_widget_show(lbl62Fa);
  gtk_widget_show(lbl63Fa);
  gtk_widget_show(lbl64Fa);
  gtk_widget_show(lbl65Fa);     //^^

  gtk_widget_show(lbl61F); //JM_
  //gtk_widget_show(lbl61G); //JM_
  //gtk_widget_show(lbl65F); //JM
  //gtk_widget_show(lbl65G);
  gtk_widget_show(lbl61Gr); //JMAHOME2
  gtk_widget_show(lbl62Gr);
  gtk_widget_show(lbl63Gr);
  gtk_widget_show(lbl64Gr);
  gtk_widget_show(lbl65Gr); //JM ==

  gtk_widget_show(btn71);
  gtk_widget_show(btn72A);      //vv dr - new AIM
  gtk_widget_show(btn73A);
  gtk_widget_show(btn74A);
  gtk_widget_show(btn75A);      //^^

  gtk_widget_show(lbl71L);
/*gtk_widget_show(lbl72L);      //vv dr - new AIM
  gtk_widget_show(lbl73L);
  gtk_widget_show(lbl74L);
  gtk_widget_show(lbl75L); //JM added*/
  gtk_widget_show(lbl72Fa);
  gtk_widget_show(lbl73Fa);
  gtk_widget_show(lbl74Fa);
  gtk_widget_show(lbl75Fa);     //^^
  //gtk_widget_show(lbl71F);  //JM_          //JM REMOVE SHIFT LABEL
  //gtk_widget_show(lbl75G); //JM_
  gtk_widget_show(lbl71Gr);
  gtk_widget_show(lbl72Gr);
  gtk_widget_show(lbl73Gr);
  gtk_widget_show(lbl74Gr);
  gtk_widget_show(lbl75Gr); //JM ==

  gtk_widget_show(btn81);
  gtk_widget_show(btn82A);      //vv dr - new AIM
  gtk_widget_show(btn83A);
  gtk_widget_show(btn84A);
  gtk_widget_show(btn85A);      //^^

  gtk_widget_show(lbl81L);
  gtk_widget_show(lbl81F); //JM added OFF with Layout 42 LAYOUT42
  gtk_widget_show(lbl81G); //JM added OFF with Layout 42 LAYOUT42
/*gtk_widget_show(lbl82L);      //vv dr - new AIM
  gtk_widget_show(lbl83L);
  gtk_widget_show(lbl84L);
  gtk_widget_show(lbl85L);  //JM add ?*/
  gtk_widget_show(lbl82Fa);
  gtk_widget_show(lbl83Fa);
  gtk_widget_show(lbl84Fa);
  gtk_widget_show(lbl85Fa);     //^^
/*gtk_widget_show(lbl82H);  //JM AIM MENU   //vv dr - new AIM
  gtk_widget_show(lbl83H);  //JM AIM MENU
  gtk_widget_show(lbl84H);  //JM AIM MENU
  gtk_widget_show(lbl85H);  //JM AIM MENU*/ //^^
  //gtk_widget_show(lbl85F); //JM
  //gtk_widget_show(lbl85G); //JM

//  gtk_widget_show(lbl81Gr); //JMAPRT
  gtk_widget_show(lbl82Gr);
  gtk_widget_show(lbl83Gr);
  gtk_widget_show(lbl84Gr); //JM TT
  gtk_widget_show(lbl85Gr); //JM

  //gtk_widget_show(lblOn);
  //JM7 gtk_widget_show(lblConfirmY);  //JM Y/N
  //JM7 gtk_widget_show(lblConfirmN);  //JM Y/N

//  gtk_widget_show(lbl31G);   //JM Remnanty from 43S. Not sure why. Removed.
  moveLabels();
//  gtk_widget_hide(lbl31G);   //JM Remnanty from 43S. Not sure why. Removed.
}



    void calcModeTamGui(void) {
      const calcKey_t *keys;

  if(running_program_jm) return;                        //JM faster during program excution
  
  keys = getSystemFlag(FLAG_USER) ? kbd_usr : kbd_std;

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
#endif // SCREEN_800X480


/********************************************//**
 * \brief Creates the calc's GUI window with all the widgets
 *
 * \param void
 * \return void
 ***********************************************/
void setupUI(void) {
#if (SCREEN_800X480 == 0)
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
    moreInfoOnError("In function setupUI:", "error while loading CSS style sheet " CSSFILE, NULL, NULL);
    exit(1);
  }
  g_object_unref(cssProvider);
  free(cssData);

      // Get the monitor geometry to determine whether the calc is portrait or landscape
      GdkRectangle monitor;
      gdk_monitor_get_geometry(gdk_display_get_monitor(gdk_display_get_default(), 0), &monitor);
      //gdk_screen_get_monitor_geometry(gdk_screen_get_default(), 0, &monitor);

      if(calcAutoLandscapePortrait) {
        calcLandscape = (monitor.height < 1025);
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
  gtk_window_set_title(GTK_WINDOW(frmCalc), "C43");                   //JM NAME
  g_signal_connect(frmCalc, "destroy", G_CALLBACK(destroyCalc), NULL);
  g_signal_connect(frmCalc, "key_press_event", G_CALLBACK(keyPressed), NULL);
  g_signal_connect(frmCalc, "key_release_event", G_CALLBACK(keyReleased), NULL);  //JM CTRL
      #ifdef RASPBERRY
        gtk_window_set_decorated(GTK_WINDOW(frmCalc), FALSE);
        gtk_window_set_position(GTK_WINDOW(frmCalc), GTK_WIN_POS_CENTER);
      #endif // RASPBERRY

      gtk_widget_add_events(GTK_WIDGET(frmCalc), GDK_CONFIGURE);

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
    backgroundImage = gtk_image_new_from_file("dm42l_L1.png");
#endif //JM Layout

  }

      gtk_fixed_put(GTK_FIXED(grid), backgroundImage, 0, 0);

#if defined(JM_LAYOUT_1A)  //JM LAYOUT 1. FINAL. Show colour band next to LCD


#ifdef S43
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
#endif

  // Frame around the f key
  lblFKey2 = gtk_label_new("");  
  gtk_widget_set_name(lblFKey2, "fSoftkeyArea");
  gtk_widget_set_size_request(lblFKey2, 61-8-2-2,  5-2);
  gtk_fixed_put(GTK_FIXED(grid), lblFKey2, 350+4+2, 563-1);


  // Frame around the g key
  lblGKey2 = gtk_label_new("");
  gtk_widget_set_name(lblGKey2, "gSoftkeyArea");
  gtk_widget_set_size_request(lblGKey2, 61-8-2-2,  5-2);
  gtk_fixed_put(GTK_FIXED(grid), lblGKey2, 350+4+2 + DELTA_KEYS_X, 563-1);


  // Frame around the Sigma+ key
//  lblEKey = gtk_label_new("");
//  gtk_widget_set_name(lblEKey,"eSoftkeyArea");
//  gtk_widget_set_size_request(lblEKey, 61-8-2-2,  5-2);
//  gtk_fixed_put(GTK_FIXED(grid), lblEKey, 350+4+2 - 4 * DELTA_KEYS_X, 563-1 - DELTA_KEYS_Y);

  // Frame around the Sigma- key
//  lblEEKey = gtk_label_new("");
//  gtk_widget_set_name(lblEEKey,"eSoftkeyArea");
//  gtk_widget_set_size_request(lblEEKey, 61-8-2-2,  5-2);
//  gtk_fixed_put(GTK_FIXED(grid), lblEEKey, 350+4+2 - 4 * DELTA_KEYS_X, 563-1 - 2*DELTA_KEYS_Y);


  // Frame around the SIN key
//  lblSKey = gtk_label_new("");
//  gtk_widget_set_name(lblSKey,"eSoftkeyArea");
//  gtk_widget_set_size_request(lblSKey, 61-8-2-2,  5-2);
//  gtk_fixed_put(GTK_FIXED(grid), lblSKey, 350+4+2 - 1 * DELTA_KEYS_X, 563-1 -0* DELTA_KEYS_Y);

#endif //JM

  // Area for the softkeys
  //lblSoftkeyArea1 = gtk_label_new("");
  //gtk_widget_set_name(lblSoftkeyArea1, "softkeyArea");
  //gtk_widget_set_size_request(lblSoftkeyArea1, 460, 40);
  //gtk_fixed_put(GTK_FIXED(grid), lblSoftkeyArea1, 33, 72+168+50);

  //lblSoftkeyArea2 = gtk_label_new("");
  //gtk_widget_set_name(lblSoftkeyArea2, "softkeyArea");
  //gtk_widget_set_size_request(lblSoftkeyArea2, 460, 44);
  //gtk_fixed_put(GTK_FIXED(grid), lblSoftkeyArea2, 33, 72+168+50+66);



  // Behind screen
/*lblBehindScreen = gtk_label_new("");                          //vv JM
  gtk_widget_set_name(lblBehindScreen, "behindScreen");
  gtk_widget_set_size_request(lblBehindScreen, 412, 252);
  gtk_fixed_put(GTK_FIXED(grid), lblBehindScreen, 57, 66);*/    //^^



  // LCD screen 400x240
  screen = gtk_drawing_area_new();
  gtk_widget_set_size_request(screen, SCREEN_WIDTH, SCREEN_HEIGHT);
  gtk_widget_set_tooltip_text(GTK_WIDGET(screen), "Copy to clipboard:\n CTRL+h: Screen image\n CTRL+c/x: X Register\n CTRL+d: Lettered Registers\n CTRL+a: All Registers\n");  //JM
  gtk_fixed_put(GTK_FIXED(grid), screen, 63, 72);
  screenStride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, SCREEN_WIDTH)/4;
  numBytes = screenStride * SCREEN_HEIGHT * 4;
  screenData = malloc(numBytes);
  if(screenData == NULL) {
    sprintf(errorMessage, "error allocating %d x %d = %d bytes for screenData", screenStride * 4, SCREEN_HEIGHT, numBytes);
    moreInfoOnError("In function setupUI:", errorMessage, NULL, NULL);
    exit(1);
  }

      g_signal_connect(screen, "draw", G_CALLBACK(drawScreen), NULL);


  #if (DEBUG_REGISTER_L == 1)
    lblRegisterL1 = gtk_label_new("");
    lblRegisterL2 = gtk_label_new("");
    gtk_widget_set_name(lblRegisterL1, "registerL");
    gtk_widget_set_name(lblRegisterL2, "registerL");
    gtk_fixed_put(GTK_FIXED(grid), lblRegisterL1, 5, 28);
    gtk_fixed_put(GTK_FIXED(grid), lblRegisterL2, 5, 46);
  #endif

  #if (SHOW_MEMORY_STATUS == 1)
    lblMemoryStatus = gtk_label_new("");
    gtk_widget_set_name(lblMemoryStatus, "memoryStatus");
    gtk_fixed_put(GTK_FIXED(grid), lblMemoryStatus, 5, 5);
  #endif

      // 1st row: F1 to F6 buttons
      btn11 = gtk_button_new_with_label("");
      btn12 = gtk_button_new_with_label("");
      btn13 = gtk_button_new_with_label("");
      btn14 = gtk_button_new_with_label("");
      btn15 = gtk_button_new_with_label("");
      btn16 = gtk_button_new_with_label("");

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

      g_signal_connect(btn11, "button-press-event", G_CALLBACK(btnFnPressed), "1");
      g_signal_connect(btn12, "button-press-event", G_CALLBACK(btnFnPressed), "2");
      g_signal_connect(btn13, "button-press-event", G_CALLBACK(btnFnPressed), "3");
      g_signal_connect(btn14, "button-press-event", G_CALLBACK(btnFnPressed), "4");
      g_signal_connect(btn15, "button-press-event", G_CALLBACK(btnFnPressed), "5");
      g_signal_connect(btn16, "button-press-event", G_CALLBACK(btnFnPressed), "6");
      g_signal_connect(btn11, "button-release-event", G_CALLBACK(btnFnReleased), "1");
      g_signal_connect(btn12, "button-release-event", G_CALLBACK(btnFnReleased), "2");
      g_signal_connect(btn13, "button-release-event", G_CALLBACK(btnFnReleased), "3");
      g_signal_connect(btn14, "button-release-event", G_CALLBACK(btnFnReleased), "4");
      g_signal_connect(btn15, "button-release-event", G_CALLBACK(btnFnReleased), "5");
      g_signal_connect(btn16, "button-release-event", G_CALLBACK(btnFnReleased), "6");

      gtk_widget_set_focus_on_click(btn11, FALSE);
      gtk_widget_set_focus_on_click(btn12, FALSE);
      gtk_widget_set_focus_on_click(btn13, FALSE);
      gtk_widget_set_focus_on_click(btn14, FALSE);
      gtk_widget_set_focus_on_click(btn15, FALSE);
      gtk_widget_set_focus_on_click(btn16, FALSE);

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
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn21), "a");  //vv dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn22), "v");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn23), "q");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn24), "o");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn25), "l");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn26), "x");  //^^
  btn21A  = gtk_button_new();                           //vv dr - new AIM
  btn22A  = gtk_button_new();
  btn23A  = gtk_button_new();
  btn24A  = gtk_button_new();
  btn25A  = gtk_button_new();
  btn26A  = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn21A), "A");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn22A), "B");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn23A), "C");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn24A), "D");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn25A), "E");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn26A), "F"); //^^
  lbl21F  = gtk_label_new("");
  lbl22F  = gtk_label_new("");
  lbl23F  = gtk_label_new("");
  lbl24F  = gtk_label_new("");
  lbl25F  = gtk_label_new("");
  lbl26F  = gtk_label_new("");
  lbl21Fa = gtk_label_new("");          //JM
  lbl22Fa = gtk_label_new("");          //JM
  lbl23Fa = gtk_label_new("");          //JM
  lbl24Fa = gtk_label_new("");          //JM AIM2
  lbl25Fa = gtk_label_new("");          //JM AIM2
  lbl26Fa = gtk_label_new("");          //JM AIM2
  lbl21G  = gtk_label_new("");
  lbl22G  = gtk_label_new("");
  lbl23G  = gtk_label_new("");
  lbl24G  = gtk_label_new("");
  lbl25G  = gtk_label_new("");
  lbl26G  = gtk_label_new("");
  //lbl21H  = gtk_label_new("A"); // ? //JMALPHA
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
  gtk_widget_set_size_request(btn21A, KEY_WIDTH_1, 0);  //vv dr - new AIM
  gtk_widget_set_size_request(btn22A, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn23A, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn24A, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn25A, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn26A, KEY_WIDTH_1, 0);  //^^

//  gtk_widget_set_name(lbl21H,  "fShiftedUnderline"); //JMALPHA
//  gtk_widget_set_name(lbl21Fa,  "fShiftedUnderline"); //JMALPHA2


  g_signal_connect(btn21, "button-press-event", G_CALLBACK(btnPressed), "00");
  g_signal_connect(btn22, "button-press-event", G_CALLBACK(btnPressed), "01");
  g_signal_connect(btn23, "button-press-event", G_CALLBACK(btnPressed), "02");
  g_signal_connect(btn24, "button-press-event", G_CALLBACK(btnPressed), "03");
  g_signal_connect(btn25, "button-press-event", G_CALLBACK(btnPressed), "04");
  g_signal_connect(btn26, "button-press-event", G_CALLBACK(btnPressed), "05");
  g_signal_connect(btn21, "button-release-event", G_CALLBACK(btnReleased), "00");
  g_signal_connect(btn22, "button-release-event", G_CALLBACK(btnReleased), "01");
  g_signal_connect(btn23, "button-release-event", G_CALLBACK(btnReleased), "02");
  g_signal_connect(btn24, "button-release-event", G_CALLBACK(btnReleased), "03");
  g_signal_connect(btn25, "button-release-event", G_CALLBACK(btnReleased), "04");
  g_signal_connect(btn26, "button-release-event", G_CALLBACK(btnReleased), "05");
  g_signal_connect(btn21A, "button-press-event", G_CALLBACK(btnPressed), "00");    //vv dr - new AIM
  g_signal_connect(btn22A, "button-press-event", G_CALLBACK(btnPressed), "01");
  g_signal_connect(btn23A, "button-press-event", G_CALLBACK(btnPressed), "02");
  g_signal_connect(btn24A, "button-press-event", G_CALLBACK(btnPressed), "03");
  g_signal_connect(btn25A, "button-press-event", G_CALLBACK(btnPressed), "04");
  g_signal_connect(btn26A, "button-press-event", G_CALLBACK(btnPressed), "05");
  g_signal_connect(btn21A, "button-release-event", G_CALLBACK(btnReleased), "00");
  g_signal_connect(btn22A, "button-release-event", G_CALLBACK(btnReleased), "01");
  g_signal_connect(btn23A, "button-release-event", G_CALLBACK(btnReleased), "02");
  g_signal_connect(btn24A, "button-release-event", G_CALLBACK(btnReleased), "03");
  g_signal_connect(btn25A, "button-release-event", G_CALLBACK(btnReleased), "04");
  g_signal_connect(btn26A, "button-release-event", G_CALLBACK(btnReleased), "05");  //^^

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
  gtk_fixed_put(GTK_FIXED(grid), lbl21Fa, 0, 0);            //JM
  gtk_fixed_put(GTK_FIXED(grid), lbl22Fa, 0, 0);            //JM
  gtk_fixed_put(GTK_FIXED(grid), lbl23Fa, 0, 0);            //JM
  gtk_fixed_put(GTK_FIXED(grid), lbl24Fa, 0, 0);            //JM
  gtk_fixed_put(GTK_FIXED(grid), lbl25Fa, 0, 0);            //JM
  gtk_fixed_put(GTK_FIXED(grid), lbl26Fa, 0, 0);            //JM
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
//  gtk_fixed_put(GTK_FIXED(grid), lbl21H, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1); //JMALPHA
  gtk_fixed_put(GTK_FIXED(grid), btn21A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn22,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl22L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), btn22A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn23,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl23L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), btn23A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn24,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl24L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), btn24A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn25,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl25L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), btn25A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn26,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl26L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), btn26A, xPos,                         yPos);   //dr - new AIM



  // 3rd row
  btn31   = gtk_button_new();
  btn32   = gtk_button_new();
  btn33   = gtk_button_new();
  btn34   = gtk_button_new();
  btn35   = gtk_button_new();
  btn36   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn31), "m");  //vv dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn32), "r");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn33), "d");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn34), "s");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn35), "c");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn36), "t");  //^^
  btn31A  = gtk_button_new();                           //vv dr - new AIM
  btn32A  = gtk_button_new();
  btn33A  = gtk_button_new();
  btn34A  = gtk_button_new();
  btn35A  = gtk_button_new();
  btn36A  = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn31A), "G");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn32A), "H");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn33A), "I");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn34A), "J");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn35A), "K");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn36A), "L"); //^^
  lbl31F  = gtk_label_new("");
  lbl32F  = gtk_label_new("");
  lbl33F  = gtk_label_new("");
  lbl34F  = gtk_label_new("");
  lbl35F  = gtk_label_new("");
  lbl36F  = gtk_label_new("");
  lbl31Fa = gtk_label_new("");          //JM AIM2
  lbl32Fa = gtk_label_new("");          //JM AIM2
  lbl33Fa = gtk_label_new("");          //JM AIM2
  lbl34Fa = gtk_label_new("");          //JM AIM2
  lbl35Fa = gtk_label_new("");          //JM AIM2
  lbl36Fa = gtk_label_new("");          //JM AIM2
//  lbl34Fa  = gtk_label_new("");  //JMALPHA2
//  lbl35Fa  = gtk_label_new("");  //JMALPHA2
  lbl31G  = gtk_label_new("");
  lbl32G  = gtk_label_new("");
  lbl33G  = gtk_label_new("");
  lbl34G  = gtk_label_new("");
  lbl35G  = gtk_label_new("");
  lbl36G  = gtk_label_new("");
//  lbl33H  = gtk_label_new("\u21e9"); // Hollow down
//  lbl34H  = gtk_label_new("\u2102"); //JM CAPS LOCK    //JMALPHA2 REMOVED                          // ("\u03b7"); // eta          //JM removed1
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
  gtk_widget_set_size_request(btn31A, KEY_WIDTH_1, 0);  //vv dr- new AIM
  gtk_widget_set_size_request(btn32A, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn33A, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn34A, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn35A, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn36A, KEY_WIDTH_1, 0);  //^^

  //gtk_widget_set_name(lbl33H,  "fShifted");
  //gtk_widget_set_name(lbl34H,  "fShifted");  //JM CAPS JMALPHA2
  // gtk_widget_set_name(lbl34H,  "gShifted");   //JM removed1

  g_signal_connect(btn31, "button-press-event", G_CALLBACK(btnPressed), "06");
  g_signal_connect(btn32, "button-press-event", G_CALLBACK(btnPressed), "07");
  g_signal_connect(btn33, "button-press-event", G_CALLBACK(btnPressed), "08");
  g_signal_connect(btn34, "button-press-event", G_CALLBACK(btnPressed), "09");
  g_signal_connect(btn35, "button-press-event", G_CALLBACK(btnPressed), "10");
  g_signal_connect(btn36, "button-press-event", G_CALLBACK(btnPressed), "11");
  g_signal_connect(btn31, "button-release-event", G_CALLBACK(btnReleased), "06");
  g_signal_connect(btn32, "button-release-event", G_CALLBACK(btnReleased), "07");
  g_signal_connect(btn33, "button-release-event", G_CALLBACK(btnReleased), "08");
  g_signal_connect(btn34, "button-release-event", G_CALLBACK(btnReleased), "09");
  g_signal_connect(btn35, "button-release-event", G_CALLBACK(btnReleased), "10");
  g_signal_connect(btn36, "button-release-event", G_CALLBACK(btnReleased), "11");
  g_signal_connect(btn31A, "button-press-event", G_CALLBACK(btnPressed), "06");    //vv dr - new AIM
  g_signal_connect(btn32A, "button-press-event", G_CALLBACK(btnPressed), "07");
  g_signal_connect(btn33A, "button-press-event", G_CALLBACK(btnPressed), "08");
  g_signal_connect(btn34A, "button-press-event", G_CALLBACK(btnPressed), "09");
  g_signal_connect(btn35A, "button-press-event", G_CALLBACK(btnPressed), "10");
  g_signal_connect(btn36A, "button-press-event", G_CALLBACK(btnPressed), "11");
  g_signal_connect(btn31A, "button-release-event", G_CALLBACK(btnReleased), "06");
  g_signal_connect(btn32A, "button-release-event", G_CALLBACK(btnReleased), "07");
  g_signal_connect(btn33A, "button-release-event", G_CALLBACK(btnReleased), "08");
  g_signal_connect(btn34A, "button-release-event", G_CALLBACK(btnReleased), "09");
  g_signal_connect(btn35A, "button-release-event", G_CALLBACK(btnReleased), "10");
  g_signal_connect(btn36A, "button-release-event", G_CALLBACK(btnReleased), "11");  //^^

  gtk_fixed_put(GTK_FIXED(grid), lbl31F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl32F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl33F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl34F,  0, 0);
//  gtk_fixed_put(GTK_FIXED(grid), lbl34Fa, 0, 0);            //JMALPHA2
//  gtk_fixed_put(GTK_FIXED(grid), lbl35Fa, 0, 0);            //JMALPHA2
  gtk_fixed_put(GTK_FIXED(grid), lbl35F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl36F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl31Fa, 0, 0);    //^^ AIM2
  gtk_fixed_put(GTK_FIXED(grid), lbl32Fa, 0, 0);    //^^ AIM2
  gtk_fixed_put(GTK_FIXED(grid), lbl33Fa, 0, 0);    //^^ AIM2
  gtk_fixed_put(GTK_FIXED(grid), lbl34Fa, 0, 0);    //^^ AIM2
  gtk_fixed_put(GTK_FIXED(grid), lbl35Fa, 0, 0);    //^^ AIM2
  gtk_fixed_put(GTK_FIXED(grid), lbl36Fa, 0, 0);    //^^ AIM2
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
  gtk_fixed_put(GTK_FIXED(grid), btn31A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn32,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl32L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), btn32A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn33,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl33L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
//  gtk_fixed_put(GTK_FIXED(grid), lbl33H, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);
  gtk_fixed_put(GTK_FIXED(grid), btn33A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn34,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl34L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
//  gtk_fixed_put(GTK_FIXED(grid), lbl34H, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);               //JM CAPS //JMALPHA temporary remove A from J
  gtk_fixed_put(GTK_FIXED(grid), btn34A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn35,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl35L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), btn35A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn36,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl36L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), btn36A, xPos,                         yPos);   //dr - new AIM



  // 4th row
  btn41   = gtk_button_new();
  btn42   = gtk_button_new();
  btn43   = gtk_button_new();
  btn44   = gtk_button_new();
  btn45   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn41), "Enter");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn42), "w");  //vv dr
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn43), "n");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn44), "e");  //^^
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn45), "Backspace");
  btn42A  = gtk_button_new();                           //vv dr - new AIM
  btn43A  = gtk_button_new();
  btn44A  = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn42A), "M");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn43A), "N");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn44A), "O"); //^^
  lbl41F  = gtk_label_new("");
  lbl42F  = gtk_label_new("");
  lbl43F  = gtk_label_new("");
  lbl44F  = gtk_label_new("");
  lbl45F  = gtk_label_new("");
  lbl41Fa = gtk_label_new("");  //JM
  lbl42Fa = gtk_label_new("");  //vv dr - new AIM
  lbl43Fa = gtk_label_new("");  //^^
  lbl44Fa = gtk_label_new("");          //JM AIM2
  lbl45Fa = gtk_label_new("");  //^^
  lbl41G  = gtk_label_new("");
  lbl42G  = gtk_label_new("");
  lbl43G  = gtk_label_new("");
  lbl44G  = gtk_label_new("");
  lbl45G  = gtk_label_new("");
  lbl42H  = gtk_label_new("");
  lbl43P  = gtk_label_new("");
  //lbl44P  = gtk_label_new("\u21e7"); // Hollow up
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
  gtk_widget_set_size_request(btn42, KEY_WIDTH_1,                0);
  gtk_widget_set_size_request(btn43, KEY_WIDTH_1,                0);
  gtk_widget_set_size_request(btn44, KEY_WIDTH_1,                0);
  gtk_widget_set_size_request(btn45, KEY_WIDTH_1,                0);
  gtk_widget_set_size_request(btn42A, KEY_WIDTH_1,               0);    //vv dr - new AIM
  gtk_widget_set_size_request(btn43A, KEY_WIDTH_1,               0);
  gtk_widget_set_size_request(btn44A, KEY_WIDTH_1,               0);    //^^

  gtk_widget_set_name(lbl43P,  "fShifted");
  //gtk_widget_set_name(lbl44P,  "fShifted");

  g_signal_connect(btn41, "button-press-event", G_CALLBACK(btnPressed), "12");
  g_signal_connect(btn42, "button-press-event", G_CALLBACK(btnPressed), "13");
  g_signal_connect(btn43, "button-press-event", G_CALLBACK(btnPressed), "14");
  g_signal_connect(btn44, "button-press-event", G_CALLBACK(btnPressed), "15");
  g_signal_connect(btn45, "button-press-event", G_CALLBACK(btnPressed), "16");
  g_signal_connect(btn41, "button-release-event", G_CALLBACK(btnReleased), "12");
  g_signal_connect(btn42, "button-release-event", G_CALLBACK(btnReleased), "13");
  g_signal_connect(btn43, "button-release-event", G_CALLBACK(btnReleased), "14");
  g_signal_connect(btn44, "button-release-event", G_CALLBACK(btnReleased), "15");
  g_signal_connect(btn45, "button-release-event", G_CALLBACK(btnReleased), "16");
  g_signal_connect(btn42A, "button-press-event", G_CALLBACK(btnPressed), "13");    //vv dr - new AIM
  g_signal_connect(btn43A, "button-press-event", G_CALLBACK(btnPressed), "14");
  g_signal_connect(btn44A, "button-press-event", G_CALLBACK(btnPressed), "15");
  g_signal_connect(btn42A, "button-release-event", G_CALLBACK(btnReleased), "13");
  g_signal_connect(btn43A, "button-release-event", G_CALLBACK(btnReleased), "14");
  g_signal_connect(btn44A, "button-release-event", G_CALLBACK(btnReleased), "15");  //^^

  gtk_fixed_put(GTK_FIXED(grid), lbl41F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl42F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl43F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl44F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl45F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl41Fa, 0, 0);    //JM
  gtk_fixed_put(GTK_FIXED(grid), lbl42Fa, 0, 0);    //vv dr - new AIM
  gtk_fixed_put(GTK_FIXED(grid), lbl43Fa, 0, 0);    //^^
  gtk_fixed_put(GTK_FIXED(grid), lbl44Fa, 0, 0);    //^^ AIM2
  gtk_fixed_put(GTK_FIXED(grid), lbl45Fa, 0, 0);    //^^
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
  gtk_fixed_put(GTK_FIXED(grid), btn42A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn43,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl43L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl43P, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);
  gtk_fixed_put(GTK_FIXED(grid), btn43A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn44,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl44L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
//  gtk_fixed_put(GTK_FIXED(grid), lbl44P, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);
  gtk_fixed_put(GTK_FIXED(grid), btn44A, xPos,                         yPos);   //dr - new AIM

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
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn55), "/");  //JM
  btn52A   = gtk_button_new();                          //vv dr - new AIM
  btn53A   = gtk_button_new();
  btn54A   = gtk_button_new();
  btn55A   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn52A), "P");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn53A), "Q");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn54A), "R");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn55A), "S"); //^^
  lbl51F  = gtk_label_new("");
  lbl52F  = gtk_label_new("");
  lbl53F  = gtk_label_new("");
  lbl54F  = gtk_label_new("");
  lbl55F  = gtk_label_new("");
  lbl52Fa = gtk_label_new("");  //vv dr - new AIM
  lbl53Fa = gtk_label_new("");
  lbl54Fa = gtk_label_new("");
  lbl55Fa = gtk_label_new("");  //^^
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
  gtk_widget_set_size_request(btn52A, KEY_WIDTH_2, 0);  //vv dr - new AIM
  gtk_widget_set_size_request(btn53A, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn54A, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn55A, KEY_WIDTH_2, 0);  //^^

  g_signal_connect(btn51, "button-press-event", G_CALLBACK(btnPressed), "17");
  g_signal_connect(btn52, "button-press-event", G_CALLBACK(btnPressed), "18");
  g_signal_connect(btn53, "button-press-event", G_CALLBACK(btnPressed), "19");
  g_signal_connect(btn54, "button-press-event", G_CALLBACK(btnPressed), "20");
  g_signal_connect(btn55, "button-press-event", G_CALLBACK(btnPressed), "21");
  g_signal_connect(btn51, "button-release-event", G_CALLBACK(btnReleased), "17");
  g_signal_connect(btn52, "button-release-event", G_CALLBACK(btnReleased), "18");
  g_signal_connect(btn53, "button-release-event", G_CALLBACK(btnReleased), "19");
  g_signal_connect(btn54, "button-release-event", G_CALLBACK(btnReleased), "20");
  g_signal_connect(btn55, "button-release-event", G_CALLBACK(btnReleased), "21");
  g_signal_connect(btn52A, "button-press-event", G_CALLBACK(btnPressed), "18");    //vv dr - new AIM
  g_signal_connect(btn53A, "button-press-event", G_CALLBACK(btnPressed), "19");
  g_signal_connect(btn54A, "button-press-event", G_CALLBACK(btnPressed), "20");
  g_signal_connect(btn55A, "button-press-event", G_CALLBACK(btnPressed), "21");
  g_signal_connect(btn52A, "button-release-event", G_CALLBACK(btnReleased), "18");
  g_signal_connect(btn53A, "button-release-event", G_CALLBACK(btnReleased), "19");
  g_signal_connect(btn54A, "button-release-event", G_CALLBACK(btnReleased), "20");
  g_signal_connect(btn55A, "button-release-event", G_CALLBACK(btnReleased), "21");  //^^

  gtk_fixed_put(GTK_FIXED(grid), lbl51F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl52F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl53F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl54F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl55F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl52Fa, 0, 0);    //vv dr - new AIM
  gtk_fixed_put(GTK_FIXED(grid), lbl53Fa, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl54Fa, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl55Fa, 0, 0);    //^^
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
  gtk_fixed_put(GTK_FIXED(grid), btn52A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn53,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl53L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), btn53A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn54,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl54L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), btn54A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn55,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl55L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), btn55A, xPos,                         yPos);   //dr - new AIM



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
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn65), "*");  //JM
  btn62A  = gtk_button_new();                           //vv dr - new AIM
  btn63A  = gtk_button_new();
  btn64A  = gtk_button_new();
  btn65A  = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn62A), "T");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn63A), "U");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn64A), "V");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn65A), "W"); //^^
  lbl61F  = gtk_label_new("");
  lbl62F  = gtk_label_new("");
  lbl63F  = gtk_label_new("");
  lbl64F  = gtk_label_new("");
  lbl65F  = gtk_label_new("");
  lbl62Fa = gtk_label_new("");  //vv dr - new AIM
  lbl63Fa = gtk_label_new("");
  lbl64Fa = gtk_label_new("");
  lbl65Fa = gtk_label_new("");  //^^
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
  gtk_widget_set_size_request(btn62A, KEY_WIDTH_2, 0);  //vv dr - new AIM
  gtk_widget_set_size_request(btn63A, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn64A, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn65A, KEY_WIDTH_2, 0);  //^^

  gtk_widget_set_name(lbl65H,  "gShifted");  //JM

  g_signal_connect(btn61, "button-press-event", G_CALLBACK(btnPressed), "22");
  g_signal_connect(btn62, "button-press-event", G_CALLBACK(btnPressed), "23");
  g_signal_connect(btn63, "button-press-event", G_CALLBACK(btnPressed), "24");
  g_signal_connect(btn64, "button-press-event", G_CALLBACK(btnPressed), "25");
  g_signal_connect(btn65, "button-press-event", G_CALLBACK(btnPressed), "26");
  g_signal_connect(btn61, "button-release-event", G_CALLBACK(btnReleased), "22");
  g_signal_connect(btn62, "button-release-event", G_CALLBACK(btnReleased), "23");
  g_signal_connect(btn63, "button-release-event", G_CALLBACK(btnReleased), "24");
  g_signal_connect(btn64, "button-release-event", G_CALLBACK(btnReleased), "25");
  g_signal_connect(btn65, "button-release-event", G_CALLBACK(btnReleased), "26");
  g_signal_connect(btn62A, "button-press-event", G_CALLBACK(btnPressed), "23");    //vv - new AIM
  g_signal_connect(btn63A, "button-press-event", G_CALLBACK(btnPressed), "24");
  g_signal_connect(btn64A, "button-press-event", G_CALLBACK(btnPressed), "25");
  g_signal_connect(btn65A, "button-press-event", G_CALLBACK(btnPressed), "26");
  g_signal_connect(btn62A, "button-release-event", G_CALLBACK(btnReleased), "23");
  g_signal_connect(btn63A, "button-release-event", G_CALLBACK(btnReleased), "24");
  g_signal_connect(btn64A, "button-release-event", G_CALLBACK(btnReleased), "25");
  g_signal_connect(btn65A, "button-release-event", G_CALLBACK(btnReleased), "26");  //^^

  gtk_fixed_put(GTK_FIXED(grid), lbl61F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl62F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl63F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl64F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl62Fa, 0, 0);    //vv dr - new AIM
  gtk_fixed_put(GTK_FIXED(grid), lbl63Fa, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl64Fa, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl65Fa, 0, 0);    //^^
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
  gtk_fixed_put(GTK_FIXED(grid), btn62A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn63,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl63L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), btn63A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn64,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl64L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
 // gtk_fixed_put(GTK_FIXED(grid), lbl64H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1);  //JM
  gtk_fixed_put(GTK_FIXED(grid), btn64A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn65,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl65L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl65H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1); //JM
  gtk_fixed_put(GTK_FIXED(grid), btn65A, xPos,                         yPos);   //dr - new AIM



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
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn75), "-");  //JM
  btn72A   = gtk_button_new();                          //vv dr - new AIM
  btn73A   = gtk_button_new();
  btn74A   = gtk_button_new();
  btn75A   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn72A), "X");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn73A), "Y");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn74A), "Z");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn75A), "_"); //dr ^^^^ - new AIM
  lbl71F  = gtk_label_new("");
  lbl72F  = gtk_label_new("");
  lbl73F  = gtk_label_new("");
  lbl74F  = gtk_label_new("");
  lbl75F  = gtk_label_new("");
  lbl72Fa = gtk_label_new("");  //vv dr - new AIM
  lbl73Fa = gtk_label_new("");
  lbl74Fa = gtk_label_new("");
  lbl75Fa = gtk_label_new("");  //^^
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
  gtk_widget_set_size_request(btn72A, KEY_WIDTH_2, 0);  //vv dr - new AIM
  gtk_widget_set_size_request(btn73A, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn74A, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn75A, KEY_WIDTH_2, 0);  //^^

  //JM Removed1 gtk_widget_set_name(lbl72H,  "gShiftedUnderline"); //JM
  //JM Removed1   gtk_widget_set_name(lbl73H,  "gShifted");  //JM

  g_signal_connect(btn71, "button-press-event", G_CALLBACK(btnPressed), "27");
  g_signal_connect(btn72, "button-press-event", G_CALLBACK(btnPressed), "28");
  g_signal_connect(btn73, "button-press-event", G_CALLBACK(btnPressed), "29");
  g_signal_connect(btn74, "button-press-event", G_CALLBACK(btnPressed), "30");
  g_signal_connect(btn75, "button-press-event", G_CALLBACK(btnPressed), "31");
  g_signal_connect(btn71, "button-release-event", G_CALLBACK(btnReleased), "27");
  g_signal_connect(btn72, "button-release-event", G_CALLBACK(btnReleased), "28");
  g_signal_connect(btn73, "button-release-event", G_CALLBACK(btnReleased), "29");
  g_signal_connect(btn74, "button-release-event", G_CALLBACK(btnReleased), "30");
  g_signal_connect(btn75, "button-release-event", G_CALLBACK(btnReleased), "31");
  g_signal_connect(btn72A, "button-press-event", G_CALLBACK(btnPressed), "28");    //vv dr - new AIM
  g_signal_connect(btn73A, "button-press-event", G_CALLBACK(btnPressed), "29");
  g_signal_connect(btn74A, "button-press-event", G_CALLBACK(btnPressed), "30");
  g_signal_connect(btn75A, "button-press-event", G_CALLBACK(btnPressed), "31");
  g_signal_connect(btn72A, "button-release-event", G_CALLBACK(btnReleased), "28");
  g_signal_connect(btn73A, "button-release-event", G_CALLBACK(btnReleased), "29");
  g_signal_connect(btn74A, "button-release-event", G_CALLBACK(btnReleased), "30");
  g_signal_connect(btn75A, "button-release-event", G_CALLBACK(btnReleased), "31");  //^^

  gtk_fixed_put(GTK_FIXED(grid), lbl71F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl72F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl73F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl74F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl75F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl72Fa, 0, 0);    //vv dr - new AIM
  gtk_fixed_put(GTK_FIXED(grid), lbl73Fa, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl74Fa, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl75Fa, 0, 0);    //^^
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
  gtk_fixed_put(GTK_FIXED(grid), btn72A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn73,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl73L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl73H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1); //JM
  gtk_fixed_put(GTK_FIXED(grid), btn73A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn74,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl74L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), btn74A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn75,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl75L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), btn75A, xPos,                         yPos);   //dr - new AIM



  // 8th row
  btn81   = gtk_button_new();
  btn82   = gtk_button_new();
  btn83   = gtk_button_new();
  btn84   = gtk_button_new();
  btn85   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn81), "Esc");  //JM
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn82), "0");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn83), ". ,");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn84), "\\"); //JM Changed from Ctrl to backslash 92
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn85), "+");  //JM
  btn82A  = gtk_button_new();                           //vv dr - new AIM
  btn83A  = gtk_button_new();
  btn84A  = gtk_button_new();
  btn85A  = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn82A), ":");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn83A), ".");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn84A), "?");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn85A), "Space"); //^^
  lbl81F  = gtk_label_new("");
  lbl82F  = gtk_label_new("");
  lbl83F  = gtk_label_new("");
  lbl84F  = gtk_label_new("");
  lbl85F  = gtk_label_new("");
  lbl82Fa = gtk_label_new("");  //vv dr - new AIM
  lbl83Fa = gtk_label_new("");
  lbl84Fa = gtk_label_new("");
  lbl85Fa = gtk_label_new("");  //^^
  lbl81G  = gtk_label_new("");
  lbl82G  = gtk_label_new("");
  lbl83G  = gtk_label_new("");
  lbl84G  = gtk_label_new("");
  lbl85G  = gtk_label_new("");
  lbl82H  = gtk_label_new("");//("\u03B1");        //JM ALPHA u221D
  lbl83H  = gtk_label_new("");//("\u2219");  //JM Alphadot -  Menu alphaDOT for AIM 2218
  lbl84H  = gtk_label_new("");//("\u221E");  //JM Alpha MATH - also considered pi \u03C0 and integral u222E
  lbl85H  = gtk_label_new("");//("\u00f1");  //JM Alpha Intnl - also considered \u2139
  lbl81L  = gtk_label_new("");
  lbl82L  = gtk_label_new("");
  lbl83L  = gtk_label_new("");
  lbl84L  = gtk_label_new("");
  lbl85L  = gtk_label_new("");
  lbl81Gr = gtk_label_new("");
  lbl82Gr = gtk_label_new("");
  lbl83Gr = gtk_label_new("");
  lbl84Gr = gtk_label_new("");
//lbl84H  = gtk_label_new("\u2399"); // Printer   //JM: WHY DID THIS LINE COME BACK ??
  lbl85Gr = gtk_label_new("");
//  lblOn   = gtk_label_new("ON");

  gtk_widget_set_size_request(btn81, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn82, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn83, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn84, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn85, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn82A, KEY_WIDTH_2, 0);  //vv dr - new AIM
  gtk_widget_set_size_request(btn83A, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn84A, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn85A, KEY_WIDTH_2, 0);  //^^

  gtk_widget_set_name(lbl82H, "greekUnderline");    //JM was gShiftedUnderline, changed to greekUnderline, x4
  gtk_widget_set_name(lbl83H, "greekUnderline");
  gtk_widget_set_name(lbl84H, "greekUnderline");
  gtk_widget_set_name(lbl85H, "greekUnderline");    //JM
//  gtk_widget_set_name(lblOn,  "On");

  g_signal_connect(btn81, "button-press-event", G_CALLBACK(btnPressed), "32");
  g_signal_connect(btn82, "button-press-event", G_CALLBACK(btnPressed), "33");
  g_signal_connect(btn83, "button-press-event", G_CALLBACK(btnPressed), "34");
  g_signal_connect(btn84, "button-press-event", G_CALLBACK(btnPressed), "35");
  g_signal_connect(btn85, "button-press-event", G_CALLBACK(btnPressed), "36");
  g_signal_connect(btn81, "button-release-event", G_CALLBACK(btnReleased), "32");
  g_signal_connect(btn82, "button-release-event", G_CALLBACK(btnReleased), "33");
  g_signal_connect(btn83, "button-release-event", G_CALLBACK(btnReleased), "34");
  g_signal_connect(btn84, "button-release-event", G_CALLBACK(btnReleased), "35");
  g_signal_connect(btn85, "button-release-event", G_CALLBACK(btnReleased), "36");
  g_signal_connect(btn82A, "button-press-event", G_CALLBACK(btnPressed), "33");    //vv dr - new AIM
  g_signal_connect(btn83A, "button-press-event", G_CALLBACK(btnPressed), "34");
  g_signal_connect(btn84A, "button-press-event", G_CALLBACK(btnPressed), "35");
  g_signal_connect(btn85A, "button-press-event", G_CALLBACK(btnPressed), "36");
  g_signal_connect(btn82A, "button-release-event", G_CALLBACK(btnReleased), "33");
  g_signal_connect(btn83A, "button-release-event", G_CALLBACK(btnReleased), "34");
  g_signal_connect(btn84A, "button-release-event", G_CALLBACK(btnReleased), "35");
  g_signal_connect(btn85A, "button-release-event", G_CALLBACK(btnReleased), "36");  //^^

  gtk_fixed_put(GTK_FIXED(grid), lbl81F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl82F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl83F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl84F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl85F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl82Fa, 0, 0);    //vv dr - new AIM
  gtk_fixed_put(GTK_FIXED(grid), lbl83Fa, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl84Fa, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl85Fa, 0, 0);    //^^
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
  gtk_fixed_move(GTK_FIXED(grid), lbl81G, xPos+KEY_WIDTH_1+ X_OFFSET_LETTER, yPos + 38); //JM+++ REMOVED AGAIN. OFF IS MANUALLY INSERTED SOMEHOW
//  gtk_fixed_put(GTK_FIXED(grid), lblOn,   0, 0);     //JM Removed ON to 81

  xPos += DELTA_KEYS_X + 18;
  gtk_fixed_put(GTK_FIXED(grid), btn82,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl82L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl82H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1);                //JM
  gtk_fixed_put(GTK_FIXED(grid), btn82A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn83,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl83L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl83H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1);
  gtk_fixed_put(GTK_FIXED(grid), btn83A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn84,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl84L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl84H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1);
  gtk_fixed_put(GTK_FIXED(grid), btn84A, xPos,                         yPos);   //dr - new AIM

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn85,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl85L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl85H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1);  //JM
  gtk_fixed_put(GTK_FIXED(grid), btn85A, xPos,                         yPos);   //dr - new AIM


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

        btnBitFields           = gtk_button_new_with_label("Bitfields");
        btnFlags               = gtk_button_new_with_label("Flags");
        btnRegisters           = gtk_button_new_with_label("Registers");
        btnLocalRegisters      = gtk_button_new_with_label("Local registers");
        btnStatisticalSums     = gtk_button_new_with_label("Statistical sums");
        btnNamedVariables      = gtk_button_new_with_label("Named variables");
        btnSavedStackRegisters = gtk_button_new_with_label("Saved stack registers");
        chkHexaString          = gtk_check_button_new_with_label("Strings in hexadecimal form");

        gtk_widget_set_name(btnBitFields,           "debugButton");
        gtk_widget_set_name(btnFlags,               "debugButton");
        gtk_widget_set_name(btnRegisters,           "debugButton");
        gtk_widget_set_name(btnLocalRegisters,      "debugButton");
        gtk_widget_set_name(btnStatisticalSums,     "debugButton");
        gtk_widget_set_name(btnNamedVariables,      "debugButton");
        gtk_widget_set_name(btnSavedStackRegisters, "debugButton");
        gtk_widget_set_name(chkHexaString,          "debugCheckbox");

        g_signal_connect(btnBitFields,           "clicked", G_CALLBACK(btnBitFieldsClicked),           NULL);
        g_signal_connect(btnFlags,               "clicked", G_CALLBACK(btnFlagsClicked),               NULL);
        g_signal_connect(btnRegisters,           "clicked", G_CALLBACK(btnRegistersClicked),           NULL);
        g_signal_connect(btnLocalRegisters,      "clicked", G_CALLBACK(btnLocalRegistersClicked),      NULL);
        g_signal_connect(btnStatisticalSums,     "clicked", G_CALLBACK(btnStatisticalSumsClicked),     NULL);
        g_signal_connect(btnNamedVariables,      "clicked", G_CALLBACK(btnNamedVariablesClicked),      NULL);
        g_signal_connect(btnSavedStackRegisters, "clicked", G_CALLBACK(btnSavedStackRegistersClicked), NULL);
        g_signal_connect(chkHexaString,          "clicked", G_CALLBACK(chkHexaStringClicked),          NULL);

        gtk_fixed_put(GTK_FIXED(grid), btnBitFields,             1 + debugWidgetDx, 1 + debugWidgetDy);
        gtk_fixed_put(GTK_FIXED(grid), btnFlags,                60 + debugWidgetDx, 1 + debugWidgetDy);
        gtk_fixed_put(GTK_FIXED(grid), btnRegisters,           101 + debugWidgetDx, 1 + debugWidgetDy);
        gtk_fixed_put(GTK_FIXED(grid), btnLocalRegisters,      166 + debugWidgetDx, 1 + debugWidgetDy);
        gtk_fixed_put(GTK_FIXED(grid), btnStatisticalSums,     260 + debugWidgetDx, 1 + debugWidgetDy);
        gtk_fixed_put(GTK_FIXED(grid), btnNamedVariables,      360 + debugWidgetDx, 1 + debugWidgetDy);
        gtk_fixed_put(GTK_FIXED(grid), btnSavedStackRegisters, 465 + debugWidgetDx, 1 + debugWidgetDy);
        gtk_fixed_put(GTK_FIXED(grid), chkHexaString,          630 + debugWidgetDx, 1 + debugWidgetDy);

        gtk_widget_show(btnBitFields);
        gtk_widget_show(btnFlags);
        gtk_widget_show(btnRegisters);
        gtk_widget_show(btnLocalRegisters);
        gtk_widget_show(btnStatisticalSums);
        gtk_widget_show(btnNamedVariables);
        gtk_widget_show(btnSavedStackRegisters);
        gtk_widget_show(chkHexaString);

        debugWindow = DBG_REGISTERS;
      #endif // (DEBUG_PANEL == 1)

      gtk_widget_show_all(frmCalc);

    #else // SCREEN_800X480 == 1
      // The main window
      frmCalc = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      gtk_window_set_default_size(GTK_WINDOW(frmCalc), 800, 480);
      gtk_window_set_decorated(GTK_WINDOW(frmCalc), FALSE);
      gtk_window_set_position(GTK_WINDOW(frmCalc), GTK_WIN_POS_CENTER);

      gtk_widget_set_name(frmCalc, "mainWindow");
      gtk_window_set_resizable(GTK_WINDOW(frmCalc), FALSE);
      g_signal_connect(frmCalc, "destroy", G_CALLBACK(destroyCalc), NULL);
      g_signal_connect(frmCalc, "key_press_event", G_CALLBACK(keyPressed), NULL);

      gtk_widget_add_events(GTK_WIDGET(frmCalc), GDK_CONFIGURE);

      // Fixed grid to freely put widgets on it
      grid = gtk_fixed_new();
      gtk_container_add(GTK_CONTAINER(frmCalc), grid);

      // LCD screen 800x480
      screen = gtk_drawing_area_new();
      gtk_widget_set_size_request(screen, SCREEN_WIDTH*2, SCREEN_HEIGHT*2);
      gtk_fixed_put(GTK_FIXED(grid), screen, 0, 0);
      screenStride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, SCREEN_WIDTH)/4;
      int numBytes = screenStride * SCREEN_HEIGHT * 4;
      screenData = malloc(numBytes);
      if(screenData == NULL) {
        sprintf(errorMessage, "error allocating %d x %d = %d bytes for screenData", screenStride * 4, SCREEN_HEIGHT, numBytes);
        moreInfoOnError("In function setupUI:", errorMessage, NULL, NULL);
        exit(1);
      }

      g_signal_connect(screen, "draw", G_CALLBACK(drawScreen), NULL);

      gtk_widget_show_all(frmCalc);
    #endif //  (SCREEN_800X480 == 0)
  }
#endif // PC_BUILD



#ifndef TESTSUITE_BUILD
  void fnOff(uint16_t unsuedParamButMandatory) {
    shiftF = false;
    shiftG = false;

    #ifdef PC_BUILD
      saveCalc();
      gtk_main_quit();
    #endif // PC_BUILD

    #ifdef DMCP_BUILD
      SET_ST(STAT_PGM_END);
    #endif // DMCP_BUILD
  }



  /********************************************//**
   * \brief Sets the calc mode to normal
   *
   * \param void
   * \return void
   ***********************************************/
  void calcModeNormal(void) {
    #ifdef PC_BUILD
      char tmp[200]; sprintf(tmp,"^^^^### calcModeNormal"); jm_show_comment(tmp);
    #endif //PC_BUILD
    calcMode = CM_NORMAL;

    if(softmenuStack[0].softmenuId == 1) { // MyAlpha
      softmenuStack[0].softmenuId = 0; // MyMenu
    }

    clearSystemFlag(FLAG_ALPHA);
    hideCursor();
    cursorEnabled = false;

    #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
      calcModeNormalGui();
    #endif // PC_BUILD && (SCREEN_800X480 == 0)
  }



  /********************************************//**
   * \brief Sets the calc mode to alpha input mode
   *
   * \param[in] unusedButMandatoryParameter uint16_t
   * \return void
   ***********************************************/
  void calcModeAim(uint16_t unusedButMandatoryParameter) {
    #ifdef PC_BUILD
      char tmp[200]; sprintf(tmp,"^^^^### calcModeAim"); jm_show_comment(tmp);
    #endif //PC_BUILD

if(!tam.mode) {
    if(!SH_BASE_AHOME) {
        showSoftmenu(-MNU_MyAlpha);
    } else
    if(SH_BASE_AHOME) {
        showSoftmenu(-MNU_ALPHA);        //JM ALPHA-HOME  Change to initialize the menu stack. it was true.
    }
}
    alphaCase = AC_UPPER;
    nextChar = NC_NORMAL;
    numLock = false;

    if(!tam.mode) {
      calcMode = CM_AIM;
      liftStack();

      clearRegisterLine(AIM_REGISTER_LINE, true, true);
      xCursor = 1;
      yCursor = Y_POSITION_OF_AIM_LINE + 6;
      cursorFont = &standardFont;
      cursorEnabled = true;
    }

    if(softmenuStack[0].softmenuId == 0) { // MyMenu
      softmenuStack[0].softmenuId = 1; // MyAlpha
    }

    setSystemFlag(FLAG_ALPHA);

    #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
      calcModeAimGui();
    #endif // PC_BUILD && (SCREEN_800X480 == 0)
  }



  /********************************************//**
   * \brief Sets the calc mode to alpha selection menu if needed
   *
   * \return void
   ***********************************************/
  void enterAsmModeIfMenuIsACatalog(int16_t id) {
    switch(-id) {
      case MNU_FCNS:      catalog = CATALOG_FCNS;    break;
      case MNU_CONST:     catalog = CATALOG_CNST;    break;
      case MNU_MENUS:     catalog = CATALOG_MENU;    break;
      case MNU_SYSFL:     catalog = CATALOG_SYFL;    break;
      case MNU_ALPHAINTL: catalog = CATALOG_AINT;    break;
      case MNU_ALPHAintl: catalog = CATALOG_aint;    break;
      case MNU_PROG:      catalog = CATALOG_PROG;    break;
      case MNU_VAR:       catalog = CATALOG_VAR;     break;
      case MNU_MATRS:     catalog = CATALOG_MATRS;   break;
      case MNU_STRINGS:   catalog = CATALOG_STRINGS; break;
      case MNU_DATES:     catalog = CATALOG_DATES;   break;
      case MNU_TIMES:     catalog = CATALOG_TIMES;   break;
      case MNU_ANGLES:    catalog = CATALOG_ANGLES;  break;
      case MNU_SINTS:     catalog = CATALOG_SINTS;   break;
      case MNU_LINTS:     catalog = CATALOG_LINTS;   break;
      case MNU_REALS:     catalog = CATALOG_REALS;   break;
      case MNU_CPXS:      catalog = CATALOG_CPXS;    break;
      default:            catalog = CATALOG_NONE;
    }
    #ifdef PC_BUILD
      char tmp[200]; sprintf(tmp,"^^^^### enterAsmMode catalog=%d",catalog); jm_show_comment(tmp);
    #endif //PC_BUILD

    if(catalog) {
      if(calcMode == CM_NIM) {
        closeNim();
      }

      alphaCase = AC_UPPER;
      nextChar = NC_NORMAL;
      numLock = false;


      clearSystemFlag(FLAG_ALPHA);
      resetAlphaSelectionBuffer();

      #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
        calcModeAimGui();
      #endif // PC_BUILD && (SCREEN_800X480 == 0)
    }
  }



  /********************************************//**
   * \brief Leaves the alpha selection mode
   *
   * \return void
   ***********************************************/
  void leaveAsmMode(void) {
    catalog = CATALOG_NONE;

    #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
      if(tam.mode && !tam.alpha) {
        calcModeTamGui();
      }
      else if(calcMode == CM_AIM || (tam.mode && tam.alpha)) {
        calcModeAimGui();
      }
      else if(calcMode == CM_NORMAL || calcMode == CM_PEM) {
        calcModeNormalGui();
      }
    #endif // PC_BUILD && (SCREEN_800X480 == 0)
  }



  /********************************************//**
   * \brief Sets the calc mode to number input mode
   *
   * \param[in] unusedButMandatoryParameter uint16_t
   * \return void
   ***********************************************/
  void calcModeNim(uint16_t unusedButMandatoryParameter) {
    #ifdef PC_BUILD
      char tmp[200]; sprintf(tmp,"^^^^### calcModeNim"); jm_show_comment(tmp);
    #endif //PC_BUILD
    saveForUndo();

    calcMode = CM_NIM;
    clearSystemFlag(FLAG_ALPHA);

    liftStack();
    real34Zero(REGISTER_REAL34_DATA(REGISTER_X));

    aimBuffer[0] = 0;
    hexDigits = 0;

    clearRegisterLine(NIM_REGISTER_LINE, true, true);
    xCursor = 1;
    cursorEnabled = true;
    cursorFont = &numericFont;
  }
  void refreshModeGui(void) {  //JM Added here to force icon update in Gui
    #ifdef PC_BUILD
      if     (calcMode == CM_NORMAL || calcMode == CM_PEM) calcModeNormalGui();
      else if(calcMode == CM_AIM) calcModeAimGui();
      else if(tam.mode) calcModeTamGui();
      else if(catalog) calcModeAimGui();
    #endif
  }

#endif // !TESTSUITE_BUILD
