//
// Created by Iamnotphage on 2024-05-24.
//

#include "Database.h"
#include "Pager.h"
#include <iostream>
#include <filesystem>
#include <set>
#include <fstream>

using namespace std::filesystem;

Database::Database() {
    this->currentState = STATE_SYS;
    this->currentPage = nullptr;
}

Database::~Database() = default;

void Database::showDatabases() {

    try{
        if(exists(dataPath) && is_directory(dataPath)){
            // Start Time
            clock_t startTime = clock();
            std::set<directory_entry> databaseNames;
            unsigned int nameMaxLength = 0;
            const std::string title = "Database";

            // Add all the directory into the set<>.
            for(const directory_entry& entry : directory_iterator(dataPath)){
                if(entry.is_directory()){
                    unsigned int currentLength = entry.path().filename().string().length();
                    nameMaxLength = currentLength > nameMaxLength ? currentLength : nameMaxLength;
                    databaseNames.insert(entry);
                }
            }

            // max name length must > "Datbase".length().
            nameMaxLength = nameMaxLength > title.length() ? nameMaxLength : title.length();

            // First-Row
            std::cout << "+";
            for(int i = 0; i < nameMaxLength + 2; i++){
                std::cout << "-";
            }
            std::cout << "+" << std::endl;

            // Second-Row
            std::cout << "| " << title;
            for(int i = 0; i < nameMaxLength - title.length(); i++){
                std::cout << " ";
            }
            std::cout << " |" << std::endl;

            // Third-Row
            std::cout << "+";
            for(int i = 0; i < nameMaxLength + 2; i++){
                std::cout << "-";
            }
            std::cout << "+" << std::endl;

            // Databases Names
            for(const directory_entry& entry : databaseNames){
                std::cout << "| " << entry.path().filename().string();

                for(int i = 0; i < nameMaxLength - entry.path().filename().string().length(); i++){
                    std::cout << " ";
                }

                std::cout << " |" << std::endl;
            }

            // Last-Row
            std::cout << "+";
            for(int i = 0; i < nameMaxLength + 2; i++){
                std::cout << "-";
            }
            std::cout << "+" << std::endl;

            // Exexute Time
            clock_t endTime = clock();
            std::cout << databaseNames.size() << " rows in set ("
                      << std::fixed << std::setprecision(2)
                      << (double)(endTime - startTime)/CLOCKS_PER_SEC
                      << " sec)" << std::endl;

        }else{
            std::cout << "[INFO] Database path doesn't exits or is not a directory." << std::endl;
        }
    }catch(const filesystem_error& e){
        std::cerr << "[INFO] " << e.what() << std::endl;
    }catch(const std::exception& e){
        std::cerr << "[INFO] " << e.what() << std::endl;
    }
}

void Database::useDatabase(const std::string& databaseName) {
    if(!validate(databaseName)) {
        std::cout << "[INFO] Unknown database " << "'" << databaseName << "'" << std::endl;
        return;
    }

    if(this->currentPage != nullptr)this->currentPage->writePage(); // 切换数据库，写回页
    this->currentDatabase = databaseName;
    this->currentState = STATE_DB;
    this->tableFiles.clear();

    std::string databasePath = this->dataPath + "/" + this->currentDatabase;

    for (const auto& entry : std::filesystem::directory_iterator(databasePath)) {
        if (entry.is_regular_file()) {
            std::string tableFileName = entry.path().filename().string();
            std::string tableName = tableFileName.substr(0, tableFileName.find_last_of('.'));
            // std::cout << "----" << entry.path().string() << std::endl;
            tableFiles[tableName] = entry.path().string();
        }
    }

    std::cout << "Database changed" << std::endl;
}

bool Database::validate(const std::string& databaseName) {

    try{
        path dbPath = dataPath + "/" + databaseName;
        return exists(dbPath) && is_directory(dbPath);
    }catch(const filesystem_error& e){
        std::cerr << "[INFO] " << e.what() << std::endl;
    }catch(const std::exception& e){
        std::cerr << "[INFO] " << e.what() << std::endl;
    }
    return false;
}

