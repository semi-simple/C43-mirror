/* This file is part of 43C.
 *
 * 43C is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 43C is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 43C.  If not, see <http://www.gnu.org/licenses/>.
 */

/********************************************//**
 * \file keyboardTweak.c
 ***********************************************/

#include "wp43s.h"


kb_timer_t timer[TMR_NUMBER];


#ifdef PC_BUILD
/********************************************//**
 * \brief Refreshes timer. This function is
 * called every 10 ms by a GTK timer.
 *
 * \param[in] data gpointer Not used
 * \return gboolean         What will happen next?
 *                          * true  = timer will call this function again
 *                          * false = timer stops calling this function
 ***********************************************/
gboolean refreshTimer(gpointer data) {  // This function is called every 10 ms by a GTK timer
  uint32_t now = getUptimeMs();
  
  for(int i=0; i<TMR_NUMBER; i++) {
    if(timer[i].state == TMR_RUNNING) {
      if(timer[i].timer_will_expire <= now)
      {
        timer[i].state = TMR_COMPLETED;
        timer[i].func(timer[i].param);
      }
    }
  }

  return TRUE;
}
#endif

#ifdef DMCP_BUILD
void refreshTimer(void) {               // This function is called when nextTimerRefresh has been elapsed
  uint32_t now = getUptimeMs();
  
  for(int i=0; i<TMR_NUMBER; i++) {
    if(timer[i].state == TMR_RUNNING) {
      if(timer[i].timer_will_expire <= now)
      {
        timer[i].state = TMR_COMPLETED;
        timer[i].func(timer[i].param);  // Callback to configured function
      }
    }
  }
}
#endif


void fnTimerDummyTest(uint16_t unusedParamButMandatory) {
#if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
  printf("fnTimerDummyTest called  %u\n", unusedParamButMandatory);
#endif
}



void fnRebuildTimerRefresh(void) {
#ifdef DMCP_BUILD
  bool_t hasRunning = false;
  uint32_t next;
  
  for(int i=0; i<TMR_NUMBER; i++) {
    if(timer[i].state == TMR_RUNNING) {
      hasRunning = true;
      next = timer[i].timer_will_expire;
      if(nextTimerRefresh != 0 && next < nextTimerRefresh) {
        nextTimerRefresh = next;
      }
      if(nextTimerRefresh == 0) {
        nextTimerRefresh = next;
      }
    }
  }
  
  if(!hasRunning) {
    nextTimerRefresh = 0;
  }
#endif
}



void fnTimerReset(void) {
  for(int i=0; i<TMR_NUMBER; i++) {
    timer[i].state = TMR_UNUSED;
  }
  fnRebuildTimerRefresh();
}



void fnTimerConfig(uint8_t nr, void(*func)(uint16_t), uint16_t param/*, uint32_t time*/) {
  if(nr < TMR_NUMBER) {
    timer[nr].func  = func;
    timer[nr].param = param;
  //timer[nr].time  = time;
    timer[nr].state = TMR_STOPPED;
  }
  fnRebuildTimerRefresh();
}



void fnTimerStart(uint8_t nr, uint16_t param, uint32_t time) {
#ifndef TESTSUITE_BUILD
  uint32_t now = getUptimeMs();
#else
  uint32_t now = 0;
#endif
  if(nr < TMR_NUMBER) {
    timer[nr].param = param;
  //timer[nr].time = time;
    timer[nr].timer_will_expire = now + time;
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


void fnTimerDel(uint8_t nr) {
  if(nr < TMR_NUMBER) {
    timer[nr].state = TMR_UNUSED;
  }
  fnRebuildTimerRefresh();
}
