#include "test_answer.h"
#include "general/source/answer.h"

#include <iostream>
#include <cassert>


namespace {
    void TestGetMail() {
        Answer::GetMail answer{{{"a", "b", "c"},
                                {"x", "y", "z"},
                                {"\\S\nU\\S", "\\A\nMO", "G\nU\\\\S"}}};

        std::string serialized = answer.SerializeForTransfer();
        assert(serialized == "\na\nb\nc\nx\ny\nz\n\\\\S\\nU\\\\S\n\\\\A\\nMO\nG\\nU\\\\\\\\S");

        auto deserialized = Answer::GetMail::DeserializeTransfer(serialized);
        assert(deserialized.mail == answer.mail);
    }

    void TestSendLetter() {
        assert(Answer::SendLetter{}.SerializeForTransfer() == "");
        Answer::SendLetter::DeserializeTransfer("");
    }

    void TestAuthorize() {
        Answer::Authorize answer{true};

        std::string serialized = answer.SerializeForTransfer();
        assert(serialized == "\n1");

        auto deserialized = Answer::Authorize::DeserializeTransfer(serialized);
        assert(deserialized.authorization_succeed == answer.authorization_succeed);
    }

    void TestTerminate() {
        assert(Answer::Terminate{}.SerializeForTransfer() == "");
        Answer::Terminate::DeserializeTransfer("");
    }

    void TestError() {
        Answer::Error answer{"A terrible error!"};

        std::string serialized = answer.SerializeForTransfer();
        assert(serialized == "A terrible error!");

        auto deserialized = Answer::Error::DeserializeTransfer(serialized);
        assert(deserialized.message == answer.message);
    }

    void TestGeneralTransfer() {
        Answer::GetMail ans1{{{"a\n", "b", "c"},
                              {"\\x", "\ny\n", "z"}}};
        auto serialized1 = Answer::SerializeForTransfer(ans1);
        assert(serialized1 == "\na\\n\nb\nc\n\\\\x\n\\ny\\n\nz");

        auto serialized2 = Answer::SerializeForTransfer(Answer::SendLetter{});
        assert(serialized2.empty());

        auto deserialized1 = Answer::DeserializeTransfer<Answer::Authorize>("\n1");
        assert(std::holds_alternative<Answer::Authorize>(deserialized1));
        assert(std::get<Answer::Authorize>(deserialized1).authorization_succeed);

        auto deserialized2 = Answer::DeserializeTransfer<Answer::Terminate>("");
        assert(std::holds_alternative<Answer::Terminate>(deserialized2));

        auto deserialized3 = Answer::DeserializeTransfer<Answer::GetMail>("Fatal error!");
        assert(std::holds_alternative<Answer::Error>(deserialized3));
        assert(std::get<Answer::Error>(deserialized3).message == "Fatal error!");
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
