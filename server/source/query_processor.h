#pragma once


#include "server.h"
#include "database.h"
#include "general/source/query.h"
#include "general/source/answer.h"


class QueryProcessor {
private:
    Database& db_;
    SessionState& session_state_;
public:
    QueryProcessor(Database& db, SessionState& session_state);

    Answer::Any operator ()(Query::GetMail);
    Answer::Any operator ()(Query::SendLetter);
    Answer::Any operator ()(Query::Authorize);
    Answer::Any operator ()(Query::Terminate);

    Answer::Any operator ()(Query::Any);
};
