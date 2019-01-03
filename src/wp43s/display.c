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

#include "wp43s.h"

/********************************************//**
 * \brief Sets the display override mode and refreshes the stack
 *
 * \param[in] displayOvr uint16_t Display override mode
 * \return void
 ***********************************************/
void fnDisplayOvr(uint16_t displayOvr) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnDisplayOvr");
  #endif

  displayModeOverride = displayOvr;
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnDisplayOvr");
  #endif
}



/********************************************//**
 * \brief Sets the product sign and refreshes the stack
 *
 * \param[in] productSign uint16_t Product sign
 * \return void
 ***********************************************/
void fnProductSign(uint16_t ps) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnProductSign");
  #endif

  productSign = ps;
  showFracMode();
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnProductSign");
  #endif
}



/********************************************//**
 * \brief Sets the radix mark and refreshes the stack
 *
 * \param[in] radixMark uint16_t Radix mark
 * \return void
 *
 ***********************************************/
void fnRadixMark(uint16_t rm) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnRadixMark");
  #endif

  radixMark = rm;
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnRadixMark");
  #endif
}



/********************************************//**
 * \brief Sets the display format FIX and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatFix(uint16_t displayFormatN) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnDisplayFormatFix");
  #endif

  displayFormat = DF_FIX;
  displayFormatDigits = displayFormatN;
  displayRealAsFraction = false;

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }
  #ifdef PC_BUILD
  else if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    showInfoDialog("In function fnDisplayFormatFix:", "converting an integer to a real16", "is to be coded", NULL);
  }
  #endif

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnDisplayFormatFix");
  #endif
}



/********************************************//**
 * \brief Sets the display format SCI and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatSci(uint16_t displayFormatN) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnDisplayFormatSci");
  #endif

  displayFormat = DF_SCI;
  displayFormatDigits = displayFormatN;
  displayRealAsFraction = false;

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }
  #ifdef PC_BUILD
  else if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    showInfoDialog("In function fnDisplayFormatSci:", "converting an integer to a real16", "is to be coded", NULL);
  }
  #endif

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnDisplayFormatSci");
  #endif
}



/********************************************//**
 * \brief Sets the display format ENG and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatEng(uint16_t displayFormatN) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnDisplayFormatEng");
  #endif

  displayFormat = DF_ENG;
  displayFormatDigits = displayFormatN;
  displayRealAsFraction = false;

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }
  #ifdef PC_BUILD
    else if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
      showInfoDialog("In function fnDisplayFormatEng:", "converting an integer to a real16", "is to be coded", NULL);
  }
  #endif

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnDisplayFormatEng");
  #endif
}



/********************************************//**
 * \brief Sets the display format ALL and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatAll(uint16_t displayFormatN) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnDisplayFormatAll");
  #endif

  displayFormat = DF_ALL;
  displayFormatDigits = displayFormatN;
  displayRealAsFraction = false;

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }
  #ifdef PC_BUILD
    else if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
      showInfoDialog("In function fnDisplayFormatAll:", "converting an integer to a real16", "is to be coded", NULL);
  }
  #endif

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnDisplayFormatAll");
  #endif
}



/********************************************//**
 * \brief Sets the number of digits afer the period and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatDsp(uint16_t displayFormatN) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnDisplayFormatDsp");
  #endif

  displayFormatDigits = displayFormatN;
  displayRealAsFraction = false;

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }
  #ifdef PC_BUILD
    else if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
      showInfoDialog("In function fnDisplayFormatDsp:", "converting an integer to a real16", "is to be coded", NULL);
  }
  #endif

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnDisplayFormatDsp");
  #endif
}



/********************************************//**
 * \brief Sets the GAP in number represemtation and refreshes the stack
 *
 * \param[in] gap uint16_t
 * \return void
 ***********************************************/
void fnDisplayFormatGap(uint16_t gap) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnDisplayFormatGap");
  #endif

  groupingGap = gap;
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnDisplayFormatGap");
  #endif
}



/********************************************//**
 * \brief Adds the power of 10 using numeric font to displayString
 *
 * \param[out] displayString char*     Result string
 * \param[in]  exponent int32_t Power of 10 to format
 * \return void
 ***********************************************/
void exponentToDisplayString(int32_t exponent, char *displayString) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("exponentToDisplayString");
  #endif

  strcpy(displayString, PRODUCT_SIGN);
  strcpy(displayString+2, NUM_SUB_10);
  supNumberToDisplayString(exponent, displayString+4);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("exponentToDisplayString");
  #endif
}



