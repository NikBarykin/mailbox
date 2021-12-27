#pragma once

#include "letter.h"

#include <vector>
#include <variant>


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

    using AnswerT = std::variant<Authorize, GetMail, SendMail, Terminate>;

    std::string SerializeForTransfer(AnswerT);
    AnswerT DeserializeTransfer(std::string);
}
