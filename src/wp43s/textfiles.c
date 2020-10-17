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

#include "wp43s.h"
#include "math.h"
#include "graphText.h"

void stackregister_csv_out(int16_t reg_b, int16_t reg_e) {
#ifndef TESTSUITE_BUILD
  char csv[TMP_STR_LENGTH];
  char csvp[TMP_STR_LENGTH];
  csvp[0] = 0;

  int16_t ix;
  ix = reg_b;
  while (ix <= reg_e) {
    if((ix>=100)&&(ix<=111)) {
       switch (ix)
       {
        case 100:   strcpy(csv, "X");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_X, csvp); break;
        case 101:   strcpy(csv, "Y");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_Y, csvp); break;
        case 102:   strcpy(csv, "Z");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_Z, csvp); break;
        case 103:   strcpy(csv, "T");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_T, csvp); break;
        case 104:   strcpy(csv, "A");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_A, csvp); break;
        case 105:   strcpy(csv, "B");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_B, csvp); break;
        case 106:   strcpy(csv, "C");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_C, csvp); break;
        case 107:   strcpy(csv, "D");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_D, csvp); break;
        case 108:   strcpy(csv, "L");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_L, csvp); break;
        case 109:   strcpy(csv, "I");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_I, csvp); break;
        case 110:   strcpy(csv, "J");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_J, csvp); break;
        case 111:   strcpy(csv, "K");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_K, csvp); break;
        default:break;
        }
      strcat(csv, csvp);
      strcat(csv, CSV_NEWLINE);
      } else
    if((ix>=0)&&(ix<=99)) {
      sprintf(csv, "%sR%02d%s%s", CSV_STR, ix, CSV_STR, CSV_TAB);
      copyRegisterToClipboardString(ix, csvp);
      strcat(csv, csvp);
      strcat(csv, CSV_NEWLINE);
    } else
    if((ix>=112)&&(ix<=112+100)) {
      sprintf(csv, "%sU%02d%s%s", CSV_STR, ix-100, CSV_STR, CSV_TAB);
      copyRegisterToClipboardString(ix, csvp);
      strcat(csv, csvp);
      strcat(csv, CSV_NEWLINE);
    }

    export_append_line(csv);                    //Output append to CSV file

    ++ix;
  }

#endif
}


void fnP_All_Regs(uint16_t unusedParamButMandatory){
  #if defined (DMCP_BUILD)
  check_create_dir("DATA");  
  make_date_filename(filename_csv,"DATA\\",".REGS.TSV");
  #endif


  switch (unusedParamButMandatory)
  {
  case 0:           //All registers
    {
       stackregister_csv_out(REGISTER_X,REGISTER_D);
       stackregister_csv_out(REGISTER_D,REGISTER_K);
       stackregister_csv_out(0,99);
       //stackregister_csv_out(FIRST_LOCAL_REGISTER,FIRST_LOCAL_REGISTER+100);
    }
    break;
  case 1:           //Stack only
    {
       stackregister_csv_out(REGISTER_X,REGISTER_D);
    }
    break;
  case 2:           //Global Registers
    {
       stackregister_csv_out(0,99);
    }
    break;
  case 3:           //USER Registers
    {
       stackregister_csv_out(FIRST_LOCAL_REGISTER,FIRST_LOCAL_REGISTER+100);
    }
    break;
  default:
    break;
  }
}





//**********************************************************************************************************

#ifndef TESTSUITE_BUILD
int16_t export_string_to_file(const char line1[TMP_STR_LENGTH]) {
  return export_string_to_filename(line1, append, "PROGRAMS", "C43_LOG.TXT");
}
#endif


//################################################################################################

void displaywords(char *line1) {  //Preprocessor and display
  char bb[2];
  char aa[2];
  bool_t state_comments=false;
  aa[1]=0;
  bb[1]=0;
  bb[0]=0;
  if(verbose_jm >=2) {print_linestr("Code:",true);}
  //printf("4:%s\n",line1);

  if(line1[stringByteLength(line1)-1] != 32) {
    strcat(line1," ");
  }

  tmpStr3000[0]=32;tmpStr3000[1]=0;

  int16_t ix = 0;
  while (line1[ix] != 0) {
    aa[0]=line1[ix];
    bb[0]=line1[ix+1];

    if ((aa[0]==47 && bb[0]==47) && state_comments == false) {         //start comment on double slash
      state_comments=true;
      ix++; //skip the second slash
    } 
    else 
    if (state_comments && (aa[0]==13 || aa[0]==10))  {    //stop comment mode at end of line
      state_comments=false;
      strcat(tmpStr3000," ");
    } 
    else
      if(!state_comments) {                //proceed only if not comment mode
        switch(aa[0]) {
          case 32:                         //remove all whitespace
          case 8:
          case 13:
          case 10:
          case 44: if(strlen(tmpStr3000)!=0) {
                     if(tmpStr3000[strlen(tmpStr3000)-1] != 32) {
                       strcat(tmpStr3000," ");
                     }
                   }
                   break;
          default: strcat(tmpStr3000,aa);
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
  while (tmpStr3000[ix] != 0) {
    aa[0]=tmpStr3000[ix];    
    if (tmpStr3000[ix-1] != 32) {
      strcat(line1,aa);
      #ifdef DISPLOADING
      strcat(ll,aa);
      if(strlen(ll)>30 && aa[0] == 32) {
        if(verbose_jm>=2) {print_linestr(ll,false);}
        ll[0]=0;
      }
      #endif
    } else {
      if(aa[0] != 32) {
        strcat(line1,aa);
        #ifdef DISPLOADING
        strcat(ll,aa);          
        if(strlen(ll)>36) {
         if(verbose_jm>=2){print_linestr(ll,false);}
          ll[0]=0;
        }
        #endif
      }
    }
    ix++;
  }
  #ifdef DISPLOADING
  if(ll[0]!=0) {
    if(verbose_jm>=2) {print_linestr(ll,false);}
  }
  #endif
//printf("6:%s\n",line1);


}






int16_t line_x,line_y;

void print_inlinestr(const char *line1, bool_t endline) {
#ifndef TESTSUITE_BUILD
    char l1[100];    //Clip the string at 40
    l1[0]=0;
    int16_t ix = 0;
    int16_t ixx;
    ixx = stringByteLength(line1);
    while(ix<ixx && ix<98 && line_x+stringWidth(l1, &standardFont, true, true) < SCREEN_WIDTH-12) {
       xcopy(l1, line1, ix+1);
       l1[ix+1]=0;
       ix = stringNextGlyph(line1, ix);
    }
    if(line_y < SCREEN_HEIGHT) { 
        line_x = showString(l1, &standardFont, line_x, line_y, vmNormal, true, true);
    }
    if(endline) {
      line_y += 20;
      line_x = 0;
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
