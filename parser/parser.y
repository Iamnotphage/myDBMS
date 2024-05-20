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

/* System-Control Statements */
systemControl:
	CREATE DATABASE databaseName ';'	{printf("[INFO] This is a create database command.\n");} 
	| SHOW DATABASES ';'				{printf("[INFO] This is a show databases command.\n");}
	| SHOW TABLES ';'					{printf("[INFO] This is a show tables command.\n");}
	| USE databaseName ';'				{printf("[INFO] This is a use database command.\n");}
	| DROP DATABASE databaseName ';'	{printf("[INFO] This is a drop database command.\n");}
	| DROP TABLE tableName ';'			{printf("[INFO] This is a drop table command.\n");}
	;

databaseName:
	ID									{printf("[INFO] Identified a database name.\n");}
	;

tableName:
	ID									{printf("[INFO] Identified a table name.\n");}
	;

/* Data-Manipulation Statements (CRUD)*/

// Create Statement.
createStatement:
	CREATE TABLE tableName '('columnsDefinition')' ';'	{printf("[INFO] This is a create-table command.\n");}
	;

columnsDefinition:
	columnName columnType								{printf("[INFO] Identified a single-column definition.\n");}
	| columnName columnType ',' columnsDefinition		{prinf("[INFO] Identified a multi-columns definitions.\n");}
	;

columnName:
	ID									{printf("[INFO] Identified a column name.\n");}
	;

columnType:
	INT									{printf("[INFO] Identified a INT column type.\n");}
	| CHAR '(' NUMBER ')'				{printf("[INFO] Identified a CHAR column type.\n");}
	;

// Query Statement.
queryStatement:
	SELECT columnsDefinition FROM tableName ';'
	| SELECT columnsDefinition FROM tableName WHERE conditions ';'
	;

conditions:

	;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}