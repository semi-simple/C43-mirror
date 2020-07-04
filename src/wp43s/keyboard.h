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

void showShiftState  (void);
void processKeyAction(int16_t item);
void fnKeyEnter      (uint16_t unusedParamButMandatory);
void fnKeyExit       (uint16_t unusedParamButMandatory);
void fnKeyCC         (uint16_t unusedParamButMandatory);
void fnKeyBackspace  (uint16_t unusedParamButMandatory);
void fnKeyUp         (uint16_t unusedParamButMandatory);
void fnKeyDown       (uint16_t unusedParamButMandatory);
void fnKeyDotD       (uint16_t unusedParamButMandatory);

#ifdef PC_BUILD
  void btnFnClicked  (GtkWidget *w, gpointer data);
  void btnFnPressed  (GtkWidget *w, GdkEvent *event, gpointer data);
  void btnFnReleased (GtkWidget *w, GdkEvent *event, gpointer data);
  void btnClicked    (GtkWidget *w, gpointer data);
  void btnPressed    (GtkWidget *w, GdkEvent *event, gpointer data);
  void btnReleased   (GtkWidget *w, GdkEvent *event, gpointer data);
#endif

#ifdef DMCP_BUILD
  void btnFnPressed (void *data);
  void btnFnReleased(void *data);
  void btnClicked   (void *w, void *data);
  void btnPressed   (void *data);
  void btnReleased  (void *data);
#endif // DMCP_BUILD
