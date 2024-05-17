cmake_minimum_required(VERSION 3.25)
project(myDBMS)

# 找到flex和bison
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# 定义源文件目录
set(PARSER_DIR ${CMAKE_SOURCE_DIR}/parser)
set(SHELL_DIR ${CMAKE_SOURCE_DIR}/shell)

# 生成解析器和词法分析器
BISON_TARGET(Parser ${PARSER_DIR}/parser.y ${PARSER_DIR}/parser.tab.c)
FLEX_TARGET(Lexer ${PARSER_DIR}/lexer.l ${PARSER_DIR}/lex.yy.c)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

# 添加源文件
add_executable(myDBMS
        ${BISON_Parser_OUTPUTS}
        ${FLEX_Lexer_OUTPUTS}
        ${SHELL_DIR}/shell.h
        ${SHELL_DIR}/shell.cpp
        main.cpp
        )

# 包含目录
target_include_directories(myDBMS PRIVATE ${PARSER_DIR})

# 链接必要的库
target_link_libraries(myDBMS PRIVATE ${FLEX_LIBRARIES} ${BISON_LIBRARIES})
