%{
#include <stdio.h>
#include <stdlib.h>
#include "../shell/shell.h"

void yyerror(const char *s);
int yylex(void);
%}

%union {
    int intval;
    char *strval;
}

%token <intval> NUMBER
%token <strval> IDENTIFIER STRING
%token INSERT SELECT VALUES

%%

query:
    insert_query
  | select_query
  ;

insert_query:
    INSERT IDENTIFIER VALUES '(' STRING ')' { printf("Insert into %s values (%s)\n", $2, $5); }
  ;

select_query:
    SELECT IDENTIFIER { printf("Select from %s\n", $2); }
  ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}