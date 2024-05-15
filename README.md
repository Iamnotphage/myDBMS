# myDBMS

```
  /$$$$$$  /$$   /$$ /$$$$$$$$ /$$   /$$      
 /$$__  $$| $$  | $$| $$_____/| $$$ | $$      
| $$  \__/| $$  | $$| $$      | $$$$| $$      
| $$      | $$$$$$$$| $$$$$   | $$ $$ $$      
| $$      | $$__  $$| $$__/   | $$  $$$$      
| $$    $$| $$  | $$| $$      | $$\  $$$      
|  $$$$$$/| $$  | $$| $$$$$$$$| $$ \  $$      
 \______/ |__/  |__/|________/|__/  \__/ 
```

设计并实现一个DBMS原型系统，可以接受基本的SQL语句，对其进行词法分析、语法分析，然后解释执行SQL语句，完成对数据库文件的相应操作，实现DBMS的基本功能。

ps: 以下`教科书`特指`西电出版社的《编译原理》`

# 环境说明

* windows11
* GCC/G++ 8.1.0
* Lex(Flex) 2.5.4a
* YACC(Bison) 2.4.1
* CLion 2023
* VSCode 插件`Yash`可以高亮Lex和YACC语法

lex和yacc在UNIX中是标配，所以windows用户需要自己去下载，而linux或者macos用户会方便一些。

下面贴出GNU官方的下载地址。

