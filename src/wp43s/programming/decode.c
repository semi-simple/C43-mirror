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


void listPrograms(void) {
}


void getStringLabelOrVariableName(uint8_t *stringAddress) {
  uint8_t stringLength = *(uint8_t *)(stringAddress++);
  xcopy(tmpStr3000 + 1000, stringAddress, stringLength);
  tmpStr3000[1000 + stringLength] = 0;
}


void getIndirectRegister(uint8_t *paramAddress, const char *op) {
  uint8_t opParam = *(uint8_t *)paramAddress;
  if(opParam < REGISTER_X) { // Global register from 00 to 99
    sprintf(tmpStr3000, "%s " STD_RIGHT_ARROW "%02u", op, opParam);
  }
  else if(opParam <= REGISTER_K) { // Lettered register from X to K
    sprintf(tmpStr3000, "%s " STD_RIGHT_ARROW "%s", op, indexOfItems[ITM_ST_X + opParam - REGISTER_X].itemSoftmenuName);
  }
  else if(opParam <= LAST_LOCAL_REGISTER) { // Local register from .00 to .98
    sprintf(tmpStr3000, "%s " STD_RIGHT_ARROW ".%02u", op, opParam - FIRST_LOCAL_REGISTER);
  }
  else {
    sprintf(tmpStr3000, "In function getIndirectRegister: %s " STD_RIGHT_ARROW " %u is not a valid parameter!", op, opParam);
  }
}


void getIndirectVariable(uint8_t *stringAddress, const char *op) {
  getStringLabelOrVariableName(stringAddress);
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wrestrict"
  sprintf(tmpStr3000, "%s " STD_RIGHT_ARROW STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpStr3000 + 1000);
  #pragma GCC diagnostic pop
}


