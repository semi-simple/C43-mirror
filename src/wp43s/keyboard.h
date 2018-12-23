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

#ifdef PC_BUILD
  void btnFnClicked   (GtkWidget *w, gpointer data);
  void btnClicked     (GtkWidget *w, gpointer data);
#endif

#ifdef DMCP_BUILD
  void btnFnClicked   (void *w, void *data);
  void btnClicked     (void *w, void *data);
#endif // DMCP_BUILD
