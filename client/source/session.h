#pragma once

#include <iostream>


// TODO: create class Session and make input and output its members
struct SessionState {
    std::string user_login;
    bool running = true;
};

void RunSession(std::string nodename, std::string servname,
                std::istream& input = std::cin,
                std::ostream& output = std::cout);
