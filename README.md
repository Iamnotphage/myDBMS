# myDBMS

设计并实现一个DBMS原型系统，可以接受基本的SQL语句，对其进行词法分析、语法分析，然后解释执行SQL语句，完成对数据库文件的相应操作，实现DBMS的基本功能。

# 环境说明

* windows11
* GCC/G++ 8.1.0
* Lex(Flex) 2.5.4a
* YACC(Bison) 2.4.1
* Clion 2023

lex和yacc在UNIX中是标配，所以windows用户需要自己去下载，而linux或者macos用户会方便一些。

下面贴出GNU官方的下载地址。

[Flex Download](https://gnuwin32.sourceforge.net/packages/flex.htm)

[Bison Download](https://gnuwin32.sourceforge.net/packages/bison.htm)

安装之后需要配置环境变量，将`bin`目录添加到系统环境变量就行了。

配置好环境变量后，cmd终端能够找到`bison.exe`和`flex.exe`，所以能够执行命令，查看一下版本号看看配置是否正确。
