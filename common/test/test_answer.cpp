#include "test_answer.h"
#include "common/source/answer.h"

#include <iostream>
#include <cassert>

// TODO: make tests independent from protocol implementation
namespace {
    void TestGetMail() {
        Answer::GetMail answer{{{"a", "b", "c"},
                                {"x", "y", "z"},
                                {"SUS", "AMO", "G\nUS"}}};

        std::string serialized = answer.SerializeForTransfer();
        assert(serialized == "0\n1\na1\nb1\nc1\nx1\ny1\nz3\nSUS3\nAMO4\nG\nUS");

        auto deserialized = Answer::GetMail::DeserializeTransfer(serialized);
        assert(deserialized.mail == answer.mail);
    }

    void TestSendLetter() {
        assert(Answer::SendLetter{}.SerializeForTransfer() == "0\n");
        Answer::SendLetter::DeserializeTransfer("0\n");
    }

    void TestAuthorize() {
        Answer::Authorize answer{"Amogus"};

        std::string serialized = answer.SerializeForTransfer();
        assert(serialized == "0\n6\nAmogus");

        auto deserialized = Answer::Authorize::DeserializeTransfer(serialized);
        assert(deserialized.authorized_login == answer.authorized_login);
    }

    void TestTerminate() {
        assert(Answer::Terminate{}.SerializeForTransfer() == "0\n");
        Answer::Terminate::DeserializeTransfer("0\n");
    }

    void TestError() {
        Answer::Error answer{"Error!"};

        std::string serialized = answer.SerializeForTransfer();
        assert(serialized == "6\nError!");

        auto deserialized = Answer::Error::DeserializeTransfer(serialized);
        assert(deserialized.message == answer.message);
    }

    void TestGeneralTransfer() {
        Answer::GetMail ans1{{{"a", "b", "c"},
                              {"x", "y", "z"}}};
        auto serialized1 = Answer::SerializeForTransfer(ans1);
        assert(serialized1 == "0\n1\na1\nb1\nc1\nx1\ny1\nz");

        auto serialized2 = Answer::SerializeForTransfer(Answer::SendLetter{});
        assert(serialized2 == "0\n");

        auto deserialized1 = Answer::DeserializeTransfer<Answer::Authorize>("0\n3\nSus");
        assert(std::holds_alternative<Answer::Authorize>(deserialized1));
        assert(std::get<Answer::Authorize>(deserialized1).authorized_login == "Sus");

        auto deserialized2 = Answer::DeserializeTransfer<Answer::Terminate>("0\n");
        assert(std::holds_alternative<Answer::Terminate>(deserialized2));

        auto deserialized3 = Answer::DeserializeTransfer<Answer::GetMail>("2\n:(");
        assert(std::holds_alternative<Answer::Error>(deserialized3));
        assert(std::get<Answer::Error>(deserialized3).message == ":(");
    }
}


void TestAnswer() {
    TestGetMail();
    TestSendLetter();
    TestAuthorize();
    TestTerminate();
    TestError();
    TestGeneralTransfer();
    std::cerr << "TestAnswer: OK" << std::endl;
}
