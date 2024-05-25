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
        if(exists(dataPath) && is_directory(dataPath)){
            for(const directory_entry& entry : directory_iterator(dataPath)){
                if(entry.is_directory()){
                    if(databaseName == entry.path().filename().string())return true;
                }
            }
        }
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
        remove_all(dataPath + "/" + databaseName);

        // Exexute Time
        clock_t endTime = clock();
        std::cout << "Query OK, 0 rows in affected ("
                  << std::fixed << std::setprecision(2)
                  << (double)(endTime - startTime)/CLOCKS_PER_SEC
                  << " sec)" << std::endl;

    }catch(const filesystem_error& e){
        std::cerr << "[INFO] " << e.what() << std::endl;
    }

}

