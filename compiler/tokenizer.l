%{
    #include "parser.tab.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
%}

/* Define keywords */

/* System-Control Keywords */
SHOW        show|SHOW
USE         use|USE

/* Data-Manipulation Keywords */
SELECT      select|SELECT
FROM        from|FROM
WHERE       where|WHERE
AND         and|AND
OR          or|OR

INSERT      insert|INSERT
INTO        into|INTO
VALUES      values|VALUES

DELETE      delete|DELETE

UPDATE      update|UPDATE
SET         set|SET

/* Data-Definition Keywords */
CREATE      create|CREATE
DROP        drop|DROP
DATABASE    database|DATABASE
DATABASES   databases|DATABASES
TABLE       table|TABLE
TABLES      tables|TABLES

CHAR        char|CHAR
INT         int|INT

/* Identifiers and Literals */
ID          [a-zA-Z][a-zA-Z_0-9]*
NUMBER      [-+]?[1-9][0-9]*
STRING      '.*'

%%


{SHOW}      {return SHOW;}
{USE}       {return USE;}


{SELECT}    {return SELECT;}
{FROM}      {return FROM;}
{WHERE}     {return WHERE;}
{AND}       {return AND;}
{OR}        {return OR;}

{INSERT}    {return INSERT;}
{INTO}      {return INTO;}
{VALUES}    {return VALUES;}

{DELETE}    {return DELETE;}

{UPDATE}    {return UPDATE;}
{SET}       {return SET;}

{CREATE}    {return CREATE;}
{DROP}      {return DROP;}
{DATABASE}  {return DATABASE;}
{DATABASES} {return DATABASES;}
{TABLE}     {return TABLE;}
{TABLES}    {return TABLES;}

{CHAR}      {return CHAR;}
{INT}       {return INT;}

{ID}        {
                yylval.chval = strdup(yytext);
                return ID;
            }

{NUMBER}    {
                yylval.intval = atoi(yytext);
                return NUMBER;
            }

{STRING}    {
                *(yytext + strlen(yytext) - 1) = '\0';
                yylval.chval = strdup(yytext + 1);
                return STRING;
            }

"("         {return '(';}
")"         {return ')';}
";"         {return ';';}
","         {return ',';}
"*"         {return '*';}
"."         {return '.';}
"<"         {return '<';}
">"         {return '>';}
"="         {return '=';}
"!"         {return '!';}


[ \n\t]+      ;/* ignore whitespace */

%%

int yywrap() {
    return 1;
}