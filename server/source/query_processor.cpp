#include "query_processor.h"


QueryProcessor::QueryProcessor(Database &db, SessionState &session_state)
        : db_(db), session_state_(session_state) {}

Answer::Any QueryProcessor::operator()(Query::GetMail) {
    return Query::GetMail::Any{db_.GetMail(session_state_.user_login)};
}

Answer::Any QueryProcessor::operator()(Query::Any query) {
    return std::visit(*this, query);
}
