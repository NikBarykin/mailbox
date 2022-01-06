#include "answer_processor.h"


#include <string>


AnswerProcessor::AnswerProcessor(SessionState &session_state, std::ostream &output)
:session_state_(session_state), output_(output) {}


void AnswerProcessor::operator()(Answer::GetMail answer) {
    if (answer.mail.empty()) {
        output_ << "Your mailbox is empty" << std::endl;
    }
    for (const Letter& letter : answer.mail) {
        output_ << "From: " << letter.from << std::endl;
        output_ << "Body:" << std::endl;
        output_ << letter.body << std::endl;
        output_ << std::string(20, '-') << std::endl;
    }
}

void AnswerProcessor::operator()(Answer::SendLetter) {
    output_ << "Letter sent successfully" << std::endl;
}

void AnswerProcessor::operator()(Answer::Authorize answer) {
    if (!answer.authorized_login.empty()) {
        session_state_.user_login = answer.authorized_login;
        output_ << "Authorized successfully" << std::endl;
    } else {
        output_ << "Wrong password" << std::endl;
    }
}

void AnswerProcessor::operator()(Answer::Terminate) {
    session_state_.running = false;
    output_ << "Session terminated" << std::endl;
}

void AnswerProcessor::operator()(Answer::Error answer) {
    output_ << "Server error: " << answer.message << std::endl;
}

void AnswerProcessor::operator()(Answer::Any answer) {
    std::visit([this](auto answer) {
        this->operator()(answer);
    }, answer);
}