void Database::dropDatabase(const std::string& databaseName) {
    if(!validate(databaseName)){
        std::cout << "[INFO] Can't drop database 'test'; database doesn't exist" << std::endl;
    }

    try{
        // Start Time
        clock_t startTime = clock();

        // Drop The Database
        if(this->currentState == STATE_DB && this->currentDatabase == databaseName){
            this->currentState = STATE_SYS;
            this->currentDatabase = "";
        }
        remove_all(dataPath + "/" + databaseName);

        // Exexute Time
        clock_t endTime = clock();
        std::cout << "Query OK, 0 row affected ("
                  << std::fixed << std::setprecision(2)
                  << (double)(endTime - startTime)/CLOCKS_PER_SEC
                  << " sec)" << std::endl;

    }catch(const filesystem_error& e){
        std::cerr << "[INFO] " << e.what() << std::endl;
    }

}

void Database::createDatabase(const std::string &databaseName) {
    if(validate(databaseName)){
        std::cout << "[INFO] Can't create database '"<< databaseName << "'; database exists" << std::endl;
    }
    // Start Time
    clock_t startTime = clock();
    if(create_directory(dataPath + "/" + databaseName)){
        // Exexute Time
        clock_t endTime = clock();
        std::cout << "Query OK, 1 row affected ("
                  << std::fixed << std::setprecision(2)
                  << (double)(endTime - startTime)/CLOCKS_PER_SEC
                  << " sec)" << std::endl;
    }else{
        std::cout << "[INFO] Can't create database '"<< databaseName << "'" << std::endl;
    }
}

void Database::showTables() {
    if(this->currentState == STATE_SYS){
        std::cout << "[INFO] No database selected" << std::endl;
    }else if(this->currentState == STATE_DB){
        std::string dbPath = dataPath + "/" + this->currentDatabase;

        try{
            if(exists(dbPath) && is_directory(dbPath)){
                // Start Time
                clock_t startTime = clock();

                std::set<directory_entry> tableNames;
                unsigned int nameMaxLength = 0;
                const std::string title = "Tables_in_" + this->currentDatabase;

                for(const directory_entry& entry : directory_iterator(dbPath)){
                    if(is_regular_file(entry)){
                        unsigned int currentLength = entry.path().filename().string().length();
                        nameMaxLength = currentLength > nameMaxLength ? currentLength : nameMaxLength;
                        tableNames.insert(entry);
                    }
                }


                if(tableNames.empty()){
                    std::cout << "Empty set (0.00 sec)" << std::endl;
                    return;
                }

                // max name length must > "Tables_in_...".length().
                nameMaxLength = nameMaxLength > title.length() ? nameMaxLength : title.length();

                // First-Row
                std::cout << "+";
                for(int i = 0; i < nameMaxLength + 2; i++){
                    std::cout << "-";
                }
                std::cout << "+" << std::endl;

                // Second-Row
                std::cout << "| " << title;
                for(int i = 0; i < nameMaxLength - title.length(); i++){
                    std::cout << " ";
                }
                std::cout << " |" << std::endl;

                // Third-Row
                std::cout << "+";
                for(int i = 0; i < nameMaxLength + 2; i++){
                    std::cout << "-";
                }
                std::cout << "+" << std::endl;

                // Databases Names
                for(const directory_entry& entry : tableNames){
                    std::cout << "| " << entry.path().filename().string();

                    for(int i = 0; i < nameMaxLength - entry.path().filename().string().length(); i++){
                        std::cout << " ";
                    }

                    std::cout << " |" << std::endl;
                }

                // Last-Row
                std::cout << "+";
                for(int i = 0; i < nameMaxLength + 2; i++){
                    std::cout << "-";
                }
                std::cout << "+" << std::endl;

                // Exexute Time
                clock_t endTime = clock();
                std::cout << tableNames.size() << " rows in set ("
                          << std::fixed << std::setprecision(2)
                          << (double)(endTime - startTime)/CLOCKS_PER_SEC
                          << " sec)" << std::endl;
            }else{
                std::cout << "[INFO] Database path doesn't exits or is not a directory." << std::endl;
            }
        }catch(const filesystem_error& e){
            std::cerr << "[INFO] " << e.what() << std::endl;
        }catch(const std::exception& e){
            std::cerr << "[INFO] " << e.what() << std::endl;
        }
    }
}

