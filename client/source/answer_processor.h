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

private:
    // Not operator() cause it makes invocations like AnswerProcessor(Answer::GetMail{}) impossible,
    // compiler just doesn't want to cast concrete answer type to Answer::Any if there is a corresponding private operator()

    void Process(Answer::GetMail);
    void Process(Answer::SendLetter);
    void Process(Answer::Authorize);
    void Process(Answer::Terminate);
    void Process(Answer::Error);

public:
    void operator ()(Answer::Any answer);
};
