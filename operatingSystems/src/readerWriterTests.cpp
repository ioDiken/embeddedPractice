/**
 * @file readerWriterTests.cpp
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief tests for all reader/writers
 * @date 2020-07-20
 */
 
#include <gtest/gtest.h> // googletest header file

#include <iostream>

#include "global.h"
#include "reader_writer.h"

int status;

inline void create_reader()
{
    status = create(CREATE_READER);
    EXPECT_TRUE(status == 0) << "Failed to create Reader" << std::endl;
}

inline void create_writer()
{
    status = create(CREATE_WRITER);
    EXPECT_TRUE(status == 0) << "Failed to create Writer" << std::endl;
}

#define print_ret_details(ret) printf("%s(%d)=%d\n", ((intptr_t)ret & RET_RW_MARKER) ? "W" : "R", \
                            RET_ISOLATE_ARG((intptr_t)ret), RET_ISOLATE_VAL((intptr_t)ret))
        ;

TEST(FirstReader, random)
{
    status = init_firstReader();
    ASSERT_TRUE(status) << "Failed to init firstReader: " << strerror(errno) << std::endl;

    int i;
    for (i = 0; i < 4; i++)
        create_writer();
    for (i = 0; i < 6; i++)
        create_reader();

    create_writer();
    
    create_reader();

    create_writer();
    
    create_reader();
    create_reader();

    // allow threads to complete if havent already
    SLEEP_MS(2000);

    // 15
    void *ret;
    int reader_val = -1;
    for (int i = 0; i < 15; i++)
    {
        join(&ret);
        print_ret_details(ret);
    }

    status = destroy_firstReader();
    ASSERT_TRUE(status) << "Failed to destroy firstReader: " << strerror(errno) << std::endl;
}

TEST(SecondReader, random)
{
    status = init_secondReader();
    ASSERT_TRUE(status) << "Failed to init secondReader: " << strerror(errno) << std::endl;
    
    int i;
    for (i = 0; i < 4; i++)
        create_reader();
    for (i = 0; i < 4; i++)
        create_writer();

    // create_writer();
    
    // create_reader();

    // create_writer();
    
    // create_reader();
    // create_reader();

    // allow threads to complete if havent already
    SLEEP_MS(5000);

    void *ret;
    for (int i = 0; i < 8; i++)
    {
        join(&ret);
        print_ret_details(ret);
    }

    status = destroy_secondReader();
    ASSERT_TRUE(status) << "Failed to destroy secondReader: " << strerror(errno) << std::endl;
}