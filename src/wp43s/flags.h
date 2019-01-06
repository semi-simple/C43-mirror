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

#define NUMBER_OF_LOCAL_FLAGS 112

#define FLAG_X        100
#define FLAG_Y        101
#define FLAG_Z        102
#define FLAG_T        103 // Tracing
#define FLAG_TRACING  103
#define FLAG_A        104
#define FLAG_B        105 // Overflow
#define FLAG_OVERFLOW 105
#define FLAG_C        106 // Carry
#define FLAG_CARRY    106
#define FLAG_D        107 // Danger
#define FLAG_DANGER   107
#define FLAG_L        108
#define FLAG_I        109 // CPXRES
#define FLAG_CPXRES   109
#define FLAG_J        110
#define FLAG_K        111

bool_t getFlag           (uint16_t f);
void   fnSetFlag         (uint16_t f);
void   fnClearFlag       (uint16_t f);
void   fnFlipFlag        (uint16_t f);
void   fnClFAll          (uint16_t unusedParamButMandatory);
void   fnIsFlagClear     (uint16_t f);
void   fnIsFlagSet       (uint16_t f);
void   fnIsFlagClearClear(uint16_t f);
void   fnIsFlagSetClear  (uint16_t f);
void   fnIsFlagClearSet  (uint16_t f);
void   fnIsFlagSetSet    (uint16_t f);
void   fnIsFlagClearFlip (uint16_t f);
void   fnIsFlagSetFlip   (uint16_t f);