void supNumberToDisplayString(int32_t supNumber, char *displayString) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("supNumberToDisplayString");
  #endif

  if(supNumber < 0) {
    supNumber = -supNumber;
    strcat(displayString, NUM_SUP_MINUS);
    displayString += 2;
  }

  if(supNumber == 0) {
    strcat(displayString, NUM_SUP_0);
  }
  else {
    int16_t digit;

    while(supNumber > 0) {
      digit = supNumber % 10;
      supNumber /= 10;

      memmove(displayString + 2, displayString, stringByteLength(displayString) + 1);

      if(digit <= 1) {
        strncpy(displayString, NUM_SUP_0, 2);
        displayString[1] += digit;
      }
      else if(digit <= 3) {
        strncpy(displayString, NUM_SUP_2, 2);
        displayString[1] += digit-2;
      }
      else {
        strncpy(displayString, NUM_SUP_4, 2);
        displayString[1] += digit-4;
      }
    }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("supNumberToDisplayString");
  #endif
}



void subNumberToDisplayString(int32_t subNumber, char *displayString) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("subNumberToDisplayString");
  #endif

  if(subNumber < 0) {
    subNumber = -subNumber;
    strcat(displayString, NUM_SUB_MINUS);
    displayString += 2;
  }

  if(subNumber == 0) {
    strcat(displayString, NUM_SUB_0);
  }
  else {
    int16_t digit;

    while(subNumber > 0) {
      digit = subNumber % 10;
      subNumber /= 10;

      memmove(displayString + 2, displayString, stringByteLength(displayString) + 1);

      strncpy(displayString, NUM_SUB_0, 2);
      displayString[1] += digit;
    }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("subNumberToDisplayString");
  #endif
}



/********************************************//**
 * \brief Formats a SP or real34
 *
 * \param[out] displayString char* Result string
 * \param[in]  x const real16_t*  Value to format
 * \param[in]  real34 bool_t         x is a real34
 * \return void
 ***********************************************/
