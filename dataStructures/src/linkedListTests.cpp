/**
 * @file hashTableTests.cpp
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief 
 * @date 2020-07-16
 */
 
#include <gtest/gtest.h> // googletest header file

#include <iostream>
#include <vector>

#include "global.h"
#include "linked_list.h"

#define LIST_MAX 20

class Linked_Lists : public ::testing::Test
{
protected:
    int status;
    LinkedList *ll;

    virtual void SetUp()
    {
        ll = new LinkedList();
    }

    virtual void TearDown()
    {
        delete ll;
    }

    void fill_list()
    {
        for (int i = 0; i < LIST_MAX; i++)
        {
            status = ll->push(i);
            ASSERT_TRUE(status) << "F when push to LL" << std::endl;
        }
    }
};

/*   Test Cases
* push
** push to list x num times?
* pop
** push to list 1 time, try pop twice
* get_len
** get length w/ 0 size, get len w/ x size
* free_lst
** push to list, free, then try pop
* reverse_lst
** push to list x times, reverse & check vals
* get_element
** push to list x times
** get ele that DNE
** get ele w/ val
* remove_element
** push to list x times
** remove ele
** get ele, should be next
* remove_vals
** assign list w/ x ele w/ val
** remove vals
** check for vals
*/

TEST_F(Linked_Lists, push)
{
    fill_list();
}

TEST_F(Linked_Lists, pop)
{
    status = ll->push(0);
    EXPECT_TRUE(status) << "F when push to LL" << std::endl;

    status = ll->pop();
    EXPECT_TRUE(status == 0) << "F vals dont match when pop from LL" << std::endl;

    status = ll->pop();
    EXPECT_TRUE(status == -1) << "NF when pop from emtpy LL" << std::endl;
}

TEST_F(Linked_Lists, get_len)
{
    fill_list();

    status = ll->get_len();
    EXPECT_TRUE(status == LIST_MAX) << "F when get_len not match list size" << std::endl;
}

TEST_F(Linked_Lists, free_lst)
{
    status = ll->push(0);
    EXPECT_TRUE(status) << "F when push to LL" << std::endl;

    ll->free_lst();

    status = ll->pop();
    EXPECT_TRUE(status == -1) << "NF when pop from empty LL" << std::endl;
}

TEST_F(Linked_Lists, reverse_list)
{
    fill_list();

    ll->reverse_lst();

    // check each val matches
    for (int i = 0; i < LIST_MAX; i++)
    {
        status = ll->pop();
        EXPECT_TRUE(status == i) << "F reversed list vals don't match" << std::endl;
    }
}

// * get_element
// ** push to list x times
// ** get ele that DNE
// ** get ele w/ val
// * remove_element
// ** push to list x times
// ** remove ele
// ** get ele, should be next
// * remove_vals
// ** assign list w/ x ele w/ val
// ** remove vals
// ** check for vals

TEST_F(Linked_Lists, get_element)
{
    fill_list();

    status = ll->get_element(LIST_MAX);
    EXPECT_TRUE(status == -1) << "NF when get_element that DNE" << std::endl;

    status = ll->get_element(0);
    EXPECT_TRUE(status == (LIST_MAX-1)) << "F when comparing get_element val to actual" << std::endl;
}

TEST_F(Linked_Lists, remove_element)
{
    fill_list();

    status = ll->remove_element(LIST_MAX);
    EXPECT_TRUE(status == -1) << "NF when remove_element that DNE" << std::endl;

    status = ll->remove_element(0);
    EXPECT_TRUE(status == (LIST_MAX-1)) << "F when comparing remove_element val to actual" << std::endl;

    status = ll->pop();
    EXPECT_TRUE(status == (LIST_MAX-2)) << "F when comparing new top val post remove_element" << std::endl;
}

TEST_F(Linked_Lists, remove_vals)
{
    for (int i = 0; i < LIST_MAX; i++)
    {
        status = ll->push(i/2);
        EXPECT_TRUE(status) << "F when push to LL" << std::endl;
    }

    // remove last 2 vals from list
    status = ll->remove_vals(0);
    EXPECT_TRUE(status) << "F when remove_vals" << std::endl;

    status = ll->get_element(LIST_MAX-3);
    EXPECT_TRUE(status == 1) << "F when checking removed val elements" << std::endl;
}