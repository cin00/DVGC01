/**********************************************************************/
/* lab 1 DVG C01 - Happy Hacker version  Donald Ross  060422          */
/**********************************************************************/
/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#define DEBUG 1

void expr();
void term();
void factor();
void operand();
/**********************************************************************/
/* define tokens + keywords                                           */
/**********************************************************************/
enum tvalues { program = 257, id, input, output, var, integer, begin, colon_equals, num, end, boolean, real};
/**********************************************************************/
/* Global Data Objects                                                */
/**********************************************************************/
int  lookahead=0;
int  is_parse_ok=1;
int tokens[] = {program, id, '(', input, ',', output, ')', ';',
                        var, id, ',', id, ',', id, ':', integer, ';',
                        begin, 
                        id, colon_equals, id, '+', id, '*', num,
                        end, '.', '$' };
/**********************************************************************/
/* Get the nexttoken from the buffer  (simulate the lexer)            */
/**********************************************************************/
int get_token()
{  
   static int i=0;
   if (tokens[i] != '$') return tokens[i++]; else return '$';
   }
/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
void match(int t)
{
   if(DEBUG) printf("\n --------In match expected: %3d, found: %3d", 
                     t, lookahead);
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
void type() {
   if (lookahead == integer) {
      match(integer);
   } else if (lookahead == boolean) {
      match(boolean);
   } else if(lookahead == real) {
      match(real);
   }
}


void id_list() {
   match(id);
   if (lookahead == ',') {
      match(',');
      id_list();
   }
}

void var_dec() {
   id_list();
   if(lookahead == ':') {
      match(':');
      //match(integer);
      type();
   }
}

void var_dec_list() {
   var_dec();
   match(';');
   if(lookahead == id) {
      var_dec_list();
   }
}

void var_part() {
   if(DEBUG) printf("\n*** In var_part");
   match(var); 
   var_dec_list();
}

void operand() {
   if (lookahead == id) {
      match(id);
   }
   else if(lookahead == num) {
      match(num);
   }
}

void factor() {
   if(lookahead == '(') {
      match('(');
      expr();
      match(')');
   }
   operand();
}

void term() {
   factor();
   if(lookahead == '*') {
      match('*');
      factor();
      term();
   }
}

void expr() {
   term();
   if(lookahead == '+') {
      match('+');
      term();
      expr();
   }
}

void assign_stat() {
   match(id);
   match(colon_equals);
   expr();
}

void stat_list() {
   assign_stat();
   if(lookahead == ';') {
      match(';');
      stat_list();
   }

}

void stat_part() {
   if(DEBUG) printf("\n *** In stat_part");
   match(begin);
   stat_list();
   match(end);
   match('.');
   //match(id); match(colon_equals); match(id); match('+');
   //match(id); match('*'); match(num); match(end); match('.');
}

void program_header()
{
   if (DEBUG) printf("\n *** In  program_header");
   match(program); match(id); match('('); match(input);
   match(','); match(output); match(')'); match(';');
   }
int prog()
{
   if (DEBUG) printf("\n *** In  program");
   lookahead = get_token();        // get the first token
   program_header();               // call the first grammar rule
   var_part();
   stat_part();
   return is_parse_ok;             // status indicator
   }
/**********************************************************************/
/* The main function (the driver)                                     */
/**********************************************************************/
int main()
{   
   return prog() ? printf(" \n Parse Successful! \n")
                 : printf(" \n Parse Failed! \n");
   }
/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