void Database::dropTable(const std::string &tableName) {
    if(this->currentState == STATE_SYS){
        std::cout << "[INFO] No database selected" << std::endl;
    }else if(this->currentState == STATE_DB){
        std::string tablePath = dataPath + "/" + this->currentDatabase + "/" + tableName;

        try {
            if (exists(tablePath) && is_regular_file(tablePath)) {
                // Start Time
                clock_t startTime = clock();

                // Drop the table.
                if(remove(tablePath)){
                    // Exexute Time
                    clock_t endTime = clock();
                    std::cout << "Query OK, 0 row affected ("
                              << std::fixed << std::setprecision(2)
                              << (double)(endTime - startTime)/CLOCKS_PER_SEC
                              << " sec)" << std::endl;
                }else{
                    std::cout << "[INFO] Drop table failed for unknown reason" << std::endl;
                }
            }else{
                std::cout << "[INFO] Unknown table '" << this->currentDatabase << "." << tableName << "'" << std::endl;
            }
        }catch(const filesystem_error& e){
            std::cerr << "[INFO] " << e.what() << std::endl;
        }catch(const std::exception& e){
            std::cerr << "[INFO] " << e.what() << std::endl;
        }
    }
}

void Database::createTable(const std::string &tableName, struct columnNode *columnHead) {
    // Construct the table file path
    std::string tablePath = dataPath + "/" + this->currentDatabase + "/" + tableName + ".txt";
    if(this->currentState == STATE_SYS) {
        std::cout << "[INFO] No database selected" << std::endl;
        return;
    }

    // Check if table already exists
    if (exists(tablePath) && is_regular_file(tablePath)) {
        std::cout << "Table '" << tableName << "' already exists" << std::endl;
        return;
    }


    // Create the table file
    std::ofstream tableFile(tablePath);
    if (!tableFile) {
        std::cerr << "Failed to create table file: " << tablePath << std::endl;
        return;
    }

    // Write FileHeader (metadata)
    tableFile << "PageNumber: 0\n";
    tableFile << "prevPage: -1\n";
    tableFile << "nextPage: 1\n";
    tableFile << "Columns:";

    // Write columns with offsets
    columnNode* currentColumn = columnHead;
    int offset = 0;
    while (currentColumn) {
        tableFile << currentColumn->columnName << ":" << offset;
        currentColumn = currentColumn->next;
        if (currentColumn) tableFile << ",";
        ++offset;
    }
    tableFile << "\n";

    // Write PageHeader
    tableFile << "recordsCount: 0\n";
    tableFile << "Infimum:" << 99999 << "\n";
    tableFile << "Supermum:" << -1 << "\n";

    // Write an empty page directory
    tableFile << "PageDirectory: \n";

    tableFile.close();
    std::cout << "Table '" << tableName << "' created successfully" << std::endl;
}

