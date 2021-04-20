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
 * \file bufferize.c
 ***********************************************/

#include "bufferize.h"

#include "addons.h"
#include "charString.h"
#include "constantPointers.h"
#include "constants.h"
#include "conversionAngles.h"
#include "dateTime.h"
#include "debug.h"
#include "display.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "gui.h"
#include "items.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/toRect.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "saveRestoreCalcState.h"
#include "screen.h"
#include "softmenus.h"
#include "sort.h"
#include "stack.h"
#include "timer.h"
#include "ui/tam.h"
#include <string.h>

#include "wp43s.h"

#ifndef TESTSUITE_BUILD
  void fnAim(uint16_t unusedButMandatoryParameter) {
    resetShiftState();  //JM
    aimBuffer[0] = 0;
    calcModeAim(NOPARAM); // Alpha Input Mode
  }



uint16_t convertItemToSubOrSup(uint16_t item, int16_t subOrSup) {
/*JM*/ //nextChar = NC_NORMAL;

    if(subOrSup == NC_SUBSCRIPT) {
      switch(item) {
        case ITM_alpha    : return ITM_SUB_alpha;
        case ITM_delta    : return ITM_SUB_delta;
        case ITM_mu       : return ITM_SUB_mu;
        case ITM_SUN      : return ITM_SUB_SUN;
        case ITM_h        : return ITM_SUB_h;
        case ITM_t        : return ITM_SUB_t;
        case ITM_INFINITY : return ITM_SUB_INFINITY;
        case ITM_s        : return ITM_SUB_s;
        case ITM_PLUS     : return ITM_SUB_PLUS;
        case ITM_MINUS    : return ITM_SUB_MINUS;
        case ITM_0        :
        case ITM_1        :
        case ITM_2        :
        case ITM_3        :
        case ITM_4        :
        case ITM_5        :
        case ITM_6        :
        case ITM_7        :
        case ITM_8        :
        case ITM_9        : return item + (ITM_SUB_9 - ITM_9);
        case ITM_a        :
        case ITM_b        :
        case ITM_c        :
        case ITM_d        :
        case ITM_e        :
        case ITM_i        :
        case ITM_j        :
        case ITM_k        :
        case ITM_l        :
        case ITM_m        :
        case ITM_n        :
        case ITM_o        :
        case ITM_p        :
        case ITM_q        :
        case ITM_u        :
        case ITM_v        :
        case ITM_w        :
        case ITM_x        :
        case ITM_y        :
        case ITM_z        : return item + (ITM_SUB_z - ITM_z);
        case ITM_A        :
        case ITM_B        :
        case ITM_C        :
        case ITM_D        :
        case ITM_E        :
        case ITM_F        :
        case ITM_G        :
        case ITM_H        :
        case ITM_I        :
        case ITM_J        :
        case ITM_K        :
        case ITM_L        :
        case ITM_M        :
        case ITM_N        :
        case ITM_O        :
        case ITM_P        :
        case ITM_Q        :
        case ITM_R        :
        case ITM_S        :
        case ITM_T        :
        case ITM_U        :
        case ITM_V        :
        case ITM_W        :
        case ITM_X        :
        case ITM_Y        :
        case ITM_Z        : return item + (ITM_SUB_Z - ITM_Z);
        default           : return item;
      }
    }
    else if(subOrSup == NC_SUPERSCRIPT) {
      switch(item) {
        case ITM_a        : return ITM_SUP_a;
        case ITM_x        : return ITM_SUP_x;
        case ITM_INFINITY : return ITM_SUP_INFINITY;
        case ITM_PLUS     : return ITM_SUP_PLUS;
        case ITM_MINUS    : return ITM_SUP_MINUS;
        case ITM_0        : 
        case ITM_1        : 
        case ITM_2        : 
        case ITM_3        : 
        case ITM_4        : 
        case ITM_5        : 
        case ITM_6        : 
        case ITM_7        : 
        case ITM_8        : 
        case ITM_9        : return item + (ITM_SUP_9 - ITM_9);
        case ITM_f        : return ITM_SUP_f;
        case ITM_g        : return ITM_SUP_g;
        case ITM_h        : return ITM_SUP_h;
        case ITM_r        : return ITM_SUP_r;
        case ITM_T        : return ITM_SUP_T;
        default           : return item;
      }
    }

    return item;
  }



  int32_t findFirstItem(const char *twoLetters) {
    int16_t menuId = softmenuStack[0].softmenuId;

    if(menuId < NUMBER_OF_DYNAMIC_SOFTMENUS) { // Dynamic softmenu
      uint8_t *firstString, *middleString;
      int16_t first, middle, last;

      first = 0;
      firstString = dynamicSoftmenu[menuId].menuContent;

      last = dynamicSoftmenu[menuId].numItems - 1;

      middle = first + (last - first) / 2;
      middleString = getNthString(dynamicSoftmenu[menuId].menuContent, middle);

      while(first + 1 < last) {
        if(compareString(twoLetters, (char *)middleString, CMP_CLEANED_STRING_ONLY) <= 0) {
          last = middle;
        }
        else {
          first = middle;
          firstString = middleString;
        }

        middle = first + (last - first) / 2;
        middleString = getNthString(dynamicSoftmenu[menuId].menuContent, middle);
      }

      if(compareString(twoLetters, (char *)firstString, CMP_CLEANED_STRING_ONLY) <= 0) {
        return first;
      }
      else {
        return last;
      }
    }

    else { // Static softmenu
      const int16_t *first, *middle, *last;
      first = softmenu[menuId].softkeyItem;
      last = first + softmenu[menuId].numItems - 1;
      while(*last == ITM_NULL) {
        last--;
      }

      middle = first + (last - first) / 2;
      //const int16_t *f = softmenu[menuId].softkeyItem;
      //printf("\n----------------------------------\nfirst  = %3" PRIu64 "   %3d\n", (int64_t)(first - f), *first);
      //printf("middle = %3" PRIu64 "   %3d\n", (int64_t)(middle - f), *middle);
      //printf("last   = %3" PRIu64 "   %3d\n", (int64_t)(last - f), *last);
      while(first + 1 < last) {
        if(compareString(twoLetters, indexOfItems[abs(*middle)].itemCatalogName, CMP_CLEANED_STRING_ONLY) <= 0) {
          last = middle;
        }
        else {
          first = middle;
        }

        middle = first + (last - first) / 2;
      //printf("\nfirst  = %3" PRIu64 "   %3d\n", (int64_t)(first - f), *first);
      //printf("middle = %3" PRIu64 "   %3d\n", (int64_t)(middle - f), *middle);
      //printf("last   = %3" PRIu64 "   %3d\n", (int64_t)(last - f), *last);
      }

      if(compareString(twoLetters, indexOfItems[abs(*first)].itemCatalogName, CMP_CLEANED_STRING_ONLY) <= 0) {
        return first - softmenu[menuId].softkeyItem;
      }
      else {
        return last - softmenu[menuId].softkeyItem;
      }
    }
  }




#ifndef TESTSUITE_BUILD                          //JMvv
void light_ASB_icon(void) {
  lcd_fill_rect(X_ALPHA_MODE,18,9,2,0xFF);
  force_refresh();
}

