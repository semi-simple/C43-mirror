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

/* ADDITIONAL C43 functions and routines */

//#define DISPLOADING

#include "wp43s.h"
#include "math.h"

char                 filename_csv[40]; //JMMAX                //JM_CSV Changed from 60 to 40 to save 20 bytes.
uint32_t             mem__32;                                 //JM_CSV


//###################################################################################
#ifdef DMCP_BUILD

TCHAR* f_gets ( //DMCP_BUILD ONLY
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



TCHAR* f_getsline ( //DMCP_BUILD ONLY
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

typedef struct { //DMCP_BUILD ONLY
  FIL *fp;    /* Ptr to the writing file */
  int idx, nchr;  /* Write index of buf[] (-1:error), number of encoding units written */
  BYTE buf[64]; /* Write buffer */
} putbuff;


/* Buffered write with code conversion */
static void putc_bfd (putbuff* pb, TCHAR c) //DMCP_BUILD ONLY
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
static int putc_flush (putbuff* pb) //DMCP_BUILD ONLY
{
  UINT nw;

  if (   pb->idx >= 0 /* Flush buffered characters to the file */
    && f_write(pb->fp, pb->buf, (UINT)pb->idx, &nw) == FR_OK
    && (UINT)pb->idx == nw) return pb->nchr;
  return EOF;
}


/* Initialize write buffer */

/* Fill memory block */
static void mem_set (void* dst, int val, UINT cnt) //DMCP_BUILD ONLY
{
  BYTE *d = (BYTE*)dst;

  do {
    *d++ = (BYTE)val;
  } while (--cnt);
}


static void putc_init (putbuff* pb, FIL* fp) //DMCP_BUILD ONLY
{
  mem_set(pb, 0, sizeof (putbuff));
  pb->fp = fp;
}



int f_putc ( //DMCP_BUILD ONLY
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

int f_puts ( //DMCP_BUILD ONLY
  const TCHAR* str, /* Pointer to the string to be output */
  FIL* fp       /* Pointer to the file object */
)
{
  putbuff pb;


  putc_init(&pb, fp);
  while (*str) putc_bfd(&pb, *str++);   /* Put the string */
  return putc_flush(&pb);
}

#endif
//###################################################################################



#ifdef DMCP_BUILD
int16_t export_append_string_to_file(const char line1[TMP_STR_LENGTH], uint8_t mode, const char filedir[40]) { //DMCP_BUILD 
char line[100];               /* Line buffer */
    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */


    /* Prepare to write */
    sys_disk_write_enable(1);
    fr = sys_is_disk_write_enable();
    if (fr==0) {
      sprintf(line,"Write error ID001--> %d    \n",fr);    print_linestr(line,true);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Opens an existing file. If not exist, creates a new file. */
    if(mode == append) fr = f_open(&fil, filedir, FA_OPEN_APPEND | FA_WRITE); else fr = f_open(&fil, filedir, FA_WRITE|FA_CREATE_ALWAYS);
    if (fr) {
      sprintf(line,"File open error ID002--> %d    \n",fr);       print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Seek to end of the file to append data */
    if(mode == append) {
      fr = f_lseek(&fil, f_size(&fil));
      if (fr) {
        sprintf(line,"Seek error ID003--> %d    \n",fr);            print_linestr(line,false);
        f_close(&fil);
        sys_disk_write_enable(0);
        return (int)fr;
      }
    }

    /* Create string and output */
    fr = f_puts(line1, &fil);
    if (fr == -1) {
      sprintf(line,"Write error ID004--> %d    \n",fr);            print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* close the file */
    fr = f_close(&fil);
    if (fr) {
      sprintf(line,"Close error ID005--> %d    \n",fr);     print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    sys_disk_write_enable(0);
 
    return 0;
  }


int16_t export_string_to_filename(const char line1[TMP_STR_LENGTH], uint8_t mode, char *dirname, char *filename) { //DMCP_BUILD 
char dirfile[40];
    //Create file name
    strcpy(dirfile,dirname);
    strcat(dirfile,"\\");
    strcat(dirfile,filename);
    
    sprintf(tmpString,"%s%s",line1,CSV_NEWLINE);
    check_create_dir(dirname);      
    if(export_append_string_to_file(tmpString, mode, dirfile) != 0) {
      //ERROR ALREADY ANNOUNCED
      return 1;
    }
  return 0;
}



int16_t import_string_from_filename(char *line1,  char *dirname,  char *filename_short,  char *filename,  char *fallback) { //DMCP_BUILD 
    
    #if (VERBOSE_LEVEL >= 2) 
      print_inlinestr("From dir:",false);
      print_inlinestr(dirname,false);
      print_inlinestr(", ",true);
    #endif
  
    #if (VERBOSE_LEVEL >= 2) 
      char line[300];               /* Line buffer */
    #endif
      
    char dirfile[200];
    dirfile[0]=0;
    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */

    //Create dir name
    check_create_dir(dirname);  

    strcpy(dirfile,dirname);
    strcat(dirfile,"\\");
    strcat(dirfile,filename_short);

    #if (VERBOSE_LEVEL >= 1) 
      print_inlinestr("1: reading:",false);
      print_inlinestr(dirfile,false);
      print_inlinestr(" ",true);
    #endif

    /* Opens an existing file. */
    fr = f_open(&fil, dirfile, FA_READ );   //| FA_OPEN_EXISTING
    if (fr != FR_OK) {
      #if (VERBOSE_LEVEL >= 1) 
        print_inlinestr("Not open. ",false);
        #if (VERBOSE_LEVEL >= 2) 
          if(fr == 4) {
            sprintf(line,"Not found ID006 --> %d ",fr); print_inlinestr(line,false);
            sprintf(line,"File: %s \n",dirfile);        print_inlinestr(line,false);
          } else {
            sprintf(line,"File open error --> %d ",fr); print_inlinestr(line,false);
          }
        #endif
      #endif
      #if (VERBOSE_LEVEL >= 2) 
        print_inlinestr(".",true);
      #endif
      f_close(&fil);
      
      if(filename[0]!=0) {
        strcpy(dirfile,dirname);
        strcat(dirfile,"\\");
        strcat(dirfile,filename);
        #if (VERBOSE_LEVEL >= 1) 
          print_inlinestr("2: reading:",false);
          print_inlinestr(dirfile,false);
          print_inlinestr(" ",true);
        #endif

        /* Opens an existing file. */
        fr = f_open(&fil, dirfile, FA_READ );   //| FA_OPEN_EXISTING
        if (fr != FR_OK) {
          #if (VERBOSE_LEVEL >= 1) 
            print_inlinestr("Not open. ",false);
            #if (VERBOSE_LEVEL >= 2) 
              if(fr == 4) {
                sprintf(line,"Not found ID007 --> %d ",fr); print_inlinestr(line,false);
                sprintf(line,"File: %s \n",dirfile);        print_inlinestr(line,false);
              } else {
                sprintf(line,"File open error --> %d ",fr); print_inlinestr(line,false);
              }
            #endif
          #endif
          #if (VERBOSE_LEVEL >= 1) 
            print_inlinestr(". Using fallback.",true);
          #endif
          f_close(&fil);
          strcpy(line1, fallback);
          return 1;
        }
      }
      else {
        #if (VERBOSE_LEVEL >= 1) 
          print_inlinestr("Using fallback.",true);
        #endif
        strcpy(line1, fallback);
        return 1;        
      }
    }

    #if (VERBOSE_LEVEL >= 1)
      print_inlinestr("reading...",false);
    #endif

    /* Read if open */
    line1[0]=0;
    f_getsline(line1, TMP_STR_LENGTH, &fil);
    f_close(&fil);

    #if (VERBOSE_LEVEL >= 1) 
      print_inlinestr("read:",true);
      print_inlinestr(line1,true);
    #endif

    return 0;
  }




void make_TSV_dir_name(void){ //DMCP_BUILD 
    uint32_t tmp__32;                                                 //JM_CSV

    tmp__32 = getUptimeMs();                                      //KEEP PERSISTENT FILE NAME FOR A PERIOD
    if ((mem__32 == 0) || (tmp__32 > mem__32 + 120000) || (stringByteLength(filename_csv) > 10 && !strcompare(filename_csv + (stringByteLength(filename_csv) - 9),".STAT.TSV"  ) ) ) {
      //Create file name
      check_create_dir("DATA");  
      make_date_filename(filename_csv,"DATA\\",".STAT.TSV");
      check_create_dir("DATA");  
    }
    mem__32 = tmp__32;
}


int16_t export_xy_to_file(graphtype x, graphtype y){
  char line[100];               /* Line buffer */
  make_TSV_dir_name();
  sprintf(line,"%.16e%s%.16e%s",x,CSV_TAB,y,CSV_NEWLINE);
  if(export_append_string_to_file(line, append, filename_csv) != 0) {
    //ERROR ALREADY ANNOUNCED
    return 1;
  }
  return 0;
}



int16_t export_append_line(char *inputstring){ //DMCP_BUILD 
char line[100];               /* Line buffer */
    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */

    /* Prepare to write */
    sys_disk_write_enable(1);
    fr = sys_is_disk_write_enable();
    if (fr==0) {
      sprintf(line,"Write error--> %d    \n",fr);     print_linestr(line,true);
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




//**********************************************************************************************************
#elif PC_BUILD
int16_t export_string_to_filename(const char line1[TMP_STR_LENGTH], uint8_t mode, char *dirname, char *filename) { //PC_BUILD

  FILE *outfile;
  char dirfile[40];
  uint16_t fr = 0;
  char line[100];               /* Line buffer */

  strcpy(dirfile,dirname);
  strcat(dirfile,"/");
  strcat(dirfile,filename);
    
  if(mode == append) outfile = fopen(dirfile, "ab"); else outfile = fopen(dirfile, "wb"); 
  if (outfile == NULL) {
    printf("Cannot open ID008: %s %s\n",dirfile,line1);
    return 1;
  }

  sprintf(tmpString,"%s%s",line1,CSV_NEWLINE);
  fr = fputs(tmpString, outfile);
  //printf(">>> %d\n",fr);
  if (fr == 0) {
    sprintf(line,"Write error ID009 --> %d    \n",fr);            
    //print_linestr(line,false);
    printf(line1);
    fclose(outfile);
    return (int)fr;
  } else {
    printf("Exported to %s: %s\n",dirfile,line1);
    fclose(outfile);
  }
  return 0;  
}


int16_t import_string_from_filename(char *line1,  char *dirname,   char *filename_short,  char *filename,  char *fallback) { //PC_BUILD

  #if (VERBOSE_LEVEL >= 2) 
    print_inlinestr("From dir:",false);
    print_inlinestr(dirname,false);
    print_inlinestr(", ",true);
  #endif
  
  char dirfile[200];
  dirfile[0]=0;
  FILE *infile;
  char onechar[2];

  strcpy(dirfile,dirname);
  strcat(dirfile,"/");
  strcat(dirfile,filename_short);

  #if (VERBOSE_LEVEL >= 1)  
    print_inlinestr("1: reading:",false);
    print_inlinestr(dirfile,false);
    print_inlinestr(" ",true);
  #endif

  /* Opens an existing file. */
  infile = fopen(dirfile, "rb");
  if (infile == NULL) {
    #if (VERBOSE_LEVEL >= 1) 
      #ifdef PC_BUILD
        printf("Cannot load ID010 %s\n",dirfile);
      #endif
      print_inlinestr("Not open. ",true);
    #endif

    if(filename[0]!=0) {
      strcpy(dirfile,dirname);
      strcat(dirfile,"/");
      strcat(dirfile,filename);
      #if (VERBOSE_LEVEL >= 1) 
        print_inlinestr("2: reading:",false);
        print_inlinestr(dirfile,false);
        print_inlinestr(" ",true);
      #endif

      /* Opens an existing file. */
      infile = fopen(dirfile, "rb");
      if (infile == NULL) {
        #if (VERBOSE_LEVEL >= 1) 
          #ifdef PC_BUILD
            printf("Cannot load %s\n",dirfile);
          #endif
          print_inlinestr("Fallback.",true);
        #endif
        strcpy(line1, fallback);
        return 1;
      }
    }
    else {
      #if (VERBOSE_LEVEL >= 1) 
        #ifdef PC_BUILD
          printf("Cannot load %s\n",dirfile);
        #endif
        print_inlinestr("Fallback.",true);
      #endif
      strcpy(line1, fallback);
      return 1;
    }
  }

  #if (VERBOSE_LEVEL >= 1) 
    print_inlinestr("reading...",false);
  #endif

  /* Read if open */
  line1[0]=0;
  onechar[1]=0;
  while(fread(&onechar,1,1,infile)) {
    strcat(line1,onechar);
  }
  fclose(infile);
  #if (VERBOSE_LEVEL >= 1) 
    #ifdef PC_BUILD
      printf("Loaded >>> %s\n",dirfile);
    #endif
    print_inlinestr("read:",true);
    print_inlinestr(line1,true);
  #endif

  #if (VERBOSE_LEVEL >= 2) 
    #ifdef PC_BUILD
      printf("Loaded %s |%s|\n",dirfile,line1);
    #endif
  #endif

return 0;
}




int16_t export_append_line(char *inputstring){  //PC_BUILD

  FILE *outfile;

//  strcpy(dirfile,"PROGRAMS/C43_LOG.TXT");

  outfile = fopen(filename_csv, "ab");
  if (outfile == NULL) {
    printf("Cannot open to append ID011: %s %s\n",filename_csv,inputstring);
    return 1;
  }


  #if !defined(__MINGW64__)
    uint16_t fr = 0;
    char line[100];               /* Line buffer */
    fr = fputs(inputstring, outfile);
    if (fr == 0) {
      sprintf(line,"Write error ID012 --> %d %s\n",fr,inputstring);            
      //print_linestr(line,false);
      printf(line);
      fclose(outfile);
      return (int)fr;
    } 
    else
  #endif
  {
    printf("Exported to %s: %s\n",filename_csv,inputstring);
    fclose(outfile);
  }
  return 0;  
}


void make_TSV_dir_name(void){  //PC_BUILD
    uint32_t tmp__32;                                                 //JM_CSV
    time_t rawTime;
    struct tm *timeInfo;
    
    tmp__32 = getUptimeMs();                                          //KEEP PERSISTENT FILE NAME FOR A PERIOD
    if ((mem__32 == 0) || (tmp__32 > mem__32 + 120000)  || (stringByteLength(filename_csv) > 10 && !strcompare(filename_csv + (stringByteLength(filename_csv) - 9),".STAT.TSV"  ) ) ) {
      //Create file name
      time(&rawTime);
      timeInfo = localtime(&rawTime);
      strftime(filename_csv, filenamelen, "%Y%m%d-%H%M%S00.STAT.TSV", timeInfo);
    }
    mem__32 = tmp__32;
}


int16_t export_xy_to_file(graphtype x, graphtype y){ //PC_BUILD
  char line[100];               /* Line buffer */
  make_TSV_dir_name();
  sprintf(line, "%.16e%s%.16e%s",x,CSV_TAB,y,CSV_NEWLINE);
  export_append_line(line);
  return 0;
}


#endif



//################################################################################################




int16_t line_x,line_y;


void print_linestr(const char *line1, bool_t line_init) {
#ifndef TESTSUITE_BUILD
    char l1[100];
    l1[0]=0;
    int16_t ix = 0;
    int16_t ixx;
    ixx = stringByteLength(line1);
    while(ix<ixx && ix<98 && stringWidth(l1, &standardFont, true, true) < SCREEN_WIDTH-12) {
       xcopy(l1, line1, ix+1);
       l1[ix+1]=0;
       ix = stringNextGlyph(line1, ix);
    }
    if(line_init) {line_y = 20;}
    if(line_y < SCREEN_HEIGHT) { 
        line_x = showString(l1, &standardFont, 0, line_y, vmNormal, true, true);
    }
    line_y += 20;
    line_x = 0;
    force_refresh();
#endif
}

void print_numberstr(const char *line1, bool_t line_init) {     //ONLY N=ASCII NUMBERS AND E AND . //FIXED FONT
#ifndef TESTSUITE_BUILD
    if(line_init) {line_y = 20;}
    if(line_y < SCREEN_HEIGHT) { 
        int16_t cnt = 0;
        char tt[2];
        while(line1[cnt] != 0 && line_x < SCREEN_WIDTH-8 +1) {
          tt[0]=line1[cnt]; tt[1]=0;
          line_x = showString(tt, &standardFont, cnt * 8, line_y, vmNormal, true, true);
          cnt++;
        }
    }
    line_y += 20;
    line_x = 0;
    force_refresh();
#endif
}

