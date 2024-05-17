/*  HENRIK HULTGREN  */

/**********************************************************************/
/* lab 1 DVG C01 - Happy Hacker version  Donald Ross  060422          */
/**********************************************************************/
/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "keytoktab.h"
#include "lexer.h"
#include "symtab.h"
#include "optab.h"

#define DEBUG 0
int  lookahead=0;
int  is_parse_ok=1;

/* PROGRAM_HEADER */
static void program_header();

/* VAR_PART */
static void type();
static void id_list();
static void var_dec();
static void var_dec_list();
static void var_part();

/* STAT_PART */
static toktyp operand();
static toktyp factor();
static toktyp term();
static toktyp expr();
static void assign_stat();
static void stat();
static void stat_list();
static void stat_part();

/* ERROR HANDLING */
static void symbol_error(char* exp) {
   printf("\nSYNTAX: Symbol expected %s found %s", exp, get_lexeme());
   is_parse_ok = 0;
}
static void id_error() {
   printf("\nSYNTAX: ID expected found %s", get_lexeme());
   is_parse_ok = 0; 
   }
static void id_declared_error() {
   printf("\nSEMANTIC: ID %s not declared", get_lexeme());
   is_parse_ok = 0; }
static void assign_error(char *leftside, char *rightside) {
   printf("\nSEMANTIC: Can't assign types %s := %s", leftside, rightside);
   is_parse_ok = 0;
}
static void dup_error() {
   printf("\nSEMANTIC: ID %s already declared", get_lexeme());
   is_parse_ok = 0;
}
static void operand_error() {
   printf("\nSYNTAX: Missing operand");
   is_parse_ok = 0; 
}
static void type_error() {
   printf("\nSYNTAX: Expected type found %s", get_lexeme());
   is_parse_ok = 0;
   }


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
      if(t == id) {
         id_error();
      } else {
         printf("\n *** Unexpected Token: expected: %3s found: %3s",
         tok2lex(t), get_lexeme());
      }
   }
}
/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
static void type() {
   if (DEBUG) printf("\n *** In  type");
   if (lookahead == integer) {
      setv_type(integer);
      match(integer);
   } else if (lookahead == boolean) {
      setv_type(boolean);
      match(boolean);
   } else if(lookahead == real) {
      setv_type(real);
      match(real);
   } else {
      type_error();
      setv_type(error);
   }
   if (DEBUG) printf("\n *** Out  type");
}


static void id_list() {
   if (DEBUG) printf("\n *** In  id_list");
   if(lookahead == id)  {
      if(find_name(get_lexeme())) {
         dup_error();
      } else {
         addv_name(get_lexeme());
      }
      match(id);
   }
   if (lookahead == ',') {
      match(',');
      id_list();
   }
   if (DEBUG) printf("\n *** Out  id_list");
}

static void var_dec() {
   if (DEBUG) printf("\n *** In  var_dec");
   id_list();
   if(lookahead == ':') match(':'); else symbol_error(":");
   type();
   if(lookahead == ';') match(';'); else symbol_error(";");
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
   if(lookahead == var) match(var); else symbol_error("var");
   var_dec_list();
   if(DEBUG) printf("\n*** Out var_part");
}

static toktyp operand() {
   if (DEBUG) printf("\n *** In  operand");
   toktyp type;

   if (lookahead == id) {
      if(!find_name(get_lexeme())) {
         id_declared_error();
      }
      type = get_ntype(get_lexeme());
      match(id);
      return type;
      } 
      
   if(lookahead == number) {
      type = integer;
      match(number);
      return type;
   }
   operand_error();
   return error;
}

static toktyp factor() {
   if (DEBUG) printf("\n *** In  factor");
   toktyp type;
   if(lookahead == '(') {
      match('(');
      type = expr();
      match(')');
   } else {
      type = operand();
   }
   if (DEBUG) printf("\n *** Out  factor");
   return type;
}

static toktyp term() {
   if (DEBUG) printf("\n *** In  term");
   toktyp type;
   type = factor();
   if(lookahead == '*') {
      match('*');
      type = get_otype('*', type, term());
   }
   if (DEBUG) printf("\n *** Out  term");
   return type;
}

static toktyp expr() {
   if (DEBUG) printf("\n *** In  expr");
   toktyp type;
   type = term();
   if(lookahead == '+') {
      match('+');
      type = get_otype('+', type, expr());
   }
   if (DEBUG) printf("\n *** Out  expr");
   return type;
}

static void assign_stat() {
   if (DEBUG) printf("\n *** In  assign_stat");
   toktyp leftSide = get_ntype(get_lexeme());
   toktyp rightSide = nfound;
   if(lookahead == id) {
      if(!find_name(get_lexeme())) {
         id_declared_error();
      }
      match(id);
   } else
      id_error();

   if(lookahead == assign) {
      match(assign);
   } else {
      symbol_error(":=");
   }
   rightSide = expr();
   
   if(leftSide != rightSide) {
      assign_error(tok2lex(leftSide), tok2lex(rightSide));
   }
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
   if (lookahead == begin) match(begin); else symbol_error("begin");
   stat_list();
   if (lookahead == end) match(end); else symbol_error("end");
   if(lookahead == '.')  match('.'); else symbol_error(".");
   if (DEBUG) printf("\n *** Out stat_part");
}

static void program_header()
{
   if (DEBUG) printf("\n *** In  program_header");
   if (lookahead == program) match(program); else symbol_error("program");
   if (lookahead == id) {
      addp_name(get_lexeme());
      match(id);
   } else {
      addp_name("???");
      match(id);
   }
   if (lookahead == '(') match('('); else symbol_error("(");
   if (lookahead == input) match(input); else symbol_error("input");
   if (lookahead == ',') match(','); else symbol_error(",");
   if (lookahead == output) match(output); else symbol_error("output");
   if (lookahead == ')') match(')'); else symbol_error(")");
   if (lookahead == ';') match(';'); else symbol_error(";");
   if (DEBUG) printf("\n *** Out  program_header");
      
   }
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
int parser()
{
   if (DEBUG) printf("\n *** In  parser");
   lookahead = get_token();        // get the first token
   if (lookahead != '$') {
      program_header();               // call the first grammar rule
      var_part();
      stat_part();
   } else {
      printf("\nSYNTAX:   Input file is empty ");
      is_parse_ok = 0;
   }

   if(lookahead == '$') {
      match('$');
   } else if(lookahead != '$') {
      printf("\nSYNTAX:    Extra symbols after end of parse! ");
      is_parse_ok = 0;
      printf("\n          ");
   while (lookahead != '$') {
      printf("%s ", get_lexeme());
      lookahead = get_token();
    }
   }
   if (is_parse_ok) printf("\nPARSE SUCCESSFUL! ");
      printf("\n________________________________________________________ ");
   p_symtab();
   return is_parse_ok;             // status indicator
   }
/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
