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

#include "timer.h"

#include "charString.h"
#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "items.h"
#include "realType.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "stack.h"
#include "screen.h"
#include "softmenus.h"
#include "stats.h"
#include "typeDefinitions.h"
#include <inttypes.h>
#include <stdio.h>
#ifdef PC_BUILD
  #include <glib.h>
#endif // PC_BUILD

#include "wp43s.h"


kb_timer_t  timer[TMR_NUMBER];
#ifdef PC_BUILD
gint64      timerLastCalled;
#endif
#ifdef DMCP_BUILD
uint32_t    timerLastCalled;
bool_t      mutexRefreshTimer = false;
#endif


uint32_t getUptimeMs(void) {
  #ifdef DMCP_BUILD
    return (uint32_t)sys_current_ms();
  #elif !defined(TESTSUITE_BUILD)
    return (uint32_t)(g_get_monotonic_time() / 1000);
  #else // TESTSUITE_BUILD
    return 0;
  #endif // DMCP_BUILD ; TESTSUITE_BUILD
}


void fnTicks(uint16_t unusedButMandatoryParameter) {
  uint32_t tim;
  longInteger_t lgInt;

  tim = getUptimeMs() / 100;

  liftStack();
  longIntegerInit(lgInt);
  uIntToLongInteger(tim, lgInt);
  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
  longIntegerFree(lgInt);
}



void fnRebuildTimerRefresh(void) {
#ifdef DMCP_BUILD
  uint32_t next;

  if(mutexRefreshTimer == false) {
    nextTimerRefresh = 0;
    for(int i = 0; i < TMR_NUMBER; i++) {
      if (timer[i].state == TMR_RUNNING) {
        next = timer[i].timer_will_expire;
        if(nextTimerRefresh != 0 && next < nextTimerRefresh) {
          nextTimerRefresh = next;
        }
        if(nextTimerRefresh == 0) {
          nextTimerRefresh = next;
        }
      }
    }
  }
#endif
}


#ifdef PC_BUILD
/********************************************//**
 * \brief Refreshes timer. This function is
 * called every 5 ms by a GTK timer.
 *
 * \param[in] data gpointer Not used
 * \return gboolean         What will happen next?
 *                          * true  = timer will call this function again
 *                          * false = timer stops calling this function
 ***********************************************/
gboolean refreshTimer(gpointer data) {      // This function is called every 5 ms by a GTK timer
  gint64 now = g_get_monotonic_time();

  if(now < timerLastCalled) {
    for(int i = 0; i < TMR_NUMBER; i++) {
      if(timer[i].state == TMR_RUNNING) {
        timer[i].state = TMR_COMPLETED;
        timer[i].func(timer[i].param);      // Callback to configured function
      }
    }
  }
  else {
    for(int i = 0; i < TMR_NUMBER; i++) {
      if(timer[i].state == TMR_RUNNING) {
        if(timer[i].timer_will_expire <= now) {
          timer[i].state = TMR_COMPLETED;
          timer[i].func(timer[i].param);    // Callback to configured function
        }
      }
    }
  }

  timerLastCalled = now;

//fnRebuildTimerRefresh();

  return TRUE;
}
#endif

#ifdef DMCP_BUILD
void refreshTimer(void) {                   // This function is called when nextTimerRefresh has been elapsed
  uint32_t now = (uint32_t)sys_current_ms();

  if(now < timerLastCalled) {
    for(int i = 0; i < TMR_NUMBER; i++) {
      if(timer[i].state == TMR_RUNNING) {
        timer[i].state = TMR_COMPLETED;
        mutexRefreshTimer = true;
        timer[i].func(timer[i].param);      // Callback to configured function
        mutexRefreshTimer = false;
      }
    }
  }
  else {
    for(int i = 0; i < TMR_NUMBER; i++) {
      if(timer[i].state == TMR_RUNNING) {
        if(timer[i].timer_will_expire <= now) {
          timer[i].state = TMR_COMPLETED;
          mutexRefreshTimer = true;
          timer[i].func(timer[i].param);    // Callback to configured function
          mutexRefreshTimer = false;
        }
      }
    }
  }

  timerLastCalled = now;

  fnRebuildTimerRefresh();
}
#endif


void fnTimerDummyTest(uint16_t param) {
#if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
  printf("fnTimerDummyTest called  %u\n", param);
#endif
}



