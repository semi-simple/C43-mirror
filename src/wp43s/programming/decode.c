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

void getVariableName(void) {
  param = (*(uint8_t *)(stepAddress++)) & 0x0f;

  xcopy(tmpStr3000 + 1000, stepAddress, param);
  tmpStr3000[param + 1000] = 0;
  stringToUtf8(tmpStr3000 + 1000, (uint8_t *)tmpStr3000);
}

void *decodeLBL(void) {
  stepAddress++;
  if(param <= 99) {
    printf(" 2  LBL %02u\n", param);
    return stepAddress;
  }
  else if(param <= 109) {
    printf(" 2  LBL %c\n", 'A' + (param - 100));
    return stepAddress;
  }
  else if(param == 255) {
    getVariableName();
    printf("%2u  LBL '%s'\n", 3 + param, tmpStr3000);

    return stepAddress + param;
  }
  else {
    printf("ERROR: %u is not an acceptable parameter for ITM_LBL! It must be from 0 to 109 or 255.\n", param);
    return NULL;
  }
}

void *decodeGTO(void) {
  stepAddress++;
  if(param <= 99) {
    printf(" 2  GTO %02u\n", param);
    return stepAddress;
  }
  else if(param <= 109) {
    printf(" 2  GTO %c\n", 'A' + (param - 100));
    return stepAddress;
  }
  else if(param == 255) {
    getVariableName();
    printf("%2u  GTO '%s'\n", 3 + param, tmpStr3000);

    return stepAddress + param;
  }
  else {
    printf("ERROR: %u is not an acceptable parameter for ITM_GTO! It must be from 0 to 109 or 255.\n", param);
    return NULL;
  }
}


void *decodeXEQ(void) {
  stepAddress++;
  if(param <= 99) {
    printf(" 2  XEQ %02u\n", param);
    return stepAddress;
  }
  else if(param <= 109) {
    printf(" 2  XEQ %c\n", 'A' + (param - 100));
    return stepAddress;
  }
  else if(param == 255) {
    getVariableName();
    printf("%2u  XEQ '%s'\n", 3 + param, tmpStr3000);

    return stepAddress + param;
  }
  else {
    printf("ERROR: %u is not an acceptable parameter for ITM_XEQ! It must be from 0 to 109 or 255.\n", param);
    return NULL;
  }
}


void *decodeRTN(void) {
  printf(" 1  RTN\n");
  return stepAddress;
}


void *decodeXNE(void) {
  stepAddress++;
  if(param <= 99) {
    printf(" 2  x ≠? %02u\n", param);
    return stepAddress;
  }
  else if(param <= 111) {
    printf(" 2  x ≠?  %c\n", "XYZTABCDLIJK"[param - 100]);
    return stepAddress;
  }
  else if(param <= 210) {
    printf(" 2  x ≠? .%02u\n", param - 112);
    return stepAddress;
  }
  else if(param == 250) {
    printf(" 2  x ≠? 0.\n");
    return stepAddress;
  }
  else if(param == 251) {
    printf(" 2  x ≠? 1.\n");
    return stepAddress;
  }
  else if(param == 255) {
    getVariableName();
    printf("%2u  x ≠? '%s'\n", 3 + param, tmpStr3000);

    return stepAddress + param;
  }
  else {
    printf("ERROR: %u is not an acceptable parameter for ITM_XNE! It must be from 0 to 210.\n", param);
    return NULL;
  }
}


void *decodeXLT(void) {
  stepAddress++;
  if(param <= 99) {
    printf(" 2  x <? %02u\n", param);
    return stepAddress;
  }
  else if(param <= 111) {
    printf(" 2  x <?  %c\n", "XYZTABCDLIJK"[param - 100]);
    return stepAddress;
  }
  else if(param <= 210) {
    printf(" 2  x <? .%02u\n", param - 112);
    return stepAddress;
  }
  else if(param == 250) {
    printf(" 2  x <? 0.\n");
    return stepAddress;
  }
  else if(param == 251) {
    printf(" 2  x <? 1.\n");
    return stepAddress;
  }
  else if(param == 255) {
    getVariableName();
    printf("%2u  x <? '%s'\n", 3 + param, tmpStr3000);

    return stepAddress + param;
  }
  else {
    printf("ERROR: %u is not an acceptable parameter for ITM_XLT! It must be from 0 to 210.\n", param);
    return NULL;
  }
}


