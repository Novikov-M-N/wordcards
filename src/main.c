#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "memory.h"

typedef struct date {
    int day;
    int month;
    int year;
} Date;

typedef struct word {
    char* foreign;
    char* transcription;
    char* translate;
    Date date;
    int wasErrors;
} Word;

Word *readFromFile(FILE *file);

int main(int argc, char **argv) {

    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    Word *w;

    // if (!strcmp(argv[1], "-add")) {
    //     if (argc == 5) {
    //         FILE *file = fopen("1.txt", "at");
    //         if (file) {
    //             time_t currentTime = time(NULL);
    //             struct tm *localTime = localtime(&currentTime);
    //             fprintf(file, "\n%s %s %s %d.%d.%d %d",
    //                 argv[2], argv[3], argv[4],
    //                 localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year - 100, 1);
    //             fclose(file);
    //         }
    //     } else {
    //         printf("incorrect using option \"-add\"\n");
    //     }
    // }

    FILE *file = fopen("1.txt", "r+t");
    if (file) {
        fclose(file);
    }

    FILE *file = fopen("1.txt", "rt");
    if (file) {
        while (!feof(file)) {
            w = readFromFile(file);
            printf("%s %s %s %d.%d.%d %d {%d}\n",
            w->foreign, w->transcription, w->translate,
            w->date.day, w->date.month, w->date.year, w->wasErrors, feof(file));
        }
        fclose(file);
    }

    freeAll();
    return 0;
}

Word *readFromFile(FILE *file) {
    Word *w = (Word*)malloc_(sizeof(Word));
    if (w) {
        char foreign[100], transcription[100], translate[100];
        fscanf(file, "%s%s%s%d.%d.%d%d", foreign, transcription, translate, &w->date.day, &w->date.month, &w->date.year, &w->wasErrors);
        w->foreign = (char*)malloc_((strlen(foreign) + 1) * sizeof(char));
        w->transcription = (char*)malloc_((strlen(transcription) + 1) * sizeof(char));
        w->translate = (char*)malloc_((strlen(translate) + 1) * sizeof(char));
        strcpy(w->foreign, foreign);
        strcpy(w->transcription, transcription);
        strcpy(w->translate, translate);
    }
    return w;
}