void fnTimerReset(void) {
#ifndef TESTSUITE_BUILD
  timerLastCalled = 0;
#endif

  for(int i = 0; i < TMR_NUMBER; i++) {
    timer[i].state = TMR_UNUSED;
    timer[i].func = fnTimerDummyTest;
    timer[i].param = 0;
  }

#ifdef DMCP_BUILD
  mutexRefreshTimer = false;
#endif
  fnRebuildTimerRefresh();
}



void fnTimerConfig(uint8_t nr, void(*func)(uint16_t), uint16_t param) {
  if(nr < TMR_NUMBER) {
    timer[nr].func  = func;
    timer[nr].param = param;
    timer[nr].state = TMR_STOPPED;
  }
  fnRebuildTimerRefresh();
}



void fnTimerStart(uint8_t nr, uint16_t param, uint32_t time) {
#ifdef DMCP_BUILD
  uint32_t now = (uint32_t)sys_current_ms();
#endif
#ifdef PC_BUILD
  gint64 now = g_get_monotonic_time();
#endif

  if(nr < TMR_NUMBER) {
    timer[nr].param = param;
#ifdef DMCP_BUILD
    timer[nr].timer_will_expire = (uint32_t)(now + time);
#endif
#ifdef PC_BUILD
    timer[nr].timer_will_expire = (gint64)(now + time *1000);
    if(timer[nr].timer_will_expire < 0) {
      timer[nr].timer_will_expire = time *1000;
    }
#endif
    timer[nr].state = TMR_RUNNING;
  }
  fnRebuildTimerRefresh();
}



void fnTimerStop(uint8_t nr) {
  if(nr < TMR_NUMBER && timer[nr].state != TMR_UNUSED) {
    timer[nr].state = TMR_STOPPED;
  }
  fnRebuildTimerRefresh();
}


void fnTimerExec(uint8_t nr) {
  if(nr < TMR_NUMBER && timer[nr].state == TMR_RUNNING) {
    timer[nr].state = TMR_COMPLETED;
    timer[nr].func(timer[nr].param);        // Callback to configured function
  }
}



void fnTimerDel(uint8_t nr) {
  if(nr < TMR_NUMBER) {
    timer[nr].state = TMR_UNUSED;
  }
  fnRebuildTimerRefresh();
}



uint16_t fnTimerGetParam(uint8_t nr) {
  uint16_t result = 0;
  
  if(nr < TMR_NUMBER) {
    result = timer[nr].param;
  }

  return result;
}



uint8_t fnTimerGetStatus(uint8_t nr) {
  uint8_t result = TMR_UNUSED;
  
  if(nr < TMR_NUMBER) {
    result = timer[nr].state;
  }

  return result;
}


// Timer application

#ifndef TESTSUITE_BUILD
static uint32_t _currentTime(void) {
  #ifdef DMCP_BUILD
    tm_t timeInfo;
    dt_t dateInfo;

    rtc_read(&timeInfo, &dateInfo);
    return (uint32_t)timeInfo.hour * 3600000u +
           (uint32_t)timeInfo.min * 60000u +
           (uint32_t)timeInfo.sec * 1000u +
           (uint32_t)timeInfo.csec * 10u;
  #else // !DMCP_BUILD
    return (uint32_t)(g_get_real_time() % 86400000000uLL / 1000uLL);
  #endif // DMCP_BUILD
}

static void _antirewinder(uint32_t currTime) {
  if(currTime < timerStartTime) {
    timerValue += 86400000u - timerStartTime;
    timerStartTime = 0u;
  }
  else if(currTime >= timerStartTime + 3600000u) {
    timerValue += 3600000u;
    timerStartTime += 3600000u;
  }
}

static uint32_t _getTimerValue(void) {
  uint32_t currTime = _currentTime();

  if(timerStartTime == TIMER_APP_STOPPED) {
    return timerValue;
  }

  _antirewinder(currTime);
  return currTime - timerStartTime + timerValue;
}
#endif // TESTSUITE_BUILD

//--  #ifdef PC_BUILD
//--    static gboolean _updateTimer(gpointer unusedData) {
//--      if(calcMode != CM_TIMER) {
//--        return FALSE;
//--      }
//--      fnUpdateTimerApp();
//--      return timerStartTime != TIMER_APP_STOPPED;
//--    }
//--  #endif // PC_BUILD

