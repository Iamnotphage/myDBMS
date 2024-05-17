%{
    #include "test.tab.h"
%}

NUM [1-9]+[0-9]*|0

%%

{NUM}		                return NUM;
[ \t]+                     /* ignore whitespace */;
.

%%


int yywrap(){
    return 1;
}