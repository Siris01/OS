#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct mem
{
    struct mem *next;
    int start;
    int end;
    char *pid;
} mem;

mem *memory;
int blocks = 0;

void allocate(char *pid, int size)
{

    mem *temp = (mem *)malloc(sizeof(mem) * blocks);
    temp = memory;

    int best_start = temp->start;
    int frag = 99999999;

    while (temp != NULL)
    {
        if (strcmp(temp->pid, "") == 0 && temp->end - temp->start >= size)
        {
            if (temp->end - temp->start - size < frag)
            {
                frag = temp->end - temp->start - size;
                best_start = temp->start;
            }
        }

        temp = temp->next;
    }

    if (frag == 99999999)
    {
        printf("Error: no space available\n");
        return;
    }

    temp = memory;

    while (best_start != temp->start)
        temp = temp->next;

    strcpy(temp->pid, pid);
    int prev_end = temp->end;
    temp->end = temp->start + size;

    if (prev_end == temp->end)
        return;

    mem *new_mem = (mem *)malloc(sizeof(mem) * blocks);
    new_mem->pid = (char *)malloc(strlen(pid) + 1);
    new_mem->start = temp->start + size;
    new_mem->end = prev_end;
    new_mem->next = temp->next;
    strcpy(new_mem->pid, "");

    temp->next = new_mem;
    return;
}

void deallocate(char *pid)
{
    mem *temp = (mem *)malloc(sizeof(mem) * blocks);
    temp = memory;

    while (temp != NULL)
    {
        if (strcmp(temp->pid, pid) == 0)
        {
            strcpy(temp->pid, "");
            return;
        }

        temp = temp->next;
    }

    printf("\nUnable to find process %s\n", pid);
    return;
}

void display()
{
    mem *temp = (mem *)malloc(sizeof(mem) * blocks);
    temp = memory;

    while (temp != NULL)
    {
        printf("%d %d %s\n", temp->start, temp->end, temp->pid);
        temp = temp->next;
    }
}

int main()
{
    blocks = 5;
    int i = 0;
    int start[5] = {100, 200, 250, 400, 500}, end[5] = {200, 250, 400, 500, 700};

    memory = (mem *)malloc(sizeof(mem) * blocks);
    mem *temp = (mem *)malloc(sizeof(mem) * blocks);

    for (i = 0; i < blocks; i++)
    {
        mem *node = (mem *)malloc(sizeof(mem));

        node->start = start[i];
        node->end = end[i];
        node->pid = (char *)malloc(strlen("") + 1);
        node->next = NULL;
        strcpy(node->pid, "");

        if (i == 0)
        {
            temp = node;
            memory = temp;
        }
        else
        {
            temp->next = node;
            temp = temp->next;
        }
    }

    display();
    allocate("A", 51);
    allocate("B", 150);
    allocate("C", 200);
    display();

    return 0;
}