void kill_ASB_icon(void) {
  lcd_fill_rect(X_ALPHA_MODE,18,9,2,0);
  force_refresh();
}
#endif                                           //JM^^


  void resetAlphaSelectionBuffer(void) {
    lgCatalogSelection = 0;
    alphaSelectionTimer = 0;
    asmBuffer[0] = 0;
    fnKeyInCatalog = 0;
    AlphaSelectionBufferTimerRunning = false;     //JMvv
    #ifndef TESTSUITE_BUILD
      kill_ASB_icon();
    #endif // TESTSUITE_BUILD                       //JM^^
  }


  bool_t timeoutAlphaSelectionBuffer(void) {       //JM
    if(alphaSelectionTimer != 0 && (getUptimeMs() - alphaSelectionTimer) > 3000){
      resetAlphaSelectionBuffer();
      return true;
    } else {
      return false;
    }
  }

  void startAlphaSelectionBuffer(void) {           //JM
    alphaSelectionTimer = getUptimeMs();
    AlphaSelectionBufferTimerRunning = true;
    #ifndef TESTSUITE_BUILD
    light_ASB_icon();
    #endif // TESTSUITE_BUILD                       //JM^^
  }






  /********************************************//**
   * \brief Adds an item to the alpha buffer
   *
   * \param[in] item uint16_t
   * \return void
   ***********************************************/
  void addItemToBuffer(uint16_t item) {

    #ifdef PC_BUILD
      char tmp[200]; sprintf(tmp,"bufferize.c: addItemToBuffer item=%d tam.mode=%d\n",item,tam.mode); jm_show_calc_state(tmp);
    #endif


    if(item == NOPARAM) {
      displayBugScreen("In function addItemToBuffer: item should not be NOPARAM=7654!");
    }
    else {
      if(calcMode == CM_NORMAL && fnKeyInCatalog && isAlphabeticSoftmenu()) {
        fnAim(NOPARAM);
      }
      if((fnKeyInCatalog || !catalog) && (calcMode == CM_AIM || tam.alpha)) {
        item = convertItemToSubOrSup(item, nextChar);
        if(stringByteLength(aimBuffer) + stringByteLength(indexOfItems[item].itemSoftmenuName) >= AIM_BUFFER_LENGTH) { /// TODO this error should never happen but who knows!
          sprintf(errorMessage, "In function addItemToBuffer: the AIM input buffer is full! %d bytes for now", AIM_BUFFER_LENGTH);
          displayBugScreen(errorMessage);
        }
        else {
          //JMCURSOR vv ADD THE CHARACTER MID-STRING =======================================================
          uint16_t ix = 0; 
          uint16_t in = 0;
          while (ix<T_cursorPos && in<T_cursorPos) {              //search the ix position in aimBuffer before the cursor
            in = stringNextGlyph(aimBuffer, in);                  //find the in position in aimBuffer which is then the cursor position
            ix++;  
          }
          T_cursorPos = in;
          char ixaa[AIM_BUFFER_LENGTH];                           //prepare temporary aimBuffer
          xcopy(ixaa, aimBuffer,in);                              //copy everything up to the cursor position
          ixaa[in]=0;                                             //stop new buffer at cursor position to be able to insert new character 
          strcat(ixaa,indexOfItems[item].itemSoftmenuName);       //add new character
          strcat(ixaa,aimBuffer + in);                            //copy rest of the aimbuffer
          strcpy(aimBuffer,ixaa);                                 //return temporary string to aimBuffer
          T_cursorPos = stringNextGlyph(aimBuffer, T_cursorPos);  //place the cursor at the next glyph boundary
          //JMCURSOR ^^ REPLACES THE FOLLOWING XCOPY, WHICH NORMALLY JUST ADDS A CHARACTER TO THE END OF THE STRING
          // xcopy(aimBuffer + stringNextGlyph(aimBuffer, stringLastGlyph(aimBuffer)), indexOfItems[item].itemSoftmenuName, stringByteLength(indexOfItems[item].itemSoftmenuName) + 1);
        }
      }

      if(catalog && !fnKeyInCatalog) {

        if(item == ITM_BACKSPACE) {
          calcModeNormal();
          return;
        }

        // NOP if not a single character input for search
        // or if we already have two characters in the search buffer
        else if(stringGlyphLength(indexOfItems[item].itemSoftmenuName) == 1 &&
                (lgCatalogSelection < ((asmBuffer[0] & 0x80) ? 3 : 2)) &&
                currentSoftmenuScrolls()) {
          int32_t pos = lgCatalogSelection++;
          if(asmBuffer[pos] != 0) {
            pos++;
          }

          asmBuffer[pos++] = indexOfItems[item].itemSoftmenuName[0];
          if(indexOfItems[item].itemSoftmenuName[0] & 0x80) { // 2 bytes
            asmBuffer[pos++] = indexOfItems[item].itemSoftmenuName[1];
          }
          asmBuffer[pos] = 0;

          softmenuStack[0].firstItem = findFirstItem(asmBuffer);
          setCatalogLastPos();
//          alphaSelectionTimer = getUptimeMs();     //JM
          startAlphaSelectionBuffer();               //JM
        }
      }

      else if(tam.mode) {
        tamProcessInput(item);
      }

      else if(calcMode == CM_NIM) {
        addItemToNimBuffer(item);
      }

      else if(calcMode != CM_AIM && (item >= ITM_A && item <= ITM_F)) {
        // We are not in NIM, but should enter NIM - this should be handled here
        // unlike digits 0 to 9 which are handled by processKeyAction
        addItemToNimBuffer(item);
      }

      else if(calcMode != CM_AIM) {
        funcOK = false;
        return;
      }

      funcOK = true;
    }
  }



  void addItemToNimBuffer(int16_t item) {
    int16_t lastChar, index;
    uint8_t savedNimNumberPart;
    bool_t done;
    char *strBase;

    if(calcMode == CM_NORMAL) {
      switch(item) {
        case ITM_EXPONENT :
          if( eRPN ) {                 //JM NEWERPN vv  NOTE: Rather force a stack lift before NIM (than after ENTER) so that normal ENTER SLS can be used for cancelling ENTER using NOP
            calcModeNim(NOPARAM);
            clearSystemFlag(FLAG_ASLIFT);      
          } 
          else {                       //JM NEWERPN ^^
          calcModeNim(NOPARAM);
          }                            //JM NEWERPN
          aimBuffer[0] = '+';
          aimBuffer[1] = '1';
          aimBuffer[2] = '.';
          aimBuffer[3] = 0;
          nimNumberPart = NP_REAL_FLOAT_PART;
          lastIntegerBase = 0;
          fnRefreshState();                                                //JMNIM
          break;

        case ITM_PERIOD :
          if( eRPN ) {                 //JM NEWERPN vv
              calcModeNim(NOPARAM);
              clearSystemFlag(FLAG_ASLIFT);      
          } 
          else {                       //JM NEWERPN ^^
          calcModeNim(NOPARAM);
          }                            //JM NEWERPN
          aimBuffer[0] = '+';
          aimBuffer[1] = '0';
          aimBuffer[2] = 0;
          nimNumberPart = NP_INT_10;
          break;

        case ITM_0 :
        case ITM_1 :
        case ITM_2 :
        case ITM_3 :
        case ITM_4 :
        case ITM_5 :
        case ITM_6 :
        case ITM_7 :
        case ITM_8 :
        case ITM_9 :
        case ITM_A :
        case ITM_B :
        case ITM_C :
        case ITM_D :
        case ITM_E :
        case ITM_F :
          if( eRPN ) {                 //JM NEWERPN vv
              calcModeNim(NOPARAM);
              clearSystemFlag(FLAG_ASLIFT);      
          } 
          else {                       //JM NEWERPN ^^
          calcModeNim(NOPARAM);
          }                            //JM NEWERPN
          aimBuffer[0] = '+';
          aimBuffer[1] = 0;
          nimNumberPart = NP_EMPTY;
          break;

        default :
          sprintf(errorMessage, "In function addItemToNimBuffer: %d is an unexpected item value when initializing NIM!", item);
          displayBugScreen(errorMessage);
          return;
      }

      //debugNIM();
    }

    done = false;

    switch(item) {
      case ITM_0 :
      case ITM_1 :
      case ITM_2 :
      case ITM_3 :
      case ITM_4 :
      case ITM_5 :
      case ITM_6 :
      case ITM_7 :
      case ITM_8 :
      case ITM_9 :
        done = true;

        switch(nimNumberPart) {
          case NP_INT_10 :
            if(item == ITM_0) {
              //if(aimBuffer[1] != '0') {  //JM_vv TYPE0; Allow starting the NIM buffer with 0000
                strcat(aimBuffer, "0");
              //}
            }
            else {
              if(aimBuffer[1] == '0') {
                //aimBuffer[1] = 0;        //JM_^^ TYPE0
              }

              strcat(aimBuffer, indexOfItems[item].itemSoftmenuName);
            }
            break;

          case NP_REAL_EXPONENT :
            if(item == ITM_0) {
              if(aimBuffer[exponentSignLocation + 1] == '0') {
                aimBuffer[strlen(aimBuffer) - 1] = 0;
              }

              if(aimBuffer[exponentSignLocation + 1] != 0 || aimBuffer[exponentSignLocation] == '+') {
                strcat(aimBuffer, "0");
              }

              if(stringToInt16(aimBuffer + exponentSignLocation) > exponentLimit || stringToInt16(aimBuffer + exponentSignLocation) < -exponentLimit) {
                aimBuffer[strlen(aimBuffer) - 1] = 0;
              }
            }
            else {
              if(aimBuffer[exponentSignLocation + 1] == '0') {
                aimBuffer[strlen(aimBuffer) - 1] = 0;
              }

              strcat(aimBuffer, indexOfItems[item].itemSoftmenuName);

              if(stringToInt16(aimBuffer + exponentSignLocation) > exponentLimit || stringToInt16(aimBuffer + exponentSignLocation) < -exponentLimit) {
                aimBuffer[strlen(aimBuffer) - 1] = 0;
              }
            }
            break;

          case NP_FRACTION_DENOMINATOR :
            if(item == ITM_0) {
              strcat(aimBuffer, "0");

              if(aimBuffer[denominatorLocation] == '0') {
                aimBuffer[denominatorLocation] = 0;
              }

              if(stringToInt32(aimBuffer + denominatorLocation) > 9999) {
                aimBuffer[strlen(aimBuffer) - 1] = 0;
              }
            }
            else {
              strcat(aimBuffer, indexOfItems[item].itemSoftmenuName);

              if(stringToInt32(aimBuffer + denominatorLocation) > 9999) {
                aimBuffer[strlen(aimBuffer) - 1] = 0;
              }
            }
            break;

          case NP_COMPLEX_INT_PART :
            if(item == ITM_0) {
              if(aimBuffer[imaginaryMantissaSignLocation + 2] != '0') {
                strcat(aimBuffer, "0");
              }
            }
            else {
              if(aimBuffer[imaginaryMantissaSignLocation + 2] == '0') {
                aimBuffer[imaginaryMantissaSignLocation + 2] = 0;
              }

              strcat(aimBuffer, indexOfItems[item].itemSoftmenuName);
            }
            break;

          case NP_COMPLEX_EXPONENT :
            if(item == ITM_0) {
              if(aimBuffer[imaginaryExponentSignLocation + 1] == '0') {
                aimBuffer[strlen(aimBuffer) - 1] = 0;
              }

              if(aimBuffer[imaginaryExponentSignLocation + 1] != 0 || aimBuffer[imaginaryExponentSignLocation] == '+') {
                strcat(aimBuffer, "0");
              }

              if(stringToInt16(aimBuffer + imaginaryExponentSignLocation) > exponentLimit || stringToInt16(aimBuffer + imaginaryExponentSignLocation) < -exponentLimit) {
                aimBuffer[strlen(aimBuffer) - 1] = 0;
              }
            }
            else {
              if(aimBuffer[imaginaryExponentSignLocation + 1] == '0') {
                aimBuffer[strlen(aimBuffer) - 1] = 0;
              }

              strcat(aimBuffer, indexOfItems[item].itemSoftmenuName);

              if(stringToInt16(aimBuffer + imaginaryExponentSignLocation) > exponentLimit || stringToInt16(aimBuffer + imaginaryExponentSignLocation) < -exponentLimit) {
                aimBuffer[strlen(aimBuffer) - 1] = 0;
              }
            }
            break;

          case NP_INT_BASE :
            strcat(aimBuffer, indexOfItems[item].itemSoftmenuName);
            strBase = strchr(aimBuffer, '#') + 1;
            if(atoi(strBase) > 16) {
              strBase[1] = 0;
            }
            else if(atoi(strBase) >= 2) {
              goto addItemToNimBuffer_exit;
            }
            break;

          default :
            if(nimNumberPart == NP_EMPTY) {
              nimNumberPart = NP_INT_10;
              //debugNIM();
            }

            strcat(aimBuffer, indexOfItems[item].itemSoftmenuName);
        }
        break;

      case ITM_A :
      case ITM_B :
      case ITM_C :
      case ITM_D :
      case ITM_E :
      case ITM_F :
        done = true;

        if(nimNumberPart == NP_EMPTY || nimNumberPart == NP_INT_10 || nimNumberPart == NP_INT_16) {
          if(aimBuffer[1] == '0') {
            //aimBuffer[1] = 0;       //JM_TYPE0
          }

          strcat(aimBuffer, indexOfItems[item].itemSoftmenuName);
          hexDigits++;

          nimNumberPart = NP_INT_16;
          //debugNIM();
        }
        break;

      case ITM_PERIOD :
        done = true;

        if(aimBuffer[strlen(aimBuffer)-1] == 'i') {
          strcat(aimBuffer, "0");
        }

        lastIntegerBase = 0;
        fnRefreshState();                                                //JMNIM

        switch(nimNumberPart) {
          case NP_INT_10 :
            strcat(aimBuffer, ".");

            nimNumberPart = NP_REAL_FLOAT_PART;
            //debugNIM();
            break;

          case NP_REAL_FLOAT_PART :
            if(aimBuffer[strlen(aimBuffer) - 1] == '.') {
              strcat(aimBuffer, "0");
            }

            for(uint16_t i=0; i<strlen(aimBuffer); i++) {
              if(aimBuffer[i] == '.') {
                aimBuffer[i] = ' ';
                break;
              }
            }
            strcat(aimBuffer, "/");

            denominatorLocation = strlen(aimBuffer);
            nimNumberPart = NP_FRACTION_DENOMINATOR;
            //debugNIM();
            break;

          case NP_COMPLEX_INT_PART :
            strcat(aimBuffer, ".");

            nimNumberPart = NP_COMPLEX_FLOAT_PART;
            //debugNIM();
            break;

          default : {}
        }
        break;

      case ITM_EXPONENT :
        done = true;

        if(aimBuffer[strlen(aimBuffer)-1] == 'i') {
          strcat(aimBuffer, "1");
        }

        lastIntegerBase = 0;
        fnRefreshState();                                                //JMNIM

        switch(nimNumberPart) {
          case NP_INT_10 :
            strcat(aimBuffer, "."); // no break here
            #ifndef __APPLE__
              __attribute__ ((fallthrough));
            #endif // !__APPLE__
          case NP_REAL_FLOAT_PART :
            strcat(aimBuffer, "e+");
            exponentSignLocation = strlen(aimBuffer) - 1;

            nimNumberPart = NP_REAL_EXPONENT;
            //debugNIM();
            break;

          case NP_COMPLEX_INT_PART :
            strcat(aimBuffer, "."); // no break here
            #ifndef __APPLE__
              __attribute__ ((fallthrough));
            #endif // !__APPLE__
          case NP_COMPLEX_FLOAT_PART :
            strcat(aimBuffer, "e+");
            imaginaryExponentSignLocation = strlen(aimBuffer) - 1;

            nimNumberPart = NP_COMPLEX_EXPONENT;
            //debugNIM();
            break;

          default : {}
        }
        break;

      case ITM_toINT : // #
        done = true;

        lastIntegerBase = 0;
        fnRefreshState();                                                //JMNIM

        if(nimNumberPart == NP_INT_10 || nimNumberPart == NP_INT_16) {
          strcat(aimBuffer, "#");

          nimNumberPart = NP_INT_BASE;
          //debugNIM();
        }
        break;

      case ITM_CHS : // +/-
        done = true;

        switch(nimNumberPart) {
          case NP_INT_10 :
          case NP_INT_16 :
          case NP_INT_BASE :
          case NP_REAL_FLOAT_PART :
          case NP_FRACTION_DENOMINATOR :
            if(aimBuffer[0] == '+') {
              aimBuffer[0] = '-';
            }
            else {
              aimBuffer[0] = '+';
            }
            break;

          case NP_REAL_EXPONENT :
            if(aimBuffer[exponentSignLocation] == '+') {
              aimBuffer[exponentSignLocation] = '-';
              if(aimBuffer[exponentSignLocation + 1] == '0') {
                aimBuffer[strlen(aimBuffer) - 1] = 0;
              }
            }
            else {
              aimBuffer[exponentSignLocation] = '+';
            }
            break;

          case NP_COMPLEX_INT_PART :
          case NP_COMPLEX_FLOAT_PART :
            if(aimBuffer[imaginaryMantissaSignLocation] == '+') {
              aimBuffer[imaginaryMantissaSignLocation] = '-';
            }
            else {
              aimBuffer[imaginaryMantissaSignLocation] = '+';
            }
            break;

          case NP_COMPLEX_EXPONENT :
            if(aimBuffer[imaginaryExponentSignLocation] == '+') {
              aimBuffer[imaginaryExponentSignLocation] = '-';
              if(aimBuffer[imaginaryExponentSignLocation + 1] == '0') {
                aimBuffer[strlen(aimBuffer) - 1] = 0;
              }
            }
            else {
              aimBuffer[imaginaryExponentSignLocation] = '+';
            }
            break;

          default : {}
        }
        break;


      case ITM_i :                          //JM HP35 compatible, in NIM
      case ITM_CC :
        if (item == ITM_i) resetShiftState();    //JM HP35 compatible, in NIM
        lastChar = strlen(aimBuffer) - 1;

        done = true;

        lastIntegerBase = 0;
        fnRefreshState();                                                //JMNIM

        switch(nimNumberPart) {
         case NP_REAL_EXPONENT :
            if((aimBuffer[lastChar] == '+' || aimBuffer[lastChar] == '-') && aimBuffer[lastChar - 1] == 'e') {
              aimBuffer[lastChar - 1] = 0;
            }
            else if(aimBuffer[lastChar] == 'e') {
              aimBuffer[lastChar] = 0;
            }
            else {
              imaginaryMantissaSignLocation = strlen(aimBuffer);
              strcat(aimBuffer, "+i");

              nimNumberPart = NP_COMPLEX_INT_PART;
              //debugNIM();
            }
            break;

          case NP_INT_10 :
            strcat(aimBuffer, "."); // no break here
            #ifndef __APPLE__
              __attribute__ ((fallthrough));
            #endif // !__APPLE__

          case NP_REAL_FLOAT_PART :
            imaginaryMantissaSignLocation = strlen(aimBuffer);
            strcat(aimBuffer, "+i");

            nimNumberPart = NP_COMPLEX_INT_PART;
            //debugNIM();
            break;

          default : {}
        }
        break;

      case ITM_CONSTpi :
        if(nimNumberPart == NP_COMPLEX_INT_PART && aimBuffer[strlen(aimBuffer) - 1] == 'i') {
          done = true;
          strcat(aimBuffer, "3.141592653589793238462643383279503");
        }
        break;

      case ITM_BACKSPACE :
        lastChar = strlen(aimBuffer) - 1;

        done = true;

        switch(nimNumberPart) {
          case NP_INT_10 :
            break;

          case NP_INT_16 :
            if(aimBuffer[lastChar] >= 'A') {
              hexDigits--;
            }

            if(hexDigits == 0) {
              nimNumberPart = NP_INT_10;
              //debugNIM();
            }
            break;

          case NP_INT_BASE :
            if(aimBuffer[lastChar] == '#') {
              if(hexDigits > 0) {
                nimNumberPart = NP_INT_16;
              }
              else {
                nimNumberPart = NP_INT_10;
              }
              //debugNIM();
            }
            break;

          case NP_REAL_FLOAT_PART :
            if(aimBuffer[lastChar] == '.') {
              nimNumberPart = NP_INT_10;
              //debugNIM();
            }
            break;

          case NP_REAL_EXPONENT :
            if(aimBuffer[lastChar] == '+' || aimBuffer[lastChar] == '-') {
              aimBuffer[lastChar--] = 0;
            }

            if(aimBuffer[lastChar] == 'e') {
              nimNumberPart = NP_INT_10;
              for(int16_t i=0; i<lastChar; i++) {
                if(aimBuffer[i] == '.') {
                  nimNumberPart = NP_REAL_FLOAT_PART;
                  break;
                }
              }
              //debugNIM();
            }
            break;

          case NP_FRACTION_DENOMINATOR :
            if(aimBuffer[lastChar] == '/') {
              nimNumberPart = NP_REAL_FLOAT_PART;
              for(int16_t i=0; i<lastChar; i++) {
                if(aimBuffer[i] == ' ') {
                  aimBuffer[i] = '.';
                  break;
                }
              }
              //debugNIM();
            }
            break;

          case NP_COMPLEX_INT_PART :
            if(aimBuffer[lastChar] == 'i') {
              nimNumberPart = NP_INT_10;
              for(int16_t i=0; i<lastChar; i++) {
                if(aimBuffer[i] == 'e') {
                  nimNumberPart = NP_REAL_EXPONENT;
                  break;
                }
                if(aimBuffer[i] == '.') {
                  nimNumberPart = NP_REAL_FLOAT_PART;
                }
              }
              //debugNIM();
              lastChar--;
            }
            break;

          case NP_COMPLEX_FLOAT_PART :
            if(aimBuffer[lastChar] == '.') {
              nimNumberPart = NP_COMPLEX_INT_PART;
              //debugNIM();
            }
            break;

          case NP_COMPLEX_EXPONENT :
            if(aimBuffer[lastChar] == '+' || aimBuffer[lastChar] == '-') {
              aimBuffer[lastChar--] = 0;
            }

            if(aimBuffer[lastChar] == 'e') {
              nimNumberPart = NP_COMPLEX_INT_PART;
              for(int16_t i=imaginaryMantissaSignLocation+2; i<lastChar; i++) {
                if(aimBuffer[i] == '.') {
                  nimNumberPart = NP_COMPLEX_FLOAT_PART;
                  break;
                }
              }
              //debugNIM();
            }
            break;

          default : {}
        }

        aimBuffer[lastChar--] = 0;

        if(lastChar == -1 || (lastChar == 0 && aimBuffer[0] == '+')) {
          calcModeNormal();
          undo();
        }
        break;

      case ITM_EXIT1 :
        addItemToNimBuffer_exit:
        done = true;
        closeNim();
        if(calcMode != CM_NIM && lastErrorCode == 0) {
          setSystemFlag(FLAG_ASLIFT);
          if(item == ITM_EXIT1) {
            saveForUndo();
          }
          return;
        }
        if(item == ITM_EXIT1) {
          saveForUndo();
        }
        break;

      case ITM_1ONX : // B for binary base    Only works in direct NIM
        if(nimNumberPart == NP_INT_BASE && aimBuffer[strlen(aimBuffer) - 1] == '#') {
          strcat(aimBuffer, "2");
          goto addItemToNimBuffer_exit;
        }
        break;

      case ITM_ENTER:                                 //JM
      case ITM_LOG10 : // D for decimal base          //JM
        if(nimNumberPart == NP_INT_BASE && aimBuffer[strlen(aimBuffer) - 1] == '#') {
          strcat(aimBuffer, "10");
          goto addItemToNimBuffer_exit;
        }
        break;

      case ITM_RCL : // H for hexadecimal base
        if(nimNumberPart == NP_INT_BASE && aimBuffer[strlen(aimBuffer) - 1] == '#') {
          strcat(aimBuffer, "16");
          goto addItemToNimBuffer_exit;
        }
        break;

      case ITM_DMS :
        if(nimNumberPart == NP_INT_10 || nimNumberPart == NP_REAL_FLOAT_PART) {
          done = true;

          closeNim();
          if(calcMode != CM_NIM && lastErrorCode == 0) {
            if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
              convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
            }

            real34FromDmsToDeg(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
            setRegisterAngularMode(REGISTER_X, amDMS);

            setSystemFlag(FLAG_ASLIFT);
            return;
          }
        }
        break;

      case ITM_dotD :
        if(nimNumberPart == NP_REAL_FLOAT_PART || nimNumberPart == NP_REAL_FLOAT_PART) {
          done = true;

          closeNim();
          if(calcMode != CM_NIM && lastErrorCode == 0) {
            convertReal34RegisterToDateRegister(REGISTER_X, REGISTER_X);
            checkDateRange(REGISTER_REAL34_DATA(REGISTER_X));

            if(lastErrorCode == 0) {
              setSystemFlag(FLAG_ASLIFT);
            }
            else {
              undo();
            }
            return;
          }
        } else {                      //JM
          done = true;                //JM
          closeNim();                 //JM
        }
        break;

      case ITM_ms :                        //JM
      case ITM_toHMS :
        if(nimNumberPart == NP_INT_10 || nimNumberPart == NP_REAL_FLOAT_PART || nimNumberPart == NP_REAL_EXPONENT) {
          done = true;

          closeNim();
          if(calcMode != CM_NIM && lastErrorCode == 0) {
            if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
              convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
            }

            hmmssInRegisterToSeconds(REGISTER_X);
            if(lastErrorCode == 0) {
              setSystemFlag(FLAG_ASLIFT);
            }
            else {
              undo();
            }
            return;
          }
        }
        break;

      case ITM_DMS2:                        //JM
        if(nimNumberPart == NP_INT_10 || nimNumberPart == NP_REAL_FLOAT_PART || nimNumberPart == NP_REAL_EXPONENT) {
          done = true;
          closeNim(); 
          fnAngularModeJM(amDMS); //it cannot be an angle at this point. If closed input, it is only real or longint
        }
        break;

      case ITM_DRG :                        //JM
        if(nimNumberPart == NP_INT_10 || nimNumberPart == NP_REAL_FLOAT_PART || nimNumberPart == NP_REAL_EXPONENT) {
          done = true;

          closeNim();

          if(calcMode != CM_NIM && lastErrorCode == 0) {
            if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
              convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
            }
            if(getRegisterDataType(REGISTER_X) == dtReal34 && getRegisterAngularMode(REGISTER_X) == amNone) {
              if(currentAngularMode == amDMS) fnCvtFromCurrentAngularMode(amDMS); else
              setRegisterAngularMode(REGISTER_X, currentAngularMode);
            }

            if(lastErrorCode == 0) {
              setSystemFlag(FLAG_ASLIFT);
            }
            else {
              undo();
            }
            return;
          }
        }
        break;



      default : keyActionProcessed = false;
    }

    if(done) {
      //Convert nimBuffer to display string

      strcpy(nimBufferDisplay, STD_SPACE_HAIR);

      switch(nimNumberPart) {
        case NP_INT_10 :          // +12345
        case NP_INT_16 :          // +123AB
        case NP_INT_BASE :        // +123AB#16
          nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);
          break;

        case NP_REAL_FLOAT_PART : // +12345.6789
          nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);
          break;

        case NP_REAL_EXPONENT : // +12345.678e+3
          nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);

          exponentToDisplayString(stringToInt32(aimBuffer + exponentSignLocation), nimBufferDisplay + stringByteLength(nimBufferDisplay), NULL, true, STD_SPACE_PUNCTUATION);
          if(aimBuffer[exponentSignLocation + 1] == 0 && aimBuffer[exponentSignLocation] == '-') {
            strcat(nimBufferDisplay, STD_SUP_MINUS);
          }
          else if(aimBuffer[exponentSignLocation + 1] == '0' && aimBuffer[exponentSignLocation] == '+') {
            strcat(nimBufferDisplay, STD_SUP_0);
          }
          break;

        case NP_FRACTION_DENOMINATOR : // +123 12/7
          nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);
          strcat(nimBufferDisplay, STD_SPACE_4_PER_EM);

          for(index=2; aimBuffer[index]!=' '; index++); // The ending semi colon is OK here
          supNumberToDisplayString(stringToInt32(aimBuffer + index + 1), nimBufferDisplay + stringByteLength(nimBufferDisplay), NULL, true, STD_SPACE_PUNCTUATION);

          strcat(nimBufferDisplay, "/");

          for(; aimBuffer[index]!='/'; index++); // The ending semi colon is OK here
          if(aimBuffer[++index] != 0) {
            subNumberToDisplayString(stringToInt32(aimBuffer + index), nimBufferDisplay + stringByteLength(nimBufferDisplay), NULL);
          }
          break;

        case NP_COMPLEX_INT_PART :   // +1.2+i15
        case NP_COMPLEX_FLOAT_PART : // +1.2+i15.69
        case NP_COMPLEX_EXPONENT :   // +1.2+i15.69e2
          // Real part
          savedNimNumberPart = nimNumberPart;

          for(index=2; index<imaginaryMantissaSignLocation && aimBuffer[index] != '.'; index++); // The ending semi colon is OK here
          if(index < imaginaryMantissaSignLocation) { // There is a decimal part in the real part
            nimNumberPart = NP_REAL_FLOAT_PART;
          }

          for(index=2; index<imaginaryMantissaSignLocation && aimBuffer[index] != 'e'; index++); // The ending semi colon is OK here
          if(index < imaginaryMantissaSignLocation) { // There is an exposant in the real part
            nimNumberPart = NP_REAL_EXPONENT;
          }

          nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);

          if(nimNumberPart == NP_REAL_EXPONENT) {
            exponentToDisplayString(stringToInt32(aimBuffer + exponentSignLocation), nimBufferDisplay + stringByteLength(nimBufferDisplay), NULL, true, STD_SPACE_PUNCTUATION);
            if(aimBuffer[exponentSignLocation + 1] == 0 && aimBuffer[exponentSignLocation] == '-') {
              strcat(nimBufferDisplay, STD_SUP_MINUS);
            }
            else if(aimBuffer[exponentSignLocation + 1] == '0' && aimBuffer[exponentSignLocation] == '+') {
              strcat(nimBufferDisplay, STD_SUP_0);
            }
          }

          nimNumberPart = savedNimNumberPart;

          // Complex "separator"
          if(getSystemFlag(FLAG_POLAR)) { // polar mode
            strcat(nimBufferDisplay, STD_SPACE_4_PER_EM STD_MEASURED_ANGLE STD_SPACE_4_PER_EM);
            if(aimBuffer[imaginaryMantissaSignLocation] == '-') {
              strcat(nimBufferDisplay, "-");
            }
          }
          else { // rectangular mode
            if(strncmp(nimBufferDisplay + stringByteLength(nimBufferDisplay) - 2, STD_SPACE_HAIR, 2) != 0) {
              strcat(nimBufferDisplay, STD_SPACE_HAIR);
            }

            if(aimBuffer[imaginaryMantissaSignLocation] == '-') {
              strcat(nimBufferDisplay, "-");
            }
            else {
              strcat(nimBufferDisplay, "+");
            }
            strcat(nimBufferDisplay, COMPLEX_UNIT);
            strcat(nimBufferDisplay, PRODUCT_SIGN);
          }

          // Imaginary part
          if(aimBuffer[imaginaryMantissaSignLocation+2] != 0) {
            savedNimNumberPart = nimNumberPart;

            for(index=imaginaryMantissaSignLocation+1; index<(int16_t)strlen(aimBuffer) && aimBuffer[index] != '.'; index++); // The ending semi colon is OK here
            if(index < (int16_t)strlen(aimBuffer)) { // There is a decimal part in the real part
              nimNumberPart = NP_REAL_FLOAT_PART;
            }

            for(index=imaginaryMantissaSignLocation+1; index<(int16_t)strlen(aimBuffer) && aimBuffer[index] != 'e'; index++); // The ending semi colon is OK here
            if(index < (int16_t)strlen(aimBuffer)) { // There is an exposant in the real part
              nimNumberPart = NP_REAL_EXPONENT;
            }

            nimBufferToDisplayBuffer(aimBuffer + imaginaryMantissaSignLocation + 1, nimBufferDisplay + stringByteLength(nimBufferDisplay));

            if(nimNumberPart == NP_REAL_EXPONENT) {
              exponentToDisplayString(stringToInt32(aimBuffer + imaginaryExponentSignLocation), nimBufferDisplay + stringByteLength(nimBufferDisplay), NULL, true, STD_SPACE_PUNCTUATION);
              if(aimBuffer[imaginaryExponentSignLocation + 1] == 0 && aimBuffer[imaginaryExponentSignLocation] == '-') {
                strcat(nimBufferDisplay, STD_SUP_MINUS);
              }
              else if(aimBuffer[imaginaryExponentSignLocation + 1] == '0' && aimBuffer[imaginaryExponentSignLocation] == '+') {
                strcat(nimBufferDisplay, STD_SUP_0);
              }
            }

            nimNumberPart = savedNimNumberPart;
          }
          break;

        default :
          sprintf(errorMessage, "In function addItemToNimBuffer: %d is an unexpected nimNumberPart value while converting buffer to display!", nimNumberPart);
          displayBugScreen(errorMessage);
      }

      if(!getSystemFlag(FLAG_DECIMP)) {
        for(index=stringByteLength(nimBufferDisplay) - 1; index>0; index--) {
          if(nimBufferDisplay[index] == '.') {
            nimBufferDisplay[index] = ',';
          }
        }
      }
    }

    else {
      switch (item) {           //JMCLOSE remove auto closenim for these functions only.
        case ITM_SQUAREROOTX :  //closeNim moved to keyboard.c / btnkeyrelease, as .ms is on longpress underneath sqrt
        case ITM_HASH_JM :      //closeNim simply not needed
          break;
        default : closeNim();
      }
      if(calcMode != CM_NIM) {
        if(item == ITM_CONSTpi || (item >= 0 && indexOfItems[item].func == fnConstant)) {
          setSystemFlag(FLAG_ASLIFT);
          lastIntegerBase = 0;                                             //JMNIM
          fnRefreshState();                                                //JMNIM
        }

        if(lastErrorCode == 0) {
          showFunctionName(item, 1000); // 1000ms = 1s
        }
      }
    }
  }



  static int16_t insertGapIP(char *displayBuffer, int16_t numDigits, int16_t nth) {
    if(groupingGap == 0)         return 0; // no gap when none is required!
    if(numDigits <= groupingGap) return 0; // there are less than groupingGap digits
    if(nth + 1 == numDigits)     return 0; // no gap after the last digit

    if((numDigits - nth) % groupingGap == 1 || groupingGap == 1) {
      strcpy(displayBuffer, STD_SPACE_PUNCTUATION);
      return 2;
    }

    return 0;
  }

  static int16_t insertGapFP(char *displayBuffer, int16_t numDigits, int16_t nth) {
    if(groupingGap == 0)         return 0; // no gap when none is required!
    if(numDigits <= groupingGap) return 0; // there are less than groupingGap digits
    if(nth + 1 == numDigits)     return 0; // no gap after the last digit

    if(nth % groupingGap == groupingGap - 1) {
      strcpy(displayBuffer, STD_SPACE_PUNCTUATION);
      return 2;
    }

    return 0;
  }



  void nimBufferToDisplayBuffer(const char *buffer, char *displayBuffer) {
    int16_t numDigits, source, dest;

    if(*buffer == '-') {
      *(displayBuffer++) = '-';
    }
    buffer++;

    int16_t groupingGapM = groupingGap;                       //JMGAP vv
    switch(nimNumberPart) {
      case NP_INT_10:                     // +12345 longint; Do not change groupingGap. Leave at user setting, default 3.
      case NP_INT_BASE:                   // +123AB#16.    ; Change groupinggap from user selection to this table, for entry
        switch(lastIntegerBase) {
          case  0: groupingGap = groupingGapM; break;
          case  2: groupingGap = 4; break;
          case  3: groupingGap = 3; break;
          case  4: groupingGap = 2; break;
          case  5:
          case  6:
          case  7:
          case  8:
          case  9:
          case 10:
          case 11:
          case 12:
          case 13:
          case 14:
          case 15: groupingGap = 3; break;
          case 16: groupingGap = 2; break;
          default: break;
        }
        break;
      case NP_INT_16:                     // +123AB.       ; Change to 2 for hex.
        groupingGap = 2;
        break;
      default:
        break;
    }                                                         //JMGAP ^^

    for(numDigits=0; buffer[numDigits]!=0 && buffer[numDigits]!='e' && buffer[numDigits]!='.' && buffer[numDigits]!=' ' && buffer[numDigits]!='#' && buffer[numDigits]!='+' && buffer[numDigits]!='-'; numDigits++); // The semicolon here is OK

    for(source=0, dest=0; source<numDigits; source++) {
      displayBuffer[dest++] = buffer[source];
      dest += insertGapIP(displayBuffer + dest, numDigits, source);
    }

    groupingGap = groupingGapM;                               //JMGAP
    displayBuffer[dest] = 0;

    if(nimNumberPart == NP_REAL_FLOAT_PART || nimNumberPart == NP_REAL_EXPONENT) {
      displayBuffer[dest++] = '.';

      buffer += source + 1;

      for(numDigits=0; buffer[numDigits]!=0 && buffer[numDigits]!='e' && buffer[numDigits]!='+' && buffer[numDigits]!='-'; numDigits++); // The semicolon here is OK

      for(source=0; source<numDigits; source++) {
        displayBuffer[dest++] = buffer[source];
        dest += insertGapFP(displayBuffer + dest, numDigits, source);
      }

      displayBuffer[dest] = 0;
    }

    else if(nimNumberPart == NP_INT_BASE) {
      strcpy(displayBuffer + dest, buffer + numDigits);
    }
  }



  void closeNim(void) {
  //printf("closeNim\n");
if(!eRPN)    setSystemFlag(FLAG_ASLIFT);
    if(nimNumberPart == NP_INT_10) {                //JM Input default type vv
      switch (Input_Default) {
      case ID_43S:                                  //   Do nothing, this is default LI/DP
      case ID_LI:                                   //   Do nothing, because default is LI/DP 
        break;
      case ID_DP:                                   //   Do Real default for DP
      case ID_CPXDP:                                //                       CPX
        nimNumberPart = NP_REAL_FLOAT_PART;
        break;
      case ID_SI:                                   //   lastIntegerBase is set in fnInDefault; I do not set it here, as the user can change it of course.
        break;
      }
    }                                               //JM ^^
    if((nimNumberPart == NP_INT_10 || nimNumberPart == NP_INT_16) && lastIntegerBase != 0) {
      strcat(aimBuffer, "#0");
      int16_t basePos = strlen(aimBuffer) - 1;
      if(lastIntegerBase <= 9) {
        aimBuffer[basePos] += lastIntegerBase;
      }
      else {
        aimBuffer[basePos++] = '1';
        aimBuffer[basePos] = '0';
        aimBuffer[basePos + 1] = 0;
        aimBuffer[basePos] += lastIntegerBase - 10;
      }

      nimNumberPart = NP_INT_BASE;
    }
    else {
      lastIntegerBase = 0;
      fnRefreshState();                                                //JMNIM
    }

    int16_t lastChar = strlen(aimBuffer) - 1;

    if(nimNumberPart != NP_INT_16) { // We need a # and a base
      if(nimNumberPart != NP_INT_BASE || aimBuffer[lastChar] != '#') { // We need a base
        if((nimNumberPart == NP_COMPLEX_EXPONENT || nimNumberPart == NP_REAL_EXPONENT) && (aimBuffer[lastChar] == '+' || aimBuffer[lastChar] == '-') && aimBuffer[lastChar - 1] == 'e') {
          aimBuffer[--lastChar] = 0;
          lastChar--;
        }
        else if(nimNumberPart == NP_REAL_EXPONENT && aimBuffer[lastChar] == 'e') {
          aimBuffer[lastChar--] = 0;
        }

        if(nimNumberPart == NP_COMPLEX_INT_PART && (aimBuffer[lastChar] == 'i' || aimBuffer[lastChar-1]*256 + aimBuffer[lastChar]*256 == 0xa221)) { // complex i or measured angle
          aimBuffer[++lastChar] = '0';                    //JM CHANGED FROM "1" to '0'. DEFAULTING TO 0+0xi WHEN ABORTING CC ENTRY.
          aimBuffer[lastChar + 1] = 0;
        }

        if((aimBuffer[0] != '-' || aimBuffer[1] != 0) && (aimBuffer[lastChar] != '-')) { // The buffer is not just the minus sign AND The last char of the buffer is not the minus sign
          calcModeNormal();

          if(nimNumberPart == NP_INT_10) {
            longInteger_t lgInt;

            longIntegerInit(lgInt);
            stringToLongInteger(aimBuffer + (aimBuffer[0] == '+' ? 1 : 0), 10, lgInt);
            convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
            longIntegerFree(lgInt);
          }
          else if(nimNumberPart == NP_INT_BASE) {
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Any change in this part must be reported in the function strToShortInteger from file testSuite.c after the line: else if(nimNumberPart == NP_INT_BASE) {
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            longInteger_t minVal, value, maxVal;
            int16_t posHash, i, lg;
            int32_t base;

            lg = strlen(aimBuffer);
            posHash = 0;
            for(i=1; i<lg; i++) {
              if(aimBuffer[i] == '#') {
                posHash = i;
                break;
              }
            }

            for(i=posHash+1; i<lg; i++) {
              if(aimBuffer[i]<'0' || aimBuffer[i]>'9') {
                // This should never happen
                displayCalcErrorMessage(ERROR_INVALID_INTEGER_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
                #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                  moreInfoOnError("In function closeNIM:", "there is a non numeric character in the base of the integer!", NULL, NULL);
                #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
                return;
              }
            }

            base = stringToInt32(aimBuffer + posHash + 1);
            if(base < 2 || base > 16) {
              displayCalcErrorMessage(ERROR_INVALID_INTEGER_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
               moreInfoOnError("In function closeNIM:", "the base of the integer must be from 2 to 16!", NULL, NULL);
              #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
              return;
            }

            for(i=aimBuffer[0] == '-' ? 1 : 0; i<posHash; i++) {
              if((aimBuffer[i] > '9' ? aimBuffer[i] - 'A' + 10 : aimBuffer[i] - '0') >= base) {
                displayCalcErrorMessage(ERROR_INVALID_INTEGER_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
                #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                  sprintf(errorMessage, "digit %c is not allowed in base %d!", aimBuffer[i], base);
                  moreInfoOnError("In function closeNIM:", errorMessage, NULL, NULL);
                #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)

                undo();
                return;
              }
            }

            longIntegerInit(value);
            aimBuffer[posHash] = 0;
            stringToLongInteger(aimBuffer + (aimBuffer[0] == '+' ? 1 : 0), base, value);

            // maxVal = 2^shortIntegerWordSize
            longIntegerInit(maxVal);
            if(shortIntegerWordSize >= 1 && shortIntegerWordSize <= 64) {
              longInteger2Pow(shortIntegerWordSize, maxVal);
            }
            else {
              sprintf(errorMessage, "In function closeNIM: %d is an unexpected value for shortIntegerWordSize!", shortIntegerWordSize);
              displayBugScreen(errorMessage);
              longIntegerFree(maxVal);
              longIntegerFree(value);
              return;
            }

            // minVal = -maxVal/2
            longIntegerInit(minVal);
            longIntegerSetZero(minVal); // Mandatory! Else segmentation fault at next instruction
            longIntegerDivideUInt(maxVal, 2, minVal); // minVal = maxVal / 2
            longIntegerSetNegativeSign(minVal); // minVal = -minVal

            if((base != 2) && (base != 4) && (base != 8) && (base != 16) && (shortIntegerMode != SIM_UNSIGN)) {
              longIntegerDivideUInt(maxVal, 2, maxVal); // maxVal /= 2
            }

            longIntegerSubtractUInt(maxVal, 1, maxVal); // maxVal--

            if(shortIntegerMode == SIM_UNSIGN) {
              longIntegerSetZero(minVal); // minVal = 0
            }

            if(shortIntegerMode == SIM_1COMPL || shortIntegerMode == SIM_SIGNMT) {
              longIntegerAddUInt(minVal, 1, minVal); // minVal++
            }

            if(longIntegerCompare(value, minVal) < 0 || longIntegerCompare(value, maxVal) > 0) {
              displayCalcErrorMessage(ERROR_WORD_SIZE_TOO_SMALL, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                char strMin[22], strMax[22];
                longIntegerToAllocatedString(minVal, strMin, sizeof(strMin));
                longIntegerToAllocatedString(maxVal, strMax, sizeof(strMax));
                sprintf(errorMessage, "For word size of %d bit%s and integer mode %s,", shortIntegerWordSize, shortIntegerWordSize>1 ? "s" : "", getShortIntegerModeName(shortIntegerMode));
                sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "the entered number must be from %s to %s!", strMin, strMax);
                moreInfoOnError("In function closeNIM:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
              #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
              longIntegerFree(maxVal);
              longIntegerFree(minVal);
              longIntegerFree(value);
              return;
            }

            reallocateRegister(REGISTER_X, dtShortInteger, SHORT_INTEGER_SIZE, base);

            char strValue[22];
            longIntegerToAllocatedString(value, strValue, sizeof(strValue));

            uint64_t val = strtoull(strValue + (longIntegerIsNegative(value) ? 1 : 0), NULL, 10); // when value is negative: discard the minus sign

            if(shortIntegerMode == SIM_UNSIGN) {
            }
            else if(shortIntegerMode == SIM_2COMPL) {
              if(longIntegerIsNegative(value)) {
                val = (~val + 1) & shortIntegerMask;
              }
            }
            else if(shortIntegerMode == SIM_1COMPL) {
              if(longIntegerIsNegative(value)) {
                val = ~val & shortIntegerMask;
              }
            }
            else if(shortIntegerMode == SIM_SIGNMT) {
              if(longIntegerIsNegative(value)) {
                val = (val & shortIntegerMask) | shortIntegerSignBit;
              }
            }
            else {
              sprintf(errorMessage, "In function closeNIM: %d is an unexpected value for shortIntegerMode!", shortIntegerMode);
              displayBugScreen(errorMessage);
              *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = 0;
            }

            *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = val;
            lastIntegerBase = base;
            fnRefreshState();                                                //JMNIM
            aimBuffer[0]=0;                                      //JMNIM Clear the NIM input buffer once written to register successfully.

            longIntegerFree(maxVal);
            longIntegerFree(minVal);
            longIntegerFree(value);
          }
          else if(nimNumberPart == NP_REAL_FLOAT_PART || nimNumberPart == NP_REAL_EXPONENT) {
          
              if(Input_Default == ID_CPXDP) {                                         //JM Input default type
                reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone); //JM Input default type
                stringToReal34(aimBuffer, REGISTER_REAL34_DATA(REGISTER_X));          //JM Input default type
                stringToReal34("0", REGISTER_IMAG34_DATA(REGISTER_X));                //JM Input default type
              }                                                                       //JM Input default type
              else {                                                                  //JM Input default type
              reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
              stringToReal34(aimBuffer, REGISTER_REAL34_DATA(REGISTER_X));
              }                                                                       //JM Input default type

          }
          else if(nimNumberPart == NP_FRACTION_DENOMINATOR) {
            int16_t i, posSpace, posSlash, lg;
            int32_t integer, numer, denom;
            real34_t temp;

            // Set Fraction mode
            if(!getSystemFlag(FLAG_FRACT)) {
              setSystemFlag(FLAG_FRACT);
            }

            lg = strlen(aimBuffer);

            posSpace = 0;
            for(i=2; i<lg; i++) {
              if(aimBuffer[i] == ' ') {
                posSpace = i;
                break;
              }
            }

            for(i=1; i<posSpace; i++) {
              if(aimBuffer[i]<'0' || aimBuffer[i]>'9') { // This should never happen
                displayCalcErrorMessage(ERROR_BAD_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
                #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                  moreInfoOnError("In function parseNimString:", "there is a non numeric character in the integer part of the fraction!", NULL, NULL);
                #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
                return;
              }
            }

            posSlash = 0;
            for(i=posSpace+2; i<lg; i++) {
              if(aimBuffer[i] == '/') {
                posSlash = i;
                break;
              }
            }

            for(i=posSpace+1; i<posSlash; i++) {
              if(aimBuffer[i]<'0' || aimBuffer[i]>'9') { // This should never happen
               displayCalcErrorMessage(ERROR_BAD_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
               #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                 moreInfoOnError("In function parseNimString:", "there is a non numeric character in the numerator part of the fraction!", NULL, NULL);
               #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
               return;
              }
            }

            for(i=posSlash+1; i<lg; i++) {
              if(aimBuffer[i]<'0' || aimBuffer[i]>'9') {
                displayCalcErrorMessage(ERROR_BAD_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
                #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                  moreInfoOnError("In function parseNimString:", "there is a non numeric character in the denominator part of the fraction!", NULL, NULL);
                #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
                return;
              }
            }

            aimBuffer[posSpace] = 0;
            aimBuffer[posSlash] = 0;
            integer = stringToInt32(aimBuffer + 1);
            numer   = stringToInt32(aimBuffer + posSpace + 1);
            denom   = stringToInt32(aimBuffer + posSlash + 1);

            if(denom == 0 && !getSystemFlag(FLAG_SPCRES)) {
              displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                moreInfoOnError("In function parseNimString:", "the denominator of the fraction should not be 0!", "Unless D flag (Danger) is set.", NULL);
              #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
              return;
            }

            reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
            int32ToReal34(numer, REGISTER_REAL34_DATA(REGISTER_X));
            int32ToReal34(denom, &temp);
            real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &temp, REGISTER_REAL34_DATA(REGISTER_X));
            int32ToReal34(integer, &temp);
            real34Add(REGISTER_REAL34_DATA(REGISTER_X), &temp, REGISTER_REAL34_DATA(REGISTER_X));
            if(aimBuffer[0] == '-') {
              real34SetNegativeSign(REGISTER_REAL34_DATA(REGISTER_X));
            }
          }
          else if(nimNumberPart == NP_COMPLEX_INT_PART || nimNumberPart == NP_COMPLEX_FLOAT_PART || nimNumberPart == NP_COMPLEX_EXPONENT) {
            int16_t imaginarySign;

            if(aimBuffer[imaginaryMantissaSignLocation] == '+') {
              imaginarySign = 1;
            }
            else {
              imaginarySign = -1;
            }
            aimBuffer[imaginaryMantissaSignLocation] = 0;

            reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
            stringToReal34(aimBuffer, REGISTER_REAL34_DATA(REGISTER_X));

            stringToReal34(aimBuffer + imaginaryMantissaSignLocation + 2, REGISTER_IMAG34_DATA(REGISTER_X));
            if(imaginarySign == -1) {
              real34SetNegativeSign(REGISTER_IMAG34_DATA(REGISTER_X));
            }

            if(getSystemFlag(FLAG_POLAR)) { // polar mode
              if(real34CompareEqual(REGISTER_REAL34_DATA(REGISTER_X), const34_0)) {
                real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
              }
              else {
                real_t magnitude, theta;

                real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &magnitude);
                real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &theta);
                convertAngleFromTo(&theta, currentAngularMode, amRadian, &ctxtReal39);
                if(realCompareLessThan(&magnitude, const_0)) {
                  realSetPositiveSign(&magnitude);
                  realAdd(&theta, const_pi, &theta, &ctxtReal39);
                  WP34S_Mod(&theta, const1071_2pi, &theta, &ctxtReal39);
                }
                realPolarToRectangular(&magnitude, &theta, &magnitude, &theta, &ctxtReal39); // theta in radian
                realToReal34(&magnitude, REGISTER_REAL34_DATA(REGISTER_X));
                realToReal34(&theta,     REGISTER_IMAG34_DATA(REGISTER_X));
              }
            }
            fnSetFlag(FLAG_CPXRES);
            fnRefreshState();                                 //drJM
          }
          else {
            sprintf(errorMessage, "In function closeNIM: %d is an unexpected nimNumberPart value!", nimNumberPart);
            displayBugScreen(errorMessage);
          }
        }
      }
    }
  }

  void closeAim(void) {
    calcModeNormal();
    popSoftmenu();

    if(aimBuffer[0] == 0) {
      undo();
    }
    else {
      int16_t len = stringByteLength(aimBuffer) + 1;

      reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), amNone);

      xcopy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, len);
      aimBuffer[0] = 0;

      setSystemFlag(FLAG_ASLIFT);
    }
  }
#endif // TESTSUITE_BUILD
