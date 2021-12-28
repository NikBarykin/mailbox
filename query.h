#pragma once

#include "letter.h"
#include "answer.h"

#include <string>
#include <variant>


namespace Query {
    // TODO: design similarity by inheritance, not by std::variant
    struct GetMail {
        using AnsT = Answer::GetMail;
        std::string SerializeForTransfer() const {

        }
        static GetMail DeserializeTransfer(std::string);
    };

    struct SendLetter {
        Letter letter;
        std::string SerializeForTransfer() const;
        static SendLetter DeserializeTransfer();
    };

    struct Authorize {
        std::string login, password;
        std::string SerializeForTransfer() const;
    };

    struct Terminate {

    };

    using QType = std::variant<GetMail, SendLetter, Authorize, Terminate>;

    QueryT DeserializeTransfer(std::string);
    std::string SerializeForTransfer(QueryT);
}



//static Query ParseTransfered(std::string);
