#pragma once

#include "letter.h"
#include "protocol.h"

#include <vector>
#include <variant>


namespace Answer {
    struct GetMail {
        std::vector<Letter> mail;

        std::string SerializeForTransfer() const;
        static GetMail DeserializeTransfer(std::string);
    };

    struct SendLetter {
        std::string SerializeForTransfer() const;
        static SendLetter DeserializeTransfer(std::string);
    };

    struct Authorize {
        bool authorization_succeed;

        std::string SerializeForTransfer() const;
        static Authorize DeserializeTransfer(std::string);
    };

    struct Terminate {
        std::string SerializeForTransfer() const;
        static Terminate DeserializeTransfer(std::string);
    };

    struct Error {
        std::string message;

        std::string SerializeForTransfer() const;
        static Error DeserializeTransfer(std::string);
    };

    using Any = std::variant<Authorize, GetMail, SendLetter, Terminate, Error>;

    std::string SerializeForTransfer(Any);

    template<class ExpectedAnswer>
    Any DeserializeTransfer(std::string);
}


// Implementations

namespace Answer {
    template<class ExpectedAnswer>
    Any DeserializeTransfer(std::string serialized_ans) {
        auto error_message = Protocol::Answer::Deserialize(serialized_ans).error_message;
        if (!error_message.empty()) {
            return Error::DeserializeTransfer(serialized_ans);
        }
        return ExpectedAnswer::DeserializeTransfer(serialized_ans);
    }
}
