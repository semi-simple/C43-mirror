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
 * \file error.c
 ***********************************************/

#include "error.h"

#include "charString.h"
#include "flags.h"
#include "fonts.h"
#include "screen.h"
#include <string.h>

#include "wp43s.h"

TO_QSPI const char *errorMessages[NUMBER_OF_ERROR_CODES] = {
/*  0 */  "No error",
/*  1 */  "An argument exceeds the function domain",
/*  2 */  "Bad time or date input",
/*  3 */  "Undefined op-code",
/*  4 */  "Overflow at +" STD_INFINITY,
/*  5 */  "Overflow at -" STD_INFINITY,
/*  6 */  "No such label found",
/*  7 */  "No such function",
/*  8 */  "Out of range",
/*  9 */  "Illegal digit in integer input for this base",
/* 10 */  "Input is too long",
/* 11 */  "RAM is full",
/* 12 */  "Stack clash",
/* 13 */  "Operation is undefined in this mode",
/* 14 */  "Word size is too small",
/* 15 */  "Too few data points for this statistic",
/* 16 */  "Distribution parameter out of valid range",
/* 17 */  "I/O error",
/* 18 */  "Invalid or corrupted data",
/* 19 */  "Flash memory is write protected",
/* 20 */  "No root found",
/* 21 */  "Matrix mismatch",
/* 22 */  "Singular matrix",
/* 23 */  "Flash memory is full",
/* 24 */  "Invalid input data type for this operation",
/* 25 */  "",
/* 26 */  "Please enter a NEW name",
/* 27 */  "Cannot delete a predefined item",
/* 28 */  "No statistic data present",
/* 29 */  "Item to be coded",
/* 30 */  "Function to be coded for that data type",
/* 31 */  "Input data types do not match",
/* 32 */  "This system flag is write protected",
/* 33 */  "Output would exceed 196 characters",
/* 34 */  "This does not work with an empty string",
/* 35 */  "No backup data found",
/* 36 */  "Undefined source variable",
/* 37 */  "This variable is write protected",
/* 38 */  "No matrix indexed",
/* 39 */  "Not enough memory for such a matrix",
/* 40 */  "No errors for selected model",
/* 41 */  "Large " STD_DELTA " and opposite signs, may be a pole",
/* 42 */  "Solver reached local extremum, no root",
/* 43 */  STD_GREATER_EQUAL "1 initial guess lies out of the domain",
/* 44 */  "The function value look constant",
/* 45 */  "Syntax error in this equation",
/* 46 */  "This equation formula is too complex",
/* 47 */  "This item cannot be assigned here",
/* 48 */  "Invalid name",
/* 49 */  "Too many variables",
/* 50 */  "Non-programmable command. Please remove.",
/* 51 */  "Bad input", // This error is not in ReM and cannot occur (theoretically).
};



#ifdef PC_BUILD
  /********************************************//**
   * \brief Displays an error message like a pop up
   *
   * \param[in] m1 const char* 1st part of the message
   * \param[in] m2 const char* 2nd part of the message
   * \param[in] m3 const char* 3rd part of the message
   * \param[in] m4 const char* 4th part of the message
   * \return void
   ***********************************************/
  void moreInfoOnError(const char *m1, const char *m2, const char *m3, const char *m4) {
    uint8_t utf8m1[2000], utf8m2[2000], utf8m3[2000], utf8m4[2000];

    if(m1 == NULL) {
      stringToUtf8("No error message available!", utf8m1);
      printf("\n%s\n", utf8m1);
    }
    else if(m2 == NULL) {
      stringToUtf8(m1, utf8m1);
      printf("\n%s\n\n", utf8m1);
    }
    else if(m3 == NULL) {
      stringToUtf8(m1, utf8m1);
      stringToUtf8(m2, utf8m2);
      printf("\n%s\n%s\n\n", utf8m1, utf8m2);
    }
    else if(m4 == NULL) {
      stringToUtf8(m1, utf8m1);
      stringToUtf8(m2, utf8m2);
      stringToUtf8(m3, utf8m3);
      printf("\n%s\n%s\n%s\n\n", utf8m1, utf8m2, utf8m3);
    }
    else {
      stringToUtf8(m1, utf8m1);
      stringToUtf8(m2, utf8m2);
      stringToUtf8(m3, utf8m3);
      stringToUtf8(m4, utf8m4);
      printf("\n%s\n%s\n%s\n%s\n\n", utf8m1, utf8m2, utf8m3, utf8m4);
    }
  }
