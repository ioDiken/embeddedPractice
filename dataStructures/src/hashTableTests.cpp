/**
 * @file hashTableTests.cpp
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief 
 * @date 2020-07-16
 */
 
#include <gtest/gtest.h> // googletest header file

#include <iostream>
#include <vector>
#include <utility> // std::pair

#include "global.h"
#include "chained_hashtable.h"

class Chained_Hash : public ::testing::Test
{
protected:
    bool status;
    chainedHash *chain;

    std::vector<std::pair<std::string,uint8_t>> people = {
        {"Jimmy", 8},
        {"Johnny", 10},
        {"Tyler", 13},
        {"Randy", 39},
        {"Azita", 23},
        {"Manal", 22},
        {"Akash", 23},
        {"Devin", 24},
        {"David", 27},
        {"Anne", 10},
        {"Baba", 13},
        {"Melis", 10},
        {"Seth", 28},
        {"NOLA", 99},
        {"NOLERRS", 255}
    };

    virtual void SetUp()
    {
        chain = new chainedHash(people.size()/2 - 1); // guarantees atleast 1 chain of 3
    }

    virtual void TearDown()
    {
        delete chain;
    }
};

TEST_F(Chained_Hash, get)
{
    // check for person that DNE
    std::string s(MAX_NAME_SIZE-1, 'a');
    person *p = chain->get(s.c_str());
    EXPECT_TRUE(p == NULL) << "NF when get name that DNE" << std::endl;

    // put person in table (continue IFF P)
    status = chain->put(s.c_str(), 5);
    ASSERT_TRUE(status) << "F to add person to hash table" << std::endl;
    
    // get person that exists
    p = chain->get(s.c_str());
    EXPECT_TRUE(std::string(p->name) == s) << "F when get name that Exist" << std::endl;

    // check for person that DNE in table
    s.pop_back();
    p = chain->get(s.c_str());
    EXPECT_TRUE(p == NULL) << "NF when get name that DNE" << std::endl;
}

TEST_F(Chained_Hash, put)
{
    // max name size
    std::string s(MAX_NAME_SIZE, 'a');
    status = chain->put(s.c_str(), 5);
    EXPECT_FALSE(status) << "NF when name_len > MAX_NAME_SIZE" << std::endl;
    
    // put person (continue iff P)
    // std::string s(MAX_NAME_SIZE-1, 'a');
    s = std::string(MAX_NAME_SIZE-1, 'a');
    status = chain->put(s.c_str(), 5);
    ASSERT_TRUE(status) << "NF when name_len > MAX_NAME_SIZE" << std::endl;

    // put in chain
    for (int i = 0; i < people.size(); i++)
    {
        status = chain->put(std::get<0>(people[i]).c_str(), std::get<1>(people[i]));
        EXPECT_TRUE(status) << "Failed to put member in hash table" << std::endl;
    }
}

TEST_F(Chained_Hash, remove_p)
{
    // put person (continue iff P)
    std::string s(MAX_NAME_SIZE-1, 'a');
    status = chain->put(s.c_str(), 5);
    ASSERT_TRUE(status) << "NF when name_len > MAX_NAME_SIZE" << std::endl;

    // remove person (continue IFF P)
    status = chain->remove(s.c_str());
    ASSERT_TRUE(status) << "F when remove person in hash table" << std::endl;

    // get person post remove
    person *p = chain->get(s.c_str());
    EXPECT_TRUE(p == NULL) << "NF when get name post remove" << std::endl;

    // put person (continue iff P)
    // used just to have a single entry in table
    status = chain->put(s.c_str(), 5);
    ASSERT_TRUE(status) << "NF when name_len > MAX_NAME_SIZE" << std::endl;
    
    // remove person that DNE
    s.pop_back();
    status = chain->remove(s.c_str());
    EXPECT_FALSE(status) << "NF when remove person that DNE" << std::endl;
}
