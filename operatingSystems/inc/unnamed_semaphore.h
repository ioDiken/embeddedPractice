/**
 * @file unnamed_semaphore.h
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief used to allow universal implementation on OSX & linux
 * @date 2020-08-02
 */

#ifndef D9ACF731_8A20_44CE_8C64_2B090E047130
#define D9ACF731_8A20_44CE_8C64_2B090E047130

#include "global.h"

#include <errno.h>
extern int errno;

#ifdef __APPLE__
#include <dispatch/dispatch.h>

// change type of sem
typedef dispatch_semaphore_t unnamed_sem_t;

inline int init_sem(unnamed_sem_t *sem, int val)
{
    *sem = dispatch_semaphore_create(val);
    if (*sem == NULL)
        return -1;
    return 0;
}

inline int destroy_sem(unnamed_sem_t *sem)
{
    //TODO: OSX equivalent DNE?
    // dispatch_release(*(dispatch_object_t *)sem);
    return 0;
}

/**
 * @brief wait for a signal
 * 
 * @param sem semaphore
 * @return int 0 on success (always successful)
 */
inline int wait_sem(unnamed_sem_t *sem)
{
    dispatch_semaphore_wait(*sem, DISPATCH_TIME_FOREVER);
    return 0;
}

/**
 * @brief signal semaphore
 * 
 * @param sem semaphore
 * @return true if a thread is woken
 */
inline bool signal_sem(unnamed_sem_t *sem)
{
    return (dispatch_semaphore_signal(*sem) != 0);
}

/*********** LINUX IMPLEMENTATION ***********/
#else

#include <semaphore.h>

// change type of sem
typedef sem_t unnamed_sem_t;

/**
 * @brief initialize semaphore
 * 
 * @param sem semaphore
 * @param val initial value
 * @return int 0 on success, else errno set
 */
inline int init_sem(unnamed_sem_t *sem, int val)
{
    return sem_init(sem, 0, val);
}

/**
 * @brief destroy semaphore
 * 
 * @param sem semaphore
 * @return int 0 on success, else errno set
 */
inline int destroy_sem(unnamed_sem_t *sem)
{
    return sem_destroy(sem);
}

/**
 * @brief wait for a signal
 * 
 * @param sem semaphore
 * @return int 0 on success
 */
inline int wait_sem(unnamed_sem_t *sem)
{
    return sem_wait(sem);
}

/**
 * @brief signal semaphore
 * 
 * @param sem semaphore
 * @return true if successful
 * @return false if failed, errno is set
 */
inline bool signal_sem(unnamed_sem_t *sem)
{
    return (sem_post(sem) == 0);
}
#endif




#endif /* D9ACF731_8A20_44CE_8C64_2B090E047130 */
