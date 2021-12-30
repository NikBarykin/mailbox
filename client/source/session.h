#pragma once

#include <iostream>


struct SessionState {
    std::string user_login;
    bool running = true;
};

void RunSession(std::istream& input = std::cin,
                std::ostream& output = std::cout);
