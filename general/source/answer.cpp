#include "answer.h"
#include "protocol.h"


namespace Answer {
    std::string GetMail::SerializeForTransfer() const {
        Protocol::Answer answer_proto;
        for (const Letter& letter : mail) {
            answer_proto.fields.push_back(letter.from);
            answer_proto.fields.push_back(letter.to);
            answer_proto.fields.push_back(letter.body);
        }
        return answer_proto.Serialize();
    }

    std::string SendLetter::SerializeForTransfer() const {
        return Protocol::Answer{}.Serialize();
    }

    std::string Authorize::SerializeForTransfer() const {

    }
}