/* This file is part of WP 43S.
 *
 * WP 43S is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WP 43S is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WP 43S.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ADDITIONAL C43 functions and routines */

/********************************************//**
 * \file arctan.h
 ***********************************************/

bool_t strcompare( char *in1, char *in2);
void fnXEQMENU(uint16_t unusedParamButMandatory);
void testprogram(uint16_t unusedParamButMandatory);
void capture_sequence(char *origin, uint16_t item);
void XEQMENU_Selection(uint16_t selection, char *line1, bool_t exec);
void runkey(uint16_t item);

void fnXEQMSAVE (uint16_t unusedParamButMandatory);
void fnXEQMLOAD (uint16_t unusedParamButMandatory);
void fnXEQMEDIT (uint16_t unusedParamButMandatory);
void fnXEQMXXEQ (uint16_t unusedParamButMandatory);