void real16ToDisplayString(const real16_t *value, bool_t real34, char *displayString) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16ToDisplayString");
  #endif

  #undef MAX_DIGITS
  #define MAX_DIGITS 19 // 16 + 1 before + 2 after

  uint8_t charIndex, digitToRound;
  uint8_t *bcd;
  int16_t digitsToDisplay, numDigits, digitPointer, firstDigit, lastDigit, i, digitCount, digitsToTruncate;
  int32_t sign, exponent;
  bool_t  ovrSCI=false, ovrENG=false, firstDigitAfterPeriod=true;

  if(real16IsInfinite(value)) {
    if(real16IsNegative(value)) {
      strcpy(displayString, "-" NUM_INFINITY);
    }
    else {
      strcpy(displayString, "+" NUM_INFINITY);
    }

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("real16ToDisplayString");
    #endif

    return;
  }

  if(real16IsNaN(value)) {
    real16ToString(value, displayString);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("real16ToDisplayString");
    #endif

    return;
  }

  bcd = (uint8_t *)(tmpStr3000 + 128 - MAX_DIGITS);

  sign = real16GetCoefficient(value, bcd+1);
  exponent = real16GetExponent(value);
  bcd[0]           = 0; // used when rounding from 9.999 to 10.000
  bcd[MAX_DIGITS-2] = 0; // used for rounding and ENG display mode
  bcd[MAX_DIGITS-1] = 0; // used for ENG display mode

  // Calculate the number of significant digits
  for(digitPointer=1; digitPointer<=MAX_DIGITS-3; digitPointer++) {
    if(bcd[digitPointer] != 0) {
      break;
    }
  }

  if(digitPointer >= MAX_DIGITS-2) { // *addr = 0.0
    firstDigit = 8;
    lastDigit  = 8;
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

  charIndex = 0;

  //////////////
  // ALL mode //
  //////////////
  if(displayFormat == DF_ALL) {
    if(exponent >= 16 || (displayFormatDigits != 0 && exponent < -(int32_t)displayFormatDigits) || (displayFormatDigits == 0 && exponent < numDigits - 16)) { // Display in SCI or ENG format
      digitsToDisplay = numDigits - 1;
      digitToRound    = firstDigit + digitsToDisplay;
      ovrSCI = (displayModeOverride == DO_SCI);
      ovrENG = (displayModeOverride == DO_ENG);
    }
    else { // display all digits without ten exponent factor
      if(exponent < 0) { // negative exponent
        // Number of digits to truncate
        digitsToTruncate = max(numDigits - exponent, 16) - 16;
        numDigits -= digitsToTruncate;
        lastDigit -= digitsToTruncate;

        // Round the displayed number
        if(bcd[lastDigit+1] >= 5) {
          bcd[lastDigit]++;
        }

        // Transfert the carry
        while(bcd[lastDigit] == 10) {
          bcd[lastDigit--] = 0;
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
        }

        // first 0 and radix mark
        displayString[charIndex++] = '0';
        if(real34) {
          displayString[charIndex] = 0;
          strcat(displayString, RADIX34_MARK_STRING);
          charIndex += 2;
        }
        else {
          displayString[charIndex++] = RADIX16_MARK_CHAR;
        }

        // Zeros before first significant digit
        for(digitCount=0, i=exponent+1; i<0; i++, digitCount--) {
          if(digitCount!=0 && groupingGap!=0 && digitCount%(uint16_t)groupingGap==0) {
            memcpy(displayString + charIndex, NUM_SPACE_PUNCTUATION, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0';
        }

        // Significant digits
        for(digitPointer=firstDigit; digitPointer<firstDigit+min(15-exponent, numDigits); digitPointer++, digitCount--) {
          if(digitCount!=0 && groupingGap!=0 && digitCount%(uint16_t)groupingGap==0) {
            memcpy(displayString + charIndex, NUM_SPACE_PUNCTUATION, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0' + bcd[digitPointer];
        }
      }
      else { // zero or positive exponent
        // The sign
        if(sign) {
         displayString[charIndex++] = '-';
        }

        for(digitCount=exponent, digitPointer=firstDigit; digitPointer<=lastDigit + max(exponent - numDigits + 1, 0); digitPointer++, digitCount--) {
          if(digitCount!=-1 && digitCount!=exponent && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap)==(uint16_t)groupingGap-1) {
            memcpy(displayString + charIndex, NUM_SPACE_PUNCTUATION, 2);
            charIndex += 2;
          }

          // Significant digit or zero
          if(digitPointer <= lastDigit) {
            displayString[charIndex++] = '0' + bcd[digitPointer];
          }
          else {
            displayString[charIndex++] = '0';
          }

          // Radix mark
          if(digitCount == 0) {
            if(real34) {
              displayString[charIndex] = 0;
              strcat(displayString, RADIX34_MARK_STRING);
              charIndex += 2;
            }
            else {
              displayString[charIndex++] = RADIX16_MARK_CHAR;
            }
          }
        }
      }

      displayString[charIndex] = 0;

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("real16ToDisplayString");
      #endif

      return;
    }
  }

  //////////////
  // FIX mode //
  //////////////
  if(displayFormat == DF_FIX) {
    if(exponent >= 16 || exponent < -(int32_t)displayFormatDigits) { // Display in SCI or ENG format
      digitsToDisplay = displayFormatDigits;
      digitToRound    = min(firstDigit + digitsToDisplay, lastDigit);
      ovrSCI = (displayModeOverride == DO_SCI);
      ovrENG = (displayModeOverride == DO_ENG);
    }
    else { // display fix nunber of digits without ten exponent factor
      if(exponent < 0) { // negative exponent
        // Number of digits to truncate
        digitsToTruncate = max(numDigits - (int16_t)displayFormatDigits - exponent - 1, 0);
        numDigits -= digitsToTruncate;
        lastDigit -= digitsToTruncate;

        // Round the displayed number
        if(bcd[lastDigit+1] >= 5) {
          bcd[lastDigit]++;
        }

        // Transfert the carry
        while(bcd[lastDigit] == 10) {
          bcd[lastDigit--] = 0;
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
        }

        // first 0 and radix mark
        displayString[charIndex++] = '0';
        if(real34) {
          displayString[charIndex] = 0;
          strcat(displayString, RADIX34_MARK_STRING);
          charIndex += 2;
        }
        else {
          displayString[charIndex++] = RADIX16_MARK_CHAR;
        }

        // Zeros before first significant digit
        for(digitCount=0, i=exponent+1; i<0; i++, digitCount--) {
          if(digitCount!=0 && groupingGap!=0 && digitCount%(uint16_t)groupingGap==0) {
            memcpy(displayString + charIndex, NUM_SPACE_PUNCTUATION, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0';
        }

        // Significant digits
        for(digitPointer=firstDigit; digitPointer<=lastDigit; digitPointer++, digitCount--) {
          if(digitCount!=0 && groupingGap!=0 && digitCount%(uint16_t)groupingGap==0) {
            memcpy(displayString + charIndex, NUM_SPACE_PUNCTUATION, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0' + bcd[digitPointer];
        }

        // Zeros after last significant digit
        for(i=1; i<=(int16_t)displayFormatDigits+exponent+1-numDigits; i++, digitCount--) {
          if(groupingGap!=0 && digitCount%(uint16_t)groupingGap==0) {
            memcpy(displayString + charIndex, NUM_SPACE_PUNCTUATION, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0';
        }
      }
      else { // zero or positive exponent
        // Number of digits to truncate
        digitsToTruncate = max(numDigits - (int16_t)displayFormatDigits - exponent - 1, 0);
        numDigits -= digitsToTruncate;
        lastDigit -= digitsToTruncate;

        // Round the displayed number
        if(bcd[lastDigit+1] >= 5) {
          bcd[lastDigit]++;
        }

        // Transfert the carry
        while(bcd[lastDigit] == 10) {
          bcd[lastDigit--] = 0;
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
        }

        for(digitCount=exponent, digitPointer=firstDigit; digitPointer<=firstDigit + min(exponent + (int16_t)displayFormatDigits, 15); digitPointer++, digitCount--) {
          if(digitCount!=-1 && digitCount!=exponent && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap)==(uint16_t)groupingGap-1) {
            memcpy(displayString + charIndex, NUM_SPACE_PUNCTUATION, 2);
            charIndex += 2;
          }

          // Significant digit or zero
          if(digitPointer <= lastDigit) {
            displayString[charIndex++] = '0' + bcd[digitPointer];
          }
          else {
            displayString[charIndex++] = '0';
          }

          // Radix mark
          if(digitCount == 0) {
            if(real34) {
              displayString[charIndex] = 0;
              strcat(displayString, RADIX34_MARK_STRING);
              charIndex += 2;
            }
            else {
              displayString[charIndex++] = RADIX16_MARK_CHAR;
            }
          }
        }
      }

      displayString[charIndex] = 0;

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("real16ToDisplayString");
      #endif

      return;
    }
  }

  //////////////
  // SCI mode //
  //////////////
  if(ovrSCI || displayFormat == DF_SCI) {
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
      bcd[digitToRound]++;
    }

    // Case when 9.9999 rounds to 10.0000
    if(digitToRound < firstDigit) {
      firstDigit--;
      lastDigit = firstDigit;
      numDigits = 1;
      exponent++;
    }

    // Sign
    if(sign) {
      displayString[charIndex++] = '-';
    }

    // First digit
    displayString[charIndex++] = '0' + bcd[firstDigit];

    // Radix mark
    if(real34) {
      displayString[charIndex] = 0;
      strcat(displayString, RADIX34_MARK_STRING);
      charIndex += 2;
    }
    else {
      displayString[charIndex++] = RADIX16_MARK_CHAR;
    }

    // Significant digits
    for(digitCount=-1, digitPointer=firstDigit+1; digitPointer<firstDigit+min(numDigits, digitsToDisplay+1); digitPointer++, digitCount--) {
      if(!firstDigitAfterPeriod && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap)==(uint16_t)groupingGap-1) {
        memcpy(displayString + charIndex, NUM_SPACE_PUNCTUATION, 2);
        charIndex += 2;
      }
      else {
        firstDigitAfterPeriod = false;
      }

      displayString[charIndex++] = '0' + bcd[digitPointer];
    }

    // The ending zeros
    for(digitPointer=0; digitPointer<=digitsToDisplay-numDigits; digitPointer++, digitCount--) {
      if(!firstDigitAfterPeriod && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap)==(uint16_t)groupingGap-1) {
        memcpy(displayString + charIndex, NUM_SPACE_PUNCTUATION, 2);
        charIndex += 2;
      }
      else {
        firstDigitAfterPeriod = false;
      }

      displayString[charIndex++] = '0';
    }

    displayString[charIndex] = 0;

    if(exponent != 0) {
      exponentToDisplayString(exponent, displayString+charIndex);
    }


    #if (LOG_FUNCTIONS == 1)
      leavingFunction("real16ToDisplayString");
    #endif

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
      bcd[digitToRound]++;
    }

    // Case when 9.9999 rounds to 10.0000
    if(digitToRound < firstDigit) {
      firstDigit--;
      lastDigit = firstDigit;
      numDigits = 1;
      exponent++;
    }

    // The sign
    if(sign) {
      displayString[charIndex++] = '-';
    }

    // Digits before radix mark
    displayString[charIndex++] = '0' + bcd[firstDigit++];
    numDigits--;
    digitsToDisplay--;
    while(modulo(exponent, 3) != 0) {
      exponent--;
      displayString[charIndex++] = '0' + bcd[firstDigit++];
      numDigits--;
      digitsToDisplay--;
    }

    // Radix Mark
    if(real34) {
      displayString[charIndex] = 0;
      strcat(displayString, RADIX34_MARK_STRING);
      charIndex += 2;
    }
    else {
      displayString[charIndex++] = RADIX16_MARK_CHAR;
    }

    // Digits after radix mark
    for(digitCount=-1, digitPointer=firstDigit; digitPointer<firstDigit+min(numDigits, digitsToDisplay+1); digitPointer++, digitCount--) {
      if(!firstDigitAfterPeriod && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap)==(uint16_t)groupingGap-1) {
        memcpy(displayString + charIndex, NUM_SPACE_PUNCTUATION, 2);
        charIndex += 2;
      }
      else {
        firstDigitAfterPeriod = false;
      }

      displayString[charIndex++] = '0' + bcd[digitPointer];
    }

    // The ending zeros
    for(digitPointer=0; digitPointer<=digitsToDisplay-numDigits; digitPointer++, digitCount--) {
      if(!firstDigitAfterPeriod && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap)==(uint16_t)groupingGap-1) {
        memcpy(displayString + charIndex, NUM_SPACE_PUNCTUATION, 2);
        charIndex += 2;
      }
      else {
        firstDigitAfterPeriod = false;
      }

      displayString[charIndex++] = '0';
    }

    displayString[charIndex] = 0;

    if(exponent != 0) {
      exponentToDisplayString(exponent, displayString+charIndex);
    }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16ToDisplayString");
  #endif
}



/********************************************//**
 * \brief Formats a real34
 *
 * \param[out] displayString char* Result string
 * \param[in]  x const real34_t*  Value to format
 * \return void
 ***********************************************/
void real34ToDisplayString(const real34_t *real34, char *displayString) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34ToDisplayString");
  #endif

  real16_t tmp;

  real34ToReal16(real34, &tmp);
  real16ToDisplayString(&tmp, true, displayString);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34ToDisplayString");
  #endif
}



void complex16ToDisplayString(const complex16_t *complex16, char *displayString) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("complex16ToDisplayString");
  #endif

  int16_t i=64;
  real16_t real16, imag16;

  if(complexMode == CM_RECTANGULAR) {
    real16Copy(REAL16_POINTER(complex16), &real16);
    real16Copy(COMPLEX16_IMAGINARY_PART_POINTER(complex16), &imag16);
  }
  else if(complexMode == CM_POLAR) {
    real16RectangularToPolar(REAL16_POINTER(complex16), COMPLEX16_IMAGINARY_PART_POINTER(complex16), &real16, &imag16);
  }
  else {
    sprintf(errorMessage, "In function complex16ToDisplayString: %d is an unexpected value for complexMode!", complexMode);
    displayBugScreen(errorMessage);
  }

  real16ToDisplayString(&real16, false, displayString);
  real16ToDisplayString(&imag16, false, displayString + i);

  if(complexMode == CM_RECTANGULAR) {
    if(displayString[i] == '-') {
      strcat(displayString, "-");
      i++;
    }
    else {
      strcat(displayString, "+");
    }

    strcat(displayString, COMPLEX_UNIT);
    strcat(displayString, PRODUCT_SIGN);
    strcat(displayString, displayString + i);
  }
  else { // POLAR
    strcat(displayString, STD_SPACE_4_PER_EM STD_MEASURED_ANGLE STD_SPACE_4_PER_EM);
    angle16ToDisplayString(&imag16, displayString + stringByteLength(displayString));
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("complex16ToDisplayString");
  #endif
}



void complex34ToDisplayString(const complex34_t *complex34, char *displayString) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("complex34ToDisplayString");
  #endif

  int16_t i=64;
  real34_t real34, imag34;

  if(complexMode == CM_RECTANGULAR) {
    real34Copy(REAL34_POINTER(complex34), &real34);
    real34Copy(COMPLEX34_IMAGINARY_PART_POINTER(complex34), &imag34);
  }
  else if(complexMode == CM_POLAR) {
    real34RectangularToPolar(REAL34_POINTER(complex34), COMPLEX34_IMAGINARY_PART_POINTER(complex34), &real34, &imag34);
  }
  else {
    sprintf(errorMessage, "In function complex34ToDisplayString: %d is an unexpected value for complexMode!", complexMode);
    displayBugScreen(errorMessage);
  }

  real34ToDisplayString(&real34, displayString);
  real34ToDisplayString(&imag34, displayString + i);

  if(complexMode == CM_RECTANGULAR) {
    if(displayString[i] == '-') {
      strcat(displayString, "-");
      i++;
    }
    else {
      strcat(displayString, "+");
    }

    strcat(displayString, COMPLEX_UNIT);
    strcat(displayString, PRODUCT_SIGN);
    strcat(displayString, displayString + i);
  }
  else { // POLAR
    strcat(displayString, STD_SPACE_4_PER_EM STD_MEASURED_ANGLE STD_SPACE_4_PER_EM);
    angle34ToDisplayString(&imag34, displayString + stringByteLength(displayString));
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("complex34ToDisplayString");
  #endif
}



/********************************************//**
 * \brief formats a fraction
 *
 * \param regist calcRegister_t
 * \param displayString char*
 * \return void
 ***********************************************/
void fractionToDisplayString(calcRegister_t regist, char *displayString) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fractionToDisplayString");
  #endif

  int16_t  sign, lessEqualGreater;
  uint64_t intPart, numer, denom;
  int16_t  u, insertAt, endingZero, gap;

  //printf("regist = "); printRegisterToConsole(regist); printf("\n");
  fraction(regist, &sign, &intPart, &numer, &denom, &lessEqualGreater);
  //printf("result of fraction(...) = %c%" FMT64U " %" FMT64U "/%" FMT64U "\n", sign==-1 ? '-' : ' ', intPart, numer, denom);

  if(fractionType == FT_PROPER) { // a b/c
    if(sign == -1) {
      displayString[0] = '-';
      displayString[1] = 0;
      endingZero = 1;
    }
    else {
      displayString[0] = 0;
      endingZero = 0;
    }

    // Integer part
    insertAt = endingZero;
    gap = -1;
    do {
      gap++;
      if(gap == groupingGap) {
        gap = 0;
        endingZero++;
        memmove(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);
        strncpy(displayString+insertAt, NUM_SPACE_PUNCTUATION, 2);
      }

      u = intPart % 10;
      intPart /= 10;
      endingZero++;
      memmove(displayString + insertAt + 1, displayString + insertAt, endingZero - insertAt);
      displayString[insertAt] = '0' + u;
    } while(intPart != 0);

    strcat(displayString, NUM_SPACE_PUNCTUATION);
    endingZero += 2;
  }

  else { // FT_IMPROPER d/c
    if(sign == -1) {
      strcpy(displayString, NUM_SUP_MINUS);
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
      memmove(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);
      strncpy(displayString+insertAt, NUM_SPACE_PUNCTUATION, 2);
    }

    u = numer % 10;
    numer /= 10;
    endingZero++;
    memmove(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);

    if(u <= 1) {
      strncpy(displayString + insertAt, NUM_SUP_0, 2);
      displayString[insertAt+1] += u;
    }
    else if(u <= 3) {
      strncpy(displayString + insertAt, NUM_SUP_2, 2);
      displayString[insertAt+1] += u-2;
    }
    else {
      strncpy(displayString + insertAt, NUM_SUP_4, 2);
      displayString[insertAt+1] += u-4;
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
      memmove(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);
      strncpy(displayString+insertAt, NUM_SPACE_PUNCTUATION, 2);
    }

    u = denom % 10;
    denom /= 10;
    endingZero++;
    memmove(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);
    strncpy(displayString + insertAt, NUM_SUB_0, 2);
    displayString[insertAt+1] += u;
  } while(denom != 0);

  // Comparison sign
       if(lessEqualGreater == -1) strcat(displayString, NUM_SPACE_PUNCTUATION "<");
  else if(lessEqualGreater ==  0) strcat(displayString, NUM_SPACE_PUNCTUATION "=");
  else if(lessEqualGreater ==  1) strcat(displayString, NUM_SPACE_PUNCTUATION ">");
  else {
    strcat(displayString, NUM_SPACE_PUNCTUATION "?");
    sprintf(errorMessage, "In function fractionToDisplayString: %d is an unexpected value for lessEqualGreater!", lessEqualGreater);
    displayBugScreen(errorMessage);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fractionToDisplayString");
  #endif
}



void registerAngleToDisplayString(calcRegister_t regist, char *displayString) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("registerAngleToDisplayString");
  #endif

  if(getRegisterDataType(regist) == dtReal16) {
    angle16ToDisplayString(REAL16_POINTER(POINTER_TO_REGISTER_DATA(regist)), displayString);
  }
  else if(getRegisterDataType(regist) == dtReal34) {
    angle34ToDisplayString(REAL34_POINTER(POINTER_TO_REGISTER_DATA(regist)), displayString);
  }
  else {
    sprintf(errorMessage, "In function registerAngleToDisplayString: register %d is %s which cannot be represented as an angle!", regist, getDataTypeName(getRegisterDataType(regist), true, false));
    displayBugScreen(errorMessage);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("registerAngleToDisplayString");
  #endif
}



void angle16ToDisplayString(const real16_t *angle, char *displayString) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("angle16ToDisplayString");
  #endif

  if(displayAngularMode == AM_DMS) {
    char degStr[27];
    uint32_t m, s, fs;
    int16_t sign;

    real16_t temp0;
    real16_t temp1; // degrees
    real16_t temp3; // minutes
    real16_t temp4; // seconds

    real16Copy(angle, &temp0);

    sign = real16IsNegative(&temp0) ? -1 : 1;
    real16SetPositiveSign(&temp0);

    // Get the degrees
    real16ToIntegral(&temp0, &temp1);

    // Get the minutes
    real16Subtract(&temp0, &temp1, &temp0);
    real16Multiply(&temp0, const16_100, &temp0);
    real16ToIntegral(&temp0, &temp3);

    // Get the seconds
    real16Subtract(&temp0, &temp3, &temp0);
    real16Multiply(&temp0, const16_100, &temp0);
    real16ToIntegral(&temp0, &temp4);

    // Get the fractional seconds
    real16Subtract(&temp0, &temp4, &temp0);
    real16Multiply(&temp0, const16_100, &temp0);
    real34ToIntegralRound(&temp0, &temp0);

    fs = real16ToUInt32(&temp0);
    s  = real16ToUInt32(&temp4);
    m  = real16ToUInt32(&temp3);

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
      real16Add(&temp1, const16_1, &temp1);
    }

    real16ToString(&temp1, degStr);
    sprintf(displayString, "%s%s" STD_DEGREE "%s%" FMT32U STD_QUOTE "%s%" FMT32U "%s%02" FMT32U STD_DOUBLE_QUOTE,
                            sign==-1 ? "-" : "",
                              degStr,         m < 10 ? STD_SPACE_FIGURE : "",
                                               m,                    s < 10 ? STD_SPACE_FIGURE : "",
                                                                       s,         RADIX16_MARK_STRING,
                                                                                    fs);
  }
  else {
    real16ToDisplayString(angle, false, displayString);

         if(displayAngularMode == AM_DEGREE) strcat(displayString, STD_DEGREE);
    else if(displayAngularMode == AM_GRAD)   strcat(displayString, STD_SUP_g);
    else if(displayAngularMode == AM_MULTPI) strcat(displayString, STD_pi);
    else if(displayAngularMode == AM_RADIAN) strcat(displayString, STD_SUP_r);
    else {
      strcat(displayString, "?");
      sprintf(errorMessage, "In function angle16ToDisplayString: %" FMT8U " is an unexpected value for displayAngularMode!", displayAngularMode);
      displayBugScreen(errorMessage);
    }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("angle16ToDisplayString");
  #endif
}



void angle34ToDisplayString(const real34_t *angle34, char *displayString) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("angle34ToDisplayString");
  #endif

  real16_t angle16;

  real34ToReal16(angle34, &angle16);
  angle16ToDisplayString(&angle16, displayString);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("angle34ToDisplayString");
  #endif
}



