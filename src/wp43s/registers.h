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

#define FIRST_LOCAL_REGISTER 112

#define REGISTER_X  100
#define REGISTER_Y  101
#define REGISTER_Z  102
#define REGISTER_T  103
#define REGISTER_A  104
#define REGISTER_B  105
#define REGISTER_C  106
#define REGISTER_D  107
#define REGISTER_L  108
#define REGISTER_I  109
#define REGISTER_J  110
#define REGISTER_K  111

// If one of the 3 next defines is changed: change also xxxLINE in screen.h
#define AIM_REGISTER_LINE REGISTER_X
#define TAM_REGISTER_LINE REGISTER_T
#define NIM_REGISTER_LINE REGISTER_X

#define FIRST_TEMPORARY_REGISTER         2000
#define NUMBER_OF_TEMPORARY_REGISTERS       8 // 7 are used here: [3] [1/x] [f] [a b/c] [1/x]

#define SAVED_REGISTER_X 3000
#define SAVED_REGISTER_Y 3001
#define SAVED_REGISTER_Z 3002
#define SAVED_REGISTER_T 3003
#define SAVED_REGISTER_A 3004
#define SAVED_REGISTER_B 3005
#define SAVED_REGISTER_C 3006
#define SAVED_REGISTER_D 3007
#define SAVED_REGISTER_L 3008

#define OFFSET_REGISTER_POINTER        0
#define LENGTH_REGISTER_POINTER       16

#define OFFSET_REGISTER_DATA_TYPE     16
#define LENGTH_REGISTER_DATA_TYPE      4

#define OFFSET_REGISTER_INFORMATION   20
#define LENGTH_REGISTER_INFORMATION    5 // Angular mode (3 bits) or base (5 bits)

#define OFFSET_FILLER                 25
#define LENGTH_FILLER                  2 // free bits

#define OFFSET_REGISTER_NAME_LENGTH   27
#define LENGTH_REGISTER_NAME_LENGTH    5 // variable name memory area length in byte, 0, 1, ..., 30. 2 to 16 used for now. 16 = 7 glyphs × 2 + terminating 0  + 1 to be even

#define getStackTop()                      (stackSize == SS_4 ? REGISTER_T : REGISTER_D)

#define getRegisterBase(regist)            getRegisterDataInfo(regist)       // Only for a small integer
#define setRegisterBase(regist, base)      setRegisterDataInfo(regist, base) // Only for a small integer
#define setRegisterSign(regist, sign)      setRegisterDataInfo(regist, sign) // Only for a big integer



/********************************************//**
 * \file registers.h
 ***********************************************/

///////////////////////////////////////////////////////
// Register numbering:
//    0 to  111 global resisters
//  112 to  199 local registers (.00 to .87)
// 1000 to 1999 named registers
// 2000 to 2007 temporary registers
// 3000 to 3007 saved stack registers (UNDO item)


/********************************************//**
 * \typedef dataType_t
 * \brief Different data types
 ***********************************************/
typedef enum {
  dtBigInteger      =  0,  ///< Z arbitrary precision integer
  dtReal16          =  1,  ///< R single precision real (64 bits)
  dtComplex16       =  2,  ///< C single precision complex (2x 64 bits), RegDataInfo contains rectangular or polar mode
  dtTime            =  4,  ///< Time
  dtDate            =  5,  ///< Date in various formats
  dtString          =  6,  ///< Alphanumeric string
  dtReal16Matrix    =  7,  ///< Single precision vector or matrix
  dtComplex16Matrix =  8,  ///< Double precision vector or matrix
  dtSmallInteger    =  9,  ///< Finite precision integer
  dtReal34          = 10,  ///< R double precision real (128 bits)
  dtComplex34       = 11   ///< C double precision complex (2x 128 bits)
  //dtLabel           = 12,  ///< Label
  //dtSystemInteger   = 13,  ///< System integer (64 bits)
  //dtFlags           = 14,  ///< Flags
  //dtConfig          = 15,  ///< Configuration
  //dtPgmStep         = 16,  ///< Program step
  //dtDirectory       = 17,  ///< Program
} dataType_t; // 4 bits (NOT 5 BITS)


uint32_t          getRegisterDataType             (calcRegister_t regist);
uint32_t          getRegisterDataPointer          (calcRegister_t regist);
uint32_t          getRegisterDataInfo             (calcRegister_t regist);
uint32_t          getRegisterNameLength           (calcRegister_t regist);
uint32_t          getRegisterNamePointer          (calcRegister_t regist);
uint16_t          getRegisterMaxStringLength      (calcRegister_t regist);
void              setRegisterDataType             (calcRegister_t regist, uint16_t dataType);
void              setRegisterDataPointer          (calcRegister_t regist, uint32_t dataPointer);
void              setRegisterDataInfo             (calcRegister_t regist, uint16_t dataInfo);
void              setRegisterNameLength           (calcRegister_t regist, uint16_t length);
void              setRegisterNamePointer          (calcRegister_t regist, uint32_t pointer);
void              setRegisterMaxStringLength      (calcRegister_t regist, uint16_t maxStringLen);
void              allocateLocalRegisters          (uint16_t n);
void              allocateNamedRegister           (const char *registerName);
uint32_t          allocateMemory                  (uint32_t numBytes);
void              allocateMemoryInsert            (uint32_t address, uint32_t numBytes);
void              freeMemory                      (uint32_t address, uint32_t numBytes);
void              freeRegisterData                (calcRegister_t regist);
uint32_t          getRegisterDataSize             (calcRegister_t regist);
void              clearRegister                   (calcRegister_t regist);
void              fnClearRegisters                (uint16_t unusedParamButMandatory);
void              fnGetLocR                       (uint16_t unusedParamButMandatory);
void              fnConvertXToReal34              (uint16_t unusedParamButMandatory);
void              fnConvertXToReal16              (uint16_t unusedParamButMandatory);
void              roundRegister                   (calcRegister_t regist);
void              copySourceRegisterToDestRegister(calcRegister_t rSource, calcRegister_t rDest);
void              fnStore                         (uint16_t r);
void              fnStoreAdd                      (uint16_t r);
void              fnStoreSub                      (uint16_t r);
void              fnStoreMult                     (uint16_t r);
void              fnStoreDiv                      (uint16_t r);
void              fnStoreMin                      (uint16_t r);
void              fnStoreMax                      (uint16_t r);
void              fnRecall                        (uint16_t r);
void              fnRecallAdd                     (uint16_t r);
void              fnRecallSub                     (uint16_t r);
void              fnRecallMult                    (uint16_t r);
void              fnRecallDiv                     (uint16_t r);
void              fnRecallMin                     (uint16_t r);
void              fnRecallMax                     (uint16_t r);
void              fnXLessThan                     (uint16_t unusedParamButMandatory);
calcRegister_t    indirectAddressing              (calcRegister_t regist);
void              printRegisterToConsole          (calcRegister_t regist, int16_t line);
void              printReal16ToConsole            (const real16_t *value);
void              printReal34ToConsole            (const real34_t *value);
void              printComplex34ToConsole         (const complex34_t *value);
void              printReal51ToConsole            (const real51_t *value);
void              printBigIntegerToConsole        (bigInteger_t *value);
void              reallocateRegister              (calcRegister_t regist, uint32_t dataType, uint16_t numBytes, uint32_t dataInfo);
calcRegister_t    allocateTemporaryRegister       (void);
void              freeTemporaryRegister           (calcRegister_t tmpReg);
