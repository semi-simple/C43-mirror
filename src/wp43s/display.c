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
 * \file display.c
 ***********************************************/

#include "display.h"

#include "c43Extensions/addons.h"
#include "charString.h"
#include "constantPointers.h"
#include "conversionAngles.h"
#include "dateTime.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "fractions.h"
#include "items.h"
#include "c43Extensions/jm.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/toPolar.h"
#include "c43Extensions/radioButtonCatalog.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include <string.h>

#include "wp43s.h"



/********************************************//**
 * \brief Sets the display format FIX and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatFix(uint16_t displayFormatN) {
  displayFormat = DF_FIX;
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
  SigFigMode = 0;                                                //JM SIGFIG Reset SIGFIG 
  UNITDisplay = false;                                           //JM UNIT display Reset
  if(getRegisterDataType(REGISTER_X) == dtTime || getRegisterDataType(REGISTER_Y) == dtTime || getRegisterDataType(REGISTER_Z) == dtTime || getRegisterDataType(REGISTER_T) == dtTime) {
    fnDisplayFormatTime(displayFormatN);
  }

  fnRefreshState();                              //drJM
}



/********************************************//**
 * \brief Sets the display format SCI and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatSci(uint16_t displayFormatN) {
  displayFormat = DF_SCI;
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
  SigFigMode = 0;                                                //JM SIGFIG Reset SIGFIG 
  UNITDisplay = false;                                           //JM UNIT display Reset

  fnRefreshState();                              //drJM
}



/********************************************//**
 * \brief Sets the display format ENG and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatEng(uint16_t displayFormatN) {
  displayFormat = DF_ENG;
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
  SigFigMode = 0;                                                //JM SIGFIG Reset SIGFIG 
  UNITDisplay = false;                                           //JM UNIT display Reset

  fnRefreshState();                              //drJM
}



/********************************************//**
 * \brief Sets the display format ALL and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatAll(uint16_t displayFormatN) {
  //if(0 <= displayFormatN && displayFormatN <= 15) {
  displayFormat = DF_ALL;
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
  SigFigMode = 0;                                                //JM SIGFIG Reset SIGFIG
  UNITDisplay = false;                                           //JM UNIT display Reset

  fnRefreshState();                              //drJM
}



/********************************************//**
 * \brief Sets the number of digits afer the period and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatDsp(uint16_t displayFormatN) {
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
}



/********************************************//**
 * \brief Sets the GAP in number represemtation and refreshes the stack
 *
 * \param[in] gap uint16_t
 * \return void
 ***********************************************/
void fnDisplayFormatGap(uint16_t gap) {
 if(gap == 1 || gap == 2) {
   gap = 0;
 }
  groupingGap = gap;
}


/********************************************//**
 * \brief Sets the display format for time and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatTime(uint16_t displayFormatN) {
  timeDisplayFormatDigits = (uint8_t)displayFormatN;
}



/********************************************//**
 * \brief Adds the power of 10 using numeric font to displayString
 *
 * \param[out] displayString char*     Result string
 * \param[in]  exponent int32_t Power of 10 to format
 * \return void
 ***********************************************/
void exponentToDisplayString(int32_t exponent, char *displayString, char *displayValueString, bool_t nimMode, const char *separator) {
  strcpy(displayString, PRODUCT_SIGN);
  displayString += 2;
  strcpy(displayString, STD_SUB_10);
  displayString += 2;
  displayString[0] = 0;

  if(displayValueString != NULL) {
    *displayValueString++ = 'e';
    *displayValueString = 0;
  }

  if(nimMode) {
    if(exponent != 0) {
      supNumberToDisplayString(exponent, displayString, displayValueString, false, separator);
    }
  }
  else {
    supNumberToDisplayString(exponent, displayString, displayValueString, false, separator);
  }
}



void supNumberToDisplayString(int32_t supNumber, char *displayString, char *displayValueString, bool_t insertGap, const char *separator) {
  if(displayValueString != NULL) {
    sprintf(displayValueString, "%" PRId32, supNumber);
  }

  if(supNumber == 0) {
    strcat(displayString, STD_SUP_0);
  }
  else {
    int16_t digitCount=0;
    bool_t greaterThan9999;

    if(supNumber < 0) {
      supNumber = -supNumber;
      strcat(displayString, STD_SUP_MINUS);
      displayString += 2;
    }

    greaterThan9999 = (supNumber > 9999);
    while(supNumber > 0) {
      int16_t digit;

      digit = supNumber % 10;
      supNumber /= 10;

      xcopy(displayString + 2, displayString, stringByteLength(displayString) + 1);

      if(digit <= 1) {
        displayString[0] = STD_SUP_0[0];
        displayString[1] = STD_SUP_0[1];
        displayString[1] += digit;
      }
      else if(digit <= 3) {
        displayString[0] = STD_SUP_2[0];
        displayString[1] = STD_SUP_2[1];
        displayString[1] += digit-2;
      }
      else {
        displayString[0] = STD_SUP_4[0];
        displayString[1] = STD_SUP_4[1];
        displayString[1] += digit-4;
      }

      if(insertGap && greaterThan9999 && supNumber > 0 && groupingGap != 0 && ((++digitCount) % groupingGap) == 0) {
        xcopy(displayString + 2, displayString, stringByteLength(displayString) + 1);
        displayString[0] = separator[0];
        displayString[1] = separator[1];
      }
    }
  }

  strcat(displayString, STD_SPACE_HAIR);
}



void subNumberToDisplayString(int32_t subNumber, char *displayString, char *displayValueString) {
  if(displayValueString != NULL) {
    sprintf(displayValueString, "%" PRId32, subNumber);
  }

  if(subNumber < 0) {
    subNumber = -subNumber;
    strcat(displayString, STD_SUB_MINUS);
    displayString += 2;
  }

  if(subNumber == 0) {
    strcat(displayString, STD_SUB_0);
  }
  else {
    while(subNumber > 0) {
      int16_t digit = subNumber % 10;
      subNumber /= 10;

      xcopy(displayString + 2, displayString, stringByteLength(displayString) + 1);

      displayString[0] = STD_SUB_0[0];
      displayString[1] = STD_SUB_0[1];
      displayString[1] += digit;
    }
  }
}



void real34ToDisplayString(const real34_t *real34, uint32_t tag, char *displayString, const font_t *font, int16_t maxWidth, int16_t displayHasNDigits, bool_t limitExponent, const char *separator, bool_t frontSpace) {
  uint8_t savedDisplayFormatDigits = displayFormatDigits;

  //maxWidth = largeur; // For the real34 width test

  if(updateDisplayValueX) {
    displayValueX[0] = 0;
  }

  if(tag == amNone) {
    real34ToDisplayString2(real34, displayString, displayHasNDigits, limitExponent, separator, false, frontSpace);
  }
  else {
    angle34ToDisplayString2(real34, tag, displayString, displayHasNDigits, limitExponent, separator, frontSpace);
  }

  while(stringWidth(displayString, font, true, true) > maxWidth) {
    if(displayFormat == DF_ALL) {
      if(displayHasNDigits == 2) {
        break;
      }
      displayHasNDigits--;
    }
    else {
      if(displayFormatDigits == 0) {
        break;
      }
      displayFormatDigits--;
    }

    if(updateDisplayValueX) {
      displayValueX[0] = 0;
    }

    if(tag == amNone) {
      real34ToDisplayString2(real34, displayString, displayHasNDigits, limitExponent, separator, false, frontSpace);
    }
    else {
      angle34ToDisplayString2(real34, tag, displayString, displayHasNDigits, limitExponent, separator, frontSpace);
    }
  }
  displayFormatDigits = savedDisplayFormatDigits;
}



/********************************************//**
 * \brief Formats a real
 *
 * \param[out] displayString char* Result string
 * \param[in]  x const real34_t*  Value to format
 * \return void
 ***********************************************/