void Database::select(struct selectNode *node) {
    // TEST DATA: select id,name,test from sys_test,sys_test2 where id = 5 AND name = 'shit';
    // SELECT id FROM testTable WHERE money='99';
    // select id,name,test from sys_test,sys_test2 where (id = 5 AND name = 'shit') OR (dsaf=3);
    // SELECT * FROM STUDENT,COURSE WHERE ((SSEX=0) AND (CID=1)) AND id = 3;
    // SELECT * FROM STUDENT,COURSE WHERE ((SSEX=0) AND (CID=1)) OR id = 3;
    // SELECT * FROM STUDENT,COURSE WHERE a=0 AND b=1 OR c=2 AND d=3 OR e=4 AND f=6 OR g=7 AND h=8;
    // 遍历列名
    /**
     * select 要做的事情
     * 1. 检查当前系统状态是否为 STATE_DB
     * 2. 检查表名是否存在
     * 3. 检查列名是否存在/或者是*表示列全选
     * 4. 检查条件结点的列名是否存在
     * 5. 从表名对应的文件读取一个页
     * 6. 如果有条件有主键，该页的最小和最大记录是否符合，不符合则再读下一个页
     * 7. 如果条件没有主键，遍历每一行记录，存到结果中
     * 8. 输出结果
     */
    // Step 1: Check current system state
    if (this->currentState != STATE_DB) {
        std::cerr << "[INFO] No database selected" << std::endl;
        return;
    }

    // Step 2: Check if table names exist
    tableNode* tableHead = node->tables;
    while (tableHead != nullptr) {
        if (!tableExists(tableHead->tableName)) {
            std::cerr << "Table '" << tableHead->tableName << "' does not exist" << std::endl;
            return;
        }
        tableHead = tableHead->next;
    }

    // Step 3: Determine columns to select
    std::vector<std::string> columnsToSelect;
    columnNode* columnHead = node->columnNames;
    tableHead = node->tables;

    if (columnHead->columnName == "*") {
        // Select all columns except 'id'
        Pager pager(tableFiles[tableHead->tableName]);
        Pager* page = pager.readPage(0);

        for (const auto& column : page->fileHeader.columnOffset) {
            if (column.first != "id") {
                columnsToSelect.push_back(column.first);
            }
        }
    } else {
        // Select specified columns
        while (columnHead != nullptr && tableHead != nullptr) {
            if (columnHead->columnName != "*") {
                if (!columnExists(tableHead->tableName, columnHead->columnName)) {
                    std::cerr << "Column '" << columnHead->columnName << "' does not exist in table '" << tableHead->tableName << "'" << std::endl;
                    return;
                }
                if (columnHead->columnName != "id") {
                    columnsToSelect.push_back(columnHead->columnName);
                }
            }
            columnHead = columnHead->next;
        }
    }

    // Step 4: Retrieve records and apply conditions
    tableHead = node->tables;
    while (tableHead != nullptr) {
        std::string tablePath = tableFiles[tableHead->tableName];
        Pager pager(tablePath);

//        std::cout << "currentPage->path : " << this->currentPage->path << std::endl;
//        std::cout << "tableName: " << tableHead->tableName << std::endl;
//        std::cout << "tableName: " << this->dataPath << std::endl;
        Pager* page = (this->currentPage && this->currentPage->path == this->dataPath + "/" + this->currentDatabase + "\\" + tableHead->tableName + ".txt")
                      ? this->currentPage
                      : pager.readPage(0);

        while (page) {
            for (const Record& record : page->records) {
                if (evaluateCondition(record, node->conditions, page->fileHeader.columnOffset)) {
                    // Print the selected columns
                    if (node->columnNames->columnName == "*") {
                        std::cout << record.data;
                    } else {
                        // Print the selected columns
                        std::istringstream dataStream(record.data);
                        std::string value;
                        std::vector<std::string> values;

                        // Fill values vector with data from record
                        while (std::getline(dataStream, value, ',')) {
                            values.push_back(value);
                        }

                        bool firstColumn = true;
                        for (const auto& columnName : columnsToSelect) {
                            int offset = page->fileHeader.columnOffset[columnName] - 1;
                            if (!firstColumn) {
                                std::cout << ",";
                            }
                            std::cout << values[offset];
                            firstColumn = false;
                        }
                    }
                    std::cout << std::endl;
                }
            }

            // Move to the next page if exists and not the current page
            if (page->fileHeader.nextPage != -1 && page != this->currentPage) {
                page = pager.readPage(page->fileHeader.nextPage);
            } else {
                break;
            }
        }

        tableHead = tableHead->next;
    }
}

bool Database::tableExists(const std::string& tableName) {
    return tableFiles.find(tableName) != tableFiles.end();
}

bool Database::columnExists(const std::string& tableName, const std::string& columnName) {
    std::ifstream tableFile(tableFiles[tableName]);
    if (!tableFile.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(tableFile, line)) {
        if (line.find("Columns:") != std::string::npos) {
            return line.find(columnName) != std::string::npos;
        }
    }
    return false;
}

