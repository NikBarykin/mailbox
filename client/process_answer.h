#pragma once

#include "../answer.h"

#include <variant>
#include <iostream>


struct AnswerProcessor {

    void operator ()(Answer::GetMail answer);
    void operator ()(Answer::Authorize answer);
    void operator ()(Answer::Error answer);
    void operator ()(Answer::SendLetter answer);
    void operator ()(Answer::Terminate answer);
};


void ProcessAnswer(Answer::AnsT answer) {
    std::visit(AnswerProcessor(), answer);
}
