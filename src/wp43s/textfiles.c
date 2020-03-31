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
 * along with 43S.  If not, see <htempp://www.gnu.org/licenses/>.
 */

/********************************************//** //JM
 * \file jmgraph.c Graphing module
 ***********************************************/

/* ADDITIONAL WP43C functions and routines */


#include "wp43s.h"
#include "math.h"



char filename_csv[30];


void stackregister_csv_out(int16_t reg_b, int16_t reg_e) {
#ifndef TESTSUITE_BUILD
  char csv[3000];
  char csvp[3000];
  csvp[0] = 0;

  int16_t ix;
  ix = reg_b;
  while (ix <= reg_e) {
    if((ix>=100)&&(ix<=111)) {
       switch (ix)
       {
        case 100:   strcpy(csv, "X,");copyRegisterToClipboardString(REGISTER_X, csvp); break;
        case 101:   strcpy(csv, "Y,");copyRegisterToClipboardString(REGISTER_Y, csvp); break;
        case 102:   strcpy(csv, "Z,");copyRegisterToClipboardString(REGISTER_Z, csvp); break;
        case 103:   strcpy(csv, "T,");copyRegisterToClipboardString(REGISTER_T, csvp); break;
        case 104:   strcpy(csv, "A,");copyRegisterToClipboardString(REGISTER_A, csvp); break;
        case 105:   strcpy(csv, "B,");copyRegisterToClipboardString(REGISTER_B, csvp); break;
        case 106:   strcpy(csv, "C,");copyRegisterToClipboardString(REGISTER_C, csvp); break;
        case 107:   strcpy(csv, "D,");copyRegisterToClipboardString(REGISTER_D, csvp); break;
        case 108:   strcpy(csv, "L,");copyRegisterToClipboardString(REGISTER_L, csvp); break;
        case 109:   strcpy(csv, "I,");copyRegisterToClipboardString(REGISTER_I, csvp); break;
        case 110:   strcpy(csv, "J,");copyRegisterToClipboardString(REGISTER_J, csvp); break;
        case 111:   strcpy(csv, "K,");copyRegisterToClipboardString(REGISTER_K, csvp); break;
        default:break;
        }
      strcat(csv, csvp);
      strcat(csv, "\n");
      }
    if((ix>=0)&&(ix<=99)) {
      sprintf(csv, "R%02d,", ix);
      copyRegisterToClipboardString(ix, csvp);
      strcat(csv, csvp);
      strcat(csv, "\n");
    }

    #ifdef DMCP_BUILD
    strcpy(tmpStr3000, csv);
    test_line();                    //Output append to CSV file
    #elif PC_BUILD
    printf("%s\n",csv);
    #endif

    ++ix;
  }

#endif
}



void fnP_All_Regs(uint16_t unusedParamButMandatory){
  #if defined (DMCP_BUILD)
  make_date_filename(filename_csv,"/SCREENS/",".CSV");

  switch (unusedParamButMandatory)
  {
  case 0:           //All registers
    {
       stackregister_csv_out(REGISTER_X,REGISTER_D);
       stackregister_csv_out(REGISTER_D,REGISTER_K);
       stackregister_csv_out(0,99);
       //register_csv_out(FIRST_LOCAL_REGISTER,FIRST_LOCAL_REGISTER+100);
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
       //register_csv_out(FIRST_LOCAL_REGISTER,FIRST_LOCAL_REGISTER+100);
    }
    break;
  default:
    break;
  }
  #endif
}




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

int16_t line_y;
char line[100];               /* Line buffer */
void print_line(bool_t line_init) {

    if(line_init) {line_y = 20;}
    showString(line, &standardFont, 1, line_y, vmNormal, true, true);
    line_y += 20;
}


/*-----------------------------------------------------------------------*/


int16_t testjm(void){
    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(&fil, "message.txt", FA_READ | FA_OPEN_ALWAYS);
    if (fr) return (int)fr;
    sprintf(line,"Opened file read--> %d    \n",fr);        print_line(true);
    while (f_gets(line, sizeof line, &fil)) {               print_line(false); }
    f_close(&fil);


    /* Prepare to write */
    sys_disk_write_enable(1);
    fr = sys_is_disk_write_enable();
    sprintf(line,"Write enable?--> %d    \n",fr);           print_line(false);

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(&fil, "message.csv", FA_OPEN_APPEND | FA_WRITE);
    if (fr) {
      sprintf(line,"Open append error--> %d    \n",fr);     print_line(false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    sprintf(line,"Opened file append--> %d    \n",fr);      print_line(false);

    /* Seek to end of the file to append data */
    fr = f_lseek(&fil, f_size(&fil));
    if (fr) {
      sprintf(line,"Seek error--> %d    \n",fr);            print_line(false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }



//    TCHAR aa;
//    aa = 48;
//    fr = f_putc(aa,&fil);
//    sprintf(line,"puts--> %d    \n",fr);                    print_line(false);

    fr = f_puts("ABCDEF\n",&fil);
    sprintf(line,"puts--> %d    \n",fr);                    print_line(false);

    fr = f_close(&fil);
    sys_disk_write_enable(0);
 
    return 0;
  }



int16_t test_xy(float x, float y){
    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */

    //Create file name
    make_date_filename(filename_csv,"/SCREENS/",".CSV");
    filename_csv[19]=0;
    strcat(filename_csv,"STATS.CSV");

    /* Prepare to write */
    sys_disk_write_enable(1);
    fr = sys_is_disk_write_enable();
    if (fr==0) {
      sprintf(line,"Write access error--> %d    \n",fr);    print_line(true);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(&fil, filename_csv, FA_OPEN_APPEND | FA_WRITE);
    if (fr) {
      sprintf(line,"File open error--> %d    \n",fr);       print_line(false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Seek to end of the file to append data */
    fr = f_lseek(&fil, f_size(&fil));
    if (fr) {
      sprintf(line,"Seek error--> %d    \n",fr);            print_line(false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

//uses tmpstr3000
    //sprintf(line,"%f, %f\n",x,y);                           print_line(false);    
    sprintf(tmpStr3000,"%f, %f\n",x,y);
    fr = f_puts(tmpStr3000, &fil);


    /* close the file */
    fr = f_close(&fil);
    if (fr) {
      sprintf(line,"File close error--> %d    \n",fr);     print_line(false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    sys_disk_write_enable(0);
 
    return 0;
  }



int16_t test_line(void){          //uses  tmpStr3000;
    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */

    /* Prepare to write */
    sys_disk_write_enable(1);
    fr = sys_is_disk_write_enable();
    if (fr==0) {
      sprintf(line,"Write access error--> %d    \n",fr);     print_line(true);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }


    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(&fil, filename_csv, FA_OPEN_APPEND | FA_WRITE);
    if (fr) {
      sprintf(line,"File open error--> %d    \n",fr);       print_line(false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Seek to end of the file to append data */
    fr = f_lseek(&fil, f_size(&fil));
    if (fr) {
      sprintf(line,"Seek error--> %d    \n",fr);            print_line(false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

//uses tmpstr3000
    fr = f_puts(tmpStr3000, &fil);


    /* close the file */
    fr = f_close(&fil);
    if (fr) {
      sprintf(line,"File close error--> %d    \n",fr);      print_line(false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    sys_disk_write_enable(0);
 
    return 0;
  }





#elif PC_BUILD

int16_t test_line(void){          //uses  tmpStr3000;
  return 0;
}

int16_t test_xy(float x, float y){
  return 0;
}

#endif


