#include "words.h"

static const Word WORDS_EMPTY_WORD = {
    .foreign = WORDS_EMPTY_STRING,
    .transcription = WORDS_EMPTY_STRING,
    .translate = WORDS_EMPTY_STRING,
    .time = 0,
    .wasErrors = 0
    };

static char dictionaryFileName[WORDS_WORD_LENGTH];

Word newWord(char *foreign, char *transcription, char *translate, time_t time, int wasErrors) {
    Word result;
    strcpy(result.foreign, foreign);
    strcpy(result.transcription, transcription);
    strcpy(result.translate, translate);
    result.time = time;
    result.wasErrors = wasErrors;
    return result;
}

static Word loadFromFile(FILE *file) {
    Word result;
    char foreign[WORDS_WORD_LENGTH];
    char transcription[WORDS_WORD_LENGTH];
    char translate[WORDS_WORD_LENGTH];
    time_t time;
    int wasErrors;
    
    if (fgets(foreign, WORDS_WORD_LENGTH, file)
    && fgets(transcription, WORDS_WORD_LENGTH, file)
    && fgets(translate, WORDS_WORD_LENGTH, file)
    && fscanf(file, "%ld-%d\n", &time, &wasErrors) == 2) {
        result = newWord(foreign, transcription, translate, time, wasErrors);
    } else {
        result = WORDS_EMPTY_WORD;
    }
    return result;
}

static int writeToFile(Word w, FILE *file) {
    return fprintf(file, "%s%s%s%ld-%d\n",
    w.foreign, w.transcription, w.translate, w.time, w.wasErrors);
}

int loadAllFromDictionary(Word *words) {
    int i = 0;

    FILE *file = fopen(dictionaryFileName, "rt");
    
    if (file) {
        while (!feof(file)) {
            Word word = loadFromFile(file);
            if (!wordIsEmpty(word)) {
                words[i] = word;
                i++;
            }
        }
        fclose(file);
    }
    return i;
}

int rewriteAllToDictionary(Word *words, int n) {
    int result = -1;
    FILE *dictionary = fopen(dictionaryFileName, "wt");
    if (dictionary) {
        for (int i = 0; i < n; i++) {
            result = writeToFile(words[i], dictionary);
            if (result < 0) {
                break;
            }
        }
        fclose(dictionary);
    }
    return result;
}

int appendToDictionary(Word word) {
    int result = -1;
    FILE *dictionary = fopen(dictionaryFileName, "at");
    if (dictionary) {
        result = writeToFile(word, dictionary);
        fclose(dictionary);
    }
    return result;
}

void printWord(Word word) {
    printf("%s%s%s%s", word.foreign, word.transcription, word.translate, ctime(&word.time));
    if (word.wasErrors) {
        printf("WAS ERRORS\n");
    } else {
        printf("NO ERRORS\n");
    }
}

int equalsWords(Word w1, Word w2) {
    return (!strcmp(w1.foreign, w2.foreign)
    && !strcmp(w1.transcription, w2.transcription)
    && !strcmp(w1.translate, w2.translate)
    && w1.time == w2.time);
}

int wordIsEmpty(Word word) {
    return equalsWords(word, WORDS_EMPTY_WORD);
}

void setDictionaryFileName(char *name) {
    strcpy(dictionaryFileName, name);
}