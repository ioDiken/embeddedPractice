/**
 * @file reader_writer.h
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief solution to reader/writer w/ various implementations
 * @date 2020-08-01
 */
//TODO: Find alternative to pthread_timeedjoin_np for OSX

#ifndef AA6E377C_5AAC_45FB_A10B_32CBB1BC185B
#define AA6E377C_5AAC_45FB_A10B_32CBB1BC185B

#include "global.h"

#include <signal.h>
#include <errno.h>

extern int errno;

#define CREATE_WRITER 1
#define CREATE_READER 0

#define RET_RW_MARKER 0x10000 // if set Writer
#define RET_ISOLATE_ARG(ret) ((ret >> 8) & 0xFF)
#define RET_ISOLATE_VAL(ret) (ret & 0xFF)

/**
 * @brief create writer or reader thread
 * 
 * @param _writer T = writer, F = reader
 * @return int pthread_create ret val (error number)
 */
int create(const bool _writer);

/**
 * @brief wait for top thread to complete
 * 
 * @param ret return value of thread
 * @param int timeout for each pthread_timedjoin_np call, default forever
 * @return int pthread_timedjoin_np ret val (error number)
 */
int join(void **ret, int thread_timeout_ms = 0);

/**
 * @brief wait for all remaining threads to complete. Beware that this can last forever
 * 
 * @param int timeout before failing and returning (in total)
 * @return int pthread_timedjoin_np ret val (error number)
 */
int join_remaining(int total_timeout_ms = 0);

/**
 * @brief initialize all semaphors and counters
 * 
 * @return true if semaphores & vars initialized correctly
 */
bool init_firstReader();

/**
 * @brief destroy semaphores & threads
 * 
 * @return true if semaphores & threads destroyed correctly
 */
bool destroy_firstReader();

/**
 * @brief initialize all semaphors and counters
 * 
 * @return true if semaphores & vars initialized correctly
 */
bool init_secondReader();

/**
 * @brief destroy semaphores & threads
 * 
 * @return true if semaphores & threads destroyed correctly
 */
bool destroy_secondReader();

#endif /* AA6E377C_5AAC_45FB_A10B_32CBB1BC185B */
