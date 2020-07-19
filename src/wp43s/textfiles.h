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


/********************************************//** //JM
 * \file jmgraph.c TEXTFILES module
 ***********************************************/

/* ADDITIONAL WP43C functions and routines */

#define      filenamelen 40
char         filename_csv[filenamelen]; //JMMAX                //JM_CSV
uint32_t     mem__32;                                 //JM_CSV

#define      CSV_NEWLINE "\n"
#define      CSV_TAB     "\t"
#define      CSV_CR      "\r"
#define      CSV_SLASH   "\\"
#define      CSV_STR     "\""
 
void         fnP_All_Regs(uint16_t unusedParamButMandatory);
void         print_inlinestr(const char line1[TMP_STR_LENGTH], bool_t endline);
void         print_linestr(const char line1[TMP_STR_LENGTH], bool_t line_init);
void         print_numberstr(const char line1[TMP_STR_LENGTH], bool_t line_init);

void         displaywords(char *line1);
int16_t      import_string_from_filename(char *line1, char *filename, char *fallback);
int16_t      export_append_line(char *inputstring);

int16_t      export_string_to_file(const char line1[TMP_STR_LENGTH]);
int16_t      export_xy_to_file(double x, double y);


int16_t T_cursorPos;
#define T_CR            "\x0D"
void fnT_ARROW(uint16_t command);