void decodeOp(uint8_t *paramAddress, const char *op, uint16_t paramMode) {
  uint8_t opParam = *(uint8_t *)(paramAddress++);

  switch(paramMode) {
    case PARAM_DECLARE_LABEL:
         if(opParam <= 99) { // Local label from 00 to 99
           sprintf(tmpStr3000, "%s %02u", op, opParam);
         }
         else if(opParam <= 109) { // Local label from A to J
           sprintf(tmpStr3000, "%s %c", op, 'A' + (opParam - 100));
         }
         else if(opParam == STRING_LABEL_VARIABLE) {
           getStringLabelOrVariableName(paramAddress);
           #pragma GCC diagnostic push
           #pragma GCC diagnostic ignored "-Wrestrict"
           sprintf(tmpStr3000, "%s " STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpStr3000 + 1000);
           #pragma GCC diagnostic pop
         }
         else {
           sprintf(tmpStr3000, "In function decodeOp case PARAM_DECLARE_LABEL: opParam %u is not a valid label!\n", opParam);
         }
         break;

    case PARAM_LABEL:
         if(opParam <= 99) { // Local label from 00 to 99
           sprintf(tmpStr3000, "%s %02u", op, opParam);
         }
         else if(opParam <= 109) { // Local label from A to J
           sprintf(tmpStr3000, "%s %c", op, 'A' + (opParam - 100));
         }
         else if(opParam == STRING_LABEL_VARIABLE) {
           getStringLabelOrVariableName(paramAddress);
           #pragma GCC diagnostic push
           #pragma GCC diagnostic ignored "-Wrestrict"
           sprintf(tmpStr3000, "%s " STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpStr3000 + 1000);
           #pragma GCC diagnostic pop
         }
         else if(opParam == INDIRECT_REGISTER) {
           getIndirectRegister(paramAddress, op);
         }
         else if(opParam == INDIRECT_VARIABLE) {
           getIndirectVariable(paramAddress, op);
         }
         else {
           sprintf(tmpStr3000, "In function decodeOp: case PARAM_LABEL, %s  %u is not a valid parameter!", op, opParam);
         }
         break;

    case PARAM_REGISTER:
         if(opParam < REGISTER_X) { // Global register from 00 to 99
           sprintf(tmpStr3000, "%s %02u", op, opParam);
         }
         else if(opParam <= REGISTER_K) { // Lettered register from X to K
           sprintf(tmpStr3000, "%s %s", op, indexOfItems[ITM_ST_X + opParam - REGISTER_X].itemSoftmenuName);
         }
         else if(opParam <= LAST_LOCAL_REGISTER) { // Local register from .00 to .98
           sprintf(tmpStr3000, "%s .%02u", op, opParam - FIRST_LOCAL_REGISTER);
         }
         else if(opParam == STRING_LABEL_VARIABLE) {
           getStringLabelOrVariableName(paramAddress);
           #pragma GCC diagnostic push
           #pragma GCC diagnostic ignored "-Wrestrict"
           sprintf(tmpStr3000, "%s " STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpStr3000 + 1000);
           #pragma GCC diagnostic pop
         }
         else if(opParam == INDIRECT_REGISTER) {
           getIndirectRegister(paramAddress, op);
         }
         else if(opParam == INDIRECT_VARIABLE) {
           getIndirectVariable(paramAddress, op);
         }
         else {
           sprintf(tmpStr3000, "In function decodeOp: case PARAM_REGISTER, %s  %u is not a valid parameter!", op, opParam);
         }
         break;

    case PARAM_FLAG:
         if(opParam < REGISTER_X) { // Global flag from 00 to 99
           sprintf(tmpStr3000, "%s %02u", op, opParam);
         }
         else if(opParam <= REGISTER_K) { // Lettered flag from X to K
           sprintf(tmpStr3000, "%s %s", op, indexOfItems[ITM_ST_X + opParam - REGISTER_X].itemSoftmenuName);
         }
         else if(opParam <= LAST_LOCAL_FLAG) { // Local flag from .00 to .15 (or .31)
           sprintf(tmpStr3000, "%s .%02u", op, opParam - FIRST_LOCAL_FLAG);
         }
         else if(FIRST_LOCAL_FLAG + NUMBER_OF_LOCAL_FLAGS <= opParam && opParam < FIRST_LOCAL_FLAG + NUMBER_OF_LOCAL_FLAGS + NUMBER_OF_SYSTEM_FLAGS) { // Local register from .00 to .15 (or .31)
           sprintf(tmpStr3000, "%s .%02u", op, opParam - FIRST_LOCAL_FLAG);
         }
         else if(opParam == INDIRECT_REGISTER) {
           getIndirectRegister(paramAddress, op);
         }
         else if(opParam == INDIRECT_VARIABLE) {
           getIndirectVariable(paramAddress, op);
         }
         else {
           sprintf(tmpStr3000, "In function decodeOp: case PARAM_FLAG, %s  %u is not a valid parameter!", op, opParam);
         }
         break;

    case PARAM_NUMBER:
         if(opParam <= 99) { // Value from 0 to 99
           sprintf(tmpStr3000, "%s %02u", op, opParam);
         }
         else if(opParam == INDIRECT_REGISTER) {
           getIndirectRegister(paramAddress, op);
         }
         else if(opParam == INDIRECT_VARIABLE) {
           getIndirectVariable(paramAddress, op);
         }
         else {
           sprintf(tmpStr3000, "In function decodeOp: case PARAM_NUMBER, %s  %u is not a valid parameter!", op, opParam);
         }
         break;

    case PARAM_COMPARE:
         if(opParam < REGISTER_X) { // Global register from 00 to 99
           sprintf(tmpStr3000, "%s %02u", op, opParam);
         }
         else if(opParam <= REGISTER_K) { // Lettered register from X to K
           sprintf(tmpStr3000, "%s %s", op, indexOfItems[ITM_ST_X + opParam - REGISTER_X].itemSoftmenuName);
         }
         else if(opParam <= LAST_LOCAL_REGISTER) { // Local register from .00 to .98
           sprintf(tmpStr3000, "%s .%02u", op, opParam - FIRST_LOCAL_REGISTER);
         }
         else if(opParam == STRING_LABEL_VARIABLE) {
           getStringLabelOrVariableName(paramAddress);
           #pragma GCC diagnostic push
           #pragma GCC diagnostic ignored "-Wrestrict"
           sprintf(tmpStr3000, "%s " STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpStr3000 + 1000);
           #pragma GCC diagnostic pop
         }
         else if(opParam == VALUE_0) {
           sprintf(tmpStr3000, "%s 0.", op);
         }
         else if(opParam == VALUE_1) {
           sprintf(tmpStr3000, "%s 1.", op);
         }
         else if(opParam == INDIRECT_REGISTER) {
           getIndirectRegister(paramAddress, op);
         }
         else if(opParam == INDIRECT_VARIABLE) {
           getIndirectVariable(paramAddress, op);
         }
         else {
           sprintf(tmpStr3000, "In function decodeOp: case PARAM_COMPARE, %s  %u is not a valid parameter!", op, opParam);
         }
         break;

    default:
         sprintf(tmpStr3000, "In function decodeOp: paramMode %u is not valid!\n", paramMode);
  }
}


