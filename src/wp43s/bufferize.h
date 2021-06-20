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

/**
 * \file bufferize.h
 ***********************************************/
#ifndef BUFFERIZE_H
#define BUFFERIZE_H

#include "typeDefinitions.h"
#include <stdint.h>
#include "realType.h"

#ifndef TESTSUITE_BUILD
  void    fnAim                    (uint16_t unusedButMandatoryParameter);
  void    resetAlphaSelectionBuffer(void);
  bool_t  timeoutAlphaSelectionBuffer(void);           //JM
  void    startAlphaSelectionBuffer  (void);           //JM
  void    light_ASB_icon(void);                        //JM
  void    kill_ASB_icon(void);                         //JM

  void    addItemToBuffer          (uint16_t item);
  void    addItemToNimBuffer       (int16_t item);
  void    closeNimWithFraction     (real34_t *dest);
  void    closeNimWithComplex      (real34_t *dest_r, real34_t *dest_i);
  void    closeNim                 (void);
  void    closeAim                 (void);
  void    nimBufferToDisplayBuffer (const char *buffer, char *displayBuffer);
#endif // TESTSUITE_BUILD

#endif // BUFFERIZE_H
