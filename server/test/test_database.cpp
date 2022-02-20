#include "test_database.h"

#include <iostream>
#include <cassert>
#include <vector>
#include <future>
#include <sstream>
#include <algorithm>

#include "../source/database.h"
#include "common/test/testing_utility.h"


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
        std::vector<Letter> letters{{a_name, b_name, "0", "1.1.1"_d},
                                    {a_name, b_name, "1", "1.1.1"_d},
                                    {b_name, a_name, "2", "1.1.1"_d},
                                    {c_name, b_name, "3", "1.1.1"_d},
                                    {a_name, c_name, "4", "1.1.1"_d},
                                    {c_name, b_name, "5", "1.1.1"_d}};


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

        std::vector<Letter> expected_b_filtered = {letters[1], letters[5]};
        auto res4 = db.GetMail(db.GetId(b_name),
                               "(from == \"" + c_name + "\" && body == \"5\") || (body == \"1\")");
        assert(std::is_permutation(res4.begin(), res4.end(), expected_b_filtered.begin()));
    }

    void TestDatabaseUser() {
        Database db;

        std::vector<std::future<void>> futures;
        futures.push_back(std::async(TestDatabaseUserOneThread, std::ref(db), "a", "b"));
        futures.push_back(std::async(TestDatabaseUserOneThread, std::ref(db), "c", "d"));
        futures.push_back(std::async(TestDatabaseUserOneThread, std::ref(db), "e", "f"));

        for (auto &future : futures) {
            future.get();
        }
    }

    void TestDatabaseLetter() {
        Database db;

        std::vector<std::future<void>> futures;
        futures.push_back(std::async(TestDatabaseLetterOneThread, std::ref(db), "a", "b", "c"));
        futures.push_back(std::async(TestDatabaseLetterOneThread, std::ref(db), "d", "e", "f"));
        futures.push_back(std::async(TestDatabaseLetterOneThread, std::ref(db), "g", "h", "i"));
        futures.push_back(std::async(TestDatabaseLetterOneThread, std::ref(db), "x", "y", "z"));

        for (auto &future : futures) {
            future.get();
        }
    }

}


void TestDatabase() {
    // We have to run these tests multiple times to increase chances of catching multithreading error
    for (int i = 0; i < 2000; ++i) {
        TestDatabaseUser();
        TestDatabaseLetter();
    }
    std::cerr << "TestDatabase: OK" << std::endl;
}
