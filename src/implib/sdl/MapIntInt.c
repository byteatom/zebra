#include "MapIntInt.h"

bool MapIntInt_ContainsKey(MapIntInt *map, int key)
{
    pMapIntInt pnext = map;
    if(map->isnull)
    {
        return 0;
    }
    while(1)
    {
        if(pnext->key==key)
        {
            return 1;
        }
        if(pnext->next!=NULL)
        {
            pnext = pnext->next;
        }
        else
        {
            break;
        }
    }
    return 0;
}
int MapIntInt_ValueAt(MapIntInt *map, int key)
{
    pMapIntInt pnext = map;
    if(map->isnull)
    {
        return 0;
    }
    while(1)
    {
        if(pnext->key==key)
        {
            return pnext->value;
        }
        if(pnext->next!=NULL)
        {
            pnext = pnext->next;
        }
        else
        {
            break;
        }
    }
    return 0;
}
void MapIntInt_Add(MapIntInt *pmap, int key, int value)
{
    pMapIntInt pnext = pmap;
    if(pmap->isnull)
    {
        pmap->isnull = false;
        pmap->key = key;
        pmap->value = value;
        pmap->next = NULL;
    }
    else
    {
        while(1)
        {
            if(pnext->next==NULL)
            {
                pnext->next = (pMapIntInt)malloc(sizeof(MapIntInt));
                pnext = pnext->next;
                pnext->key = key;
                pnext->value = value;
                pnext->next = NULL;
                break;
            }
            pnext = pnext->next;
        }
    }
}
void MapIntInt_Free(MapIntInt *map)
{
    pMapIntInt pnext = map,plast;
    if(map->isnull)
    {
        return;
    }
    while(pnext->next!=NULL)
    {
        plast = pnext->next;
        pnext = plast->next;
        free(plast);
        if(pnext==NULL)
        {
            break;
        }
    }
}
