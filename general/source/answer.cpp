#include "answer.h"
#include "protocol.h"


namespace Answer {
    std::string GetMail::SerializeForTransfer() const {
        return Protocol::Answer{}.Serialize();
    }

    std::string SendLetter::SerializeForTransfer() const {

    }
}