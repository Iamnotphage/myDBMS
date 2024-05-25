cmake_minimum_required(VERSION 3.25)
project(myDBMS)

# 找到flex和bison
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# 定义源文件目录
set(COMPILER_DIR ${CMAKE_SOURCE_DIR}/compiler)
set(SHELL_DIR ${CMAKE_SOURCE_DIR}/shell)
set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/engine)

# 生成解析器和词法分析器
BISON_TARGET(Parser ${COMPILER_DIR}/parser.y ${COMPILER_DIR}/parser.tab.cpp)
FLEX_TARGET(Tokenizer ${COMPILER_DIR}/tokenizer.l ${COMPILER_DIR}/lex.yy.cpp)
ADD_FLEX_BISON_DEPENDENCY(Tokenizer Parser)

# 添加源文件
add_executable(myDBMS
        ${BISON_Parser_OUTPUTS}
        ${FLEX_Tokenizer_OUTPUTS}
        ${SHELL_DIR}/shell.h
        ${SHELL_DIR}/shell.cpp
        ${ENGINE_DIR}/Database.h
        ${ENGINE_DIR}/Database.cpp
        main.cpp
        )

# 包含目录
target_include_directories(myDBMS
        PRIVATE
        ${COMPILER_DIR}
        ${ENGINE_DIR}
        )

# 链接必要的库
target_link_libraries(myDBMS PRIVATE ${FLEX_LIBRARIES} ${BISON_LIBRARIES})

# 移除register关键字
add_definitions(-Dregister=)

# 设置C++标准
set_target_properties(myDBMS PROPERTIES CXX_STANDARD 17)

# 静态链接
set_target_properties(myDBMS PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
