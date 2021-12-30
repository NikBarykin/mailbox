#include "query.h"


namespace Query {
    Answer::AnsT DeserializeQueryAnswer(QType query, std::string serialized_answer) {
        auto deserialize_answer_visitor = [&serialized_answer]<class QueryType>(QueryType query) {
            return Answer::DeserializeTransfer<typename QueryType::AnsT>(serialized_answer);
        };
        return std::visit(deserialize_answer_visitor, query);
    }
}
