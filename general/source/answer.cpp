#include "answer.h"
#include "protocol.h"

#include <cassert>


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

    GetMail GetMail::DeserializeTransfer(std::string serialized) {
        const auto& ans_fields = Protocol::Answer::Deserialize(serialized).fields;
        assert(ans_fields.size() % 3 == 0);
        std::vector<Letter> mail(ans_fields.size() / 3);
        for (size_t letter_i = 0; letter_i < mail.size(); ++letter_i) {
            mail[letter_i].from = ans_fields[letter_i * 3];
            mail[letter_i].to = ans_fields[letter_i * 3 + 1];
            mail[letter_i].body = ans_fields[letter_i * 3 + 2];
        }
        return {mail};
    }


    std::string SendLetter::SerializeForTransfer() const {
        return Protocol::Answer{}.Serialize();
    }

    SendLetter SendLetter::DeserializeTransfer(std::string serialized) {
        return {};
    }


    std::string Authorize::SerializeForTransfer() const {
        Protocol::Answer answer_proto;
        answer_proto.fields.push_back(std::to_string(authorization_succeed));
        return answer_proto.Serialize();
    }

    Authorize Authorize::DeserializeTransfer(std::string serialized) {
        auto answer_proto = Protocol::Answer::Deserialize(serialized);
        return {answer_proto.fields[0] == "1"};
    }


    std::string Terminate::SerializeForTransfer() const {
        return Protocol::Answer{}.Serialize();
    }

    Terminate Terminate::DeserializeTransfer(std::string serialized) {
        return {};
    }


    std::string Error::SerializeForTransfer() const {
        return Protocol::Answer{message}.Serialize();
    }

    Error Error::DeserializeTransfer(std::string serialized) {
        auto answer_proto = Protocol::Answer::Deserialize(serialized);
        return {answer_proto.error_message};
    }

    std::string SerializeForTransfer(Any answer) {
        auto answer_serializer = [](auto answer) {
            return answer.SerializeForTransfer();
        };
        return std::visit(answer_serializer, answer);
    }
}
