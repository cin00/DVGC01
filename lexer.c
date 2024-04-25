/**********************************************************************/
/* lab 1 DVG C01 - Lexer OBJECT                                       */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define BUFSIZE 1024
#define LEXSIZE   30
static char buffer[BUFSIZE];
static char lexbuf[LEXSIZE];
static int  pbuf  = 0;               /* current index program buffer  */
static int  plex  = 0;               /* current index lexeme  buffer  */

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/* buffer functions                                                   */
/**********************************************************************/
/**********************************************************************/
/* Read the input file into the buffer                                */
/**********************************************************************/

static void get_prog()
{
   //printf("\n *** TO BE DONE");
   FILE *fp;
   int ch;
   
   fp = fopen("testok1.pas", "r");

   if (fp == NULL) {
      perror("Error opening file");
      return;
   }

   pbuf = 0;
   while ((ch = fgetc(fp)) != EOF) {
      buffer[pbuf++] = ch;
   }
   buffer[pbuf++] = '$';
   buffer[++pbuf] = '\0';
   fclose(fp);

   }     

/**********************************************************************/
/* Display the buffer                                                 */
/**********************************************************************/  

static void pbuffer()
{   
   //printf("\n *** TO BE DONE");
   printf("--------------------------------------------------------\n");
   printf(" THE PROGRAM TEXT \n");
   printf("--------------------------------------------------------\n");
   int i = 0;
   for(i = 0; i < BUFSIZE - 1; i++) {
      printf("%c", buffer[i]);
   }
   printf("\n--------------------------------------------------------\n");
   }

/**********************************************************************/
/* Copy a character from the program buffer to the lexeme buffer      */
/**********************************************************************/

static void get_char()
{   
   if (buffer[pbuf] < BUFSIZE - 1) {
      lexbuf[plex++] = buffer[pbuf++];

      //lexbuf[plex] = '\0';
   }
}

/**********************************************************************/
/* End of buffer handling functions                                   */
/**********************************************************************/

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

/**********************************************************************/
/* Public wrapper functions                                              */
/**********************************************************************/
void lexer_init() {
   get_prog();
}

void print_buffer() {
   pbuffer();
}

/**********************************************************************/
/* Return a token                                                     */
/**********************************************************************/
int get_token()
{  
   plex = 0;
   while (isspace(buffer[pbuf])) {
      pbuf++;
      if(buffer[pbuf] == '\0') return nfound;
   }
   
   if (buffer[pbuf] == '$') {
      return '$';

   } else if (isalpha(buffer[pbuf])) {
      do {
         get_char();
      } while (isalnum(buffer[pbuf]));
      toktyp result = key2tok(lexbuf);
      return result != nfound ? result : nfound;

   } else if (isdigit(buffer[pbuf])) {
         do {
            get_char();
         } while (isdigit(buffer[pbuf]));
         return number;

   } else if (buffer[pbuf] != '\0') {
         get_char();
         toktyp result = lex2tok(lexbuf);
         return result != nfound ? result : nfound;
      } else {
         //exit(-1);
         printf("Unhandled character '%d' at position %d\n", buffer[pbuf], pbuf);
         pbuf++; 
         return nfound;
      }
   }

/**********************************************************************/
/* Return a lexeme                                                    */
/**********************************************************************/
char * get_lexeme()
{  
   //printf("\n *** TO BE DONE");  return "$";
   if (buffer[pbuf] == '$') {
      return "$";
   }
   return lexbuf;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

