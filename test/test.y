%{
    #include <stdio.h>
    #include <string.h>
    int yylex(void);
    void yyerror(char *);
%}

%token NUM

%%
expr:
    NUM {printf("This is a number.\n")};
    ;
%%
void yyerror(char *str){
    fprintf(stderr,"error:%s\n",str);
}

// int main()
// {
//     yyparse();
// }