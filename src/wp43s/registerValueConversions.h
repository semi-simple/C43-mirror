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
 * \file registerValueConversions.h
 ***********************************************/

void convertRegister16To34                           (calcRegister_t regist);
void convertRegister34To16                           (calcRegister_t regist);
void convertLongIntegerRegisterToReal16Register       (calcRegister_t source, calcRegister_t destination);
void convertLongIntegerRegisterToReal34Register       (calcRegister_t source, calcRegister_t destination);
void convertLongIntegerRegisterToShortIntegerRegister (calcRegister_t source, calcRegister_t destination);
void convertShortIntegerRegisterToReal16Register     (calcRegister_t source, calcRegister_t destination);
void convertShortIntegerRegisterToReal34Register     (calcRegister_t source, calcRegister_t destination);
void convertShortIntegerRegisterLongIntegerRegister   (calcRegister_t source, calcRegister_t destination);

void convertLongIntegerToLongIntegerRegister           (const longInteger_t *longInteger, calcRegister_t regist);
void convertLongIntegerRegisterToLongInteger           (calcRegister_t regist, longInteger_t *longInteger);

void convertShortIntegerRegisterToUInt64             (calcRegister_t regist, int16_t *sign, uint64_t *value);
void convertUInt64ToShortIntegerRegister             (int16_t sign, uint64_t value, uint32_t base, calcRegister_t regist);

#if (ANGLE16 == 1)
  #define convertLongIntegerRegisterToAngleRegister(source, destination)   convertLongIntegerRegisterToReal16Register(source, destination)
  #define convertShortIntegerRegisterToAngleRegister(source, destination) convertShortIntegerRegisterToReal16Register(source, destination)
#endif
#if (ANGLE34 == 1)
  #define convertLongIntegerRegisterToAngleRegister(source, destination)   convertLongIntegerRegisterToReal34Register(source, destination)
  #define convertShortIntegerRegisterToAngleRegister(source, destination) convertShortIntegerRegisterToReal34Register(source, destination)
#endif
