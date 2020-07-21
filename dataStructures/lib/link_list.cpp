/**
 * @file link_list.cpp
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief 
 * @date 2020-07-20
 */

#ifndef C968CE69_F0FA_4343_8E83_C782C54504F2
#define C968CE69_F0FA_4343_8E83_C782C54504F2

#include "linked_list.h"

bool LinkedList::push(uint8_t val)
{
	DEBUG("\nBeginning %s\n", __func__);

    node_t *prev = head;

    head = (node_t *) malloc(sizeof(node_t));
    if (head == NULL)
    {
        printf("malloc failed\n");
        return false;
    }

    head->val = val;
    head->next = prev;

    DEBUG("Pushing: %d\n", val);
    return true;
}

int LinkedList::pop()
{
	DEBUG("\nBeginning %s\n", __func__);

    if (head == NULL)
        return -1;

    int val = head->val;
    node_t *tmp = head;

    head = head->next;

    free(tmp);

    DEBUG("Returning: %d\n", val);
    return val;
}

int LinkedList::get_len()
{
	DEBUG("\nBeginning %s\n", __func__);
    
    node_t *tmp = head;
    int i = 0;

    while(tmp != NULL)
    {
        i++;
        tmp = tmp->next;
    }

    return i;
}

void LinkedList::free_lst()
{
 	DEBUG("\nBeginning %s\n", __func__);
    
   node_t *tmp = head,
          *fr;

    while (tmp != NULL)
    {
        fr = tmp;
        tmp = tmp->next;
        free(fr);
    }

    head = NULL;
}

void LinkedList::reverse_lst()
{
	DEBUG("\nBeginning %s\n", __func__);

    node_t *curr = head,
           *prev = NULL,
           *next;

    while (curr != NULL)
    {
        next = curr->next; // save next
        curr->next = prev; // reverse next of current
        prev = curr; // save current
        curr = next; // move current to next
    }
    head = prev;
}

int LinkedList::get_element(int i)
{
	DEBUG("\nBeginning %s\n", __func__);

    node_t *tmp = head;

    // cycle to list index, fail if len < i
    for (int j = 0; tmp != NULL; j++)
    {
        if (j == i)
            return tmp->val;
        
        tmp = tmp->next;
    }

    return -1;
}

int LinkedList::remove_element(int i)
{
	DEBUG("\nBeginning %s\n", __func__);

    node_t **tmp = &head;
    node_t *fr;

    // cycle to list index, fail if len < i
    for (int j = 0; *tmp != NULL; j++)
    {
        if (j == i)
        {
            fr = *tmp;
            *tmp = (*tmp)->next;
            
            int val = fr->val;
            free(fr);

            DEBUG("Element w/ val (%d) removed\n",val);
            return val;
        }
        
        tmp = &(*tmp)->next;
    }

    return -1;
}

bool LinkedList::remove_vals(uint8_t val)
{
	DEBUG("\nBeginning %s\n", __func__);

    node_t **tmp = &head;
    node_t *fr;

    bool ret = false;

    while (*tmp != NULL)
    {
        if ((*tmp)->val == val)
        {
            fr = *tmp;
            *tmp = (*tmp)->next;

            free(fr);
            ret = true;

            DEBUG("Removing Element w/ val: %d\n",val);
        }
        else
        {
            tmp = &(*tmp)->next;
        }
    }

    return ret;
}

void LinkedList::print_list()
{
	DEBUG("\nBeginning %s\n", __func__);

    node_t *tmp = head;

    while(tmp != NULL)
    {
        printf("%d -> ",tmp->val);
        tmp = tmp->next;
    }
    printf("NULL\n");
}

#endif /* C968CE69_F0FA_4343_8E83_C782C54504F2 */
