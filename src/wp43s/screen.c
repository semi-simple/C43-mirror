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
 * \file screen.c Screen related functions
 ***********************************************/

#include "wp43s.h"

#ifdef PC_BUILD
/********************************************//**
 * \brief Draws the calc's screen on the PC window widget
 *
 * \param[in] widget GtkWidget* Not used
 * \param[in] cr cairo_t*
 * \param[in] data gpointer     Not used
 * \return gboolean
 ***********************************************/
gboolean drawScreen(GtkWidget *widget, cairo_t *cr, gpointer data) {
  cairo_surface_t *imageSurface;

  imageSurface = cairo_image_surface_create_for_data((unsigned char *)screenData, CAIRO_FORMAT_RGB24, SCREEN_WIDTH, SCREEN_HEIGHT, screenStride*4);
  cairo_set_source_surface(cr, imageSurface, 0, 0);
  cairo_surface_mark_dirty(imageSurface);
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

  imageSurface = cairo_image_surface_create_for_data((unsigned char *)screenData, CAIRO_FORMAT_RGB24, SCREEN_WIDTH, SCREEN_HEIGHT, screenStride*4);
  gtk_clipboard_set_image(clipboard, gdk_pixbuf_get_from_surface(imageSurface, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
}



static void angularUnitToString(uint32_t angularMode, char *string) {
  switch(angularMode) {
    case AM_DEGREE: strcpy(string, STD_DEGREE); break;
    case AM_DMS:    strcpy(string, "d.ms");     break;
    case AM_GRAD:   strcpy(string, "g");        break;
    case AM_RADIAN: strcpy(string, "r");        break;
    case AM_MULTPI: strcpy(string, STD_pi);     break;
    case AM_NONE:   break;
    default:        strcpy(string, "?");
  }
}



void copyRegisterToClipboardString(calcRegister_t regist, char *clipboardString) {
  longInteger_t lgInt;
  int16_t base, sign, n;
  uint64_t shortInt;
  static const char digits[17] = "0123456789ABCDEF";

  switch(getRegisterDataType(regist)) {
    case dtLongInteger:
      convertLongIntegerRegisterToLongInteger(regist, lgInt);
      longIntegerToAllocatedString(lgInt, tmpStr3000, sizeof(tmpStr3000));
      longIntegerFree(lgInt);
      break;

    case dtReal16:
      real16ToString(REGISTER_REAL16_DATA(regist), tmpStr3000 + TMP_STR_LENGTH/2);
      if(strchr(tmpStr3000 + TMP_STR_LENGTH/2, '.') == NULL && strchr(tmpStr3000 + TMP_STR_LENGTH/2, 'E') == NULL) {
        strcat(tmpStr3000 + TMP_STR_LENGTH/2, ".");
      }
      angularUnitToString(getRegisterAngularMode(regist), tmpStr3000 + TMP_STR_LENGTH/2 + strlen(tmpStr3000 + TMP_STR_LENGTH/2));
      stringToUtf8(tmpStr3000 + TMP_STR_LENGTH/2, (uint8_t *)tmpStr3000);
      break;

    case dtComplex16:
      real16ToString(REGISTER_REAL16_DATA(regist), tmpStr3000);
      if(real16IsNegative(REGISTER_IMAG16_DATA(regist))) {
        strcat(tmpStr3000, " - ix");
        real16SetPositiveSign(REGISTER_IMAG16_DATA(regist));
        real16ToString(REGISTER_IMAG16_DATA(regist), tmpStr3000 + strlen(tmpStr3000));
        real16SetNegativeSign(REGISTER_IMAG16_DATA(regist));
      }
      else {
        strcat(tmpStr3000, " + ix");
        real16ToString(REGISTER_IMAG16_DATA(regist), tmpStr3000 + strlen(tmpStr3000));
      }
      break;

    case dtTime:
      strcpy(tmpStr3000, "Copying a time to the clipboard is to be coded!");
      break;

    case dtDate:
      strcpy(tmpStr3000, "Copying a date to the clipboard is to be coded!");
      break;

    case dtString:
      memcpy(tmpStr3000 + TMP_STR_LENGTH/2, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist))+1);
      stringToUtf8(tmpStr3000 + TMP_STR_LENGTH/2, (uint8_t *)tmpStr3000);
      break;

    case dtReal16Matrix:
      strcpy(tmpStr3000, "Copying a real16 matrix to the clipboard is to be coded!");
      break;

    case dtComplex16Matrix:
      strcpy(tmpStr3000, "Copying a complex16 matrix to the clipboard is to be coded!");
      break;

    case dtShortInteger:
      convertShortIntegerRegisterToUInt64(regist, &sign, &shortInt);
      base = getRegisterShortIntegerBase(regist);

      n = ERROR_MESSAGE_LENGTH - 100;
      sprintf(errorMessage + n--, "#%d (word size = %u)", base, shortIntegerWordSize);

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

      strcpy(tmpStr3000, errorMessage + n);
      break;

    case dtReal34:
      real34ToString(REGISTER_REAL34_DATA(regist), tmpStr3000 + TMP_STR_LENGTH/2);
      if(strchr(tmpStr3000 + TMP_STR_LENGTH/2, '.') == NULL && strchr(tmpStr3000 + TMP_STR_LENGTH/2, 'E') == NULL) {
        strcat(tmpStr3000 + TMP_STR_LENGTH/2, ".");
      }
      angularUnitToString(getRegisterAngularMode(regist), tmpStr3000 + TMP_STR_LENGTH/2 + strlen(tmpStr3000 + TMP_STR_LENGTH/2));
      stringToUtf8(tmpStr3000 + TMP_STR_LENGTH/2, (uint8_t *)tmpStr3000);
      break;

    case dtComplex34:
      real34ToString(REGISTER_REAL34_DATA(regist), tmpStr3000);
      if(real34IsNegative(REGISTER_IMAG34_DATA(regist))) {
        strcat(tmpStr3000, " - ix");
        real34SetPositiveSign(REGISTER_IMAG34_DATA(regist));
        real34ToString(REGISTER_IMAG34_DATA(regist), tmpStr3000 + strlen(tmpStr3000));
        real34SetNegativeSign(REGISTER_IMAG34_DATA(regist));
      }
      else {
        strcat(tmpStr3000, " + ix");
        real34ToString(REGISTER_IMAG34_DATA(regist), tmpStr3000 + strlen(tmpStr3000));
      }
      break;

    default:
      sprintf(tmpStr3000, "In function copyRegisterXToClipboard, the data type %" FMT32U " is unknown! Please try to reproduce and submit a bug.", getRegisterDataType(regist));
  }

  strcpy(clipboardString, tmpStr3000);
}



