//
// Created by Iamnotphage on 2024-05-24.
//

#ifndef MYDBMS_PAGER_H
#define MYDBMS_PAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

const unsigned int PAGE_SIZE = 4096;
const unsigned int FILE_HEADER_SIZE = sizeof(int) * 3;
const unsigned int PAGE_HEADER_SIZE = sizeof(int) * 2;
const unsigned int RECORDS_SIZE = PAGE_SIZE - FILE_HEADER_SIZE - PAGE_HEADER_SIZE;
const unsigned int ROW_PER_PAGE = 8 + RECORDS_SIZE / 64; // 8 + 63 = 71; 大概63行数据，8行头信息
const int DEFAULT_INFIMUM = 99999;
const int DEFAULT_SUPERMUM = -1;

struct FileHeader {
    int pageNumber; // 当前页的页号
    std::unordered_map<std::string, int> columnOffset; // 在Records中列名对应的偏移（第几个逗号）
    int prevPage; // 上一页偏移 (-PAGE_SIZE)
    int nextPage; // 下一页偏移 (+PAGE_SIZE)
};

struct PageHeader {
    int recordsCount; // 当前页记录的数目
    int pageState; // 页的状态
};

struct Record {
    int id; // 主键
    std::string data; // 数据(逗号分隔)
    int nextOffset; // 下一条数据的偏移量
};

class Pager {
public:
    std::string path; // 当前页所属表名（即文件名）
    FileHeader fileHeader;
    PageHeader pageHeader;
    int Infimum; // 当前页最小记录
    int Supermum; // 当前页最大记录
    bool isDirty;
    std::vector<int> pageDirectory; // 页目录存储记录的偏移量
    std::vector<Record> records; // 当前页的记录


    Pager(const std::string& filePath); // 初始化时从外存读页



    Pager* readPage(int ID); // 将页从外存读入内存，这里还没实现BTree，先根据path读文件，遍历页来找目标id所在的页
    void writePage(); // 页的状态设为DIRTY，并在内存中更新页

};


#endif //MYDBMS_PAGER_H
