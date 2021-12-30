#pragma once

#include "letter.h"
#include "protocol.h"

#include <vector>
#include <variant>


namespace Answer {
    struct Authorize {
        bool authorization_succeed;

        static Authorize DeserializeTransfer(std::string);
    };

    struct GetMail {
        std::vector<Letter> letters;
    };

    struct SendLetter {

    };

    struct Terminate {

    };

    struct Error {
        std::string error_message;
    };

    using Any = std::variant<Authorize, GetMail, SendLetter, Terminate, Error>;

    std::string SerializeForTransfer(Any);
    template<class ExpectedT>
    Any DeserializeTransfer(std::string transfer) {
        auto [error_message, body] = Protocol::DeserializeAnswer(transfer);
        if (!error_message.empty()) {
            return Error{error_message};
        }
        return ExpectedT::DeserializeTransfer(transfer);
    }
}