void real34ToDisplayString2(const real34_t *real34, char *displayString, int16_t displayHasNDigits, bool_t limitExponent, const char *separator, bool_t noFix, bool_t frontSpace) {
  #undef MAX_DIGITS
  #define MAX_DIGITS 37 // 34 + 1 before (used when rounding from 9.999 to 10.000) + 2 after (used for rounding and ENG display mode)

  uint8_t charIndex, valueIndex, digitToRound=0;
  uint8_t *bcd;
  int16_t digitsToDisplay=0, numDigits, digitPointer, firstDigit, lastDigit, i, digitCount, digitsToTruncate, exponent;
  int32_t sign;
  bool_t  ovrSCI=false, ovrENG=false, firstDigitAfterPeriod=true;
  real34_t value34;
  real_t value;

  real34ToReal(real34, &value);
  ctxtReal39.digits =  (displayFormat == DF_FIX ? 24 : displayHasNDigits); // This line is for FIX n displaying more than 16 digits. e.g. in FIX 15: 123 456.789 123 456 789 123 
  //ctxtReal39.digits =  displayHasNDigits; // This line is for fixed number of displayed digits, e.g. in FIX 15: 123 456.789 123 456 8
  realPlus(&value, &value, &ctxtReal39);
  ctxtReal39.digits = 39;
  realToReal34(&value, &value34);
  if(real34IsNegative(real34)) {
    real34SetNegativeSign(&value34);
  }

  bcd = (uint8_t *)(tmpString + 256 - MAX_DIGITS);
  memset(bcd, 0, MAX_DIGITS);

  sign = real34GetCoefficient(&value34, bcd + 1);
  exponent = real34GetExponent(&value34);

  // Calculate the number of significant digits
  for(digitPointer=1; digitPointer<=MAX_DIGITS-3; digitPointer++) {
    if(bcd[digitPointer] != 0) {
      break;
    }
  }

  if(digitPointer >= MAX_DIGITS-2) { // *real = 0.0
    firstDigit = 0;
    lastDigit  = 0;
    numDigits  = 1;
    exponent   = 0;
  }
  else {
    firstDigit = digitPointer;

    for(digitPointer=MAX_DIGITS-3; digitPointer>=1; digitPointer--) {
      if(bcd[digitPointer] == 0) {
        exponent++;
      }
      else {
        break;
      }
    }
    lastDigit = digitPointer;

    numDigits = lastDigit - firstDigit;
    exponent += numDigits++;
  }

/*  if(limitExponent && abs(exponent) > exponentLimit) {
    if(exponent > exponentLimit) {
      if(real34IsPositive(&value34)) {
        realToReal34(const_plusInfinity, &value34);
      }
      else {
        realToReal34(const_minusInfinity, &value34);
      }
    }
    else if(exponent < -exponentLimit) {
      real34Zero(&value34);

      bcd = (uint8_t *)(tmpString + 256 - MAX_DIGITS);
      memset(bcd, 0, MAX_DIGITS);

      sign = 0;
      exponent = 0;
      firstDigit = 0;
      lastDigit  = 0;
      numDigits  = 1;
      exponent   = 0;
    }
  }*/

  if(limitExponent && abs(exponent) > exponentLimit) {
    if(exponent > exponentLimit) {
      if(real34IsPositive(&value34)) {
        if(frontSpace) {
          strcpy(displayString, " " STD_LEFT_SINGLE_QUOTE STD_INFINITY STD_RIGHT_SINGLE_QUOTE);
        }
        else {
          strcpy(displayString, STD_LEFT_SINGLE_QUOTE STD_INFINITY STD_RIGHT_SINGLE_QUOTE);
        }
        if(updateDisplayValueX) {
          strcpy(displayValueX + strlen(displayValueX), "9e9999");
        }
      }
      else {
        strcpy(displayString, "-" STD_LEFT_SINGLE_QUOTE STD_INFINITY STD_RIGHT_SINGLE_QUOTE);
        if(updateDisplayValueX) {
          strcpy(displayValueX + strlen(displayValueX), "-9e9999");
        }
      }
      return;
    }
    else if(exponent < -exponentLimit) {
      if(real34IsPositive(&value34)) {
        strcpy(displayString, STD_LEFT_SINGLE_QUOTE "0." STD_RIGHT_SINGLE_QUOTE);
        if(updateDisplayValueX) {
          strcpy(displayValueX + strlen(displayValueX), "0");
        }
      }
      else {
        strcpy(displayString, STD_LEFT_SINGLE_QUOTE "0." STD_RIGHT_SINGLE_QUOTE);
        if(updateDisplayValueX) {
          strcpy(displayValueX + strlen(displayValueX), "0");
        }
      }
      return;
    }
  }

  if(real34IsInfinite(&value34)) {
    if(real34IsNegative(&value34)) {
      strcpy(displayString, "-" STD_INFINITY);
      if(updateDisplayValueX) {
        strcpy(displayValueX + strlen(displayValueX), "-9e9999");
      }
    }
    else {
      strcpy(displayString, " " STD_INFINITY);
      if(updateDisplayValueX) {
        strcpy(displayValueX + strlen(displayValueX), "9e9999");
      }
    }
    return;
  }

  if(real34IsNaN(&value34)) {
    real34ToString(&value34, displayString);
    if(updateDisplayValueX) {
      real34ToString(&value34, displayValueX + strlen(displayValueX));
    }
    return;
  }

  charIndex = 0;
  valueIndex = (updateDisplayValueX ? strlen(displayValueX) : 0);

  //////////////
  // ALL mode //
  //////////////
  if(displayFormat == DF_ALL) {
    if(noFix || exponent >= displayHasNDigits || (displayFormatDigits != 0 && exponent < -(int32_t)displayFormatDigits) || (displayFormatDigits == 0 && exponent < numDigits - displayHasNDigits)) { // Display in SCI or ENG format
      digitsToDisplay = numDigits - 1;
      digitToRound    = firstDigit + digitsToDisplay;
      ovrSCI = !getSystemFlag(FLAG_ALLENG);
      ovrENG = getSystemFlag(FLAG_ALLENG);
    }
    else { // display all digits without ten exponent factor
      // Number of digits to truncate
      digitsToTruncate = max(numDigits - exponent, displayHasNDigits) - displayHasNDigits;
      numDigits -= digitsToTruncate;
      lastDigit -= digitsToTruncate;

      // Round the displayed number
      if(bcd[lastDigit+1] >= 5) {
        bcd[lastDigit]++;
      }

      // Transfert the carry
      while(bcd[lastDigit] == 10) {
        bcd[lastDigit--] = 0;
        numDigits--;
        bcd[lastDigit]++;
      }

      // Case when 9.9999 rounds to 10.0000
      if(lastDigit < firstDigit) {
        firstDigit--;
        lastDigit = firstDigit;
        numDigits = 1;
        exponent++;
      }

      // The sign
      if(sign) {
        displayString[charIndex++] = '-';
        if(updateDisplayValueX) {
          displayValueX[valueIndex++] = '-';
        }
      }
      else {
        if(frontSpace) {
          displayString[charIndex++] = ' ';
        }
      }

      if(exponent < 0) { // negative exponent
        // first 0 and radix mark
        displayString[charIndex++] = '0';
        if(updateDisplayValueX) {
          displayValueX[valueIndex++] = '0';
        }
        displayString[charIndex] = 0;
        strcat(displayString, RADIX34_MARK_STRING);
        charIndex++;
        if(updateDisplayValueX) {
          displayValueX[valueIndex++] = '.';
        }

        // Zeros before first significant digit
        for(digitCount=0, i=exponent+1; i<0; i++, digitCount--) {
          if(digitCount != 0 && groupingGap != 0 && digitCount%(uint16_t)groupingGap == 0) {
            xcopy(displayString + charIndex, separator, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0';
          if(updateDisplayValueX) {
            displayValueX[valueIndex++] = '0';
          }
        }

        // Significant digits
        for(digitPointer=firstDigit; digitPointer<firstDigit+min(displayHasNDigits - 1 - exponent, numDigits); digitPointer++, digitCount--) {
          if(digitCount != 0 && groupingGap != 0 && digitCount%(uint16_t)groupingGap == 0) {
            xcopy(displayString + charIndex, separator, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0' + bcd[digitPointer];
          if(updateDisplayValueX) {
            displayValueX[valueIndex++] = '0' + bcd[digitPointer];
          }
        }
      }
      else { // zero or positive exponent
        for(digitCount=exponent, digitPointer=firstDigit; digitPointer<=lastDigit + max(exponent - numDigits + 1, 0); digitPointer++, digitCount--) {
          if(digitCount != -1 && digitCount != exponent && groupingGap != 0 && modulo(digitCount, (uint16_t)groupingGap) == (uint16_t)groupingGap - 1) {
            xcopy(displayString + charIndex, separator, 2);
            charIndex += 2;
          }

          // Significant digit or zero
          if(digitPointer <= lastDigit) {
            displayString[charIndex++] = '0' + bcd[digitPointer];
            if(updateDisplayValueX) {
              displayValueX[valueIndex++] = '0' + bcd[digitPointer];
            }
          }
          else {
            displayString[charIndex++] = '0';
            if(updateDisplayValueX) {
              displayValueX[valueIndex++] = '0';
            }
          }

          // Radix mark
          if(digitCount == 0) {
            displayString[charIndex] = 0;
            strcat(displayString, RADIX34_MARK_STRING);
            charIndex++;
            if(updateDisplayValueX) {
              displayValueX[valueIndex++] = '.';
            }
          }
        }
      }

      displayString[charIndex] = 0;
      if(updateDisplayValueX) {
        displayValueX[valueIndex] = 0;
      }
      return;
    }
  }

  //////////////
  // FIX mode //
  //////////////
  if(displayFormat == DF_FIX) {
    if(noFix || exponent >= displayHasNDigits || exponent < -(int32_t)displayFormatDigits) { // Display in SCI or ENG format
      digitsToDisplay = displayFormatDigits;
      digitToRound    = min(firstDigit + digitsToDisplay, lastDigit);
      ovrSCI = !getSystemFlag(FLAG_ALLENG);
      ovrENG = getSystemFlag(FLAG_ALLENG);
    }
    else { // display fix number of digits without ten exponent factor
      // Number of digits to truncate

      int displayFormatDigits_active;                                    //JM SIGFIGNEW vv
      if(SigFigMode >= 1) {
        displayFormatDigits_active =  max((SigFigMode+1)-exponent-1,0);  //Convert SIG to FIX.
      } else {
        displayFormatDigits_active = displayFormatDigits;
      }                                                                  //JM SIGFIGNEW ^^

      digitsToTruncate = max(numDigits - (int16_t)displayFormatDigits_active - exponent - 1, 0);   //JM SIGFIGNEW hackpoint
      numDigits -= digitsToTruncate;
      lastDigit -= digitsToTruncate;

      if(SigFigMode >= 1) {                                              //JM SIGFIGNEW vv
        digitToRound = firstDigit + SigFigMode;
      } else {
        digitToRound = lastDigit;
      }                                                                  //JM SIGFIGNEW ^^

      // Round the displayed number
      if(bcd[digitToRound+1] >= 5) {
        bcd[digitToRound]++;
      }

      // Transfert the carry
      while(bcd[digitToRound] == 10) {
        bcd[digitToRound--] = 0;
        numDigits--;
        bcd[digitToRound]++;
      }

      // Case when 9.9999 rounds to 10.0000
      if(digitToRound < firstDigit) {
        firstDigit--;
        lastDigit = firstDigit;
        numDigits = 1;
        exponent++;
      }

      //JM SIGFIG - blank out non-sig digits to the right                 //JM SIGFIGNEW vv
      if(SigFigMode>=1) {
        if((SigFigMode+1)-exponent-1 < 0) {
           for (digitCount = firstDigit + (SigFigMode+1); digitCount <= lastDigit; digitCount++) {
            bcd[digitCount] = 0;
            }
        }
      }                                                                   //JM SIGFIG

      // The sign
      if(sign) {
        displayString[charIndex++] = '-';
        if(updateDisplayValueX) {
          displayValueX[valueIndex++] = '-';
        }
      }
      else {
        if(frontSpace) {
          displayString[charIndex++] = ' ';
        }
      }

      if(exponent < 0) { // negative exponent
        // first 0 and radix mark
        displayString[charIndex++] = '0';
        if(updateDisplayValueX) {
          displayValueX[valueIndex++] = '0';
        }
        displayString[charIndex] = 0;
        strcat(displayString, RADIX34_MARK_STRING);
        charIndex++;
        if(updateDisplayValueX) {
          displayValueX[valueIndex++] = '.';
        }

        // Zeros before first significant digit
        for(digitCount=0, i=exponent+1; i<0; i++, digitCount--) {
          if(digitCount!=0 && groupingGap!=0 && digitCount%(uint16_t)groupingGap==0) {
            xcopy(displayString + charIndex, separator, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0';
          if(updateDisplayValueX) {
            displayValueX[valueIndex++] = '0';
          }
        }

        // Significant digits
        for(digitPointer=firstDigit; digitPointer<=lastDigit; digitPointer++, digitCount--) {
          if(digitCount!=0 && groupingGap!=0 && digitCount%(uint16_t)groupingGap==0) {
            xcopy(displayString + charIndex, separator, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0' + bcd[digitPointer];
          if(updateDisplayValueX) {
            displayValueX[valueIndex++] = '0' + bcd[digitPointer];
          }
        }

        // Zeros after last significant digit
        for(i=1; i<=(int16_t)displayFormatDigits_active+exponent+1-numDigits; i++, digitCount--) {   //JM SIGFIGNEW hackpoint
          if(groupingGap!=0 && digitCount%(uint16_t)groupingGap==0) {
            xcopy(displayString + charIndex, separator, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0';
          if(updateDisplayValueX) {
            displayValueX[valueIndex++] = '0';
          }
        }
      }
      else { // zero or positive exponent
  //JM SIGFIGNEW hackpoint
        for(digitCount=exponent, digitPointer=firstDigit; digitPointer<=firstDigit + exponent + (int16_t)displayFormatDigits_active; digitPointer++, digitCount--) { // This line is for FIX n displaying more than 16 digits. e.g. in FIX 15: 123 456.789 123 456 789 123 
        //for(digitCount=exponent, digitPointer=firstDigit; digitPointer<=firstDigit + min(exponent + (int16_t)displayFormatDigits, 15); digitPointer++, digitCount--) { // This line is for fixed number of displayed digits, e.g. in FIX 15: 123 456.789 123 456 8
          if(digitCount!=-1 && digitCount!=exponent && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap) == (uint16_t)groupingGap - 1) {
            xcopy(displayString + charIndex, separator, 2);
            charIndex += 2;
          }

          // Significant digit or zero
          if(digitPointer <= lastDigit) {
            displayString[charIndex++] = '0' + bcd[digitPointer];
            if(updateDisplayValueX) {
              displayValueX[valueIndex++] = '0' + bcd[digitPointer];
            }
          }
          else {
            displayString[charIndex++] = '0';
            if(updateDisplayValueX) {
              displayValueX[valueIndex++] = '0';
            }
          }

          // Radix mark
          if(digitCount == 0) {
            displayString[charIndex] = 0;
            strcat(displayString, RADIX34_MARK_STRING);
            charIndex++;
            if(updateDisplayValueX) {
              displayValueX[valueIndex++] = '.';
            }
          }
        }
      }

      displayString[charIndex] = 0;
      if(updateDisplayValueX) {
        displayValueX[valueIndex] = 0;
      }
      return;
    }
  }

  //////////////
  // SCI mode //
  //////////////
  if(ovrSCI  || displayFormat == DF_SCI) {
    // Round the displayed number
    if(!ovrSCI) {
      digitsToDisplay = displayFormatDigits;
      digitToRound    = min(firstDigit + (int16_t)displayFormatDigits, lastDigit);
    }

    if(bcd[digitToRound + 1] >= 5) {
      bcd[digitToRound]++;
    }

    // Transfert the carry
    while(bcd[digitToRound] == 10) {
      bcd[digitToRound--] = 0;
      numDigits--;
      bcd[digitToRound]++;
    }

    // Case when 9.9999 rounds to 10.0000
    if(digitToRound < firstDigit) {
      firstDigit--;
      numDigits = 1;
      exponent++;
    }

    // Sign
    if(sign) {
      displayString[charIndex++] = '-';
      if(updateDisplayValueX) {
        displayValueX[valueIndex++] = '-';
      }
    }
    else {
      if(frontSpace) {
        displayString[charIndex++] = ' ';
      }
    }

    // First digit
    displayString[charIndex++] = '0' + bcd[firstDigit];
    if(updateDisplayValueX) {
      displayValueX[valueIndex++] = '0' + bcd[firstDigit];
    }

    // Radix mark
    displayString[charIndex] = 0;
    strcat(displayString, RADIX34_MARK_STRING);
    charIndex++;
    if(updateDisplayValueX) {
      displayValueX[valueIndex++] = '.';
    }

    // Significant digits
    for(digitCount=-1, digitPointer=firstDigit+1; digitPointer<firstDigit+min(numDigits, digitsToDisplay+1); digitPointer++, digitCount--) {
      if(!firstDigitAfterPeriod && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap) == (uint16_t)groupingGap - 1) {
        xcopy(displayString + charIndex, separator, 2);
        charIndex += 2;
      }
      else {
        firstDigitAfterPeriod = false;
      }

      displayString[charIndex++] = '0' + bcd[digitPointer];
      if(updateDisplayValueX) {
        displayValueX[valueIndex++] = '0' + bcd[digitPointer];
      }
    }

    // The ending zeros
    for(digitPointer=0; digitPointer<=digitsToDisplay-numDigits; digitPointer++, digitCount--) {
      if(!firstDigitAfterPeriod && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap) == (uint16_t)groupingGap - 1) {
        xcopy(displayString + charIndex, separator, 2);
        charIndex += 2;
      }
      else {
        firstDigitAfterPeriod = false;
      }

      displayString[charIndex++] = '0';
      if(updateDisplayValueX) {
        displayValueX[valueIndex++] = '0';
      }
    }

    displayString[charIndex] = 0;
    if(updateDisplayValueX) {
      displayValueX[valueIndex] = 0;
    }

    if(exponent != 0) {
      if(updateDisplayValueX) {
        exponentToDisplayString(exponent, displayString + charIndex, displayValueX + valueIndex, false, separator);
      }
      else {
        exponentToDisplayString(exponent, displayString + charIndex, NULL,                       false, separator);
      }
    }
    return;
  }

  //////////////
  // ENG mode //
  //////////////
  if(ovrENG || displayFormat == DF_ENG) {
    // Round the displayed number
    if(!ovrENG) {
      digitsToDisplay = displayFormatDigits;
      digitToRound    = min(firstDigit + digitsToDisplay, lastDigit);
    }

    if(bcd[digitToRound + 1] >= 5) {
      bcd[digitToRound]++;
    }

    bcd[digitToRound + 1] = 0;
    bcd[digitToRound + 2] = 0;

    // Transfert the carry
    while(bcd[digitToRound] == 10) {
      bcd[digitToRound--] = 0;
      numDigits--;
      bcd[digitToRound]++;
    }

    // Case when 9.9999 rounds to 10.0000
    if(digitToRound < firstDigit) {
      firstDigit--;
      numDigits = 1;
      exponent++;
    }

    // The sign
    if(sign) {
      displayString[charIndex++] = '-';
      if(updateDisplayValueX) {
        displayValueX[valueIndex++] = '-';
      }
    }
    else {
      if(frontSpace){
        displayString[charIndex++] = ' ';
      }
    }

    // Digits before radix mark
    displayString[charIndex++] = '0' + bcd[firstDigit];
    if(updateDisplayValueX) {
      displayValueX[valueIndex++] = '0' + bcd[firstDigit];
    }
    firstDigit++;
    numDigits--;
    digitsToDisplay--;
    while(modulo(exponent, 3) != 0) {
      exponent--;
      displayString[charIndex++] = '0' + bcd[firstDigit];
      if(updateDisplayValueX) {
        displayValueX[valueIndex++] = '0' + bcd[firstDigit];
      }
      firstDigit++;
      numDigits--;
      digitsToDisplay--;
    }

    // Radix Mark
    displayString[charIndex] = 0;
    strcat(displayString, RADIX34_MARK_STRING);
    charIndex++;
    if(updateDisplayValueX) {
      displayValueX[valueIndex++] = '.';
    }

    // Digits after radix mark
    for(digitCount=-1, digitPointer=firstDigit; digitPointer<firstDigit+min(numDigits, digitsToDisplay+1); digitPointer++, digitCount--) {
      if(!firstDigitAfterPeriod && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap) == (uint16_t)groupingGap - 1) {
        xcopy(displayString + charIndex, separator, 2);
        charIndex += 2;
      }
      else {
        firstDigitAfterPeriod = false;
      }

      displayString[charIndex++] = '0' + bcd[digitPointer];
      if(updateDisplayValueX) {
        displayValueX[valueIndex++] = '0' + bcd[digitPointer];
      }
    }

    // The ending zeros
    for(digitPointer=0; digitPointer<=digitsToDisplay-max(0, numDigits); digitPointer++, digitCount--) {
      if(!firstDigitAfterPeriod && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap) == (uint16_t)groupingGap - 1) {
        xcopy(displayString + charIndex, separator, 2);
        charIndex += 2;
      }
      else {
        firstDigitAfterPeriod = false;
      }

      displayString[charIndex++] = '0';
      if(updateDisplayValueX) {
        displayValueX[valueIndex++] = '0';
      }
    }

    displayString[charIndex] = 0;
    if(updateDisplayValueX) {
      displayValueX[valueIndex] = 0;
    }

    if(exponent != 0) {
      if(UNITDisplay == false) {                                                        //JM UNIT
        if(updateDisplayValueX) {
          exponentToDisplayString(exponent, displayString + charIndex, displayValueX + valueIndex, false, separator);
        }
        else {
          exponentToDisplayString(exponent, displayString + charIndex, NULL,                       false, separator);
        }
      }                                                                                 //JM UNIT
      else {                                                                            //JM UNIT
        exponentToUnitDisplayString(exponent, displayString + charIndex, displayValueX + valueIndex, false, separator);          //JM UNIT
      }                                                                                 //JM UNIT
    }
  }
}



void complex34ToDisplayString(const complex34_t *complex34, char *displayString, const font_t *font, int16_t maxWidth, int16_t displayHasNDigits, bool_t limitExponent, const char *separator, bool_t frontSpace) {
  uint8_t savedDisplayFormatDigits = displayFormatDigits;

  if(updateDisplayValueX) {
    displayValueX[0] = 0;
  }

  complex34ToDisplayString2(complex34, displayString, displayHasNDigits, limitExponent, separator, frontSpace);
  while(stringWidth(displayString, font, true, true) > maxWidth) {
    if(displayFormat == DF_ALL) {
      if(displayHasNDigits == 2) {
        break;
      }
      displayHasNDigits--;
    }
    else {
      if(displayFormatDigits == 0) {
        break;
      }
      displayFormatDigits--;
    }

    if(updateDisplayValueX) {
      displayValueX[0] = 0;
    }

    complex34ToDisplayString2(complex34, displayString, displayHasNDigits, limitExponent, separator, frontSpace);
  }
  displayFormatDigits = savedDisplayFormatDigits;
}



void complex34ToDisplayString2(const complex34_t *complex34, char *displayString, int16_t displayHasNDigits, bool_t limitExponent, const char *separator, bool_t frontSpace) {
  int16_t i=100;
  real34_t real34, imag34;
  real_t real, imagIc;

  if(getSystemFlag(FLAG_POLAR)) { // polar mode
    real34ToReal(VARIABLE_REAL34_DATA(complex34), &real);
    real34ToReal(VARIABLE_IMAG34_DATA(complex34), &imagIc);
    realRectangularToPolar(&real, &imagIc, &real, &imagIc, &ctxtReal39); // imagIc in radian
    convertAngleFromTo(&imagIc, amRadian, currentAngularMode, &ctxtReal39);
    realToReal34(&real, &real34);
    realToReal34(&imagIc, &imag34);
  }
  else { // rectangular mode
    real34Copy(VARIABLE_REAL34_DATA(complex34), &real34);
    real34Copy(VARIABLE_IMAG34_DATA(complex34), &imag34);
  }

  real34ToDisplayString2(&real34, displayString, displayHasNDigits, limitExponent, separator, false, frontSpace);

  if(updateDisplayValueX) {
    if(getSystemFlag(FLAG_POLAR)) {
      strcat(displayValueX, "j");
    }
    else {
      strcat(displayValueX, "i");
    }
  }

  real34ToDisplayString2(&imag34, displayString + i, displayHasNDigits, limitExponent, separator, false, false);

  if(getSystemFlag(FLAG_POLAR)) { // polar mode
    strcat(displayString, STD_SPACE_4_PER_EM STD_MEASURED_ANGLE STD_SPACE_4_PER_EM);
    angle34ToDisplayString2(&imag34, currentAngularMode, displayString + stringByteLength(displayString), displayHasNDigits, limitExponent, separator, false);
  }
  else { // rectangular mode
    if(strncmp(displayString + stringByteLength(displayString) - 2, STD_SPACE_HAIR, 2) != 0) {
      strcat(displayString, STD_SPACE_HAIR);
    }

    if(displayString[i] == '-') {
      strcat(displayString, "-");
      i++;
    }
    else {
      strcat(displayString, "+");
    }

    strcat(displayString, COMPLEX_UNIT);
    strcat(displayString, PRODUCT_SIGN);
    xcopy(strchr(displayString, '\0'), displayString + i, strlen(displayString + i) + 1);
  }
}



/********************************************//**
 * \brief formats a fraction
 *
 * \param regist calcRegister_t
 * \param displayString char*
 * \return void
 ***********************************************/
void fractionToDisplayString(calcRegister_t regist, char *displayString) {
  int16_t  sign, lessEqualGreater;
  uint64_t intPart, numer, denom;
  int16_t  u, insertAt, endingZero, gap;

  //printf("regist = "); printRegisterToConsole(regist); printf("\n");
  fraction(regist, &sign, &intPart, &numer, &denom, &lessEqualGreater);
  //printf("result of fraction(...) = %c%" PRIu64 " %" PRIu64 "/%" PRIu64 "\n", sign==-1 ? '-' : ' ', intPart, numer, denom);

  if(getSystemFlag(FLAG_PROPFR)) { // a b/c
    if(updateDisplayValueX) {
      sprintf(displayValueX, "%s%" PRIu64 " %" PRIu64 "/%" PRIu64, (sign == -1 ? "-" : ""), intPart, numer, denom);
    }

    if(sign == -1) {
      displayString[0] = '-';
      displayString[1] = 0;
      endingZero = 1;
    }
    else {
      displayString[0] = 0;
      endingZero = 0;

      if(updateDisplayValueX) {
      }
    }

    // Integer part
    insertAt = endingZero;
    gap = -1;
    do {
      gap++;
      if(gap == groupingGap) {
        gap = 0;
        endingZero++;
        xcopy(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);
        displayString[insertAt]     = STD_SPACE_PUNCTUATION[0];
        displayString[insertAt + 1] = STD_SPACE_PUNCTUATION[1];
      }

      u = intPart % 10;
      intPart /= 10;
      endingZero++;
      xcopy(displayString + insertAt + 1, displayString + insertAt, endingZero - insertAt);
      displayString[insertAt] = '0' + u;
    } while(intPart != 0);

    strcat(displayString, STD_SPACE_PUNCTUATION);
    endingZero += 2;
  }

  else { // FT_IMPROPER d/c
    if(updateDisplayValueX) {
      sprintf(displayValueX, "%s%" PRIu64 "/%" PRIu64, (sign == -1 ? "-" : ""), numer, denom);
    }

    if(sign == -1) {
      strcpy(displayString, STD_SUP_MINUS);
      endingZero = 2;
    }
    else {
      displayString[0] = 0;
      endingZero = 0;
    }
  }

  // Numerator
  insertAt = endingZero;
  gap = -1;
  do {
    gap++;
    if(gap == groupingGap) {
      gap = 0;
      endingZero++;
      xcopy(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);
      displayString[insertAt]     = STD_SPACE_PUNCTUATION[0];
      displayString[insertAt + 1] = STD_SPACE_PUNCTUATION[1];
    }

    u = numer % 10;
    numer /= 10;
    endingZero++;
    xcopy(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);

    if(u <= 1) {
      displayString[insertAt]     = STD_SUP_0[0];
      displayString[insertAt + 1] = STD_SUP_0[1];
      displayString[insertAt + 1] += u;
    }
    else if(u <= 3) {
      displayString[insertAt]     = STD_SUP_2[0];
      displayString[insertAt + 1] = STD_SUP_2[1];
      displayString[insertAt + 1] += u - 2;
    }
    else {
      displayString[insertAt]     = STD_SUP_4[0];
      displayString[insertAt + 1] = STD_SUP_4[1];
      displayString[insertAt + 1] += u - 4;
    }

  } while(numer != 0);


  // Fraction bar
  strcat(displayString, "/");
  endingZero++;


  // Denominator
  insertAt = endingZero;
  gap = -1;
  do {
    gap++;
    if(gap == groupingGap) {
      gap = 0;
      endingZero++;
      xcopy(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);
      displayString[insertAt]     = STD_SPACE_PUNCTUATION[0];
      displayString[insertAt + 1] = STD_SPACE_PUNCTUATION[1];
    }

    u = denom % 10;
    denom /= 10;
    endingZero++;
    xcopy(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);
    displayString[insertAt]     = STD_SUB_0[0];
    displayString[insertAt + 1] = STD_SUB_0[1];
    displayString[insertAt + 1] += u;
  } while(denom != 0);

  // Comparison sign
       if(lessEqualGreater == -1) strcat(displayString, STD_SPACE_PUNCTUATION "<" STD_SPACE_HAIR);
  else if(lessEqualGreater ==  0) strcat(displayString, STD_SPACE_PUNCTUATION "=" STD_SPACE_HAIR);
  else if(lessEqualGreater ==  1) strcat(displayString, STD_SPACE_PUNCTUATION ">" STD_SPACE_HAIR);
  else {
    strcat(displayString, STD_SPACE_PUNCTUATION "?");
    sprintf(errorMessage, "In function fractionToDisplayString: %d is an unexpected value for lessEqualGreater!", lessEqualGreater);
    displayBugScreen(errorMessage);
  }
}



void angle34ToDisplayString2(const real34_t *angle34, uint8_t mode, char *displayString, int16_t displayHasNDigits, bool_t limitExponent, const char *separator, bool_t frontSpace) {
  if(mode == amDMS) {
    char degStr[27];
    uint32_t m, s, fs;
    int16_t sign;
    bool_t overflow;
    real34_t angle34Dms;
    real_t angleDms, degrees, minutes, seconds;

    real34FromDegToDms(angle34, &angle34Dms);
    real34ToReal(&angle34Dms, &angleDms);

    sign = realIsNegative(&angleDms);
    realSetPositiveSign(&angleDms);

    // Get the degrees
    realToIntegralValue(&angleDms, &degrees, DEC_ROUND_DOWN, &ctxtReal39);

    // Get the minutes
    realSubtract(&angleDms, &degrees, &angleDms, &ctxtReal39);
    realMultiply(&angleDms, const_100, &angleDms, &ctxtReal39);
    realToIntegralValue(&angleDms, &minutes, DEC_ROUND_DOWN, &ctxtReal39);

    // Get the seconds
    realSubtract(&angleDms, &minutes, &angleDms, &ctxtReal39);
    realMultiply(&angleDms, const_100, &angleDms, &ctxtReal39);
    realToIntegralValue(&angleDms, &seconds, DEC_ROUND_DOWN, &ctxtReal39);

    // Get the fractional seconds
    realSubtract(&angleDms, &seconds, &angleDms, &ctxtReal39);
    realMultiply(&angleDms, const_100, &angleDms, &ctxtReal39);

    realToUInt32(&angleDms, DEC_ROUND_DOWN, &fs, &overflow);
    realToUInt32(&seconds,  DEC_ROUND_DOWN, &s,  &overflow);
    realToUInt32(&minutes,  DEC_ROUND_DOWN, &m,  &overflow);

    if(fs >= 100) {
      fs -= 100;
      s++;
    }

    if(s >= 60) {
      s -= 60;
      m++;
    }

    if(m >= 60) {
      m -= 60;
      realAdd(&degrees, const_1, &degrees, &ctxtReal39);
    }

    realToString(&degrees, degStr);
    for(int32_t i=0; degStr[i]!=0; i++) {
      if(degStr[i] == '.') {
        degStr[i] = 0;
        break;
      }
    }

    sprintf(displayString, "%s%s" STD_DEGREE "%s%" PRIu32 STD_QUOTE "%s%" PRIu32 "%s%02" PRIu32 STD_DOUBLE_QUOTE,
                            sign ? "-" : " ",
                              degStr,         m < 10 ? " " : "",
                                                m,                   s < 10 ? " " : "",
                                                                       s,         RADIX34_MARK_STRING,
                                                                                    fs);
  }
  else if(mode == amMultPi) {
    real34_t multPi34;
    real_t multPi;

    real34ToReal(angle34, &multPi);
    realDivide(&multPi, const_pi, &multPi, &ctxtReal39);
    realToReal34(&multPi, &multPi34);
    real34ToDisplayString2(&multPi34, displayString, displayHasNDigits, limitExponent, separator, mode == amSecond, frontSpace);
    strcat(displayString, STD_pi);
  }
  else {
    real34ToDisplayString2(angle34, displayString, displayHasNDigits, limitExponent, separator, mode == amSecond, frontSpace);

         if(mode == amRadian) strcat(displayString, STD_SUP_r);
    else if(mode == amGrad)   strcat(displayString, STD_SUP_g);
    else if(mode == amDegree) strcat(displayString, STD_DEGREE);
    else if(mode == amSecond) strcat(displayString, "s");
    else {
      strcat(displayString, "?");
      sprintf(errorMessage, "In function angle34ToDisplayString2: %" PRIu8 " is an unexpected value for mode!", mode);
      displayBugScreen(errorMessage);
    }
  }
}



void shortIntegerToDisplayString(calcRegister_t regist, char *displayString, bool_t determineFont) {
  int16_t i, j, k, unit, gap, digit, bitsPerDigit, maxDigits, base;
  uint64_t number, sign;

  base    = getRegisterTag(regist);
  number  = *(REGISTER_SHORT_INTEGER_DATA(regist));

  if(base <= 1 || base >= 17) {
    sprintf(errorMessage, "In function shortIntegerToDisplayString: %d is an unexpected value for base!", base);
    displayBugScreen(errorMessage);
    base = 10;
  }

  //number &= shortIntegerMask;

  if(shortIntegerMode == SIM_UNSIGN || base == 2 || base == 4 || base == 8 || base == 16) {
    sign = 0;
  }
  else {
    sign = number & shortIntegerSignBit;
  }

  if(sign) {
    if(shortIntegerMode == SIM_2COMPL) {
      number |= ~shortIntegerMask;
      number = ~number + 1;
    }
    else if(shortIntegerMode == SIM_1COMPL) {
      number = ~number;
    }
    else if(shortIntegerMode == SIM_SIGNMT) {
      number &= ~shortIntegerSignBit;
    }
    else {
      sprintf(errorMessage, "In function shortIntegerToDisplayString: %d is an unexpected value for shortIntegerMode!", shortIntegerMode);
      displayBugScreen(errorMessage);
    }

    number &= shortIntegerMask;
  }

  i = ERROR_MESSAGE_LENGTH / 2;

  if(number == 0) {
    displayString[i++] = '0';
    digit = 1;
  }
  else {
    digit = 0;
  }

  if(groupingGap == 0) {
    gap = 0;
  }
  else {
    if(base == 2) {
      gap = 4;
    }
    else if(base == 4 || base == 16) { //JMGAP vv
      gap = 2;
    }
    else if(base == 8) {
      gap = 3;                         //  keeping base == 8 separate as opposed to just dropping it from base == 4 and base == 16  allows it to be changed back to 2 easily.
    }                                  //JMGAP ^^
    else {
      gap = 3;
    }
  }

  while(number) {
    if(gap != 0 && digit != 0 && digit%gap == 0) {
      displayString[i++] = ' ';
    }
    digit++;

    unit = number % base;
    number /= base;
    displayString[i++] = digits[unit];
  }

  // Add leading zeros
  if(getSystemFlag(FLAG_LEAD0)) {
         if(base ==  2) bitsPerDigit = 1;
    else if(base ==  4) bitsPerDigit = 2;
    else if(base ==  8) bitsPerDigit = 3;
    else if(base == 16) bitsPerDigit = 4;
    else                bitsPerDigit = 0;

    if(bitsPerDigit != 0) {
      maxDigits = shortIntegerWordSize / bitsPerDigit;
      if(shortIntegerWordSize % bitsPerDigit) {
        maxDigits++;
      }

      while(digit < maxDigits) {
        if(gap != 0 && digit%gap == 0) {
          displayString[i++] = ' ';
        }
        digit++;

        displayString[i++] = '0';
      }
    }
  }

  if(sign) {
    displayString[i++] = '-';
  }

  if(determineFont) { // The font is not yet determined
    // 1st try: numeric font digits from 30 to 39
    fontForShortInteger = &numericFont;

    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = STD_SPACE_PUNCTUATION[0];
        displayString[j]   = STD_SPACE_PUNCTUATION[1];
      }
      else {
        displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, STD_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, fontForShortInteger, false, false) < SCREEN_WIDTH) {
      return;
    }

    // 2nd try: numeric font digits from 2487 to 2490
    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = STD_SPACE_PUNCTUATION[0];
        displayString[j]   = STD_SPACE_PUNCTUATION[1];
      }
      else if(0x30 <= displayString[k] && displayString[k] <= 0x39) {
        displayString[j++] = NUM_0_b[0];
        displayString[j]   = NUM_0_b[1] - '0' + displayString[k];
      }
      else {
        displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, STD_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, fontForShortInteger, false, false) < SCREEN_WIDTH) {
      return;
    }

    // 3rd try: standard font digits from 30 to 39
    fontForShortInteger = &standardFont;

    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = STD_SPACE_PUNCTUATION[0];
        displayString[j]   = STD_SPACE_PUNCTUATION[1];
      }
      else {
       displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, STD_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(temporaryInformation == TI_SHOW_REGISTER_BIG || stringWidth(displayString, fontForShortInteger, false, false) < SCREEN_WIDTH) {     //JMSHOW
      return;
    }

    // 4th and last try: standard font digits 220e and 2064 (binary)
    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = STD_SPACE_PUNCTUATION[0];
        displayString[j]   = STD_SPACE_PUNCTUATION[1];
      }
      else if(displayString[k] == '0') {
        displayString[j++] = STD_ZERO[0];
        displayString[j]   = STD_ZERO[1];
      }
      else if(displayString[k] == '1') {
        displayString[j++] = STD_ONE[0];
        displayString[j]   = STD_ONE[1];
      }
      else {
        displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, STD_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, fontForShortInteger, false, false) < SCREEN_WIDTH) {
      return;
    }

    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function shortIntegerToDisplayString: the integer data representation is too wide (1)!", displayString, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)

    strcpy(displayString, "Integer data representation to wide!");
  }

  else { // the font is already determined (standard font)
    fontForShortInteger = &standardFont;

    // 1st try: standard font digits from 30 to 39
    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = STD_SPACE_PUNCTUATION[0];
        displayString[j]   = STD_SPACE_PUNCTUATION[1];
      }
      else {
        displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, STD_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, fontForShortInteger, false, false) < SCREEN_WIDTH) {
      return;
    }

    // 2nd and last try: standard font digits 220e and 2064 (binary)
    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = STD_SPACE_PUNCTUATION[0];
        displayString[j]   = STD_SPACE_PUNCTUATION[1];
      }
      else if(displayString[k] == '0') {
        displayString[j++] = STD_ZERO[0];
        displayString[j]   = STD_ZERO[1];
      }
      else if(displayString[k] == '1') {
        displayString[j++] = STD_ONE[0];
        displayString[j]   = STD_ONE[1];
      }
      else {
       displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, STD_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, fontForShortInteger, false, false) < SCREEN_WIDTH) {
      return;
    }

    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function shortIntegerToDisplayString: the integer data representation is too wide (2)!", displayString, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)

    strcpy(displayString, "Integer data representation to wide!");
  }
}



