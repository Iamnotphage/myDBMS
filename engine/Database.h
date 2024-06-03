//
// Created by Iamnotphage on 2024-05-24.
//

#ifndef MYDBMS_DATABASE_H
#define MYDBMS_DATABASE_H

#include <string>
#include <unordered_map>
#include "Pager.h"

#define STATE_SYS 0
#define STATE_DB 1

struct columnNode{
    std::string columnName;
    int charLength;
    struct columnNode* next = nullptr;
};

// for SELECT node;
struct tableNode{
    std::string tableName;
    struct tableNode* next = nullptr;
};

struct conditionNode{
    std::string columnName;
    // 如果op是AND或者OR，说明是一个中间结点，有左右子树，cloumnName和value为空。
    // 如果这个结点是叶子节点，则代表这是一个表达式结点，columnName op value;
    enum op{
        AND, OR, GREATER, LESS, EQUAL, NOT_EQUAL
    }op;
    enum rightOperandType{
        INT, STRING
    }rightOperandType;
    int intval;
    std::string chval;

    struct conditionNode* left = nullptr;
    struct conditionNode* right = nullptr;
};

// SELECT [columnNames] FROM [tables] WHERE [conditions];
struct selectNode{
    struct columnNode* columnNames = nullptr;
    struct tableNode* tables = nullptr;
    struct conditionNode* conditions = nullptr;
};

// for INSERT node;
struct valueNode{
    enum type{
        INT, STRING
    }type;
    int intval;
    std::string chval;
    struct valueNode* next = nullptr;
};

// INSERT INTO [table] ([columnNames]) VALUES ([values]);
// INSERT INTO [table] VALUES ([values]);
struct insertNode{
    std::string tableName;
    struct columnNode* columnNames = nullptr;
    struct valueNode* values = nullptr;
};

// for UPDATE node;
struct assignmentNode{
    std::string columnName;
    enum type{
        INT, STRING
    }type;
    int intval;
    std::string chval;
    struct assignmentNode* next = nullptr;
};

// UPDATE [tableName] SET [assignments] WHERE [conditions];
struct updateNode{
    std::string tableName;
    struct assignmentNode* assignments = nullptr;
    struct conditionNode* conditions = nullptr;
};

// DELETE FROM [tableName];
// DELETE FROM [tableName] WHERE [conditions];
struct deleteNode{
    std::string tableName;
    struct conditionNode* conditions = nullptr;
};

/**
 * @class Database
 * @brief A class to manage and execute SQL command.
 *
 * This class provides API to the front-end.
 *
 * @author Chen
 */
class Database {
public:
    Database();
    ~Database();

    /**
     * @brief SHOW DATABASES;
     *
     * Lists all the databases in data directory.
     */
    void showDatabases();

    /**
     * @brief USE [databaseName];
     *
     * Change the current view into the specific database.
     *
     * @param databaseName the name of database.
     */
    void useDatabase(const std::string& databaseName);

    /**
     * @brief DROP DATABASE [databaseName];
     *
     * Drop the specific database;
     *
     * @param databaseName the name of database.
     */
    void dropDatabase(const std::string& databaseName);

    /**
     * @brief CREATE DATABASE [databaseName];
     *
     * Create a database named [databaseName].
     *
     * @param databaseName the name of database.
     */
    void createDatabase(const std::string& databaseName);

    /**
     * @brief SHOW TABLES;
     *
     * Show all the tables in a database.
     */
    void showTables();

    /**
     * @brief DROP TABLE [tableName];
     *
     * Drop the specific table in a selected database.
     * @param tableName the name of table.
     */
    void dropTable(const std::string& tableName);

    /**
     * @brief CREATE TABLE [tableName] ([columnsDefinitons]);
     *
     * @param tableName the name of table.
     * @param columnHead the head of the column nodes.
     */
    void createTable(const std::string& tableName, struct columnNode* columnHead);

    /**
     * @brief SELECT [columnNames] FROM [tables] WHERE [contidions];
     *
     * @param node the select-node
     */
    void select(struct selectNode* node);

    /**
     * @brief
     * INSERT INTO [tableName] ([columnNames]) VALUES ([values]);
     *
     * INSERT INTO [tableName] VALUES ([values]);
     *
     * @param node the insert-node
     */
    void insert(struct insertNode* node);

    /**
     * @brief UPDATE [tableName] SET [assignments] WHERE [conditions];
     *
     * @param node the update-node
     */
    void update(struct updateNode* node);

    /**
     * @brief
     * DELETE FROM [tableName];
     * DELETE FROM [tableName] WHERE [conditions];
     *
     * @param node the delete-node
     */
    void deleteFrom(struct deleteNode* node);
private:
    int currentState;
    const std::string dataPath = "../data";
    std::string currentDatabase;
    std::unordered_map<std::string, std::string> tableFiles;
    Pager* currentPage; // 当前页 (这里可以改为存放页的某类容器，可以实现LRU)

    bool validate(const std::string& databaseName);

    void traverseConditions(conditionNode *node);

    bool evaluateCondition(const Record &record, conditionNode *condition,
                           const std::unordered_map<std::string, int> &columnOffset);

    bool tableExists(const std::string &tableName);

    bool columnExists(const std::string &tableName, const std::string &columnName);
};


#endif //MYDBMS_DATABASE_H
