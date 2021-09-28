/* data.h
 */

#ifndef ___DATA___
#define ___DATA___

#include <pthread.h>
#include <stdio.h>
#include <string.h>

struct Article
{
    char ** words;
    unsigned int numWords;
};

struct Library
{
    struct Article ** articles;
    unsigned int numArticles;
};

#endif
