//
// Created by Iamnotphage on 2024-05-24.
//

#include "Pager.h"
#include <iostream>
#include <sstream>

Pager::Pager(const std::string& path) {
    this->path = path;
    this->Infimum = DEFAULT_INFIMUM;
    this->Supermum = DEFAULT_SUPERMUM;
    this->isDirty = false;
}

Pager* Pager::readPage(int ID) {
    std::ifstream tableFile;
    tableFile.open(path);
    if (!tableFile) {
        std::cerr << "Failed to open table file: " << path << std::endl;
        return nullptr;
    }

    auto* page = new Pager(this->path);

    bool pageDirectoryRead = false;
    std::string line;
    while (std::getline(tableFile, line)) {
        std::istringstream iss(line);
        std::string key;
        if (!pageDirectoryRead) {
            std::getline(iss, key, ':');
            if (key == "PageNumber") {
                iss >> page->fileHeader.pageNumber;
            } else if (key == "prevPage") {
                iss >> page->fileHeader.prevPage;
            } else if (key == "nextPage") {
                iss >> page->fileHeader.nextPage;
            } else if (key == "Columns") {
                std::string columns;
                std::getline(iss, columns);
                std::istringstream colStream(columns);
                std::string col;
                while (std::getline(colStream, col, ',')) {
                    auto pos = col.find(':');
                    if (pos != std::string::npos) {
                        std::string name = col.substr(0, pos);
                        int offset = std::stoi(col.substr(pos + 1));
                        page->fileHeader.columnOffset[name] = offset;
                    }
                }
            } else if (key == "recordsCount") {
                iss >> page->pageHeader.recordsCount;
            } else if (key == "pageState") {
                iss >> page->pageHeader.pageState;
            } else if (key == "Infimum") {
                iss >> page->Infimum;
            } else if (key == "Supermum") {
                iss >> page->Supermum;
            } else if (key == "PageDirectory") {
                int offset;
                while (iss >> offset) {
                    page->pageDirectory.push_back(offset);
                }
                pageDirectoryRead = true;
            }
        } else {
            // This line is a record
            // std::cout << "TEST" << std::endl;
            std::istringstream recordStream(line);
            Record record;
            char delimiter;

            recordStream >> record.nextOffset >> delimiter >> record.id >> delimiter;
            std::getline(recordStream, record.data);

            page->records.push_back(record);
        }
    }

//    std::cout << "TEST" << std::endl;
//    for(const Record& r : page->records){
//        std::cout << r.id << std::endl;
//        std::cout << r.data << std::endl;
//    }

    tableFile.close();
    return page;
}

void Pager::writePage() {

}