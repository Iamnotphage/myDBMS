%{
#include <stdio.h>
%}

%%

([1-9]+[0-9]*)|[0]	ECHO;
[0-9]+\.[0-9]+
[a-zA-Z]+
.

%%

int main(int argc, char **argv)
{
  yylex();
  yywrap();
}
int yywrap()
{
	return 1;
}