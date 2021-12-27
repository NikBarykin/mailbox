#pragma once

#include "letter.h"

#include <string>


namespace Query {

    class Base {
        virtual std::string TransferString() const = 0;
    };

    class Authorize : public Base {
        std::string login, password;
    };

    class GetMail : public  Base {

    };

    class SendMail : public Base {
        Letter letter;
    };

    class Terminate : public Base {

    };

    Base& ParseTransfer(std::string);
}



//static Query ParseTransfered(std::string);
