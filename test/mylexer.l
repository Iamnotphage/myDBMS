%{
    #define ID 0
    #define NUMBER 1
%}

char [a-zA-Z]
digit [0-9]
digits {digit}+
optional_fraction ("."{digits})?
optional_exponent (E[+-]?{digits})?

%%
{char}({char}|{digit})* {printf("identified a ID %s: length: %d\n", yytext, yyleng);
                         return ID;}

{digits}{optional_fraction}{optional_exponent} {printf("identified a NUMBER %s: length: %d\n", yytext, yyleng);
                                                return NUMBER;}

%%
int main(void){
    printf("Done, token type: %d\n", yylex());
}

int yywrap(){
    return 1;
}