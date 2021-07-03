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

#include "screen.h"

#include "browsers/browsers.h"
#include "bufferize.h"
#include "charString.h"
#include "constantPointers.h"
#include "curveFitting.h"
#include "dateTime.h"
#include "debug.h"
#include "display.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "c43Extensions/graphs.h"
#include "c43Extensions/graphText.h"
#include "c43Extensions/inlineTest.h"
#include "items.h"
#include "c43Extensions/jm.h"
#include "keyboard.h"
#include "c43Extensions/keyboardTweak.h"
#include "mathematics/comparisonReals.h"
#include "matrix.h"
#include "memory.h"
#include "plotstat.h"
#include "programming/manage.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "softmenus.h"
#include "stack.h"
#include "statusBar.h"
#include "timer.h"
#include "version.h"
#include <string.h>

#include "wp43s.h"

#ifndef TESTSUITE_BUILD
  static const char *whoStr = "WP" STD_SPACE_3_PER_EM "43S" STD_SPACE_3_PER_EM "by" STD_SPACE_3_PER_EM "Pauli," STD_SPACE_3_PER_EM "Walter" STD_SPACE_3_PER_EM "&" STD_SPACE_3_PER_EM "Martin";
  static const char *versionStr = "WP" STD_SPACE_3_PER_EM "43S" STD_SPACE_3_PER_EM VERSION_STRING;

  /* Names of day of week */
  TO_QSPI static const char *nameOfWday_en[8] = {"invalid day of week",                                   "Monday",            "Tuesday",                     "Wednesday",               "Thursday",           "Friday",             "Saturday",             "Sunday"};
  /*
  TO_QSPI static const char *nameOfWday_de[8] = {"ung" STD_u_DIARESIS "ltiger Wochentag",                 "Montag",            "Dienstag",                    "Mittwoch",                "Donnerstag",         "Freitag",            "Samstag",              "Sonntag"};
  TO_QSPI static const char *nameOfWday_fr[8] = {"jour de la semaine invalide",                           "lundi",             "mardi",                       "mercredi",                "jeudi",              "vendredi",           "samedi",               "dimanche"};
  TO_QSPI static const char *nameOfWday_es[8] = {"d" STD_i_ACUTE "a inv" STD_a_ACUTE "lido de la semana", "lunes",             "martes",                      "mi" STD_e_ACUTE "rcoles", "jueves",             "viernes",            "s" STD_a_ACUTE "bado", "domingo"};
  TO_QSPI static const char *nameOfWday_it[8] = {"giorno della settimana non valido",                     "luned" STD_i_GRAVE, "marted" STD_i_GRAVE,          "mercoled" STD_i_GRAVE,    "gioved" STD_i_GRAVE, "venerd" STD_i_GRAVE, "sabato",               "domenica"};
  TO_QSPI static const char *nameOfWday_pt[8] = {"dia inv" STD_a_ACUTE "lido da semana",                  "segunda-feira",     "ter" STD_c_CEDILLA "a-feira", "quarta-feira",            "quinta-feira",       "sexta-feira",        "s" STD_a_ACUTE "bado", "domingo"};
  */
#endif // TESTSUITE_BUILD


