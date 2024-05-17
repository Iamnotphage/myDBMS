%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%%

[ \t\n]+            ;  // Ignore whitespace
[0-9]+              { yylval.intval = atoi(yytext); return NUMBER; }
"insert"            { return INSERT; }
"select"            { return SELECT; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.strval = strdup(yytext); return IDENTIFIER; }
\"[^\"]*\"          { yylval.strval = strdup(yytext + 1); yylval.strval[strlen(yylval.strval) - 1] = 0; return STRING; }

.                   { return yytext[0]; }

%%

int yywrap() {
    return 1;
}