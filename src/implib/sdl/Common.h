#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char bool;
#define true 1
#define false 0


long getCurrentTime();
int getRandom(int max);
void StringPadRight(const char *Source, int totalCount,char paddingChar, char *output);
int getWordCount(const char *words);
int getWordByIndex(const char *words,int index,char *word);
void getAllWordByIndex(const char *words,int index,char *word);

#endif // COMMON_H_INCLUDED