#ifdef PC_BUILD
  gboolean drawScreen(GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_surface_t *imageSurface;

    imageSurface = cairo_image_surface_create_for_data((unsigned char *)screenData, CAIRO_FORMAT_RGB24, SCREEN_WIDTH, SCREEN_HEIGHT, screenStride * 4);
    #if defined(RASPBERRY) && (SCREEN_800X480 == 1)
      cairo_scale(cr, 2.0, 2.0);
    #endif // defined(RASPBERRY) && (SCREEN_800X480 == 1)
    cairo_set_source_surface(cr, imageSurface, 0, 0);
    cairo_surface_mark_dirty(imageSurface);
    #if defined(RASPBERRY) && (SCREEN_800X480 == 1)
      cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_FAST);
    #endif // defined(RASPBERRY) && (SCREEN_800X480 == 1)
    cairo_paint(cr);
    cairo_surface_destroy(imageSurface);

    screenChange = false;

    return FALSE;
  }



  void copyScreenToClipboard(void) {
    cairo_surface_t *imageSurface;
    GtkClipboard *clipboard;

    clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_clear(clipboard);
    gtk_clipboard_set_text(clipboard, "", 0); //JM FOUND TIP TO PROPERLY CLEAR CLIPBOARD: https://stackoverflow.com/questions/2418487/clear-the-system-clipboard-using-the-gtk-lib-in-c/2419673#2419673

    imageSurface = cairo_image_surface_create_for_data((unsigned char *)screenData, CAIRO_FORMAT_RGB24, SCREEN_WIDTH, SCREEN_HEIGHT, screenStride * 4);
    gtk_clipboard_set_image(clipboard, gdk_pixbuf_get_from_surface(imageSurface, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
  }


  #endif                                                //JMCSV
  #if defined (PC_BUILD) || defined (DMCP_BUILD)        //JMCSV

  static void angularUnitToString(angularMode_t angularMode, char *string) {
    switch(angularMode) {
      case amRadian: strcpy(string, "r");        break;
      case amMultPi: strcpy(string, STD_pi);     break;
      case amGrad:   strcpy(string, "g");        break;
      case amDegree: strcpy(string, STD_DEGREE); break;
      case amDMS:    strcpy(string, "d.ms");     break;
      case amNone:   break;
      default:        strcpy(string, "?");
    }
  }



  void copyRegisterToClipboardString(calcRegister_t regist, char *clipboardString) {
    longInteger_t lgInt;
    int16_t base, sign, n;
    uint64_t shortInt;
    char tmp2[TMP_STR_LENGTH];                           //JMCSV

    switch(getRegisterDataType(regist)) {
      case dtLongInteger:
        convertLongIntegerRegisterToLongInteger(regist, lgInt);
        longIntegerToAllocatedString(lgInt, tmpString, TMP_STR_LENGTH);
        longIntegerFree(lgInt);
        tmp2[0]=0;                                         //JMCSV add apostrophies
        strcat(tmp2,"\"");                                 //JMCSV
        strcat(tmp2,tmpString);                           //JMCSV
        //strcpy(tmpString,tmp2);                         //JMCSV
        tmp2[TMP_STR_LENGTH-1]=0;                          //JMCSV trying a better way, in case the terminating 0 is not copied
        xcopy(tmpString,tmp2,min(TMP_STR_LENGTH-3,stringByteLength(tmp2)+1 ));  //JMCSV trying a better way
        strcat(tmpString,"\"");                           //JMCSV
        break;

      case dtTime:
        timeToDisplayString(regist, tmpString + TMP_STR_LENGTH/2, false);
        stringToUtf8(tmpString + TMP_STR_LENGTH/2, (uint8_t *)tmpString);
        tmp2[0]=0;                                         //JMCSV add apostrophies
        strcat(tmp2,"\"");                                 //JMCSV
        strcat(tmp2,tmpString);                           //JMCSV
        strcpy(tmpString,tmp2);                           //JMCSV
        strcat(tmpString,"\"");                           //JMCSV
        break;

      case dtDate:
        dateToDisplayString(regist, tmpString + TMP_STR_LENGTH/2);
        stringToUtf8(tmpString + TMP_STR_LENGTH/2, (uint8_t *)tmpString);
        tmp2[0]=0;                                         //JMCSV add apostrophies
        strcat(tmp2,"\"");                                 //JMCSV
        strcat(tmp2,tmpString);                           //JMCSV
        strcpy(tmpString,tmp2);                           //JMCSV
        strcat(tmpString,"\"");                           //JMCSV
        break;

      case dtString:
        xcopy(tmpString + TMP_STR_LENGTH/2, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
        stringToUtf8(tmpString + TMP_STR_LENGTH/2, (uint8_t *)tmpString);
        tmp2[0]=0;                                         //JMCSV add apostrophies
        strcat(tmp2,"\"");                                 //JMCSV
        strcat(tmp2,tmpString);                           //JMCSV
        strcpy(tmpString,tmp2);                           //JMCSV
        strcat(tmpString,"\"");                           //JMCSV
        break;

      case dtReal34Matrix:
        real34MatrixToDisplayString(regist, tmpString + TMP_STR_LENGTH/2);
        break;

      case dtComplex34Matrix:
        complex34MatrixToDisplayString(regist, tmpString + TMP_STR_LENGTH/2);
        break;

      case dtShortInteger:
        convertShortIntegerRegisterToUInt64(regist, &sign, &shortInt);
        base = getRegisterShortIntegerBase(regist);

        n = ERROR_MESSAGE_LENGTH - 100;
        sprintf(errorMessage + n--, "#%d%s (word size = %u)", base, CSV_TAB, shortIntegerWordSize);  //JMCSV added comma

        if(shortInt == 0) {
          errorMessage[n--] = '0';
        }
        else {
          while(shortInt != 0) {
            errorMessage[n--] = digits[shortInt % base];
            shortInt /= base;
          }
          if(sign) {
            errorMessage[n--] = '-';
          }
        }
        n++;

        strcpy(tmpString, errorMessage + n);
        break;

      case dtReal34:
        real34ToString(REGISTER_REAL34_DATA(regist), tmpString + TMP_STR_LENGTH/2);
        if(strchr(tmpString + TMP_STR_LENGTH/2, '.') == NULL && strchr(tmpString + TMP_STR_LENGTH/2, 'E') == NULL) {
          strcat(tmpString + TMP_STR_LENGTH/2, ".");
        }
        angularUnitToString(getRegisterAngularMode(regist), tmpString + TMP_STR_LENGTH/2 + strlen(tmpString + TMP_STR_LENGTH/2));
        stringToUtf8(tmpString + TMP_STR_LENGTH/2, (uint8_t *)tmpString);
        break;

      case dtComplex34:
        real34ToString(REGISTER_REAL34_DATA(regist), tmpString);
        if(real34IsNegative(REGISTER_IMAG34_DATA(regist))) {
          strcat(tmpString, " - ix");
          real34SetPositiveSign(REGISTER_IMAG34_DATA(regist));
          real34ToString(REGISTER_IMAG34_DATA(regist), tmpString + strlen(tmpString));
          real34SetNegativeSign(REGISTER_IMAG34_DATA(regist));
        }
        else {
          strcat(tmpString, " + ix");
          real34ToString(REGISTER_IMAG34_DATA(regist), tmpString + strlen(tmpString));
        }
        break;

      case dtConfig:
        xcopy(tmpString, "Configuration data", 19);
        break;

      default:
        sprintf(tmpString, "In function copyRegisterXToClipboard, the data type %" PRIu32 " is unknown! Please try to reproduce and submit a bug.", getRegisterDataType(regist));
    }

    strcpy(clipboardString, tmpString);
  }

  #endif                                                //JMCSV
  #ifdef PC_BUILD                                       //JMCSV


  void copyRegisterXToClipboard(void) {
    GtkClipboard *clipboard;
    char clipboardString[3000];

    clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_clear(clipboard);
    gtk_clipboard_set_text(clipboard, "", 0); //JM FOUND TIP TO PROPERLY CLEAR CLIPBOARD: https://stackoverflow.com/questions/2418487/clear-the-system-clipboard-using-the-gtk-lib-in-c/2419673#2419673

    copyRegisterToClipboardString(REGISTER_X, clipboardString);

    gtk_clipboard_set_text(clipboard, tmpString, -1);
  }



  void copyStackRegistersToClipboardString(char *clipboardString) {
    char *ptr = clipboardString;

    strcpy(ptr, "K = ");
    ptr += 4;
    copyRegisterToClipboardString(REGISTER_K, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "J = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_J, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "I = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_I, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "L = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_L, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "D = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_D, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "C = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_C, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "B = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_B, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "A = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_A, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "T = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_T, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "Z = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_Z, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "Y = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_Y, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "X = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_X, ptr);
  }



  void copyStackRegistersToClipboard(void) {
    GtkClipboard *clipboard;
    char clipboardString[10000];

    clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_clear(clipboard);
    gtk_clipboard_set_text(clipboard, "", 0); //JM FOUND TIP TO PROPERLY CLEAR CLIPBOARD: https://stackoverflow.com/questions/2418487/clear-the-system-clipboard-using-the-gtk-lib-in-c/2419673#2419673

    copyStackRegistersToClipboardString(clipboardString);

    gtk_clipboard_set_text(clipboard, clipboardString, -1);
  }



  void copyAllRegistersToClipboard(void) {
    GtkClipboard *clipboard;
    char clipboardString[15000], *ptr = clipboardString;

    clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_clear(clipboard);
    gtk_clipboard_set_text(clipboard, "", 0); //JM FOUND TIP TO PROPERLY CLEAR CLIPBOARD: https://stackoverflow.com/questions/2418487/clear-the-system-clipboard-using-the-gtk-lib-in-c/2419673#2419673

    copyStackRegistersToClipboardString(ptr);

    for(int32_t regist=99; regist>=0; --regist) {
      ptr += strlen(ptr);
      sprintf(ptr, LINEBREAK "R%02d = ", regist);
      ptr += strlen(ptr);
      copyRegisterToClipboardString(regist, ptr);
    }

    for(int32_t regist=currentNumberOfLocalRegisters-1; regist>=0; --regist) {
      ptr += strlen(ptr);
      sprintf(ptr, LINEBREAK "R.%02d = ", regist);
      ptr += strlen(ptr);
      copyRegisterToClipboardString(FIRST_LOCAL_REGISTER + regist, ptr);
    }

    if(statisticalSumsPointer != NULL) {
      char sumName[40];
      for(int32_t sum=0; sum<NUMBER_OF_STATISTICAL_SUMS; sum++) {
        ptr += strlen(ptr);

        switch(sum) {
          case  0: strcpy(sumName,           "n             "           ); break;
          case  1: strcpy(sumName, STD_SIGMA "(x)          "            ); break;
          case  2: strcpy(sumName, STD_SIGMA "(y)          "            ); break;
          case  3: strcpy(sumName, STD_SIGMA "(x" STD_SUP_2 ")         "); break;
          case  4: strcpy(sumName, STD_SIGMA "(x" STD_SUP_2 "y)        "); break;
          case  5: strcpy(sumName, STD_SIGMA "(y" STD_SUP_2 ")         "); break;
          case  6: strcpy(sumName, STD_SIGMA "(xy)         "            ); break;
          case  7: strcpy(sumName, STD_SIGMA "(ln(x)" STD_CROSS "ln(y))"); break;
          case  8: strcpy(sumName, STD_SIGMA "(ln(x))      "            ); break;
          case  9: strcpy(sumName, STD_SIGMA "(ln" STD_SUP_2 "(x))     "); break;
          case 10: strcpy(sumName, STD_SIGMA "(y ln(x))    "            ); break;
          case 11: strcpy(sumName, STD_SIGMA "(ln(y))      "            ); break;
          case 12: strcpy(sumName, STD_SIGMA "(ln" STD_SUP_2 "(y))     "); break;
          case 13: strcpy(sumName, STD_SIGMA "(x ln(y))    "            ); break;
          case 14: strcpy(sumName, STD_SIGMA "(ln(y)/x)    "            ); break;
          case 15: strcpy(sumName, STD_SIGMA "(x" STD_SUP_2 "/y)       "); break;
          case 16: strcpy(sumName, STD_SIGMA "(1/x)        "            ); break;
          case 17: strcpy(sumName, STD_SIGMA "(1/x" STD_SUP_2 ")       "); break;
          case 18: strcpy(sumName, STD_SIGMA "(x/y)        "            ); break;
          case 19: strcpy(sumName, STD_SIGMA "(1/y)        "            ); break;
          case 20: strcpy(sumName, STD_SIGMA "(1/y" STD_SUP_2 ")       "); break;
          case 21: strcpy(sumName, STD_SIGMA "(x" STD_SUP_3 ")         "); break;
          case 22: strcpy(sumName, STD_SIGMA "(x" STD_SUP_4 ")         "); break;
          case 23: strcpy(sumName,           "x min         "           ); break;
          case 24: strcpy(sumName,           "x max         "           ); break;
          case 25: strcpy(sumName,           "y min         "           ); break;
          case 26: strcpy(sumName,           "y max         "           ); break;
          default: strcpy(sumName,           "?             "           );
        }

        sprintf(ptr, LINEBREAK "SR%02d = ", sum);
        ptr += strlen(ptr);
        stringToUtf8(sumName, (uint8_t *)ptr);
        ptr += strlen(ptr);
        strcpy(ptr, " = ");
        ptr += strlen(ptr);
        realToString(statisticalSumsPointer + REAL_SIZE * sum, tmpString);
        if(strchr(tmpString, '.') == NULL && strchr(tmpString, 'E') == NULL) {
          strcat(tmpString, ".");
        }
        strcpy(ptr, tmpString);
      }
    }

    gtk_clipboard_set_text(clipboard, clipboardString, -1);
  }



  /********************************************//**
   * \brief Refreshes calc's screen. This function is
   * called every SCREEN_REFRESH_PERIOD ms by a GTK timer.
   * * make the cursor blink if needed
   * * refresh date and time in the status bar if needed
   * * refresh the whole screen if needed
   *
   * \param[in] data gpointer Not used
   * \return gboolean         What will happen next?
   *                          * true  = timer will call this function again
   *                          * false = timer stops calling this function
   ***********************************************/
  #define cursorCycle 3                      //JM cursor vv
  int8_t cursorBlinkCounter;                 //JM cursor ^^
  gboolean refreshLcd(gpointer unusedData) { // This function is called every SCREEN_REFRESH_PERIOD ms by a GTK timer
    // Cursor blinking
    static bool_t cursorBlink=true;

    if(cursorEnabled) {
      if(++cursorBlinkCounter > cursorCycle) {         //JM cursor vv
        cursorBlinkCounter = 0;
        if(cursorBlink) {
          showGlyph(STD_CURSOR, cursorFont, xCursor, yCursor, vmNormal, true, false);
          }                                              //JM cursor ^^
        else {
          hideCursor();
        }
      cursorBlink = !cursorBlink;
      }
    }

    // Function name display
    if(showFunctionNameCounter > 0) {
      showFunctionNameCounter -= SCREEN_REFRESH_PERIOD;
      if(showFunctionNameCounter <= 0) {
        hideFunctionName();
        showFunctionName(ITM_NOP, 0);
      }
    }

    // Update date and time
    getTimeString(dateTimeString);
    if(strcmp(dateTimeString, oldTime)) {
      strcpy(oldTime, dateTimeString);
      #if (DEBUG_INSTEAD_STATUS_BAR != 1)
        showDateTime();
      #endif // (DEBUG_INSTEAD_STATUS_BAR != 1)
    }

    // If LCD has changed: update the GTK screen
    if(screenChange) {
      #if defined(LINUX) && (DEBUG_PANEL == 1)
        refreshDebugPanel();
      #endif // defined(LINUX) && (DEBUG_PANEL == 1)

      gtk_widget_queue_draw(screen);
      while(gtk_events_pending()) {
        gtk_main_iteration();
      }
    }

    // Alpha selection timer
    if(AlphaSelectionBufferTimerRunning) {         //JMvv
      if(catalog/* && alphaSelectionTimer != 0 && (getUptimeMs() - alphaSelectionTimer) > 3000*/) { // More than 3 seconds elapsed since last keypress
        timeoutAlphaSelectionBuffer();             //JM^^
      }
    }

    return TRUE;
  }
#elif defined DMCP_BUILD
  #define cursorCycle 3                      //JM cursor vv
  int8_t cursorBlinkCounter;                 //JM cursor ^^
  void refreshLcd(void) {// This function is called roughly every SCREEN_REFRESH_PERIOD ms from the main loop
    // Cursor blinking
    static bool_t cursorBlink=true;

    if(cursorEnabled) {
      if(++cursorBlinkCounter > cursorCycle) {         //JM cursor vv
      cursorBlinkCounter = 0;
      if(cursorBlink) {
        showGlyph(STD_CURSOR, cursorFont, xCursor, yCursor, vmNormal, true, false);
      }                                              //JM cursor ^^
      else {
        hideCursor();
      }
      cursorBlink = !cursorBlink;
    }
  }

    // Function name display
    if(showFunctionNameCounter>0) {
      showFunctionNameCounter -= FAST_SCREEN_REFRESH_PERIOD;
      if(showFunctionNameCounter <= 0) {
        hideFunctionName();
        showFunctionName(ITM_NOP, 0);
      }
    }

    // Update date and time
    getTimeString(dateTimeString);
    if(strcmp(dateTimeString, oldTime)) {
      strcpy(oldTime, dateTimeString);
      #if (DEBUG_INSTEAD_STATUS_BAR != 1)
        showDateTime();
      #endif // (DEBUG_INSTEAD_STATUS_BAR != 1)

      if(!getSystemFlag(FLAG_AUTOFF)) {
        reset_auto_off();
      }


    }

    if(usb_powered() == 1) {
      if(!getSystemFlag(FLAG_USB)) {
        setSystemFlag(FLAG_USB);
        clearSystemFlag(FLAG_LOWBAT);
        showHideUsbLowBattery();
      }
    }
    else {
      if(getSystemFlag(FLAG_USB)) {
        clearSystemFlag(FLAG_USB);
      }

      if(get_vbat() < 2000) {
        if(!getSystemFlag(FLAG_LOWBAT)) {
          setSystemFlag(FLAG_LOWBAT);
          showHideUsbLowBattery();
        }
        SET_ST(STAT_PGM_END);
      }
      else if(get_vbat() < 2500) {
        if(!getSystemFlag(FLAG_LOWBAT)) {
          setSystemFlag(FLAG_LOWBAT);
          showHideUsbLowBattery();
        }
      }
      else {
        if(getSystemFlag(FLAG_LOWBAT)) {
          clearSystemFlag(FLAG_LOWBAT);
          showHideUsbLowBattery();
        }
      }
    }

    // Alpha selection timer
  if(AlphaSelectionBufferTimerRunning) {         //JMvv
    if(catalog)/* && alphaSelectionTimer != 0 && (getUptimeMs() - alphaSelectionTimer) > 3000)*/ { // More than 3 seconds elapsed since last keypress
      timeoutAlphaSelectionBuffer();             //JM^^
    }
  }
}
#endif // PC_BUILD DMCP_BUILD



#ifndef TESTSUITE_BUILD
void refreshFn(uint16_t timerType) {                        //vv dr - general timeout handler
  if(timerType == TO_FG_LONG) { Shft_handler(); }
  if(timerType == TO_CL_LONG) { LongpressKey_handler(); }
  if(timerType == TO_FG_TIMR) { Shft_stop(); }
  if(timerType == TO_FN_LONG) { FN_handler(); }
}                                                           //^^



void underline(int16_t y) {                     //JM
  int16_t i;
   for( i = 0; i < 6; i++ ){
     if( (calcMode != CM_GRAPH && calcMode != CM_PLOT_STAT) || ((calcMode == CM_GRAPH || calcMode == CM_PLOT_STAT) && (i <= 1)))
       underline_softkey(i, y, true);
   }
}                                               //JM


uint32_t ul;
void clear_ul(void) {
  ULGL = false;
  ULFL = false;
  ul = 0;                                       //JM Set all bits 00-23 to zero
}
                                                //JM vv LONGPRESS.   false auto clears
void underline_softkey(int16_t xSoftkey, int16_t ySoftKey, bool_t dontclear) {
  int16_t x, y, x1, y1, x2, y2;
  uint32_t tmp;

  if((calcMode == CM_GRAPH || calcMode == CM_PLOT_STAT) && xSoftkey >= 2) {
      return;
  }

  if(xSoftkey < 0 || xSoftkey > 5) return;


  if(jm_FG_LINE) {

//JMUL all changed  vv  
    if(!dontclear) {                            //JM Recursively call the same routine to clear the previous line
      for(y=0; y<ySoftKey; y++) {
        tmp = ul;
        if( ((tmp >> (y*6+xSoftkey)) & 1U)) {   //JM To check a bit, shift the number n to the right, then bitwise AND it:
          underline_softkey(xSoftkey,y,true);
        }
      }
    }
    ul ^= 1UL << (ySoftKey*6+xSoftkey);         //JM The XOR operator (^) can be used to toggle a bit.

//JMUL all changed  ^^

    if(0 <= xSoftkey && xSoftkey <= 5) {
      x1 = 67 * xSoftkey - 1;
      x2 = x1 + 67;
    } else {
      x1 = 0;
      x2 = 0;
    }

    if(0 <= ySoftKey && ySoftKey <= 2) {
      y1 = 217 - SOFTMENU_HEIGHT * ySoftKey;
      y2 = y1 + SOFTMENU_HEIGHT;
    } else {
      y1 = 0;
      y2 = 0;
    }

    y = y2-3-1;
    if(y>=0) {                                  //JM Make provision for out of range parameter, used to not plot the line and only for the recursive line removal
      for(x=x2-66+1; x<min(x2-1,SCREEN_WIDTH); x++) {
        if(mod(x, 2) == 0) {
            invert_Pixel  ((uint32_t) x, (uint32_t) y);
            invert_Pixel  ((uint32_t) x, (uint32_t) (y+2));
        }
        else {
            invert_Pixel  (x, y+1);
        }
      }
    }
  }
}                                            //JM ^^



void FN_handler() {                          //JM FN LONGPRESS vv Handler FN Key shift longpress handler
                                             //   Processing cycles here while the key is pressed, that is, after PRESS #1, waiting for RELEASE #2
  if((FN_state = ST_1_PRESS1) && FN_timeouts_in_progress && (FN_key_pressed != 0)) {

    if(fnTimerGetStatus(TO_FN_LONG) == TMR_COMPLETED) {
      FN_handle_timed_out_to_EXEC = false;
      if(!shiftF && !shiftG) {                            //   Current shift state
        shiftF = true;        //S_shF();                  //   New shift state
        showShiftState();
        refreshRegisterLine(REGISTER_T); //clearRegisterLine(Y_POSITION_OF_REGISTER_T_LINE - 4, REGISTER_LINE_HEIGHT); //JM FN clear the previous shift function name
        showFunctionName(nameFunction(FN_key_pressed-37,6),0);
        FN_timed_out_to_RELEASE_EXEC = true;
        underline_softkey(FN_key_pressed-38,1, false);
        fnTimerStart(TO_FN_LONG, TO_FN_LONG, JM_TO_FN_LONG);          //dr
        #ifdef FN_TIME_DEBUG1
          printf("Handler 1, KEY=%d \n",FN_key_pressed);
        #endif
      }
      else if(shiftF && !shiftG) {
        shiftG = true;                  
        shiftF = false;                 
        showShiftState();
        refreshRegisterLine(REGISTER_T); //clearRegisterLine(Y_POSITION_OF_REGISTER_T_LINE - 4, REGISTER_LINE_HEIGHT); //JM FN clear the previous shift function name
        showFunctionName(nameFunction(FN_key_pressed-37,12),0);
        FN_timed_out_to_RELEASE_EXEC = true;
        underline_softkey(FN_key_pressed-38,2, false);
        fnTimerStart(TO_FN_LONG, TO_FN_LONG, JM_TO_FN_LONG);          //dr
        #ifdef FN_TIME_DEBUG1
          printf("Handler 2, KEY=%d \n",FN_key_pressed);
        #endif
      }
      else if((!shiftF && shiftG) || (shiftF && shiftG)) {
        refreshRegisterLine(REGISTER_T); //clearRegisterLine(Y_POSITION_OF_REGISTER_T_LINE - 4, REGISTER_LINE_HEIGHT); //JM FN clear the previous shift function name
        showFunctionName(ITM_NOP, 0);
        FN_timed_out_to_NOP = true;
        underline_softkey(FN_key_pressed-38,3, false);   //  Purposely select row 3 which does not exist, just to activate the 'clear previous line'
        FN_timeouts_in_progress = false;
        fnTimerStop(TO_FN_LONG);                                      //dr
        #ifdef FN_TIME_DEBUG1
          printf("Handler 3, KEY=%d \n",FN_key_pressed);
        #endif
      }
    }
  }
}                                        //JM ^^



void Shft_handler() {                        //JM SHIFT NEW vv
  if(Shft_timeouts) {

    if(fnTimerGetStatus(TO_FG_LONG) == TMR_COMPLETED) {
      fnTimerStop(TO_3S_CTFF);
      if(!shiftF && !shiftG) {
        shiftF = true;
        fnTimerStart(TO_FG_LONG, TO_FG_LONG, JM_TO_FG_LONG);
        showShiftState();
      }
      else if(shiftF && !shiftG) {
        shiftG = true;
        shiftF = false;
        fnTimerStart(TO_FG_LONG, TO_FG_LONG, JM_TO_FG_LONG);
        showShiftState();
      }
      else if((!shiftF && shiftG) || (shiftF && shiftG)) {
        Shft_timeouts = false;
        fnTimerStop(TO_FG_LONG);
        fnTimerStop(TO_FG_TIMR);
        resetShiftState();                        //force into no shift state, i.e. to wait
        if(HOME3) {
          #ifdef PC_BUILD          
            jm_show_calc_state("screen.c: Shft_handler: HOME3");
          #endif //PC_BUILD
          if(softmenuStack[0].softmenuId == mm_MNU_HOME) {              //JM shifts    //softmenuStackPointerJM
            popSoftmenu();                                                                                                  //JM shifts
          }
          else {
            if(calcMode == CM_AIM) {                                                                                        //JM shifts
            }
            else {                                                                                                          //JM SHIFTS
              showSoftmenu(-MNU_HOME);                                                                          //JM shifts  //JM ALPHA-HOME
            }                                                                                                               //JM shifts
          }
        showSoftmenuCurrentPart();
        }
      }
    }
  }
}                                        //JM ^^



void LongpressKey_handler() {
  if(fnTimerGetStatus(TO_CL_LONG) == TMR_COMPLETED) {
    if(JM_auto_longpress_enabled != 0) {
      showFunctionName(JM_auto_longpress_enabled, 1000);            //fnClearStack(0);
      JM_auto_longpress_enabled = 0;
    } 
  }
}



void Shft_stop() {
  Shft_timeouts = false;
  fnTimerStop(TO_FG_LONG);
  fnTimerStop(TO_FG_TIMR);
  resetShiftState();                        //force into no shift state, i.e. to wait
}
  #ifndef DMCP_BUILD
    void setBlackPixel(uint32_t x, uint32_t y) {
      //if(y >= (uint32_t)(-6)) return;  //JM allowing allowing -1..-5 for top row text
    
      if(x>=SCREEN_WIDTH || y>=SCREEN_HEIGHT) {
        printf("In function setBlackPixel: x=%u or %d, y=%u or %d outside the screen!\n", x, (int32_t)(x), y, (int32_t)(y) );
        return;
      }

      *(screenData + y*screenStride + x) = ON_PIXEL;
      screenChange = true;
    }



    void setWhitePixel(uint32_t x, uint32_t y) {
      //if(y >= (uint32_t)(-6)) return;  //JM allowing allowing -1..-5 for top row text
      
      if(x>=SCREEN_WIDTH || y>=SCREEN_HEIGHT) {
        printf("In function setWhitePixel: x=%u or %d, y=%u or %d outside the screen!\n", x, (int32_t)(x), y, (int32_t)(y) );
        return;
      }

      *(screenData + y*screenStride + x) = OFF_PIXEL;
      screenChange = true;
    }


  /********************************************//**                       //JM
   * \brief Inverts a pixel on the screen (white/black).
   *
   * \param[in] x uint32_t x coordinate from 0 (left) to 399 (right)
   * \param[in] y uint32_t y coordinate from 0 (top) to 239 (bottom)
   * \return void
   ***********************************************/
void invert_Pixel(uint32_t x, uint32_t y) {           //JM
    if(x>=SCREEN_WIDTH || y>=SCREEN_HEIGHT) {
      printf("In function invert_Pixel: x=%d, y=%d outside the screen!\n", x, y);
      return;
    }

    *(screenData + y*screenStride + x) = *(screenData + y*screenStride + x) ^ 0xffffff;
    screenChange = true;
}



    int16_t clearScreenCounter = 0;                       //JM ClearScreen Test
    void lcd_fill_rect(uint32_t x, uint32_t y, uint32_t dx, uint32_t dy, int val) {
      uint32_t line, col, pixelColor, *pixel, endX = x + dx, endY = y + dy;

      //if(y >= (uint32_t)(-100)) return;  //JM allowing -100 to measure the size in pixels; allowing -1..-5 for top row text

      if(x==0 && y==0 && dx==SCREEN_WIDTH && dy==SCREEN_HEIGHT) {  //JMTOCHECK is this needed?
        printf(">>> screen.c: clearScreen: clearScreenCounter=%d\n",clearScreenCounter++);    //JMYY ClearScreen Test  #endif
        clear_ul(); //JMUL
      }
        if(endX > SCREEN_WIDTH || endY > SCREEN_HEIGHT) {
          printf("In function lcd_fill_rect: x=%u, y=%u, dx=%u, dy=%u, val=%d outside the screen!\n", x, y, dx, dy, val);
          return;
        }

      pixelColor = (val == LCD_SET_VALUE ? OFF_PIXEL : ON_PIXEL);
      for(line=y; line<endY; line++) {
        for(col=x, pixel=screenData + line*screenStride + x; col<endX; col++, pixel++) {
          *pixel = pixelColor;
        }
      }

      screenChange = true;
    }
  #endif // !DMCP_BUILD




//JM: If maxiC is set, then, if a glyph is not found in numericfont, it will be fetched and enlarged from standardfont

uint8_t  combinationFonts = combinationFontsDefault;
uint8_t  miniC = 0;                                                              //JM miniature letters
uint8_t  maxiC = 0;                                                              //JM ENLARGE letters. Use Numericfont & combinationFontsDefault=2;
bool_t   noShow = false;                                                         //JM
uint8_t  displaymode = stdNoEnlarge;

  uint32_t showGlyphCode(uint16_t charCode, const font_t *font, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols) {
  uint32_t col, row, xGlyph, endingCols;
  int32_t  glyphId;
  int8_t   byte, *data;
  const glyph_t  *glyph;
  int8_t rep_enlarge;

  bool_t enlarge = false;                                   //JM ENLARGE vv
  if(combinationFonts == stdnumEnlarge || combinationFonts == numHalf) {
    if(maxiC == 1 && font == &numericFont) {                //JM allow enlargements
      glyphId = findGlyph(font, charCode);
      if(glyphId < 0) {           //JM if there is not a large glyph, enlarge the small letter
        enlarge = true;
        font = &standardFont;
      }
    }
  } else
  if(combinationFonts == 1) {
    if(maxiC == 1 && font == &standardFont) {                //JM allow enlargements
      enlarge = true;
    }                                                       //JM ENLARGE ^^
  }

    glyphId = findGlyph(font, charCode);
    if(glyphId >= 0) {
      glyph = (font->glyphs) + glyphId;
    }
    else if(glyphId == -1) {
      generateNotFoundGlyph(-1, charCode);
      glyph = &glyphNotFound;
    }
    else if(glyphId == -2) {
      generateNotFoundGlyph(-2, charCode);
      glyph = &glyphNotFound;
    }
    else {
      glyph = NULL;
    }

    if(glyph == NULL) {
      sprintf(errorMessage, "In function showGlyphCode: %" PRIi32 " is an unexpected value returned by fingGlyph!", glyphId);
      displayBugScreen(errorMessage);
      return 0;
    }

  data = (int8_t *)glyph->data;
  uint32_t y0 = y;                                                   //JMmini 0-reference
  xGlyph      = showLeadingCols ? glyph->colsBeforeGlyph : 0;
  endingCols  = showEndingCols ? glyph->colsAfterGlyph : 0;

#define REDUCT_A 3
#define REDUCT_B 4
#define REDUCT_OFF 3

  // Clearing the space needed by the glyph
  if(enlarge && combinationFonts !=0) rep_enlarge = 2; else rep_enlarge = 1;                //JM ENLARGE
  if(!noShow) lcd_fill_rect(x, y, ((xGlyph + glyph->colsGlyph + endingCols) >> miniC), rep_enlarge*((glyph->rowsAboveGlyph + glyph->rowsGlyph + glyph->rowsBelowGlyph) >> miniC)-(rep_enlarge-1)*4, (videoMode == vmNormal ? LCD_SET_VALUE : LCD_EMPTY_VALUE));  //JMmini
  if(displaymode == numHalf) {y += (glyph->rowsAboveGlyph*REDUCT_A/REDUCT_B);} else {y += glyph->rowsAboveGlyph;}        //JM REDUCE
  //x += xGlyph; //JM

  // Drawing the glyph
  for(row=0; row<glyph->rowsGlyph; row++, y++) {
    if(displaymode == numHalf) {if((int)((REDUCT_A*row+REDUCT_OFF)) % REDUCT_B == 0) y--;}                           //JM REDUCE
    if(enlarge && combinationFonts !=0) rep_enlarge = 1; else rep_enlarge = 0;                //JM ENLARGE
    while (rep_enlarge >= 0) {                                             //JM ENLARGE ^^

        // Drawing the columns of the glyph
      int32_t bit = 7;
      for(col=0; col<glyph->colsGlyph; col++) {
        if(bit == 7) {
            //        byte = *(data++);
            byte = *(data);                                                  //JM ENLARGE
            if(rep_enlarge == 0) data++;                                     //JM ENLARGE
            if(miniC!=0) byte = (uint8_t)byte | (((uint8_t)byte) << 1);           //JMmini
        }

        if(byte & 0x80) {// MSB set
          if(videoMode == vmNormal) { // Black pixel for white background
            if(!noShow) setBlackPixel(x+((xGlyph+col) >> miniC), y0+((y-y0) >> miniC));       //JMmini
          }
          else { // White pixel for black background
            if(!noShow) setWhitePixel(x+((xGlyph+col) >> miniC), y0+((y-y0) >> miniC));       //JMmini
          }
        }

        byte <<= 1;

        if(--bit == -1) {
          bit = 7;
        }
      }
      if(rep_enlarge == 1 && row!=3 && row!=6 && row!=9 && row!=12) y++;     //JM ENLARGE vv do not advance the row counter for four rows, to match the row height of the enlarge font
      rep_enlarge--;
    }
  }
  return x + ((xGlyph + glyph->colsGlyph + endingCols) >> miniC);        //JMmini
}



  /* Returns the character code from the first glyph of a string.
   *
   * \param[in]     ch     String whose first glyph is to extract
   * \param[in,out] offset Offset which is updated, or null if zero and no update
   * \return Character code for that glyph
   */
  static uint16_t charCodeFromString(const char *ch, uint16_t *offset) {
    uint16_t charCode;
    uint16_t loffset = (offset != 0) ? *offset : 0;

    charCode = (uint8_t)ch[loffset++];
    if(charCode &0x0080) {
      charCode = (charCode << 8) | (uint8_t)ch[loffset++];
    }
    if(offset != 0) {
      *offset = loffset;
    }
    return charCode;
  }



  uint32_t showGlyph(const char *ch, const font_t *font, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols) {
    return showGlyphCode(charCodeFromString(ch, 0), font, x, y, videoMode, showLeadingCols, showEndingCols);
  }



  /* Finds the cols and rows for a glyph.
   *
   * \param[in]     ch     const char*   String whose first glyph is to find the bounds for
   * \param[in,out] offset uint16_t*     Offset for string or null if zero should be used
   * \param[in]     font   const font_t* Font to use
   * \param[out]    col    uint32_t*     Number of columns for the glyph
   * \param[out]    row    uint32_t*     Number of rows for the glyph
   */
  static void getGlyphBounds(const char *ch, uint16_t *offset, const font_t *font, uint32_t *col, uint32_t *row) {
    int32_t        glyphId;
    const glyph_t *glyph;

    glyphId = findGlyph(font, charCodeFromString(ch, offset));
    if (glyphId < 0) {
      sprintf(errorMessage, "In function getGlyphBounds: %" PRIi32 " is an unexpected value returned by findGlyph!", glyphId);
      displayBugScreen(errorMessage);
      return;
    }
    glyph = (font->glyphs) + glyphId;
    *col = glyph->colsBeforeGlyph + glyph->colsGlyph + glyph->colsAfterGlyph;
    *row = glyph->rowsAboveGlyph + glyph->rowsGlyph + glyph->rowsBelowGlyph;
  }


  uint8_t  compressString = 0;                                                              //JM compressString

  uint32_t showString(const char *string, const font_t *font, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols) {
    uint16_t ch, lg;
    bool_t   slc, sec;

    lg = stringByteLength(string);

    ch = 0;
    while(string[ch] != 0) {
      if(lg == 1 || (lg == 2 && (string[0] & 0x80))) {// The string is 1 glyph long
        slc = showLeadingCols;
        sec = showEndingCols;
      }
      else if(ch == 0) {// First glyph
        slc = showLeadingCols;
        sec = true;
      }
      else if(ch == lg-1 || (ch == lg-2 && (string[ch] & 0x80))) {// Last glyph
        slc = true;
        sec = showEndingCols;
      }
      else {// Glyph between first and last glyph
        slc = true;
        sec = true;
      }

      x = showGlyphCode(charCodeFromString(string, &ch), font, x, y, videoMode, slc, sec) - compressString;
    }
    compressString = 0;        //JM compressString
    return x;
  }



#ifdef CLEARNAME
  /********************************************//**
   * \brief Finds the cols and rows for a string if showing leading and ending columns
   *
   * \param[in]  ch   const char*   String to find the bounds of
   * \param[in]  font const font_t* Font to use
   * \param[out] col  uint32_t*     Number of columns for the string
   * \param[out] row  uint32_t*     Number of rows for the string
   */
  static void getStringBounds(const char *string, const font_t *font, uint32_t *col, uint32_t *row) {
    uint16_t ch = 0;
    uint32_t lcol, lrow;
    *col = 0;
    *row = 0;

    while(string[ch] != 0) {
      getGlyphBounds(string, &ch, font, &lcol, &lrow);
      *col += lcol;
      if(lrow > *row) {
        *row = lrow;
      }
    }
  }
#endif //CLEARNAME
  

  uint32_t showStringC43(const char *string, int mode, int comp, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols ) {
    int combinationFontsM = combinationFonts;
    if(combinationFontsDefault == 0) mode = stdNoEnlarge;
    
    compressString = comp;
    displaymode = mode;             // miniC and maxiC to be depreciated in favour of displaymode
    if(mode == stdNoEnlarge)         { miniC = 0 ; maxiC = 0; combinationFonts = combinationFontsDefault; x = showString(string, &standardFont, x, y, videoMode, showLeadingCols, showEndingCols );    } else
      if(mode == stdEnlarge)         { miniC = 0 ; maxiC = 1; combinationFonts = stdEnlarge;              x = showString(string, &standardFont, x, y, videoMode, showLeadingCols, showEndingCols );    } else
        if(mode == stdnumEnlarge)    { miniC = 0 ; maxiC = 1; combinationFonts = stdnumEnlarge;           x = showString(string, &numericFont , x, y, videoMode, showLeadingCols, showEndingCols );    } else
           if(mode == numSmall)      { miniC = 1 ; maxiC = 0; combinationFonts = combinationFontsDefault; x = showString(string, &numericFont , x, y, videoMode, showLeadingCols, showEndingCols );    } else
             if(mode == numHalf)     { miniC = 0 ; maxiC = 1; combinationFonts = numHalf;                 x = showString(string, &numericFont , x, y, videoMode, showLeadingCols, showEndingCols );    } else
               x = 0;
    
    miniC = 0; maxiC = 0; combinationFonts = combinationFontsM; compressString = 0; noShow = false; displaymode = stdNoEnlarge;
    return x;
  }



  uint32_t  stringWidthC43(const char *str, int mode, int comp, bool_t withLeadingEmptyRows, bool_t withEndingEmptyRows){
     noShow = true;
     return showStringC43(str, mode, comp, 0, 0, vmNormal, withLeadingEmptyRows, withEndingEmptyRows );
     //noShow = false; //no need to redo
  }


  void showDisp(uint16_t offset, uint8_t _h1) {
  #define line_h1 38

        uint32_t w = stringWidthC43(tmpString + offset, stdnumEnlarge, nocompress, true, true);
        if(w < SCREEN_WIDTH) {
          showStringC43(tmpString + offset, stdnumEnlarge, nocompress,  SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h1 * _h1, vmNormal, true, true);
          return;
        }

        w = stringWidthC43(tmpString + offset, stdEnlarge, nocompress, true, true);
        if(w < SCREEN_WIDTH) {
          showStringC43(tmpString + offset, stdEnlarge, nocompress,  SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h1 * _h1, vmNormal, true, true);
          return;
        }

        w = stringWidthC43(tmpString + offset, stdNoEnlarge, nocompress, true, true);
        if(w < SCREEN_WIDTH) {
          showStringC43(tmpString + offset, stdNoEnlarge, nocompress,  SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h1 * _h1, vmNormal, true, true);
          return;
        }

        w = stringWidthC43(tmpString + offset, numSmall, nocompress, true, true);
        if(w < SCREEN_WIDTH) {
          showStringC43(tmpString + offset, numSmall, nocompress,  SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h1 * _h1, vmNormal, true, true);
          return;
        }

        w = stringWidthC43(tmpString + offset, numSmall, stdcompress, true, true);
        if(w < SCREEN_WIDTH) {
          showStringC43(tmpString + offset, numSmall, stdcompress,  SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h1 * _h1, vmNormal, true, true);
          return;
        }
printf(">>>>>> %s\n",tmpString + offset);
        w = stringWidth(tmpString + offset + 2, &standardFont, true, true);
printf(">>>>>> %u\n",w);
        if(w < SCREEN_WIDTH) {
          showString(tmpString + offset + 2, &standardFont,  SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h1 * _h1, vmNormal, true, true);
          return;
        }
    }



uint8_t lines    = 2;      //lines   0
uint8_t y_offset = 3;
uint8_t x_offset = 0;      //pixels 40
uint16_t current_cursor_x = 0;
uint16_t current_cursor_y = 0;
int16_t  displayAIMbufferoffset;
//#define lines 5                                                          //JMCURSOR vv
//#define y_offset 2    //lines   0
uint32_t showStringEdC43(uint32_t lastline, int16_t offset, int16_t edcursor, const char *string, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols, bool_t noshow) {
  uint16_t ch, charCode, lg;
  int16_t  glyphId;
  bool_t   slc, sec;
  uint32_t  numPixels, orglastlines, tmpxy;
  const    glyph_t *glyph;
  uint8_t  editlines     = 5 ; 
//  uint8_t  maxbeforejump = 30; 
  uint8_t  yincr         = 21; 
  const    font_t *font;


  if(combinationFonts == 2) {
    font = &numericFont;                             //JM ENLARGE
  } else {
    font = &standardFont;                            //JM ENLARGE
  }


  lg = stringByteLength(string + offset);
  //printf("######## Printing:%s offset:%d length:%d:%s\n",string, offset, lg, string + offset);
  //printf("##>> %d x:%d y:%d \n",edcursor, x,y);
  if(combinationFonts !=0) {
    editlines     = 3 ;       //JM ENLARGE 5    number of editing lines                                        //JMCURSOR vv
//    maxbeforejump = 21;       //JM ENLARGE 30   number of bytes in string before jumpuing to multi line 
    yincr         = 35;       //JM ENLARGE 21   distasnce between editing wrapped lines
    x_offset      = 0;    //pixels 40
    if(/*lg>maxbeforejump*/ stringWidth(string + offset, &numericFont, showLeadingCols, showEndingCols) > SCREEN_WIDTH - 50 ) {  //jump from large letters to small letters
      if(/*lg > 3*maxbeforejump*/ stringWidth(string + offset, &numericFont, showLeadingCols, showEndingCols) > SCREEN_WIDTH * 3 - 70 ) combinationFonts = 0;  //Auto change to small font after some characters
      lines = editlines;
      y_offset = 1;
    } else {
      lines = 2;              //jump back to small letters
      y_offset = 3;
      last_CM = 253; //Force redraw
    }
  } else {
    editlines     = 5 ;       //JM ENLARGE 5    number of editing lines                                        //JMCURSOR vv
//    maxbeforejump = 30;       //JM ENLARGE 30   number of bytes in string before jumpuing to multi line 
    yincr         = 21;       //JM ENLARGE 21   distasnce between editing wrapped lines
    x_offset      = 0;    //pixels 40
    if(/*lg>maxbeforejump*/ stringWidth(string + offset, &standardFont, showLeadingCols, showEndingCols) > SCREEN_WIDTH - 50 ) {
      lines = editlines;
      y_offset = 2;
    } else {
      lines = 2;
      y_offset = 3;
      combinationFonts = combinationFontsDefault;  //Auto change back to large font
      last_CM = 253; //Force redraw
    }
  }
  if(lines == editlines || lg == 0) last_CM = 253; //Force redraw if 
  //printf("^^^^ combinationFonts=%d editlines=%d yincr=%d %d %d \n",combinationFonts, editlines, yincr, lines, last_CM);

  orglastlines = lastline;

  if(lastline > y_offset) {
    if(!noshow) clearScreen_old(false, true,false);
    x = x_offset; 
    y = (yincr-1) + y_offset * (yincr-1);
  }
  //****************************************
  ch = offset;
  while(string[ch] != 0) {

    //printf("%3d:%3d ",ch,(uint8_t)string[ch]);
    
    if(lg == 1 || (lg == 2 && (string[offset] & 0x80))) {// The string is 1 glyph long
      slc = showLeadingCols;
      sec = showEndingCols;
    }
    else if(ch == 0) {// First glyph
      slc = showLeadingCols;
      sec = true;
    }
    else if(ch == lg-1 || (ch == lg-2 && (string[ch] & 0x80))) {// Last glyph
      slc = true;
      sec = showEndingCols;
    }
    else {// Glyph between first and last glyph
      slc = true;
      sec = true;
    }

    //printf("^^^^ ch=%d edcursor=%d string[ch]=%d \n",ch, edcursor, string[ch]);

    if(ch == edcursor) {                 //draw cursor
       current_cursor_x = x;
       current_cursor_y = y;
       tmpxy = y-1;
       while (tmpxy < y + (yincr+1)) {
         if(!noshow) setBlackPixel(x,tmpxy);
         if(!noshow) setBlackPixel(x+1,tmpxy);
         tmpxy++;
       }
       x+=2;
    }

    charCode = (uint8_t)string[ch++];                         //get glyph code
    if(charCode & 0x80) {// MSB set?
      charCode = (charCode<<8) | (uint8_t)string[ch++];
    }
    glyph = NULL;
    glyphId = findGlyph(font, charCode);
    if(glyphId >= 0) {
      glyph = (font->glyphs) + glyphId;
    }
    else if(glyphId == -1) {                   //JMvv
      generateNotFoundGlyph(-1, charCode);
      glyph = &glyphNotFound;
    }
    else if(glyphId == -2) {
      generateNotFoundGlyph(-2, charCode);
      glyph = &glyphNotFound;
    }
    else {
      glyph = NULL;
    }                                         //JM^^


    numPixels = 0;

    numPixels += glyph->colsGlyph + glyph->colsAfterGlyph + glyph->colsBeforeGlyph;    // get width of glyph
    //printf(">>> lastline=%d string[ch]=%d x=%d numPixels=%d", lastline, string[ch], x, numPixels);
    if(string[ch]== 0) numPixels += 8;
    //printf("±±± lastline=%d string[ch]=%d x=%d numPixels=%d\n", lastline, string[ch], x, numPixels);

    if(x + numPixels > SCREEN_WIDTH-1 && lastline == orglastlines) { 
      x = x_offset; 
      y += yincr; 
      lastline--;
    } else    
    if(x + numPixels > SCREEN_WIDTH-1 && lastline > 1) { 
      x = 1; 
      y += yincr; 
      lastline--;
    } else
    if(x + numPixels > SCREEN_WIDTH-1 && lastline <= 1) {   
      xCursor = x;
      yCursor = y;
      return x;
    }

    maxiC = 1;                                                                            //JM
      if(y!=(uint32_t)(-100)) x = showGlyphCode(charCode, font, x, y, videoMode, slc, sec) - compressString;        //JM compressString
    maxiC = 0;                                                                            //JM

  }
  //printf("\n");

  xCursor = x;
  yCursor = y;
  compressString = 0;                                                                     //JM compressString
  return xCursor;
}

                                                          //JMCURSOR ^^


void refresh_gui(void) {                                        //JM vv
#ifdef PC_BUILD
  while(gtk_events_pending()) {
    gtk_main_iteration();
  }
#endif
}


void force_refresh(void) {
#ifdef PC_BUILD
  gtk_widget_queue_draw(screen);

//FULL UPDATE (UGLY)
#ifdef FULLUPDATE
   refresh_gui();
#endif

#endif
#if DMCP_BUILD
  lcd_forced_refresh ();
#endif

//printf(">>> screenc:force_refresh\n");

}                                                              //JM ^^






  void hideCursor(void) {
    if(cursorEnabled) {
      if(cursorFont == &standardFont) {
        lcd_fill_rect(xCursor, yCursor + 10,  6,  6, LCD_SET_VALUE);
      }
      else {
        lcd_fill_rect(xCursor, yCursor + 15, 13, 13, LCD_SET_VALUE);
      }
    }
  }



  void showFunctionName(int16_t item, int16_t delayInMs) {
    uint32_t fcol, frow, gcol, grow;

  char padding[20];                                          //JM
  if(item == ITM_NOP && delayInMs == 0) {                        //JMvv Handle second and third longpress
    if(longpressDelayedkey2 != 0) {                              //  If a delayed key2 is defined, qeue it
      item = longpressDelayedkey2; 
      delayInMs = FUNCTION_NOPTIME;
      longpressDelayedkey2 = 0;
    } else
    if(longpressDelayedkey3 != 0) {                              //  If a delayed key3 is defined, qeue it
      item = longpressDelayedkey3; 
      delayInMs = FUNCTION_NOPTIME;
      longpressDelayedkey3 = 0;
    }
  }                                                              //JM^^

    char *functionName;
    if(item != MNU_DYNAMIC) {
      functionName = indexOfItems[abs(item)].itemCatalogName;
    }
    else {
      functionName = dynmenuGetLabel(dynamicMenuItem);
    }

  showFunctionNameItem = item;
  if(running_program_jm) return;                             //JM
  showFunctionNameCounter = delayInMs;
  strcpy(padding,functionName);   //JM
  strcat(padding,"    ");                                    //JM
  if(stringWidth(padding, &standardFont, true, true) + /*1*/ 20 + lineTWidth > SCREEN_WIDTH) {                //JM
    clearRegisterLine(REGISTER_T, true, false);
  }

    // Draw over SHIFT f and SHIFT g in case they were present (otherwise they will be obscured by the function name)
    getGlyphBounds(STD_SUP_f, 0, &numericFont, &fcol, &frow);
    getGlyphBounds(STD_SUP_g, 0, &numericFont, &gcol, &grow);
    lcd_fill_rect(0, Y_POSITION_OF_REGISTER_T_LINE, (fcol > gcol ? fcol : gcol), (frow > grow ? frow : grow), LCD_SET_VALUE);

  showString(padding, &standardFont, /*1*/ 20, Y_POSITION_OF_REGISTER_T_LINE /*+ 6*/, vmNormal, true, true);      //JM
}



void hideFunctionName(void) {
#ifdef CLEARNAME
  uint32_t col, row;
  getStringBounds(indexOfItems[abs(showFunctionNameItem)].itemCatalogName, &standardFont, &col, &row);
  lcd_fill_rect(1, Y_POSITION_OF_REGISTER_T_LINE+6, col, row, LCD_SET_VALUE);
  showFunctionNameItem = 0;
#endif //CLEARNAME

  showFunctionNameCounter = 0;

  if(running_program_jm) return;                             //JM
  if(calcMode!=CM_AIM) refreshRegisterLine(REGISTER_T);      //JM DO NOT CHANGE BACK TO CLEARING ONLY A SHORT PIECE. CHANGED IN TWEAKED AS WELL>
}



  void clearRegisterLine(calcRegister_t regist, bool_t clearTop, bool_t clearBottom) {
    if(REGISTER_X <= regist && regist <= REGISTER_T) {
      uint32_t yStart = Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X);
      uint32_t height = 32;

      if(clearTop) {
        yStart -= 4;
        height += 4;
      }

      if(clearBottom) {
        height += 4;
        if(regist == REGISTER_X) {
          height += 3;
        }
      }

      lcd_fill_rect(0, yStart, SCREEN_WIDTH, height, LCD_SET_VALUE);
    }
  }


uint8_t   displayStack_m = 255;                                                  //JMSHOIDISP

  void refreshRegisterLine(calcRegister_t regist) {
    int32_t w;
    int16_t wLastBaseNumeric, wLastBaseStandard, prefixWidth = 0, lineWidth = 0;
    bool_t prefixPre = true;
    bool_t prefixPost = true;
    const uint8_t origDisplayStack = displayStack;

    char prefix[200], lastBase[4];

if(displayStackSHOIDISP != 0 && lastIntegerBase != 0 && getRegisterDataType(REGISTER_X) == dtShortInteger) { //JMSHOI                   
  if(displayStack != 4-displayStackSHOIDISP) {displayStack_m = displayStack;}   //JMSHOI
  fnDisplayStack(4-displayStackSHOIDISP);                                       //JMSHOI             
} else {                                                                        //JMSHOI 
  if(displayStack_m != 255) {                                                   //JMSHOI
    fnDisplayStack(displayStack_m);                                             //JMSHOI
    displayStack_m = 255;                                                       //JMSHOI
  } else {
    //fnDisplayStack(4);  //removed because it clamps DSTACK to 4
                                                                                //displayStack_m = 255;//is already 255
  }                                                                             //JMSHOI
}                                                                               //JMSHOI

  #if (DEBUG_PANEL == 1)
    refreshDebugPanel();
  #endif

    if((calcMode != CM_BUG_ON_SCREEN) && (calcMode != CM_PLOT_STAT) && (calcMode != CM_GRAPH) && (calcMode != CM_LISTXY)) {               //JM      
      if(temporaryInformation != TI_SHOW_REGISTER_BIG) {                        //JMSHOW
        clearRegisterLine(regist, true, (regist != REGISTER_Y));
      }                                                                         //JMSHOW

      #ifdef PC_BUILD
        #if (DEBUG_REGISTER_L == 1 || SHOW_MEMORY_STATUS == 1)
          char tmpStr[1000];
        #endif // (DEBUG_REGISTER_L == 1 || SHOW_MEMORY_STATUS == 1)
        #if (DEBUG_REGISTER_L == 1)
          char string1[1000], string2[1000], *p;
          uint16_t i;

          strcpy(string1, "L = ");

          if(getRegisterDataType(REGISTER_L) == dtReal34) {
            strcat(string1, "real34 = ");
            formatReal34Debug(string2, (real34_t *)getRegisterDataPointer(REGISTER_L));
            strcat(string2, " ");
            strcat(string2, getAngularModeName(getRegisterAngularMode(REGISTER_L)));
          }

          else if(getRegisterDataType(REGISTER_L) == dtComplex34) {
            strcat(string1, "complex34 = ");
            formatComplex34Debug(string2, (void *)getRegisterDataPointer(REGISTER_L));
          }

          else if(getRegisterDataType(REGISTER_L) == dtString) {
            strcat(string1, "string = ");
            for(i=0, p=REGISTER_STRING_DATA(REGISTER_L); i<=stringByteLength(REGISTER_STRING_DATA(REGISTER_L)); i++, p++) {
              string2[i] = *p;
            }
          }

          else if(getRegisterDataType(REGISTER_L) == dtShortInteger) {
            strcat(string1, "short integer = ");
            shortIntegerToDisplayString(REGISTER_L, string2, false);
            strcat(string2, STD_SPACE_3_PER_EM);
            strcat(string2, getShortIntegerModeName(shortIntegerMode));
          }

          else if(getRegisterDataType(REGISTER_L) == dtLongInteger) {
            strcat(string1, "long integer = ");
            longIntegerRegisterToDisplayString(REGISTER_L, string2, sizeof(string2), SCREEN_WIDTH, 50, STD_SPACE_PUNCTUATION, true);
          }

          else if(getRegisterDataType(REGISTER_L) == dtTime) {
            strcat(string1, "time = ");
            formatReal34Debug(string2, (real34_t *)getRegisterDataPointer(REGISTER_L));
          }

          else if(getRegisterDataType(REGISTER_L) == dtDate) {
            strcat(string1, "date = ");
            formatReal34Debug(string2, (real34_t *)getRegisterDataPointer(REGISTER_L));
          }

          else if(getRegisterDataType(REGISTER_L) == dtReal34Matrix) {
            sprintf(&string1[strlen(string1)], "real34 %" PRIu16 STD_CROSS "%" PRIu16 " matrix = ",
              REGISTER_REAL34_MATRIX_DBLOCK(REGISTER_L)->matrixRows, REGISTER_REAL34_MATRIX_DBLOCK(REGISTER_L)->matrixColumns);
            formatReal34Debug(string2, REGISTER_REAL34_MATRIX_M_ELEMENTS(REGISTER_L));
          }

          else if(getRegisterDataType(REGISTER_L) == dtComplex34Matrix) {
            sprintf(&string1[strlen(string1)], "complex34 %" PRIu16 STD_CROSS "%" PRIu16 " matrix = ",
              REGISTER_COMPLEX34_MATRIX_DBLOCK(REGISTER_L)->matrixRows, REGISTER_COMPLEX34_MATRIX_DBLOCK(REGISTER_L)->matrixColumns);
            formatComplex34Debug(string2, REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(REGISTER_L));
          }

          else if(getRegisterDataType(REGISTER_L) == dtConfig) {
            strcat(string1, "Configuration data");
            string2[0] = 0;
          }

          else {
            sprintf(string2, "data type %s not supported for now!", getRegisterDataTypeName(REGISTER_L, false, false));
          }

          stringToUtf8(string1, (uint8_t *)tmpStr);
          stringToUtf8(string2, (uint8_t *)tmpStr + 500);

          gtk_label_set_label(GTK_LABEL(lblRegisterL1), tmpStr);
          gtk_label_set_label(GTK_LABEL(lblRegisterL2), tmpStr + 500);
          gtk_widget_show(lblRegisterL1);
          gtk_widget_show(lblRegisterL2);
        #endif // (DEBUG_REGISTER_L == 1)
        #if (SHOW_MEMORY_STATUS == 1)
          char string[1000];

          sprintf(string, "%" PRId32 " bytes free (%" PRId32 " region%s), C43 %" PRIu64 " bytes, GMP %" PRIu64 " bytes -> should always be 0", getFreeRamMemory(), numberOfFreeMemoryRegions, numberOfFreeMemoryRegions==1 ? "" : "s", TO_BYTES((uint64_t)wp43sMemInBlocks), (uint64_t)gmpMemInBytes);
          stringToUtf8(string, (uint8_t *)tmpStr);
          gtk_label_set_label(GTK_LABEL(lblMemoryStatus), tmpStr);
          gtk_widget_show(lblMemoryStatus);
        #endif // (SHOW_MEMORY_STATUS == 1)
      #endif // PC_BUILD

      #if defined VERBOSE_SCREEN && defined PC_BUILD
        printf("^^^^Display Register: %d temporaryInformation: %d\n",regist, temporaryInformation);
      #endif //VERBOSE_SCREEN


      if(getRegisterDataType(REGISTER_X) == dtReal34Matrix || calcMode == CM_MIM) {
        real34Matrix_t matrix;
        linkToRealMatrixRegister(calcMode == CM_MIM ? matrixIndex : REGISTER_X, &matrix);
        const uint16_t rows = matrix.header.matrixRows;
        const uint16_t cols = matrix.header.matrixColumns;
        bool_t smallFont = (rows >= 5);
        int16_t dummyVal[MATRIX_MAX_COLUMNS * (MATRIX_MAX_ROWS + 1) + 1] = {};
        if(getRealMatrixColumnWidths(&matrix, &numericFont, dummyVal, dummyVal + MATRIX_MAX_COLUMNS, dummyVal + (MATRIX_MAX_ROWS + 1) * MATRIX_MAX_COLUMNS, cols > MATRIX_MAX_COLUMNS ? MATRIX_MAX_COLUMNS : cols) > MATRIX_LINE_WIDTH) smallFont = true;
        if(rows == 2 && cols > 1 && !smallFont) displayStack = 3;
        if(rows == 3 && cols > 1) displayStack = smallFont ? 3 : 2;
        if(rows == 4 && cols > 1) displayStack = smallFont ? 2 : 1;
        if(rows >= 5 && cols > 1) displayStack = 2;
        if(calcMode == CM_MIM) displayStack -= 2;
        if(displayStack > 4 /* in case of overflow */) displayStack = 0;
      }
      else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix || calcMode == CM_MIM) {
        complex34Matrix_t matrix;
        linkToComplexMatrixRegister(calcMode == CM_MIM ? matrixIndex : REGISTER_X, &matrix);
        const uint16_t rows = matrix.header.matrixRows;
        const uint16_t cols = matrix.header.matrixColumns;
        bool_t smallFont = (rows >= 5);
        int16_t dummyVal[MATRIX_MAX_COLUMNS * (MATRIX_MAX_ROWS * 2 + 3) + 1] = {};
        if(getComplexMatrixColumnWidths(&matrix, &numericFont, dummyVal, dummyVal + MATRIX_MAX_COLUMNS, dummyVal + MATRIX_MAX_COLUMNS * 2, dummyVal + MATRIX_MAX_COLUMNS * 3, dummyVal + MATRIX_MAX_COLUMNS * (MATRIX_MAX_ROWS + 3), dummyVal + MATRIX_MAX_COLUMNS * (MATRIX_MAX_ROWS * 2 + 3), cols > MATRIX_MAX_COLUMNS ? MATRIX_MAX_COLUMNS : cols) > MATRIX_LINE_WIDTH) smallFont = true;
        if(rows == 2 && cols > 1 && !smallFont) displayStack = 3;
        if(rows == 3 && cols > 1) displayStack = smallFont ? 3 : 2;
        if(rows == 4 && cols > 1) displayStack = smallFont ? 2 : 1;
        if(rows >= 5 && cols > 1) displayStack = 2;
        if(calcMode == CM_MIM) displayStack -= 2;
        if(displayStack > 4 /* in case of overflow */) displayStack = 0;
      }

      if(temporaryInformation == TI_STATISTIC_LR && (getRegisterDataType(REGISTER_X) != dtReal34)) {
        if(regist == REGISTER_X) {
          if( (uint16_t)((~lrSelection) & 0x01FF) == 511) {
            sprintf(tmpString, "L.R. selected to OrthoF.");
          } else {
            sprintf(tmpString, "L.R. selected to %03" PRIu16 ".", (uint16_t)((~lrSelection) & 0x01FF));
          }
           #if (EXTRA_INFO_ON_CALC_ERROR == 1)
             sprintf(errorMessage, "BestF is set, but will not work until REAL data points are used.");
             moreInfoOnError("In function refreshRegisterLine:", errorMessage, errorMessages[24], NULL);
           #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
           w = stringWidth(tmpString, &standardFont, true, true);
           showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
        }            
      }

      else if(temporaryInformation == TI_ARE_YOU_SURE && regist == REGISTER_X) {
        showString("Are you sure?", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
      }

      else if(temporaryInformation == TI_WHO)
        if (regist == REGISTER_X) {
          showString(whoStr, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
        } else {
        if (regist == REGISTER_Y) {
          showString(WHO2, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);          
        }
      }

      else if(temporaryInformation == TI_VERSION && regist == REGISTER_X) {
        showString(versionStr, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
      }

      else if(temporaryInformation == TI_FALSE && regist == TRUE_FALSE_REGISTER_LINE) {
        sprintf(tmpString, "false");
        showString(tmpString, &standardFont, 1, Y_POSITION_OF_TRUE_FALSE_LINE + 6, vmNormal, true, true);
      }

      else if(temporaryInformation == TI_TRUE && regist == TRUE_FALSE_REGISTER_LINE) {
        sprintf(tmpString, "true");
        showString(tmpString, &standardFont, 1, Y_POSITION_OF_TRUE_FALSE_LINE + 6, vmNormal, true, true);
      }

      else if(temporaryInformation == TI_RESET && regist == REGISTER_X) {
        sprintf(tmpString, "Data, programs, and definitions cleared");
        w = stringWidth(tmpString, &standardFont, true, true);
        showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
      }

      else if(temporaryInformation == TI_SAVED && regist == REGISTER_X) {
        showString("Saved", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
      }

      else if(temporaryInformation == TI_BACKUP_RESTORED && regist == REGISTER_X) {
        showString("Backup restored", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
      }

      else if(temporaryInformation == TI_UNDO_DISABLED && regist == REGISTER_X) {
        showString("Not enough memory for undo", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
      }

//Original SHOW

      else if(temporaryInformation == TI_SHOW_REGISTER && regist == REGISTER_T) { // L1
        w = stringWidth(tmpString, &standardFont, true, true);
        showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*0, vmNormal, true, true);
      }

      else if(temporaryInformation == TI_SHOW_REGISTER && regist == REGISTER_Z && tmpString[300] != 0) { // L2 & L3
        w = stringWidth(tmpString + 300, &standardFont, true, true);
        showString(tmpString + 300, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*1, vmNormal, true, true);

        if(tmpString[600]) {
          w = stringWidth(tmpString + 600, &standardFont, true, true);
          showString(tmpString + 600, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*2, vmNormal, true, true);
        }
      }

      else if(temporaryInformation == TI_SHOW_REGISTER && regist == REGISTER_Y && tmpString[900] != 0) { // L4 & L5
        w = stringWidth(tmpString + 900, &standardFont, true, true);
        showString(tmpString + 900, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*3, vmNormal, true, true);

        if(tmpString[1200]) {
          w = stringWidth(tmpString + 1200, &standardFont, true, true);
          showString(tmpString + 1200, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*4, vmNormal, true, true);
        }
      }

      else if(temporaryInformation == TI_SHOW_REGISTER && regist == REGISTER_X && tmpString[1500] != 0) { // L6 & L7
        w = stringWidth(tmpString + 1500, &standardFont, true, true);
        showString(tmpString + 1500, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*5, vmNormal, true, true);

        if(tmpString[1800]) {
          w = stringWidth(tmpString + 1800, &standardFont, true, true);
          showString(tmpString + 1800, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*6, vmNormal, true, true);
        }
      }



// NEW SHOW                                                                  //JMSHOW vv
        else if(temporaryInformation == TI_SHOW_REGISTER_SMALL) {
          #define line_h0 21
          switch(regist) {
            // L1
            case REGISTER_T: w = stringWidth(tmpString, &standardFont, true, true);
                             showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true);
                             #if defined VERBOSE_SCREEN && defined PC_BUILD
                               printf("^^^^NEW SHOW A: Display Register T: %s\n",tmpString);
                             #endif //VERBOSE_SCREEN
                             break;
            // L2 & L3
            case REGISTER_Z: w = stringWidth(tmpString + 300, &standardFont, true, true);
                             showString(tmpString + 300, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h0, vmNormal, true, true);
                             #if defined VERBOSE_SCREEN && defined PC_BUILD
                               printf("^^^^NEW SHOW A: Display Register Z: %s\n",tmpString + 300);
                             #endif //VERBOSE_SCREEN
                             if(tmpString[600]) {
                               w = stringWidth(tmpString + 600, &standardFont, true, true);
                               showString(tmpString + 600, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h0*2, vmNormal, true, true);
                               #if defined VERBOSE_SCREEN && defined PC_BUILD
                                 printf("^^^^NEW SHOW A: Display Register Z2: %s\n",tmpString + 600);
                               #endif //VERBOSE_SCREEN
                             }
                             break;
            // L4 & L5
            case REGISTER_Y: w = stringWidth(tmpString + 900, &standardFont, true, true);
                             showString(tmpString + 900, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h0*3, vmNormal, true, true);
                             #if defined VERBOSE_SCREEN && defined PC_BUILD
                               printf("^^^^NEW SHOW A: Display Register Y: %s\n",tmpString + 900);
                             #endif //VERBOSE_SCREEN
                             if(tmpString[1200]) {
                               w = stringWidth(tmpString + 1200, &standardFont, true, true);
                               showString(tmpString + 1200, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h0*4, vmNormal, true, true);
                               #if defined VERBOSE_SCREEN && defined PC_BUILD
                                 printf("^^^^NEW SHOW A: Display Register Y2: %s\n",tmpString + 1200);
                               #endif //VERBOSE_SCREEN
                             }
                             break;
            // L6 & L7
            case REGISTER_X: w = stringWidth(tmpString + 1500, &standardFont, true, true);
                             showString(tmpString + 1500, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h0*5, vmNormal, true, true);
                             #if defined VERBOSE_SCREEN && defined PC_BUILD
                               printf("^^^^NEW SHOW A: Display Register X: %s\n",tmpString + 1500);
                             #endif //VERBOSE_SCREEN
                             if(tmpString[1800]) {
                               w = stringWidth(tmpString + 1800, &standardFont, true, true);
                               showString(tmpString + 1800, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h0*6, vmNormal, true, true);
                               #if defined VERBOSE_SCREEN && defined PC_BUILD
                                 printf("^^^^NEW SHOW A: Display Register X2: %s\n",tmpString + 1800);
                               #endif //VERBOSE_SCREEN
                             }
                             break;
            default: {}
          }
        }

        else if(temporaryInformation == TI_SHOW_REGISTER_BIG) {            //JMSHOW ^^
          switch(regist) {
            // L1
            case REGISTER_T: showDisp(0  ,0);
                             break;
            // L2 & L3
            case REGISTER_Z: showDisp(300,1);
                             break;
            // L4 & L5
            case REGISTER_Y: showDisp(600,2);
                             break;
            // L6 & L7
            case REGISTER_X: showDisp(900,3);
                             break;
            default: {}
          }
        }                                                                 //JMSHOW ^^


      else if(regist < REGISTER_X + displayStack || (lastErrorCode != 0 && regist == errorMessageRegisterLine)) {
        prefixWidth = 0;
        const int16_t baseY = Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X + ((getRegisterDataType(REGISTER_X) == dtReal34Matrix || getRegisterDataType(REGISTER_X) == dtComplex34Matrix) ? 4 - displayStack : 0));

        if(lastErrorCode != 0 && regist == errorMessageRegisterLine) {
          if(stringWidth(errorMessages[lastErrorCode], &standardFont, true, true) <= SCREEN_WIDTH - 1) {
            showString(errorMessages[lastErrorCode], &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
          }
          else {
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              sprintf(errorMessage, "Error message %" PRIu8 " is too wide!", lastErrorCode);
              moreInfoOnError("In function refreshRegisterLine:", errorMessage, errorMessages[lastErrorCode], NULL);
            #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(tmpString, "Error message %" PRIu8 " is too wide!", lastErrorCode);
            w = stringWidth(tmpString, &standardFont, true, true);
            showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
          }
        }

        else if(regist == NIM_REGISTER_LINE && calcMode == CM_NIM) {
          if(lastIntegerBase != 0) {
            lastBase[0] = '#';
            if(lastIntegerBase > 9) {
              lastBase[1] = '1';
              lastBase[2] = '0' + (lastIntegerBase - 10);
              lastBase[3] = 0;
            }
            else {
              lastBase[1] = '0' + lastIntegerBase;
              lastBase[2] = 0;
            }
            wLastBaseNumeric  = stringWidth(lastBase, &numericFont,  true, true);
            wLastBaseStandard = stringWidth(lastBase, &standardFont, true, true);
          }
          else {
            wLastBaseNumeric  = 0;
            wLastBaseStandard = 0;
          }

          displayNim(nimBufferDisplay, lastBase, wLastBaseNumeric, wLastBaseStandard);
        }

        else if(regist == AIM_REGISTER_LINE && calcMode == CM_AIM && !tam.mode) {
/* JM Removed and replaced with JM CURSOR vv
          if(stringWidth(aimBuffer, &standardFont, true, true) < SCREEN_WIDTH - 8) { // 8 is the standard font cursor width
            xCursor = showString(aimBuffer, &standardFont, 1, Y_POSITION_OF_NIM_LINE + 6, vmNormal, true, true);
            yCursor = Y_POSITION_OF_NIM_LINE + 6;
            cursorFont = &standardFont;
          }
          else {
            w = stringByteLength(aimBuffer) + 1;
            xcopy(tmpString,        aimBuffer, w);
            xcopy(tmpString + 1500, aimBuffer, w);
            while(stringWidth(tmpString, &standardFont, true, true) >= SCREEN_WIDTH - 1) {
              w = stringLastGlyph(tmpString);
              tmpString[w] = 0;
            }

            if(stringWidth(tmpString + 1500 + w, &standardFont, true, true) >= SCREEN_WIDTH - 8) { // 8 is the standard font cursor width
              btnClicked(NULL, "16"); // back space
            }
            else {
              showString(tmpString, &standardFont, 1, Y_POSITION_OF_NIM_LINE - 3, vmNormal, true, true);

              xCursor = showString(tmpString + 1500 + w, &standardFont, 1, Y_POSITION_OF_NIM_LINE + 18, vmNormal, true, true);
              yCursor = Y_POSITION_OF_NIM_LINE + 18;
              cursorFont = &standardFont;
            }
          }
        }
*/ // JMCURSOR ^^


  //JMCURSOR vv

        int16_t tmplen = stringByteLength(aimBuffer);
        if(T_cursorPos > tmplen) {T_cursorPos = tmplen;}     //Do range checking in case the cursor starts off outside of range
        if(T_cursorPos < 0)      {T_cursorPos = tmplen;}     //Do range checking in case the cursor starts off outside of range
        int16_t cnt;
        displayAIMbufferoffset = 0;                                         //Determine offset to be able to display the latter part of the string

        int16_t sw=SCREEN_WIDTH-1-x_offset;              //Calculate the total available pixels in the number of lines available.
        if(lines==1 && y_offset<1) {sw = SCREEN_WIDTH-1 - x_offset;} 
        else if(lines==2 && y_offset<=1) {sw = (SCREEN_WIDTH-1 - x_offset)*(2-y_offset);}
        else if(lines>2) {sw = (SCREEN_WIDTH-1 - x_offset)*2 + (lines-2)*(SCREEN_WIDTH-1);}
        cnt = 0;
        do {
          //printf(">>>a %d %d >? %d\n", stringWidth(aimBuffer + displayAIMbufferoffset, font, true, true), - stringWidth(aimBuffer + T_cursorPos, font, true, true), sw);
          while(/*cnt++ < 100 && */
               ((int32_t)stringWidthC43(aimBuffer + displayAIMbufferoffset, combinationFonts ,nocompress, true, true) - (int32_t)stringWidthC43(aimBuffer + T_cursorPos, combinationFonts ,nocompress, true, true) +lines*15 > sw) &&     //assume max of 15 pixels lost at the end of each line 
          	   (displayAIMbufferoffset <= tmplen) && 
          	   (displayAIMbufferoffset + 10 < T_cursorPos)
          	   ) 
            {
              //printf(">>>b %d %d >? %d\n", stringWidth(aimBuffer + displayAIMbufferoffset, font, true, true), - stringWidth(aimBuffer + T_cursorPos, font, true, true), sw);
              displayAIMbufferoffset = stringNextGlyph(aimBuffer, displayAIMbufferoffset);
            }

          showStringEdC43(lines ,displayAIMbufferoffset, T_cursorPos, aimBuffer, 1, Y_POSITION_OF_NIM_LINE - 3, vmNormal, true, true, false);  //display up to the cursor

          if(xCursor > SCREEN_WIDTH-1) displayAIMbufferoffset = stringNextGlyph(aimBuffer, displayAIMbufferoffset);
          //printf(">>>c length:%d T_cursorPos:%d displayAIMbufferoffset:%d x:%d y:%d\n",tmplen, T_cursorPos, displayAIMbufferoffset, xCursor, yCursor);
        } while(cnt++ < 150 && xCursor > SCREEN_WIDTH-1);


        if(T_cursorPos == tmplen) cursorEnabled = true; else cursorEnabled = false; 
        if(combinationFonts == 2) {
          cursorFont = &numericFont;                             //JM ENLARGE
        } else {
          cursorFont = &standardFont;                            //JM ENLARGE
        }
      }
  //JMCURSOR  ^^


        else if(temporaryInformation == TI_NO_INFO
                && getSystemFlag(FLAG_FRACT)
                    && (    getRegisterDataType(regist) == dtReal34
                         && (
                                (   real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(regist), const34_1e_4)
                                 && real34CompareAbsLessThan(REGISTER_REAL34_DATA(regist), const34_1e6)
                                )
                             || real34IsZero(REGISTER_REAL34_DATA(regist))
                            )
                       )
               ) {
          fractionToDisplayString(regist, tmpString);

          w = stringWidth(tmpString, &numericFont, false, true);
          lineWidth = w;
          if(w <= SCREEN_WIDTH) {
            showString(tmpString, &numericFont, SCREEN_WIDTH - w, baseY, vmNormal, false, true);
          }
          else {
            w = stringWidth(tmpString, &standardFont, false, true);
            lineWidth = w;
            if(w > SCREEN_WIDTH) {
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                moreInfoOnError("In function refreshRegisterLine:", "Fraction representation too wide!", tmpString, NULL);
              #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
              strcpy(tmpString, "Fraction representation too wide!");
              w = stringWidth(tmpString, &standardFont, false, true);
              lineWidth = w;
            }
            showString(tmpString, &standardFont, SCREEN_WIDTH - w, baseY, vmNormal, false, true);
          }
        }

        else if(getRegisterDataType(regist) == dtReal34) {
          if(temporaryInformation == TI_RADIUS_THETA) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "r =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_theta " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_X_Y) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "x =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, "y =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_RE_IM) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "Im =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, "Re =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_SUMX_SUMY) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_SIGMA "x =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_SIGMA "y =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_XMIN_YMIN) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "x" STD_SUB_m STD_SUB_i STD_SUB_n " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, "y" STD_SUB_m STD_SUB_i STD_SUB_n " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_XMAX_YMAX) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "x" STD_SUB_m STD_SUB_a STD_SUB_x " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, "y" STD_SUB_m STD_SUB_a STD_SUB_x " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_SA) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "s(a" STD_SUB_0 ") =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, "s(a" STD_SUB_1 ") =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_MEANX_MEANY) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_x_BAR " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_y_BAR " =");
               prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
           }

          else if(temporaryInformation == TI_SAMPLSTDDEV) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "s" STD_SUB_x " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, "s" STD_SUB_y " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_POPLSTDDEV) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_sigma STD_SUB_x " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_sigma STD_SUB_y " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_STDERR) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "s" STD_SUB_m STD_SUB_x " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, "s" STD_SUB_m STD_SUB_y " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_GEOMMEANX_GEOMMEANY) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_x_BAR STD_SUB_G " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_y_BAR STD_SUB_G " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_GEOMSAMPLSTDDEV) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_epsilon STD_SUB_x " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_epsilon STD_SUB_y " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_GEOMPOPLSTDDEV) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_epsilon STD_SUB_m STD_SUB_x " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_epsilon STD_SUB_m STD_SUB_y " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_GEOMSTDERR) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_epsilon STD_SUB_p STD_SUB_x " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_epsilon STD_SUB_p STD_SUB_y " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_WEIGHTEDMEANX) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_x_BAR STD_SUB_w " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_WEIGHTEDSAMPLSTDDEV) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "s" STD_SUB_w " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_WEIGHTEDPOPLSTDDEV) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_sigma STD_SUB_w " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_WEIGHTEDSTDERR) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "s" STD_SUB_m STD_SUB_w " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          //L.R. Display
          else if(temporaryInformation == TI_LR && lrChosen != 0) {
            #define LRWidth 140
            bool_t prefixPre = false;
            bool_t prefixPost = false;

            if(lrChosen == CF_CAUCHY_FITTING || lrChosen == CF_GAUSS_FITTING || lrChosen == CF_PARABOLIC_FITTING){
              if(regist == REGISTER_X) {
                strcpy(prefix,getCurveFitModeFormula(lrChosen));
                while(stringWidth(prefix, &standardFont, prefixPre, prefixPost) + 1 < LRWidth) {
                  strcat(prefix,STD_SPACE_6_PER_EM);
                }
                strcat(prefix,"a" STD_SUB_0 " =");
                prefixWidth = stringWidth(prefix, &standardFont, prefixPre, prefixPost) + 1;
              } else
              if(regist == REGISTER_Y) {
                strcpy(prefix,"y = ");
                while(stringWidth(prefix, &standardFont, prefixPre, prefixPost) + 1 < LRWidth) {
                  strcat(prefix,STD_SPACE_6_PER_EM);
                }
                strcat(prefix, "a" STD_SUB_1 " =");
                prefixWidth = stringWidth(prefix, &standardFont, prefixPre, prefixPost) + 1;
              } else
              if(regist == REGISTER_Z) {
                strcpy(prefix, eatSpacesEnd(getCurveFitModeName(lrChosen)));
                if(lrCountOnes(lrSelection)>1) strcat(prefix,lrChosen == 0 ? "" : STD_SUP_ASTERISK);
                while(stringWidth(prefix, &standardFont, prefixPre, prefixPost) + 1 < LRWidth) {
                  strcat(prefix,STD_SPACE_6_PER_EM);
                }
                strcat(prefix, "a" STD_SUB_2 " =");
                prefixWidth = stringWidth(prefix, &standardFont, prefixPre, prefixPost) + 1;
              }
            } else

            if(regist == REGISTER_X) {
              strcpy(prefix,"y = ");
              strcat(prefix,getCurveFitModeFormula(lrChosen));
              while(stringWidth(prefix, &standardFont, prefixPre, prefixPost) + 1 < LRWidth) {
                strcat(prefix,STD_SPACE_6_PER_EM);
              }
              strcat(prefix,"a" STD_SUB_0 " =");
              prefixWidth = stringWidth(prefix, &standardFont, prefixPre, prefixPost) + 1;
            } else if(regist == REGISTER_Y) {
              strcpy(prefix, eatSpacesEnd(getCurveFitModeName(lrChosen)));
              if(lrCountOnes(lrSelection)>1) strcat(prefix,lrChosen == 0 ? "" : STD_SUP_ASTERISK);
              while(stringWidth(prefix, &standardFont, prefixPre, prefixPost) + 1 < LRWidth) {
                strcat(prefix,STD_SPACE_6_PER_EM);
              }
              strcat(prefix, "a" STD_SUB_1 " =");
              prefixWidth = stringWidth(prefix, &standardFont, prefixPre, prefixPost) + 1;
            }

          }

