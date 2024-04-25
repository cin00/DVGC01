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
   int i = 0;
   int ch;

   while ((ch = getchar()) != EOF) {
      buffer[i++] = ch;
   }
   //buffer[i++] = '\n';
   buffer[i++] = '$';
   }     

/**********************************************************************/
/* Display the buffer                                                 */
/**********************************************************************/  

static void pbuffer()
{   
   printf("--------------------------------------------------------\n");
   printf(" THE PROGRAM TEXT \n");
   printf("--------------------------------------------------------\n");
   printf("\n%s", buffer);
   printf("\n------------------------------------------------------\n");
   }

/**********************************************************************/
/* Copy a character from the program buffer to the lexeme buffer      */
/**********************************************************************/

static void get_char()
{   
   lexbuf[plex++] = buffer[pbuf++];
}

/**********************************************************************/
/* End of buffer handling functions                                   */
/**********************************************************************/

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

/**********************************************************************/
/* Public wrapper funktioner för lite testning                        */
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
   
   if(pbuf == 0) {
      get_prog();
      pbuffer();
      if(strlen(buffer) == 2) {
         return -1;
      }
   }

   memset(lexbuf, 0, LEXSIZE);
   plex = 0;

   /* Ignorera white space */
   while (isspace(buffer[pbuf])) {
      pbuf++;
   }

   /* Kopiera första char från buffer */
   get_char();

   /* If alfanumerisk */
   if(isalpha(lexbuf[0])) {
      while(!ispunct(buffer[pbuf]) && !isspace(buffer[pbuf])) {
         get_char();
      }
      return key2tok(lexbuf);
   }

   /* If siffra */
   if(isdigit(lexbuf[0])) {
      while(!ispunct(buffer[pbuf]) && !isalpha(buffer[pbuf]) && !isspace(buffer[pbuf])) {
         get_char();
      }
      return number;
   }

   /* If varken siffra eller alfanum */
   if((lexbuf[0] == ':') && (buffer[pbuf] == '=')) {
      get_char();
   }
   return lex2tok(lexbuf);
   }

/**********************************************************************/
/* Return a lexeme                                                    */
/**********************************************************************/
char * get_lexeme()
{ 
   return lexbuf;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

