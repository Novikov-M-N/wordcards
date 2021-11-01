#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "words.h"

#define MAX_COUNT_OF_WORDS 8192
#define DEFAULT_GAME_WORD_COUNT 20
#define PARAM_ADD "-add"
#define PARAM_GAME "-start"
#define PARAM_ADD_HELP_MESSAGE "Add word to dictionary:\n\
    -add <foreign> <transcription> <translate>\n\
for example:\n\
    -add \"at least\" \"at lēst\" \"по меньшей мере\"\n"
#define GAME_MODE_OLDEST_WORDS 1
#define GAME_MODE_NEWEST_WORDS 2
#define GAME_DIRECTION_FOREIGN_FIRST 1
#define GAME_DIRECTION_TRANSLATE_FIRST 2
#define WORD_PASSED_CHAR ' '

#define cls() printf("\033[2J\033[H")

void game(int wordsCount);
void showWord(Word word, int direction);
int inArray(int *array, int n, int value);
char getOneChar();
void formationWordSet(Word *words, int count, int *indexes, int *wordsCount);
void passingWordSet(Word *words, int wordsCount, int* indexes, int direction);
void showWithTitle(char *s, char *title);

int main(int argc, char **argv) {
    setDictionaryFileName("/home/mnovikov37/.wrun/dictionary.txt");

    if (argc > 1) {
        if (!strcmp(argv[1], PARAM_ADD)) {
            if (argc > 4) {
                time_t currentTime = time(NULL);
                char foreign[WORDS_WORD_LENGTH] = "";
                char transcription[WORDS_WORD_LENGTH] = "";
                char translate[WORDS_WORD_LENGTH] = "";
                strcat(foreign, argv[2]);
                strcat(foreign, "\n");
                strcat(transcription, argv[3]);
                strcat(transcription, "\n");
                strcat(translate, argv[4]);
                strcat(translate, "\n");
                Word word = newWord(foreign, transcription, translate, currentTime, 1);
                if (appendToDictionary(word) > 0) {
                    printf("success append to dictionary:\n");
                    printWord(word);
                } else {
                    printf("error of append to dictionary:\n");
                    printWord(word);
                    printf("try again\n");
                }
            } else {
                printf(PARAM_ADD_HELP_MESSAGE);
            }
        }
        int wordsCount;
        if (sscanf(argv[1], "%d", &wordsCount) == 1) {
            game(wordsCount);
        }
    } else {
        game(DEFAULT_GAME_WORD_COUNT);
    }
        
    return 0;
}

void game(int wordsCount) {
    Word words[MAX_COUNT_OF_WORDS];
    int indexes[MAX_COUNT_OF_WORDS];
    int count = loadAllFromDictionary(words);
    formationWordSet(words, count, indexes, &wordsCount);
    passingWordSet(words, wordsCount, indexes, GAME_DIRECTION_FOREIGN_FIRST);
    passingWordSet(words, wordsCount, indexes, GAME_DIRECTION_TRANSLATE_FIRST);
    rewriteAllToDictionary(words, count);
}

void formationWordSet(Word *words, int count, int *indexes, int *wordsCount) {
    int j = 0;
    for (int i = 0; i < count && j < *wordsCount; i++) {
        if (words[i].wasErrors) {
            indexes[j] = i;
            j++;
        }
    }
    if (j < *wordsCount) {
        for (;j < *wordsCount; j++) {
            time_t currentTime = __LONG_MAX__;
            int index = -1;
            for (int i = 0; i < count; i++) {
                if (words[i].time < currentTime && !inArray(indexes, j, i)) {
                    index = i;
                    currentTime = words[i].time;
                }
            }
            if (index != -1) {
                indexes[j] = index;
            } else {
                *wordsCount = j;
                break;
            }
        }
    }
    for (int i = 0; i < *wordsCount; i++) {
        words[indexes[i]].wasErrors = 0;
        words[indexes[i]].time = time(NULL);
    }
}

void passingWordSet(Word *words, int wordsCount, int* indexes, int direction) {
    int passedIndexes[MAX_COUNT_OF_WORDS];
    int notPassedIndexes[MAX_COUNT_OF_WORDS];
    int passedIndexesCount = 0;
    int notPassedIndexesCount = 0;
    srand(time(NULL));
    while (passedIndexesCount < wordsCount){
        while (passedIndexesCount + notPassedIndexesCount < wordsCount) {
            int i = rand() % wordsCount;
            int index = indexes[i];
            if (!inArray(passedIndexes, passedIndexesCount, index)
            && !inArray(notPassedIndexes, notPassedIndexesCount, index)) {
                showWord(words[index], direction);
                printf("[space] - word passed right\n");
                char ch = getOneChar();
                if (ch == WORD_PASSED_CHAR) {
                    passedIndexes[passedIndexesCount] = index;
                    passedIndexesCount++;
                } else {
                    words[index].wasErrors = 1;
                    notPassedIndexes[notPassedIndexesCount] = index;
                    notPassedIndexesCount++;
                }
            }
        }
        notPassedIndexesCount = 0;
    }
}

void showWord(Word word, int direction) {
    if (direction == GAME_DIRECTION_FOREIGN_FIRST) {
        char *title = "forward direction";
        showWithTitle(word.foreign, title);
        getOneChar();
        showWithTitle(word.transcription, title);
        getOneChar();
        showWithTitle(word.translate, title);
    } else {
        char *title = "backward direction";
        showWithTitle(word.translate, title);
        getOneChar();
        showWithTitle(word.foreign, title);
        getOneChar();
        showWithTitle(word.transcription, title);
    }
}

void showWithTitle(char *s,  char *title) {
    cls();
    printf("%s\n********************\n", title);
    printf("%s", s);
    printf("********************\n");
}

int inArray(int *array, int n, int value) {
    int result = 0;
    for (int i = 0; i < n; i++) {
        if (array[i] == value) {
            result = 1;
            break;
        }
    }
    return result;
}

char getOneChar() {
    char result = getchar();
    if (result != '\n') {
        while(getchar() != '\n');
    }
    return result;
}