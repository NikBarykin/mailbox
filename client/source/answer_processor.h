#pragma once


#include "general/source/answer.h"
#include "session.h"

#include <variant>
#include <iostream>


class AnswerProcessor {
private:
    SessionState& session_state_;
    std::ostream& output_;
public:
    AnswerProcessor(SessionState&, std::ostream& output);

    void operator ()(Answer::GetMail);
    void operator ()(Answer::SendLetter);
    void operator ()(Answer::Authorize);
    void operator ()(Answer::Terminate);
    void operator ()(Answer::Error);

    void operator ()(Answer::Any answer);
};
