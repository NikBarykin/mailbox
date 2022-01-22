#pragma once

#include <iostream>

// NOTE: client is just an interface. It doesn't do any work by itself
// NOTE: there is no class Session because session is a process not an entity

struct SessionState {
    std::string user_login;
    bool running = true;
};

void RunSession(std::string nodename, std::string servname,
                std::istream& input = std::cin,
                std::ostream& output = std::cout);
