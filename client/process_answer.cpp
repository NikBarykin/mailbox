//
// Created by baryk on 28.12.2021.
//

#include "process_answer.h"
#pragma once

#include "../answer.h"

#include <variant>
#include <iostream>


struct AnswerProcessor {
    void operator ()(Answer::GetMail answer) {
        for (Letter letter : answer.mail) {
            std::cout << letter.from << std::endl;
            std::cout << letter.body << std::endl;
        }
    }
    void operator ()(Answer::Authorize answer) {

    }
    void operator ()(Answer::Error answer) {

    }
    void operator ()(Answer::SendLetter answer) {

    }
    void operator ()(Answer::Terminate answer) {

    }

};


void ProcessAnswer(Answer::AnsT answer) {
    std::visit(AnswerProcessor(), answer);
}
