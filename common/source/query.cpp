#include "query.h"
#include "protocol.h"

#include <stdexcept>
#include <cassert>


// TODO: optimize by passing references or moving
namespace Query {
    std::string GetMail::SerializeForTransfer() const {
        return Protocol::Query{name, {letter_filter}}.Serialize();
    }

    GetMail GetMail::DeserializeTransfer(std::string serialized_query) {
        auto query_proto = Protocol::Query::Deserialize(serialized_query);
        assert(query_proto.name == name);
        return {query_proto.arguments[0]};
    }


    std::string SendLetter::SerializeForTransfer() const {
        Protocol::Query query_proto{name, {
            letter.from, letter.to, letter.body, letter.date.AsString()}};
        return query_proto.Serialize();
    }

    SendLetter SendLetter::DeserializeTransfer(std::string serialized_query) {
        auto query_proto = Protocol::Query::Deserialize(serialized_query);
        assert(query_proto.name == name);
        const auto& arguments = query_proto.arguments;
        return {{arguments[0], arguments[1], arguments[2], Date::ParseFrom(arguments[3])}};
    }


    std::string Authorize::SerializeForTransfer() const {
        return Protocol::Query{name, {login, password}}.Serialize();
    }

    Authorize Authorize::DeserializeTransfer(std::string serialized_query) {
        auto query_proto = Protocol::Query::Deserialize(serialized_query);
        assert(query_proto.name == name);
        return {query_proto.arguments[0], query_proto.arguments[1]};
    }


    std::string Terminate::SerializeForTransfer() const {
        return Protocol::Query{name}.Serialize();
    }

    Terminate Terminate::DeserializeTransfer(std::string serialized_query) {
        auto query_proto = Protocol::Query::Deserialize(serialized_query);
        assert(query_proto.name == name);
        return {};
    }


    std::string SerializeForTransfer(Any query) {
        auto query_serializer = [](auto query) {
            return query.SerializeForTransfer();
        };
        return std::visit(query_serializer, query);
    }

    Any DeserializeTransfer(std::string serialized_query) {
        std::string query_name = Protocol::Query::Deserialize(serialized_query).name;
        if (query_name == GetMail::name) {
            return GetMail::DeserializeTransfer(serialized_query);
        } else if (query_name == SendLetter::name) {
            return SendLetter::DeserializeTransfer(serialized_query);
        } else if (query_name == Authorize::name) {
            return Authorize::DeserializeTransfer(serialized_query);
        } else if (query_name == Terminate::name) {
            return Terminate::DeserializeTransfer(serialized_query);
        } else {
            throw std::runtime_error("Invalid query name: " + query_name);
        }
    }

    Answer::Any DeserializeQueryAnswer(Any query, std::string serialized_answer) {
        auto answer_deserializer = [&serialized_answer]<class QueryType>(QueryType query) {
            return Answer::DeserializeTransfer<typename QueryType::Answer>(serialized_answer);
        };
        return std::visit(answer_deserializer, query);
    }
}