void fnItemTimerApp(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  calcMode = CM_TIMER;
  showSoftmenu(-MNU_TIMERF);
  rbr1stDigit = true;
  watchIconEnabled = false;
  if(timerStartTime != TIMER_APP_STOPPED) {
    fnTimerStart(TO_TIMER_APP, TO_TIMER_APP, TIMER_APP_PERIOD);
//--  #ifdef PC_BUILD
//--      gdk_threads_add_timeout(100, _updateTimer, NULL);
//--  #endif // PC_BUILD
  }
#endif // TESTSUITE_BUILD
}

void fnAddTimerApp(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  real_t tmp;

  uInt32ToReal(_getTimerValue() / 100u, &tmp);
  tmp.exponent -= 1;
  realDivide(&tmp, const_3600, &tmp, &ctxtReal39);

  fnStatSum(0);
  if(lastErrorCode != ERROR_NONE) {
    liftStack();
    clearRegister(REGISTER_X);
    lastErrorCode = ERROR_NONE;
  }
  real34Add(REGISTER_REAL34_DATA(REGISTER_X), const34_1, REGISTER_REAL34_DATA(REGISTER_X));
  liftStack();
  realToReal34(&tmp, REGISTER_REAL34_DATA(REGISTER_X));
  fnSigma(1);

  refreshScreen();
#endif // TESTSUITE_BUILD
}

void fnDecisecondTimerApp(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  timerCraAndDeciseconds ^= 0x80u;
#endif // TESTSUITE_BUILD
}

void fnResetTimerApp(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  timerValue = 0;
  timerTotalTime = 0;
  if(timerStartTime != TIMER_APP_STOPPED) {
    timerStartTime = _currentTime();
    fnTimerStart(TO_TIMER_APP, TO_TIMER_APP, TIMER_APP_PERIOD);
  }
  rbr1stDigit = true;
#endif // TESTSUITE_BUILD
}

void fnStartStopTimerApp(void) {
#ifndef TESTSUITE_BUILD
  if(timerStartTime == TIMER_APP_STOPPED) {
    setSystemFlag(FLAG_RUNTIM);
    timerStartTime = _currentTime();
    fnTimerStart(TO_TIMER_APP, TO_TIMER_APP, TIMER_APP_PERIOD);
//--  #ifdef PC_BUILD
//--      gdk_threads_add_timeout(100, _updateTimer, NULL);
//--  #endif // PC_BUILD
  }
  else {
    fnStopTimerApp();
  }
  rbr1stDigit = true;
#endif // TESTSUITE_BUILD
}

void fnStopTimerApp(void) {
#ifndef TESTSUITE_BUILD
  if(timerStartTime != TIMER_APP_STOPPED) {
    const uint32_t msec = _currentTime();
    timerValue += msec - timerStartTime;
    if(timerTotalTime > 0) timerTotalTime += msec - timerStartTime;
    timerStartTime = TIMER_APP_STOPPED;
    fnTimerStop(TO_TIMER_APP);
  }
  clearSystemFlag(FLAG_RUNTIM);
  watchIconEnabled = false;
#endif // TESTSUITE_BUILD
}

void fnShowTimerApp(void) {
#ifndef TESTSUITE_BUILD
  if(calcMode == CM_TIMER) {
    const uint32_t msec = _getTimerValue();
    clearRegisterLine(REGISTER_T, true, true);

    if(timerTotalTime > 0) {
      const uint32_t tmsec = msec - timerValue + timerTotalTime;
      if(timerCraAndDeciseconds & 0x80u) {
        sprintf(tmpString, "%" PRIu32 ":%02" PRIu32 ":%02" PRIu32 ".%" PRIu32 STD_SUP_T, tmsec / 3600000u, tmsec % 3600000u / 60000u, tmsec % 60000u / 1000u, tmsec % 1000u / 100u);
      }
      else {
        sprintf(tmpString, "%" PRIu32 ":%02" PRIu32 ":%02" PRIu32 STD_SUP_T, tmsec / 3600000u, tmsec % 3600000u / 60000u, tmsec % 60000u / 1000u);
      }
      showString(tmpString, &numericFont, 1, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, false, true);
    }

    if(timerCraAndDeciseconds & 0x80u) {
      sprintf(tmpString, "%" PRIu32 ":%02" PRIu32 ":%02" PRIu32 ".%" PRIu32, msec / 3600000u, msec % 3600000u / 60000u, msec % 60000u / 1000u, msec % 1000u / 100u);
    }
    else {
      sprintf(tmpString, "%" PRIu32 ":%02" PRIu32 ":%02" PRIu32 , msec / 3600000u, msec % 3600000u / 60000u, msec % 60000u / 1000u);
    }

    if(rbr1stDigit) {
      sprintf(tmpString + stringByteLength(tmpString), " [%02" PRIu32 "]", (uint32_t)(timerCraAndDeciseconds & 0x7fu));
    }
    else if(aimBuffer[AIM_BUFFER_LENGTH / 2] == 0) {
      sprintf(tmpString + stringByteLength(tmpString), " [" STD_CURSOR STD_SPACE_FIGURE "]");
    }
    else {
      sprintf(tmpString + stringByteLength(tmpString), " [%" PRId32 STD_CURSOR "]", (int32_t)(aimBuffer[AIM_BUFFER_LENGTH / 2] - '0'));
    }
    showString(tmpString, &numericFont, timerTotalTime > 0 ? SCREEN_WIDTH * 45 / 100 : 1, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, false, true);
  }
#endif // TESTSUITE_BUILD
}

