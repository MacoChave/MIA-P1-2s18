#ifndef MLIST_H_INCLUDED
#define MLIST_H_INCLUDED

#include <stdlib.h>

typedef struct MList MList;
typedef struct MListNode MListNode;

struct MListNode
{
    void * info;
    MListNode * next;
};

struct MList
{
    int size;
    MListNode * first;
    MListNode * last;
};

MList * newMList ()
{
    MList * list = (MList *)malloc(sizeof(MList));
    list->first = NULL;
    list->last = NULL;
    list->size = 0;

    return list;
}

MListNode * newMListNode()
{
    MListNode * node = (MListNode *)malloc(sizeof(MListNode));
    node->info = NULL;
    node->next = NULL;

    return node;
}

void push_front (MList ** list, void * info)
{
    MListNode * node = newMListNode();
    node->info = info;

    if ((*list)->size == 0)
        (*list)->first = (*list)->last = node;
    else
    {
        node->next = (*list)->first;
        (*list)->first = node;
    }

    (*list)->size++;
}

void push_back (MList ** list, void * info)
{
    MListNode * node = newMListNode();
    node->info = info;

    if ((*list)->size == 0)
        (*list)->first = (*list)->last = node;
    else
    {
        (*list)->last->next = node;
        (*list)->last = node;
    }

    (*list)->size++;
}

void * pop_front (MList ** list)
{
    if ((*list)->size > 0)
    {
        MListNode * temp = (*list)->first;
        (*list)->first = (*list)->first->next;
        (*list)->size--;

        void * result = temp->info;
        free(temp);
        temp = NULL;

        return result;
    }
    return NULL;
}

void * peek_front (MList * list)
{
    if (list->size > 0)
        return list->first->info;

    return NULL;
}

void * peek_back (MList * list)
{
    if (list->size > 0)
        return list->first->info;
    
    return NULL;
}

void clearMList(MList ** list)
{
    printf("Limpiando lista >>\n");
    while ((*list)->size > 0)
    {
        void * temp = pop_front(list);
        free(temp);
        temp = NULL;
    }
    printf("<< Lista limpia\n");
}

#endif // MLIST_H_INCLUDED