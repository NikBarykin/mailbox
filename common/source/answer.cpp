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
            answer_proto.fields.push_back(letter.date.AsString());
        }
        return answer_proto.Serialize();
    }

    GetMail GetMail::DeserializeTransfer(std::string serialized) {
        static constexpr int LETTER_FIELDS_N = 4;

        const auto& ans_fields = Protocol::Answer::Deserialize(std::move(serialized)).fields;

        assert(ans_fields.size() % LETTER_FIELDS_N == 0);
        size_t n_letters = ans_fields.size() / LETTER_FIELDS_N;

        std::vector<Letter> mail;
        mail.reserve(n_letters);
        for (size_t letter_i = 0; letter_i < n_letters; ++letter_i) {
            mail.push_back({ans_fields[letter_i * LETTER_FIELDS_N],
                            ans_fields[letter_i * LETTER_FIELDS_N + 1],
                            ans_fields[letter_i * LETTER_FIELDS_N + 2],
                            Date::ParseFrom(ans_fields[letter_i * LETTER_FIELDS_N + 3])});
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
        answer_proto.fields.push_back(authorized_login);
        return answer_proto.Serialize();
    }

    Authorize Authorize::DeserializeTransfer(std::string serialized) {
        auto answer_proto = Protocol::Answer::Deserialize(serialized);
        return {answer_proto.fields[0]};
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
