/**********************************************************************/
/* constants & public functions                                       */
/**********************************************************************/

#ifndef LEX_H
#define LEX_H

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int    get_token();                   /* return a token               */
char * get_lexeme();                  /* return a lexeme              */

void lexer_init();
void print_buffer();

#endif

/**********************************************************************/
/* end of definitions                                                 */
/**********************************************************************/
