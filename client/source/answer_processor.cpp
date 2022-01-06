#include "answer_processor.h"


#include <string>


AnswerProcessor::AnswerProcessor(SessionState &session_state, std::ostream &output)
:session_state_(session_state), output_(output) {}


void AnswerProcessor::Process(Answer::GetMail answer) {
    if (answer.mail.empty()) {
        output_ << "Your mailbox is empty\n";
        return;
    }
    bool first_letter = true;
    for (const Letter &letter: answer.mail) {
        if (first_letter) {
            first_letter = false;
        } else {
            output_ << std::string(15, '-') << "\n";
        }
        output_ << "From: " << letter.from << "\n";
        output_ << letter.body << "\n";
    }
}

void AnswerProcessor::Process(Answer::SendLetter) {
    output_ << "Letter sent successfully\n";
}

void AnswerProcessor::Process(Answer::Authorize answer) {
    if (answer.authorized_login.empty()) {
        output_ << "Wrong password\n";
        return;
    }
    session_state_.user_login = answer.authorized_login;
    output_ << "Authorized successfully\n";
}

void AnswerProcessor::Process(Answer::Terminate) {
    session_state_.running = false;
    output_ << "Session terminated\n";
}

void AnswerProcessor::Process(Answer::Error answer) {
    output_ << "Server error: " << answer.message << "\n";
}

void AnswerProcessor::operator()(Answer::Any answer) {
    std::visit([this](auto answer) {
        this->Process(answer);
    }, answer);
    output_ << std::endl;
}
