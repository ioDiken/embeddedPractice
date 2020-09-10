/**
 * @file reader_writer.cpp
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief solution to reader/writer w/ various implementations
 * @date 2020-08-01
 */

#ifndef B8CC91F8_85D0_4C8B_B659_85157B64EC58
#define B8CC91F8_85D0_4C8B_B659_85157B64EC58

#include "reader_writer.h"
#include "unnamed_semaphore.h"

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <queue>

// used to join/kill threads in order of creation (FIFO)
static std::queue<pthread_t> created_threads;

// variables used for various implementations
static unnamed_sem_t rw_mutex, wr_mutex, r_mutex, w_mutex;
static int reader_cnt, writer_cnt, sh_val, reader_num, writer_num;

static void *(*writer_func)(void *);
static void *(*reader_func)(void *);

#define DEBUG_PRINT_RW_STS(wr,arg) DEBUG("%s(%d)=%d\n",(wr)?"W":"R",arg,sh_val)

void kill_remaining();

/************** FIRST READER FUNCTIONS **************/
void *firstReader_write(void *arg)
{
    wait_sem(&rw_mutex);

    sh_val++;
    void *tmp = (void *) (sh_val | RET_RW_MARKER | ((intptr_t)arg << 8));
    DEBUG_PRINT_RW_STS(CREATE_WRITER,arg);
    SLEEP_MS(2);

    signal_sem(&rw_mutex);

    return tmp;
}

void *firstReader_read(void *arg)
{
    /** Check Readers **/
    wait_sem(&r_mutex);
    reader_cnt++;
    if (reader_cnt == 1) // first reader
        wait_sem(&rw_mutex);
    signal_sem(&r_mutex);
    /*******************/

    void *tmp = (void *) (sh_val | ((intptr_t)arg << 8));
    DEBUG_PRINT_RW_STS(CREATE_READER,arg);
    SLEEP_MS(2); // allow other threads to be created/queued

    wait_sem(&r_mutex);
    reader_cnt--;
    if (reader_cnt == 0) // last reader
        signal_sem(&rw_mutex);
    signal_sem(&r_mutex);

    return tmp;
}

bool init_firstReader()
{
    DEBUG_START_FUNC();

    if (init_sem(&rw_mutex,1) < 0)
        return false;
    if (init_sem(&r_mutex,1) < 0)
        return false;

    reader_cnt = 0;
    sh_val = 0;
    reader_num = 0;
    writer_num = 0;

    // set global function pointers
    reader_func = firstReader_read;
    writer_func = firstReader_write;

    return true;
}

bool destroy_firstReader()
{
    DEBUG_START_FUNC();

    kill_remaining();

    if (destroy_sem(&rw_mutex) < 0)
        return false;
    if (destroy_sem(&r_mutex) < 0)
        return false;
    return true;
}
/****************************************************/

/************* SECOND READER FUNCTIONS **************/
void *secondReader_write(void *arg)
{
    DEBUG_START_FUNC();
    
    /** check Writer **/
    wait_sem(&w_mutex);
    writer_cnt++;
    if (writer_cnt != 0) { // if any writers
        signal_sem(&w_mutex);
        wait_sem(&wr_mutex);
    } else {
        signal_sem(&w_mutex);
    }
    /******************/

    /** check for Reader **/
    wait_sem(&r_mutex);
    if (reader_cnt != 0) { // if any readers
        signal_sem(&r_mutex);
        wait_sem(&wr_mutex);
    } else {
        signal_sem(&r_mutex);
    }
    /***********************/

    sh_val++;
    void *tmp = (void *) (sh_val | RET_RW_MARKER | ((intptr_t)arg << 8));
    // DEBUG_PRINT_RW_STS(CREATE_WRITER,arg);
    SLEEP_MS(2);

    signal_sem(&wr_mutex); // signal write can occur again
    /** check/signal for writers **/
    wait_sem(&w_mutex);
    writer_cnt--;
    if (writer_cnt == 0)
        signal_sem(&rw_mutex);
    signal_sem(&w_mutex);

    return tmp;
}

void *secondReader_read(void *arg)
{
    DEBUG_START_FUNC();

    /** check for writers **/
    wait_sem(&w_mutex);
    if (writer_cnt != 0) { // if any writers
        signal_sem(&w_mutex);
        wait_sem(&rw_mutex);
    } else {
        signal_sem(&w_mutex);
    }
    /***********************/

    /** add readers **/
    wait_sem(&r_mutex);
    reader_cnt++;
    signal_sem(&r_mutex);
    /***********************/

    void *tmp = (void *) (sh_val | ((intptr_t)arg << 8));
    // DEBUG_PRINT_RW_STS(CREATE_READER,arg);
    SLEEP_MS(2);

    wait_sem(&r_mutex);
    reader_cnt--;
    if (reader_cnt == 0)   // last reader
        signal_sem(&wr_mutex);
    signal_sem(&r_mutex);
    
    wait_sem(&w_mutex);
    if (writer_cnt == 0)
        signal_sem(&rw_mutex);
    signal_sem(&w_mutex);

    return tmp;
}

bool init_secondReader()
{
    DEBUG_START_FUNC();

    if (init_sem(&rw_mutex,0) < 0)
        return false;
    if (init_sem(&r_mutex,1) < 0)
        return false;
    if (init_sem(&wr_mutex,1) < 0)
        return false;
    if (init_sem(&w_mutex,1) < 0)
        return false;

    reader_cnt = 0;
    sh_val = 0;
    reader_num = 0;
    writer_num = 0;

    // set global function pointers
    reader_func = secondReader_read;
    writer_func = secondReader_write;

    return true;
}

bool destroy_secondReader()
{
    DEBUG_START_FUNC();

    kill_remaining();

    if (destroy_sem(&rw_mutex) < 0)
        return false;
    if (destroy_sem(&r_mutex) < 0)
        return false;
    if (destroy_sem(&wr_mutex) < 0)
        return false;
    if (destroy_sem(&w_mutex) < 0)
        return false;
    return true;
}
/****************************************************/

int create(const bool _writer)
{
    pthread_t useless_pthread; // used for holder for pthread creation
    created_threads.push(useless_pthread);

    if (_writer)
    {
        void *arg = (void *) writer_num++;
        return pthread_create(&created_threads.back(), NULL, writer_func, (void *) arg);
    }
    else
    {
        void *arg = (void *) reader_num++;
        return pthread_create(&created_threads.back(), NULL, reader_func, (void *) arg);
    }
}

int join(void **retval, int thread_timeout_ms)
{
    #ifdef __APPLE__
    int ret = pthread_join(created_threads.front(),retval);
    #else
    struct timespec t;
    t.tv_sec = 0;
    t.tv_nsec = thread_timeout_ms * 1000;
    int ret = pthread_timedjoin_np(created_threads.front(),NULL);
    #endif
    created_threads.pop();
    return ret;
}

int join_remaining(int total_timeout_ms)
{
    DEBUG_START_FUNC();

    int ret;
    total_timeout_ms /= created_threads.size();

    while (!created_threads.empty())
    {
        // if ((ret = join(total_timeout_ms)) != 0)
        //     return ret;
    }
}

/**
 * @brief kill all remaining threads
 * 
 */
void kill_remaining()
{
    DEBUG_START_FUNC();

    // kill the rest of the threads if they havent completed
    while (!created_threads.empty())
    {
        pthread_kill(created_threads.front(), SIGKILL);
        created_threads.pop();
    }
}


#endif /* B8CC91F8_85D0_4C8B_B659_85157B64EC58 */
