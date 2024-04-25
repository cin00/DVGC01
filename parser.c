/**********************************************************************/
/* lab 1 DVG C01 - Happy Hacker version  Donald Ross  060422          */
/**********************************************************************/
/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Andra objekt                                                       */
/**********************************************************************/
#include "keytoktab.h"
#include "lexer.h"
#include "symtab.h"


/**********************************************************************/
/* Attribut                                                           */
/**********************************************************************/
#define DEBUG 1
int  lookahead=0;
int  is_parse_ok=1;

/**********************************************************************/
/* Privata metoder för detta objekt                                   */
/**********************************************************************/
static void expr();
static void term();
static void factor();
static void operand();

/* PROGRAM_HEADER */
static void program_header();

/* VAR_PART */
static void type();
static void id_list();
static void var_dec();
static void var_dec_list();
static void var_part();

/* STAT_PART */
static void operand();
static void factor();
static void term();
static void expr();
static void assign_stat();
static void stat();
static void stat_list();
static void stat_part();

/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
void match(int t)
{
   if(DEBUG) printf("\n --------In match expected: %s, found: %s", 
                     tok2lex(t), get_lexeme(t));
   if (lookahead == t) lookahead = get_token();
   else {
      is_parse_ok=0;
      printf("\n *** Unexpected Token: expected: %3d found: %3d",
              t, lookahead);
      }
   }
/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
static void type() {
   if (DEBUG) printf("\n *** In  type");
   if (lookahead == integer) {
      match(integer);
   } else if (lookahead == boolean) {
      match(boolean);
   } else if(lookahead == real) {
      match(real);
   }
   if (DEBUG) printf("\n *** Out  type");
}


static void id_list() {
   if (DEBUG) printf("\n *** In  id_list");
   if(lookahead == id)  match(id); else return; //Error handling TODO
   if (lookahead == ',') {
      match(',');
      id_list();
   }
   if (DEBUG) printf("\n *** Out  id_list");
}

static void var_dec() {
   if (DEBUG) printf("\n *** In  var_dec");
   id_list();
   if(lookahead == ':') match(':'); else return; //Error handling TODO
   type();
   if(lookahead == ';') match(';'); else return; //Error handling TODO
   if (DEBUG) printf("\n *** Out  var_dec");
   }

static void var_dec_list() {
   if (DEBUG) printf("\n *** In  var_dec_list");
   var_dec();
   if(lookahead == id) {
      var_dec_list();
   }
}

static void var_part() {
   if(DEBUG) printf("\n*** In var_part");
   if(lookahead == var) match(var); else return; //Error handling TODO
   var_dec_list();
   if(DEBUG) printf("\n*** Out var_part");
}

static void operand() {
   if (lookahead == id) {
      match(id);
   }
   else if(lookahead == number) {
      match(number);
   }
}

static void factor() {
   if(lookahead == '(') {
      match('(');
      expr();
      match(')');
   }
   operand();
}

static void term() {
   factor();
   if(lookahead == '*') {
      match('*');
      factor();
      term();
   }
}

static void expr() {
   if (DEBUG) printf("\n *** In  expr");
   term();
   if(lookahead == '+') {
      match('+');
      term();
      expr();
   }
   if (DEBUG) printf("\n *** Out  expr");
}

static void assign_stat() {
   if (DEBUG) printf("\n *** In  assign_stat");
   if(lookahead == id) match(id); else return; //Error handling TODO
   if(lookahead == assign) match(assign); else return; //Error handling TODO
   expr();
   if (DEBUG) printf("\n *** Out  assign_stat");
}

static void stat() {
   if (DEBUG) printf("\n *** In  stat");
   assign_stat();
   if (DEBUG) printf("\n *** Out stat");
}

static void stat_list() {
   if (DEBUG) printf("\n *** In stat_list");
   stat();
   if(lookahead == ';') {
      match(';');
      stat_list();
   }
   if (DEBUG) printf("\n *** Out stat_list");

}

static void stat_part() {
   if (DEBUG) printf("\n *** In stat_part");
   if (lookahead == begin)  match(begin); else return; //Error handling TODO
   stat_list();
   if (lookahead == end) match(end); else return; //Error handling TODO
   if(lookahead == '.')  match('.'); else return; //Error handling TODO
   if (DEBUG) printf("\n *** Out stat_part");
}

static void program_header()
{
   if (DEBUG) printf("\n *** In  program_header");
   if (lookahead == program) match(program); else return; //error handling TODO

   if (lookahead == '(') match('('); else return; //Error handling TODO
   if (lookahead == input) match(input); else return; //Error handling TODO
   if (lookahead == ',') match(','); else return; //Error handling TODO
   if (lookahead == output) match(output); else return; //Error handling TODO
   if (lookahead == ')') match(')'); else return; //Error handling TODO
   if (lookahead == ';') match(';'); else return; //Error handling TODO
   if (DEBUG) printf("\n *** Out  program_header");
      
   }
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
int parser()
{
   if (DEBUG) printf("\n *** In  parser");
   lookahead = get_token();        // get the first token
   program_header();               // call the first grammar rule
   var_part();
   stat_part();
   return is_parse_ok;             // status indicator
   }
/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
