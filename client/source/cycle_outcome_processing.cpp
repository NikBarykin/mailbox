#include "cycle_outcome_processing.h"


namespace {
    void Process(const Query::GetMail&, Query::GetMail::Answer answer,
                 SessionState &, std::ostream & output) {
        if (answer.mail.empty()) {
            output << "Your mailbox is empty\n";
            return;
        }
        for (size_t letter_i = 0; letter_i < answer.mail.size(); ++letter_i) {
            const Letter & letter = answer.mail[letter_i];
            output << letter_i + 1 << ".\n";
            output << "Date: " << letter.date.AsString() << '\n';
            output << "From: " << letter.from << "\n";
            output << letter.body << "\n";
        }
    }

    void Process(const Query::SendLetter&, Query::SendLetter::Answer,
                 SessionState &, std::ostream & output) {
        output << "Letter sent successfully\n";
    }

    // TODO: upgrade with Answer::Authorize upgrade
    void Process(Query::Authorize query, Query::Authorize::Answer answer,
                 SessionState & session_state, std::ostream & output) {
        if (answer.authorized_login.empty()) {
            output << "Wrong password\n";
            return;
        }
        session_state.user_login = answer.authorized_login;
        output << "Authorized successfully\n";
    }

    void Process(Query::Terminate, Query::Terminate::Answer,
                 SessionState & session_state, std::ostream & output) {
        session_state.running = false;
        output << "Session terminated\n";
    }

    void ProcessError(Answer::Error answer, std::ostream & output) {
        output << "Server error: " << answer.message << "\n";
    }
}


void ProcessCycleOutcome(Query::Any query, Answer::Any answer,
                         SessionState & session_state, std::ostream & output) {
    if (std::holds_alternative<Answer::Error>(answer)) {
        ProcessError(std::get<Answer::Error>(answer), output);
    } else {
        std::visit([&answer, &session_state, &output]<class QueryType>(QueryType query) {
            Process(query, std::get<typename QueryType::Answer>(answer), session_state, output);
        }, query);
    }
    output << std::endl;
}
