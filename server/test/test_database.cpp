#include "test_database.h"
#include "../source/database.h"


#include <iostream>
#include <cassert>
#include <vector>
#include <future>
#include <sstream>
#include <algorithm>


// TODO: Generate big tests


namespace {
    void TestDatabaseUser() {
        Database db;
        assert(db.Authorize("a", "a") == db.GetId("a"));
        assert(!db.Authorize("a", "b"));
        Database::UserId b_id = db.GetId("b");
        assert(db.GetId("a") != b_id);
        assert(db.Authorize("b", "b") == b_id);
        assert(!db.Authorize("b", "a"));
        assert(db.Authorize("b", "b"));
    }
//
//    Letter operator"" _L(const char* letter_s, size_t) {
//        std::istringstream iss(letter_s);
//        Letter result;
//        std::getline(iss, result.from, ' ');
//        std::getline(iss, result.to, ' ');
//        std::getline(iss, result.body, ' ');
//        return result;
//    }

    void TestDatabaseLetter() {
        Database db;
        std::vector<Letter> letters = {Letter{"a", "b", "0"},
                                       Letter{"a", "b", "1"},
                                       Letter{"b", "a", "2"},
                                       Letter{"c", "d", "3"},
                                       Letter{"a", "e", "4"},
                                       Letter{"e", "b", "5"}};
        db.AddLetters(letters.begin(), --letters.end());
        db.AddLetter(letters.back());
        std::vector<Letter> expected1 = {letters[2]};
        auto res1 = db.GetMail(db.GetId("a"));
        assert(std::is_permutation(res1.begin(), res1.end(), expected1.begin()));
        std::vector<Letter> expected2 = {letters[0], letters[1], letters[5]};
        auto res2 = db.GetMail(db.GetId("b"));
        assert(std::is_permutation(res2.begin(), res2.end(), expected2.begin()));
        std::vector<Letter> expected3 = {letters[4]};
        auto res3 = db.GetMail(db.GetId("e"));
        assert(std::is_permutation(res3.begin(), res3.end(), expected3.begin()));
        assert(db.GetMail(db.GetId("c")).empty());
        assert(db.GetMail(db.GetId("x")).empty());
    }
//
//    void TestDatabaseLetter() {
//        Database db;
//        Letter l1{"a", "b", "hey, a"};
//        Letter l2{"b", "a", "hey, b"};
//        Letter l3{"c", "a", "hey, a\nhow was you day?"};
//        Letter l4{ "a", "c", "hey, c\nI've had a great day!"};
//        Letter l5{"c", "a", "that's awesome"};
//        db.AddLetter(l1);
//        db.AddLetter(l2);
//        db.AddLetter(l3);
//        db.AddLetter(l4);
//        db.AddLetter(l5);
//        std::vector<Letter> expected_a_mail = {l2, l3, l5};
//        assert(db.GetMail("a") == expected_a_mail);
//        std::vector<Letter> expected_b_mail = {l1};
//        assert(db.GetMail("b") == expected_b_mail);
//        std::vector<Letter> expected_c_mail = {l4};
//        assert(db.GetMail("c") == expected_c_mail);
//        assert(db.GetMail("random person").empty());
//    }
//
//    void TestDatabaseMultipleThreads() {
//        Database db;
//        auto letter_adder = [&db]() {
//            for (int i = 0; i < 100; ++i) {
//                db.AddLetter({"x", "y", "bla-bla-bla"});
//            }
//        };
//        auto letter_tester = [&db]() {
//            assert(db.GetMail("a").empty());
//            assert(db.GetMail("b").empty());
//            Letter l1 = {"a", "b", "1"};
//            db.AddLetter(l1);
//            assert(db.GetMail("a").empty());
//            Letter l2 = {"b", "a", "2"};
//            db.AddLetter(l2);
//            Letter l3 = {"c", "a", "3"};
//            db.AddLetter(l3);
//            Letter l4 = {"c", "d", "4"};
//            db.AddLetter(l4);
//            std::vector<Letter> expected1 = {l2, l3};
//            assert(db.GetMail("a") == expected1);
//            std::vector<Letter> expected2 = {l4};
//            assert(db.GetMail("d") == expected2);
//            Letter l5 = {"d", "c", "5"};
//            db.AddLetter(l5);
//            Letter l6 = {"d", "a", "6"};
//            db.AddLetter(l6);
//            Letter l7 = {"d", "b", "7"};
//            db.AddLetter(l7);
//            std::vector<Letter> expected3 = {l2, l3, l6};
//            assert(db.GetMail("a") == expected3);
//            std::vector<Letter> expected4 = {l1, l7};
//            assert(db.GetMail("b") == expected4);
//            std::vector<Letter> expected5 = {l5};
//            assert(db.GetMail("c") == expected5);
//        };
//        std::vector<std::future<void>> futures;
//        futures.push_back(std::async(letter_adder));
//        futures.push_back(std::async(letter_tester));
//        futures.push_back(std::async(letter_adder));
//    }
}


void TestDatabase() {
    TestDatabaseUser();
    TestDatabaseLetter();
    //    TestDatabaseSimple();
    // We have to run this test multiple times to increase chances of catching multithread error
//    for (int i = 0; i < 25; ++i) {
//        TestDatabaseMultipleThreads();
//    }
    std::cerr << "TestDatabase: OK" << std::endl;
}
