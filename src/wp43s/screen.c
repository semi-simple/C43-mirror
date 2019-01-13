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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("drawScreen");
  #endif

  cairo_surface_t *imageSurface;

  imageSurface = cairo_image_surface_create_for_data((unsigned char *)screenData, CAIRO_FORMAT_RGB24, SCREEN_WIDTH, SCREEN_HEIGHT, screenStride*4);
  cairo_set_source_surface(cr, imageSurface, 0, 0);
  cairo_surface_mark_dirty(imageSurface);
  cairo_paint(cr);
  cairo_surface_destroy(imageSurface);

  screenChange = false;

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("drawScreen");
  #endif

  return FALSE;
}



void copyScreenToClipboard(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("copyScreenToClipboard");
  #endif

  cairo_surface_t *imageSurface;
  GtkClipboard *clipboard;

  printf("Screen copy to clipboard\n");

  clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
  gtk_clipboard_clear(clipboard);

  imageSurface = cairo_image_surface_create_for_data((unsigned char *)screenData, CAIRO_FORMAT_RGB24, SCREEN_WIDTH, SCREEN_HEIGHT, screenStride*4);
  gtk_clipboard_set_image(clipboard, gdk_pixbuf_get_from_surface(imageSurface, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("copyScreenToClipboard");
  #endif
}



/********************************************//**
 * \brief Stops the program, wait for ENTER on the PC's
 *        keyboard or click on close button
 *
 * \param void
 * \return void
 ***********************************************/
void waitAndSee(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("waitAndSee");
  #endif

  refreshStack();
  #if (DEBUG_PANEL == 1)
   refreshDebugPanel();
  #endif
  refreshScreen(NULL);

  while(gtk_events_pending()) {
    gtk_main_iteration();
  }

  showInfoDialog("Click close to continue", NULL, NULL, NULL);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("waitAndSee");
  #endif
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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("refreshScreen");
  #endif

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

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("refreshScreen");
  #endif

  return TRUE;
}
#endif


/********************************************//**
 * \brief Sets a pixel on the screen (black).
 *
 * \param[in] x int16_t x coordinate from 0 (left) to 399 (right)
 * \param[in] y int16_t y coordinate from 0 (top) to 239 (bottom)
 * \return void
 ***********************************************/
void setPixel(int16_t x, int16_t y) {
  #if (LOG_FUNCTIONS == 1)
    //enteringFunction("setPixel");
  #endif

  #ifdef PC_BUILD
    if(x<0 || x>=SCREEN_WIDTH || y<0 || y>=SCREEN_HEIGHT) {
      printf("In function setPixel: x=%d, y=%d outside the screen!\n", x, y);

      #if (LOG_FUNCTIONS == 1)
        //leavingFunction("setPixel");
      #endif

      return;
    }

    *(screenData + y*screenStride + x) = ON_PIXEL;
    screenChange = true;
  #endif

  #ifdef DMCP_BUILD
    bitblt24(x, 1, y, 1, BLT_OR, BLT_NONE);
  #endif

  #if (LOG_FUNCTIONS == 1)
    //leavingFunction("setPixel");
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
  #if (LOG_FUNCTIONS == 1)
    //enteringFunction("clearPixel");
  #endif

  #ifdef PC_BUILD
    if(x<0 || x>=SCREEN_WIDTH || y<0 || y>=SCREEN_HEIGHT) {
      printf("In function clearPixel: x=%d, y=%d outside the screen!\n", x, y);

      #if (LOG_FUNCTIONS == 1)
        //leavingFunction("clearPixel");
      #endif

      return;
    }

    *(screenData + y*screenStride + x) = OFF_PIXEL;
    screenChange = true;
  #endif

  #ifdef DMCP_BUILD
    bitblt24(x, 1, y, 1, BLT_ANDN, BLT_NONE);
  #endif

  #if (LOG_FUNCTIONS == 1)
    //leavingFunction("clearPixel");
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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("showGlyphCode");
  #endif

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

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("showGlyphCode");
    #endif

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

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("showGlyphCode");
  #endif

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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("showGlyph");
  #endif

  uint16_t charCode;

  charCode = (uint8_t)*ch;
  if(charCode &0x0080) {
    charCode = (charCode << 8) | (uint8_t)*(ch+1);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("showGlyph");
  #endif

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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("showString");
  #endif

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

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("showString");
  #endif

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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("clearScreen");
  #endif

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

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("clearScreen");
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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("showCursor");
  #endif

  if(cursorFont == CF_STANDARD) {
    showGlyph(STD_CURSOR, &standardFont, xCursor, yCursor, vmNormal, true, false);
  }
  else {
    showGlyph(NUM_CURSOR, &numericFont,  xCursor, yCursor, vmNormal, true, false);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("showCursor");
  #endif
}



/********************************************//**
 * \brief Hides the cursor
 *
 * \param void
 * \return void
 ***********************************************/
void hideCursor(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("hideCursor");
  #endif

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

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("hideCursor");
  #endif
}



/********************************************//**
 * \brief Clears one register line
 *
 * \param[in] yStart int16_t y coordinate from where starting to clear
 * \return void
 ***********************************************/
void clearRegisterLine(int16_t yStart, int16_t height) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("clearRegisterLine");
  #endif

  int16_t x, y;

  for(x=0; x<SCREEN_WIDTH; x++) {
    for(y=yStart; y<yStart+height; y++) {
      clearPixel(x, y);
    }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("clearRegisterLine");
  #endif
}



/********************************************//**
 * \brief Displays one register line
 *
 * \param[in] regist int16_t Register line to display
 * \return void
 ***********************************************/
void refreshRegisterLine(calcRegister_t regist) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("refreshRegisterLine");
  #endif

  int16_t w, prefixWidth;
  char    prefix[15];

  #if (DEBUG_PANEL == 1)
    refreshDebugPanel();
  #endif
  if(calcMode != CM_BUG_ON_SCREEN) {
    if(REGISTER_X <= regist && regist <= REGISTER_T) {
      if(lastErrorCode == 0 || regist != errorRegisterLine) {
        if(regist == REGISTER_T) {
          clearRegisterLine(134 - 37*(regist-100), 32);
        }
        else {
          clearRegisterLine(132 - 37*(regist-100), 34);
        }

        #ifdef PC_BUILD
          #if (DEBUG_REGISTER_L == 1)
            char     string1[500], string2[500], *p;
            uint16_t i, n=0;

            strcpy(string1, "L = ");

            if(getRegisterDataType(REGISTER_L) == dtReal16) {
              strcat(string1, "real16 = ");
              formatReal16Debug(string2 + n, getRegisterDataPointer(REGISTER_L));
            }

            else if(getRegisterDataType(REGISTER_L) == dtReal34) {
              strcat(string1, "real34 = ");
              formatReal34Debug(string2 + n, getRegisterDataPointer(REGISTER_L));
            }

            else if(getRegisterDataType(REGISTER_L) == dtComplex16) {
              strcat(string1, "complex16 = ");
              formatComplex16Debug(string2 + n, getRegisterDataPointer(REGISTER_L));
            }

            else if(getRegisterDataType(REGISTER_L) == dtComplex34) {
              strcat(string1, "complex34 = ");
              formatComplex34Debug(string2 + n, getRegisterDataPointer(REGISTER_L));
            }

            else if(getRegisterDataType(REGISTER_L) == dtString) {
              strcat(string1, "string = ");
              for(i=0, p=POINTER_TO_REGISTER_STRING(REGISTER_L); i<=stringByteLength(POINTER_TO_REGISTER_STRING(REGISTER_L)); i++, p++) {
                string2[n + i] = *p;
              }
            }

            else if(getRegisterDataType(REGISTER_L) == dtSmallInteger) {
              const font_t *font = &standardFont;

              strcat(string1, "small integer = ");
              smallIntegerToDisplayString(REGISTER_L, string2 + n, &font);
              strcat(string2 + n, STD_SPACE_3_PER_EM);
              strcat(string2 + n, getSmallIntegerModeName(smallIntegerMode));
            }

            else if(getRegisterDataType(REGISTER_L) == dtBigInteger) {
              strcat(string1, "big integer = ");
              bigIntegerToDisplayString(REGISTER_L, string2 + n);
            }

            else {
              sprintf(string2 + n, "data type %s not supported for now!", getRegisterDataTypeName(REGISTER_L, false, false));
            }

            stringToUtf8(string1, (uint8_t *)tmpStr3000);
            stringToUtf8(string2, (uint8_t *)tmpStr3000 + 1000);

            gtk_label_set_label(GTK_LABEL(lblRegisterL1), tmpStr3000);
            gtk_label_set_label(GTK_LABEL(lblRegisterL2), tmpStr3000 + 1000);
            gtk_widget_show(lblRegisterL1);
            gtk_widget_show(lblRegisterL2);
          #endif
        #endif

        if(temporaryInformation == TI_ARE_YOU_SURE && regist == REGISTER_X) {
          showString("Are you sure?", &standardFont, 1, 134 - 37*(regist-100), vmNormal, false, false);
        }

        else if(temporaryInformation == TI_WHO && regist == REGISTER_X) {
          showString(WHO, &standardFont, 1, 134 - 37*(regist-100), vmNormal, false, false);
        }

        else if(temporaryInformation == TI_VERSION && regist == REGISTER_X) {
          showString(VERSION, &standardFont, 1, 134 - 37*(regist-100), vmNormal, false, false);
        }

        else if(temporaryInformation == TI_FALSE && regist == REGISTER_X) {
          sprintf(tmpStr3000, "false");
          w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
          showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
        }

        else if(temporaryInformation == TI_TRUE && regist == REGISTER_X) {
          sprintf(tmpStr3000, "true");
          w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
          showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
        }

        else if(temporaryInformation == TI_RESET && regist == REGISTER_X) {
          sprintf(tmpStr3000, "Data, programs, and definitions cleared");
          w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
          showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
        }

        else if(regist < REGISTER_X + displayStack || (lastErrorCode != 0 && regist == errorMessageRegisterLine)) {
          prefixWidth = 0;

          if(lastErrorCode != 0 && regist == errorMessageRegisterLine) {
            w = stringWidth(errorMessages[lastErrorCode], &standardFont, false, false) + 1;
            if(w <= SCREEN_WIDTH) {
              showString(errorMessages[lastErrorCode], &standardFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
            }
            else {
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                sprintf(errorMessage, "Error message %" FMT8U " is too wide!", lastErrorCode);
                showInfoDialog("In function refreshRegisterLine:", errorMessage, errorMessages[lastErrorCode], NULL);
              #endif
              sprintf(tmpStr3000, "Error message %" FMT8U " is too wide!", lastErrorCode);
              w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
              showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
            }
          }

          else if(regist == NIM_REGISTER_LINE && calcMode == CM_NIM) {
            w = stringWidth(nimBufferDisplay, &numericFont, false, true) + 13;
            if(w <= SCREEN_WIDTH) {
              xCursor = showString(nimBufferDisplay, &numericFont, 1, 134 - 37*(regist-100), vmNormal, false, true);
              cursorFont = CF_NUMERIC;
            }
            else {
              xCursor = showString(nimBufferDisplay, &standardFont, 1, 134 - 37*(regist-100), vmNormal, false, true);
              cursorFont = CF_STANDARD;
            }
          }

          else if(   displayRealAsFraction
                  && (
                          (   getRegisterDataType(regist) == dtReal16
                           && (
                                  (   real16CompareAbsGreaterThan(REAL16_POINTER(POINTER_TO_REGISTER_DATA(regist)), const16_1e_4)
                                   && real16CompareAbsLessThan(REAL16_POINTER(POINTER_TO_REGISTER_DATA(regist)), const16_1e6)
                                  )
                               || real16IsZero(POINTER_TO_REGISTER_DATA(regist))
                              )
                          )
                       || (   getRegisterDataType(regist) == dtReal34
                           && (
                                  (   real34CompareAbsGreaterThan(REAL34_POINTER(POINTER_TO_REGISTER_DATA(regist)), const34_1e_4)
                                   && real34CompareAbsLessThan(REAL34_POINTER(POINTER_TO_REGISTER_DATA(regist)), const34_1e6)
                                  )
                               || real34IsZero(POINTER_TO_REGISTER_DATA(regist))
                              )
                          )
                     )
                 ) {
            fractionToDisplayString(regist, tmpStr3000);

            if(temporaryInformation == TI_STATISTIC_SUMS) {
              if(regist == REGISTER_Y) {
                sprintf(prefix, "Data point %03" FMT32S, real34ToInt32(RAM(statisticalSumsPointer)));
                prefixWidth = stringWidth(prefix, &standardFont, false, false) + 2;
                for(w=0; w<SCREEN_WIDTH; w++) {
                  setPixel(w, Y_POSITION_OF_REGISTER_Y_LINE - 2);
                }
              }
            }

            w = stringWidth(tmpStr3000, &numericFont, false, false) + 1;
            if(w + prefixWidth <= SCREEN_WIDTH) {
              if(prefixWidth > 0) {
                showString(prefix, &standardFont, 1, 146 - 37*(regist-100), vmNormal, false, false);
              }
              showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
            }
            else {
              w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
              if(w + prefixWidth > SCREEN_WIDTH) {
                #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                  showInfoDialog("In function refreshRegisterLine:", "Fraction representation too wide!", tmpStr3000, NULL);
                #endif
                strcpy(tmpStr3000, "Fraction representation too wide!");
                w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
              }
              if(prefixWidth > 0) {
                showString(prefix, &standardFont, 1, 146 - 37*(regist-100), vmNormal, false, false);
              }
              showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
            }
          }

          else if(getRegisterDataType(regist) == dtReal16) {
            real16ToDisplayString(REAL16_POINTER(POINTER_TO_REGISTER_DATA(regist)), false, tmpStr3000);

            if(temporaryInformation == TI_RADIUS_THETA) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "r" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, false, false) + 2;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, STD_theta STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, false, false) + 2;
                registerAngleToDisplayString(regist, tmpStr3000);
              }
            }

            else if(temporaryInformation == TI_X_Y) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "x" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, false, false) + 2;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "y" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, false, false) + 2;
              }
            }

            else if(temporaryInformation == TI_ANGLE) {
              if(regist == REGISTER_X) {
                registerAngleToDisplayString(regist, tmpStr3000);
              }
            }

            else if(temporaryInformation == TI_STATISTIC_SUMS) {
              if(regist == REGISTER_Y) {
                sprintf(prefix, "Data point %03" FMT32S, real34ToInt32(RAM(statisticalSumsPointer)));
                prefixWidth = stringWidth(prefix, &standardFont, false, false) + 2;
                for(w=0; w<SCREEN_WIDTH; w++) {
                  setPixel(w, Y_POSITION_OF_REGISTER_Y_LINE - 2);
                }
              }
            }

            w = stringWidth(tmpStr3000, &numericFont, false, false) + 1;
            if(w + prefixWidth <= SCREEN_WIDTH) {
              if(prefixWidth > 0) {
                showString(prefix, &standardFont, 1, 146 - 37*(regist-100), vmNormal, false, false);
              }
              showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
            }
            else {
              w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
              if(w + prefixWidth > SCREEN_WIDTH) {
                #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                  showInfoDialog("In function refreshRegisterLine:", "real16 representation too wide!", tmpStr3000, NULL);
                #endif
                strcpy(tmpStr3000, "real16 representation too wide!");
                w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
              }
              if(prefixWidth > 0) {
                showString(prefix, &standardFont, 1, 146 - 37*(regist-100), vmNormal, false, false);
              }
              showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
            }
          }

          else if(getRegisterDataType(regist) == dtReal34) {
            real34ToDisplayString(REAL34_POINTER(POINTER_TO_REGISTER_DATA(regist)), tmpStr3000);

            w = stringWidth(tmpStr3000, &numericFont, false, false) + 1;
            if(w + prefixWidth <= SCREEN_WIDTH) {
              if(prefixWidth > 0) {
                showString(prefix, &standardFont, 1, 146 - 37*(regist-100), vmNormal, false, false);
              }
              showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
            }
            else {
              w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
              if(w + prefixWidth > SCREEN_WIDTH) {
                #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                  showInfoDialog("In function refreshRegisterLine:", "real34 representation too wide!", tmpStr3000, NULL);
                #endif
                strcpy(tmpStr3000, "real34 representation too wide!");
                w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
              }
              if(prefixWidth > 0) {
                showString(prefix, &standardFont, 1, 146 - 37*(regist-100), vmNormal, false, false);
              }
              showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
            }
          }

          else if(getRegisterDataType(regist) == dtComplex16) {
            complex16ToDisplayString(COMPLEX16_POINTER(POINTER_TO_REGISTER_DATA(regist)), tmpStr3000);

            w = stringWidth(tmpStr3000, &numericFont, false, false) + 1;

            if(w <= SCREEN_WIDTH) {
              showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
            }
            else {
              w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
              if(w > SCREEN_WIDTH) {
                #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                  showInfoDialog("In function refreshRegisterLine:", "complex16 representation too wide!", tmpStr3000, NULL);
                #endif
                strcpy(tmpStr3000, "complex16 representation too wide!");
                w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
              }
              showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
            }
          }

          else if(getRegisterDataType(regist) == dtComplex34) {
            complex34ToDisplayString(COMPLEX34_POINTER(POINTER_TO_REGISTER_DATA(regist)), tmpStr3000);

            w = stringWidth(tmpStr3000, &numericFont, false, false) + 1;

            if(w <= SCREEN_WIDTH) {
              showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
            }
            else {
              w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
              if(w > SCREEN_WIDTH) {
                #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                  showInfoDialog("In function refreshRegisterLine:", "complex34 representation too wide!", tmpStr3000, NULL);
                #endif
                strcpy(tmpStr3000, "complex34 representation too wide!");
                w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
              }
              showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
            }
          }

          else if(getRegisterDataType(regist) == dtString) {
            w = stringWidth(POINTER_TO_REGISTER_STRING(regist), &standardFont, false, true) + 1;

            if(w > SCREEN_WIDTH) {
              strcpy(tmpStr3000, "String is too wide!");
            }

            showString(POINTER_TO_REGISTER_STRING(regist), &standardFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, true);
          }

          else if(getRegisterDataType(regist) == dtSmallInteger) {
            const font_t *font;

            font = NULL;
            smallIntegerToDisplayString(regist, tmpStr3000, &font);
            showString(tmpStr3000, font, SCREEN_WIDTH - stringWidth(tmpStr3000, font, false, false) - 1, 134 - 37*(regist-100), vmNormal, false, false);
          }

          else if(getRegisterDataType(regist) == dtBigInteger) {
            bigIntegerToDisplayString(regist, tmpStr3000);

            w = stringWidth(tmpStr3000, &numericFont, false, false) + 1;
            if(w <= SCREEN_WIDTH) {
              showString(tmpStr3000, &numericFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
            }
            else {
              w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
              if(w > SCREEN_WIDTH) {
                #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                  showInfoDialog("In function refreshRegisterLine:", "Big integer representation too wide!", tmpStr3000, NULL);
                #endif
                strcpy(tmpStr3000, "Big integer representation too wide!");
              }
              w = stringWidth(tmpStr3000, &standardFont, false, false) + 1;
              showString(tmpStr3000, &standardFont, SCREEN_WIDTH - w, 134 - 37*(regist-100), vmNormal, false, false);
            }
          }

          else {
            sprintf(tmpStr3000, "Showing %s: to be coded!", getRegisterDataTypeName(regist, true, false));
            showString(tmpStr3000, &standardFont, SCREEN_WIDTH - stringWidth(tmpStr3000, &standardFont, false, false) - 1, 134 - 37*(regist-100), vmNormal, false, false);
          }
        }
      }
    }
    else {
      sprintf(errorMessage, "In function refreshRegisterLine: regist=%" FMT16S " but only register X, Y, Z and T are valid here (from 100 to 103)!", regist);
      displayBugScreen(errorMessage);
    }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("refreshRegisterLine");
  #endif
}
