%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "Database.h"

	int yylex(void);
	void yyerror(const char *s);

	Database core;
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
	| insertStatement
	| updateStatement
	| deleteStatement
	;

/* System-Control Statements */
systemControl:
	CREATE DATABASE databaseName ';'	{printf("[INFO] This is a create database command.\n");} 
	| SHOW DATABASES ';'				{
											// printf("[INFO] This is a show databases command.\n");
											core.showDatabases();
										}
	| SHOW TABLES ';'					{printf("[INFO] This is a show tables command.\n");}
	| USE databaseName ';'				{
											// printf("[INFO] This is a use database command.\n");
											core.useDatabase($2);
										}
	| DROP DATABASE databaseName ';'	{
											// printf("[INFO] This is a drop database command.\n");
											core.dropDatabase($3);
										}
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

// Insert statement.
insertStatement:
	INSERT INTO tableName '(' columnNames ')' VALUES '(' values ')' ';'	{printf("[INFO] Identified a strong-insert command.\n");}
	| INSERT INTO tableName VALUES '(' values ')' ';'					{printf("[INFO] Identified a weak-insert command.\n");}
	;

values:
	value
	| value ',' values
	;

value:
	NUMBER
	| STRING
	;

// Update statement.
updateStatement:
	UPDATE tableName SET assignments WHERE conditions ';'	{printf("[INFO] Identified a update command.\n");}
	;

assignments:
	assignment
	| assignment ',' assignments
	;

assignment:
	columnName '=' value
	;

// Delete statement.
deleteStatement:
	DELETE FROM tableName ';'	{printf("[INFO] Identified a whole-table delete command.\n");}
	| DELETE FROM tableName WHERE conditions ';'	{printf("[INFO] Identified a delete command.\n");}
	;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}