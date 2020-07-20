/**
 * @file hashTableTests.cpp
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief tests for both circular wrapping & non-wrapping
 * @date 2020-07-20
 */
 
#include <gtest/gtest.h> // googletest header file

#include <iostream>
#include <vector>
#include <string>
#include <utility> // std::pair

#include "global.h"
#include "circular_queue.h"

class Queue_T : public ::testing::Test
{
protected:
    bool status;
    q_entry ret_entry;
    int q_size;
    circularQueue *q;
    circularQueue *qw;

    virtual void SetUp()
    {
        q_size = 8;

        q = new circularQueue(q_size);
        qw = new circularQueue_wrap(q_size);
    }

    virtual void TearDown()
    {
        delete q;
        delete qw;
    }
};

/* Test Cases
* isEmpty
** at start
** after queue 1
** after dequeue all
** after queue to full
* isFull
** at start
** after queue 1
** after dequeue 1
** after queue to full
* enq
** queue msg size larger than MAX
** queue 1
** queue to full + 1
** after dequeue 1
** queue to full (should only be 1)
* deq
** queue 1 & dequeue to fail
** queue full & dequeue to fail
* wrapping
** queue to full & wrap
** empty queue and compare vals to ensure entries match order
** empty to empty
*/

TEST_F(Queue_T, empty)
{
    std::string s(MAX_MSG_SIZE-1, 'a');

    // at start
    status = q->isEmpty();
    EXPECT_TRUE(status) << "F when isEmpty on new queue" << std::endl;

    // enqueue (continue IFF P)
    status = q->enq(s.c_str(), 0);
    ASSERT_TRUE(status) << "F when enqueue" << std::endl;

    // when 1 in q
    status = q->isEmpty();
    EXPECT_FALSE(status) << "NF when isEmpty w/ 1 queue entry" << std::endl;

    // remove 1 from q
    ret_entry = q->deq();
    ASSERT_TRUE(ret_entry != FAILED_DEQ) << "F when dequeue" << std::endl;

    // at start
    status = q->isEmpty();
    EXPECT_TRUE(status) << "F when isEmpty post emptying queue" << std::endl;

    // max queue
    for (int i = 0; i < q_size; i++)
    {
        // enqueue (continue IFF P)
        status = q->enq(s.c_str(), 0);
        ASSERT_TRUE(status) << "F when adding to queue" << std::endl;
    }

    // when q is full
    status = q->isEmpty();
    EXPECT_FALSE(status) << "NF when isEmpty on full queue" << std::endl;
}

TEST_F(Queue_T, full)
{
    std::string s(MAX_MSG_SIZE-1, 'a');

    // at start
    status = q->isFull();
    EXPECT_FALSE(status) << "NF when isFull on new queue" << std::endl;

    // enqueue (continue IFF P)
    status = q->enq(s.c_str(), 0);
    ASSERT_TRUE(status) << "F when enqueue" << std::endl;

    // when 1 in q
    status = q->isFull();
    EXPECT_FALSE(status) << "NF when isFull w/ 1 queue entry" << std::endl;

    // max queue
    for (int i = 1; i < q_size; i++)
    {
        // enqueue (continue IFF P)
        status = q->enq(s.c_str(), 0);
        ASSERT_TRUE(status) << "F when adding to queue" << std::endl;
    }

    // when q is full
    status = q->isFull();
    EXPECT_TRUE(status) << "F when isFull on full queue" << std::endl;
}

TEST_F(Queue_T, enqueue)
{
    std::string s(MAX_MSG_SIZE, 'a');

    // check name longer than max msg len
    status = q->enq(s.c_str(),0);
    EXPECT_FALSE(status) << "NF when enq msg > MAX_LEN" << std::endl;
    s.pop_back(); // decrease to max len

    // queue 1
    status = q->enq(s.c_str(),0);
    EXPECT_TRUE(status) << "F when enq first" << std::endl;

    // fill queue
    for (int i = 1; i < q_size; i++)
    {
        status = q->enq(s.c_str(),0);
        ASSERT_TRUE(status) << "F when adding to queue" << std::endl;
    }

    // add when q is full
    status = q->enq(s.c_str(),0);
    EXPECT_FALSE(status) << "NF when adding to full queue" << std::endl;

    // remove 1 from q
    ret_entry = q->deq();
    ASSERT_TRUE(ret_entry != FAILED_DEQ) << "F when dequeue" << std::endl;

    // queue 1
    status = q->enq(s.c_str(),0);
    EXPECT_TRUE(status) << "F when adding to queue" << std::endl;

    // add when q is full
    status = q->enq(s.c_str(),0);
    EXPECT_FALSE(status) << "NF when adding to full queue" << std::endl;
}

TEST_F(Queue_T, dequeue)
{
    std::string s(MAX_MSG_SIZE-1, 'a');

    // queue 1
    status = q->enq(s.c_str(),0);
    EXPECT_TRUE(status) << "F when enq first" << std::endl;

    // remove 1 from q
    ret_entry = q->deq();
    EXPECT_TRUE(ret_entry != FAILED_DEQ) << "F when dequeue" << std::endl;

    // no fail when deq from empty queue
    ret_entry = q->deq();
    EXPECT_TRUE(ret_entry == FAILED_DEQ) << "NF when dequeue from empty queue" << std::endl;
}

TEST_F(Queue_T, wrapping)
{
    // queue to full & more
    std::string s(MAX_MSG_SIZE-1, 'a');
    q_entry q_s_entry;
    strcpy(q_s_entry.msg,s.c_str());
    q_s_entry.val = 0;
    for (int i = 0; i < q_size; i++)
    {
        status = qw->enq(q_s_entry.msg,q_s_entry.val);
        EXPECT_TRUE(status) << "F when adding to queue" << std::endl;
    }

    // add 2 more to queue to ensure wrapping
    std::string s1(MAX_MSG_SIZE-2, 'a');
    q_entry q_s1_entry;
    strcpy(q_s1_entry.msg,s1.c_str());
    q_s1_entry.val = 0;
    for (int i = 0; i < 2; i++)
    {
        status = qw->enq(q_s1_entry.msg,q_s1_entry.val);
        EXPECT_TRUE(status) << "F when adding to queue after wrapping" << std::endl;
    }

    // check entries prior to wrap
    for (int i = 2; i < q_size; i++)
    {
        ret_entry = qw->deq();
        EXPECT_TRUE(ret_entry == q_s_entry) << "F when deq and comparing initial non-wrapped entries" << std::endl; 
    }

    // check entries post wrap
    for (int i = 0; i < 2; i++)
    {
        ret_entry = qw->deq();
        EXPECT_TRUE(ret_entry == q_s1_entry) << "F when deq and comparing wrapped entries" << std::endl; 
    }

    status = qw->isEmpty();
    EXPECT_TRUE(status) << "NF when checking if wrapped queue is empty" << std::endl;
}