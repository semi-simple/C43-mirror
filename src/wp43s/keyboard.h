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
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#ifdef PC_BUILD
  #include <gtk/gtk.h>
  #include <gdk/gdk.h>
#endif // PC_BUILD

void resetShiftState (void);  //JM
void leavePem        (void);
void showShiftState  (void);
void processKeyAction(int16_t item);
void fnKeyEnter      (uint16_t unusedButMandatoryParameter);
void fnKeyExit       (uint16_t unusedButMandatoryParameter);
void fnKeyCC         (uint16_t unusedButMandatoryParameter);
void fnKeyBackspace  (uint16_t unusedButMandatoryParameter);
void fnKeyUp         (uint16_t unusedButMandatoryParameter);
void fnKeyDown       (uint16_t unusedButMandatoryParameter);
void fnKeyDotD       (uint16_t unusedButMandatoryParameter);

void executeFunction(const char *data, int16_t item_);

#define ST_0_INIT     0     //STATE 0 INIT             //JM vv FN-DOUBLE
#define ST_1_PRESS1   1     //STATE 1 FIRST PRESS
#define ST_2_REL1     2     //STATE 2 FIRST RELEASE
#define ST_3_PRESS2   3     //STATE 3 SECOND PRESS     //Double click determination 2 to 3 < 75 ms.
#define ST_4_REL2     4     //STATE 4 SECOND RELEASE   //JM ^^


#ifdef PC_BUILD
  void btnFnClicked  (GtkWidget *notUsed, gpointer data);
  void btnFnClickedP (GtkWidget *notUsed, gpointer data);                     //JM
  void btnFnClickedR (GtkWidget *notUsed, gpointer data);                     //JM
  void btnFnPressed  (GtkWidget *notUsed, GdkEvent *event, gpointer data);
  void btnFnReleased (GtkWidget *notUsed, GdkEvent *event, gpointer data);
  void btnClicked    (GtkWidget *notUsed, gpointer data);
  void btnClickedP   (GtkWidget *notUsed, gpointer data);                     //JM
  void btnClickedR   (GtkWidget *notUsed, gpointer data);                     //JM
  void btnPressed    (GtkWidget *notUsed, GdkEvent *event, gpointer data);
  void btnReleased   (GtkWidget *notUsed, GdkEvent *event, gpointer data);
#endif // PC_BUILD

#ifdef DMCP_BUILD
  void btnFnClicked (void *w, void *data);
  void btnFnPressed (void *data);
  void btnFnReleased(void *data);
  void btnClicked   (void *w, void *data);
  void btnPressed   (void *data);
  void btnReleased  (void *data);
#endif // DMCP_BUILD

#endif // KEYBOARD_H
