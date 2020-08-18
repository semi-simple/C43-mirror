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

  imageSurface = cairo_image_surface_create_for_data((unsigned char *)screenData, CAIRO_FORMAT_RGB24, SCREEN_WIDTH, SCREEN_HEIGHT, screenStride * 4);
  imageSurface = cairo_image_surface_create_for_data((unsigned char *)screenData, CAIRO_FORMAT_RGB24, SCREEN_WIDTH, SCREEN_HEIGHT, screenStride * 4);
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

  imageSurface = cairo_image_surface_create_for_data((unsigned char *)screenData, CAIRO_FORMAT_RGB24, SCREEN_WIDTH, SCREEN_HEIGHT, screenStride * 4);
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

  switch(getRegisterDataType(regist)) {
    case dtLongInteger:
      convertLongIntegerRegisterToLongInteger(regist, lgInt);
      longIntegerToAllocatedString(lgInt, tmpStr3000, sizeof(tmpStr3000));
      longIntegerFree(lgInt);
      break;

    case dtTime:
      strcpy(tmpStr3000, "Copying a time to the clipboard is to be coded!");
      break;

    case dtDate:
      strcpy(tmpStr3000, "Copying a date to the clipboard is to be coded!");
      break;

    case dtString:
      xcopy(tmpStr3000 + TMP_STR_LENGTH/2, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist))+1);
      stringToUtf8(tmpStr3000 + TMP_STR_LENGTH/2, (uint8_t *)tmpStr3000);
      break;

    case dtReal34Matrix:
      strcpy(tmpStr3000, "Copying a real16 matrix to the clipboard is to be coded!");
      break;

    case dtComplex34Matrix:
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

    case dtConfig:
      xcopy(tmpStr3000, "Configuration data", 19);
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

  for(int32_t regist=allLocalRegisterPointer->numberOfLocalRegisters-1; regist>=0; --regist) {
    ptr += strlen(ptr);
    sprintf(ptr, LINEBREAK "R.%02d = ", regist);
    ptr += strlen(ptr);
    copyRegisterToClipboardString(FIRST_LOCAL_REGISTER + regist, ptr);
  }

  if(statisticalSumsPointer != NULL) {
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
      realToString(statisticalSumsPointer + REAL_SIZE * sum, tmpStr3000);
      if(strchr(tmpStr3000, '.') == NULL && strchr(tmpStr3000, 'E') == NULL) {
        strcat(tmpStr3000, ".");
      }
      strcpy(ptr, tmpStr3000);
    }
  }

  gtk_clipboard_set_text(clipboard, clipboardString, -1);
}



