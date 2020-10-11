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

void getStringLabelOrVariableName(void) {
  param = *(uint8_t *)(stepAddress++);
  xcopy(tmpStr3000 + 2000, stepAddress, param);
  tmpStr3000[2000 + param] = 0;
}


void decodeOp(const char *op) {
  stepAddress++;
  if(param < REGISTER_X) { // Global register from 00 to 99
    sprintf(tmpStr3000 + 1000, "%s %02u", op, param);
  }

  else if(param <= REGISTER_K) { // Lettered register from X to K
    sprintf(tmpStr3000 + 1000, "%s %s", op, indexOfItems[ITM_ST_X + param - REGISTER_X].itemSoftmenuName);
  }
  else if(param <= LAST_LOCAL_REGISTER) { // Local register from .00 to .98
    sprintf(tmpStr3000 + 1000, "%s .%02u", op, param - FIRST_LOCAL_REGISTER);
  }

  else if(param == VALUE_0) {
    sprintf(tmpStr3000 + 1000, "%s 0.", op);
  }

  else if(param == VALUE_1) {
    sprintf(tmpStr3000 + 1000, "%s 1.", op);
  }

  else if(param == STRING_LABEL_VARIABLE) {
    getStringLabelOrVariableName();
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wrestrict"
    sprintf(tmpStr3000 + 1000, "%s " STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpStr3000 + 2000);
    #pragma GCC diagnostic pop
    stepAddress += param;
  }

  else if(param == INDIRECT_REGISTER) {
    param = *(uint8_t *)(stepAddress++);
    if(param < REGISTER_X) { // Global register from 00 to 99
      sprintf(tmpStr3000 + 1000, "%s " STD_RIGHT_ARROW "%02u", op, param);
    }
    else if(param <= REGISTER_K) { // Lettered register from X to K
      sprintf(tmpStr3000 + 1000, "%s " STD_RIGHT_ARROW "%s", op, indexOfItems[ITM_ST_X + param - REGISTER_X].itemSoftmenuName);
    }
    else if(param <= LAST_LOCAL_REGISTER) { // Local register from .00 to .98
      sprintf(tmpStr3000 + 1000, "%s " STD_RIGHT_ARROW ".%02u", op, param - FIRST_LOCAL_REGISTER);
    }
    else {
      sprintf(tmpStr3000, "%s " STD_RIGHT_ARROW "ERROR: %u is not an acceptable parameter!", op, param);
      stepAddress = NULL;
    }
  }

  else if(param == INDIRECT_VARIABLE) {
    param = *(uint8_t *)(stepAddress++);
    getStringLabelOrVariableName();
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wrestrict"
    sprintf(tmpStr3000 + 1000, "%s " STD_RIGHT_ARROW STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpStr3000 + 2000);
    #pragma GCC diagnostic pop
    stepAddress += param;
  }

  else {
    sprintf(tmpStr3000, "%s ERROR: %u is not an acceptable parameter!", op, param);
    stepAddress = NULL;
  }

  stringToUtf8(tmpStr3000 + 1000, (uint8_t *)tmpStr3000);
}


void *decodeLBL(void) {
  decodeOp("LBL");
  printf("%s\n", tmpStr3000);
  return stepAddress;
}


void *decodeGTO(void) {
  decodeOp("GTO");
  printf("%s\n", tmpStr3000);
  return stepAddress;
}


void *decodeXEQ(void) {
  decodeOp("XEQ");
  printf("%s\n", tmpStr3000);
  return stepAddress;
}


void *decodeRTN(void) {
  printf("RTN\n");
  return stepAddress;
}


void *decodeISG(void) {
  decodeOp("ISG");
  printf("%s\n", tmpStr3000);
  return stepAddress;
}


void *decodeXNE(void) {
  decodeOp("x " STD_NOT_EQUAL "?");
  printf("%s\n", tmpStr3000);
  return stepAddress;
}


void *decodeXLT(void) {
  decodeOp("x <?");
  printf("%s\n", tmpStr3000);
  return stepAddress;
}


void *decodeXGT(void) {
  decodeOp("x >?");
  printf("%s\n", tmpStr3000);
  return stepAddress;
}


void *decodeSTO(void) {
  decodeOp("STO");
  printf("%s\n", tmpStr3000);
  return stepAddress;
}


void *decodeSTOPLUS(void) {
  decodeOp("STO+");
  printf("%s\n", tmpStr3000);
  return stepAddress;
}


void *decodeRCL(void) {
  decodeOp("RCL");
  printf("%s\n", tmpStr3000);
  return stepAddress;
}


void *decodeSQUARE(void) {
  sprintf(tmpStr3000 + 1000, "x" STD_SUP_2);
  stringToUtf8(tmpStr3000 + 1000, (uint8_t *)tmpStr3000);
  printf("%s\n", tmpStr3000);
  return stepAddress;
}


void *decodeSQUAREROOTX(void) {
  sprintf(tmpStr3000 + 1000, STD_SQUARE_ROOT "x");
  stringToUtf8(tmpStr3000 + 1000, (uint8_t *)tmpStr3000);
  printf("%s\n", tmpStr3000);
  return stepAddress;
}


void *decodeIP(void) {
  printf("IP\n");
  return stepAddress;
}


