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
 * \file statusbar.c Status bar management
 ***********************************************/

#include "wp43s.h"

/********************************************//**
 * \brief Displays date and time in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showDateTime(void) {
  int16_t x;

  //if(displayDateTime) {
    showGlyph(STD_SPACE_EM, &standardFont, X_REAL_COMPLEX-12, 0, vmNormal, true, true); // Clear garbage after time STD_SPACE_EM is 0+0+12 pixel wide
    showGlyph(STD_SPACE_EM, &standardFont, X_REAL_COMPLEX-24, 0, vmNormal, true, true); // Clear garbage after time STD_SPACE_EM is 0+0+12 pixel wide

    getDateString(dateTimeString);
    x = showString(dateTimeString, &standardFont, X_DATE, 0, vmNormal, true, true);

    x = showGlyph(timeFormat==TF_H12 ? STD_SPACE_3_PER_EM : STD_SPACE_FIGURE, &standardFont, x, 0, vmNormal, true, true); // is 0+0+8 pixel wide

    getTimeString(dateTimeString);
    showString(dateTimeString, &standardFont, x, 0, vmNormal, true, false);
  //}
}



/********************************************//**
 * \brief Displays the complex result mode C or R in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showRealComplexResult(void) {
  if(getFlag(FLAG_CPXRES)) {
    showGlyph(STD_COMPLEX_C, &standardFont, X_REAL_COMPLEX, 0, vmNormal, true, false); // Complex C is 0+8+3 pixel wide
  }
  else {
    showGlyph(STD_REAL_R,    &standardFont, X_REAL_COMPLEX, 0, vmNormal, true, false); // Real R    is 0+8+3 pixel wide
  }
}



/********************************************//**
 * \brief Displays the complex mode rectangular or polar in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showComplexMode(void) {
  if(complexMode == CM_POLAR) {
   showGlyph(STD_SUN,           &standardFont, X_COMPLEX_MODE, 0, vmNormal, true, true); // Sun         is 0+12+2 pixel wide
  }
  else {
   showGlyph(STD_RIGHT_ANGLE,   &standardFont, X_COMPLEX_MODE, 0, vmNormal, true, true); // Right angle is 0+12+2 pixel wide
  }
}



/********************************************//**
 * \brief Displays the angular mode in the status bar
 *
 * \param void
 * \return void
 *
 ***********************************************/
void showAngularMode(void) {
  int16_t x;

  x = showGlyph(STD_MEASURED_ANGLE, &standardFont, X_ANGULAR_MODE, 0, vmNormal, true, true); // Angle is 0+9+3 pixel wide

  if(angularMode == AM_DEGREE) {
    x = showGlyph(STD_DEGREE,             &standardFont, x, 0, vmNormal, true, true); // °  is 0+6+2 pixel wide
    showGlyph(STD_SPACE_HAIR,             &standardFont, x, 0, vmNormal, true, true); //    is 0+0+1 pixel wide
  }
  else if(angularMode == AM_GRAD) {
    x = showGlyph(STD_SUP_g,              &standardFont, x, 0, vmNormal, true, true); // g  is 0+6+2 pixel wide
    showGlyph(STD_SPACE_HAIR,             &standardFont, x, 0, vmNormal, true, true); //    is 0+0+1 pixel wide
  }
  else if(angularMode == AM_RADIAN) {
    x = showGlyph(STD_SUP_r,              &standardFont, x, 0, vmNormal, true, true); // r  is 0+6+2 pixel wide
    showGlyph(STD_SPACE_HAIR,             &standardFont, x, 0, vmNormal, true, true); //    is 0+0+1 pixel wide
  }
  else if(angularMode == AM_MULTPI) {
    showGlyph(STD_pi,                     &standardFont, x, 0, vmNormal, true, true); // pi is 0+9+2 pixel wide
  }
  else if(angularMode == AM_DMS) {
    x = showGlyph(STD_RIGHT_DOUBLE_QUOTE, &standardFont, x, 0, vmNormal, true, true); // "  is 0+6+2 pixel wide
    showGlyph(STD_SPACE_HAIR,             &standardFont, x, 0, vmNormal, true, true); //    is 0+0+1 pixel wide
  }
  else {
    sprintf(errorMessage, "In function showAngularMode: %" FMT8U " is an unexpected value for angularMode!", angularMode);
    displayBugScreen(errorMessage);
  }
}