void copyRegisterXToClipboard(void) {
  GtkClipboard *clipboard;
  char clipboardString[3000];

  clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
  gtk_clipboard_clear(clipboard);
  gtk_clipboard_set_text(clipboard, "", 0); //JM FOUND TIP TO PROPERLY CLEAR CLIPBOARD: https://stackoverflow.com/questions/2418487/clear-the-system-clipboard-using-the-gtk-lib-in-c/2419673#2419673

  copyRegisterToClipboardString(REGISTER_X, clipboardString);

  gtk_clipboard_set_text(clipboard, tmpStr3000, -1);
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
  char clipboardString[15000], sumName[40], *ptr = clipboardString;

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

  for(int32_t regist=numberOfLocalRegisters-1; regist>=0; --regist) {
    ptr += strlen(ptr);
    sprintf(ptr, LINEBREAK "R.%02d = ", regist);
    ptr += strlen(ptr);
    copyRegisterToClipboardString(FIRST_LOCAL_REGISTER + regist, ptr);
  }

  if(statisticalSumsPointer != NULL) {
    for(int32_t regist=0; regist<STATISTICAL_SUMS; regist++) {
      ptr += strlen(ptr);

      switch(regist) {
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
        default: strcpy(sumName,           "?              "          );
      }

      sprintf(ptr, LINEBREAK "SR%02d = ", regist);
      ptr += strlen(ptr);
      stringToUtf8(sumName, (uint8_t *)ptr);
      ptr += strlen(ptr);
      strcpy(ptr, " = ");
      ptr += strlen(ptr);
      real34ToString(statisticalSumsPointer + regist*REAL34_SIZE, tmpStr3000);
      if(strchr(tmpStr3000, '.') == NULL && strchr(tmpStr3000, 'E') == NULL) {
        strcat(tmpStr3000, ".");
      }
      strcpy(ptr, tmpStr3000);
    }
  }

  gtk_clipboard_set_text(clipboard, clipboardString, -1);
}



/********************************************//**
 * \brief Stops the program, wait for ENTER on the PC's
 *        keyboard or click on close button
 *
 * \param void
 * \return void
 ***********************************************/
void waitAndSee(void) {
  refreshStack();
  #if (DEBUG_PANEL == 1)
    refreshDebugPanel();
  #endif
  refreshScreen(NULL);

  while(gtk_events_pending()) {
    gtk_main_iteration();
  }

  showInfoDialog("Click close to continue", NULL, NULL, NULL);
}



/********************************************//**
 * \brief Refreshes calc's screen. This function is
 * called every 100 ms by a GTK timer.
 * * make the cursor blink if needed
 * * refresh date and time in the status bar if needed
 * * refresh the I flag in the status bar (CPXRES or REALRE) if needed
 * * refresh the overflow and carry flags in the status bar if needed
 * * refresh the whole screen if needed
 *
 * \param[in] data gpointer Not used
 * \return gboolean         What will happen next?
 *                          * true  = timer will call this function again
 *                          * false = timer stops calling this function
 ***********************************************/
