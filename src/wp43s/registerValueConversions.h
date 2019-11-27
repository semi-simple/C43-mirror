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

void convertLongIntegerRegisterToReal16Register      (calcRegister_t source, calcRegister_t destination);
void convertLongIntegerRegisterToReal34Register      (calcRegister_t source, calcRegister_t destination);
void convertLongIntegerRegisterToShortIntegerRegister(calcRegister_t source, calcRegister_t destination);
void convertLongIntegerRegisterToReal16              (calcRegister_t source, real16_t *destination);
void convertLongIntegerRegisterToReal34              (calcRegister_t source, real34_t *destination);
void convertLongIntegerRegisterToRealIc              (calcRegister_t source, realIc_t *destination);
void convertLongIntegerRegisterToLongInteger         (calcRegister_t regist, longInteger_t longInteger);

void convertLongIntegerToLongIntegerRegister         (const longInteger_t longInteger, calcRegister_t regist);
void convertLongIntegerToShortIntegerRegister        (longInteger_t lgInt, uint32_t base, calcRegister_t destination);

void convertShortIntegerRegisterToReal16Register     (calcRegister_t source, calcRegister_t destination);
void convertShortIntegerRegisterToReal34Register     (calcRegister_t source, calcRegister_t destination);
void convertShortIntegerRegisterToLongIntegerRegister(calcRegister_t source, calcRegister_t destination);
void convertShortIntegerRegisterToLongInteger        (calcRegister_t source, longInteger_t lgInt);
void convertShortIntegerRegisterToRealIc             (calcRegister_t source, realIc_t *destination);

void convertShortIntegerRegisterToUInt64             (calcRegister_t regist, int16_t *sign, uint64_t *value);
void convertUInt64ToShortIntegerRegister             (int16_t sign, uint64_t value, uint32_t base, calcRegister_t regist);

void convertReal16ToLongInteger(real16_t *real16, longInteger_t lgInt, enum rounding roundingMode);
void convertReal34ToLongInteger(real34_t *real34, longInteger_t lgInt, enum rounding roundingMode);
void convertRealIcToLongInteger(realIc_t *realIc, longInteger_t lgInt, enum rounding roundingMode);
void convertReal16ToLongIntegerRegister(real16_t *real16, calcRegister_t dest, enum rounding roundingMode);
void convertReal34ToLongIntegerRegister(real34_t *real34, calcRegister_t dest, enum rounding roundingMode);
void convertRealIcToLongIntegerRegister(realIc_t *realIc, calcRegister_t dest, enum rounding roundingMode);