void *decodeSTO(void) {
  stepAddress++;
  if(param <= 99) {
    printf(" 2  STO %02u\n", param);
    return stepAddress;
  }
  else if(param <= 111) {
    printf(" 2  STO %c\n", "XYZTABCDLIJK"[param - 100]);
    return stepAddress;
  }
  else if(param <= 210) {
    printf(" 2  STO .%02u\n", param - 112);
    return stepAddress;
  }
  else if(param == 255) {
    getVariableName();
    printf("%2u  STO '%s'\n", 3 + param, tmpStr3000);

    return stepAddress + param;
  }
  else {
    printf("ERROR: %u is not an acceptable parameter for ITM_STO! It must be from 0 to 210.\n", param);
    return NULL;
  }
}


void *decodeSTOPLUS(void) {
  stepAddress++;
  if(param <= 99) {
    printf(" 2  STO+ %02u\n", param);
    return stepAddress;
  }
  else if(param <= 111) {
    printf(" 2  STO+ %c\n", "XYZTABCDLIJK"[param - 100]);
    return stepAddress;
  }
  else if(param <= 210) {
    printf(" 2  STO+ .%02u\n", param - 112);
    return stepAddress;
  }
  else if(param == 255) {
    getVariableName();
    printf("%2u  STO+ '%s'\n", 3 + param, tmpStr3000);

    return stepAddress + param;
  }
  else {
    printf("ERROR: %u is not an acceptable parameter for ITM_STOPLUS! It must be from 0 to 210.\n", param);
    return NULL;
  }
}


void *decodeRCL(void) {
  stepAddress++;
  if(param <= 99) {
    printf(" 2  RCL %02u\n", param);
    return stepAddress;
  }
  else if(param <= 111) {
    printf(" 2  RCL %c\n", "XYZTABCDLIJK"[param - 100]);
    return stepAddress;
  }
  else if(param <= 210) {
    printf(" 2  RCL .%02u\n", param - 112);
    return stepAddress;
  }
  else if(param == 255) {
    getVariableName();
    printf("%2u  RCL '%s'\n", 3 + param, tmpStr3000);

    return stepAddress + param;
  }
  else {
    printf("ERROR: %u is not an acceptable parameter for ITM_RCL! It must be from 0 to 210.\n", param);
    return NULL;
  }
}


void *decodeSQUAREROOTX(void) {
  printf(" 1  √x\n");
  return stepAddress;
}


void *decodeIP(void) {
  printf(" 1  IP\n");
  return stepAddress;
}


void *decodeMOD(void) {
  printf(" 1  MOD\n");
  return stepAddress;
}


void *decodeMAGNITUDE(void) {
  printf(" 1  |x|\n");
  return stepAddress;
}


void *decodeLITT(void) {
  uint16_t numberOfBytes;

  stepAddress++;
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
}


void *decodeALL(void) {
  stepAddress++;
  printf(" 3  ALL %02u\n", param);
  return stepAddress;
}


void *decodeEND(void) {
  printf(" 2  END\n");
  return stepAddress;
}


void *decodeSTOP(void) {
  printf(" 2  STOP\n");
  return stepAddress;
}


void *decodeDENDD(void) {
  printf(" 2  .END.\n");
  return NULL;
}


void *decodeOneStep(void) {
  uint8_t step8 = *(uint8_t *)(stepAddress++);
  param = *(uint8_t *)stepAddress;

  switch(step8) {
    case ITM_LBL:         return decodeLBL();         //   1
    case ITM_GTO:         return decodeGTO();         //   2
    case ITM_XEQ:         return decodeXEQ();         //   3
    case ITM_RTN:         return decodeRTN();         //   4
    case ITM_XNE:         return decodeXNE();         //  12
    case ITM_XLT:         return decodeXLT();         //  16
    case ITM_STO:         return decodeSTO();         //  44
    case ITM_STOPLUS:     return decodeSTOPLUS();     //  45
    case ITM_RCL:         return decodeRCL();         //  51
    case ITM_SQUAREROOTX: return decodeSQUAREROOTX(); //  61
    case ITM_IP:          return decodeIP();          //  93
    case ITM_MOD:         return decodeMOD();         // 102
    case ITM_MAGNITUDE:   return decodeMAGNITUDE();   // 105
    case ITM_LITT:        return decodeLITT();        // 114

    default:
      if((step8 & 0x80) == 0) {
        printf("ERROR: single byte instruction %u is unknown!\n", step8);
        return NULL;
      }

      stepAddress++;
      switch(((uint16_t)(step8 & 0x7F) << 8) | param) {
        case ITM_ALL:     return decodeALL();         //  1400
        case ITM_END:     return decodeEND();         //  1448
        case ITM_STOP:    return decodeSTOP();        //  1604
        case 0x7fff:      return decodeDENDD();       // 32767

        default:
          printf("ERROR: double byte instruction %u is unknown!\n", ((uint16_t)(step8 & 0x7F) << 8) | param);
          return NULL;
      }
  }
}

