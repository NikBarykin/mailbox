#pragma once

#include "letter.h"
#include "answer.h"

#include <string>
#include <variant>


namespace Query {
    // TODO: maybe design similarity by inheritance, not by std::variant
    struct GetMail {
        static inline const std::string name = "GetMail";
        using Answer = Answer::GetMail;

        std::string letter_filter; // empty => all letters fit

        std::string SerializeForTransfer() const;
        static GetMail DeserializeTransfer(std::string);
    };

    struct SendLetter {
        static inline const std::string name = "SendLetter";
        using Answer = Answer::SendLetter;

        Letter letter;

        std::string SerializeForTransfer() const;
        static SendLetter DeserializeTransfer(std::string);
    };

    struct Authorize {
        static inline const std::string name = "Authorize";
        using Answer = Answer::Authorize;

        std::string login, password;

        std::string SerializeForTransfer() const;
        static Authorize DeserializeTransfer(std::string);
    };

    struct Terminate {
        static inline const std::string name = "Terminate";
        using Answer = Answer::Terminate;

        std::string SerializeForTransfer() const;
        static Terminate DeserializeTransfer(std::string);
    };

    using Any = std::variant<GetMail, SendLetter, Authorize, Terminate>;

    std::string SerializeForTransfer(Any query);
    Any DeserializeTransfer(std::string);

    Answer::Any DeserializeQueryAnswer(Any query, std::string serialized_answer);
}