gboolean refreshScreen(gpointer data) {// This function is called every 100 ms by a GTK timer
  // Cursor blinking
  if(cursorEnabled) {
    cursorBlinkCounter = (cursorBlinkCounter + 1) % 10;
    if(cursorBlinkCounter <  5) {
      showCursor();
    }
    else {
      hideCursor();
    }
  }

  // Function name display
  if(showFunctionNameCounter>0) {
    if(--showFunctionNameCounter == 0) {
      hideFunctionName();
      showFunctionName(ITM_NOP, 0);
    }
  }

  // Update date and time
  getTimeString(dateTimeString);
  if(strcmp(dateTimeString, oldTime)) {
    allowScreenUpdate = true;
    strcpy(oldTime, dateTimeString);
    showDateTime();
  }

  // If LCD has changed: update the GTK screen
  if(screenChange && allowScreenUpdate) {
    #if (__linux__ == 1) && (DEBUG_PANEL == 1)
      refreshDebugPanel();
    #endif

    gtk_widget_queue_draw(screen);
    while(gtk_events_pending()) {
      gtk_main_iteration();
    }
  }

#ifdef JM_MULTISHIFT                             //JM TIMER - checks on any key pressed.
  if((ShiftTimoutMode || Home3TimerMode)) {      //JM  && (shiftF || shiftG)      //JM TIMER - Only consider if a shift is actually pending
    if(JM_SHIFT_RESET-- == 0) {                  //JM TIMER
      JM_SHIFT_RESET = JM_SHIFT_TIMER_LOOP - JM_3_SHIFT_CUTOFF - 1;   //JM TIMER
      JM_SHIFT_HOME_TIMER1 = JM_SHIFT_TIMER_LOOP;  //JM TIMER //max forces previous delay to max
      if(ShiftTimoutMode) {                      //JM TIMER
        resetShiftState();                       //JM TIMER
      }                                          //JM TIMER
      else {
        if(JM_SHIFT_RESET == JM_SHIFT_TIMER_OFF - 1) { 
          JM_SHIFT_RESET++;
        }
      }
    }                                            //JM TIMER
  }                                              //JM TIMER
#endif                                           //JM TIMER

  return TRUE;
}
#elif defined DMCP_BUILD
void refreshScreen(void) {// This function is called roughly every 100 ms from the main loop
  // Cursor blinking
  if(cursorEnabled) {
    cursorBlinkCounter = (cursorBlinkCounter + 1) % 10;
    if(cursorBlinkCounter <  5) {
      showCursor();
    }
    else {
      hideCursor();
    }
  }

  // Function name display
  if(showFunctionNameCounter>0) {
    if(--showFunctionNameCounter == 0) {
      hideFunctionName();
      showFunctionName(ITM_NOP, 0);
    }
  }

  // Update date and time
  getTimeString(dateTimeString);
  if(strcmp(dateTimeString, oldTime)) {
    allowScreenUpdate = true;
    strcpy(oldTime, dateTimeString);
    showDateTime();
  }

#ifdef JM_MULTISHIFT                             //JM TIMER - checks on any key pressed.
  if((ShiftTimoutMode || Home3TimerMode)) {      //JM  && (shiftF || shiftG)      //JM TIMER - Only consider if a shift is actually pending
    if(JM_SHIFT_RESET-- == 0) {                  //JM TIMER
      JM_SHIFT_RESET =  JM_SHIFT_TIMER_LOOP - JM_3_SHIFT_CUTOFF - 1;    //JM TIMER
      JM_SHIFT_HOME_TIMER1 = JM_SHIFT_TIMER_LOOP;  //JM TIMER //max forces previous delay to max
      if(ShiftTimoutMode) {                      //JM TIMER
        resetShiftState();                       //JM TIMER
      }                                          //JM TIMER
      else {
        if(JM_SHIFT_RESET == JM_SHIFT_TIMER_OFF - 1) { 
          JM_SHIFT_RESET++;
        }
      }
    }                                            //JM TIMER
  }                                              //JM TIMER
#endif                                           //JM TIMER

}
#endif



#ifndef TESTSUITE_BUILD
//vv dr StopWatch
/********************************************//**
 * \brief Start StopWatch
 *
 * \param[in] void
 * \return void
 ***********************************************/
void fnSwStart(void) {
#ifdef DMCP_BUILD
  swStart = sys_current_ms();
#endif
#ifdef PC_BUILD
  swStart = g_get_monotonic_time();
#endif
}



/********************************************//**
 * \brief Stop StopWatch
 *
 * \param[in] void
 * \return void
 ***********************************************/
void fnSwStop(void) {
#ifdef DMCP_BUILD
  swStop = sys_current_ms();
#endif
#ifdef PC_BUILD
  swStop = g_get_monotonic_time();
#endif
  uint32_t swTime = swStop - swStart;
  char snum[50];
#ifdef DMCP_BUILD
  showString("ms:", &standardFont, 30, 60, vmNormal, false, false);
#endif
#ifdef PC_BUILD
  showString(STD_mu "s:", &standardFont, 30, 60, vmNormal, false, false);
#endif
  itoa(swTime, snum, 10);
  strcat(snum, "         ");
  showString(snum, &standardFont, 60, 60, vmNormal, false, false);
}
//^^



/********************************************//**
 * \brief Draws the dots on the margins of the f and g lines on screen
 *
 * \param[in] Info uint16_t
 * \return void
 ***********************************************/
void JM_DOT(int16_t xx, int16_t yy) {                          // To draw the dots for f/g on screen

  setPixel (xx+4,yy+7);
  setPixel (xx+5,yy+6);
  setPixel (xx+6,yy+6);
  setPixel (xx+6,yy+5);
  setPixel (xx+7,yy+4);
  setPixel (xx+6,yy+3);
  setPixel (xx+6,yy+2);
  setPixel (xx+5,yy+2);
  setPixel (xx+4,yy+2);
  setPixel (xx+3,yy+2);
  setPixel (xx+2,yy+2);
  setPixel (xx+2,yy+3);
  setPixel (xx+2,yy+4);
  setPixel (xx+2,yy+5);
  setPixel (xx+2,yy+6);
  setPixel (xx+3,yy+6);
  setPixel (xx+4,yy+6);
  setPixel (xx+5,yy+5);
  setPixel (xx+6,yy+4);
  setPixel (xx+5,yy+3);
  setPixel (xx+3,yy+3);
  setPixel (xx+3,yy+5);
  clearPixel (xx+4,yy+7);
  clearPixel (xx+5,yy+7);
  clearPixel (xx+6,yy+7);
  clearPixel (xx+6,yy+6);
  clearPixel (xx+7,yy+6);
  clearPixel (xx+7,yy+5);
  clearPixel (xx+7,yy+4);
  clearPixel (xx+7,yy+3);
  clearPixel (xx+6,yy+2);
  clearPixel (xx+6,yy+1);
  clearPixel (xx+5,yy+1);
  clearPixel (xx+4,yy+1);
  clearPixel (xx+3,yy+1);
  clearPixel (xx+2,yy+2);
  clearPixel (xx+1,yy+3);
  clearPixel (xx+1,yy+4);
  clearPixel (xx+1,yy+5);
  clearPixel (xx+1,yy+6);
  clearPixel (xx+2,yy+6);
  clearPixel (xx+3,yy+7);
}