/*
          else if(temporaryInformation == TI_SXY) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "s" STD_SUB_x STD_SUB_y " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_COV) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "s" STD_SUB_m STD_SUB_w " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }
*/
          else if(temporaryInformation == TI_CALCY) {
            if(regist == REGISTER_X) {
              prefix[0]=0;
              if(lrChosen == 0) {
              } else {
                strcpy(prefix,eatSpacesEnd(getCurveFitModeName(lrChosen)));
                if(lrCountOnes(lrSelection)>1) strcat(prefix,STD_SUP_ASTERISK);
                strcat(prefix, STD_SPACE_FIGURE);
              }
              strcat(prefix, STD_y_CIRC " =");
              prefixWidth = stringWidth(prefix, &standardFont, false,false) + 1;
            }
          }

          else if(temporaryInformation == TI_CALCX) {
            if(regist == REGISTER_X) {
              prefix[0]=0;
              if(lrChosen == 0) {
              } else {
                strcpy(prefix,eatSpacesEnd(getCurveFitModeName(lrChosen)));
                if(lrCountOnes(lrSelection)>1) strcat(prefix,STD_SUP_ASTERISK);
                strcat(prefix, STD_SPACE_FIGURE);
              }
              strcat(prefix, STD_x_CIRC " =");
              prefixWidth = stringWidth(prefix, &standardFont, false,false) + 1;
            }
          }

          else if(temporaryInformation == TI_CALCX2) {
            if(regist == REGISTER_X) {
              prefix[0]=0;
              if(lrChosen == 0) {
              } else {
                strcpy(prefix,eatSpacesEnd(getCurveFitModeName(lrChosen)));
                if(lrCountOnes(lrSelection)>1) strcat(prefix,STD_SUP_ASTERISK);
                strcat(prefix, STD_SPACE_FIGURE);
              }
              strcat(prefix, STD_x_CIRC STD_SUB_1 " =" );
              prefixWidth = stringWidth(prefix, &standardFont, false,false) + 1;
            } else
            if(regist == REGISTER_Y) {
              prefix[0]=0;
              if(lrChosen == 0) {
              } else {
                strcpy(prefix,eatSpacesEnd(getCurveFitModeName(lrChosen)));
                if(lrCountOnes(lrSelection)>1) strcat(prefix,STD_SUP_ASTERISK);
                strcat(prefix, STD_SPACE_FIGURE);
              }
              strcat(prefix, STD_x_CIRC STD_SUB_2 " =");
              prefixWidth = stringWidth(prefix, &standardFont, false,false) + 1;
            }
          }

          else if(temporaryInformation == TI_CORR) {             
            if(regist == REGISTER_X) {
              prefix[0]=0;
              if(lrChosen == 0) {
              } else {
                strcpy(prefix,eatSpacesEnd(getCurveFitModeName(lrChosen)));
                if(lrCountOnes(lrSelection)>1) strcat(prefix,STD_SUP_ASTERISK);
                strcat(prefix, STD_SPACE_FIGURE);
              }
              strcat(prefix, "r =");
              prefixWidth = stringWidth(prefix, &standardFont, false,false) + 1;
            }
          }

          else if(temporaryInformation == TI_SMI) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "s" STD_SUB_m STD_SUB_i " =");
              prefixWidth = stringWidth(prefix, &standardFont, false,false) + 1;
            }
          }
          //L.R. Display



          else if(temporaryInformation == TI_HARMMEANX_HARMMEANY) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_x_BAR STD_SUB_H " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_y_BAR STD_SUB_H " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_RMSMEANX_RMSMEANY) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_y_BAR STD_SUB_R STD_SUB_M STD_SUB_S " =");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_STATISTIC_SUMS) {
            realToInt32(SIGMA_N, w);
            if(regist == REGISTER_X && w > LIM) {
              sprintf(prefix, "Plot memory full, continuing");                
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            if(regist == REGISTER_Y) {
              sprintf(prefix, "Data point %03" PRId32, w);                
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              lcd_fill_rect(0, Y_POSITION_OF_REGISTER_Y_LINE - 2, SCREEN_WIDTH, 1, LCD_EMPTY_VALUE);
            }
          }

          else if(temporaryInformation == TI_STATISTIC_LR) {
             if(regist == REGISTER_X) {
               if( (uint16_t)((~lrSelection) & 0x01FF) == 511) {
                 sprintf(prefix, "L.R. selected to OrthoF");
               } else {
                 sprintf(prefix, "L.R. selected to %03" PRIu16, (uint16_t)((~lrSelection) & 0x01FF));
               }
               prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
               //lcd_fill_rect(0, Y_POSITION_OF_REGISTER_Y_LINE - 2, SCREEN_WIDTH, 1, LCD_EMPTY_VALUE);
             }            
           }


            else if(temporaryInformation == TI_ABC) {                             //JM EE \/
              if(regist == REGISTER_X) {
                strcpy(prefix, "c" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "b" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Z) {
                strcpy(prefix, "a" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            else if(temporaryInformation == TI_ABBCCA) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "ca" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "bc" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Z) {
                strcpy(prefix, "ab" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            else if(temporaryInformation == TI_012) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "sym2" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "sym1" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Z) {
                strcpy(prefix, "sym0" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            else if(temporaryInformation == TI_FROM_DMS) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "decimal" STD_DEGREE STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            if(prefixWidth > 0) {
              if(regist == REGISTER_X) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
              } else
              if(regist == REGISTER_Y) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_Y_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_Y) + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
              } else
              if(regist == REGISTER_Z) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_Z_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_Z) + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
              }
            }
                                                                       //JM EE ^


          real34ToDisplayString(REGISTER_REAL34_DATA(regist), getRegisterAngularMode(regist), tmpString, &numericFont, SCREEN_WIDTH - prefixWidth, NUMBER_OF_DISPLAY_DIGITS, true, STD_SPACE_PUNCTUATION, false);

          w = stringWidth(tmpString, &numericFont, false, true);
          lineWidth = w;
          if(prefixWidth > 0) {
            showString(prefix, &standardFont, 1, baseY + TEMPORARY_INFO_OFFSET, vmNormal, prefixPre, prefixPost);
          }
          showString(tmpString, &numericFont, SCREEN_WIDTH - w, baseY, vmNormal, false, true);
        }

          //JM else if(getRegisterDataType(regist) == dtComplex34) {                                                                                                      //JM EE Removed and replaced with the below
          //JM complex34ToDisplayString(REGISTER_COMPLEX34_DATA(regist), tmpString, &numericFont, SCREEN_WIDTH, NUMBER_OF_DISPLAY_DIGITS, true, STD_SPACE_PUNCTUATION);   //JM EE Removed and replaced with the below

          else if(getRegisterDataType(regist) == dtComplex34) {
            if(temporaryInformation == TI_ABC) {                             //JM EE \/
              if(regist == REGISTER_X) {
                strcpy(prefix, "c" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "b" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Z) {
                strcpy(prefix, "a" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            else if(temporaryInformation == TI_ABBCCA) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "ca" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "bc" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Z) {
                strcpy(prefix, "ab" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            else if(temporaryInformation == TI_012) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "sym2" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "sym1" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Z) {
                strcpy(prefix, "sym0" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            if(prefixWidth > 0) {
              if(regist == REGISTER_X) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
              } else
              if(regist == REGISTER_Y) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_Y_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_Y) + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
              } else
              if(regist == REGISTER_Z) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_Z_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_Z) + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
              }
            }
                                                                       //JM EE ^



            complex34ToDisplayString(REGISTER_COMPLEX34_DATA(regist), tmpString, &numericFont, SCREEN_WIDTH - prefixWidth, NUMBER_OF_DISPLAY_DIGITS,true, STD_SPACE_PUNCTUATION, false);

          w = stringWidth(tmpString, &numericFont, false, true);
          lineWidth = w;
          showString(tmpString, &numericFont, SCREEN_WIDTH - w, baseY, vmNormal, false, true);
        }

        else if(getRegisterDataType(regist) == dtString) {
                                                                                  //JMvv

       //JM REGISTER STRING LARGE FONTS
        #ifdef STACK_X_STR_LRG_FONT
          //This is for X
          w = stringWidthC43(REGISTER_STRING_DATA(regist), stdnumEnlarge, nocompress, false, true);
          if(regist == REGISTER_X && w<SCREEN_WIDTH) {
            lineWidth = w; //slighly incorrect if special characters are there as well.
            showStringC43(REGISTER_STRING_DATA(regist), stdnumEnlarge, nocompress, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, false, true);
          } else                                                                   //JM
        #endif

          #ifdef STACK_X_STR_MED_FONT
            //This is for X
            if(regist == REGISTER_X && stringWidthC43(REGISTER_STRING_DATA(regist), numHalf, nocompress, false, true) < SCREEN_WIDTH) {
              w = stringWidthC43(REGISTER_STRING_DATA(regist), numHalf,    nocompress, false, true);
              lineWidth = w;
              showStringC43(REGISTER_STRING_DATA(regist), numHalf, nocompress, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, false, true);
            } else                                                                   //JM
          #endif

            #ifdef STACK_STR_MED_FONT
              //This is for Y, Z & T
              if(regist >= REGISTER_Y && regist <= REGISTER_T && stringWidthC43(REGISTER_STRING_DATA(regist), numHalf, nocompress, false, true) < SCREEN_WIDTH) {
                w = stringWidthC43(REGISTER_STRING_DATA(regist), numHalf, nocompress, false, true);
                lineWidth = w;
                showStringC43(REGISTER_STRING_DATA(regist), numHalf, nocompress, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, false, true);
              } else                                                                   //JM
            #endif
          //JM ^^ large fonts


        { //printf("^^^^#### combinationFonts=%d maxiC=%d miniC=%d displaymode=%d\n",combinationFonts, maxiC, miniC, displaymode);
          w = stringWidth(REGISTER_STRING_DATA(regist), &standardFont, false, true);

          if(w >= SCREEN_WIDTH) {
            if(regist == REGISTER_X) {
              xcopy(tmpString, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
              do {
                tmpString[stringLastGlyph(tmpString)] = 0;
                w = stringWidth(tmpString, &standardFont, false, true);
              } while(w >= SCREEN_WIDTH);
              showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - 3, vmNormal, false, true);

              w = stringByteLength(tmpString);
              xcopy(tmpString, REGISTER_STRING_DATA(regist) + w, stringByteLength(REGISTER_STRING_DATA(regist) + w) + 1);
              w = stringWidth(tmpString, &standardFont, false, true);
              if(w >= SCREEN_WIDTH) {
                do {
                  tmpString[stringLastGlyph(tmpString)] = 0;
                  w = stringWidth(tmpString, &standardFont, false, true);
                } while(w >= SCREEN_WIDTH - 14); // 14 is the width of STD_ELLIPSIS
                xcopy(tmpString + stringByteLength(tmpString), STD_ELLIPSIS, 3);
                w += 14;
              }
              showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE + 18, vmNormal, false, true);
            }
            else {
              xcopy(tmpString, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
              do {
                tmpString[stringLastGlyph(tmpString)] = 0;
                w = stringWidth(tmpString, &standardFont, false, true);
              } while(w >= SCREEN_WIDTH - 14); // 14 is the width of STD_ELLIPSIS
              xcopy(tmpString + stringByteLength(tmpString), STD_ELLIPSIS, 3);
              w += 14;
              lineWidth = w;
              showString(tmpString, &standardFont, SCREEN_WIDTH - w, baseY + 6, vmNormal, false, true);
            }
          }
          else {
            lineWidth = w;
            showString(REGISTER_STRING_DATA(regist), &standardFont, SCREEN_WIDTH - w, baseY + 6, vmNormal, false, true);
          }
        }
      }

      else if(getRegisterDataType(regist) == dtShortInteger) {
        shortIntegerToDisplayString(regist, tmpString, true);
        showString(tmpString, fontForShortInteger, SCREEN_WIDTH - stringWidth(tmpString, fontForShortInteger, false, true), Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
        //JM SHOIDISP // use the top part of the screen for HEX and BIN    //JM vv SHOIDISP
        if(displayStack == 4-displayStackSHOIDISP && lastIntegerBase != 0) {
          if(displayStack == 1){
            copySourceRegisterToDestRegister(REGISTER_Y,TEMP_REGISTER_1);
            copySourceRegisterToDestRegister(REGISTER_X,REGISTER_Y);
            setRegisterTag(REGISTER_Y, 2);
            shortIntegerToDisplayString(REGISTER_Y, tmpString, true);
            if(lastErrorCode == 0 && stringWidth(tmpString, fontForShortInteger, false, true) + stringWidth("  X: ", &standardFont, false, true) <= SCREEN_WIDTH) {
              showString("  X: ", &standardFont, 0, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
            }
            showString(tmpString, fontForShortInteger, SCREEN_WIDTH - stringWidth(tmpString, fontForShortInteger, false, true), Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
            copySourceRegisterToDestRegister(TEMP_REGISTER_1,REGISTER_Y);
          }
          if(displayStack == 1 || displayStack == 2){
            copySourceRegisterToDestRegister(REGISTER_Z,TEMP_REGISTER_1);
            copySourceRegisterToDestRegister(REGISTER_X,REGISTER_Z);
            if(displayStack == 2) setRegisterTag(REGISTER_Z, 2); else 
              if(displayStack == 1) setRegisterTag(REGISTER_Z, 8);
            shortIntegerToDisplayString(REGISTER_Z, tmpString, true);
            if(lastErrorCode == 0 && stringWidth(tmpString, fontForShortInteger, false, true) + stringWidth("  X: ", &standardFont, false, true) <= SCREEN_WIDTH) {
              showString("  X: ", &standardFont, 0, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
            }
            showString(tmpString, fontForShortInteger, SCREEN_WIDTH - stringWidth(tmpString, fontForShortInteger, false, true), Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
            copySourceRegisterToDestRegister(TEMP_REGISTER_1,REGISTER_Z);
          }
          if(displayStack == 1 || displayStack == 2 || displayStack ==3){
            copySourceRegisterToDestRegister(REGISTER_T,TEMP_REGISTER_1);
            copySourceRegisterToDestRegister(REGISTER_X,REGISTER_T);
            setRegisterTag(REGISTER_T, 16);
            shortIntegerToDisplayString(REGISTER_T, tmpString, true);
            if(lastErrorCode == 0 && stringWidth(tmpString, fontForShortInteger, false, true) + stringWidth("  X: ", &standardFont, false, true) <= SCREEN_WIDTH) {
              showString("  X: ", &standardFont, 0, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_T - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
            }
            showString(tmpString, fontForShortInteger, SCREEN_WIDTH - stringWidth(tmpString, fontForShortInteger, false, true), Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_T - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
            copySourceRegisterToDestRegister(TEMP_REGISTER_1,REGISTER_T);
          }
            if(displayStack == 3)
              lcd_fill_rect(0, Y_POSITION_OF_REGISTER_Z_LINE - 2, SCREEN_WIDTH, 1, 0xFF); else
            if(displayStack == 2)
              lcd_fill_rect(0, Y_POSITION_OF_REGISTER_Y_LINE - 2, SCREEN_WIDTH, 1, 0xFF); else
            if(displayStack == 1)
              lcd_fill_rect(0, Y_POSITION_OF_REGISTER_X_LINE - 2, SCREEN_WIDTH, 1, 0xFF);            
        }                                                                 //JM ^^

              }

      else if(getRegisterDataType(regist) == dtLongInteger) {

             if(temporaryInformation == TI_ms) {                             //JMms vv
              if(regist == REGISTER_X) {
                strcpy(prefix, "t (ms)" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            if(prefixWidth > 0) {
              if(regist == REGISTER_X) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE + TEMPORARY_INFO_OFFSET - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
              }
            }                                                               //JMms ^^

          longIntegerRegisterToDisplayString(regist, tmpString, TMP_STR_LENGTH, SCREEN_WIDTH - prefixWidth, 50, STD_SPACE_PUNCTUATION, true);          //JMms added prefix   //JM added last parameter: Allow LARGELI

          if(temporaryInformation == TI_DAY_OF_WEEK) {
            if(regist == REGISTER_X) {
              if(strcmp(tmpString, "1") == 0) strcpy(prefix, nameOfWday_en[1]);
              else if(strcmp(tmpString, "2") == 0) strcpy(prefix, nameOfWday_en[2]);
              else if(strcmp(tmpString, "3") == 0) strcpy(prefix, nameOfWday_en[3]);
              else if(strcmp(tmpString, "4") == 0) strcpy(prefix, nameOfWday_en[4]);
              else if(strcmp(tmpString, "5") == 0) strcpy(prefix, nameOfWday_en[5]);
              else if(strcmp(tmpString, "6") == 0) strcpy(prefix, nameOfWday_en[6]);
              else if(strcmp(tmpString, "7") == 0) strcpy(prefix, nameOfWday_en[7]);
              else strcpy(prefix, nameOfWday_en[0]);
              showString(prefix, &standardFont, 1, baseY + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
            }
          }

          w = stringWidth(tmpString, &numericFont, false, true);
          lineWidth = w;

          if(w <= SCREEN_WIDTH) {
            showString(tmpString, &numericFont, SCREEN_WIDTH - w, baseY, vmNormal, false, true);
          }
          else {
            w = stringWidth(tmpString, &standardFont, false, true);
            if(w > SCREEN_WIDTH) {
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                moreInfoOnError("In function refreshRegisterLine:", "Long integer representation too wide!", tmpString, NULL);
              #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
              strcpy(tmpString, "Long integer representation too wide!");
            }
            w = stringWidth(tmpString, &standardFont, false, true);
            lineWidth = w;
            showString(tmpString, &standardFont, SCREEN_WIDTH - w, baseY + 6, vmNormal, false, true);
          }
        }

        else if(getRegisterDataType(regist) == dtTime) {
          timeToDisplayString(regist, tmpString, false);
          w = stringWidth(tmpString, &numericFont, false, true);
          showString(tmpString, &numericFont, SCREEN_WIDTH - w, baseY, vmNormal, false, true);
        }

        else if(getRegisterDataType(regist) == dtDate) {
          if(temporaryInformation == TI_DAY_OF_WEEK) {
            if(regist == REGISTER_X) {
              strcpy(prefix, nameOfWday_en[getDayOfWeek(regist)]);
              showString(prefix, &standardFont, 1, baseY + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
            }
          }

          dateToDisplayString(regist, tmpString);
          w = stringWidth(tmpString, &numericFont, false, true);
          showString(tmpString, &numericFont, SCREEN_WIDTH - w, baseY, vmNormal, false, true);
        }

        else if(getRegisterDataType(regist) == dtConfig) {
          xcopy(tmpString, "Configuration data", 19);
          w = stringWidth(tmpString, &numericFont, false, true);
          lineWidth = w;
          showString(tmpString, &numericFont, SCREEN_WIDTH - w, baseY, vmNormal, false, true);
        }

        else if(getRegisterDataType(regist) == dtReal34Matrix) {
          if(regist == REGISTER_X && calcMode != CM_MIM) {
            real34Matrix_t matrix;
            linkToRealMatrixRegister(REGISTER_X, &matrix);
            showRealMatrix(&matrix);
            if(lastErrorCode != 0)
              refreshRegisterLine(errorMessageRegisterLine);
            if(temporaryInformation == TI_TRUE || temporaryInformation == TI_FALSE)
              refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
          }
          else {
            real34MatrixToDisplayString(regist, tmpString);
            w = stringWidth(tmpString, &numericFont, false, true);
            lineWidth = w;
            showString(tmpString, &numericFont, SCREEN_WIDTH - w - 2, baseY, vmNormal, false, true);
          }

          if(temporaryInformation == TI_INACCURATE && regist == REGISTER_X) {
            showString("This result may be inaccurate", &standardFont, 1, Y_POSITION_OF_ERR_LINE, vmNormal, true, true);
          }
        }

        else if(getRegisterDataType(regist) == dtComplex34Matrix) {
          if(regist == REGISTER_X && calcMode != CM_MIM) {
            complex34Matrix_t matrix;
            linkToComplexMatrixRegister(REGISTER_X, &matrix);
            showComplexMatrix(&matrix);
            if(lastErrorCode != 0)
              refreshRegisterLine(errorMessageRegisterLine);
            if(temporaryInformation == TI_TRUE || temporaryInformation == TI_FALSE)
              refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
          }
          else {
            complex34MatrixToDisplayString(regist, tmpString);
            w = stringWidth(tmpString, &numericFont, false, true);
            lineWidth = w;
            showString(tmpString, &numericFont, SCREEN_WIDTH - w - 2, baseY, vmNormal, false, true);
          }

          if(temporaryInformation == TI_INACCURATE && regist == REGISTER_X) {
            showString("This result may be inaccurate", &standardFont, 1, Y_POSITION_OF_ERR_LINE, vmNormal, true, true);
          }
        }

        else {
          sprintf(tmpString, "Displaying %s: to be coded!", getRegisterDataTypeName(regist, true, false));
          showString(tmpString, &standardFont, SCREEN_WIDTH - stringWidth(tmpString, &standardFont, false, true), baseY + 6, vmNormal, false, true);
        }
      }

      if(regist == REGISTER_T) {
        lineTWidth = lineWidth;
      }
    }

    if(getRegisterDataType(REGISTER_X) == dtReal34Matrix || getRegisterDataType(REGISTER_X) == dtComplex34Matrix || calcMode == CM_MIM) {
      displayStack = origDisplayStack;
    }
  }



  void displayNim(const char *nim, const char *lastBase, int16_t wLastBaseNumeric, int16_t wLastBaseStandard) {
    int16_t w;
    if(stringWidth(nim, &numericFont, true, true) + wLastBaseNumeric <= SCREEN_WIDTH - 16) { // 16 is the numeric font cursor width
      xCursor = showString(nim, &numericFont, 0, Y_POSITION_OF_NIM_LINE, vmNormal, true, true);
      yCursor = Y_POSITION_OF_NIM_LINE;
      cursorFont = &numericFont;

      if(lastIntegerBase != 0) {
        showString(lastBase, &numericFont, xCursor + 16, Y_POSITION_OF_NIM_LINE, vmNormal, true, true);
      }
    }
    else if(stringWidth(nim, &standardFont, true, true) + wLastBaseStandard <= SCREEN_WIDTH - 8) { // 8 is the standard font cursor width
      xCursor = showString(nim, &standardFont, 0, Y_POSITION_OF_NIM_LINE + 6, vmNormal, true, true);
      yCursor = Y_POSITION_OF_NIM_LINE + 6;
      cursorFont = &standardFont;

      if(lastIntegerBase != 0) {
        showString(lastBase, &standardFont, xCursor + 8, Y_POSITION_OF_NIM_LINE + 6, vmNormal, true, true);
      }
    }
    else {
      w = stringByteLength(nim) + 1;
      xcopy(tmpString,        nim, w);
      xcopy(tmpString + 1500, nim, w);
      while(stringWidth(tmpString, &standardFont, true, true) >= SCREEN_WIDTH) {
        w = stringLastGlyph(tmpString);
        tmpString[w] = 0;
      }

      if(stringWidth(tmpString + 1500 + w, &standardFont, true, true) + wLastBaseStandard > SCREEN_WIDTH - 8) { // 8 is the standard font cursor width
        btnClicked(NULL, "16"); // back space
      }
      else {
        showString(tmpString, &standardFont, 0, Y_POSITION_OF_NIM_LINE - 3, vmNormal, true, true);

        xCursor = showString(tmpString + 1500 + w, &standardFont, 0, Y_POSITION_OF_NIM_LINE + 18, vmNormal, true, true);
        yCursor = Y_POSITION_OF_NIM_LINE + 18;
        cursorFont = &standardFont;

        if(lastIntegerBase != 0) {
          showString(lastBase, &standardFont, xCursor + 8, Y_POSITION_OF_NIM_LINE + 18, vmNormal, true, true);
        }
      }
    }
  }



  static void displayShiftAndTamBuffer(void) {
    if(shiftF) {
      showGlyph(STD_SUP_f, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // f is pixel 4+8+3 wide
    }
    else if(shiftG) {
      showGlyph(STD_SUP_g, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // g is pixel 4+10+1 wide
    }

    if(tam.mode) {
      if(calcMode == CM_PEM) { // Variable line to display TAM informations
        lcd_fill_rect(45+20, tamOverPemYPos, 168, 20, LCD_SET_VALUE);
        showString(tamBuffer, &standardFont, 75+20, tamOverPemYPos, vmNormal,  false, false);
      }
      else { // Fixed line to display TAM informations
        if(shiftF || shiftG) {
          lcd_fill_rect(18, Y_POSITION_OF_TAM_LINE, 120, 32, LCD_SET_VALUE);
        }
        else {
          lcd_fill_rect(0, Y_POSITION_OF_TAM_LINE, 138, 32, LCD_SET_VALUE);
        }
        showString(tamBuffer, &standardFont, 18, Y_POSITION_OF_TAM_LINE + 6, vmNormal, true, true);
      }
    }
  }


int16_t refreshScreenCounter = 0;        //JM
//uint8_t last_CM = 255;
void refreshScreen(void) {
if (running_program_jm) return;          //JM TEST PROGRAM!
#ifdef PC_BUILD
  jm_show_calc_state("refreshScreen");
  printf(">>> refreshScreenCounter=%d calcMode=%d last_CM=%d \n",refreshScreenCounter++, calcMode, last_CM);    //JMYY
#endif
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStart(3); }     //dr
#endif

  //clearScreen();  //JM do not use this clearscreen. Rather use the distributed clearscreens, WITH the if(last_CM != calcMode)


  if(calcMode!=CM_AIM && calcMode!=CM_NIM && calcMode!=CM_PLOT_STAT && calcMode!=CM_GRAPH && calcMode!=CM_LISTXY) {last_CM = 254;}  //JM Force NON-CM_AIM and NON-CM_NIM to refresh to be compatible to 43S 

  switch(calcMode) {
    case CM_FLAG_BROWSER:
      last_CM = calcMode;
      clearScreen();
      flagBrowser(NOPARAM);
      refreshStatusBar();
      break;

    case CM_FONT_BROWSER:
      last_CM = calcMode;
      clearScreen();
      fontBrowser(NOPARAM);
      refreshStatusBar();
      break;

    case CM_REGISTER_BROWSER:
      last_CM = calcMode;
      clearScreen();
      registerBrowser(NOPARAM);
      refreshStatusBar();
      break;

    case CM_PEM:
      clearScreen();
      showSoftmenuCurrentPart();
      fnPem(NOPARAM);
      displayShiftAndTamBuffer();
      refreshStatusBar();
      break;

      case CM_NORMAL:
      case CM_AIM:
      case CM_NIM:
      case CM_MIM:
      case CM_ASSIGN:
      case CM_ERROR_MESSAGE:
    case CM_CONFIRMATION:
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStart(0); }     //dr
#endif
      if(last_CM != calcMode) {
          clearScreen();

        // The ordering of the 4 lines below is important for SHOW (temporaryInformation == TI_SHOW_REGISTER)
          refreshRegisterLine(REGISTER_T);
          refreshRegisterLine(REGISTER_Z);
          refreshRegisterLine(REGISTER_Y);
      }

#ifdef INLINE_TEST
  if(testEnabled) { fnSwStop(0); }      //dr
#endif
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStart(1); }     //dr
#endif
        refreshRegisterLine(REGISTER_X);
        if(calcMode == CM_MIM) {
          showMatrixEditor();
        }

#ifdef INLINE_TEST
  if(testEnabled) { fnSwStop(1); }      //dr
#endif
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStart(2); }     //dr
#endif

      if((last_CM != calcMode) || (doRefreshSoftMenu)) {
        last_CM = calcMode;
        doRefreshSoftMenu = false;
        displayShiftAndTamBuffer();
        showSoftmenuCurrentPart();
        hourGlassIconEnabled = false;
        refreshStatusBar();

      }
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStop(2); }      //dr
#endif

      break;

    case CM_LISTXY:                     //JM
      if((last_CM != calcMode) || (doRefreshSoftMenu)) {
        last_CM = calcMode;
        doRefreshSoftMenu = false;
        displayShiftAndTamBuffer();
        refreshStatusBar();
        fnStatList();
        hourGlassIconEnabled = false;
        refreshStatusBar();
      }
      break;


    case CM_GRAPH:                      //JM
      if((last_CM != calcMode) || (doRefreshSoftMenu)) {
        last_CM = calcMode;
        doRefreshSoftMenu = false;
        displayShiftAndTamBuffer();
        showSoftmenuCurrentPart();
        refreshStatusBar();
        graph_plotmem();
        hourGlassIconEnabled = false;
        refreshStatusBar();
      }
      break;


    case CM_PLOT_STAT:
      if((last_CM != calcMode) || (doRefreshSoftMenu)) {
        if(last_CM == 252) last_CM--; else last_CM = 252; //calcMode;
        doRefreshSoftMenu = false;
        displayShiftAndTamBuffer();
        showSoftmenuCurrentPart();
        refreshStatusBar();
        hourGlassIconEnabled = true;
        graphPlotstat(plotSelection);
        graphDrawLRline(plotSelection);
        hourGlassIconEnabled = false;
        refreshStatusBar();
      }
      break;

      default: {}
    }

  #ifndef DMCP_BUILD
    refreshLcd(NULL);
  #endif // !DMCP_BUILD

#ifdef INLINE_TEST
  if(testEnabled) { fnSwStop(3); }      //dr
#endif
}
#endif // !TESTSUITE_BUILD



void fnScreenDump(uint16_t unusedButMandatoryParameter) {
  #ifdef PC_BUILD
    FILE *bmp;
    char bmpFileName[22];
    time_t rawTime;
    struct tm *timeInfo;
    int32_t x, y;
    uint32_t uint32;
    uint16_t uint16;
    uint8_t  uint8;

    time(&rawTime);
    timeInfo = localtime(&rawTime);

    strftime(bmpFileName, 22, "%Y%m%d-%H%M%S00.bmp", timeInfo);
    bmp = fopen(bmpFileName, "wb");

    fwrite("BM", 1, 2, bmp);        // Offset 0x00  0  BMP header

    uint32 = (SCREEN_WIDTH/8 * SCREEN_HEIGHT) + 610;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x02  2  File size

    uint32 = 0;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x06  6  unused

    uint32 = 0x00000082;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x0a 10  Offset where the bitmap data can be found

    uint32 = 0x0000006c;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x0e 14  Number of bytes in DIB header

    uint32 = SCREEN_WIDTH;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x12 18  Bitmap width

    uint32 = SCREEN_HEIGHT;
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
    for(y=SCREEN_HEIGHT-1; y>=0; y--) {
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
  #endif // PC_BUILD

  #ifdef DMCP_BUILD
    resetShiftState();                  //JM To avoid f or g top left of the screen, clear to make sure
    create_screenshot(0);
  #endif // DMCP_BUILD
}



#ifndef TESTSUITE_BUILD 
/********************************************//**   //JM vv
 * \brief Clears parts of the screen
 *
 * \param[in] clearStatusBar bool_t Clear the status bar
 * \param[in] clearRegLines bool_t  Clear the register lines
 * \param[in] clearSoftkeys bool_t  Clear the softkey area
 * \return void
 ***********************************************/
void clearScreen_old(bool_t clearStatusBar, bool_t clearRegisterLines, bool_t clearSoftkeys) {      //JMOLD
  if(clearStatusBar) {
    lcd_fill_rect(0, 0, SCREEN_WIDTH, 20, 0);
  }
  if(clearRegisterLines) {
    lcd_fill_rect(0, 20, SCREEN_WIDTH, 151, 0);
  }
  if(clearSoftkeys) {
    clear_ul(); //JMUL
    lcd_fill_rect(0, 171, SCREEN_WIDTH, 69, 0);
    lcd_fill_rect(0, 171-5, 20, 5, 0);
  }
}                                                       //JM ^^
#endif //TESTSUITE_BUILD