bool Database::evaluateCondition(const Record& record, conditionNode* condition, const std::unordered_map<std::string, int>& columnOffset) {
    if (!condition) {
        return true; // No condition means always true
    }

    if (condition->left && condition->right) {
        bool leftResult = evaluateCondition(record, condition->left, columnOffset);
        bool rightResult = evaluateCondition(record, condition->right, columnOffset);
        if (condition->op == conditionNode::AND) {
            return leftResult && rightResult;
        } else if (condition->op == conditionNode::OR) {
            return leftResult || rightResult;
        }
    } else {
        // This is a leaf node with an actual condition
        std::istringstream recordData(record.data);
        std::string value;
        int currentOffset = 1;
        while (std::getline(recordData, value, ',')) {
            if (currentOffset == columnOffset.at(condition->columnName)) {
                if (condition->rightOperandType == conditionNode::INT) {
                    int recordValue = std::stoi(value);
                    switch (condition->op) {
                        case conditionNode::EQUAL:
                            return recordValue == condition->intval;
                        case conditionNode::NOT_EQUAL:
                            return recordValue != condition->intval;
                        case conditionNode::GREATER:
                            return recordValue > condition->intval;
                        case conditionNode::LESS:
                            return recordValue < condition->intval;
                        default:
                            return false;
                    }
                } else if (condition->rightOperandType == conditionNode::STRING) {
                    switch (condition->op) {
                        case conditionNode::EQUAL:
                            return value == condition->chval;
                        case conditionNode::NOT_EQUAL:
                            return value != condition->chval;
                        default:
                            return false;
                    }
                }
            }
            ++currentOffset;
        }
    }

    return false;
}

// 递归遍历条件节点
void Database::traverseConditions(struct conditionNode* node) {
    if (!node) return;

    if (node->op == conditionNode::AND || node->op == conditionNode::OR) {
        traverseConditions(node->left);
        if (node->op == conditionNode::AND) {
            std::cout << "AND" << std::endl;
        } else {
            std::cout << "OR" << std::endl;
        }
        traverseConditions(node->right);
    } else {
        std::cout << "columnName: " << node->columnName << std::endl;
        switch (node->op) {
            case conditionNode::EQUAL:
                std::cout << "EQUAL" << std::endl;
                break;
            case conditionNode::GREATER:
                std::cout << "GREATER" << std::endl;
                break;
            case conditionNode::LESS:
                std::cout << "LESS" << std::endl;
                break;
            case conditionNode::NOT_EQUAL:
                std::cout << "NOT_EQUAL" << std::endl;
                break;
            default:
                break;
        }

        if (node->rightOperandType == conditionNode::INT) {
            std::cout << "right operand: " << node->intval << std::endl;
        } else if (node->rightOperandType == conditionNode::STRING) {
            std::cout << "right operand: " << node->chval << std::endl;
        }
    }
}




