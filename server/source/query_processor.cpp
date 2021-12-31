#include "query_processor.h"


QueryProcessor::QueryProcessor(Database &db, SessionState &session_state)
        : db_(db), session_state_(session_state) {}

Answer::Any QueryProcessor::operator()(Query::GetMail) {
    if (!session_state_.user_id) {
        return Answer::Error{"Not authorized"};
    }
    return Query::GetMail::Answer{db_.GetMail(*session_state_.user_id)};
}

// TODO:: maybe forbid sending letter to yourself
Answer::Any QueryProcessor::operator()(Query::SendLetter send_letter_q) {
    if (!session_state_.user_id) {
        return Answer::Error{"Not authorized"};
    }
    db_.AddLetter(send_letter_q.letter);
    return Answer::SendLetter{};
}

Answer::Any QueryProcessor::operator()(Query::Authorize authorize_q) {
    std::optional<Database::UserId> user_id = db_.Authorize(authorize_q.login, authorize_q.password);
    if (!user_id) {
        return Answer::Error{"Wrong password"};
    }
    session_state_.user_id = db_.GetId(authorize_q.login);
    return Answer::Authorize{.authorization_succeed = true};
}

// TODO: maybe return Answer::Error if trying to terminate already terminated session (like it is strange)
Answer::Any QueryProcessor::operator()(Query::Terminate) {
    session_state_.running = false;
    return Answer::Terminate{};
}

Answer::Any QueryProcessor::operator()(Query::Any query) {
    return std::visit(*this, query);
}
