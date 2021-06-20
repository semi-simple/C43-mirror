/* This file is part of WP 43S.
 *
 * WP 43S is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WP 43S is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WP 43S.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ADDITIONAL C43 functions and routines */


/********************************************//**
 * \file radioButtonCatalog.h
 ***********************************************/

#ifndef RADIOBUTTONCATALOG_H
#define RADIOBUTTONCATALOG_H

#include <stdint.h>

#define RB_FALSE    0
#define RB_TRUE     1
#define CB_FALSE    2
#define CB_TRUE     3

#define NOVAL              -126
#define ITEM_NOT_CODED     -127


int8_t   fnCbIsSet             (int16_t item);
void     fnRefreshState        (void);
int16_t  fnItemShowValue       (int16_t item);
char*    figlabel              (const char* label, int16_t showValue);


/********************************************//**
 * \typedef radiocb_t
 * \brief Structure keeping the information for one item
 ***********************************************/
//??? #define RB_NO   ' '
#define RB_AM   '!' // AngularMode
#define RB_CM   '"' // ComplexMode
#define RB_CU   '#' // ComplexUnit
#define RB_CF   '$' // CurveFitting
#define RB_DF   '%' // DateFormat
#define RB_DI   '\''// DisplayFormat
#define RB_DO   '(' // DisplayModeOverride
//??? #define RB_FT   ')' // FractionType
#define RB_IM   '*' // IntegerMode
#define RB_PS   '+' // ProductSign
#define RB_RX   ',' // RadixMark
#define RB_SS   '-' // StackSize
#define RB_TF   '.' // TimeFormat
#define RB_WS   '/' // WordSize
//#define RB_BCR  '0' // ComplexResult
//#define RB_BLZ  '1' // LeadingZeros
#define RB_SA   '>' // SigmaAssign
#define RB_ID   '?' // InputDefault
#define CB_JC   'A' // CheckBox 
#define RB_HX   'B' // BASE
#define RB_NM   'C' // NORMALMODE
//??? #define RB_AM2  'C' // AngularMode
//??? #define RB_ZM   'D' // AngularMode



typedef struct {
  uint16_t itemNr;            ///<
  uint16_t param;             ///< 1st parameter to the above function
  char     radioButton;       ///< Menu of RADIO in which the item is located: see #define RB_*
} radiocb_t;

#endif // RADIOBUTTONCATALOG_H