void fnUpdateTimerApp(void) {
#ifndef TESTSUITE_BUILD
  if(calcMode == CM_TIMER) {
    fnShowTimerApp();
    displayShiftAndTamBuffer();
    #ifdef DMCP_BUILD
      refreshLcd();
      lcd_refresh();
    #else // !DMCP_BUILD
      refreshLcd(NULL);
    #endif // DMCP_BUILD
  }
#endif // TESTSUITE_BUILD
}

void fnEnterTimerApp(void) {
#ifndef TESTSUITE_BUILD
  if(rbr1stDigit) {
    real_t tmp;
    uInt32ToReal(_getTimerValue() / 100u, &tmp);
    tmp.exponent -= 1;
    reallocateRegister(timerCraAndDeciseconds & 0x7fu, dtTime, REAL34_SIZE, amNone);
    realToReal34(&tmp, REGISTER_REAL34_DATA(timerCraAndDeciseconds & 0x7fu));
    fnUpTimerApp();
  }
  else if(aimBuffer[AIM_BUFFER_LENGTH / 2] == 0) {
    rbr1stDigit = true;
  }
  else {
    timerCraAndDeciseconds = (timerCraAndDeciseconds & 0x80u) + (uint8_t)(aimBuffer[AIM_BUFFER_LENGTH / 2] - '0');
    rbr1stDigit = true;
  }
#endif // TESTSUITE_BUILD
}

void fnDotTimerApp(void) {
#ifndef TESTSUITE_BUILD
  const uint32_t msec = _getTimerValue();
  real_t tmp;

  uInt32ToReal(msec / 100u, &tmp);
  tmp.exponent -= 1;
  reallocateRegister(timerCraAndDeciseconds & 0x7fu, dtTime, REAL34_SIZE, amNone);
  realToReal34(&tmp, REGISTER_REAL34_DATA(timerCraAndDeciseconds & 0x7fu));

  fnUpTimerApp();

  timerValue = 0;
  timerTotalTime += msec;
  if(timerStartTime != TIMER_APP_STOPPED) {
    timerStartTime = _currentTime();
    fnTimerStart(TO_TIMER_APP, TO_TIMER_APP, TIMER_APP_PERIOD);
  }
#endif // TESTSUITE_BUILD
}

void fnPlusTimerApp(void) {
#ifndef TESTSUITE_BUILD
  const uint32_t msec = _getTimerValue();
  real_t tmp;

  uInt32ToReal(msec / 100u, &tmp);
  tmp.exponent -= 1;
  reallocateRegister(timerCraAndDeciseconds & 0x7fu, dtTime, REAL34_SIZE, amNone);
  realToReal34(&tmp, REGISTER_REAL34_DATA(timerCraAndDeciseconds & 0x7fu));
  realDivide(&tmp, const_3600, &tmp, &ctxtReal39);

  fnStatSum(0);
  if(lastErrorCode != ERROR_NONE) {
    liftStack();
    clearRegister(REGISTER_X);
    lastErrorCode = ERROR_NONE;
  }
  real34Add(REGISTER_REAL34_DATA(REGISTER_X), const34_1, REGISTER_REAL34_DATA(REGISTER_X));
  liftStack();
  realToReal34(&tmp, REGISTER_REAL34_DATA(REGISTER_X));
  fnSigma(1);

  fnUpTimerApp();

  timerValue = 0;
  timerTotalTime += msec;
  if(timerStartTime != TIMER_APP_STOPPED) {
    timerStartTime = _currentTime();
    fnTimerStart(TO_TIMER_APP, TO_TIMER_APP, TIMER_APP_PERIOD);
  }

  refreshScreen();
#endif // TESTSUITE_BUILD
}

