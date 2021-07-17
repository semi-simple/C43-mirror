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
 * \file generateTestPgms.c
 ***********************************************/

#include <stdlib.h>
#include "fonts.h"
#include "items.h"
#include "defines.h"

#include "wp43s.h"


int main(int argc, char* argv[]) {
  uint8_t memory[65536], *currentStep;
  FILE *testPgms;
  uint32_t sizeOfPgms;

  if(argc < 2) {
    printf("Usage: generateTestPgms <output file>\n");
    return 1;
  }

  memset(memory, 0, 65536);
  currentStep = memory;

  { // Prime number checker
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'P';
    *(currentStep++) = 'r';
    *(currentStep++) = 'i';
    *(currentStep++) = 'm';
    *(currentStep++) = 'e';

    *(currentStep++) = ITM_MAGNITUDE;

    *(currentStep++) = ITM_IP;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_SQUAREROOTX;

    *(currentStep++) = ITM_IP;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 1;

    // 10
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 2;

    // 20
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    // 30
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    // 40
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    // 50
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '8';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    // 60
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    // 70
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '8';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    // 80
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    // 90
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    // 100
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    // 110
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '1';
    *(currentStep++) = '0';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '1';
    *(currentStep++) = '0';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 2;

    // 120
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOP;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_MOD;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = VALUE_0;

    *(currentStep++) = ITM_RTN;

    // 130
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 1;

    // 132
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // Len1
    // 1
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // Len2
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'e';
    *(currentStep++) = 'n';
    *(currentStep++) = '2';

    // 2
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // Len3
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'e';
    *(currentStep++) = 'n';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    // 3
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // Len4
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'e';
    *(currentStep++) = 'n';
    *(currentStep++) = '4';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 3;

    // 4
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // Len5
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'e';
    *(currentStep++) = 'n';
    *(currentStep++) = '5';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 3;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 4;

    // 5
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // Len6
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'e';
    *(currentStep++) = 'n';
    *(currentStep++) = '6';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 3;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 4;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 5;

    // 6
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // Len7
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'e';
    *(currentStep++) = 'n';
    *(currentStep++) = '7';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 3;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 4;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 5;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 6;

    // 7
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // Len8
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'e';
    *(currentStep++) = 'n';
    *(currentStep++) = '8';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 3;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 4;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 5;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 6;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 7;

    // 8
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // Bairstow polynomial root finder
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 7; // String length
    *(currentStep++) = 'B';
    *(currentStep++) = 'a';
    *(currentStep++) = 'i';
    *(currentStep++) = 'r';
    *(currentStep++) = 's';
    *(currentStep++) = 't';
    *(currentStep++) = 'o';

    *(currentStep++) = (ITM_ALL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALL       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_CLREGS >> 8) | 0x80;
    *(currentStep++) =  ITM_CLREGS       & 0xff;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_LABEL_VARIABLE;
    *(currentStep++) = 19; // String length
    *(currentStep++) = 'P';
    *(currentStep++) = 'o';
    *(currentStep++) = 'l';
    *(currentStep++) = 'y';
    *(currentStep++) = 'n';
    *(currentStep++) = 'o';
    *(currentStep++) = 'm';
    *(currentStep++) = 'i';
    *(currentStep++) = 'a';
    *(currentStep++) = 'l';
    *(currentStep++) = ' ';
    *(currentStep++) = 'd';
    *(currentStep++) = 'e';
    *(currentStep++) = 'g';
    *(currentStep++) = 'r';
    *(currentStep++) = 'e';
    *(currentStep++) = 'e';
    *(currentStep++) = ' ';
    *(currentStep++) = '?';

    *(currentStep++) = ITM_STOP;

    *(currentStep++) = ITM_MAGNITUDE;

    *(currentStep++) = ITM_IP;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 90;

    // 10
    *(currentStep++) = ITM_STO;
    *(currentStep++) = 91;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 3; // String length
    *(currentStep++) = '1';
    *(currentStep++) = 'e';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'A' - 'A'; // A

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_LABEL_VARIABLE;
    *(currentStep++) = 2; // String length
    *(currentStep++) = 'x';
    *(currentStep++) = '^';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_ADD;

    // 20
    *(currentStep++) = ITM_STOP;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'A' - 'A'; // A

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    // 30
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 100 + 'B' - 'A'; // B

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_LABEL_VARIABLE;
    *(currentStep++) = 10; // String length
    *(currentStep++) = 'T';
    *(currentStep++) = 'o';
    *(currentStep++) = 'l';
    *(currentStep++) = 'e';
    *(currentStep++) = 'r';
    *(currentStep++) = 'a';
    *(currentStep++) = 'n';
    *(currentStep++) = 'c';
    *(currentStep++) = 'e';
    *(currentStep++) = '?';

    *(currentStep++) = ITM_STOP;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 10;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '3';

    // 40
    *(currentStep++) = ITM_STO;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 95;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 96;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 100 + 'D' - 'A'; // D

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 100 + 'J' - 'A'; // J

    // 50
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 11;

    *(currentStep++) = ITM_STOP;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 10;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 1;

    // 60
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 13;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_LABEL_VARIABLE;
    *(currentStep++) = 9; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'a';
    *(currentStep++) = 's';
    *(currentStep++) = 't';
    *(currentStep++) = ' ';
    *(currentStep++) = 'r';
    *(currentStep++) = 'o';
    *(currentStep++) = 'o';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 100 + 'B' - 'A'; // B

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 93;

    // 70
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 3; // String length
    *(currentStep++) = '1';
    *(currentStep++) = 'e';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 100 + 'C' - 'A'; // C

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    // 80
    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'C' - 'A'; // C

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'D' - 'A'; // D

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 97;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 98;

    // 90
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'I' - 'A'; // I

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 3; // String length
    *(currentStep++) = '1';
    *(currentStep++) = 'e';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 89;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '3';
    *(currentStep++) = '0';

    // 100
    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    // 110
    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'E' - 'A'; // E

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 95;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_CHS;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    // 120
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 96;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_CHS;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 89;

    // 130
    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 89;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'E' - 'A'; // E

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 84;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 85;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '3';
    *(currentStep++) = '0';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 3; // String length
    *(currentStep++) = '1';
    *(currentStep++) = 'e';
    *(currentStep++) = '3';

    // 140
    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 89;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '6';
    *(currentStep++) = '0';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 93;

    // 150
    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'F' - 'A'; // F

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    // 160
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 95;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_CHS;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 96;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_CHS;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    // 170
    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 89;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 89;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'F' - 'A'; // F

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    // 180
    *(currentStep++) = ITM_STO;
    *(currentStep++) = 93;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 95;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 96;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    // 190
    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 88;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 84;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 85;

    // 200
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_SUB;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 88;

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 87;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 95;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 96;

    // 210
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 84;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 85;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 88;

    // 220
    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 86;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 87;

    *(currentStep++) = ITM_MAGNITUDE;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 86;

    *(currentStep++) = ITM_MAGNITUDE;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'G' - 'A'; // G

    // 230
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 87;

    *(currentStep++) = ITM_MAGNITUDE;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 97;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'H' - 'A'; // H

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'G' - 'A'; // G

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 86;

    *(currentStep++) = ITM_MAGNITUDE;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 97;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'H' - 'A'; // H

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 87;

    // 240
    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 95;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 86;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 96;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 98;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 3; // String length
    *(currentStep++) = '1';
    *(currentStep++) = 'e';
    *(currentStep++) = '4';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 98;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 4;

    // 250
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 97;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'I' - 'A';

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'J' - 'A'; // J

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '3';
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    // 260
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_SUB;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 3; // String length
    *(currentStep++) = '1';
    *(currentStep++) = 'e';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 88;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 91;

    // 270
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 88;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_SUB;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 90;

    // 280
    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 11;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 95;

    *(currentStep++) = ITM_CHS;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_SQUARE;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 96;

    // 290
    *(currentStep++) = ITM_SUB;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_SQUAREROOTX;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_SQUAREROOTX;

    // 300
    *(currentStep++) = ITM_SUB;

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 12;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_LABEL_VARIABLE;
    *(currentStep++) = 12; // String length
    *(currentStep++) = '.';
    *(currentStep++) = '.';
    *(currentStep++) = '.';
    *(currentStep++) = ' ';
    *(currentStep++) = 'c';
    *(currentStep++) = 'o';
    *(currentStep++) = 'n';
    *(currentStep++) = 't';
    *(currentStep++) = 'i';
    *(currentStep++) = 'n';
    *(currentStep++) = 'u';
    *(currentStep++) = 'e';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_CHS;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 91;

    // 310
    *(currentStep++) = ITM_SQUARE;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_SUB;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_SQUAREROOTX;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 91;

    // 320
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_SQUAREROOTX;

    *(currentStep++) = ITM_SUB;

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 13;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_LABEL_VARIABLE;
    *(currentStep++) = 12; // String length
    *(currentStep++) = '.';
    *(currentStep++) = '.';
    *(currentStep++) = '.';
    *(currentStep++) = ' ';
    *(currentStep++) = 'c';
    *(currentStep++) = 'o';
    *(currentStep++) = 'n';
    *(currentStep++) = 't';
    *(currentStep++) = 'i';
    *(currentStep++) = 'n';
    *(currentStep++) = 'u';
    *(currentStep++) = 'e';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_CHS;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 91;

    // 330
    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 4;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++)= STRING_LABEL_VARIABLE;
    *(currentStep++) = 11; // String length
    *(currentStep++) = 'E';
    *(currentStep++) = 'r';
    *(currentStep++) = 'r';
    *(currentStep++) = 'o';
    *(currentStep++) = 'r';
    *(currentStep++) = ' ';
    *(currentStep++) = 'm';
    *(currentStep++) = '>';
    *(currentStep++) = '8';
    *(currentStep++) = '0';
    *(currentStep++) = '!';

    *(currentStep++) = ITM_RTN;

    // 337
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // Speed test. See: https://forum.swissmicros.com/viewtopic.php?p=17308
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'S';
    *(currentStep++) = 'p';
    *(currentStep++) = 'e';
    *(currentStep++) = 'e';
    *(currentStep++) = 'd';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 10;

    *(currentStep++) = (ITM_TICKS >> 8) | 0x80;
    *(currentStep++) =  ITM_TICKS       & 0xff;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 11;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 12;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 1;

    // 10
    *(currentStep++) = ITM_arctan;

    *(currentStep++) = ITM_sin;

    *(currentStep++) = ITM_EXP;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '3';

    *(currentStep++) = ITM_1ONX;

    *(currentStep++) = ITM_YX;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 11;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 12;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_ADD;

    // 20
    *(currentStep++) = ITM_STO;
    *(currentStep++) = 12;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = 10;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 1;

    *(currentStep++) = (ITM_TICKS >> 8) | 0x80;
    *(currentStep++) =  ITM_TICKS       & 0xff;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_SUB;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '1';
    *(currentStep++) = '0';

    *(currentStep++) = ITM_DIV;

    // 28
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // Factorial: the recursive way ReM page 261
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 7; // String length
    *(currentStep++) = 'R';
    *(currentStep++) = 'e';
    *(currentStep++) = 'M';
    *(currentStep++) = 'p';
    *(currentStep++) = '2';
    *(currentStep++) = '6';
    *(currentStep++) = '1';

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'F';
    *(currentStep++) = 'a';
    *(currentStep++) = 'c';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_IP;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = VALUE_1;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_LocR >> 8) | 0x80;
    *(currentStep++) =  ITM_LocR       & 0xff;
    *(currentStep++) = 1;

    // 10
    *(currentStep++) = ITM_STO;
    *(currentStep++) = 112; // .00

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'F';
    *(currentStep++) = 'a';
    *(currentStep++) = 'c';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = 112; // .00

    *(currentStep++) = ITM_RTN;

    // 15
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // OM page 203
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 6; // String length
    *(currentStep++) = 'O';
    *(currentStep++) = 'M';
    *(currentStep++) = 'p';
    *(currentStep++) = '2';
    *(currentStep++) = '0';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_SQUARE;

    *(currentStep++) = ITM_CONSTpi;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RTN;

    // 6
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // OM page 206
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 6; // String length
    *(currentStep++) = 'O';
    *(currentStep++) = 'M';
    *(currentStep++) = 'p';
    *(currentStep++) = '2';
    *(currentStep++) = '0';
    *(currentStep++) = '6';

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'K';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_SQUARE;

    *(currentStep++) = ITM_CONSTpi;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'B';
    *(currentStep++) = 'A';
    *(currentStep++) = 'S';
    *(currentStep++) = 'E';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = STD_SIGMA[0];
    *(currentStep++) = STD_SIGMA[1];
    *(currentStep++) = 'B';

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'B';
    *(currentStep++) = 'A';
    *(currentStep++) = 'S';
    *(currentStep++) = 'E';

    // 10
    *(currentStep++) = ITM_INPUT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'h';

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 6; // String length
    *(currentStep++) = 'V';
    *(currentStep++) = 'O';
    *(currentStep++) = 'L';
    *(currentStep++) = 'U';
    *(currentStep++) = 'M';
    *(currentStep++) = 'E';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = STD_SIGMA[0];
    *(currentStep++) = STD_SIGMA[1];
    *(currentStep++) = 'V';

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 6; // String length
    *(currentStep++) = 'V';
    *(currentStep++) = 'O';
    *(currentStep++) = 'L';
    *(currentStep++) = 'U';
    *(currentStep++) = 'M';
    *(currentStep++) = 'E';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_LONG_INTEGER;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'B';
    *(currentStep++) = 'A';
    *(currentStep++) = 'S';
    *(currentStep++) = 'E';

    // 20
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_LONG_INTEGER;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = STD_SIGMA[0];
    *(currentStep++) = STD_SIGMA[1];
    *(currentStep++) = 'S';

    *(currentStep++) = ITM_RTN;

    // 25
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // OM page 218
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 6; // String length
    *(currentStep++) = 'O';
    *(currentStep++) = 'M';
    *(currentStep++) = 'p';
    *(currentStep++) = '2';
    *(currentStep++) = '1';
    *(currentStep++) = '8';

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'X';

    *(currentStep++) = (ITM_RAN >> 8) | 0x80;
    *(currentStep++) =  ITM_RAN       & 0xff;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 24;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 24;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'X';

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'Y';

    *(currentStep++) = (ITM_LocR >> 8) | 0x80;
    *(currentStep++) =  ITM_LocR       & 0xff;
    *(currentStep++) = 2;

    // 10
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'A' - 'A'; // A

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 24;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = 100; // X

    *(currentStep++) = ITM_STO;
    *(currentStep++) = FIRST_LOCAL_REGISTER + 1;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'C' - 'A'; // C

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER + 1;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = 101; // Y

    // 20
    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'B' - 'A'; // B

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'D' - 'A'; // D

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = FIRST_LOCAL_REGISTER + 1;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'C' - 'A'; // C

    *(currentStep++) = ITM_FS;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'A' - 'A'; // A

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'B' - 'A'; // B

    *(currentStep++) = ITM_SF;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    // 30
    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XexY;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER + 1;

    *(currentStep++) = ITM_XexY;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'D' - 'A'; // D

    // 35
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // OM page 220
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 6; // String length
    *(currentStep++) = 'O';
    *(currentStep++) = 'M';
    *(currentStep++) = 'p';
    *(currentStep++) = '2';
    *(currentStep++) = '2';
    *(currentStep++) = '0';

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'Z';

    *(currentStep++) = (ITM_SIGN >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGN       & 0xff;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'A' - 'A'; // A

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = REGISTER_L;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = REGISTER_L;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_L;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'B' - 'A'; // B

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    // 10
    *(currentStep++) = ITM_XGT;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'C' - 'A'; // C

    *(currentStep++) = ITM_XexY;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = REGISTER_L;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'C' - 'A'; // C

    *(currentStep++) = ITM_Rdown;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'B' - 'A'; // B

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_L;

    // 20
    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_Z;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = REGISTER_L;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'A' - 'A'; // A

    // 23
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // OM page 225
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 6; // String length
    *(currentStep++) = 'O';
    *(currentStep++) = 'M';
    *(currentStep++) = 'p';
    *(currentStep++) = '2';
    *(currentStep++) = '2';
    *(currentStep++) = '5';

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'P';
    *(currentStep++) = 'F';
    *(currentStep++) = 'a';
    *(currentStep++) = 'l';
    *(currentStep++) = 'l';

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '.';
    *(currentStep++) = '5';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = STD_DELTA[0];
    *(currentStep++) = STD_DELTA[1];
    *(currentStep++) = 't';

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 4; // String length
    *(currentStep++) = '.';
    *(currentStep++) = '0';
    *(currentStep++) = '0';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'b';

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 3; // String length
    *(currentStep++) = '1';
    *(currentStep++) = 'e';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'f';

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0';

    // 10
    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'f';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 1;

    *(currentStep++) = (CST_18 >> 8) | 0x80;
    *(currentStep++) =  CST_18       & 0xff;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'b';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'f';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_SQUARE;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_SUB;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 't';

    // 20
    *(currentStep++) = ITM_XGT;
    *(currentStep++) = VALUE_0;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_DROP;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 3;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_DROP;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 3;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = STD_DELTA[0];
    *(currentStep++) = STD_DELTA[1];
    *(currentStep++) = 't';

    // 30
    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'f';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = STD_DELTA[0];
    *(currentStep++) = STD_DELTA[1];
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 't';

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'f';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'f';

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STOP;

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'f';

    *(currentStep++) = ITM_STOP;

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'f';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    // 40
    *(currentStep++) = ITM_STOP;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 1;

    // 42
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // OM page 227
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 6; // String length
    *(currentStep++) = 'O';
    *(currentStep++) = 'M';
    *(currentStep++) = 'p';
    *(currentStep++) = '2';
    *(currentStep++) = '2';
    *(currentStep++) = '7';

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'O';
    *(currentStep++) = 's';
    *(currentStep++) = 'c';
    *(currentStep++) = 'i';

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '.';
    *(currentStep++) = '2';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = STD_DELTA[0];
    *(currentStep++) = STD_DELTA[1];
    *(currentStep++) = 't';

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 2; // String length
    *(currentStep++) = STD_alpha[0];
    *(currentStep++) = STD_alpha[1];

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '.';
    *(currentStep++) = '5';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 2; // String length
    *(currentStep++) = STD_beta[0];
    *(currentStep++) = STD_beta[1];

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    // 10
    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 2; // String length
    *(currentStep++) = STD_gamma[0];
    *(currentStep++) = STD_gamma[1];

    *(currentStep++) = (ITM_RAD >> 8) | 0x80;
    *(currentStep++) =  ITM_RAD       & 0xff;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '.';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 2; // String length
    *(currentStep++) = STD_omega[0];
    *(currentStep++) = STD_omega[1];

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 3; // String length
    *(currentStep++) = '1';
    *(currentStep++) = '.';
    *(currentStep++) = '5';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'f';

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'f';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 1;

    // 20
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 't';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 2; // String length
    *(currentStep++) = STD_omega[0];
    *(currentStep++) = STD_omega[1];

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_sin;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 2; // String length
    *(currentStep++) = STD_gamma[0];
    *(currentStep++) = STD_gamma[1];

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 2; // String length
    *(currentStep++) = STD_beta[0];
    *(currentStep++) = STD_beta[1];

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'f';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_SUB;

    // 30
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 2; // String length
    *(currentStep++) = STD_alpha[0];
    *(currentStep++) = STD_alpha[1];

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'f';

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_SUB;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 't';

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = VALUE_0;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_DROP;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_LONG_INTEGER;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_DIV;

    // 40
    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 3;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_DROP;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 3;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = STD_DELTA[0];
    *(currentStep++) = STD_DELTA[1];
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'f';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = STD_DELTA[0];
    *(currentStep++) = STD_DELTA[1];
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 't';

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'f';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'f';

    // 50
    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STOP;

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'f';

    *(currentStep++) = ITM_STOP;

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'f';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STOP;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 1;

    // 57
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // OM page 229
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 6; // String length
    *(currentStep++) = 'O';
    *(currentStep++) = 'M';
    *(currentStep++) = 'p';
    *(currentStep++) = '2';
    *(currentStep++) = '2';
    *(currentStep++) = '9';

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 6; // String length
    *(currentStep++) = 'S';
    *(currentStep++) = 'a';
    *(currentStep++) = 't';
    *(currentStep++) = 'e';
    *(currentStep++) = 'l';
    *(currentStep++) = 'l';

    *(currentStep++) = ITM_INPUT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'x';

    *(currentStep++) = ITM_INPUT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'y';

    *(currentStep++) = ITM_INPUT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'x';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_INPUT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'y';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '.';
    *(currentStep++) = '1';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = STD_DELTA[0];
    *(currentStep++) = STD_DELTA[1];
    *(currentStep++) = 't';

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_LONG_INTEGER;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1';

    // 10
    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'a';

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_LONG_INTEGER;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 't';

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'y';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'y';

    *(currentStep++) = ITM_SQUARE;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'x';

    *(currentStep++) = ITM_SQUARE;

    *(currentStep++) = ITM_ADD;

    // 20
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 4; // String length
    *(currentStep++) = '-';
    *(currentStep++) = '1';
    *(currentStep++) = '.';
    *(currentStep++) = '5';

    *(currentStep++) = ITM_YX;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'a';

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 108; // L

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'x';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 't';

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = VALUE_0;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_DROP;

    // 30
    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_LONG_INTEGER;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_XexY;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_LONG_INTEGER;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2';

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_XexY;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 3;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_DROP;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 3;

    // 40
    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = STD_DELTA[0];
    *(currentStep++) = STD_DELTA[1];
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'x';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_DROP;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = STD_DELTA[0];
    *(currentStep++) = STD_DELTA[1];
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'y';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = STD_DELTA[0];
    *(currentStep++) = STD_DELTA[1];
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 't';

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'x';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'x';

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'x';

    // 50
    *(currentStep++) = ITM_STOP;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = STD_DELTA[0];
    *(currentStep++) = STD_DELTA[1];
    *(currentStep++) = 't';

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'd';
    *(currentStep++) = 'y';
    *(currentStep++) = '/';
    *(currentStep++) = 'd';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'y';

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 1; // String length
    *(currentStep++) = 'y';

    *(currentStep++) = ITM_STOP;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 1;

    // 57
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }

  { // All OP's
    // 1
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 7; // String length
    *(currentStep++) = 'A';
    *(currentStep++) = 'l';
    *(currentStep++) = 'l';
    *(currentStep++) = 'O';
    *(currentStep++) = 'p';
    *(currentStep++) = '\'';
    *(currentStep++) = 's';

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 'J' - 'A' + 100; // J

    // 10
    *(currentStep++) = ITM_GTO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5;
    *(currentStep++) = 'L';
    *(currentStep++) = 'a';
    *(currentStep++) = 'b';
    *(currentStep++) = 'e';
    *(currentStep++) = 'l';

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 99;

    // 20
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5;
    *(currentStep++) = 'L';
    *(currentStep++) = 'a';
    *(currentStep++) = 'b';
    *(currentStep++) = 'e';
    *(currentStep++) = 'l';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    // 30
    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    // 40
    *(currentStep++) = ITM_ISE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    // 50
    *(currentStep++) = ITM_ISG;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = 0;

    // 60
    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    // 70
    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    // 80
    *(currentStep++) = ITM_DSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = REGISTER_X;

    // 90
    *(currentStep++) = ITM_DSL;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    // 100
    *(currentStep++) = ITM_DSL;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    // 110
    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    // 120
    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = 0;

    // 130
    *(currentStep++) = ITM_XNE;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    // 140
    *(currentStep++) = ITM_XNE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XEQUP0;

    *(currentStep++) = ITM_XEQUM0;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    // 150
    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = 0;

    // 160
    *(currentStep++) = ITM_XLT;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    // 170
    *(currentStep++) = ITM_XLT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    // 180
    *(currentStep++) = ITM_XLE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = REGISTER_X;

    // 190
    *(currentStep++) = ITM_XGE;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    // 200
    *(currentStep++) = ITM_XGE;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    // 210
    *(currentStep++) = ITM_XGT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_FC;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    // 220
    *(currentStep++) = ITM_FC;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_FS;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = 99;

    // 230
    *(currentStep++) = ITM_FS;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    // 240
    *(currentStep++) = ITM_FS;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_EVEN;

    *(currentStep++) = ITM_ODD;

    *(currentStep++) = ITM_FPQ;

    *(currentStep++) = ITM_INT;

    *(currentStep++) = ITM_CPX;

    *(currentStep++) = ITM_MATR;

    *(currentStep++) = ITM_NAN;

    *(currentStep++) = ITM_REAL;

    *(currentStep++) = ITM_SPEC;

    // 250
    *(currentStep++) = ITM_STRI;

    *(currentStep++) = ITM_PMINFINITY;

    *(currentStep++) = ITM_PRIME;

    *(currentStep++) = ITM_TOP;

    *(currentStep++) = ITM_ENTER;

    *(currentStep++) = ITM_XexY;

    *(currentStep++) = ITM_DROP;

    *(currentStep++) = ITM_PAUSE;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_PAUSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_PAUSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_PAUSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_PAUSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_PAUSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_PAUSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_PAUSE;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_Rup;

    *(currentStep++) = ITM_Rdown;

    *(currentStep++) = ITM_CLX;

    *(currentStep++) = ITM_FILL;

    *(currentStep++) = ITM_INPUT;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_INPUT;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_INPUT;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_INPUT;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_INPUT;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_INPUT;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_INPUT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_COMB;

    *(currentStep++) = ITM_PERM;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_CONVG;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_CONVG;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_CONVG;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_CONVG;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_CONVG;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_CONVG;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_CONVG;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_CONVG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_CONVG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_CONVG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_CONVG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_CONVG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_CONVG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_CONVG;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_ENTRY;

    *(currentStep++) = ITM_SQUARE;

    *(currentStep++) = ITM_CUBE;

    *(currentStep++) = ITM_YX;

    *(currentStep++) = ITM_SQUAREROOTX;

    *(currentStep++) = ITM_CUBEROOT;

    *(currentStep++) = ITM_XTHROOT;

    *(currentStep++) = ITM_2X;

    *(currentStep++) = ITM_EXP;

    *(currentStep++) = ITM_ROUND;

    *(currentStep++) = ITM_10x;

    *(currentStep++) = ITM_LOG2;

    *(currentStep++) = ITM_LN;

    *(currentStep++) = ITM_STOP;

    *(currentStep++) = ITM_LOG10;

    *(currentStep++) = ITM_LOGXY;

    *(currentStep++) = ITM_1ONX;

    *(currentStep++) = ITM_cos;

    *(currentStep++) = ITM_cosh;

    *(currentStep++) = ITM_sin;

    *(currentStep++) = ITM_KEYQ;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_KEYQ;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_KEYQ;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_KEYQ;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_KEYQ;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_KEYQ;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_KEYQ;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_KEYQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_KEYQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_KEYQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_KEYQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_KEYQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_KEYQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_KEYQ;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_sinh;

    *(currentStep++) = ITM_tan;

    *(currentStep++) = ITM_tanh;

    *(currentStep++) = ITM_arccos;

    *(currentStep++) = ITM_arcosh;

    *(currentStep++) = ITM_arcsin;

    *(currentStep++) = ITM_arsinh;

    *(currentStep++) = ITM_arctan;

    *(currentStep++) = ITM_artanh;

    *(currentStep++) = ITM_CEIL;

    *(currentStep++) = ITM_FLOOR;

    *(currentStep++) = ITM_GCD;

    *(currentStep++) = ITM_LCM;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_INC;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_INC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_IP;

    *(currentStep++) = ITM_FP;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_SUB;

    *(currentStep++) = ITM_CHS;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_IDIV;

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_VIEW;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_MOD;

    *(currentStep++) = ITM_MAX;

    *(currentStep++) = ITM_MIN;

    *(currentStep++) = ITM_MAGNITUDE;

    *(currentStep++) = ITM_NEIGHB;

    *(currentStep++) = ITM_NEXTP;

    *(currentStep++) = ITM_XFACT;

    *(currentStep++) = ITM_CONSTpi;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_SF;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_FF;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_M_SQR;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = BINARY_SHORT_INTEGER;
    *(currentStep++) = 2; // Base
    *(currentStep++) = 1;
    *(currentStep++) = 2;
    *(currentStep++) = 3;
    *(currentStep++) = 4;
    *(currentStep++) = 5;
    *(currentStep++) = 6;
    *(currentStep++) = 7;
    *(currentStep++) = 0xF8;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = BINARY_SHORT_INTEGER;
    *(currentStep++) = 3; // Base
    *(currentStep++) = 1;
    *(currentStep++) = 2;
    *(currentStep++) = 3;
    *(currentStep++) = 4;
    *(currentStep++) = 5;
    *(currentStep++) = 6;
    *(currentStep++) = 7;
    *(currentStep++) = 0xF8;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = BINARY_SHORT_INTEGER;
    *(currentStep++) = 4; // Base
    *(currentStep++) = 1;
    *(currentStep++) = 2;
    *(currentStep++) = 3;
    *(currentStep++) = 4;
    *(currentStep++) = 5;
    *(currentStep++) = 6;
    *(currentStep++) = 7;
    *(currentStep++) = 0xF8;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = BINARY_SHORT_INTEGER;
    *(currentStep++) = 8; // Base
    *(currentStep++) = 1;
    *(currentStep++) = 2;
    *(currentStep++) = 3;
    *(currentStep++) = 4;
    *(currentStep++) = 5;
    *(currentStep++) = 6;
    *(currentStep++) = 7;
    *(currentStep++) = 0xF8;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = BINARY_SHORT_INTEGER;
    *(currentStep++) = 10; // Base
    *(currentStep++) = 1;
    *(currentStep++) = 2;
    *(currentStep++) = 3;
    *(currentStep++) = 4;
    *(currentStep++) = 5;
    *(currentStep++) = 6;
    *(currentStep++) = 7;
    *(currentStep++) = 0xF8;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = BINARY_SHORT_INTEGER;
    *(currentStep++) = 16; // Base
    *(currentStep++) = 1;
    *(currentStep++) = 2;
    *(currentStep++) = 3;
    *(currentStep++) = 4;
    *(currentStep++) = 5;
    *(currentStep++) = 6;
    *(currentStep++) = 7;
    *(currentStep++) = 0xF8;

    *(currentStep++) = (ITM_NOP >> 8) | 0x80; // TODO: implement ITM_LITERAL BINARY_LONG_INTEGER
    *(currentStep++) =  ITM_NOP       & 0xff;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = BINARY_REAL34;
    *(currentStep++) =  1;
    *(currentStep++) =  2;
    *(currentStep++) =  3;
    *(currentStep++) =  4;
    *(currentStep++) =  5;
    *(currentStep++) =  6;
    *(currentStep++) =  7;
    *(currentStep++) =  8;
    *(currentStep++) =  9;
    *(currentStep++) = 10;
    *(currentStep++) = 11;
    *(currentStep++) = 12;
    *(currentStep++) = 13;
    *(currentStep++) = 14;
    *(currentStep++) = 15;
    *(currentStep++) = 16;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = BINARY_COMPLEX34;
    *(currentStep++) =  1;
    *(currentStep++) =  2;
    *(currentStep++) =  3;
    *(currentStep++) =  4;
    *(currentStep++) =  5;
    *(currentStep++) =  6;
    *(currentStep++) =  7;
    *(currentStep++) =  8;
    *(currentStep++) =  9;
    *(currentStep++) = 10;
    *(currentStep++) = 11;
    *(currentStep++) = 12;
    *(currentStep++) = 13;
    *(currentStep++) = 14;
    *(currentStep++) = 15;
    *(currentStep++) = 16;
    *(currentStep++) = 17;
    *(currentStep++) = 18;
    *(currentStep++) = 19;
    *(currentStep++) = 20;
    *(currentStep++) = 21;
    *(currentStep++) = 22;
    *(currentStep++) = 23;
    *(currentStep++) = 24;
    *(currentStep++) = 25;
    *(currentStep++) = 26;
    *(currentStep++) = 27;
    *(currentStep++) = 28;
    *(currentStep++) = 29;
    *(currentStep++) = 30;
    *(currentStep++) = 31;
    *(currentStep++) = 32;

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_SHORT_INTEGER;
    *(currentStep++) =  16; // Base
    *(currentStep++) =  7;  // String length
    *(currentStep++) =  '-';
    *(currentStep++) =  'A';
    *(currentStep++) =  'B';
    *(currentStep++) =  'C';
    *(currentStep++) =  'D';
    *(currentStep++) =  'E';
    *(currentStep++) =  'F';

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_LONG_INTEGER;
    *(currentStep++) =  6;  // String length
    *(currentStep++) =  '-';
    *(currentStep++) =  '1';
    *(currentStep++) =  '2';
    *(currentStep++) =  '3';
    *(currentStep++) =  '4';
    *(currentStep++) =  '5';

    *(currentStep++) = ITM_LITERAL;
    *(currentStep++) = STRING_COMPLEX34;
    *(currentStep++) = 4; // String length
    *(currentStep++) = '1';
    *(currentStep++) = '-';
    *(currentStep++) = 'i';
    *(currentStep++) = '2';

    *(currentStep++) = ITM_toDEG;

    *(currentStep++) = ITM_toDMS;

    *(currentStep++) = ITM_toGRAD;

    *(currentStep++) = ITM_toMULpi;

    *(currentStep++) = ITM_toRAD;

    *(currentStep++) = ITM_DtoR;

    *(currentStep++) = ITM_RtoD;

    *(currentStep++) = ITM_RMD;

    *(currentStep++) = ITM_LOGICALNOT;

    *(currentStep++) = ITM_LOGICALAND;

    *(currentStep++) = ITM_LOGICALOR;

    *(currentStep++) = ITM_LOGICALXOR;

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_Xex;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (CST_01 >> 8) | 0x80;
    *(currentStep++) =  CST_01       & 0xff;

    *(currentStep++) = (CST_02 >> 8) | 0x80;
    *(currentStep++) =  CST_02       & 0xff;

    *(currentStep++) = (CST_03 >> 8) | 0x80;
    *(currentStep++) =  CST_03       & 0xff;

    *(currentStep++) = (CST_04 >> 8) | 0x80;
    *(currentStep++) =  CST_04       & 0xff;

    *(currentStep++) = (CST_05 >> 8) | 0x80;
    *(currentStep++) =  CST_05       & 0xff;

    *(currentStep++) = (CST_06 >> 8) | 0x80;
    *(currentStep++) =  CST_06       & 0xff;

    *(currentStep++) = (CST_07 >> 8) | 0x80;
    *(currentStep++) =  CST_07       & 0xff;

    *(currentStep++) = (CST_08 >> 8) | 0x80;
    *(currentStep++) =  CST_08       & 0xff;

    *(currentStep++) = (CST_09 >> 8) | 0x80;
    *(currentStep++) =  CST_09       & 0xff;

    *(currentStep++) = (CST_10 >> 8) | 0x80;
    *(currentStep++) =  CST_10       & 0xff;

    *(currentStep++) = (CST_11 >> 8) | 0x80;
    *(currentStep++) =  CST_11       & 0xff;

    *(currentStep++) = (CST_12 >> 8) | 0x80;
    *(currentStep++) =  CST_12       & 0xff;

    *(currentStep++) = (CST_13 >> 8) | 0x80;
    *(currentStep++) =  CST_13       & 0xff;

    *(currentStep++) = (CST_14 >> 8) | 0x80;
    *(currentStep++) =  CST_14       & 0xff;

    *(currentStep++) = (CST_15 >> 8) | 0x80;
    *(currentStep++) =  CST_15       & 0xff;

    *(currentStep++) = (CST_16 >> 8) | 0x80;
    *(currentStep++) =  CST_16       & 0xff;

    *(currentStep++) = (CST_17 >> 8) | 0x80;
    *(currentStep++) =  CST_17       & 0xff;

    *(currentStep++) = (CST_18 >> 8) | 0x80;
    *(currentStep++) =  CST_18       & 0xff;

    *(currentStep++) = (CST_19 >> 8) | 0x80;
    *(currentStep++) =  CST_19       & 0xff;

    *(currentStep++) = (CST_20 >> 8) | 0x80;
    *(currentStep++) =  CST_20       & 0xff;

    *(currentStep++) = (CST_21 >> 8) | 0x80;
    *(currentStep++) =  CST_21       & 0xff;

    *(currentStep++) = (CST_22 >> 8) | 0x80;
    *(currentStep++) =  CST_22       & 0xff;

    *(currentStep++) = (CST_23 >> 8) | 0x80;
    *(currentStep++) =  CST_23       & 0xff;

    *(currentStep++) = (CST_24 >> 8) | 0x80;
    *(currentStep++) =  CST_24       & 0xff;

    *(currentStep++) = (CST_25 >> 8) | 0x80;
    *(currentStep++) =  CST_25       & 0xff;

    *(currentStep++) = (CST_26 >> 8) | 0x80;
    *(currentStep++) =  CST_26       & 0xff;

    *(currentStep++) = (CST_27 >> 8) | 0x80;
    *(currentStep++) =  CST_27       & 0xff;

    *(currentStep++) = (CST_28 >> 8) | 0x80;
    *(currentStep++) =  CST_28       & 0xff;

    *(currentStep++) = (CST_29 >> 8) | 0x80;
    *(currentStep++) =  CST_29       & 0xff;

    *(currentStep++) = (CST_30 >> 8) | 0x80;
    *(currentStep++) =  CST_30       & 0xff;

    *(currentStep++) = (CST_31 >> 8) | 0x80;
    *(currentStep++) =  CST_31       & 0xff;

    *(currentStep++) = (CST_32 >> 8) | 0x80;
    *(currentStep++) =  CST_32       & 0xff;

    *(currentStep++) = (CST_33 >> 8) | 0x80;
    *(currentStep++) =  CST_33       & 0xff;

    *(currentStep++) = (CST_34 >> 8) | 0x80;
    *(currentStep++) =  CST_34       & 0xff;

    *(currentStep++) = (CST_35 >> 8) | 0x80;
    *(currentStep++) =  CST_35       & 0xff;

    *(currentStep++) = (CST_36 >> 8) | 0x80;
    *(currentStep++) =  CST_36       & 0xff;

    *(currentStep++) = (CST_37 >> 8) | 0x80;
    *(currentStep++) =  CST_37       & 0xff;

    *(currentStep++) = (CST_38 >> 8) | 0x80;
    *(currentStep++) =  CST_38       & 0xff;

    *(currentStep++) = (CST_39 >> 8) | 0x80;
    *(currentStep++) =  CST_39       & 0xff;

    *(currentStep++) = (CST_40 >> 8) | 0x80;
    *(currentStep++) =  CST_40       & 0xff;

    *(currentStep++) = (CST_41 >> 8) | 0x80;
    *(currentStep++) =  CST_41       & 0xff;

    *(currentStep++) = (CST_42 >> 8) | 0x80;
    *(currentStep++) =  CST_42       & 0xff;

    *(currentStep++) = (CST_43 >> 8) | 0x80;
    *(currentStep++) =  CST_43       & 0xff;

    *(currentStep++) = (CST_44 >> 8) | 0x80;
    *(currentStep++) =  CST_44       & 0xff;

    *(currentStep++) = (CST_45 >> 8) | 0x80;
    *(currentStep++) =  CST_45       & 0xff;

    *(currentStep++) = (CST_46 >> 8) | 0x80;
    *(currentStep++) =  CST_46       & 0xff;

    *(currentStep++) = (CST_47 >> 8) | 0x80;
    *(currentStep++) =  CST_47       & 0xff;

    *(currentStep++) = (CST_48 >> 8) | 0x80;
    *(currentStep++) =  CST_48       & 0xff;

    *(currentStep++) = (CST_49 >> 8) | 0x80;
    *(currentStep++) =  CST_49       & 0xff;

    *(currentStep++) = (CST_50 >> 8) | 0x80;
    *(currentStep++) =  CST_50       & 0xff;

    *(currentStep++) = (CST_51 >> 8) | 0x80;
    *(currentStep++) =  CST_51       & 0xff;

    *(currentStep++) = (CST_52 >> 8) | 0x80;
    *(currentStep++) =  CST_52       & 0xff;

    *(currentStep++) = (CST_53 >> 8) | 0x80;
    *(currentStep++) =  CST_53       & 0xff;

    *(currentStep++) = (CST_54 >> 8) | 0x80;
    *(currentStep++) =  CST_54       & 0xff;

    *(currentStep++) = (CST_55 >> 8) | 0x80;
    *(currentStep++) =  CST_55       & 0xff;

    *(currentStep++) = (CST_56 >> 8) | 0x80;
    *(currentStep++) =  CST_56       & 0xff;

    *(currentStep++) = (CST_57 >> 8) | 0x80;
    *(currentStep++) =  CST_57       & 0xff;

    *(currentStep++) = (CST_58 >> 8) | 0x80;
    *(currentStep++) =  CST_58       & 0xff;

    *(currentStep++) = (CST_59 >> 8) | 0x80;
    *(currentStep++) =  CST_59       & 0xff;

    *(currentStep++) = (CST_60 >> 8) | 0x80;
    *(currentStep++) =  CST_60       & 0xff;

    *(currentStep++) = (CST_61 >> 8) | 0x80;
    *(currentStep++) =  CST_61       & 0xff;

    *(currentStep++) = (CST_62 >> 8) | 0x80;
    *(currentStep++) =  CST_62       & 0xff;

    *(currentStep++) = (CST_63 >> 8) | 0x80;
    *(currentStep++) =  CST_63       & 0xff;

    *(currentStep++) = (CST_64 >> 8) | 0x80;
    *(currentStep++) =  CST_64       & 0xff;

    *(currentStep++) = (CST_65 >> 8) | 0x80;
    *(currentStep++) =  CST_65       & 0xff;

    *(currentStep++) = (CST_66 >> 8) | 0x80;
    *(currentStep++) =  CST_66       & 0xff;

    *(currentStep++) = (CST_67 >> 8) | 0x80;
    *(currentStep++) =  CST_67       & 0xff;

    *(currentStep++) = (CST_68 >> 8) | 0x80;
    *(currentStep++) =  CST_68       & 0xff;

    *(currentStep++) = (CST_69 >> 8) | 0x80;
    *(currentStep++) =  CST_69       & 0xff;

    *(currentStep++) = (CST_70 >> 8) | 0x80;
    *(currentStep++) =  CST_70       & 0xff;

    *(currentStep++) = (CST_71 >> 8) | 0x80;
    *(currentStep++) =  CST_71       & 0xff;

    *(currentStep++) = (CST_72 >> 8) | 0x80;
    *(currentStep++) =  CST_72       & 0xff;

    *(currentStep++) = (CST_73 >> 8) | 0x80;
    *(currentStep++) =  CST_73       & 0xff;

    *(currentStep++) = (CST_74 >> 8) | 0x80;
    *(currentStep++) =  CST_74       & 0xff;

    *(currentStep++) = (CST_75 >> 8) | 0x80;
    *(currentStep++) =  CST_75       & 0xff;

    *(currentStep++) = (CST_76 >> 8) | 0x80;
    *(currentStep++) =  CST_76       & 0xff;

    *(currentStep++) = (CST_77 >> 8) | 0x80;
    *(currentStep++) =  CST_77       & 0xff;

    *(currentStep++) = (CST_78 >> 8) | 0x80;
    *(currentStep++) =  CST_78       & 0xff;

    *(currentStep++) = (CST_79 >> 8) | 0x80;
    *(currentStep++) =  CST_79       & 0xff;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) =  1;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) =  79;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_CtoF >> 8) | 0x80;
    *(currentStep++) =  ITM_CtoF       & 0xff;

    *(currentStep++) = (ITM_FtoC >> 8) | 0x80;
    *(currentStep++) =  ITM_FtoC       & 0xff;

    *(currentStep++) = (ITM_DBtoPR >> 8) | 0x80;
    *(currentStep++) =  ITM_DBtoPR       & 0xff;

    *(currentStep++) = (ITM_DBtoPRb >> 8) | 0x80;
    *(currentStep++) =  ITM_DBtoPRb       & 0xff;

    *(currentStep++) = (ITM_DBtoPRc >> 8) | 0x80;
    *(currentStep++) =  ITM_DBtoPRc       & 0xff;

    *(currentStep++) = (ITM_DBtoFR >> 8) | 0x80;
    *(currentStep++) =  ITM_DBtoFR       & 0xff;

    *(currentStep++) = (ITM_DBtoFRb >> 8) | 0x80;
    *(currentStep++) =  ITM_DBtoFRb       & 0xff;

    *(currentStep++) = (ITM_DBtoFRc >> 8) | 0x80;
    *(currentStep++) =  ITM_DBtoFRc       & 0xff;

    *(currentStep++) = (ITM_PRtoDB >> 8) | 0x80;
    *(currentStep++) =  ITM_PRtoDB       & 0xff;

    *(currentStep++) = (ITM_PRtoDBb >> 8) | 0x80;
    *(currentStep++) =  ITM_PRtoDBb       & 0xff;

    *(currentStep++) = (ITM_PRtoDBc >> 8) | 0x80;
    *(currentStep++) =  ITM_PRtoDBc       & 0xff;

    *(currentStep++) = (ITM_FRtoDB >> 8) | 0x80;
    *(currentStep++) =  ITM_FRtoDB       & 0xff;

    *(currentStep++) = (ITM_FRtoDBb >> 8) | 0x80;
    *(currentStep++) =  ITM_FRtoDBb       & 0xff;

    *(currentStep++) = (ITM_FRtoDBc >> 8) | 0x80;
    *(currentStep++) =  ITM_FRtoDBc       & 0xff;

    *(currentStep++) = (ITM_ACtoHA >> 8) | 0x80;
    *(currentStep++) =  ITM_ACtoHA       & 0xff;

    *(currentStep++) = (ITM_ACtoHAb >> 8) | 0x80;
    *(currentStep++) =  ITM_ACtoHAb       & 0xff;

    *(currentStep++) = (ITM_HAtoAC >> 8) | 0x80;
    *(currentStep++) =  ITM_HAtoAC       & 0xff;

    *(currentStep++) = (ITM_HAtoACb >> 8) | 0x80;
    *(currentStep++) =  ITM_HAtoACb       & 0xff;

    *(currentStep++) = (ITM_ACUStoHA >> 8) | 0x80;
    *(currentStep++) =  ITM_ACUStoHA       & 0xff;

    *(currentStep++) = (ITM_ACUStoHAb >> 8) | 0x80;
    *(currentStep++) =  ITM_ACUStoHAb       & 0xff;

    *(currentStep++) = (ITM_HAtoACUS >> 8) | 0x80;
    *(currentStep++) =  ITM_HAtoACUS       & 0xff;

    *(currentStep++) = (ITM_HAtoACUSb >> 8) | 0x80;
    *(currentStep++) =  ITM_HAtoACUSb       & 0xff;

    *(currentStep++) = (ITM_PAtoATM >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoATM       & 0xff;

    *(currentStep++) = (ITM_ATMtoPA >> 8) | 0x80;
    *(currentStep++) =  ITM_ATMtoPA       & 0xff;

    *(currentStep++) = (ITM_AUtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_AUtoM       & 0xff;

    *(currentStep++) = (ITM_MtoAU >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoAU       & 0xff;

    *(currentStep++) = (ITM_BARtoPA >> 8) | 0x80;
    *(currentStep++) =  ITM_BARtoPA       & 0xff;

    *(currentStep++) = (ITM_PAtoBAR >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoBAR       & 0xff;

    *(currentStep++) = (ITM_BTUtoJ >> 8) | 0x80;
    *(currentStep++) =  ITM_BTUtoJ       & 0xff;

    *(currentStep++) = (ITM_JtoBTU >> 8) | 0x80;
    *(currentStep++) =  ITM_JtoBTU       & 0xff;

    *(currentStep++) = (ITM_CALtoJ >> 8) | 0x80;
    *(currentStep++) =  ITM_CALtoJ       & 0xff;

    *(currentStep++) = (ITM_JtoCAL >> 8) | 0x80;
    *(currentStep++) =  ITM_JtoCAL       & 0xff;

    *(currentStep++) = (ITM_LBFFTtoNM >> 8) | 0x80;
    *(currentStep++) =  ITM_LBFFTtoNM       & 0xff;

    *(currentStep++) = (ITM_LBFFTtoNMb >> 8) | 0x80;
    *(currentStep++) =  ITM_LBFFTtoNMb       & 0xff;

    *(currentStep++) = (ITM_NMtoLBFFT >> 8) | 0x80;
    *(currentStep++) =  ITM_NMtoLBFFT       & 0xff;

    *(currentStep++) = (ITM_NMtoLBFFTb >> 8) | 0x80;
    *(currentStep++) =  ITM_NMtoLBFFTb       & 0xff;

    *(currentStep++) = (ITM_CWTtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_CWTtoKG       & 0xff;

    *(currentStep++) = (ITM_KGtoCWT >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoCWT       & 0xff;

    *(currentStep++) = (ITM_FTtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_FTtoM       & 0xff;

    *(currentStep++) = (ITM_MtoFT >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFT       & 0xff;

    *(currentStep++) = (ITM_FTUStoM >> 8) | 0x80;
    *(currentStep++) =  ITM_FTUStoM       & 0xff;

    *(currentStep++) = (ITM_FTUStoMb >> 8) | 0x80;
    *(currentStep++) =  ITM_FTUStoMb       & 0xff;

    *(currentStep++) = (ITM_FTUStoMc >> 8) | 0x80;
    *(currentStep++) =  ITM_FTUStoMc       & 0xff;

    *(currentStep++) = (ITM_MtoFTUS >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFTUS       & 0xff;

    *(currentStep++) = (ITM_MtoFTUSb >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFTUSb       & 0xff;

    *(currentStep++) = (ITM_MtoFTUSc >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFTUSc       & 0xff;

    *(currentStep++) = (ITM_FZUKtoML >> 8) | 0x80;
    *(currentStep++) =  ITM_FZUKtoML       & 0xff;

    *(currentStep++) = (ITM_FZUKtoMLb >> 8) | 0x80;
    *(currentStep++) =  ITM_FZUKtoMLb       & 0xff;

    *(currentStep++) = (ITM_MLtoFZUK >> 8) | 0x80;
    *(currentStep++) =  ITM_MLtoFZUK       & 0xff;

    *(currentStep++) = (ITM_MLtoFZUKb >> 8) | 0x80;
    *(currentStep++) =  ITM_MLtoFZUKb       & 0xff;

    *(currentStep++) = (ITM_FZUStoML >> 8) | 0x80;
    *(currentStep++) =  ITM_FZUStoML       & 0xff;

    *(currentStep++) = (ITM_FZUStoMLb >> 8) | 0x80;
    *(currentStep++) =  ITM_FZUStoMLb       & 0xff;

    *(currentStep++) = (ITM_MLtoFZUS >> 8) | 0x80;
    *(currentStep++) =  ITM_MLtoFZUS       & 0xff;

    *(currentStep++) = (ITM_MLtoFZUSb >> 8) | 0x80;
    *(currentStep++) =  ITM_MLtoFZUSb       & 0xff;

    *(currentStep++) = (ITM_GLUKtoL >> 8) | 0x80;
    *(currentStep++) =  ITM_GLUKtoL       & 0xff;

    *(currentStep++) = (ITM_LtoGLUK >> 8) | 0x80;
    *(currentStep++) =  ITM_LtoGLUK       & 0xff;

    *(currentStep++) = (ITM_GLUStoL >> 8) | 0x80;
    *(currentStep++) =  ITM_GLUStoL       & 0xff;

    *(currentStep++) = (ITM_LtoGLUS >> 8) | 0x80;
    *(currentStep++) =  ITM_LtoGLUS       & 0xff;

    *(currentStep++) = (ITM_HPEtoW >> 8) | 0x80;
    *(currentStep++) =  ITM_HPEtoW       & 0xff;

    *(currentStep++) = (ITM_WtoHPE >> 8) | 0x80;
    *(currentStep++) =  ITM_WtoHPE       & 0xff;

    *(currentStep++) = (ITM_HPMtoW >> 8) | 0x80;
    *(currentStep++) =  ITM_HPMtoW       & 0xff;

    *(currentStep++) = (ITM_WtoHPM >> 8) | 0x80;
    *(currentStep++) =  ITM_WtoHPM       & 0xff;

    *(currentStep++) = (ITM_HPUKtoW >> 8) | 0x80;
    *(currentStep++) =  ITM_HPUKtoW       & 0xff;

    *(currentStep++) = (ITM_WtoHPUK >> 8) | 0x80;
    *(currentStep++) =  ITM_WtoHPUK       & 0xff;

    *(currentStep++) = (ITM_INCHHGtoPA >> 8) | 0x80;
    *(currentStep++) =  ITM_INCHHGtoPA       & 0xff;

    *(currentStep++) = (ITM_INCHHGtoPAb >> 8) | 0x80;
    *(currentStep++) =  ITM_INCHHGtoPAb       & 0xff;

    *(currentStep++) = (ITM_PAtoINCHHG >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoINCHHG       & 0xff;

    *(currentStep++) = (ITM_PAtoINCHHGb >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoINCHHGb       & 0xff;

    *(currentStep++) = (ITM_INCHtoMM >> 8) | 0x80;
    *(currentStep++) =  ITM_INCHtoMM       & 0xff;

    *(currentStep++) = (ITM_MMtoINCH >> 8) | 0x80;
    *(currentStep++) =  ITM_MMtoINCH       & 0xff;

    *(currentStep++) = (ITM_WHtoJ >> 8) | 0x80;
    *(currentStep++) =  ITM_WHtoJ       & 0xff;

    *(currentStep++) = (ITM_JtoWH >> 8) | 0x80;
    *(currentStep++) =  ITM_JtoWH       & 0xff;

    *(currentStep++) = (ITM_KGtoLBS >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoLBS       & 0xff;

    *(currentStep++) = (ITM_LBStoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_LBStoKG       & 0xff;

    *(currentStep++) = (ITM_GtoOZ >> 8) | 0x80;
    *(currentStep++) =  ITM_GtoOZ       & 0xff;

    *(currentStep++) = (ITM_OZtoG >> 8) | 0x80;
    *(currentStep++) =  ITM_OZtoG       & 0xff;

    *(currentStep++) = (ITM_KGtoSCW >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoSCW       & 0xff;

    *(currentStep++) = (ITM_KGtoSCWb >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoSCWb       & 0xff;

    *(currentStep++) = (ITM_SCWtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_SCWtoKG       & 0xff;

    *(currentStep++) = (ITM_SCWtoKGb >> 8) | 0x80;
    *(currentStep++) =  ITM_SCWtoKGb       & 0xff;

    *(currentStep++) = (ITM_KGtoSTO >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoSTO       & 0xff;

    *(currentStep++) = (ITM_KGtoSTOb >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoSTOb       & 0xff;

    *(currentStep++) = (ITM_STOtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOtoKG       & 0xff;

    *(currentStep++) = (ITM_STOtoKGb >> 8) | 0x80;
    *(currentStep++) =  ITM_STOtoKGb       & 0xff;

    *(currentStep++) = (ITM_KGtoST >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoST       & 0xff;

    *(currentStep++) = (ITM_KGtoSTb >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoSTb       & 0xff;

    *(currentStep++) = (ITM_KGtoSTc >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoSTc       & 0xff;

    *(currentStep++) = (ITM_STtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_STtoKG       & 0xff;

    *(currentStep++) = (ITM_STtoKGb >> 8) | 0x80;
    *(currentStep++) =  ITM_STtoKGb       & 0xff;

    *(currentStep++) = (ITM_STtoKGc >> 8) | 0x80;
    *(currentStep++) =  ITM_STtoKGc       & 0xff;

    *(currentStep++) = (ITM_KGtoTON >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoTON       & 0xff;

    *(currentStep++) = (ITM_KGtoLIANG >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoLIANG       & 0xff;

    *(currentStep++) = (ITM_KGtoLIANGb >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoLIANGb       & 0xff;

    *(currentStep++) = (ITM_TONtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_TONtoKG       & 0xff;

    *(currentStep++) = (ITM_LIANGtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_LIANGtoKG       & 0xff;

    *(currentStep++) = (ITM_LIANGtoKGb >> 8) | 0x80;
    *(currentStep++) =  ITM_LIANGtoKGb       & 0xff;

    *(currentStep++) = (ITM_GtoTRZ >> 8) | 0x80;
    *(currentStep++) =  ITM_GtoTRZ       & 0xff;

    *(currentStep++) = (ITM_GtoTRZb >> 8) | 0x80;
    *(currentStep++) =  ITM_GtoTRZb       & 0xff;

    *(currentStep++) = (ITM_TRZtoG >> 8) | 0x80;
    *(currentStep++) =  ITM_TRZtoG       & 0xff;

    *(currentStep++) = (ITM_TRZtoGb >> 8) | 0x80;
    *(currentStep++) =  ITM_TRZtoGb       & 0xff;

    *(currentStep++) = (ITM_LBFtoN >> 8) | 0x80;
    *(currentStep++) =  ITM_LBFtoN       & 0xff;

    *(currentStep++) = (ITM_NtoLBF >> 8) | 0x80;
    *(currentStep++) =  ITM_NtoLBF       & 0xff;

    *(currentStep++) = (ITM_LYtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_LYtoM       & 0xff;

    *(currentStep++) = (ITM_MtoLY >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoLY       & 0xff;

    *(currentStep++) = (ITM_MMHGtoPA >> 8) | 0x80;
    *(currentStep++) =  ITM_MMHGtoPA       & 0xff;

    *(currentStep++) = (ITM_MMHGtoPAb >> 8) | 0x80;
    *(currentStep++) =  ITM_MMHGtoPAb       & 0xff;

    *(currentStep++) = (ITM_PAtoMMHG >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoMMHG       & 0xff;

    *(currentStep++) = (ITM_PAtoMMHGb >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoMMHGb       & 0xff;

    *(currentStep++) = (ITM_MItoKM >> 8) | 0x80;
    *(currentStep++) =  ITM_MItoKM       & 0xff;

    *(currentStep++) = (ITM_KMtoMI >> 8) | 0x80;
    *(currentStep++) =  ITM_KMtoMI       & 0xff;

    *(currentStep++) = (ITM_KMtoNMI >> 8) | 0x80;
    *(currentStep++) =  ITM_KMtoNMI       & 0xff;

    *(currentStep++) = (ITM_NMItoKM >> 8) | 0x80;
    *(currentStep++) =  ITM_NMItoKM       & 0xff;

    *(currentStep++) = (ITM_MtoPC >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoPC       & 0xff;

    *(currentStep++) = (ITM_PCtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_PCtoM       & 0xff;

    *(currentStep++) = (ITM_MMtoPOINT >> 8) | 0x80;
    *(currentStep++) =  ITM_MMtoPOINT       & 0xff;

    *(currentStep++) = (ITM_MMtoPOINTb >> 8) | 0x80;
    *(currentStep++) =  ITM_MMtoPOINTb       & 0xff;

    *(currentStep++) = (ITM_MMtoPOINTc >> 8) | 0x80;
    *(currentStep++) =  ITM_MMtoPOINTc       & 0xff;

    *(currentStep++) = (ITM_POINTtoMM >> 8) | 0x80;
    *(currentStep++) =  ITM_POINTtoMM       & 0xff;

    *(currentStep++) = (ITM_POINTtoMMb >> 8) | 0x80;
    *(currentStep++) =  ITM_POINTtoMMb       & 0xff;

    *(currentStep++) = (ITM_POINTtoMMc >> 8) | 0x80;
    *(currentStep++) =  ITM_POINTtoMMc       & 0xff;

    *(currentStep++) = (ITM_MtoYD >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoYD       & 0xff;

    *(currentStep++) = (ITM_YDtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_YDtoM       & 0xff;

    *(currentStep++) = (ITM_PSItoPA >> 8) | 0x80;
    *(currentStep++) =  ITM_PSItoPA       & 0xff;

    *(currentStep++) = (ITM_PAtoPSI >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoPSI       & 0xff;

    *(currentStep++) = (ITM_PAtoTOR >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoTOR       & 0xff;

    *(currentStep++) = (ITM_PAtoTORb >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoTORb       & 0xff;

    *(currentStep++) = (ITM_TORtoPA >> 8) | 0x80;
    *(currentStep++) =  ITM_TORtoPA       & 0xff;

    *(currentStep++) = (ITM_TORtoPAb >> 8) | 0x80;
    *(currentStep++) =  ITM_TORtoPAb       & 0xff;

    *(currentStep++) = (ITM_StoYEAR >> 8) | 0x80;
    *(currentStep++) =  ITM_StoYEAR       & 0xff;

    *(currentStep++) = (ITM_YEARtoS >> 8) | 0x80;
    *(currentStep++) =  ITM_YEARtoS       & 0xff;

    *(currentStep++) = (ITM_CARATtoG >> 8) | 0x80;
    *(currentStep++) =  ITM_CARATtoG       & 0xff;

    *(currentStep++) = (ITM_CARATtoGb >> 8) | 0x80;
    *(currentStep++) =  ITM_CARATtoGb       & 0xff;

    *(currentStep++) = (ITM_JINtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_JINtoKG       & 0xff;

    *(currentStep++) = (ITM_GtoCARAT >> 8) | 0x80;
    *(currentStep++) =  ITM_GtoCARAT       & 0xff;

    *(currentStep++) = (ITM_GtoCARATb >> 8) | 0x80;
    *(currentStep++) =  ITM_GtoCARATb       & 0xff;

    *(currentStep++) = (ITM_KGtoJIN >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoJIN       & 0xff;

    *(currentStep++) = (ITM_QTtoL >> 8) | 0x80;
    *(currentStep++) =  ITM_QTtoL       & 0xff;

    *(currentStep++) = (ITM_LtoQT >> 8) | 0x80;
    *(currentStep++) =  ITM_LtoQT       & 0xff;

    *(currentStep++) = (ITM_FATHOMtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_FATHOMtoM       & 0xff;

    *(currentStep++) = (ITM_FATHOMtoMb >> 8) | 0x80;
    *(currentStep++) =  ITM_FATHOMtoMb       & 0xff;

    *(currentStep++) = (ITM_FATHOMtoMc >> 8) | 0x80;
    *(currentStep++) =  ITM_FATHOMtoMc       & 0xff;

    *(currentStep++) = (ITM_MtoFATHOM >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFATHOM       & 0xff;

    *(currentStep++) = (ITM_MtoFATHOMb >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFATHOMb       & 0xff;

    *(currentStep++) = (ITM_MtoFATHOMc >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFATHOMc       & 0xff;

    *(currentStep++) = (ITM_BARRELtoM3 >> 8) | 0x80;
    *(currentStep++) =  ITM_BARRELtoM3       & 0xff;

    *(currentStep++) = (ITM_BARRELtoM3b >> 8) | 0x80;
    *(currentStep++) =  ITM_BARRELtoM3b       & 0xff;

    *(currentStep++) = (ITM_M3toBARREL >> 8) | 0x80;
    *(currentStep++) =  ITM_M3toBARREL       & 0xff;

    *(currentStep++) = (ITM_M3toBARRELb >> 8) | 0x80;
    *(currentStep++) =  ITM_M3toBARRELb       & 0xff;

    *(currentStep++) = (ITM_ATMtoPAb >> 8) | 0x80;
    *(currentStep++) =  ITM_ATMtoPAb       & 0xff;

    *(currentStep++) = (ITM_PAtoATMb >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoATMb       & 0xff;

    *(currentStep++) = (ITM_HECTAREtoM2 >> 8) | 0x80;
    *(currentStep++) =  ITM_HECTAREtoM2       & 0xff;

    *(currentStep++) = (ITM_M2toHECTARE >> 8) | 0x80;
    *(currentStep++) =  ITM_M2toHECTARE       & 0xff;

    *(currentStep++) = (ITM_MUtoM2 >> 8) | 0x80;
    *(currentStep++) =  ITM_MUtoM2       & 0xff;

    *(currentStep++) = (ITM_M2toMU >> 8) | 0x80;
    *(currentStep++) =  ITM_M2toMU       & 0xff;

    *(currentStep++) = (ITM_LItoM >> 8) | 0x80;
    *(currentStep++) =  ITM_LItoM       & 0xff;

    *(currentStep++) = (ITM_MtoLI >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoLI       & 0xff;

    *(currentStep++) = (ITM_CHItoM >> 8) | 0x80;
    *(currentStep++) =  ITM_CHItoM       & 0xff;

    *(currentStep++) = (ITM_MtoCHI >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoCHI       & 0xff;

    *(currentStep++) = (ITM_YINtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_YINtoM       & 0xff;

    *(currentStep++) = (ITM_MtoYIN >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoYIN       & 0xff;

    *(currentStep++) = (ITM_CUNtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_CUNtoM       & 0xff;

    *(currentStep++) = (ITM_MtoCUN >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoCUN       & 0xff;

    *(currentStep++) = (ITM_ZHANGtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_ZHANGtoM       & 0xff;

    *(currentStep++) = (ITM_ZHANGtoMb >> 8) | 0x80;
    *(currentStep++) =  ITM_ZHANGtoMb       & 0xff;

    *(currentStep++) = (ITM_MtoZHANG >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoZHANG       & 0xff;

    *(currentStep++) = (ITM_MtoZHANGb >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoZHANGb       & 0xff;

    *(currentStep++) = (ITM_FENtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_FENtoM       & 0xff;

    *(currentStep++) = (ITM_MtoFEN >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFEN       & 0xff;

    *(currentStep++) = (ITM_FCC >> 8) | 0x80;
    *(currentStep++) =  ITM_FCC       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FCC >> 8) | 0x80;
    *(currentStep++) =  ITM_FCC       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FCC >> 8) | 0x80;
    *(currentStep++) =  ITM_FCC       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_FCC >> 8) | 0x80;
    *(currentStep++) =  ITM_FCC       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_FCC >> 8) | 0x80;
    *(currentStep++) =  ITM_FCC       & 0xff;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = (ITM_FCC >> 8) | 0x80;
    *(currentStep++) =  ITM_FCC       & 0xff;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = (ITM_FCC >> 8) | 0x80;
    *(currentStep++) =  ITM_FCC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FCC >> 8) | 0x80;
    *(currentStep++) =  ITM_FCC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FCC >> 8) | 0x80;
    *(currentStep++) =  ITM_FCC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_FCC >> 8) | 0x80;
    *(currentStep++) =  ITM_FCC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_FCC >> 8) | 0x80;
    *(currentStep++) =  ITM_FCC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FCC >> 8) | 0x80;
    *(currentStep++) =  ITM_FCC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FCC >> 8) | 0x80;
    *(currentStep++) =  ITM_FCC       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_FCS >> 8) | 0x80;
    *(currentStep++) =  ITM_FCS       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FCS >> 8) | 0x80;
    *(currentStep++) =  ITM_FCS       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FCS >> 8) | 0x80;
    *(currentStep++) =  ITM_FCS       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_FCS >> 8) | 0x80;
    *(currentStep++) =  ITM_FCS       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_FCS >> 8) | 0x80;
    *(currentStep++) =  ITM_FCS       & 0xff;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = (ITM_FCS >> 8) | 0x80;
    *(currentStep++) =  ITM_FCS       & 0xff;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = (ITM_FCS >> 8) | 0x80;
    *(currentStep++) =  ITM_FCS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FCS >> 8) | 0x80;
    *(currentStep++) =  ITM_FCS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FCS >> 8) | 0x80;
    *(currentStep++) =  ITM_FCS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_FCS >> 8) | 0x80;
    *(currentStep++) =  ITM_FCS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_FCS >> 8) | 0x80;
    *(currentStep++) =  ITM_FCS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FCS >> 8) | 0x80;
    *(currentStep++) =  ITM_FCS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FCS >> 8) | 0x80;
    *(currentStep++) =  ITM_FCS       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_FCF >> 8) | 0x80;
    *(currentStep++) =  ITM_FCF       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FCF >> 8) | 0x80;
    *(currentStep++) =  ITM_FCF       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FCF >> 8) | 0x80;
    *(currentStep++) =  ITM_FCF       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_FCF >> 8) | 0x80;
    *(currentStep++) =  ITM_FCF       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_FCF >> 8) | 0x80;
    *(currentStep++) =  ITM_FCF       & 0xff;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = (ITM_FCF >> 8) | 0x80;
    *(currentStep++) =  ITM_FCF       & 0xff;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = (ITM_FCF >> 8) | 0x80;
    *(currentStep++) =  ITM_FCF       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FCF >> 8) | 0x80;
    *(currentStep++) =  ITM_FCF       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FCF >> 8) | 0x80;
    *(currentStep++) =  ITM_FCF       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_FCF >> 8) | 0x80;
    *(currentStep++) =  ITM_FCF       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_FCF >> 8) | 0x80;
    *(currentStep++) =  ITM_FCF       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FCF >> 8) | 0x80;
    *(currentStep++) =  ITM_FCF       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FCF >> 8) | 0x80;
    *(currentStep++) =  ITM_FCF       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_FSC >> 8) | 0x80;
    *(currentStep++) =  ITM_FSC       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FSC >> 8) | 0x80;
    *(currentStep++) =  ITM_FSC       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FSC >> 8) | 0x80;
    *(currentStep++) =  ITM_FSC       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_FSC >> 8) | 0x80;
    *(currentStep++) =  ITM_FSC       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_FSC >> 8) | 0x80;
    *(currentStep++) =  ITM_FSC       & 0xff;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = (ITM_FSC >> 8) | 0x80;
    *(currentStep++) =  ITM_FSC       & 0xff;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = (ITM_FSC >> 8) | 0x80;
    *(currentStep++) =  ITM_FSC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FSC >> 8) | 0x80;
    *(currentStep++) =  ITM_FSC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FSC >> 8) | 0x80;
    *(currentStep++) =  ITM_FSC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_FSC >> 8) | 0x80;
    *(currentStep++) =  ITM_FSC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_FSC >> 8) | 0x80;
    *(currentStep++) =  ITM_FSC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FSC >> 8) | 0x80;
    *(currentStep++) =  ITM_FSC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FSC >> 8) | 0x80;
    *(currentStep++) =  ITM_FSC       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_FSS >> 8) | 0x80;
    *(currentStep++) =  ITM_FSS       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FSS >> 8) | 0x80;
    *(currentStep++) =  ITM_FSS       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FSS >> 8) | 0x80;
    *(currentStep++) =  ITM_FSS       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_FSS >> 8) | 0x80;
    *(currentStep++) =  ITM_FSS       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_FSS >> 8) | 0x80;
    *(currentStep++) =  ITM_FSS       & 0xff;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = (ITM_FSS >> 8) | 0x80;
    *(currentStep++) =  ITM_FSS       & 0xff;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = (ITM_FSS >> 8) | 0x80;
    *(currentStep++) =  ITM_FSS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FSS >> 8) | 0x80;
    *(currentStep++) =  ITM_FSS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FSS >> 8) | 0x80;
    *(currentStep++) =  ITM_FSS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_FSS >> 8) | 0x80;
    *(currentStep++) =  ITM_FSS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_FSS >> 8) | 0x80;
    *(currentStep++) =  ITM_FSS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FSS >> 8) | 0x80;
    *(currentStep++) =  ITM_FSS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FSS >> 8) | 0x80;
    *(currentStep++) =  ITM_FSS       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_FSF >> 8) | 0x80;
    *(currentStep++) =  ITM_FSF       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FSF >> 8) | 0x80;
    *(currentStep++) =  ITM_FSF       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FSF >> 8) | 0x80;
    *(currentStep++) =  ITM_FSF       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_FSF >> 8) | 0x80;
    *(currentStep++) =  ITM_FSF       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_FSF >> 8) | 0x80;
    *(currentStep++) =  ITM_FSF       & 0xff;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = (ITM_FSF >> 8) | 0x80;
    *(currentStep++) =  ITM_FSF       & 0xff;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = (ITM_FSF >> 8) | 0x80;
    *(currentStep++) =  ITM_FSF       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FSF >> 8) | 0x80;
    *(currentStep++) =  ITM_FSF       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FSF >> 8) | 0x80;
    *(currentStep++) =  ITM_FSF       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_FSF >> 8) | 0x80;
    *(currentStep++) =  ITM_FSF       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_FSF >> 8) | 0x80;
    *(currentStep++) =  ITM_FSF       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FSF >> 8) | 0x80;
    *(currentStep++) =  ITM_FSF       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FSF >> 8) | 0x80;
    *(currentStep++) =  ITM_FSF       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_LOGICALNAND >> 8) | 0x80;
    *(currentStep++) =  ITM_LOGICALNAND       & 0xff;

    *(currentStep++) = (ITM_LOGICALNOR >> 8) | 0x80;
    *(currentStep++) =  ITM_LOGICALNOR       & 0xff;

    *(currentStep++) = (ITM_LOGICALXNOR >> 8) | 0x80;
    *(currentStep++) =  ITM_LOGICALXNOR       & 0xff;

    *(currentStep++) = (ITM_BS >> 8) | 0x80;
    *(currentStep++) =  ITM_BS       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_BS >> 8) | 0x80;
    *(currentStep++) =  ITM_BS       & 0xff;
    *(currentStep++) = 63;

    *(currentStep++) = (ITM_BS >> 8) | 0x80;
    *(currentStep++) =  ITM_BS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_BS >> 8) | 0x80;
    *(currentStep++) =  ITM_BS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_BS >> 8) | 0x80;
    *(currentStep++) =  ITM_BS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_BS >> 8) | 0x80;
    *(currentStep++) =  ITM_BS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_BS >> 8) | 0x80;
    *(currentStep++) =  ITM_BS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_BS >> 8) | 0x80;
    *(currentStep++) =  ITM_BS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_BS >> 8) | 0x80;
    *(currentStep++) =  ITM_BS       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_BC >> 8) | 0x80;
    *(currentStep++) =  ITM_BC       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_BC >> 8) | 0x80;
    *(currentStep++) =  ITM_BC       & 0xff;
    *(currentStep++) = 63;

    *(currentStep++) = (ITM_BC >> 8) | 0x80;
    *(currentStep++) =  ITM_BC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_BC >> 8) | 0x80;
    *(currentStep++) =  ITM_BC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_BC >> 8) | 0x80;
    *(currentStep++) =  ITM_BC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_BC >> 8) | 0x80;
    *(currentStep++) =  ITM_BC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_BC >> 8) | 0x80;
    *(currentStep++) =  ITM_BC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_BC >> 8) | 0x80;
    *(currentStep++) =  ITM_BC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_BC >> 8) | 0x80;
    *(currentStep++) =  ITM_BC       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_CB >> 8) | 0x80;
    *(currentStep++) =  ITM_CB       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_CB >> 8) | 0x80;
    *(currentStep++) =  ITM_CB       & 0xff;
    *(currentStep++) = 63;

    *(currentStep++) = (ITM_CB >> 8) | 0x80;
    *(currentStep++) =  ITM_CB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_CB >> 8) | 0x80;
    *(currentStep++) =  ITM_CB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_CB >> 8) | 0x80;
    *(currentStep++) =  ITM_CB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_CB >> 8) | 0x80;
    *(currentStep++) =  ITM_CB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_CB >> 8) | 0x80;
    *(currentStep++) =  ITM_CB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_CB >> 8) | 0x80;
    *(currentStep++) =  ITM_CB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_CB >> 8) | 0x80;
    *(currentStep++) =  ITM_CB       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_SB >> 8) | 0x80;
    *(currentStep++) =  ITM_SB       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SB >> 8) | 0x80;
    *(currentStep++) =  ITM_SB       & 0xff;
    *(currentStep++) = 63;

    *(currentStep++) = (ITM_SB >> 8) | 0x80;
    *(currentStep++) =  ITM_SB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SB >> 8) | 0x80;
    *(currentStep++) =  ITM_SB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_SB >> 8) | 0x80;
    *(currentStep++) =  ITM_SB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_SB >> 8) | 0x80;
    *(currentStep++) =  ITM_SB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_SB >> 8) | 0x80;
    *(currentStep++) =  ITM_SB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SB >> 8) | 0x80;
    *(currentStep++) =  ITM_SB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SB >> 8) | 0x80;
    *(currentStep++) =  ITM_SB       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_FB >> 8) | 0x80;
    *(currentStep++) =  ITM_FB       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FB >> 8) | 0x80;
    *(currentStep++) =  ITM_FB       & 0xff;
    *(currentStep++) = 63;

    *(currentStep++) = (ITM_FB >> 8) | 0x80;
    *(currentStep++) =  ITM_FB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FB >> 8) | 0x80;
    *(currentStep++) =  ITM_FB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FB >> 8) | 0x80;
    *(currentStep++) =  ITM_FB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_FB >> 8) | 0x80;
    *(currentStep++) =  ITM_FB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_FB >> 8) | 0x80;
    *(currentStep++) =  ITM_FB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FB >> 8) | 0x80;
    *(currentStep++) =  ITM_FB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FB >> 8) | 0x80;
    *(currentStep++) =  ITM_FB       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_RL >> 8) | 0x80;
    *(currentStep++) =  ITM_RL       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RL >> 8) | 0x80;
    *(currentStep++) =  ITM_RL       & 0xff;
    *(currentStep++) = 63;

    *(currentStep++) = (ITM_RL >> 8) | 0x80;
    *(currentStep++) =  ITM_RL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RL >> 8) | 0x80;
    *(currentStep++) =  ITM_RL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RL >> 8) | 0x80;
    *(currentStep++) =  ITM_RL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RL >> 8) | 0x80;
    *(currentStep++) =  ITM_RL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RL >> 8) | 0x80;
    *(currentStep++) =  ITM_RL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RL >> 8) | 0x80;
    *(currentStep++) =  ITM_RL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RL >> 8) | 0x80;
    *(currentStep++) =  ITM_RL       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_RLC >> 8) | 0x80;
    *(currentStep++) =  ITM_RLC       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RLC >> 8) | 0x80;
    *(currentStep++) =  ITM_RLC       & 0xff;
    *(currentStep++) = 63;

    *(currentStep++) = (ITM_RLC >> 8) | 0x80;
    *(currentStep++) =  ITM_RLC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RLC >> 8) | 0x80;
    *(currentStep++) =  ITM_RLC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RLC >> 8) | 0x80;
    *(currentStep++) =  ITM_RLC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RLC >> 8) | 0x80;
    *(currentStep++) =  ITM_RLC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RLC >> 8) | 0x80;
    *(currentStep++) =  ITM_RLC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RLC >> 8) | 0x80;
    *(currentStep++) =  ITM_RLC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RLC >> 8) | 0x80;
    *(currentStep++) =  ITM_RLC       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_RR >> 8) | 0x80;
    *(currentStep++) =  ITM_RR       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RR >> 8) | 0x80;
    *(currentStep++) =  ITM_RR       & 0xff;
    *(currentStep++) = 63;

    *(currentStep++) = (ITM_RR >> 8) | 0x80;
    *(currentStep++) =  ITM_RR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RR >> 8) | 0x80;
    *(currentStep++) =  ITM_RR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RR >> 8) | 0x80;
    *(currentStep++) =  ITM_RR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RR >> 8) | 0x80;
    *(currentStep++) =  ITM_RR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RR >> 8) | 0x80;
    *(currentStep++) =  ITM_RR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RR >> 8) | 0x80;
    *(currentStep++) =  ITM_RR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RR >> 8) | 0x80;
    *(currentStep++) =  ITM_RR       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_RRC >> 8) | 0x80;
    *(currentStep++) =  ITM_RRC       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RRC >> 8) | 0x80;
    *(currentStep++) =  ITM_RRC       & 0xff;
    *(currentStep++) = 63;

    *(currentStep++) = (ITM_RRC >> 8) | 0x80;
    *(currentStep++) =  ITM_RRC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RRC >> 8) | 0x80;
    *(currentStep++) =  ITM_RRC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RRC >> 8) | 0x80;
    *(currentStep++) =  ITM_RRC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RRC >> 8) | 0x80;
    *(currentStep++) =  ITM_RRC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RRC >> 8) | 0x80;
    *(currentStep++) =  ITM_RRC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RRC >> 8) | 0x80;
    *(currentStep++) =  ITM_RRC       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RRC >> 8) | 0x80;
    *(currentStep++) =  ITM_RRC       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_SL >> 8) | 0x80;
    *(currentStep++) =  ITM_SL       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SL >> 8) | 0x80;
    *(currentStep++) =  ITM_SL       & 0xff;
    *(currentStep++) = 63;

    *(currentStep++) = (ITM_SL >> 8) | 0x80;
    *(currentStep++) =  ITM_SL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SL >> 8) | 0x80;
    *(currentStep++) =  ITM_SL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_SL >> 8) | 0x80;
    *(currentStep++) =  ITM_SL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_SL >> 8) | 0x80;
    *(currentStep++) =  ITM_SL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_SL >> 8) | 0x80;
    *(currentStep++) =  ITM_SL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SL >> 8) | 0x80;
    *(currentStep++) =  ITM_SL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SL >> 8) | 0x80;
    *(currentStep++) =  ITM_SL       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_SR >> 8) | 0x80;
    *(currentStep++) =  ITM_SR       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SR >> 8) | 0x80;
    *(currentStep++) =  ITM_SR       & 0xff;
    *(currentStep++) = 63;

    *(currentStep++) = (ITM_SR >> 8) | 0x80;
    *(currentStep++) =  ITM_SR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SR >> 8) | 0x80;
    *(currentStep++) =  ITM_SR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_SR >> 8) | 0x80;
    *(currentStep++) =  ITM_SR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_SR >> 8) | 0x80;
    *(currentStep++) =  ITM_SR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_SR >> 8) | 0x80;
    *(currentStep++) =  ITM_SR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SR >> 8) | 0x80;
    *(currentStep++) =  ITM_SR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SR >> 8) | 0x80;
    *(currentStep++) =  ITM_SR       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ASR       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ASR       & 0xff;
    *(currentStep++) = 63;

    *(currentStep++) = (ITM_ASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ASR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ASR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ASR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ASR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ASR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ASR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ASR       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_LJ >> 8) | 0x80;
    *(currentStep++) =  ITM_LJ       & 0xff;

    *(currentStep++) = (ITM_RJ >> 8) | 0x80;
    *(currentStep++) =  ITM_RJ       & 0xff;

    *(currentStep++) = (ITM_MASKL >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKL       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_MASKL >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKL       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_MASKL >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_MASKL >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_MASKL >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_MASKL >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_MASKL >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_MASKL >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_MASKL >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKL       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_MASKR >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKR       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_MASKR >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKR       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_MASKR >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_MASKR >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_MASKR >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_MASKR >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_MASKR >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_MASKR >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_MASKR >> 8) | 0x80;
    *(currentStep++) =  ITM_MASKR       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_MIRROR >> 8) | 0x80;
    *(currentStep++) =  ITM_MIRROR       & 0xff;

    *(currentStep++) = (ITM_NUMB >> 8) | 0x80;
    *(currentStep++) =  ITM_NUMB       & 0xff;

    *(currentStep++) = (ITM_SDL >> 8) | 0x80;
    *(currentStep++) =  ITM_SDL       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SDL >> 8) | 0x80;
    *(currentStep++) =  ITM_SDL       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_SDL >> 8) | 0x80;
    *(currentStep++) =  ITM_SDL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SDL >> 8) | 0x80;
    *(currentStep++) =  ITM_SDL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_SDL >> 8) | 0x80;
    *(currentStep++) =  ITM_SDL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_SDL >> 8) | 0x80;
    *(currentStep++) =  ITM_SDL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_SDL >> 8) | 0x80;
    *(currentStep++) =  ITM_SDL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SDL >> 8) | 0x80;
    *(currentStep++) =  ITM_SDL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SDL >> 8) | 0x80;
    *(currentStep++) =  ITM_SDL       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_SDR >> 8) | 0x80;
    *(currentStep++) =  ITM_SDR       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SDR >> 8) | 0x80;
    *(currentStep++) =  ITM_SDR       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_SDR >> 8) | 0x80;
    *(currentStep++) =  ITM_SDR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SDR >> 8) | 0x80;
    *(currentStep++) =  ITM_SDR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_SDR >> 8) | 0x80;
    *(currentStep++) =  ITM_SDR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_SDR >> 8) | 0x80;
    *(currentStep++) =  ITM_SDR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_SDR >> 8) | 0x80;
    *(currentStep++) =  ITM_SDR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SDR >> 8) | 0x80;
    *(currentStep++) =  ITM_SDR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SDR >> 8) | 0x80;
    *(currentStep++) =  ITM_SDR       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_SIGMAPLUS >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAPLUS       & 0xff;

    *(currentStep++) = (ITM_SIGMAMINUS >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAMINUS       & 0xff;

    *(currentStep++) = (ITM_NSIGMA >> 8) | 0x80;
    *(currentStep++) =  ITM_NSIGMA       & 0xff;

    *(currentStep++) = (ITM_SIGMAx >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAx       & 0xff;

    *(currentStep++) = (ITM_SIGMAy >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAy       & 0xff;

    *(currentStep++) = (ITM_SIGMAx2 >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAx2       & 0xff;

    *(currentStep++) = (ITM_SIGMAx2y >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAx2y       & 0xff;

    *(currentStep++) = (ITM_SIGMAy2 >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAy2       & 0xff;

    *(currentStep++) = (ITM_SIGMAxy >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAxy       & 0xff;

    *(currentStep++) = (ITM_SIGMAlnxy >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAlnxy       & 0xff;

    *(currentStep++) = (ITM_SIGMAlnx >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAlnx       & 0xff;

    *(currentStep++) = (ITM_SIGMAln2x >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAln2x       & 0xff;

    *(currentStep++) = (ITM_SIGMAylnx >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAylnx       & 0xff;

    *(currentStep++) = (ITM_SIGMAlny >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAlny       & 0xff;

    *(currentStep++) = (ITM_SIGMAln2y >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAln2y       & 0xff;

    *(currentStep++) = (ITM_SIGMAxlny >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAxlny       & 0xff;

    *(currentStep++) = (ITM_SIGMAlnyonx >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAlnyonx       & 0xff;

    *(currentStep++) = (ITM_SIGMAx2ony >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAx2ony       & 0xff;

    *(currentStep++) = (ITM_SIGMA1onx >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMA1onx       & 0xff;

    *(currentStep++) = (ITM_SIGMA1onx2 >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMA1onx2       & 0xff;

    *(currentStep++) = (ITM_SIGMAxony >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAxony       & 0xff;

    *(currentStep++) = (ITM_SIGMA1ony >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMA1ony       & 0xff;

    *(currentStep++) = (ITM_SIGMA1ony2 >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMA1ony2       & 0xff;

    *(currentStep++) = (ITM_SIGMAx3 >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAx3       & 0xff;

    *(currentStep++) = (ITM_SIGMAx4 >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAx4       & 0xff;

    *(currentStep++) = (ITM_BINOMP >> 8) | 0x80;
    *(currentStep++) =  ITM_BINOMP       & 0xff;

    *(currentStep++) = (ITM_BINOM >> 8) | 0x80;
    *(currentStep++) =  ITM_BINOM       & 0xff;

    *(currentStep++) = (ITM_BINOMU >> 8) | 0x80;
    *(currentStep++) =  ITM_BINOMU       & 0xff;

    *(currentStep++) = (ITM_BINOMM1 >> 8) | 0x80;
    *(currentStep++) =  ITM_BINOMM1       & 0xff;

    *(currentStep++) = (ITM_CAUCHP >> 8) | 0x80;
    *(currentStep++) =  ITM_CAUCHP       & 0xff;

    *(currentStep++) = (ITM_CAUCH >> 8) | 0x80;
    *(currentStep++) =  ITM_CAUCH       & 0xff;

    *(currentStep++) = (ITM_CAUCHU >> 8) | 0x80;
    *(currentStep++) =  ITM_CAUCHU       & 0xff;

    *(currentStep++) = (ITM_CAUCHM1 >> 8) | 0x80;
    *(currentStep++) =  ITM_CAUCHM1       & 0xff;

    *(currentStep++) = (ITM_EXPONP >> 8) | 0x80;
    *(currentStep++) =  ITM_EXPONP       & 0xff;

    *(currentStep++) = (ITM_EXPON >> 8) | 0x80;
    *(currentStep++) =  ITM_EXPON       & 0xff;

    *(currentStep++) = (ITM_EXPONU >> 8) | 0x80;
    *(currentStep++) =  ITM_EXPONU       & 0xff;

    *(currentStep++) = (ITM_EXPONM1 >> 8) | 0x80;
    *(currentStep++) =  ITM_EXPONM1       & 0xff;

    *(currentStep++) = (ITM_FPX >> 8) | 0x80;
    *(currentStep++) =  ITM_FPX       & 0xff;

    *(currentStep++) = (ITM_FX >> 8) | 0x80;
    *(currentStep++) =  ITM_FX       & 0xff;

    *(currentStep++) = (ITM_FUX >> 8) | 0x80;
    *(currentStep++) =  ITM_FUX       & 0xff;

    *(currentStep++) = (ITM_FM1P >> 8) | 0x80;
    *(currentStep++) =  ITM_FM1P       & 0xff;

    *(currentStep++) = (ITM_GEOMP >> 8) | 0x80;
    *(currentStep++) =  ITM_GEOMP       & 0xff;

    *(currentStep++) = (ITM_GEOM>> 8) | 0x80;
    *(currentStep++) =  ITM_GEOM       & 0xff;

    *(currentStep++) = (ITM_GEOMU >> 8) | 0x80;
    *(currentStep++) =  ITM_GEOMU       & 0xff;

    *(currentStep++) = (ITM_GEOMM1 >> 8) | 0x80;
    *(currentStep++) =  ITM_GEOMM1       & 0xff;

    *(currentStep++) = (ITM_HYPERP >> 8) | 0x80;
    *(currentStep++) =  ITM_HYPERP       & 0xff;

    *(currentStep++) = (ITM_HYPER >> 8) | 0x80;
    *(currentStep++) =  ITM_HYPER       & 0xff;

    *(currentStep++) = (ITM_HYPERU >> 8) | 0x80;
    *(currentStep++) =  ITM_HYPERU       & 0xff;

    *(currentStep++) = (ITM_HYPERM1 >> 8) | 0x80;
    *(currentStep++) =  ITM_HYPERM1       & 0xff;

    *(currentStep++) = (ITM_LGNRMP >> 8) | 0x80;
    *(currentStep++) =  ITM_LGNRMP       & 0xff;

    *(currentStep++) = (ITM_LGNRM>> 8) | 0x80;
    *(currentStep++) =  ITM_LGNRM       & 0xff;

    *(currentStep++) = (ITM_LGNRMU >> 8) | 0x80;
    *(currentStep++) =  ITM_LGNRMU       & 0xff;

    *(currentStep++) = (ITM_LGNRMM1 >> 8) | 0x80;
    *(currentStep++) =  ITM_LGNRMM1       & 0xff;

    *(currentStep++) = (ITM_LOGISP >> 8) | 0x80;
    *(currentStep++) =  ITM_LOGISP       & 0xff;

    *(currentStep++) = (ITM_LOGIS>> 8) | 0x80;
    *(currentStep++) =  ITM_LOGIS       & 0xff;

    *(currentStep++) = (ITM_LOGISU >> 8) | 0x80;
    *(currentStep++) =  ITM_LOGISU       & 0xff;

    *(currentStep++) = (ITM_LOGISM1 >> 8) | 0x80;
    *(currentStep++) =  ITM_LOGISM1       & 0xff;

    *(currentStep++) = (ITM_NBINP >> 8) | 0x80;
    *(currentStep++) =  ITM_NBINP       & 0xff;

    *(currentStep++) = (ITM_NBIN>> 8) | 0x80;
    *(currentStep++) =  ITM_NBIN       & 0xff;

    *(currentStep++) = (ITM_NBINU >> 8) | 0x80;
    *(currentStep++) =  ITM_NBINU       & 0xff;

    *(currentStep++) = (ITM_NBINM1 >> 8) | 0x80;
    *(currentStep++) =  ITM_NBINM1       & 0xff;

    *(currentStep++) = (ITM_NORMLP >> 8) | 0x80;
    *(currentStep++) =  ITM_NORMLP       & 0xff;

    *(currentStep++) = (ITM_NORML>> 8) | 0x80;
    *(currentStep++) =  ITM_NORML       & 0xff;

    *(currentStep++) = (ITM_NORMLU >> 8) | 0x80;
    *(currentStep++) =  ITM_NORMLU       & 0xff;

    *(currentStep++) = (ITM_NORMLM1 >> 8) | 0x80;
    *(currentStep++) =  ITM_NORMLM1       & 0xff;

    *(currentStep++) = (ITM_POISSP >> 8) | 0x80;
    *(currentStep++) =  ITM_POISSP       & 0xff;

    *(currentStep++) = (ITM_POISS>> 8) | 0x80;
    *(currentStep++) =  ITM_POISS       & 0xff;

    *(currentStep++) = (ITM_POISSU >> 8) | 0x80;
    *(currentStep++) =  ITM_POISSU       & 0xff;

    *(currentStep++) = (ITM_POISSM1 >> 8) | 0x80;
    *(currentStep++) =  ITM_POISSM1       & 0xff;

    *(currentStep++) = (ITM_TPX >> 8) | 0x80;
    *(currentStep++) =  ITM_TPX       & 0xff;

    *(currentStep++) = (ITM_TX >> 8) | 0x80;
    *(currentStep++) =  ITM_TX       & 0xff;

    *(currentStep++) = (ITM_TUX >> 8) | 0x80;
    *(currentStep++) =  ITM_TUX       & 0xff;

    *(currentStep++) = (ITM_TM1P >> 8) | 0x80;
    *(currentStep++) =  ITM_TM1P       & 0xff;

    *(currentStep++) = (ITM_WEIBLP >> 8) | 0x80;
    *(currentStep++) =  ITM_WEIBLP       & 0xff;

    *(currentStep++) = (ITM_WEIBL>> 8) | 0x80;
    *(currentStep++) =  ITM_WEIBL       & 0xff;

    *(currentStep++) = (ITM_WEIBLU >> 8) | 0x80;
    *(currentStep++) =  ITM_WEIBLU       & 0xff;

    *(currentStep++) = (ITM_WEIBLM1 >> 8) | 0x80;
    *(currentStep++) =  ITM_WEIBLM1       & 0xff;

    *(currentStep++) = (ITM_chi2Px >> 8) | 0x80;
    *(currentStep++) =  ITM_chi2Px       & 0xff;

    *(currentStep++) = (ITM_chi2x>> 8) | 0x80;
    *(currentStep++) =  ITM_chi2x       & 0xff;

    *(currentStep++) = (ITM_chi2ux >> 8) | 0x80;
    *(currentStep++) =  ITM_chi2ux       & 0xff;

    *(currentStep++) = (ITM_chi2M1 >> 8) | 0x80;
    *(currentStep++) =  ITM_chi2M1       & 0xff;

    *(currentStep++) = (ITM_BESTF >> 8) | 0x80;
    *(currentStep++) =  ITM_BESTF       & 0xff;
    *(currentStep++) = 20;
    *(currentStep++) = 01;

    *(currentStep++) = (ITM_EXPF >> 8) | 0x80;
    *(currentStep++) =  ITM_EXPF       & 0xff;

    *(currentStep++) = (ITM_LINF >> 8) | 0x80;
    *(currentStep++) =  ITM_LINF       & 0xff;

    *(currentStep++) = (ITM_LOGF >> 8) | 0x80;
    *(currentStep++) =  ITM_LOGF       & 0xff;

    *(currentStep++) = (ITM_ORTHOF >> 8) | 0x80;
    *(currentStep++) =  ITM_ORTHOF       & 0xff;

    *(currentStep++) = (ITM_POWERF >> 8) | 0x80;
    *(currentStep++) =  ITM_POWERF       & 0xff;

    *(currentStep++) = (ITM_GAUSSF >> 8) | 0x80;
    *(currentStep++) =  ITM_GAUSSF       & 0xff;

    *(currentStep++) = (ITM_CAUCHF >> 8) | 0x80;
    *(currentStep++) =  ITM_CAUCHF       & 0xff;

    *(currentStep++) = (ITM_PARABF >> 8) | 0x80;
    *(currentStep++) =  ITM_PARABF       & 0xff;

    *(currentStep++) = (ITM_HYPF >> 8) | 0x80;
    *(currentStep++) =  ITM_HYPF       & 0xff;

    *(currentStep++) = (ITM_ROOTF >> 8) | 0x80;
    *(currentStep++) =  ITM_ROOTF       & 0xff;

    *(currentStep++) = (ITM_1COMPL >> 8) | 0x80;
    *(currentStep++) =  ITM_1COMPL       & 0xff;

    *(currentStep++) = (ITM_SNAP >> 8) | 0x80;
    *(currentStep++) =  ITM_SNAP       & 0xff;

    *(currentStep++) = (ITM_2COMPL >> 8) | 0x80;
    *(currentStep++) =  ITM_2COMPL       & 0xff;

    *(currentStep++) = (ITM_ABS >> 8) | 0x80;
    *(currentStep++) =  ITM_ABS       & 0xff;

    *(currentStep++) = (ITM_AGM >> 8) | 0x80;
    *(currentStep++) =  ITM_AGM       & 0xff;

    *(currentStep++) = (ITM_AGRAPH >> 8) | 0x80;
    *(currentStep++) =  ITM_AGRAPH       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_AGRAPH >> 8) | 0x80;
    *(currentStep++) =  ITM_AGRAPH       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_AGRAPH >> 8) | 0x80;
    *(currentStep++) =  ITM_AGRAPH       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_AGRAPH >> 8) | 0x80;
    *(currentStep++) =  ITM_AGRAPH       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_AGRAPH >> 8) | 0x80;
    *(currentStep++) =  ITM_AGRAPH       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_AGRAPH >> 8) | 0x80;
    *(currentStep++) =  ITM_AGRAPH       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_AGRAPH >> 8) | 0x80;
    *(currentStep++) =  ITM_AGRAPH       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_AGRAPH >> 8) | 0x80;
    *(currentStep++) =  ITM_AGRAPH       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_AGRAPH >> 8) | 0x80;
    *(currentStep++) =  ITM_AGRAPH       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ALL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALL       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALL       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_ALL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALL       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_BACK >> 8) | 0x80;
    *(currentStep++) =  ITM_BACK       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_BACK >> 8) | 0x80;
    *(currentStep++) =  ITM_BACK       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_BACK >> 8) | 0x80;
    *(currentStep++) =  ITM_BACK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_BACK >> 8) | 0x80;
    *(currentStep++) =  ITM_BACK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_BACK >> 8) | 0x80;
    *(currentStep++) =  ITM_BACK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_BACK >> 8) | 0x80;
    *(currentStep++) =  ITM_BACK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_BACK >> 8) | 0x80;
    *(currentStep++) =  ITM_BACK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_BACK >> 8) | 0x80;
    *(currentStep++) =  ITM_BACK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_BACK >> 8) | 0x80;
    *(currentStep++) =  ITM_BACK       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_BATT >> 8) | 0x80;
    *(currentStep++) =  ITM_BATT       & 0xff;

    *(currentStep++) = (ITM_BEEP >> 8) | 0x80;
    *(currentStep++) =  ITM_BEEP       & 0xff;

    *(currentStep++) = (ITM_BEGINP >> 8) | 0x80;
    *(currentStep++) =  ITM_BEGINP       & 0xff;

    *(currentStep++) = (ITM_BN >> 8) | 0x80;
    *(currentStep++) =  ITM_BN       & 0xff;

    *(currentStep++) = (ITM_BNS >> 8) | 0x80;
    *(currentStep++) =  ITM_BNS       & 0xff;

    *(currentStep++) = (ITM_CASE >> 8) | 0x80;
    *(currentStep++) =  ITM_CASE       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_CASE >> 8) | 0x80;
    *(currentStep++) =  ITM_CASE       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_CASE >> 8) | 0x80;
    *(currentStep++) =  ITM_CASE       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_CASE >> 8) | 0x80;
    *(currentStep++) =  ITM_CASE       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_CASE >> 8) | 0x80;
    *(currentStep++) =  ITM_CASE       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_CASE >> 8) | 0x80;
    *(currentStep++) =  ITM_CASE       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_CASE >> 8) | 0x80;
    *(currentStep++) =  ITM_CASE       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_CASE >> 8) | 0x80;
    *(currentStep++) =  ITM_CASE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_CASE >> 8) | 0x80;
    *(currentStep++) =  ITM_CASE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_CASE >> 8) | 0x80;
    *(currentStep++) =  ITM_CASE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_CASE >> 8) | 0x80;
    *(currentStep++) =  ITM_CASE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_CASE >> 8) | 0x80;
    *(currentStep++) =  ITM_CASE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_CASE >> 8) | 0x80;
    *(currentStep++) =  ITM_CASE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_CASE >> 8) | 0x80;
    *(currentStep++) =  ITM_CASE       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_CLCVAR >> 8) | 0x80;
    *(currentStep++) =  ITM_CLCVAR       & 0xff;

    *(currentStep++) = (ITM_CLFALL >> 8) | 0x80;
    *(currentStep++) =  ITM_CLFALL       & 0xff;

    *(currentStep++) = (ITM_TGLFRT >> 8) | 0x80;
    *(currentStep++) =  ITM_TGLFRT       & 0xff;

    *(currentStep++) = (ITM_CLLCD >> 8) | 0x80;
    *(currentStep++) =  ITM_CLLCD       & 0xff;

    *(currentStep++) = (ITM_CLMENU >> 8) | 0x80;
    *(currentStep++) =  ITM_CLMENU       & 0xff;

    *(currentStep++) = (ITM_CLREGS >> 8) | 0x80;
    *(currentStep++) =  ITM_CLREGS       & 0xff;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = (ITM_CLSIGMA >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSIGMA       & 0xff;

    *(currentStep++) = (ITM_STOMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMAX       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_STOMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMAX       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_STOMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMAX       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_STOMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMAX       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_STOMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMAX       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMAX       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMAX       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_STOMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMAX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_STOMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMAX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_STOMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMAX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_STOMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMAX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_STOMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMAX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMAX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMAX       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_CONJ >> 8) | 0x80;
    *(currentStep++) =  ITM_CONJ       & 0xff;

    *(currentStep++) = (ITM_RCLMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMAX       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RCLMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMAX       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RCLMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMAX       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RCLMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMAX       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RCLMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMAX       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMAX       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMAX       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_RCLMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMAX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RCLMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMAX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RCLMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMAX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RCLMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMAX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RCLMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMAX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMAX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMAX       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_CORR >> 8) | 0x80;
    *(currentStep++) =  ITM_CORR       & 0xff;

    *(currentStep++) = (ITM_COV >> 8) | 0x80;
    *(currentStep++) =  ITM_COV       & 0xff;

    *(currentStep++) = (ITM_BESTFQ >> 8) | 0x80;
    *(currentStep++) =  ITM_BESTFQ       & 0xff;

    *(currentStep++) = (ITM_CROSS_PROD >> 8) | 0x80;
    *(currentStep++) =  ITM_CROSS_PROD       & 0xff;

    *(currentStep++) = (ITM_CXtoRE >> 8) | 0x80;
    *(currentStep++) =  ITM_CXtoRE       & 0xff;

    *(currentStep++) = (ITM_DATE >> 8) | 0x80;
    *(currentStep++) =  ITM_DATE       & 0xff;

    *(currentStep++) = (ITM_DATEto >> 8) | 0x80;
    *(currentStep++) =  ITM_DATEto       & 0xff;

    *(currentStep++) = (ITM_DAY >> 8) | 0x80;
    *(currentStep++) =  ITM_DAY       & 0xff;

    *(currentStep++) = (ITM_DBLR >> 8) | 0x80;
    *(currentStep++) =  ITM_DBLR       & 0xff;

    *(currentStep++) = (ITM_DBLMULT >> 8) | 0x80;
    *(currentStep++) =  ITM_DBLMULT       & 0xff;

    *(currentStep++) = (ITM_DBLDIV >> 8) | 0x80;
    *(currentStep++) =  ITM_DBLDIV       & 0xff;

    *(currentStep++) = (ITM_DECOMP >> 8) | 0x80;
    *(currentStep++) =  ITM_DECOMP       & 0xff;

    *(currentStep++) = (ITM_DEG >> 8) | 0x80;
    *(currentStep++) =  ITM_DEG       & 0xff;

    *(currentStep++) = (ITM_DEGto >> 8) | 0x80;
    *(currentStep++) =  ITM_DEGto       & 0xff;

    *(currentStep++) = (ITM_SA >> 8) | 0x80;
    *(currentStep++) =  ITM_SA       & 0xff;

    *(currentStep++) = (ITM_DENMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_DENMAX       & 0xff;

    *(currentStep++) = (ITM_DOT_PROD >> 8) | 0x80;
    *(currentStep++) =  ITM_DOT_PROD       & 0xff;

    *(currentStep++) = (ITM_DSTACK >> 8) | 0x80;
    *(currentStep++) =  ITM_DSTACK       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_DSTACK >> 8) | 0x80;
    *(currentStep++) =  ITM_DSTACK       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_DSTACK >> 8) | 0x80;
    *(currentStep++) =  ITM_DSTACK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_DSTACK >> 8) | 0x80;
    *(currentStep++) =  ITM_DSTACK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_DSTACK >> 8) | 0x80;
    *(currentStep++) =  ITM_DSTACK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_DSTACK >> 8) | 0x80;
    *(currentStep++) =  ITM_DSTACK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_DSTACK >> 8) | 0x80;
    *(currentStep++) =  ITM_DSTACK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_DSTACK >> 8) | 0x80;
    *(currentStep++) =  ITM_DSTACK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_DSTACK >> 8) | 0x80;
    *(currentStep++) =  ITM_DSTACK       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_DMS >> 8) | 0x80;
    *(currentStep++) =  ITM_DMS       & 0xff;

    *(currentStep++) = (ITM_DMSto >> 8) | 0x80;
    *(currentStep++) =  ITM_DMSto       & 0xff;

    *(currentStep++) = (ITM_DMY >> 8) | 0x80;
    *(currentStep++) =  ITM_DMY       & 0xff;

    *(currentStep++) = (ITM_DtoJ >> 8) | 0x80;
    *(currentStep++) =  ITM_DtoJ       & 0xff;

    *(currentStep++) = (ITM_DELITM >> 8) | 0x80;
    *(currentStep++) =  ITM_DELITM       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_EIGVAL >> 8) | 0x80;
    *(currentStep++) =  ITM_EIGVAL       & 0xff;

    *(currentStep++) = (ITM_EIGVEC >> 8) | 0x80;
    *(currentStep++) =  ITM_EIGVEC       & 0xff;

    *(currentStep++) = (ITM_ENDP >> 8) | 0x80;
    *(currentStep++) =  ITM_ENDP       & 0xff;

    *(currentStep++) = (ITM_ENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ENG       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ENG       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_ENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ENG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ENG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ENG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ENG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ENG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ENG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ENG       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ENORM >> 8) | 0x80;
    *(currentStep++) =  ITM_ENORM       & 0xff;

    *(currentStep++) = (ITM_RCLMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMIN       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RCLMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMIN       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RCLMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMIN       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RCLMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMIN       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RCLMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMIN       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMIN       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMIN       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_RCLMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMIN       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RCLMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMIN       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RCLMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMIN       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RCLMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMIN       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RCLMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMIN       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMIN       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLMIN       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ERF >> 8) | 0x80;
    *(currentStep++) =  ITM_ERF       & 0xff;

    *(currentStep++) = (ITM_ERFC >> 8) | 0x80;
    *(currentStep++) =  ITM_ERFC       & 0xff;

    *(currentStep++) = (ITM_ERR >> 8) | 0x80;
    *(currentStep++) =  ITM_ERR       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ERR >> 8) | 0x80;
    *(currentStep++) =  ITM_ERR       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_ERR >> 8) | 0x80;
    *(currentStep++) =  ITM_ERR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ERR >> 8) | 0x80;
    *(currentStep++) =  ITM_ERR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ERR >> 8) | 0x80;
    *(currentStep++) =  ITM_ERR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ERR >> 8) | 0x80;
    *(currentStep++) =  ITM_ERR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ERR >> 8) | 0x80;
    *(currentStep++) =  ITM_ERR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ERR >> 8) | 0x80;
    *(currentStep++) =  ITM_ERR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ERR >> 8) | 0x80;
    *(currentStep++) =  ITM_ERR       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_EXITALL >> 8) | 0x80;
    *(currentStep++) =  ITM_EXITALL       & 0xff;

    *(currentStep++) = (ITM_EXPT >> 8) | 0x80;
    *(currentStep++) =  ITM_EXPT       & 0xff;

    *(currentStep++) = (ITM_GET_JUL_GREG >> 8) | 0x80;
    *(currentStep++) =  ITM_GET_JUL_GREG       & 0xff;

    *(currentStep++) = (ITM_FIB >> 8) | 0x80;
    *(currentStep++) =  ITM_FIB       & 0xff;

    *(currentStep++) = (ITM_FIX >> 8) | 0x80;
    *(currentStep++) =  ITM_FIX       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FIX >> 8) | 0x80;
    *(currentStep++) =  ITM_FIX       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_FIX >> 8) | 0x80;
    *(currentStep++) =  ITM_FIX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FIX >> 8) | 0x80;
    *(currentStep++) =  ITM_FIX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FIX >> 8) | 0x80;
    *(currentStep++) =  ITM_FIX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_FIX >> 8) | 0x80;
    *(currentStep++) =  ITM_FIX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_FIX >> 8) | 0x80;
    *(currentStep++) =  ITM_FIX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FIX >> 8) | 0x80;
    *(currentStep++) =  ITM_FIX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_FIX >> 8) | 0x80;
    *(currentStep++) =  ITM_FIX       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_FLASH >> 8) | 0x80;
    *(currentStep++) =  ITM_FLASH       & 0xff;

    *(currentStep++) = (ITM_FQX >> 8) | 0x80;
    *(currentStep++) =  ITM_FQX       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'b';
    *(currentStep++) = 'l';

    *(currentStep++) = (ITM_FQX >> 8) | 0x80;
    *(currentStep++) =  ITM_FQX       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FQX >> 8) | 0x80;
    *(currentStep++) =  ITM_FQX       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FQX >> 8) | 0x80;
    *(currentStep++) =  ITM_FQX       & 0xff;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = (ITM_FQX >> 8) | 0x80;
    *(currentStep++) =  ITM_FQX       & 0xff;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = (ITM_FDQX >> 8) | 0x80;
    *(currentStep++) =  ITM_FDQX       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'b';
    *(currentStep++) = 'l';

    *(currentStep++) = (ITM_FDQX >> 8) | 0x80;
    *(currentStep++) =  ITM_FDQX       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_FDQX >> 8) | 0x80;
    *(currentStep++) =  ITM_FDQX       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_FDQX >> 8) | 0x80;
    *(currentStep++) =  ITM_FDQX       & 0xff;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = (ITM_FDQX >> 8) | 0x80;
    *(currentStep++) =  ITM_FDQX       & 0xff;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = (ITM_GAP >> 8) | 0x80;
    *(currentStep++) =  ITM_GAP       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_GAP >> 8) | 0x80;
    *(currentStep++) =  ITM_GAP       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_GAP >> 8) | 0x80;
    *(currentStep++) =  ITM_GAP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_GAP >> 8) | 0x80;
    *(currentStep++) =  ITM_GAP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_GAP >> 8) | 0x80;
    *(currentStep++) =  ITM_GAP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_GAP >> 8) | 0x80;
    *(currentStep++) =  ITM_GAP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_GAP >> 8) | 0x80;
    *(currentStep++) =  ITM_GAP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_GAP >> 8) | 0x80;
    *(currentStep++) =  ITM_GAP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_GAP >> 8) | 0x80;
    *(currentStep++) =  ITM_GAP       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_GD >> 8) | 0x80;
    *(currentStep++) =  ITM_GD       & 0xff;

    *(currentStep++) = (ITM_GDM1 >> 8) | 0x80;
    *(currentStep++) =  ITM_GDM1       & 0xff;

    *(currentStep++) = (ITM_GRAD >> 8) | 0x80;
    *(currentStep++) =  ITM_GRAD       & 0xff;

    *(currentStep++) = (ITM_GRADto >> 8) | 0x80;
    *(currentStep++) =  ITM_GRADto       & 0xff;

    *(currentStep++) = (ITM_HN >> 8) | 0x80;
    *(currentStep++) =  ITM_HN       & 0xff;

    *(currentStep++) = (ITM_HNP >> 8) | 0x80;
    *(currentStep++) =  ITM_HNP       & 0xff;

    *(currentStep++) = (ITM_IM >> 8) | 0x80;
    *(currentStep++) =  ITM_IM       & 0xff;

    *(currentStep++) = (ITM_INDEX >> 8) | 0x80;
    *(currentStep++) =  ITM_INDEX       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'M';
    *(currentStep++) = 'a';
    *(currentStep++) = 't';
    *(currentStep++) = '_';
    *(currentStep++) = 'A';

    *(currentStep++) = (ITM_INDEX >> 8) | 0x80;
    *(currentStep++) =  ITM_INDEX       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_INDEX >> 8) | 0x80;
    *(currentStep++) =  ITM_INDEX       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_INDEX >> 8) | 0x80;
    *(currentStep++) =  ITM_INDEX       & 0xff;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = (ITM_INDEX >> 8) | 0x80;
    *(currentStep++) =  ITM_INDEX       & 0xff;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = (ITM_IXYZ >> 8) | 0x80;
    *(currentStep++) =  ITM_IXYZ       & 0xff;

    *(currentStep++) = (ITM_IGAMMAP >> 8) | 0x80;
    *(currentStep++) =  ITM_IGAMMAP       & 0xff;

    *(currentStep++) = (ITM_IGAMMAQ >> 8) | 0x80;
    *(currentStep++) =  ITM_IGAMMAQ       & 0xff;

    *(currentStep++) = (ITM_IPLUS >> 8) | 0x80;
    *(currentStep++) =  ITM_IPLUS       & 0xff;

    *(currentStep++) = (ITM_IMINUS >> 8) | 0x80;
    *(currentStep++) =  ITM_IMINUS       & 0xff;

    *(currentStep++) = (ITM_JYX >> 8) | 0x80;
    *(currentStep++) =  ITM_JYX       & 0xff;

    *(currentStep++) = (ITM_JPLUS >> 8) | 0x80;
    *(currentStep++) =  ITM_JPLUS       & 0xff;

    *(currentStep++) = (ITM_JMINUS >> 8) | 0x80;
    *(currentStep++) =  ITM_JMINUS       & 0xff;

    *(currentStep++) = (ITM_JUL_GREG >> 8) | 0x80;
    *(currentStep++) =  ITM_JUL_GREG       & 0xff;

    *(currentStep++) = (ITM_JtoD >> 8) | 0x80;
    *(currentStep++) =  ITM_JtoD       & 0xff;

    *(currentStep++) = (ITM_KEY >> 8) | 0x80;
    *(currentStep++) =  ITM_KEY       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_KEY >> 8) | 0x80;
    *(currentStep++) =  ITM_KEY       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_KEY >> 8) | 0x80;
    *(currentStep++) =  ITM_KEY       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_KEY >> 8) | 0x80;
    *(currentStep++) =  ITM_KEY       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_KEY >> 8) | 0x80;
    *(currentStep++) =  ITM_KEY       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_KEY >> 8) | 0x80;
    *(currentStep++) =  ITM_KEY       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_KEY >> 8) | 0x80;
    *(currentStep++) =  ITM_KEY       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_KEY >> 8) | 0x80;
    *(currentStep++) =  ITM_KEY       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_KEY >> 8) | 0x80;
    *(currentStep++) =  ITM_KEY       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_sinc >> 8) | 0x80;
    *(currentStep++) =  ITM_sinc       & 0xff;

    *(currentStep++) = (ITM_KTYP >> 8) | 0x80;
    *(currentStep++) =  ITM_KTYP       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_KTYP >> 8) | 0x80;
    *(currentStep++) =  ITM_KTYP       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_KTYP >> 8) | 0x80;
    *(currentStep++) =  ITM_KTYP       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_KTYP >> 8) | 0x80;
    *(currentStep++) =  ITM_KTYP       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_KTYP >> 8) | 0x80;
    *(currentStep++) =  ITM_KTYP       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_KTYP >> 8) | 0x80;
    *(currentStep++) =  ITM_KTYP       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_KTYP >> 8) | 0x80;
    *(currentStep++) =  ITM_KTYP       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_KTYP >> 8) | 0x80;
    *(currentStep++) =  ITM_KTYP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_KTYP >> 8) | 0x80;
    *(currentStep++) =  ITM_KTYP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_KTYP >> 8) | 0x80;
    *(currentStep++) =  ITM_KTYP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_KTYP >> 8) | 0x80;
    *(currentStep++) =  ITM_KTYP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_KTYP >> 8) | 0x80;
    *(currentStep++) =  ITM_KTYP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_KTYP >> 8) | 0x80;
    *(currentStep++) =  ITM_KTYP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_KTYP >> 8) | 0x80;
    *(currentStep++) =  ITM_KTYP       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_LASTX >> 8) | 0x80;
    *(currentStep++) =  ITM_LASTX       & 0xff;

    *(currentStep++) = (ITM_LBLQ >> 8) | 0x80;
    *(currentStep++) =  ITM_LBLQ       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'b';
    *(currentStep++) = 'l';

    *(currentStep++) = (ITM_LBLQ >> 8) | 0x80;
    *(currentStep++) =  ITM_LBLQ       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_LBLQ >> 8) | 0x80;
    *(currentStep++) =  ITM_LBLQ       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_LBLQ >> 8) | 0x80;
    *(currentStep++) =  ITM_LBLQ       & 0xff;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = (ITM_LBLQ >> 8) | 0x80;
    *(currentStep++) =  ITM_LBLQ       & 0xff;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = (ITM_LEAP >> 8) | 0x80;
    *(currentStep++) =  ITM_LEAP       & 0xff;

    *(currentStep++) = (ITM_Lm >> 8) | 0x80;
    *(currentStep++) =  ITM_Lm       & 0xff;

    *(currentStep++) = (ITM_LmALPHA >> 8) | 0x80;
    *(currentStep++) =  ITM_LmALPHA       & 0xff;

    *(currentStep++) = (ITM_LNBETA >> 8) | 0x80;
    *(currentStep++) =  ITM_LNBETA       & 0xff;

    *(currentStep++) = (ITM_LNGAMMA >> 8) | 0x80;
    *(currentStep++) =  ITM_LNGAMMA       & 0xff;

    *(currentStep++) = (ITM_LocR >> 8) | 0x80;
    *(currentStep++) =  ITM_LocR       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_LocR >> 8) | 0x80;
    *(currentStep++) =  ITM_LocR       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_LocR >> 8) | 0x80;
    *(currentStep++) =  ITM_LocR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_LocR >> 8) | 0x80;
    *(currentStep++) =  ITM_LocR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_LocR >> 8) | 0x80;
    *(currentStep++) =  ITM_LocR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_LocR >> 8) | 0x80;
    *(currentStep++) =  ITM_LocR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_LocR >> 8) | 0x80;
    *(currentStep++) =  ITM_LocR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_LocR >> 8) | 0x80;
    *(currentStep++) =  ITM_LocR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_LocR >> 8) | 0x80;
    *(currentStep++) =  ITM_LocR       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_LocRQ >> 8) | 0x80;
    *(currentStep++) =  ITM_LocRQ       & 0xff;

    *(currentStep++) = (ITM_LR >> 8) | 0x80;
    *(currentStep++) =  ITM_LR       & 0xff;

    *(currentStep++) = (ITM_MANT >> 8) | 0x80;
    *(currentStep++) =  ITM_MANT       & 0xff;

    *(currentStep++) = (ITM_MEM >> 8) | 0x80;
    *(currentStep++) =  ITM_MEM       & 0xff;

    *(currentStep++) = (ITM_MENU >> 8) | 0x80;
    *(currentStep++) =  ITM_MENU       & 0xff;

    *(currentStep++) = (ITM_MONTH >> 8) | 0x80;
    *(currentStep++) =  ITM_MONTH       & 0xff;

    *(currentStep++) = (ITM_MSG >> 8) | 0x80;
    *(currentStep++) =  ITM_MSG       & 0xff;

    *(currentStep++) = (ITM_MULPI >> 8) | 0x80;
    *(currentStep++) =  ITM_MULPI       & 0xff;

    *(currentStep++) = (ITM_MVAR >> 8) | 0x80;
    *(currentStep++) =  ITM_MVAR       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_M_DIM >> 8) | 0x80;
    *(currentStep++) =  ITM_M_DIM       & 0xff;

    *(currentStep++) = (ITM_M_DIMQ >> 8) | 0x80;
    *(currentStep++) =  ITM_M_DIMQ       & 0xff;

    *(currentStep++) = (ITM_MDY >> 8) | 0x80;
    *(currentStep++) =  ITM_MDY       & 0xff;

    *(currentStep++) = (ITM_M_GET >> 8) | 0x80;
    *(currentStep++) =  ITM_M_GET       & 0xff;

    *(currentStep++) = (ITM_M_LU >> 8) | 0x80;
    *(currentStep++) =  ITM_M_LU       & 0xff;

    *(currentStep++) = (ITM_M_NEW >> 8) | 0x80;
    *(currentStep++) =  ITM_M_NEW       & 0xff;

    *(currentStep++) = (ITM_M_PUT >> 8) | 0x80;
    *(currentStep++) =  ITM_M_PUT       & 0xff;

    *(currentStep++) = (ITM_M_RR >> 8) | 0x80;
    *(currentStep++) =  ITM_M_RR       & 0xff;

    *(currentStep++) = (ITM_sincpi >> 8) | 0x80;
    *(currentStep++) =  ITM_sincpi       & 0xff;

    *(currentStep++) = (ITM_NOP >> 8) | 0x80;
    *(currentStep++) =  ITM_NOP       & 0xff;

    *(currentStep++) = (ITM_OFF >> 8) | 0x80;
    *(currentStep++) =  ITM_OFF       & 0xff;

    *(currentStep++) = (ITM_DROPY >> 8) | 0x80;
    *(currentStep++) =  ITM_DROPY       & 0xff;

    *(currentStep++) = (ITM_STOMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMIN       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_STOMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMIN       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_STOMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMIN       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_STOMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMIN       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_STOMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMIN       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMIN       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMIN       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_STOMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMIN       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_STOMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMIN       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_STOMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMIN       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_STOMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMIN       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_STOMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMIN       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMIN       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_STOMIN       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_PGMINT >> 8) | 0x80;
    *(currentStep++) =  ITM_PGMINT       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'b';
    *(currentStep++) = 'l';

    *(currentStep++) = (ITM_PGMINT >> 8) | 0x80;
    *(currentStep++) =  ITM_PGMINT       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PGMINT >> 8) | 0x80;
    *(currentStep++) =  ITM_PGMINT       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_PGMINT >> 8) | 0x80;
    *(currentStep++) =  ITM_PGMINT       & 0xff;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = (ITM_PGMINT >> 8) | 0x80;
    *(currentStep++) =  ITM_PGMINT       & 0xff;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = (ITM_PGMSLV >> 8) | 0x80;
    *(currentStep++) =  ITM_PGMSLV       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'b';
    *(currentStep++) = 'l';

    *(currentStep++) = (ITM_PGMSLV >> 8) | 0x80;
    *(currentStep++) =  ITM_PGMSLV       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PGMSLV >> 8) | 0x80;
    *(currentStep++) =  ITM_PGMSLV       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_PGMSLV >> 8) | 0x80;
    *(currentStep++) =  ITM_PGMSLV       & 0xff;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = (ITM_PGMSLV >> 8) | 0x80;
    *(currentStep++) =  ITM_PGMSLV       & 0xff;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = (ITM_PIXEL >> 8) | 0x80;
    *(currentStep++) =  ITM_PIXEL       & 0xff;

    *(currentStep++) = (ITM_PLOT >> 8) | 0x80;
    *(currentStep++) =  ITM_PLOT       & 0xff;

    *(currentStep++) = (ITM_Pn >> 8) | 0x80;
    *(currentStep++) =  ITM_Pn       & 0xff;

    *(currentStep++) = (ITM_POINT >> 8) | 0x80;
    *(currentStep++) =  ITM_POINT       & 0xff;

    *(currentStep++) = (ITM_POPLR >> 8) | 0x80;
    *(currentStep++) =  ITM_POPLR       & 0xff;

    *(currentStep++) = (ITM_PRCL >> 8) | 0x80;
    *(currentStep++) =  ITM_PRCL       & 0xff;

    *(currentStep++) = (ITM_PUTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PUTK       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PUTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PUTK       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_PUTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PUTK       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_PUTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PUTK       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_PUTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PUTK       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PUTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PUTK       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PUTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PUTK       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_PUTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PUTK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PUTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PUTK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_PUTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PUTK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_PUTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PUTK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_PUTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PUTK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PUTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PUTK       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PUTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PUTK       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_RAD >> 8) | 0x80;
    *(currentStep++) =  ITM_RAD       & 0xff;

    *(currentStep++) = (ITM_RADto >> 8) | 0x80;
    *(currentStep++) =  ITM_RADto       & 0xff;

    *(currentStep++) = (ITM_RAN >> 8) | 0x80;
    *(currentStep++) =  ITM_RAN       & 0xff;

    *(currentStep++) = (ITM_RCLCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLCFG       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RCLCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLCFG       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RCLCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLCFG       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RCLCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLCFG       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RCLCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLCFG       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLCFG       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLCFG       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_RCLCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLCFG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RCLCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLCFG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RCLCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLCFG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RCLCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLCFG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RCLCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLCFG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLCFG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLCFG       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_RCLEL >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLEL       & 0xff;

    *(currentStep++) = (ITM_RCLIJ >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLIJ       & 0xff;

    *(currentStep++) = (ITM_RCLS >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLS       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RCLS >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLS       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RCLS >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLS       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RCLS >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLS       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RCLS >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLS       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLS >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLS       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLS >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLS       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_RCLS >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RCLS >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RCLS >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RCLS >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RCLS >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLS >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RCLS >> 8) | 0x80;
    *(currentStep++) =  ITM_RCLS       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_RDP >> 8) | 0x80;
    *(currentStep++) =  ITM_RDP       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RDP >> 8) | 0x80;
    *(currentStep++) =  ITM_RDP       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_RDP >> 8) | 0x80;
    *(currentStep++) =  ITM_RDP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RDP >> 8) | 0x80;
    *(currentStep++) =  ITM_RDP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RDP >> 8) | 0x80;
    *(currentStep++) =  ITM_RDP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RDP >> 8) | 0x80;
    *(currentStep++) =  ITM_RDP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RDP >> 8) | 0x80;
    *(currentStep++) =  ITM_RDP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RDP >> 8) | 0x80;
    *(currentStep++) =  ITM_RDP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RDP >> 8) | 0x80;
    *(currentStep++) =  ITM_RDP       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_RE >> 8) | 0x80;
    *(currentStep++) =  ITM_RE       & 0xff;

    *(currentStep++) = (ITM_RECV >> 8) | 0x80;
    *(currentStep++) =  ITM_RECV       & 0xff;

    *(currentStep++) = (ITM_REtoCX >> 8) | 0x80;
    *(currentStep++) =  ITM_REtoCX       & 0xff;

    *(currentStep++) = (ITM_REexIM >> 8) | 0x80;
    *(currentStep++) =  ITM_REexIM       & 0xff;

    *(currentStep++) = (ITM_RM >> 8) | 0x80;
    *(currentStep++) =  ITM_RM       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RM >> 8) | 0x80;
    *(currentStep++) =  ITM_RM       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_RM >> 8) | 0x80;
    *(currentStep++) =  ITM_RM       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RM >> 8) | 0x80;
    *(currentStep++) =  ITM_RM       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RM >> 8) | 0x80;
    *(currentStep++) =  ITM_RM       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RM >> 8) | 0x80;
    *(currentStep++) =  ITM_RM       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RM >> 8) | 0x80;
    *(currentStep++) =  ITM_RM       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RM >> 8) | 0x80;
    *(currentStep++) =  ITM_RM       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RM >> 8) | 0x80;
    *(currentStep++) =  ITM_RM       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_DSP >> 8) | 0x80;
    *(currentStep++) =  ITM_DSP       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_DSP >> 8) | 0x80;
    *(currentStep++) =  ITM_DSP       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_DSP >> 8) | 0x80;
    *(currentStep++) =  ITM_DSP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_DSP >> 8) | 0x80;
    *(currentStep++) =  ITM_DSP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_DSP >> 8) | 0x80;
    *(currentStep++) =  ITM_DSP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_DSP >> 8) | 0x80;
    *(currentStep++) =  ITM_DSP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_DSP >> 8) | 0x80;
    *(currentStep++) =  ITM_DSP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_DSP >> 8) | 0x80;
    *(currentStep++) =  ITM_DSP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_DSP >> 8) | 0x80;
    *(currentStep++) =  ITM_DSP       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_RMQ >> 8) | 0x80;
    *(currentStep++) =  ITM_RMQ       & 0xff;

    *(currentStep++) = (ITM_RNORM >> 8) | 0x80;
    *(currentStep++) =  ITM_RNORM       & 0xff;

    *(currentStep++) = (ITM_EX1 >> 8) | 0x80;
    *(currentStep++) =  ITM_EX1       & 0xff;

    *(currentStep++) = (ITM_ROUNDI >> 8) | 0x80;
    *(currentStep++) =  ITM_ROUNDI       & 0xff;

    *(currentStep++) = (ITM_RSD >> 8) | 0x80;
    *(currentStep++) =  ITM_RSD       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RSD >> 8) | 0x80;
    *(currentStep++) =  ITM_RSD       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_RSD >> 8) | 0x80;
    *(currentStep++) =  ITM_RSD       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_RSD >> 8) | 0x80;
    *(currentStep++) =  ITM_RSD       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_RSD >> 8) | 0x80;
    *(currentStep++) =  ITM_RSD       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_RSD >> 8) | 0x80;
    *(currentStep++) =  ITM_RSD       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_RSD >> 8) | 0x80;
    *(currentStep++) =  ITM_RSD       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RSD >> 8) | 0x80;
    *(currentStep++) =  ITM_RSD       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_RSD >> 8) | 0x80;
    *(currentStep++) =  ITM_RSD       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_RSUM >> 8) | 0x80;
    *(currentStep++) =  ITM_RSUM       & 0xff;

    *(currentStep++) = (ITM_RTNP1 >> 8) | 0x80;
    *(currentStep++) =  ITM_RTNP1       & 0xff;

    *(currentStep++) = (ITM_R_CLR >> 8) | 0x80;
    *(currentStep++) =  ITM_R_CLR       & 0xff;

    *(currentStep++) = (ITM_R_COPY >> 8) | 0x80;
    *(currentStep++) =  ITM_R_COPY       & 0xff;

    *(currentStep++) = (ITM_R_SORT >> 8) | 0x80;
    *(currentStep++) =  ITM_R_SORT       & 0xff;

    *(currentStep++) = (ITM_R_SWAP >> 8) | 0x80;
    *(currentStep++) =  ITM_R_SWAP       & 0xff;

    *(currentStep++) = (ITM_STDDEVWEIGHTED >> 8) | 0x80;
    *(currentStep++) =  ITM_STDDEVWEIGHTED       & 0xff;

    *(currentStep++) = (ITM_SCI >> 8) | 0x80;
    *(currentStep++) =  ITM_SCI       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SCI >> 8) | 0x80;
    *(currentStep++) =  ITM_SCI       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_SCI >> 8) | 0x80;
    *(currentStep++) =  ITM_SCI       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SCI >> 8) | 0x80;
    *(currentStep++) =  ITM_SCI       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_SCI >> 8) | 0x80;
    *(currentStep++) =  ITM_SCI       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_SCI >> 8) | 0x80;
    *(currentStep++) =  ITM_SCI       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_SCI >> 8) | 0x80;
    *(currentStep++) =  ITM_SCI       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SCI >> 8) | 0x80;
    *(currentStep++) =  ITM_SCI       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SDIGS >> 8) | 0x80;
    *(currentStep++) =  ITM_SDIGS       & 0xff;

    *(currentStep++) = (ITM_SEED >> 8) | 0x80;
    *(currentStep++) =  ITM_SEED       & 0xff;

    *(currentStep++) = (ITM_SEND >> 8) | 0x80;
    *(currentStep++) =  ITM_SEND       & 0xff;

    *(currentStep++) = (ITM_SETCHN >> 8) | 0x80;
    *(currentStep++) =  ITM_SETCHN       & 0xff;

    *(currentStep++) = (ITM_SETDAT >> 8) | 0x80;
    *(currentStep++) =  ITM_SETDAT       & 0xff;

    *(currentStep++) = (ITM_SETEUR >> 8) | 0x80;
    *(currentStep++) =  ITM_SETEUR       & 0xff;

    *(currentStep++) = (ITM_SETIND >> 8) | 0x80;
    *(currentStep++) =  ITM_SETIND       & 0xff;

    *(currentStep++) = (ITM_SETJPN >> 8) | 0x80;
    *(currentStep++) =  ITM_SETJPN       & 0xff;

    *(currentStep++) = (ITM_SETSIG >> 8) | 0x80;
    *(currentStep++) =  ITM_SETSIG       & 0xff;

    *(currentStep++) = (ITM_SETTIM >> 8) | 0x80;
    *(currentStep++) =  ITM_SETTIM       & 0xff;

    *(currentStep++) = (ITM_SETUK >> 8) | 0x80;
    *(currentStep++) =  ITM_SETUK       & 0xff;

    *(currentStep++) = (ITM_SETUSA >> 8) | 0x80;
    *(currentStep++) =  ITM_SETUSA       & 0xff;

    *(currentStep++) = (ITM_SIGN >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGN       & 0xff;

    *(currentStep++) = (ITM_SIGNMT >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGNMT       & 0xff;

    *(currentStep++) = (ITM_SIM_EQ >> 8) | 0x80;
    *(currentStep++) =  ITM_SIM_EQ       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SIM_EQ >> 8) | 0x80;
    *(currentStep++) =  ITM_SIM_EQ       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_SIM_EQ >> 8) | 0x80;
    *(currentStep++) =  ITM_SIM_EQ       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SIM_EQ >> 8) | 0x80;
    *(currentStep++) =  ITM_SIM_EQ       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_SIM_EQ >> 8) | 0x80;
    *(currentStep++) =  ITM_SIM_EQ       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_SIM_EQ >> 8) | 0x80;
    *(currentStep++) =  ITM_SIM_EQ       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_SIM_EQ >> 8) | 0x80;
    *(currentStep++) =  ITM_SIM_EQ       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SIM_EQ >> 8) | 0x80;
    *(currentStep++) =  ITM_SIM_EQ       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SKIP >> 8) | 0x80;
    *(currentStep++) =  ITM_SKIP       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SKIP >> 8) | 0x80;
    *(currentStep++) =  ITM_SKIP       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_SKIP >> 8) | 0x80;
    *(currentStep++) =  ITM_SKIP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SKIP >> 8) | 0x80;
    *(currentStep++) =  ITM_SKIP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_SKIP >> 8) | 0x80;
    *(currentStep++) =  ITM_SKIP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_SKIP >> 8) | 0x80;
    *(currentStep++) =  ITM_SKIP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_SKIP >> 8) | 0x80;
    *(currentStep++) =  ITM_SKIP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SKIP >> 8) | 0x80;
    *(currentStep++) =  ITM_SKIP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SLVQ >> 8) | 0x80;
    *(currentStep++) =  ITM_SLVQ       & 0xff;

    *(currentStep++) = (ITM_SM >> 8) | 0x80;
    *(currentStep++) =  ITM_SM       & 0xff;

    *(currentStep++) = (ITM_ISM >> 8) | 0x80;
    *(currentStep++) =  ITM_ISM       & 0xff;

    *(currentStep++) = (ITM_SMW >> 8) | 0x80;
    *(currentStep++) =  ITM_SMW       & 0xff;

    *(currentStep++) = (ITM_SOLVE >> 8) | 0x80;
    *(currentStep++) =  ITM_SOLVE       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'b';
    *(currentStep++) = 'l';

    *(currentStep++) = (ITM_SOLVE >> 8) | 0x80;
    *(currentStep++) =  ITM_SOLVE       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SOLVE >> 8) | 0x80;
    *(currentStep++) =  ITM_SOLVE       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_SOLVE >> 8) | 0x80;
    *(currentStep++) =  ITM_SOLVE       & 0xff;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = (ITM_SOLVE >> 8) | 0x80;
    *(currentStep++) =  ITM_SOLVE       & 0xff;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = (ITM_SSIZE >> 8) | 0x80;
    *(currentStep++) =  ITM_SSIZE       & 0xff;

    *(currentStep++) = (ITM_STOCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOCFG       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_STOCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOCFG       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_STOCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOCFG       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_STOCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOCFG       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_STOCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOCFG       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOCFG       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOCFG       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_STOCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOCFG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_STOCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOCFG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_STOCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOCFG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_STOCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOCFG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_STOCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOCFG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOCFG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOCFG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOCFG       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_STOEL >> 8) | 0x80;
    *(currentStep++) =  ITM_STOEL       & 0xff;

    *(currentStep++) = (ITM_STOIJ >> 8) | 0x80;
    *(currentStep++) =  ITM_STOIJ       & 0xff;

    *(currentStep++) = (ITM_LN1X >> 8) | 0x80;
    *(currentStep++) =  ITM_LN1X       & 0xff;

    *(currentStep++) = (ITM_STOS >> 8) | 0x80;
    *(currentStep++) =  ITM_STOS       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_STOS >> 8) | 0x80;
    *(currentStep++) =  ITM_STOS       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_STOS >> 8) | 0x80;
    *(currentStep++) =  ITM_STOS       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_STOS >> 8) | 0x80;
    *(currentStep++) =  ITM_STOS       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_STOS >> 8) | 0x80;
    *(currentStep++) =  ITM_STOS       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOS >> 8) | 0x80;
    *(currentStep++) =  ITM_STOS       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOS >> 8) | 0x80;
    *(currentStep++) =  ITM_STOS       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_STOS >> 8) | 0x80;
    *(currentStep++) =  ITM_STOS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_STOS >> 8) | 0x80;
    *(currentStep++) =  ITM_STOS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_STOS >> 8) | 0x80;
    *(currentStep++) =  ITM_STOS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_STOS >> 8) | 0x80;
    *(currentStep++) =  ITM_STOS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_STOS >> 8) | 0x80;
    *(currentStep++) =  ITM_STOS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOS >> 8) | 0x80;
    *(currentStep++) =  ITM_STOS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_STOS >> 8) | 0x80;
    *(currentStep++) =  ITM_STOS       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_SUM >> 8) | 0x80;
    *(currentStep++) =  ITM_SUM       & 0xff;

    *(currentStep++) = (ITM_SW >> 8) | 0x80;
    *(currentStep++) =  ITM_SW       & 0xff;

    *(currentStep++) = (ITM_SXY >> 8) | 0x80;
    *(currentStep++) =  ITM_SXY       & 0xff;

    *(currentStep++) = (ITM_TDISP >> 8) | 0x80;
    *(currentStep++) =  ITM_TDISP       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_TDISP >> 8) | 0x80;
    *(currentStep++) =  ITM_TDISP       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_TDISP >> 8) | 0x80;
    *(currentStep++) =  ITM_TDISP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_TDISP >> 8) | 0x80;
    *(currentStep++) =  ITM_TDISP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_TDISP >> 8) | 0x80;
    *(currentStep++) =  ITM_TDISP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_TDISP >> 8) | 0x80;
    *(currentStep++) =  ITM_TDISP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_TDISP >> 8) | 0x80;
    *(currentStep++) =  ITM_TDISP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_TDISP >> 8) | 0x80;
    *(currentStep++) =  ITM_TDISP       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_TICKS >> 8) | 0x80;
    *(currentStep++) =  ITM_TICKS       & 0xff;

    *(currentStep++) = (ITM_TIME >> 8) | 0x80;
    *(currentStep++) =  ITM_TIME       & 0xff;

    *(currentStep++) = (ITM_Tn >> 8) | 0x80;
    *(currentStep++) =  ITM_Tn       & 0xff;

    *(currentStep++) = (ITM_TONE >> 8) | 0x80;
    *(currentStep++) =  ITM_TONE       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_TONE >> 8) | 0x80;
    *(currentStep++) =  ITM_TONE       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_TONE >> 8) | 0x80;
    *(currentStep++) =  ITM_TONE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_TONE >> 8) | 0x80;
    *(currentStep++) =  ITM_TONE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_TONE >> 8) | 0x80;
    *(currentStep++) =  ITM_TONE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_TONE >> 8) | 0x80;
    *(currentStep++) =  ITM_TONE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_TONE >> 8) | 0x80;
    *(currentStep++) =  ITM_TONE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_TONE >> 8) | 0x80;
    *(currentStep++) =  ITM_TONE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_Tex >> 8) | 0x80;
    *(currentStep++) =  ITM_Tex       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_Tex >> 8) | 0x80;
    *(currentStep++) =  ITM_Tex       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_Tex >> 8) | 0x80;
    *(currentStep++) =  ITM_Tex       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_Tex >> 8) | 0x80;
    *(currentStep++) =  ITM_Tex       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_Tex >> 8) | 0x80;
    *(currentStep++) =  ITM_Tex       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_Tex >> 8) | 0x80;
    *(currentStep++) =  ITM_Tex       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_Tex >> 8) | 0x80;
    *(currentStep++) =  ITM_Tex       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_Tex >> 8) | 0x80;
    *(currentStep++) =  ITM_Tex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_Tex >> 8) | 0x80;
    *(currentStep++) =  ITM_Tex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_Tex >> 8) | 0x80;
    *(currentStep++) =  ITM_Tex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_Tex >> 8) | 0x80;
    *(currentStep++) =  ITM_Tex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_Tex >> 8) | 0x80;
    *(currentStep++) =  ITM_Tex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_Tex >> 8) | 0x80;
    *(currentStep++) =  ITM_Tex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_Tex >> 8) | 0x80;
    *(currentStep++) =  ITM_Tex       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ULP >> 8) | 0x80;
    *(currentStep++) =  ITM_ULP       & 0xff;

    *(currentStep++) = (ITM_Un >> 8) | 0x80;
    *(currentStep++) =  ITM_Un       & 0xff;

    *(currentStep++) = (ITM_UNITV >> 8) | 0x80;
    *(currentStep++) =  ITM_UNITV       & 0xff;

    *(currentStep++) = (ITM_UNSIGN >> 8) | 0x80;
    *(currentStep++) =  ITM_UNSIGN       & 0xff;

    *(currentStep++) = (ITM_VARMNU >> 8) | 0x80;
    *(currentStep++) =  ITM_VARMNU       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'b';
    *(currentStep++) = 'l';

    *(currentStep++) = (ITM_VARMNU >> 8) | 0x80;
    *(currentStep++) =  ITM_VARMNU       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_VARMNU >> 8) | 0x80;
    *(currentStep++) =  ITM_VARMNU       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_VARMNU >> 8) | 0x80;
    *(currentStep++) =  ITM_VARMNU       & 0xff;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = (ITM_VARMNU >> 8) | 0x80;
    *(currentStep++) =  ITM_VARMNU       & 0xff;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = (ITM_IDIVR >> 8) | 0x80;
    *(currentStep++) =  ITM_IDIVR       & 0xff;

    *(currentStep++) = (ITM_WDAY >> 8) | 0x80;
    *(currentStep++) =  ITM_WDAY       & 0xff;

    *(currentStep++) = (ITM_WM >> 8) | 0x80;
    *(currentStep++) =  ITM_WM       & 0xff;

    *(currentStep++) = (ITM_WP >> 8) | 0x80;
    *(currentStep++) =  ITM_WP       & 0xff;

    *(currentStep++) = (ITM_WM1 >> 8) | 0x80;
    *(currentStep++) =  ITM_WM1       & 0xff;

    *(currentStep++) = (ITM_WSIZE >> 8) | 0x80;
    *(currentStep++) =  ITM_WSIZE       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_WSIZE >> 8) | 0x80;
    *(currentStep++) =  ITM_WSIZE       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_WSIZE >> 8) | 0x80;
    *(currentStep++) =  ITM_WSIZE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_WSIZE >> 8) | 0x80;
    *(currentStep++) =  ITM_WSIZE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_WSIZE >> 8) | 0x80;
    *(currentStep++) =  ITM_WSIZE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_WSIZE >> 8) | 0x80;
    *(currentStep++) =  ITM_WSIZE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_WSIZE >> 8) | 0x80;
    *(currentStep++) =  ITM_WSIZE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_WSIZE >> 8) | 0x80;
    *(currentStep++) =  ITM_WSIZE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_WSIZEQ >> 8) | 0x80;
    *(currentStep++) =  ITM_WSIZEQ       & 0xff;

    *(currentStep++) = (ITM_XBAR >> 8) | 0x80;
    *(currentStep++) =  ITM_XBAR       & 0xff;

    *(currentStep++) = (ITM_XG >> 8) | 0x80;
    *(currentStep++) =  ITM_XG       & 0xff;

    *(currentStep++) = (ITM_XW >> 8) | 0x80;
    *(currentStep++) =  ITM_XW       & 0xff;

    *(currentStep++) = (ITM_XCIRC >> 8) | 0x80;
    *(currentStep++) =  ITM_XCIRC       & 0xff;

    *(currentStep++) = (ITM_XtoDATE >> 8) | 0x80;
    *(currentStep++) =  ITM_XtoDATE       & 0xff;

    *(currentStep++) = (ITM_XtoALPHA >> 8) | 0x80;
    *(currentStep++) =  ITM_XtoALPHA       & 0xff;

    *(currentStep++) = (ITM_YEAR >> 8) | 0x80;
    *(currentStep++) =  ITM_YEAR       & 0xff;

    *(currentStep++) = (ITM_YCIRC >> 8) | 0x80;
    *(currentStep++) =  ITM_YCIRC       & 0xff;

    *(currentStep++) = (ITM_YMD >> 8) | 0x80;
    *(currentStep++) =  ITM_YMD       & 0xff;

    *(currentStep++) = (ITM_Yex >> 8) | 0x80;
    *(currentStep++) =  ITM_Yex       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_Yex >> 8) | 0x80;
    *(currentStep++) =  ITM_Yex       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_Yex >> 8) | 0x80;
    *(currentStep++) =  ITM_Yex       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_Yex >> 8) | 0x80;
    *(currentStep++) =  ITM_Yex       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_Yex >> 8) | 0x80;
    *(currentStep++) =  ITM_Yex       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_Yex >> 8) | 0x80;
    *(currentStep++) =  ITM_Yex       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_Yex >> 8) | 0x80;
    *(currentStep++) =  ITM_Yex       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_Yex >> 8) | 0x80;
    *(currentStep++) =  ITM_Yex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_Yex >> 8) | 0x80;
    *(currentStep++) =  ITM_Yex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_Yex >> 8) | 0x80;
    *(currentStep++) =  ITM_Yex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_Yex >> 8) | 0x80;
    *(currentStep++) =  ITM_Yex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_Yex >> 8) | 0x80;
    *(currentStep++) =  ITM_Yex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_Yex >> 8) | 0x80;
    *(currentStep++) =  ITM_Yex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_Yex >> 8) | 0x80;
    *(currentStep++) =  ITM_Yex       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_Zex >> 8) | 0x80;
    *(currentStep++) =  ITM_Zex       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_Zex >> 8) | 0x80;
    *(currentStep++) =  ITM_Zex       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_Zex >> 8) | 0x80;
    *(currentStep++) =  ITM_Zex       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_Zex >> 8) | 0x80;
    *(currentStep++) =  ITM_Zex       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_Zex >> 8) | 0x80;
    *(currentStep++) =  ITM_Zex       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_Zex >> 8) | 0x80;
    *(currentStep++) =  ITM_Zex       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_Zex >> 8) | 0x80;
    *(currentStep++) =  ITM_Zex       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_Zex >> 8) | 0x80;
    *(currentStep++) =  ITM_Zex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_Zex >> 8) | 0x80;
    *(currentStep++) =  ITM_Zex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_Zex >> 8) | 0x80;
    *(currentStep++) =  ITM_Zex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_Zex >> 8) | 0x80;
    *(currentStep++) =  ITM_Zex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_Zex >> 8) | 0x80;
    *(currentStep++) =  ITM_Zex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_Zex >> 8) | 0x80;
    *(currentStep++) =  ITM_Zex       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_Zex >> 8) | 0x80;
    *(currentStep++) =  ITM_Zex       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ALPHALENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHALENG       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALPHALENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHALENG       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALPHALENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHALENG       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALPHALENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHALENG       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALPHALENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHALENG       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHALENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHALENG       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHALENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHALENG       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ALPHALENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHALENG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALPHALENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHALENG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALPHALENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHALENG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALPHALENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHALENG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALPHALENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHALENG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHALENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHALENG       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHALENG >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHALENG       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_XMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_XMAX       & 0xff;

    *(currentStep++) = (ITM_XMIN >> 8) | 0x80;
    *(currentStep++) =  ITM_XMIN       & 0xff;

    *(currentStep++) = (ITM_ALPHAPOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAPOS       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALPHAPOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAPOS       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALPHAPOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAPOS       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALPHAPOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAPOS       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALPHAPOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAPOS       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHAPOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAPOS       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHAPOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAPOS       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ALPHAPOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAPOS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALPHAPOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAPOS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALPHAPOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAPOS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALPHAPOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAPOS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALPHAPOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAPOS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHAPOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAPOS       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHAPOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAPOS       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ALPHARL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARL       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALPHARL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARL       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALPHARL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARL       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALPHARL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARL       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALPHARL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARL       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHARL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARL       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHARL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARL       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ALPHARL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALPHARL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALPHARL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALPHARL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALPHARL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHARL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHARL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARL       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ALPHARR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARR       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALPHARR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARR       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALPHARR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARR       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALPHARR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARR       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALPHARR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARR       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHARR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARR       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHARR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARR       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ALPHARR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALPHARR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALPHARR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALPHARR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALPHARR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHARR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHARR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHARR       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ALPHASL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASL       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALPHASL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASL       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALPHASL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASL       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALPHASL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASL       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALPHASL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASL       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHASL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASL       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHASL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASL       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ALPHASL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALPHASL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALPHASL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALPHASL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALPHASL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHASL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASL       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHASL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASL       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ALPHASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASR       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALPHASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASR       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALPHASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASR       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALPHASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASR       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALPHASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASR       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASR       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASR       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ALPHASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALPHASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALPHASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALPHASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALPHASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHASR >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHASR       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ALPHAtoX >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAtoX       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALPHAtoX >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAtoX       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALPHAtoX >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAtoX       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALPHAtoX >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAtoX       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALPHAtoX >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAtoX       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHAtoX >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAtoX       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHAtoX >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAtoX       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_ALPHAtoX >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAtoX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_ALPHAtoX >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAtoX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_ALPHAtoX >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAtoX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_ALPHAtoX >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAtoX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_ALPHAtoX >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAtoX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHAtoX >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAtoX       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_ALPHAtoX >> 8) | 0x80;
    *(currentStep++) =  ITM_ALPHAtoX       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_BETAXY >> 8) | 0x80;
    *(currentStep++) =  ITM_BETAXY       & 0xff;

    *(currentStep++) = (ITM_gammaXY >> 8) | 0x80;
    *(currentStep++) =  ITM_gammaXY       & 0xff;

    *(currentStep++) = (ITM_GAMMAXY >> 8) | 0x80;
    *(currentStep++) =  ITM_GAMMAXY       & 0xff;

    *(currentStep++) = (ITM_GAMMAX >> 8) | 0x80;
    *(currentStep++) =  ITM_GAMMAX       & 0xff;

    *(currentStep++) = (ITM_DELTAPC >> 8) | 0x80;
    *(currentStep++) =  ITM_DELTAPC       & 0xff;

    *(currentStep++) = (ITM_SCATTFACT >> 8) | 0x80;
    *(currentStep++) =  ITM_SCATTFACT       & 0xff;

    *(currentStep++) = (ITM_SCATTFACTm >> 8) | 0x80;
    *(currentStep++) =  ITM_SCATTFACTm       & 0xff;

    *(currentStep++) = (ITM_SCATTFACTp >> 8) | 0x80;
    *(currentStep++) =  ITM_SCATTFACTp       & 0xff;

    *(currentStep++) = (ITM_zetaX >> 8) | 0x80;
    *(currentStep++) =  ITM_zetaX       & 0xff;

    *(currentStep++) = (ITM_PIn >> 8) | 0x80;
    *(currentStep++) =  ITM_PIn       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'b';
    *(currentStep++) = 'l';

    *(currentStep++) = (ITM_PIn >> 8) | 0x80;
    *(currentStep++) =  ITM_PIn       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PIn >> 8) | 0x80;
    *(currentStep++) =  ITM_PIn       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_PIn >> 8) | 0x80;
    *(currentStep++) =  ITM_PIn       & 0xff;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = (ITM_PIn >> 8) | 0x80;
    *(currentStep++) =  ITM_PIn       & 0xff;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = (ITM_SIGMAn >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAn       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'b';
    *(currentStep++) = 'l';

    *(currentStep++) = (ITM_SIGMAn >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAn       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SIGMAn >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAn       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_SIGMAn >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAn       & 0xff;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = (ITM_SIGMAn >> 8) | 0x80;
    *(currentStep++) =  ITM_SIGMAn       & 0xff;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = (ITM_STDDEV >> 8) | 0x80;
    *(currentStep++) =  ITM_STDDEV       & 0xff;

    *(currentStep++) = (ITM_STDDEVPOP >> 8) | 0x80;
    *(currentStep++) =  ITM_STDDEVPOP       & 0xff;

    *(currentStep++) = (ITM_RANI >> 8) | 0x80;
    *(currentStep++) =  ITM_RANI       & 0xff;

    *(currentStep++) = (ITM_PRINTERX >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERX       & 0xff;

    *(currentStep++) = (ITM_RANGE >> 8) | 0x80;
    *(currentStep++) =  ITM_RANGE       & 0xff;

    *(currentStep++) = (ITM_GETRANGE >> 8) | 0x80;
    *(currentStep++) =  ITM_GETRANGE       & 0xff;

    *(currentStep++) = (ITM_M1X >> 8) | 0x80;
    *(currentStep++) =  ITM_M1X       & 0xff;

    *(currentStep++) = (ITM_XMOD >> 8) | 0x80;
    *(currentStep++) =  ITM_XMOD       & 0xff;

    *(currentStep++) = (ITM_sn >> 8) | 0x80;
    *(currentStep++) =  ITM_sn       & 0xff;

    *(currentStep++) = (ITM_cn >> 8) | 0x80;
    *(currentStep++) =  ITM_cn       & 0xff;

    *(currentStep++) = (ITM_dn >> 8) | 0x80;
    *(currentStep++) =  ITM_dn       & 0xff;

    *(currentStep++) = (ITM_toDATE >> 8) | 0x80;
    *(currentStep++) =  ITM_toDATE       & 0xff;

    *(currentStep++) = (ITM_sn >> 8) | 0x80;
    *(currentStep++) =  ITM_sn       & 0xff;

    *(currentStep++) = (ITM_cn >> 8) | 0x80;
    *(currentStep++) =  ITM_cn       & 0xff;

    *(currentStep++) = (ITM_dn >> 8) | 0x80;
    *(currentStep++) =  ITM_dn       & 0xff;

    *(currentStep++) = (ITM_toHR >> 8) | 0x80;
    *(currentStep++) =  ITM_toHR       & 0xff;

    *(currentStep++) = (ITM_toHMS >> 8) | 0x80;
    *(currentStep++) =  ITM_toHMS       & 0xff;

    *(currentStep++) = (ITM_toINT >> 8) | 0x80;
    *(currentStep++) =  ITM_toINT       & 0xff;

    *(currentStep++) = (ITM_toPOL >> 8) | 0x80;
    *(currentStep++) =  ITM_toPOL       & 0xff;

    *(currentStep++) = (ITM_MPItoR >> 8) | 0x80;
    *(currentStep++) =  ITM_MPItoR       & 0xff;

    *(currentStep++) = (ITM_RtoMPI >> 8) | 0x80;
    *(currentStep++) =  ITM_RtoMPI       & 0xff;

    *(currentStep++) = (ITM_toREAL >> 8) | 0x80;
    *(currentStep++) =  ITM_toREAL       & 0xff;

    *(currentStep++) = (ITM_toREC >> 8) | 0x80;
    *(currentStep++) =  ITM_toREC       & 0xff;

    *(currentStep++) = (ITM_DtoDMS >> 8) | 0x80;
    *(currentStep++) =  ITM_DtoDMS       & 0xff;

    *(currentStep++) = (ITM_SHUFFLE >> 8) | 0x80;
    *(currentStep++) =  ITM_SHUFFLE       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SHUFFLE >> 8) | 0x80;
    *(currentStep++) =  ITM_SHUFFLE       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_SHUFFLE >> 8) | 0x80;
    *(currentStep++) =  ITM_SHUFFLE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_SHUFFLE >> 8) | 0x80;
    *(currentStep++) =  ITM_SHUFFLE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_SHUFFLE >> 8) | 0x80;
    *(currentStep++) =  ITM_SHUFFLE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_SHUFFLE >> 8) | 0x80;
    *(currentStep++) =  ITM_SHUFFLE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_SHUFFLE >> 8) | 0x80;
    *(currentStep++) =  ITM_SHUFFLE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_SHUFFLE >> 8) | 0x80;
    *(currentStep++) =  ITM_SHUFFLE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PC >> 8) | 0x80;
    *(currentStep++) =  ITM_PC       & 0xff;

    *(currentStep++) = (ITM_PCMRR >> 8) | 0x80;
    *(currentStep++) =  ITM_PCMRR       & 0xff;

    *(currentStep++) = (ITM_PCT >> 8) | 0x80;
    *(currentStep++) =  ITM_PCT       & 0xff;

    *(currentStep++) = (ITM_PCSIGMA >> 8) | 0x80;
    *(currentStep++) =  ITM_PCSIGMA       & 0xff;

    *(currentStep++) = (ITM_PCPMG >> 8) | 0x80;
    *(currentStep++) =  ITM_PCPMG       & 0xff;

    *(currentStep++) = (ITM_INTEGRAL >> 8) | 0x80;
    *(currentStep++) =  ITM_INTEGRAL       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'b';
    *(currentStep++) = 'l';

    *(currentStep++) = (ITM_INTEGRAL >> 8) | 0x80;
    *(currentStep++) =  ITM_INTEGRAL       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_INTEGRAL >> 8) | 0x80;
    *(currentStep++) =  ITM_INTEGRAL       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_INTEGRAL >> 8) | 0x80;
    *(currentStep++) =  ITM_INTEGRAL       & 0xff;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = (ITM_INTEGRAL >> 8) | 0x80;
    *(currentStep++) =  ITM_INTEGRAL       & 0xff;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = (ITM_PMOD >> 8) | 0x80;
    *(currentStep++) =  ITM_PMOD       & 0xff;

    *(currentStep++) = (ITM_M_DET >> 8) | 0x80;
    *(currentStep++) =  ITM_M_DET       & 0xff;

    *(currentStep++) = (ITM_PARALLEL >> 8) | 0x80;
    *(currentStep++) =  ITM_PARALLEL       & 0xff;

    *(currentStep++) = (ITM_M_TRANSP >> 8) | 0x80;
    *(currentStep++) =  ITM_M_TRANSP       & 0xff;

    *(currentStep++) = (ITM_M_INV >> 8) | 0x80;
    *(currentStep++) =  ITM_M_INV       & 0xff;

    *(currentStep++) = (ITM_ANGLE >> 8) | 0x80;
    *(currentStep++) =  ITM_ANGLE       & 0xff;

    *(currentStep++) = (ITM_MULPIto >> 8) | 0x80;
    *(currentStep++) =  ITM_MULPIto       & 0xff;

    *(currentStep++) = (ITM_PRINTERADV >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERADV       & 0xff;

    *(currentStep++) = (ITM_PRINTERCHAR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERCHAR       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PRINTERCHAR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERCHAR       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_PRINTERCHAR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERCHAR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PRINTERCHAR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERCHAR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_PRINTERCHAR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERCHAR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_PRINTERCHAR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERCHAR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_PRINTERCHAR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERCHAR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PRINTERCHAR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERCHAR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PRINTERDLAY >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERDLAY       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PRINTERDLAY >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERDLAY       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_PRINTERDLAY >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERDLAY       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PRINTERDLAY >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERDLAY       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_PRINTERDLAY >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERDLAY       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_PRINTERDLAY >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERDLAY       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_PRINTERDLAY >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERDLAY       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PRINTERDLAY >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERDLAY       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PRINTERLCD >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERLCD       & 0xff;

    *(currentStep++) = (ITM_PRINTERMODE >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERMODE       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PRINTERMODE >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERMODE       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_PRINTERMODE >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERMODE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PRINTERMODE >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERMODE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_PRINTERMODE >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERMODE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_PRINTERMODE >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERMODE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_PRINTERMODE >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERMODE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PRINTERMODE >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERMODE       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PRINTERPROG >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERPROG       & 0xff;

    *(currentStep++) = (ITM_PRINTERR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERR       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PRINTERR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERR       & 0xff;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_PRINTERR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERR       & 0xff;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_PRINTERR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERR       & 0xff;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_PRINTERR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERR       & 0xff;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PRINTERR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERR       & 0xff;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PRINTERR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERR       & 0xff;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_PRINTERR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PRINTERR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_PRINTERR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_PRINTERR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_PRINTERR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PRINTERR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERR       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PRINTERR >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERR       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = (ITM_PRINTERREGS >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERREGS       & 0xff;

    *(currentStep++) = (ITM_PRINTERSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERSTK       & 0xff;

    *(currentStep++) = (ITM_PRINTERTAB >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERTAB       & 0xff;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PRINTERTAB >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERTAB       & 0xff;
    *(currentStep++) = 64;

    *(currentStep++) = (ITM_PRINTERTAB >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERTAB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_PRINTERTAB >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERTAB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_PRINTERTAB >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERTAB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_PRINTERTAB >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERTAB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_PRINTERTAB >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERTAB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PRINTERTAB >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERTAB       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_PRINTERUSER >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERUSER       & 0xff;

    *(currentStep++) = (ITM_PRINTERWIDTH >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERWIDTH       & 0xff;

    *(currentStep++) = (ITM_PRINTERSIGMA >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERSIGMA       & 0xff;

    *(currentStep++) = (ITM_PRINTERHASH >> 8) | 0x80;
    *(currentStep++) =  ITM_PRINTERHASH       & 0xff;

    *(currentStep++) = (ITM_Kk >> 8) | 0x80;
    *(currentStep++) =  ITM_Kk       & 0xff;

    *(currentStep++) = (ITM_Ek >> 8) | 0x80;
    *(currentStep++) =  ITM_Ek       & 0xff;

    *(currentStep++) = (ITM_PInk >> 8) | 0x80;
    *(currentStep++) =  ITM_PInk       & 0xff;

    *(currentStep++) = (ITM_SHOW >> 8) | 0x80;
    *(currentStep++) =  ITM_SHOW       & 0xff;

    *(currentStep++) = (ITM_DMStoD >> 8) | 0x80;
    *(currentStep++) =  ITM_DMStoD       & 0xff;

    *(currentStep++) = (ITM_VANGLE >> 8) | 0x80;
    *(currentStep++) =  ITM_VANGLE       & 0xff;

    *(currentStep++) = (ITM_XH >> 8) | 0x80;
    *(currentStep++) =  ITM_XH       & 0xff;

    *(currentStep++) = (ITM_XRMS >> 8) | 0x80;
    *(currentStep++) =  ITM_XRMS       & 0xff;

    *(currentStep++) = (ITM_ACOS >> 8) | 0x80;
    *(currentStep++) =  ITM_ACOS       & 0xff;

    *(currentStep++) = (ITM_ASIN >> 8) | 0x80;
    *(currentStep++) =  ITM_ASIN       & 0xff;

    *(currentStep++) = (ITM_ATAN >> 8) | 0x80;
    *(currentStep++) =  ITM_ATAN       & 0xff;

    *(currentStep++) = (ITM_DET >> 8) | 0x80;
    *(currentStep++) =  ITM_DET       & 0xff;

    *(currentStep++) = (ITM_INVRT >> 8) | 0x80;
    *(currentStep++) =  ITM_INVRT       & 0xff;

    *(currentStep++) = (ITM_TRANS >> 8) | 0x80;
    *(currentStep++) =  ITM_TRANS       & 0xff;

    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;
  }


  *(currentStep++) = 255; // .END.
  *(currentStep++) = 255; // .END.

  testPgms = fopen(argv[1], "wb");
  if(testPgms == NULL) {
    fprintf(stderr, "Cannot create file %s\n", argv[1]);
    exit(1);
  }

  sizeOfPgms = currentStep - memory;
  fwrite(&sizeOfPgms, 1, sizeof(sizeOfPgms), testPgms);
  fwrite(memory,      1, sizeOfPgms,         testPgms);
  fclose(testPgms);

  printf("Test programs generated: %s\n", argv[1]);

  return 0;
}
