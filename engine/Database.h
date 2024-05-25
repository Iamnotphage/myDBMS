//
// Created by Iamnotphage on 2024-05-24.
//

#ifndef MYDBMS_DATABASE_H
#define MYDBMS_DATABASE_H

#include <string>

#define STATE_SYS 0
#define STATE_DB 1
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
     * @brief USE DATBASE;
     *
     * Change the current view into the specific datbase.
     *
     * @param databaseName the name of database.
     */
    void useDatabase(std::string databaseName);
private:
    int currentState;
};


#endif //MYDBMS_DATABASE_H
