#pragma once

#include "letter.h"

#include <string>
#include <variant>


namespace Query {
    // TODO: design similarity by inheritance, not by std::variant
    struct GetMail {
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
    };

    using QueryT = std::variant<GetMail, SendLetter, Authorize>;

    QueryT DeserializeTransfer(std::string);
    std::string SerializeForTransfer(QueryT);
}



//static Query ParseTransfered(std::string);
