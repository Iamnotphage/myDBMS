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

	struct valueNode* valueHead;
	struct insertNode* insertHead;

	struct assignmentNode* assignmentHead;
	struct updateNode* updateHead;
}

/* Non-Terminated Symbols */
%type<chval> databaseName tableName columnName
%type<columnHead> columnsDefinition columnType columnNames

%type<selectHead> queryStatement
%type<tableHead> tableNames
%type<conditionHead> conditions condition operator leftOperand rightOperand

%type<insertHead> insertStatement
%type<valueHead> values value

%type<updateHead> updateStatement
%type<assignmentHead> assignment assignments

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
											$$->next = $3;
										}
	;

// Top-level conditions rules
conditions:
    condition
    | condition AND conditions         {
                                            $$ = new conditionNode;
                                            $$->left = $1;
                                            $$->right = $3;
                                            $$->op = conditionNode::AND;
                                            $$->type = conditionNode::LOGIC;
                                        }
    | condition OR conditions           {
                                            $$ = new conditionNode;
                                            $$->left = $1;
                                            $$->right = $3;
                                            $$->op = conditionNode::OR;
                                            $$->type = conditionNode::LOGIC;
                                        }
    ;

// Single condition rule
condition:
    leftOperand operator rightOperand   {
                                            $$ = new conditionNode;
                                            $$->left = $1;
                                            $$->right = $3;
                                            $$->op = $2->op;
                                            $$->type = conditionNode::LOGIC;
                                        }
	| '(' condition ')'                 {
                                            $$ = $2;
                                        }
    ;

// Left operand can be a column name
leftOperand:
    columnName                          {
                                            $$ = new conditionNode;
                                            $$->chval = $1;
                                            $$->type = conditionNode::STRING; // Assuming column names are strings
                                        }
    ;

// Operator definitions
operator:
    '<'                                 {
                                            $$ = new conditionNode;
                                            $$->op = conditionNode::LESS;
                                        }
    | '>'                               {
                                            $$ = new conditionNode;
                                            $$->op = conditionNode::GREATER;
                                        }
    | '='                               {
                                            $$ = new conditionNode;
                                            $$->op = conditionNode::EQUAL;
                                        }
    | '!' '='                           {
                                            $$ = new conditionNode;
                                            $$->op = conditionNode::NOT;
                                        }
    | '<' '>'                           {
                                            $$ = new conditionNode;
                                            $$->op = conditionNode::NOT;
                                        }
    ;

// Right operand can be a number or a string
rightOperand:
    NUMBER                              {
                                            printf("[INFO] A NUMBER.\n");
                                            $$ = new conditionNode;
                                            $$->intval = $<intval>1;
                                            $$->type = conditionNode::INT;
                                        }
    | STRING                            {
                                            printf("[INFO] A STRING.\n");
                                            $$ = new conditionNode;
                                            $$->chval = $<chval>1;
                                            $$->type = conditionNode::STRING;
                                        }
    ;

// Insert statement.
insertStatement:
	INSERT INTO tableName '(' columnNames ')' VALUES '(' values ')' ';'	
										{
											// printf("[INFO] Identified a strong-insert command.\n");
											$$ = new struct insertNode;
											$$->tableName = $3;
											$$->columnNames = $5;
											$$->values = $9;
											core.insert($$);
										}
	| INSERT INTO tableName VALUES '(' values ')' ';'					
										{	
											// printf("[INFO] Identified a weak-insert command.\n");
											$$ = new struct insertNode;
											$$->tableName = $3;
											$$->values = $6;
											core.insert($$);
										}
	;

values:
	value								{
											$$ = $1;
										}
	| value ',' values					{
											$$ = $1;
											$$->next = $3;
										}
	;

value:
	NUMBER								{
											printf("[INFO] Identified value NUMBER.\n");
											$$ = new struct valueNode;
											$$->type = valueNode::INT;
											$$->intval = $<intval>1;
										}
	| STRING							{
											printf("[INFO] Identified value STRING.\n");
											$$ = new struct valueNode;
											$$->type = valueNode::STRING;
											$$->chval = $<chval>1;
										}
	;

// Update statement.
updateStatement:
	UPDATE tableName SET assignments WHERE conditions ';'
										{	
											printf("[INFO] Identified a update command.\n");
											$$ = new struct updateNode;
											$$->tableName = $2;
											$$->assignments = $4;
											$$->conditions = $6;
											// core.update($$);
										}
	;

assignments:
	assignment							{
											printf("[INFO] Identified a assignment.\n");
											$$ = $1;
										}
	| assignment ',' assignments		{
											printf("[INFO] Identified a assignments.\n");
											$$ = $1;
											$$->next = $3;
										}
	;

assignment:
	columnName '=' value				{
											printf("[INFO] Identified a assignments [a=b].\n");
											$$ = new struct assignmentNode;
											$$->columnName = $1;
											if($3->type == valueNode::INT){
												$$->type = assignmentNode::INT;
												$$->intval = $3->intval;
											}else if($3->type == valueNode::STRING){
												$$->type = assignmentNode::STRING;
												$$->chval = $3->chval;
											}
										}
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