void Database::insert(struct insertNode *node) {
    // TEST: insert into testTable (id,name,score) values(10,'chen',100);
    // insert into testTable values(10,'chen',100);
    // insert into student(sname,sage,ssex) values('CHEN',20,0);
    /**
     * insert 要做的事情
     * 1. 检查系统状态
     * 2. 检查表名是否存在
     * 3. 检查当前页是否满，满则读下一页（从外存换页，要检查页是否DIRTY，如果DIRTY则要写回外存）
     * 4. 未满则在当前页（内存）写入，并标记DIRTY
     * 5. 检查是否指定列名struct columnNode* columnNames，若为空，则按照值全部插入；若不为空，未指定的列名写入null
     * 6. 写入一行记录成功，暂时不写回外存，只有下一次换页才写回外存 (COW)
     */
    // Step 1: Check current system state
    if (this->currentState != STATE_DB) {
        std::cerr << "[INFO] No database selected" << std::endl;
        return;
    }

    // Step 2: Check if table exists
    if (!tableExists(node->tableName)) {
        std::cerr << "Table '" << node->tableName << "' does not exist" << std::endl;
        return;
    }

    // Step 3: Check if current page is full
    std::string tablePath = tableFiles[node->tableName];
    Pager pager(tablePath);
    this->currentPage = pager.readPage(0);

    while (this->currentPage->isFull()) {
        if (this->currentPage->fileHeader.nextPage != -1) {

            if (this->currentPage->isDirty) {
                // Write back the dirty page to disk
                this->currentPage->writePage();
            }
            this->currentPage = pager.readPage(this->currentPage->fileHeader.nextPage);
        } else {

            // Check if current page is dirty
            if (this->currentPage->isDirty) {
                // Write back the dirty page to disk
                this->currentPage->writePage();
            }

            // Create a new page
            Pager* newPage = new Pager(tablePath);
            newPage->fileHeader.pageNumber = this->currentPage->fileHeader.pageNumber + 1;
            newPage->fileHeader.prevPage = this->currentPage->fileHeader.pageNumber;
            this->currentPage->fileHeader.nextPage = newPage->fileHeader.pageNumber;
            newPage->fileHeader.nextPage = -1;
            newPage->isDirty = true;

            // Update current page and write the new page
            this->currentPage->writePage();
            this->currentPage = newPage;
        }
    }

    // Step 5: Write record into current page
    Record newRecord;
    this->currentPage->Supermum++;
    newRecord.id = this->currentPage->Supermum;
    newRecord.nextOffset = 1;

    std::ostringstream recordStream;
    struct valueNode* value = node->values;

    if (node->columnNames) {
        // Insert values based on specified columns
        std::unordered_map<std::string, std::string> columnValues;
        struct columnNode* colNode = node->columnNames;

        while (colNode && value) {
            if (value->type == valueNode::INT) {
                columnValues[colNode->columnName] = std::to_string(value->intval);
            } else {
                columnValues[colNode->columnName] = value->chval;
            }
            colNode = colNode->next;
            value = value->next;
        }

        // Sort columns by their offset value
        std::vector<std::pair<std::string, int>> sortedColumns(this->currentPage->fileHeader.columnOffset.begin(), this->currentPage->fileHeader.columnOffset.end());
        std::sort(sortedColumns.begin(), sortedColumns.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second < b.second;
        });

        // Fill missing columns with "null"
        for (const auto& col : sortedColumns) {
            if (col.first == "id") continue;
            if (columnValues.find(col.first) != columnValues.end()) {
                recordStream << columnValues[col.first];
            } else {
                recordStream << "null";
            }
            recordStream << ",";
        }
    } else {
        // Insert all values sequentially
        while (value) {
            if (value->type == valueNode::INT) {
                recordStream << value->intval;
            } else {
                recordStream << value->chval;
            }
            recordStream << ",";
            value = value->next;
        }
    }

    newRecord.data = recordStream.str();
    newRecord.data.pop_back(); // Remove trailing comma
    this->currentPage->records.push_back(newRecord);
    this->currentPage->pageHeader.recordsCount++;
    this->currentPage->isDirty = true;
}

void Database::update(struct updateNode *node) { // 应该是assignment有问题
    // TEST: UPDATE testTable SET id=3,name='chen',score=100 WHERE money='infinity';
    // UPDATE testTable SET id=3,score=100 WHERE money='1';
    // UPDATE testTable SET id=3,score='100' WHERE money='1' AND id = 3 AND id = '4';
    // 打印表名
    std::cout << "Table Name: " << node->tableName << std::endl;

    // 遍历赋值
    std::cout << "Traverse Assignments" << std::endl;
    struct assignmentNode* assignmentHead = node->assignments;
    while (assignmentHead != nullptr) {
        std::cout << "Column name: " << assignmentHead->columnName << std::endl;
        if (assignmentHead->type == assignmentNode::INT) {
            std::cout << "INT value: " << assignmentHead->intval << std::endl;
        } else if (assignmentHead->type == assignmentNode::STRING) {
            std::cout << "STRING value: " << assignmentHead->chval << std::endl;
        }
        assignmentHead = assignmentHead->next;
    }

    // 遍历条件
    std::cout << "Traverse Conditions" << std::endl;
    traverseConditions(node->conditions);
}

void Database::deleteFrom(struct deleteNode *node) {
    // TEST: DELETE FROM STUDENT WHERE (SAGE>21) AND (SSEX=0);
    // print tableName
    std::cout << "Table Name: " << node->tableName << std::endl;

    struct conditionNode* conditionHead = node->conditions;
    if(conditionHead != nullptr){
        traverseConditions(conditionHead);
    }
}

