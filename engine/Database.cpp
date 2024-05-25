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
    const std::string dataPath = "../data";

    try{
        if(exists(dataPath) && is_directory(dataPath)){

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

        }else{
            std::cout << "[INFO] Database path doesn't exits or is not a directory." << std::endl;
        }
    }catch(const filesystem_error& e){
        std::cerr << "[INFO] " << e.what() << std::endl;
    }catch(const std::exception& e){
        std::cerr << "[INFO] " << e.what() << std::endl;
    }
}

void Database::useDatabase(const std::string databaseName) {
    std::cout << "Datbase changed." << std::endl;
    std::cout << "TEST: " << databaseName << std::endl;
}

