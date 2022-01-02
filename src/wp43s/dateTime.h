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
 * \file dateTime.h
 ***********************************************/
#ifndef DATETIME_H
#define DATETIME_H

#include "realType.h"
#include "typeDefinitions.h"
#include <stdint.h>

void fnSetDateFormat         (uint16_t dateFormat);

/********************************************//**
 * \brief Convert internal date representation to Julian day number (floored)
 *
 * \param[in] source real34_t*
 * \param[out] destination real34_t*
 * \return void
 ***********************************************/
void internalDateToJulianDay (const real34_t *source, real34_t *destination);

/********************************************//**
 * \brief Convert Julian day number (floored) to internal date representation
 *
 * \param[in] source real34_t*
 * \param[out] destination real34_t*
 * \return void
 ***********************************************/
void julianDayToInternalDate (const real34_t *source, real34_t *destination);

/********************************************//**
 * \brief Check date argument and get Julian day
 *
 * \param[in] regist calcRegister_t register
 * \param[out] jd real34_t* Julian day (floored)
 * \return bool_t true if valid
 ***********************************************/
bool_t checkDateArgument     (calcRegister_t regist, real34_t *jd);

/********************************************//**
 * \brief Check for leap year
 *
 * \param[in] year real34_t*
 * \return bool_t true if leap year
 ***********************************************/
bool_t isLeapYear            (const real34_t *year);

/********************************************//**
 * \brief Check if date is valid (e.g. 30 Feb is invalid)
 *
 * \param[in] year real34_t*
 * \param[in] month real34_t*
 * \param[in] day real34_t*
 * \return bool_t true if valid
 ***********************************************/
bool_t isValidDay            (const real34_t *year, const real34_t *month, const real34_t *day);

/********************************************//**
 * \brief Convert date into Julian day number floored
 *
 * \param[in] year real34_t*
 * \param[in] month real34_t*
 * \param[in] day real34_t*
 * \param[out] jd real34_t*
 * \return void
 ***********************************************/
void composeJulianDay        (const real34_t *year, const real34_t *month, const real34_t *day, real34_t *jd);
void composeJulianDay_g      (const real34_t *year, const real34_t *month, const real34_t *day, real34_t *jd);
void composeJulianDay_j      (const real34_t *year, const real34_t *month, const real34_t *day, real34_t *jd);

/********************************************//**
 * \brief Convert Julian day number (floored) into date
 *
 * \param[in] jd real34_t*
 * \param[out] year real34_t*
 * \param[out] month real34_t*
 * \param[out] day real34_t*
 * \return void
 ***********************************************/
void decomposeJulianDay      (const real34_t *jd, real34_t *year, real34_t *month, real34_t *day);

/********************************************//**
 * \brief Get day of week from date in given register
 *
 * \param[in] regist calcRegister_t register
 * \return uint32_t day of week (1 = Monday, 7 = Sunday), 0 if invalid
 ***********************************************/
uint32_t getDayOfWeek        (calcRegister_t regist);

/********************************************//**
 * \brief Check date range
 *
 * \param[in] time34 real34_t*
 * \return void
 ***********************************************/
void checkDateRange          (const real34_t *date34);

/********************************************//**
 * \brief Convert H.MMSS into seconds
 *
 * \param[in] src real34_t* H.MMSS-formatted time value (for input)
 * \param[out] dest real34_t* time value in seconds (internal representation)
 * \return void
 ***********************************************/
void hmmssToSeconds          (const real34_t *src, real34_t *dest);

/********************************************//**
 * \brief Convert H.MMSS in given register into time
 *
 * \param[in] regist calcRegister_t register
 * \return void
 ***********************************************/
void hmmssInRegisterToSeconds(calcRegister_t regist);

/********************************************//**
 * \brief Check time range
 *
 * \param[in] time34 real34_t*
 * \return void
 ***********************************************/
void checkTimeRange          (const real34_t *time34);

void fnJulianToDate          (uint16_t unusedButMandatoryParameter);
void fnDateToJulian          (uint16_t unusedButMandatoryParameter);
void fnIsLeap                (uint16_t unusedButMandatoryParameter);
void fnSetFirstGregorianDay  (uint16_t unusedButMandatoryParameter);
void fnGetFirstGregorianDay  (uint16_t unusedButMandatoryParameter);
void fnXToDate               (uint16_t unusedButMandatoryParameter);
void fnYear                  (uint16_t unusedButMandatoryParameter);
void fnMonth                 (uint16_t unusedButMandatoryParameter);
void fnDay                   (uint16_t unusedButMandatoryParameter);
void fnWday                  (uint16_t unusedButMandatoryParameter);
void fnDateTo                (uint16_t unusedButMandatoryParameter);
void fnToDate                (uint16_t unusedButMandatoryParameter);
void fnToHr                  (uint16_t unusedButMandatoryParameter);
void fnToHms                 (uint16_t unusedButMandatoryParameter);
void fnDate                  (uint16_t unusedButMandatoryParameter);
void fnTime                  (uint16_t unusedButMandatoryParameter);
void fnSetDate               (uint16_t unusedButMandatoryParameter);
void fnSetTime               (uint16_t unusedButMandatoryParameter);

/********************************************//**
 * \brief Gets the system date
 *
 * \param[out] dateString char* String receiving the system date.  Must be at least 11 bytes long (yyyy-mm-dd plus terminating 0)
 * \return void
 ***********************************************/
void getDateString           (char *dateString);

/********************************************//**
 * \brief Gets the system time
 *
 * \param[out] timeString char* String receiving the system time. Must be at least 8 bytes long (03:15pm plus terminating 0)
 * \return void
 ***********************************************/
void getTimeString           (char *timeString);

#endif // DATETIME_H
