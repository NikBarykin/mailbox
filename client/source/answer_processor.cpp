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
        output_ << "Body: " << std::endl;
        output_ << letter.body << std::endl;
        output_ << std::string(20, '-');
    }
}

void AnswerProcessor::operator()(Answer::SendLetter) {
    output_ << "Letter sent successfully" << std::endl;
}

void AnswerProcessor::operator()(Answer::Authorize answer) {
    if (answer.authorization_succeed) {
        output_ << "Authorized successfully" << std::endl;
    } else {
        output_ << "Wrong password" << std::endl;
    }
}

void AnswerProcessor::operator()(Answer::Terminate) {
    output_ << "Session terminated" << std::endl;
}

void AnswerProcessor::operator()(Answer::Error answer) {
    output_ << "Error: " << answer.message << std::endl;
}

void AnswerProcessor::operator()(Answer::Any answer) {
    std::visit(*this, answer);
}
