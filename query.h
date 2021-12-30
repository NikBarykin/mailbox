#pragma once

#include "letter.h"
#include "answer.h"

#include <string>
#include <variant>


namespace Query {
    // TODO: design similarity by inheritance, not by std::variant
    struct GetMail {
        using Any = Answer::GetMail;
        std::string SerializeForTransfer() const {

        }
        static GetMail DeserializeTransfer(std::string);
    };

    struct SendLetter {
        using Any = Answer::GetMail;
        Letter letter;
        std::string SerializeForTransfer() const;
        static SendLetter DeserializeTransfer();
    };

    struct Authorize {
        using Any = Answer::GetMail;
        std::string login, password;
        std::string SerializeForTransfer() const;
    };

    struct Terminate {
        using Any = Answer::GetMail;

    };

    using Any = std::variant<GetMail, SendLetter, Authorize, Terminate>;

    Any DeserializeTransfer(std::string);
    std::string SerializeForTransfer(Any);

    Answer::Any DeserializeQueryAnswer(Any, std::string serialized_answer);
}
