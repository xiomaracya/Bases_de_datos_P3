#include "utils.h"
#include <stdlib.h>
#include <string.h>
#define BufferLength 100

int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char *indexName)
{
    char name[BufferLength] = "\0";
    strcpy(name, fileName);
    indexName = strtok(name, ".");
    strcat(indexName, ".idx");
    return;
}

bool createTable(const char *tableName)
{
    FILE *f = NULL;
    int element = -1;
    char *indexName = "\0";

    f = fopen(tableName, "rb");
    if (f == NULL)
    {
        f = fopen(tableName, "wb");
        if (f == NULL)
        {
            return false;
        }
        if (fwrite(&element, sizeof(int), 1, f) != 1)
        {
            fclose(f);
            return false;
        }
    }

    replaceExtensionByIdx(tableName, indexName);

    if (createIndex(indexName) == false)
    {
        fclose(f);
        return false;
    }

    fclose(f);
    return true;
}

bool createIndex(const char *indexName)
{
    FILE *f = NULL;
    int element = -1;
    f = fopen(indexName, "rb");
    if (f == NULL)
    {
        f = fopen(indexName, "wb");
        if (f == NULL)
        {
            return false;
        }
        if (fwrite(&element, sizeof(int), 1, f) != 1)
        {
            fclose(f);
            return false;
        }
        if (fwrite(&element, sizeof(int), 1, f) != 1)
        {
            fclose(f);
            return false;
        }
    }

    fclose(f);
    return true;
}
void printnode(size_t _level, size_t level, FILE *indexFileHandler, int node_id, char side)
{
    int h1 = 0, h2 = 0, p = 0, datos = 0, i;
    char pk[5] = {0};

    long offset = 8 + node_id * (4 + 4 * sizeof(int));
    if (fseek(indexFileHandler, offset, SEEK_SET) != 0)
    {
        return;
    }

    printf("NODE_ID: %d ", node_id);

    printf("OFFSET: %ld ", offset);

    if (_level >= level)
    {
        return;
    }

    if (fread(pk, sizeof(char), sizeof(pk) - 1, indexFileHandler) != sizeof(pk) - 1)
    {
        return;
    }
    if (fread(&h1, sizeof(int), 1, indexFileHandler) != 1)
    {
        return;
    }
    if (fread(&h2, sizeof(int), 1, indexFileHandler) != 1)
    {
        return;
    }
    if (fread(&p, sizeof(int), 1, indexFileHandler) != 1)
    {
        return;
    }
    if (fread(&datos, sizeof(int), 1, indexFileHandler) != 1)
    {
        return;
    }

    for (i = 0; i < (int)_level; i++)
    {
        printf("\t");
    }
    printf("%c %s (%d) : %d\n", side, pk, node_id, datos);

    if (h1 != -1)
    {
        printnode(_level + 1, level, indexFileHandler, h1, 'l');
    }
    if (h2 != -1)
    {
        printnode(_level + 1, level, indexFileHandler, h2, 'r');
    }
    return;
}

void printTree(size_t level, const char *indexName)
{
    FILE *f = NULL;
    int loc_root = -1, borrados = -1;
    f = fopen(indexName, "rb");
    if (f == NULL)
    {
        return;
    }
    if (fread(&loc_root, sizeof(int), 1, f) != 1 || loc_root < 0)
    {
        fclose(f);
        return;
    }
    if (fread(&borrados, sizeof(int), 1, f) != 1)
    {
        fclose(f);
        return;
    }
    if (fseek(f, loc_root, SEEK_CUR) != 0)
    {
        return;
    }
    printnode(0, level, f, loc_root, ' ');
    if (f)
    {
        fclose(f);
    }
    return;
}

bool findKey(const char *book_id, const char *indexName,
             int *nodeIDOrDataOffset)
{
    int h1 = 0, h2 = 0, p = 0, datos = 0, loc_root = -1, borrados = -1;
    char pk[5] = {0};
    FILE *f;
    int current_node;
    f = fopen(indexName, "rb");
    if (f == NULL)
    {
        return false;
    }

    if (fread(&loc_root, sizeof(int), 1, f) != 1 || loc_root < 0)
    {
        fclose(f);
        return false;
    }
    if (fread(&borrados, sizeof(int), 1, f) != 1)
    {
        fclose(f);
        return false;
    }
    if (fseek(f, loc_root, SEEK_CUR) != 0)
    {
        return false;
    }
    current_node = loc_root;

    while (current_node != -1)
    {
        long offset = 8 + current_node * (4 + 4 * sizeof(int));
        if (fseek(f, offset, SEEK_SET) != 0)
        {
            return false;
        }

        if (fread(pk, sizeof(char), sizeof(pk) - 1, f) != sizeof(pk) - 1)
        {
            return false;
        }
        if (fread(&h1, sizeof(int), 1, f) != 1)
        {
            return false;
        }
        if (fread(&h2, sizeof(int), 1, f) != 1)
        {
            return false;
        }
        if (fread(&p, sizeof(int), 1, f) != 1)
        {
            return false;
        }
        if (fread(&datos, sizeof(int), 1, f) != 1)
        {
            return false;
        }

        if (strncmp(book_id, pk, 4*sizeof(char)) == 0)
        {
            *nodeIDOrDataOffset = datos;
            fclose(f);
            return true;
        } else if (h1 == -1 && h2 == -1)
        {
            *nodeIDOrDataOffset = current_node;
            fclose(f);
            return false;
        } else if(strcmp(book_id,pk) < 0){
            *nodeIDOrDataOffset = current_node;
            current_node = h1;
        } else{
            *nodeIDOrDataOffset = current_node;
            current_node = h2;
        }
    }
    fclose(f);
    return false;
}
bool addIndexEntry(char *book_id, int bookOffset, char const *indexName)
{
    return true;
}

bool addTableEntry(Book *book, const char *dataName,
                   const char *indexName)
{
    return true;
}