void longIntegerRegisterToDisplayString(calcRegister_t regist, char *displayString, int32_t strLg, int16_t max_Width, int16_t maxExp, const char *separator, bool_t allowLARGELI) { //JM mod max_Width;   //JM added last parameter: Allow LARGELI
  int16_t exponentStep;
  uint32_t exponentShift, exponentShiftLimit;
  longInteger_t lgInt;
  int16_t maxWidth;                                         //JM align longints

  convertLongIntegerRegisterToLongInteger(regist, lgInt);

  if(longIntegerIsNegative(lgInt)) {maxWidth = max_Width;}  //JM align longints
  else {maxWidth = max_Width - 8;}                          //JM align longints

  exponentShift = (longIntegerBits(lgInt) - 1) * 0.3010299956639811952137;
  exponentStep = (groupingGap == 0 ? 1 : groupingGap);
  exponentShift = (exponentShift / exponentStep + 1) * exponentStep;
  exponentShiftLimit = (maxExp / exponentStep + 1) * exponentStep;
  if(exponentShift > exponentShiftLimit) {
    exponentShift -= exponentShiftLimit;

    // Why do the following lines not work (for a big exponentShift) instead of the for loop below ?
    //longIntegerInitSizeInBits(divisor, longIntegerBits(lgInt));
    //longIntegerPowerUIntUInt(10u, exponentShift, divisor);
    //longIntegerDivide(lgInt, divisor, lgInt);
    //longIntegerFree(divisor);
    for(int32_t i=(int32_t)exponentShift; i>=1; i--) {
      if(i >= 9)      {longIntegerDivideUInt(lgInt, 1000000000, lgInt); i -= 8;}
      else if(i >= 4) {longIntegerDivideUInt(lgInt,      10000, lgInt); i -= 3;}
      else            {longIntegerDivideUInt(lgInt,         10, lgInt);}
    }
  }
  else {
    exponentShift = 0;
  }

  longIntegerToAllocatedString(lgInt, displayString, strLg);
  if(updateDisplayValueX) {
    strcpy(displayValueX, displayString);
  }

  longIntegerFree(lgInt);

  if(groupingGap > 0) {
    int16_t len = strlen(displayString);
    for(int16_t i=len - groupingGap; i>0; i-=groupingGap) {
      if(i != 1 || displayString[0] != '-') {
        xcopy(displayString + i + 2, displayString + i, len - i + 1);
        displayString[i] = *separator;
        displayString[i + 1] = *(separator + 1);
        len += 2;
      }
    }
  }
  if(stringWidth(displayString, allowLARGELI && jm_LARGELI ? &numericFont : &standardFont, false, false) > maxWidth) {      //JM
    char exponentString[14], lastRemovedDigit;
    int16_t lastChar, stringStep, tenExponent;

    stringStep = (groupingGap == 0 ? 1 : groupingGap + 2);
    tenExponent = exponentStep + exponentShift;
    lastChar = strlen(displayString) - stringStep;
    lastRemovedDigit = displayString[lastChar + 2];
    displayString[lastChar] = 0;
    if(updateDisplayValueX) {
      displayValueX[strlen(displayValueX) - max(groupingGap, 1)] = 0;
    }
    exponentString[0] = 0;
    exponentToDisplayString(tenExponent, exponentString, NULL, false, separator);
    while(stringWidth(displayString,   allowLARGELI && jm_LARGELI ? &numericFont : &standardFont, false, true) + stringWidth(exponentString,   allowLARGELI && jm_LARGELI ? &numericFont : &standardFont, true, false) > maxWidth) {  //JM jm_LARGELI
      lastChar -= stringStep;
      tenExponent += exponentStep;
      lastRemovedDigit = displayString[lastChar + 2];
      displayString[lastChar] = 0;
      if(updateDisplayValueX) {
        displayValueX[strlen(displayValueX) - max(groupingGap, 1)] = 0;
      }
      exponentString[0] = 0;
      exponentToDisplayString(tenExponent, exponentString, NULL, false, separator);
    }

    if(lastRemovedDigit >= '5') { // Round up
      lastChar = strlen(displayString) - 1;
      displayString[lastChar]++;
      while(displayString[lastChar] > '9') {
        displayString[lastChar--] = '0';
        while(lastChar >= 0 && (displayString[lastChar] < '0' || displayString[lastChar] > '9')) lastChar--;
        if(lastChar >= 0) {
          displayString[lastChar]++;
        }
        else { // We are rounding up from 9999... to 10000...
          lastChar = (displayString[0] == '-' ? 1 : 0);
          xcopy(displayString + lastChar + 1, displayString + lastChar, strlen(displayString) + 1);
          displayString[lastChar] = '1';
          if(groupingGap != 0 && displayString[lastChar + groupingGap + 2] == *(separator + 1)) { // We need to insert a new goup separator
            xcopy(displayString + lastChar + 3, displayString + lastChar + 1, strlen(displayString));
            displayString[lastChar + 1] = *separator;
            displayString[lastChar + 2] = *(separator + 1);
          }

          // Has the string become too long?
          if(stringWidth(displayString,   allowLARGELI && jm_LARGELI ? &numericFont : &standardFont, false, true) + stringWidth(exponentString,   allowLARGELI && jm_LARGELI ? &numericFont : &standardFont, true, false) > maxWidth) {   //JM jm_LARGELI
            lastChar = strlen(displayString) - stringStep;
            tenExponent += exponentStep;
            displayString[lastChar] = 0;
            if(updateDisplayValueX) {
              displayValueX[strlen(displayValueX) - max(groupingGap, 1)] = 0;
            }
            exponentString[0] = 0;
            exponentToDisplayString(tenExponent, exponentString, NULL, false, separator);
          }
        }
      }

      if(updateDisplayValueX) {
        lastChar = strlen(displayValueX) - 1;
        displayValueX[lastChar]++;
        while(lastChar>0 && '0' <= displayValueX[lastChar - 1] && displayValueX[lastChar - 1] <= '9' && displayValueX[lastChar] > '9') {
          displayValueX[lastChar--] = '0';
          displayValueX[lastChar]++;
        }

        if(displayValueX[lastChar] > '9') { // We are rounding 9999... to 10000...
          xcopy(displayValueX + 1, displayValueX, strlen(displayValueX) + 1);
          displayValueX[lastChar++] = '1';
          displayValueX[lastChar] = '0';
        }
      }
    }

    strcat(displayString, exponentString);

    if(updateDisplayValueX) {
      sprintf(displayValueX + strlen(displayValueX), "e%d", tenExponent);
    }
  }
}