/********************************************//**
 * \brief Sets a pixel on the screen (black).
 *
 * \param[in] x int16_t x coordinate from 0 (left) to 399 (right)
 * \param[in] y int16_t y coordinate from 0 (top) to 239 (bottom)
 * \return void
 ***********************************************/
void setPixel(int16_t x, int16_t y) {
  #ifdef PC_BUILD
    if(x<0 || x>=SCREEN_WIDTH || y<0 || y>=SCREEN_HEIGHT) {
      //printf("In function setPixel: x=%d, y=%d outside the screen!\n", x, y);
      return;
    }

    *(screenData + y*screenStride + x) = ON_PIXEL;
    screenChange = true;
  #endif

  #ifdef DMCP_BUILD
    bitblt24(x, 1, y, 1, BLT_OR, BLT_NONE);
  #endif
}



/********************************************//**
 * \brief Clears a pixel on the screen (white).
 *
 * \param[in] x int16_t x coordinate from 0 (left) to 399 (right)
 * \param[in] y int16_t y coordinate from 0 (top) to 239 (bottom)
 * \return void
 ***********************************************/
void clearPixel(int16_t x, int16_t y) {
  #ifdef PC_BUILD
    if(x<0 || x>=SCREEN_WIDTH || y<0 || y>=SCREEN_HEIGHT) {
      //printf("In function clearPixel: x=%d, y=%d outside the screen!\n", x, y);
      return;
    }

    *(screenData + y*screenStride + x) = OFF_PIXEL;
    screenChange = true;
  #endif

  #ifdef DMCP_BUILD
    bitblt24(x, 1, y, 1, BLT_ANDN, BLT_NONE);
  #endif
}



/********************************************//**
 * \brief Displays a glyph using it's Unicode code point
 *
 * \param[in] charCode uint16_t      Unicode code point of the glyph to display
 * \param[in] font font_t*           Font to use
 * \param[in] x int16_t              x coordinate where to display the glyph
 * \param[in] y int16_t              y coordinate where to display the glyph
 * \param[in] videoMode videoMode_t  Video mode normal or reverse
 * \param[in] showLeadingCols bool_t Display the leading empty columns
 * \param[in] showEndingCols bool_t  Display the ending empty columns
 * \return int16_t                   x coordinate for the next glyph
 ***********************************************/
int16_t showGlyphCode(uint16_t charCode, const font_t *font, int16_t x, int16_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols) {
  int16_t  col, row, xGlyph, xEndingCols, endingCols, bit, glyphId;
  int8_t   byte, *data;
  const glyph_t  *glyph;

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
    sprintf(errorMessage, "In function showGlyphCode: %d is an unexpected value returned by fingGlyph!", glyphId);
    displayBugScreen(errorMessage);
    return 0;
  }

  data = (int8_t *)glyph->data;

  xGlyph      = showLeadingCols ? glyph->colsBeforeGlyph : 0;
  xEndingCols = x + xGlyph + glyph->colsGlyph;
  endingCols  = showEndingCols ? glyph->colsAfterGlyph : 0;

  // Clearing the rows above the glyph
  for(row=0; row<glyph->rowsAboveGlyph; row++, y++) {
    for(col=0; col<xGlyph + glyph->colsGlyph + endingCols; col++) {
      if(videoMode == vmNormal) {
        clearPixel(x+col, y);
      }
      else {
        setPixel(x+col, y);
      }
    }
  }

  // Drawing the glyph
  for(row=0; row<glyph->rowsGlyph; row++, y++) {
    // Clearing the columns before the glyph
    if(showLeadingCols) {
      for(col=0; col<glyph->colsBeforeGlyph; col++) {
        if(videoMode == vmNormal) {
          clearPixel(x+col, y);
        }
        else {
          setPixel(x+col, y);
        }
      }
    }

    // Drawing the columns of the glyph
    bit = 7;
    for(col=0; col<glyph->colsGlyph; col++) {
      if(bit == 7) {
        byte = *(data++);
      }

      if(byte & 0x80) {// MSB set
        if(videoMode == vmNormal) {
          setPixel(x+xGlyph+col, y);
        }
        else {
          clearPixel(x+xGlyph+col, y);
        }
      }
      else {
        if(videoMode == vmNormal) {
          clearPixel(x+xGlyph+col, y);
        }
        else {
          setPixel(x+xGlyph+col, y);
        }
      }

      byte <<= 1;

      if(--bit == -1) {
        bit = 7;
      }
    }

    // clearing the columns after the glyph
    for(col=0; col<endingCols; col++) {
      if(videoMode == vmNormal) {
        clearPixel(xEndingCols+col, y);
      }
      else {
        setPixel(xEndingCols+col, y);
      }
    }
  }

  // Clearing the rows below the glyph
  for(row=0; row<glyph->rowsBelowGlyph; row++, y++) {
    for(col=0; col<xGlyph + glyph->colsGlyph + endingCols; col++) {
      if(videoMode == vmNormal) {
        clearPixel(x+col, y);
      }
      else {
       setPixel(x+col, y);
      }
    }
  }
  return x + xGlyph + glyph->colsGlyph + endingCols;
}



/********************************************//**
 * \brief Displays the first glyph of a string
 *
 * \param[in] ch const char*         String whose first glyph is to display
 * \param[in] font font_t*           Font to use
 * \param[in] x int16_t              x coordinate where to display the glyph
 * \param[in] y int16_t              y coordinate where to display the glyph
 * \param[in] videoMode videoMode_t  Video mode normal or reverse
 * \param[in] showLeadingCols bool_t Display the leading empty columns
 * \param[in] showEndingCols bool_t  Display the ending empty columns
 * \return int16_t                   x coordinate for the next glyph
 ***********************************************/
