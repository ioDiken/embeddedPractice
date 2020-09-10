/**
 * @file priority_queue.cpp
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief implemented using binary tree & array of LL for location
 * @TODO: modify algo to avoid "process" starvation
 * @date 2020-07-21
 */
#if 0
#ifndef C5C19C5E_17DF_4A66_ADAC_F8E0F569CBB6
#define C5C19C5E_17DF_4A66_ADAC_F8E0F569CBB6

#include "priority_queue.h"
#include <stdlib.h>

static inline uint16_t parent(uint16_t i)
{
    return (i - 1)/2;
}

static inline uint16_t child_l(uint16_t i)
{
    return (2*i) + 1;
}

static inline uint16_t child_r(uint16_t i)
{
    return (2*i) + 2;
}

priorityQueue::priorityQueue(uint16_t queue_size)
{
    queue = (q_item *) malloc(sizeof(q_item)*queue_size);
    check_malloc(queue, "Failed to malloc piority queue\n");

    #ifndef TO_BE_DONE
    // allocate memory for binary locator LL
    // locator = (node_t **) malloc(sizeof(node_t)*MIN_PRIORITY);
    // check_malloc(locator, "Failed to malloc locating LL for binary tree\n");
    // initialize locator list to all NULL
    for (char i = 0; i < MIN_PRIORITY; i++)
    {
        locator[i] = NULL;
    }
    #endif

    q_cap = queue_size;
    q_size = 0;
}

priorityQueue::~priorityQueue()
{
    free(queue);
    #ifndef TO_BE_DONE
    free_locator();
    #endif
}

bool priorityQueue::insert(uint32_t pid, uint8_t priority)
{
    if ((q_size+1) == q_cap)
    {
        DEBUG("Priority Queue full (%d)\n",q_cap);
        return false;
    }

    // assign value to end of queue
    q_item item = {
        .pid = pid,
        .priority = priority
    };
    queue[q_size] = item;
    
    swim(q_size);
    q_size++;
}

inline void priorityQueue::swim(uint16_t i)
{
    uint16_t p = parent(i);

    // bubble up until at root
    while (i > 0 && less(i,p))
    {
        swap(i,p);
        i = p;
        p = parent(i);
    }
}

inline void priorityQueue::swap(uint16_t i, uint16_t j)
{
    q_item item = queue[i];
    queue[i] = queue[j];
    queue[j] = item;
}

#ifndef TO_BE_DONE
// push_back instead of push because process order still matters
bool priorityQueue::push_back(uint8_t priority, uint16_t loc)
{
    DEBUG("\nBeginning %s\n", __func__);

    node_t *curr = locator[priority];

    while (curr != NULL)
    {
        curr = curr->next;
    }

    curr->next = (node_t *) malloc(sizeof(node_t));

    if (curr->next == NULL)
    {
        printf("malloc failed\n");
        return false;
    }

    curr->next->loc = loc;
    curr->next->next = NULL;

    DEBUG("Pushing_back: %d\n", loc);
    return true;
}

int priorityQueue::pop(uint8_t priority)
{
	DEBUG("\nBeginning %s\n", __func__);

    if (locator[priority] == NULL)
        return -1;

    int loc = locator[priority]->loc;
    node_t *tmp = locator[priority];

    locator[priority] = locator[priority]->next;

    free(tmp);

    DEBUG("Returning: %d\n", loc);
    return loc;
}

void priorityQueue::free_locator()
{
 	DEBUG("\nBeginning %s\n", __func__);
    
   node_t *tmp, *fr;
    // loop through tmp freeing each val
    for (int i = 0; i < MIN_PRIORITY; i++)
    {
        while ( != NULL)
        {
            fr = *tmp;
            tmp = &(*tmp)->next;
            free(fr);
        }
    }
}
#endif

#endif /* C5C19C5E_17DF_4A66_ADAC_F8E0F569CBB6 */
#endif