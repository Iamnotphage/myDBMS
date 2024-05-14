//
// Created by Iamnotphage on 2024-05-14.
//

#ifndef MYDBMS_SHELL_H
#define MYDBMS_SHELL_H


class shell {
public:
    shell();
    void start();
    ~shell();

private:
    char* input;
    void init();
    void printPrompt();
};


#endif //MYDBMS_SHELL_H