[Flex Download](https://gnuwin32.sourceforge.net/packages/flex.htm)

[Bison Download](https://gnuwin32.sourceforge.net/packages/bison.htm)

安装之后需要配置环境变量，将`bin`目录添加到系统环境变量就行了。

配置好环境变量后，cmd终端能够找到`bison.exe`和`flex.exe`，所以能够执行命令，查看一下版本号看看配置是否正确。

# 前置知识
1. CMake相关的前置知识: [Quick CMake Tutorial](https://www.jetbrains.com/help/clion/quick-cmake-tutorial.html)
2. 正则表达式: [flex官方说明](https://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/html_mono/flex.html#SEC7) 或教科书
3. Lex程序基本结构: [简单程序演示](https://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/html_mono/flex.html#SEC5) 或教科书2.5章
4. YACC程序基本结构: [YACC官方文档](https://www.gnu.org/software/bison/manual/bison.html#Examples) 

# Lex源程序说明

Lex用来生成`词法分析器`（词法分析器生成器），能识别正规式，并执行给定的动作。输出的文件是`.yy.c`后缀。


## Lex源程序结构

Lex源程序的结构被`%%`符号分为三/四部分(查看教科书2.5章):

```c++
%{
Declarations
%}
Definitions
%%
Rules
%%
User subroutines
```
1. Declarations段包含一些C的头文件，宏定义，函数声明，全局变量声明
2. Definitions段包含一些 `正则表达式` 的名字 (比如`digit [0-9]`，digit是名字)
3. Rules段定义`{patterns} {actions}`每一个模式串（正则表达式）对应一个动作（C代码片段）
4. User subroutines段可以定义函数

比如上述文件名为`lex.l`

通过命令`flex lex.l`生成`lex.yy.c`文件，再`gcc`编译生成`.exe`文件，就能对输入记号流进行词法分析。

## 全局变量/函数

**分析源码，需要注意Lex程序中常用的几个全局变量和函数**

| 全局变量/函数      | 说明                                                                               |
|--------------|----------------------------------------------------------------------------------|
| char *yytext | 输入序列(字符串)                                                                        |
| int yyleng   | 输入序列的长度                                                                          |
| int yylex()  | 词法分析驱动器的入口，扫描输入序列后，匹配到`正则表达式`(最长的那一条)，执行对应的`C代码`，返回代码段返回的值(代码段没写返回值yylex()默认返回0) |
| int yywrap() | 词法分析器分析结束时，自动调用yywrap()。如果其返回值为1，则结束分析过程；如果返回值为0，则继续扫描下一个输入。                     |


## 例子

例子（或参考编译原理2.5章节）：

**识别输入序列，输出记号类型:**

```c++
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
```
文件名为`mylexer.l`，运行步骤(Windows):

1. lex源程序编译: ```flex .\mylexer.l```
2. 对生成的C源文件`lex.yy.c`编译: ```gcc lex.yy.c```
3. 运行编译完的可执行文件`a.exe`: ```.\a.exe```

程序可以识别两类记号，一种是标识符，一种是数字.

若识别到正则表达式对应的字符串，执行对应的C代码.

main()函数将自动调用生成的yylex()函数。

yylex()执行完之后询问yywrap()，是否需要再扫描后续输入。

# YACC源程序说明

Yet Another Compiler Compiler.

语法分析器生成器。识别手工设计的产生式(Productions)执行对应的语义动作。文件后缀`.y`，输出文件后缀`.tab.c`

## YACC源程序结构

YACC源程序的结构也是类似的三/四段(查看教科书3.5章)

```c++
%{
Declarations
%}
Definitions
%%
Productions
%%
User subroutines
```
这里`Declarations`和`User subroutines`和lex源程序是一样的作用。

特别说明的是`Definitions`段和`Productions`段。

前者比lex源码多了一些YACC转有的变量，后者是定义语法产生式(一说文法，都是grammar)，并且与手写的符号不太一样。

## 关于Definitions段

**终结符**

用`token`定义

```c++
%token NUMBER
```

**结合性和优先级**

```c++
%left '+' '-'
%left '*' '/'
```
变量`left`代表左结合，同一行的符号优先级相同。下面行的优先级比上面行的高。

## 全局变量/函数

| 全局变量/函数        | 说明                                                  |
|----------------|-----------------------------------------------------|
| YYSTYPE yylval | YYSTYPE类型（其实就是int），默认是int，可以通过union自定义。存储当前词法单元的属性值 |
| char *yytext   | 同lex中的yytext，指向当前匹配的输入字符串                           |
| int yyleng     | 同lex中的yyleng，表示当前匹配的输入字符串的长度                        |
| int yylex()    | 同lex中的yylex()，词法分析器函数                               |
| int yyparse()  | 语法分析器函数，解析输入内容，并根据语法规则执行对应代码                        |
| void yyerror() | 错误处理，用户自定义                                          |
| int yywrap()   | 同lex中的yywrap()，返回1表示输入结束，0表示还有输入                    |

**特别地，在产生式中，对应的动作**（也就是对应的C代码段，原文actions）可以使用`$`符号指代产生式的左部或者右部的某个符号。

```c++
$1、$2 和 $$ 的使用
$n：用于访问产生式右侧第 n 个符号的值。$1 表示第一个符号的值，$2 表示第二个符号的值，依此类推。
$$：用于表示产生式左侧非终结符的值。

......前文省略
%%

expr : expr '+' expr { printf("Result: %d\n", $1.intval + $3.intval); }
     | expr '-' expr { printf("Result: %d\n", $1.intval - $3.intval); }
     | expr '*' expr { printf("Result: %d\n", $1.intval * $3.intval); }
     | expr '/' expr { printf("Result: %d\n", $1.intval / $3.intval); }
     | NUMBER        { $$ = $1.intval; }
     ;

%%
......后文省略
```

## 例子

这里是单个YACC程序，没有配合lex。用户手动输入代替lex词法分析之后产生的token stream.

所以手动定义了yylex()，后续lex和YACC配合时，yylex()由lex自动生成。

```c++
%{
    #include<ctype.h>
    #include<stdio.h>
    int yylex();
    void yyerror(const char*);
%}

%token NUMBER
%left '+' '-'
%left '*' '/'

%% // 这里是产生式 expr是非终结符，NUMBER是终结符
expr : expr '+' expr    {printf("Identified [add].\n");}
     | expr '-' expr    {printf("Identified [sub].\n");}
     | expr '*' expr    {printf("Identified [multiply].\n");}
     | expr '/' expr    {printf("Identified [divide].\n");}
     | '(' expr ')'     {printf("Identified [round bracket].\n");}
     | NUMBER           {printf("Identified [NUMBER].\n");}
     ;
%%

int main(void){
    return yyparse();
}

int yylex(void){
    int c;
    while((c = getchar()) == ' ');
    if(isdigit(c)){
        ungetc(c, stdin);
        scanf("%d", &yylval);
        return NUMBER;
    }
    if(c == '\n')return 0;
    return c;
}

void yyerror(const char *s){
    printf("%s", s);
}
```

文件名为`myparser.y`，运行步骤(Windows):

1. 编译.y程序: ```bison .\myparser.y```
2. 编译生成的C程序: ```gcc .\myparser.tab.c```
3. 执行生成的可执行文件: ```.\a.exe```

输入字符串(其实是token stream)，可以识别表达式。

这里是main()函数调用yyparse()函数

而yyparse()将调用yylex()函数 *(这里因为只由一个YACC程序组成，所以yylex()函数是用户自定义的)* 获取输入的token，并语法分析

匹配到产生式就执行对应的代码段。

# 数据库设计
