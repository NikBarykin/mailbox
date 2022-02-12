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
    void TestDatabaseUserOneThread(Database& db, std::string a_name, std::string b_name) {
        assert(a_name != b_name);
        assert(db.Authorize(a_name, a_name) == db.GetId(a_name));
        assert(!db.Authorize(a_name, b_name));
        Database::UserId b_id = db.GetId(b_name);
        assert(db.GetId(a_name) != b_id);
        assert(db.Authorize(b_name, b_name) == b_id);
        assert(!db.Authorize(b_name, a_name));
        assert(db.Authorize(b_name, b_name));
    }

    void TestDatabaseLetterOneThread(Database& db, std::string a_name, std::string b_name, std::string c_name) {
        std::vector<Letter> letters = {Letter{a_name, b_name, "0"},
                                       Letter{a_name, b_name, "1"},
                                       Letter{b_name, a_name, "2"},
                                       Letter{c_name, b_name, "3"},
                                       Letter{a_name, c_name, "4"},
                                       Letter{c_name, b_name, "5"}};


        assert(db.GetMail(db.GetId(a_name)).empty());

        db.AddLetters(letters.begin(), --letters.end());
        db.AddLetter(letters.back());

        std::vector<Letter> expected_a = {letters[2]};
        auto res1 = db.GetMail(db.GetId(a_name));
        assert(std::is_permutation(res1.begin(), res1.end(), expected_a.begin()));

        std::vector<Letter> expected_b = {letters[0], letters[1], letters[3], letters[5]};
        auto res2 = db.GetMail(db.GetId(b_name));
        assert(std::is_permutation(res2.begin(), res2.end(), expected_b.begin()));

        std::vector<Letter> expected_c = {letters[4]};
        auto res3 = db.GetMail(db.GetId(c_name));
        assert(std::is_permutation(res3.begin(), res3.end(), expected_c.begin()));
    }

    void TestDatabaseUser() {
        Database db;
        std::future<void> future1 = std::async(TestDatabaseUserOneThread, std::ref(db), "a", "b");
        std::future<void> future2 = std::async(TestDatabaseUserOneThread, std::ref(db), "c", "d");
        std::future<void> future3 = std::async(TestDatabaseUserOneThread, std::ref(db), "e", "f");

        future1.get();
        future2.get();
        future3.get();
    }

    void TestDatabaseLetter() {
        Database db;
        std::future<void> future1 = std::async(TestDatabaseLetterOneThread, std::ref(db), "a", "b", "c");
        std::future<void> future2 = std::async(TestDatabaseLetterOneThread, std::ref(db), "d", "e", "f");
        std::future<void> future3 = std::async(TestDatabaseLetterOneThread, std::ref(db), "g", "h", "i");
        std::future<void> future4 = std::async(TestDatabaseLetterOneThread, std::ref(db), "x", "y", "z");

        future1.get();
        future2.get();
        future3.get();
        future4.get();
    }

}


void TestDatabase() {
    // We have to run these tests multiple times to increase chances of catching multithreading error
    for (int i = 0; i < 10000; ++i) {
        TestDatabaseUser();
        TestDatabaseLetter();
    }
    std::cerr << "TestDatabase: OK" << std::endl;
}
