#include <iostream>
#include <string.h>


int yyparse(void); // 从别的文件找这些函数
void yy_scan_string(const char* str);




int main() {
    std::string inputLine;

    // 从标准输入读取一行
    std::getline(std::cin, inputLine);

    // 将输入字符串传递给词法分析器
    yy_scan_string(inputLine.c_str()); // c风格的string，其实就是char*

    // 调用语法分析器
    yyparse();

    return 0;
}