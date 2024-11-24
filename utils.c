#include "utils.h"
#include <string.h>
#include <stdlib.h>
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
    
    int size, zero, offset=0;
    char pk[5], *long_var;
    printf("OK");
    if(fread(pk, sizeof(char), 4, indexFileHandler)!=4){
        printf("ERROR");
        return;
    }
    if(fread(&size, sizeof(int), 1, indexFileHandler)!=1){
        printf("ERROR");
        return;
    }
    if(fread(&zero, sizeof(int), 3, indexFileHandler)!=3){
        printf("ERROR");
        return;
    }
    long_var = (char*)malloc(sizeof(char)*size);
    if(long_var==NULL){
        return;
    }
    if(fread(long_var, sizeof(char), size, indexFileHandler)!=(size_t)size){
        printf("ERROR");
        return;
    }
    free(long_var);

    printf("%c %s (%d) : %d", side, pk, node_id, offset);

    printnode(_level+1, level, indexFileHandler, node_id*2+1, 'l');
    printnode(_level+1, level, indexFileHandler, node_id*2+2, 'r');

    
    return;
}

void printTree(size_t level, const char * indexName)
{
    int offset_borrado;
    FILE *f = NULL;
    f = fopen(indexName, "rb");
    if(f == NULL){
        return;
    }

    fread(&offset_borrado, sizeof(int), 4, f);

    printnode(0, level, f, 0, 'l');

    fclose(f);
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