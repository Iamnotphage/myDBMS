%{
	#include <stdio.h>
	#include <stdlib.h>

	//#include "shell/shell.h"

	int yylex(void);
	void yyerror(const char *s);
%}

// Define yylval;
%union {
    int intval;
    char* chval;
}

// System-Control Tokens
%token SHOW
%token USE

// Data-Manipulation Tokens
%token SELECT
%token FROM
%token WHERE

%left AND
%left OR

%token INSERT
%token INTO
%token VALUES

%token DELETE

%token UPDATE
%token SET

// Data-Definition Keywords
%token CREATE
%token DROP
%token DATABASE
%token DATABASES
%token TABLE
%token TABLES

%token CHAR
%token INT

// Identifiers and Literals
%token ID
%token NUMBER
%token STRING

%%

// System-Control Statements
systemControl:
	CREATE DATABASE database ';'	{printf("[INFO] this is a create database command.\n");} 
	| SHOW DATABASES ';'			{printf("[INFO] this is a show databases command.\n");}
	| SHOW TABLES ';'				{printf("[INFO] this is a show tables command.\n");}
	| USE database ';'				{printf("[INFO] this is a use database command.\n");}
	;

database:
	ID
	;
	
%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}