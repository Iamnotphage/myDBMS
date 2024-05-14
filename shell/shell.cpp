//
// Created by Iamnotphage on 2024-05-14.
//
#include<iostream>
#include "shell.h"

shell::shell() {

}

shell::~shell() {

}

void shell::start() {
    init();

    do{
        printPrompt();
        std::cin>> input;

    }while(input[0] != 'e');

}

void shell::init() {
    input = (char *)malloc((sizeof(char)) * 256);
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
    std::cout << "\033[34m\033[1m" << "mysql > " << "\033[0m";
}


