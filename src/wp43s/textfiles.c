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


#include "wp43s.h"
#include "math.h"

char                 filename_csv[40]; //JMMAX                //JM_CSV Changed from 60 to 40 to save 20 bytes.
uint32_t             mem__32;                                 //JM_CSV


//#####################################################################################



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

    test_line(csv);                    //Output append to CSV file

    ++ix;
  }

#endif
}



void fnP_All_Regs(uint16_t unusedParamButMandatory){
  #if defined (DMCP_BUILD)
  make_date_filename(filename_csv,"/SCREENS/",".TSV");
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


//###################################################################################
//###################################################################################
//###################################################################################

#ifdef DMCP_BUILD

TCHAR* f_gets (
  TCHAR* buff,  /* Pointer to the buffer to store read string */
  int len,    /* Size of string buffer (items) */
  FIL* fp     /* Pointer to the file object */
)
{
  int nc = 0;
  TCHAR *p = buff;
  BYTE s[4];
  UINT rc;
  DWORD dc;

      /* Byte-by-byte read without any conversion (ANSI/OEM API) */
  len -= 1; /* Make a room for the terminator */
  while (nc < len) {
    f_read(fp, s, 1, &rc);  /* Get a byte */
    if (rc != 1) break;   /* EOF? */
    dc = s[0];
    if (dc == '\r') continue;
    *p++ = (TCHAR)dc; nc++;
    if (dc == '\n') break;
  }

  *p = 0;   /* Terminate the string */
  return nc ? buff : 0; /* When no data read due to EOF or error, return with error. */
}



TCHAR* f_getsline (
  TCHAR* buff,  /* Pointer to the buffer to store read string */
  int len,    /* Size of string buffer (items) */
  FIL* fp     /* Pointer to the file object */
)
{
  int nc = 0;
  TCHAR *p = buff;
  BYTE s[2];
  UINT rc;
  BYTE sc;
  s[1]=0;
      /* Byte-by-byte read without any conversion (ANSI/OEM API) */
  len -= 1; /* Make a room for the terminator */
  while (nc < len) {
    f_read(fp, s, 1, &rc);  /* Get a byte */
    if (rc != 1) break;   /* EOF? */
    sc = s[0];
    *p++ = (TCHAR)sc; nc++;
  }

  *p = 0;   /* Terminate the string */
  return nc ? buff : 0; /* When no data read due to EOF or error, return with error. */
}


/*-----------------------------------------------------------------------*/
/* Put a Character to the File (sub-functions)                           */
/*-----------------------------------------------------------------------*/

/* Putchar output buffer and work area */

typedef struct {
  FIL *fp;    /* Ptr to the writing file */
  int idx, nchr;  /* Write index of buf[] (-1:error), number of encoding units written */
  BYTE buf[64]; /* Write buffer */
} putbuff;


/* Buffered write with code conversion */
static void putc_bfd (putbuff* pb, TCHAR c)
{
  UINT n;
  int i, nc;


  if (c == '\n') {    /* LF -> CRLF conversion */
    putc_bfd(pb, '\r');
  }

  i = pb->idx;        /* Write index of pb->buf[] */
  if (i < 0) return;
  nc = pb->nchr;      /* Write unit counter */


  /* ANSI/OEM input (without re-encoding) */
  pb->buf[i++] = (BYTE)c;


  if (i >= (int)(sizeof pb->buf) - 4) { /* Write buffered characters to the file */
    f_write(pb->fp, pb->buf, (UINT)i, &n);
    i = (n == (UINT)i) ? 0 : -1;
  }
  pb->idx = i;
  pb->nchr = nc + 1;
}


/* Flush remaining characters in the buffer */
static int putc_flush (putbuff* pb)
{
  UINT nw;

  if (   pb->idx >= 0 /* Flush buffered characters to the file */
    && f_write(pb->fp, pb->buf, (UINT)pb->idx, &nw) == FR_OK
    && (UINT)pb->idx == nw) return pb->nchr;
  return EOF;
}


/* Initialize write buffer */

/* Fill memory block */
static void mem_set (void* dst, int val, UINT cnt)
{
  BYTE *d = (BYTE*)dst;

  do {
    *d++ = (BYTE)val;
  } while (--cnt);
}


static void putc_init (putbuff* pb, FIL* fp)
{
  mem_set(pb, 0, sizeof (putbuff));
  pb->fp = fp;
}



int f_putc (
  TCHAR c,  /* A character to be output */
  FIL* fp   /* Pointer to the file object */
)
{
  putbuff pb;


  putc_init(&pb, fp);
  putc_bfd(&pb, c); /* Put the character */
  return putc_flush(&pb);
}




/*-----------------------------------------------------------------------*/
/* Put a String to the File                                              */
/*-----------------------------------------------------------------------*/

int f_puts (
  const TCHAR* str, /* Pointer to the string to be output */
  FIL* fp       /* Pointer to the file object */
)
{
  putbuff pb;


  putc_init(&pb, fp);
  while (*str) putc_bfd(&pb, *str++);   /* Put the string */
  return putc_flush(&pb);
}

/*-----------------------------------------------------------------------*/
//###################################################################################
//###################################################################################






//#####################################################################################
//#####################################################################################



int16_t export_string_to_file(const char line1[TMP_STR_LENGTH]) {
char line[100];               /* Line buffer */
    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */

    //Create file name
    strcpy(filename_csv,"/PROGRAMS/XEQDEMO1.TSV");      

    /* Prepare to write */
    sys_disk_write_enable(1);
    fr = sys_is_disk_write_enable();
    if (fr==0) {
      sprintf(line,"Write access error--> %d    \n",fr);    print_linestr(line,true);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(&fil, filename_csv, FA_OPEN_APPEND | FA_WRITE);
    if (fr) {
      sprintf(line,"File open error--> %d    \n",fr);       print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Seek to end of the file to append data */
    fr = f_lseek(&fil, f_size(&fil));
    if (fr) {
      sprintf(line,"Seek error--> %d    \n",fr);            print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Create string and output */
    sprintf(tmpStr3000,"%s%s",line1,CSV_NEWLINE);
    fr = f_puts(tmpStr3000, &fil);
    if (fr == -1) {
      sprintf(line,"Write error--> %d    \n",fr);            print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* close the file */
    fr = f_close(&fil);
    if (fr) {
      sprintf(line,"File close error--> %d    \n",fr);     print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    sys_disk_write_enable(0);
 
    return 0;
  }







int16_t import_string_from_file(char *line1) {
char line[100];               /* Line buffer */

    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */

    //Create file name
    strcpy(filename_csv,"/PROGRAMS/XEQDEMO2.TSV");      


    /* Opens an existing file. */
    fr = f_open(&fil, filename_csv, FA_READ | FA_OPEN_EXISTING);
    if (fr) {
      sprintf(line,"File read open error--> %d    \n",fr);       print_linestr(line,false);
      f_close(&fil)
      ;
      return (int)fr;
    }

    /* Read if open */
    line1[0]=0;
    f_getsline(line1, TMP_STR_LENGTH, &fil);
//       strcpy(line,"|");
  //     strncat(line,line1,20);
    //   strcat(line,"|");
      // print_linestr(line,false); 
    f_close(&fil);
    return 0;
  }





int16_t import_string_from_filename(char *line1, const char *filename) {
char line[TMP_STR_LENGTH];        /* Line buffer */

    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */

    //Create file name
    strcpy(filename_csv,"/PROGRAMS/");
    strcat(filename_csv,filename);
    strcat(filename_csv,".TSV");

    /* Opens an existing file. */
    fr = f_open(&fil, filename_csv, FA_READ | FA_OPEN_EXISTING);
    if (fr) {
      sprintf(line,"File read open error--> %d    \n",fr);       print_linestr(line,false);
      f_close(&fil)
      ;
      return (int)fr;
    }

    /* Read if open */
    line1[0]=0;
    f_getsline(line1, TMP_STR_LENGTH, &fil);
    f_close(&fil);
    return 0;
  }





//#####################################################################################
//#####################################################################################






/*-----------------------------------------------------------------------*/


int16_t testjm(void){
char line[100];               /* Line buffer */

    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(&fil, "message.txt", FA_READ | FA_OPEN_ALWAYS);
    if (fr) return (int)fr;
    sprintf(line,"Opened file read--> %d    \n",fr);        print_linestr(line,true);
    while (f_gets(line, sizeof line, &fil)) {               print_linestr(line,false); }
    f_close(&fil);


    /* Prepare to write */
    sys_disk_write_enable(1);
    fr = sys_is_disk_write_enable();
    sprintf(line,"Write enable?--> %d    \n",fr);           print_linestr(line,false);

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(&fil, "message.csv", FA_OPEN_APPEND | FA_WRITE);
    if (fr) {
      sprintf(line,"Open append error--> %d    \n",fr);     print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    sprintf(line,"Opened file append--> %d    \n",fr);      print_linestr(line,false);

    /* Seek to end of the file to append data */
    fr = f_lseek(&fil, f_size(&fil));
    if (fr) {
      sprintf(line,"Seek error--> %d    \n",fr);            print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }



//    TCHAR aa;
//    aa = 48;
//    fr = f_putc(aa,&fil);
//    sprintf(line,"puts--> %d    \n",fr);                    print_linestr(line,false);

    fr = f_puts("ABCDEF\n",&fil);
    sprintf(line,"puts--> %d    \n",fr);                    print_linestr(line,false);

    fr = f_close(&fil);
    sys_disk_write_enable(0);
 
    return 0;
  }




int16_t test_xy(float x, float y){
char line[100];               /* Line buffer */
uint32_t             tmp__32;                                 //JM_CSV

    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */

    tmp__32 = getUptimeMs();
    if ((mem__32 == 0) || (tmp__32 > mem__32 + 120000)) {
      //Create file name
      make_date_filename(filename_csv,"/SCREENS/",".TSV");
      //filename_csv[19+3]=0;                                     //20200331-180STATS
      strcat(filename_csv,"STATS.TSV");      
    }
    mem__32 = tmp__32;



    /* Prepare to write */
    sys_disk_write_enable(1);
    fr = sys_is_disk_write_enable();
    if (fr==0) {
      sprintf(line,"Write access error--> %d    \n",fr);    print_linestr(line,true);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(&fil, filename_csv, FA_OPEN_APPEND | FA_WRITE);
    if (fr) {
      sprintf(line,"File open error--> %d    \n",fr);       print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Seek to end of the file to append data */
    fr = f_lseek(&fil, f_size(&fil));
    if (fr) {
      sprintf(line,"Seek error--> %d    \n",fr);            print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

//uses tmpstr3000
    //sprintf(line,"%f, %f\n",x,y);                           print_linestr(line,false);    
    sprintf(tmpStr3000,"%f%s%f%s",x,CSV_TAB,y,CSV_NEWLINE);
    fr = f_puts(tmpStr3000, &fil);
    if (fr == -1) {
      sprintf(line,"Write error--> %d    \n",fr);            print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* close the file */
    fr = f_close(&fil);
    if (fr) {
      sprintf(line,"File close error--> %d    \n",fr);     print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    sys_disk_write_enable(0);
 
    return 0;
  }



int16_t test_line(char *inputstring){
char line[100];               /* Line buffer */
    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */

    /* Prepare to write */
    sys_disk_write_enable(1);
    fr = sys_is_disk_write_enable();
    if (fr==0) {
      sprintf(line,"Write access error--> %d    \n",fr);     print_linestr(line,true);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }


    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(&fil, filename_csv, FA_OPEN_APPEND | FA_WRITE);
    if (fr) {
      sprintf(line,"File open error--> %d    \n",fr);       print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Seek to end of the file to append data */
    fr = f_lseek(&fil, f_size(&fil));
    if (fr) {
      sprintf(line,"Seek error--> %d    \n",fr);            print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    fr = f_puts(inputstring, &fil);


    /* close the file */
    fr = f_close(&fil);
    if (fr) {
      sprintf(line,"File close error--> %d    \n",fr);      print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    sys_disk_write_enable(0);
 
    return 0;
  }





#elif PC_BUILD

int16_t import_string_from_filename(char *line1,  char *filename) {

  if (strcompare(filename,"XEQM01")) strcpy(line1,"XEQLBL 01 TEST01 \"3\" ENTER \"4\" CHS Y^X "); else
  if (strcompare(filename,"XEQM02")) strcpy(line1,"XEQLBL 02 TEST02 \"1\" ENTER \"81\" /   "); else
  if (strcompare(filename,"XEQM03")) strcpy(line1,"XEQLBL 03 MP2203 \"2\" ENTER \"2203\" Y^X \"1\" - ENTER PRIME? "); else
  if (strcompare(filename,"XEQM04")) strcpy(line1,"XEQLBL 04 MP2281 \"2\" ENTER \"2281\" Y^X \"1\" - ENTER PRIME? "); else
  if (strcompare(filename,"XEQM05")) strcpy(line1,"XEQLBL 05 MP3217 \"2\" ENTER \"3217\" Y^X \"1\" - ENTER PRIME? "); else
  if (strcompare(filename,"XEQM06")) strcpy(line1,"XEQLBL 06 CUBE   RCL \"11\" \"3\" Y^X RCL \"12\" \"3\" Y^X RCL \"13\" \"3\" Y^X + + "); else
  if (strcompare(filename,"XEQM07")) strcpy(line1,"XEQLBL 07 LBL07 "); else
  if (strcompare(filename,"XEQM08")) strcpy(line1,"XEQLBL 08 LBL08 "); else
  if (strcompare(filename,"XEQM09")) strcpy(line1,"XEQLBL 09 LBL09 "); else
  if (strcompare(filename,"XEQM10")) strcpy(line1,"XEQLBL 10 LBL10 "); else
  if (strcompare(filename,"XEQM11")) strcpy(line1,"XEQLBL 11 LBL11 "); else
  if (strcompare(filename,"XEQM12")) strcpy(line1,"XEQLBL 12 LBL12 "); else
  if (strcompare(filename,"XEQM13")) strcpy(line1,"XEQLBL 13 LBL13 "); else
  if (strcompare(filename,"XEQM14")) strcpy(line1,"XEQLBL 14 LBL14 "); else
  if (strcompare(filename,"XEQM15")) strcpy(line1,"XEQLBL 15 LBL15 "); else
  if (strcompare(filename,"XEQM16")) strcpy(line1,"XEQLBL 16 LBL16 "); else
  if (strcompare(filename,"XEQM17")) strcpy(line1,"XEQLBL 17 LBL17 "); else
  if (strcompare(filename,"XEQM18")) strcpy(line1,"XEQLBL 18 LBL18 ");

return 0;
}



int16_t import_string_from_file(char *line1) {
  printf("import_string_from_file not implemented in sim: %s\n",line1);
  return 0;  
}

int16_t export_string_to_file(const char line1[TMP_STR_LENGTH]) {
  printf("export_string_to_file not implemented in sim: %s\n",line1);
  return 0;  
}


int16_t test_line(char *inputstring){
  printf("test_line not implemented in sim: %s\n",inputstring);
  return 0;
}

int16_t test_xy(float x, float y){
  printf("test_xy not implemented in sim: %f%s%f%s",x,CSV_TAB,y,CSV_NEWLINE);
  return 0;
}

#endif





void displaywords(char *line1) {  //Preprocessor and display
  char ll[50];
  char bb[2];
  char aa[2];
  bool_t state_comments=false;
  aa[1]=0;
  bb[1]=0;
  bb[0]=0;
  print_linestr("Code:",true);
  //printf("4:%s\n",line1);

  tmpStr3000[0]=32;tmpStr3000[1]=0;
  ll[0]=0;
  int16_t ix = 0;
  while (line1[ix] != 0) {
    aa[0]=line1[ix];
    bb[0]=line1[ix+1];

    if ((aa[0]==47 && bb[0]==47)) {         //start and stop comment mode on double slash
      state_comments=!state_comments;
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

  aa[0]=0; aa[1]=0;                  //remove consequtive spaces
  ll[0]=0;
  ix = 1;
  line1[0]=0;
  while (tmpStr3000[ix] != 0) {
    aa[0]=tmpStr3000[ix];    
    if (tmpStr3000[ix-1] != 32) {
      strcat(line1,aa);
      strcat(ll,aa);
      if(strlen(ll)>30 && aa[0] == 32) {print_linestr(ll,false);ll[0]=0;}
    } else {
      if(aa[0] != 32) {
      strcat(line1,aa);
      strcat(ll,aa);          
      if(strlen(ll)>36) {print_linestr(ll,false);ll[0]=0;}
      }
    }
    ix++;
  }
  if(ll[0]!=0) {print_linestr(ll,false);}
//printf("6:%s\n",line1);


}








int16_t line_y;

//void print_line(bool_t line_init) {
//#ifndef TESTSUITE_BUILD
//char line[100];               /* Line buffer */
//    if(line_init) {line_y = 20;}
//    showString(line, &standardFont, 1, line_y, vmNormal, true, true);
//    line_y += 20;
//#endif
//}


void print_linestr(const char line1[TMP_STR_LENGTH], bool_t line_init) {
#ifndef TESTSUITE_BUILD
    if(line_init) {line_y = 20;}
    if(line_y < SCREEN_HEIGHT) { 
        showString(line1, &standardFont, 1, line_y, vmNormal, true, true);
    }
    line_y += 20;
    force_refresh();
#endif
}