int16_t showGlyph(const char *ch, const font_t *font, int16_t x, int16_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols) {
  uint16_t charCode;

  charCode = (uint8_t)*ch;
  if(charCode &0x0080) {
    charCode = (charCode << 8) | (uint8_t)*(ch+1);
  }
  return showGlyphCode(charCode, font, x, y, videoMode, showLeadingCols, showEndingCols);
}



uint8_t  compressString = 0;                                                              //JM compressString
/********************************************//**
 * \brief Displays a 0 terminated string
 *
 * \param[in] string const char*     String whose first glyph is to display
 * \param[in] font font_t*           Font to use
 * \param[in] x int16_t              x coordinate where to display the glyph
 * \param[in] y int16_t              y coordinate where to display the glyph
 * \param[in] videoMode videoMode_t  Video mode normal or reverse
 * \param[in] showLeadingCols bool_t Display the leading empty columns
 * \param[in] showEndingCols bool_t  Display the ending empty columns
 * \return int16_t                   x coordinate for the next glyph
 ***********************************************/
int16_t showString(const char *string, const font_t *font, int16_t x, int16_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols) {
  uint16_t ch, charCode, lg;
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

    charCode = (uint8_t)string[ch++];
    if(charCode & 0x80) {// MSB set?
      charCode = (charCode<<8) | (uint8_t)string[ch++];
    }

    x = showGlyphCode(charCode, font, x, y, videoMode, slc, sec) - compressString;        //JM compressString
  }
  return x;
}



/********************************************//**
 * \brief Clears parts of the screen
 *
 * \param[in] clearStatusBar bool_t Clear the status bar
 * \param[in] clearRegLines bool_t  Clear the register lines
 * \param[in] clearSoftkeys bool_t  Clear the softkey area
 * \return void
 ***********************************************/
void clearScreen(bool_t clearStatusBar, bool_t clearRegisterLines, bool_t clearSoftkeys) {
  #ifdef PC_BUILD
    int16_t x, y;

    if(clearStatusBar) {
      for(y=0; y<20; y++) {
        for(x=0; x<SCREEN_WIDTH; x++) {
         clearPixel(x, y);
        }
      }
    }

    if(clearRegisterLines) {
      for(y=20; y<167; y++) {
        for(x=0; x<SCREEN_WIDTH; x++) {
          clearPixel(x, y);
        }
      }
    }

    if(clearSoftkeys) {
      for(y=167; y<SCREEN_HEIGHT; y++) {
        for(x=0; x<SCREEN_WIDTH; x++) {
          clearPixel(x, y);
        }
      }
    }
  #endif

  #if DMCP_BUILD
    if(clearStatusBar) {
      lcd_fill_rect(0, 0, SCREEN_WIDTH, 20, 0);
    }

    if(clearRegisterLines) {
      lcd_fill_rect(0, 20, SCREEN_WIDTH, 147, 0);
    }

    if(clearSoftkeys) {
      lcd_fill_rect(0, 167, SCREEN_WIDTH, 73, 0);
    }
  #endif
}



/********************************************//**
 * \brief Displays the cursor at it's coordinates using
 * the correct font
 *
 * \param void
 * \return void
 ***********************************************/
void showCursor(void) {
  if(cursorFont == CF_STANDARD) {
    showGlyph(STD_CURSOR, &standardFont, xCursor, yCursor, vmNormal, true, false);
  }
  else {
    showGlyph(NUM_CURSOR, &numericFont,  xCursor, yCursor, vmNormal, true, false);
  }
}



/********************************************//**
 * \brief Hides the cursor
 *
 * \param void
 * \return void
 ***********************************************/
void hideCursor(void) {
  uint16_t x, y;

  if(cursorEnabled) {
    if(cursorFont == CF_STANDARD) {
      for(x=xCursor; x<xCursor+6; x++) {
        for(y=yCursor+10; y<yCursor+16; y++) {
          clearPixel(x, y);
        }
      }
    }
    else {
      for(x=xCursor; x<xCursor+13; x++) {
        for(y=yCursor+15; y<yCursor+28; y++) {
          clearPixel(x, y);
        }
      }
    }
  }
}



/********************************************//**
 * \brief Displays the function of the
 * currently pressed button in the
 * upper left corner of the T register line
 *
 * \param[in] item     int16_t  Item ID to show
 * \param[in] counter  int8_t   number of 1/10 seconds until NOP
 * \return void
 ***********************************************/
void showFunctionName(int16_t item, int8_t counter) {
  showFunctionNameItem = item;
  showFunctionNameCounter = counter;
  if(stringWidth(indexOfItems[item].itemName, &standardFont, true, true) + 1 + lineTWidth > SCREEN_WIDTH) {
    clearRegisterLine(Y_POSITION_OF_REGISTER_T_LINE - 4, REGISTER_LINE_HEIGHT);
  }
  showString(indexOfItems[item].itemName, &standardFont, 1, Y_POSITION_OF_REGISTER_T_LINE + 6, vmNormal, true, true);
}



/********************************************//**
 * \brief Hides the function name in the
 * upper left corner of the T register line
 * and clears the counter
 *
 * \param void
 * \return void
 ***********************************************/
void hideFunctionName() {
  showFunctionNameItem = 0;
  showFunctionNameCounter = 0;
  refreshRegisterLine(REGISTER_T);
}



