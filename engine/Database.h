//
// Created by Iamnotphage on 2024-05-24.
//

#ifndef MYDBMS_DATABASE_H
#define MYDBMS_DATABASE_H

#include <string>

#define STATE_SYS 0
#define STATE_DB 1

struct columnNode{
    std::string columnName;
    int charLength;
    struct columnNode* next = nullptr;
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
     * @brief CREATE TABLE [tableName] ([columnsDefinitons])
     *
     * @param tableName the name of table.
     * @param columnHead the head of the column nodes.
     */
    void createTable(const std::string& tableName, struct columnNode* columnHead);

private:
    int currentState;
    const std::string dataPath = "../data";
    std::string currentDatabase;

    bool validate(const std::string& databaseName);
};


#endif //MYDBMS_DATABASE_H
