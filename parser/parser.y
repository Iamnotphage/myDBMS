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

/* Non-Terminated Symbols */
%type<chval> databaseName tableName columnName

/* Terminated Symbols */
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

/* Start-Statement : make sure that bison don't warn you. */
startStatement:
	systemControl
	| createStatement
	| queryStatement
	;

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
	| columnName columnType ',' columnsDefinition		{printf("[INFO] Identified a multi-columns definitions.\n");}
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
	SELECT columnNames FROM tableNames ';'				{printf("[INFO] Identified a select command.\n");}
	| SELECT columnNames FROM tableNames WHERE conditions ';'	{printf("[INFO] Identified a select with conditions command.\n");}
	;

columnNames:
	columnName
	| columnName ',' columnNames
	;

tableNames:
	tableName
	| tableName ',' tableNames
	;

conditions:
	condition
	| '(' conditions ')'
	| conditions AND conditions
	| conditions OR conditions
	;

condition:
	leftOperand operator rightOperand
	;

leftOperand:
	columnName
	;

operator:
	'<'
	| '>'
	| '='
	| '!''='
	| '<''>'
	;

rightOperand:
	NUMBER
	| STRING
	;


%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}