void smallIntegerToDisplayString(calcRegister_t regist, char *displayString, const font_t **font) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("smallIntegerToDisplayString");
  #endif

  int16_t i, j, k, unit, gap, digit, bitsPerDigit, maxDigits, base;
  uint64_t number, sign;
  static const char digits[17] = "0123456789ABCDEF";

  base    = getRegisterBase(regist);
  number  = *(uint64_t *)(POINTER_TO_REGISTER_DATA(regist));

  if(base <= 1 || base >= 17) {
    sprintf(errorMessage, "In function smallIntegerToDisplayString: %d is an unexpected value for base!", base);
    displayBugScreen(errorMessage);
    base = 10;
  }

  number &= smallIntegerMask;

  if(smallIntegerMode == SIM_UNSIGN || base == 2 || base == 4 || base == 8 || base == 16) {
    sign = 0;
  }
  else {
    sign = number & smallIntegerSignBit;
  }

  if(sign) {
    if(smallIntegerMode == SIM_2COMPL) {
      number |= ~smallIntegerMask;
      number = ~number + 1;
    }
    else if(smallIntegerMode == SIM_1COMPL) {
      number = ~number;
    }
    else if(smallIntegerMode == SIM_SIGNMT) {
      number &= ~smallIntegerSignBit;
    }
    else {
      sprintf(errorMessage, "In function smallIntegerToDisplayString: %d is an unexpected value for smallIntegerMode!", smallIntegerMode);
      displayBugScreen(errorMessage);
    }

    number &= smallIntegerMask;
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
    else if(base == 4 || base == 8 || base == 16) {
      gap = 2;
    }
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
  if(displayLeadingZeros) {
         if(base ==  2) bitsPerDigit = 1;
    else if(base ==  4) bitsPerDigit = 2;
    else if(base ==  8) bitsPerDigit = 3;
    else if(base == 16) bitsPerDigit = 4;
    else                bitsPerDigit = 0;

    if(bitsPerDigit != 0) {
      maxDigits = smallIntegerWordSize / bitsPerDigit;
      if(smallIntegerWordSize%bitsPerDigit) {
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

  if(*font == NULL) { // The font is not yet determined
    // 1st try: numeric font digits from 30 to 39
    *font = &numericFont;

    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = 0xa0;
        displayString[j]   = 0x08;
      }
      else {
        displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, NUM_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, *font, false, false) < SCREEN_WIDTH) {

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("smallIntegerToDisplayString");
      #endif

      return;
    }

    // 2nd try: numeric font digits from 2487 to 2490
    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = 0xa0;
        displayString[j]   = 0x08;
      }
      else if(0x30 <= displayString[k] && displayString[k] <= 0x39) {
        displayString[j++] = 0xa4;
        displayString[j] = 0x87 - '0' + displayString[k];
      }
      else {
        displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, NUM_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, *font, false, false) < SCREEN_WIDTH) {

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("smallIntegerToDisplayString");
      #endif

      return;
    }

    // 3rd try: standard font digits from 30 to 39
    *font = &standardFont;

    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = 0xa0;
        displayString[j]   = 0x08;
      }
      else {
       displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, NUM_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, *font, false, false) < SCREEN_WIDTH) {

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("smallIntegerToDisplayString");
      #endif

      return;
    }

    // 4th and last try: standard font digits 220e and 2064 (binary)
    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = 0xa0;
        displayString[j]   = 0x08;
      }
      else if(displayString[k] == '0') {
        displayString[j++] = 0xa2;
        displayString[j] = 0x0e;
      }
      else if(displayString[k] == '1') {
        displayString[j++] = 0xa0;
        displayString[j] = 0x27;
      }
      else {
        displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, NUM_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, &standardFont, false, false) < SCREEN_WIDTH) {

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("smallIntegerToDisplayString");
      #endif

      return;
    }

    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function smallIntegerToDisplayString: the integer data representation is too wide (1)!", displayString, NULL, NULL);
    #endif

    strcpy(displayString, "Integer data representation to wide!");
  }

  else { // the font is already determined (standard font)
    *font = &standardFont;

    // 1st try: standard font digits from 30 to 39
    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = 0xa0;
        displayString[j]   = 0x08;
      }
      else {
        displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, NUM_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, *font, false, false) < SCREEN_WIDTH) {

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("smallIntegerToDisplayString");
      #endif

      return;
    }

    // 2nd and last try: standard font digits 220e and 2064 (binary)
    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = 0xa0;
        displayString[j]   = 0x08;
      }
      else if(displayString[k] == '0') {
        displayString[j++] = 0xa2;
        displayString[j] = 0x0e;
      }
      else if(displayString[k] == '1') {
        displayString[j++] = 0xa0;
        displayString[j] = 0x27;
      }
      else {
       displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, NUM_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, *font, false, false) < SCREEN_WIDTH) {

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("smallIntegerToDisplayString");
      #endif

      return;
    }

    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function smallIntegerToDisplayString: the integer data representation is too wide (2)!", displayString, NULL, NULL);
    #endif

    strcpy(displayString, "Integer data representation to wide!");
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("smallIntegerToDisplayString");
  #endif
}



