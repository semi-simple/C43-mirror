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
 * \file decode.c
 ***********************************************/

#include "wp43s.h"

void *decodeOneStep(void *stepAddress) {
  uint16_t step16, numberOfGlyphs, numberOfBytes;
  uint8_t step8 = *(uint8_t *)stepAddress;
  uint8_t param = *(uint8_t *)(stepAddress + 1);

  switch(step8) {
    case ITM_LBL: // 1
      if(param <= 99) {
        printf(" 2  LBL %02u\n", param);
        return stepAddress + 2;
      }
      else if(param <= 109) {
        printf(" 2  LBL %c\n", 'A' + (param - 100));
        return stepAddress + 2;
      }
      else if(param == 255) {
        stepAddress += 2;
        param = *(uint8_t *)stepAddress++;
        numberOfGlyphs = param >> 4;
        numberOfBytes = param & 0x0f;

        xcopy(tmpStr3000, stepAddress, numberOfBytes);
        tmpStr3000[numberOfBytes] = 0;
        stringToUtf8(tmpStr3000, (uint8_t *)(tmpStr3000 + 1000));
        printf("%2u  LBL '%s'\n", 3 + numberOfBytes, tmpStr3000 + 1000);

        return stepAddress + numberOfBytes;
      }
      else {
        printf("ERROR: %u is not an acceptable parameter for ITM_LBL! It must be from 0 to 109 or 255.\n", param);
        return NULL;
      }
      break;


    case ITM_XEQ: // 3
      if(param <= 99) {
        printf(" 2  XEQ %02u\n", param);
        return stepAddress + 2;
      }
      else if(param <= 109) {
        printf(" 2  XEQ %c\n", 'A' + (param - 100));
        return stepAddress + 2;
      }
      else if(param == 255) {
        stepAddress += 2;
        param = *(uint8_t *)stepAddress++;
        numberOfGlyphs = param >> 4;
        numberOfBytes = param & 0x0f;

        xcopy(tmpStr3000, stepAddress, numberOfBytes);
        tmpStr3000[numberOfBytes] = 0;
        stringToUtf8(tmpStr3000, (uint8_t *)(tmpStr3000 + 1000));
        printf("%2u  XEQ '%s'\n", 3 + numberOfBytes, tmpStr3000 + 1000);

        return stepAddress + numberOfBytes;
      }
      else {
        printf("ERROR: %u is not an acceptable parameter for ITM_XEQ! It must be from 0 to 109 or 255.\n", param);
        return NULL;
      }
      break;


    case ITM_STO: // 44
      if(param <= 99) {
        printf(" 2  STO %02u\n", param);
        return stepAddress + 2;
      }
      else if(param <= 111) {
        printf(" 2  STO %c\n", "XYZTABCDLIJK"[param - 100]);
        return stepAddress + 2;
      }
      else if(param <= 210) {
        printf(" 2  STO .%02u\n", param - 112);
        return stepAddress + 2;
      }
      else {
        printf("ERROR: %u is not an acceptable parameter for ITM_STO! It must be from 0 to 210.\n", param);
        return NULL;
      }
      break;


    case ITM_SQUAREROOTX: // 61
      printf(" 1  √x\n");
      return stepAddress + 1;
      break;


    case ITM_IP: // 93
      printf(" 1  IP\n");
      return stepAddress + 1;
      break;


    case ITM_MAGNITUDE: // 105
      printf(" 1  |x|\n");
      return stepAddress + 1;
      break;


    case ITM_LITT: // 114
      switch(param) {
        //case 1: // Binary short integer
        //  break;

        //case 2: // String short integer
        //  break;

        //case 3: // Long integer
        //  break;

        //case 4: // Binary real34
        //  break;

        case 5: // String real34
          stepAddress += 2;
          numberOfBytes = *(uint8_t *)stepAddress++;

          xcopy(tmpStr3000, stepAddress, numberOfBytes);
          tmpStr3000[numberOfBytes] = 0;
          printf("%2u  %s\n", 3 + numberOfBytes, tmpStr3000);

          return stepAddress + numberOfBytes;
          break;

        //case 6: // Binary complex34
        //  break;

        //case 7: // String complex34
        //  break;

        //case 8: // String
        //  break;

        //case 9: // Date
        //  break;

        //case 10: // Time
        //  break;

        default:
          printf("ERROR: %u is not an acceptable parameter for ITM_LITT! It must be from 1 to 10.\n", param);
          return NULL;
      }
      break;


    default:
      if((step8 & 0x80) == 0) {
        printf("ERROR: single byte instruction %u is unknown!\n", step8);
        return NULL;
      }

      step16 = ((uint16_t)(step8 & 0x7F) << 8) | param;

      switch(step16) {
        case ITM_END:
          printf(" 1  END\n");
          return stepAddress + 2;

        case 0x7fff:
          printf(" 2  .END.\n");
          return NULL;


        default:
          printf("ERROR: double byte instruction %u is unknown!\n", step16);
          return NULL;
      }
  }
}

