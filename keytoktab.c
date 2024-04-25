/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
/**********************************************************************/
/* type definitions                                                   */
/**********************************************************************/
typedef struct tab {
	char 	* text;
	int 	token;
	} tab;
	
/**********************************************************************/
/* data objects (tables)                                              */
/**********************************************************************/
static tab tokentab[ ] = {
        {"id", 	        id},
	{"number", 	number},
	{":=", 	        assign},
	{"undef", 	undef},
	{"predef", 	predef},
	{"tempty", 	tempty},
	{"error",        error},
	{"type",         typ},
	{"$",            '$'},
	{"(",            '('},
	{")",            ')'},
	{"*",            '*'},
	{"+",            '+'},
	{",",            ','},
	{"-",            '-'},
	{".",            '.'},
	{"/",            '/'},
	{":",            ':'},
	{";",            ';'},
	{"=",            '='},
	{"TERROR", 	nfound}
        };


static tab keywordtab[ ] = {
	{"program", 	program},
	{"input", 	input},
	{"output", 	output},
	{"var", 	var},
	{"begin", 	begin},
	{"end", 	end},
	{"boolean", 	boolean},
	{"integer", 	integer},
	{"real", 	real},
	{"KERROR", 	nfound}
	} ;
   
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Display the tables                                                 */
/**********************************************************************/
void p_toktab()
{  
   //printf("\n *** TO BE DONE");
   int i;
   printf("\nToken table:\n");
   printf("Text\tToken\n");
   printf("--------------------------\n");
   for(i = 0; tokentab[i].token != nfound; i++) {
		printf("%s\t%d\n", tokentab[i].text, tokentab[i].token);
	}
   printf("Keyword\tToken\n");
   printf("--------------------------\n");
   for(i = 0; (keywordtab[i].token != nfound); i++) {
		printf("%s\t%d\n", keywordtab[i].text, keywordtab[i].token);
	}
}
/**********************************************************************/
/* lex2tok - convert a lexeme to a token                              */
/**********************************************************************/
toktyp lex2tok(char * fplex)
{
   int i;
   for (i = 0; tokentab[i].token != nfound; i++) {
	if(strcmp(fplex, tokentab[i].text) == 0) {
		return tokentab[i].token;
	}
   }
   for(i = 0; keywordtab[i].token != nfound; i++) {
	if(strcmp(fplex, keywordtab[i].text) == 0) {
		return keywordtab[i].token;
	}
   }
   return nfound;
}

/**********************************************************************/
/* key2tok - convert a keyword to a token                             */
/**********************************************************************/
toktyp key2tok(char * fplex)
{
	int i;
	for(i = 0; keywordtab[i].token != nfound; i++) {
		if(strcmp(fplex, keywordtab[i].text) == 0) {
			return keywordtab[i].token;
		}
	}
	return nfound;
}

/**********************************************************************/
/* tok2lex - convert a token to a lexeme                              */
/**********************************************************************/
char * tok2lex(toktyp ftok)
{
	int i;
	for(i = 0; tokentab[i].token != nfound; i++) {
		if(ftok == tokentab[i].token) {
			return tokentab[i].text;
		}
	}
	for(i = 0; keywordtab[i].token != nfound; i++) {
		if(ftok == keywordtab[i].token) {
			return keywordtab[i].text;
		}
	}
	return "KERROR";
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