/********************************************//**
 * \brief Displays the faction mode in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showFracMode(void) {
  int16_t x = 0;

  showString(STD_SPACE_EM STD_SPACE_EM STD_SPACE_EM STD_SPACE_EM STD_SPACE_EM, &standardFont, X_INTEGER_MODE-12*5, 0, vmNormal, true, true); // STD_SPACE_EM is 0+0+12 pixel wide

  if(denominatorMode == DM_ANY && denMax == DM_DENMAX) {
    showString("/max", &standardFont, X_FRAC_MODE, 0, vmNormal, true, true);
  }
  else {
    if((denominatorMode==DM_ANY && denMax!=DM_DENMAX) || denominatorMode==DM_FIX || denominatorMode==DM_FAC) {
      sprintf(tmpStr3000, "/%" FMT32U, denMax);
      x = showString(tmpStr3000, &standardFont, X_FRAC_MODE, 0, vmNormal, true, true);
    }

    if(denominatorMode == DM_FIX) {
     showGlyphCode('f',  &standardFont, x, 0, vmNormal, true, true); // f is 0+7+3 pixel wide
    }
    else if(denominatorMode == DM_FAC) {
     showString(PRODUCT_SIGN, &standardFont, x, 0, vmNormal, true, true); // STD_DOT is 0+3+2 pixel wide and STD_CROSS is 0+7+2 pixel wide
    }
    else if(denominatorMode != DM_ANY) {
      sprintf(errorMessage, "In function showFracMode: %" FMT8U " is an unexpected value for denominatorMode!", denominatorMode);
      displayBugScreen(errorMessage);
    }
  }
}



/********************************************//**
 * \brief Displays the integer mode icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showIntegerMode(void) {
  if(smallIntegerWordSize <= 9) {
    sprintf(tmpStr3000, STD_SPACE_FIGURE "%" FMT8U ":%c", smallIntegerWordSize, smallIntegerMode==SIM_1COMPL?'1':(smallIntegerMode==SIM_2COMPL?'2':(smallIntegerMode==SIM_UNSIGN?'u':(smallIntegerMode==SIM_SIGNMT?'s':'?'))));
  }
  else {
    sprintf(tmpStr3000, "%" FMT8U ":%c", smallIntegerWordSize, smallIntegerMode==SIM_1COMPL?'1':(smallIntegerMode==SIM_2COMPL?'2':(smallIntegerMode==SIM_UNSIGN?'u':(smallIntegerMode==SIM_SIGNMT?'s':'?'))));
  }

  showString(tmpStr3000, &standardFont, X_INTEGER_MODE, 0, vmNormal, true, true);
}



/********************************************//**
 * \brief Displays the overflow flag and the carry flag
 *
 * \param void
 * \return void
 ***********************************************/
void showOverflowCarry(void) {
  int16_t x, y;

  showGlyph(STD_OVERFLOW_CARRY, &standardFont, X_OVERFLOW_CARRY, 0, vmNormal, true, false); // STD_OVERFLOW_CARRY is 0+6+3 pixel wide

  if(!getFlag(FLAG_OVERFLOW)) { // Overflow flag is cleared
    for(x=X_OVERFLOW_CARRY; x<X_OVERFLOW_CARRY+6; x++) {
      for(y=2; y<=8; y++) {
        clearPixel(x, y);
      }
    }
  }

  if(!getFlag(FLAG_CARRY)) { // Carry flag is cleared
    for(x=X_OVERFLOW_CARRY; x<X_OVERFLOW_CARRY+6; x++) {
      for(y=12; y<=18; y++) {
        clearPixel(x, y);
      }
    }
  }
}



/********************************************//**
 * \brief Displays the alpha mode
 *
 * \param void
 * \return void
 ***********************************************/
