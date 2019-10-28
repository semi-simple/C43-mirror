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
 * \file config.h
 ***********************************************/

void     fnConfigChina         (uint16_t unusedParamButMandatory);
void     fnConfigEurope        (uint16_t unusedParamButMandatory);
void     fnConfigIndia         (uint16_t unusedParamButMandatory);
void     fnConfigJapan         (uint16_t unusedParamButMandatory);
void     fnConfigUk            (uint16_t unusedParamButMandatory);
void     fnConfigUsa           (uint16_t unusedParamButMandatory);
void     fnIntegerMode         (uint16_t mode);
void     fnLeadingZeros        (uint16_t displayLeadingZeros);
void     fnWho                 (uint16_t unusedParamButMandatory);
void     fnVersion             (uint16_t unusedParamButMandatory);
void     fnFreeMemory          (uint16_t unusedParamButMandatory);
void     fnGetRoundingMode     (uint16_t unusedParamButMandatory);
void     fnGetIntegerSignMode  (uint16_t unusedParamButMandatory);
void     fnGetWordSize         (uint16_t unusedParamButMandatory);
void     fnSetWordSize         (uint16_t WS);
void     fnFreeFlashMemory     (uint16_t unusedParamButMandatory);
void     fnBatteryVoltage      (uint16_t unusedParamButMandatory);
uint32_t getFreeFlash          (void);
void     fnGetSignificantDigits(uint16_t unusedParamButMandatory);
void     fnRoundingMode        (uint16_t RM);
void     fnAngularMode         (uint16_t angularMode);
void     fnReset               (uint16_t confirmation);
void     fnComplexUnit         (uint16_t complexUnit);
void     fnComplexResult       (uint16_t complexRes);
void     fnComplexMode         (uint16_t complexMode);
void     setConfirmationMode   (void (*func)(uint16_t));
void     fnClAll               (uint16_t confirmation);
void     fnClPAll              (uint16_t confirmation);
void     fnClSigma             (uint16_t unusedParamButMandatory);
void     backToSystem          (uint16_t unusedParamButMandatory);
int8_t   fnCbIsSet             (int16_t item);                                  //dr build RadioButton, CheckBox
void     fnRefreshRadioState   (char rb, uint16_t mode);
void     fnRefreshComboxState  (char rb, uint16_t param, bool_t state);
void     fnRebuildRadioState   (void);                                          //^^

/********************************************//**
 * \typedef radiocb_t
 * \brief Structure keeping the information for one item
 ***********************************************/
#define RB_NO   ' '
#define RB_AM   '!' // AngularMode
#define RB_CM   '"' // ComplexMode
#define RB_CU   '#' // ComplexUnit
#define RB_CF   '$' // CurveFitting
#define RB_DF   '%' // DateFormat
#define RB_DM   '&' // DenominatorMode
#define RB_DI   '\''// DisplayFormat
#define RB_DO   '(' // DisplayModeOverride
#define RB_FT   ')' // FractionType
#define RB_SIM  '*' // IntegerMode
#define RB_PS   '+' // ProductSign
#define RB_RM   ',' // RadixMark
#define RB_SS   '-' // StackSize
#define RB_TF   '.' // TimeFormat
#define RB_BCR  '/' // ComplexResult
#define RB_BLZ  '0' // LeadingZeros
#define RB_SA   '>' // SigmaAssign
#define RB_ID   '?' // InputDefault
#define CB_NO   '@'
#define CB_JC   'A' // CheckBox 
typedef struct {
  uint16_t itemNr;            ///<
  uint16_t param;             ///< 1st parameter to the above function
  uint8_t  state;             ///<
  char     radioButton;       ///< Menu of RADIO in which the item is located: see #define RB_*
} radiocb_t;
