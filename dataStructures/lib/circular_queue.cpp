/**
 * @file circular_queue.cpp
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief circular queue of variable size w/|w/o wrapping
 * @date 2020-07-16
 */
#ifndef DA814913_C727_4C93_8B7E_D384DB48D78A
#define DA814913_C727_4C93_8B7E_D384DB48D78A

#include <string.h>
#include "circular_queue.h"

#define CIRCULAR_INC(var,max_sz) (var = (var+1) % max_sz)

inline bool check_msg_len(const char *msg)
{
	if (MAX_MSG_SIZE == strnlen(msg, MAX_MSG_SIZE))
	{
		DEBUG("%.*s... longer than max msg len(%d)\n",
               MAX_MSG_SIZE,msg,MAX_MSG_SIZE);
		return false;
	}
	return true;
}

circularQueue::circularQueue(uint16_t size)
{
    q = (q_entry *) malloc(sizeof(q_entry)*size);
    check_malloc((void *) q, "Failed to malloc queue\n");

    head = -1;
    tail = 0;
    q_sz = size;
}

circularQueue::~circularQueue()
{
    free(q);
}

bool circularQueue::enq(const char *msg, uint8_t val)
{
	DEBUG("\nBeginning %s\n", __func__);

    // check msg len
    if (!check_msg_len(msg))
        return false;
    
    if (!isFull())
    {
        strncpy(q[tail].msg, msg, MAX_MSG_SIZE);
        q[tail].val = val;

        // update vars depending on implementation
        enq_inc();
        return true;
    }

    return false;
}

q_entry circularQueue::deq()
{
	DEBUG("\nBeginning %s\n", __func__);

    if (!isEmpty())
    {
        q_entry ret = q[head];

        CIRCULAR_INC(head,q_sz);

        // if head+1 == tail, then reset vars
        // aka final item
        if (head == tail)
        {
            DEBUG("Resetting Queue\n");
            head = -1; // empty condition
            tail = 0;
        }

        return ret;
    }

    return FAILED_DEQ;
}

bool circularQueue::isFull()
{
	DEBUG("\nBeginning %s\n", __func__);

    if (head == tail)
    {
        DEBUG("Queue is full\n");
        return true;
    }
    return false;
}

bool circularQueue::isEmpty()
{
	DEBUG("\nBeginning %s\n", __func__);

    if (head == -1)
    {
        DEBUG("Queue is empty\n");
        return true;
    }
    return false;
}

void circularQueue::enq_inc()
{
	DEBUG("\nBeginning %s\n", __func__);

    // if first entry
    if (isEmpty())
    {
        DEBUG("First Entry in Queue\n");
        head++;
    }

    CIRCULAR_INC(tail,q_sz);
    
}

void circularQueue::print_queue()
{
	DEBUG("\nBeginning %s\n", __func__);

    if (isEmpty())
    {
        printf("Queue is Empty\n");
        return;
    }

    printf("  ->  %s | %02x\n", q[head].msg, q[head].val);
    for (int i = head+1; i != tail; CIRCULAR_INC(i,q_sz))
    {
        printf("  ->  %s | %02x\n", q[i].msg, q[i].val);
    }
}

void circularQueue_wrap::enq_inc()
{
    // if first entry
    if (isEmpty())
    {
        DEBUG("First Entry in Queue\n");
        head++;
    }
    // increment head w/ tail if wrapping has begun
    else if (tail == head)
    {
        head++;
    }

    CIRCULAR_INC(tail,q_sz);
}

#endif /* DA814913_C727_4C93_8B7E_D384DB48D78A */