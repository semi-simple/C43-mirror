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
 * \file jmgraph.c Graphing module
 ***********************************************/

/* ADDITIONAL C43 functions and routines */

//#define DISPLOADING

#include "c43Extensions/textfiles.h"

#include "charString.h"
#include "c43Extensions/graphText.h"
#include "screen.h"
#include <string.h>

#include "wp43s.h"

  uint8_t reg_Name(uint16_t no) {
    switch(no) {
      case 100:   return 'X'; break;
      case 101:   return 'Y'; break;
      case 102:   return 'Z'; break;
      case 103:   return 'T'; break;
      case 104:   return 'A'; break;
      case 105:   return 'B'; break;
      case 106:   return 'C'; break;
      case 107:   return 'D'; break;
      case 108:   return 'L'; break;
      case 109:   return 'I'; break;
      case 110:   return 'J'; break;
      case 111:   return 'K'; break;
      default:return 0;break;
    }
  }

void stackregister_csv_out(int16_t reg_b, int16_t reg_e) {
#ifndef TESTSUITE_BUILD

  char csv[TMP_STR_LENGTH];
  char tmp[TMP_STR_LENGTH];
  tmp[0] = 0;

  int16_t ix;
  ix = reg_b;
  while (ix <= reg_e) {
    if((ix>=100)&&(ix<=111)) {
      csv[1]=0;
      csv[0]=reg_Name(ix);
      strcat(csv, CSV_TAB);
      #if (VERBOSE_LEVEL >= 1) 
        print_linestr("-2b",false);
      #endif
      copyRegisterToClipboardString(ix, tmp);
      #if (VERBOSE_LEVEL >= 1) 
        print_linestr("-2c",false);
      #endif
      strcat(csv, tmp);
      strcat(csv, CSV_NEWLINE);
      } else
    if((ix>=0)&&(ix<=99)) {
      sprintf(csv, "%sR%02d%s%s", CSV_STR, ix, CSV_STR, CSV_TAB);
      copyRegisterToClipboardString(ix, tmp);
      strcat(csv, tmp);
      strcat(csv, CSV_NEWLINE);
    } else
    if((ix>=112)&&(ix<=112+100)) {
      sprintf(csv, "%sU%02d%s%s", CSV_STR, ix-100, CSV_STR, CSV_TAB);
      copyRegisterToClipboardString(ix, tmp);
      strcat(csv, tmp);
      strcat(csv, CSV_NEWLINE);
    }

    export_append_line(csv);                    //Output append to CSV file

    #if (VERBOSE_LEVEL >= 1) 
      strcat(csv,"------------------>");
      print_linestr(csv,false);
    #endif

    ++ix;
  }

#endif
}




//**********************************************************************************************************

#ifndef TESTSUITE_BUILD
int16_t export_string_to_file(const char line1[TMP_STR_LENGTH]) {
  return export_string_to_filename(line1, APPEND, "PROGRAMS", "C43_LOG.TXT");
}
#endif


//################################################################################################


// OUTPUT is in TMPSTR

void displaywords(char *line1) {  //Preprocessor and display
  char bb[2];
  char aa[2];
  bool_t state_comments=false;
  aa[1]=0;
  bb[1]=0;
  bb[0]=0;

  #if (VERBOSE_LEVEL >= 2) 
    print_linestr("Code:",true);
  #endif
  //printf("4:%s\n",line1);

  #if (VERBOSE_LEVEL >= 2) 
   char tmp[400];          //Messages
   sprintf(tmp, " F: Displaywords: %lu bytes.\n",stringByteLength(line1) );
   print_linestr(tmp,false);
   print_linestr(line1,false);
  #endif



  if(line1[stringByteLength(line1)-1] != 32) {
    strcat(line1," ");
  }

  tmpString[0]=32;tmpString[1]=0;

  int16_t ix = 0;
  while (line1[ix] != 0) {
    aa[0]=line1[ix];
    bb[0]=line1[ix+1];

    if ((aa[0]=='/' && bb[0]=='/') && state_comments == false) {         //start comment on double slash
      state_comments=true;
      ix++; //skip the second slash
    } 
    else 
    if (state_comments && (aa[0]==13 || aa[0]==10))  {    //stop comment mode at end of line
      state_comments=false;
      strcat(tmpString," ");
    } 
    else
      if(!state_comments) {                //proceed only if not comment mode
        switch(aa[0]) {
          case ' ':                         //remove all whitespace
          case 8:
          case 13:
          case 10:
          case ',': if(strlen(tmpString)!=0) {
                     if(tmpString[strlen(tmpString)-1] != ' ') {
                       strcat(tmpString," ");
                     }
                   }
                   break;
          default: strcat(tmpString,aa);
        }    
      }

    ix++;
  }
  //printf("5:%s\n",line1);

  #ifdef DISPLOADING
  char ll[50];
  ll[0]=0;
  #endif
  aa[0]=0; aa[1]=0;                  //remove consequtive spaces
  ix = 1;
  line1[0]=0;
  while (tmpString[ix] != 0) {
    aa[0]=tmpString[ix];    
    if (tmpString[ix-1] != ' ') {
      strcat(line1,aa);
      #ifdef DISPLOADING
      strcat(ll,aa);
      if(strlen(ll)>30 && aa[0] == ' ') {
        #if (VERBOSE_LEVEL >= 2) 
          print_linestr(ll,false);
        #endif
        ll[0]=0;
      }
      #endif
    } else {
      if(aa[0] != ' ') {
        strcat(line1,aa);
        #ifdef DISPLOADING
        strcat(ll,aa);          
        if(strlen(ll)>36) {
         #if (VERBOSE_LEVEL >= 2) 
           print_linestr(ll,false);
         #endif
          ll[0]=0;
        }
        #endif
      }
    }
    ix++;
  }
  #ifdef DISPLOADING
  if(ll[0]!=0) {
    #if (VERBOSE_LEVEL >= 2) 
      print_linestr(ll,false);
    #endif
  }
  #endif
//printf("6:%s\n",line1);


}






uint32_t t_line_x, t_line_y;

void print_inlinestr(const char *line1, bool_t endline) {
#ifndef TESTSUITE_BUILD
    char l1[100];    //Clip the string at 40
    l1[0]=0;
    int16_t ix = 0;
    int16_t ixx;
    ixx = stringByteLength(line1);
    while(ix<ixx && ix<98 && t_line_x + stringWidth(l1, &standardFont, true, true) < SCREEN_WIDTH-12) {
       xcopy(l1, line1, ix+1);
       l1[ix+1]=0;
       ix = stringNextGlyph(line1, ix);
    }
    if(t_line_y < SCREEN_HEIGHT) { 
        t_line_x = showString(l1, &standardFont, t_line_x, t_line_y, vmNormal, true, true);
    }
    if(endline) {
      t_line_y += 20;
      t_line_x = 0;
    }
    force_refresh();
#endif
}



void print_Register_line(calcRegister_t regist, const char *before, const char *after, bool_t line_init) {
#ifndef TESTSUITE_BUILD
    char str[200];
    char dest[1000];

    strcpy(dest,before);
    copyRegisterToClipboardString(regist, str);
    strcat(dest,str);
    strcat(dest,after);

    print_numberstr(dest, line_init);

#endif
}
