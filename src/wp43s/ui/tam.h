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
 * \file tam.h
 ***********************************************/

#ifndef TESTSUITE_BUILD
  /********************************************//**
   * \brief Sets the calc mode to TAM buffer mode
   *
   * \return void
   ***********************************************/
  void tamEnterMode    (int16_t func);

  /********************************************//**
   * \brief Leaves the TAM buffer
   *
   * \return void
   ***********************************************/
  void tamLeaveMode    (void);

  /********************************************//**
   * \brief Processes input for the TAM buffer
   *
   * At the moment this still assumes that AIM input
   * is handled by addItemToBuffer rather than here
   * except for backspaces
   *
   * \param[in] uint16_t item the ITM value to process
   * \return void
   ***********************************************/
  void tamProcessInput (uint16_t item);
#endif // TESTSUITE_BUILD
