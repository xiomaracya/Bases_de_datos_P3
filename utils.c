#include "utils.h"
#include <string.h>
#define BUFFER_LENGTH 100

int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char * indexName) {
    char name[BUFFER_LENGTH];
    strcpy(name, fileName);
    indexName = strtok(name, ".");
    strcat(indexName, ".idx");
    return;
}


bool createTable(const char * tableName) {
    FILE *f = NULL;
    char *indexName = "\0";
    int element = -1;

    f=fopen(tableName, "rb");
    if(f == NULL){
        f=fopen(tableName, "wb");
        if(f == NULL){
            return false;
        }
        if(fwrite(&element, sizeof(int), 1, f)!=1){
            fclose(f);
            return false;
        }
    }

    replaceExtensionByIdx(tableName, indexName);

    if(createIndex(indexName) == false){
        fclose(f);
        return false;
    }

    fclose(f);
    return true;
    }

bool createIndex(const char *indexName) {
    FILE *f = NULL;
    int element1 = -1;
    int element2 = -1;
    f=fopen(indexName, "rb");
    if(f == NULL){
        f=fopen(indexName, "wb");
        if(f == NULL){
            return false;
        }
        if(fwrite(&element1, sizeof(int), 1, f)!=1){
            fclose(f);
            return false;
        }
        if(fwrite(&element2, sizeof(int), 1, f)!=1){
            fclose(f);
            return false;
        }
    }

    fclose(f);
    return true;
}
void printnode(size_t _level, size_t level,
               FILE * indexFileHandler, int node_id, char side) {
    return;
}

void printTree(size_t level, const char * indexName)
{
    return;
}

bool findKey(const char * book_id, const char *indexName,
             int * nodeIDOrDataOffset)
 {
     return true;
 }

bool addIndexEntry(char * book_id,  int bookOffset, char const * indexName) {
    return true;
}

bool addTableEntry(Book * book, const char * dataName,
                   const char * indexName) {
    return true;
}