void *decodeADD(void) {
  printf("+\n");
  return stepAddress;
}


void *decodeSUB(void) {
  printf("-\n");
  return stepAddress;
}


void *decodeMULT(void) {
  xcopy(tmpStr3000 + 1000, STD_CROSS, 3);
  stringToUtf8(tmpStr3000 + 1000, (uint8_t *)tmpStr3000);
  printf("%s\n", tmpStr3000);
  return stepAddress;
}


void *decodeCHS(void) {
  printf("+/-\n");
  return stepAddress;
}


void *decodeDIV(void) {
  printf("/\n");
  return stepAddress;
}


void *decodeMOD(void) {
  printf("MOD\n");
  return stepAddress;
}


void *decodeMAGNITUDE(void) {
  printf("|x|\n");
  return stepAddress;
}


void *decodeLITT(void) {
  stepAddress++;
  switch(param) {
    //case BINARY_SHORT_INTEGER:
    //  break;

    //case STRING_SHORT_INTEGER:
    //  break;

    //case BINARY_LONG_INTEGER:
    //  break;

    //case STRING_LONG_INTEGER:
    //  break;

    //case BINARY_REAL34:
    //  break;

    case STRING_REAL34:
      getStringLabelOrVariableName();
      stringToUtf8(tmpStr3000 + 2000, (uint8_t *)tmpStr3000);
      printf("%s\n", tmpStr3000);
      return stepAddress + param;
      break;

    //case BINARY_COMPLEX34:
    //  break;

    //case STRING_COMPLEX34:
    //  break;

    case STRING_LABEL_VARIABLE:
      getStringLabelOrVariableName();
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wrestrict"
      sprintf(tmpStr3000 + 1000, STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, tmpStr3000 + 2000);
      #pragma GCC diagnostic pop
      stringToUtf8(tmpStr3000 + 1000, (uint8_t *)tmpStr3000);
      printf("%s\n", tmpStr3000);
      return stepAddress + param;
      break;

    //case BINARY_DATE:
    //  break;

    //case STRING_DATE:
    //  break;

    //case BINARY_TIME:
    //  break;

    //case STRING_TIME:
    //  break;

    default:
      printf("ERROR: %u is not an acceptable parameter for ITM_LITT!\n", param);
      return NULL;
  }
}


void *decodeALL(void) {
  stepAddress++;
  printf("ALL %02u\n", param);
  return stepAddress;
}


void *decodeCLREGS(void) {
  printf("CLREGS\n");
  return stepAddress;
}


void *decodeCLSTK(void) {
  printf("CLSTK\n");
  return stepAddress;
}


void *decodeEND(void) {
  printf("END\n");
  return stepAddress;
}


void *decodeSTOP(void) {
  printf("STOP\n");
  return stepAddress;
}


void *decodeDENDD(void) {
  printf(".END.\n");
  return NULL;
}


void *decodeOneStep(void) {
  uint8_t item8 = *(uint8_t *)(stepAddress++);
  uint16_t item16;

  param = *(uint8_t *)stepAddress;

  switch(item8) {
    case ITM_LBL:         return decodeLBL();         //   1
    case ITM_GTO:         return decodeGTO();         //   2
    case ITM_XEQ:         return decodeXEQ();         //   3
    case ITM_RTN:         return decodeRTN();         //   4
    case ITM_ISG:         return decodeISG();         //   6
    case ITM_XNE:         return decodeXNE();         //  12
    case ITM_XLT:         return decodeXLT();         //  16
    case ITM_XGT:         return decodeXGT();         //  19
    case ITM_STO:         return decodeSTO();         //  44
    case ITM_STOPLUS:     return decodeSTOPLUS();     //  45
    case ITM_RCL:         return decodeRCL();         //  51
    case ITM_SQUARE:      return decodeSQUARE();      //  58
    case ITM_SQUAREROOTX: return decodeSQUAREROOTX(); //  61
    case ITM_IP:          return decodeIP();          //  93
    case ITM_ADD:         return decodeADD();         //  95
    case ITM_SUB:         return decodeSUB();         //  96
    case ITM_CHS:         return decodeCHS();         //  97
    case ITM_MULT:        return decodeMULT();        //  98
    case ITM_DIV:         return decodeDIV();         //  99
    case ITM_MOD:         return decodeMOD();         // 102
    case ITM_MAGNITUDE:   return decodeMAGNITUDE();   // 105
    case ITM_LITT:        return decodeLITT();        // 114

    default:
      if((item8 & 0x80) == 0) {
        printf("ERROR: single byte instruction %u is unknown!\n", item8);
        return NULL;
      }

      item16 = ((uint16_t)(item8 & 0x7F) << 8) | param;
      param = *(uint8_t *)(++stepAddress);
      switch(item16) {
        case ITM_ALL:     return decodeALL();         //  1400
        case ITM_CLREGS:  return decodeCLREGS();      //  1417
        case ITM_CLSTK:   return decodeCLSTK();       //  1418
        case ITM_END:     return decodeEND();         //  1448
        case ITM_STOP:    return decodeSTOP();        //  1604
        case 0x7fff:      return decodeDENDD();       // 32767

        default:
          printf("ERROR: double byte instruction %u is unknown!\n", ((uint16_t)(item8 & 0x7F) << 8) | param);
          return NULL;
      }
  }
}