void fnUpTimerApp(void) {
#ifndef TESTSUITE_BUILD
  if((timerCraAndDeciseconds & 0x7fu) >= 99u) {
    timerCraAndDeciseconds &= 0x80u;
  }
  else {
    ++timerCraAndDeciseconds;
  }
  rbr1stDigit = true;
#endif // TESTSUITE_BUILD
}

void fnDownTimerApp(void) {
#ifndef TESTSUITE_BUILD
  if((timerCraAndDeciseconds & 0x7fu) == 0u) {
    timerCraAndDeciseconds |= 99u;
  }
  else {
    --timerCraAndDeciseconds;
  }
  rbr1stDigit = true;
#endif // TESTSUITE_BUILD
}

void fnDigitKeyTimerApp(uint16_t digit) {
#ifndef TESTSUITE_BUILD
  if(rbr1stDigit || aimBuffer[AIM_BUFFER_LENGTH / 2] == 0) {
    aimBuffer[AIM_BUFFER_LENGTH / 2    ] = digit + '0';
    aimBuffer[AIM_BUFFER_LENGTH / 2 + 1] = 0;
    rbr1stDigit = false;
  }
  else {
    timerCraAndDeciseconds = (timerCraAndDeciseconds & 0x80u) + (uint8_t)(aimBuffer[AIM_BUFFER_LENGTH / 2] - '0') * 10u + digit;
    rbr1stDigit = true;
  }
#endif // TESTSUITE_BUILD
}

void fnRecallTimerApp(uint16_t regist) {
#ifndef TESTSUITE_BUILD
  real_t tmp;
  bool_t overflow;
  uint32_t val;

  switch(getRegisterDataType(regist)) {
    case dtTime:
      real34ToReal(REGISTER_REAL34_DATA(regist), &tmp);
      tmp.exponent += 3;
      realToUInt32(&tmp, DEC_ROUND_DOWN, &val, &overflow);
      break;
    case dtReal34:
      real34ToReal(REGISTER_REAL34_DATA(regist), &tmp);
      realMultiply(&tmp, const_3600, &tmp, &ctxtReal39);
      tmp.exponent += 3;
      realToUInt32(&tmp, DEC_ROUND_HALF_EVEN, &val, &overflow);
      break;
    case dtLongInteger:
      convertLongIntegerRegisterToReal(regist, &tmp, &ctxtReal39);
      realMultiply(&tmp, const_3600, &tmp, &ctxtReal39);
      tmp.exponent += 3;
      realToUInt32(&tmp, DEC_ROUND_HALF_EVEN, &val, &overflow);
      break;
    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "cannot recall %s to the stopwatch", getRegisterDataTypeName(regist, true, false));
        moreInfoOnError("In function fnRecallTimerApp:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
  }

  if(overflow) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "the %s does not fit to uint32_t", getRegisterDataTypeName(regist, true, false));
      moreInfoOnError("In function fnRecallTimerApp:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    timerValue = val;
    if(timerStartTime != TIMER_APP_STOPPED) {
      timerStartTime = _currentTime();
      fnTimerStart(TO_TIMER_APP, TO_TIMER_APP, TIMER_APP_PERIOD);
    }
  }
#endif // TESTSUITE_BUILD
}

void fnBackspaceTimerApp(void) {
#ifndef TESTSUITE_BUILD
  if(rbr1stDigit) {
    fnResetTimerApp(NOPARAM);
  }
  else if(aimBuffer[AIM_BUFFER_LENGTH / 2] == 0) {
    rbr1stDigit = true;
  }
  else {
    aimBuffer[AIM_BUFFER_LENGTH / 2] = 0;
  }
#endif // TESTSUITE_BUILD
}

void fnLeaveTimerApp(void) {
#ifndef TESTSUITE_BUILD
  popSoftmenu();
  rbr1stDigit = true;
  calcMode = previousCalcMode;
  watchIconEnabled = (timerStartTime != TIMER_APP_STOPPED);
#endif // TESTSUITE_BUILD
}

void fnPollTimerApp(void) { // poll every minute not to rewind the timer
#ifndef TESTSUITE_BUILD
  if(calcMode != CM_TIMER && timerStartTime != TIMER_APP_STOPPED) {
    _antirewinder(_currentTime());
  }
#endif // TESTSUITE_BUILD
}
