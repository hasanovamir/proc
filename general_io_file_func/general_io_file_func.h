#ifndef GENERAL_IO
#define GENERAL_IO

//————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

//————————————————————————————————————————————————————————————————————————————————

int get_file_size (const char* file_name);

//————————————————————————————————————————————————————————————————————————————————

#ifndef N_DEBUG
#define DEBUG_ASSERT(cond)\
if (!cond)\
{\
    fprintf(stderr, "%s in %s:%d func:%s\n", #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
    exit(1);\
}
#else
#define DEBUG_ASSERT() 
#endif //N_DEBUG

//————————————————————————————————————————————————————————————————————————————————

#endif //GENERAL_IO