void decodeLITT(uint8_t *litteralAddress) {
  switch(*(uint8_t *)(litteralAddress++)) {
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
      getStringLabelOrVariableName(litteralAddress);
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wrestrict"
      sprintf(tmpStr3000, "%s", tmpStr3000 + 1000);
      #pragma GCC diagnostic pop
      if(strchr(tmpStr3000, 'e') == NULL && strchr(tmpStr3000, '.') == NULL) {
        strcat(tmpStr3000, RADIX34_MARK_STRING);
      }
      break;

    //case BINARY_COMPLEX34:
    //  break;

    //case STRING_COMPLEX34:
    //  break;

    case STRING_LABEL_VARIABLE:
      getStringLabelOrVariableName(litteralAddress);
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wrestrict"
      sprintf(tmpStr3000, STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, tmpStr3000 + 1000);
      #pragma GCC diagnostic pop
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
      printf("ERROR: %u is not an acceptable parameter for ITM_LITT!\n", *(uint8_t *)(litteralAddress - 1));
      break;
  }
}


void decodeOneStep(uint8_t *stepAddress) {
  uint8_t item8 = *(uint8_t *)(stepAddress++);
  uint16_t item16;

  switch(item8) {
    case ITM_LBL:         //   1
      decodeOp(stepAddress, indexOfItems[item8].itemSoftmenuName, PARAM_DECLARE_LABEL);
      break;

    case ITM_GTO:         //   2
    case ITM_XEQ:         //   3
      decodeOp(stepAddress, indexOfItems[item8].itemSoftmenuName, PARAM_LABEL);
      break;

    case ITM_ISE:         //   5
    case ITM_ISG:         //   6
    case ITM_ISZ:         //   7
    case ITM_DSE:         //   8
    case ITM_DSL:         //   9
    case ITM_DSZ:         //  10
    case ITM_STO:         //  44
    case ITM_STOADD:      //  45
    case ITM_STOSUB:      //  46
    case ITM_STOMULT:     //  47
    case ITM_STODIV:      //  48
    case ITM_STOMAX:      //  49
    case ITM_STOMIN:      //  50
    case ITM_RCL:         //  51
    case ITM_RCLADD:      //  52
    case ITM_RCLSUB:      //  53
    case ITM_RCLMULT:     //  54
    case ITM_RCLDIV:      //  55
    case ITM_RCLMAX:      //  56
    case ITM_RCLMIN:      //  57
    case ITM_DEC:         //  91
    case ITM_INC:         //  92
      decodeOp(stepAddress, indexOfItems[item8].itemSoftmenuName, PARAM_REGISTER);
      break;

    case ITM_XEQU:        //  11
    case ITM_XNE:         //  12
    case ITM_XAEQU:       //  15
    case ITM_XLT:         //  16
    case ITM_XLE:         //  17
    case ITM_XGE:         //  18
    case ITM_XGT:         //  19
      decodeOp(stepAddress, indexOfItems[item8].itemSoftmenuName, PARAM_COMPARE);
      break;

    case ITM_FC:          //  20
    case ITM_FS:          //  21
    case ITM_CF:          // 110
    case ITM_SF:          // 111
    case ITM_FF:          // 112
      decodeOp(stepAddress, indexOfItems[item8].itemSoftmenuName, PARAM_FLAG);
      break;

    case ITM_RTN:         //   4
    case ITM_XEQUP0:      //  13
    case ITM_XEQUM0:      //  14
    case ITM_EVEN:        //  22
    case ITM_ODD:         //  23
    case ITM_FPQ:         //  24
    case ITM_INT:         //  25
    case ITM_CPX:         //  26
    case ITM_MATR:        //  27
    case ITM_NAN:         //  28
    case ITM_REAL:        //  29
    case ITM_SPEC:        //  30
    case ITM_STRI:        //  31
    case ITM_PMINFINITY:  //  32
    case ITM_PRIME:       //  33
    case ITM_TOP:         //  34
    case ITM_ENTER:       //  35
    case ITM_XexY:        //  36
    case ITM_DROP:        //  37
    case ITM_DROPY:       //  38
    case ITM_Rup:         //  39
    case ITM_Rdown:       //  40
    case ITM_CLX:         //  41
    case ITM_FILL:        //  42
    case ITM_INPUT:       //  43
    case ITM_SQUARE:      //  58
    case ITM_CUBE:        //  59
    case ITM_YX:          //  60
    case ITM_SQUAREROOTX: //  61
    case ITM_CUBEROOT:    //  62
    case ITM_XTHROOT:     //  63
    case ITM_2X:          //  64
    case ITM_EXP:         //  65
    case ITM_EX1:         //  66
    case ITM_10x:         //  67
    case ITM_LOG2:        //  68
    case ITM_LN:          //  69
    case ITM_LN1X:        //  70
    case ITM_LOG10:       //  71
    case ITM_LOGXY:       //  72
    case ITM_1ONX:        //  73
    case ITM_cos:         //  74
    case ITM_cosh:        //  75
    case ITM_sin:         //  76
    case ITM_sinc:        //  77
    case ITM_sinh:        //  78
    case ITM_tan:         //  79
    case ITM_tanh:        //  80
    case ITM_arccos:      //  81
    case ITM_arcosh:      //  82
    case ITM_arcsin:      //  83
    case ITM_arsinh:      //  84
    case ITM_arctan:      //  85
    case ITM_artanh:      //  86
    case ITM_CEIL:        //  87
    case ITM_FLOOR:       //  88
    case ITM_GCD:         //  89
    case ITM_LCM:         //  90
    case ITM_IP:          //  93
    case ITM_FP:          //  94
    case ITM_ADD:         //  95
    case ITM_SUB:         //  96
    case ITM_CHS:         //  97
    case ITM_MULT:        //  98
    case ITM_DIV:         //  99
    case ITM_IDIV:        // 100
    case ITM_IDIVR:       // 101
    case ITM_MOD:         // 102
    case ITM_MAX:         // 103
    case ITM_MIN:         // 104
    case ITM_MAGNITUDE:   // 105
    case ITM_NEIGHB:      // 106
    case ITM_NEXTP:       // 107
    case ITM_XFACT:       // 108
    case ITM_pi:          // 109
    case ITM_sincpi:      // 113
      sprintf(tmpStr3000, "%s", indexOfItems[item8].itemSoftmenuName);
      break;

    case ITM_LITT:        // 114
       decodeLITT(stepAddress);
       break;

    default:
      if((item8 & 0x80) == 0) {
        printf("ERROR: single byte instruction %u is unknown!\n", item8);
        break;
      }

      item16 = ((uint16_t)(item8 & 0x7F) << 8) | *(uint8_t *)(stepAddress++);
      switch(item16) {
        case ITM_CNST:     //   207
        case ITM_ALL:      //  1400
        case ITM_ENG:      //  1450
        case ITM_FIX:      //  1463
        case ITM_LocR:     //  1504
        case ITM_SCI:      //  1577
          decodeOp(stepAddress, indexOfItems[item16].itemSoftmenuName, PARAM_NUMBER);
          break;

        case ITM_CtoF:     //   220
        case ITM_FtoC:     //   221
        case ITM_DBtoPR:   //   222
        case ITM_DBtoFR:   //   225
        case ITM_PRtoDB:   //   228
        case ITM_FRtoDB:   //   231
        case ITM_ACtoM2:   //   234
        case ITM_M2toAC:   //   236
        case ITM_ACUStoM2: //   238
        case ITM_M2toACUS: //   240
        case ITM_PAtoATM:  //   242
        case ITM_ATMtoPA:  //   243
        case ITM_CLREGS:   //  1417
        case ITM_CLSTK:    //  1418
        case ITM_END:      //  1448
        case ITM_STOP:     //  1604
        case ITM_TICKS:    //  1610
          sprintf(tmpStr3000, "%s", indexOfItems[item16].itemSoftmenuName);
          break;

        case 0x7fff:     // 32767
          break;

        default:
          printf("ERROR: double byte instruction %u is unknown!\n", ((uint16_t)(item8 & 0x7F) << 8) | *(uint8_t *)(stepAddress - 1));
          break;
      }
  }
}
