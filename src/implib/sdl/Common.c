#include <stdio.h>
#include <stdlib.h>
#include "Common.h"


#include <time.h>

long getCurrentTime()
{
    //struct timeval tv;
    //gettimeofday(&tv,NULL);
    //return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    long secs = time(NULL)*1000*getRandom(1000);
    return secs;
}
int getRandom(int max)
{
    srand((int)time(NULL)+rand());
    if(max<=0)
    {
        return (int)rand();
    }
    return (int)(max*rand()/(RAND_MAX+1.0));
}
void StringPadRight(const char *Source, int totalCount,char paddingChar, char *output)
{
    if(paddingChar=='\0')
    {
        return;
    }
    strcpy(output,Source);
    while(strlen(output)<totalCount)
    {
        output[strlen(output)] = paddingChar;
    }

}
//start first is 1
void String_Delete(char *Source,int start,int count)
{
    //12345 1,2

    char *temp = (char *)malloc(strlen(Source)+1);
    memset(temp,0,strlen(Source)+1);
    memcpy(temp,Source,start-1);
    strcpy(&temp[start-1],&Source[start+count-1]);

    strcpy(Source,temp);
    free(temp);
}

int getWordCount(const char *words)
{
    int count = 0;
    int i,len;
    unsigned char b;
    for(i=0,len=0;i<strlen(words);i+=len)
    {
        b = words[i];
        if(b>=0xFC)
        {
            len = 6;
        }
        else if(b>=0xF8)
        {
            len = 5;
        }
        else if(b>=0xF0)
        {
            len = 4;
        }
        else if(b>=0xE0)
        {
            len = 3;
        }
        else if(b>=0xC0)
        {
            len = 2;
        }
        else
        {
            len = 1;
        }
        count++;
    }
    return count;
}
//index: first is 1
int getWordByIndex(const char *words,int index,char *word)
{
    int count = 0;
    int i,len;
    unsigned char b;
    for(i=0,len=0;i<strlen(words);i+=len)
    {
        b = words[i];
        if(b>=0xFC)
        {
            len = 6;
        }
        else if(b>=0xF8)
        {
            len = 5;
        }
        else if(b>=0xF0)
        {
            len = 4;
        }
        else if(b>=0xE0)
        {
            len = 3;
        }
        else if(b>=0xC0)
        {
            len = 2;
        }
        else
        {
            len = 1;
        }
        if(count==(index-1))
        {
            memcpy(word,&words[i],len);
            word[len] = '\0';
            break;
        }
        count++;
    }
    return len;
}
//index: first is 1
void getAllWordByIndex(const char *words,int index,char *word)
{
    int count = 0;
    int i,len;
    unsigned char b;
    for(i=0,len=0;i<strlen(words);i+=len)
    {
        b = words[i];
        if(b>=0xFC)
        {
            len = 6;
        }
        else if(b>=0xF8)
        {
            len = 5;
        }
        else if(b>=0xF0)
        {
            len = 4;
        }
        else if(b>=0xE0)
        {
            len = 3;
        }
        else if(b>=0xC0)
        {
            len = 2;
        }
        else
        {
            len = 1;
        }
        if(count==(index-1))
        {
            strcpy(word,&words[i]);
            break;
        }
        count++;
    }
    //return len;
}
