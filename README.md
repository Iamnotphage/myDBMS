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

# 环境说明

* windows11
* GCC/G++ 8.1.0
* Lex(Flex) 2.5.4a
* YACC(Bison) 2.4.1
* CLion 2023

lex和yacc在UNIX中是标配，所以windows用户需要自己去下载，而linux或者macos用户会方便一些。

下面贴出GNU官方的下载地址。

[Flex Download](https://gnuwin32.sourceforge.net/packages/flex.htm)

[Bison Download](https://gnuwin32.sourceforge.net/packages/bison.htm)

安装之后需要配置环境变量，将`bin`目录添加到系统环境变量就行了。

配置好环境变量后，cmd终端能够找到`bison.exe`和`flex.exe`，所以能够执行命令，查看一下版本号看看配置是否正确。

# 前置知识
1. CMake相关的前置知识: [Quick CMake Tutorial](https://www.jetbrains.com/help/clion/quick-cmake-tutorial.html)
2. 正则表达式: [flex官方说明](https://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/html_mono/flex.html#SEC7)
3. Lex程序基本结构: [简单程序演示](https://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/html_mono/flex.html#SEC5)
4. YACC程序基本结构: [YACC官方文档](https://www.gnu.org/software/bison/manual/bison.html#Examples)

# Lex源程序说明

Lex用来生成`词法分析器`（词法分析器生成器），能识别正规式，并执行给定的动作。输出的文件是`.yy.c`后缀。

Lex源程序的结构被`%%`符号分为三部分:

```c++
%{定义
    C声明
    辅助定义正规式
%}

%%
规则
    词法规则正规式
    语义动作(C代码)
    
%%
用户子程序
    C源程序
```
比如上述文件名为`lex.l`

通过命令`flex lex.l`生成`lex.yy.c`文件，再`gcc`编译生成`.exe`文件，就能对输入记号流进行词法分析。

**需要注意的是Lex程序中常用的几个全局变量和函数**

| 全局变量/函数      | 说明                                                           |
|--------------|--------------------------------------------------------------|
| yytext       | 输入序列                                                         |
| yyleng       | 输入序列的长度                                                      |
| int yylex()  | 词法分析驱动器的入口，返回`正则表达式`对应的`C代码`的返回值                             |
| int yywrap() | 词法分析器分析结束时，自动调用yywrap()。如果其返回值为1，则结束分析过程；如果返回值为0，则继续扫描下一个输入。 |



# YACC源程序说明

# 数据库设计
