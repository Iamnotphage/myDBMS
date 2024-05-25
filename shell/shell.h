//
// Created by Iamnotphage on 2024-05-14.
//

#ifndef MYDBMS_SHELL_H
#define MYDBMS_SHELL_H

/**
 * @class shell
 * @brief A class to interact with user.
 *
 * User can input SQL commands, and the shell will
 * send them to the compiler.
 *
 * @author Chen
 */
class shell {
public:
    shell();
    void start();
    ~shell();

private:
    void init();
    void printPrompt(bool);
};


#endif //MYDBMS_SHELL_H
