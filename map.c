/* map.c
 */
 
#include "data.h"
#include <stdlib.h>


/* --------------------------------------------------------------------
 * GLOBAL VARIABLES
 * --------------------------------------------------------------------
 * count: A counter, keeping track of the number of times that the word 
 *        appears in the library
 * done: Keep track of the number of threads that have done executing
 * str: Placeholder for the word that we need to check
 * lock_count: Used to lock the global variable 'count' in threads
 * lock_done: Used to lock the global variable 'done' in threads
 * cv: Used to track the conditional variable 'done' in threads
 * --------------------------------------------------------------------
 */
volatile int count = 0;
volatile int done = 0;
char *str = NULL;
pthread_mutex_t lock_count = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_done = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;


/* --------------------------------------------------------------------
 * countArticle
 * --------------------------------------------------------------------
 * Takes an article containing words and a word.
 * Returns the total number of times that the word appears in the this
 * article
 * --------------------------------------------------------------------
 */
void *countArticle(void *art) {
    struct Article *a = (struct Article *) art;
    int countWord = 0;
    unsigned int size = a->numWords;

    // printf("This article has %u words.\n", a->numWords);

    for (int i = 0; i < size; i++) {
        if (!strcmp(a->words[i], str)) {
            // find a word
            countWord++;
        }
    }

    // update global variable
    pthread_mutex_lock(&lock_count);
    pthread_mutex_lock(&lock_done);
    count += countWord;
    done++;
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&lock_done);
    pthread_mutex_unlock(&lock_count);

    pthread_exit(NULL);
}


/* --------------------------------------------------------------------
 * traditionalCount
 * --------------------------------------------------------------------
 * The traditional way to perform the count
 * --------------------------------------------------------------------
 */
int traditionalCount(struct Library *lib, char *word) {
    int counter = 0;

    for (int i = 0; i < lib->numArticles; i++) {
        struct Article *art = lib->articles[i];
        for (int j = 0; j < art->numWords; j++) {
            if (strcmp(art->words[j], word) == 0) {
                counter++;
            }
        }
    }

    return counter;
}


/* --------------------------------------------------------------------
 * CountOccurrences
 * --------------------------------------------------------------------
 * Takes a Library of articles containing words and a word.
 * Returns the total number of times that the word appears in the 
 * Library.
 *
 * For example, "There the thesis sits on the theatre bench.", contains
 * 2 occurences of the word "the".
 * --------------------------------------------------------------------
 */
int CountOccurrences( struct  Library * lib, char * word )
{
    // perform the traditional count and verify the answer
    // printf("The correct answer should be: %d.\n", traditionalCount(lib, word));

    unsigned int size = lib->numArticles;
    pthread_t threads[size];
    str = strdup(word);

    int rc;

    for (int i = 0; i < size; i++) {
        // printf("Now Creating Thread %d\n", i);
        rc = pthread_create(&threads[i], NULL, countArticle, (void *) lib->articles[i]);
        if (rc) {
            printf("ERROR: Return code from pthread_create() is %d.\n", rc);
            exit(-1);
        }
    }

    // wait until all threads are done
    pthread_mutex_lock(&lock_done);
    while (done < size) {
        // printf("Waiting for worker threads to complete... %d to go.\n", size - done);
        pthread_cond_wait(&cv, &lock_done);
    }
    pthread_mutex_unlock(&lock_done);

    // free all resources
    pthread_cond_destroy(&cv);
	pthread_mutex_destroy(&lock_count);
    pthread_mutex_destroy(&lock_done);
    free(str);

    return count;
}

