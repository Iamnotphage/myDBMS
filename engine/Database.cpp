//
// Created by Iamnotphage on 2024-05-24.
//

#include "Database.h"
#include <iostream>
#include <filesystem>
#include <set>

using namespace std::filesystem;

Database::Database() {
    this->currentState = STATE_SYS;
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

    this->currentDatabase = databaseName;
    this->currentState = STATE_DB;

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
    std::string tablePath = dataPath + "/" + this->currentDatabase + "/" + tableName;
    if(exists(tablePath) && is_regular_file(tablePath)){
        std::cout << "Table '" << tableName << "' already exists" << std::endl;
        return;
    }

    struct columnNode* head = columnHead;
    std::cout << "Table name: " << tableName << std::endl;
    // create table testCreate (id INT, name CHAR(10), score INT);
    while(head != nullptr){
        std::cout << "column name: " << head->columnName << std::endl << "char length: " << head->charLength << std::endl;
        head = head->next;
    }



    // Free the nodes.
    head = columnHead;
    while(head != nullptr){
        columnNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

void Database::select(struct selectNode *node) {
    // TEST DATA: select id,name,test from sys_test,sys_test2 where id = 5 AND name = 'shit';
    // 遍历列名
    std::cout << "Traverse Column Names" << std::endl;
    struct columnNode* columnHead = node->columnNames;
    while(columnHead != nullptr){
        std::cout << "column name: " << columnHead->columnName << std::endl;
        columnHead = columnHead->next;
    }

    // 遍历表名
    std::cout << "Traverse Table Names" << std::endl;
    struct tableNode* tableHead = node->tables;
    while(tableHead != nullptr){
        std::cout << "table name: " << tableHead->tableName << std::endl;
        tableHead = tableHead->next;
    }

    // 遍历条件
    std::cout << "Traverse Conditions" << std::endl;
    struct conditionNode* conditionHead = node->conditions;

    traverseConditions(conditionHead);
}

// 递归遍历条件节点
void Database::traverseConditions(struct conditionNode* node) {
    if (node == nullptr) {
        return;
    }
    // 处理左子节点
    if (node->left != nullptr) {
        traverseConditions(node->left);
    }

    // 处理当前节点
    switch (node->op) {
        case conditionNode::AND:
            std::cout << "AND" << std::endl;
            break;
        case conditionNode::OR:
            std::cout << "OR" << std::endl;
            break;
        case conditionNode::EQUAL:
            std::cout << "EQUAL" << std::endl;
            break;
        case conditionNode::GREATER:
            std::cout << "GREATER" << std::endl;
            break;
        case conditionNode::LESS:
            std::cout << "LESS" << std::endl;
            break;
        case conditionNode::NOT:
            std::cout << "NOT" << std::endl;
            break;
    }

    if (node->type == conditionNode::INT) {
        std::cout << "INT value: " << node->intval << std::endl;
    } else if (node->type == conditionNode::STRING) {
        std::cout << "STRING value: " << node->chval << std::endl;
    }

    // 处理右子节点
    if (node->right != nullptr) {
        traverseConditions(node->right);
    }
}

void Database::insert(struct insertNode *node) {
    // TEST: insert into testTable (id,name,score) values(10,'chen',100);
    // insert into testTable values(10,'chen',100);
    // 打印表名
    std::cout << "Table Name: " << node->tableName << std::endl;

    // 遍历列名
    std::cout << "Traverse Column Names" << std::endl;
    struct columnNode* columnHead = node->columnNames;
    while (columnHead != nullptr) {
        std::cout << "Column name: " << columnHead->columnName << std::endl;
        columnHead = columnHead->next;
    }

    // 遍历值
    std::cout << "Traverse Values" << std::endl;
    struct valueNode* valueHead = node->values;
    while (valueHead != nullptr) {
        if (valueHead->type == valueNode::INT) {
            std::cout << "INT value: " << valueHead->intval << std::endl;
        } else if (valueHead->type == valueNode::STRING) {
            std::cout << "STRING value: " << valueHead->chval << std::endl;
        }
        valueHead = valueHead->next;
    }
}