void longIntegerToAllocatedString(const longInteger_t lgInt, char *str, int32_t strLen) {
  int32_t numberOfDigits, stringLen, counter;
  longInteger_t x;

  str[0] = '0';
  str[1] = 0;
  if(lgInt->_mp_size == 0) {
    return;
  }

  //numberOfDigits = longIntegerBase10Digits(lgInt); // GMP documentation says the result can be 1 to big
  numberOfDigits = mpz_sizeinbase(lgInt, 10); // GMP documentation says the result can be 1 to big
  if(lgInt->_mp_size < 0) {
    stringLen = numberOfDigits + 2; // 1 for the trailing 0 and 1 for the minus sign
    str[0] = '-';
  }
  else {
    stringLen = numberOfDigits + 1; // 1 for the trailing 0
  }

  if(strLen < stringLen) {
    sprintf(errorMessage, "In function longIntegerToAllocatedString: the string str (%" PRId32 " bytes) is too small to hold the base 10 representation of lgInt, %" PRId32 " are needed!", strLen, stringLen);
    displayBugScreen(errorMessage);
    return;
  }

  str[stringLen - 1] = 0;

  //longIntegerInitSizeInBits(x, longIntegerBits(lgInt));
  mpz_init2(x, mpz_sizeinbase(lgInt, 2));

  //longIntegerAddUInt(lgInt, 0, x);
  mpz_add_ui(x, lgInt, 0);

  //longIntegerSetPositiveSign(x);
  x->_mp_size =  abs(x->_mp_size);


  stringLen -= 2; // set stringLen to the last digit of the base 10 representation
  counter = numberOfDigits;
  //while(!longIntegerIsZero(x)) {
  while(x->_mp_size != 0) {
    str[stringLen--] = '0' + mpz_tdiv_ui(x, 10);

    //longIntegerDivideUInt(x, 10, x);
    mpz_tdiv_q_ui(x, x, 10);

    counter--;
  }

  if(counter == 1) { // digit was 1 too big
    xcopy(str + stringLen, str + stringLen + 1, numberOfDigits);
  }

  //longIntegerFree(x);
  mpz_clear(x);
}


