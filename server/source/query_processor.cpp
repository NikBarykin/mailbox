#include "query_processor.h"


QueryProcessor::QueryProcessor(Database &db, SessionState &session_state)
        : db_(db), session_state_(session_state) {}

Answer::Any QueryProcessor::operator()(Query::GetMail) {
    if (!session_state_.user_id) {
        return Answer::Error{"Not authorized"};
    }
    return Query::GetMail::Answer{db_.GetMail(*session_state_.user_id)};
}

// TODO: maybe forbid sending letter to yourself
// TODO: check if letter sender id differs from *session_state.user_id.
//  Also, add corresponding unit-test
Answer::Any QueryProcessor::operator()(Query::SendLetter send_letter_q) {
    if (!session_state_.user_id) {
        return Answer::Error{"Not authorized"};
    }
    db_.AddLetter(send_letter_q.letter);
    return Answer::SendLetter{};
}

// TODO: maybe forbid 2 clients to be authorized in one account at the same time
Answer::Any QueryProcessor::operator()(Query::Authorize authorize_q) {
    std::optional<Database::UserId> user_id = db_.Authorize(authorize_q.login, authorize_q.password);
    if (user_id) {
        session_state_.user_id = user_id;
    }
    return Answer::Authorize{user_id.has_value() ? authorize_q.login : ""};
}

Answer::Any QueryProcessor::operator()(Query::Terminate) {
    session_state_.running = false;
    return Answer::Terminate{};
}

Answer::Any QueryProcessor::operator()(Query::Any query) {
    if (!session_state_.running) {
        return Answer::Error{"Query in terminated session"};
    }
    return std::visit(*this, query);
}
