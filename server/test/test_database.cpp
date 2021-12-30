#include "test_database.h"
#include "../source/database.h"


#include <iostream>
#include <cassert>
#include <vector>


namespace {
    void TestDatabaseSimple() {
        Database db;
        Letter l1{"a", "b", "hey, a"};
        Letter l2{"b", "a", "hey, b"};
        Letter l3{"c", "a", "hey, a\nhow was you day?"};
        Letter l4{"a", "c", "hey, c\nI've had a great day!"};
        Letter l5{"c", "a", "that's awesome"};
        db.AddLetter(l1);
        db.AddLetter(l2);
        db.AddLetter(l3);
        db.AddLetter(l4);
        db.AddLetter(l5);
        std::vector<Letter> expected_a_mail = {l2, l3, l5};
        assert(db.GetMail("a") == expected_a_mail);
        std::vector<Letter> expected_b_mail = {l1};
        assert(db.GetMail("b") == expected_b_mail);
        std::vector<Letter> expected_c_mail = {l4};
        assert(db.GetMail("c") == expected_c_mail);
    }

    void TestDatabaseMultipleThreads() {

    }
}


void TestDatabase() {
    std::cerr << "TestDatabase: OK" << std::endl;
}
