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
 * \file dateTime.c
 ***********************************************/

#include "wp43s.h"

void fnSetDateFormat(uint16_t dateFormat) {
  switch(dateFormat) {
    case ITM_DMY:
      clearSystemFlag(FLAG_MDY);
      clearSystemFlag(FLAG_YMD);
      setSystemFlag(FLAG_DMY);
      break;

    case ITM_MDY:
      clearSystemFlag(FLAG_DMY);
      clearSystemFlag(FLAG_YMD);
      setSystemFlag(FLAG_MDY);
      break;

    case ITM_YMD:
      clearSystemFlag(FLAG_MDY);
      clearSystemFlag(FLAG_DMY);
      setSystemFlag(FLAG_YMD);
      break;

    default: {}
  }
}

/********************************************//**
 * \brief Convert H.MMSS into seconds
 *
 * \param[in] src real34_t* H.MMSS-formatted time value (for input)
 * \param[out] dest real34_t* time value in seconds (internal representation)
 * \return void
 ***********************************************/
void hmmssToSeconds(const real34_t *src, real34_t *dest) {
  real34_t time34, real34, value34;
  int32_t sign;

  sign = real34IsNegative(src);

  // Hours
  real34CopyAbs(src, &time34);
  real34ToIntegralValue(&time34, &real34, DEC_ROUND_DOWN);
  int32ToReal34(3600, &value34);
  real34Multiply(&real34, &value34, dest);

  // Minutes
  real34Subtract(&time34, &real34, &time34);
  int32ToReal34(100, &value34);
  real34Multiply(&time34, &value34, &time34);
  real34ToIntegralValue(&time34, &real34, DEC_ROUND_DOWN);
  int32ToReal34(60, &value34);
  real34Multiply(&value34, &real34, &value34);
  real34Add(dest, &value34, dest);

  // Seconds
  real34Subtract(&time34, &real34, &time34);
  int32ToReal34(100, &value34);
  real34Multiply(&time34, &value34, &time34);
  real34Add(dest, &time34, dest);

  // Sign
  if(sign) {
    real34SetNegativeSign(dest);
  }
}

/********************************************//**
 * \brief Convert H.MMSS in given register into time
 *
 * \param[in] regist calcRegister_t register
 * \return void
 ***********************************************/
void hmmssInRegisterToSeconds(calcRegister_t regist) {
  real34_t real34;
  real34Copy(REGISTER_REAL34_DATA(regist), &real34);
  reallocateRegister(regist, dtTime, REAL34_SIZE, AM_NONE);
  hmmssToSeconds(&real34, REGISTER_REAL34_DATA(regist));
}


void fnToHr(uint16_t unusedButMandatoryParameter) {
  switch(getRegisterDataType(REGISTER_X)) {
    case dtTime:
      convertTimeRegisterToReal34Register(REGISTER_X, REGISTER_X);
      break;

    default :
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "data type %s cannot be converted to a real34!", getRegisterDataTypeName(REGISTER_X, false, false));
        moreInfoOnError("In function fnToReal:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
  }
}

void fnToHms(uint16_t unusedButMandatoryParameter) {
  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger :
      convertLongIntegerRegisterToTimeRegister(REGISTER_X, REGISTER_X);
      break;

    case dtReal34:
      convertReal34RegisterToTimeRegister(REGISTER_X, REGISTER_X);
      break;

    case dtTime:
      /* already in hours: do nothing */
      break;

    default :
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "data type %s cannot be converted to time!", getRegisterDataTypeName(REGISTER_X, false, false));
        moreInfoOnError("In function fnToReal:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
  }
}


void fnTime(uint16_t unusedButMandatoryParameter) {
  real34_t time34;
#ifdef PC_BUILD
  time_t epoch = time(NULL);
  struct tm *timeInfo = localtime(&epoch);

  uInt32ToReal34((uint32_t)timeInfo->tm_hour * 3600u + (uint32_t)timeInfo->tm_min * 60u + (uint32_t)timeInfo->tm_sec,
    &time34);
#endif // PC_BUILD

#ifdef DMCP_BUILD
  tm_t timeInfo;
  dt_t dateInfo;

  rtc_read(&timeInfo, &dateInfo);
  uInt32ToReal34((uint32_t)timeInfo.hour * 3600u + (uint32_t)timeInfo.min * 60u + (uint32_t)timeInfo.sec,
    &time34);
#endif // DMCP_BUILD
  liftStack();
  reallocateRegister(REGISTER_X, dtTime, REAL34_SIZE, AM_NONE);
  real34Copy(&time34, REGISTER_REAL34_DATA(REGISTER_X));
}


