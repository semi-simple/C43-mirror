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
 * \file keyboard.h
 ***********************************************/

void resetShiftState(void);
void showShiftState (void);
void fnComplexCCCC  (uint16_t unusedParamButMandatory);


int16_t nameFunction(int16_t fn, int16_t itemShift);   //JM LONGPRESS FN

int8_t TC_compare(uint32_t timecheck);                 //JM vv FN-DOUBLE 
#define TC_Expired     1    //Timecheck Time expired
#define TC_Not_expired -1   //Timecheck Time not yet expired
#define TC_Equals      0    //Timecheck Time expired
#define TC_NA          127  //Timecheck Time ZERO not available

#define ST_0_INIT     0     //STATE 0 INIT
#define ST_1_PRESS1   1     //STATE 1 FIRST PRESS
#define ST_2_REL1     2     //STATE 2 FIRST RELEASE
#define ST_3_PRESS2   3     //STATE 3 SECOND PRESS     //Double click determination 2 to 3 < 75 ms.
#define ST_4_REL2     4     //STATE 4 SECOND RELEASE
#define ST_5_EXEC     5     //STATE FOR EXECUTING
uint8_t FN_state; // = ST_0_INIT;                      //JM ^^

void disp__(uint8_t nr, int32_t dud);                                    //DISPLAY time on DM42 screen



#ifdef PC_BUILD
  void btnFnClicked   (GtkWidget *w, gpointer data);
  void btnFnPressed   (GtkWidget *w, gpointer data);   //JM LONGPRESS FN
  void btnFnReleased  (GtkWidget *w, gpointer data);   //JM LONGPRESS FN
  void btnClicked     (GtkWidget *w, gpointer data);
  void btnPressed     (GtkWidget *w, gpointer data);
  void btnReleased    (GtkWidget *w, gpointer data);
#endif

#ifdef DMCP_BUILD
  void btnFnClicked   (void *w, void *data);
  void btnFnPressed   (void *w, void *data);   //JM LONGPRESS FN
  void btnFnReleased   (void *w, void *data);  //JM LONGPRESS FN
  void btnClicked     (void *w, void *data);
  void btnPressed     (void *w, void *data);
  void btnReleased    (void *w, void *data);
#endif // DMCP_BUILD
