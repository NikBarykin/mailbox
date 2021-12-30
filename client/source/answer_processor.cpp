//
// Created by baryk on 29.12.2021.
//

#include "answer_processor.h"


void AnswerProcessor::operator()(Answer::AnsT answer) {
    std::visit(*this, answer);
}
