#include "utils.h"
#include <stdlib.h>
#include <string.h>
#define BufferLength 100

int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char * indexName) {
    char name[BufferLength] = "\0";
    strcpy(name, fileName);
    indexName = strtok(name, ".");
    strcat(indexName, ".idx");
    return;
}


bool createTable(const char * tableName) {
    FILE *f = NULL;
    int element=-1;
    char *indexName = "\0";

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
    int element=-1;
    f=fopen(indexName, "rb");
    if(f == NULL){
        f=fopen(indexName, "wb");
        if(f == NULL){
            return false;
        }
        if(fwrite(&element, sizeof(int), 1, f)!=1){
            fclose(f);
            return false;
        }
        if(fwrite(&element, sizeof(int), 1, f)!=1){
            fclose(f);
            return false;
        }
    }

    fclose(f);
    return true;
}
void printnode(size_t _level, size_t level, FILE * indexFileHandler, int node_id, char side) {
    int h1= 0, h2=0 , p= 0, datos= 0, i;
    char pk[5] = {0};

    if (_level >= level) {
        printf("Profundidad máxima alcanzada");
        return;
    }

    if(fread(pk, sizeof(char), sizeof(pk)-1, indexFileHandler)!=sizeof(pk)-1){
        printf("ERROR1");
        return;
    }
    if(fread(&h1, sizeof(int), 1, indexFileHandler)!=1){
        printf("ERROR2");
        return;
    }
    if(fread(&h2, sizeof(int), 1, indexFileHandler)!=1){
        printf("ERROR3");
        return;
    }
    if(fread(&p, sizeof(int), 1, indexFileHandler)!=1){
        printf("ERROR4");
        return;
    }
    if(fread(&datos, sizeof(int), 1, indexFileHandler)!=1){
        printf("ERROR5");
        return;
    }

    for (i = 0; i < (int)_level; i++) {
        printf("\t"); 
    }
    printf("%c %s (%d) : %d\n", side, pk, node_id, datos);

    fseek(indexFileHandler, h1, SEEK_SET);
    printnode(_level + 1, level, indexFileHandler, node_id*2+1, 'l');
    fseek(indexFileHandler, h2, SEEK_SET);
    printnode(_level + 1, level, indexFileHandler, node_id*2+2, 'r');
    return;
}

void printTree(size_t level, const char * indexName)
{
    FILE *f = NULL;
    int loc_root = -1;
    f = fopen(indexName, "rb");
    if (f == NULL){
        printf("Error al abrir el fichero");
        return;
    }
    if (fread(&loc_root, sizeof(int), 1, f) != 1 || loc_root < 0){
        fclose(f);
        printf("El fichero esta vacío");
        return;
    }
    fseek(f, loc_root, SEEK_SET);
    printnode(0, level, f, 0,' ');
    if (f){
        fclose(f);
    }
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