/** based on itoa, integer to string
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
static void uint32ToString_jm(uint32_t val, char* result, uint8_t len) { //jm
    int32_t value = val;
    char* ptr = result, *ptr1 = result, tmp_char;
    int32_t tmp_value;
    int cnt = 0;
    do {
        tmp_value = value;
        value /= 10;
        *ptr++ = "9876543210123456789" [9 + (tmp_value - value * 10)];
    } while ( ++cnt != len);
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
}

void dateToDisplayString(calcRegister_t regist, char *displayString) {
  char syy[10],smm[10],sdd[10];
  real34_t j, y, yy, m, d;
  uint64_t yearVal;
  char sign[] = {0, 0};

  internalDateToJulianDay(REGISTER_REAL34_DATA(regist), &j);
  decomposeJulianDay(&j, &y, &m, &d);
  if(real34IsNegative(&y)) {
    sign[0] = '-';
  }
  real34CopyAbs(&y, &y);
  real34CopyAbs(&y, &yy);
  real34DivideRemainder(&y, const34_2p32, &y);
  real34Divide(&yy, const34_2p32, &yy);
  real34ToIntegralValue(&yy, &yy, DEC_ROUND_DOWN);
  yearVal = ((uint64_t)real34ToUInt32(&yy) << 32) | ((uint64_t)real34ToUInt32(&y));

  uint32ToString_jm(yearVal,syy,4);
  uint32_t mm = real34ToUInt32(&m);
  uint32ToString_jm(mm,smm,2);
  uint32_t dd = real34ToUInt32(&d);
  uint32ToString_jm(dd,sdd,2);

  if(getSystemFlag(FLAG_DMY)) {
    sprintf(displayString, "%s.%s.%s%s", sdd, smm, sign, syy); //jm
  }
  else if(getSystemFlag(FLAG_MDY)) {
    sprintf(displayString, "%s/%s/%s%s", smm, sdd, sign, syy); //jm
  }
  else { // YMD
    sprintf(displayString, "%s%s-%s-%s", sign, syy, smm, sdd); //jm
  }
}



void timeToDisplayString(calcRegister_t regist, char *displayString, bool_t ignoreTDisp) {
  real_t real, value, tmp, h, m, s;
  longInteger_t hli;
  int32_t sign, i;
  uint32_t digits, tDigits = 0u, bDigits, m32, s32;
  char digitBuf[16], digitBuf2[48];
  char* bufPtr;
  bool_t isValid12hTime = false, isAfternoon = false, overflow;
  uint8_t savedDisplayFormat = displayFormat, savedDisplayFormatDigits = displayFormatDigits;

  real34ToReal(REGISTER_REAL34_DATA(regist), &real);
  sign = realIsNegative(&real);

  // Short time (displayed like SCI/ENG)
  if(timeDisplayFormatDigits == 0) {
    realDivide(const_1, const_1000, &value, &ctxtReal39);
  }
  else if((timeDisplayFormatDigits == 1) || (timeDisplayFormatDigits == 2)) {
    realCopy(const_60, &value);
  }
  else {
    realCopy(const_1, &value);
    for(i = 3; i < timeDisplayFormatDigits; ++i) {
      --value.exponent;
      if(i == 5) break;
    }
  }
  if(realCompareAbsLessThan(&real, const_1)) {
    realCopy(const_1, &tmp), tmp.exponent -= 33;
    realDivideRemainder(&real, &tmp, &tmp, &ctxtReal39);
  }
  else {
    realZero(&tmp);
  }
  if(realCompareAbsLessThan(&real, &value) || (ignoreTDisp && (!realIsZero(&tmp)))) {
    if(ignoreTDisp || (timeDisplayFormatDigits == 0)) {
      displayFormat = DF_ALL;
      displayFormatDigits = 0;
    }
    else {
      displayFormat = getSystemFlag(FLAG_ALLENG) ? DF_ENG : DF_SCI;
      displayFormatDigits = 3;
    }
    real34ToDisplayString(REGISTER_REAL34_DATA(regist), amSecond, displayString, &standardFont, 2000, ignoreTDisp ? 34 : 16, false, STD_SPACE_4_PER_EM, false);
    displayFormatDigits = savedDisplayFormatDigits;
    displayFormat = savedDisplayFormat;
    return;
  }
  displayFormatDigits = savedDisplayFormatDigits;
  displayFormat = savedDisplayFormat;

  // Hours
  realDivide(&real, const_3600, &h, &ctxtReal39);
  realSetPositiveSign(&h);
  realToIntegralValue(&h, &h, DEC_ROUND_DOWN, &ctxtReal39);

  // Pre-rounding
  if(!ignoreTDisp) {
    switch(timeDisplayFormatDigits) {
      case 0:
        int32ToReal(86400, &value);
        if((!sign) && (!getSystemFlag(FLAG_TDM24)) && realCompareLessThan(&real, &value)) {
          isValid12hTime = true;
        }
        for(bDigits = 0; bDigits < (isValid12hTime ? 14 : 16); ++bDigits) {
          if(realCompareAbsLessThan(&h, const_100)) break;
          ++value.exponent;
        }
        tDigits = isValid12hTime ? 13 : 15;
        isValid12hTime = false;
        goto do_rounding;
      case 1: case 2: // round to minutes
        realDivide(&real, const_60, &real, &ctxtReal39);
        realToIntegralValue(&real, &real, DEC_ROUND_DOWN, &ctxtReal39);
        realMultiply(&real, const_60, &real, &ctxtReal39);
        break;
      default: // round to seconds, milliseconds, microseconds, ...
        tDigits = timeDisplayFormatDigits + 1;
        bDigits = 4u;
      do_rounding:
        for(digits = bDigits; digits < tDigits; ++digits) {
          ++real.exponent;
        }
        realToIntegralValue(&real, &real, timeDisplayFormatDigits == 0 ? DEC_ROUND_HALF_UP : DEC_ROUND_DOWN, &ctxtReal39);
        for(digits = bDigits; digits < tDigits; ++digits) {
          --real.exponent;
        }
        tDigits = 0u;
    }
  }
  realSetPositiveSign(&real);

  // Seconds
  realToIntegralValue(&real, &s, DEC_ROUND_DOWN, &ctxtReal39);
  realSubtract(&real, &s, &real, &ctxtReal39); // Fractional part
  // Minutes
  realDivide(&s, const_60, &m, &ctxtReal39);
  realToIntegralValue(&m, &m, DEC_ROUND_DOWN, &ctxtReal39);
  realDivideRemainder(&s, const_60, &s, &ctxtReal39);
  realDivideRemainder(&m, const_60, &m, &ctxtReal39);
  // 12-hour time
  if((!getSystemFlag(FLAG_TDM24)) && (!sign)) {
    int32ToReal(24, &value);
    if(realCompareLessThan(&h, &value)) {
      isValid12hTime = true;
      int32ToReal(12, &value);
      if(realCompareGreaterEqual(&h, &value)) {
        isAfternoon = true;
        if(!realCompareLessEqual(&h, &value)) {
          realSubtract(&h, &value, &h, &ctxtReal39);
        }
      }
      else if(realIsZero(&h)) {
        realAdd(&h, &value, &h, &ctxtReal39);
      }
    }
  }

  // Display Hours
  strcpy(displayString, sign ? "-" : " ");
  longIntegerInit(hli);
  convertRealToLongInteger(&h, hli, DEC_ROUND_DOWN);
  longIntegerToAllocatedString(hli, digitBuf2, sizeof(digitBuf2));
  longIntegerFree(hli);

  bufPtr = digitBuf2;
  digitBuf[1] = '\0';
  for(digits = strlen(digitBuf2); digits > 0; --digits){
    digitBuf[0] = *bufPtr++;
    strcat(displayString, digitBuf);
    if((digits % 3u == 1) && (digits > 1)) {
      strcat(displayString, STD_SPACE_4_PER_EM);
    }
    ++tDigits;
  }

  if((!ignoreTDisp) && (timeDisplayFormatDigits == 1 || timeDisplayFormatDigits == 2 || (++tDigits) > (isValid12hTime ? 16 : 18))) {
    // Display Minutes
    realToUInt32(&m, DEC_ROUND_DOWN, &m32, &overflow);
    sprintf(digitBuf, ":%02" PRIu32, m32);
    strcat(displayString, digitBuf);
  }

  else {
    // Display MM:SS
    realToUInt32(&m, DEC_ROUND_DOWN, &m32, &overflow);
    realToUInt32(&s, DEC_ROUND_DOWN, &s32, &overflow);
    sprintf(digitBuf, ":%02" PRIu32 ":%02" PRIu32, m32, s32);
    strcat(displayString, digitBuf);

    // Display fractional part of seconds
    digits = 0u;
    realZero(&value);
    while(1) {
      realSubtract(&real, &value, &real, &ctxtReal39);
      if(ignoreTDisp || (timeDisplayFormatDigits == 0)) {
        if(realIsZero(&real)) {
          break;
        }
      }
      else {
        if(digits + 4 > timeDisplayFormatDigits) {
          break;
        }
      }
      if((!ignoreTDisp) && ((++tDigits) > (isValid12hTime ? 16 : 18))) {
        break;
      }
      realMultiply(&real, const_10, &real, &ctxtReal39);
      realToIntegralValue(&real, &value, DEC_ROUND_DOWN, &ctxtReal39);

      if(digits == 0u) {
        strcat(displayString, RADIX34_MARK_STRING);
      }
      else if(digits % 3 == 0u) {
        strcat(displayString, STD_SPACE_4_PER_EM);
      }

      realToUInt32(&value, DEC_ROUND_DOWN, &s32, &overflow);
      sprintf(digitBuf, "%" PRIu32, s32);
      strcat(displayString, digitBuf);
      ++digits;
    }
  }

  // for 12-hour time
  if(isAfternoon) {
    strcat(displayString, "p.m.");
  }
  else if(isValid12hTime) {
    strcat(displayString, "a.m.");
  }
}

void real34MatrixToDisplayString(calcRegister_t regist, char *displayString) {
  dataBlock_t* dblock = REGISTER_REAL34_MATRIX_DBLOCK(regist);
  sprintf(displayString, "[%" PRIu16 STD_CROSS "%" PRIu16" Matrix]", dblock->matrixRows, dblock->matrixColumns);
}

void complex34MatrixToDisplayString(calcRegister_t regist, char *displayString) {
  dataBlock_t* dblock = REGISTER_REAL34_MATRIX_DBLOCK(regist);
  sprintf(displayString, "[%" PRIu16 STD_CROSS "%" PRIu16 " " STD_COMPLEX_C " Matrix]", dblock->matrixRows, dblock->matrixColumns);
}

void fnShow(uint16_t unusedButMandatoryParameter) {
#ifndef SAVE_SPACE_DM42_5
  uint8_t savedDisplayFormat = displayFormat, savedDisplayFormatDigits = displayFormatDigits;
  uint8_t savedSigFigMode = SigFigMode;           //JM
  bool_t savedUNITDisplay = UNITDisplay;          //JM

  int16_t source, dest, last, d, maxWidth, offset, bytesProcessed;
  real34_t real34;
  char *separator;
  bool_t thereIsANextLine;

  displayFormat = DF_ALL;
  displayFormatDigits = 0;
  SigFigMode = 0;                                //JM SIGFIG
  UNITDisplay = false;                           //JM SIGFIG

  tmpString[   0] = 0; // L1
  tmpString[ 300] = 0; // L2
  tmpString[ 600] = 0; // L3
  tmpString[ 900] = 0; // L4
  tmpString[1200] = 0; // L5
  tmpString[1500] = 0; // L6
  tmpString[1800] = 0; // L7

  temporaryInformation = TI_SHOW_REGISTER;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      separator = STD_SPACE_4_PER_EM;
      longIntegerRegisterToDisplayString(REGISTER_X, errorMessage, WRITE_BUFFER_LEN, 3200, 400, separator, false);//JM added last parameter: Allow LARGELI

      last = stringByteLength(errorMessage);
      source = 0;
      dest = 0;

      if(groupingGap == 0) {
        maxWidth = SCREEN_WIDTH - stringWidth("0", &standardFont, true, true);
      }
      else {
        maxWidth = SCREEN_WIDTH - stringWidth("0", &standardFont, true, true)*groupingGap - stringWidth(separator, &standardFont, true, true);
      }

      for(d=0; d<=1800 ; d+=300) {
        dest = d;
        while(source < last && stringWidth(tmpString + d, &standardFont, true, true) <= maxWidth) {
          do {
            tmpString[dest] = errorMessage[source];
            if(tmpString[dest] & 0x80) {
              tmpString[++dest] = errorMessage[++source];
            }
            source++;
            tmpString[++dest] = 0;
          } while(source < last && groupingGap > 0 && (errorMessage[source] != *separator || errorMessage[source + 1] != *(separator + 1)));
        }
      }

      if(source < last) { // The long integer is too long
        xcopy(tmpString + dest - 2, STD_ELLIPSIS, 2);
        xcopy(tmpString + dest, STD_SPACE_6_PER_EM, 2);
        tmpString[dest + 2] = 0;
      }
      break;

    case dtReal34:
      real34ToDisplayString(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), tmpString, &standardFont, 2000, 34, false, STD_SPACE_4_PER_EM, false);
      break;

    case dtComplex34:
      // Real part
      separator = STD_SPACE_4_PER_EM;
      real34ToDisplayString(REGISTER_REAL34_DATA(REGISTER_X), amNone, tmpString, &standardFont, 2000, 34, false, separator, false);

      // +/- i×
      real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), &real34);
      last = 300;
      while(tmpString[last]) last++;
      xcopy(tmpString + last++, (real34IsNegative(&real34) ? "-" : "+"), 1);
      xcopy(tmpString + last++, COMPLEX_UNIT, 1);
      xcopy(tmpString + last, PRODUCT_SIGN, 3);

      // Imaginary part
      real34SetPositiveSign(&real34);
      real34ToDisplayString(&real34, amNone, tmpString + 600, &standardFont, 2000, 34, false, separator, false);

      if(stringWidth(tmpString + 300, &standardFont, true, true) + stringWidth(tmpString + 600, &standardFont, true, true) <= SCREEN_WIDTH) {
        last = 300;
        while(tmpString[last]) last++;
        xcopy(tmpString + last, tmpString + 600, strlen(tmpString + 600) + 1);
        tmpString[600] = 0;
      }

      if(stringWidth(tmpString, &standardFont, true, true) + stringWidth(tmpString + 300, &standardFont, true, true) <= SCREEN_WIDTH) {
        last = 0;
        while(tmpString[last]) last++;
        xcopy(tmpString + last, tmpString + 300, strlen(tmpString + 300) + 1);
        xcopy(tmpString + 300,  tmpString + 600, strlen(tmpString + 600) + 1);
        tmpString[600] = 0;
      }
      break;

    case dtTime:
      timeToDisplayString(REGISTER_X, tmpString, true);
      break;

    case dtDate:
      dateToDisplayString(REGISTER_X, tmpString);
      break;

    case dtString:
      offset = 0;
      thereIsANextLine = true;
      bytesProcessed = 0;
      while(thereIsANextLine) {
        xcopy(tmpString + offset, REGISTER_STRING_DATA(REGISTER_X) + bytesProcessed, stringByteLength(REGISTER_STRING_DATA(REGISTER_X) + bytesProcessed) + 1);
        thereIsANextLine = false;
        while(stringWidth(tmpString + offset, &standardFont, false, true) >= SCREEN_WIDTH) {
          tmpString[offset + stringLastGlyph(tmpString + offset)] = 0;
          thereIsANextLine = true;
        }
        bytesProcessed += stringByteLength(tmpString + offset);
        offset += 300;
        tmpString[offset] = 0;
      }
      break;

    case dtConfig:
      xcopy(tmpString, "Configuration data", 19);
      break;

    default:
      temporaryInformation = TI_NO_INFO;
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "cannot SHOW %s", getRegisterDataTypeName(REGISTER_X, true, false));
        moreInfoOnError("In function fnShow:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
  }

  displayFormat = savedDisplayFormat;
  displayFormatDigits = savedDisplayFormatDigits;
  SigFigMode = savedSigFigMode;                            //JM SIGFIG
  UNITDisplay = savedUNITDisplay;                          //JM SIGFIG

#endif //SAVE_SPACE_DM42_5
}





void SHOW_reset(void){
  char ss[3];
  uint8_t ix;

  for(ix=0; ix<=8; ix++) { //L1 ... L7
    tmpString[ix*300]=0;    
  }

  temporaryInformation = TI_SHOW_REGISTER_SMALL;

  ss[0]=0;
  ss[1]=0;
  if(SHOWregis >= 0 && SHOWregis < 100) {
    snprintf(tmpString + 2100, 10, "%d:", SHOWregis);
  } else
  switch (SHOWregis) {
    case REGISTER_X: ss[0]='X'; break;
    case REGISTER_Y: ss[0]='Y'; break;
    case REGISTER_Z: ss[0]='Z'; break;
    case REGISTER_T: ss[0]='T'; break;
    case REGISTER_A: ss[0]='A'; break;
    case REGISTER_B: ss[0]='B'; break;
    case REGISTER_C: ss[0]='C'; break;
    case REGISTER_D: ss[0]='D'; break;
    case REGISTER_L: ss[0]='L'; break;
    case REGISTER_I: ss[0]='I'; break;
    case REGISTER_J: ss[0]='J'; break;
    case REGISTER_K: ss[0]='K'; break;
    default: break;
  }
  strcpy(tmpString + 2100, ss);
  strcat(tmpString + 2100, ": ");
}


void checkAndEat(int16_t *source, int16_t last, int16_t *dest) {
  uint8_t ix;
  if(*source < last && groupingGap!=0) {                  //Not in the last line
    for(ix=0; ix<=3; ix++) {
      if(!(tmpString[(*dest)-2] & 0x80)) {(*dest)--; (*source)--;} //Eat away characters at the end to line up the last space          
    }
    tmpString[*dest] = 0;
  }            
}


void fnShow_SCROLL(uint16_t fnShow_param) {                // Heavily modified by JM from the original fnShow
#ifndef TESTSUITE_BUILD
  uint8_t savedDisplayFormat = displayFormat, savedDisplayFormatDigits = displayFormatDigits, savedSigFigMode = SigFigMode;
  bool_t savedUNITDisplay = UNITDisplay;
  bool_t thereIsANextLine;
  int16_t source, dest, last, d, maxWidth, i, offset, bytesProcessed, aa, aa2=0, aa3=0, aa4=0;
  uint64_t nn;
  real34_t real34;
  char *separator;

  displayFormat = DF_ALL;
  displayFormatDigits = 0;

  #define lowest_SHOW REGISTER_X //0                // Lowest register. Change to 0 for all registers, or use REGISTER_X 
  switch(fnShow_param) {
    case NOPARAM:
    case 0:  SHOWregis = REGISTER_X;
             break;
    case 1:  if(SHOWregis==9999) {SHOWregis = REGISTER_X;}
             else
             {
               SHOWregis++;                         //Activated by KEY_UP
               if(SHOWregis > REGISTER_K) {
                 SHOWregis = lowest_SHOW;  
               }
             }
             break;   
    case 2:  if(SHOWregis==9999) {SHOWregis = REGISTER_X;}
             else
             {
               SHOWregis--;                         //Activate by Key_DOWN
               if(SHOWregis < lowest_SHOW) {
                 SHOWregis = REGISTER_K;  
               } 
             }
             break; 
    case 99:                                        //RESET every time a key is pressed.
             SHOWregis = REGISTER_X;
             return;
             break;
    default: 
      break;
  }



  #ifndef TESTSUITE_BUILD
    #ifdef PC_BUILD
    printf(">>> ---- clearScreen_old from display.c fnShow_SCROLL\n");
    #endif
    clearScreen_old(false, true, false); //Clear screen content while NEW SHOW
  #endif
  SHOW_reset();
  
  switch(getRegisterDataType(SHOWregis)) {
    case dtLongInteger:

      #if defined (VERBOSE_SCREEN) && defined (PC_BUILD)
        printf("SHOW:Longint\n");
      #endif

      strcpy(errorMessage,tmpString + 2100);
      separator = STD_SPACE_4_PER_EM;
      longIntegerRegisterToDisplayString(SHOWregis, errorMessage + 3, WRITE_BUFFER_LEN, 4*400, 350, STD_SPACE_4_PER_EM, false);  //JM added last parameter: Allow LARGELI

      last = stringByteLength(errorMessage);
      source = 0;
      dest = 0;

 
      { //printf("2: %d\n",stringGlyphLength(tmpString + 2100));
        temporaryInformation = TI_SHOW_REGISTER_BIG;
        if(groupingGap == 0) {
          maxWidth = SCREEN_WIDTH - stringWidth("0", &numericFont, true, true);
        }
        else {
          char tmp[4];
          strcpy(tmp,"0");
          strcat(tmp,separator);
          maxWidth = SCREEN_WIDTH - stringWidth(tmp , &numericFont, true, true);          //Add the separator that gets added to the last character
        }

        for(d=0; d<=1200 ; d+=300) {                                                      //LARGE font, fill 7 lines at 0, 300, 600, 900, 1200
                                                                                          //   fill all lines, and check if the 5th line  contains any characters.
          dest = d;
          while(source < last && stringWidth(tmpString + d, &numericFont, true, true) <=  maxWidth) {
            tmpString[dest] = errorMessage[source];
            if(tmpString[dest] & 0x80) {
              tmpString[++dest] = errorMessage[++source];
            }
            source++;
            tmpString[++dest] = 0;
          }

          while (source < last && groupingGap!=0 && !(tmpString[dest-2] & 0x80)) //Not in the last line. Eat away characters at the end to line up the last space.
            {
              dest--; 
              source--;
            }
          tmpString[dest] = 0;

          if(!(source < last) && groupingGap!=0 && (tmpString[dest+0] != 0) && !(tmpString[dest+0] == *(separator + 0) && (tmpString[dest+1] == *(separator + 1)))) {               //Last line
            tmpString[dest+0] = *(separator + 0); //0xa0;       //Add a space to the very end to space last line nicely.
            tmpString[dest+1] = *(separator + 1); //0x05;
            tmpString[dest+2] = 0;
            dest+=2;
          }
        }
      }      

      //printf("### %d %d %d\n",(uint8_t) tmpString[1200],(uint8_t)  tmpString[1201],(uint8_t) tmpString[1202]);
      if(tmpString[1200] != 0) {

        #if defined (VERBOSE_SCREEN) && defined (PC_BUILD)
          printf("SHOW:Longint too long\n");
        #endif
          
        SHOW_reset();
        strcpy(errorMessage,tmpString + 2100);
        longIntegerRegisterToDisplayString(SHOWregis, errorMessage + 3, WRITE_BUFFER_LEN, 7*400, 350, STD_SPACE_4_PER_EM, false);  //JM added last parameter: Allow LARGELI

        last = stringByteLength(errorMessage);
        source = 0;
        dest = 0;

        //printf("1: %d\n",stringGlyphLength(tmpString + 2100));

        if(groupingGap == 0) {
          maxWidth = SCREEN_WIDTH - stringWidth("0", &standardFont, true, true);
        }
        else {
          maxWidth = SCREEN_WIDTH - stringWidth("0", &standardFont, true, true)*groupingGap - stringWidth(separator, &standardFont, true, true);
        }

        for(d=0; d<=1800 ; d+=300) {                                                      //Small font, fill 7 lines at 0, 300, 600, 900, 1200, 1500, 1800
          dest = d;
          while(source < last && stringWidth(tmpString + d, &standardFont, true, true) <= maxWidth) {
            do {
              tmpString[dest] = errorMessage[source];
              if(tmpString[dest] & 0x80) {
                tmpString[++dest] = errorMessage[++source];
              }
              source++;
              tmpString[++dest] = 0;
            } while(source < last && groupingGap > 0 && (errorMessage[source] != *separator || errorMessage[source + 1] != *(separator + 1)));
          }
        }
        if(source < last) { // The long integer is too long
          xcopy(tmpString + dest - 2, STD_ELLIPSIS, 2);
          xcopy(tmpString + dest, STD_SPACE_6_PER_EM, 2);
          tmpString[dest + 2] = 0;
        }
      } 
      break;


    case dtReal34:
      #if defined (VERBOSE_SCREEN) && defined (PC_BUILD)
        printf("SHOW:Real\n");
      #endif
      temporaryInformation = TI_SHOW_REGISTER_BIG;
      separator = STD_SPACE_4_PER_EM;
      real34ToDisplayString(REGISTER_REAL34_DATA(SHOWregis), getRegisterAngularMode(SHOWregis), tmpString + 2103, &numericFont, 2000, 34, false, separator, false);
      last = 2100 + stringByteLength(tmpString + 2100);
      source = 2100;
      for(d=0; d<=900 ; d+=300) {
        dest = d;
        while(source < last && stringWidth(tmpString + d, &numericFont, true, true) <= SCREEN_WIDTH - 8*2) {
          tmpString[dest] = tmpString[source];
          if(tmpString[dest] & 0x80) {
            tmpString[++dest] = tmpString[++source];
          }
          source++;
          tmpString[++dest] = 0;
        }
      }

      if(getRegisterAngularMode(SHOWregis) != amNone) {
        aa = getRegisterAngularMode(SHOWregis);
        switch(getRegisterAngularMode(SHOWregis)) {
          case amDegree: aa = amDMS; break;
          case amRadian: aa = amDegree; break;
          case amGrad: aa = amRadian; break;
          case amMultPi: aa = amRadian; break;
          case amDMS: aa = amDegree; break;
          default:break;
        }
        real34Copy(REGISTER_REAL34_DATA(SHOWregis), &real34);
        convertAngle34FromTo(&real34, getRegisterAngularMode(SHOWregis), aa);
        real34ToDisplayString(&real34, aa, tmpString + 2103, &numericFont, 2000, 34, false, separator, false);
        last = 2100 + stringByteLength(tmpString + 2100);
        source = 2100;
        for(d=600; d<=900 ; d+=300) {
          dest = d;
          while(source < last && stringWidth(tmpString + d, &numericFont, true, true) <= SCREEN_WIDTH - 8*2) {
            tmpString[dest] = tmpString[source];
            if(tmpString[dest] & 0x80) {
              tmpString[++dest] = tmpString[++source];
            }
            source++;
            tmpString[++dest] = 0;
          }
        }
      }
      

      break;


    case dtComplex34:
      #if defined (VERBOSE_SCREEN) && defined (PC_BUILD)
        printf("SHOW:Complex\n");
      #endif
      temporaryInformation = TI_SHOW_REGISTER_BIG;

      // Real part into +0
      separator = STD_SPACE_4_PER_EM;
      real34ToDisplayString(REGISTER_REAL34_DATA(SHOWregis), amNone, tmpString, &numericFont, 2000, 34, false, separator,false);
      for(i=stringByteLength(tmpString) - 1; i>0; i--) {
        if(tmpString[i] == 0x08) {
          tmpString[i] = 0x05;
        }
      }
  
      // +/- i× into +300
      real34Copy(REGISTER_IMAG34_DATA(SHOWregis), &real34);
      last = 300;
      while(tmpString[last]) last++;
      xcopy(tmpString + last++, (real34IsNegative(&real34) ? "-" : "+"), 1);
      xcopy(tmpString + last++, COMPLEX_UNIT, 1);
      xcopy(tmpString + last,   PRODUCT_SIGN, 3);

      // Imaginary part into +600
      real34SetPositiveSign(&real34);
      real34ToDisplayString(&real34, amNone, tmpString + 600, &numericFont, 2000, 34, false, separator,false);
      for(i=stringByteLength(tmpString + 600) - 1; i>0; i--) {
        if(tmpString[600 + i] == 0x08) {
          tmpString[600 + i] = 0x05;
        }
      }
  
//vv      strncat(tmpString + 300, tmpString +  600, 299); //add +i. and imag
//vv      tmpString[600] = 0;

      //Concatenate +ix and IMAG into 300
      last = 300;
      while(tmpString[last]) last++;
      xcopy(tmpString + last, tmpString + 600,  strlen(tmpString + 600) + 1);
      tmpString[600] = 0;
//^^
      //Check if REAL + IMAG fits into two lines
      if(stringWidth(tmpString, &numericFont, true, true) + stringWidth(tmpString + 300, &numericFont, true, true) <= 2*SCREEN_WIDTH) {
      //  if it fits, copy all into +0
        int xx = stringByteLength(tmpString) + stringByteLength(tmpString + 300);
//        strncat(tmpString, tmpString +  300, 299);
        xcopy(tmpString + stringByteLength(tmpString), tmpString + 300,stringByteLength(tmpString + 300));
        tmpString[xx] = 0;
        tmpString[300] = 0;
      }
  
//vv      strncat(tmpString + 2103, tmpString + 0, 299-3);  //COPY REAL
//vv      tmpString[0] = 0;


      //copy +0 REAL or REAL+IMAG result into destination 2100 (label already in 2100-2102)
      last = 2100;
      while(tmpString[last]) last++; 
      xcopy(tmpString + last, tmpString + 0,  strlen(tmpString + 0) + 1);
      tmpString[0] = 0;
//^^
  
//vv      strcpy(tmpString + 2400, tmpString + 300);        //COPY IMAG
//vv      tmpString[300] = 0;
//new

      //copy IMAG result into +2400
      last = 2400;
//      while(tmpString[last]) last++;
      xcopy(tmpString + last, tmpString + 300,  strlen(tmpString + 300) + 1);
      tmpString[300] = 0;
//^^
  

      //write 2100+ into two lines, 0+ and 300+
      last = 2100 + stringByteLength(tmpString + 2100);
      source = 2100;
      for(d=0; d<=300 ; d+=300) {
        dest = d;
        while(source < last && stringWidth(tmpString + d, &numericFont, true, true) <= SCREEN_WIDTH - 8*2) {
          tmpString[dest] = tmpString[source];
          if(tmpString[dest] & 0x80) {
            tmpString[++dest] = tmpString[++source];
          }
          source++;
          tmpString[++dest] = 0;
        }
      }
      
      //write 2400+ into two lines, 300+ and 900+
      last = 2400 + stringByteLength(tmpString + 2400);
      source = 2400;
      for(d=600; d<=900 ; d+=300) {
        dest = d;
        while(source < last && stringWidth(tmpString + d, &numericFont, true, true) <= SCREEN_WIDTH - 8*2) {
          tmpString[dest] = tmpString[source];
          if(tmpString[dest] & 0x80) {
            tmpString[++dest] = tmpString[++source];
          }
          source++;
          tmpString[++dest] = 0;
        }
      }
  
      if (tmpString[300]==0) {                          //shift up if line is empty
        //vv new       strcpy(tmpString + 300, tmpString + 600);
        xcopy(tmpString + 300, tmpString + 600,  min(300,strlen(tmpString + 600) + 1));
        //vv new        strcpy(tmpString + 600, tmpString + 900);
        xcopy(tmpString + 600, tmpString + 900,  min(300,strlen(tmpString + 900) + 1));
        tmpString[900] = 0;
      }
  
      if (tmpString[600]==0) {                          //shift up if line is empty
        //vv new        strcpy(tmpString + 600, tmpString + 900);
        xcopy(tmpString + 600, tmpString + 900,  min(300,strlen(tmpString + 900) + 1));
        tmpString[900] = 0;
      }
      break;


    case dtShortInteger:
      #if defined (VERBOSE_SCREEN) && defined (PC_BUILD)
        printf("SHOW:Shortint\n");
      #endif
      temporaryInformation = TI_SHOW_REGISTER_BIG;

      shortIntegerToDisplayString(SHOWregis, tmpString + 2103, true);
    
      if(getRegisterTag(SHOWregis) == 2) {
        source = 2100;
        dest = 2400;
        while(tmpString[source] !=0 ) {
          if((uint8_t)(tmpString[source]) == 160 && (uint8_t)(tmpString[source+1]) == 39) {
            source++;
            tmpString[dest++]=49;
          } else
            if((uint8_t)(tmpString[source]) == 162 && (uint8_t)(tmpString[source+1]) == 14) {
              source++;
              tmpString[dest++]=48;
            } else {
              tmpString[dest++] = tmpString[source];
            }
          source++;
        }
        tmpString[dest]=0;
      } else {
        strcpy(tmpString + 2400,tmpString + 2100);
      }
 	

      last = 2400 + stringByteLength(tmpString + 2400);
      source = 2400;
      for(d=0; d<=900 ; d+=300) {
        dest = d;
        if(dest != 0){strcat(tmpString + dest,"  ");dest+=2;}               //space below the T:
        while(source < last && stringWidth(tmpString + d, &numericFont, true, true) <= SCREEN_WIDTH - 8*2) {
          tmpString[dest] = tmpString[source];
          if(tmpString[dest] & 0x80) {
            tmpString[++dest] = tmpString[++source];
          }
          source++;
          tmpString[++dest] = 0;
        }
        checkAndEat(&source, last, &dest);
      }

      convertShortIntegerRegisterToUInt64(REGISTER_X, &aa, &nn);
      aa = getRegisterTag(SHOWregis);
      switch(aa){
      	case 2: if(nn <= 0x00000000FFFFFFFF) {
      		      aa2=0;  aa3=8;  aa4=16; break;
      	        } else {
      	          aa2=0;  aa3=0;  aa4=0; break;
      	        }
      	case 3: if(nn <= 0x00000000FFFFFFFF) {
      		      aa2=0;  aa3=8;  aa4=16; break;
      	        } else {
      	          aa2=0;  aa3=0;  aa4=16; break;
      	        }
      	case 4: if(nn <= 0x00000000FFFFFFFF) {
      		      aa2=8;  aa3=10;  aa4=16; break;
      	        } else {
      	          aa2=0;  aa3=8;  aa4=16; break;
      	        }
      	case 5:
      	case 6:
      	case 7: if(nn <= 0x00000000FFFFFFFF) {
      		      aa2=8;  aa3=10;  aa4=16; break;
      	        } else 
      	        if(nn <= 0x003FFFFFFFFFFFFF) {
      	          aa2=0;  aa3=8;  aa4=16; break;
      	        } else {
      	          aa2=0;  aa3=0;  aa4=16; break;
      	        }
      	case  9:
      	case 11: if(nn <= 0x001FFFFFFFFFFFFF) {
      	           aa2= 8;  aa3=10;  aa4=16; break;
      	         } else {
      	           aa2= 0;  aa3= 8;  aa4=16; break;
      	         }
      	case 12:
      	case 13:
      	case 14: 
      	case 15: if(nn <= 0x001FFFFFFFFFFFFF) {
      	           aa2= 8;  aa3=10;  aa4=16; break;
      	         } else {
      	           aa2=10;  aa3= 0;  aa4=16; break;
      	         }
      	case 16: if(nn <= 0x00000000FFFFFFFF) {
      	           aa2=4;  aa3= 8;  aa4=10; break;
      	         } else {
      	           aa2=10;  aa3= 0;  aa4=8; break;
      	         }
      	case 10: if(nn <= 0x00000000FFFFFFFF) {
      	           aa2=4;  aa3= 8;  aa4=16; break;
      	         } else {
      	           aa2=0;  aa3= 8;  aa4=16; break;
      	         }
      	case  8: if(nn <= 0x00000000FFFFFFFF) {
      	           aa2=4;  aa3=10;  aa4=16; break;
      	         } else {
      	           aa2=10;  aa3=0;  aa4=16; break;
      	         }
      }


      if(aa2){
        setRegisterTag(SHOWregis,aa2);
        shortIntegerToDisplayString(SHOWregis, tmpString + 2103, true);
        strcpy(tmpString + 2400,tmpString + 2100);
        last = 2400 + stringByteLength(tmpString + 2400);
        source = 2400;
        tmpString[300]=0;
        for(d=300; d<=900 ; d+=300) {
          dest = d;
          if(dest != 300){strcat(tmpString + dest,"  ");dest+=2;}               //space below the T:
          while(source < last && stringWidth(tmpString + d, &numericFont, true, true) <= SCREEN_WIDTH - 8*2) {
            tmpString[dest] = tmpString[source];
            if(tmpString[dest] & 0x80) {
              tmpString[++dest] = tmpString[++source];
            }
            source++;
            tmpString[++dest] = 0;
          }
          checkAndEat(&source, last, &dest);
        }
      }
      if(aa3){
        setRegisterTag(SHOWregis,aa3);
        shortIntegerToDisplayString(SHOWregis, tmpString + 2103, true);
        strcpy(tmpString + 2400,tmpString + 2100);
        last = 2400 + stringByteLength(tmpString + 2400);
        source = 2400;
        tmpString[600]=0;
        for(d=600; d<=900 ; d+=300) {
          dest = d;
          if(dest != 600){strcat(tmpString + dest,"  ");dest+=2;}               //space below the T:
          while(source < last && stringWidth(tmpString + d, &numericFont, true, true) <= SCREEN_WIDTH - 8*2) {
            tmpString[dest] = tmpString[source];
            if(tmpString[dest] & 0x80) {
              tmpString[++dest] = tmpString[++source];
            }
            source++;
            tmpString[++dest] = 0;
          }
          checkAndEat(&source, last, &dest);
        }
      }
      if(aa4){
        setRegisterTag(SHOWregis,aa4);
        shortIntegerToDisplayString(SHOWregis, tmpString + 2103, true);
        strcpy(tmpString + 2400,tmpString + 2100);
        last = 2400 + stringByteLength(tmpString + 2400);
        source = 2400;
        tmpString[900]=0;
        for(d=900; d<=900 ; d+=300) {
          dest = d;
          if(dest != 900){strcat(tmpString + dest,"  ");dest+=2;}               //space below the T:
          while(source < last && stringWidth(tmpString + d, &numericFont, true, true) <= SCREEN_WIDTH - 8*2) {
            tmpString[dest] = tmpString[source];
            if(tmpString[dest] & 0x80) {
              tmpString[++dest] = tmpString[++source];
            }
            source++;
            tmpString[++dest] = 0;
          }
          checkAndEat(&source, last, &dest);
        }
      }
      setRegisterTag(SHOWregis,aa);
      break;

    case dtTime:
      //SHOW_reset();
      strcpy(tmpString, tmpString + 2100);
      temporaryInformation = TI_SHOW_REGISTER_BIG;
      timeToDisplayString(SHOWregis, tmpString + 3, true);
      break;

    case dtDate:
      //SHOW_reset();
      strcpy(tmpString, tmpString + 2100);
      temporaryInformation = TI_SHOW_REGISTER_BIG;
      dateToDisplayString(SHOWregis, tmpString + 3);
      break;


    case dtString:
      #if defined (VERBOSE_SCREEN) && defined (PC_BUILD)
        printf("SHOW:String\n");
      #endif //VERBOSE_SCREEN

      SHOW_reset();
      temporaryInformation = TI_SHOW_REGISTER_BIG; //First try one line of big font.
      offset = 0;
      thereIsANextLine = true;
      bytesProcessed = 2100;
      strcat(tmpString + 2100, "'");
      strcat(tmpString + 2100, REGISTER_STRING_DATA(SHOWregis));//, stringByteLength(REGISTER_STRING_DATA(SHOWregis)) + 4+1);
      strcat(tmpString + 2100, "'");
      while(thereIsANextLine) {
        xcopy(tmpString + offset, tmpString + bytesProcessed, stringByteLength(tmpString + bytesProcessed) + 1);
        thereIsANextLine = false;
        #if defined VERBOSE_SCREEN && defined PC_BUILD
          uint32_t tmp = 0;
          printf("^^^0 %4u",tmp);
          printf("^^^^$$ %s %d\n",tmpString + 2100,stringWidthC43(tmpString + 2100, stdnumEnlarge, nocompress, false, true));
        #endif //VERBOSE_SCREEN
        while(stringWidthC43(tmpString + offset, stdnumEnlarge, nocompress, false, true) >= SCREEN_WIDTH) {
          tmpString[offset + stringLastGlyph(tmpString + offset)] = 0;
          thereIsANextLine = true;
          #if defined (VERBOSE_SCREEN) && defined (PC_BUILD)
            printf("^^^A %4u",tmp++);
            printf("^^^^$$ %s %d\n",tmpString + offset,stringWidthC43(tmpString + offset, stdnumEnlarge, nocompress, false, true));
          #endif //VERBOSE_SCREEN
        }
        bytesProcessed += stringByteLength(tmpString + offset);
        offset += 300;
        tmpString[offset] = 0;
      }
      if(offset <= 1200) break; //else continue on the small font


      SHOW_reset();
      temporaryInformation = TI_SHOW_REGISTER_SMALL;
      offset = 0;
      thereIsANextLine = true;
      bytesProcessed = 2100;
      strcat(tmpString + 2100, "'");
      strcat(tmpString + 2100, REGISTER_STRING_DATA(SHOWregis));//, stringByteLength(REGISTER_STRING_DATA(SHOWregis)) + 4+1);
      strcat(tmpString + 2100, "'");
      #if defined (VERBOSE_SCREEN) && defined (PC_BUILD)
        uint32_t tmp2 = 0;
      #endif //VERBOSE_SCREEN
      while(thereIsANextLine) {
        xcopy(tmpString + offset, tmpString + bytesProcessed, stringByteLength(tmpString + bytesProcessed) + 1);
        thereIsANextLine = false;
        #if defined (VERBOSE_SCREEN) && defined (PC_BUILD)
          tmp =0;
        #endif //VERBOSE_SCREEN
        while(stringWidth(tmpString + offset, &standardFont, false, true) >= SCREEN_WIDTH) {
          tmpString[offset + stringLastGlyph(tmpString + offset)] = 0;
          thereIsANextLine = true;
          #if defined (VERBOSE_SCREEN) && defined (PC_BUILD)
            printf("^^^B %4u %4u",tmp2, tmp++);
            printf("^^^^$$ %s %d\n",tmpString + offset,stringWidth(tmpString + offset, &standardFont, false, true));
          #endif //VERBOSE_SCREEN
        }
        #if defined (VERBOSE_SCREEN) && defined (PC_BUILD)
          tmp2++;
        #endif //VERBOSE_SCREEN
        bytesProcessed += stringByteLength(tmpString + offset);
        offset += 300;
        tmpString[offset] = 0;
      }
      break;


    case dtConfig:
      temporaryInformation = TI_SHOW_REGISTER_BIG;
      xcopy(tmpString, "Configuration data", 19);
      break;


    default:
      temporaryInformation = TI_NO_INFO;
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, SHOWregis);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "cannot SHOW %s%s", tmpString + 2100, getRegisterDataTypeName(SHOWregis, true, false));
        moreInfoOnError("In function fnShow:", errorMessage, NULL, NULL);
      #endif
      return;
  }


  displayFormat = savedDisplayFormat;
  displayFormatDigits = savedDisplayFormatDigits;
  SigFigMode = savedSigFigMode;                            //JM SIGFIG
  UNITDisplay = savedUNITDisplay;                          //JM SIGFIG
  #if defined (VERBOSE_SCREEN) && defined (PC_BUILD)
    printf("SHOW:Done |%s|\n",tmpString);
  #endif

#endif //TESTSUITE_BUILD

}