void showAlphaMode(void) {
  if(calcMode == CM_AIM) {
    if(alphaCase == AC_UPPER) {
      showString(STD_ALPHA, &standardFont, X_ALPHA_MODE, 0, vmNormal, true, false); // STD_ALPHA is 0+9+2 pixel wide
    }
    else {
      showString(STD_alpha, &standardFont, X_ALPHA_MODE, 0, vmNormal, true, false); // STD_alpha is 0+9+2 pixel wide
    }
  }
  else {
    showGlyphCode(' ',  &standardFont, X_ALPHA_MODE, 0, vmNormal, true, true); // is 0+0+10 pixel wide
  }
}



/********************************************//**
 * \brief Displays the hourglass icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showHourGlass(void) {
  hourGlassIconEnabled = true;
  showGlyph(STD_HOURGLASS, &standardFont, X_HOURGLASS, 0, vmNormal, true, false); // is 0+11+3 pixel wide
}



/********************************************//**
 * \brief Hides the hourglass icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void hideHourGlass(void) {
  hourGlassIconEnabled = false;
  showGlyph(STD_SPACE_EM, &standardFont, X_HOURGLASS, 0, vmNormal, true, true); // STD_SPACE_EM is 12 pixel wide
}



/********************************************//**
 * \brief Toggles the hourglass icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void toggleHourGlass(void) {
  if(hourGlassIconEnabled) {
    showGlyph(STD_SPACE_EM,  &standardFont, X_HOURGLASS, 0, vmNormal, true, true);  // STD_SPACE_EM is 12 pixel wide
  }
  else {
    showGlyph(STD_HOURGLASS, &standardFont, X_HOURGLASS, 0, vmNormal, true, false); // is 0+11+3 pixel wide
  }
  hourGlassIconEnabled = !hourGlassIconEnabled;
}



/********************************************//**
 * \brief Displays or hides the program begin icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showPgmBegin(void) {
  if(programCounter == 0) {
    showGlyph(STD_PGM_BEGIN, &standardFont, X_PROGRAM_BEGIN, 0, vmNormal, true, false); // is 0+10+3 pixel wide
  }
  else {
    showString(STD_SPACE_EM, &standardFont, X_PROGRAM_BEGIN, 0, vmNormal, true, true); // STD_SPACE_EM is 12 pixel wide
  }
}



/********************************************//**
 * \brief Displays the watch icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showWatch(void) {
  watchIconEnabled = true;
  showGlyph(STD_WATCH, &standardFont, X_WATCH, 0, vmNormal, true, false); // is 0+13+1 pixel wide
}



/********************************************//**
 * \brief Hides the watch icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void hideWatch(void) {
  watchIconEnabled = false;
  showString(STD_SPACE_EM STD_SPACE_HAIR, &standardFont, X_WATCH, 0, vmNormal, true, true); // STD_SPACE_EM is 12 pixel wide
}



/********************************************//**
 * \brief Toggles the watch icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void toggleWatch(void) {
  if(watchIconEnabled) {
    showString(STD_SPACE_EM STD_SPACE_HAIR, &standardFont, X_WATCH, 0, vmNormal, true, true);  // STD_SPACE_EM is 12 pixel wide
  }
  else {
    showGlyph(STD_WATCH,                    &standardFont, X_WATCH, 0, vmNormal, true, false); // is 0+13+1 pixel wide
  }
  watchIconEnabled = !watchIconEnabled;
}



/********************************************//**
 * \brief Displays the serial I/O icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showSerialIO(void) {
  serialIOIconEnabled = true;
  showGlyph(STD_SERIAL_IO, &standardFont, X_SERIAL_IO, 0, vmNormal, true, false); // is 0+8+3 pixel wide
}



/********************************************//**
 * \brief Hides the serial I/O icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void hideSerialIO(void) {
  serialIOIconEnabled = false;
  showGlyphCode(' ', &standardFont, X_SERIAL_IO, 0, vmNormal, true, true); // is 10 pixel wide
}



/********************************************//**
 * \brief Toggles the serial I/O icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void toggleSerialIO(void) {
  if(serialIOIconEnabled) {
    showGlyphCode(' ',       &standardFont, X_SERIAL_IO, 0, vmNormal, true, true);  // is 10 pixel wide
  }
  else {
    showGlyph(STD_SERIAL_IO, &standardFont, X_SERIAL_IO, 0, vmNormal, true, false); // is 0+8+3 pixel wide
  }
  serialIOIconEnabled = !serialIOIconEnabled;
}



/********************************************//**
 * \brief Displays the printer icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showPrinter(void) {
  printerIconEnabled = true;
  showGlyph(STD_PRINTER, &standardFont, X_PRINTER, 0, vmNormal, true, false); // is 0+12+3 pixel wide
}



/********************************************//**
 * \brief Hides the printer icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void hidePrinter(void) {
  printerIconEnabled = false;
  showString(STD_SPACE_EM, &standardFont, X_PRINTER, 0, vmNormal, true, true); // STD_SPACE_EM is 12 pixel wide
}



/********************************************//**
 * \brief Toggles the printer icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void togglePrinter(void) {
  if(printerIconEnabled) {
    showString(STD_SPACE_EM, &standardFont, X_PRINTER, 0, vmNormal, true, true);  // STD_SPACE_EM is 12 pixel wide
  }
  else {
    showGlyph(STD_PRINTER,   &standardFont, X_PRINTER, 0, vmNormal, true, false); // is 0+12+3 pixel wide
  }
  printerIconEnabled = !printerIconEnabled;
}



/********************************************//**
 * \brief Displays the battery icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showBattery(void) {
  batteryIconEnabled = true;
  showGlyph(STD_BATTERY, &standardFont, X_BATTERY, 0, vmNormal, true, false); // is 0+10+2 pixel wide
}



/********************************************//**
 * \brief Hides the battery icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void hideBattery(void) {
  batteryIconEnabled = false;
  showGlyphCode(' ', &standardFont, X_BATTERY, 0, vmNormal, true, true); // is 10 pixel wide
}



/********************************************//**
 * \brief Toggles the battery icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void toggleBattery(void) {
  if(batteryIconEnabled) {
    showGlyphCode(' ',     &standardFont, X_BATTERY, 0, vmNormal, true, true);  // is 10 pixel wide
  }
  else {
    showGlyph(STD_BATTERY, &standardFont, X_BATTERY, 0, vmNormal, true, false); // is 0+10+2 pixel wide
  }
  batteryIconEnabled = !batteryIconEnabled;
}



/********************************************//**
 * \brief Displays the user mode icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showUserMode(void) {
  userModeEnabled = true;
  showGlyph(STD_USER_MODE, &standardFont, X_USER_MODE, 0, vmNormal, false, false); // STD_USER_MODE is 0+12+2 pixel wide

  #ifdef PC_BUILD
    if(calcMode == CM_NORMAL) calcModeNormalGui();
    else if(calcMode == CM_AIM) calcModeAimGui();
    else if(calcMode == CM_TAM) calcModeTamGui();
  #endif
}



/********************************************//**
 * \brief Hides the user mode icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void hideUserMode(void) {
  userModeEnabled = false;
  showString(STD_SPACE_EM, &standardFont, X_USER_MODE, 0, vmNormal, true, true);   // STD_SPACE_EM is 12 pixel wide

  #ifdef PC_BUILD
    if(calcMode == CM_NORMAL) calcModeNormalGui();
    else if(calcMode == CM_AIM) calcModeAimGui();
    else if(calcMode == CM_TAM) calcModeTamGui();
  #endif
}



/********************************************//**
 * \brief Toggles the user mode icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void toggleUserMode(void) {
  if(userModeEnabled) {
    showString(STD_SPACE_EM, &standardFont, X_USER_MODE, 0, vmNormal, true, true);   // STD_SPACE_EM is 12 pixel wide
  }
  else {
    showGlyph(STD_USER_MODE, &standardFont, X_USER_MODE, 0, vmNormal, false, false); // STD_USER_MODE is 0+12+2 pixel wide
  }
  userModeEnabled = !userModeEnabled;

  #ifdef PC_BUILD
    if(calcMode == CM_NORMAL) calcModeNormalGui();
    else if(calcMode == CM_AIM) calcModeAimGui();
    else if(calcMode == CM_TAM) calcModeTamGui();
  #endif
}
