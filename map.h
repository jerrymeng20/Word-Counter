/* map.h
 */

#ifndef ___MAP___
#define ___MAP___

#include "data.h"

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
int CountOccurrences( struct  Library * lib, char * word );

#endif