/********************************************//**
 * \brief Refreshes calc's screen. This function is
 * called every SCREEN_REFRESH_PERIOD ms by a GTK timer.
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
gboolean refreshLcd(gpointer unusedData) { // This function is called every SCREEN_REFRESH_PERIOD ms by a GTK timer
  // Cursor blinking
  static bool_t cursorBlink=true;

  if(cursorEnabled) {
    if(cursorBlink) {
      showGlyph(STD_CURSOR, cursorFont, xCursor, yCursor, vmNormal, true, false);
    }
    else {
      hideCursor();
    }
    cursorBlink = !cursorBlink;
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
    showDateTime();
  }

  // If LCD has changed: update the GTK screen
  if(screenChange) {
    #if (__linux__ == 1) && (DEBUG_PANEL == 1)
      refreshDebugPanel();
    #endif

printf("refreshLcd (screenChange == true)\n");
    gtk_widget_queue_draw(screen);
    while(gtk_events_pending()) {
      gtk_main_iteration();
    }
  }

  // Alpha selection timer
  if((calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_AIM) && alphaSelectionTimer != 0 && (getUptimeMs() - alphaSelectionTimer) > 3000) { // More than 3 seconds elapsed since last keypress
    resetAlphaSelectionBuffer();
  }

  return TRUE;
}
#elif defined DMCP_BUILD
void refreshLcd(void) {// This function is called roughly every SCREEN_REFRESH_PERIOD ms from the main loop
  // Cursor blinking
  static bool_t cursorBlink=true;

  if(cursorEnabled) {
    if(cursorBlink) {
      showGlyph(STD_CURSOR, cursorFont, xCursor, yCursor, vmNormal, true, false);
    }
    else {
      hideCursor();
    }
    cursorBlink = !cursorBlink;
  }

  // Function name display
  if(showFunctionNameCounter>0) {
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
    showDateTime();
  }

  if(get_lowbat_state() == 1 || get_vbat() < 2500) {
    if(!getSystemFlag(FLAG_LOWBAT)) {
      setSystemFlag(FLAG_LOWBAT);
      showHideLowBattery();
    }
  }
  else {
    if(getSystemFlag(FLAG_LOWBAT)) {
      clearSystemFlag(FLAG_LOWBAT);
      showHideLowBattery();
    }
  }

  if(usb_powered() == 1) {
    if(!getSystemFlag(FLAG_USB)) {
      setSystemFlag(FLAG_USB);
      showHideUSB();
    }
  }
  else {
    if(getSystemFlag(FLAG_USB)) {
      clearSystemFlag(FLAG_USB);
      showHideUSB();
    }
  }

  // Alpha selection timer
  if((calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_AIM) && alphaSelectionTimer != 0 && (getUptimeMs() - alphaSelectionTimer) > 3000) { // More than 3 seconds elapsed since last keypress
    resetAlphaSelectionBuffer();
  }
}
#endif



#ifndef TESTSUITE_BUILD
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
      printf("In function setPixel: x=%d, y=%d outside the screen!\n", x, y);
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
      printf("In function clearPixel: x=%d, y=%d outside the screen!\n", x, y);
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
    #ifdef PC_BUILD                                                                 // Dani Rau
      for(col=0; col<xGlyph + glyph->colsGlyph + endingCols; col++) {
        if(videoMode == vmNormal) {
          clearPixel(x+col, y);
        }
        else {
          setPixel(x+col, y);
        }
      }
    #endif                                                                          // vv Dani Rau

    #if DMCP_BUILD
      if(videoMode == vmNormal) {
        lcd_fill_rect(x, y, xGlyph + glyph->colsGlyph + endingCols, 1, 0);
      }
      else {
        lcd_fill_rect(x, y, xGlyph + glyph->colsGlyph + endingCols, 1, 0xFF);
      }
    #endif                                                                          // ^^ Dani Rau
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
    #ifdef PC_BUILD                                                                 // Dani Rau
      for(col=0; col<xGlyph + glyph->colsGlyph + endingCols; col++) {
        if(videoMode == vmNormal) {
          clearPixel(x+col, y);
        }
        else {
          setPixel(x+col, y);
        }
      }
    #endif                                                                          // vv Dani Rau

    #if DMCP_BUILD
      if(videoMode == vmNormal) {
        lcd_fill_rect(x, y, xGlyph + glyph->colsGlyph + endingCols, 1, 0);
      }
      else {
        lcd_fill_rect(x, y, xGlyph + glyph->colsGlyph + endingCols, 1, 0xFF);
      }
      #endif                                                                        // ^^ Dani Rau
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

    x = showGlyphCode(charCode, font, x, y, videoMode, slc, sec);
  }
  return x;
}



/********************************************//**
 * \brief Hides the cursor
 *
 * \param void
 * \return void
 ***********************************************/
