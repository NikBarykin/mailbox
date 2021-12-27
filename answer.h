#pragma once

#include "letter.h"

#include <vector>


namespace Answer {
    struct Authorize {
        bool authorization_succeed;
    };

    struct GetMail {
        std::vector<Letter> mail;
    };

    struct SendMail {

    };

    struct Terminate {

    };
}
