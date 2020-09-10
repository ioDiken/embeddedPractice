/**
 * @file binary_heap.h
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief implemented using binary tree
 * @TODO add hash table for holding pid (incase process is woken)
 * @date 2020-07-21
 */
#if 0
#ifndef E13D40BB_60B1_4034_80DA_9BD3A4619340
#define E13D40BB_60B1_4034_80DA_9BD3A4619340

#include "global.h"
#include <stdint.h>

#define MIN_PRIORITY 30 // max 0
#define TO_BE_DONE

typedef struct q_item {
    uint32_t pid;
    uint8_t priority;
} q_item;

// structure that is returned on fail of pop/get
#define MAGIC_PRIORITY_FAIL 0xEA
q_item FAILED_Q = {
    .pid = 0,
    .priority = MAGIC_PRIORITY_FAIL
};

class priorityQueue
{
public:
    priorityQueue(uint16_t queue_size);
    ~priorityQueue();

    bool insert(uint32_t pid, uint8_t priority);
    q_item popHighestPriority();
    q_item getHighestPriority();

private:
    q_item *queue;
    uint16_t q_size, q_cap;

    inline void swim(uint16_t i); // move hash table locations?
    inline bool less(uint16_t i, uint16_t j);
    inline bool greater(uint16_t i, uint16_t j);
    inline void sink(uint16_t i);
    inline void swap(uint16_t i, uint16_t j);

    #ifndef TO_BE_DONE
    // hash for location of item w/ priority X
    typedef struct node_t {
        uint16_t loc; // location in binary heap
        struct node_t *next;
    } node_t;
    node_t *locator[MIN_PRIORITY]; // used to locate

    // push to back of binary locator LL
    bool push_back(uint8_t priority, uint16_t loc);
    // pop from binary locator LL
    int pop(uint8_t priority);
    // remove location from binary locator because it needs to be updated
    // TODO
    // free all in binary locator LL
    void free_locator();
    #endif
};


#endif /* E13D40BB_60B1_4034_80DA_9BD3A4619340 */
#endif