#endif // PC_BUILD



void fnRaiseError(uint16_t errorCode) {
  displayCalcErrorMessage((uint8_t)errorCode, ERR_REGISTER_LINE, REGISTER_X);
}



void displayCalcErrorMessage(uint8_t errorCode, calcRegister_t errMessageRegisterLine, calcRegister_t errRegisterLine) {
  if(errorCode >= NUMBER_OF_ERROR_CODES || errorCode == 0) {
    sprintf(errorMessage, "In function displayCalcErrorMessage: %d is an unexpected value for errorCode!", errorCode);
    displayBugScreen(errorMessage);
  }

  else if(errMessageRegisterLine > REGISTER_T || errMessageRegisterLine < REGISTER_X) {
    sprintf(errorMessage, "In function displayCalcErrorMessage: %d is an unexpected value for errMessageRegisterLine! Must be from 100 (register X) to 103 (register T)", errMessageRegisterLine);
    displayBugScreen(errorMessage);
  }

  else if(errRegisterLine > REGISTER_T || errRegisterLine < REGISTER_X) {
    sprintf(errorMessage, "In function displayCalcErrorMessage: %d is an unexpected value for errRegisterLine! Must be from 100 (register X) to 103 (register T)", errRegisterLine);
    displayBugScreen(errorMessage);
  }

  else {
    lastErrorCode            = errorCode;
    errorMessageRegisterLine = errMessageRegisterLine;
  }
}



#ifndef TESTSUITE_BUILD
  void nextWord(const char *str, int16_t *pos, char *word) {
    int16_t i = 0;

    while(str[*pos] != 0 && str[*pos] != ' ') {
      word[i++] = str[(*pos)++];
    }

    word[i] = 0;

    while(str[*pos] == ' ') {
      (*pos)++;
    }
  }



  /********************************************//**
   * \brief Displays an error message.
   *        When this happens it's likely a bug!
   *
   * \param[in] message char* The message
   * \return void
   ***********************************************/
  void displayBugScreen(const char *msg) {
    if(calcMode != CM_BUG_ON_SCREEN) {
      int16_t y, pos;
      char line[100], word[50], message[1000];
      bool_t firstWordOfLine;

      previousCalcMode = calcMode;
      calcMode = CM_BUG_ON_SCREEN;
      clearSystemFlag(FLAG_ALPHA);
      hideCursor();
      cursorEnabled = false;

      lcd_fill_rect(0, 20, SCREEN_WIDTH, 220, LCD_SET_VALUE);

      y = 20;
      showString("This is most likely a bug in the firmware!", &standardFont, 1, y, vmNormal, true, false);
      y += 20;

      strcpy(message, msg);
      strcat(message, " Try to reproduce this and report a bug. Press EXIT to leave." );

      pos = 0;
      line[0] = 0;
      firstWordOfLine = true;

      nextWord(message, &pos, word);
      while(word[0] != 0) {
        if(stringWidth(line, &standardFont, true, true) + (firstWordOfLine ? 0 : 4) + stringWidth(word, &standardFont, true, true) >= SCREEN_WIDTH) { // 4 is the width of STD_SPACE_PUNCTUATION
          showString(line, &standardFont, 1, y, vmNormal, true, false);
          y += 20;
          line[0] = 0;
          firstWordOfLine = true;
        }

        if(firstWordOfLine) {
          strcpy(line, word);
          firstWordOfLine = false;
        }
        else {
          strcat(line, STD_SPACE_PUNCTUATION);
          strcat(line, word);
        }

        nextWord(message, &pos, word);
      }

      if(line[0] != 0) {
        showString(line, &standardFont, 1, y, vmNormal, true, false);
      }
    }
  }
#endif // !TESTSUITE_BUILD



/********************************************//**
 * \brief Data type error, common function
 *
 * \param void
 * \return void
 ***********************************************/
#if (EXTRA_INFO_ON_CALC_ERROR != 1)
  void typeError(void) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  }
#endif // (EXTRA_INFO_ON_CALC_ERROR != 1)