void fnSetTime(uint16_t unusedButMandatoryParameter) {
#ifdef PC_BUILD
  displayCalcErrorMessage(ERROR_FUNCTION_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
  sprintf(errorMessage, "SETTIM is unavailable on the emulator!");
  moreInfoOnError("In function fnSetTime:", errorMessage, NULL, NULL);
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
#endif // PC_BUILD

#ifdef DMCP_BUILD
  tm_t timeInfo;
  dt_t dateInfo;
  real34_t time34, value34;
  int32_t timeVal;

  if(getRegisterDataType(REGISTER_X) == dtTime) {
    int32ToReal34(86400, &value34);
    if(real34IsNegative(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34CompareGreaterEqual(REGISTER_REAL34_DATA(REGISTER_X), &value34)) {
      displayCalcErrorMessage(ERROR_BAD_TIME_OR_DATE_INPUT, ERR_REGISTER_LINE, REGISTER_X);
    }
    else {
      rtc_read(&timeInfo, &dateInfo);
      int32ToReal34(100, &value34);
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), &value34, &time34);
      real34ToIntegralValue(&time34, &time34, DEC_ROUND_DOWN);
      timeVal = real34ToInt32(&time34);
      timeInfo.csec =  timeVal         % 100;
      timeInfo.sec  = (timeVal /= 100) %  60;
      timeInfo.min  = (timeVal /=  60) %  60;
      timeInfo.hour = (timeVal /=  60);
      rtc_write(&timeInfo, &dateInfo);
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  }
#endif // DMCP_BUILD
}


/********************************************//**
 * \brief Gets the system date
 *
 * \param[out] dateString char* String receiving the system date.  Must be at least 11 bytes long (yyyy-mm-dd plus terminating 0)
 * \return void
 ***********************************************/
void getDateString(char *dateString) {
  #ifdef PC_BUILD
    time_t epoch = time(NULL);
    struct tm *timeInfo = localtime(&epoch);

    // For the format string : man strftime
    if(!getSystemFlag(FLAG_TDM24)) { // time format = 12H ==> 2 digit year
      if(getSystemFlag(FLAG_DMY)) {
        strftime(dateString, 11, "%d.%m.%y", timeInfo);
      }
      else if(getSystemFlag(FLAG_YMD)) {
        strftime(dateString, 11, "%y-%m-%d", timeInfo);
      }
      else if(getSystemFlag(FLAG_MDY)) {
        strftime(dateString, 11, "%m/%d/%y", timeInfo);
      }
      else {
        strcpy(dateString, "?? ?? ????");
      }
    }
    else {// 4 digit year
      if(getSystemFlag(FLAG_DMY)) {
        strftime(dateString, 11, "%d.%m.%Y", timeInfo);
      }
      else if(getSystemFlag(FLAG_YMD)) {
        strftime(dateString, 11, "%Y-%m-%d", timeInfo);
      }
      else if(getSystemFlag(FLAG_MDY)) {
        strftime(dateString, 11, "%m/%d/%Y", timeInfo);
      }
      else {
        strcpy(dateString, "?? ?? ????");
      }
    }
  #endif // PC_BUILD

  #ifdef DMCP_BUILD
    tm_t timeInfo;
    dt_t dateInfo;

    rtc_read(&timeInfo, &dateInfo);
    if(!getSystemFlag(FLAG_TDM24)) { // 2 digit year
      if(getSystemFlag(FLAG_DMY)) {
        sprintf(dateString, "%02d.%02d.%02d", dateInfo.day, dateInfo.month, dateInfo.year % 100);
      }
      else if(getSystemFlag(FLAG_YMD)) {
        sprintf(dateString, "%02d-%02d-%02d", dateInfo.year % 100, dateInfo.month, dateInfo.day);
      }
      else if(getSystemFlag(FLAG_MDY)) {
        sprintf(dateString, "%02d/%02d/%02d", dateInfo.month, dateInfo.day, dateInfo.year % 100);
      }
      else {
        strcpy(dateString, "?? ?? ????");
      }
    }
    else {// 4 digit year
      if(getSystemFlag(FLAG_DMY)) {
        sprintf(dateString, "%02d.%02d.%04d", dateInfo.day, dateInfo.month, dateInfo.year);
      }
      else if(getSystemFlag(FLAG_YMD)) {
        sprintf(dateString, "%04d-%02d-%02d", dateInfo.year, dateInfo.month, dateInfo.day);
      }
      else if(getSystemFlag(FLAG_MDY)) {
        sprintf(dateString, "%02d/%02d/%04d", dateInfo.month, dateInfo.day, dateInfo.year);
      }
      else {
        strcpy(dateString, "?? ?? ????");
      }
    }
  #endif // DMCP_BUILD
}



/********************************************//**
 * \brief Gets the system time
 *
 * \param[out] timeString char* String receiving the system time. Must be at least 8 bytes long (03:15pm plus terminating 0)
 * \return void
 ***********************************************/
void getTimeString(char *timeString) {
  #ifdef PC_BUILD
    time_t epoch = time(NULL);
    struct tm *timeInfo = localtime(&epoch);

    // For the format string : man strftime
    if(getSystemFlag(FLAG_TDM24)) { // time format = 24H
      strftime(timeString, 8, "%H:%M", timeInfo); // %R don't work on Windows
    }
    else {
      strftime(timeString, 8, "%I:%M", timeInfo); // I could use %p but in many locals the AM and PM string are empty
      if(timeInfo->tm_hour >= 12) {
        strcat(timeString, "pm");
      }
      else {
        strcat(timeString, "am");
      }
    }
  #endif // PC_BUILD

  #ifdef DMCP_BUILD
    tm_t timeInfo;
    dt_t dateInfo;

    rtc_read(&timeInfo, &dateInfo);
    if(!getSystemFlag(FLAG_TDM24)) {
      sprintf(timeString, "%02d:%02d", (timeInfo.hour % 12) == 0 ? 12 : timeInfo.hour % 12, timeInfo.min);
      if(timeInfo.hour >= 12) {
        strcat(timeString, "pm");
      }
      else {
        strcat(timeString, "am");
      }
    }
    else {
      sprintf(timeString, "%02d:%02d", timeInfo.hour, timeInfo.min);
    }
  #endif // DMCP_BUILD
}
