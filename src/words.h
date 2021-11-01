#ifndef SRC_WORDS_H_
#define SRC_WORDS_H_

#include <stdio.h>
#include <string.h>
#include <time.h>

#define WORDS_WORD_LENGTH 256
#define WORDS_EMPTY_STRING "EMPTY\n"
#define WORDS_EMPTY_CHAR 0

typedef struct word {
    char foreign[WORDS_WORD_LENGTH];
    char transcription[WORDS_WORD_LENGTH];
    char translate[WORDS_WORD_LENGTH];
    time_t time;
    int wasErrors;
} Word;

Word newWord(char *foreign, char *transcription, char *translate, time_t time, int wasErrors);
int loadAllFromDictionary(Word *words);
int rewriteAllToDictionary(Word *words, int n);
int appendToDictionary(Word word);
void printWord(Word word);
int equalsWords(Word w1, Word w2);
int wordIsEmpty(Word word);
void setDictionaryFileName(char *name);

#endif  // SRC_WORDS_H_