/********************************************//**
 * \brief Clears one register line
 *
 * \param[in] yStart int16_t y coordinate from where starting to clear
 * \return void
 ***********************************************/
void clearRegisterLine(int16_t yStart, int16_t height) {
  int16_t x, y;

  for(x=0; x<SCREEN_WIDTH; x++) {
    for(y=yStart; y<yStart+height; y++) {
      clearPixel(x, y);
    }
  }
}



/********************************************//**
 * \brief Displays one register line
 *
 * \param[in] regist int16_t Register line to display
 * \return void
 ***********************************************/
void refreshRegisterLine(calcRegister_t regist) {
  int16_t w, wLastBase, prefixWidth, lineWidth = 0;
  char    prefix[15], lastBase[4];

  #if (DEBUG_PANEL == 1)
    refreshDebugPanel();
  #endif

  if(calcMode != CM_BUG_ON_SCREEN) {
    if(REGISTER_X <= regist && regist <= REGISTER_T) {
      if(lastErrorCode == 0 || regist != errorRegisterLine) {
        clearRegisterLine(Y_POSITION_OF_REGISTER_X_LINE - 4 - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), REGISTER_LINE_HEIGHT);

        #ifdef PC_BUILD
          #if (DEBUG_REGISTER_L == 1)
            char     string1[1000], string2[1000], *p;
            uint16_t i;

            strcpy(string1, "L = ");

            if(getRegisterDataType(REGISTER_L) == dtReal16) {
              strcat(string1, "real16 = ");
              formatReal16Debug(string2, getRegisterDataPointer(REGISTER_L));
              strcat(string2, " ");
              strcat(string2, getAngularModeName(getRegisterAngularMode(REGISTER_L)));
            }

            else if(getRegisterDataType(REGISTER_L) == dtReal34) {
              strcat(string1, "real34 = ");
              formatReal34Debug(string2, getRegisterDataPointer(REGISTER_L));
              strcat(string2, " ");
              strcat(string2, getAngularModeName(getRegisterAngularMode(REGISTER_L)));
            }

            else if(getRegisterDataType(REGISTER_L) == dtComplex16) {
              strcat(string1, "complex16 = ");
              formatComplex16Debug(string2, getRegisterDataPointer(REGISTER_L));
            }

            else if(getRegisterDataType(REGISTER_L) == dtComplex34) {
              strcat(string1, "complex34 = ");
              formatComplex34Debug(string2, getRegisterDataPointer(REGISTER_L));
            }

            else if(getRegisterDataType(REGISTER_L) == dtString) {
              strcat(string1, "string = ");
              for(i=0, p=REGISTER_STRING_DATA(REGISTER_L); i<=stringByteLength(REGISTER_STRING_DATA(REGISTER_L)); i++, p++) {
                string2[i] = *p;
              }
            }

            else if(getRegisterDataType(REGISTER_L) == dtShortInteger) {
              const font_t *font = &standardFont;

              strcat(string1, "short integer = ");
              shortIntegerToDisplayString(REGISTER_L, string2, &font);
              strcat(string2, STD_SPACE_3_PER_EM);
              strcat(string2, getShortIntegerModeName(shortIntegerMode));
            }

            else if(getRegisterDataType(REGISTER_L) == dtLongInteger) {
              strcat(string1, "long integer = ");
              longIntegerToDisplayString(REGISTER_L, string2, sizeof(string2), SCREEN_WIDTH);
            }

            else {
              sprintf(string2, "data type %s not supported for now!", getRegisterDataTypeName(REGISTER_L, false, false));
            }

            stringToUtf8(string1, (uint8_t *)tmpStr3000);
            stringToUtf8(string2, (uint8_t *)tmpStr3000 + TMP_STR_LENGTH / 2);

            gtk_label_set_label(GTK_LABEL(lblRegisterL1), tmpStr3000);
            gtk_label_set_label(GTK_LABEL(lblRegisterL2), tmpStr3000 + TMP_STR_LENGTH / 2);
            gtk_widget_show(lblRegisterL1);
            gtk_widget_show(lblRegisterL2);
          #endif
          #if (SHOW_MEMORY_STATUS == 1)
            char string[1000];

            sprintf(string, "%" FMT32S " bytes free (%" FMT32S " block%s), 43C %" FMT32U " bytes, GMP %" FMT32U " bytes -> should always be 0", getFreeRamMemory(), numberOfFreeBlocks, numberOfFreeBlocks==1 ? "" : "s", (uint32_t)wp43sMemInBytes, (uint32_t)gmpMemInBytes); //JM WP43C
            stringToUtf8(string, (uint8_t *)tmpStr3000);
            gtk_label_set_label(GTK_LABEL(lblMemoryStatus), tmpStr3000);
            gtk_widget_show(lblMemoryStatus);
          #endif
        #endif

        if(temporaryInformation == TI_ARE_YOU_SURE && regist == REGISTER_X) {
          showString("Are you sure?", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
        }

        else if(temporaryInformation == TI_WHO && regist == REGISTER_X) {
          showString(WHO, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
          showString(WHO2, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X + 1), vmNormal, true, true);      // JM ID
        }

        else if(temporaryInformation == TI_VERSION && regist == REGISTER_X) {
          showString(VERSION, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
        }

        else if(temporaryInformation == TI_FALSE && regist == REGISTER_X) {
          sprintf(tmpStr3000, "false");
          w = stringWidth(tmpStr3000, &standardFont, true, true);
          showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
        }

        else if(temporaryInformation == TI_TRUE && regist == REGISTER_X) {
          sprintf(tmpStr3000, "true");
          w = stringWidth(tmpStr3000, &standardFont, true, true);
          showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
        }

        else if(temporaryInformation == TI_RESET && regist == REGISTER_X) {
          sprintf(tmpStr3000, "Data, programs, and definitions cleared");
          w = stringWidth(tmpStr3000, &standardFont, true, true);
          showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
        }

        else if(regist < REGISTER_X + displayStack || (lastErrorCode != 0 && regist == errorMessageRegisterLine)) {
          prefixWidth = 0;

          if(lastErrorCode != 0 && regist == errorMessageRegisterLine) {
            w = stringWidth(errorMessages[lastErrorCode], &standardFont, true, true);
            if(w <= SCREEN_WIDTH) {
              showString(errorMessages[lastErrorCode], &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
            }
            else {
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                sprintf(errorMessage, "Error message %" FMT8U " is too wide!", lastErrorCode);
                showInfoDialog("In function refreshRegisterLine:", errorMessage, errorMessages[lastErrorCode], NULL);
              #endif
              sprintf(tmpStr3000, "Error message %" FMT8U " is too wide!", lastErrorCode);
              w = stringWidth(tmpStr3000, &standardFont, true, true);
              showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
            }
          }

          else if(regist == NIM_REGISTER_LINE && calcMode == CM_NIM) {
            if(lastIntegerBase != 0) {
              sprintf(lastBase, "#%" FMT32U, lastIntegerBase);
              wLastBase = stringWidth(lastBase, &numericFont, true, true);
            }
            else {
              wLastBase = 0;
            }

            if(stringWidth(nimBufferDisplay, &numericFont, true, true) + wLastBase <= SCREEN_WIDTH - 16) { // 16 is the standard font cursor widh
              xCursor = showString(nimBufferDisplay, &numericFont, 0, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
              cursorFont = CF_NUMERIC;

              if(lastIntegerBase != 0) {
                showString(lastBase, &numericFont, xCursor + 16, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
              }
            }
            else {
              xCursor = showString(nimBufferDisplay, &standardFont, 0, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
              cursorFont = CF_STANDARD;

              if(lastIntegerBase != 0) {
                showString(lastBase, &standardFont, xCursor + 8, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
              }
            }
          }

          else if(   displayRealAsFraction
                  && (
                          (   getRegisterDataType(regist) == dtReal16
                           && (
                                  (   real16CompareAbsGreaterThan(REGISTER_REAL16_DATA(regist), const16_1e_4)
                                   && real16CompareAbsLessThan(REGISTER_REAL16_DATA(regist), const16_1e6)
                                  )
                               || real16IsZero(REGISTER_REAL16_DATA(regist))
                              )
                          )
                       || (   getRegisterDataType(regist) == dtReal34
                           && (
                                  (   real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(regist), const34_1e_4)
                                   && real34CompareAbsLessThan(REGISTER_REAL34_DATA(regist), const34_1e6)
                                  )
                               || real34IsZero(REGISTER_REAL34_DATA(regist))
                              )
                          )
                     )
                 ) {
            fractionToDisplayString(regist, tmpStr3000);

            if(temporaryInformation == TI_STATISTIC_SUMS) {
              if(regist == REGISTER_Y) {
                sprintf(prefix, "Data point %03" FMT32S, real34ToInt32(statisticalSumsPointer));
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
                for(w=0; w<SCREEN_WIDTH; w++) {
                  setPixel(w, Y_POSITION_OF_REGISTER_Y_LINE - 2);
                }
              }
            }

            w = stringWidth(tmpStr3000, &numericFont, false, true);
            lineWidth = w;
            if(w + prefixWidth <= SCREEN_WIDTH) {
              if(prefixWidth > 0) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE + TEMPORARY_INFO_OFFSET - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
              }
              showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
            }
            else {
              w = stringWidth(tmpStr3000, &standardFont, false, true);
              lineWidth = w;
              if(w + prefixWidth > SCREEN_WIDTH) {
                #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                  showInfoDialog("In function refreshRegisterLine:", "Fraction representation too wide!", tmpStr3000, NULL);
                #endif
                strcpy(tmpStr3000, "Fraction representation too wide!");
                w = stringWidth(tmpStr3000, &standardFont, false, true);
                lineWidth = w;
              }
              if(prefixWidth > 0) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE + TEMPORARY_INFO_OFFSET - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
              }
              showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
            }
          }

          else if(getRegisterDataType(regist) == dtReal16) {
            if(temporaryInformation == TI_RADIUS_THETA) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "r" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              if(regist == REGISTER_Y) {
                strcpy(prefix, STD_theta STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }

              real16ToDisplayString(REGISTER_REAL16_DATA(regist), getRegisterAngularMode(regist), tmpStr3000, &numericFont, SCREEN_WIDTH - prefixWidth);

              w = stringWidth(tmpStr3000, &numericFont, false, true);
              lineWidth = w;
              if(prefixWidth > 0) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE + TEMPORARY_INFO_OFFSET - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
              }
              showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
            }

            else if(temporaryInformation == TI_THETA_RADIUS) {
              if(regist == REGISTER_X) {
                strcpy(prefix, STD_theta STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              if(regist == REGISTER_Y) {
                strcpy(prefix, "r" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }

              real16ToDisplayString(REGISTER_REAL16_DATA(regist), getRegisterAngularMode(regist), tmpStr3000, &numericFont, SCREEN_WIDTH - prefixWidth);

              w = stringWidth(tmpStr3000, &numericFont, false, true);
              lineWidth = w;
              if(prefixWidth > 0) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE + TEMPORARY_INFO_OFFSET - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
              }
              showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
            }

            else if(temporaryInformation == TI_X_Y) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "x" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "y" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            else if(temporaryInformation == TI_RE_IM) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "Im" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "Re" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            else if(temporaryInformation == TI_STATISTIC_SUMS) {
              if(regist == REGISTER_Y) {
                sprintf(prefix, "Data point %03" FMT32S, real34ToInt32(statisticalSumsPointer));
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
                for(w=0; w<SCREEN_WIDTH; w++) {
                  setPixel(w, Y_POSITION_OF_REGISTER_Y_LINE - 2);
                }
              }
            }

            real16ToDisplayString(REGISTER_REAL16_DATA(regist), getRegisterAngularMode(regist), tmpStr3000, &numericFont, SCREEN_WIDTH - prefixWidth);

            w = stringWidth(tmpStr3000, &numericFont, false, true);
            lineWidth = w;
            if(prefixWidth > 0) {
              showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE + TEMPORARY_INFO_OFFSET - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
            }
            showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
          }

          else if(getRegisterDataType(regist) == dtReal34) {
            if(temporaryInformation == TI_RADIUS_THETA) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "r" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              if(regist == REGISTER_Y) {
                strcpy(prefix, STD_theta STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }

              real34ToDisplayString(REGISTER_REAL34_DATA(regist), getRegisterAngularMode(regist), tmpStr3000, &numericFont, SCREEN_WIDTH - prefixWidth);

              w = stringWidth(tmpStr3000, &numericFont, false, true);
              lineWidth = w;
              if(prefixWidth > 0) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE + TEMPORARY_INFO_OFFSET - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
              }
              showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
            }

            else if(temporaryInformation == TI_RADIUS_THETA) {
              if(regist == REGISTER_X) {
                strcpy(prefix, STD_theta STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              if(regist == REGISTER_Y) {
                strcpy(prefix, "r" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }

              real34ToDisplayString(REGISTER_REAL34_DATA(regist), getRegisterAngularMode(regist), tmpStr3000, &numericFont, SCREEN_WIDTH - prefixWidth);

              w = stringWidth(tmpStr3000, &numericFont, false, true);
              lineWidth = w;
              if(prefixWidth > 0) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE + TEMPORARY_INFO_OFFSET - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
              }
              showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
            }

            else if(temporaryInformation == TI_X_Y) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "x" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "y" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }
            else if(temporaryInformation == TI_RE_IM) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "Im" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "Re" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            real34ToDisplayString(REGISTER_REAL34_DATA(regist), getRegisterAngularMode(regist), tmpStr3000, &numericFont, SCREEN_WIDTH - prefixWidth);

            w = stringWidth(tmpStr3000, &numericFont, false, true);
            lineWidth = w;
            if(prefixWidth > 0) {
              showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE + TEMPORARY_INFO_OFFSET - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
            }
            showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
          }

          else if(getRegisterDataType(regist) == dtComplex16) {

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
            }                                                                       //JM EE ^

            complex16ToDisplayString(REGISTER_COMPLEX16_DATA(regist), tmpStr3000, &numericFont, SCREEN_WIDTH);

            w = stringWidth(tmpStr3000, &numericFont, false, true);
            lineWidth = w;
            showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
          }

          else if(getRegisterDataType(regist) == dtComplex34) {
            complex34ToDisplayString(REGISTER_COMPLEX34_DATA(regist), tmpStr3000, &numericFont, SCREEN_WIDTH);

            w = stringWidth(tmpStr3000, &numericFont, false, true);
            lineWidth = w;
            showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
          }

          else if(getRegisterDataType(regist) == dtString) {
            w = stringWidth(REGISTER_STRING_DATA(regist), &standardFont, false, true);
            lineWidth = w;

            if(w > SCREEN_WIDTH) {
              strcpy(tmpStr3000, "String is too wide!");
              w = stringWidth(tmpStr3000, &standardFont, false, true);
              lineWidth = w;
            }

            showString(REGISTER_STRING_DATA(regist), &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
          }

          else if(getRegisterDataType(regist) == dtShortInteger) {
            const font_t *font;

            font = NULL;
            shortIntegerToDisplayString(regist, tmpStr3000, &font);
            showString(tmpStr3000, font, SCREEN_WIDTH - stringWidth(tmpStr3000, font, false, true), Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
          }

          else if(getRegisterDataType(regist) == dtLongInteger) {
            longIntegerToDisplayString(regist, tmpStr3000, TMP_STR_LENGTH, SCREEN_WIDTH);

            w = stringWidth(tmpStr3000, &numericFont, false, true);
            lineWidth = w;

            if(w <= SCREEN_WIDTH) {
              showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
            }
            else {
              w = stringWidth(tmpStr3000, &standardFont, false, true);
              if(w > SCREEN_WIDTH) {
                #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                  showInfoDialog("In function refreshRegisterLine:", "Long integer representation too wide!", tmpStr3000, NULL);
                #endif
                strcpy(tmpStr3000, "Long integer representation too wide!");
              }
              w = stringWidth(tmpStr3000, &standardFont, false, true);
              lineWidth = w;
              showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
            }
          }

          else {
            sprintf(tmpStr3000, "Showing %s: to be coded!", getRegisterDataTypeName(regist, true, false));
            showString(tmpStr3000, &standardFont, SCREEN_WIDTH - stringWidth(tmpStr3000, &standardFont, false, true), Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
          }
        }
      }

      if(regist == REGISTER_T) {
        lineTWidth = lineWidth;
      }
    }
    else {
      sprintf(errorMessage, "In function refreshRegisterLine: regist=%" FMT16S " but only register X, Y, Z and T are valid here (from 100 to 103)!", regist);
      displayBugScreen(errorMessage);
    }
  }
}
#endif
