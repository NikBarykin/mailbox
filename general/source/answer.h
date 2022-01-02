#pragma once

#include "letter.h"
#include "protocol.h"

#include <vector>
#include <variant>


namespace Answer {
    struct Authorize {
        bool authorization_succeed;

        std::string SerializeForTransfer() const;
        static Authorize DeserializeTransfer(std::string);
    };

    struct GetMail {
        std::vector<Letter> mail;

        std::string SerializeForTransfer() const;
        static GetMail DeserializeTransfer(std::string);
    };

    struct SendLetter {
        std::string SerializeForTransfer() const;
        static SendLetter DeserializeTransfer(std::string);
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
        Protocol::Answer ans_proto = Protocol::Answer::Deserialize(serialized_ans);
        if (!ans_proto.error_message.empty()) {
            return Error{ans_proto.error_message};
        }
        return ExpectedAnswer::DeserializeTransfer(ans_proto.body);
    }
}
