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
 * \file graphText.c TEXTFILES module
 ***********************************************/

/* ADDITIONAL WP43C functions and routines */


#define      CSV_NEWLINE "\n"
#define      CSV_TAB     "\t"
#define      CSV_CR      "\r"
#define      CSV_SLASH   "\\"
#define      CSV_STR     "\""
#define      T_CR        "\x0D"
#define      append  0
#define      overwrite 1

void         create_filename(char *fn);

int16_t      export_append_line(char *inputstring);
int16_t      export_string_to_filename(const char line1[TMP_STR_LENGTH], uint8_t mode, char *dirname, char *filename);

#define      filenamelen 40
extern char         filename_csv[filenamelen]; //JMMAX                //JM_CSV
extern uint32_t     mem__32;                                          //JM_CSV
extern bool_t       cancelFilename;
void         print_numberstr(const char *line1, bool_t line_init);
void         print_linestr  (const char *line1, bool_t line_init);
int16_t      export_xy_to_file(graphtype x, graphtype y);
int16_t      import_string_from_filename(char *line1,  char *dirname,   char *filename_short,  char *filename,  char *fallback, bool_t scanning);

