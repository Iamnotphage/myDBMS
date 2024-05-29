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

	struct columnNode* columnHead;
	struct selectNode* selectHead;
	struct tableNode* tableHead;
	struct conditionNode* conditionHead;
}

/* Non-Terminated Symbols */
%type<chval> databaseName tableName columnName
%type<columnHead> columnsDefinition columnType columnNames
%type<selectHead> queryStatement
%type<tableHead> tableNames
%type<conditionHead> conditions condition operator leftOperand rightOperand

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
	CREATE DATABASE databaseName ';'	{
											// printf("[INFO] This is a create database command.\n");
											core.createDatabase($3);
										} 
	| SHOW DATABASES ';'				{
											// printf("[INFO] This is a show databases command.\n");
											core.showDatabases();
										}
	| USE databaseName ';'				{
											// printf("[INFO] This is a use database command.\n");
											core.useDatabase($2);
										}
	| DROP DATABASE databaseName ';'	{
											// printf("[INFO] This is a drop database command.\n");
											core.dropDatabase($3);
										}
	| SHOW TABLES ';'					{
											// printf("[INFO] This is a show tables command.\n");
											core.showTables();
										}
	| DROP TABLE tableName ';'			{
											//printf("[INFO] This is a drop table command.\n");
											core.dropTable($3);
										}
	;

databaseName:
	ID									{
											// printf("[INFO] Identified a database name.\n");
										}
	;

tableName:
	ID									{	
											//printf("[INFO] Identified a table name.\n");
										}
	;

/* Data-Manipulation Statements (CRUD)*/

// Create Statement.
createStatement:
	CREATE TABLE tableName '('columnsDefinition')' ';'	{
															// printf("[INFO] This is a create-table command.\n");
															core.createTable($3, $5);
														}
	;

columnsDefinition:
	columnName columnType								{	
															// printf("[INFO] Identified a single-column definition.\n");
															$$ = $2;
															$$->columnName = $1;
														}
	| columnName columnType ',' columnsDefinition		{	
															//printf("[INFO] Identified a multi-columns definitions.\n");
															$2->columnName = $1;
															$2->next = $4;
															$$ = $2;
														}
	;

columnName:
	ID									{ 
											//printf("[INFO] Identified a column name.\n");
											$$ = $<chval>1;
										}
	;

columnType:
	INT									{	
											// printf("[INFO] Identified a INT column type.\n");
											$$ = new struct columnNode;
											$$->charLength = 0;

										}
	| CHAR '(' NUMBER ')'				{	
											// printf("[INFO] Identified a CHAR column type.\n");
											$$ = new struct columnNode;
											$$->charLength = $<intval>3;
										}
	;

// Query Statement.
queryStatement:
	SELECT columnNames FROM tableNames ';'				{
															// printf("[INFO] Identified a select command.\n");
															$$ = new struct selectNode;
															$$->columnNames = $2;
															$$->tables = $4;
															core.select($$);
														}
	| SELECT columnNames FROM tableNames WHERE conditions ';'	
														{	
															// printf("[INFO] Identified a select with conditions command.\n");
															$$ = new struct selectNode;
															$$->columnNames = $2;
															$$->tables = $4;
															$$->conditions = $6;
															core.select($$);
														}
	;

columnNames:
	'*'									{
											$$ = new struct columnNode;
											$$->columnName = "*";
										}
	| columnName						{
											$$ = new struct columnNode;
											$$->columnName = $1;
										}
	| columnName ',' columnNames		{
											$$ = new struct columnNode;
											$$->columnName = $1;
											$$->next = $3;
										}
	;

tableNames:
	tableName							{
											$$ = new struct tableNode;
											$$->tableName = $1;
										}
	| tableName ',' tableNames			{
											$$ = new struct tableNode;
											$$->tableName = $1;
											$$->nextTable = $3;
										}
	;

conditions:
	condition							{	
											$$ = $1;	
										}
	| '(' conditions ')'				{	
											$$ = $2;
										}
	| conditions AND conditions			{	
											$$ = new struct conditionNode;
											$$->left = $1;
											$$->right = $3;
											$$->op = conditionNode::AND;
											$$->type = conditionNode::LOGIC;
										}
	| conditions OR conditions			{
											$$ = new struct conditionNode;
											$$->left = $1;
											$$->right = $3;
											$$->op = conditionNode::OR;
											$$->type = conditionNode::LOGIC;
										}
	;

condition:
	leftOperand operator rightOperand	{
											$$ = $2;
											$$->left = $1;
											$$->right = $3;
										}
	;

leftOperand:
	columnName							{
											$$ = new struct conditionNode;
											$$->chval = $1;
										}
	;

operator:
	'<'									{	
											$$ = new struct conditionNode;
											$$->op = conditionNode::LESS;
										}
	| '>'								{	
											$$ = new struct conditionNode;
											$$->op = conditionNode::GREATER;
										}
	| '='								{	
											$$ = new struct conditionNode;
											$$->op = conditionNode::EQUAL;
										}
	| '!''='							{	
											$$ = new struct conditionNode;
											$$->op = conditionNode::NOT;
										}
	| '<''>'							{	
											$$ = new struct conditionNode;
											$$->op = conditionNode::NOT;
										}
	;

rightOperand:
	NUMBER								{
											$$ = new struct conditionNode;
											$$->intval = $<intval>1;
											$$->type = conditionNode::INT;
										}
	| STRING							{
											$$ = new struct conditionNode;
											$$->chval = $<chval>1;
											$$->type = conditionNode::STRING;
										}
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