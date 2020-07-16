/**
 * @file global.h
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief 
 * @date 2020-07-16
 */

#include <stdlib.h>
#include <iostream>

#ifndef A0A9ECC1_3A79_4D60_99E2_5C2C22D6C056
#define A0A9ECC1_3A79_4D60_99E2_5C2C22D6C056

// remove any debug print statements
// #define DEBUG_
#ifdef DEBUG_
#define DEBUG(...) printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

// redirect cout guard class to ensure buffer is reset
struct cout_redirect {
    cout_redirect( std::streambuf * new_buffer ) 
        : old( std::cout.rdbuf( new_buffer ) )
    { }

    ~cout_redirect( ) {
        std::cout.rdbuf( old );
    }

private:
    std::streambuf * old;
};



#endif /* A0A9ECC1_3A79_4D60_99E2_5C2C22D6C056 */
