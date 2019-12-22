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



#define FUNC_NOPARAM  0
#define FUNC_CVT      1

#define RE_ACCURATE   0
#define RE_INACCURATE 1


typedef struct {
  char     name[25];
  void     (*func)(uint16_t);
} funcTest_t;

void strToShortInteger        (char *nimBuffer, calcRegister_t regist);
char *endOfString             (char *string);
char hexToChar                (const char *string);
void setParameter             (char *p);
void inParameters             (char *token);
void checkRegisterType        (calcRegister_t regist, char letter, uint32_t expectedDataType, uint32_t tag);
void checkExpectedOutParameter(char *p);
void outParameters            (char *token);
void functionToCall           (char *functionName);
void abortTest                (void);
void standardizeLine          (void);
void processLine              (void);
void processOneFile           (void);
void processTests             (void);
int  relativeErrorReal34      (real34_t *expectedValue, real34_t *value, char *numberPart, calcRegister_t regist, char letter);
void wrongRegisterValue       (calcRegister_t regist, char letter, char *expectedValue);
void expectedAndShouldBeValue (calcRegister_t regist, char letter, char *expectedValue, char *expectedAndValue);
