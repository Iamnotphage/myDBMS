//
// Created by Iamnotphage on 2024-05-14.
//
#include<iostream>
#include<cstring>
#include "shell.h"

extern "C" {
    int yyparse(void);
    void yy_scan_string(const char* str);
}

shell::shell() {
    init();
}

shell::~shell() {

}

void shell::start() {
    std::string inputLine;
    while(true){
        printPrompt();
        std::getline(std::cin, inputLine);
        if(inputLine == "exit")break;

        input = strdup(inputLine.c_str());

        // lexical analyze
        yy_scan_string(input);

        // grammar analyze
        yyparse();

        free(input);
        input = nullptr;
    }

}

void shell::init() {
    input = nullptr;
    std::cout << "\033[32m\033[1m" <<
               "  /$$$$$$  /$$   /$$ /$$$$$$$$ /$$   /$$      \n"
               " /$$__  $$| $$  | $$| $$_____/| $$$ | $$      \n"
               "| $$  \\__/| $$  | $$| $$      | $$$$| $$      \n"
               "| $$      | $$$$$$$$| $$$$$   | $$ $$ $$      \n"
               "| $$      | $$__  $$| $$__/   | $$  $$$$      \n"
               "| $$    $$| $$  | $$| $$      | $$\\  $$$      \n"
               "|  $$$$$$/| $$  | $$| $$$$$$$$| $$ \\  $$      \n"
               " \\______/ |__/  |__/|________/|__/  \\__/ "
               << "\033[0m" << std:: endl;
}

void shell::printPrompt() {
    std::cout << "\033[34m\033[1m" << "mysql> " << "\033[0m";
}


