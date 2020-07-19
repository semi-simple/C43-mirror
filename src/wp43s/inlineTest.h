/* This file is part of 43C.
 *
 * 43C is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 43C is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 43C.  If not, see <http://www.gnu.org/licenses/>.
 */

/********************************************//**
 * \file inlineTest.h
 ***********************************************/

 
#ifdef INLINE_TEST
extern bool_t   testEnabled;
extern uint16_t testBitset;


#define DR_ITM_TST             32


void     fnSwStart            (uint8_t nr);       // Start StopWatch, 0..3
void     fnSwStop             (uint8_t nr);       // Stop StopWatch, 0..3

void fnSetInlineTest          (uint16_t drConfig);
void fnGetInlineTestBsToX     (uint16_t unusedParamButMandatory);
void fnSetInlineTestXToBs     (uint16_t unusedParamButMandatory);
bool_t fnTestBitIsSet         (uint8_t bit);
#endif