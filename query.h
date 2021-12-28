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
        using AnsT = Answer::GetMail;
        Letter letter;
        std::string SerializeForTransfer() const;
        static SendLetter DeserializeTransfer();
    };

    struct Authorize {
        using AnsT = Answer::GetMail;
        std::string login, password;
        std::string SerializeForTransfer() const;
    };

    struct Terminate {
        using AnsT = Answer::GetMail;

    };

    using QType = std::variant<GetMail, SendLetter, Authorize, Terminate>;

    QType DeserializeTransfer(std::string);
    std::string SerializeForTransfer(QType);
}



//static Query ParseTransfered(std::string);
