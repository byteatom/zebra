#ifndef MAPINTINT_H_INCLUDED
#define MAPINTINT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "Common.h"

typedef struct _MapIntInt
{
    bool isnull;
    int key;
    int value;
    struct _MapIntInt *next;
}MapIntInt,*pMapIntInt;

bool MapIntInt_ContainsKey(MapIntInt *map, int key);
int MapIntInt_ValueAt(MapIntInt *map, int key);
void MapIntInt_Add(MapIntInt *map, int key, int value);
void MapIntInt_Free(MapIntInt *map);

#endif // MAPINTINT_H_INCLUDED
