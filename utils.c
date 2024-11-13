#include "utils.h"
#include <string.h>

int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char * indexName) {
    indexName = strtok((char*)fileName, ".");
    strcat(indexName, ".idx");
    return;
}


bool createTable(const char * tableName) {
    FILE *f = NULL;
    char *indexName = "\0";

    f=fopen(tableName, "r");
    if(f == NULL){
        f=fopen(tableName, "w");
        if(f == NULL){
            return false;
        }
        fprintf(f, "-1");
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
    f=fopen(indexName, "w");
    if(f == NULL){
        return false;
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