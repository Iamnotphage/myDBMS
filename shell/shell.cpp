//
// Created by Iamnotphage on 2024-05-14.
//
#include<iostream>
#include<cstring>
#include "shell.h"

// lex和YACC生成C文件就用下面的
//extern "C" {
//    int yyparse(void);
//    void yy_scan_string(const char* str);
//}

int yyparse();
void yy_scan_string(const char* str);


shell::shell() {
    init();
}

shell::~shell() = default;

void shell::start() {
    std::string inputLine;
    std::string commandBuffer;

    while(true) {
        printPrompt(commandBuffer.empty());
        std::getline(std::cin, inputLine);

        // Append the input line to the command buffer
        commandBuffer += inputLine;

        // If just a '\n'
        if(commandBuffer.empty())continue;

        // If EXIT;
        if(commandBuffer == "EXIT" || commandBuffer == "exit") {
            printf("Bye");
            break;
        }

        // If the command ends with a semicolon, process it
        if(commandBuffer.back() == ';') {

            char* input = strdup(commandBuffer.c_str());
            commandBuffer.clear();

            // Tokenizer
            yy_scan_string(input);

            // Parser
            int state = yyparse();

            if(state == 0) {
                // ACCEPT
                // printf("[INFO] statement accept.\n");
            } else if(state == 1) {
                // ABORT
                printf("[INFO] grammar wrong.\n");
            } else if(state == 2) {
                // NO-MEMORY
                printf("[INFO] no memory.\n");
            }

            free(input);
        } else {
            // Continue to next line input
            commandBuffer += "\n";
        }
    }

}

void shell::init() {
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

void shell::printPrompt(bool newCommand) {
    if(newCommand) {
        std::cout << "\033[34m\033[1m" << "mysql> " << "\033[0m";
    } else {
        std::cout << "\033[34m\033[1m" << "    -> " << "\033[0m";
    }

}