void bigIntegerToDisplayString(calcRegister_t regist, char *displayString) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("bigIntegerToDisplayString");
  #endif

  int16_t len;
  bigInteger_t tempBigInteger;

  convertBigIntegerRegisterToBigInteger(regist, &tempBigInteger);
  bigIntegerToString(&tempBigInteger, displayString, 10);

  if(groupingGap > 0) {
    len = strlen(displayString);
    for(int16_t i=len - groupingGap; i>0; i-=groupingGap) {
      if(i != 1 || displayString[0] != '-') {
        memmove(displayString + i + 2, displayString + i, len - i + 1);
        displayString[i] = 0xa0;
        displayString[i+1] = 0x08;
        len += 2;
      }
    }
  }

  if(stringWidth(displayString, &standardFont, false, false) > SCREEN_WIDTH - 2) {
    char exponentString[14];
    int16_t lastChar, stringStep, exponentStep, tenExponent;

    stringStep = (groupingGap == 0 ? 1 : groupingGap + 2);
    exponentStep = (groupingGap == 0 ? 1 : groupingGap);
    tenExponent = exponentStep;
    lastChar = strlen(displayString) - stringStep;
    displayString[lastChar] = 0;
    exponentString[0] = 0;
    exponentToDisplayString(tenExponent, exponentString);
    while(stringWidth(displayString, &standardFont, false, true) + stringWidth(exponentString, &standardFont, true, false) > SCREEN_WIDTH - 2) {
      lastChar -= stringStep;
      tenExponent += exponentStep;
      displayString[lastChar] = 0;
      exponentString[0] = 0;
      exponentToDisplayString(tenExponent, exponentString);
    }

    strcat(displayString, exponentString);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("bigIntegerToDisplayString");
  #endif
}



void dateToDisplayString(calcRegister_t regist, char *displayString) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("dateToDisplayString");
  #endif

  sprintf(displayString, "%" FMT64S, *(int64_t *)(POINTER_TO_REGISTER_DATA(regist)));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("dateToDisplayString");
  #endif
}



void timeToDisplayString(calcRegister_t regist, char *displayString) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("timeToDisplayString");
  #endif

  sprintf(displayString, "%" FMT64S, *(int64_t *)(POINTER_TO_REGISTER_DATA(regist)));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("timeToDisplayString");
  #endif
}
