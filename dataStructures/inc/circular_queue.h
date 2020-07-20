/**
 * @file circular_queue.h
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief circular queue of variable size w/|w/o wrapping
 * @date 2020-07-16
 */

#ifndef BDDCC465_2659_433E_933B_C012C75D02DD
#define BDDCC465_2659_433E_933B_C012C75D02DD

#include "global.h"
#include <stdint.h>

#define MAX_MSG_SIZE 20

typedef struct q_entry {
    char msg[MAX_MSG_SIZE];
    uint8_t val;
} q_entry;

// used to return failed dequeue
static q_entry FAILED_DEQ = {
    .msg = "FAILED_DEQ",
    .val = 0
};

/*     Operator Comparisons for q_entry       */
static bool operator==(const q_entry& lhs, const q_entry& rhs)
{
    if (strncmp(lhs.msg, rhs.msg, MAX_MSG_SIZE) == 0)
        if (lhs.val == rhs.val)
            return true;
    return false;
}
static bool operator!=(const q_entry& lhs, const q_entry& rhs)
{
    if (strncmp(lhs.msg, rhs.msg, MAX_MSG_SIZE) == 0)
        if (lhs.val == rhs.val)
            return false;
    return true;
}
/**********************************************/

/**
 * @brief non-wrapping circular queue implemenation
 *        used as base class for wrapping
 */
class circularQueue
{
public:
    circularQueue(uint16_t size);
    ~circularQueue();

    bool enq(const char *msg, uint8_t val);
    q_entry deq();
    virtual bool isFull(); // for redefinition if wrapping
    bool isEmpty();
    void print_queue();

protected:
    q_entry *q;
    int head, tail;
    uint16_t q_sz;

    virtual bool enq_check(); // checks & updates front
    virtual void enq_inc(); // post enq increment
    virtual bool deq_check(); // checks & updates rear
    virtual void deq_inc(); // post deq increment
};

class circularQueue_wrap : public circularQueue
{
public:
    circularQueue_wrap(uint16_t size);
    ~circularQueue_wrap();
    virtual bool isFull() override {return false;} // always false
protected:
    virtual bool enq_check() override; // checks & updates front/rear
    virtual bool deq_check() override; // checks & updates front/rear
    virtual void print_queue();
};


#endif /* BDDCC465_2659_433E_933B_C012C75D02DD */