void hideCursor(void) {
  #ifdef PC_BUILD                                         // Dani Rau
    uint16_t x, y;

    if(cursorEnabled) {
      if(cursorFont == &standardFont) {
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
  #endif                                                  // vv Dani Rau

  #if DMCP_BUILD
    if(cursorEnabled) {
      if(cursorFont == &standardFont) {
        lcd_fill_rect(xCursor, yCursor+10, 6, 6, 0);
      }
      else {
        lcd_fill_rect(xCursor, yCursor+15, 13, 13, 0);
      }
    }
  #endif                                                  // ^^ Dani Rau
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
void showFunctionName(int16_t item, int16_t delayInMs) {
  showFunctionNameItem = item;
  showFunctionNameCounter = delayInMs;
  if(stringWidth(indexOfItems[abs(item)].itemCatalogName, &standardFont, true, true) + 1 + lineTWidth > SCREEN_WIDTH) {
    clearRegisterLine(REGISTER_T, true, false);
  }
  showString(indexOfItems[abs(item)].itemCatalogName, &standardFont, 1, Y_POSITION_OF_REGISTER_T_LINE + 6, vmNormal, true, true);
}



/********************************************//**
 * \brief Hides the function name in the
 * upper left corner of the T register line
 * and clears the counter
 *
 * \param void
 * \return void
 ***********************************************/
void hideFunctionName(void) {
  showFunctionNameItem = 0;
  showFunctionNameCounter = 0;
}



/********************************************//**
 * \brief Clears one register line
 *
 * \param[in] yStart int16_t y coordinate from where starting to clear
 * \return void
 ***********************************************/
void clearRegisterLine(calcRegister_t regist, bool_t clearTop, bool_t clearBottom) {
  int16_t yStart, height;

  if(REGISTER_X <= regist && regist <= REGISTER_T) {
    yStart = Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X);
    height = 32;

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

    #ifdef PC_BUILD
      int16_t x, y;
      for(x=0; x<SCREEN_WIDTH; x++) {
        for(y=yStart; y<yStart+height; y++) {
          clearPixel(x, y);
        }
      }
    #endif

    #ifdef DMCP_BUILD
      lcd_fill_rect(0, yStart, SCREEN_WIDTH, height, 0);
    #endif
  }
}



/********************************************//**
 * \brief Displays one register line
 *
 * \param[in] regist int16_t Register line to display
 * \return void
 ***********************************************/
void refreshRegisterLine(calcRegister_t regist) {
  int16_t w, wLastBaseNumeric, wLastBaseStandard, prefixWidth, lineWidth = 0;
  char prefix[18], lastBase[4];

  #if (DEBUG_PANEL == 1)
    refreshDebugPanel();
  #endif

  if(calcMode != CM_BUG_ON_SCREEN) {
    clearRegisterLine(regist, true, (regist != REGISTER_Y));

    #ifdef PC_BUILD
      #if (DEBUG_REGISTER_L == 1 || SHOW_MEMORY_STATUS == 1)
        char tmpStr[1000];
      #endif
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
          longIntegerRegisterToDisplayString(REGISTER_L, string2, sizeof(string2), SCREEN_WIDTH, 50, STD_SPACE_PUNCTUATION);
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
      #endif
      #if (SHOW_MEMORY_STATUS == 1)
        char string[1000];

        sprintf(string, "%" FMT32S " bytes free (%" FMT32S " block%s), 43S %" FMT32U " bytes, GMP %" FMT32U " bytes -> should always be 0", getFreeRamMemory(), numberOfFreeBlocks, numberOfFreeBlocks==1 ? "" : "s", (uint32_t)wp43sMemInBytes, (uint32_t)gmpMemInBytes);
        stringToUtf8(string, (uint8_t *)tmpStr);
        gtk_label_set_label(GTK_LABEL(lblMemoryStatus), tmpStr);
        gtk_widget_show(lblMemoryStatus);
      #endif
    #endif

    if(temporaryInformation == TI_ARE_YOU_SURE && regist == REGISTER_X) {
      showString("Are you sure?", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
    }

    else if(temporaryInformation == TI_WHO && regist == REGISTER_X) {
      showString(WHO, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
    }

    else if(temporaryInformation == TI_VERSION && regist == REGISTER_X) {
      showString(VERSION, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
    }

    else if(temporaryInformation == TI_FALSE && regist == TRUE_FALSE_REGISTER_LINE) {
      sprintf(tmpStr3000, "false");
      showString(tmpStr3000, &standardFont, 1, Y_POSITION_OF_TRUE_FALSE_LINE + 6, vmNormal, true, true);
    }

    else if(temporaryInformation == TI_TRUE && regist == TRUE_FALSE_REGISTER_LINE) {
      sprintf(tmpStr3000, "true");
      showString(tmpStr3000, &standardFont, 1, Y_POSITION_OF_TRUE_FALSE_LINE + 6, vmNormal, true, true);
    }

    else if(temporaryInformation == TI_RESET && regist == REGISTER_X) {
      sprintf(tmpStr3000, "Data, programs, and definitions cleared");
      w = stringWidth(tmpStr3000, &standardFont, true, true);
      showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
    }

    else if(temporaryInformation == TI_SAVED && regist == REGISTER_X) {
      showString("Saved", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
    }

    else if(temporaryInformation == TI_BACKUP_RESTORED && regist == REGISTER_X) {
      showString("Backup restored", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
    }

    else if(temporaryInformation == TI_SHOW_REGISTER && regist == REGISTER_T) { // L1
      w = stringWidth(tmpStr3000, &standardFont, true, true);
      showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*0, vmNormal, true, true);
    }

    else if(temporaryInformation == TI_SHOW_REGISTER && regist == REGISTER_Z && tmpStr3000[300] != 0) { // L2 & L3
      w = stringWidth(tmpStr3000 + 300, &standardFont, true, true);
      showString(tmpStr3000 + 300, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*1, vmNormal, true, true);

      if(tmpStr3000[600]) {
        w = stringWidth(tmpStr3000 + 600, &standardFont, true, true);
        showString(tmpStr3000 + 600, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*2, vmNormal, true, true);
      }
    }

    else if(temporaryInformation == TI_SHOW_REGISTER && regist == REGISTER_Y && tmpStr3000[900] != 0) { // L4 & L5
      w = stringWidth(tmpStr3000 + 900, &standardFont, true, true);
      showString(tmpStr3000 + 900, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*3, vmNormal, true, true);

      if(tmpStr3000[1200]) {
        w = stringWidth(tmpStr3000 + 1200, &standardFont, true, true);
        showString(tmpStr3000 + 1200, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*4, vmNormal, true, true);
      }
    }

    else if(temporaryInformation == TI_SHOW_REGISTER && regist == REGISTER_X && tmpStr3000[1500] != 0) { // L6 & L7
      w = stringWidth(tmpStr3000 + 1500, &standardFont, true, true);
      showString(tmpStr3000 + 1500, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*5, vmNormal, true, true);

      if(tmpStr3000[1800]) {
        w = stringWidth(tmpStr3000 + 1800, &standardFont, true, true);
        showString(tmpStr3000 + 1800, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*6, vmNormal, true, true);
      }
    }

    else if(regist < REGISTER_X + displayStack || (lastErrorCode != 0 && regist == errorMessageRegisterLine)) {
      prefixWidth = 0;

      if(lastErrorCode != 0 && regist == errorMessageRegisterLine) {
        if(stringWidth(errorMessages[lastErrorCode], &standardFont, true, true) <= SCREEN_WIDTH - 1) {
          showString(errorMessages[lastErrorCode], &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
        }
        else {
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "Error message %" FMT8U " is too wide!", lastErrorCode);
            moreInfoOnError("In function refreshRegisterLine:", errorMessage, errorMessages[lastErrorCode], NULL);
          #endif
          sprintf(tmpStr3000, "Error message %" FMT8U " is too wide!", lastErrorCode);
          w = stringWidth(tmpStr3000, &standardFont, true, true);
          showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
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

        if(stringWidth(nimBufferDisplay, &numericFont, true, true) + wLastBaseNumeric <= SCREEN_WIDTH - 16) { // 16 is the numeric font cursor width
          xCursor = showString(nimBufferDisplay, &numericFont, 0, Y_POSITION_OF_NIM_LINE, vmNormal, true, true);
          yCursor = Y_POSITION_OF_NIM_LINE;
          cursorFont = &numericFont;

          if(lastIntegerBase != 0) {
            showString(lastBase, &numericFont, xCursor + 16, Y_POSITION_OF_NIM_LINE, vmNormal, true, true);
          }
        }
        else if(stringWidth(nimBufferDisplay, &standardFont, true, true) + wLastBaseStandard <= SCREEN_WIDTH - 8) { // 8 is the standard font cursor width
          xCursor = showString(nimBufferDisplay, &standardFont, 0, Y_POSITION_OF_NIM_LINE + 6, vmNormal, true, true);
          yCursor = Y_POSITION_OF_NIM_LINE + 6;
          cursorFont = &standardFont;

          if(lastIntegerBase != 0) {
            showString(lastBase, &standardFont, xCursor + 8, Y_POSITION_OF_NIM_LINE + 6, vmNormal, true, true);
          }
        }
        else {
          w = stringByteLength(nimBufferDisplay) + 1;
          xcopy(tmpStr3000,        nimBufferDisplay, w);
          xcopy(tmpStr3000 + 1500, nimBufferDisplay, w);
          while(stringWidth(tmpStr3000, &standardFont, true, true) >= SCREEN_WIDTH) {
            w = stringLastGlyph(tmpStr3000);
            tmpStr3000[w] = 0;
          }

          if(stringWidth(tmpStr3000 + 1500 + w, &standardFont, true, true) + wLastBaseStandard > SCREEN_WIDTH - 8) { // 8 is the standard font cursor width
            btnClicked(NULL, "16"); // back space
          }
          else {
            showString(tmpStr3000, &standardFont, 0, Y_POSITION_OF_NIM_LINE - 3, vmNormal, true, true);

            xCursor = showString(tmpStr3000 + 1500 + w, &standardFont, 0, Y_POSITION_OF_NIM_LINE + 18, vmNormal, true, true);
            yCursor = Y_POSITION_OF_NIM_LINE + 18;
            cursorFont = &standardFont;

            if(lastIntegerBase != 0) {
              showString(lastBase, &standardFont, xCursor + 8, Y_POSITION_OF_NIM_LINE + 18, vmNormal, true, true);
            }
          }
        }
      }

      else if(regist == AIM_REGISTER_LINE && (calcMode == CM_AIM || calcMode == CM_ASM_OVER_AIM)) {
        if(stringWidth(aimBuffer, &standardFont, true, true) < SCREEN_WIDTH - 8) { // 8 is the standard font cursor width
          xCursor = showString(aimBuffer, &standardFont, 1, Y_POSITION_OF_NIM_LINE + 6, vmNormal, true, true);
          yCursor = Y_POSITION_OF_NIM_LINE + 6;
          cursorFont = &standardFont;
        }
        else {
          w = stringByteLength(aimBuffer) + 1;
          xcopy(tmpStr3000,        aimBuffer, w);
          xcopy(tmpStr3000 + 1500, aimBuffer, w);
          while(stringWidth(tmpStr3000, &standardFont, true, true) >= SCREEN_WIDTH - 1) {
            w = stringLastGlyph(tmpStr3000);
            tmpStr3000[w] = 0;
          }

          if(stringWidth(tmpStr3000 + 1500 + w, &standardFont, true, true) >= SCREEN_WIDTH - 8) { // 8 is the standard font cursor width
            btnClicked(NULL, "16"); // back space
          }
          else {
            showString(tmpStr3000, &standardFont, 1, Y_POSITION_OF_NIM_LINE - 3, vmNormal, true, true);

            xCursor = showString(tmpStr3000 + 1500 + w, &standardFont, 1, Y_POSITION_OF_NIM_LINE + 18, vmNormal, true, true);
            yCursor = Y_POSITION_OF_NIM_LINE + 18;
            cursorFont = &standardFont;
          }
        }
      }

      else if(   getSystemFlag(FLAG_FRACT)
              && (    getRegisterDataType(regist) == dtReal34
                   && (
                          (   real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(regist), const34_1e_4)
                           && real34CompareAbsLessThan(REGISTER_REAL34_DATA(regist), const34_1e6)
                          )
                       || real34IsZero(REGISTER_REAL34_DATA(regist))
                      )
                 )
             ) {
        fractionToDisplayString(regist, tmpStr3000);

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
              moreInfoOnError("In function refreshRegisterLine:", "Fraction representation too wide!", tmpStr3000, NULL);
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

      else if(getRegisterDataType(regist) == dtReal34) {
        if(temporaryInformation == TI_RADIUS_THETA) {
          if(regist == REGISTER_X) {
            strcpy(prefix, "r" STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
          else if(regist == REGISTER_Y) {
            strcpy(prefix, STD_theta STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
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

        else if(temporaryInformation == TI_SUMX_SUMY) {
          if(regist == REGISTER_X) {
            strcpy(prefix, STD_SIGMA "x" STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
          else if(regist == REGISTER_Y) {
            strcpy(prefix, STD_SIGMA "y" STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_XMIN_YMIN) {
          if(regist == REGISTER_X) {
            strcpy(prefix, "x" STD_SUB_m STD_SUB_i STD_SUB_n STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
          else if(regist == REGISTER_Y) {
            strcpy(prefix, "y" STD_SUB_m STD_SUB_i STD_SUB_n STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_XMAX_YMAX) {
          if(regist == REGISTER_X) {
            strcpy(prefix, "x" STD_SUB_m STD_SUB_a STD_SUB_x STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
          else if(regist == REGISTER_Y) {
            strcpy(prefix, "y" STD_SUB_m STD_SUB_a STD_SUB_x STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_MEANX_MEANY) {
          if(regist == REGISTER_X) {
            strcpy(prefix, STD_x_BAR STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
          else if(regist == REGISTER_Y) {
            strcpy(prefix, STD_y_BAR STD_SPACE_FIGURE "=");
             prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
         }

        else if(temporaryInformation == TI_SAMPLSTDDEV) {
          if(regist == REGISTER_X) {
            strcpy(prefix, "s" STD_SUB_x STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
          else if(regist == REGISTER_Y) {
            strcpy(prefix, "s" STD_SUB_y STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_POPLSTDDEV) {
          if(regist == REGISTER_X) {
            strcpy(prefix, STD_sigma STD_SUB_x STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
          else if(regist == REGISTER_Y) {
            strcpy(prefix, STD_sigma STD_SUB_y STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_STDERR) {
          if(regist == REGISTER_X) {
            strcpy(prefix, "s" STD_SUB_m STD_SUB_x STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
          else if(regist == REGISTER_Y) {
            strcpy(prefix, "s" STD_SUB_m STD_SUB_y STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_GEOMMEANX_GEOMMEANY) {
          if(regist == REGISTER_X) {
            strcpy(prefix, STD_x_BAR STD_SUB_G STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
          else if(regist == REGISTER_Y) {
            strcpy(prefix, STD_y_BAR STD_SUB_G STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_GEOMSAMPLSTDDEV) {
          if(regist == REGISTER_X) {
            strcpy(prefix, STD_epsilon STD_SUB_x STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
          else if(regist == REGISTER_Y) {
            strcpy(prefix, STD_epsilon STD_SUB_y STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_GEOMPOPLSTDDEV) {
          if(regist == REGISTER_X) {
            strcpy(prefix, STD_epsilon STD_SUB_m STD_SUB_x STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
          else if(regist == REGISTER_Y) {
            strcpy(prefix, STD_epsilon STD_SUB_m STD_SUB_y STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_GEOMSTDERR) {
          if(regist == REGISTER_X) {
            strcpy(prefix, STD_epsilon STD_SUB_p STD_SUB_x STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
          else if(regist == REGISTER_Y) {
            strcpy(prefix, STD_epsilon STD_SUB_p STD_SUB_y STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_WEIGHTEDMEANX) {
          if(regist == REGISTER_X) {
            strcpy(prefix, STD_x_BAR STD_SUB_w STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_WEIGHTEDSAMPLSTDDEV) {
          if(regist == REGISTER_X) {
            strcpy(prefix, "s" STD_SUB_w STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_WEIGHTEDPOPLSTDDEV) {
          if(regist == REGISTER_X) {
            strcpy(prefix, STD_sigma STD_SUB_w STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_WEIGHTEDSTDERR) {
          if(regist == REGISTER_X) {
            strcpy(prefix, "s" STD_SUB_m STD_SUB_w STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_HARMMEANX_HARMMEANY) {
          if(regist == REGISTER_X) {
            strcpy(prefix, STD_x_BAR STD_SUB_H STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
          else if(regist == REGISTER_Y) {
            strcpy(prefix, STD_y_BAR STD_SUB_H STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_RMSMEANX_RMSMEANY) {
          if(regist == REGISTER_X) {
            strcpy(prefix, STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
          else if(regist == REGISTER_Y) {
            strcpy(prefix, STD_y_BAR STD_SUB_R STD_SUB_M STD_SUB_S STD_SPACE_FIGURE "=");
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
          }
        }

        else if(temporaryInformation == TI_STATISTIC_SUMS) {
          if(regist == REGISTER_Y) {
            realToInt32(SIGMA_N, w);
            sprintf(prefix, "Data point %03" FMT16S, w);
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;

            #ifdef PC_BUILD
              for(w=0; w<SCREEN_WIDTH; w++) {
                setPixel(w, Y_POSITION_OF_REGISTER_Y_LINE - 2);
              }
            #endif

            #if DMCP_BUILD
              lcd_fill_rect(0, Y_POSITION_OF_REGISTER_Y_LINE - 2, SCREEN_WIDTH, 1, 0);
            #endif
          }
        }

        real34ToDisplayString(REGISTER_REAL34_DATA(regist), getRegisterAngularMode(regist), tmpStr3000, &numericFont, SCREEN_WIDTH - prefixWidth, NUMBER_OF_DISPLAY_DIGITS, true, STD_SPACE_PUNCTUATION);

        w = stringWidth(tmpStr3000, &numericFont, false, true);
        lineWidth = w;
        if(prefixWidth > 0) {
          showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
        }
        showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
      }

      else if(getRegisterDataType(regist) == dtComplex34) {
        complex34ToDisplayString(REGISTER_COMPLEX34_DATA(regist), tmpStr3000, &numericFont, SCREEN_WIDTH, NUMBER_OF_DISPLAY_DIGITS, true, STD_SPACE_PUNCTUATION);

        w = stringWidth(tmpStr3000, &numericFont, false, true);
        lineWidth = w;
        showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
      }

      else if(getRegisterDataType(regist) == dtString) {
        w = stringWidth(REGISTER_STRING_DATA(regist), &standardFont, false, true);

        if(w >= SCREEN_WIDTH) {
          if(regist == REGISTER_X) {
            xcopy(tmpStr3000, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
            do {
              tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
              w = stringWidth(tmpStr3000, &standardFont, false, true);
            } while(w >= SCREEN_WIDTH);
            showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - 3, vmNormal, false, true);

            w = stringByteLength(tmpStr3000);
            xcopy(tmpStr3000, REGISTER_STRING_DATA(regist) + w, stringByteLength(REGISTER_STRING_DATA(regist) + w) + 1);
            w = stringWidth(tmpStr3000, &standardFont, false, true);
            if(w >= SCREEN_WIDTH) {
              do {
                tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
                w = stringWidth(tmpStr3000, &standardFont, false, true);
              } while(w >= SCREEN_WIDTH - 14); // 14 is the width of STD_ELLIPSIS
              xcopy(tmpStr3000 + stringByteLength(tmpStr3000), STD_ELLIPSIS, 3);
              w += 14;
            }
            showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE + 18, vmNormal, false, true);
          }
          else {
            xcopy(tmpStr3000, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
            do {
              tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
              w = stringWidth(tmpStr3000, &standardFont, false, true);
            } while(w >= SCREEN_WIDTH - 14); // 14 is the width of STD_ELLIPSIS
            xcopy(tmpStr3000 + stringByteLength(tmpStr3000), STD_ELLIPSIS, 3);
            w += 14;
            lineWidth = w;
            showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, false, true);
          }
        }
        else {
          lineWidth = w;
          showString(REGISTER_STRING_DATA(regist), &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, false, true);
        }
      }

      else if(getRegisterDataType(regist) == dtShortInteger) {
        shortIntegerToDisplayString(regist, tmpStr3000, true);
        showString(tmpStr3000, fontForShortInteger, SCREEN_WIDTH - stringWidth(tmpStr3000, fontForShortInteger, false, true), Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
      }

      else if(getRegisterDataType(regist) == dtLongInteger) {
        longIntegerRegisterToDisplayString(regist, tmpStr3000, TMP_STR_LENGTH, SCREEN_WIDTH, 50, STD_SPACE_PUNCTUATION);

        w = stringWidth(tmpStr3000, &numericFont, false, true);
        lineWidth = w;

        if(w <= SCREEN_WIDTH) {
          showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
        }
        else {
          w = stringWidth(tmpStr3000, &standardFont, false, true);
          if(w > SCREEN_WIDTH) {
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              moreInfoOnError("In function refreshRegisterLine:", "Long integer representation too wide!", tmpStr3000, NULL);
            #endif
            strcpy(tmpStr3000, "Long integer representation too wide!");
          }
          w = stringWidth(tmpStr3000, &standardFont, false, true);
          lineWidth = w;
          showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, false, true);
        }
      }

      else if(getRegisterDataType(regist) == dtConfig) {
        xcopy(tmpStr3000, "Configuration data", 19);
        w = stringWidth(tmpStr3000, &numericFont, false, true);
        lineWidth = w;
        showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
      }

      else {
        sprintf(tmpStr3000, "Displaying %s: to be coded!", getRegisterDataTypeName(regist, true, false));
        showString(tmpStr3000, &standardFont, SCREEN_WIDTH - stringWidth(tmpStr3000, &standardFont, false, true), Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, false, true);
      }
    }

    if(regist == REGISTER_T) {
      lineTWidth = lineWidth;
    }
  }
}



void clearScreen(void) {
  #ifdef PC_BUILD
    int16_t x, y;

    for(y=0; y<SCREEN_HEIGHT; y++) {
      for(x=0; x<SCREEN_WIDTH; x++) {
        clearPixel(x, y);
      }
    }
  #endif

  #if DMCP_BUILD
    lcd_fill_rect(0, 0, SCREEN_WIDTH, 240, 0);
  #endif
}



void refreshScreen(void) {
  switch(calcMode) {
    case CM_FLAG_BROWSER:
      clearScreen();
      flagBrowser(NOPARAM);
      refreshStatusBar();
      break;

    case CM_FONT_BROWSER:
      clearScreen();
      fontBrowser(NOPARAM);
      refreshStatusBar();
      break;

    case CM_REGISTER_BROWSER:
      clearScreen();
      registerBrowser(NOPARAM);
      refreshStatusBar();
      break;

    case CM_BUG_ON_SCREEN:
      break;

    case CM_NORMAL:
    case CM_AIM:
    case CM_TAM:
    case CM_NIM:
    case CM_ASM:
    case CM_ASM_OVER_TAM:
    case CM_ASM_OVER_AIM:
    case CM_ASSIGN:
    case CM_ERROR_MESSAGE:
    case CM_CONFIRMATION:
      clearScreen();

      // The ordering of the 4 lines below is important for SHOW (temporaryInformation == TI_SHOW_REGISTER)
      refreshRegisterLine(REGISTER_T);
      refreshRegisterLine(REGISTER_Z);
      refreshRegisterLine(REGISTER_Y);
      refreshRegisterLine(REGISTER_X);

      if(shiftF) {
        showGlyph(STD_SUP_f, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // f is pixel 4+8+3 wide
      }
      else if(shiftG) {
        showGlyph(STD_SUP_g, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // g is pixel 4+10+1 wide
      }
      else {
        if(calcMode == CM_TAM || calcMode == CM_ASM_OVER_TAM) {
          #ifdef PC_BUILD
            int16_t x, y;

            for(y=Y_POSITION_OF_TAM_LINE; y<Y_POSITION_OF_TAM_LINE+32; y++) {
              for(x=0; x<100; x++) {
                clearPixel(x, y);
              }
            }
          #endif

          #if DMCP_BUILD
            lcd_fill_rect(0, Y_POSITION_OF_TAM_LINE, 100, 32, 0);
          #endif

          showString(tamBuffer, &standardFont, 20, Y_POSITION_OF_TAM_LINE + 6, vmNormal, true, true);
        }
      }

      showSoftmenuCurrentPart();

      hourGlassIconEnabled = false;
      refreshStatusBar();
      break;

    default: {}
  }
}
#endif



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
#endif

#ifdef DMCP_BUILD
  create_screenshot(0);
#endif
}
