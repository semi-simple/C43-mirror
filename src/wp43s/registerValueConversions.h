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

void convertRegister16To34                        (calcRegister_t regist);
void convertRegister34To16                        (calcRegister_t regist);
void convertBigIntegerRegisterToReal16Register    (calcRegister_t source, calcRegister_t destination);
void convertBigIntegerRegisterToReal34Register    (calcRegister_t source, calcRegister_t destination);
void convertSmallIntegerRegisterToReal16Register  (calcRegister_t source, calcRegister_t destination);
void convertSmallIntegerRegisterToReal34Register  (calcRegister_t source, calcRegister_t destination);
void convertSmallIntegerRegisterBigIntegerRegister(calcRegister_t source, calcRegister_t destination);

void convertBigIntegerToBigIntegerRegister        (const bigInteger_t *bigInteger, calcRegister_t regist);
void convertBigIntegerRegisterToBigInteger        (calcRegister_t regist, bigInteger_t *bigInteger);

void convertSmallIntegerRegisterToUInt64          (calcRegister_t regist, int16_t *sign, uint64_t *value);
void convertUInt64ToSmallIntegerRegister          (int16_t sign, uint64_t value, uint32_t base, calcRegister_t regist);
