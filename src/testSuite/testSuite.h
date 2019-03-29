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
 * \file testSuite.h
 ***********************************************/



extern char line[10000];
extern int32_t lineNumber;

typedef struct {
  char     name[25];
  void     (*func)(uint16_t);
} funcTest_t;

void strToShortInteger        (char *nimBuffer, calcRegister_t regist);
char *endOfString             (char *string);
void setParameter             (char *p);
void inParameters             (char *token);
void checkRegisterType        (calcRegister_t regist, char letter, uint32_t expectedDataType);
void checkExpectedOutParameter(char *p);
void outParameters            (char *token);
void functionToCall           (char *functionName);
void processLine              (char *line);
